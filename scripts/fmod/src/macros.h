#ifndef __FOFMOD_MACROS_H__
#define __FOFMOD_MACROS_H__


#define ISFLAG(x, flag)     ( (x & flag) != 0 )
#define ISFLAGS(x, flags)   ( (x & flags) == flags )
#define SETFLAG(x,flag)     ( x |= flag )
#define UNSETFLAG(x, flag ) ( x &= (~flag) )


#endif //__FOFMOD_MACROS_H__