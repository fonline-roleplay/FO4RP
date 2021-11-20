#ifndef __FOFMOD_THREAD_H__
#define __FOFMOD_THREAD_H__

#include "platform.h"
#include <string>

#ifdef __WINDOWS__
	#include "windows.h" 
#elif defined ( __LINUX__ )
	#include <pthread.h>
#endif //__WINDOWS__  || __LINUX__

#include "fofmodlock.h"


typedef enum EXECTHREAD_FLAG : unsigned int
{
	SUSPEND = 0x00000001,
	TERMINATE = 0x00000002

} FOFMOD_EXECTHREAD_FLAG;

typedef enum EXECTHREAD_EVENT : unsigned int
{
	FINISHED = 0,
	TERMINATED = 1
} FOFMOD_EXECTHREAD_EVENT;

typedef enum EXECTHREAD_RESULT : unsigned int
{	
	RES_FINISH = 0
} FOFMOD_EXECTHREAD_RESULT;

namespace FOFMOD
{
	
	// function signature for thread to execute
	typedef FOFMOD_EXECTHREAD_RESULT (*generic_thread_handler)(void* data);
	
	class ExecutionThread;
	
	class InternalThreadData
	{
		public:
			FOFMOD::AtomicLock locker;
			InternalThreadData();
			~InternalThreadData();
			ExecutionThread* host;
			generic_thread_handler	func;
			unsigned int flags;
	};
	
	class ExecutionThreadData
	{
		protected:
			FOFMOD::AtomicLock locker;
			InternalThreadData* internalThreadData;
			void* userData;
			
		public:
			ExecutionThreadData();
			~ExecutionThreadData();
			InternalThreadData* GetInternalData();
			void* GetUserData();
			void SetInternalData( InternalThreadData* ptr );
			void SetUserData( void* ptr );
	};
	
	
	struct ThreadEventData
	{
		int result;
/* 		union _data
		{
			
		}; */
	};
	typedef ThreadEventData ThreadEventData_t;
	
	
	class ExecutionThread
	{
		protected:
			FOFMOD::AtomicLock locker;
		#ifdef __WINDOWS__
			DWORD sysThreadId;
			HANDLE sysThreadHandle;
		#elif defined ( __LINUX__ ) 
			pthread_t pthreadId;
		#endif // __WINDOWS__ || __LINUX__
			std::string name;
			bool isRunning;
			ExecutionThreadData tdata;
			
		public:
			ExecutionThread();
			~ExecutionThread();
			bool Init();
			bool Start( generic_thread_handler func, void* data );
			void Stop();
			bool IsRunning();
			void SetName( std::string& newName );
			std::string& GetName();
			void OnThreadEvent( int eventType, void* eventData );
			
		#ifdef __WINDOWS__
		static DWORD WINAPI thread_entry_point( void* data );
		#elif defined ( __LINUX__ ) 
		static void* thread_entry_point ( void* data );
		#endif // __WINDOWS__ || __LINUX__ 
	};
	
	extern ThreadEventData_t* ThreadEventData_New();
	extern void ThreadEventData_Init( ThreadEventData_t* ptr );
	extern void ThreadEventData_Delete( ThreadEventData_t* ptr );
	
};

#endif // __FOFMOD_THREAD_H__