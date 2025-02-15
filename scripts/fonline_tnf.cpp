#ifndef MAIN
#define MAIN

#include "fonline_tnf.h"
#include "AngelScript/scriptfilesystem.cpp"

#include <time.h>


// Extern data definition
_GlobalVars GlobalVars;

// Slot/parameters allowing
//EXPORT bool allowSlot_Hand1( uint8, Item &, Critter &, Critter & toCr );

// Parameters Get behavior
EXPORT int getParam_Strength( CritterMutual & cr, uint );
EXPORT int getParam_Perception( CritterMutual & cr, uint );
EXPORT int getParam_Endurance( CritterMutual & cr, uint );
EXPORT int getParam_Charisma( CritterMutual & cr, uint );
EXPORT int getParam_Intellegence( CritterMutual & cr, uint );
EXPORT int getParam_Agility( CritterMutual & cr, uint );
EXPORT int getParam_Luck( CritterMutual & cr, uint );
EXPORT int getParam_Hp( CritterMutual & cr, uint );
EXPORT int getParam_MaxLife( CritterMutual & cr, uint );
EXPORT int getParam_MaxAp( CritterMutual & cr, uint );
EXPORT int getParam_Ap( CritterMutual & cr, uint );
EXPORT int getParam_RegenAp( CritterMutual & cr, uint );
EXPORT int getParam_MaxMoveAp( CritterMutual & cr, uint );
EXPORT int getParam_MoveAp( CritterMutual & cr, uint );
EXPORT int getParam_MaxWeight( CritterMutual & cr, uint );
EXPORT int getParam_Sequence( CritterMutual & cr, uint );
EXPORT int getParam_MeleeDmg( CritterMutual & cr, uint );
EXPORT int getParam_HealingRate( CritterMutual & cr, uint );
EXPORT int getParam_CriticalChance( CritterMutual & cr, uint );
EXPORT int getParam_MaxCritical( CritterMutual & cr, uint );
EXPORT int getParam_Ac( CritterMutual & cr, uint );
EXPORT int getParam_DamageResistance( CritterMutual& cr, uint index );
EXPORT int getParam_DamageThreshold( CritterMutual& cr, uint index );
EXPORT int getParam_RadiationResist( CritterMutual & cr, uint );
EXPORT int getParam_PoisonResist( CritterMutual & cr, uint );
EXPORT int getParam_Timeout( CritterMutual& cr, uint index );
EXPORT int getParam_Reputation( CritterMutual& cr, uint index );
EXPORT void changedParam_Reputation( CritterMutual& cr, uint index, int oldValue );

// Extended methods
EXPORT bool Critter_IsInjured( CritterMutual& cr );
EXPORT bool Critter_IsDmgEye( CritterMutual& cr );
EXPORT bool Critter_IsDmgLeg( CritterMutual& cr );
EXPORT bool Critter_IsDmgTwoLeg( CritterMutual& cr );
EXPORT bool Critter_IsDmgArm( CritterMutual& cr );
EXPORT bool Critter_IsDmgTwoArm( CritterMutual& cr );
EXPORT bool Critter_IsAddicted( CritterMutual& cr );
EXPORT bool Critter_IsOverweight( CritterMutual& cr );
EXPORT bool Item_Weapon_IsHtHAttack( Item& item, uint8 mode );
EXPORT bool Item_Weapon_IsGunAttack( Item& item, uint8 mode );
EXPORT bool Item_Weapon_IsRangedAttack( Item& item, uint8 mode );
EXPORT int  ItemTransferCount( CritterMutual& cr );
EXPORT int  AccessContainerId( CritterMutual& cr );

// Callbacks
uint GetUseApCost( CritterMutual& cr, Item& item, uint8 mode );
uint GetAttackDistantion( CritterMutual& cr, Item& item, uint8 mode );

// Generic stuff
int  GetNightPersonBonus();
uint GetAimApCost( int hitLocation );
uint GetAimHit( int hitLocation );
uint GetMultihex( CritterMutual& cr );

// tnf server
#ifdef __SERVER
EXPORT uint Map_GetTile( Map& map, uint16 tx, uint16 ty );
EXPORT uint Map_GetRoof( Map& map, uint16 tx, uint16 ty );
EXPORT bool Map_SetTile( Map& map, uint16 tx, uint16 ty, uint picHash );
EXPORT bool Map_SetRoof( Map& map, uint16 tx, uint16 ty, uint picHash );
EXPORT bool Map_HasRoof( Map& map, uint16 hexX, uint16 hexY );
EXPORT int  Critter_GetSocket(Critter& cr);
EXPORT uint Critter_GetUID(Critter& cr, uint8 num);
// EXPORT uint Critter_GetItemTransferCount( Critter& cr );
EXPORT void Critter_GetIp( Critter& cr, CScriptArray* array );
EXPORT uint Critter_GetCurrentIp( Critter& cr );
#endif // __SERVER

/************************************************************************/
/*                          TNF - includes                              */
/************************************************************************/


#ifdef __SERVER
time_t GetTime( )
{
    return time( nullptr );
}
#endif

#ifdef __CLIENT
time_t ServerTime;
time_t TimeSetTime;

void trySetServerTime( time_t _time )
{
    ServerTime = _time;
    TimeSetTime = time( nullptr );
}

time_t GetTime( )
{
    return ServerTime + TimeSetTime - time( nullptr );
}

EXPORT void SetServerTime( int timepart0, int timepart1, int, ScriptString*, CScriptArray* )
{
	trySetServerTime( timepart0 | timepart1 );
}
#endif

