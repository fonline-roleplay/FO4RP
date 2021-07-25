#ifndef __FOFMODSYSTEM__
#define __FOFMODSYSTEM__


#include "fofmodsystem.h"
#include "_defines.fos"
#include "fonline.h"
#include "tinydir.h"
#include "miniz.h"
#include "util.h"
#include "stdio.h"
#include "perfcounter.h"
#include <cstring>
#include "cwalk.h"
#include "macros.h"
#include "fofmoddsp.h"

// stdbool.h fuckery
#undef bool 
#undef true
#undef false

namespace FOFMOD
{	
	const int fileExtNum = 4;
	const std::string fileExtensions[fileExtNum] = { std::string(".ogg"), std::string(".flac"), std::string(".wav"), std::string(".mp3") };
	const char* PathToSounds = "./data/sound/sfx/";
	const char* PathToMusic  = "./data/sound/music/";
	const char  PathSeparator = '/'; // should be platform specific, and not here

	bool IsValidSoundExtension( const char* filename )
	{
		bool result = false;
		if( filename )
		{
			for( int i = 0; i < fileExtNum; i ++ )
			{
				if( cstr_ends_with( filename, fileExtensions[i].c_str() ) )
				{
					result = true;
					break;
				}
			}	
		}
		return result;
	}

	System::IndexedArchiveFile::IndexedArchiveFile() {} // ban

	System::IndexedArchiveFile::IndexedArchiveFile(FOFMOD::IArchiveFile* arch )
	{
		this->arch = arch;
		this->isMapped = false;
	}

	System::IndexedArchiveFile::~IndexedArchiveFile()
	{
		if( this->arch )
		{
			delete this->arch;
			this->arch = NULL;
		}
	}

	System::ChannelCallbackData::ChannelCallbackData()
	{
		this->system    = NULL;
		this->channel   = NULL;
	}

	System::ChannelCallbackData::~ChannelCallbackData()
	{
		
	}

	// fmod supports single callback for each event, event should be determined inside the callback itself
	FMOD_RESULT F_CALLBACK System::ChannelCallback( 	FMOD_CHANNELCONTROL *channelcontrol, 
														FMOD_CHANNELCONTROL_TYPE controlType, 
														FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, 
														void* commanddata1, 
														void* commanddata2 )
	{

		FOFMOD_DEBUG_LOG("Channel callback \n");
		if( callbacktype  == FMOD_CHANNELCONTROL_CALLBACK_TYPE::FMOD_CHANNELCONTROL_CALLBACK_END )
		{
			FOFMOD_DEBUG_LOG("Channel sound end callback \n");
			FMOD::Channel* channel = (FMOD::Channel*)channelcontrol;
			FOFMOD::System::ChannelCallbackData* cbdata = NULL; 
			channel->getUserData( (void**)&cbdata );
			if( cbdata )
			{
				
				FOFMOD::Channel* chn = cbdata->channel;
				cbdata->system->OnChannelEnd( chn );
				chn->Invalidate();
				chn->Release();
				
				delete cbdata;
				FOFMOD_DEBUG_LOG("Channel <%u> play end \n", channel);
			}
		}
		
		return FMOD_OK;
	}

	System::System()
	{
		this->initialized = false;
		this->FSystem = NULL;
		this->soundChannelGroup = NULL;
		this->musicChannelGroup = NULL;
		this->indexedArchives.clear();
		this->soundNames.clear();
		this->musicNames.clear();
		this->cachedDSPEffects.clear();
	}

	System::~System()
	{
		if( this->soundChannelGroup )
		{
			this->soundChannelGroup = NULL;
		}

		if( this->musicChannelGroup )
		{
			this->musicChannelGroup = NULL;
		}

		if( this->FSystem )
		{
			this->FSystem->release();
			this->FSystem = NULL;
		}

		this->indexedArchives.clear();
		this->soundNames.clear();
		this->musicNames.clear();
		this->initialized = false;
	}

