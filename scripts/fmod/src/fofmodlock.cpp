#ifndef __FOFMOD_LOCK__
#define __FOFMOD_LOCK__

#include "fofmodlock.h"

namespace FOFMOD
{
	
	AtomicLock::AtomicLock()
	{
		#ifdef __WINDOWS__
			this->ownerId = ALOC_NOOWNER;
			this->sysThreadHandle = ALOC_NOHANDLE;
		#elif defined ( __LINUX__ ) 
			this->ownerId = ALOC_NOOWNER;
		#endif // __WINDOWS__ || __LINUX__
		
	}
	
	AtomicLock::~AtomicLock()
	{
		
	}
	
	void AtomicLock::Lock()
	{
		#ifdef __WINDOWS__
			HANDLE h_curthread = GetCurrentThread();
			DWORD  threadId = GetThreadId( h_curthread );
			DWORD  curOwnerId = INTERLOCKED_EXCHANGE( &(this->ownerId), this->ownerId );
			if( curOwnerId == ALOC_NOOWNER )
			{
				// no owner
				// set owner to current thread
				INTERLOCKED_EXCHANGE( &(this->ownerId), threadId );
				
				if( this->sysThreadHandle != ALOC_NOHANDLE )
				{
					CloseHandle( this->sysThreadHandle );
				}
				
				this->sysThreadHandle = h_curthread;
				return;
			}
			else
			{
				if( curOwnerId == threadId )
				{
					// same thread asking for lock
					CloseHandle( h_curthread );
					return;
				}
				else
				{
					while( true )
					{
						// lock current thread to await unlocking
						curOwnerId = INTERLOCKED_EXCHANGE( &(this->ownerId), this->ownerId ) ;
						if( curOwnerId == ALOC_NOOWNER )
						{
							// its free at last
							// grab it as owner and come back to parent caller
							INTERLOCKED_EXCHANGE( &(this->ownerId), threadId );
							
							if( this->sysThreadHandle != ALOC_NOHANDLE )
							{
								CloseHandle( this->sysThreadHandle );
							}
							
							this->sysThreadHandle = h_curthread;
							break;
						}
					}
				}
			}
		#elif defined ( __LINUX__ ) 
		
		#endif
		
	}
	
	bool AtomicLock::TryLock()
	{
		#ifdef __WINDOWS__
			HANDLE h_curthread = GetCurrentThread();
			DWORD  threadId = GetThreadId( h_curthread );
			DWORD  curOwnerId = INTERLOCKED_EXCHANGE( &(this->ownerId), this->ownerId );
			if( curOwnerId == ALOC_NOOWNER )
			{
				// no owner
				// set owner to current thread
				INTERLOCKED_EXCHANGE( &(this->ownerId), threadId );
				
				if( this->sysThreadHandle != ALOC_NOHANDLE )
				{
					CloseHandle( this->sysThreadHandle );
				}
				
				this->sysThreadHandle =  h_curthread;
				return true;
			}
			else
			{
				if( curOwnerId == threadId )
				{
					// same thread asking for lock
					CloseHandle( h_curthread );
					return true;
				}
				else
				{
					// already owned by someone else, just return false
					CloseHandle( h_curthread );
					return false;
				}
			}
		#elif defined ( __LINUX__ ) 
		
		#endif
		return true;
	}
	
	void AtomicLock::Unlock()
	{
		if( this->IsLocked() )
		{
			#ifdef __WINDOWS__
			HANDLE h_curthread = GetCurrentThread();
			DWORD  threadId = GetThreadId( h_curthread );
			DWORD  curOwnerId = INTERLOCKED_EXCHANGE( &(this->ownerId), this->ownerId );
			if( threadId == curOwnerId )
			{
				// current thread is owner, unlock
				INTERLOCKED_EXCHANGE( &(this->ownerId), ALOC_NOOWNER );
				CloseHandle( this->sysThreadHandle );
				this->sysThreadHandle = ALOC_NOHANDLE;
				CloseHandle( h_curthread );
			}
			#elif defined ( __LINUX__ ) 
			
			#endif
		}
	}
	
	bool AtomicLock::IsLocked()
	{
		bool result = false;
		#ifdef __WINDOWS__
			DWORD  curOwnerId = INTERLOCKED_EXCHANGE( &(this->ownerId), this->ownerId );
			result = (curOwnerId != ALOC_NOOWNER);
		#elif defined ( __LINUX__ ) 
			
		#endif
		return result;
	}

};

#endif // __FOFMOD_LOCK__