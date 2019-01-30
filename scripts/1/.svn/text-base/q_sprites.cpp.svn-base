#include "q_sprites.h"
#include "revenge.h"

// #define DISABLE_NEW

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
//#include <D3DX9Effect.h>


//IDirect3D9**         direct3d           = ;
//IDirect3DDevice9**   direct3d_device    = ;

void* ENGINE_PTR_METHOD_InsertSprite;
void* ENGINE_PTR_METHOD_AnimLoad;

//void* ENGINE_PTR_METHOD_DrawPoints = 0x0051ECB0;

//LoadEffect

struct UPrepPoint
{
    int16 x, y;
    uint un1, un2;
    D3DCOLOR color;

    UPrepPoint()
    {
        x=0; y=0;
        un1=0; un2=0;
        color=0;
    }
    void Set(uint16 ax, uint16 ay, D3DCOLOR acolor)
    {
        x=ax; y=ay; color=acolor;
    }
};

typedef vector<UPrepPoint> UPrepPointVec;

struct CSpriteManager
{
    uint v1;
    uint v2;
    uint v3;
    IDirect3D9* direct3d;
    IDirect3DDevice9*   direct3d_device;

    int DrawPoints(UPrepPointVec* vec, D3DPRIMITIVETYPE ptype, void* p1, uint v2, uint v3)
    {
        int retval=0;
        _asm {
            push v3
            push v2
            push p1
            push ptype
            push vec
            mov ecx, this
            mov eax, 0x0051ECB0
            call eax
            mov retval, eax
        }
        return retval;
    }
    void Flush()
    {
        _asm {
            mov ecx, this
            mov eax, 0x0051E660
            call eax
        }
    }
} *SpriteManager = (CSpriteManager*)0x2F85510;

//int CSpriteManager::DrawPoints(void* vec, D3DPRIMITIVETYPE ptype, void* p1, uint v2, uint v3) = ;

typedef vector< Sprite* >           SprVec;
typedef vector< Sprite* >::iterator SprVecIt;

struct SAnim          // AnyFrames
{
    uint*   SprId;
    uint16* p2;       // offsX?			//+0x4
    uint16* p3;       // offsY?			//+0x8
    uint    SprCount; // max sprIndex + 1	//+0xC
    uint    CurSpr;   // +0x10
};

typedef vector< SAnim** >           AnimVec;
typedef vector< SAnim** >::iterator AnimVecIt;

struct CSprites
{
    SprVec  SpritesVec;
    uint    ValidSprites;

    Sprite* InsertSprite( int drawOrderType, int hexX, int hexY, uint un4, int scrX, int scrY, uint sprId, uint un8, uint un9, uint un10, uint un11, uint un12 )
    {
        Sprite* spr = NULL;
        _asm {
            mov eax, ENGINE_PTR_METHOD_InsertSprite
            mov ecx, this
            push    un12
            push    un11
            push    un10
            push    un9
            push    un8
            push    sprId
            push    scrY
            push    scrX
            push    un4
            push    hexY
            push    hexX
            push    drawOrderType
            call eax
            mov spr, eax
        }
        return spr;
    }
};

struct CAnims
{
    AnimVec VAnims;

    uint    AnimLoad( uint hash, uint dir, uint val )
    {
        uint animId = 0;
        _asm {
            mov eax, ENGINE_PTR_METHOD_AnimLoad
            mov ecx, this
            push    val
            push    dir
            push    hash
            call eax
            mov animId, eax
        }
        return animId;
    }
};

struct IMEC // Interface Main Engine Class
{
    void*     pFOnine;

    uint*     unnamed1;
    uint*     unnamed2;
    char*     pass;
    void**    p1;
    void**    p2;
    uint16*   MapWidth;
    uint16*   MapHeight;
    Field**   Map;                               // fields
    uint8**   hexValidBeginIt;
    uint8**   hexValidEndIt;
    uint**    unnamed3;
    CSprites* Sprites;
    uint*     unnamed4;
    uint*     doConnect;
    uint*     unnamed5;
    CAnims*   Anims;
} GameMain;

struct CFOnlineD3D
{
    uint     unnamed1;
    uint     unnamed2;
    char     pass[ 16 ];
    void*    p1;
    void*    p2;
    uint16   MapWidth;
    uint16   MapHeight;
    Field*   Map;             // fields
    uint8*   hexValidBeginIt;
    uint8*   hexValidEndIt;
    uint     unnamed3[ 25 ];  // d3d
    CSprites Sprites;
    uint     unnamed4[ 442 ]; // d3d
    uint     doConnect;
    uint     unnamed5[ 104 ]; // d3d
    CAnims   Anims;
}* FOnlineD3D;

struct CFOnlineOGL
{
    uint     unnamed1;
    uint     unnamed2;
    char     pass[ 16 ];
    void*    p1;
    void*    p2;
    uint16   MapWidth;
    uint16   MapHeight;
    Field*   Map;             // fields
    uint8*   hexValidBeginIt;
    uint8*   hexValidEndIt;
    uint     unnamed3[ 33 ];  // ogl
    CSprites Sprites;
    uint     unnamed4[ 558 ]; // ogl
    CAnims   Anims;
}* FOnlineOGL;

