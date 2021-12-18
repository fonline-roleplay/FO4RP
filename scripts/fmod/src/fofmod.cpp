
#include "fofmod.h"
#include "_defines.fos"
#include "fonline.h"
#include "fofmodchannelAS.h"
#include "fofmodsoundAS.h"
#include "fofmoddspAS.h"

#include "string.h"


FOFMOD::System* FMODSystem = NULL;
const char* STR_INIT_ERROR = "Use of unitialized FMOD system \n";

#define NONE
#define FMOD_DEFAULT_CHANNEL_COUNT ( 32 )

#define FMODCHECK( retrn ) if( !FMODSystem ) 		\
{ 													\
  FMOD_Initialize(FMOD_DEFAULT_CHANNEL_COUNT);		\
  if( !FMODSystem->IsInit() )					 	\
  { 												\
  	Log(STR_INIT_ERROR); 							\
  	return retrn ; 									\
  } 												\
} 													

void RegisterASInterface();

bool FMOD_Initialize( unsigned int channelCount );
void FMOD_Update();
bool FMOD_TouchArchive( ScriptString& filename );
// void FMOD_PreloadSounds( ScriptString& dir );
// void FMOD_PreloadSoundFile( ScriptString& path );
// void FMOD_DeleteCachedSound( ScriptString& soundName );
void FMOD_PauseAllSounds( bool state );
void FMOD_PauseAllMusic( bool state );
void FMOD_StopAllSounds();
void FMOD_StopAllMusic();
void FMOD_StopAll();
void FMOD_PauseAll( bool state );
float FMOD_GetSoundsVolume();
float FMOD_GetMusicVolume();
void FMOD_SetSoundsVolume( float volume );
void FMOD_SetMusicVolume( float volume );
void FMOD_Set3DSettings( float dopplerScale, float distanceFactor, float rolloffScale );
void FMOD_Set3DListenerPosition( float x, float y, float z );
void FMOD_Set3DListenerVelocity( float x, float y, float z );
void FMOD_Set3DListenerForward( float x, float y, float z );
void FMOD_Set3DListenerUp( float x, float y, float z );

FOFMOD::Sound* FMOD_GetSound( ScriptString& soundName, int soundType );

FOFMOD::Channel* FMOD_PlaySound( ScriptString& soundName, bool paused );
FOFMOD::Channel* FMOD_PlayMusic( ScriptString& soundName, bool paused );

FOFMOD::DSP* FMOD_CreateEffect( int effectType, CScriptArray& params );
void FMOD_ApplyEffect( FOFMOD::DSP& effect );
void FMOD_DropEffect( FOFMOD::DSP& effect );
void FMOD_ApplyMusicEffect( FOFMOD::DSP& effect );
void FMOD_ApplySoundsEffect( FOFMOD::DSP& effect );
void FMOD_DropMusicEffect( FOFMOD::DSP& effect );
void FMOD_DropSoundsEffect( FOFMOD::DSP& effect );
void FMOD_DropAllMusicEffects();
void FMOD_DropAllSoundsEffects();
void FMOD_DropAllEffects();


FONLINE_DLL_ENTRY( compiler )
{
	RegisterASInterface();
}


bool FMOD_Initialize( unsigned int channelCount )
{
	Log("FMOD Initializing... \n");

	bool result = true;
	if( !FMODSystem )
	{
		FMODSystem = new FOFMOD::System();
	}
	result = FMODSystem->Initialize( channelCount ) == FMOD_OK;

	if( result )
		Log("FMOD Initialized.\n");

	return result;
}

// void FMOD_PreloadSounds( ScriptString& dir )
// {
// 	FMODCHECK(NONE)
// 	FMODSystem->PreloadSounds( dir.c_str() );
// }

// void FMOD_PreloadSoundFile( ScriptString& path )
// {
// 	FMODCHECK(NONE)
// }

// void FMOD_DeleteCachedSound( ScriptString& soundName )
// {
// 	FMODCHECK(NONE)
// }

FOFMOD::Sound* FMOD_GetSound( ScriptString& soundName, int soundType )
{
	FMODCHECK(NULL)
	FOFMOD::Sound* ret = NULL;
	ret = FMODSystem->GetSound( soundName.c_std_str(), (FOFMOD_SOUND_TYPE)soundType );
	if( ret )
		ret->Addref();
	return ret;
}

