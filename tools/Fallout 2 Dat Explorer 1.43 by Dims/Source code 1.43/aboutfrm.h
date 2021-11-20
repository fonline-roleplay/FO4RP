//---------------------------------------------------------------------------
#ifndef aboutfrmH
#define aboutfrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAbout : public TForm
{
__published:	// IDE-managed Components
        TButton *Button1;
        TPanel *Panel1;
        TListBox *ListBox1;
        TLabel *Label1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Label1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAbout *About;
//---------------------------------------------------------------------------
#endif
