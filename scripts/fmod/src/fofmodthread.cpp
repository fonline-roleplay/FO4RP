#ifndef __FOFMOD_THREAD__
#define __FOFMOD_THREAD__

#include "fofmodthread.h"
#include <cstring>
#include "macros.h"
#include "_defines.fos"
#include "fonline.h"

namespace FOFMOD
{

	InternalThreadData::InternalThreadData()
	{
		this->func = NULL;
		this->flags = 0;
	}
	
	InternalThreadData::~InternalThreadData()
	{
		
	}
	
	ExecutionThreadData::ExecutionThreadData()
	{
		this->internalThreadData = NULL;
		this->userData = NULL;
	}
	
	ExecutionThreadData::~ExecutionThreadData()
	{
		
	}
	
	InternalThreadData* ExecutionThreadData::GetInternalData()
	{
		InternalThreadData* result = NULL;
		this->locker.Lock();
		result = this->internalThreadData;
		this->locker.Unlock();
		return result;
	}

	void* ExecutionThreadData::GetUserData()
	{
		void* result = NULL;
		this->locker.Lock();
		result = this->userData;
		this->locker.Unlock();
		return result;
	}
	
	void ExecutionThreadData::SetInternalData( InternalThreadData* ptr )
	{
		this->locker.Lock();
		this->internalThreadData = ptr;
		this->locker.Unlock();
	}
	
