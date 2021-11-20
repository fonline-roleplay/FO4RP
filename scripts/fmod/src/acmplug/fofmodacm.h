#ifndef __FOFMOD_ACM_H__
#define __FOFMOD_ACM_H__

#include "fmod.hpp"
#include "fmod_errors.h"
#include "platform.h"

NO_MANGLING EXPORT FMOD_RESULT FMOD_System_RegisterCodec(  FMOD_SYSTEM *system, FMOD_CODEC_DESCRIPTION *description, unsigned int *handle, unsigned int priority );

#endif //__FOFMOD_ACM_H__