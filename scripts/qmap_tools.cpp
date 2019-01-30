#include "qmap_tools.h"

inline bool DMO64_ChechDataSize( ScriptArray* array, uint tile )
{
    return ( ( array->GetElementSize() <= 8 ) && ( ( array->GetSize() * array->GetElementSize() ) >= ( tile + 1 ) * 8 ) );
}

bool DMO64_get( ScriptArray* array, uint tile, uint16& hs, uint16& val, uint16& hexX, uint16& hexY, uint8& layer )
{
    // if(data.length()<tile*2+2) return false;
    if( !DMO64_ChechDataSize( array, tile ) )
        return false;

    // uint8* data = &(array->GetBuffer());
    // uint p = (uint)(data+tile*8);

    uint8* p = ( array->GetBuffer() + tile * 8 );

    hs = *( (uint16*) ( p ) );

    // hs_1=uint16(data[tile*2]&0xFFFF);
    val = *( (uint16*) ( p + 2 ) );

    // hs_2=uint16((data[tile*2]>>16)&0xFFFF);
    hexX = ( *( (uint16*) ( p + 4 ) ) ) & 0xFFF;

    // hexX=uint16(data[tile*2+1]&0xFFF);

    // hexY = (*((uint16*)(p+44)))&0xFFF;
    hexY = ( ( *( (uint16*) ( p + 5 ) ) ) >> 4 ) & 0xFFF;

    // hexY=uint16((data[tile*2+1]>>12)&0xFFF);
    layer = ( *( (uint8*) ( p + 7 ) ) );
    // layer=uint8((data[tile*2+1]>>24)&0xFF);

    return true;
}

/*bool DMO64_get(ScriptArray* array, uint tile, uint16& hs, uint16& val)
   {
        if(!DMO64_ChechDataSize(array, tile)) return false;

        uint8* p = array->GetBuffer()+tile*8;

        hs = *((uint16*)(p));
        val = *((uint16*)(p+2));

        //hs_1=uint16(data[tile*2]&0xFFFF);
        //hs_2=uint16((data[tile*2]>>16)&0xFFFF);

        return true;
   }*/

/*bool DMO64_getTileFromData(ScriptArray* array, uint tile, uint16& hs)
   {
        if(!DMO64_ChechDataSize(array, tile)) return false;

        hs = *((uint16*)(&(array->GetBuffer())+tile*8));

        return true;
   }

   uint DMO64_getHSFromData(ScriptArray* array, uint tile)
   {
        if(!DMO64_ChechDataSize(array, tile)) return 0;

        uint hs = *((uint16*)(&(array->GetBuffer())+tile*8);

        return hs;
   }

   bool DMO64_checkTileData(ScriptArray* array)
   {
        uint hs = DMO64_getHSFromData(array, 0);
        return (hs!=0);
   }

   bool DMO64_checkTileData(ScriptArray* array, uint tile)
   {
        uint hs = DMO64_getHSFromData(array, tile);
        return (hs!=0);
   }
 */

bool DMO64_insertLast( ScriptArray* array, uint16 hs, uint16 val, uint hex )
{
    // if(!checkTileData(hs_1, hs_2, hexX, hexY)) return false;

    uint len = array->GetSize() * array->GetElementSize();
    if( len % 8 != 0 )
        return false;

    array->Resize( ( len + 8 ) / array->GetElementSize() );

    uint8* p = array->GetBuffer() + len;

    *( (uint16*) p ) = hs;
    *( (uint16*) ( p + 2 ) ) = val;
    *( (uint*) ( p + 4 ) ) = hex;

    /*data[p]=int(hs_1)&0xFFFF;
       data[p++]|=(int(hs_2)&0xFFFF)<<16;
       data[p]=int(hexX)&0xFFF;
       data[p]|=(int(hexY)&0xFFF)<<12;
       data[p++]|=(int(layer)&0xFF)<<24;*/

    return true;
}

