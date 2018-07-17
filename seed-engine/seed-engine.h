#pragma once

#ifdef SEED_ENGINE_DLL
#define SEED_ENGINE_DLL_API __declspec(dllexport)
#else
#define SEED_ENGINE_DLL_API __declspec(dllimport)
#endif

#include "..\ecs\scene.h"

class SEED_ENGINE_DLL_API SeedEngine
{
private:
	SeedEngine(void);

private:
	static const bool Initialize(ECS::Scene * start_scene);

public:
	template<class _Scene> static const bool Initialize(void) { return SeedEngine::Initialize(new _Scene); }
	static const bool Run(void);
	static const bool Finalize(void);
};

