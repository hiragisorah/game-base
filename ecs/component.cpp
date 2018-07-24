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
	void Component::OnAdd(void)
	{
	}
	void Component::OnUpdate(void)
	{
	}
	void Component::OnRender(void)
	{
	}
	void Component::OnRemove(void)
	{
	}
	void Component::remove(void)
	{
		this->OnRemove();
		(*this->self_)->remove();
	}
	const Utility::RefPtr<Entity>& Component::get_owner(void) const
	{
		return this->owner_;
	}
}