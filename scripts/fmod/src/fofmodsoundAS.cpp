#ifndef __FOFMOD_SOUND_AS__
#define __FOFMOD_SOUND_AS__

#include "fofmodsoundAS.h"

namespace FOFMOD
{
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
};

#endif // __FOFMOD_SOUND_AS__