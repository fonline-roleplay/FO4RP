#ifndef __FOFMOD_SOUNDBINARY__
#define __FOFMOD_SOUNDBINARY__

#include "memory.h"
#include "refcount.h"
#include <cstring>

namespace FOFMOD
{
	class SoundBinaryData : public IRefcountable
	{
		protected:
			unsigned int refcount;

		public:
			DataPtr_t bindata;

			SoundBinaryData();
			~SoundBinaryData();

			void Addref() override;
			void Release() override;
	}

	SoundBinaryData::SoundBinaryData()
	{
		this->refcount = 0;
	}

	SoundBinaryData::~SoundBinaryData()
	{
		if( this->bindata.ptr )
		{
			free( this->bindata.ptr );
		}
	}

	void SoundBinaryData::Addref()
	{
		#if defined ( FO_GCC )
		INTERLOCKED_INCREMENT (&this->refcount, 1);
		#else
		INTERLOCKED_INCREMENT (&this->refcount );
		#endif
	}

	void SoundBinaryData::Release()
	{
		if(!
		#if defined ( FO_GCC ) 
		INTERLOCKED_DECREMENT ( &this->refcount, 1 )
		#else
		INTERLOCKED_DECREMENT ( &this->refcount )
		#endif
		)
		{
			delete this;
		}

	}
}

#endif // __FOFMOD_SOUNDBINARY__