#ifndef __FOFMODCHANNELGROUP__
#define __FOFMODCHANNELGROUP__

#include "fofmodchannelgroup.h"
#include "stddef.h"

namespace FOFMOD
{
	ChannelGroup::ChannelGroup( )
	{

	}


	ChannelGroup::ChannelGroup( FMOD::System& ownerSystem )
	{
		this->handle = NULL;
		if( &ownerSystem )
		{
			this->ownerSystem = &ownerSystem;
		}
	}
	
	ChannelGroup::~ChannelGroup()
	{
		if( this->handle )
		{
			this->handle->release();
		}
	}

	FMOD_RESULT ChannelGroup::Initialize( const char* name )
	{
		FMOD_RESULT result = FMOD_ERR_UNINITIALIZED;
		if( this->ownerSystem )
		{
			result = this->ownerSystem->createChannelGroup( name, &this->handle );
		}
		return result;
	}

	FOFMOD::Channel*  ChannelGroup::PlaySound ( const std::string& soundName, bool paused )
	{
		FOFMOD::Channel* chn = NULL;
		if( this->ownerSystem )
		{
			FOFMOD::Sound* snd = this->ownerSystem->GetCachedSound( soundName );
			if( snd )
			{
				chn = new FOFMOD::Channel();
				chn->Addref();
				this->FSystem->playSound( snd->handle, this->handle, paused, &chn->handle  );
				chn = this->soundChannelGroup->PlaySound( snd );
			}
		}
		return chn;
	}

}

#endif // __FOFMODCHANNELGROUP__