/*struct ResNode
   {
        void* p1;
        void* below;	//0x08
        void* above;	//0x0C
        uint Hash; //0x10
        uint val; //0x14
        SAnim* Anim; //0x18
   }*/

struct CResourceManager
{
    void* p1;           // +0x00		//vec?
    void* p2;           // +0x04		//vec?
    void* p3;           // +0x08		//vec?
    void* p4;           // +0x0C		//0
    void* p5;           // +0x10		//0
    void* p6;           // +0x14		//trash
    void* p7;           // +0x18		//trash
    void* p8;           // +0x1C		//0
    uint  v1;           // +0x20		//2
    // ---anims---
    void* p10;          // +0x24		//0
    void* p11;          // +0x28		//ResNodes
    void* p12;          // +0x2C
    void* p13;          // +0x30
    // void* p14;	//+0x34

    /*SAnim* GetAnim( uint hash, uint dir, uint val )
       {
        SAnim* anim = NULL;
        _asm {
            mov eax, 0x004D7E80
            mov ecx, this
            push    val
            push    dir
            push    hash
            call eax
            mov anim, eax
        }
        return anim;
       }*/
    // 004D7E80  .text     Debug data  ?GetAnim@ResourceManager@@QAEPAUAnyFrames@@IHH@Z
}* ResourceManager;

EXPORT uint GetSprId( uint animId, int sprIndex )
{
    if( ( GameMain.Anims->VAnims.size() <= animId ) || ( sprIndex != 0 ) )
        return 0;

    SAnim* anim = *( GameMain.Anims->VAnims[ animId ] );

    if( anim->SprId )
    {
        uint id = *( anim->SprId );

        return id;
    }

    return 0;
}

EXPORT bool Field_SetTile( bool isRoof, uint animId, uint8 layer, int16 offsX, int16 offsY, Field* field )
{
    // char buff[200];

    Field::TileVec* tiles;
    if( isRoof )
        tiles = &field->Roofs;
    else
        tiles = &field->Tiles;

    if( GameMain.Anims->VAnims.size() <= animId )
        return false;

    void* anim = (void*) ( *( GameMain.Anims->VAnims[ animId ] ) );

    if( anim == NULL )
        return false;

    Field::TileVec::iterator It = tiles->begin(), ItEnd = tiles->end();
    uint                     len = tiles->size();

    #ifdef DISABLE_NEW
    if( It == 0 )
        return false;
    #endif

    for( ; It < ItEnd; It++ )
    {
        if( It->Anim == anim )
            return false;
        if( It->Layer == layer )
        {
            // sprintf(buff, "offsX: %i, offsY: %i; ", It->OffsX, It->OffsY);
            // Log(buff);
            break;
        }
    }
    if( It == ItEnd )
    {
        /*if(It!=0) {*/
        tiles->resize( len + 1 );
        It = tiles->begin();
        It += len;                                               /*}
                                                                    else
                                                                    {
                                                                    Log("1");
                                                                    uint size = sizeof(Field::Tile);
                                                                    sprintf(buff, "size %u; ", size);
                                                                    Log(buff);
                                                                    char* p = new char[size];
                                                                    Log("3");
                                                                    //Field::Tile* tile = new Field::Tile;
                                                                    uint* tvec = (uint*)tiles;
                                                                    *tvec = (uint) p;
                                                                    *(tvec+1) = (*tvec) + size;
                                                                    *(tvec+2) = *(tvec+1);
                                                                    It=tiles->begin();
                                                                    Log("5");
                                                                    }*/
    }

    It->Anim = anim;
    It->OffsX = offsX;
    It->OffsY = offsY;
    It->Layer = layer;

    return true;
}

EXPORT bool SetTile( uint16 hexX, uint16 hexY, bool isRoof, uint animId, uint8 layer, int16 offsX, int16 offsY )
{
    Field* field = GetField( hexX, hexY );

    if( field == NULL )
        return false;

    if( !Field_SetTile( isRoof, animId, layer, offsX, offsY, field ) )
        return false;

    if( isRoof && field->RoofNum == 0 )
    {
        for( int i = 0; i < 4; i++ )
        {
            Field* field0 = GetField( hexX + 2 * ( -1 + abs( i - 1 ) ), hexY + 2 * ( -1 + abs( 2 - i ) ) );
            if( field0 == NULL )
                continue;
            if( field0->RoofNum != 0 )
            {
                field->RoofNum = field0->RoofNum;
                break;
            }
        }

        if( field->RoofNum == 0 )
            field->RoofNum = ( rand() % 10000 ) - 10001;

        int rn = field->RoofNum;

        for( uint i = 1; i < 4; i++ )
        {
            Field* field0 = GetField( hexX + ( i & 1 ), hexY + ( ( i >> 1 ) & 1 ) );
            if( field0 == NULL )
                continue;
            field0->RoofNum = rn;
        }

        /*GetField(hexX+1, hexY)->RoofNum = rn;
           GetField(hexX, hexY+1)->RoofNum = rn;
           GetField(hexX+1, hexY+1)->RoofNum = rn;*/

    }

    return true;
}

