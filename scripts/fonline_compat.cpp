#define SKIP_PRAGMAS
#include "_defines.fos"

#include "fonline.h"

FONLINE_DLL_ENTRY( isCompiler )
{
}

EXPORT ScriptString& new_script_string(const char* s, unsigned int len) {
    ScriptString& str = ScriptString::Create();
    str.assign(s, len);
    return str;
}

EXPORT void release_script_string(ScriptString& str) {
    str.Release();
}

EXPORT void int_vec_push_back(IntVec& vec, int val) {
    vec.push_back(val);
}

struct ASCompat {
    void* new_script_string;
    void* release_script_string;
    void* int_vec_push_back;
} ASCOMPAT;

EXPORT uint GetASCompat() {
    ASCOMPAT.new_script_string = (void*) new_script_string;
    ASCOMPAT.release_script_string = (void*) release_script_string;
    ASCOMPAT.int_vec_push_back = (void*) int_vec_push_back;
    return (uint)(void*) &ASCOMPAT;
}
