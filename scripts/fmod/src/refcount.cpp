








#include "refcount.h"


	
GenericRefcountable::GenericRefcountable()
{

}

void GenericRefcountable::Addref()
{
	#if defined ( FO_GCC )
	INTERLOCKED_INCREMENT (&this->count, 1);
	#else
	INTERLOCKED_INCREMENT (&this->count );
	#endif
}

void GenericRefcountable::Release()
{
	if(!
	#if defined ( FO_GCC ) 
	INTERLOCKED_DECREMENT ( &this->count, 1 )
	#else
	INTERLOCKED_DECREMENT ( &this->count )
	#endif
	)
	{
		// refcount zero
	}

}


unsigned int GenericRefcountable::GetRefcount()
{
	unsigned int result = INTERLOCKED_EXCHANGE( &(this->count), this->count );
	return result;
}


template < typename T >
AAuxiliaryRefcounter< T >::AAuxiliaryRefcounter( T* subject )
{
	this->refcount = 0;
	this->subject  = subject;
}

template < typename T >
AAuxiliaryRefcounter< T >::~AAuxiliaryRefcounter()
{

}

template < typename T >
void AAuxiliaryRefcounter< T >::Addref()
{
	#if defined ( FO_GCC )
	INTERLOCKED_INCREMENT (&this->refcount, 1);
	#else
	INTERLOCKED_INCREMENT (&this->refcount );
	#endif
}

template < typename T >
void AAuxiliaryRefcounter< T >::Release()
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
}

template < typename T >
unsigned int AAuxiliaryRefcounter< T >::GetRefcount()
{
	unsigned int result = INTERLOCKED_EXCHANGE( &(this->refcount), this->refcount );
	return result;
}

