#include "second_scene.h"
#include "main_scene.h"

#include "..\seed-engine\seed-engine.h"

void SecondScene::OnUpdate(void)
{
	static int n = 0;
	if (++n > 200)
	{
		SeedEngine::ChangeScene<MainScene>();
		n = 0;
	}
	if(n % 10 == 0)
		std::cout << __FUNCTION__ << std::endl;
}
