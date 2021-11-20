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
		this->refcount = 0;
	}
	
	DSP::~DSP()
	{
		this->SetHandle( NULL );
	}
	
	void DSP::Addref()
	{
		#if defined ( FO_GCC )
		INTERLOCKED_INCREMENT (&this->refcount, 1);
		#else
		INTERLOCKED_INCREMENT (&this->refcount );
		#endif
	}

	void DSP::Release()
	{
		if(!
		#if defined ( FO_GCC ) 
		INTERLOCKED_DECREMENT ( &this->refcount, 1 )
		#else
		INTERLOCKED_DECREMENT ( &this->refcount )
		#endif
		)
		{
			FOFMOD_DEBUG_LOG("Deleting DSP %u at refcount %u \n ", this->handle, this->refcount );
			delete this;
		}
	}
	
	unsigned int DSP::GetRefcount()
	{
		unsigned int result = INTERLOCKED_EXCHANGE( &(this->refcount), this->refcount );
		return result;
	}
	
	void DSP::IsValid(bool* value)
	{
		*value = ( ( this->handle != NULL ) );
	}
	
	
	FMOD_DSP_TYPE DSP::GetType() const
	{
		FMOD_DSP_TYPE result = FMOD_DSP_TYPE::FMOD_DSP_TYPE_MAX;
		if( this->handle )
		{
			this->handle->getType( &result );
		}
		return result;
	}
	
	void DSP::SetHandle( FMOD::DSP* hndl )
	{
		if( hndl )
		{
			bool res = false;
			this->IsValid( &res );
			
			if( res )
			{
				if( this->handle == hndl )
				{
					return;
				}
			}
			
			this->handle = hndl;
		}
		else
		{
			this->Invalidate();
		}
	}
	
	void DSP::GetHandle( FMOD::DSP** hndl ) const 
	{
		*hndl = this->handle;
	}
	
	void DSP::Invalidate()
	{
		if( this->handle )
		{
			this->handle->release();
			this->handle = NULL;
		}
	}
	
	void DSP::ParseParams( float* params, unsigned int paramsCount )
	{
		if( this->handle )
		{
			if( params )
			{
				if( paramsCount )
				{
					int numparams = 0;
					this->handle->getNumParameters( &numparams );
					if( numparams )
					{
						FMOD_DSP_PARAMETER_DESC* paramInfo;
						int curParam = 0;
						//Log("numparams %d paramC %d\n", numparams, paramsCount );
						while ( ( curParam < numparams ) && ( curParam < paramsCount ) )
						{
						//	Log("Cur param %d\n", curParam);
							float val = params[curParam];
							this->handle->getParameterInfo( curParam, &paramInfo );
							FMOD_DSP_PARAMETER_TYPE ptype = paramInfo->type;
							switch( ptype )
							{
								case ( FMOD_DSP_PARAMETER_TYPE_FLOAT ):
								{
									this->handle->setParameterFloat( curParam, val );
									break;
								}
								case( FMOD_DSP_PARAMETER_TYPE_INT ):
								{
									this->handle->setParameterInt( curParam, (int)val );
									break;
								}
								case( FMOD_DSP_PARAMETER_TYPE_BOOL ):
								{
									this->handle->setParameterBool( curParam, ( val > 0.0f ) ? true : false );
									break;
								}
								// no data, unneccessary for external
								default:
									break;
							}
							curParam++;
						}
					}
				}
			}
		}	
	}
	
	bool DSP::operator==(const FOFMOD::DSP& r)
	{
		bool result = false;
		FMOD::DSP* dh = NULL;
		r.GetHandle( &dh );
		if( dh )
		{
			FMOD::DSP* th = NULL;
			this->GetHandle( &th );
			result = ( th == dh );
		}
		return result;
	}
	
	bool DSP::operator==(const FMOD::DSP& r)
	{
		bool result = false;
		
		FMOD::DSP* th = NULL;
		this->GetHandle( &th );
		result = ( th == &r );
		
		return result;
	}
	
};

#endif // __FOFMOD_DSP__