EXPORT Sprite* AddMapSprite( uint16 hexX, uint16 hexY, uint animId, int sprIndex, int offsX, int offsY, int drawOrderType, int drawOffsY )
{
    uint   sprId = *( ( *( GameMain.Anims->VAnims[ animId ] ) )->SprId );

    Field* field = *GameMain.Map + ( hexY * ( *GameMain.MapWidth ) + hexX );

    return GameMain.Sprites->InsertSprite( drawOrderType, hexX, hexY + drawOffsY, 0, field->ScrX + offsX, field->ScrY + offsY, sprId, 0, 0, 0, 0, 0 );
}

// SAnim* anim123 = NULL;

EXPORT uint TestAnim( uint hash, uint16 hexX, uint16 hexY, uint val )
{
    // if(anim123 != NULL)
    // {
//		return 3;
    // }

    uint animId = GameMain.Anims->AnimLoad( hash, 0, val );

    // anim123 = ResourceManager->GetAnim(hash, 0, val);
    // if(anim123 == NULL) return 1;
    if( animId == 0 )
        return 1;

    // uint sprId = *(anim123->SprId);
    uint    sprId = *( ( *( GameMain.Anims->VAnims[ animId ] ) )->SprId );

    Field*  field = GetField( hexX, hexY );
    Sprite* spr = GameMain.Sprites->InsertSprite( 24, hexX, hexY, 0, field->ScrX, field->ScrY, sprId, 0, 0, 0, 0, 0 );
    if( spr == NULL )
        return 2;
    return 0;
}


EXPORT Sprite* GetSprite( uint sprId, uint16 hexX, uint16 hexY )
{
    SprVecIt sprIt = GameMain.Sprites->SpritesVec.begin();
    uint     count = GameMain.Sprites->ValidSprites;

    if( count == 0 || sprId >= GameMain.Anims->VAnims.size() )
        return NULL;

    SAnim* anim = *( GameMain.Anims->VAnims[ sprId ] );

    if( anim->SprId )
    {
        uint id = *( anim->SprId );

        for( SprVecIt sprEnd = sprIt + count; sprIt < sprEnd; sprIt++ )
        {
            Sprite* spr = *sprIt;
            if( spr->GetSprId() == id && spr->HexX == hexX && spr->HexY == hexY )
                return spr;
        }
    }

    return NULL;
}

EXPORT Sprite* GetSpriteDOT( int dot, uint16 hexX, uint16 hexY )
{
    SprVecIt sprIt = GameMain.Sprites->SpritesVec.begin();
    uint     count = GameMain.Sprites->ValidSprites;

    if( count == 0 )
        return NULL;

    SprVecIt sprEnd = sprIt + count;

    for( ; sprIt < sprEnd; sprIt++ )
    {
        Sprite* spr = *sprIt;

        if( spr->Valid != false )
        {
            if( spr->DrawOrderType == dot && spr->HexX == hexX && spr->HexY == hexY )
            {
                return spr;
            }
        }
    }

    return NULL;
}

EXPORT void SortSprites()
{
    SprVecIt sprIt = GameMain.Sprites->SpritesVec.begin();
    uint     count = GameMain.Sprites->ValidSprites;
    // SprVecIt sprEnd = sprIt+count;

    for( uint i = 0; i < count; i++ )
    {
        for( uint j = i + 1; j < count; j++ )
        {
            if( sprIt[ i ]->DrawOrderPos < sprIt[ j ]->DrawOrderPos )
                continue;
            Sprite* spr = sprIt[ j ];
            sprIt[ j ] = sprIt[ i ];
            sprIt[ i ] = spr;
        }
    }
    for( uint i = 0; i < count; i++ )
    {
        sprIt[ i ]->TreeIndex = i;
    }
}

EXPORT void Sprite_RecalcOrder( int hexY, Sprite* sprite )
{
    if( sprite->DrawOrderType > 13 )
    {
        sprite->DrawOrderPos = ( 99999999 + ( sprite->HexY + hexY ) * 10000 + sprite->HexX ) * 5 * 4 + sprite->DrawOrderType;
    }
    else
    {
        sprite->DrawOrderPos = ( ( sprite->DrawOrderType * 10000 + sprite->HexY + hexY ) * 10000 ) + sprite->HexX;
    }
}

