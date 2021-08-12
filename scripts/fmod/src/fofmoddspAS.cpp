#ifndef __FOFMOD_DSP_AS__
#define __FOFMOD_DSP_AS__

#include "fofmoddspAS.h"

namespace FOFMOD
{
	
	void Script_DSP_Addref( FOFMOD::DSP* ptr )
	{
		if( ptr )
		{
			ptr->Addref();
		}
	}
	
	void Script_DSP_Release( FOFMOD::DSP* ptr )
	{
		if( ptr )
		{
			ptr->Release();
		}
	}
	
	bool Script_DSP_IsValid( FOFMOD::DSP* ptr )
	{
		bool result = false;
		if( ptr )
		{
			ptr->IsValid( &result );
		}
		return result;
	}
}


#endif // __FOFMOD_DSP_AS__