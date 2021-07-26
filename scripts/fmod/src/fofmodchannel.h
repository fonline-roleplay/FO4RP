#ifndef __FOFMOD_CHANNEL_H__
#define __FOFMOD_CHANNEL_H__

// TODO
#include "fofmoddef.h"
#include "refcount.h"
#include "fofmodsound.h"

namespace FOFMOD
{
	class Sound;
	class System;
	
	class ScriptChannelRefcounter;
	
	
	class Channel;
	class CoreChannelRefcounter : public  AAuxiliaryRefcounter< FOFMOD::Channel >
	{
		private:
			CoreChannelRefcounter();
		
		public:
			CoreChannelRefcounter( FOFMOD::Channel* subject );
			~CoreChannelRefcounter();
			void OnZero() override;
	};
	
	
	class Channel : public IRefcountable
	{
		private:
			Channel();
		
		protected:
			FOFMOD::System* system;
			unsigned int refcount;

		public:
			CoreChannelRefcounter* coreRefcounter;
			ScriptChannelRefcounter* scriptRefcounter;
			FMOD::Channel* handle;
			FOFMOD::Sound* sound;
			Channel( FOFMOD::System* system );
			~Channel();
			void Addref() override;
			void Release() override;
			unsigned int GetRefcount() override;
			void SetHandle( FMOD::Channel* chn );
			void GetHandle( FMOD::Channel** chn );
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
			void GetPriority( int* value );
			void SetPriority( int priority );
			void SetEffect( int effectType, float* params, unsigned int paramsSize, bool condition );
			void SetFrequency( float hertz );
			void GetFrequency( float* value );
			void SetVolumeRamp( bool condition );
			void GetVolumeRamp( bool* value );
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
			
			FOFMOD::System* GetSystem();
			void Invalidate(); // for external signal processing, fmod doesnt invalidate my handle
	};
	
};



#endif // __FOFMOD_CHANNEL_H__