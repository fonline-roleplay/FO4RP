#ifndef __FOFMOD_FILESYSTEM__
#define __FOFMOD_FILESYSTEM__

#include "filesystem.h"

int filesystem_access( const char* fname )
{
	return access( fname, F_OK );
}


#endif // __FOFMOD_FILESYSTEM__