bool DMO64_insertLast( ScriptArray* array, ScriptArray* from_array )
{
    uint len = array->GetSize() * array->GetElementSize();
    uint len2 = from_array->GetSize() * from_array->GetElementSize();

    if( len % 8 != 0 || len2 == 0 || len2 % 8 != 0 )
        return false;

    array->Resize( ( len + len2 ) / array->GetElementSize() );

    memcpy( array->GetBuffer() + len, from_array->GetBuffer(), len2 );

    return true;
}

/*bool DMO64_copyTileData(ScriptArray* array, ScriptArray* from_array, uint tile)
   {
        //if(!DMO64_ChechDataSize(from_array, tile)) return false;

        uint len=array->GetSize()*array->GetElementSize();
        uint len2=from_array->GetSize()*from_array->GetElementSize();

        if(len%8!=0 || len2==0 || len2%8!=0) return false;

        array->Resize((len+8)/array->GetElementSize());

        memcpy(array->GetBuffer()+len, from_array->GetBuffer()+tile*8, len2);

        return true;
   }*/

/*bool DMO64_setData(ScriptArray* array, uint tile, uint16 hs, uint16 val, uint hex)
   {
        //if(!DMO64_ChechDataSize(array, tile)) return false;
        uint len=array->GetSize()*array->GetElementSize();

        if(len%8!=0) return false;

        if(hs+1==0 && hex+1==0) return false;

        uint p = (uint)((&(array->GetBuffer()))+tile*8);

        if(hs+1!=0) *((uint16*)p) = hs;
        if(val+1!=0) *((uint16*)p+16) = val;

        if(hex+1!=0) *((uint*)p+32) = hex;
        return true;
   }*/

int uint16ToInt( uint16 val1, uint16 val2 )
{
    int t = 0;
    _asm {
        mov ax, val2
        sal             eax, 16
        mov             ax, val1
        mov             t, eax
    }
    /*_asm{
            mov		eax, [t]
            mov		bx,	val1
            mov		word ptr [eax], bx
            mov		bx,	val2
            mov		word ptr [eax+4], bx
       }*/
    return t;
}

uint DMO64_makeHex( uint16 hexX, uint16 hexY, uint8 layer )
{
    uint t;
    _asm {
        xor             eax,    eax
        mov             ah,             layer
        sal             eax,    4
        mov             bx,             hexY
        and             bx,             0xFFF
        or              ax,             bx
        sal             eax,    12
        mov             bx,             hexX
        and             bx,             0xFFF
        or              ax,             bx
        mov             t,              eax
    }
    return t;
}

int DMO64_search( ScriptArray* array, uint hex )
{
    uint* p = (uint*) ( array->GetBuffer() + 4 );

    for( uint i = 0, len = ( array->GetSize() * array->GetElementSize() ) / 8; i < len; i++ )
    {
        if( ( *p ) == hex )
            return i;
        p += 2;
    }
    return -1;
}

int DMO64_search( ScriptArray* array, uint hex, uint begin, uint end )
{
    uint* p = (uint*) ( array->GetBuffer() + begin * 8 + 4 );

    for( uint i = begin; i < end; i++ )
    {
        if( ( *p ) == hex )
            return i;
        p += 2;
    }
    return -1;
}

bool DMO64_set( ScriptArray* array, uint16 hs, uint16 val, uint hex, bool rewrite )
{
    if( array->GetElementSize() > 8 ||
        hex == 0 )
        return false;                      // если гекс невалидный - выходим

    int tile = DMO64_search( array, hex ); // проверяем есть ли уже тайл с такими координатами

    if( tile != -1 )                       // тайл с такими координатами уже есть
    {
        if( !rewrite )
            return false;                  // если перезапись старых тайлов запрещена - выходим

        if( hs == 0 )                      // если номер хэша нулевой, то удаляем тайл
        {
            int nofe = 8 / array->GetElementSize();
            array->ResizeAt( -nofe, nofe * tile );
        }

        else
            *( (uint*) ( array->GetBuffer() + 8 * tile ) ) = hs;  // иначе изменяем номер хэша для тайла
    }
    else                                                          // иначе пишем новый тайл
    {
        if( hs == 0 ||                                            // если номер хэша или одна из координат нулевая - выходим
            !DMO64_insertLast( array, hs, val, hex ) )            // добавляем в массив новый тайл
            return false;
    }

    return true;
}

