#include "component.h"
#include "entity.h"

namespace ECS
{
	Component::Component(void)
	{

	}
	Component::~Component(void)
	{
	}
	void Component::set_owner(Utility::RefPtr<Entity>& owner)
	{
		this->owner_ = owner;
		//this->owner_
	}
	const Utility::RefPtr<Entity>& Component::get_owner(void) const
	{
		return this->owner_;
	}
}