#pragma once

#include <iostream>
#include "..\ecs\entity.h"

class SecondScene : public ECS::Entity
{
private:
	void OnCreate(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
	void OnUpdate(void) override;
	void OnDestroyed(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};