
#ifndef __FOFMOD__PERFORMANCE_COUNTER__
#define __FOFMOD__PERFORMANCE_COUNTER__

#include "perfcounter.h"

namespace FOFMOD
{

#if defined ( FO_WINDOWS )


	Perfcounter::Perfcounter()
	{
		this->PCFreq= 0.0;
		this->counterStart = 0;
	}

	Perfcounter::~Perfcounter()
	{

	}

	void Perfcounter::Start()
	{
		LARGE_INTEGER li;
	    QueryPerformanceFrequency(&li);

	    this->PCFreq = double(li.QuadPart)/1000.0;

	    QueryPerformanceCounter(&li);
	    this->counterStart = li.QuadPart;
	}

	double Perfcounter::Get()
	{
		LARGE_INTEGER li;
	    QueryPerformanceCounter(&li);
	    return double(li.QuadPart-counterStart)/PCFreq;
	}

#endif // FO_WINDOWS

}

#endif // __FOFMOD__PERFORMANCE_COUNTER__