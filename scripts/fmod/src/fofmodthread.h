#ifndef __FOFMOD_THREAD_H__
#define __FOFMOD_THREAD_H__

#include "platform.h"
#include <string>

#ifdef __WINDOWS__
	#include "windows.h" 
#elif defined ( __LINUX__ )
	#include <pthread.h>
#endif //__WINDOWS__  || __LINUX__


typedef enum EXECTHREAD_FLAG : unsigned int
{
	SUSPEND = 0x00000001,
	TERMINATE = 0x00000002

} FOFMOD_EXECTHREAD_FLAG;

namespace FOFMOD
{
	
	// function signature for thread to execute
	typedef void (*generic_thread_handler)(void* data);
	
	struct InternalThreadData
	{
		generic_thread_handler	func;
		unsigned int flags;
	};
	typedef InternalThreadData InternalThreadData_t;
	
	struct ExecutionThreadData
	{
		InternalThreadData_t* internalThreadData;
		void* userData;
	};
	typedef ExecutionThreadData ExecutionThreadData_t;
	
	
	class ExecutionThread
	{
		protected:
		#ifdef __WINDOWS__
			DWORD sysThreadId;
			HANDLE sysThreadHandle;
		#elif defined ( __LINUX__ ) 
			pthread_t pthreadId;
		#endif // __WINDOWS__ || __LINUX__
			std::string name;
			bool isRunning;
			ExecutionThreadData_t tdata;
			
		public:
			ExecutionThread();
			~ExecutionThread();
			bool Init();
			bool Start( generic_thread_handler func, void* data );
			void Stop();
			void SetName( std::string& newName );
			std::string& GetName();
			
		#ifdef __WINDOWS__
		static DWORD WINAPI thread_entry_point( void* data );
		#elif defined ( __LINUX__ ) 
		static void* thread_entry_point ( void* data );
		#endif // __WINDOWS__ || __LINUX__ 
	};
	
	extern InternalThreadData_t* InternalThreadData_New();
	extern void InternalThreadData_Delete( InternalThreadData_t* ptr );
	extern void InternalThreadData_Init ( InternalThreadData_t* ptr );
	extern void InternalThreadData_Clear( InternalThreadData_t* ptr );
	
	extern ExecutionThreadData_t* ExecutionThreadData_New();
	extern void ExecutionThreadData_Delete( ExecutionThreadData_t* ptr );
	extern void ExecutionThreadData_Init( ExecutionThreadData_t* ptr );
	extern void ExecutionThreadData_Clear( ExecutionThreadData_t* ptr );
	
};

#endif // __FOFMOD_THREAD_H__