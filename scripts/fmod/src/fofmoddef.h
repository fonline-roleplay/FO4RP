#ifndef __FOFMODDEF_H__
#define __FOFMODDEF_H__

#include "fmod.hpp"
#include "fmod_errors.h"

#ifdef FOFMOD_DEBUG
	#define FOFMOD_DEBUG_LOG( ... ) Log( __VA_ARGS__ );
#else
	#define FOFMOD_DEBUG_LOG( ... )
#endif // FOFMOD_DEBUG



#endif // __FOFMODDEF_H__