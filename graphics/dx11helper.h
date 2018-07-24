#pragma once

#include <d3dcompiler.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl\client.h>
#include <Windows.h>

#include <string>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

namespace Dx11Helper
{
	struct Dx11Shader
	{
		Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout_;
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> constant_buffers_;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader_;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> geometry_shader_;
		Microsoft::WRL::ComPtr<ID3D11HullShader> hull_shader_;
		Microsoft::WRL::ComPtr<ID3D11DomainShader> domain_shader_;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader_;
	};
	// シェーダの生成
	// シェーダリフレクションによってコンスタントバッファー、インプットレイアウトの自動生成
	void LoadShader(
		const Microsoft::WRL::ComPtr<ID3D11Device> & device,
		const std::string & file_name,
		Dx11Shader & shader
	);

	template<class _VertexType>
	void CreateVertexBuffer(
		const Microsoft::WRL::ComPtr<ID3D11Device> & device,
		const std::vector<_VertexType> & vertices,
		Microsoft::WRL::ComPtr<ID3D11Buffer> & vertex_buffer,
		unsigned int & stride,
		unsigned int & vertex_cnt
	)
	{
		stride = sizeof(_VertexType);
		vertex_cnt = vertices.size();

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = vertex_cnt * stride;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();

		device->CreateBuffer(&bd, &sd, vertex_buffer.GetAddressOf());
	}

	template<class _IndexType>
	void CreateIndexBuffer(
		const Microsoft::WRL::ComPtr<ID3D11Device> & device,
		const std::vector<_IndexType> & indices,
		Microsoft::WRL::ComPtr<ID3D11Buffer> & index_buffer,
		unsigned int & index_cnt
	)
	{
		index_cnt = indices.size();

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = index_cnt * sizeof(_IndexType);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = indices.data();

		device->CreateBuffer(&bd, &sd, index_buffer.GetAddressOf());
	}
}