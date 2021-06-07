#pragma once

#include "meteor\core\window.h"

#if defined(METEOR_PLATFORM_WINDOWS)
#include "win.h"
#endif // METEOR_PLATFORM_WINDOWS

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(WindowProperties& properties);
		virtual ~WindowsWindow();

		virtual void			  Initialize(const WindowProperties& properties) override;

		virtual void			  OnUpdate() override;

		virtual const mtrString&  GetTitle() const override { return m_Properties.Title; }
		virtual mtrUInt			  GetWidth() const override { return m_Properties.Width; }
		virtual mtrUInt			  GetHeight() const override { return m_Properties.Height; }
		virtual mtrBool			  IsVSync() const override { return m_Properties.VSync; }
		virtual mtrBool			  IsFullscreen() const override { return m_IsFullscreen; }
		virtual mtrBool			  IsMinimized() const override { return m_IsMinimized; }
		virtual void*			  GetWindowHandle() const override { return m_Handle; }

		virtual void			  ToogleVSync() override;
		virtual void			  ToogleFullscreen() override;
		virtual void			  SetMinimized(mtrBool state) override;
		virtual void			  SetEventCallback(const EventCallbackFn& callback) override;
	private:
		static LRESULT WINAPI	  WindowProcSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI	  WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	private:
		HWND					  m_Handle;
		RECT					  m_WndRect;
		WNDCLASSEX				  m_WndClass;
								  
		mtrBool					  m_IsMinimized = false;
		mtrBool					  m_IsFullscreen = false;
								  
		WindowProperties		  m_Properties;			
	};

#endif // METEOR_PLATFORM_WINDOWS
}