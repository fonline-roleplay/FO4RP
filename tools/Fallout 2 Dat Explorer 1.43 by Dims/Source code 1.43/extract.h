//---------------------------------------------------------------------------
#ifndef extractH
#define extractH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TExtractForm : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TAnimate *Animate1;
        TLabel *Label1;
        TProgressBar *ProgressBar1;
        TLabel *Label2;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TExtractForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TExtractForm *ExtractForm;
//---------------------------------------------------------------------------
#endif
