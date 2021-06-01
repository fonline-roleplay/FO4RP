#ifndef __FOFMOD_FILE_H__
#define __FOFMOD_FILE_H__

#include <cstring>
#include "stdio.h"
#include "_defines.fos"
#include "fonline.h"


#if defined ( FO_GCC )
	#define _FileOpen( handle, path, mode) handle = fopen( path, mode )
#elif defined ( FO_MSVC )
	#if defined ( FO_WINDOWS )
		#define _FileOpen( handle, path, mode) fopen_s( &handle, path, mode );
	#endif
#endif

#endif //__FOFMOD_FILE_H__