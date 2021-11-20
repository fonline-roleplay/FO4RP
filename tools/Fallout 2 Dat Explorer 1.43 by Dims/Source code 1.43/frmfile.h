//---------------------------------------------------------------------------
#ifndef frmfileH
#define frmfileH

#include <Classes.hpp>
//---------------------------------------------------------------------------
class TFrmFile
{
public:
   unsigned char *FrmBuf, *pBegin, *pEnd, *ptr;
   long FileSize;
   int width, height, aligned_width, frame_size;
   PALETTEENTRY pal[256];
   HANDLE h_frm;
   bool lError;
   UINT ErrorType;

   TFrmFile(String filename);
   virtual ~TFrmFile();
protected:

};
#endif
