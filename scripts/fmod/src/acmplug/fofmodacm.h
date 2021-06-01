#ifndef __FOFMOD_ACM_H__
#define __FOFMOD_ACM_H__

#include "fmod.hpp"
#include "fmod_errors.h"

// Detect operating system
#if defined ( _WIN32 ) || defined ( _WIN64 )
# define __WINDOWS__
#elif defined ( __linux__ )
# define __LINUX__
#else
# error "Unknown operating system."
#endif

// Detect compiler
#if defined ( __GNUC__ )
# define __GCC__
#elif defined ( _MSC_VER ) && !defined ( __MWERKS__ )
# define __MSVC__
#else
# error "Unknown compiler."
#endif


#undef NO_MANGLING
#undef EXPORT
#undef IMPORT


#if defined ( __GCC__ )
	#define NO_MANGLING extern "C"
	#define EXPORT __attribute__(visibility("default"))
	#define IMPORT
#else
#if defined (__MSVC__ )
	#define NO_MANGLING extern "C"
	#define EXPORT __declspec(dllexport)
	#define IMPORT __declspec(dllimport)
#else
#error "FUCK"
#endif

NO_MANGLING EXPORT FMOD_RESULT FMOD_System_RegisterCodec(  FMOD_SYSTEM *system, FMOD_CODEC_DESCRIPTION *description, unsigned int *handle, unsigned int priority );

#endif //__FOFMOD_ACM_H__