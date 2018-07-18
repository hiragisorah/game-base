#include "entity.h"

namespace ECS
{
	Entity::Entity(void)
	{
	}
	Entity::~Entity(void)
	{
	}
	void Entity::set_parent(Utility::OriginalPtr<Entity>& parent)
	{
		this->parent_ = parent;
	}
	void Entity::set_parent(Utility::RefPtr<Entity>& parent)
	{
		this->parent_ = parent;
	}
	const Utility::RefPtr<Entity> & Entity::get_parent(void) const
	{
		return this->parent_;
	}
	void Entity::add_child(Utility::OriginalPtr<Entity>& child)
	{
		child->set_parent(this->self_);
		this->children_.emplace_back(child);
	}
	void Entity::add_child(Utility::RefPtr<Entity>& child)
	{
		child.safe()->set_parent(this->self_);
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