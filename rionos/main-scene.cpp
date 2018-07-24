#include "main-scene.h"
#include "second-scene.h"

#include "obj1.h"
#include "..\seed-engine\seed-engine.h"

#include <DirectXMath.h>

struct CBUFFER
{
	DirectX::XMMATRIX world_;
	DirectX::XMFLOAT2A view_port_;
};

void MainScene::OnCreate(void)
{
	SeedEngine::graphics()->LoadShader(SHADER_TYPE::_DEFAULT, "../resource/shader/default.hlsl");
	SeedEngine::graphics()->LoadGeometry(GEOMETRY_TYPE::_POINT, "");

	this->create_child<Obj1>();
}

void MainScene::OnUpdate(void)
{

}

void MainScene::OnRender(void)
{
	CBUFFER cbuffer;
	cbuffer.world_ = DirectX::XMMatrixIdentity();
	cbuffer.view_port_.x = 1280.f;
	cbuffer.view_port_.y = 720.f;

	SeedEngine::graphics()->SetViewPort(VIEWPORT_TYPE::DEFAULT);
	SeedEngine::graphics()->SetRenderTarget(RENDER_TARGET::BACK_BUFFER);
	SeedEngine::graphics()->ClearRenderTarget(RENDER_TARGET::BACK_BUFFER);

	SeedEngine::graphics()->SetupShader(SHADER_TYPE::_DEFAULT);
	SeedEngine::graphics()->UpdateConstantBuffer(SHADER_TYPE::_DEFAULT, &cbuffer, 0);
	SeedEngine::graphics()->Draw(GEOMETRY_TYPE::_POINT);

	SeedEngine::graphics()->Present(SYNC_INTERVAL::LIMITED);
}

void MainScene::OnDestroy(void)
{

}