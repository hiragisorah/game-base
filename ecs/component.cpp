#include "component.h"
#include "entity.h"

namespace ECS
{
	Component::Component(void)
		: owner_(nullptr)
	{

	}
	Component::~Component(void)
	{
	}
	void Component::set_owner(Utility::OriginalPtr<Entity>& owner)
	{
		this->owner_ = owner;
	}
	void Component::set_owner(Utility::RefPtr<Entity>& owner)
	{
		this->owner_ = owner;
	}
	const Utility::RefPtr<Entity>& Component::get_owner(void) const
	{
		return this->owner_;
	}
}