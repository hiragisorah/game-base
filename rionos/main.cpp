#include "..\seed-engine\seed-engine.h"
#include "main-scene.h"

#define WIN32_LEAD_AND_MEAN
#include <Windows.h>

#include <memory>

#include <vector>
#include <iostream>

#include "..\ecs\entity.h"

#pragma warning(disable: 4189)
#ifdef _DEBUG
int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif

	return !SeedEngine::Initialize<MainScene>() || !SeedEngine::Run() || !SeedEngine::Finalize();
}