//---------------------------------------------------------------------------

#ifndef optionsH
#define optionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TfrmOptions : public TForm
{
__published:	// IDE-managed Components
        TListView *ListView1;
        TImageList *il1;
        TEdit *edFallDir;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
private:	// User declarations
public:		// User declarations
        __fastcall TfrmOptions(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmOptions *frmOptions;
//---------------------------------------------------------------------------
#endif
