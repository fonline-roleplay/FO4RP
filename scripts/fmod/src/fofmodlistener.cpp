#ifndef __FOFMODLISTENER__
#define __FOFMODLISTENER__

#include "fofmoddef.h"
#include "fofmodlistener.h"
#include <cstring>

namespace FOFMOD
{

	Listener3D::Listener3D()
	{
		memset( this, 0, sizeof( FOFMOD::Listener3D ) );
	}

	Listener3D::~Listener3D()
	{

	}
};

#endif //__FOFMODLISTENER__