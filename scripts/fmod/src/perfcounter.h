
#ifndef __FOFMOD__PERFORMANCE_COUNTER_H__
#define __FOFMOD__PERFORMANCE_COUNTER_H__

#undef __PERFCOUNT__

#include "_defines.fos"
#include "fonline.h"

#if defined ( FO_WINDOWS )
	#define __PERFCOUNT__
	/*======================== Performance Counter ==================*/
	#include <Windows.h>
	#include <stdio.h>
#endif // FO_WINDOWS

namespace FOFMOD
{

#if defined ( FO_WINDOWS )

	class Perfcounter
	{
		protected:
			double PCFreq;
			__int64 counterStart;
		public:
			Perfcounter();
			~Perfcounter();

			void Start();
			double Get();
	};

#endif // FO_WINDOWS

}

#endif // __FOFMOD__PERFORMANCE_COUNTER_H__