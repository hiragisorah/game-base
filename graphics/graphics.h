#pragma once

#ifdef GRAPHICS_DLL
#define GRAPHICS_DLL_API __declspec(dllexport)
#else
#define GRAPHICS_DLL_API __declspec(dllimport)
#endif

#include "..\window\window.h"
#include "..\ecs\utility.h"
#include "renderer.h"

class GRAPHICS_DLL_API Graphics
{
public:
	Graphics(Window * const window);
	~Graphics(void);

public:
	bool Initalize(void);
	bool Finalize(void);
	
	// ****
public:
	void Present(const SYNC_INTERVAL & sync_interval);

	// �����_�����O�^�[�Q�b�g
public:
	void SetRenderTarget(const RENDER_TARGET & render_target);
	void ClearRenderTarget(const RENDER_TARGET & render_target);

	// �r���[�|�[�g
public:
	void SetViewPort(const VIEWPORT_TYPE & view_port);

	// �V�F�[�_
public:
	void LoadShader(const SHADER_TYPE & shader_type, const std::string & file_name);
	void SetupShader(const SHADER_TYPE & shader_type);
	void UpdateConstantBuffer(const SHADER_TYPE & shader_type, void * constant_buffer, const unsigned int & buffer_num);
	void UnloadShader(const SHADER_TYPE & shader_type);
	
	// �W�I���g��
public:
	void LoadGeometry(const GEOMETRY_TYPE & geometry_type, const std::string & file_name);
	void Draw(const GEOMETRY_TYPE & geometry_type);
	void UnloadGeometry(const GEOMETRY_TYPE & geometry_type);

	// �B������
private:
	class Impl;
	Impl * impl_ = nullptr;
};