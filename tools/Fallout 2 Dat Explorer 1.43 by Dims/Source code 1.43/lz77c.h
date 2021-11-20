//---------------------------------------------------------------------------
#ifndef lz77cH
#define lz77cH
//---------------------------------------------------------------------------
void InitTree(void);
void InsertNode(int r);
void DeleteNode(int p);
void Encode (void);
void Decode (void);
//int main(int argc, char *argv[]);
void ExtractF1(HANDLE h_in, ULONG Offset, ULONG PackedSize,
                            String FullName, String WinTempDir);
#endif