EXPORT Sprite* GetMonitorSprite( int x, int y )
{
    SprVecIt sprIt = GameMain.Sprites->SpritesVec.begin();
    uint     count = GameMain.Sprites->ValidSprites;

    if( count == 0 )
        return NULL;

    if( x == 0 && y == 0 )
    {
        x = int(FOnline->MouseX * FOnline->SpritesZoom);
        y = int(FOnline->MouseY * FOnline->SpritesZoom);
    }

    // SprVecIt sprEnd = sprIt+count;

    for( SprVecIt spr1 = sprIt + count - 1; spr1 >= sprIt; spr1-- )
    {
        Sprite* spr = *spr1;

        if( spr->Valid != false )
        {
            if( FOnline->IsSpriteHit( spr, x, y, true ) )
            {
                return spr;
            }
        }
    }

    return NULL;
}

void Blank_Ref( void* obj )
{}

uint Sprite_GetSprId_proxy( Sprite* sprite )
{
    return sprite->GetSprId();
}

void Sprite_GetPos_proxy( int& x, int& y, Sprite* sprite )
{
    return sprite->GetPos( x, y );
}

float Linear_Interpolate( float a, float b, float x )
{
    return a * ( 1.0f - x ) + b * x;
}

float Cosine_Interpolate( float a, float b, float x )
{
    float ft = x * 3.1415927,
          f = ( 1.0f - cos( ft ) ) * 0.5f;

    return a * ( 1.0f - f ) + b * f;
}

/*
   // JAVA REFERENCE IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.

   public final class ImprovedNoise {
   static public double noise(double x, double y, double z) {
      int X = (int)Math.floor(x) & 255,                  // FIND UNIT CUBE THAT
          Y = (int)Math.floor(y) & 255,                  // CONTAINS POINT.
          Z = (int)Math.floor(z) & 255;
      x -= Math.floor(x);                                // FIND RELATIVE X,Y,Z
      y -= Math.floor(y);                                // OF POINT IN CUBE.
      z -= Math.floor(z);
      double u = fade(x),                                // COMPUTE FADE CURVES
             v = fade(y),                                // FOR EACH OF X,Y,Z.
             w = fade(z);
      int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      // HASH COORDINATES OF
          B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

      return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
                                     grad(p[BA  ], x-1, y  , z   )), // BLENDED
                             lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
                                     grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
                     lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
                                     grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                     grad(p[BB+1], x-1, y-1, z-1 ))));
   }
   static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
   static double lerp(double t, double a, double b) { return a + t * (b - a); }
   static double grad(int hash, double x, double y, double z) {
      int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
      double u = h<8||h==12||h==13 ? x : y,   // INTO 12 GRADIENT DIRECTIONS.
             v = h<4||h==12||h==13 ? y : z;
      return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
   }
   static final int p[] = new int[512], permutation[] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   };
   static { for (int i=0; i < 256 ; i++) p[256+i] = p[i] = permutation[i]; }
 */

uint NoiseSeed = 789221;

float Noise2d( int x, int y )
{
    int64 n = x + y * 97171;
    n = ( n << 13 ) ^ n;
    return ( 1.0f - ( ( n * ( n * n * 15731 + NoiseSeed ) + 1376312589 ) & 0x7fffffff ) / 1073741824.0f );
} // 789221

float SmoothedNoise2d( int x, int y )
{
    float corners = ( Noise2d( x - 1, y - 1 ) + Noise2d( x + 1, y - 1 ) + Noise2d( x - 1, y + 1 ) + Noise2d( x + 1, y + 1 ) ) / 16,
          sides   = ( Noise2d( x - 1, y )  + Noise2d( x + 1, y )  + Noise2d( x, y - 1 )  + Noise2d( x, y + 1 ) ) /  8,
          center  =  Noise2d( x, y ) / 4;
    return corners + sides + center;
}

float InterpolatedNoise2d( float x, float y )
{
    int   integer_X    = (int) x;
    float fractional_X = x - integer_X;

    int   integer_Y    = (int) y;
    float fractional_Y = y - integer_Y;

    float v1 = SmoothedNoise2d( integer_X,     integer_Y ),
          v2 = SmoothedNoise2d( integer_X + 1, integer_Y ),
          v3 = SmoothedNoise2d( integer_X,     integer_Y + 1 ),
          v4 = SmoothedNoise2d( integer_X + 1, integer_Y + 1 );

    float i1 = Cosine_Interpolate( v1, v2, fractional_X ),
          i2 = Cosine_Interpolate( v3, v4, fractional_X );

    return Cosine_Interpolate( i1, i2, fractional_Y );
}

float persistence = 1 / 2;
uint  Number_Of_Octaves = 4;

float PerlinNoise2D( float x, float y )
{
    float total = 0,
          p = persistence;
    int   n = Number_Of_Octaves;

    // float frequency = 0.25f;

    for( int i = 0; i < n; i++ )
    {
        float frequency = pow( 2.0f, -i - 2 );
        float amplitude = pow( p, i );

        total += InterpolatedNoise2d( x * frequency, y * frequency ) * amplitude;
        // frequency*=2;
    }

    return total;
}

