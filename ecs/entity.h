#pragma once

#include "component.h"

#include <vector>

namespace ECS
{
	class Entity
	{
	public:
		Entity(void);
		virtual ~Entity(void);

		// êe
	private:
		Utility::RefPtr<Entity> self_;
		Utility::RefPtr<Entity> parent_;

	public:
		void set_parent(Utility::OriginalPtr<Entity> & parent);
		void set_parent(Utility::RefPtr<Entity> & parent);
		const Utility::RefPtr<Entity> & get_parent(void) const;

		// éq
	private:
		std::vector<Utility::OriginalPtr<Entity>> children_;
		
	public:
		void add_child(Utility::OriginalPtr<Entity> & child);
		void add_child(Utility::RefPtr<Entity> & child);

	private:
		void UpdateComponents(void);
		void UpdateChildren(void);

	public:
		virtual void Update(void);

	};
}