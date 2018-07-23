#include "graphics.h"

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include <unordered_map>

#include "dx11helper.h"

using namespace Microsoft::WRL;

class Graphics::Impl final
{
public:
	Impl(Window * const window)
		: window_(window)
		, clear_color_(.2f, .4f, .8f, 1.f)
	{}

private:
	Window * const window_;

public:
	const bool Initialize(void)
	{
		if (!this->InitializeSwapChain() || !this->InitializeRenderTargets() || !this->InitializeDepthStencilViews() || !this->InitializeViewPorts())
			return false;

		return true;
	}
	const bool Finalize(void)
	{
		return true;
	}

	// ****
public:
	void Present(const SYNC_INTERVAL & sync_interval)
	{
		this->swap_chain_->Present(static_cast<unsigned int>(sync_interval), 0);
	}

	// レンダリングターゲット
public:
	void SetRenderTarget(const RENDER_TARGET & render_target)
	{
		this->context_->OMSetRenderTargets(1, this->rtv_[RENDER_TARGET::BACK_BUFFER].GetAddressOf(), this->dsv_[RENDER_TARGET::BACK_BUFFER].Get());
	}
	void ClearRenderTarget(const RENDER_TARGET & render_target)
	{
		this->context_->ClearRenderTargetView(rtv_[RENDER_TARGET::BACK_BUFFER].Get(), (float*)&clear_color_);
		this->context_->ClearDepthStencilView(dsv_[RENDER_TARGET::BACK_BUFFER].Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	}

	// シェーダ
public:
	void LoadShader(const SHADER_TYPE & shader_type, const std::string &  file_name)
	{
		Dx11Helper::LoadShader(this->device_, file_name, this->shader_[shader_type]);
	}
	void SetupShader(const SHADER_TYPE & shader_type)
	{
		auto & shader = this->shader_[shader_type];

		this->context_->VSSetShader(shader.vertex_shader_.Get(), nullptr, 0);
		this->context_->GSSetShader(shader.geometry_shader_.Get(), nullptr, 0);
		this->context_->HSSetShader(shader.hull_shader_.Get(), nullptr, 0);
		this->context_->DSSetShader(shader.domain_shader_.Get(), nullptr, 0);
		this->context_->PSSetShader(shader.pixel_shader_.Get(), nullptr, 0);

		this->context_->IASetInputLayout(shader.input_layout_.Get());
		this->context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto buffer_cnt = shader.constant_buffers_.size();

		this->context_->VSSetConstantBuffers(0, buffer_cnt, shader.constant_buffers_[0].GetAddressOf());
		this->context_->GSSetConstantBuffers(0, buffer_cnt, shader.constant_buffers_[0].GetAddressOf());
		this->context_->HSSetConstantBuffers(0, buffer_cnt, shader.constant_buffers_[0].GetAddressOf());
		this->context_->DSSetConstantBuffers(0, buffer_cnt, shader.constant_buffers_[0].GetAddressOf());
		this->context_->PSSetConstantBuffers(0, buffer_cnt, shader.constant_buffers_[0].GetAddressOf());
	}
	void UnloadShader(const SHADER_TYPE & shader_type)
	{
		this->shader_.erase(shader_type);
	}

	// ジオメトリ
public:
	void Draw(const GEOMETRY_TYPE & geometry_type)
	{
		this->context_->IASetVertexBuffers(0, 1, this->vertex_buffer_[geometry_type].GetAddressOf(), &this->strides_[geometry_type], 0);
		this->context_->IASetPrimitiveTopology(this->topology_[geometry_type]);

		this->context_->Draw(this->vertex_cnt_[geometry_type], 0);
	}
	void DrawIndexed(const GEOMETRY_TYPE & geometry_type)
	{
		this->context_->IASetVertexBuffers(0, 1, this->vertex_buffer_[geometry_type].GetAddressOf(), &this->strides_[geometry_type], 0);
		this->context_->IASetIndexBuffer(this->index_buffer_[geometry_type].Get(), DXGI_FORMAT_R32_UINT, 0);
		this->context_->IASetPrimitiveTopology(this->topology_[geometry_type]);

		this->context_->DrawIndexed(this->index_cnt_[geometry_type], 0, 0);
	}

private:
	const bool InitializeSwapChain(void)
	{
		// デバイスとスワップチェーンの作成 
		DXGI_SWAP_CHAIN_DESC sd;
		memset(&sd, 0, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = window_->get_width<unsigned int>();
		sd.BufferDesc.Height = window_->get_height<unsigned int>();
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = window_->get_hwnd<HWND>();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = true;

		D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL * feature_level = nullptr;

		auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			0, &feature_levels, 1, D3D11_SDK_VERSION, &sd, &this->swap_chain_, &this->device_,
			feature_level, &this->context_);

		return SUCCEEDED(hr);
	}
	const bool InitializeRenderTargets(void)
	{
		return this->InitializeBackBuffer();
	}
	const bool InitializeDepthStencilViews(void)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

		//深度マップテクスチャをレンダーターゲットにする際のデプスステンシルビュー用のテクスチャーを作成
		D3D11_TEXTURE2D_DESC tex_desc = {};
		tex_desc.Width = window_->get_width<unsigned int>();
		tex_desc.Height = window_->get_height<unsigned int>();
		tex_desc.MipLevels = 1;
		tex_desc.ArraySize = 1;
		tex_desc.Format = DXGI_FORMAT_D32_FLOAT;
		tex_desc.SampleDesc.Count = 1;
		tex_desc.SampleDesc.Quality = 0;
		tex_desc.Usage = D3D11_USAGE_DEFAULT;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		tex_desc.CPUAccessFlags = 0;
		tex_desc.MiscFlags = 0;

		this->device_->CreateTexture2D(&tex_desc, nullptr, tex_2d.GetAddressOf());
		auto hr = device_->CreateDepthStencilView(tex_2d.Get(), nullptr, this->dsv_[RENDER_TARGET::BACK_BUFFER].GetAddressOf());

		return SUCCEEDED(hr);
	}
	const bool InitializeViewPorts(void)
	{
		auto & vp = this->viewport_[VIEWPORT_TYPE::DEFAULT];

		vp.Width = window_->get_width<float>();
		vp.Height = window_->get_height<float>();
		vp.MinDepth = 0.f;
		vp.MaxDepth = 1.f;
		vp.TopLeftX = 0.f;
		vp.TopLeftY = 0.f;

		return true;
	}

private:
	const bool InitializeBackBuffer(void)
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

