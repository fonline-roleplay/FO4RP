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
			bool IsValid();
			void SetHandle( FMOD::DSP* handle );
			void GetHandle( FMOD::DSP** handle );
			FMOD_DSP_TYPE GetType();
			void Invalidate();
			void ParseParams( float* params, unsigned int paramsCount );

	};
	
};

#endif // __FOFMOD_DSP_H__