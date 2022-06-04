
// Detect operating system
#if defined ( _WIN32 ) || defined ( _WIN64 )
# define FO_WINDOWS
#elif defined ( __linux__ )
# define FO_LINUX
#else
# error "Unknown operating system."
#endif

// Detect compiler
#if defined ( __GNUC__ )
#ifndef __FONLINE_LIB_H__
#define __FONLINE_LIB_H__

# define FO_GCC
#elif defined ( _MSC_VER ) && !defined ( __MWERKS__ )
# define FO_MSVC
#else
# error "Unknown compiler."
#endif

// Detect CPU
#if ( defined ( FO_MSVC ) && defined ( _M_IX86 ) ) || ( defined ( FO_GCC ) && !defined ( __LP64__ ) )
# define FO_X86
#elif ( defined ( FO_MSVC ) && defined ( _M_X64 ) ) || ( defined ( FO_GCC ) && defined ( __LP64__ ) )
# define FO_X64
#else
# error "Unknown CPU."
#endif

// Detect target
#if defined ( __SERVER )
# define TARGET_NAME                SERVER
#elif defined ( __CLIENT )
# define TARGET_NAME                CLIENT
#elif defined ( __MAPPER )
# define TARGET_NAME                MAPPER
#else
# error __SERVER / __CLIENT / __MAPPER any of this must be defined
#endif

// Platform specific options
#define NDEBUG
#ifdef FO_MSVC
# define _WINDOWS
# define _MBCS
# define _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_DEPRECATE
# define _HAS_ITERATOR_DEBUGGING    0
# define _SECURE_SCL                0
# define _HAS_EXCEPTIONS            0
#endif

#ifdef FO_WINDOWS
# ifdef FO_MSVC
#  define EXPORT                    extern "C" __declspec( dllexport )
#  define EXPORT_UNINITIALIZED      extern "C" __declspec( dllexport ) extern
# else // FO_GCC
#  define EXPORT                    extern "C" __attribute__( ( dllexport ) )
#  define EXPORT_UNINITIALIZED      extern "C" __attribute__( ( dllexport ) ) extern
# endif
#else
# define EXPORT                     extern "C" __attribute__( ( visibility( "default" ) ) )
# define EXPORT_UNINITIALIZED       extern "C" __attribute__( ( visibility( "default" ) ) )
#endif


// AngelScript
#include "AngelScript/angelscript.h"
EXPORT_UNINITIALIZED asIScriptEngine* ASEngine;

// AngelScript add-ons
#define FONLINE_DLL
#include "AngelScript/scriptstring.h"
#include "AngelScript/scriptarray.h"
#include "AngelScript/scriptfile.h"
#include "AngelScript/scriptdictionary.h"
#include "AngelScript/scriptany.h"
#include "AngelScript/scriptmath.h"
#undef FONLINE_DLL


typedef char                                 int8;
typedef unsigned char                        uint8;
typedef short                                int16;
typedef unsigned short                       uint16;
typedef unsigned int                         uint;
#if defined ( FO_MSVC )
typedef unsigned __int64                     uint64;
typedef __int64                              int64;
#elif defined ( FO_GCC )
# include <inttypes.h>
typedef uint64_t                             uint64;
typedef int64_t                              int64;
#endif


// Generic
EXPORT_UNINITIALIZED void              ( * Log )( const char* frmt, ... );
EXPORT_UNINITIALIZED asIScriptContext* ( *ScriptGetActiveContext )( );
EXPORT_UNINITIALIZED const char* (ScriptGetLibraryOptions) ( );
EXPORT_UNINITIALIZED const char* (ScriptGetLibraryVersion) ( );

#define FONLINE_DLL_ENTRY( isCompiler )               \
    GameOptions * FOnline;                            \
    asIScriptEngine* ASEngine;                        \
    EXPORT void TARGET_NAME() {}                      \
    void ( * Log )( const char* frmt, ... );          \
    asIScriptContext* ( *ScriptGetActiveContext )( ); \
    const char* (ScriptGetLibraryOptions) ( );        \
    const char* (ScriptGetLibraryVersion) ( );        \
    EXPORT void DllMainEx( bool isCompiler )
// FONLINE_DLL_ENTRY


#endif // __FONLINE_LIB_H__
