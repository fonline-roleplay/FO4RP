#ifndef Q_SPRITES_H
#define Q_SPRITES_H

// Script constants
#define SKIP_PRAGMAS
#include "_defines.fos"

// Disable macro redefinition warning
#pragma warning (push)
#pragma warning (disable : 4005)
#include "fonline.h"
#pragma warning (pop)


#ifdef Q_SPRITES
	void ClearAllTiles();
	void RegisterNativeSprites( asIScriptEngine* engine, bool compiler );
#endif

#ifndef Q_SPRITES
	extern 	void ClearAllTiles();
	extern	void RegisterNativeSprites( asIScriptEngine* engine, bool compiler );
#endif

#endif
