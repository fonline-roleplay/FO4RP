#ifndef __FOFMOD_CHANNELCONTROL__
#define __FOFMOD_CHANNELCONTROL__


#include "fofmodchannelcontrol.h"


namespace FOFMOD
{
	

	ChannelControl::ChannelControl()
	{
		this->activeEffects.clear();
	}
	
	ChannelControl::~ChannelControl()
	{
		this->DropAllEffects();
	}
	
	void ChannelControl::GetSystem(FOFMOD::System** ptr )
	{
		*ptr = this->system;
	}
	
	void ChannelControl::SetHandle( FMOD::ChannelControl* handle )
	{
		if( handle )
		{
			this->cchandle = handle;
		}
		else
		{
			this->cchandle = NULL;
		}
	}
	
	void ChannelControl::GetPitch( float* value )
	{
		if( this->cchandle )
		{
			this->cchandle->getPitch( value );
		}
	}

	void ChannelControl::SetPitch(float pitch)
	{
		if( this->cchandle )
		{
			this->cchandle->setPitch( pitch );
		}
	}
	
	void ChannelControl::SetPaused( bool paused )
	{
		if( this->cchandle )
		{
			this->cchandle->setPaused( paused );
		}
	}

	void ChannelControl::IsPaused( bool* value )
	{
		if( this->cchandle )
		{
			this->cchandle->getPaused( value );
		}
	}
	
	void ChannelControl::Stop()
	{
		if( this->cchandle )
		{
			this->cchandle->stop();
		}
	}


	void ChannelControl::IsPlaying( bool* value )
	{
		if( this->cchandle )
		{
			this->cchandle->isPlaying( value );
		}
	}
	
	void ChannelControl::SetEffect( FOFMOD::DSP* effect )
	{
		FMOD::DSP* handle = NULL;
		effect->GetHandle( &handle );
		if( handle )
		{
			if( this->cchandle )
			{
				for( DspPtrList::iterator it = this->activeEffects.begin(); it != this->activeEffects.end(); it++ )
				{
					FOFMOD::DSP* cur = *it;
					if( effect == cur  ) // already in
						return;
				}
				

				this->cchandle->addDSP( FMOD_CHANNELCONTROL_DSP_TAIL, handle );
				
				this->activeEffects.push_back( effect );
				effect->Addref();
			}
		}
	}
	
	void ChannelControl::DropEffect( FOFMOD::DSP* effect )
	{
		FMOD::DSP* handle = NULL;
		effect->GetHandle( &handle );
		if( handle )
		{
			if( this->cchandle )
			{
				this->cchandle->removeDSP( handle );
				
				for( DspPtrList::iterator it = this->activeEffects.begin(); it != this->activeEffects.end();  )
				{
					FOFMOD::DSP* cur = *it;
					if( effect == cur  )
					{
						this->cchandle->removeDSP( handle );
						it = this->activeEffects.erase(it);
						cur->Release();
						break;
					}
					else
					{
						it++;
					}
				}
			}
		}
	}
	
	void ChannelControl::DropAllEffects()
	{
		if( this->cchandle )
		{
			for( DspPtrList::iterator it = this->activeEffects.begin(); it != this->activeEffects.end(); it++ )
			{
				FOFMOD::DSP* cur = *it;
				if( cur )
				{
					FMOD::DSP* hndl = NULL;
					cur->GetHandle( &hndl );
					this->cchandle->removeDSP( hndl );
					cur->Release();
				}
			}
			
			this->activeEffects.clear();
		}
	}
	
	void ChannelControl::SetVolumeRamp( bool condition )
	{
		if( this->cchandle )
		{
			this->cchandle->setVolumeRamp( condition );
		}
	}
	
	void ChannelControl::GetVolumeRamp( bool* value )
	{
		if( this->cchandle )
		{
			this->cchandle->getVolumeRamp( value );
		}
	}
	
	void ChannelControl::SetVolume( float volume )
	{
		if( this->cchandle )
		{
			this->cchandle->setVolume( volume );
		}
	}

	void ChannelControl::GetVolume( float* value )
	{
		if( this->cchandle )
		{
			this->cchandle->getVolume( value );
		}
	}
	
	void ChannelControl::Set3DPosition( float x, float y, float z )
	{
		if( this->cchandle )
		{
			FMOD_VECTOR vec = {x, y, z};
			this->cchandle->set3DAttributes( &vec , NULL );
		}
	}

	void ChannelControl::Set3DVelocity( float x, float y, float z )
	{
		if( this->cchandle )
		{
			FMOD_VECTOR vec = {x, y, z};
			this->cchandle->set3DAttributes( NULL, &vec );
		}
	}
	
	void ChannelControl::Set3DMinMaxDistance( float minDistance, float maxDistance )
	{
		if( this->cchandle )
		{
			this->cchandle->set3DMinMaxDistance( minDistance, maxDistance );
		}
	}

	void ChannelControl::Get3DMinMaxDistance( float* minDistance, float* maxDistance )
	{
		if( this->cchandle )
		{
			this->cchandle->get3DMinMaxDistance( minDistance, maxDistance );
		}
	}

	void ChannelControl::Set3DLevel( float level )
	{
		if( this->cchandle )
		{
			this->cchandle->set3DLevel( level );
		}
	}

	void ChannelControl::Get3DLevel( float* value )
	{
		if( this->cchandle )
		{
			this->cchandle->get3DLevel( value );
		}
	}
}

#endif // __FOFMOD_CHANNELCONTROL__