//---------------------------------------------------------------------------
#ifndef msgfrmH
#define msgfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TfrmMsg : public TForm
{
__published:	// IDE-managed Components
        TListView *ListView1;
        TPanel *Panel1;
        TComboBox *ComboBox1;
        TStatusBar *StatusBar1;
        TImageList *ImageList1;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ComboBox1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmMsg(TComponent* Owner, String FileName);
        void __fastcall LoadMSGFile(String FileName);
        void __fastcall RefreshList(void);
        void __fastcall AddToList(String Value1, String Value2, String Data);
        DWORD FileSize;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMsg *frmMsg;
//---------------------------------------------------------------------------
#endif