	void ExecutionThreadData::SetUserData( void* ptr )
	{
		this->locker.Lock();
		this->userData = ptr;
		this->locker.Unlock();
	}

	
	ExecutionThread::ExecutionThread()
	{
		#ifdef __WINDOWS__
			this->sysThreadId = -1;
			this->sysThreadHandle = 0;
		#elif defined ( __LINUX__ ) 
			this->pthreadId = -1;
		#endif
		
		this->isRunning = false;
		
		this->tdata.SetInternalData( new InternalThreadData () );
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
			Log("Starting new thread %s\n", this->GetName.c_str());
			InternalThreadData* itd = this->tdata.GetInternalData();
			
			itd->locker.Lock();
			itd->func = func;
			SETFLAG ( itd->flags,  FOFMOD_EXECTHREAD_FLAG::SUSPEND );
			itd->host = this;
			itd->locker.Unlock();
			
			this->tdata.SetUserData( data );
			
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
			
			itd->locker.Lock();
			UNSETFLAG(  itd->flags, FOFMOD_EXECTHREAD_FLAG::SUSPEND );
			itd->locker.Unlock();
		}
		this->isRunning = result;
		return result;
	}
	
	void ExecutionThread::Stop()
	{
		this->locker.Lock();
		if( !this->isStopping )
		{
			InternalThreadData* itd = this->tdata.GetInternalData();
			if( itd )
			{
				UNSETFLAG(  itd->flags, FOFMOD_EXECTHREAD_FLAG::TERMINATE );
			}
			this->isStopping = true;
		}
		this->locker.Unlock();
	}
	
	void ExecutionThread::Finish()
	{
		this->locker.Lock();
		if( this->isRunning )
		{
			#ifdef __WINDOWS__
		
				this->isRunning = false;
				
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
					this->pthreadId = -1;
				}
				
			#endif
			
			this->isFinished = true;

		}
		this->locker.Unlock();
	}
	
	void ExecutionThread::Suspend()
	{
		InternalThreadData* itd = this->tdata.GetInternalData();
		if( itd )
		{
			itd->locker.Lock();
			UNSETFLAG(  itd->flags, FOFMOD_EXECTHREAD_FLAG::SUSPEND );
			itd->locker.Unlock();
		}
		
	}
	
	void ExecutionThread::Join()
	{
		while(true)
		{
			bool isFin = false;
			this->locker.Lock();
			isFin = this->isFinished();
			this->locker.Unlock();
			if(isFin)
				break;
		}
	}
	
	void ExecutionThread::Terminate()
	{
		InternalThreadData* itd = this->tdata.GetInternalData();
		if( itd )
		{
			itd->locker.Lock();
			UNSETFLAG(  itd->flags, FOFMOD_EXECTHREAD_FLAG::TERMINATE );
			itd->locker.Unlock();
		}
	}
	
	bool ExecutionThread::IsRunning()
	{
		bool result = true;
		this->locker.Lock();
		result = this->isRunning;
		this->locker.Unlock();
		return result;
	}
	
	bool ExecutionThread::IsStopping()
	{
		bool result = true;
		this->locker.Lock();
		result = this->isStopping;
		this->locker.Unlock();
		return result;
	}
	
	
	bool ExecutionThread::IsFinished()
	{
		bool result = true;
		this->locker.Lock();
		result=this->isFinished;
		this->locker.Unlock();
		return result;
	}
	
	bool ExecutionThread::IsSuspended()
	{
		bool result = true;
		this->locker.Lock();
		result = this->isSuspended;
		this->locker.Unlock();
		return result;
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
		
		int fresult = 0; //
		
		if( data )
		{
			ExecutionThreadData* tdata = (ExecutionThreadData*)data;
			void* userData = tdata->GetUserData();
			
			
			InternalThreadData* itd = tdata->GetInternalData();
			
			itd->locker.Lock();
			generic_thread_handler func = itd->func;
			ExecutionThread* host = itd->host;
			itd->locker.Unlock();
			
			unsigned int flags = 0;
			bool suspend = true;
			while( suspend )
			{
				itd->locker.Lock();
				flags = itd->flags;
				itd->locker.Unlock();
				suspend = ISFLAG( flags, FOFMOD_EXECTHREAD_FLAG::SUSPEND );
			}
			
			bool work = true;
			if ( func )
			{
				while( work )
				{
					
					itd->locker.Lock();
					flags = itd->flags;
					itd->locker.Unlock();
					
					work = !ISFLAG( flags, FOFMOD_EXECTHREAD_FLAG::TERMINATE );
					
					if(!work)
						break;
					
					fresult = func( userData );
					switch( fresult )
					{
						case( EXECTHREAD_RESULT::RES_FINISH ):
						{
							ThreadEventData* ted = ThreadEventData_New();
							ted->result = FOFMOD_EXECTHREAD_EVENT::FINISHED;
							host->OnThreadEvent( fresult, ted );
							ThreadEventData_Delete( ted );
							work = false;
							break;
						}
						case( EXECTHREAD_RESULT::RES_REPEAT):
						{
							break;
						}
						
						default:
							break;
					}
				}
			}
			

		}
		
		ThreadEventData* ted = ThreadEventData_New();
		ted->result = FOFMOD_EXECTHREAD_EVENT::TERMINATED;
		host->OnThreadEvent( fresult, ted );
		ThreadEventData_Delete( ted );
		return result;
	}
	
	void ExecutionThread::OnThreadEvent( int eventType, void* eventData )
	{
		switch ( eventType )
		{
			case ( FOFMOD_EXECTHREAD_EVENT::FINISHED ):
			{
				this->Finish();
				break;
			}
			case ( FOFMOD_EXECTHREAD_EVENT::TERMINATED ):
			{
				this->Finish();
				break;
			}
			default:
				break;
		}
	}
	
	
	ThreadEventData_t* ThreadEventData_New()
	{
		ThreadEventData* ted = (ThreadEventData*)malloc(sizeof(ThreadEventData));
		ThreadEventData_Init( ted );
		return ted;
	}
	
	void ThreadEventData_Init( ThreadEventData_t* ptr )
	{
		memset( ptr, 0, sizeof( ThreadEventData ) );
	}
	
	void ThreadEventData_Delete( ThreadEventData_t* ptr )
	{
		free(ptr);
	}
	
};
#endif // __FOFMOD_THREAD__