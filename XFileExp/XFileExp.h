/**********************************************************************
 *<
	FILE: XFileExp.h

	DESCRIPTION:	Includes for Plugins

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/

#ifndef __XFileExp__H
#define __XFileExp__H

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"


#include <direct.h>
//#include <d3dx9.h>
// Direct3D includes
#include <d3d9.h>
#include <d3dx9.h>
//#include <dxerr9.h>
#include "dxfile.h"

#include "decomp.h"
#include "stdmat.h"
#include "shape.h"

#include <string>
#include <vector>
#include <algorithm>

#include <CS/Phyexp.h>


#include<vector>

//SIMPLE TYPE
#define RELEASE(x) if(x) {x->Release(); x = NULL;}

#define XFileExp_CLASS_ID	Class_ID(0x932321f0, 0x9177f305)

#define SKINMOD_CLASS_ID    Class_ID(0x0095C723, 0x00015666)



extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;



class XFileExp : public SceneExport {
public:

    static HWND hParams;

    int				ExtCount();					// Number of extensions supported
    const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
    const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
    const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
    const TCHAR *	AuthorName();				// ASCII Author name
    const TCHAR *	CopyrightMessage();			// ASCII Copyright message
    const TCHAR *	OtherMessage1();			// Other message #1
    const TCHAR *	OtherMessage2();			// Other message #2
    unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
    void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box

    BOOL	SupportsOptions(int ext, DWORD options);
    int		DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);

    //Constructor/Destructor
    XFileExp();
    ~XFileExp();		
};



class XFileExpClassDesc : public ClassDesc2 {
public:
    int 			IsPublic() { return TRUE; }
    void *			Create(BOOL loading = FALSE) { return new XFileExp(); }
    const TCHAR *	ClassName() { return GetString(IDS_CLASS_NAME); }
    SClass_ID		SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
    Class_ID		ClassID() { return XFileExp_CLASS_ID; }
    const TCHAR* 	Category() { return GetString(IDS_CATEGORY); }

    const TCHAR*	InternalName() { return _T("XFileExp"); }	// returns fixed parsable name (scripter-visible name)
    HINSTANCE		HInstance() { return hInstance; }					// returns owning module handle


};



#endif
