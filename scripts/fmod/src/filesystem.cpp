#ifndef __FOFMOD_FILESYSTEM__
#define __FOFMOD_FILESYSTEM__

#include "filesystem.h"
#include "unistd.h"

int filesystem_access( const char* fname )
{
	return access( fname, F_OK );
}


#endif // __FOFMOD_FILESYSTEM__