void fillNoiseBuffer( float* buffer, uint8 size )
{
    uint y0 = 0;
    for( int y = 0; y < size; y++ )
    {
        for( int x = 0; x < size; x++ )
        {
            buffer[ y0 + x ] = PerlinNoise2D( x, y );
        }
        y0 += size;
    }
}
/*
   float smooth(float* buffer, size, x, y)
   {


        float corners = ( Noise2d(x-1, y-1)+Noise2d(x+1, y-1)+Noise2d(x-1, y+1)+Noise2d(x+1, y+1) ) / 16,
    sides   = ( Noise2d(x-1, y)  +Noise2d(x+1, y)  +Noise2d(x, y-1)  +Noise2d(x, y+1) ) /  8,
    center  =  Noise2d(x, y) / 4;
   }
 */
void GenerateDesert( ScriptArray* array, uint size )
{
    if( array->GetElementSize() != sizeof( float ) )
    {
        Log( "GenerateDesert: Error! Wrong type of array." );
        return;
    }
    array->Resize( size * size );

    /*float buffer5[5*5];
       fillNoiseBuffer(buffer5, 5);

       float buffer15[15*15];
       fillNoiseBuffer(buffer15, 15);

       float buffer25[25*25];
       fillNoiseBuffer(buffer25, 25);

       float buffer75[75*75];
       fillNoiseBuffer(buffer75, 75);*/

    float* buffer = (float*) array->GetBuffer();
    fillNoiseBuffer( buffer, size );

    /*for(int y=0, i=0; y<75; y++)
       {
            for(int x=0; x<75; x++)
            {
                    buffer[i]=buffer5[(y/15)*5+x/15]/4 + buffer15[(y/5)*25+x/5]/4 + buffer25[(y/3)*25+x/3]/4 + buffer75[i]/4;
                    i++;
            }
       }*/



    // memcpy(array->buffer->data, buffer, size*size*sizeof(float));
}

void Field_ClearTiles( bool roofs, Field* field )
{
    if( roofs )
        field->Roofs.clear();
    else
        field->Tiles.clear();
}

void ClearAllTiles()
{
    Field* field = *GameMain.Map;
    for(uint i=0, len = (*GameMain.MapWidth)*(*GameMain.MapHeight); i<len; i++)
    {
        if(!field) continue;
        field->Tiles.clear();
        field->Roofs.clear();
        field++;
    }

}

void Field_ChangeTileLayer( bool isRoof, uint8 fromLayer, uint8 toLayer, Field* field )
{
    Field::TileVec* tiles;
    if( isRoof )
        tiles = &( field->Roofs );
    else
        tiles = &( field->Tiles );

    for( Field::TileVec::iterator It = tiles->begin(), ItEnd = tiles->end(); It < ItEnd; It++ )
    {
        if( It->Layer == fromLayer )
            It->Layer = toLayer;
    }
}

//004830E0  FOnlineD3D.?NetDisconnect@FOClient@@QAEXXZ(guessed void)
void NetDisconnect()
{
    void* p = GameMain.pFOnine;
    _asm {
        mov ecx, p
        mov eax, 0x004830E0
        call eax
    }
}


void NetLogin(uint mode, ScriptString* login, ScriptString* pass)
{
    NetDisconnect();

    switch(mode)
    {
        case 1: break; //коннект без лоина и пароля, для скачивания кеша
        case 2: //обычный логин
        {
            if(login==NULL || pass==NULL) return;
            uint passlen = pass->length(), loglen = login->length();
            if(passlen>14 || passlen<3 || loglen<FOnline->MinNameLength || loglen>FOnline->MaxNameLength) return;

            memcpy(GameMain.pass, pass->c_str(), passlen+1);

            const_cast<ScriptString&>(FOnline->Name) = *login;

            break;
        }
        case 3: return; //регистрация
                        //0049636E  |.  8B96 C83A0000 MOV EDX,DWORD PTR DS:[ESI+3AC8]
                        //00496374  |.  52            PUSH EDX                                                                                       ; /Arg1
                        //00496375  |.  E8 C605FDFF   CALL ?Net_SendCreatePlayer@FOClient@@QAEXPAVCritterCl@@@Z                                      ; \FOnlineD3D.?Net_SendCreatePlayer@FOClient@@QAEXPAVCritterCl@@@Z
        case 4: return; // сэйв/лоад сингла
        default: return;  // дисконнект
    }

    *GameMain.doConnect = mode;
}


// class QDrawSystem
// {} qDrawSystem;

struct CUSTOMVERTEX
{
    float x,y,z;
    D3DCOLOR diffuse;
};



struct FOEffect
{
    uint un1, un2;
    ID3DXEffect* dxeffect;
    uint EffectFlags,
    EffectParam,
    un3, un4;
    D3DXHANDLE TechniqueSimple;
    uint un5, un6, un7, un8, un9, un10, un11, un12, un13, un14, un15,
    EffectVariables;
};


#define M_PI       3.14159265358979323846

ID3DXEffect* g_pEffect = NULL;

