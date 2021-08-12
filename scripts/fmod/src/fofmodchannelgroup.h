#ifndef __FOFMODCHANNELGROUP_H__
#define __FOFMODCHANNELGROUP_H__

#include "fofmoddef.h"
#include "fofmodchannel.h"
#include "fofmodchannelcontrol.h"

namespace FOFMOD
{
	class Channel;
	
	typedef std::vector< FOFMOD::Channel* > ChannelVec;
	class ChannelGroup : public ChannelControl
	{

		private:
			ChannelGroup(); // ban default constructor from external calls

		protected:
			FMOD::System* owner;
			FMOD::ChannelGroup* handle;

		public:
			ChannelGroup( FMOD::System* ownerSystem );
			~ChannelGroup();
			void SetHandle( FMOD::ChannelGroup* hndl );
			void GetHandle( FMOD::ChannelGroup** ptr );
	};
};


#endif // __FOFMODCHANNELGROUP_H__