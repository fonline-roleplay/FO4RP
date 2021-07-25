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
	
	bool DSP::IsValid()
	{
		return  ( ( this->handle != NULL ) );
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
	
	void DSP::GetHandle( FMOD::DSP** handle )
	{
		*handle = this->handle;
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
						FMOD_DSP_PARAMETER_DESC* paramInfo = (FMOD_DSP_PARAMETER_DESC*)malloc(sizeof(FMOD_DSP_PARAMETER_DESC));
						memset( paramInfo, 0, sizeof(FMOD_DSP_PARAMETER_DESC) );
						int curParam = 0;
						while ( curParam < numparams )
						{
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
									this->handle->setParameterBool( curParam, (bool)val );
									break;
								}
								// no data, unneccessary for external
								default:
									break;
							}
						}
						free( paramInfo );
					}
				}
			}
		}	
	}
	
	
};

#endif // __FOFMOD_DSP__