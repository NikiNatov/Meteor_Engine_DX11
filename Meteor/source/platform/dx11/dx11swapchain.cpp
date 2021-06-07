#include "pch.h"
#include "dx11swapchain.h"

#include "meteor\core\application.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

	// --------------------------------------------------------------------------------------------------------------------------------------
	DX11SwapChain::DX11SwapChain(DX11Device& device, void* windowHandle, const SwapChainDescription& description)
		: m_Device(device), m_SwapChainDesc(description)
	{
		// Get the factory that the device was created with
		IDXGIDevice* dxgiDevice = nullptr;
		IDXGIAdapter* dxgiAdapter = nullptr;
		IDXGIFactory* dxgiFactory = nullptr;

		DX_CALL((*m_Device)->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
		DX_CALL(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));
		DX_CALL(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

		// Create the swap chain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = m_SwapChainDesc.BufferCount;
		swapChainDesc.BufferDesc.Width = m_SwapChainDesc.Width;
		swapChainDesc.BufferDesc.Height = m_SwapChainDesc.Height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 144;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapChainDesc.OutputWindow = static_cast<HWND>(windowHandle);
		swapChainDesc.Windowed = !m_SwapChainDesc.Fullscreen;

		DX_CALL(dxgiFactory->CreateSwapChain((*device).Get(), &swapChainDesc, &m_SwapChain));

		// Release resources
		dxgiDevice->Release();
		dxgiAdapter->Release();
		dxgiFactory->Release();
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	DX11SwapChain::~DX11SwapChain()
	{
		// Make sure we toogle off fullscreen when destroying the window so we don't crash
		if (m_SwapChainDesc.Fullscreen)
			ToggleFullscreen();
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11SwapChain::ClearBackBuffer()
	{
		const mtrFloat color[4] = { 0.4f, 0.7f, 1.0f, 1.0f };
		m_Device.GetD3DImmediateContext()->ClearRenderTargetView(m_BackBufferRTV.Get(), color);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11SwapChain::ClearDepthBuffer()
	{
		m_Device.GetD3DImmediateContext()->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11SwapChain::Present()
	{
		m_Device.GetD3DImmediateContext()->OMSetRenderTargets(1, m_BackBufferRTV.GetAddressOf(), m_DepthStencilView.Get());
		DX_CALL(m_SwapChain->Present(m_SwapChainDesc.VSync, 0));
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11SwapChain::ToogleVSync()
	{
		m_SwapChainDesc.VSync = !m_SwapChainDesc.VSync;
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11SwapChain::ToggleFullscreen()
	{
		m_SwapChainDesc.Fullscreen = !m_SwapChainDesc.Fullscreen; 

		if (m_SwapChainDesc.Fullscreen)
		{
			DXGI_MODE_DESC desc = {};
			desc.Width = GetSystemMetrics(SM_CXSCREEN);
			desc.Height = GetSystemMetrics(SM_CYSCREEN);

			DX_CALL(m_SwapChain->ResizeTarget(&desc));
			DX_CALL(m_SwapChain->SetFullscreenState(m_SwapChainDesc.Fullscreen, nullptr));
			DX_CALL(m_SwapChain->ResizeTarget(&desc));
		}
		else
		{
			DXGI_MODE_DESC desc = {};
			desc.Width = 1280;
			desc.Height = 720;

			DX_CALL(m_SwapChain->SetFullscreenState(m_SwapChainDesc.Fullscreen, nullptr));
			DX_CALL(m_SwapChain->ResizeTarget(&desc));
		}
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11SwapChain::Resize(mtrUInt width, mtrUInt height)
	{
		ID3D11DeviceContext* devContext = m_Device.GetD3DImmediateContext().Get();

		// Release resources used by the swap chain
		if (m_DepthStencilView)
			m_DepthStencilView->Release();
		if (m_BackBufferRTV)
			m_BackBufferRTV->Release();

		// Resize the buffers
		m_SwapChainDesc.Width = width;
		m_SwapChainDesc.Height = height;
		DX_CALL(m_SwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		// Recreate render targets
		wrl::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
		DX_CALL(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		DX_CALL((*m_Device)->CreateRenderTargetView(backBuffer.Get(), nullptr, m_BackBufferRTV.GetAddressOf()));

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

		wrl::ComPtr<ID3D11Texture2D> depthBuffer = nullptr;
		DX_CALL((*m_Device)->CreateTexture2D(&depthStencilDesc, nullptr, &depthBuffer));
		DX_CALL((*m_Device)->CreateDepthStencilView(depthBuffer.Get(), nullptr, m_DepthStencilView.GetAddressOf()));

		devContext->OMSetRenderTargets(1, m_BackBufferRTV.GetAddressOf(), m_DepthStencilView.Get());

		// Recreate viewport
		D3D11_VIEWPORT viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<mtrFloat>(width);
		viewport.Height = static_cast<mtrFloat>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		devContext->RSSetViewports(1, &viewport);
	}

#endif // METEOR_PLATFORM_WINDOWS
	
}