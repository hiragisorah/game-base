#pragma once

namespace ECS
{
	class Component
	{
	public:
		Component(void);
		virtual ~Component(void);

	public:
		virtual void Initialize(void) {}
		virtual void Update(void) {}
		virtual void Finalize(void) {}
	};
}