FOFMOD::Channel* FMOD_PlaySound( ScriptString& soundName, bool paused )
{
	FMODCHECK(NULL)
	FOFMOD::Channel* ret = NULL;
	ret = FMODSystem->PlaySound( soundName.c_std_str(), paused );
	if( ret )
		Script_Channel_Addref( ret );
	return ret;
}

FOFMOD::Channel* FMOD_PlayMusic( ScriptString& soundName, bool paused )
{
	FMODCHECK(NULL)
	FOFMOD::Channel* ret = NULL;
	ret = FMODSystem->PlayMusic( soundName.c_std_str(), paused );
	if( ret )
		Script_Channel_Addref( ret );
	return ret;
}

void FMOD_Update()
{
	FMODCHECK(NONE)
	FMODSystem->Update();
}

void FMOD_PauseAllSounds( bool state )
{
	FMODCHECK(NONE)
	FMODSystem->PauseAllSounds( state );
}

void FMOD_PauseAllMusic( bool state )
{
	FMODCHECK(NONE)
	FMODSystem->PauseAllMusic( state );
}

void FMOD_StopAllSounds()
{
	FMODCHECK(NONE)
	FMODSystem->StopAllSounds();
}

void FMOD_StopAllMusic()
{
	FMODCHECK(NONE)
	FMODSystem->StopAllMusic();
}

void FMOD_StopAll()
{
	FMODCHECK(NONE)
	FMODSystem->StopAll();
}

void FMOD_PauseAll( bool state )
{
	FMODCHECK(NONE)
	FMODSystem->PauseAll( state );
}


float FMOD_GetSoundsVolume()
{
	FMODCHECK(0.0)
	float result = 0.0;
	FMODSystem->GetSoundsVolume( &result );
	return result;
}

float FMOD_GetMusicVolume()
{
	FMODCHECK(0.0)
	float result = 0.0;
	FMODSystem->GetMusicVolume( &result );
	return result;
}

void FMOD_SetSoundsVolume( float volume )
{
	FMODCHECK(NONE)
	FMODSystem->SetSoundsVolume( volume );
}

void FMOD_SetMusicVolume( float volume )
{
	FMODCHECK(NONE)
	FMODSystem->SetMusicVolume( volume );
}

bool FMOD_TouchArchive( ScriptString& filename )
{
	FMODCHECK(false)
	return FMODSystem->TouchArchive( filename.c_std_str() );
}

void FMOD_Set3DSettings( float dopplerScale, float distanceFactor, float rolloffScale )
{
	FMODCHECK(NONE)
	FMODSystem->Set3DSettings( dopplerScale, distanceFactor, rolloffScale );
}

void FMOD_Set3DListenerPosition( float x, float y, float z )
{
	FMODCHECK(NONE)
	FMODSystem->Set3DListenerPosition( x, y, z );
}

void FMOD_Set3DListenerVelocity( float x, float y, float z )
{
	FMODCHECK(NONE)
	FMODSystem->Set3DListenerVelocity( x, y, z );
}

void FMOD_Set3DListenerForward( float x, float y, float z )	
{
	FMODCHECK(NONE)
	FMODSystem->Set3DListenerForward( x, y, z );
}

void FMOD_Set3DListenerUp( float x, float y, float z )
{
	FMODCHECK(NONE)
	FMODSystem->Set3DListenerUp( x, y, z );
}

FOFMOD::DSP* FMOD_CreateEffect( int effectType, CScriptArray& params )
{
	FMODCHECK(NULL);
	FOFMOD::DSP* ret = NULL;
	// int typei = params.GetElementTypeId();
	// asIObjectType* arrOType = ASEngine->GetObjectTypeById( typei );
	// const char* typeName = arrOType->GetName();
	// Log( "typename %s \n", typeName );
	// if( strcmp( typeName, "float" ) == 0 )
	// {
		asUINT paramCount = params.GetSize();
		float* fparams = NULL;
		if( paramCount )
		{
			fparams = (float*)malloc( paramCount*sizeof( float ) );
			memset(fparams, 0, paramCount*sizeof( float ) );
			for( asUINT i = 0; i < paramCount; i++ )
			{
				fparams[i] =  *( ( float* ) params.At( i ) ) ;
			}
			
		}
		FMODSystem->CreateDSPEffect( (FMOD_DSP_TYPE)effectType, fparams, (unsigned int)paramCount, &ret );
		free(fparams);
	// }
	if( ret )
		Script_DSP_Addref( ret );
	return ret;
	
}


