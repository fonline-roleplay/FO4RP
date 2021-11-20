//////////////////////////////////////////////////////////////////////
// TDatFile Class
//////////////////////////////////////////////////////////////////////

#include "macros.h"
#include "error.h"
#include "datfile.h"
#include <memory.h>
#include <stdio.h>

#include "FileCtrl.hpp"
#include "extract.h"
#include "mainform.h"

#include "zlib\zlib.h"
#include "lz77c.h"

char *error_types[] = {
   "Cannot open file.",                       // ERR_CANNOT_OPEN_FILE
   "File invalid or truncated.",              // ERR_FILE_TRUNCATED
   "This file not supported.",                // ERR_FILE_NOT_SUPPORTED
   "Not enough memory to allocate buffer.",   // ERR_ALLOC_MEMORY
   "Cannot open DAT file.",                   // ERR_CANNOT_OPEN_DAT_FILE
   "Cannot create output file.",              // ERR_CANNOT_CREATE_OUT_FILE
   "Cannot write to output file. Disk full?", // ERR_CANNOT_WRITE_TO_FILE
   "Fatal error."                             // ERR_UNKNOWN
};
DWORD emptydatheader[]={0x00, 0x04, 0x0c};
//------------------------------------------------------------------------------
TDatFile::TDatFile(String filename, UINT DatType)
{
   lError = TRUE;
   buff = NULL;
   m_pInBuf = NULL;
   Fallout1 = FALSE;
   unsigned long i;

   if (DatType == ID_EMPTYDAT)
   {
      h_in = CreateFile(filename.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE,
		NULL);

      if (h_in == INVALID_HANDLE_VALUE)
      {
         ErrorType = ERR_CANNOT_CREATE_OUT_FILE;
         return;
      }
      if (WriteFile(h_in, emptydatheader, 12, &i, NULL) == 0) //not compressed
      {
          ErrorType = ERR_CANNOT_WRITE_TO_FILE;
          return;
      }
      CloseHandle(h_in);
   }

   h_in = CreateFile(filename.c_str(),
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
   if (h_in == INVALID_HANDLE_VALUE)
   {
       ErrorType = ERR_CANNOT_OPEN_FILE;
       return;
   }
   if (!ReadTree())
      return;
   m_pInBuf = (BYTE *) malloc(ZLIB_BUFF_SIZE);
   if (m_pInBuf == NULL)
   {
      ErrorType = ERR_ALLOC_MEMORY;
      return;
   }
   lError = FALSE;
   DatFileName = filename;
   return;
}
//------------------------------------------------------------------------------
bool TDatFile::ReadTree()
{
   unsigned long i, pof, n, m, F1DirCount, F1FilesTotal, F1StrLenDW,
                  F1compressed, F1offset, F1filesize, F1unknown;
   BYTE F1StrLen;
   char F1StrBuf[255];
   TStringList *dirs = new TStringList;

   i = SetFilePointer(h_in, -8, NULL, FILE_END);
   if (i == 0xFFFFFFFF)
   {
       ErrorType = ERR_FILE_TRUNCATED;
       return FALSE;
   }
   ReadFile(h_in, &TreeSize, 4, &i, NULL);
   ReadFile(h_in, &FileSizeFromDat, 4, &i, NULL);
   i = SetFilePointer(h_in, 0, NULL, FILE_BEGIN); //Added for Fallout1
   ReadFile(h_in, &F1DirCount, 4, &i, NULL); //Added for Fallout1
   RevDw(&F1DirCount); //Added for Fallout1
   if (F1DirCount == 0x01 || F1DirCount == 0x33) Fallout1 = TRUE; //Added for Fallout1
   if (GetFileSize(h_in, NULL) != FileSizeFromDat && Fallout1 == FALSE)
   {
      ErrorType = ERR_FILE_NOT_SUPPORTED;
      return FALSE;
   }
   if (!Fallout1)
   {
      i=SetFilePointer (h_in, -(TreeSize+8), NULL, FILE_END);
      ReadFile(h_in, &FilesTotal, 4, &i, NULL);
   }
   else //FALLOUT 1 !!!
   {
      // Получаем длинну DAT файла
      FileSizeFromDat = GetFileSize(h_in, NULL);
      // Пропускаем первые 2 байта
      i=SetFilePointer (h_in, 16, NULL, FILE_BEGIN);
      // Некоторые начальные установки
      TreeSize = 0;
      FilesTotal = 0;
      pof = 16;
      // Заполняем массив директориями
      for (n = 1; n <= F1DirCount; n++)
      {
         ReadFile(h_in, &F1StrLen, 1, &i, NULL);
         ReadFile(h_in, &F1StrBuf, F1StrLen, &i, NULL);
         pof += F1StrLen + 1;
         F1StrBuf[F1StrLen++] = 0x5C;
         if (F1StrLen == 2) F1StrLen = 0;
         F1StrBuf[F1StrLen] = 0x00;
         dirs->Add(String(F1StrBuf));
      }
      // Подсчитываем длинну дерева необходимую для конвертации
      for (n = 1; n <= F1DirCount; n++)
      {
         ReadFile(h_in, &F1FilesTotal, 4, &i, NULL);
         RevDw(&F1FilesTotal);
         i=SetFilePointer(h_in, 12, NULL, FILE_CURRENT);
         for (m = 1; m <= F1FilesTotal; m++)
         {
            FilesTotal++;
            ReadFile(h_in, &F1StrLen, 1, &i, NULL);
            TreeSize += dirs->Strings[n - 1].Length() + F1StrLen + 17; //17=4+1+12
            i=SetFilePointer(h_in, F1StrLen + 16, NULL, FILE_CURRENT);
         }
      }
      // В TreeSize теперь длинна дерева необходимая для конвертации
   }

   if (buff != NULL)
      free(buff);
   if ((buff = (char *)malloc(TreeSize)) == NULL)
   {
      ErrorType = ERR_ALLOC_MEMORY;
      return FALSE;
   }
   ZeroMemory(buff, TreeSize);

   if (!Fallout1)
      ReadFile(h_in, buff, TreeSize - 4, &i, NULL);
   else
   {
      ptr = buff;
      i=SetFilePointer(h_in, pof, NULL, FILE_BEGIN); //Restore old file pointer
      TreeSize += 4; // Учитывается 4 байта под FilesTotal
      for (n = 1; n <= F1DirCount; n++)
      {
         ReadFile(h_in, &F1FilesTotal, 4, &i, NULL);
         RevDw(&F1FilesTotal);
         i=SetFilePointer(h_in, 12, NULL, FILE_CURRENT);
         for (m = 1; m <= F1FilesTotal; m++)
         {
            ReadFile(h_in, &F1StrLen, 1, &i, NULL);
            F1StrLenDW = dirs->Strings[n - 1].Length() + F1StrLen;
            memcpy(ptr, &F1StrLenDW, 4);
            ptr += 4;
            memcpy(ptr, dirs->Strings[n - 1].c_str(), dirs->Strings[n - 1].Length());
            ptr += dirs->Strings[n - 1].Length();
            ReadFile(h_in, ptr, F1StrLen, &i, NULL);
            ptr += F1StrLen;
            ReadFile(h_in, &F1compressed, 4, &i, NULL);
            RevDw(&F1compressed);
            ReadFile(h_in, &F1offset, 4, &i, NULL);
            RevDw(&F1offset);
            ReadFile(h_in, &F1filesize, 4, &i, NULL);
            RevDw(&F1filesize);
            ReadFile(h_in, &F1unknown, 4, &i, NULL);
            RevDw(&F1unknown);
            if (F1compressed == 0x40) //compressed
               *ptr = 0x01;
            else
               *ptr = 0x00;
            ptr ++;
            memcpy(ptr, &F1filesize, 4); // RealSize
            ptr += 4;
            memcpy(ptr, &F1unknown, 4); // PackedSize
            ptr += 4;
            memcpy(ptr, &F1offset, 4); // Offset
            ptr += 4;
         }
      }
      delete dirs;
   }

   return TRUE;
}
//------------------------------------------------------------------------------
void TDatFile::ResetPtr(void)
{
   ptr = buff;
   FileIndex = 0;
}
//------------------------------------------------------------------------------
bool TDatFile::FindFile(String Mask, bool FillStructure)
{
   while (FileIndex < FilesTotal)
   {
      FileIndex ++;
      FileNameSize = *(ULONG *)ptr;
      ptr += 4;
      memcpy(&FileName, ptr, FileNameSize);
      FileName[FileNameSize] = 0;
      FullName = (AnsiString)FileName;
      if(FullName.Pos(Mask) || !Mask.Length())
      {
         if (FillStructure)
         {
            FileType = *(ptr + FileNameSize);
            RealSize = *(DWORD *)(ptr + FileNameSize + 1);
            PackedSize = *(DWORD *)(ptr + FileNameSize + 5);
            if (!FileType && !Fallout1)
               if (RealSize != PackedSize)
                  FileType = 1; // Fixing bug
            Offset = *(DWORD *)(ptr + FileNameSize + 9);
         }
         ptr += FileNameSize + 13;
         return TRUE;
      }
      ptr += FileNameSize + 13;
   }
   return FALSE;
}
//------------------------------------------------------------------------------
String TDatFile::ExtractFiles(TStringList *MaskList, String DestinationFolder)
{
   int i, n, MaskedFilesTotal = 0;
   String RetFileName;
   FileList = new TStringList;

   lError = FALSE;
   CancelOp = FALSE;
   n = MaskList->Count;

   pExForm = new TExtractForm(Form1);   //Create extract form
   pExForm->Show();
   pExForm->Caption = "Extract file(s)";
   pExForm->Label1->Caption = "Preparing to extract ...";
   pExForm->Update();

   if (n == 0)
      MaskedFilesTotal = FilesTotal;
   else
   {
      for (i = 0; i < MaskList->Count; i++)    //Calculate masked files
      {
         ResetPtr();
         while (FindFile(MaskList->Strings[i], FALSE))
            MaskedFilesTotal ++;
      }
   }

   bool Found;
   UINT Step = MaskedFilesTotal / 25 + 1;
   UINT Index = 0;
   pExForm->ProgressBar1->Max = MaskedFilesTotal;
   pExForm->ProgressBar1->Step = Step;

   ResetPtr();
   while (FindFile("", TRUE))
   {
      Found = FALSE;
      Index ++;
      for (i = 0; i < MaskList->Count; i++)
      {
         if (FullName.Pos(MaskList->Strings[i]))
         {
            pExForm->Label1->Caption = FullName;
            pExForm->Label1->Update();
            RetFileName = FullName;
            Found = TRUE;
            break;
         }
      }
      if (Found || n == 0)
      {
         if (n != 0)
            if (MaskList->Strings[i].Pos("."))   //If file
               MaskList->Delete(i);
         if (Index % Step == 0)
         {
            pExForm->ProgressBar1->StepIt();
            pExForm->ProgressBar1->Update();
         }
         ForceDirectories(DestinationFolder + "\\" + GetPathName());
         Application->ProcessMessages();
         FullName = DestinationFolder + "\\" + FullName;
         if (DestinationFolder == Form1->WinTempDir)
            ExtractFile(1);
         else
            ExtractFile(0);
         if (lError || CancelOp)
            break;
      }
   }
   delete pExForm;
   if (FileList)
      delete FileList;
   return (DestinationFolder + "\\" + RetFileName);
}
//------------------------------------------------------------------------------
void TDatFile::ExtractFile(int Temp)
{
   filebuff = NULL;
   gzipbuff = NULL;
   h_out = NULL;
   unsigned long i = 0;
   unsigned __int64 percent;
   
   if (FileType)
   {
       if (PackedSize || !Fallout1)
       {
          gzipbuff = (char *) malloc(RealSize);
          if (gzipbuff == NULL)
          {
             ErrorType = ERR_ALLOC_MEMORY;
             lError = TRUE;
             goto err;
          }
       }
       if (Fallout1) // Fallout1 LZ77 compressed file
          ExtractF1(h_in, Offset, PackedSize, FullName, Form1->WinTempDir);
   }
   else
   {
       if (PackedSize || Fallout1)
       {
          filebuff = (char *) malloc(RealSize);
          if (filebuff == NULL)
          {
             ErrorType = ERR_ALLOC_MEMORY;
             lError = TRUE;
             goto err;
          }
      }
   }

   i = SetFilePointer (h_in, Offset, NULL,FILE_BEGIN);

   if (FileType)     //if compressed
   {
	z_stream stream;
 	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.next_in = Z_NULL;
	stream.avail_in = 0;
	if (::inflateInit (&stream)!=Z_OK)
        {
           ErrorType = ERR_UNKNOWN;
           lError = TRUE;
           goto err;
	}

	stream.next_out = gzipbuff;
	stream.avail_out = RealSize; // ZLIB_BUFF_SIZE <-----!!! Fixing bug !!!-

	int res = Z_OK;

 	while ( res == Z_OK )
        {
	   if (stream.avail_in == 0) // stream.avail_in == 0
	   {
              stream.next_in = m_pInBuf;
              if(::ReadFile(h_in, m_pInBuf, ZLIB_BUFF_SIZE, (unsigned long*)&stream.avail_in, NULL)==0)
              {
                 ErrorType = ERR_UNKNOWN;
                 lError = TRUE;
                 goto err;
              }
           }

	   // INFLATING BUFFER
	   res = ::inflate (&stream, Z_PARTIAL_FLUSH); //Z_NO_FLUSH

           if (stream.avail_out == 0 || res == Z_STREAM_END)
           {
	      stream.avail_out = ZLIB_BUFF_SIZE;
	      stream.next_out = gzipbuff + ZLIB_BUFF_SIZE;
           }
           if (RealSize)
           {
              percent = (100 * stream.total_out) / RealSize;
              pExForm->Label1->Caption = FullName +" [" + String(percent) + "%]";
              pExForm->Label1->Update();
           }
           if (stream.total_out == RealSize)
              res = Z_STREAM_END;
        }
	::inflateEnd (&stream);
   }
   else //if not compressed
   {
      if (::ReadFile(h_in, filebuff, RealSize, &i, NULL)==0)
      {
         ErrorType = ERR_UNKNOWN;
         lError = TRUE;
         goto err;
      }
   }
     
   if (Temp)
     h_out = CreateFile(FullName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_TEMPORARY,
		NULL);
   else
     h_out = CreateFile(FullName.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE,
		NULL);

     if (h_out == INVALID_HANDLE_VALUE)
     {
         ErrorType = ERR_CANNOT_CREATE_OUT_FILE;
         lError = TRUE;
         goto err;
     }

     if (FileType)
     {
        if (WriteFile(h_out, gzipbuff, RealSize, &i, NULL) == 0) //compressed
        {
            ErrorType = ERR_CANNOT_WRITE_TO_FILE;
            lError = TRUE;
            goto err;
        }
     }
     else
     {
        if (WriteFile(h_out, filebuff, RealSize, &i, NULL) == 0) //not compressed
        {
            ErrorType = ERR_CANNOT_WRITE_TO_FILE;
            lError = TRUE;
            goto err;
        }
      }
err: //----------------LABEL-------------------
   if (h_out != INVALID_HANDLE_VALUE)
      CloseHandle(h_out);
   if (filebuff != NULL)
      free(filebuff);
   if (gzipbuff != NULL)
      free(gzipbuff);
}
//------------------------------------------------------------------------------
void TDatFile::ImportFiles(TStrings *FilesList,
                             String SourceFolder, String DestinationFolder)
{
   String TempFolder = "";
   unsigned long i;  //WARNING !!! Это процедура очень опасна для DAT
   CancelOp = FALSE;
   lError = FALSE;

   pExForm = new TExtractForm(Form1);   //Create extract form
   pExForm->Show();
   pExForm->Caption = "Import file(s)";
   pExForm->Update();
   UINT Step = FilesList->Count / 25 + 1;
   pExForm->ProgressBar1->Max = FilesList->Count;
   pExForm->ProgressBar1->Step = Step;

   i = SetFilePointer (h_in, -(TreeSize+8), NULL, FILE_END);
   if (!SetEndOfFile(h_in))
   {
       pExForm->Close();
       ErrorType = ERR_UNKNOWN;
       ShowError();
       return;
   }  // Хвост обрезан ! (Осталась одна дата)

   HANDLE h_tmptree = CreateFile("c:\\temptree.dat",
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE,
		NULL);

   if (h_tmptree == INVALID_HANDLE_VALUE)
   {
       pExForm->Close();
       ErrorType = ERR_CANNOT_CREATE_OUT_FILE;
       ShowError();
       return;
   } // Создаём и открываем временный файл для хранения импортированного дерева.

   Offset = FileSizeFromDat - (TreeSize + 8); //Смещение для нового имп. файла
   tmpTreeSize = 0;                         //Размер нового дерева

   for (int I = 0; I < FilesList->Count; I ++)
   {
      pExForm->Label1->Caption = FilesList->Strings[I];
      pExForm->Label1->Update();
      if (SourceFolder != 0)
      {
         FullName = FilesList->Strings[I].SubString(SourceFolder.Length() + 2, 0xFFFF);
         TempFolder = GetPathName();
      }
      if (!ImportFile(h_in, h_tmptree, DestinationFolder + TempFolder,
                                        FilesList->Strings[I]) || CancelOp)
         break;
      else
      {
          if (I % Step == 0)
         {
            pExForm->ProgressBar1->StepIt();
            pExForm->ProgressBar1->Update();
         }
         pExForm->Label2->Caption = String(FilesList->Count - I) + " item(s) remaining";
         pExForm->Label2->Update();
         Application->ProcessMessages();
      }
   }
   pExForm->Label1->Caption = "Updating inforamtion ...";
   pExForm->Update();

   WriteFile(h_in, &FilesTotal, 4, &i, NULL);
   i = SetFilePointer (h_tmptree, 0, NULL, FILE_BEGIN);
   BYTE *pTmpBuff = (BYTE *)malloc(tmpTreeSize);
   ReadFile(h_tmptree, pTmpBuff, tmpTreeSize, &i, NULL);
   WriteFile(h_in, buff, TreeSize - 4, &i, NULL); //TreeSize-4 - без FilesTotal
   WriteFile(h_in, pTmpBuff, tmpTreeSize, &i, NULL);
   free(pTmpBuff);
   TreeSize += tmpTreeSize;
   WriteFile(h_in, &TreeSize, 4, &i, NULL);
   WriteFile(h_in, &FileSizeFromDat, 4, &i, NULL);

   CloseHandle(h_tmptree);

   ReadTree();
   delete pExForm;
   if (lError)
      ShowError();
}
//------------------------------------------------------------------------------
bool TDatFile::ImportFile(HANDLE h_tmpdat, HANDLE h_tmptree, 
                          String DestinationFolder, String impfilename)
{
   unsigned long i;
   unsigned __int64 percent;
   filebuff = NULL;
   gzipbuff = NULL;

   h_imp = CreateFile(impfilename.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
   if (h_imp == INVALID_HANDLE_VALUE)
   {
       ErrorType = ERR_CANNOT_OPEN_FILE;
       lError = TRUE;
       return FALSE;
   }

   i = SetFilePointer (h_imp, 0, NULL, FILE_BEGIN);
   RealSize = GetFileSize(h_imp, NULL);
   if (RealSize == 0)
   {
      ErrorType = ERR_FILE_TRUNCATED;
      lError = TRUE;
      CloseHandle(h_imp);
      return FALSE;
   }

   gzipbuff = (char *) malloc(RealSize);
   if (gzipbuff == NULL)
   {
      ErrorType = ERR_ALLOC_MEMORY;
      lError = TRUE;
      CloseHandle(h_imp);
      return FALSE;
   }

   z_stream stream;
   stream.zalloc = Z_NULL;
   stream.zfree = Z_NULL;
   stream.opaque = Z_NULL;
   stream.next_in = Z_NULL;
   stream.avail_in = 0;
   if (::deflateInit (&stream, Z_BEST_SPEED) != Z_OK)// Z_DEFAULT_COMPRESSION 
   {
      ErrorType = ERR_UNKNOWN;
      lError = TRUE;
      CloseHandle(h_imp);
      return FALSE;
   }

   stream.next_out = gzipbuff;   //Containing compressed Data
   stream.avail_out = RealSize;// RealSize

   int res = Z_OK;

   while ( res == Z_OK )
   {
      if (stream.avail_in == 0) // stream.avail_in == 0
      {
          stream.next_in = m_pInBuf;
          if(::ReadFile(h_imp, m_pInBuf, ZLIB_BUFF_SIZE, (unsigned long*)&stream.avail_in, NULL)==0)
          {
             ErrorType = ERR_UNKNOWN;
             lError = TRUE;
             return FALSE;
          }
      }

      // DEFLATING BUFFER
      res = ::deflate (&stream, Z_PARTIAL_FLUSH); //Z_PARTIAL_FLUSH, Z_FINISH, Z_FULL_FLUSH, Z_NO_FLUSH

      if (stream.avail_out == 0 && stream.total_in < RealSize)//stream.avail_out == 0 || res == Z_STREAM_END
      {
	   stream.avail_out = ZLIB_BUFF_SIZE; //ZLIB_BUFF_SIZE
	   stream.next_out = gzipbuff + ZLIB_BUFF_SIZE;
      }
      if (stream.total_in >= RealSize) //
         res = ::deflate (&stream, Z_FINISH); //Z_PARTIAL_FLUSH, Z_FINISH

      if (RealSize)
      {
         percent = (100 * stream.total_in) / RealSize;
         pExForm->Label1->Caption = impfilename +" [" + String(percent) + "%]";
         pExForm->Label1->Update();
      }
   }
   ::deflateEnd (&stream);

   PackedSize = stream.total_out;

   FileType = 1; // File Packed
   if (PackedSize >= RealSize)
   {
      // Если нет смысла сжимать файл, то он переписывается в оригинале
      i = SetFilePointer (h_imp, 0, NULL, FILE_BEGIN);
      if(::ReadFile(h_imp, gzipbuff, RealSize, &i, NULL) == 0)
      {
         ErrorType = ERR_UNKNOWN;
         lError = TRUE;
         return FALSE;
      }
      PackedSize = RealSize;
      FileType = 0;//File Normal
   }
   if (WriteFile(h_tmpdat, gzipbuff, PackedSize, &i, NULL) == 0)
   {
       ErrorType = ERR_CANNOT_WRITE_TO_FILE;
       lError = TRUE;
       return FALSE;
   }

   FullName = impfilename;
   FullName = DestinationFolder + GetFileName(TRUE);
   char *TempString = FullName.c_str();
   FileNameSize = FullName.Length();
   ULONG NeedBytes = 17 + FileNameSize;
   BYTE *pTmpTreeBuf = (BYTE *)malloc(NeedBytes);
   memcpy(pTmpTreeBuf, &FileNameSize, 4);
   memcpy(pTmpTreeBuf + 4, TempString, FullName.Length());
   memcpy(pTmpTreeBuf + 4 + FileNameSize, &FileType, 1);
   memcpy(pTmpTreeBuf + 5 + FileNameSize, &RealSize, 4);
   memcpy(pTmpTreeBuf + 9 + FileNameSize, &PackedSize, 4);
   memcpy(pTmpTreeBuf + 13 + FileNameSize, &Offset, 4);
   if (WriteFile(h_tmptree, pTmpTreeBuf, NeedBytes, &i, NULL) == 0)
   {
       ErrorType = ERR_CANNOT_WRITE_TO_FILE;
       lError = TRUE;
       return FALSE;
   }
   free(pTmpTreeBuf);

   Offset += PackedSize;
   tmpTreeSize += NeedBytes;
   FilesTotal ++;
   FileSizeFromDat += PackedSize + NeedBytes;

   CloseHandle(h_imp);
   if (filebuff != NULL)
      free(filebuff);
   if (gzipbuff != NULL)
      free(gzipbuff);
   return TRUE;
}
//------------------------------------------------------------------------------
UINT TDatFile::CalculateFiles(String Mask)
{
   ULONG FilesTotal = 0;
   ResetPtr();
   while (FindFile(Mask, FALSE))
      FilesTotal ++;
   return FilesTotal;
}
//------------------------------------------------------------------------------
String TDatFile::GetPathName(void)
{
   return FullName.SubString(1, FullName.LastDelimiter("\\"));
}
//------------------------------------------------------------------------------
String TDatFile::GetFileName(bool WithExtension)
{
   int nPosition = FullName.LastDelimiter("\\");
   if (WithExtension)
      return FullName.SubString(nPosition + 1, 255);
   return FullName.SubString(nPosition + 1,
                    FullName.LastDelimiter(".") - nPosition - 1);

}
//------------------------------------------------------------------------------
String TDatFile::GetDirOrFile(String Mask)
{
   int n;
   String LastName;
   LastName = FullName.SubString(Mask.Length() + 1, FileNameSize);
   if (n = LastName.Pos("\\"))
      return LastName.SubString(0, n);
   return LastName;
}
//------------------------------------------------------------------------------
void TDatFile::RevDw(DWORD *addr)
{
   BYTE *b, tmp;
   b = (BYTE*)addr;
   tmp = *(b + 3);
   *(b + 3) = *b;
   *b = tmp;
   tmp = *(b + 2);
   *(b + 2) = *(b + 1);
   *(b + 1) = tmp;
}
//------------------------------------------------------------------------------
void TDatFile::ShowError(void)
{
   if (lError)
      MessageBox(NULL, *(error_types + ErrorType), "Error", MB_OK);
   lError = FALSE;
}
//------------------------------------------------------------------------------
TDatFile::~TDatFile()
{
   if (h_in != INVALID_HANDLE_VALUE)
      CloseHandle(h_in);
   if (m_pInBuf != NULL)
      free(m_pInBuf);
   if (buff != NULL)
      free(buff);
}
