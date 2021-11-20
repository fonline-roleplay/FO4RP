#ifndef __FONLINE_TNF__
#define __FONLINE_TNF__

#define __PIZDEC // oy vey !
// Script constants
#define SKIP_PRAGMAS
#include "_defines.fos"

// Disable macro redefinition warning
#pragma warning (push)
#pragma warning (disable : 4005)
#include "fonline.h"
#pragma warning (pop)

// Script global variables
struct _GlobalVars
{
    int*  CurX;
    int*  CurY;
    uint* HitAimEyes;
    uint* HitAimHead;
    uint* HitAimGroin;
    uint* HitAimTorso;
    uint* HitAimArms;
    uint* HitAimLegs;
	uint* __AllowRealDayTime;
} extern GlobalVars;

// #ifdef __CLIENT
// struct _ClientGlobals
// {
	// uint8* __CurrentAim;
	////pad +3 bytes
// } ClientGlobals;
// #endif

#ifdef __PIZDEC

#ifdef MAIN

	#include "qmap_tools.h"
	
	#ifdef __CLIENT
		#pragma comment(lib, "User32.lib")
		#include "q_sprites.h"
	#endif
	
#endif

#endif

#endif // __FONLINE_TNF__
