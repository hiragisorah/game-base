#pragma once

#include "component.h"
#include "utility.h"

#include <vector>
#include <unordered_map>
#include <typeinfo>

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
		void set_parent(Utility::OriPtr<Entity> & parent);
		void set_self(Utility::OriPtr<Entity> & self);
		const Utility::RefPtr<Entity> & get_parent(void) const;

		// éq
	private:
		std::vector<Utility::OriPtr<Entity>> children_;
		std::unordered_map<std::string, Utility::OriPtr<Component>> components_;
		
	public:
		template<class _Entity, class ... Args> _Entity * const create_child(const Args &... args)
		{
			this->children_.emplace_back();

			auto & child = this->children_.back();

			child.make<_Entity>(args ...);

			child->parent_ = this->self_;

			child->OnCreate();

			return static_cast<_Entity*>(*child);
		}
		template<class _Component, class ... Args> void add_component(const Args &... args)
		{
			auto name = typeid(_Component).name();

			auto & component = this->components_[name];

			component.make<_Component>(args ...);

			component->owner_ = this->self_;

			component->self_ = component;

			component->OnAdd();
		}
		template<class _Component> _Component * const get_component(void)
		{
			auto name = typeid(_Component).name();

			auto & component = this->components_[name];

			return static_cast<_Component*>(*component);
		}
		void destroy(void);

	public:
		void ComponentsUpdate(void);
		void ChildrenUpdate(void);
		void ComponentsRender(void);
		void ChildrenRender(void);

	public:
		virtual void OnCreate(void);
		virtual void OnUpdate(void);
		virtual void OnRender(void);
		virtual void OnDestroy(void);
	};
}