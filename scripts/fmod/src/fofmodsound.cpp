#ifndef __FOFMOD_SOUND__
#define __FOFMOD_SOUND__

#include <stddef.h>
#include "_defines.fos"
#include "fonline.h"
#include "fofmodsound.h"

namespace FOFMOD
{
	
	// ban
	Sound::Sound()
	{
		
	}
	
	Sound::Sound( FOFMOD::System* system )
	{
		this->refcount = 0;
		this->handle  = NULL;
		this->cacheData = NULL;
		this->system = system;
	}
	
	Sound::~Sound()
	{
		this->SetHandle( NULL );
		this->SetCache( NULL );
	}
	

	void Sound::Addref()
	{
		#if defined ( FO_GCC )
		INTERLOCKED_INCREMENT (&this->refcount, 1);
		#else
		INTERLOCKED_INCREMENT (&this->refcount );
		#endif
	}

	void Sound::Release()
	{
		if(!
		#if defined ( FO_GCC ) 
		INTERLOCKED_DECREMENT ( &this->refcount, 1 )
		#else
		INTERLOCKED_DECREMENT ( &this->refcount )
		#endif
		)
		{
			FOFMOD_DEBUG_LOG(" Deleting Sound %u at refcount %u \n ", this->handle, this->refcount );
			delete this;
		}
	}
	
	unsigned int Sound::GetRefcount()
	{
		unsigned int result = INTERLOCKED_EXCHANGE( &(this->refcount), this->refcount );
		return result;
	}

	void  Sound::SetCache( FOFMOD::CacheSoundData* cdata )
	{
		if( cdata )
		{
			if( this->cacheData )
			{
				if( this->cacheData == cdata )
				{
					// same memory object
					// do nothing
					return;
				}
				else
				{
					// different
					this->cacheData->Release();
				}
			}
			
			this->cacheData = cdata;
			cdata->Addref();
		}
		else
		{
			if( this->cacheData )
			{
				this->cacheData->Release();
				this->cacheData = NULL;
			}
		}
	}
	
	void Sound::SetHandle( FMOD::Sound* snd )
	{
		if( snd )
		{
			if( this->handle )
			{
				if( this->handle == snd )
				{
					return;
				}
				else
				{
					this->handle->release();
				}
			}
			
			this->handle = snd;
		}
		else
		{
			this->Invalidate();
		}
	}
	
	unsigned int Sound::GetLength()
	{
		unsigned int result = 0;
		if( this->handle )
		{
			this->handle->getLength( &result, FMOD_TIMEUNIT_MS );
		}
		return result;
	}

	void Sound::SetLoopCount( int loopCount )
	{
		if( this->handle )
		{
			this->handle->setLoopCount( loopCount );
		}
	}

	int Sound::GetLoopCount()
	{
		int result = 0;
		if( this->handle )
		{
			this->handle->getLoopCount( &result );
		}
		return result;
	}

	bool Sound::IsValid()
	{
		return (this->handle != NULL);
	}
	
	void Sound::Invalidate()
	{
		if( this->handle )
		{
			if( this->handle )
			{
				this->handle->release();
				this->handle = NULL;
			}
		}
	}



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	


}

#endif // __FOFMOD_SOUND__