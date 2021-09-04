//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "dirform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInputDir *InputDir;
//---------------------------------------------------------------------------
__fastcall TInputDir::TInputDir(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInputDir::Button2Click(TObject *Sender)
{
   Edit1->Text = "?";
}
//---------------------------------------------------------------------------
void __fastcall TInputDir::Button1Click(TObject *Sender)
{
   FolderName = Edit1->Text;
}
//---------------------------------------------------------------------------
