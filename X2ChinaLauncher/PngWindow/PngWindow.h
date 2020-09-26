#pragma  once
#pragma comment(lib, "imgdecoder.lib")

/* part 01 :: imgdecoder */
DWORD _stdcall ImgNewDecoder(LPVOID /*out*/*ppDecoder);
DWORD _stdcall ImgDeleteDecoder(LPVOID /*in*/pDecoder);
DWORD _stdcall ImgNewDIBFromFile(LPVOID /*in*/pDecoder, const char* /*in*/pFileName, LPVOID /*out*/*pImg);
DWORD _stdcall ImgNewDIBFromMem(LPVOID /*in*/pDecoder, UCHAR* /*in*/pMem, INT iMemSize, LPVOID /*out*/*pImg);
DWORD _stdcall ImgNewDIBFromURL(LPVOID /*in*/pDecoder, const char* /*in*/pURL, LPVOID /*out*/*pImg);
DWORD _stdcall ImgDeleteDIBSection(LPVOID /*in*/pImg);
DWORD _stdcall ImgGetHandle(LPVOID /*in*/pImg, HBITMAP /*out*/*pBitmap, LPVOID /*out*/*ppDIBBits);

static CBitmap m_bmpDialog;
static LPVOID m_pImg;
static LPVOID m_pImgDecoder;

enum
{
	ERR_NOERROR									= 0,
	ERR_FILEOPEN								= -1000,	///< Error opening file
	ERR_ATTRIBUTESNOTSUPPORTED,						///< 
	ERR_INVALIDFILE,											///< Unknown file type
	ERR_NOTSUPORTEDFILEVERSION,
	ERR_CORRUPT,	
};

void WindowAlphaBlendFromPNG(const char* pFileName, HWND wnd);
void PreMultiplyRGBChannels(CBitmap &bmp, LPBYTE pBitmapBits);
void UpdateDialog(HWND wnd, CBitmap &bmp, BYTE SourceConstantAlpha=255);