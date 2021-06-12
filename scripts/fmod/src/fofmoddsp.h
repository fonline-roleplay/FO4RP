#ifndef __FOFMOD_DSP_H__
#define __FOFMOD_DSP_H__

#include "fofmoddef.h"

namespace FOFMOD
{
	class System;
	class Channel;
	
	class DSP
	{
		protected:
			DSP();
			FOFMOD::System* system;
			FMOD::ChannelGroup* group; 
			FMOD::DSP* handle;
			FOFMOD::Channel* channel;
			
		public:
			DSP( FOFMOD::System* system );
			~DSP();
			bool IsValid();
			void SetGroup( FMOD::ChannelGroup* group );
			void SetChannel( FOFMOD::Channel* channel );
			void SetHandle( FMOD::DSP* handle );
			FMOD::ChannelGroup* GetGroup();
			FOFMOD::Channel* GetChannel();
			FMOD_DSP_TYPE GetType();
			void Invalidate();
	};
};

#endif // __FOFMOD_DSP_H__