#include "scriptarray.h"

extern "C"
{
    __declspec(dllimport) int ScriptArray_GetElementTypeId(const CScriptArray* array);
    __declspec(dllimport) asUINT ScriptArray_GetSize(const CScriptArray* array);
    __declspec(dllimport) void ScriptArray_Resize(CScriptArray* array, asUINT size);
    __declspec(dllimport) void ScriptArray_ResizeAt(CScriptArray* array, int delta, asUINT at);
    __declspec(dllimport) void ScriptArray_InsertAt(CScriptArray* array, asUINT index, void* value);
    __declspec(dllimport) void* ScriptArray_GetBuffer(CScriptArray* array);
}

int CScriptArray::GetElementTypeId() const
{
    return ScriptArray_GetElementTypeId(this);
}

asUINT CScriptArray::GetSize() const
{
    return ScriptArray_GetSize(this);
}

void CScriptArray::Resize(asUINT size)
{
    ScriptArray_Resize(this, size);
}

void CScriptArray::Resize(int delta, asUINT at)
{
    ScriptArray_ResizeAt(this, delta, at);
}

void CScriptArray::InsertAt(asUINT index, void* value)
{
    ScriptArray_InsertAt(this, index, value);
}

void* CScriptArray::GetBuffer()
{
    return ScriptArray_GetBuffer(this);
}
