#include "seed-engine.h"

#include "..\graphics\graphics.h"
#include "..\window\window.h"

namespace
{
	ECS::Scene * current_scene_ = nullptr;
}

SeedEngine::SeedEngine()
{
}

const bool SeedEngine::Initialize(ECS::Scene * start_scene)
{
	current_scene_ = start_scene;
	return Window::Initalize("Rionos", 1280U, 720U) && Graphics::Initalize();
}

const bool SeedEngine::Run()
{
	while (Window::Run() && Graphics::Run());

	return true;
}


const bool SeedEngine::Finalize()
{
	delete current_scene_;
	return Graphics::Finalize() && Window::Finalize();
}
