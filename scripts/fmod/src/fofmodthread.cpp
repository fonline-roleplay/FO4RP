#ifndef __FOFMOD_THREAD__
#define __FOFMOD_THREAD__

#include "fofmodthread.h"
#include <cstring>
#include "macros.h"

namespace FOFMOD
{

	ExecutionThread::ExecutionThread()
	{
		#ifdef __WINDOWS__
			this->sysThreadId = -1;
			this->sysThreadHandle = 0;
		#elif defined ( __LINUX__ ) 
			this->pthreadId = -1;
		#endif
		
		this->isRunning = false;
		
		ExecutionThreadData_Init( &(this->tdata) );
		this->tdata.internalThreadData = InternalThreadData_New();
	}
	
	ExecutionThread::~ExecutionThread()
	{
		this->Stop();
	}
	
	bool ExecutionThread::Init()
	{
		bool result = true;
		
		return result;
	}
	
	bool ExecutionThread::Start( generic_thread_handler func, void* data )
	{
		bool result = true;
		this->Stop();
		if( func )
		{
			this->tdata.internalThreadData->func = func;
			this->tdata.userData = data;
			SETFLAG ( this->tdata.internalThreadData->flags, FOFMOD_EXECTHREAD_FLAG::SUSPEND );
			
			#ifdef __WINDOWS__

				this->sysThreadHandle = 
				CreateThread
				(
					NULL,                  							// default security attributes
					0,                      						// use default stack size  
					ExecutionThread::thread_entry_point,       		// thread function name
					&(this->tdata),         						// argument to thread function 
					CREATE_SUSPENDED,       						// use default creation flags 
					&(this->sysThreadId) 							// returns the thread identifier 
				);
				
				// do some internal job before resuming
				
				ResumeThread( this->sysThreadHandle );
			
			#elif defined ( __LINUX__ ) 
			
				// todo ?
				pthread_create
				( &(this->pthreadId), 
					   NULL, 
					   ExecutionThread::thread_entry_point, 
					   &(this->tdata)
				)
				
			#endif
			
			UNSETFLAG(  this->tdata.internalThreadData->flags, FOFMOD_EXECTHREAD_FLAG::SUSPEND );
		}
		this->isRunning = result;
		return result;
	}
	
	void ExecutionThread::Stop()
	{
		if( this->isRunning )
		{
			#ifdef __WINDOWS__
		
				if( this->sysThreadId )
				{
					this->sysThreadId = -1;
				}
				
				if( this->sysThreadHandle )
				{
					CloseHandle( this->sysThreadHandle );
				}
		
			#elif defined ( __LINUX__ ) 
			
				if( this->pthreadId )
				{
					pthread_cancel(pthreadId);
					this->pthreadId = -1;
				}
				
			#endif
			
			this->isRunning = false;
			
		//	free( this->tdata.internalThreadData );
		//	memset( &(this->tdata), 0, sizeof( ExecutionThreadData_t ) );
			ExecutionThreadData_Clear( &(this->tdata) );
		}	
	}
	
	void ExecutionThread::SetName( std::string& newName )
	{
		this->name = newName;
	}
	
	std::string& ExecutionThread::GetName()
	{
		return this->name;
	}
	
	
	#ifdef __WINDOWS__
	DWORD WINAPI ExecutionThread::thread_entry_point( void* data )
	#elif defined ( __LINUX__ ) 
	void* ExecutionThread::thread_entry_point ( void* data )
	#endif // __WINDOWS__ || __LINUX__
	{
		#ifdef __WINDOWS__
		DWORD result = 0;
		#elif defined ( __LINUX__ )
		void* result = NULL;
		#endif // __WINDOWS__ || __LINUX__
		
		if( data )
		{
			ExecutionThreadData* tdata = (ExecutionThreadData*)data;
			bool work = true;
			while( work )
			{
				// do work
				if( tdata->internalThreadData->func )
				{
					generic_thread_handler func = tdata->internalThreadData->func;
					func(tdata->userData);
				}
				
				// get data lock, lock it, get values, unlock it
				work = !ISFLAG( tdata->internalThreadData->flags, FOFMOD_EXECTHREAD_FLAG::TERMINATE );
			}
		}
		
		return result;
	} 
	
	
	InternalThreadData_t* InternalThreadData_New()
	{
		MALLOC_NEW( InternalThreadData_t, result )
		return result;
	}
	
	void InternalThreadData_Delete( InternalThreadData_t* ptr )
	{

	}
	
	void InternalThreadData_Init ( InternalThreadData_t* ptr )
	{
		InternalThreadData_Clear( ptr );
	}
	
	void InternalThreadData_Clear( InternalThreadData_t* ptr )
	{

	}
	
	ExecutionThreadData_t* ExecutionThreadData_New()
	{
		MALLOC_NEW( ExecutionThreadData_t, result )
		return result;
	}
	
	void ExecutionThreadData_Delete( ExecutionThreadData_t* ptr )
	{

	}
	
	void ExecutionThreadData_Init( ExecutionThreadData_t* ptr )
	{

		ExecutionThreadData_Clear( ptr );
	}
	
	void ExecutionThreadData_Clear( ExecutionThreadData_t* ptr )
	{

	}
	


};
#endif // __FOFMOD_THREAD__