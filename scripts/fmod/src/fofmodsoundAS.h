#ifndef __FOFMOD_SOUND_AS_H__
#define __FOFMOD_SOUND_AS_H__

#include "fofmodsound.h"

namespace FOFMOD
{
	// Angelscript interfacing for type methods
	extern void Script_Sound_Addref( FOFMOD::Sound* ptr );
	extern void Script_Sound_Release( FOFMOD::Sound* ptr );
	extern unsigned int Script_Sound_GetLength( FOFMOD::Sound* ptr );
	extern void	Script_Sound_SetLoopCount( int loopCount, FOFMOD::Sound* ptr ); // -1 for forever
	extern int  Script_Sound_GetLoopCount( FOFMOD::Sound* ptr );
	extern bool Script_Sound_IsValid( FOFMOD::Sound* ptr );
};

#endif // __FOFMOD_SOUND_AS_H__