EXPORT int64 Script_GetServerTime( )
{
    return GetTime( );
}

int GetServerHour( )
{
    tm timeinfo;
    time_t rawtime = GetTime( );
    localtime_s( &timeinfo, &rawtime );
    return timeinfo.tm_hour;
}

#ifdef __CLIENT

#include <Windows.h>

bool CBPaste( ScriptString& str )
{
    HGLOBAL hglb;
    LPTSTR  lptstr;

    if( !IsClipboardFormatAvailable( CF_TEXT ) || !OpenClipboard( NULL ) )
        return false;

    hglb = GetClipboardData( CF_TEXT );
    if( hglb )
    {
        lptstr = (LPTSTR) GlobalLock( hglb );
        if( lptstr )
        {
            str = lptstr;
            GlobalUnlock( lptstr );
        }
    }
    CloseClipboard();
    return str.length() > 0;
}

uint GetHardware()
{
    SYSTEM_INFO siSysInfo;

    // Копируем информацию о железе в структуру SYSTEM_INFO.

    GetSystemInfo( &siSysInfo );
    uint hardwareId = siSysInfo.dwOemId;

    // Отображаем содержимое структуры SYSTEM_INFO.

    /*printf("Hardware information: \n");
       printf("  OEM ID: %u\n", siSysInfo.dwOemId);
       printf("  Number of processors: %u\n",
       siSysInfo.dwNumberOfProcessors);
       printf("  Page size: %u\n", siSysInfo.dwPageSize);
       printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
       printf("  Minimum application address: %lx\n",
       siSysInfo.lpMinimumApplicationAddress);
       printf("  Maximum application address: %lx\n",
       siSysInfo.lpMaximumApplicationAddress);
       printf("  Active processor mask: %u\n",
       siSysInfo.dwActiveProcessorMask);*/

    return hardwareId;
}

#endif


struct StartCallback
{
    string module_name;
    string func_name;

    StartCallback( const char* mn, const char* fn )
    {
        module_name = mn;
        func_name = fn;
    }
};

vector< StartCallback* > startCallbacks;

void AddStartCallback( ScriptString& module, ScriptString& func )
{
    startCallbacks.push_back( new StartCallback(module.c_str(), func.c_str() ) );
    //Log("Register %s in %s\n", startCallbacks.back()->func_name.c_str(), startCallbacks.back()->module_name.c_str());
    //Log("Register %s in %s\n", func.c_str(), module.c_str());
}

void CallStartCallbacks()
{
    uint len = startCallbacks.size();
    Log("Autostart: %u callbacks registered.\n", len);

    for( uint i = 0; i < len; i++ )
    {
        StartCallback* sc = startCallbacks[ i ];
        if( sc == NULL )
            continue;

        Log("Start %s@%s... ", sc->module_name.c_str(), sc->func_name.c_str());

        asIScriptModule* module = ASEngine->GetModule( sc->module_name.c_str());
        if( module == NULL )
        {
            Log("FAIL! Can't find module.\n");
            continue;
        }

        asIScriptFunction* sfunc = module->GetFunctionByName( sc->func_name.c_str());
        if( !sfunc )
        {
            Log("FAIL! Can't find function.\n");
            continue;
        }

        Log("Ok.\n");

        asIScriptContext* ctx = ASEngine->CreateContext();
        ctx->Prepare( sfunc );

        int r = ctx->Execute();
        ctx->Release();
    }
    startCallbacks.clear();
}

/************************************************************************/
/* Initialization                                                       */
/************************************************************************/

// In this functions (DllMain and DllLoad) all global variables is NOT initialized, use FONLINE_DLL_ENTRY instead
#if defined ( FO_WINDOWS )
int __stdcall DllMain( void* module, unsigned long reason, void* reserved ) { return 1; }
#elif defined ( FO_LINUX )
void __attribute__( ( constructor ) ) DllLoad()   {}
void __attribute__( ( destructor ) )  DllUnload() {}
#endif

