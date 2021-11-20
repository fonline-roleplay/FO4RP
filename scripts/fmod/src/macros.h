#ifndef __FOFMOD_MACROS_H__
#define __FOFMOD_MACROS_H__


#define ISFLAG(x, flag)     ( (x & flag) != 0 )
#define ISFLAGS(x, flags)   ( (x & flags) == flags )
#define SETFLAG(x,flag)     ( x |= flag )
#define UNSETFLAG(x, flag ) ( x &= (~flag) )

#define MIN(x, min) ( x < min ? min : x )
#define MAX(x, max) ( x > max ? max : x )
#define CLAMPA(x, min, max) ( MIN(MAX(x, max ), min ) )

#define MALLOC_NEW(type, name)  type* name = (type*)malloc(sizeof(type)); \
							 if( name ) \
							 { 				\
								memset( name, 0, sizeof(type) ); \
							 }
							 
#endif //__FOFMOD_MACROS_H__