bool DMO64_add( ScriptArray* array, ScriptArray* from_array, bool rewrite )
{
    if( array->GetElementSize() > 8 || from_array->GetElementSize() > 8 )
        return false;

    int  nofe = 8 / array->GetElementSize();

    uint len = array->GetSize() * array->GetElementSize(),
         num = len >> 3, // len/8
         len2 = from_array->GetSize() * from_array->GetElementSize(),
         num2 = len2 >> 3;

    if( len % 8 != 0 || num2 == 0 || len2 % 8 != 0 )
        return false;

    array->Resize( ( num + num2 ) * nofe ); // grow(from_array.length());

    int    tile = 0;
    uint   hex = 0;
    uint16 hs = 0;

    uint8* p2 = from_array->GetBuffer(),
    * p = array->GetBuffer(),
    * p1 = p + len;

    for( uint i = 0; i < num2; i++ )
    {
        hex = *( (uint*) ( p2 + 4 ) );
        if( hex != 0 )
        {
            tile = DMO64_search( array, hex, 1, num );

            // if(!getTileFromData(data, tile, hs_1, hs_2)) return false;

            if( tile != -1 )
            {
                if( rewrite )
                    memcpy( p + tile * nofe, p2, 8 );

                // hs = *((uint16*)(p);

                // if(hs==0) {array->ResizeAt(-nofe, nofe*tile); num--;}

                // if(setTileData(data, tile, hs_1, hs_2, -1, -1, -1)) return false;
            }
            else
            {
                hs = *( (uint16*) ( p2 ) );
                if( hs != 0 )
                {
                    memcpy( p1, p2, 8 );
                    p1 += 8;
                }
            }
        }
        p2 += 8;
    }

    array->Resize( ( p1 - p ) / array->GetElementSize() );

    return true;
}

uint DMO64_getHashNum( ScriptArray* array, uint hex )
{
    int tile = DMO64_search( array, hex );
    if( tile == -1 )
        return 0;
    return *( (uint16*) ( ( array->GetBuffer() ) + tile * 8 ) );
}

void RegisterQmapTools( asIScriptEngine* engine, bool compiler )
{
    int r;

    r = engine->RegisterGlobalFunction( "bool DMO64_get(int[]&, uint, uint16&, uint16&, uint16&, uint16&, uint8&)", asFUNCTION( DMO64_get ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "bool DMO64_insertLast(int[]&, int[]&)", asFUNCTIONPR( DMO64_insertLast, ( ScriptArray *, ScriptArray* ), bool ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "uint DMO64_makeHex(uint16, uint16, uint8)", asFUNCTION( DMO64_makeHex ), asCALL_CDECL );
    r = engine->RegisterGlobalFunction( "int uint16ToInt(uint16, uint16)", asFUNCTION( uint16ToInt ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "bool DMO64_set(int[]&, uint16, uint16, uint, bool)", asFUNCTIONPR( DMO64_set, ( ScriptArray *, uint16, uint16, uint, bool ), bool ), asCALL_CDECL );
    r = engine->RegisterGlobalFunction( "bool DMO64_add(int[]&, int[]&, bool)", asFUNCTIONPR( DMO64_add, ( ScriptArray *, ScriptArray *, bool ), bool ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "uint DMO64_getHashNum(int[]&, uint)", asFUNCTION( DMO64_getHashNum ), asCALL_CDECL );

    if( compiler )
        return;
}