		// バックバッファーテクスチャーを取得
		this->swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tex_2d);

		// そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
		auto hr = this->device_->CreateRenderTargetView(tex_2d.Get(), nullptr, this->rtv_[RENDER_TARGET::BACK_BUFFER].GetAddressOf());

		return SUCCEEDED(hr);
	}

private:
	ComPtr<IDXGISwapChain> swap_chain_;
	ComPtr<ID3D11Device> device_;
	ComPtr<ID3D11DeviceContext> context_;

	DirectX::XMFLOAT4 clear_color_;

	// データベース
private:
	std::unordered_map<RENDER_TARGET, ComPtr<ID3D11RenderTargetView>> rtv_;

	std::unordered_map<RENDER_TARGET, ComPtr<ID3D11DepthStencilView>> dsv_;

	std::unordered_map<VIEWPORT_TYPE, D3D11_VIEWPORT> viewport_;
	
	std::unordered_map<SHADER_TYPE, Dx11Helper::Dx11Shader> shader_;

	std::unordered_map<GEOMETRY_TYPE, ComPtr<ID3D11Buffer>> index_buffer_;

	std::unordered_map<GEOMETRY_TYPE, ComPtr<ID3D11Buffer>> vertex_buffer_;
	std::unordered_map<GEOMETRY_TYPE, unsigned int> strides_;
	std::unordered_map<GEOMETRY_TYPE, unsigned int> vertex_cnt_;
	std::unordered_map<GEOMETRY_TYPE, unsigned int> index_cnt_;

	std::unordered_map<GEOMETRY_TYPE, D3D11_PRIMITIVE_TOPOLOGY> topology_;
};

Graphics::Graphics(Window * const window)
	: impl_(new Graphics::Impl(window))
{
}

Graphics::~Graphics(void)
{
	delete this->impl_;
}

bool Graphics::Initalize(void)
{
	return this->impl_->Initialize();
}

bool Graphics::Finalize(void)
{
	return this->impl_->Finalize();
}

void Graphics::ClearRenderTarget(const RENDER_TARGET & render_target)
{
	this->impl_->ClearRenderTarget(render_target);
}

void Graphics::Present(const SYNC_INTERVAL & sync_interval)
{
	this->impl_->Present(sync_interval);
}

void Graphics::SetRenderTarget(const RENDER_TARGET & render_target)
{
	this->impl_->SetRenderTarget(render_target);
}

void Graphics::ClearRenderTarget(const RENDER_TARGET & render_target)
{
	this->impl_->ClearRenderTarget(render_target);
}

void Graphics::LoadShader(const SHADER_TYPE & shader_type, const std::string & file_name)
{
	this->impl_->LoadShader(shader_type, file_name);
}

void Graphics::SetupShader(const SHADER_TYPE & shader_type)
{
	this->impl_->SetupShader(shader_type);
}

void Graphics::UpdateConstantBuffer(void * constant_buffer, const unsigned int & buffer_num)
{
}

void Graphics::UnloadShader(const SHADER_TYPE & shader_type)
{
	this->impl_->UnloadShader(shader_type);
}

void Graphics::Draw(const GEOMETRY_TYPE & geometry_type)
{
	this->impl_->Draw(geometry_type);
}

void Graphics::DrawIndexed(const GEOMETRY_TYPE & geometry_type)
{
	this->impl_->DrawIndexed(geometry_type);
}
