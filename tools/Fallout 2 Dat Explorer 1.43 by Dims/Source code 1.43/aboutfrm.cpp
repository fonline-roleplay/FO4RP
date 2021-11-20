//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "aboutfrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAbout *About;
//---------------------------------------------------------------------------
__fastcall TAbout::TAbout(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAbout::FormCreate(TObject *Sender)
{
   ListBox1->Items->Add("Fallout 2 DAT explorer   ver. 1.43");
   ListBox1->Items->Add("");
   ListBox1->Items->Add("  C++ code by Dims");
   ListBox1->Items->Add("    TeamX group member");
   ListBox1->Items->Add("      Tallinn , 2000   Build <26.09.00>");
   ListBox1->Items->Add("");
   ListBox1->Items->Add("           E-mail:  dims@hot.ee");
}
//---------------------------------------------------------------------------
void __fastcall TAbout::Label1Click(TObject *Sender)
{
   String St = "http://" + Label1->Caption;
   ShellExecute(Handle, "open", St.c_str() ,0, 0,SW_SHOW);
}
//---------------------------------------------------------------------------

