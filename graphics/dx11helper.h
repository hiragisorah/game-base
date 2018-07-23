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
		Microsoft::WRL::ComPtr<ID3D11InputLayout>& input_layout_;
		std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>>& constant_buffers_;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>& vertex_shader_;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>& geometry_shader_;
		Microsoft::WRL::ComPtr<ID3D11HullShader>& hull_shader_;
		Microsoft::WRL::ComPtr<ID3D11DomainShader>& domain_shader_;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>& pixel_shader_;
	};
	// シェーダの生成
	// シェーダリフレクションによってコンスタントバッファー、インプットレイアウトの自動生成
	static void LoadShader(
		const Microsoft::WRL::ComPtr<ID3D11Device> & device,
		const std::string & file_name,
		Dx11Shader shader
	);
}