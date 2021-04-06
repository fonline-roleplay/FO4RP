#ifndef __REFCOUNT_H__
#define __REFCOUNT_H__


#include "_defines.fos"
#include "fonline.h"

#undef INTERLOCKED_INCREMENT	
#undef INTERLOCKED_DECREMENT

#if defined ( FO_GCC )
	#define INTERLOCKED_INCREMENT __sync_add_and_fetch
	#define INTERLOCKED_DECREMENT __sync_sub_and_fetch
#elif defined ( FO_MSVC )
	#if defined ( FO_WINDOWS )
		#include "Windows.h"
		#define INTERLOCKED_INCREMENT InterlockedIncrement
		#define INTERLOCKED_DECREMENT InterlockedDecrement
	#elif 
		#error "FO_MSVC and not FO_WINDOWS, Can't define INTERLOCKED." // FO_MSVC is compiler determined, and MSVC is not compiling for non windows ?
	#endif
#endif

class IRefcountable
{
	public:
		virtual void Addref()  				= 0;
		virtual void Release() 				= 0;
};

class GenericRefcountable : public IRefcountable
{
	protected:
		unsigned int count;

	public:
		GenericRefcountable();
		void Addref() override;
		void Release() override;
};

template < typename T >
class AAuxiliaryRefcounter : IRefcountable
{
	protected:
		AAuxiliaryRefcounter();
		unsigned int refcount;
		T* subject;

	public:
		AAuxiliaryRefcounter(T* subject);
		~AAuxiliaryRefcounter();

		void Addref() override;
		void Release() override;

		void OnZero() = 0;
};

#endif // __REFCOUNT_H__