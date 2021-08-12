#ifndef __FOFMODCHANNELGROUP__
#define __FOFMODCHANNELGROUP__

#include "fofmodchannelgroup.h"
#include "stddef.h"

namespace FOFMOD
{
	ChannelGroup::ChannelGroup( )
	{

	}


	ChannelGroup::ChannelGroup( FMOD::System* ownerSystem )
	: ChannelControl()
	{
		this->handle = NULL;
		this->owner = ownerSystem;
	}
	
	ChannelGroup::~ChannelGroup()
	{
		if( this->handle )
		{
			this->handle->release();
		}
	}
		
	void ChannelGroup::SetHandle( FMOD::ChannelGroup* hndl )
	{
		if( this->handle )
		{
			this->handle->release();
		}
		
		if( hndl )
		{
			this->handle = hndl;
		}
		else
		{
			this->handle = NULL;
		}
		
		ChannelControl::SetHandle( hndl );
	}
	
	void ChannelGroup::GetHandle( FMOD::ChannelGroup** ptr )
	{
		*ptr = this->handle;
	}
}

#endif // __FOFMODCHANNELGROUP__