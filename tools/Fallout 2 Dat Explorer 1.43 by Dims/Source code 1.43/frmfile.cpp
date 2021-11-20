//////////////////////////////////////////////////////////////////////
// TFrmFile Class
//////////////////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include "frmfile.h"
#include "error.h"
#include "frmfrm.h"
#include "mainform.h"
#include "palette.h"

//------------------------------------------------------------------------------
TFrmFile::TFrmFile(String filename)
{
   h_frm = CreateFile(filename.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

   if (h_frm == INVALID_HANDLE_VALUE)
   {
      ErrorType = ERR_CANNOT_OPEN_FILE;
      return;
   }
   FileSize = GetFileSize(h_frm, NULL);
   if ((FrmBuf = (char *)malloc(FileSize)) == NULL)
   {
      ErrorType = ERR_ALLOC_MEMORY;
      return;
   }
   unsigned long i;
   ReadFile(h_frm, FrmBuf, FileSize, &i, NULL);
   CloseHandle(h_frm);

   pBegin = FrmBuf + 0x3e;
   pEnd = FrmBuf + FileSize;

   width = pBegin[0] * 256 + pBegin[1];
   height = pBegin[2] * 256 + pBegin[3];
   aligned_width = (4 - width % 4) % 4;
   frame_size = (pBegin[5] * 256 + pBegin[6]) * 256 + pBegin[7];
   ptr = pBegin + 12 + width * (height-1);

   HBITMAP image = NULL;
   image = CreateBitmap(width, height, 1, 8, pBegin + 0x0C);//
   Graphics::TBitmap *pBitmap;
   pBitmap = new (Graphics::TBitmap);
   pBitmap->Handle = image;
   pBitmap->Width = width;
   pBitmap->Height = height;
   for (int i = 0; i < 256; i++)
   {
      pal[i].peBlue = f2_palette[i * 4];
      pal[i].peGreen = f2_palette[i * 4 + 1];
      pal[i].peRed = f2_palette[i * 4 + 2];
      pal[i].peFlags = NULL;
   }

   TFrmForm *pFrmForm;
   pFrmForm = new TFrmForm(Form1);
   pFrmForm->Show();
//   pFrmForm->Canvas->Draw((pFrmForm->Width/2) - (pBitmap->Width/2),
//               (pFrmForm->Height/2) - (pBitmap->Height/2), pBitmap);
//   pFrmForm->Canvas->Draw(10, 10, (Graphics::TBitmap)image); LOadFormFile

 }
//------------------------------------------------------------------------------
TFrmFile::~TFrmFile()
{
   if (FrmBuf != NULL)
      free(FrmBuf);
}
