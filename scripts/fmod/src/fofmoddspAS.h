#ifndef __FOFMOD_DSP_AS_H__
#define __FOFMOD_DSP_AS_H__

#include "fofmoddsp.h"
#include "fonline.h"

namespace FOFMOD
{
	
	extern void Script_DSP_Addref( FOFMOD::DSP* ptr );
	extern void Script_DSP_Release( FOFMOD::DSP* ptr );
	extern bool Script_DSP_IsValid( FOFMOD::DSP* ptr );
}


#endif // __FOFMOD_DSP_AS_H__