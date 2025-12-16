#ifndef Q_SPRITES
#define Q_SPRITES

#include "q_sprites.h"

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
    float ft = x * 3.1415927f,
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
    int y0 = 0;
    for( int y = 0; y < size; y++ )
    {
        for( int x = 0; x < size; x++ )
        {
            buffer[ y0 + x ] = PerlinNoise2D( (float)x, (float)y );
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
void GenerateDesert( CScriptArray* array, uint size )
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

bool Field_HasRoof( Field* field )
{
    return !field->Roofs.empty();
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

void RegisterNativeSprites( asIScriptEngine* engine, bool compiler )
{
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

    r = engine->RegisterObjectMethod( "NativeField", "void ClearTiles(bool)", asFUNCTION( Field_ClearTiles ), asCALL_CDECL_OBJLAST );
    r = engine->RegisterObjectMethod( "NativeField", "void ChangeTileLayer(bool, uint8, uint8)", asFUNCTION( Field_ChangeTileLayer ), asCALL_CDECL_OBJLAST );

    r = engine->RegisterObjectMethod( "NativeField", "bool HasRoof()", asFUNCTION( Field_HasRoof ), asCALL_CDECL_OBJLAST );

    r = engine->RegisterGlobalFunction( "NativeField@ GetField(uint16, uint16)", asFUNCTION( GetField ), asCALL_CDECL );

    r = engine->RegisterGlobalFunction( "void GenerateDesert(float[]&, uint)", asFUNCTION( GenerateDesert ), asCALL_CDECL );

    if( compiler )
        return;
}
#endif
