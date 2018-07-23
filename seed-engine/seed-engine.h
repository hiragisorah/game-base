#pragma once

#ifdef SEED_ENGINE_DLL
#define SEED_ENGINE_DLL_API __declspec(dllexport)
#else
#define SEED_ENGINE_DLL_API __declspec(dllimport)
#endif

#include "..\ecs\entity.h"
#include "..\ecs\utility.h"
#include "..\graphics\graphics.h"
#include "..\window\window.h"

class SEED_ENGINE_DLL_API SeedEngine
{
private:
	SeedEngine(void);

private:
	static const bool Initialize(void);

public:
	static void Ready(void);
	template<class _Entity, class ... Args> static const bool Initialize(const Args &... args)
	{
		Ready();
		current_scene_->make<_Entity>(args ...);
		return Initialize();
	}
	template<class _Entity, class ... Args> static void ChangeScene(const Args &... args)
	{
		next_scene_->make<_Entity>(args ...);
	}
	static const bool Run(void);
	static const bool Finalize(void);

private:
	static Graphics * graphics_;
	static Window * window_;
	static ECS::Utility::OriPtr<ECS::Entity> * current_scene_;
	static ECS::Utility::OriPtr<ECS::Entity> * next_scene_;

public:
	static Graphics * const graphics(void);
	static Window * const window(void);

public:
	static const bool Process(void);
};

