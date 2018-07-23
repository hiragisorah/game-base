#include "main_scene.h"
#include "second_scene.h"

#include "..\seed-engine\seed-engine.h"

void MainScene::OnUpdate(void)
{
	static int n = 0;
	if (++n > 200)
	{
		SeedEngine::ChangeScene<SecondScene>();
		n = 0;
	}
	if (n % 10 == 0)
		std::cout << __FUNCTION__ << std::endl;
}
