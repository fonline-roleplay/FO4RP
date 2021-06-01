#ifndef __FOFMOD_MEMORY_H__
#define __FOFMOD_MEMORY_H__

#include "cstring"

struct MemoryObject
{
	unsigned int base;
	unsigned int offset;
	unsigned int size;
	char* name;

};


struct DataPtr
{
	void* ptr;
	unsigned int size;
};

typedef MemoryObject MemoryObject_t;
typedef DataPtr DataPtr_t;


#endif // __FOFMOD_MEMORY_H__