FONLINE_DLL_ENTRY( isCompiler )
{
    #ifdef __CLIENT
   RegisterNativeSprites( ASEngine, isCompiler );

    ASEngine->RegisterGlobalFunction( "bool CBPaste(string&)", asFUNCTION( CBPaste ), asCALL_CDECL );
    ASEngine->RegisterGlobalFunction( "uint GetHardware()", asFUNCTION( GetHardware ), asCALL_CDECL );
	ASEngine->RegisterGlobalFunction("uint GetUseApCost(CritterCl&, ItemCl&, uint8)", asFUNCTION(GetUseApCost), asCALL_CDECL);
    ASEngine->RegisterGlobalFunction("uint GetAttackDistantion(CritterCl&, ItemCl&, uint8)", asFUNCTION(GetAttackDistantion), asCALL_CDECL);
    #endif

    RegisterQmapTools( ASEngine, isCompiler );
	RegisterScriptFileSystem( );

    #ifdef __SERVER
    ASEngine->RegisterGlobalFunction( "void AddStartCallback(string&, string&)", asFUNCTION( AddStartCallback ), asCALL_CDECL );
    ASEngine->RegisterGlobalFunction( "void CallStartCallbacks()", asFUNCTION( CallStartCallbacks ), asCALL_CDECL );
	ASEngine->RegisterGlobalFunction("uint GetUseApCost(Critter&, Item&, uint8)", asFUNCTION(GetUseApCost), asCALL_CDECL);
    ASEngine->RegisterGlobalFunction("uint GetAttackDistantion(Critter&, Item&, uint8)", asFUNCTION(GetAttackDistantion), asCALL_CDECL);
    #endif

    if( isCompiler )
        return;

    // Register callbacks
    FOnline->GetUseApCost = &GetUseApCost;
    FOnline->GetAttackDistantion = &GetAttackDistantion;

    // Register script global vars
    memset( &GlobalVars, 0, sizeof( GlobalVars ) );
    for( asUINT i = 0; i < ASEngine->GetGlobalPropertyCount(); i++ )
    {
        const char* name;
        void*       ptr;
        if( ASEngine->GetGlobalPropertyByIndex( i, &name, NULL, NULL, NULL, NULL, &ptr ) < 0 )
            continue;

        #define REGISTER_GLOBAL_VAR( type, gvar ) \
            else if( !strcmp( # gvar, name ) )    \
                GlobalVars.gvar = (type*) ptr
        REGISTER_GLOBAL_VAR( int, CurX );
        REGISTER_GLOBAL_VAR( int, CurY );
        REGISTER_GLOBAL_VAR( uint, HitAimEyes );
        REGISTER_GLOBAL_VAR( uint, HitAimHead );
        REGISTER_GLOBAL_VAR( uint, HitAimGroin );
        REGISTER_GLOBAL_VAR( uint, HitAimTorso );
        REGISTER_GLOBAL_VAR( uint, HitAimArms );
        REGISTER_GLOBAL_VAR( uint, HitAimLegs );
        REGISTER_GLOBAL_VAR( uint, __AllowRealDayTime  );
    }
	
	// #ifdef __CLIENT
		// else
		// if
		// (!strcmp("__CurrentAim", name))
			// ClientGlobals.__CurrentAim = (uint8*)ptr;
	// #endif
}

/************************************************************************/
/* Slot/parameters allowing                                             */
/************************************************************************/

/*EXPORT bool allowSlot_Hand1( uint8, Item&, Critter&, Critter& toCr )
{
    return toCr.Params[ PE_AWARENESS ] != 0;
}*/

/************************************************************************/
/* Parameters Get behaviors                                             */
/************************************************************************/

#define STAT_MAX_VAL	15

EXPORT int getParam_Strength( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_STRENGTH ] + cr.Params[ ST_STRENGTH_EXT ];
    return CLAMP( val, 1, STAT_MAX_VAL );
}

EXPORT int getParam_Perception( CritterMutual& cr, uint )
{
    int val = ( cr.Params[ DAMAGE_EYE ] ? 1 : cr.Params[ ST_PERCEPTION ] + cr.Params[ ST_PERCEPTION_EXT ] );
    if( cr.Params[ TRAIT_NIGHT_PERSON ] )
        val += GetNightPersonBonus();
    return CLAMP( val, 1, STAT_MAX_VAL );
}

EXPORT int getParam_Endurance( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_ENDURANCE ] + cr.Params[ ST_ENDURANCE_EXT ];
    return CLAMP( val, 1, STAT_MAX_VAL );
}

EXPORT int getParam_Charisma( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_CHARISMA ] + cr.Params[ ST_CHARISMA_EXT ];
    return CLAMP( val, 1, STAT_MAX_VAL );
}

EXPORT int getParam_Intellegence( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_INTELLECT ] + cr.Params[ ST_INTELLECT_EXT ];
    if( cr.Params[ TRAIT_NIGHT_PERSON ] )
        val += GetNightPersonBonus();
    return CLAMP( val, 1, STAT_MAX_VAL );
}

EXPORT int getParam_Agility( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_AGILITY ] + cr.Params[ ST_AGILITY_EXT ];
    return CLAMP( val, 1, STAT_MAX_VAL );
}

EXPORT int getParam_Luck( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_LUCK ] + cr.Params[ ST_LUCK_EXT ];
    return CLAMP( val, 1, STAT_MAX_VAL );
}

EXPORT int getParam_Hp( CritterMutual& cr, uint )
{
    return cr.Params[ ST_CURRENT_HP ];
}

EXPORT int getParam_MaxLife( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_MAX_LIFE ] + cr.Params[ ST_MAX_LIFE_EXT ] + cr.Params[ ST_STRENGTH ] * 3 + cr.Params[ ST_ENDURANCE ] * 5;
	if( cr.Params[ TRAIT_SEX_APPEAL ] )
	{
		val -= 50;
	}
	
    return CLAMP( val, 1, 9999 );
}

EXPORT int getParam_MaxAp( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_ACTION_POINTS ] + cr.Params[ ST_ACTION_POINTS_EXT ] + APPOINTS_BASE;
    return CLAMP( val, 1, 9999 );
}

EXPORT int getParam_Ap( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_CURRENT_AP ];
    val /= AP_DIVIDER;
    return CLAMP( val, -9999, 9999 );
}

EXPORT int getParam_RegenAp( CritterMutual& cr, uint )
{
    if( cr.Params[ CR_SLEEPING_STATE ] > 0 )
    {
        return 0;
    }
	
    int val = APREGEN_BASE + cr.Params[ ST_APREGEN ] + cr.Params[ ST_APREGEN_EXT ];
    val += getParam_Agility( cr, 0 ) * APREGEN_PER_AGI;
    val += getParam_Endurance( cr, 0 ) * APREGEN_PER_END;
	
	if( cr.Params[ TRAIT_KAMIKAZE ] )
	{
		val += cr.Params[ ST_AGILITY ] * KAMIKAZE_AP_REGEN_BONUS;
	}
	
	if( cr.Params[ CR_STUNNED ] )
	{
		val /= STUNNED_AP_REGEN_MALUS;
	}
	
	val -= CLAMP( cr.Params[ ST_POISONING_LEVEL ] / 3, 0, 900 );
	
    return CLAMP( val, 0, APREGEN_MAX );
}

