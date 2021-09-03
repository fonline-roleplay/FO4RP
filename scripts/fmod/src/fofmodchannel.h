#ifndef __FOFMOD_CHANNEL_H__
#define __FOFMOD_CHANNEL_H__

// TODO
#include "fofmoddef.h"
#include "refcount.h"
#include "fofmodsound.h"
#include "fofmodchannelcontrol.h"

namespace FOFMOD
{
	class Sound;
	class System;
	class DSP;
	
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
	
	
	class Channel : public ChannelControl, public IRefcountable
	{
		private:
			Channel();
		
		protected:
			FOFMOD::System* system;
			FMOD::Channel* handle;
			FOFMOD::Sound* sound;

		public:
			CoreChannelRefcounter* coreRefcounter;
			ScriptChannelRefcounter* scriptRefcounter;
			Channel( FOFMOD::System* system );
			~Channel();
			void Addref() override;
			void Release() override;
			unsigned int GetRefcount() override;
			void SetHandle( FMOD::Channel* chn );
			void GetHandle( FMOD::Channel** chn );
			void SetSound( FOFMOD::Sound* snd );
			void GetSound( FOFMOD::Sound** value );
			void IsValid( bool* value );
			
			// Channel core
			void SetPlaybackPosition( unsigned int positionMs );
			void GetPlaybackPosition( unsigned int* value );
			void GetLoopCount( int* value );
			void SetLoopCount( int count );
			void SetLoopPoints( unsigned int loopStart, unsigned int loopStartType, unsigned int loopEnd, unsigned int loopEndType );
			void GetLoopPoints( unsigned int* loopStart, unsigned int loopStartType, unsigned int* loopEnd, unsigned int loopEndType );
			void SetFrequency( float hertz );
			void GetFrequency( float* value );
			void GetPriority( int* value );
			void SetPriority( int priority );
			
			void Stop() override;
			
			FOFMOD::System* GetSystem();
			void Invalidate(); // for external signal processing, fmod doesnt invalidate my handle
	};
	
};



#endif // __FOFMOD_CHANNEL_H__