#ifndef __ZIPFILE_H__
#define __ZIPFILE_H__

#include "miniz.h"
#include "memory.h"
#include "vector"
#include "archive.h"

class ZipFile : public AArchiveFile
{

	public:
		ZipFile();
		~ZipFile();

}

#endif // __ZIPFILE_H__