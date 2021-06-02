#ifndef __FOFMOD_CACHEDATA__
#define __FOFMOD_CACHEDATA__

#include "cachedata.h"
#include "_defines.fos"
#include "fonline.h"

namespace FOFMOD
{
	CacheSoundData::CacheSoundData() {} // BANNED

	CacheSoundData::CacheSoundData(FOFMOD::System* ownerSystem )
	{
		this->refcount = 0;
		this->data  = NULL;
		this->size  = 0;
		this->owner = ownerSystem;
	}

	CacheSoundData::~CacheSoundData()
	{
		if( this->data )
		{
			free ( data );
		}

		if( this->owner )
		{
			FOFMOD_DEBUG_LOG("Deletincg cached data \n")
			this->owner->DeleteCachedSound( this->it );
		}
	}

	void CacheSoundData::Addref()
	{
		#if defined ( FO_GCC )
		INTERLOCKED_INCREMENT (&this->refcount, 1);
		#else
		INTERLOCKED_INCREMENT (&this->refcount );
		#endif
	}

	void CacheSoundData::Release()
	{
		if(!
		#if defined ( FO_GCC ) 
		INTERLOCKED_DECREMENT ( &this->refcount, 1 )
		#else
		INTERLOCKED_DECREMENT ( &this->refcount )
		#endif
		)
		{
			FOFMOD_DEBUG_LOG("Cache sound data refcount zero release \n");
			delete this;
		}
	}
}


#endif // __FOFMOD_CACHEDATA__