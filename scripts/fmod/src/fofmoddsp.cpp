#ifndef __FOFMOD_DSP__
#define __FOFMOD_DSP__

#include "fofmoddsp.h"
#include "stddef.h"

namespace FOFMOD
{
	// protected, ban.
	DSP::DSP(){}
	
	DSP::DSP( FOFMOD::System* system )
	{
		this->system = system;
	}
	
	DSP::~DSP()
	{
		
	}
	
	bool DSP::IsValid()
	{
		return  ( ( this->handle != NULL ) && ( ( this->channel != NULL ) || ( this->group != NULL ) ) );
	}
	
	void DSP::SetGroup( FMOD::ChannelGroup* group )
	{
		this->group = group;
	}
	
	void DSP::SetChannel( FOFMOD::Channel* channel )
	{
		this->channel = channel;
	}
	
	FMOD::ChannelGroup* DSP::GetGroup()
	{
		return this->group;
	}
	
	FOFMOD::Channel* DSP::GetChannel()
	{
		return this->channel;
	}
	
	FMOD_DSP_TYPE DSP::GetType()
	{
		FMOD_DSP_TYPE result = FMOD_DSP_TYPE::FMOD_DSP_TYPE_MAX;
		if( this->handle )
		{
			this->handle->getType( &result );
		}
		return result;
	}
	
	void DSP::SetHandle( FMOD::DSP* handle )
	{
		if( handle )
		{
			if( this->IsValid() )
			{
				if( this->handle == handle )
				{
					return;
				}
			}
			
			this->handle = handle;
		}
		else
		{
			this->Invalidate();
		}
	}
	
	void DSP::Invalidate()
	{
		if( this->handle )
		{
			this->handle->release();
			this->handle = NULL;
		}
	}
	
};

#endif // __FOFMOD_DSP__