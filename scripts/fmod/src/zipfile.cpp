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

							//FOFMOD_DEBUG_LOG( "Supported file base <%d> offset <%d>size <%d> uncomp size <%d> name <%s> \n", 
							// archive_mem_obj.memObj.base,
							// archive_mem_obj.memObj.offset,
							// archive_mem_obj.memObj.size,
							// archive_mem_obj.uncompressed_size,
							// archive_mem_obj.memObj.name);
						}
						else
						{
							//FOFMOD_DEBUG_LOG( "file is not supported %s \n", file_stat.m_filename );
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
			if( !this->IsOpened() )
			{
				this->Open();
			}

			if( symbol->memObj.size && symbol->uncompressed_size )
			{
				mz_zip_zero_struct( &this->zipFile );
				this->zipStatus =  mz_zip_reader_init_cfile( &this->zipFile, this->handle, 0,  0 );
				result = mz_zip_reader_extract_to_heap( &this->zipFile, symbol->index, size, 0 );
				mz_zip_reader_end( &this->zipFile );
			}
		}
		return result;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// void FOZipFile::Touch()
	// {
	// 	// touch as zipfile then alias music/sound files
	// 	ZipFile::Touch();
	// 	for( ArchiveMemoryObjectMap::iterator cur = this->objects.begin(); cur != this->objects.end(); cur++ )
	// 	{
	// 		ArchiveMemoryObject_t* obj = &(cur->second);
	// 		if( obj )
	// 		{
	// 			if( obj->memObj.name )
	// 			{
	// 				const char* const soundExtensions[] = { ".ogg", ".mp3", ".flac", ".wav" };
	// 				const unsigned int soundExtNum      = 4;
	// 				const char* extension;
	// 				unsigned int extLen;
	// 				cwk_path_get_extension( obj->memObj.name, &extension, &extLen );
	// 				if( extLen && extension )
	// 				{
	// 					unsigned int indexer = 0;
	// 					while( indexer < soundExtNum )
	// 					{
	// 						if( strcmp( extension, soundExtensions[indexer] ) == 0 )
	// 						{
	// 							this->MapSoundFile( obj );
	// 							break;
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }



	// void FOZipFile::MapSoundFile( ArchiveMemoryObject_t* symbol )
	// {
	// 	struct cwk_segment segment;
	// 	if( cwk_path_get_first_segment( symbol->memObj.name, &segment) )
	// 	{
	// 		// has segments
	// 		memset( &segment, 0, sizeof( struct cwk_segment ) );
	// 		if( cwk_path_get_last_segment( symbol->memObj.name, &segment ) )
	// 		{
	// 			// last segment if actual filename in path;
	// 			const char* filename = (char*)malloc( segment.size );
	// 			if( filename )
	// 			{
	// 				memcpy( (void*)filename, segment.begin, segment.size );
	// 				cwk_path_get_previous_segment( &segment );
	// 				bool isMusic = false;
	// 				bool isSound = false;

	// 				if( strcmp( segment.begin, "music") == 0 )
	// 				{
	// 					isMusic = true;
	// 				}
	// 				else
	// 				if ( strcmp( segment.begin, "sfx" ) == 0 )
	// 				{
	// 					isSound = true;
	// 				}

	// 				cwk_path_get_previous_segment( &segment );

	// 				if( strcmp( segment.begin, "sounds") == 0 )
	// 				{
	// 					if( !cwk_path_get_previous_segment( &segment ) ) // there is nothing behind sounds
	// 					{
	// 						// all good
	// 						std::map< std::string, std::string >* mapPtr = NULL;
	// 						if( isMusic )
	// 						{
	// 							mapPtr = &this->musicNames;
	// 						}
	// 						else
	// 						if( isSound )
	// 						{
	// 							mapPtr = &this->soundNames;
	// 						}

	// 						if( mapPtr )
	// 						{
	// 							mapPtr->insert( std::pair< std::string, std::string >( std::string( filename ), std::string( symbol->memObj.name ) ) );
	// 						}
	// 					}
	// 				}

	// 				free( (void*)filename );
	// 				filename = NULL;
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		// the file is located in root dir of the archive
	// 		// do not do anything, fuck this file
	// 	}
	// }

	// void* FOZipFile::GetContent( const char* name, unsigned int* size)
	// {
	// 	// get regular object and try to map into alised name if requested name has no results on search
	// 	void* result = NULL;


	// 	return result;
	// }

	// void* FOZipFile::GetContent( ArchiveMemoryObject_t* symbol, unsigned int* size)
	// {
	// 	void* result = NULL;


	// 	return result;
	// }

}

#endif // __FOFMOD_ZIPFILE__