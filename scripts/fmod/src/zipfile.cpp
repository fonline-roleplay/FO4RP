#ifndef __FOFMOD_ZIPFILE__
#define __FOFMOD_ZIPFILE__

#include "zipfile.h"
#include "vector"
#include "cstring"
#include "stdio.h"
#include "file.h"
#include "fofmoddef.h"
#include "fofmodsystem.h"
#include "util.h"


namespace FOFMOD
{

	ZipFile::ZipFile()
	{

	}

	ZipFile::~ZipFile()
	{
		this->Close();
	}

	void ZipFile::Touch()
	{
		//FOFMOD_DEBUG_LOG( "Trying to touch %s \n", this->currentFilePath );
		if( this->IsOpened() )
		{
			//FOFMOD_DEBUG_LOG( "File is opened, continue touch.. \n" );
			this->DropContent();

			/// cache current position and go back to start of the file
			int cur = ftell( this->handle );
			rewind( this->handle );

			mz_zip_zero_struct( &this->zipFile );
			this->zipStatus =  mz_zip_reader_init_cfile( &this->zipFile, this->handle, 0,  0 );
			mz_uint count =  mz_zip_reader_get_num_files( &this->zipFile );
			//FOFMOD_DEBUG_LOG( "Files count <%d> in archive, initcfile result <%d>  last error <%s> \n", count, this->zipStatus, mz_zip_get_error_string ( this->zipFile.m_last_error ) );
			for( unsigned int i = 0; i < count; i ++ )
			{
				//FOFMOD_DEBUG_LOG( "Processing file num <%d> in archive... \n", i );
				mz_zip_archive_file_stat file_stat;
				if ( !mz_zip_reader_file_stat( &this->zipFile, i, &file_stat ) )
				{
					//FOFMOD_DEBUG_LOG( "mz_zip_reader_file_stat() failed!\n" );
					continue;
				}
				else
				{
					if( mz_zip_reader_is_file_a_directory( &this->zipFile, i ) )
					{
						//FOFMOD_DEBUG_LOG( "file is a directory %s \n", file_stat.m_filename );
						continue;
					}
					else
					{
						if( mz_zip_reader_is_file_supported ( &this->zipFile, i ) )
						{
							ArchiveMemoryObject_t archive_mem_obj;
							memset( &archive_mem_obj, 0, sizeof( ArchiveMemoryObject_t ) );
							archive_mem_obj.memObj.base   			  		= file_stat.m_central_dir_ofs;
							archive_mem_obj.memObj.offset    			  	= file_stat.m_local_header_ofs;
							archive_mem_obj.memObj.size      		  		= file_stat.m_comp_size;
							archive_mem_obj.uncompressed_size 				= file_stat.m_uncomp_size;
							archive_mem_obj.index							= file_stat.m_file_index;
							archive_mem_obj.memObj.name    			  		= (char*) malloc( MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE );
							strcpy( archive_mem_obj.memObj.name, (const char*) &file_stat.m_filename );
							this->AddContent( archive_mem_obj.memObj.name, archive_mem_obj );

						}
						else
						{
							continue;
						}
					}
				}
			}
				
			// return back to original position;
			fseek( this->handle, cur, SEEK_SET );
			mz_zip_reader_end( &this->zipFile );
		}
	}

	void ZipFile::Open( const char* filename )
	{
		//FOFMOD_DEBUG_LOG("Zipfile Open filename \n");
		AArchiveFile::Open( filename );

		if( this->IsOpened() )
		{
			mz_zip_zero_struct( &this->zipFile );
			this->zipStatus =  mz_zip_reader_init_cfile( &this->zipFile, this->handle, 0,  0 );
		}
	}

	void ZipFile::Open()
	{
		//FOFMOD_DEBUG_LOG("Zipfile Open \n");
		AArchiveFile::Open();

		if( this->IsOpened() )
		{
			mz_zip_zero_struct( &this->zipFile );
			this->zipStatus =  mz_zip_reader_init_cfile( &this->zipFile, this->handle, 0,  0 );
		}
	}

	void* ZipFile::GetContent( const char* name, unsigned int* size )
	{
		void* result = NULL;
		ArchiveMemoryObject_t* existing = this->GetSymbol( name );
		if( existing )
		{
			result = this->GetContent( existing, size );
		}

		return result;
	}

	void* ZipFile::GetContent( ArchiveMemoryObject_t* symbol, unsigned int* size)
	{

		void* result = NULL;
		if( symbol )
		{
			if( symbol->memObj.size && symbol->uncompressed_size )
			{
				if( !this->IsOpened() )
				{
					this->Open();

					// couldnt open
					if( !this->IsOpened() )
						return result;
				}

				unsigned int cur = ftell( this->handle );
				rewind( this->handle );;

				result = mz_zip_reader_extract_to_heap( &this->zipFile, symbol->index, size, 0 );

				fseek( this->handle, cur, SEEK_SET );
			}

		}
		return result;
	}


	void  ZipFile::Close()
	{
		if( this->IsOpened() )
		{
			mz_zip_reader_end( &this->zipFile );
			this->zipStatus = 0; 
		}

		AArchiveFile::Close();
	}

}

#endif // __FOFMOD_ZIPFILE__