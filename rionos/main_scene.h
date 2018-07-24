#pragma once

#include <iostream>

#include "..\ecs\entity.h"

class MainScene : public ECS::Entity
{
private:
	void OnCreate(void) override;
	void OnUpdate(void) override;
	void OnDestroyed(void) override
	{
		//std::cout << __FUNCTION__ << std::endl;
	}
};