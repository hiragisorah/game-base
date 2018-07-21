#pragma once

#include <vector>
#include <algorithm>

namespace ECS
{
	namespace Utility
	{
		class IRefPtr;

		class IOriPtr
		{
			friend IRefPtr;

		protected:
			IOriPtr(void)
				: ptr_(nullptr)
			{

			}
			virtual ~IOriPtr(void);

		protected:
			void * ptr_;

		protected:
			std::vector<IRefPtr*> ref_list_;

		protected:
			void CleanRefs(void);
		};

		class IRefPtr
		{
			friend IOriPtr;

		protected:
			IRefPtr(void)
				: original_(nullptr)
			{

			}
			IRefPtr(IOriPtr * const original)
				: original_(original)
			{
				this->original_->ref_list_.emplace_back(this);
			}
			virtual ~IRefPtr(void)
			{
				if (this->original_)
				{
					auto & data = this->original_->ref_list_;
					std::remove_if(data.begin(), data.end(), [&](IRefPtr * ref)
					{
						return ref == this;
					}
					);
				}
			}
			void set_original(IOriPtr * const original)
			{
				this->original_ = original;
				this->original_->ref_list_.emplace_back(this);
			}

		protected:
			IOriPtr * original_;
		};

		template<class _Type> class OriPtr final : public IOriPtr
		{
		public:
			~OriPtr(void)
			{
				delete static_cast<_Type*>(this->ptr_);
			}

		public:
			_Type * const operator->(void)
			{
				return static_cast<_Type*>(this->ptr_);
			}
			_Type * const operator*(void)
			{
				return static_cast<_Type*>(this->ptr_);
			}

		public:
			template<class _NewType = _Type, class ... Args> void make(const Args &... args)
			{
				if (this->ptr_)
				{
					delete this->ptr_;
				}
				this->ptr_ = new _NewType(args ...);
			}
			void remove(void)
			{
				if (this->ptr_)
				{
					delete static_cast<_Type*>(this->ptr_);
					this->ptr_ = nullptr;
				}

				this->CleanRefs();
			}
		};

		template<class _Type> class RefPtr final : public IRefPtr
		{
		public:
			RefPtr(void)
			{
			}
			template<class _NewType> RefPtr(const OriPtr<_NewType> & original)
				: IRefPtr(&original)
			{
				static_cast<_Type*>(*original);
			}

		public:
			const bool is_connected(void)
			{
				return this->original_;
			}

		public:
			_Type * const operator->(void)
			{
				return *(*static_cast<OriPtr<_Type>*>(this->original_));
			}
			IOriPtr * const operator*(void)
			{
				return this->original_;
			}
			template<class _NewType> const RefPtr<_Type> & operator=(OriPtr<_NewType> & original)
			{
				static_cast<_Type*>(*original);

				this->set_original(&original);

				return *this;
			}
			template<class _NewType> const RefPtr<_Type> & operator=(const RefPtr<_NewType> & ref)
			{
				static_cast<_Type*>(*ref.original);

				this->set_original(&ref.original);
			}
		};
	}
}