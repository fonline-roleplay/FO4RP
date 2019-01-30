#ifndef CELLS_H
#define CELLS_H

#include "_cell.fos"

class Cell;
class CellContent;
class CellWorld;

class Cell
{
    // uint16 X;
    // uint16 Y;
    uint8        Wildness;
    CellContent* Content;

    Cell();
};

class CellContent
{
    uint   MapId;
    // uint Index;
    uint8  Type;
    int8   Visibility;
    uint16 Durability;

    uint   Critters[ CELL_CRITTERS_MAX ];

    CellContent();
    CellContent( uint mapId, uint8 type, int8 visibility );

    uint GetCrittersCount( int type );
    uint GetCritters( CScriptsArray& crs, int type );
    bool ShiftCritter( Critter& cr, uint type, bool del )
    Map * GetMap();
};

#endif
