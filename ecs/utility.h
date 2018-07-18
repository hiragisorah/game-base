#pragma once

#include <vector>
#include <algorithm>

namespace ECS
{
	namespace Utility
	{
		template<class _Type> class RefPtr;

		template<class _Type> class OriginalPtr final
		{
			friend RefPtr<_Type>;

		public:
			template<class ... Args> OriginalPtr(const Args &... args)
				: ptr_(args ...)
			{}
			~OriginalPtr(void)
			{
				for (auto & ref : this->ref_list_)
					ref->original_ = nullptr;
			}
			
		private:
			std::vector<RefPtr<_Type>*> ref_list_;

		private:
			_Type ptr_;

		public:
			_Type * const operator->(void)
			{
				return &this->ptr_;
			}
		};

		template<class _Type> class RefPtr final
		{
			friend OriginalPtr<_Type>;

		public:
			RefPtr(void)
				: original_(nullptr)
			{

			}
			RefPtr(OriginalPtr<_Type> * const original)
				: original_(original)
			{
				this->original_->ref_list_.emplace_back(this);
			}
			RefPtr(OriginalPtr<_Type> & original)
				: original_(&original)
			{
				this->original_->ref_list_.emplace_back(this);
			}
			~RefPtr(void)
			{
				this->Release();
			}

		private:
			void Release(void)
			{
				if (this->original_)
				{
					auto & vct = this->original_->ref_list_;
					vct.erase(std::remove_if(vct.begin(), vct.end(), [&](RefPtr<_Type> * a) { return a == this; }), vct.end());
				}
			}

		private:
			OriginalPtr<_Type> * original_;

		public:
			const RefPtr<_Type> & operator=(OriginalPtr<_Type> & original)
			{
				this->Release();

				this->original_ = &original;
				this->original_->ref_list_.emplace_back(this);

				return *this;
			}
			const RefPtr<_Type> & operator=(RefPtr<_Type> & ref)
			{
				if (this->original_ != ref.original_)
				{
					this->Release();

					this->original_ = ref.original_;
					this->original_->ref_list_.emplace_back(this);
				}

				return *this;
			}
			_Type * const safe(void)
			{
				if (this->original_)
					return &this->original_->ptr_;
				else
					return nullptr;
			}
		};
	}
}