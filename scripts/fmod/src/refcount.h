#ifndef __REFCOUNT_H__
#define __REFCOUNT_H__


#include "_defines.fos"
#include "fonline.h"
#include "fofmodlock.h"


class IRefcountable
{
	public:
		virtual void Addref()  				= 0;
		virtual void Release() 				= 0;
		virtual unsigned int GetRefcount()  = 0;
};

class GenericRefcountable : public IRefcountable
{
	protected:
		unsigned int count;

	public:
		GenericRefcountable();
		void Addref() override;
		void Release() override;
		unsigned int GetRefcount() override;
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
		virtual ~AAuxiliaryRefcounter();

		void Addref() override;
		void Release() override;
		unsigned int GetRefcount() override;

		virtual void OnZero() = 0;
};

#endif // __REFCOUNT_H__