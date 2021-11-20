#ifndef __FOFMOD_CHANNEL_AS__
#define __FOFMOD_CHANNEL_AS__

#include "fofmodchannelAS.h"
#include "macros.h"

namespace FOFMOD
{
	
	ScriptChannelRefcounter::ScriptChannelRefcounter(){}//
	
	ScriptChannelRefcounter::ScriptChannelRefcounter( FOFMOD::Channel* subject ) 
	: AAuxiliaryRefcounter < FOFMOD::Channel > ( subject )
	{
		
	}
	
	ScriptChannelRefcounter::~ScriptChannelRefcounter()
	{
		
	}
	
	void ScriptChannelRefcounter::OnZero()
	{
		FOFMOD::Channel* subject = this->subject;
		if( subject )
		{
			unsigned int refc = subject->GetRefcount();
			
			bool res = false;
			subject->IsPlaying( &res );
			if( res )
			{
				res = false;
				subject->IsPaused( &res );
				if( res )
				{
					FOFMOD_DEBUG_LOG("script paused sound zero \n");
					subject->Stop();
				}
			}
			
			if ( !refc )
			{
				delete subject;
				FOFMOD_DEBUG_LOG( "script channel zero delete \n" );
			}
		}
	}
	
	void Script_Channel_Addref( FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->scriptRefcounter->Addref();
		}
	}

	void Script_Channel_Release( FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->scriptRefcounter->Release();
		}
	}

	void Script_Channel_SetPlaybackPosition( unsigned int positionMs, FOFMOD::Channel* ptr)
	{
		if( ptr )
		{
			ptr->SetPlaybackPosition( positionMs );
		}
		
	}

	unsigned int Script_Channel_GetPlaybackPosition( FOFMOD::Channel* ptr )
	{
		unsigned int result = 0;
		if( ptr )
		{
			ptr->GetPlaybackPosition( &result );
		}
		return result;
	}

	FOFMOD::Sound* Script_Channel_GetSound( FOFMOD::Channel* ptr )
	{
		FOFMOD::Sound* result = NULL;
		if( ptr )
		{
			ptr->GetSound( &result );
		}
		return result;
	}

	float Script_Channel_GetPitch( FOFMOD::Channel* ptr )
	{
		float result = 0.0;
		if( ptr )
		{
			ptr->GetPitch( &result );
		}
		return result;
	}

	void Script_Channel_SetPitch( float pitch, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->SetPitch( pitch );
		}
	}

	bool Script_Channel_IsValid( FOFMOD::Channel* ptr )
	{
		bool result = false;
		if( ptr )
		{
			ptr->IsValid( &result );
		}
		return result;
	}


	void Script_Channel_SetPaused( bool paused, FOFMOD::Channel* ptr  )
	{	
		if( ptr )
		{
			ptr->SetPaused( paused );
		}
	}

	bool Script_Channel_IsPaused( FOFMOD::Channel* ptr )
	{
		bool result = false;
		if( ptr )
		{
			ptr->IsPaused( &result );
		}
		return result;
	}
	
	int Script_Channel_GetPriority( FOFMOD::Channel* ptr )
	{
		int result = 0;
		if( ptr )
		{
			ptr->GetPriority( &result );
		}
		return result;
	}
	
	void Script_Channel_SetPriority( int priority, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			priority = CLAMPA( priority, 0, 255 );
			ptr->SetPriority( priority );
		}
	}
	
	void Script_Channel_SetEffect( FOFMOD::DSP* effect, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			if( effect )
			{
				ptr->SetEffect( effect );
			}
		}
	}
	
	void Script_Channel_DropEffect( FOFMOD::DSP* effect, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			if( effect )
			{
				ptr->DropEffect( effect );
			}
		}
	}
	
	void Script_Channel_SetFrequency( float hertz, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->SetFrequency( hertz );
		}
	}
	
	float Script_Channel_GetFrequency( FOFMOD::Channel* ptr )
	{
		float result = 0.0;
		if( ptr )
		{
			ptr->GetFrequency( &result );
		}
		return result;
	}
	
	void Script_Channel_SetVolumeRamp( bool condition, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->SetVolumeRamp( condition );
		}
	}
	bool Script_Channel_GetVolumeRamp( FOFMOD::Channel* ptr )
	{
		bool result = false;
		if( ptr )
		{
			ptr->GetVolumeRamp( &result );
		}
		return result;
	}
	

	void Script_Channel_Stop(FOFMOD::Channel* ptr  )
	{
		if( ptr )
		{
			ptr->Stop();
		}
	}

	bool Script_Channel_IsPlaying( FOFMOD::Channel* ptr )
	{
		bool result = false;
		if( ptr )
		{
			ptr->IsPlaying( &result );
		}
		return result;
	}

	void Script_Channel_SetVolume( float volume, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->SetVolume( volume );
		}
	}

	float Script_Channel_GetVolume( FOFMOD::Channel* ptr )
	{
		float result = 0.0;
		if( ptr )
		{
			ptr->GetVolume( &result );
		}
		return result;
	}

	void Script_Channel_Set3DPosition( float x, float y, float z, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->Set3DPosition( x, y, z );
		}
	}

	void Script_Channel_Set3DVelocity( float x, float y, float z, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->Set3DVelocity( x, y, z );
		}
	}

	int Script_Channel_GetLoopCount( FOFMOD::Channel* ptr )
	{
		int result = 0;
		if( ptr )
		{
			ptr->GetLoopCount( &result );
		}
		return result;
	}

	void Script_Channel_SetLoopCount( int count, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->SetLoopCount( count );
		}
	}

	void Script_Channel_GetLoopPoints( unsigned int* loopStart, unsigned int loopStartType, unsigned int* loopEnd, unsigned int loopEndType, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->GetLoopPoints( loopStart, loopStartType, loopEnd, loopEndType );
		}
	}

	void Script_Channel_SetLoopPoints( unsigned int loopStart, unsigned int loopStartType, unsigned int loopEnd, unsigned int loopEndType, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->SetLoopPoints( loopStart, loopStartType, loopEnd, loopEndType );
		}
	}

	void Script_Channel_Set3DMinMaxDistance( float minDistance, float maxDistance, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->Set3DMinMaxDistance( minDistance, maxDistance );
		}
	}
	
	void Script_Channel_Get3DMinMaxDistance( float* minDistance, float* maxDistance, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->Get3DMinMaxDistance( minDistance, maxDistance );
		}
	}

	void Script_Channel_Set3DLevel( float level, FOFMOD::Channel* ptr )
	{
		if( ptr )
		{
			ptr->Set3DLevel( level );
		}
	}

	float Script_Channel_Get3DLevel( FOFMOD::Channel* ptr )
	{
		float result = 0.0f;

		if( ptr )
		{
			ptr->Get3DLevel( &result );
		}

		return result;
	}
};

#endif // __FOFMOD_CHANNEL_AS__