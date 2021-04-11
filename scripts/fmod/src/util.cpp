#ifndef __UTIL__
#define __UTIL__

#include "util.h"
#include <cstring>
#include <stdlib.h>

int cstr_ends_with( const char* src, const char* ends )
{
	int ret = 0;
	unsigned int strL = std::strlen( src );
	unsigned int strR = std::strlen( ends );
	const char* pos = strstr( src, ends );
	if( pos )
	{
		if( ( pos - src )  == ( strL - strR ) )
		{
			ret = 1;
		}
	}
	return ret;
}

int  cstr_start_with( const char* src, const char* starts )
{
	int ret = 0;
	const char* pos = strstr( src, starts );
	if( pos )
	{
		if( pos == src )
		{
			ret = 1;
		}
	}
	return ret;
}

char* cstr_copy_heap( const char* src )
{
	char* result = NULL;
	unsigned int srcLen = strlen( src );
	result = (char*) malloc( srcLen + 1 );
	strcpy( result, src );
	return result;
}

void cstr_path_get_ext( const char* src, char* out, unsigned int maxSize )
{
	const char* pos = strrchr( src, '.' );
	if( pos )
	{
		unsigned int len = strlen( pos );
		if( len < maxSize )
		{
			strcpy( out, pos );
		}
	}
}

#endif //__UTIL__