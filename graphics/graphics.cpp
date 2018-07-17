#include "graphics.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include "..\window\window.h"

#pragma comment(lib, "d3d11.lib")

namespace
{
	using namespace Microsoft::WRL;

	ComPtr<IDXGISwapChain> swap_chain_;
	ComPtr<ID3D11Device> device_;
	ComPtr<ID3D11DeviceContext> context_;

	ComPtr<ID3D11RenderTargetView> back_buffer_rtv_;

	ComPtr<ID3D11DepthStencilView> dsv_;

	D3D11_VIEWPORT viewport_;

	DirectX::XMVECTOR clear_color_ = DirectX::XMVectorSet(.2f, .4f, .8f, 1.f);


	const bool InitializeSwapChain(void)
	{
		// デバイスとスワップチェーンの作成 
		DXGI_SWAP_CHAIN_DESC sd;
		memset(&sd, 0, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = Window::get_width<unsigned int>();
		sd.BufferDesc.Height = Window::get_height<unsigned int>();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = Window::get_hwnd<HWND>();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;

		D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL * feature_level = nullptr;

		auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			0, &feature_levels, 1, D3D11_SDK_VERSION, &sd, &swap_chain_, &device_,
			feature_level, &context_);

		return SUCCEEDED(hr);
	}
	const bool InitializeBackBuffer(void)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

		// バックバッファーテクスチャーを取得
		swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tex_2d);

		// そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
		auto hr = device_->CreateRenderTargetView(tex_2d.Get(), nullptr, back_buffer_rtv_.GetAddressOf());

		return SUCCEEDED(hr);
	}
	const bool InitializeDepthStencilView(void)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

		//深度マップテクスチャをレンダーターゲットにする際のデプスステンシルビュー用のテクスチャーを作成
		D3D11_TEXTURE2D_DESC tex_desc = {};
		tex_desc.Width = Window::get_width<unsigned int>();
		tex_desc.Height = Window::get_height<unsigned int>();
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.Format = DXGI_FORMAT_D32_FLOAT;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;

		device_->CreateTexture2D(&tex_desc, nullptr, tex_2d.GetAddressOf());
		auto hr = device_->CreateDepthStencilView(tex_2d.Get(), nullptr, dsv_.GetAddressOf());

		return SUCCEEDED(hr);
	}
	const bool InitializeViewPort(void)
	{
		auto & vp = viewport_;

		vp.Width = Window::get_width<float>();
		vp.Height =  Window::get_height<float>();
		vp.MinDepth = 0.f;
		vp.MaxDepth = 1.f;
		vp.TopLeftX = 0.f;
		vp.TopLeftY = 0.f;

		return true;
	}
}

bool Graphics::Initalize(void)
{
	if (!InitializeSwapChain() || !InitializeBackBuffer() || !InitializeDepthStencilView() || !InitializeViewPort())
		return false;

	return true;
}

bool Graphics::Run(void)
{
	context_->ClearRenderTargetView(back_buffer_rtv_.Get(), (float*)&clear_color_);
	context_->ClearDepthStencilView(dsv_.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

	swap_chain_->Present(1, 0);

	return true;
}

bool Graphics::Finalize(void)
{
	return true;
}
