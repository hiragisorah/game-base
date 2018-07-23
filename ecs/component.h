#pragma once

#include "utility.h"

namespace ECS
{
	class Entity;

	class Component
	{
		friend Entity;

	public:
		Component(void);
		virtual ~Component(void);

	private:
		Utility::RefPtr<Component> self_;
		Utility::RefPtr<Entity> owner_;

	public:
		virtual void OnAdd(void);
		virtual void OnUpdate(void);
		virtual void OnRemove(void);

	public:
		void remove(void);

	public:
		const Utility::RefPtr<Entity> & get_owner(void) const;
	};
}