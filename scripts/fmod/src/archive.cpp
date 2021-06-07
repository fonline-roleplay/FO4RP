#ifndef __FOFMOD_ARCHIVE__
#define __FOFMOD_ARCHIVE__


#include "archive.h"
#include "cstring"
#include "stddef.h"
#include "file.h"
#include "fofmoddef.h"
#include "util.h"

namespace FOFMOD
{

	AArchiveFile::AArchiveFile()
	{
		this->objects.clear();
		this->handle 			= NULL;
		this->currentFilePath 	= NULL;
		this->mode				= NULL;
		this->isOpened 		 	= false;

	}

	AArchiveFile::~AArchiveFile()
	{
		if( this->handle )
		{
			this->Close();
			this->handle = NULL;
		}

		if( this->currentFilePath )
		{
			free( this->currentFilePath );
			this->currentFilePath = NULL;
		}

		this->DropContent();
		this->isOpened = false;
	}


	void AArchiveFile::AddContent( const char* name, ArchiveMemoryObject_t& object )
	{
		if( name )
		{
			this->objects.insert( std::pair< std::string, ArchiveMemoryObject_t >( std::string( name ), object ) );
		}
	}


	void AArchiveFile::SetName( const char* name )
	{
		if( name )
		{
			char* fnameCpy = cstr_copy_heap( name );
			if( fnameCpy )
			{
				if( this->currentFilePath )
				{
					free( this->currentFilePath );
					this->DropContent();
				}
				this->currentFilePath = fnameCpy;
			}
		}
	}

	void AArchiveFile::Open( const char* filename )
	{
		FOFMOD_DEBUG_LOG("Opening Archive file %s \n", filename );
		if( this->IsOpened() )
		{
			FOFMOD_DEBUG_LOG("Its opened already. \n", filename );
			// already opened same file
			if( strstr( filename, this->currentFilePath ) == 0 )
			{
				this->Close();
				this->Open();
			}
			else
			{
				// different file
				this->Close();
				this->SetName( filename );
				this->Open();
			}
		}
		else
		{
			FOFMOD_DEBUG_LOG("Its not opened already. \n", filename );

			if( !this->currentFilePath )
			{
				this->SetName( filename );
			}

			if( strstr( filename, this->currentFilePath ) == 0 )
			{
				FOFMOD_DEBUG_LOG("Its same file. \n", filename );
				this->Open();
			}
			else
			{
				FOFMOD_DEBUG_LOG("Its not same file. \n", filename );
				this->SetName( filename );
				this->Open();
			}

		}
	}

	void AArchiveFile::Open()
	{
		if( this->currentFilePath )
		{
			if( !this->IsOpened() )
			{
				_FileOpen( handle, this->currentFilePath, "rb" );
				if( this->handle )
				{
					FOFMOD_DEBUG_LOG("File handle got \n" );
					this->isOpened = true;
				}
			}
			else
			{
				this->Close();
				this->Open();
			}
		}
	}
	
	const char* AArchiveFile::GetCurrentFilename()
	{
		return this->currentFilePath;
	}


	void AArchiveFile::Close( )
	{
		if( this->IsOpened() )
		{
			int result = fclose( this->handle );
			if( result == 0 )
			{
				this->handle = NULL;
				this->isOpened = false;
			}
		}
	}

	void AArchiveFile::DropContent()
	{

		for( ArchiveMemoryObjectMap::iterator bi = this->objects.begin(); bi != this->objects.end(); bi++ )
		{
			ArchiveMemoryObject_t* cur = &bi->second;
			if( cur->memObj.name )
			{
				free( cur->memObj.name );
			}
		}

		this->objects.clear();
	}

	bool AArchiveFile::IsOpened()
	{
		return ( this->isOpened && ( this->handle != NULL ) );
	}


	ArchiveMemoryObject* AArchiveFile::GetSymbol( const char* name )
	{
		ArchiveMemoryObject* result = NULL;
		ArchiveMemoryObjectMap::iterator found = this->objects.find( name );
		if( found != this->objects.end () )
		{
			result = &found->second;
		}
 
		return result;
	}

	void AArchiveFile::GetSymbolNames( std::vector< std::string >& strVec )
	{
		
		unsigned int objCount = this->objects.size();
		if( objCount > 0 )
		{
			unsigned int frontIndex = strVec.size();
			strVec.resize( this->objects.size() + frontIndex );
			for( ArchiveMemoryObjectMap::iterator cur = this->objects.begin(); cur != this->objects.end(); cur++ )
			{
				ArchiveMemoryObject_t* obj = &(cur->second);
				if( obj )
				{
					if( obj->memObj.name )
					{
						//FOFMOD_DEBUG_LOG("index %d \n", frontIndex);
						strVec[frontIndex] = obj->memObj.name;
						//FOFMOD_DEBUG_LOG("KURWA %s \n", strVec[frontIndex]);
					}
				}
				frontIndex++;
			}
		}
	}
}



#endif // __FOFMOD_ARCHIVE__