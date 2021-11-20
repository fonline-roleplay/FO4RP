#ifndef __FOFMOD_DSP_H__
#define __FOFMOD_DSP_H__

#include "fofmoddef.h"
#include "refcount.h"

namespace FOFMOD
{
	class System;
	class Channel;
	
	class DSP : public IRefcountable
	{
		protected:
			DSP();
			FOFMOD::System* system; 
			FMOD::DSP* handle;
			unsigned int refcount;
			
		public:
			DSP( FOFMOD::System* system );
			~DSP();
			void Addref() override;
			void Release() override;
			unsigned int GetRefcount() override;
			void IsValid( bool* value );
			void SetHandle( FMOD::DSP* hndl );
			void GetHandle( FMOD::DSP** hndl ) const ;
			FMOD_DSP_TYPE GetType() const ;
			void Invalidate();
			void ParseParams( float* params, unsigned int paramsCount );
			bool operator==(const FOFMOD::DSP& r);
			bool operator==(const FMOD::DSP& r);

	};
	
};

#endif // __FOFMOD_DSP_H__