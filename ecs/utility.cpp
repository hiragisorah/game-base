#include "utility.h"

ECS::Utility::IOriPtr::~IOriPtr(void)
{
	for (auto & ref : this->ref_list_)
		ref->original_ = nullptr;
}

void ECS::Utility::IOriPtr::CleanRefs(void)
{
	for (auto & ref : this->ref_list_)
		ref->original_ = nullptr;

	this->ref_list_.clear();
}