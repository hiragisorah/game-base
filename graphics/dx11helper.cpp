#include "dx11helper.h"

#include <iostream>

#if defined(DEBUG) || defined(_DEBUG)
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

static void CreateInputLayoutAndConstantBufferFromShader(
	const Microsoft::WRL::ComPtr<ID3D11Device> & device,
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> & constant_buffers,
	Microsoft::WRL::ComPtr<ID3D11InputLayout>& input_layout,
	ID3DBlob * blob
);

static DXGI_FORMAT GetDxgiFormat(
	D3D_REGISTER_COMPONENT_TYPE type,
	BYTE mask
);

void Dx11Helper::LoadShader(const Microsoft::WRL::ComPtr<ID3D11Device>& device, const std::string & file_name, Dx11Shader & shader)
{
	ID3DBlob * blob = nullptr;
	ID3DBlob * error = nullptr;

	if (FAILED(D3DCompileFromFile(std::wstring(file_name.begin(), file_name.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		if (error != nullptr)
			std::cout << __FUNCTION__ << "::" << (char*)error->GetBufferPointer() << std::endl;
		else
			std::cout << __FUNCTION__ << "::シェーダーの読み込みに失敗しました。" << std::endl;

		return;
	}
	else
	{
		device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, shader.vertex_shader_.GetAddressOf());
		CreateInputLayoutAndConstantBufferFromShader(device, shader.constant_buffers_, shader.input_layout_, blob);
	}

	if (SUCCEEDED(D3DCompileFromFile(std::wstring(file_name.begin(), file_name.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, shader.geometry_shader_.GetAddressOf());
	}

	if (SUCCEEDED(D3DCompileFromFile(std::wstring(file_name.begin(), file_name.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "HS", "hs_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		device->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, shader.hull_shader_.GetAddressOf());
	}

	if (SUCCEEDED(D3DCompileFromFile(std::wstring(file_name.begin(), file_name.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "DS", "ds_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		device->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, shader.domain_shader_.GetAddressOf());
	}

	if (SUCCEEDED(D3DCompileFromFile(std::wstring(file_name.begin(), file_name.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, shader.pixel_shader_.GetAddressOf());
	}
}

void CreateInputLayoutAndConstantBufferFromShader(
	const Microsoft::WRL::ComPtr<ID3D11Device> & device,
	std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> & constant_buffers,
	Microsoft::WRL::ComPtr<ID3D11InputLayout>& input_layout,
	ID3DBlob * blob)
{
	ID3D11ShaderReflection * reflector = nullptr;
	D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflector);

	D3D11_SHADER_DESC shader_desc;
	reflector->GetDesc(&shader_desc);

	constant_buffers.resize(shader_desc.ConstantBuffers);

	for (auto n = 0U; n < shader_desc.ConstantBuffers; ++n)
	{
		int size = 0;
		auto cb = reflector->GetConstantBufferByIndex(n);
		D3D11_SHADER_BUFFER_DESC desc;
		cb->GetDesc(&desc);

		for (size_t j = 0; j < desc.Variables; ++j)
		{
			auto v = cb->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC vdesc;
			v->GetDesc(&vdesc);
			if (vdesc.Size % 16)
				size += vdesc.Size + 16 - (vdesc.Size % 16);
			else
				size += vdesc.Size;
		}

		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = size;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(device->CreateBuffer(&bd, nullptr, constant_buffers[n].GetAddressOf())))
			std::cout << "コンスタントバッファーの作成に失敗しました。" << std::endl;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> element;
	for (unsigned int i = 0; i < shader_desc.InputParameters; ++i) {
		D3D11_SIGNATURE_PARAMETER_DESC sigdesc;
		reflector->GetInputParameterDesc(i, &sigdesc);

		auto format = GetDxgiFormat(sigdesc.ComponentType, sigdesc.Mask);

		D3D11_INPUT_ELEMENT_DESC eledesc =
		{
			sigdesc.SemanticName
			, sigdesc.SemanticIndex
			, format
			, 0
			, D3D11_APPEND_ALIGNED_ELEMENT
			, D3D11_INPUT_PER_VERTEX_DATA
			, 0
		};

		element.emplace_back(eledesc);
	}

	if (!element.empty())
		if (FAILED(device->CreateInputLayout(&element[0], element.size(),
			blob->GetBufferPointer(), blob->GetBufferSize(), input_layout.GetAddressOf())))
			std::cout << "インプットレイアウトの作成に失敗しました。" << std::endl;
}

DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
	if (mask == 0x0F)
	{
		// xyzw
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		}
	}

	if (mask == 0x07)
	{
		// xyz
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32B32_UINT;
		}
	}

	if (mask == 0x3)
	{
		// xy
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32_UINT;
		}
	}

	if (mask == 0x1)
	{
		// x
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32_UINT;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}
