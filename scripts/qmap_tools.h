#ifndef QMAP_TOOLS_H
#define QMAP_TOOLS_H

// Script constants
#define SKIP_PRAGMAS
#include "_defines.fos"

// Disable macro redefinition warning
#pragma warning (push)
#pragma warning (disable : 4005)
#include "fonline.h"
#pragma warning (pop)

bool DMO64_get( ScriptArray* array, uint tile, uint16& hs, uint16& val, uint16& hexX, uint16& hexY, uint8& layer );
bool DMO64_insertLast( ScriptArray* array, ScriptArray* from_array );
int  uint16ToInt( uint16 val1, uint16 val2 );
uint DMO64_makeHex( uint16 hexX, uint16 hexY, uint8 layer );
bool DMO64_set( ScriptArray* array, uint16 hs, uint16 val, uint hex, bool rewrite );
bool DMO64_add( ScriptArray* array, ScriptArray* from_array, bool rewrite );
uint DMO64_getHashNum( ScriptArray* array, uint hex );

void RegisterQmapTools( asIScriptEngine* engine, bool compiler );

#endif
