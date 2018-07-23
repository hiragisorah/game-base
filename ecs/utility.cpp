#include "utility.h"

ECS::Utility::IOriPtr::IOriPtr(IOriPtr * const original)
{
	this->ptr_ = original->ptr_;

	for (auto ref : original->ref_list_)
		ref->original_ = this;

	original->ref_list_.swap(this->ref_list_);

	original->ptr_ = nullptr;

	original->CleanRefs();
}

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