#include "entity.h"

namespace ECS
{
	Entity::Entity(void)
	{
	}
	Entity::~Entity(void)
	{
	}
	void Entity::set_parent(Utility::RefPtr<Entity>& parent)
	{
		this->parent_ = parent;
		this->parent_->children_.emplace_back(*this->self_);
	}
	void Entity::set_parent(Utility::OriPtr<Entity>& parent)
	{
		this->parent_ = parent;
		this->parent_->children_.emplace_back(*this->self_);
	}
	void Entity::set_self(Utility::OriPtr<Entity>& self)
	{
		this->self_ = self;
	}
	const Utility::RefPtr<Entity> & Entity::get_parent(void) const
	{
		return this->parent_;
	}
	void Entity::destroy(void)
	{
		this->OnDestroy();
		(*this->self_)->remove();
	}
	void Entity::ComponentsUpdate(void)
	{
		for (auto itr = this->components_.begin(); itr != this->components_.end();)
		{
			auto & component_map = (*itr);
			if (*component_map.second)
			{
				component_map.second->OnUpdate();
				++itr;
			}
			else
			{
				component_map.second->OnRemove();
				itr = this->components_.erase(itr);
			}
		}
	}
	void Entity::ChildrenUpdate(void)
	{
		for (auto n = 0U; n < this->children_.size();)
		{
			auto & child = this->children_[n];
			if (*child)
			{
				child->OnUpdate();
				child->ComponentsUpdate();
				child->ChildrenUpdate();
				++n;
			}
			else
			{
				auto next_itr = this->children_.erase(this->children_.begin() + n);
				n = std::distance(this->children_.begin(), next_itr);
			}
		}
	}
	void Entity::ComponentsRender(void)
	{
		for (auto itr = this->components_.begin(); itr != this->components_.end();)
		{
			auto & component_map = (*itr);
			if (*component_map.second)
			{
				component_map.second->OnRender();
				++itr;
			}
			else
			{
				component_map.second->OnRemove();
				itr = this->components_.erase(itr);
			}
		}
	}
	void Entity::ChildrenRender(void)
	{
		for (auto n = 0U; n < this->children_.size();)
		{
			auto & child = this->children_[n];
			if (*child)
			{
				child->OnRender();
				child->ComponentsRender();
				child->ChildrenRender();
				++n;
			}
			else
			{
				auto next_itr = this->children_.erase(this->children_.begin() + n);
				n = std::distance(this->children_.begin(), next_itr);
			}
		}
	}
	void Entity::OnCreate(void)
	{
	}
	void Entity::OnUpdate(void)
	{
	}
	void Entity::OnRender(void)
	{
	}
	void Entity::OnDestroy(void)
	{
	}
}