void FMOD_ApplyEffect( FOFMOD::DSP& effect )
{
	FMODCHECK(NONE);
	FMODSystem->ApplyDSPEffect( &effect );
}

void FMOD_DropEffect( FOFMOD::DSP& effect )
{
	FMODCHECK(NONE);
	FMODSystem->DropDSPEffect( &effect );
}

void FMOD_ApplyMusicEffect( FOFMOD::DSP& effect )
{
	FMODCHECK(NONE);
	FMODSystem->ApplyMusicDSPEffect( &effect );
}

void FMOD_ApplySoundsEffect( FOFMOD::DSP& effect )
{
	FMODCHECK(NONE);
	FMODSystem->ApplySoundsDSPEffect( &effect );
}

void FMOD_DropMusicEffect( FOFMOD::DSP& effect )
{
	FMODCHECK(NONE);
	FMODSystem->DropMusicDSPEffect( &effect );
}

void FMOD_DropSoundsEffect( FOFMOD::DSP& effect )
{
	FMODCHECK(NONE);
	FMODSystem->DropSoundsDSPEffect( &effect );
}

void FMOD_DropAllMusicEffects()
{
	FMODCHECK(NONE);
	FMODSystem->DropAllMusicDSPEffects();
}

void FMOD_DropAllSoundsEffects()
{
	FMODCHECK(NONE);
	FMODSystem->DropAllSoundsDSPEffects();
}

void FMOD_DropAllEffects()
{
	FMODCHECK(NONE);
	FMODSystem->DropAllDSPEffects();
}


