#ifndef __FOFMOD_CHANNEL_H__
#define __FOFMOD_CHANNEL_H__

// TODO
#include "fofmoddef.h"
#include "refcount.h"
#include "fofmodsound.h"

namespace FOFMOD
{
	class Sound;
	
	class Channel : public IRefcountable
	{
		protected:
			unsigned int refcount;

		public:
			FMOD::Channel* handle;
			FOFMOD::Sound* sound;
			Channel();
			~Channel();
			void Addref() override;
			void Release() override;
			void SetHandle( FMOD::Channel* chn );
			void SetSound( FOFMOD::Sound* snd );
			void SetPlaybackPosition( unsigned int positionMs );
			void GetPlaybackPosition( unsigned int* value );
			void GetSound( FOFMOD::Sound** value );
			void GetPitch( float* value );
			void SetPitch(float pitch);
			void IsValid( bool* value );
			void SetPaused( bool paused );
			void IsPaused( bool* value );
			void Stop();
			void IsPlaying( bool* value );
			void SetVolume( float volume );
			void GetVolume( float* value );
			void Set3DPosition( float x, float y, float z );
			void Set3DVelocity( float x, float y, float z );
			void GetLoopCount( int* value );
			void SetLoopCount( int count );
			void SetLoopPoints( unsigned int loopStart, unsigned int loopStartType, unsigned int loopEnd, unsigned int loopEndType );
			void GetLoopPoints( unsigned int* loopStart, unsigned int loopStartType, unsigned int* loopEnd, unsigned int loopEndType );
			void Set3DMinMaxDistance( float minDistance, float maxDistance );
			void Get3DMinMaxDistance( float* minDistance, float* maxDistance );
			void Set3DLevel( float level );
			void Get3DLevel( float* value );

			void Invalidate(); // for external signal processing, fmod doesnt invalidate my handle
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

	// volume
	extern void Script_Channel_SetVolume( float volume, FOFMOD::Channel* ptr );
	extern float Script_Channel_GetVolume( FOFMOD::Channel* ptr );

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



#endif // __FOFMOD_CHANNEL_H__