EXPORT int getParam_MaxMoveAp( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_MAX_MOVE_AP ];
    return CLAMP( val, 0, 9999 );
}

EXPORT int getParam_MoveAp( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_MOVE_AP ];
    return CLAMP( val, 0, 9999 );
}


EXPORT int getParam_MaxWeight(CritterMutual& cr, uint)
{
	
	int val = max( cr.Params[ ST_CARRY_WEIGHT ] + cr.Params[ ST_CARRY_WEIGHT_EXT ], 0 );
    val += 1000 * ( CW_BASE + (cr.Params[ ST_STRENGTH ] * ( CW_PER_STR - cr.Params[ TRAIT_SMALL_FRAME ] * SMALL_FRAME_CW_MALUS_DIV ) ) );
	
    return CLAMP(val, 0, 2000000000);
}



/*	//Рассчет скоростей на ходу.
   #ifdef __SERVER
        if(FOnline->GameTimeTick < cr.PrevHexTick+2000)
        {
                int weight = cr.GetItemsWeight();

                int oldWalk = cr.Params[MODE_NO_WALK],
                walk = 0,
                oldRun = cr.Params[MODE_NO_RUN],
                run = 0;

                if(weight>val) walk=1;
                if(weight*2>val) run=1;

                if(walk!=oldWalk)
                {
                        cr.Params[MODE_NO_WALK]=walk;
                        cr.ParamsChanged.push_back(MODE_NO_WALK);
                        cr.ParamsIsChanged[MODE_NO_WALK]=true;
                        FOnline->CritterChangeParameter(cr, MODE_NO_WALK);
                }
                if(run!=oldRun)
                {
                        cr.Params[MODE_NO_RUN]=run;
                        cr.ParamsChanged.push_back(MODE_NO_RUN);
                        cr.ParamsIsChanged[MODE_NO_RUN]=true;
                        FOnline->CritterChangeParameter(cr, MODE_NO_RUN);
                }
        }
   #endif
 */

EXPORT int getParam_Sequence( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_SEQUENCE ] + cr.Params[ ST_SEQUENCE_EXT ] + getParam_Perception( cr, 0 ) * 2;
    return CLAMP( val, 0, 9999 );
}

EXPORT int getParam_MeleeDmg( CritterMutual& cr, uint )
{
    int strength = getParam_Strength( cr, 0 );
    int val = cr.Params[ TRAIT_NERD ] == 0 ? ( cr.Params[ ST_MELEE_DAMAGE ] + cr.Params[ ST_MELEE_DAMAGE_EXT ] + MELEE_DAMAGE_BASE + cr.Params[ ST_STRENGTH ] * MELEE_DAMAGE_PER_STR ) : 0;
    return CLAMP( val, 0, 9999 );
}

EXPORT int getParam_HealingRate( CritterMutual& cr, uint )
{
    int e = getParam_Endurance( cr, 0 );
    int val = cr.Params[ ST_HEALING_RATE ] + cr.Params[ ST_HEALING_RATE_EXT ] + max( 1, e / 3 );
    return CLAMP( val, 0, 9999 );
}

EXPORT int getParam_CriticalChance( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_CRITICAL_CHANCE ] + cr.Params[ ST_CRITICAL_CHANCE_EXT ] + getParam_Luck( cr, 0 );
    return CLAMP( val, 0, 100 );
}

EXPORT int getParam_MaxCritical( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_MAX_CRITICAL ] + cr.Params[ ST_MAX_CRITICAL_EXT ];
    return CLAMP( val, -100, 100 );
}

EXPORT int getParam_Ac( CritterMutual& cr, uint )
{
    int val = cr.Params[ TRAIT_KAMIKAZE ] ? 0 : cr.Params[ ST_ARMOR_CLASS ] + cr.Params[ ST_ARMOR_CLASS_EXT ] + getParam_Agility( cr, 0 ) * 5;
	if( cr.Params[ CR_DAZED ] )
	{
		val -= DAZED_AC_MALUS;
	}
	
    const Item* armor = cr.ItemSlotArmor;
    if( armor->GetId() && armor->IsArmor() )
	{
        val -= armor->Proto->Armor_AC;
    }

	return val;
}

