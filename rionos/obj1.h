#pragma once

#include <iostream>

#include "..\ecs\entity.h"

class Obj1 : public ECS::Entity
{
public:
	void OnUpdate(void) override
	{
		std::cout << "ios" << std::endl;
	}
};