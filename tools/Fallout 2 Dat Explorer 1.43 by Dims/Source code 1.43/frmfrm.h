//---------------------------------------------------------------------------
#ifndef frmfrmH
#define frmfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFrmForm : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
        __fastcall TFrmForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmForm *FrmForm;
//---------------------------------------------------------------------------
#endif
