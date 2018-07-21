#include "..\seed-engine\seed-engine.h"
#include "main_scene.h"

#define WIN32_LEAD_AND_MEAN
#include <Windows.h>

#include <memory>

#include <vector>
#include <iostream>

#include "..\ecs\entity.h"

class Test : public ECS::Entity
{
public:
	int n_;
};

#pragma warning(disable: 4189)
#ifdef _DEBUG
int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif

	Test a;
	ECS::Utility::RefPtr<Test> b, c;
	b = a.create_child<Test>();
	
	c = b;

	c->n_ = 50;

	std::cout << b->n_ << std::endl;
	
	b->n_ = 10;

	std::cout << c->n_ << std::endl;

	!SeedEngine::Initialize<MainScene>() || !SeedEngine::Run() || SeedEngine::Finalize();
}