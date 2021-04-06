#ifndef __FOFMOD_SOUNDBINARY_H__
#define __FOFMOD_SOUNDBINARY_H__

#include "memory.h"
#include "refcount.h"

namespace FOFMOD
{
	class SoundBinaryData : public IRefcountable
	{
		public:
			DataPtr_t bindata;

			SoundBinaryData();
			~SoundBinaryData();

			void Addref() override;
			void Release() override;
	}
}

#endif // __FOFMOD_SOUNDBINARY_H__