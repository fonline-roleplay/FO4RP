#ifndef __FOFMOD_ARCHIVE_H__
#define __FOFMOD_ARCHIVE_H__

#include "memory.h"
#include <map>
#include <string>
#include <vector>
#include "stdio.h"

namespace FOFMOD
{

	struct ArchiveMemoryObject
	{
		MemoryObject_t memObj;
		unsigned int uncompressed_size;
		unsigned int index;
	};

	typedef ArchiveMemoryObject ArchiveMemoryObject_t;
	typedef std::map< std::string, ArchiveMemoryObject_t > ArchiveMemoryObjectMap;


	class IArchiveFile
	{
		public:
			virtual void  Open( const char* filename  ) = 0;
			virtual void  Open() = 0;
			virtual void  Touch() = 0;
			virtual void  Close() = 0;
			virtual bool  IsOpened() = 0;
			virtual ArchiveMemoryObject_t* GetSymbol( const char* name ) = 0;
			virtual void* GetContent( const char* name, unsigned int* size ) = 0;
			virtual void* GetContent( ArchiveMemoryObject_t* symbol, unsigned int* size) = 0;
			virtual void  GetSymbolNames( std::vector< std::string >& strVec ) = 0;
	};


	class AArchiveFile : public IArchiveFile
	{	
		protected:
			ArchiveMemoryObjectMap objects;
			FILE* handle;
			char* currentFilePath;
			char* mode;
			bool isOpened;
			void SetName( const char* name );
			void AddContent( const char* name, ArchiveMemoryObject_t& object );
			void DropContent();

		public:
			AArchiveFile();;
			virtual ~AArchiveFile(); // 
			
			void  Open( const char* filename ) override;
			void  Open() override;
			void  Touch() = 0;
			void  Close() override;
			bool  IsOpened() override;
			ArchiveMemoryObject_t*  GetSymbol( const char* name ) override;
			void* GetContent( const char* name, unsigned int* size ) = 0;
			void* GetContent( ArchiveMemoryObject_t* symbol, unsigned int* size) = 0;
			void  GetSymbolNames( std::vector< std::string >& strVec ) override;
	};

};

#endif // __FOFMOD_ARCHIVE_H__