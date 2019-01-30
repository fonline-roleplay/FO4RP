#ifndef REVENGE_H
#define REVENGE_H

// REVerse ENGinEering

#ifdef __SERVER

// 004E9D90  .text     Debug data  ?Cl_RunClientScript@SScriptFunc@FOServer>
# define     ENGINE_PTR_FUNC_Cl_RunClientScript          ( 0x004E9D90 )


#endif

#ifdef __CLIENT


// d3d
// 2F85510 - sprite manager
// 85DD30 - пароль

// 00856D28  .data     Debug data  ?Self@FOClient@@2PAV1@A // d3d
# define        ENGINE_PTR_STRUCT_FOClient_D3D           ( 0x00856D28 )
// 00834BA0  .data     Debug data  ?Self@FOClient@@2PAV1@A // ogl
# define        ENGINE_PTR_STRUCT_FOClient_OGL           ( 0x00834BA0 )

// 02F85408  .data     Debug data  ?ResMngr@@3VResourceManager@@A //d3d
# define        ENGINE_PTR_STRUCT_ResourceManager_D3D    ( 0x02DEDED0 )
// 02F63298  .data     Debug data  ?ResMngr@@3VResourceManager@@A //ogl
# define        ENGINE_PTR_STRUCT_ResourceManager_OGL    ( 0x02DEDED0 )

// 00444C10  .text     Debug data  ?InsertSprite@Sprites@@QAEAAVSprite@@HHHHHHIPAIPAF1PAEPA_N@Z // d3d
# define        ENGINE_PTR_METHOD_InsertSprite_D3D       ( 0x00444C10 )
// 00444C10  .text     Debug data  ?InsertSprite@Sprites@@QAEAAVSprite@@HHHHHHIPAIPAF1PAEPA_N@Z // ogl
# define        ENGINE_PTR_METHOD_InsertSprite_OGL       ( 0x00445EE0 )

// 0046CF60  .text     Debug data  ?AnimLoad@FOClient@@QAEIIEH@Z //d3d
# define        ENGINE_PTR_METHOD_AnimLoad_D3D           ( 0x0046CF60 )
// 0046DEE0  .text     Debug data  ?AnimLoad@FOClient@@QAEIIEH@Z //ogl
# define        ENGINE_PTR_METHOD_AnimLoad_OGL           ( 0x0046DEE0 )

#endif

#endif
