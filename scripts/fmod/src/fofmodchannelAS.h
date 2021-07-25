#ifndef __FOFMOD_CHANNEL_AS_H__
#define __FOFMOD_CHANNEL_AS_H__

#include "fofmodchannel.h"
#include "fonline.h"

namespace FOFMOD
{
	class ScriptChannelRefcounter : public  AAuxiliaryRefcounter< FOFMOD::Channel >
	{
		void OnZero() override;
	};
	
	extern void Script_Channel_SetPlaybackPosition( unsigned int positionMs, FOFMOD::Channel* ptr);
	extern unsigned int Script_Channel_GetPlaybackPosition( FOFMOD::Channel* ptr );
	extern FOFMOD::Sound* Script_Channel_GetSound( FOFMOD::Channel* ptr );

	// pitches
	extern float Script_Channel_GetPitch( FOFMOD::Channel* ptr );
	extern void Script_Channel_SetPitch( float pitch, FOFMOD::Channel* ptr );

	// script object specific
	extern void Script_Channel_Addref( FOFMOD::Channel* ptr );
	extern void Script_Channel_Release( FOFMOD::Channel* ptr );
	extern bool Script_Channel_IsValid( FOFMOD::Channel* ptr );

	// playback
	extern void Script_Channel_SetPaused( bool paused, FOFMOD::Channel* ptr );
	extern bool Script_Channel_IsPaused( FOFMOD::Channel* ptr  );
	extern void Script_Channel_Stop( FOFMOD::Channel* ptr  );
	extern bool Script_Channel_IsPlaying( FOFMOD::Channel* ptr );
	extern int  Script_Channel_GetPriority( FOFMOD::Channel* ptr );
	extern void Script_Channel_SetPriority ( int priority, FOFMOD::Channel* ptr );
	extern void Script_Channel_SetEffect( int effectType, ScriptArray* params, bool condition, FOFMOD::Channel* ptr );
	extern void Script_Channel_SetFrequency( float hertz, FOFMOD::Channel* ptr );
	extern float Script_Channel_GetFrequency( FOFMOD::Channel* ptr );

	// volume
	extern void  Script_Channel_SetVolume( float volume, FOFMOD::Channel* ptr );
	extern float Script_Channel_GetVolume( FOFMOD::Channel* ptr );
	extern void  Script_Channel_SetVolumeRamp( bool condition, FOFMOD::Channel* ptr );
	extern bool  Script_Channel_GetVolumeRamp( FOFMOD::Channel* ptr );

	// positioning
	extern void Script_Channel_Set3DPosition( float x, float y, float z, FOFMOD::Channel* ptr );
	extern void Script_Channel_Set3DVelocity( float x, float y, float z, FOFMOD::Channel* ptr );
	extern void Script_Channel_Set3DMinMaxDistance( float minDistance, float maxDistance, FOFMOD::Channel* ptr );
	extern void Script_Channel_Get3DMinMaxDistance( float* minDistance, float* maxDistance, FOFMOD::Channel* ptr );
	extern void Script_Channel_Set3DLevel( float level, FOFMOD::Channel* ptr );
	extern float Script_Channel_Get3DLevel( FOFMOD::Channel* ptr );

	// loops
	extern int Script_Channel_GetLoopCount( FOFMOD::Channel* ptr );
	extern void Script_Channel_SetLoopCount( int count, FOFMOD::Channel* ptr );
	extern void Script_Channel_GetLoopPoints( unsigned int* loopStart, unsigned int loopStartType, unsigned int* loopEnd, unsigned int loopEndType, FOFMOD::Channel* ptr );
	extern void Script_Channel_SetLoopPoints( unsigned int loopStart, unsigned int loopStartType, unsigned int loopEnd, unsigned int loopEndType, FOFMOD::Channel* ptr );

};

#endif // __FOFMOD_CHANNEL_AS_H__