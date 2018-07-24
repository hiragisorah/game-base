#pragma once

#include <iostream>

#include "..\ecs\entity.h"

class MainScene : public ECS::Entity
{
private:
	void OnCreate(void) override;
	void OnUpdate(void) override;
	void OnRender(void) override;
	void OnDestroy(void) override;
};