DWORD _stdcall ImgNewDecoder(LPVOID /*out*/*ppDecoder);
DWORD _stdcall ImgDeleteDecoder(LPVOID /*in*/pDecoder);
DWORD _stdcall ImgNewDIBFromFile(LPVOID /*in*/pDecoder, LPCTSTR /*in*/pFileName, LPVOID /*out*/*pImg);
DWORD _stdcall ImgNewDIBFromMem(LPVOID /*in*/pDecoder, UCHAR* /*in*/pMem, INT iMemSize, LPVOID /*out*/*pImg);
DWORD _stdcall ImgNewDIBFromResource(LPVOID /*in*/pDecoder, HINSTANCE hInstResource, int ResourceID, LPVOID /*out*/*pImg);
DWORD _stdcall ImgDeleteDIBSection(LPVOID /*in*/pImg);
DWORD _stdcall ImgGetHandle(LPVOID /*in*/pImg, HBITMAP /*out*/*pBitmap, LPVOID /*out*/*ppDIBBits);