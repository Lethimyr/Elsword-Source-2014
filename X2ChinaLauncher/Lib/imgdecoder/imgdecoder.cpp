//
// For instructions read the header file.
//
// Created by Rui Godinho Lopes <ruiglopes@yahoo.com>
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED. USE IT AT YOUT OWN RISK. THE AUTHOR ACCEPTS NO
// LIABILITY FOR ANY DATA DAMAGE/LOSS THAT THIS PRODUCT MAY CAUSE.
//
// imgdecoder.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "imgdecoder.h"
#include <math.h>
#include <malloc.h>
#include "planydec.h"
#include "plwinbmp.h"
#include "pldibsect.h"
//#include "anydec.h"
//#include "winbmp.h"
//#include "dibsect.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		::DisableThreadLibraryCalls((HINSTANCE)hModule);

	/*switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}*/

	return TRUE;
}


DWORD _stdcall ImgNewDecoder(LPVOID /*out*/*ppDecoder)
{
	if (!ppDecoder)
		return -1;
	*ppDecoder= new PLAnyPicDecoder;
	return *ppDecoder? 0 : -2;
}


DWORD _stdcall ImgDeleteDecoder(LPVOID /*in*/pDecoder)
{
	delete (PLAnyPicDecoder*)pDecoder;
	return 0;
}


DWORD _stdcall ImgNewDIBFromFile(LPVOID /*in*/pDecoder, LPCTSTR /*in*/pFileName, LPVOID /*out*/*pImg)
{
	if (!pDecoder || !pImg)
		return -1;

	PLAnyPicDecoder& Decoder= *(PLAnyPicDecoder*)pDecoder;
	*pImg= NULL;
	PLDIBSection *pBmp= NULL;

	int nErrorCode= 0;
	try
	{
		pBmp= new PLDIBSection;
		if (!pBmp)
			return -1;
		Decoder.MakeBmpFromFile(pFileName, pBmp, PLPixelFormat::A8R8G8B8);
		*pImg= pBmp;
	}
	catch (PLTextException e)
	{
		delete pBmp;
		nErrorCode= e.GetCode();
	}
	return nErrorCode;
}

DWORD _stdcall ImgNewDIBFromMem(LPVOID /*in*/pDecoder, UCHAR* /*in*/pMem, INT iMemSize, LPVOID /*out*/*pImg)
{
	if (!pDecoder || !pImg)
		return -1;

	PLAnyPicDecoder& Decoder= *(PLAnyPicDecoder*)pDecoder;
	*pImg= NULL;
	PLDIBSection *pBmp= NULL;

	int nErrorCode= 0;
	try
	{
		pBmp= new PLDIBSection;
		if (!pBmp)
			return -1;
		Decoder.MakeBmpFromMemory(pMem, iMemSize, pBmp, PLPixelFormat::A8R8G8B8);
		*pImg= pBmp;
	}
	catch (PLTextException e)
	{
		delete pBmp;
		nErrorCode= e.GetCode();
	}
	return nErrorCode;
}

DWORD _stdcall ImgNewDIBFromResource(LPVOID /*in*/pDecoder, HINSTANCE hInstResource, int ResourceID, LPVOID /*out*/*pImg)
{
	if (!pDecoder || !pImg)
		return -1;

	PLAnyPicDecoder& Decoder= *(PLAnyPicDecoder*)pDecoder;
	*pImg= NULL;
	PLDIBSection *pBmp= NULL;

	int nErrorCode= 0;
	try
	{
		pBmp= new PLDIBSection;
		if (!pBmp)
			return -1;
		Decoder.MakeBmpFromResource(hInstResource, ResourceID, pBmp, PLPixelFormat::A8R8G8B8);
		*pImg= pBmp;
	}
	catch (PLTextException e)
	{
		delete pBmp;
		nErrorCode= e.GetCode();
	}
	return nErrorCode;
}

DWORD _stdcall ImgNewDIBFromURL(LPVOID /*in*/pDecoder, const char* /*in*/pURL, LPVOID /*out*/*pImg)
{
	if (!pDecoder || !pImg)
		return -1;

	PLAnyPicDecoder& Decoder= *(PLAnyPicDecoder*)pDecoder;
	*pImg= NULL;
	PLDIBSection *pBmp= NULL;

	int nErrorCode= 0;
	try
	{
		pBmp= new PLDIBSection;
		if (!pBmp)
			return -1;
		Decoder.MakeBmpFromURL(pURL, pBmp, PLPixelFormat::A8R8G8B8);
		*pImg= pBmp;
	}
	catch (PLTextException e)
	{
		delete pBmp;
		nErrorCode= e.GetCode();
	}
	return nErrorCode;
}

DWORD _stdcall ImgDeleteDIBSection(LPVOID /*in*/pImg)
{
	delete (PLDIBSection *)pImg;
	return 0;
}


DWORD _stdcall ImgGetHandle(LPVOID /*in*/pImg, HBITMAP /*out*/*pBitmap, LPVOID /*out*/*ppDIBBits)
{
	if (!pImg || !pBitmap || !ppDIBBits)
		return -1;

	PLDIBSection &Img= *(PLDIBSection *)pImg;

	*pBitmap= (HBITMAP)Img.GetHandle();
	*ppDIBBits= Img.GetBits();

	return 0;
}
