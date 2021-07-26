#ifndef __REFCOUNT_H__
#define __REFCOUNT_H__


#include "_defines.fos"
#include "fonline.h"
#include "fofmodlock.h"


class IRefcountable
{
	public:
		virtual void Addref()  				= 0;
		virtual void Release() 				= 0;
		virtual unsigned int GetRefcount()  = 0;
};

class GenericRefcountable : public IRefcountable
{
	protected:
		unsigned int count;

	public:
		GenericRefcountable();
		void Addref() override;
		void Release() override;
		unsigned int GetRefcount() override;
};

template < typename T >
class AAuxiliaryRefcounter : IRefcountable
{
	protected:
		AAuxiliaryRefcounter(){} // ban
		unsigned int refcount;
		T* subject;

	public:
		AAuxiliaryRefcounter( T* subject )
		{
			this->refcount = 0;
			this->subject  = subject;
		}
		
		virtual ~AAuxiliaryRefcounter()
		{

		}

		void Addref() override
		{
			#if defined ( FO_GCC )
			INTERLOCKED_INCREMENT (&this->refcount, 1);
			#else
			INTERLOCKED_INCREMENT (&this->refcount );
			#endif
		};

		void Release() override
		{
			if(!
			#if defined ( FO_GCC ) 
			INTERLOCKED_DECREMENT ( &this->refcount, 1 )
			#else
			INTERLOCKED_DECREMENT ( &this->refcount )
			#endif
			)
			{
				if( this->subject )
				{
					this->OnZero();
				}
			}
		};
		
		unsigned int GetRefcount() override
		{
			unsigned int result = INTERLOCKED_EXCHANGE( &(this->refcount), this->refcount );
			return result;
		};

		virtual void OnZero() = 0;
};



// template < typename T >
// AAuxiliaryRefcounter< T >::AAuxiliaryRefcounter( T* subject )
// {
	// this->refcount = 0;
	// this->subject  = subject;
// }

// template < typename T >
// AAuxiliaryRefcounter< T >::~AAuxiliaryRefcounter()
// {

// }

// template < typename T >
// void AAuxiliaryRefcounter< T >::Addref()
// {
	// #if defined ( FO_GCC )
	// INTERLOCKED_INCREMENT (&this->refcount, 1);
	// #else
	// INTERLOCKED_INCREMENT (&this->refcount );
	// #endif
// }

// template < typename T >
// void AAuxiliaryRefcounter< T >::Release()
// {
	// if(!
	// #if defined ( FO_GCC ) 
	// INTERLOCKED_DECREMENT ( &this->refcount, 1 )
	// #else
	// INTERLOCKED_DECREMENT ( &this->refcount )
	// #endif
	// )
	// {
		// if( this->subject )
		// {
			// this->OnZero();
		// }
	// }
// }

// template < typename T >
// unsigned int AAuxiliaryRefcounter< T >::GetRefcount()
// {
	// unsigned int result = INTERLOCKED_EXCHANGE( &(this->refcount), this->refcount );
	// return result;
// }

#endif // __REFCOUNT_H__