void RegisterASInterface()
{
 	Log("Binding FMOD sound system API... \n" );
 	const char* STR_BIND_ERROR = "Failed to bind %s error <%d> \n";
	int r = 0;
	if( ASEngine )
	{	
		r = ASEngine->RegisterObjectType( "FMODSound", 0, asOBJ_REF );
		if( !r )
			Log("Failed to register object type %s %d \n ", "FMODSound", r );
		r = ASEngine->RegisterObjectBehaviour("FMODSound", asBEHAVE_ADDREF, "void f()", 					asFUNCTION(FOFMOD::Script_Sound_Addref), asCALL_CDECL_OBJLAST);
		if( !r )
			Log("Failed to register addref for %s %d \n ", "FMODSound", r );
		r = ASEngine->RegisterObjectBehaviour("FMODSound", asBEHAVE_RELEASE, "void f()", 					asFUNCTION(FOFMOD::Script_Sound_Release), asCALL_CDECL_OBJLAST);
		if( !r )
			Log("Failed to register release for %s %d \n ", "FMODSound", r);
		r = ASEngine->RegisterObjectMethod("FMODSound", "uint GetLength()", 								asFUNCTION(FOFMOD::Script_Sound_GetLength), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODSound::GetLength", r );
		r = ASEngine->RegisterObjectMethod("FMODSound", "void SetLoopCount(uint count)", 					asFUNCTION(FOFMOD::Script_Sound_SetLoopCount), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODSound::SetLoopCount", r );
		r = ASEngine->RegisterObjectMethod("FMODSound", "uint GetLoopCount()", 								asFUNCTION(FOFMOD::Script_Sound_GetLoopCount), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODSound::GetLoopCount", r );
		r = ASEngine->RegisterObjectMethod("FMODSound", "bool IsValid()", 									asFUNCTION(FOFMOD::Script_Sound_IsValid), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODSound::IsValid", r );
		
		
		r = ASEngine->RegisterObjectType( "FMODEffect", 0, asOBJ_REF );
		if( !r )
			Log("Failed to register object type %s %d \n ", "FMODEffect", r );
		r = ASEngine->RegisterObjectBehaviour("FMODEffect", asBEHAVE_ADDREF, "void f()", 					asFUNCTION(FOFMOD::Script_DSP_Addref), asCALL_CDECL_OBJLAST);
		if( !r )
			Log("Failed to register addref for %s %d \n ", "FMODEffect", r );
		r = ASEngine->RegisterObjectBehaviour("FMODEffect", asBEHAVE_RELEASE, "void f()", 					asFUNCTION(FOFMOD::Script_DSP_Release), asCALL_CDECL_OBJLAST);
		if( !r )
			Log("Failed to register release for %s %d \n ", "FMODEffect", r);
		r = ASEngine->RegisterObjectMethod("FMODEffect", "bool IsValid()", 									asFUNCTION(FOFMOD::Script_DSP_IsValid), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODEffect::IsValid", r );


		r = ASEngine->RegisterObjectType( "FMODChannel", 0, asOBJ_REF );
		if( !r )
			Log("Failed to register object type %s %d \n ", "FMODChannel", r );
		r = ASEngine->RegisterObjectBehaviour("FMODChannel", asBEHAVE_ADDREF, "void f()", 					asFUNCTION(FOFMOD::Script_Channel_Addref), asCALL_CDECL_OBJLAST);
		if( !r )
			Log("Failed to register addref for %s %d \n ", "FMODChannel", r );
		r = ASEngine->RegisterObjectBehaviour("FMODChannel", asBEHAVE_RELEASE, "void f()", 					asFUNCTION(FOFMOD::Script_Channel_Release), asCALL_CDECL_OBJLAST);
		if( !r )
			Log("Failed to register release for %s %d \n ", "FMODChannel", r);
		r = ASEngine->RegisterObjectMethod("FMODChannel", "FMODSound@ GetSound()",	 						asFUNCTION(FOFMOD::Script_Channel_GetSound), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetSound", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetPlaybackPosition(uint positionMs)", 		asFUNCTION(FOFMOD::Script_Channel_SetPlaybackPosition), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetPlaybackPosition", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "uint GetPlaybackPosition()",						asFUNCTION(FOFMOD::Script_Channel_GetPlaybackPosition), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetPlaybackPosition", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetPitch(float pitch)", 					asFUNCTION(FOFMOD::Script_Channel_SetPitch), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetPitch", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "float GetPitch()", 								asFUNCTION(FOFMOD::Script_Channel_GetPitch), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetPitch", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "bool IsValid()", 								asFUNCTION(FOFMOD::Script_Channel_IsValid), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::IsValid", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetPaused(bool paused)", 					asFUNCTION(FOFMOD::Script_Channel_SetPaused), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetPaused", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "bool IsPaused()", 								asFUNCTION(FOFMOD::Script_Channel_IsPaused), asCALL_CDECL_OBJLAST);
		if( !r )
		Log(STR_BIND_ERROR, "FMODChannel::IsPaused", r );
			r = ASEngine->RegisterObjectMethod("FMODChannel", "int GetPriority()", 							asFUNCTION(FOFMOD::Script_Channel_GetPriority), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetPriority", r );
			r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetPriority( int priority )", 			asFUNCTION(FOFMOD::Script_Channel_SetPriority), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetPriority", r );

		r = ASEngine->RegisterObjectMethod("FMODChannel", "void Stop()", 									asFUNCTION(FOFMOD::Script_Channel_Stop), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::Stop", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "bool IsPlaying()", 								asFUNCTION(FOFMOD::Script_Channel_IsPlaying), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::IsPlaying", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetVolume(float volume)", 					asFUNCTION(FOFMOD::Script_Channel_SetVolume), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetVolume", r );

		r = ASEngine->RegisterObjectMethod("FMODChannel", "float GetVolume()", 								asFUNCTION(FOFMOD::Script_Channel_GetVolume), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetVolume", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void Set3DPosition(float x, float y, float z)", 	asFUNCTION(FOFMOD::Script_Channel_Set3DPosition), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::Set3DPosition", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void Set3DVelocity(float x, float y, float z)", 	asFUNCTION(FOFMOD::Script_Channel_Set3DVelocity), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::Set3DVelocity", r );	
		r = ASEngine->RegisterObjectMethod("FMODChannel", "int GetLoopCount()", 							asFUNCTION(FOFMOD::Script_Channel_GetLoopCount), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetLoopCount", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetLoopCount(int count)", 					asFUNCTION(FOFMOD::Script_Channel_SetLoopCount), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetLoopCount", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void GetLoopPoints(uint& loopStart, uint loopStartType, uint& loopEnd, uint loopEndType)", 	asFUNCTION(FOFMOD::Script_Channel_GetLoopPoints), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetLoopPoints", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetLoopPoints(uint loopStart, uint loopStartType,uint loopEnd, uint loopEndType)", 	asFUNCTION(FOFMOD::Script_Channel_SetLoopPoints), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetLoopPoints", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void Set3DMinMaxDistance(float min, float max)", 	asFUNCTION(FOFMOD::Script_Channel_Set3DMinMaxDistance), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::Set3DMinMaxDistance", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void Get3DMinMaxDistance(float& min, float& max)", 	asFUNCTION(FOFMOD::Script_Channel_Get3DMinMaxDistance), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::Get3DMinMaxDistance", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void Set3DLevel(float level)", 	asFUNCTION(FOFMOD::Script_Channel_Set3DLevel), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::Set3DLevel", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "float Get3DLevel()", 			asFUNCTION(FOFMOD::Script_Channel_Get3DLevel), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::Get3DLevel", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "float GetFrequency()", 			asFUNCTION(FOFMOD::Script_Channel_GetFrequency), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetFrequency", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetFrequency(float hertz)", 			asFUNCTION(FOFMOD::Script_Channel_SetFrequency), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetFrequency", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "bool GetVolumeRamp()", 			asFUNCTION(FOFMOD::Script_Channel_GetVolumeRamp), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::GetVolumeRamp", r );
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetVolumeRamp(bool condition)", 			asFUNCTION(FOFMOD::Script_Channel_SetVolumeRamp), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetVolumeRamp", r );
		
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void SetEffect(FMODEffect& effect)", 			asFUNCTION(FOFMOD::Script_Channel_SetEffect), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::SetEffect", r );
		
		r = ASEngine->RegisterObjectMethod("FMODChannel", "void DropEffect(FMODEffect& effect)", 			asFUNCTION(FOFMOD::Script_Channel_DropEffect), asCALL_CDECL_OBJLAST);
		if( !r )
			Log(STR_BIND_ERROR, "FMODChannel::DropEffect", r );



		r = ASEngine->RegisterGlobalFunction("bool FMOD_Initialize(uint channelCount)", 					asFUNCTION(FMOD_Initialize), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_Initialize", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_Update()", 													asFUNCTION(FMOD_Update), 			asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_Update", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_TouchArchive(string& filename)", 							asFUNCTION(FMOD_TouchArchive), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_TouchArchive", r );
		// r = ASEngine->RegisterGlobalFunction("void FMOD_PreloadSounds(string& dir)", 								asFUNCTION(FMOD_PreloadSounds), 	asCALL_CDECL );
		// if( !r )
		// 	Log(STR_BIND_ERROR, "FMOD_PreloadSounds", r );
		// r = ASEngine->RegisterGlobalFunction("void FMOD_PreloadSoundFile(string& path)", 							asFUNCTION(FMOD_PreloadSoundFile), 	asCALL_CDECL );
		// if( !r )
		// 	Log(STR_BIND_ERROR, "FMOD_PreloadSound", r );
		// r = ASEngine->RegisterGlobalFunction("void FMOD_DeleteCachedSound(string& soundName)",   					asFUNCTION(FMOD_DeleteCachedSound), asCALL_CDECL );
		// if( !r )
		// 	Log(STR_BIND_ERROR, "FMOD_DeleteCachedSound", r );
		r = ASEngine->RegisterGlobalFunction("FMODChannel@ FMOD_PlaySound(string& soundName,  bool paused)",   		asFUNCTION(FMOD_PlaySound), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_PlaySound", r );
		r = ASEngine->RegisterGlobalFunction("FMODChannel@ FMOD_PlayMusic(string& soundName,  bool paused)",   		asFUNCTION(FMOD_PlayMusic), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_PlayMusic", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_PauseAllSounds( bool state )",   							asFUNCTION(FMOD_PauseAllSounds),	asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_PauseAllSounds", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_PauseAllMusic( bool state )",   							asFUNCTION(FMOD_PauseAllMusic), 	asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_PauseAllMusic", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_StopAllSounds()",   										asFUNCTION(FMOD_StopAllSounds), 	asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_StopAllSounds", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_StopAllMusic()",   											asFUNCTION(FMOD_StopAllMusic), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_StopAllMusic", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_StopAll()",   												asFUNCTION(FMOD_StopAll), 			asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_StopAll", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_PauseAll( bool state )",   									asFUNCTION(FMOD_PauseAll), 			asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_PauseAll", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_SetSoundsVolume( float volume )",   						asFUNCTION(FMOD_SetSoundsVolume), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_SetSoundsVolume", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_SetMusicVolume( float volume )",   							asFUNCTION(FMOD_SetMusicVolume), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_SetMusicVolume", r );
		r = ASEngine->RegisterGlobalFunction("float FMOD_GetSoundsVolume()",   										asFUNCTION(FMOD_GetSoundsVolume), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_GetSoundsVolume", r );
		r = ASEngine->RegisterGlobalFunction("float FMOD_GetMusicVolume()",   										asFUNCTION(FMOD_GetMusicVolume), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_GetMusicVolume", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_Set3DSettings(float dopplerScale, float distanceFactor, float rolloffScale)",  	asFUNCTION(FMOD_Set3DSettings), asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_Set3DSettings", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_Set3DListenerPosition(float x, float y, float z)",   		asFUNCTION(FMOD_Set3DListenerPosition), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_Set3DListenerPosition", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_Set3DListenerVelocity(float x, float y, float z)",   		asFUNCTION(FMOD_Set3DListenerVelocity), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_Set3DListenerVelocity", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_Set3DListenerForward(float x, float y, float z)",   		asFUNCTION(FMOD_Set3DListenerForward), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_Set3DListenerForward", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_Set3DListenerUp(float x, float y, float z)",   				asFUNCTION(FMOD_Set3DListenerUp), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_Set3DListenerUp", r );
		r = ASEngine->RegisterGlobalFunction("FMODSound@ FMOD_GetSound( string& soundName, int soundType )",   		asFUNCTION(FMOD_GetSound), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_GetSound", r );
		r = ASEngine->RegisterGlobalFunction("FMODEffect@ FMOD_CreateEffect( int effectType, array<float>& params )",  asFUNCTION( FMOD_CreateEffect ), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_CreateEffect", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_ApplyEffect( FMODEffect& effect )",         asFUNCTION(FMOD_ApplyEffect), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_ApplyEffect", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_DropEffect( FMODEffect& effect )",         asFUNCTION(FMOD_DropEffect), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_DropEffect", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_ApplyMusicEffect( FMODEffect& effect )",    asFUNCTION(FMOD_ApplyMusicEffect), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_ApplyMusicEffect", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_ApplySoundsEffect( FMODEffect& effect )",   asFUNCTION(FMOD_ApplySoundsEffect), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_ApplySoundsEffect", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_DropMusicEffect( FMODEffect& effect )",   	asFUNCTION(FMOD_DropMusicEffect), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_DropMusicEffect", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_DropSoundsEffect( FMODEffect& effect )",   	asFUNCTION(FMOD_DropSoundsEffect), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_DropSoundsEffect", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_DropAllMusicEffects()", asFUNCTION(FMOD_DropAllMusicEffects), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_DropAllMusicEffects", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_DropAllSoundsEffects()", asFUNCTION(FMOD_DropAllSoundsEffects), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_DropAllSoundsEffects", r );
		r = ASEngine->RegisterGlobalFunction("void FMOD_DropAllEffects()",   	asFUNCTION(FMOD_DropAllEffects), 		asCALL_CDECL );
		if( !r )
			Log(STR_BIND_ERROR, "FMOD_DropAllEffects", r );
		//if( !r )
		// 	Log("Failed to register object type %s %d \n ", "FMODSystem", r );
	}
		
	if( r )
		Log("FMOD sound system API bound. \n" );
	else
		Log("FMOD sound system API bind fail, error <%d>. \n", r );
}
