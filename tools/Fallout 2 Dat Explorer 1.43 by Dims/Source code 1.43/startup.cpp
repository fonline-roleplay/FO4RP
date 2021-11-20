//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "startup.h"
#include "mainform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TStartupForm *StartupForm;
//---------------------------------------------------------------------------
__fastcall TStartupForm::TStartupForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TStartupForm::ListView1Change(TObject *Sender,
      TListItem *Item, TItemChange Change)
{
   if(ListView1->Selected == NULL)
      Button1->Enabled = FALSE;
   else
      Button1->Enabled = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TStartupForm::Button1Click(TObject *Sender)
{
   this->Hide();
   if (TabControl1->TabIndex == 0) // Open
      Form1->NewDATclass(ID_OPENDAT);
   else                            // maybe New
      Form1->NewDATclass(ID_EMPTYDAT);
}
//---------------------------------------------------------------------------
void __fastcall TStartupForm::ListView1DblClick(TObject *Sender)
{
   if(!ListView1->SelCount)
      return;
   Button1->Click();
}
//---------------------------------------------------------------------------