EXPORT int getParam_DamageResistance( CritterMutual& cr, uint index )
{
    int         dmgType = index - ST_NORMAL_RESIST + 1;

    const Item* armor = cr.ItemSlotArmor;
    int         val = 0;
    int         drVal = 0;
    switch( dmgType )
    {
    case DAMAGE_NORMAL:
        val = cr.Params[ ST_NORMAL_RESIST ]  + cr.Params[ ST_NORMAL_RESIST_EXT ];
        drVal = armor->Proto->Armor_DRNormal;
        break;
    case DAMAGE_LASER:
        val = cr.Params[ ST_LASER_RESIST ]   + cr.Params[ ST_LASER_RESIST_EXT ];
        drVal = armor->Proto->Armor_DRLaser;
        break;
    case DAMAGE_FIRE:
        val = cr.Params[ ST_FIRE_RESIST ]    + cr.Params[ ST_FIRE_RESIST_EXT ];
        drVal = armor->Proto->Armor_DRFire;
        break;
    case DAMAGE_PLASMA:
        val = cr.Params[ ST_PLASMA_RESIST ]  + cr.Params[ ST_PLASMA_RESIST_EXT ];
        drVal = armor->Proto->Armor_DRPlasma;
        break;
    case DAMAGE_ELECTR:
        val = cr.Params[ ST_ELECTRO_RESIST ] + cr.Params[ ST_ELECTRO_RESIST_EXT ];
        drVal = armor->Proto->Armor_DRElectr;
        break;
    case DAMAGE_EMP:
        val = cr.Params[ ST_EMP_RESIST ]     + cr.Params[ ST_EMP_RESIST_EXT ];
        drVal = armor->Proto->Armor_DREmp;
        break;
    case DAMAGE_EXPLODE:
        val = cr.Params[ ST_EXPLODE_RESIST ] + cr.Params[ ST_EXPLODE_RESIST_EXT ];
        drVal = armor->Proto->Armor_DRExplode;
        break;
    case DAMAGE_UNCALLED:
    default:
        break;
    }

    if( armor->GetId() && armor->IsArmor() )
        val += drVal * ( 100 -( armor-> GetDeteriorationProc() / DETORIORATION_MODIFIER ) ) / 100;

    if( dmgType == DAMAGE_EMP )
        return CLAMP( val, 0, 999 );
    return CLAMP( val, 0, 90 );
}

EXPORT int getParam_DamageThreshold( CritterMutual& cr, uint index )
{
    int         dmgType = index - ST_NORMAL_ABSORB + 1;

    const Item* armor = cr.ItemSlotArmor;
    int         val = 0;
    int         dtVal = 0;
    switch( dmgType )
    {
    case DAMAGE_NORMAL:
        val = cr.Params[ ST_NORMAL_ABSORB ]  + cr.Params[ ST_NORMAL_ABSORB_EXT ];
        dtVal = armor->Proto->Armor_DTNormal;
        break;
    case DAMAGE_LASER:
        val = cr.Params[ ST_LASER_ABSORB ]   + cr.Params[ ST_LASER_ABSORB_EXT ];
        dtVal = armor->Proto->Armor_DTLaser;
        break;
    case DAMAGE_FIRE:
        val = cr.Params[ ST_FIRE_ABSORB ]    + cr.Params[ ST_FIRE_ABSORB_EXT ];
        dtVal = armor->Proto->Armor_DTFire;
        break;
    case DAMAGE_PLASMA:
        val = cr.Params[ ST_PLASMA_ABSORB ]  + cr.Params[ ST_PLASMA_ABSORB_EXT ];
        dtVal = armor->Proto->Armor_DTPlasma;
        break;
    case DAMAGE_ELECTR:
        val = cr.Params[ ST_ELECTRO_ABSORB ] + cr.Params[ ST_ELECTRO_ABSORB_EXT ];
        dtVal = armor->Proto->Armor_DTElectr;
        break;
    case DAMAGE_EMP:
        val = cr.Params[ ST_EMP_ABSORB ]     + cr.Params[ ST_EMP_ABSORB_EXT ];
        dtVal = armor->Proto->Armor_DTEmp;
        break;
    case DAMAGE_EXPLODE:
        val = cr.Params[ ST_EXPLODE_ABSORB ] + cr.Params[ ST_EXPLODE_ABSORB_EXT ];
        dtVal = armor->Proto->Armor_DTExplode;
        break;
    case DAMAGE_UNCALLED:
    default:
        break;
    }

    if( armor->GetId() && armor->IsArmor() )
        val += dtVal * ( 100 - ( armor-> GetDeteriorationProc() / DETORIORATION_MODIFIER ) ) / 100;

    return CLAMP( val, 0, 999 );
}

EXPORT int getParam_RadiationResist( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_RADIATION_RESISTANCE ] + cr.Params[ ST_RADIATION_RESISTANCE_EXT ] + getParam_Endurance( cr, 0 ) * 2;
    return CLAMP( val, 0, 95 );
}

EXPORT int getParam_PoisonResist( CritterMutual& cr, uint )
{
    int val = cr.Params[ ST_POISON_RESISTANCE ] + cr.Params[ ST_POISON_RESISTANCE_EXT ] + getParam_Endurance( cr, 0 ) * 5;
    return CLAMP( val, 0, 95 );
}

EXPORT int getParam_Timeout( CritterMutual& cr, uint index )
{
    return (uint) cr.Params[ index ] > FOnline->FullSecond ? (uint) cr.Params[ index ] - FOnline->FullSecond : 0;
}

EXPORT int getParam_Reputation( CritterMutual& cr, uint index )
{
    #ifdef __SERVER
    if( cr.Params[ index ] == 0x80000000 )
    {
        FOnline->CritterChangeParameter( cr, index );
        const_cast< int* >( cr.Params )[ index ] = 0;
    }
    #else
    if( cr.Params[ index ] == 0x80000000 )
        return 0;
    #endif
    return cr.Params[ index ];
}

EXPORT void changedParam_Reputation( CritterMutual& cr, uint index, int oldValue )
{
    if( oldValue == 0x80000000 )
        const_cast< int* >( cr.Params )[ index ] += 0x80000000;
}

/************************************************************************/
/* Extended methods                                                     */
/************************************************************************/

EXPORT bool Critter_IsInjured( CritterMutual& cr )
{
    return Critter_IsDmgArm( cr ) || Critter_IsDmgLeg( cr ) || Critter_IsDmgEye( cr );
}

