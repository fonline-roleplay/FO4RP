#ifndef __FOFMODSYSTEM_H__
#define __FOFMODSYSTEM_H__

#include "fofmoddef.h"
#include "refcount.h"
#include <string>
#include <map>
#include "fofmodlistener.h"
#include "fofmodsound.h"
#include "fofmodchannel.h"
#include "archive.h"
#include "zipfile.h"
#include "memory.h"


namespace FOFMOD
{

	
	extern const int fileExtNum ;
	extern const std::string fileExtensions[];
	extern bool IsValidSoundExtension( const char* filename );


	class System
	{

		typedef std::map< std::string, FOFMOD::Sound* > SoundMap;
		typedef std::vector< FOFMOD::Sound* > SoundVec;
		typedef std::vector< FOFMOD::IArchiveFile* > ArchiveFilePtrVec;

		class CacheSoundData;
		typedef std::map< std::string, CacheSoundData* > CachedDataMap;

		class CacheSoundData : IRefcountable
		{

			protected:
				CacheSoundData();
				unsigned int refcount;

			public:
				CacheSoundData(FOFMOD::System* ownerSystem );
				~CacheSoundData();
				CachedDataMap::iterator it;
				FOFMOD::System* owner;
				void* data;
				unsigned int size;

				void Addref() override;
				void Release() override;
		};

		class ChannelCallbackData
		{
			public:
				ChannelCallbackData();
				~ChannelCallbackData();
				FOFMOD::System* system;
				FOFMOD::Sound*  sound;
				FOFMOD::System::CacheSoundData* cacheData;
		};


		public:
			static FMOD_RESULT F_CALLBACK ChannelCallback( FMOD_CHANNELCONTROL *channelcontrol, 
															FMOD_CHANNELCONTROL_TYPE controlType, 
															FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, 
															void* commanddata1, 
															void* commanddata2 );

			static const unsigned int CREATEFLAGS_STREAM = FMOD_OPENMEMORY_POINT | FMOD_CREATESTREAM;

		protected:
			FMOD::System* FSystem;
			FMOD::ChannelGroup*   soundChannelGroup;
			FMOD::ChannelGroup*   musicChannelGroup;
			bool initialized;

			
			// streamed sounds cannot be reused between different plays, so each new sound be generating own stream, but the prototype data is same.
			CachedDataMap 	 cachedSoundsData;

			ArchiveFilePtrVec indexedArchives;
			FOFMOD::Listener3D listener;
			void SoundFromMemory( void* ptr, unsigned int size, FOFMOD::Sound** sptr );
			void SoundFromArchive( const std::string& filename, FOFMOD::Sound** sptr, CacheSoundData** cache );
			void SoundFromFile( const std::string& filename, FOFMOD::Sound** sptr, CacheSoundData** cache );
			void GetSound( const std::string& filename, FOFMOD::Sound** sptr, CacheSoundData** cache );
			void AddCachedSound( const std::string& filename, void* data, unsigned int size, CacheSoundData** cache );	
			void GetCachedSound( const std::string& filename, CacheSoundData** cache );
			void Play( const std::string& soundName, FMOD::ChannelGroup* group, FOFMOD::Channel** chn, bool paused );
			

		public:
			System();
			~System();
			void Update();
			bool IsInit(){ return this->initialized; };
			FMOD_RESULT Initialize( unsigned int channelCount );

			// Caching and resource management
			bool TouchArchive( const std::string& filename );
			void DeleteCachedSound( CachedDataMap::iterator iterator );
			///////////////////////////////////

			// Playback and controls
			FOFMOD::Channel* PlaySound(  const std::string& soundName, bool paused );
			FOFMOD::Channel* PlayMusic( const std::string& soundName, bool paused );
			void SetMusicVolume( float volume );
			void GetMusicVolume( float* volume );
			void SetSoundsVolume( float volume );
			void GetSoundsVolume( float* volume );
			void PauseAllSounds( bool state );
			void PauseAllMusic( bool state);
			void StopAllSounds();
			void StopAllMusic();
			void StopAll();
			void PauseAll( bool state );
			void Set3DSettings( float dopplerScale, float distanceFactor, float rolloffScale );
			void Set3DListenerPosition( float x, float y, float z );
			void Set3DListenerVelocity( float x, float y, float z );
			void Set3DListenerForward( float x, float y, float z );
			void Set3DListenerUp( float x, float y, float z );
			///////////////////////////////////
	};


	// Angelscript interfacing for type methods // unused
	extern FOFMOD::System* Script_System_Factory();
};


#endif // __FOFMODSYSTEM_H__