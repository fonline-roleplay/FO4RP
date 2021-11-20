//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "msgfrm.h"
#include "mainform.h"
#include "ascii.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMsg *frmMsg;
char *buf, *ptr;
//---------------------------------------------------------------------------
__fastcall TfrmMsg::TfrmMsg(TComponent* Owner, String FileName)
        : TForm(Owner)
{
   StatusBar1->Panels->Items[0]->Text = "File: " + FileName;
   this->Caption = FileName;
   LoadMSGFile(FileName);
   ComboBox1->ItemIndex = Form1->version;
   RefreshList();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMsg::LoadMSGFile(String FileName)
{
   unsigned long i;
   HANDLE h_in = CreateFile(FileName.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
   if (h_in == INVALID_HANDLE_VALUE)
       return;
   FileSize = GetFileSize(h_in, NULL);
   buf = new char[FileSize];
   ReadFile(h_in, buf, FileSize, &i, NULL);
   CloseHandle(h_in);

}
//---------------------------------------------------------------------------
void __fastcall TfrmMsg::RefreshList(void)
{
   char *ptr2 = NULL;
   char *tmp_buf, *tmp_buf2, *ascii;
   int Position, Length;
   ptr = buf;

   ListView1->Items->Clear();

   if (Form1->version == 0)
      ascii = ascii_levaja;
   else
      ascii = ascii_fargus;

   String Data, Value1 = NULL, Value2 = NULL;
   TStringList *strings = new TStringList;

   while (ptr < buf + FileSize)
   {
      ptr2 = strchr((char*)ptr, 0x0A);
      if (ptr2 == NULL) ptr2 = buf + FileSize;

      tmp_buf = new char[ptr2 - ptr];
      memset(tmp_buf, 0x00, sizeof(tmp_buf));
      memcpy(tmp_buf, ptr, ptr2 - ptr - 1);
      tmp_buf2 = tmp_buf;
      while (ptr++ < ptr2)
      {
         *tmp_buf2 = ascii[(BYTE)*tmp_buf2];
         tmp_buf2++;
      }
      ptr = ++ptr2;
      Data = (String)tmp_buf;
      delete tmp_buf;
      if (Data.IsEmpty())
         continue;

      switch (Data[1])
      {
         case '#' :
            AddToList("-----", "", Data);
            break;
         case '{' :
            while (!Data.IsEmpty())
            {
               Position = Data.Pos("}");
               Length = Data.Length();
               if (Position == 0) Position = Length;
               strings->Add(Data.SubString(2, Position - 2));
               Data = Data.SubString(++Position, Length - Position);
            }
            if (strings->Count < 3) break;
            Value1 = strings->Strings[0];
            Value2 = strings->Strings[1];
            AddToList(Value1, Value2, strings->Strings[2]);
            strings->Clear();
            break;
         default :
            AddToList(Value1, Value2, Data);
      }
   }
   delete strings;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMsg::AddToList(String Value1, String Value2, String Data)
{
   TListItems *pItems = ListView1->Items;
   TListItem *ListItem;
   ListItem = pItems->Add();
   ListItem->Caption = Value1;
   ListItem->SubItems->Add(Value2);
   ListItem->SubItems->Add(Data);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMsg::FormClose(TObject *Sender, TCloseAction &Action)
{
   if (buf)
      delete buf;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMsg::ComboBox1Change(TObject *Sender)
{
   Form1->version = ComboBox1->ItemIndex;
   RefreshList();
}
//---------------------------------------------------------------------------
