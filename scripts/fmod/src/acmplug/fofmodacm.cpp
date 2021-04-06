#ifndef __FOFMOD_ACM__
#define __FOFMOD_ACM__

#include "fofmodacm.h"


// typedef struct FMOD_CODEC_DESCRIPTION {
//   const char                         *name;
//   unsigned int                        version;
//   int                                 defaultasstream;
//   FMOD_TIMEUNIT                       timeunits;
//   FMOD_CODEC_OPEN_CALLBACK            open;
//   FMOD_CODEC_CLOSE_CALLBACK           close;
//   FMOD_CODEC_READ_CALLBACK            read;
//   FMOD_CODEC_GETLENGTH_CALLBACK       getlength;
//   FMOD_CODEC_SETPOSITION_CALLBACK     setposition;
//   FMOD_CODEC_GETPOSITION_CALLBACK     getposition;
//   FMOD_CODEC_SOUNDCREATE_CALLBACK     soundcreate;
//   FMOD_CODEC_GETWAVEFORMAT_CALLBACK   getwaveformat;
// } FMOD_CODEC_DESCRIPTION;


FMOD_RESULT FMOD_System_RegisterCodec(  FMOD_SYSTEM *system, FMOD_CODEC_DESCRIPTION *description, unsigned int *handle, unsigned int priority )
{
	
	return FMOD_OK;
}

#endif //__FOFMOD_ACM__