//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop


#include "mainform.h"
#include "aboutfrm.h"
#include "datfile.h"
#include "frmfile.h"
#include "msgfrm.h"
#include "startup.h"
#include "dirform.h"

#include <WindowsX.h>
//#include <shlobj.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TStringList *PathList;
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
   FpProgress = NULL;
   lPainted = FALSE;
   version = 0;
   WinTempDir = GetTemporaryFolder();
}
//---------------------------------------------------------------------------
String __fastcall TForm1::GetTemporaryFolder(void)
{
   char pBuffPath [255];
   DWORD n = GetTempPath(255, pBuffPath);
   pBuffPath[n - 1] = 0;
   return String(pBuffPath);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Exit1Click(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToolButton10Click(TObject *Sender)
{
   ToolButton10->Down = TRUE;
   Toolbar2->Checked = TRUE;
   Toolbar3->Checked = TRUE;
   ListView1->ViewStyle = vsIcon;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton11Click(TObject *Sender)
{
   ToolButton11->Down = TRUE;
   Smallicons1->Checked = TRUE;
   Smallicons2->Checked = TRUE;
   ListView1->ViewStyle = vsSmallIcon;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton12Click(TObject *Sender)
{
   ToolButton12->Down = TRUE;
   Listview2->Checked = TRUE;
   Listview3->Checked = TRUE;
   ListView1->ViewStyle = vsList;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton13Click(TObject *Sender)
{
   ToolButton13->Down = TRUE;
   Detailview1->Checked = TRUE;
   Detailview2->Checked = TRUE;
   ListView1->ViewStyle = vsReport;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton1Click(TObject *Sender)
   //Button "NEW"
{
   NewDATclass(ID_EMPTYDAT);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToolButton3Click(TObject *Sender)
   //Button "OPEN"
{
   NewDATclass(ID_OPENDAT);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewDATclass(UINT Type)
{
   OpenDialog1->FileName = "";
   OpenDialog1->Options.Clear();
   OpenDialog1->Options <<ofEnableSizing;
   OpenDialog1->Filter = "Fallout2 Data File (*.dat)|*.dat";

   SaveDialog1->Filter = "Fallout2 Data File (*.dat)|*.dat";
   SaveDialog1->FileName = "";

   if (Type ==  ID_EMPTYDAT)
      if (!SaveDialog1->Execute())
         return;
      else
      {
         if (pDAT)
         {
            delete pDAT;
            pDAT = NULL;
         }
         if (SaveDialog1->FileName.Pos(".dat"))
            pDAT = new TDatFile(SaveDialog1->FileName, ID_EMPTYDAT);
         else
            pDAT = new TDatFile(SaveDialog1->FileName + ".dat", ID_EMPTYDAT);
      }
   else
      if (!OpenDialog1->Execute())
         return;
      else
      {
         if (pDAT)
         {
            delete pDAT;
            pDAT = NULL;
         }
         pDAT = new TDatFile(OpenDialog1->FileName, ID_OPENDAT);
      }

   if (pDAT->lError)
   {
      pDAT->ShowError();
      delete pDAT;
      pDAT = NULL;
      return;
   }
   TreeView1->Color = clWindow;
   ListView1->Color = clWindow;
   FillTree();
   if (Type == ID_EMPTYDAT)
      FillDefaultDirs();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ToolButton15Click(TObject *Sender)
{
   About->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FillTree(void)
{
   UINT n, i, nIndex;
   bool lFound;
   String FullPath, TempPath, CurrDir, FullCurrDir;

   TTreeNodes* pItems = TreeView1->Items;
   TTreeNode *pNode, *pFirstNode;
   if (PathList)
      delete PathList;
   PathList = new TStringList;

   StatusBar1->Panels->Items[0]->Text = "Open ...";
   StatusBar1->Update();

   pItems->Clear();     //Bug was here
   pFirstNode = pItems->Add(NULL, pDAT->DatFileName); //OpenDialog1->FileName
   pFirstNode->ImageIndex = 2;
   pFirstNode->SelectedIndex = 2;
   pItems->BeginUpdate();

   pDAT->ResetPtr();
   while (pDAT->FindFile("", FALSE))
   {
      nIndex = 0;
      FullPath = pDAT->FullName;
      TempPath = FullPath;
      while (n = TempPath.Pos("\\"))
      {
         CurrDir = TempPath.SubString(0, n - 1); //First dir in FullPath
         TempPath = TempPath.SubString(n + 1, FullPath.Length());
         FullCurrDir = FullPath.SubString(0, FullPath.Pos(TempPath) - 1);
         lFound = FALSE;
         for (i = PathList->Count; i > 0; i--)
         {
            if (PathList->Strings[i - 1] == FullCurrDir)
            {
               lFound = TRUE;
               nIndex = i;
               break;
            }
         }
         if (!lFound)
         {
            pNode = pItems->AddChild(pItems->Item[nIndex], CurrDir);
            nIndex = PathList->Count + 1;
            pNode->ImageIndex = 0;
            pNode->SelectedIndex = 1;
            PathList->Add(FullCurrDir);
         }
      }
   }
//   PathList->Sort();
   pItems->EndUpdate();
   pFirstNode->Expand(TRUE);
   TreeView1->TopItem = pFirstNode;
   pFirstNode->Selected = TRUE;
   StatusBar1->Panels->Items[0]->Text = "Ready";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
//   ListView1->Arrange(arDefault);
   ListView1->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RefreshList()
{
   TListItems *pItems = ListView1->Items;
   bool lFound;
   TListItem *ListItem;
   UINT i, ImageIndex, DirCount = 0;

   if (!pDAT)
      return;  //Empty project

   pItems->BeginUpdate();
   pItems->Clear();

   String FullPath, Mask = GetCurrentDatDir();
   TStringList *FileList = new TStringList;

   UINT MaskedFiles = pDAT->CalculateFiles(Mask);
   NewProgressBar(MaskedFiles); //Create progress bar on form
   StatusBar1->Panels->Items[0]->Text = "Read ...";
   StatusBar1->Update();
   ListView1->AllocBy = MaskedFiles;

   UINT Index = 0;
   UINT StepValue = MaskedFiles / 15 + 1;
   pDAT->ResetPtr();
   while (pDAT->FindFile(Mask, TRUE))
   {
      FullPath = pDAT->GetDirOrFile(Mask);
      ImageIndex = 3;
      if (FullPath.Pos("\\"))
      {
         FullPath = FullPath.SubString(0, FullPath.Length() - 1);
         ImageIndex = 0;
      }

      lFound = FALSE;
      for (i = FileList->Count; i > 0; i--)
         if (FileList->Strings[i - 1] == FullPath)
         {
            lFound = TRUE;
            break;
         }
      if (!lFound)
      {
         FileList->Add(FullPath);
         if (!ImageIndex)//Verify if <DIR>
         {
            ListItem = ListView1->Items->Insert(DirCount);
            DirCount ++;
            ListItem->SubItems->Add("<DIR>");
         }
         else
         {
            ListItem = ListView1->Items->Add();
            if (pDAT->FileType)
               ListItem->SubItems->Add("Packed");
            else
               ListItem->SubItems->Add("Normal");
            ListItem->SubItems->Add((String)pDAT->RealSize);
            ListItem->SubItems->Add((String)pDAT->PackedSize);
            ListItem->SubItems->Add((String)pDAT->Offset);
         }
         ListItem->Caption = FullPath;
         ListItem->ImageIndex = ImageIndex;
      }
      if (!(++Index%StepValue))
         FpProgress->StepIt();
   }
   pItems->EndUpdate();
   delete FileList;
   StatusBar1->Panels->Items[0]->Text = "Ready";
   StatusBar1->Panels->Items[1]->Text = String(DirCount) + " folder(s) and " +
                                 String(pItems->Count - DirCount) + " file(s)";
   StatusBar1->Panels->Items[3]->Text = Mask;
   DelProgressBar();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListView1SelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
   StatusBar1->Panels->Items[2]->Text = (String)ListView1->SelCount +
                                          " item(s) selected";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopupMenu1Popup(TObject *Sender)
{
   if (!ListView1->SelCount)
   {
      Extractto1->Enabled = FALSE;
      Ext_to1->Enabled = FALSE;
      Open2->Enabled = FALSE;
   }
   else
   {
      Extractto1->Enabled = TRUE;
      Ext_to1->Enabled = TRUE;
      Open2->Enabled = TRUE;
   }

   if (!ListView1->Items->Count)
      Selectall1->Enabled = FALSE;
   else
      Selectall1->Enabled = TRUE;

   ExtrAll1->Enabled = FALSE;
   Extra1->Enabled = FALSE;

   if (pDAT == NULL)
   {
      Selectedfiles1->Enabled = FALSE;
      Selectedfiles2->Enabled = FALSE;
      New2->Enabled = FALSE;
      Fromfolder1->Enabled = FALSE;
      Fromfolder2->Enabled = FALSE;
   }
   else
   {
      Selectedfiles1->Enabled = TRUE;
      Selectedfiles2->Enabled = TRUE;
      New2->Enabled = TRUE;
      Fromfolder1->Enabled = TRUE;
      Fromfolder2->Enabled = TRUE;
      if (pDAT->FilesTotal)
      {
         ExtrAll1->Enabled = TRUE;
         Extra1->Enabled = TRUE;
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListView1DblClick(TObject *Sender)
{
   TListItem *ListItem = ListView1->Selected;
   TTreeNode *PrevNode, *Node = TreeView1->Selected;
   bool lUpDir = String(Sender->ClassName()) == "TToolButton";

   if (!Node) return;                    //if first dir
   if (!ListView1->SelCount && !lUpDir) return;

   if (!lUpDir)
      if (ListItem->Caption.Pos(".") && !lUpDir)
      {  //File DblClick
         String xxx = ExtractFiles(ID_TEMP, ID_SELECTED_FILES);
         OpenFile(xxx);
         return;
      }

   String Path = GetCurrentDatDir();      //Directory dblclick
   UINT i;
   if (lUpDir)                         //Up dir pressed
   {
      if (Path == "") //!Node->AbsoluteIndex
         return;
      PrevNode = Node->Parent;
      i = PrevNode->AbsoluteIndex;
   }
   else
   {
      Path += ListItem->Caption + "\\";
      for (i = PathList->Count; i > 0; i--)
         if (PathList->Strings[i - 1] == Path)
            break;
   }
   Node->Expand(TRUE);
   TreeView1->Items->Item[i]->Selected = TRUE;
   Node = TreeView1->Selected;
   TreeView1->TopItem = Node;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenFile(String filename)
{
   //--FRM FILE---
   if (filename.LowerCase().Pos(".frm"))
   {
      pFRM = new TFrmFile(filename);
      delete pFRM;
      return;
   }
   //--MSG FILE---
   if (filename.LowerCase().Pos(".msg"))
   {
      TfrmMsg *pMSGForm = new TfrmMsg(Form1, filename);   //Create MSG viewer
      return;
   }
   // For unknown types
   int err = (int)ShellExecute(Handle, "open", filename.c_str() ,0, 0,SW_SHOW);
      if (err == SE_ERR_ASSOCINCOMPLETE || err == SE_ERR_NOASSOC)
         ShellExecute(Handle, "open", "notepad.exe", filename.c_str(), 0, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Selectall1Click(TObject *Sender)
{
   TListItems *pItems = ListView1->Items;
   for (int i = 0; i < pItems->Count ; i++)
      pItems->Item[i]->Selected = TRUE;
}
//---------------------------------------------------------------------------
String __fastcall TForm1::GetCurrentDatDir(void)
{
   int NodeIndex = TreeView1->Selected->AbsoluteIndex;
   if (NodeIndex > 0 && PathList->Count)
      return PathList->Strings[NodeIndex - 1];
   else
      return "";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewProgressBar(UINT MaxValue)
{
   FpProgress = new TProgressBar(StatusBar1);
   FpProgress->Parent = StatusBar1;
   FpProgress->Top = 3;
   FpProgress->Height = StatusBar1->Height - 4;
   FpProgress->Left = 81;
   FpProgress->Width = 200;
   FpProgress->Enabled = true;
   FpProgress->Min = 0;
   FpProgress->Max = MaxValue;
   FpProgress->Step = MaxValue / 15;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DelProgressBar(void)
{
   if (FpProgress)
      delete FpProgress;
}
//---------------------------------------------------------------------------
String __fastcall TForm1::ExtractFiles(bool idWhere, bool idMask)
{
   String Path, DestinationFolder, retFile = "";
   TListItems *pItems = ListView1->Items;
   TTreeNode *Node = TreeView1->Selected;
   TStringList *m_MaskList = new TStringList;

   if (idWhere == ID_TEMP)
      DestinationFolder = WinTempDir;
   else
      DestinationFolder = SelectFolder("Choose a Folder in which to extract the file(s).");
   if (DestinationFolder.IsEmpty())  //If user pressed CANCEL
      return NULL;

   if (idMask == ID_ALL_FILES) //  Extract All files
   {
      pDAT->ExtractFiles(m_MaskList, DestinationFolder);
      goto end;
   }

   if (!Node->AbsoluteIndex)  //Verify tree at folder
      Path = "";
   else
      Path = PathList->Strings[Node->AbsoluteIndex - 1];

   for (int i = 0; i < pItems->Count; i++)   //Adding selected items to list
      if (pItems->Item[i]->Selected)
         m_MaskList->Add(Path + pItems->Item[i]->Caption);

   retFile = pDAT->ExtractFiles(m_MaskList, DestinationFolder);
end:
   if (pDAT->lError)
      pDAT->ShowError();
   delete m_MaskList;

   return retFile;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Extractto1Click(TObject *Sender)
{
   ExtractFiles(ID_CHOOSEDIR, ID_SELECTED_FILES);
}
//---------------------------------------------------------------------------
String __fastcall TForm1::SelectFolder(char *msg)
{
    // Use Windows 95 Directory box to get dir
        BROWSEINFO bi;
        char WDir[MAX_PATH];
  	char FolderName[MAX_PATH];
  	LPITEMIDLIST ItemID;

  	memset(&bi, 0, sizeof(BROWSEINFO));
  	memset(WDir, 0, MAX_PATH);
  	bi.hwndOwner = Handle;
  	bi.pszDisplayName = FolderName;
  	bi.lpszTitle = msg;
        bi.ulFlags = BIF_RETURNONLYFSDIRS|BIF_DONTGOBELOWDOMAIN;
  	ItemID = SHBrowseForFolder(&bi);
  	SHGetPathFromIDList(ItemID, WDir);
  	GlobalFreePtr(ItemID);
        return String(WDir);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormPaint(TObject *Sender)
{
   if (lPainted)
      return;
   lPainted = TRUE;
   TStartupForm *pStartForm = new TStartupForm(Form1);   //Create extract form
   pStartForm->ShowModal();
}
//---------------------------------------------------------------------------
//>>>>>>    Import from directory      <<<<<<<<<
void __fastcall TForm1::Fromfolder1Click(TObject *Sender)
{
   String fldr = SelectFolder("Select import directory:");
   if (fldr.IsEmpty())  //If user pressed CANCEL
      return ;
   TStringList *Files = new (TStringList);
   ImportFromFolder(fldr, Files);
   pDAT->ImportFiles(Files, fldr, GetCurrentDatDir());
   FillTree();
   delete Files;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ImportFromFolder(String fldr, TStrings *Files)
{
   String FstrCurFileName;

   WIN32_FIND_DATA FindFileData;
   AnsiString strSearchFiles = fldr + "\\*.*"; //FstrDirectory + FstrFileMask
   HANDLE hFirstFileHandle = FindFirstFile(strSearchFiles.c_str(), &FindFileData );

   while (hFirstFileHandle)
   {
      FstrCurFileName = fldr + "\\" + FindFileData.cFileName; // Add to arrray
      if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
         Files->Add(FstrCurFileName);
      else
         if ((String)FindFileData.cAlternateFileName != "")
            ImportFromFolder(FstrCurFileName, Files);
      if (FindNextFile(hFirstFileHandle, &FindFileData) == FALSE)
         break;
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Selectedfiles1Click(TObject *Sender)
{  // Import selected file(s)
   OpenDialog1->Filter = "Any files (*.*)|*.*";
   OpenDialog1->FileName = "";
   OpenDialog1->Options.Clear();
   OpenDialog1->Options << ofAllowMultiSelect <<ofEnableSizing;
   if (!OpenDialog1->Execute())
      return;
   pDAT->ImportFiles(OpenDialog1->Files, NULL, GetCurrentDatDir());
   RefreshList();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CreateFolder(int AbsIndex, String NewFolder)
{
   TTreeNodes* pItems = TreeView1->Items;
   TTreeNode *pNode;
//   TListItem  *ListItem;

   while (NewFolder == "") //Create new custom folder
   {
      NewFolder = GetFolderNameForm(STR_CREATE_FOLDER);
      if (NewFolder == NULL)
         return;
   }

   pNode = pItems->AddChild(pItems->Item[AbsIndex], NewFolder);
   pNode->ImageIndex = 0;
   pNode->SelectedIndex = 1;
   int NewIndex = pNode->AbsoluteIndex;
   if (AbsIndex)
      PathList->Insert(NewIndex - 1, PathList->Strings[AbsIndex - 1] + NewFolder + "\\");
   else
      PathList->Insert(NewIndex - 1, NewFolder + "\\");
   pNode->Selected = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FillDefaultDirs(void)
{
   HWND hWndParent = GetActiveWindow();
   if(MessageBox(hWndParent, "Create default folders?", "Question", MB_APPLMODAL|MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1) == ID_NO)
      return;

   TTreeNodes* pItems = TreeView1->Items;
   pItems->BeginUpdate();

   CreateFolder(0, "art");
   CreateFolder(1, "backgrnd");
   CreateFolder(1, "cuts");
   CreateFolder(1, "heads");
   CreateFolder(1, "intrface");
   CreateFolder(1, "inven");
   CreateFolder(1, "items");
   CreateFolder(1, "misc");
   CreateFolder(1, "scenery");
   CreateFolder(1, "skilldex");
   CreateFolder(1, "splash");
   CreateFolder(1, "tiles");
   CreateFolder(1, "walls");
   CreateFolder(0, "data");
   CreateFolder(0, "maps");
   CreateFolder(0, "premade");
   CreateFolder(0, "proto");
   CreateFolder(17, "CRITTERS");
   CreateFolder(17, "ITEMS");
   CreateFolder(17, "MISC");
   CreateFolder(17, "SCENERY");
   CreateFolder(17, "TILES");
   CreateFolder(22, "PATTERNS");
   CreateFolder(17, "WALLS");
   CreateFolder(0, "scripts");
   CreateFolder(0, "sound");
   CreateFolder(26, "SFX");
   CreateFolder(26, "Speech");
   CreateFolder(28, "BOSSS");
   CreateFolder(28, "ELDER");
   CreateFolder(28, "HAKU2");
   CreateFolder(28, "HAKUN");
   CreateFolder(28, "HRLD2");
   CreateFolder(28, "LYNET");
   CreateFolder(28, "MRCUS");
   CreateFolder(28, "MYRON");
   CreateFolder(28, "narrator");
   CreateFolder(28, "POWER");
   CreateFolder(28, "PRESI");
   CreateFolder(28, "SULIK");
   CreateFolder(28, "TNDI2");
   CreateFolder(0, "text");
   CreateFolder(42, "english");
   CreateFolder(43, "cuts");
   CreateFolder(43, "dialog");
   CreateFolder(43, "game");

   pItems->EndUpdate();
   pItems->Item[0]->Expand(TRUE);
   TreeView1->TopItem = pItems->Item[0];
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Createnewfolder1Click(TObject *Sender)
{
   CreateFolder(TreeView1->Selected->AbsoluteIndex, "");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TreeView1Change(TObject *Sender, TTreeNode *Node)
{
   RefreshList();
}
//---------------------------------------------------------------------------
String __fastcall TForm1::GetFolderNameForm(String Caption)
{
   String RetValue, TempDir;
   bool lExisting;
   TInputDir *pCreateFolderForm = new TInputDir(Form1);   //Create extract form
   pCreateFolderForm->Label1->Caption = Caption;
   do
   {
      lExisting = FALSE;
      pCreateFolderForm->FolderName = "?";
      pCreateFolderForm->ShowModal();
      pCreateFolderForm->Hide();
      RetValue = pCreateFolderForm->FolderName;
      TempDir = GetCurrentDatDir() + RetValue + "\\";
      for (int I = 0; I < PathList->Count; I ++)
         if (PathList->Strings[I].Pos(TempDir))
            lExisting = TRUE;
      if (!lExisting)
         break;
   } while (TRUE);
   delete pCreateFolderForm;
   if (RetValue.Pos("\\") || RetValue.Pos("/") || RetValue.Pos(":")
       || RetValue.Pos("*") || RetValue.Pos("?") || RetValue.Pos('"')
       || RetValue.Pos("<") || RetValue.Pos(">") || RetValue.Pos("|"))
      return NULL;
   else
      return RetValue;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PopupMenu2Popup(TObject *Sender)
{
   if (pDAT == NULL)
      NewFolder1->Enabled = FALSE;
   else
      NewFolder1->Enabled = TRUE;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Extra1Click(TObject *Sender)
{
   ExtractFiles(ID_CHOOSEDIR, ID_ALL_FILES);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
   if (PathList)
      delete PathList;
   if (pDAT)
      delete pDAT;
}
//---------------------------------------------------------------------------