float xqwe = 0,
xqwe_step = M_PI/128;

int8 efinit = 0;
bool InitTestEffect(IDirect3DDevice9* d3dDevice)
{
    DWORD dwShaderFlags = 0;

    dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

    // Read the D3DX effect file

    LPD3DXBUFFER err = NULL;

    HRESULT res = D3DXCreateEffectFromFile(
        d3dDevice,
        "H:\\projects\\fo\\fo_cl_rp\\data\\effects\\Sand.fx\0",
        NULL, // CONST D3DXMACRO* pDefines,
        NULL, // LPD3DXINCLUDE pInclude,
        dwShaderFlags,
        NULL, // LPD3DXEFFECTPOOL pPool,
        &g_pEffect,
        &err );

    if(g_pEffect==NULL) Log("FAAAAAAIL!!! %u \n", res);
    else Log("GOOOOD!!! %u \n", res);

    if(err!=NULL) Log("err: %s ", (char*)err->GetBufferPointer());

    return(FAILED(res));
}


int TestDirect3d(uint test_type)
{
    if((SpriteManager->direct3d==0) || (SpriteManager->direct3d_device==0)) return -10;
    IDirect3DDevice9* d3dDevice = SpriteManager->direct3d_device;


    if(efinit==0)
    {
        efinit = InitTestEffect(d3dDevice)?-1:1;
        if(efinit<0) Log("Load test effect failed.");
        else Log("Load test effect done.");
    }
    else if(efinit<0) return -1;

    //return 0;



    if(xqwe>=(M_PI*2)) xqwe=0;

    switch(test_type)
    {
        case 3:
        {


            UPrepPointVec pvec;
            pvec.resize(5);
            float cc = cos(xqwe), cs = sin(xqwe);
            int icc = (uint)((cc+1.0f)*0x7C), ics = (uint)((cs+1.0f)*0x7C);

            int cposx = 400+cc*100, cposy = 300-cs*100;

            cc*=100.0f; cs*=100.0f;

            pvec[0].Set(cposx+cc, cposy+cs, D3DCOLOR_ARGB(0xff, icc, 0xff-icc, 0));
            pvec[1].Set(cposx-cc, cposy-cs, D3DCOLOR_ARGB(0xff, icc, icc, 0xff-icc));
            pvec[2].Set(cposx+cs, cposy+cc, D3DCOLOR_ARGB(0xff, ics, ics, 0xff-ics));
            pvec[3].Set(cposx-cs, cposy-cc, D3DCOLOR_ARGB(0xff, ics, 0xff-ics, 0));
            pvec[4].Set(cposx+cc, cposy+cs, D3DCOLOR_ARGB(0xff, icc, 0xff-icc, 0));
            SpriteManager->DrawPoints(&pvec, D3DPT_LINESTRIP, 0, 0, 0);

            break;
        }
        case 2:
        {
            SpriteManager->Flush();

            FOEffect* effect = *((FOEffect**)(((char*)SpriteManager)+0x218));

            if(effect==NULL) break;

            ID3DXEffect* dxeffect = effect->dxeffect;

            if(dxeffect==NULL) break;

            CUSTOMVERTEX Vertices[] =
            {
                {0,    0,    1.0, 0xFFFF0000},
                {cos(xqwe),    sin(xqwe),  1.0, 0xFF0000FF}//,
                //{0.5,  0.5,  1.0, 0xFFFFFFFF},
                //{0.5,  0.9,    1.0, 0xFFFFFFFF}
            };

            d3dDevice->SetFVF( D3DFVF_XYZ| D3DFVF_DIFFUSE );

            dxeffect->SetTechnique(effect->TechniqueSimple);

            uint passes;
            dxeffect->Begin(&passes, effect->EffectFlags);
            for(uint i=0; i<passes; i++)
            {
                dxeffect->BeginPass(i);
                d3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, (void*)Vertices, sizeof(CUSTOMVERTEX));
                dxeffect->EndPass();
            }
            dxeffect->End();

            //d3dDevice->EndScene();

            //d3dDevice->Present( NULL, NULL, NULL, NULL );

            //d3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL );
            //d3dDevice->SetRenderState( D3DRS_STENCILREF, 0 );

            //d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
            //d3dDevice->SetRenderState( D3DRS_STENCILENABLE, FALSE );




            //d3dDevice->BeginScene();

            break;
        }
        case 0:
        {
            if(g_pEffect==NULL) {/*Log("g_pEffect==NULL");*/ break;}

            CUSTOMVERTEX Vertices[] =
            {
                {0,    0,    1.0, 0xFFFF0000},
                {cos(xqwe),    sin(xqwe),  1.0, 0xFF0000FF}//,
                //{0.5,  0.5,  1.0, 0xFFFFFFFF},
                //{0.5,  0.9,    1.0, 0xFFFFFFFF}
            };

            d3dDevice->SetFVF( D3DFVF_XYZ| D3DFVF_DIFFUSE );

            uint cPasses=0;
            // Apply the technique contained in the effect

            g_pEffect->SetTechnique("BasicGen");

            g_pEffect->Begin(&cPasses, 0);

            for (uint iPass = 0; iPass < cPasses; iPass++)
            {
                g_pEffect->BeginPass(iPass);

                // Only call CommitChanges if any state changes have happened
                // after BeginPass is called
                //g_pEffect->CommitChanges();

                d3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, (void*)Vertices, sizeof(CUSTOMVERTEX));

                // Render the mesh with the applied technique
                //g_pMesh->DrawSubset(0);

                g_pEffect->EndPass();
            }
            g_pEffect->End();


            break;
        }
    }

    xqwe+=xqwe_step;

    return 0;
}

