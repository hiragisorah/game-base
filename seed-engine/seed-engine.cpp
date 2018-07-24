#include "seed-engine.h"

Graphics * SeedEngine::graphics_ = nullptr;
Window * SeedEngine::window_ = nullptr;
ECS::Utility::OriPtr<ECS::Entity> * SeedEngine::current_scene_;
ECS::Utility::OriPtr<ECS::Entity> * SeedEngine::next_scene_;

SeedEngine::SeedEngine()
{
}

const bool SeedEngine::Initialize(void)
{
	window_ = new Window;
	graphics_ = new Graphics(window_);
	return window_->Initalize("Rionos", 1280U, 720U) && graphics_->Initalize();
}
void SeedEngine::Ready(void)
{
	current_scene_ = new ECS::Utility::OriPtr<ECS::Entity>;
	next_scene_ = new ECS::Utility::OriPtr<ECS::Entity>;
}
const bool SeedEngine::Run()
{
	(*current_scene_)->set_self(*current_scene_);
	(*current_scene_)->OnCreate();

	while (Process() && window_->Run());

	return true;
}

const bool SeedEngine::Finalize()
{
	delete current_scene_;
	delete next_scene_;

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

const bool SeedEngine::Process(void)
{
	auto & current = (*current_scene_);
	auto & next = (*next_scene_);

	if (*current)
	{
		current->OnUpdate();
		current->ComponentsUpdate();
		current->ChildrenUpdate();

		current->OnRender();
		current->ComponentsRender();
		current->ChildrenRender();
	}

	if (*next)
	{
		if (*current)
		{
			current->destroy();
		}
		else
		{
			std::swap(current, next);
			current->set_self(current);
			current->OnCreate();
		}
	}
	else
	{
		if (!*current)
			return false;
	}

	return true;
}
