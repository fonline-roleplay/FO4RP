#ifndef __FOFMOD_ZIPFILE_H__
#define __FOFMOD_ZIPFILE_H__

#include "miniz.h"
#include "memory.h"
#include "stddef.h"
#include "archive.h"
#include <map>


namespace FOFMOD
{

	class ZipFile : public AArchiveFile
	{	
		protected:
			mz_bool zipStatus;
			mz_zip_archive zipFile;
			unsigned int zipFileSize;

		public:
			ZipFile();
			~ZipFile();

		void  Open( const char* filename ) override;
		void  Open() override;
		void  Touch() override;
		void  Close() override;
		void* GetContent( const char* name, unsigned int* size) override;
		void* GetContent( ArchiveMemoryObject_t* symbol, unsigned int* size) override;

	};
	
};
#endif // __ZIPFILE_H__