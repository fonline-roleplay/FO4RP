//---------------------------------------------------------------------------
#ifndef startupH
#define startupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TStartupForm : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TButton *Button2;
        TTabControl *TabControl1;
        TListView *ListView1;
        TImageList *ImageList1;
        void __fastcall ListView1Change(TObject *Sender, TListItem *Item,
          TItemChange Change);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ListView1DblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TStartupForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TStartupForm *StartupForm;
//---------------------------------------------------------------------------
#endif
 