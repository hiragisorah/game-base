#pragma once

#include <iostream>

#include "..\ecs\entity.h"
#include "obj1.h"

class MainScene : public ECS::Entity
{
private:
	void OnCreate(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
		this->create_child<Obj1>();
	}
	void OnUpdate(void) override;
	void OnDestroyed(void) override
	{
		std::cout << __FUNCTION__ << std::endl;
	}
};