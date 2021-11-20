//---------------------------------------------------------------------------
#ifndef dirformH
#define dirformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#define STR_CREATE_FOLDER "Input new folder name:"
//---------------------------------------------------------------------------
class TInputDir : public TForm
{
__published:	// IDE-managed Components
        TEdit *Edit1;
        TButton *Button1;
        TButton *Button2;
        TLabel *Label1;
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TInputDir(TComponent* Owner);
        String FolderName;
};
//---------------------------------------------------------------------------
extern PACKAGE TInputDir *InputDir;
//---------------------------------------------------------------------------
#endif
