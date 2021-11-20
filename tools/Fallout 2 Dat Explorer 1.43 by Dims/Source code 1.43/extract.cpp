//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "extract.h"
#include "mainform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TExtractForm *ExtractForm;
//---------------------------------------------------------------------------
__fastcall TExtractForm::TExtractForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TExtractForm::Button1Click(TObject *Sender)
{
      if(MessageBox(NULL, "Are You sure ?", "Warning", MB_TASKMODAL|MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2) == ID_YES)
         Form1->pDAT->CancelOp = TRUE;
}
//---------------------------------------------------------------------------
