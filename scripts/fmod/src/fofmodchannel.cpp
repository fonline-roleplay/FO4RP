#ifndef __FOFMOD_CHANNEL__
#define __FOFMOD_CHANNEL__

// TODO
#include "fofmodchannel.h"
#include "fofmodchannelAS.h"
#include "stddef.h"
#include "_defines.fos"
#include "fonline.h"
#include "macros.h"
#include "fofmoddsp.h"

namespace FOFMOD
{
	
	CoreChannelRefcounter::CoreChannelRefcounter(){}//ban
	
	CoreChannelRefcounter::CoreChannelRefcounter( FOFMOD::Channel* subject ) 
	: AAuxiliaryRefcounter < FOFMOD::Channel > ( subject )
	{
	
	}
	
	CoreChannelRefcounter::~CoreChannelRefcounter()
	{
		
	}
	
	void CoreChannelRefcounter::OnZero()
	{
		FOFMOD::Channel* subject = this->subject;
		if( subject )
		{
			if ( !subject->GetRefcount() )
			{
				FOFMOD::System* chSystem;
				subject->GetSystem(&chSystem);
				chSystem->OnChannelDelete(subject);
				delete subject;
				FOFMOD_DEBUG_LOG( "core channel release delete \n" );
			}
			
		}
	}
	
	Channel::Channel(){} // ban
	
	Channel::Channel( FOFMOD::System* system )
	: ChannelControl()
	{
		this->handle = NULL;
		this->sound = NULL;
		this->system = system;
		this->coreRefcounter   = new CoreChannelRefcounter( this );
		this->scriptRefcounter = new ScriptChannelRefcounter( this );
	}
	
	Channel::~Channel()
	{
		this->SetSound( NULL );
		delete( this->coreRefcounter );
		delete( this->scriptRefcounter );
	}
	
	void Channel::Addref()
	{
		this->coreRefcounter->Addref();
	}

	void Channel::Release()
	{
		this->coreRefcounter->Release();
	}
	
	unsigned int Channel::GetRefcount()
	{
		unsigned int result = ( this->coreRefcounter->GetRefcount() ) + ( this->scriptRefcounter->GetRefcount() ) ;
		return result;
	}
	
	void Channel::SetHandle( FMOD::Channel* chn )
	{
		if( chn )
		{
			if( this->handle )
			{
				if( this->handle == chn )
				{
					return;
				}
			}
			
			this->handle = chn;
			ChannelControl::SetHandle( static_cast<FMOD::ChannelControl*> ( chn ) );
		}
		else
		{
			if( this->handle )
			{
				this->Invalidate();
			}
		}
	}
	
	void Channel::GetHandle( FMOD::Channel** chn )
	{
		*chn = this->handle;
	}

	void Channel::SetSound( FOFMOD::Sound* snd )
	{
		if( snd )
		{
			if( this->sound )
			{
				if( this->sound == snd )
				{
					return;
				}
				else
				{
					this->sound->Release();
					
				}
			}
			
			this->sound = snd;
			snd->Addref();
			
		}
		else
		{
			if( this->sound )
			{
				this->sound->Release();
				this->sound = NULL;
			}
		}
	}
	


	void Channel::SetPlaybackPosition( unsigned int positionMs )
	{
		if( this->handle )
		{
			this->handle->setPosition( positionMs, FMOD_TIMEUNIT_MS );
		}
	}

	void Channel::GetPlaybackPosition( unsigned int* value )
	{
		if( this->handle )
		{
			this->handle->getPosition( value, FMOD_TIMEUNIT_MS );
		}
	}

	void Channel::GetSound( FOFMOD::Sound** value )
	{
		if( this->sound )
		{
			*value = this->sound;
			(*value)->Addref();
		}
	}

	void Channel::IsValid( bool* value )
	{
		// fmod docs say the handle will be invalidated by the system when its done, but it does not define what is "invalidation", assuming NULL;
		*value = ( this->handle != NULL ); 
	}


	void Channel::SetFrequency( float hertz )
	{
		if( this->handle )
		{
			this->handle->setFrequency( hertz );
		}
	}
	
	void Channel::GetFrequency( float* value )
	{
		if( this->handle )
		{
			this->handle->getFrequency( value );
		}
	}
	
	
	void Channel::GetPriority( int* value )
	{
		if( this->handle )
		{
			this->handle->getPriority( value );
		}
	}
	
	void Channel::SetPriority( int value )
	{
		if( this->handle )
		{
			this->handle->setPriority( value );
		}
	}
	
	void Channel::Stop() // override
	{
		ChannelControl::Stop();
		this->SetHandle( NULL );
	}

	void Channel::GetLoopCount( int* value )
	{
		if( this->handle )
		{
			this->handle->getLoopCount( value );
		}
	}
	void Channel::SetLoopCount( int count )
	{
		if( this->handle )
		{
			this->handle->setLoopCount( count );
		}
	}

	void Channel::SetLoopPoints( unsigned int loopStart, unsigned int loopStartType, unsigned int loopEnd, unsigned int loopEndType )
	{
		if( this->handle )
		{
			this->handle->setLoopPoints( loopStart, loopStartType, loopEnd, loopEndType );
		}
	}

	void Channel::GetLoopPoints( unsigned int* loopStart, unsigned int loopStartType, unsigned int* loopEnd, unsigned int loopEndType )
	{
		if( this->handle )
		{
			this->handle->getLoopPoints( loopStart, loopStartType, loopEnd, loopEndType );
		}
	}

	void Channel::Invalidate()
	{
		if( this->handle )
		{
			this->handle = NULL;
			ChannelControl::SetHandle( NULL );
		}
	}



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
}

#endif // __FOFMOD_CHANNEL__