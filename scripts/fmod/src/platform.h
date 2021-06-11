#ifndef __FOFMOD_PLATFORM_H__
#define __FOFMOD_PLATFORM_H__
	// platform specific defs

	#undef __WINDOWS__
	#undef __LINUX__

	// Detect operating system
	#if defined ( _WIN32 ) || defined ( _WIN64 )
		# define __WINDOWS__
		#elif defined ( __linux__ )
		# define __LINUX__
		#else
		# error "Unknown operating system."
	#endif

	#undef __GCC__
	#undef __MSVC__

	// Detect compiler
	#if defined ( __GNUC__ )
		# define __GCC__
		#elif defined ( _MSC_VER ) && !defined ( __MWERKS__ )
		# define __MSVC__
		#else
		# error "Unknown compiler."
	#endif


	#undef NO_MANGLING
	#undef DLLEXPORT
	#undef DLLIMPORT

	#if defined ( __GCC__ )
		#define NO_MANGLING extern "C"
		#define DLLEXPORT __attribute__(visibility("default"))
		#define DLLIMPORT
	#elif defined (__MSVC__ )
		#define NO_MANGLING extern "C"
		#define DLLEXPORT __declspec(dllexport)
		#define DLLIMPORT __declspec(dllimport)
	#else
	#error "FUCK"
	#endif

#endif // __FOFMOD_PLATFORM_H__