#ifndef __FOFMOD_CHANNELCONTROL_H__
#define __FOFMOD_CHANNELCONTROL_H__


#include "fofmoddef.h"
#include "fofmoddsp.h"
#include <list>


namespace FOFMOD
{
	typedef std::list<FOFMOD::DSP*> DspPtrList;
	
	class ChannelControl // base class for channelControl core interface implementors
	{
		
		protected:
			ChannelControl();
			FMOD::ChannelControl* cchandle;
			DspPtrList activeEffects;
			void SetHandle( FMOD::ChannelControl* handle );
			
		public:
			virtual ~ChannelControl();
			
			void GetPitch( float* value );
			void SetPitch(float pitch);
			void IsValid( bool* value );
			void SetPaused( bool paused );
			void IsPaused( bool* value );
			void IsPlaying( bool* value );
			void SetEffect( FOFMOD::DSP* effect );
			void DropEffect( FOFMOD::DSP* effect );
			void DropAllEffects();
			void SetVolumeRamp( bool condition );
			void GetVolumeRamp( bool* value );
			void SetVolume( float volume );
			void GetVolume( float* value );
			void Set3DPosition( float x, float y, float z );
			void Set3DVelocity( float x, float y, float z );
			void Set3DMinMaxDistance( float minDistance, float maxDistance );
			void Get3DMinMaxDistance( float* minDistance, float* maxDistance );
			void Set3DLevel( float level );
			void Get3DLevel( float* value );
			
			virtual void Stop();
	};
}

#endif // __FOFMOD_CHANNELCONTROL_H__