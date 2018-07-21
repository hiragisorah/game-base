#pragma once

#include "utility.h"

namespace ECS
{
	class Entity;
	class Component
	{
	public:
		Component(void);
		virtual ~Component(void);

	private:
		Utility::RefPtr<Entity> owner_;

	public:
		virtual void Initialize(void) {}
		virtual void Update(void) {}
		virtual void Finalize(void) {}

	public:
		void set_owner(Utility::RefPtr<Entity> & owner);
		const Utility::RefPtr<Entity> & get_owner(void) const;
	};
}