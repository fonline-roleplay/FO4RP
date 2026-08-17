#include "scriptarray.h"

#ifdef _WIN32
#include <windows.h>

static FARPROC ResolveScriptArrayFunction(const char* name)
{
	return GetProcAddress(GetModuleHandle(nullptr), name);
}
#else
#include <dlfcn.h>

static void* ResolveScriptArrayFunction(const char* name)
{
	return dlsym(RTLD_DEFAULT, name);
}
#endif

int CScriptArray::GetElementTypeId() const
{
	using Function = int (*)(const CScriptArray*);
	static Function function = reinterpret_cast<Function>(ResolveScriptArrayFunction("ScriptArray_GetElementTypeId"));
	return function(this);
}

asUINT CScriptArray::GetSize() const
{
	using Function = asUINT (*)(const CScriptArray*);
	static Function function = reinterpret_cast<Function>(ResolveScriptArrayFunction("ScriptArray_GetSize"));
	return function(this);
}

void CScriptArray::Resize(asUINT size)
{
	using Function = void (*)(CScriptArray*, asUINT);
	static Function function = reinterpret_cast<Function>(ResolveScriptArrayFunction("ScriptArray_Resize"));
	function(this, size);
}

void CScriptArray::Resize(int delta, asUINT at)
{
	using Function = void (*)(CScriptArray*, int, asUINT);
	static Function function = reinterpret_cast<Function>(ResolveScriptArrayFunction("ScriptArray_ResizeAt"));
	function(this, delta, at);
}

void CScriptArray::InsertAt(asUINT index, void* value)
{
	using Function = void (*)(CScriptArray*, asUINT, void*);
	static Function function = reinterpret_cast<Function>(ResolveScriptArrayFunction("ScriptArray_InsertAt"));
	function(this, index, value);
}

void* CScriptArray::GetBuffer()
{
	using Function = void* (*)(CScriptArray*);
	static Function function = reinterpret_cast<Function>(ResolveScriptArrayFunction("ScriptArray_GetBuffer"));
	return function(this);
}
