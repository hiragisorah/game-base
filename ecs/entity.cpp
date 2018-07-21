#include "entity.h"

namespace ECS
{
	Entity::Entity(void)
	{
	}
	Entity::~Entity(void)
	{
		for (auto & child : this->children_)
			delete child;
	}
	void Entity::set_parent(Utility::RefPtr<Entity>& parent)
	{
		this->parent_ = parent;
	}
	const Utility::RefPtr<Entity> & Entity::get_parent(void) const
	{
		return this->parent_;
	}
	void Entity::UpdateComponents(void)
	{

	}
	void Entity::UpdateChildren(void)
	{

	}
	void Entity::Update(void)
	{
	}
}