#ifndef __FOFMOD_CACHEDATA_H__
#define __FOFMOD_CACHEDATA_H__

#include "fofmoddef.h"
#include "refcount.h"
#include "fofmodsystem.h"
#include <string>
#include <map>

namespace FOFMOD
{	

	class System;

	class CacheSoundData;
	typedef std::map< std::string, CacheSoundData* > CachedDataMap;
	
	class CacheSoundData : IRefcountable
	{

		protected:
			CacheSoundData();
			unsigned int refcount;

		public:
			CacheSoundData(FOFMOD::System* ownerSystem );
			~CacheSoundData();
			CachedDataMap::iterator it;
			FOFMOD::System* owner;
			void* data;
			unsigned int size;

			void Addref() override;
			void Release() override;
	};
	
}

#endif // __FOFMOD_CACHEDATA_H__