	void System::Update()
	{
		this->FSystem->update();
	}

	FMOD_RESULT System::Initialize( unsigned int channelCount )
	{
		FMOD_RESULT result;
		if( !this->IsInit() )
		{
			result = FMOD::System_Create(&this->FSystem);
			if (result != FMOD_OK)
			{
			    Log("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			}
			else
			{
				unsigned int version;
				result = this->FSystem->getVersion(&version);

			    if ( version < FMOD_VERSION )
			    {
			        Log("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
					return FMOD_ERR_VERSION;
			    }
			    else
			    {
					#ifdef FOFMOD_PROFILING
						#define INIT_FLAGS ( FMOD_INIT_PROFILE_ENABLE | FMOD_INIT_3D_RIGHTHANDED )
					#else
						#define INIT_FLAGS ( FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED )
					#endif
			    	// continue initialize
			    	FOFMOD_DEBUG_LOG("FMOD version %u \n", version);
					result = this->FSystem->init( channelCount, INIT_FLAGS, 0 );
					if (result != FMOD_OK)
					{
					    Log("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
					}
					else
					{
						// continue initialize
						// channel groups
						result = this->FSystem->createChannelGroup( "sounds", &this->soundChannelGroup );
						if( result == FMOD_OK )
						{
							result = this->FSystem->createChannelGroup( "music", &this->musicChannelGroup );

							if( result == FMOD_OK )
							{
								#ifdef FOFMOD_DEBUG
								FOFMOD_DEBUG_LOG("Codec plugins listing... \n");
								int pluginsCount = 0;
								result = this->FSystem->getNumPlugins( FMOD_PLUGINTYPE_CODEC, &pluginsCount );

								for( int i = 0; i < pluginsCount; i ++ )
								{
									unsigned int pluginHandle = 0;
									FMOD_RESULT plugResult = this->FSystem->getPluginHandle ( FMOD_PLUGINTYPE_CODEC, i, &pluginHandle );
									if( plugResult = FMOD_OK )
									{
										const int nameLen = 1024;
										char* pluginName = new char[nameLen];
										unsigned int version = 0;
										FMOD_PLUGINTYPE ptype;
										plugResult = this->FSystem->getPluginInfo( pluginHandle, &ptype, pluginName, nameLen, &version );
										if( plugResult )
										{
											FOFMOD_DEBUG_LOG("Codec plugin loaded %s %d \n", pluginName, version );
										}
										delete pluginName;
									}
								}

								#endif // FOFMOD_DEBUG
							}
						}
					}
			    }
			}
		}
		else
			result = FMOD_OK;
		
		if( result == FMOD_OK )
			this->initialized = true;

		return result;
	}

	bool System::TouchArchive( const std::string& filename )
	{
		FOFMOD_DEBUG_LOG("Touch archive file at path <%s>. \n", filename.c_str() );
		bool result = false;
		
		if( this->IsArchiveTouched( filename ) )
		{
			FOFMOD_DEBUG_LOG("Already touched <%s>. \n", filename.c_str() );
			return true;
		}

		char ext[12];
		cstr_path_get_ext( filename.c_str(), (char*)&ext, 12 );
		_strlwr( (char*)&ext );
		if( strcmp( ext, ".zip" ) == 0 )
		{
			FOFMOD::ZipFile* zipFile = new FOFMOD::ZipFile();
			zipFile->Open( filename.c_str() );
			if( zipFile->IsOpened() )
			{
				// file exist and opened
				zipFile->Touch();
				zipFile->Close();
				FOFMOD::System::IndexedArchiveFile* indexedArch = new FOFMOD::System::IndexedArchiveFile( zipFile );
				this->indexedArchives.push_back( indexedArch );
				this->MapArchive( this->indexedArchives.size() - 1 );
				result = true;
			}
			else
			{
				FOFMOD_DEBUG_LOG("Touch archive file at path <%s> failed. \n", filename.c_str() );
				delete zipFile;
			}
		}

		return result;
	}
	
	bool System::IsArchiveTouched( const std::string& filename )
	{
		for( IndexedArchiveFilePtrVec::iterator it = this->indexedArchives.begin(); it != this->indexedArchives.end(); it++ )
		{
			IndexedArchiveFile* iaf = *it;
			if( iaf )
			{
				FOFMOD::IArchiveFile* arch = iaf->arch;
				const char* archFilename = arch->GetCurrentFilename();
				if( archFilename )
				{
					if( filename == archFilename ) 
					{
						return true;
					}
				}
				else
					return false;
			}
		}
		return false;
	}
	
	// TODO ?
	void System::UntouchArchive( const std::string& filename )
	{
		if( this->IsArchiveTouched( filename ) )
		{
			for( IndexedArchiveFilePtrVec::iterator it = this->indexedArchives.begin(); it != this->indexedArchives.end(); it++ )
			{
				IndexedArchiveFile* iaf = *it;
				if( iaf )
				{
					FOFMOD::IArchiveFile* arch = iaf->arch;
					const char* archFilename = arch->GetCurrentFilename();
					if( filename == archFilename )
					{
						this->indexedArchives.erase( it );
						break;
					}
				}
			}
		}
	}

	void System::DeleteCachedSound( CachedDataMap::iterator iterator )
	{
		// mother fucker
		this->cachedSoundsData.erase( iterator );
	}


	void System::Play( const std::string& soundName, FOFMOD_SOUND_TYPE type, FMOD::ChannelGroup* group, FOFMOD::Channel** chn, bool paused )
	{
		#ifdef FOFMOD_DEBUG
			#ifdef __PERFCOUNT__
			static FOFMOD::Perfcounter perfcounter;
			perfcounter.Start();
			#endif // __PERFCOUNT__
		#endif // FOFMOD_DEBUG

		FOFMOD::Sound* snd   =  NULL;
		CacheSoundData* cacheData = NULL;
		this->GetSound( soundName, type, &snd, &cacheData );
		
		if( snd )
		{
			(*chn) = new FOFMOD::Channel( this );
			FMOD_RESULT result = this->PlaySound( snd, group, paused, *chn );
			if( result == FMOD_OK )
			{

			}
			else
			{
				delete *chn;
				delete snd;
				*chn = NULL;
				snd = NULL;
			}
		}

		#ifdef FOFMOD_DEBUG
			#ifdef __PERFCOUNT__
			FOFMOD_DEBUG_LOG("PlaySound handle time <%f ms> sound <%s> \n ", perfcounter.Get(), soundName.c_str() );
			#endif // __PERFCOUNT__
		#endif // FOFMOD_DEBUG
	}
	
	FMOD_RESULT System::PlaySound( FOFMOD::Sound* snd, FMOD::ChannelGroup* group, bool paused, FOFMOD::Channel* chn )
	{
		FMOD_RESULT result = FMOD_OK;
		
		result = this->FSystem->playSound( snd->handle, group, paused, &( chn->handle ) );
		if( result == FMOD_OK )
		{
			FOFMOD::System::ChannelCallbackData* cbdata = new FOFMOD::System::ChannelCallbackData();
			cbdata->system = this;
			cbdata->channel = chn; 

			chn->SetSound( snd );
			chn->Addref(); // to invalidate and release in callback
			chn->handle->setCallback( FOFMOD::System::ChannelCallback );
			chn->handle->setUserData( cbdata );

			if( !paused )
				chn->handle->setPaused( false );
		}
		return result;
	}

	void System::MapArchive( unsigned int index )
	{
		if( index < this->indexedArchives.size() )
		{
			FOFMOD_DEBUG_LOG("Mapping archive at index %d \n", index );
			IndexedArchiveFile* iaf = this->indexedArchives[index];
			if( !(iaf->isMapped) )
			{
				//FOFMOD_DEBUG_LOG("Not mapped \n");
				IArchiveFile* arch = iaf->arch;
				std::vector< std::string > strVec;
				arch->GetSymbolNames( strVec );
				for( unsigned int i = 0, j = strVec.size(); i < j; i++ )
				{
					// the symbol name is basically a filename with path in an archive file with file directories inside
					FOFMOD::System::SoundAlias alias;
					alias.fullname = strVec[i];
					alias.index    = index;
					//FOFMOD_DEBUG_LOG("fullname index %s %d \n", alias.fullname.c_str(), i );
					struct cwk_segment segment;
					if( cwk_path_get_first_segment( alias.fullname.c_str(), &segment) )
					{
						// has segments
						memset( &segment, 0, sizeof( struct cwk_segment ) );
						if( cwk_path_get_last_segment( alias.fullname.c_str(), &segment ) )
						{
							// last segment if actual filename in path;
							alias.name.assign( segment.begin, segment.size );
							if( alias.name.size() > 0 )
							{
								//FOFMOD_DEBUG_LOG("alias name %s \n", alias.name.c_str() );
								cwk_path_get_previous_segment( &segment );
								bool isMusic = false; 
								bool isSound = false;

								if( memcmp( segment.begin, "music", segment.size ) == 0 )
								{
									isMusic = true;
								}
								else
								if ( memcmp( segment.begin, "sfx", segment.size ) == 0 )
								{
									isSound = true;
								}

								cwk_path_get_previous_segment( &segment );
								if( memcmp( segment.begin, "sound", segment.size) == 0 )
								{
									if( !cwk_path_get_previous_segment( &segment ) ) // there is nothing behind sounds
									{
										// all good
										std::map< std::string, SoundAlias >* mapPtr = NULL;
										if( isMusic )
										{
											mapPtr = &this->musicNames;
										}
										else
										if( isSound )
										{
											mapPtr = &this->soundNames;
										}

										if( mapPtr )
										{
											std::map< std::string, SoundAlias >::iterator exist = mapPtr->find( alias.name );
											if( exist != mapPtr->end() )
											{
												mapPtr->erase( exist ); // there can be only one
												FOFMOD_DEBUG_LOG("Mapping duplicate <%s> <%s> \n", alias.name.c_str(), ( isSound ? "sound" : "music" ) );
											}
											//FOFMOD_DEBUG_LOG("Mapping <%s> dir as name <%s> <%s> \n", ( isSound ? "sound" : "music" ), alias.fullname.c_str(), alias.name.c_str() );
											mapPtr->insert( std::pair< std::string, SoundAlias >( alias.name, alias ) );
										}
									}
								}
							}
						}
					}
					else
					{
						// the file is located in root dir of the archive
						// do not do anything, fuck this file
					}
				}
			}

			iaf->isMapped = true;
		}
	}

	FOFMOD::Channel* System::PlaySound( const std::string& soundName, bool paused )
	{	
		FOFMOD::Channel* chn = NULL;
		this->Play( soundName, SOUND_TYPE::SOUND, this->soundChannelGroup, &chn, paused );
		return chn;
	}

	FOFMOD::Channel* System::PlayMusic( const std::string& soundName, bool paused )
	{

		FOFMOD::Channel* chn = NULL;
		this->Play( soundName, FOFMOD_SOUND_TYPE::MUSIC, this->musicChannelGroup, &chn, paused );
		return chn;
	}

	void System::SoundFromMemory( void* ptr, unsigned int size, FOFMOD::Sound** sptr )
	{

		FMOD_CREATESOUNDEXINFO memLoadInfo = { sizeof(FMOD_CREATESOUNDEXINFO), size };

		FMOD::Sound* fsnd = NULL;
		FMOD_RESULT result = this->FSystem->createSound(  (const char*)(ptr), CREATEFLAGS_STREAM , &memLoadInfo, &fsnd );
		if( result == FMOD_OK )
		{
			*sptr = new FOFMOD::Sound( this );
			(*sptr)->SetHandle( fsnd );
		}
	}

	void System::SoundFromArchive( const std::string& filename, const std::string cacheName, FOFMOD_SOUND_TYPE type, unsigned int flags, FOFMOD::Sound** sptr, CacheSoundData** cache )
	{
		SoundAlias* alias = NULL;
		std::map< std::string, SoundAlias >::iterator found = this->soundNames.end();

		unsigned int size = 0;
		void* binary = NULL;

		if( !ISFLAG( flags, FOFMOD_SOUND_FLAG::NO_LOOKUP ) )
		{
			//FOFMOD_DEBUG_LOG("Lookup mapped name %s %d \n", filename.c_str(), type );
			switch( type )
			{
				case( FOFMOD_SOUND_TYPE::SOUND ):
				{
					//FOFMOD_DEBUG_LOG("Looking in soundNames \n");
					found = this->soundNames.find( filename );
					if( found == this->soundNames.end() ) 
						return;
					break;
				}
				case( FOFMOD_SOUND_TYPE::MUSIC ):
				{
					found = this->musicNames.find( filename );
					if( found == this->musicNames.end() )
						return;
					break;
				}
				default:
					return;
			}

			alias = &(found->second);
			//FOFMOD_DEBUG_LOG("Alias found %s %s \n", alias->name.c_str(), alias->fullname.c_str() );

			IndexedArchiveFile* iaf = this->indexedArchives[ alias->index ];

			binary = iaf->arch->GetContent( alias->fullname.c_str(), &size );
		}
		else
		{
			// from end to begin, last indexed archive in priority
			for( IndexedArchiveFilePtrVec::reverse_iterator it = this->indexedArchives.rbegin(); it != this->indexedArchives.rend(); it++ )
			{
				IndexedArchiveFile* iaf = *it;
				binary = iaf->arch->GetContent( filename.c_str(), &size );
				if( binary )
					break;
			}
		}
	
		if( binary )
		{
			this->SoundFromMemory( binary, size, sptr );
			if( !( *sptr ) )
			{
				// could not create the sound
				free( binary );
			}
			else
			{	
				this->AddCachedSound( cacheName, binary, size, cache );
			}
		}
	}

	void System::SoundFromFile( const std::string& filename, FOFMOD_SOUND_TYPE type, FOFMOD::Sound** sptr, CacheSoundData** cache )
	{
		
		FILE* file = fopen( filename.c_str(), "rb" );
		if( file )
		{
			FOFMOD_DEBUG_LOG("File opened \n");

			fseek( file, 0, SEEK_END );
			unsigned int size = ftell( file );
			char* binary = (char*)malloc(size);
			rewind( file );

			unsigned int readSize = fread( binary, 1, size, file );
			fclose( file );
			if( readSize == size ) 
			{
				FOFMOD_DEBUG_LOG("Play sound from file read ok <%d> <%d> \n", size, readSize);

				this->SoundFromMemory( binary, size, sptr );

				if( !(*sptr) )
				{
					free ( binary );
				}
				else
				{
					this->AddCachedSound( filename, binary, size, cache );
				}		
			}
			else
			{
				// error reading file to heap
				free( binary );
			}
		}
		
	}

	void System::GetSound( const std::string& filename, FOFMOD_SOUND_TYPE type, FOFMOD::Sound** sptr, CacheSoundData** cache  )
	{	
		
		std::string pathName;
		unsigned int flags = 0;
		if( filename.find ( PathSeparator ) != std::string::npos ) // its a path based filename, override lookup
		{
			pathName = filename;
			SETFLAG( flags, FOFMOD_SOUND_FLAG::NO_LOOKUP );
		}
		else
		{
			switch ( type )
			{
				case( FOFMOD_SOUND_TYPE::SOUND ):
				{
					pathName.assign( PathToSounds );
					pathName += filename;
					break;
				}

				case( FOFMOD_SOUND_TYPE::MUSIC ):
				{
					pathName.assign( PathToMusic );
					pathName += filename;
					break;	
				}
				default:
					return;
			}
		}
		

		FOFMOD_DEBUG_LOG("PathName %s  filename %s \n", pathName.c_str(), filename.c_str() );

		this->GetCachedSound( pathName, cache );
		if( *cache )
		{
			FOFMOD_DEBUG_LOG("Get sound from cached \n");
			this->SoundFromMemory( (*cache)->data, (*cache)->size, sptr );
		}
		else
		{
			FOFMOD_DEBUG_LOG("Get sound from file \n")
			this->SoundFromFile( pathName, type, sptr, cache );

			if( !(*sptr) )
			{
				// check for archives
				FOFMOD_DEBUG_LOG("Get sound from file failed, trying get sound from archive. \n")
				this->SoundFromArchive( filename, pathName, type, flags, sptr, cache );
			}
		}
		
		if( *cache )
		{
			if( *sptr )
			{
				(*sptr)->SetCache( *cache );
			}
			
			(*cache)->Release();
		}
	}
	
	FOFMOD::Sound* System::GetSound( const std::string& filename, FOFMOD_SOUND_TYPE type )
	{
		FOFMOD::Sound* snd = NULL;
		FOFMOD::CacheSoundData* cache = NULL;
		this->GetSound( filename, type, &snd, &cache );
		return snd;
	}

	void System::AddCachedSound( const std::string& filename, void* data, unsigned int size, CacheSoundData** cache )
	{
		this->GetCachedSound( filename, cache );
		if( !(*cache) )
		{
			*cache = new FOFMOD::CacheSoundData( this );
			(*cache)->data = data;
			(*cache)->size = size;
			std::pair< CachedDataMap::iterator, bool > insertionResult = this->cachedSoundsData.insert( std::pair< std::string , FOFMOD::CacheSoundData* >( filename, (*cache) ) );
			if( insertionResult.second )
			{
				(*cache)->Addref();
				(*cache)->it = insertionResult.first;
			}
			else
			{
				FOFMOD_DEBUG_LOG("Caching playing sound fail <%s> \n", filename.c_str() );
				delete (*cache);
				*cache = NULL;
			}
		}
		else
		{
			// already there
			(*cache)->Release();
			FOFMOD_DEBUG_LOG("Caching playing sound already cached <%s> \n", filename.c_str() );
		}
	}
	
	void System::AddCachedDSPEffect( FOFMOD::DSP* dspEffect )
	{
		dspEffect->Addref();
		this->cachedDSPEffects.push_back( dspEffect );
	}

	void System::GetCachedSound( const std::string& filename, CacheSoundData** cache )
	{
		CachedDataMap::iterator found = this->cachedSoundsData.find( filename );
		if( found != this->cachedSoundsData.end () )
		{
			*cache = found->second;
			(*cache)->Addref();
		}
	}
	
	
	void System::OnChannelEnd( FOFMOD::Channel* channel )
	{
		
	}

	void System::SetMusicVolume( float volume )
	{
		if( this->musicChannelGroup )
		{
			this->musicChannelGroup->setVolume( volume );
		}
	}

	void System::GetMusicVolume( float* volume )
	{
		if( this->musicChannelGroup )
		{
			this->musicChannelGroup->getVolume( volume );
		}
	}

	void System::SetSoundsVolume( float volume )
	{
		if( this->soundChannelGroup )
		{
			this->soundChannelGroup->setVolume( volume );
		}
	}

	void System::GetSoundsVolume( float* volume )
	{
		if( this->soundChannelGroup )
		{
			this->soundChannelGroup->getVolume( volume );
		}
	}

	void System::PauseAllSounds( bool state )
	{
		if( this->soundChannelGroup )
		{
			this->soundChannelGroup->setPaused( state );
		}
	}

	void System::PauseAllMusic( bool state )
	{
		if( this->musicChannelGroup )
		{
			this->musicChannelGroup->setPaused( state );
		}
	}


	void System::StopAllSounds()
	{
		if( this->soundChannelGroup )
		{
			this->soundChannelGroup->stop();
		}
	}
	
	void System::StopAllMusic()
	{
		if( this->musicChannelGroup )
		{
			this->musicChannelGroup->stop();
		}
	}

	void System::StopAll()
	{
		this->StopAllSounds();
		this->StopAllMusic();
	}

	void System::PauseAll( bool state )
	{
		this->PauseAllSounds( state );
		this->PauseAllMusic( state );
	}
	
	void System::Set3DSettings( float dopplerScale, float distanceFactor, float rolloffScale )
	{
		this->FSystem->set3DSettings( dopplerScale, distanceFactor, rolloffScale );
	}


	#define SETLISTENERDATA(data, x, y, z) \
	this->listener.data.x = x; \
	this->listener.data.y = y; \
	this->listener.data.z = z; 

	void System::Set3DListenerPosition( float x, float y, float z )
	{
		SETLISTENERDATA(position, x, y, z);
		this->FSystem->set3DListenerAttributes( 0, &(this->listener.position), NULL, NULL, NULL );
	}

	void System::Set3DListenerVelocity( float x, float y, float z )
	{
		SETLISTENERDATA(velocity, x, y, z);
		this->FSystem->set3DListenerAttributes( 0, NULL, &(this->listener.velocity), NULL, NULL );
	}

	void System::Set3DListenerForward( float x, float y, float z )
	{
		SETLISTENERDATA(forward, x, y, z);
		this->FSystem->set3DListenerAttributes( 0, NULL, NULL, &(this->listener.forward), NULL );
	}

	void System::Set3DListenerUp( float x, float y, float z )
	{ 
		SETLISTENERDATA(up, x, y, z);
		this->FSystem->set3DListenerAttributes( 0, NULL, NULL, NULL, &(this->listener.up) );
	}

	
	void System::CreateDSPEffect( FMOD_DSP_TYPE effectType, float* params, unsigned int paramsCount, FOFMOD::DSP** dspEffect )
	{
		effectType = CLAMPA( effectType, FMOD_DSP_TYPE_UNKNOWN, FMOD_DSP_TYPE_MAX );
		FMOD::DSP* fdsp = NULL;
		FMOD_RESULT fres = this->FSystem->createDSPByType( effectType, &fdsp );
		if( fres == FMOD_OK )
		{
			*dspEffect = new FOFMOD::DSP( this );
			(*dspEffect)->SetHandle( fdsp );
			(*dspEffect)->ParseParams( params, paramsCount );
			this->AddCachedDSPEffect( *dspEffect );
		}
	}
	
	void System::ApplyMusicDSPEffect( FOFMOD::DSP* dspEffect )
	{
		if( this->musicChannelGroup )
		{
			FMOD::DSP* handle = NULL;
			dspEffect->GetHandle( &handle );
			if( handle )
			{
				this->musicChannelGroup->addDSP( FMOD_CHANNELCONTROL_DSP_TAIL, handle );
			}
		}
	}
	
	void System::ApplySoundsDSPEffect( FOFMOD::DSP* dspEffect )
	{
		
	}
	
	void System::ApplyDSPEffect( FOFMOD::DSP* dspEffect )
	{
		
	}
	
	void System::DropMusicDSPEffect( FOFMOD::DSP* dspEffect )
	{
		
	}
	
	void System::DropSoundsDSPEffect( FOFMOD::DSP* dspEffect )
	{
		
	}
	
	void System::DropDSPEffect( FOFMOD::DSP* dspEffect )
	{
		
	}
	
	void System::DropAllSoundsDSPEffect()
	{
		
	}
	
	void System::DropAllMusicDSPEffect()
	{
		
	}
	
	void System::DropAllDSPEffect()
	{
		
	}
}


#endif // __FOFMODSYSTEM__