void RegisterNativeSprites( asIScriptEngine* engine, bool compiler )
{
    STATIC_ASSERT( offsetof( CFOnlineOGL, MapWidth )                            == 0x20  );
    STATIC_ASSERT( offsetof( CFOnlineD3D, MapWidth )                                == 0x20  );
    STATIC_ASSERT( offsetof( CFOnlineD3D, Sprites )                              == 0x94  ); // d3d
    STATIC_ASSERT( offsetof( CFOnlineOGL, Sprites )                          == 0xB4  );     // ogl

    STATIC_ASSERT( offsetof( CFOnlineD3D, doConnect)                         == 0x78c  ); // d3d

    STATIC_ASSERT( offsetof( CFOnlineD3D, Anims )                           == 0x930  );     // d3d
    STATIC_ASSERT( offsetof( CFOnlineOGL, Anims )                           == 0x97C  );     // ogl

    int r;

    // Register the type
    r = engine->RegisterObjectType( "NativeSprite", 0 /*sizeof(Sprite)*/, asOBJ_REF );

    // Registering the addref/release behaviours
    r = engine->RegisterObjectBehaviour( "NativeSprite", asBEHAVE_ADDREF, "void f()", asFUNCTION( Blank_Ref ), asCALL_CDECL_OBJLAST );
    r = engine->RegisterObjectBehaviour( "NativeSprite", asBEHAVE_RELEASE, "void f()", asFUNCTION( Blank_Ref ), asCALL_CDECL_OBJLAST );

    r = engine->RegisterObjectProperty( "NativeSprite", "int DrawOrderType", offsetof( Sprite, DrawOrderType ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "uint DrawOrderPos", offsetof( Sprite, DrawOrderPos ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "uint TreeIndex", offsetof( Sprite, TreeIndex ) );

    r = engine->RegisterObjectProperty( "NativeSprite", "uint SprId", offsetof( Sprite, SprId ) );

    r = engine->RegisterObjectProperty( "NativeSprite", "int HexX", offsetof( Sprite, HexX ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "int HexY", offsetof( Sprite, HexY ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "int ScrX", offsetof( Sprite, ScrX ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "int ScrY", offsetof( Sprite, ScrY ) );

    r = engine->RegisterObjectProperty( "NativeSprite", "int EggType", offsetof( Sprite, EggType ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "int ContourType", offsetof( Sprite, ContourType ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "uint ContourColor", offsetof( Sprite, ContourColor ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "uint Color", offsetof( Sprite, Color ) );
    r = engine->RegisterObjectProperty( "NativeSprite", "uint FlashMask", offsetof( Sprite, FlashMask ) );

    r = engine->RegisterObjectMethod( "NativeSprite", "uint GetSprId()", asFUNCTION( Sprite_GetSprId_proxy ), asCALL_CDECL_OBJLAST );
    r = engine->RegisterObjectMethod( "NativeSprite", "void GetPos(int&, int&)", asFUNCTION( Sprite_GetPos_proxy ), asCALL_CDECL_OBJLAST );
    r = engine->RegisterObjectMethod( "NativeSprite", "void RecalcOrder(int)", asFUNCTION( Sprite_RecalcOrder ), asCALL_CDECL_OBJLAST );

    r = engine->RegisterGlobalFunction( "NativeSprite@ GetSprite(uint, uint16, uint16)", asFUNCTION( GetSprite ), asCALL_CDECL );
    r = engine->RegisterGlobalFunction( "NativeSprite@ GetSpriteDOT(int, uint, uint)", asFUNCTION( GetSpriteDOT ), asCALL_CDECL );
    r = engine->RegisterGlobalFunction( "NativeSprite@ GetMonitorSprite(int, int)", asFUNCTION( GetMonitorSprite ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "void SortSprites()", asFUNCTION( SortSprites ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "NativeSprite@ AddMapSprite(uint16, uint16, uint, int, int, int, int, int)", asFUNCTION( AddMapSprite ), asCALL_CDECL );
    r = engine->RegisterGlobalFunction( "uint TestAnim(uint, uint16, uint16, uint)", asFUNCTION( TestAnim ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "bool SetTile(uint16, uint16, bool, uint, uint8, int16, int16)", asFUNCTION( SetTile ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "uint GetSprId(uint, int)", asFUNCTION( GetSprId ), asCALL_CDECL );



    r = engine->RegisterObjectType( "NativeField", 0 /*sizeof(Sprite)*/, asOBJ_REF );

    r = engine->RegisterObjectBehaviour( "NativeField", asBEHAVE_ADDREF, "void f()", asFUNCTION( Blank_Ref ), asCALL_CDECL_OBJLAST );
    r = engine->RegisterObjectBehaviour( "NativeField", asBEHAVE_RELEASE, "void f()", asFUNCTION( Blank_Ref ), asCALL_CDECL_OBJLAST );

    r = engine->RegisterObjectProperty( "NativeField", "bool ScrollBlock", offsetof( Field, ScrollBlock ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsWall", offsetof( Field, IsWall ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsWallSAI", offsetof( Field, IsWallSAI ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsWallTransp", offsetof( Field, IsWallTransp ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsScen", offsetof( Field, IsScen ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsExitGrid", offsetof( Field, IsExitGrid ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsNotPassed", offsetof( Field, IsNotPassed ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsNotRaked", offsetof( Field, IsNotRaked ) );
    r = engine->RegisterObjectProperty( "NativeField", "bool IsNoLight", offsetof( Field, IsNoLight ) );

    r = engine->RegisterObjectMethod( "NativeField", "bool SetTile(bool, uint, uint8, int16, int16)", asFUNCTION( Field_SetTile ), asCALL_CDECL_OBJLAST );
    r = engine->RegisterObjectMethod( "NativeField", "void ClearTiles(bool)", asFUNCTION( Field_ClearTiles ), asCALL_CDECL_OBJLAST );
    r = engine->RegisterObjectMethod( "NativeField", "void ChangeTileLayer(bool, uint8, uint8)", asFUNCTION( Field_ChangeTileLayer ), asCALL_CDECL_OBJLAST );

    r = engine->RegisterGlobalFunction( "NativeField@ GetField(uint16, uint16)", asFUNCTION( GetField ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "void GenerateDesert(float[]&, uint)", asFUNCTION( GenerateDesert ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "void NetLogin(uint, string@, string@)", asFUNCTION( NetLogin ), asCALL_CDECL );
    r = engine->RegisterGlobalFunction( "void NetDisconnect()", asFUNCTION( NetDisconnect ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "int TestDirect3d(uint)", asFUNCTION( TestDirect3d ), asCALL_CDECL );

    if( compiler )
        return;

    if( *( (uint8*) 0x00401042 ) == 0xC3 ) // D3D
    {
        FOnlineD3D = *( (CFOnlineD3D**) ENGINE_PTR_STRUCT_FOClient_D3D );

        GameMain.pFOnine = (void*)FOnlineD3D;

        GameMain.pass = FOnlineD3D->pass;

        GameMain.MapWidth = &FOnlineD3D->MapWidth;
        GameMain.MapHeight = &FOnlineD3D->MapHeight;
        GameMain.Map = &FOnlineD3D->Map;
        GameMain.hexValidBeginIt = &FOnlineD3D->hexValidBeginIt;
        GameMain.hexValidEndIt = &FOnlineD3D->hexValidEndIt;
        GameMain.Sprites = &FOnlineD3D->Sprites;

        GameMain.doConnect = &FOnlineD3D->doConnect;

        GameMain.Anims = &FOnlineD3D->Anims;

        ResourceManager = *( (CResourceManager**) ENGINE_PTR_STRUCT_ResourceManager_D3D );

        ENGINE_PTR_METHOD_InsertSprite          = (void*) ENGINE_PTR_METHOD_InsertSprite_D3D;
        ENGINE_PTR_METHOD_AnimLoad                      = (void*) ENGINE_PTR_METHOD_AnimLoad_D3D;
    }
    else      // OGL
    {
        FOnlineOGL = *( (CFOnlineOGL**) ENGINE_PTR_STRUCT_FOClient_OGL );

        GameMain.pFOnine = (void*)FOnlineOGL;

        GameMain.MapWidth = &FOnlineOGL->MapWidth;
        GameMain.MapHeight = &FOnlineOGL->MapHeight;
        GameMain.Map = &FOnlineOGL->Map;
        GameMain.hexValidBeginIt = &FOnlineOGL->hexValidBeginIt;
        GameMain.hexValidEndIt = &FOnlineOGL->hexValidEndIt;
        GameMain.Sprites = &FOnlineOGL->Sprites;
        GameMain.Anims = &FOnlineOGL->Anims;

        ResourceManager = *( (CResourceManager**) ENGINE_PTR_STRUCT_ResourceManager_OGL );

        ENGINE_PTR_METHOD_InsertSprite          = (void*) ENGINE_PTR_METHOD_InsertSprite_OGL;
        ENGINE_PTR_METHOD_AnimLoad                      = (void*) ENGINE_PTR_METHOD_AnimLoad_OGL;

    }

}
