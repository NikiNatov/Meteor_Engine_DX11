#include "pch.h"
#include "windowswindow.h"

#include "meteor\events\eventinclude.h"
#include "meteor\core\logger.h"

#include "meteor\renderer\api\gfxcontext.h"
#include "resource.h"

#include "imgui.h"
#include "examples\imgui_impl_win32.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)
	EXTERN_C IMAGE_DOS_HEADER __ImageBase;
	HINSTANCE hInstance;

	// --------------------------------------------------------------------------------------------------------------------------------------
	ScopedPtr<Window> Window::Create(WindowProperties& properties)
	{
		return CreateScoped<WindowsWindow>(properties);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	LRESULT WINAPI WindowsWindow::WindowProcSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		if (Msg == WM_NCCREATE)
		{
			// If the function is called for the first time set the window data pointer and change the callback function to our custom function
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowsWindow* const window = static_cast<WindowsWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::WindowProc));
			return WindowProc(hWnd, Msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	LRESULT WINAPI WindowsWindow::WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		WindowsWindow* const window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
			return true;

		switch (Msg)
		{
			case WM_DESTROY:
			case WM_CLOSE:
			{
				WindowClosedEvent e;
				window->m_Properties.EventCallback(e);
				PostQuitMessage(0);
				return 0;
			}
			case WM_KILLFOCUS:
			{
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				KeyPressedEvent e(static_cast<Key>(wParam), 0);
				window->m_Properties.EventCallback(e);
				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				KeyReleasedEvent e(static_cast<Key>(wParam));
				window->m_Properties.EventCallback(e);
				break;
			}
			case WM_CHAR:
			{
				KeyTypedEvent e(static_cast<Key>(wParam));
				window->m_Properties.EventCallback(e);
				break;
			}
			case WM_MOUSEMOVE:
			{
				const POINTS pt = MAKEPOINTS(lParam);

				MouseMovedEvent e(pt.x, pt.y);
				window->m_Properties.EventCallback(e);
				break;
			}
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent e(static_cast<MouseButton>(wParam));
				window->m_Properties.EventCallback(e);
				break;
			}
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent e(static_cast<MouseButton>(wParam));
				window->m_Properties.EventCallback(e);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				const POINTS pt = MAKEPOINTS(lParam);
				const int delta = GET_WHEEL_DELTA_WPARAM(wParam);

				MouseScrolledEvent e(0, delta);
				window->m_Properties.EventCallback(e);
				break;
			}
			case WM_SIZE:
			{
				if (wParam == SIZE_MINIMIZED)
					window->SetMinimized(true);
				else if (wParam == SIZE_RESTORED)
					window->SetMinimized(false);

				window->m_Properties.Width = LOWORD(lParam);
				window->m_Properties.Height = HIWORD(lParam);
				
				// Only resize the swap chain if the window is not minimized
				if (wParam != SIZE_MINIMIZED)
					GfxContext::GetSwapChain()->Resize(window->m_Properties.Width, window->m_Properties.Height);

				WindowResizedEvent e(window->m_Properties.Width, window->m_Properties.Height);
				window->m_Properties.EventCallback(e);
				break;
			}
		}

		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	WindowsWindow::WindowsWindow(WindowProperties& properties)
		: m_Properties(properties)
	{
		Initialize(m_Properties);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	WindowsWindow::~WindowsWindow()
	{
		UnregisterClass("Meteor Engine Window", hInstance);
		DestroyWindow(m_Handle);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void WindowsWindow::Initialize(const WindowProperties& properties)
	{
		hInstance = (HINSTANCE)&__ImageBase;

		// Initialize window
		m_WndClass = {};
		m_WndClass.cbSize = sizeof(m_WndClass);
		m_WndClass.style = CS_OWNDC;
		m_WndClass.lpfnWndProc = WindowProcSetup;
		m_WndClass.cbClsExtra = 0;
		m_WndClass.cbWndExtra = 0;
		m_WndClass.hInstance = hInstance;
		m_WndClass.hCursor = nullptr;
		m_WndClass.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
		m_WndClass.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
		m_WndClass.hbrBackground = nullptr;
		m_WndClass.lpszMenuName = nullptr;
		m_WndClass.lpszClassName = "Meteor Engine Window";
		RegisterClassEx(&m_WndClass);

		m_WndRect.left = 100;
		m_WndRect.right = m_Properties.Width + m_WndRect.left;
		m_WndRect.top = 100;
		m_WndRect.bottom = m_Properties.Height + m_WndRect.top;

		METEOR_ENGINE_ASSERT(AdjustWindowRect(&m_WndRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_SIZEBOX, FALSE), "Failed to adjust window rect!");

		m_Handle = CreateWindowEx(0, "Meteor Engine Window", m_Properties.Title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, m_WndRect.right - m_WndRect.left, m_WndRect.bottom - m_WndRect.top, nullptr, nullptr, hInstance, this);

		METEOR_ENGINE_ASSERT(m_Handle, "Failed to create the window!");

		// Initialize graphics context
		SwapChainDescription swapChainDesc;
		swapChainDesc.BufferCount = 3;
		swapChainDesc.Width = m_Properties.Width;
		swapChainDesc.Height = m_Properties.Height;
		swapChainDesc.VSync = m_Properties.VSync;
		swapChainDesc.Fullscreen = m_IsFullscreen;

		GfxContext::CreateContext(swapChainDesc, m_Handle);

		ShowWindow(m_Handle, SW_MAXIMIZE);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void WindowsWindow::OnUpdate()
	{
		// Process window messages
		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Swap the buffers
		GfxContext::GetSwapChain()->Present();
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void WindowsWindow::ToogleVSync()
	{
		m_Properties.VSync = !m_Properties.VSync;
		GfxContext::GetSwapChain()->ToogleVSync();
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void WindowsWindow::ToogleFullscreen()
	{
		m_IsFullscreen = !m_IsFullscreen;
		GfxContext::GetSwapChain()->ToggleFullscreen();
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void WindowsWindow::SetMinimized(mtrBool state)
	{
		m_IsMinimized = state;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Properties.EventCallback = callback;
	}

#endif // METEOR_PLATFORM_WINDOWS
}
