#ifndef __FOFMODLISTENER_H__
#define __FOFMODLISTENER_H__

#include "fofmoddef.h"


namespace FOFMOD
{
	class Listener3D
	{
		public:
			Listener3D();
			~Listener3D();

			FMOD_VECTOR position;
			FMOD_VECTOR velocity;
			FMOD_VECTOR forward;
			FMOD_VECTOR up;
			
	};
};

#endif //__FOFMODLISTENER_H__