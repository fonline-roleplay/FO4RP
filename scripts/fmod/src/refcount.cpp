








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