EXPORT bool Critter_IsDmgEye( CritterMutual& cr )
{
    return cr.Params[ DAMAGE_EYE ] != 0;
}

EXPORT bool Critter_IsDmgLeg( CritterMutual& cr )
{
    return cr.Params[ DAMAGE_RIGHT_LEG ] || cr.Params[ DAMAGE_LEFT_LEG ];
}

EXPORT bool Critter_IsDmgTwoLeg( CritterMutual& cr )
{
    return cr.Params[ DAMAGE_RIGHT_LEG ] && cr.Params[ DAMAGE_LEFT_LEG ];
}

EXPORT bool Critter_IsDmgArm( CritterMutual& cr )
{
    return cr.Params[ DAMAGE_RIGHT_ARM ] || cr.Params[ DAMAGE_LEFT_ARM ];
}

EXPORT bool Critter_IsDmgTwoArm( CritterMutual& cr )
{
    return cr.Params[ DAMAGE_RIGHT_ARM ] && cr.Params[ DAMAGE_LEFT_ARM ];
}

EXPORT bool Critter_IsAddicted( CritterMutual& cr )
{
    for( uint i = ADDICTION_BEGIN; i <= ADDICTION_END; i++ )
        if( cr.Params[ i ] )
            return true;
    return false;
}

EXPORT bool Critter_IsOverweight( CritterMutual& cr )
{
    // Calculate inventory items weight
    uint w = 0;
    for( ItemVecIt it = cr.InvItems.begin(), end = cr.InvItems.end(); it != end; ++it )
        w += ( *it )->GetWeight();

    return w > (uint) getParam_MaxWeight( cr, 0 );
}

EXPORT bool Item_Weapon_IsHtHAttack( Item& item, uint8 mode )
{
    if( !item.IsWeapon() || !item.WeapIsUseAviable( mode & 7 ) )
        return false;
    int skill = SKILL_OFFSET( item.Proto->Weapon_Skill[ mode & 7 ] );
    return skill == SK_UNARMED || skill == SK_MELEE_WEAPONS;
}

EXPORT bool Item_Weapon_IsGunAttack( Item& item, uint8 mode )
{
    if( !item.IsWeapon() || !item.WeapIsUseAviable( mode & 7 ) )
        return false;
    int skill = SKILL_OFFSET( item.Proto->Weapon_Skill[ mode & 7 ] );
    return skill == SK_SMALL_GUNS || skill == SK_MEDIUM_GUNS || skill == SK_BIG_GUNS;
}

EXPORT bool Item_Weapon_IsRangedAttack( Item& item, uint8 mode )
{
    if( !item.IsWeapon() || !item.WeapIsUseAviable( mode & 7 ) )
        return false;
    int skill = SKILL_OFFSET( item.Proto->Weapon_Skill[ mode & 7 ] );
    return skill == SK_SMALL_GUNS || skill == SK_MEDIUM_GUNS || skill == SK_BIG_GUNS || skill == SK_THROWING;
}

EXPORT int ItemTransferCount( CritterMutual& cr )
{
    int s = 0;
    #ifdef __SERVER
    s = cr.ItemTransferCount;
    #else
    s = 0;
    #endif
    return s;
}
EXPORT int AccessContainerId( CritterMutual& cr )
{
    int s = 0;
    #ifdef __SERVER
    s = cr.AccessContainerId;
    #else
    s = 0;
    #endif
    return s;
}

/************************************************************************/
/* Callbacks                                                            */
/************************************************************************/

uint GetUseApCost( CritterMutual& cr, Item& item, uint8 mode )
{
    uint8 use = mode & 0xF;
    uint8 aim = mode >> 4;
	uint apCost = 1;

	if( use == USE_USE )
	{
		apCost = ( item.Proto -> Item_UseAp == 0 ? FOnline -> RtApCostUseItem : item.Proto -> Item_UseAp );
	}
	else if( use == USE_RELOAD )
	{
		apCost = ( item.Proto -> Weapon_ReloadAp == 0 ? FOnline -> RtApCostReloadWeapon : item.Proto -> Weapon_ReloadAp );
	}
	else if( use >= USE_PRIMARY && use <= USE_THIRD && item.IsWeapon() )
	{
		int skill = item.Proto -> Weapon_Skill[ use ];
		bool hthAttack = Item_Weapon_IsHtHAttack( item, mode );
		bool rangedAttack = Item_Weapon_IsRangedAttack( item, mode );
		bool isBurst = ( item.Proto->Weapon_Round[ use ] > 1 );

		apCost = item.Proto -> Weapon_ApCost[ use ];

		if( aim )
		{
			apCost += ( apCost * GetAimApCost( aim ) ) / 100;
		}

		if( rangedAttack )
		{
			if( cr.Params[ TRAIT_FAST_SHOT ] && !hthAttack ) 
			{
				apCost = apCost * FAST_SHOT_AP_MUL / 100;
			}
		}
		
		if( cr.Params[ TRAIT_SADIST ] )
		{
			apCost = apCost * ONE_HANDER_AP_MUL / 100;
		}
	}

	if( cr.Params[ CR_ARMOR_BRISK_EQUIPPED ] > 0 )
	{
		apCost = ( apCost * ARMOR_PERK_BRISK_BONUS) / 100;
	}
	
	if( cr.Params[ CR_ARMOR_SLUGGISH_EQUIPPED ] > 0 )
	{
		apCost = ( apCost * ARMOR_PERK_SLUGGISH_MALUS) / 100;
	}
    
	if( apCost < 1 )
	{
		apCost = 1;
	}
	
	return apCost;
}

