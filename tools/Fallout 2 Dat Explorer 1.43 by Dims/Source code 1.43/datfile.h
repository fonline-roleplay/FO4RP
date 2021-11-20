#if !defined(_DATFILE_H)
#define _DATFILE_H

#include <windows.h>
#include <Classes.hpp>

#include <stdio.h>
#include <dir.h>
#include <alloc.h>
#include <stdlib.h>
#include "extract.h"

class TDatFile
{
public:
   char FileName[256];
   String FullName;
   BYTE  FileType; //если там 1, то файл считается компрессированым(не всегда).
   DWORD RealSize; //Размер файла без декомпрессии
   DWORD PackedSize; //Размер сжатого файла
   DWORD Offset; //Адрес файла в виде смещения от начала DAT-файла.
   ULONG FileIndex; //Index of file in DAT
   ULONG FileNameSize;

   bool lError;
   bool Fallout1; //Added for Fallout1
   UINT ErrorType;

   HANDLE h_in, h_out, h_imp; //Handles: (DAT, output, import) files

   BYTE *m_pInBuf;
   TExtractForm *pExForm;
   
   String DatFileName;
   TStringList *FileList;
   ULONG FileSizeFromDat;
   ULONG TreeSize;
   ULONG tmpTreeSize;
   ULONG FilesTotal;

   bool CancelOp;

   char *buff, *ptr, *filebuff, *gzipbuff; //in buff - DATtree, ptr - pointer

   bool ReadTree(void);
   bool FindFile(String Mask, bool FillStructure);
   String ExtractFiles(TStringList *MaskList, String DestinationFolder);
   void ExtractFile(int Temp);
   bool ImportFile(HANDLE h_outdat, HANDLE h_outtree,
                   String DestinationFolder, String impfilename);
   void ImportFiles(TStrings *FilesList, String SourceFolder, String DestinationFolder);
   UINT CalculateFiles(String Mask);
   String GetPathName(void);
   String GetFileName(bool WithExtension);
   String GetDirOrFile(String Mask);
   void RevDw(DWORD *addr);
   void ResetPtr(void);
   void ShowError(void);

   TDatFile(String filename, UINT idType);
   virtual ~TDatFile();

protected:
   void FindInDirectory(String PathAndMask);

};

#endif
