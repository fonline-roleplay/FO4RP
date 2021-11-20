//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("datexpl.res");
USEFORM("mainform.cpp", Form1);
USEFORM("extract.cpp", ExtractForm);
USEFORM("aboutfrm.cpp", About);
USELIB("MEMMGR.LIB");
USELIB("LIB\ZLIB10.LIB");
USEUNIT("datfile.cpp");
USEFORM("startup.cpp", StartupForm);
USEUNIT("frmfile.cpp");
USEFORM("dirform.cpp", InputDir);
USEFORM("frmfrm.cpp", FrmForm);
USEFORM("msgfrm.cpp", frmMsg);
USEUNIT("lz77c.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Fallout2 DAT explorer";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->CreateForm(__classid(TAbout), &About);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