// Very limited function for calculating unarmed attack AP cost
uint GetWeaponProtoApCost(CritterMutual& cr, ProtoItem& prot, uint8 mode)
{
	uint8 use = mode & 0xF;
	uint8 aim = mode >> 4;

   	uint apCost = 1;

	if(use >= USE_PRIMARY && use <= USE_THIRD)
	{
		int skill = prot.Weapon_Skill[use];

		apCost = prot.Weapon_ApCost[use];

		if(aim)
		{
			apCost += (apCost*GetAimApCost(aim))/100;
		}
	}
    if(apCost < 1) apCost = 1;
    return apCost;
}


uint GetAttackDistantion( CritterMutual& cr, Item& item, uint8 mode )
{
    uint8 use = mode & 0xF;
    int   dist = item.Proto->Weapon_MaxDist[ use ];
    int   strength = getParam_Strength( cr, 0 );
    if( item.Proto->Weapon_Skill[ use ] == SKILL_OFFSET( SK_THROWING ) )
        dist = min( dist, 3 * min( 10, strength ) );
    if( Item_Weapon_IsHtHAttack( item, mode ) && cr.Params[ MODE_RANGE_HTH ] )
        dist++;
    dist += GetMultihex( cr );
    if( dist < 0 )
        dist = 0;
    return dist;
}

/************************************************************************/
/* Generic stuff                                                        */
/************************************************************************/

int GetNightPersonBonus( )
{
    if( *GlobalVars.__AllowRealDayTime != 1 )
    {
        if( FOnline->Hour < 6 || FOnline->Hour > 18 )
        {
            return 1;
        }
        if( FOnline->Hour == 6 && FOnline->Minute == 0 )
        {
            return 1;
        }
        if( FOnline->Hour == 18 && FOnline->Minute > 0 )
        {
            return 1;
        }
    }
    else
    {
        const int hour = GetServerHour( );
        if( hour < 6 || hour > 18 )
        {
            return 1;
        }
        if( hour == 6 && hour == 0 )
        {
            return 1;
        }
        if( hour == 18 && hour > 0 )
        {
            return 1;
        }
    }
    return -1;
}

uint GetAimApCost( int hitLocation )
{
    switch( hitLocation )
    {
    case HIT_LOCATION_TORSO:
        return FOnline->ApCostAimTorso;
    case HIT_LOCATION_EYES:
        return FOnline->ApCostAimEyes;
    case HIT_LOCATION_HEAD:
        return FOnline->ApCostAimHead;
    case HIT_LOCATION_LEFT_ARM:
    case HIT_LOCATION_RIGHT_ARM:
        return FOnline->ApCostAimArms;
    case HIT_LOCATION_GROIN:
        return FOnline->ApCostAimGroin;
    case HIT_LOCATION_RIGHT_LEG:
    case HIT_LOCATION_LEFT_LEG:
        return FOnline->ApCostAimLegs;
    case HIT_LOCATION_NONE:
    case HIT_LOCATION_UNCALLED:
    default:
        break;
    }
    return 0;
}

uint GetAimHit( int hitLocation )
{
    switch( hitLocation )
    {
    case HIT_LOCATION_TORSO:
        return *GlobalVars.HitAimTorso;
    case HIT_LOCATION_EYES:
        return *GlobalVars.HitAimEyes;
    case HIT_LOCATION_HEAD:
        return *GlobalVars.HitAimHead;
    case HIT_LOCATION_LEFT_ARM:
    case HIT_LOCATION_RIGHT_ARM:
        return *GlobalVars.HitAimArms;
    case HIT_LOCATION_GROIN:
        return *GlobalVars.HitAimGroin;
    case HIT_LOCATION_RIGHT_LEG:
    case HIT_LOCATION_LEFT_LEG:
        return *GlobalVars.HitAimLegs;
    case HIT_LOCATION_NONE:
    case HIT_LOCATION_UNCALLED:
    default:
        break;
    }
    return 0;
}

uint GetMultihex( CritterMutual& cr )
{
    int mh = cr.Multihex;
    if( mh < 0 )
        mh = FOnline->CritterTypes[ cr.BaseType ].Multihex;
    return CLAMP( mh, 0, MAX_HEX_OFFSET );
}


/************************************************************************/
/*                         TNF - common                                 */
/************************************************************************/

EXPORT void Item_SetInvPic( Item& item, uint hash )
{
    const_cast< uint& >( item.Data.PicInvHash ) = hash;
    return;
}

EXPORT void Item_SetMapPic( Item& item, uint hash )
{
    const_cast< uint& >( item.Data.PicMapHash ) = hash;
    return;
}

/************************************************************************/
/*                         TNF - server                                 */
/************************************************************************/

#ifdef __SERVER

struct ClientEx : Client
{
	uint UID[5];
	volatile int Sock; // in fact, SOCKET
	sockaddr_in From;
};



EXPORT int Critter_GetSocket(Critter& cr)
{
    if (cr.CritterIsNpc) return 0;
    return ((ClientEx&)cr).Sock;
}


EXPORT uint Critter_GetUID(Critter& cr, uint8 num)
{
    if (cr.CritterIsNpc) return 0;
    return ((ClientEx&)cr).UID[num];
}



