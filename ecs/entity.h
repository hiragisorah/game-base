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
		void set_parent(Utility::RefPtr<Entity> & parent);
		const Utility::RefPtr<Entity> & get_parent(void) const;

		// éq
	private:
		std::vector<Utility::OriPtr<Entity>*> children_;
		
	public:
		template<class _Entity, class ... Args> Utility::OriPtr<Entity> & create_child(const Args &... args)
		{
			this->children_.emplace_back(new Utility::OriPtr<Entity>);

			this->children_.back()->make<_Entity>(args ...);

			return *this->children_.back();
		}

	private:
		void UpdateComponents(void);
		void UpdateChildren(void);

	public:
		virtual void Update(void);

	};
}