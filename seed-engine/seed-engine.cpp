#include "seed-engine.h"

namespace
{
	ECS::Scene * current_scene_ = nullptr;
}

Graphics * SeedEngine::graphics_ = nullptr;
Window * SeedEngine::window_ = nullptr;

SeedEngine::SeedEngine()
{
}

const bool SeedEngine::Initialize(ECS::Scene * start_scene)
{
	window_ = new Window;
	graphics_ = new Graphics(window_);
	current_scene_ = start_scene;
	return window_->Initalize("Rionos", 1280U, 720U) && graphics_->Initalize();
}

const bool SeedEngine::Run()
{
	while (window_->Run() && graphics_->Run());

	return true;
}


const bool SeedEngine::Finalize()
{
	delete current_scene_;
	
	auto ret = graphics_->Finalize() && window_->Finalize();

	delete graphics_;
	delete window_;

	return ret;
}

Graphics * const SeedEngine::graphics(void)
{
	return graphics_;
}

Window * const SeedEngine::window(void)
{
	return window_;
}