uint GetTiles( Map& map, uint16 hexX, uint16 hexY, bool is_roof, vector< uint >& finded )
{
    ProtoMap::TileVec& tiles = const_cast< ProtoMap::TileVec& >( map.Proto->Tiles );

    for( uint i = 0, j = tiles.size(); i < j; i++ )
    {
        if( tiles[ i ].HexX != hexX || tiles[ i ].HexY != hexY || tiles[ i ].IsRoof != is_roof )
            continue;
        finded.push_back( tiles[ i ].NameHash );
    }
    return finded.size();
}

EXPORT uint Map_GetTiles( Map& map, uint16 hexX, uint16 hexY, bool is_roof, CScriptArray& array )
{
    vector< uint > finded;

    if( array.GetElementSize() != 4 )
        return 0;

    uint delta = GetTiles( map, hexX, hexY, is_roof, finded );
    if( delta == 0 )
        return 0;

    uint size = array.GetSize();
    array.Resize( size+delta );
	for( uint i = size, iend = size+delta; i < iend; i++ )
		array.InsertAt( i, &finded[i - size] );

    return delta;
}

EXPORT uint Map_GetTile( Map& map, uint16 tx, uint16 ty )
{
    vector< uint > finded;
    if( GetTiles( map, tx * 2, ty * 2, false, finded ) != 1 )
        return 0;
    return finded[ 0 ];
}

EXPORT uint Map_GetRoof( Map& map, uint16 tx, uint16 ty )
{
    vector< uint > finded;

    if( GetTiles( map, tx * 2, ty * 2, true, finded ) != 1 )
        return 0;

    return finded[ 0 ];
}

// Returns true if map cell {hexX,hexY} is under roof
EXPORT bool Map_HasRoof( Map& map, uint16 hexX, uint16 hexY )
{
	ProtoMap::TileVec & tiles = const_cast < ProtoMap::TileVec & > ( map.Proto -> Tiles );
	for( uint i = 0, length = tiles.size(); i < length; i++ )
	{
    	if( ( ( tiles[ i ].HexX == hexX) || ( tiles[ i ].HexX == hexX + 1 ) ) // Roof tile may have size 2x2
    		&& ( ( tiles[ i ].HexY == hexY) || ( tiles[ i ].HexY == hexY + 1 ) )
    		&& tiles[ i ].IsRoof )
		{
			return true;
		}
    }
    return false;
}

EXPORT bool Map_SetTile( Map& map, uint16 tx, uint16 ty, uint picHash )
{
    // if(map.IsNotValid) return 0;
    // ProtoMap* pMap = map.Proto;
    // if(pMap->(Header.MaxHexX/2)<tx || pMap->(Header.MaxHexY/2)<ty) return 0;
    // pMap->SetTile(tx, ty, picHash);
    // return true;
    return false;
}

EXPORT bool Map_SetRoof( Map& map, uint16 tx, uint16 ty, uint picHash )
{
    // if(map.IsNotValid) return 0;
    // ProtoMap* pMap = map.Proto;
    // if(pMap->(Header.MaxHexX/2)<tx || pMap->(Header.MaxHexY/2)<ty) return 0;
    // pMap->SetRoof(tx, ty, picHash);
    // return true;
    return false;
}

EXPORT void Critter_GetIp( Critter& cr, CScriptArray* array )
{
    array->Resize( MAX_STORED_IP );
    for( uint i = 0; i < MAX_STORED_IP; i++ )
		array->InsertAt( i, &(const_cast<uint&>(cr.DataExt->PlayIp[i])) );
}

EXPORT uint Critter_GetCurrentIp( Critter& cr )
{
	if( cr.CritterIsNpc )
		return( 0 );
	else
	{
		uint idx = cr.DataExt->CurrentIp;
		if( idx >= MAX_STORED_IP )
			return( 0 );
		else
			return( cr.DataExt->PlayIp[idx] );
	}
}

EXPORT void Critter_SetWorldPos( CritterMutual& cr, uint16 x, uint16 y ) // pm added
{
    const_cast< uint16& >( cr.WorldX ) = x;
    const_cast< uint16& >( cr.WorldY ) = y;
}

EXPORT int Map_GetScenParam( Map& map, uint16 tx, uint16 ty, uint protoId, uint8 num )
{
    if( num < 0 || num > 10 )
        return -1;

    const MapObjectVec& obj = map.Proto->SceneriesVec;

    for( MapObjectVecIt it = obj.begin(); it < obj.end(); ++it )
    {
        if( ( *it )->MapX != tx || ( *it )->MapY != ty || ( *it )->ProtoId != protoId )
            continue;
        return ( *it )->MScenery.Param[ num ];
    }

    return -1;
}

#endif // __SERVER

#ifdef __CLIENT

EXPORT int Map_GetScenParam( uint16 tx, uint16 ty, uint protoId, uint8 num )
{
    if( num < 0 || num > 10 )
        return -1;

    uint    width = FOnline->ClientMapWidth;
    Field   field = FOnline->ClientMap[ ty * width + tx ];

    ItemVec items = field.Items;

    for( ItemVecIt it = items.begin(); it < items.end(); ++it )
    {
        const ProtoItem* proto = ( *it )->Proto;
        if( proto->ProtoId != protoId || ( *it )->Accessory != 2 )
            continue;                                                            // 2 == ACCESSORY_HEX

        return proto->StartValue[ num ];
    }

    return -1;
}

EXPORT uint8 Item_GetDurability( Item& item )
{
    return item.Data.Rate;
}

#endif // MAIN

#endif // __CLIENT
