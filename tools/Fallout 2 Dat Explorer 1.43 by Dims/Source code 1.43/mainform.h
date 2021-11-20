//---------------------------------------------------------------------------
#ifndef mainformH
#define mainformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Dialogs.hpp>
#include "datfile.h"
#include "frmfile.h"
//---------------------------------------------------------------------------
#define ID_EMPTYDAT 0
#define ID_OPENDAT 1

#define ID_TEMP 0
#define ID_CHOOSEDIR 1

#define ID_ALL_FILES 0
#define ID_SELECTED_FILES 1
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TTreeView *TreeView1;
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TToolButton *ToolButton12;
        TToolButton *ToolButton13;
        TToolButton *ToolButton14;
        TToolButton *ToolButton15;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Exit1;
        TMenuItem *N2;
        TMenuItem *Open1;
        TMenuItem *New1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TMenuItem *HowtoUseHelp1;
        TMenuItem *SearchforHelpOn1;
        TMenuItem *Contents1;
        TListView *ListView1;
        TStatusBar *StatusBar1;
        TSplitter *Splitter1;
        TImageList *ImageList1;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TMenuItem *View1;
        TMenuItem *Toolbar2;
        TMenuItem *Smallicons1;
        TMenuItem *Listview2;
        TMenuItem *Detailview1;
        TImageList *ImageList2;
        TImageList *ImageList3;
        TPopupMenu *PopupMenu1;
        TMenuItem *View2;
        TMenuItem *N3;
        TMenuItem *ArrangeIcons1;
        TMenuItem *N4;
        TMenuItem *Selectall1;
        TMenuItem *N5;
        TMenuItem *Toolbar3;
        TMenuItem *Smallicons2;
        TMenuItem *Listview3;
        TMenuItem *Detailview2;
        TMenuItem *byName1;
        TMenuItem *byType1;
        TMenuItem *bySize1;
        TMenuItem *Extractto1;
        TMenuItem *N1;
        TMenuItem *Open2;
        TMenuItem *Importfiles1;
        TMenuItem *Fromfolder1;
        TMenuItem *Selectedfiles1;
        TMenuItem *FromotherDATfile1;
        TMenuItem *N6;
        TMenuItem *New2;
        TMenuItem *Folder1;
        TPopupMenu *PopupMenu2;
        TMenuItem *NewFolder1;
        TMenuItem *Extract1;
        TMenuItem *Import1;
        TMenuItem *ExtrAll1;
        TMenuItem *Extra1;
        TMenuItem *N7;
        TMenuItem *Ext_to1;
        TMenuItem *Selectedfiles2;
        TMenuItem *Fromfolder2;
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall ToolButton10Click(TObject *Sender);
        void __fastcall ToolButton11Click(TObject *Sender);
        void __fastcall ToolButton12Click(TObject *Sender);
        void __fastcall ToolButton13Click(TObject *Sender);
        void __fastcall ToolButton1Click(TObject *Sender);
        void __fastcall ToolButton3Click(TObject *Sender);
        void __fastcall ToolButton15Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall ListView1SelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
        void __fastcall PopupMenu1Popup(TObject *Sender);
        void __fastcall ListView1DblClick(TObject *Sender);
        void __fastcall Selectall1Click(TObject *Sender);
        void __fastcall Extractto1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall Fromfolder1Click(TObject *Sender);
        void __fastcall Selectedfiles1Click(TObject *Sender);
        void __fastcall Createnewfolder1Click(TObject *Sender);
        void __fastcall TreeView1Change(TObject *Sender, TTreeNode *Node);
        void __fastcall PopupMenu2Popup(TObject *Sender);
        void __fastcall Extra1Click(TObject *Sender);
private:	// User declarations
        TProgressBar *FpProgress;
public:		// User declarations
        TDatFile *pDAT;
        TFrmFile *pFRM;
        __fastcall TForm1(TComponent* Owner);
        void __fastcall NewDATclass(UINT idType);
        void __fastcall FillTree(void);
        void __fastcall RefreshList(void);
        void __fastcall OpenFile(String filename);
        String __fastcall ExtractFiles(bool idWhere, bool idMask);
        void __fastcall ImportFromFolder(String Folder, TStrings *Files);
        String __fastcall SelectFolder(char *msg);
        void __fastcall NewProgressBar(UINT MaxValue);
        void __fastcall DelProgressBar(void);
        String __fastcall GetCurrentDatDir(void);
        void __fastcall CreateFolder(int AbsIndex, String NewFolder);
        void __fastcall FillDefaultDirs(void);
        String __fastcall GetFolderNameForm(String Caption);
        String __fastcall GetTemporaryFolder(void);
        unsigned long lngFileTotal;
        String WinTempDir;
        bool lPainted;
        int version;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
