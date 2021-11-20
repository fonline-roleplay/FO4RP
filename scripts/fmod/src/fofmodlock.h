#ifndef __FOFMOD_LOCK_H__
#define __FOFMOD_LOCK_H__

#include "platform.h"

#ifdef __WINDOWS__
	#include "windows.h" 
#elif defined ( __LINUX__ )
	#include <pthread.h>
#endif //__WINDOWS__  || __LINUX__

#undef INTERLOCKED_INCREMENT	
#undef INTERLOCKED_DECREMENT
#undef INTERLOCKED_EXCHANGE

#if defined ( __GCC__ )
	#define INTERLOCKED_INCREMENT __sync_add_and_fetch
	#define INTERLOCKED_DECREMENT __sync_sub_and_fetch
	#define INTERLOCKED_EXCHANGE  __sync_lock_test_and_set
#elif defined ( __MSVC__ )
	#if defined ( __WINDOWS__ )
		#define INTERLOCKED_INCREMENT InterlockedIncrement
		#define INTERLOCKED_DECREMENT InterlockedDecrement
		#define INTERLOCKED_EXCHANGE  InterlockedExchange
	#elif 
		#error "__MSVC__ and not __WINDOWS__, Can't define INTERLOCKED." // __MSVC__ is compiler determined, and MSVC is not compiling for non windows ?
	#endif
#endif


#define ALOC_NOOWNER  ( -1 )
#define ALOC_NOHANDLE ( NULL )

namespace FOFMOD
{
	class AtomicLock
	{
		protected:
		#ifdef __WINDOWS__
			DWORD ownerId;
			HANDLE sysThreadHandle;
		#elif defined ( __LINUX__ ) 
			pthread_t ownerId;
		#endif // __WINDOWS__ || __LINUX__
		
		public:
			AtomicLock();
			~AtomicLock();
			void Lock();
			bool TryLock();
			void Unlock();
			bool IsLocked();
	};
};

#endif // __FOFMOD_LOCK_H__