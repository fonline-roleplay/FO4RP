#ifndef __FOFMOD_SOUND__
#define __FOFMOD_SOUND__

#include "fofmodsound.h"
#include <stddef.h>
#include "_defines.fos"
#include "fonline.h"


namespace FOFMOD
{

	Sound::Sound()
	{
		this->refcount = 0;
		this->handle  = NULL;
	}
	
	Sound::Sound( FMOD::Sound* handle )
	{
		this->refcount = 0;
		this->handle = handle;
	}

	Sound::~Sound()
	{
		if( this->handle )
		{
			this->handle->release();
			this->handle = NULL;
		}
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
			FOFMOD_DEBUG_LOG(" Deleting scriptSound %u at refcount %u \n ", this->handle, this->refcount );
			delete this;
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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	void Script_Sound_Addref( FOFMOD::Sound* ptr )
	{
		if( ptr )
		{
			ptr->Addref();
		}
	}

	void Script_Sound_Release( FOFMOD::Sound* ptr )
	{
		if( ptr )
		{
			ptr->Release();
		}
	}

	unsigned int Script_Sound_GetLength( FOFMOD::Sound* ptr )
	{
		unsigned int result = 0;
		if( ptr )
		{
			result = ptr->GetLength();
		}
		return result;
	}

	void Script_Sound_SetLoopCount( int loopCount, FOFMOD::Sound* ptr)
	{
		if( ptr )
		{
			ptr->SetLoopCount( loopCount );
		}
		
	}

	int Script_Sound_GetLoopCount( FOFMOD::Sound* ptr )
	{
		int result = 0;
		if( ptr )
		{
			result = ptr->GetLoopCount();
		}
		return result;
	}

	bool Script_Sound_IsValid( FOFMOD::Sound* ptr )
	{
		bool result = false;
		if( ptr )
		{
			result = ptr->IsValid();
		}

		return result;
	}


}

#endif // __FOFMOD_SOUND__