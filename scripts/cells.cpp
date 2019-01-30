#include "cells.h"

Cell::Cell()
{
    Wildness = 50;
    Content = NULL;
}

CellContent::CellContent()
{
    MapId = 0;
    // Index=uint(-1);
    Type = 0;
    Visibility = 0;
    Durability = 0;

    // Critters.resize(CELL_CRITTERS_MAX);
}

CellContent::CellContent( uint mapId, uint8 type, int8 visibility )
{
    MapId = mapId;
    // Index=uint(-1);
    Type = type;
    Visibility = visibility;
    Durability = 255;

    // Critters.resize(CELL_CRITTERS_MAX);
}

uint CellContent::GetCrittersCount( int type )
{
    uint count = 0;
    if( type >= 0 )
    {
        if( type >= CELL_CRITTERS_MAX )
            return 0;

        count = Critters[ type ];
    }
    else
        ( type == -1 )
        {
            for( uint i = 0; i < CELL_CRITTERS_MOB; i++ )
            {
                count += Critters[ i ];
            }
        }
        else
            ( )
            {
                for( uint i = 0; i < CELL_CRITTERS_MAX; i++ )
                {
                    count += Critters[ i ];
                }
            }
            return count;
}
