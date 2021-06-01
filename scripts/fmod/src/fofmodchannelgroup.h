#ifndef __FOFMODCHANNELGROUP_H__
#define __FOFMODCHANNELGROUP_H__

#include "fofmoddef.h"
#include "fofmodchannel.h"

namespace FOFMOD
{
	typedef std::vector< FOFMOD::Channel* > ChannelVec;

	class ChannelGroup
	{

		private:
			ChannelGroup(); // ban default constructor from external calls

		protected:
			FMOD::System* owner;
			FMOD::ChannelGroup* handle;
			FOFMOD::ChannelVec playingSounds;

		public:
			ChannelGroup( FMOD::System& ownerSystem );
			~ChannelGroup();
			FMOD_RESULT Initialize( const char* name );
			FOFMOD::Channel* PlaySound ( const std::string& soundName, bool paused );
	};
};


#endif // __FOFMODCHANNELGROUP_H__