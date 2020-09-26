/**********************************************************************
 *<
	FILE: XFileExp.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/

#include "XFileExp.h"
#include "CXExpManager.h"

extern SDialogOptions g_DlgOptions;

static XFileExpClassDesc XFileExpDesc;
ClassDesc2* GetXFileExpDesc() { return &XFileExpDesc; }






//--- XFileExp -------------------------------------------------------
XFileExp::XFileExp()
{    
}

XFileExp::~XFileExp() 
{    
}

int XFileExp::ExtCount()
{
	//TODO: Returns the number of file name extensions supported by the plug-in.
	return 2;
}

const TCHAR *XFileExp::Ext(int n)
{		
	//TODO: Return the 'i-th' file name extension (i.e. "3DS").
    if(n==0)
	    return _T("X");
    else if(n==1)
        return _T("Y");

    return _T("X");
}

const TCHAR *XFileExp::LongDesc()
{
	//TODO: Return long ASCII description (i.e. "Targa 2.0 Image File")
	return _T("DirectX Mesh File");
}
	
const TCHAR *XFileExp::ShortDesc() 
{			
	//TODO: Return short ASCII description (i.e. "Targa")
	return _T("X-Mesh");
}

const TCHAR *XFileExp::AuthorName()
{			
	//TODO: Return ASCII Author name
	return _T("");
}

const TCHAR *XFileExp::CopyrightMessage() 
{	
	// Return ASCII Copyright message
	return _T("");
}

const TCHAR *XFileExp::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("X-File");
}

const TCHAR *XFileExp::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("Y-File");
}

unsigned int XFileExp::Version()
{				
	//TODO: Return Version number * 100 (i.e. v3.01 = 301)
	return 100;
}

void XFileExp::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL XFileExp::SupportsOptions(int ext, DWORD options)
{
	// TODO Decide which options to support.  Simply return
	// true for each option supported by each Extension 
	// the exporter supports.

    if(ext == 0) // x
    {
    }
    else if(ext == 1) // y
    {
    }

	return TRUE;
}


int	XFileExp::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	//TODO: Implement the actual file Export here and 
	//		return TRUE If the file is exported properly      

    HRESULT hr;   

    XExpManager *pManager = new XExpManager();

    if(pManager == NULL)
    {
        MessageBox(NULL, "Export Fail!", "Error!", MB_OK);
        return 0;
    }   
        
     hr = pManager->XExport(name, ei, i, suppressPrompts, options);
     if(FAILED(hr))
     {
         MessageBox(NULL, "먼지 모르것지만 실패!", "Fail!", MB_OK);
     }
     
     
     
    
    if(pManager)
        delete pManager;

	return 1;
}


