#include "CXExpManager.h"

//#include "XExpFunc.h"
//#include "XExpUtil.h"

//#include "rmxfguid.h"
#include "rmxftmpl.h"

#include "debug.h"
#include "DevilNet.h"


SDialogOptions g_DlgOptions;

BOOL g_bXExt = TRUE;
BOOL g_bSwapTriOrder = TRUE; // true: LH(DX), false: RH(OpenGL)

TextureConverter    g_TextureConverter;
CStringTable        g_stStrings;

//CToolTipCtrl m_tooltip;

extern SAnimFrameAge       g_AnimFrameAge;

#define SavePossibleNum 12
char Items[SavePossibleNum][15] = 
{
    ".tga", 
    ".dds",
    ".jpg", 
    ".pcx", 
    ".png", 
    ".pnm", 
    ".raw", 
    ".sgi", 
    ".bmp", 
    ".tif", 
    ".pal", 
    ".hdr"
};

#define ddsFormatNum 6
char ddsFormat[ddsFormatNum][15] =
{
    "None", "DXT1", "DXT2", "DXT3", "DXT4", "DXT5"
};

char Extstr[128] = ".tga";
char FormatStr[128] = "Node";

const GUID* aIds[] = {&DXFILEOBJ_XSkinMeshHeader, &DXFILEOBJ_VertexDuplicationIndices, &DXFILEOBJ_SkinWeights};


BOOL CALLBACK XFileExpOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) 
{
    static XFileExp *imp = NULL;
    TCHAR buff[MAX_PATH];

    switch(message) 
    {
    case WM_INITDIALOG:
        imp = (XFileExp *)lParam;
        CenterWindow(hWnd,GetParent(hWnd));
    
		#pragma region Texture File Type ComboBox
        for ( int i = 0; i < SavePossibleNum; ++i )
            SendDlgItemMessage( hWnd, IDC_COMBO_FILETYPE, CB_ADDSTRING, 0, (LPARAM)Items[i] );
        SendDlgItemMessage( hWnd, IDC_COMBO_FILETYPE, CB_SETCURSEL, 0, 0 );
		#pragma endregion File Type 변경

		#pragma region Texture File Type DDS File Option ComboBox
        for ( int i = 0; i < ddsFormatNum; ++i )
            SendDlgItemMessage( hWnd, IDC_COMBO_DDSTYPE, CB_ADDSTRING, 0, (LPARAM)ddsFormat[i] );
        SendDlgItemMessage( hWnd, IDC_COMBO_DDSTYPE, CB_SETCURSEL, 0, 0 );
		
		EnableWindow( GetDlgItem(hWnd, IDC_COMBO_DDSTYPE), false );
		#pragma endregion dds 파일 옵션

		#pragma region CheckBox Option
        CheckDlgButton( hWnd, IDC_CHECK_INCLUDE_ANIDATA, g_DlgOptions.m_bSaveAnimationData ? BST_CHECKED : BST_UNCHECKED );    
        //CheckDlgButton( hWnd, IDC_CHECK_YEXTENSION, g_DlgOptions.m_bObjectByYExtension ? BST_CHECKED : BST_UNCHECKED );       
        CheckDlgButton( hWnd, IDC_CHECK_OPTIMIZE, g_DlgOptions.m_bOptimize ? BST_CHECKED : BST_UNCHECKED );          
        CheckDlgButton( hWnd, IDC_CHECK_SMOOTHNORMAL, g_DlgOptions.m_bSmoothNormal ? BST_CHECKED : BST_UNCHECKED );          
        CheckDlgButton( hWnd, IDC_CHECK_SKINWEIGHT, g_DlgOptions.m_bAddSkinWeight ? BST_CHECKED : BST_UNCHECKED );  
        CheckDlgButton( hWnd, IDC_CHECK_EMPTYFRAME, !g_DlgOptions.m_bEmptyFrameAttach ? BST_CHECKED : BST_UNCHECKED );
        //CheckDlgButton(hWnd, IDC_CHECK_CONVERTKOGMESH, !g_DlgOptions.m_bConvertKOGMesh ? BST_CHECKED : BST_UNCHECKED );
		#pragma endregion 체크 박스 옵션
        
		#pragma region 불필요한 버텍스가 있는지 확인
        char infoStr[100];
        //sprintf(infoStr, "( File : %s )", g_DlgOptions.m_pFileName);
        //SetDlgItemText(hWnd, IDC_STATIC_FILENAME, infoStr);

        if ( g_DlgOptions.m_nTotalVertex2 - g_DlgOptions.m_nTotalVertex1 == 0 )
            sprintf( infoStr, "Non Garbage Vertexs ^^v", g_DlgOptions.m_nTotalVertex2 - g_DlgOptions.m_nTotalVertex1 );
        else
            sprintf( infoStr, "Garbage Vertexs : %d", g_DlgOptions.m_nTotalVertex2 - g_DlgOptions.m_nTotalVertex1 );
        SetDlgItemText(hWnd, IDC_STATIC_INFO, infoStr);
		#pragma endregion 버텍스 관련

		#pragma region X-File Format RadioButton
		switch ( g_DlgOptions.m_xFormat )
        {
        case DXFILEFORMAT_BINARY:
            CheckRadioButton(hWnd,IDC_RADIO_TEXT_FORMAT,IDC_RADIO_BIN_FORMAT,IDC_RADIO_BIN_FORMAT);
            break;

        case DXFILEFORMAT_TEXT:
            CheckRadioButton(hWnd,IDC_RADIO_TEXT_FORMAT,IDC_RADIO_BIN_FORMAT,IDC_RADIO_TEXT_FORMAT);
            break;

        case DXFILEFORMAT_BINARY | DXFILEFORMAT_COMPRESSED:
            CheckRadioButton(hWnd,IDC_RADIO_TEXT_FORMAT,IDC_RADIO_BIN_FORMAT,IDC_RADIO_BINCOMPRESSED_FORMAT);
            break;
        }
		#pragma endregion 파일 포맷

		#pragma region Y-File Select
		if ( g_DlgOptions.m_bObjectByYExtension )
			CheckRadioButton( hWnd, IDC_RADIO_X_EXT, IDC_RADIO_Y_EXT, IDC_RADIO_Y_EXT );
		else
			CheckRadioButton( hWnd, IDC_RADIO_X_EXT, IDC_RADIO_Y_EXT, IDC_RADIO_X_EXT );
		#pragma endregion Y 파일로 저장

		#pragma region Animation Setup
		{
			Interval	interval;
			DWORD		dwTicksPerFrame;
			DWORD		dwStartTime;		
			DWORD		dwEndTime;
			TCHAR drive[MAX_PATH];
			TCHAR dir[MAX_PATH];
			TCHAR fname[MAX_PATH];
			TCHAR ext[MAX_PATH];

			// 애니메이션 프레임 구간설정        
			interval		= g_DlgOptions.m_pInterface->GetAnimRange();
			dwTicksPerFrame	= GetTicksPerFrame();
			dwStartTime		= interval.Start();
			dwEndTime		= interval.End();

			_stprintf(buff,_T("%d"),g_DlgOptions.m_iAnimSamplingRate);
			SetDlgItemText(hWnd,IDC_EDIT_SAMPLING,buff);

			g_AnimFrameAge.m_dwStartFrame = dwStartTime/dwTicksPerFrame;
			_stprintf(buff,_T("%d"), g_AnimFrameAge.m_dwStartFrame );
			SetDlgItemText( hWnd, IDC_EDIT_FRAME_RANGE_START, buff );

			g_AnimFrameAge.m_dwEndFrame = dwEndTime/dwTicksPerFrame;
			_stprintf(buff,_T("%d"), g_AnimFrameAge.m_dwEndFrame );
			SetDlgItemText( hWnd, IDC_EDIT_FRAME_RANGE_END, buff );

			// EDITED : 애니메이션 이름 설정.  
			_tsplitpath( g_DlgOptions.m_strAnimName, drive, dir, fname, ext);

			_stprintf(buff,_T("%s"), fname );//"Anim01");
			SetDlgItemText(hWnd,IDC_EDIT_ANINAME,buff);
        }
        
        if(g_DlgOptions.m_bSaveAnimationData)
        {
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), true);
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), true);
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), true);
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), true);
        }
        else
        {
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), false);
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), false);
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), false);
            EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), false);
		}
		#pragma endregion 애니메이션 정보

		return TRUE;

    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDC_BUTTON_EXPORT:
            g_DlgOptions.m_bProceedWithExport = TRUE;
            EndDialog(hWnd,0);
            break;
        case IDC_BUTTON_CANCEL:
            g_DlgOptions.m_bProceedWithExport = FALSE;
            EndDialog(hWnd,0);
            break;

        case IDC_RADIO_TEXT_FORMAT:
            g_DlgOptions.m_xFormat = DXFILEFORMAT_TEXT;
            break;
        case IDC_RADIO_BIN_FORMAT:
            g_DlgOptions.m_xFormat = DXFILEFORMAT_BINARY;
            break;
        case IDC_RADIO_BINCOMPRESSED_FORMAT:
            g_DlgOptions.m_xFormat = DXFILEFORMAT_BINARY | DXFILEFORMAT_COMPRESSED;
            break;
        case IDC_CHECK_SKINWEIGHT:
            g_DlgOptions.m_bAddSkinWeight = !g_DlgOptions.m_bAddSkinWeight;
            break;

        case IDC_CHECK_INCLUDE_ANIDATA:
            g_DlgOptions.m_bSaveAnimationData = !g_DlgOptions.m_bSaveAnimationData;

            if(g_DlgOptions.m_bSaveAnimationData)
            {
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), true);
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), true);
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), true);
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), true);
            }
            else
            {
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), false);
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), false);
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), false);
                EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), false);
            }
            break;

        case IDC_CHECK_FULLPATH:
            g_DlgOptions.m_bTextureFullPath = !g_DlgOptions.m_bTextureFullPath;
            break;

		/*case IDC_CHECK_YEXTENSION:
            g_DlgOptions.m_bObjectByYExtension = !g_DlgOptions.m_bObjectByYExtension;
            if(g_DlgOptions.m_bObjectByYExtension == TRUE)
            {
                CheckDlgButton(hWnd, IDC_CHECK_SKINWEIGHT, BST_UNCHECKED );  
                g_DlgOptions.m_bAddSkinWeight = false;
                EnableWindow(GetDlgItem(hWnd, IDC_CHECK_SKINWEIGHT), false);
            }
            else
            {
                CheckDlgButton(hWnd, IDC_CHECK_SKINWEIGHT, BST_CHECKED );  
                g_DlgOptions.m_bAddSkinWeight = true;
                EnableWindow(GetDlgItem(hWnd, IDC_CHECK_SKINWEIGHT), true);
            }
            break;
		*/
		case IDC_RADIO_X_EXT:
			{
				g_DlgOptions.m_bObjectByYExtension = false;
				g_DlgOptions.m_bAddSkinWeight = true;
				g_DlgOptions.m_bSaveAnimationData = true;
				CheckDlgButton(hWnd, IDC_CHECK_INCLUDE_ANIDATA, BST_CHECKED );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), true);
			} break;

		case IDC_RADIO_Y_EXT:
			{
				g_DlgOptions.m_bObjectByYExtension = true;
				g_DlgOptions.m_bAddSkinWeight = false;
				g_DlgOptions.m_bSaveAnimationData = false;
				CheckDlgButton(hWnd, IDC_CHECK_INCLUDE_ANIDATA, BST_UNCHECKED );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), false);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), false);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), false);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), false);
			} break;

        case IDC_CHECK_OPTIMIZE:
            g_DlgOptions.m_bOptimize = !g_DlgOptions.m_bOptimize;
            break;
        case IDC_CHECK_SMOOTHNORMAL:
            g_DlgOptions.m_bSmoothNormal = !g_DlgOptions.m_bSmoothNormal;
            break;
        case IDC_CHECK_EMPTYFRAME:
            g_DlgOptions.m_bEmptyFrameAttach = !g_DlgOptions.m_bEmptyFrameAttach;
            break;
        case IDC_CHECK_CONVERTKOGMESH:
            g_DlgOptions.m_bConvertKOGMesh = !g_DlgOptions.m_bConvertKOGMesh;
            break;

        case IDC_EDIT_SAMPLING:
            GetDlgItemText(hWnd,IDC_EDIT_SAMPLING,buff,sizeof(buff) / sizeof(TCHAR));
            g_DlgOptions.m_iAnimSamplingRate = _ttoi(buff);
            break;
        
        case IDC_EDIT_FRAME_RANGE_START:
            GetDlgItemText(hWnd,IDC_EDIT_FRAME_RANGE_START,buff,sizeof(buff) / sizeof(TCHAR));
            g_AnimFrameAge.m_dwStartFrame = _ttoi(buff);
            break;

        case IDC_EDIT_FRAME_RANGE_END:
            GetDlgItemText(hWnd,IDC_EDIT_FRAME_RANGE_END,buff,sizeof(buff) / sizeof(TCHAR));
            g_AnimFrameAge.m_dwEndFrame = _ttoi(buff);
            break;

        case IDC_EDIT_ANINAME:
            GetDlgItemText(hWnd,IDC_EDIT_ANINAME,buff,sizeof(buff) / sizeof(TCHAR));
            _tcscpy( g_DlgOptions.m_strAnimName, buff );
            break;

        case IDC_COMBO_FILETYPE:
            {
                switch ( HIWORD(wParam) ) 
                {

                case CBN_SELCHANGE:
                    int i;
                    i=SendDlgItemMessage(hWnd, IDC_COMBO_FILETYPE,CB_GETCURSEL,0,0);
                    SendDlgItemMessage(hWnd, IDC_COMBO_FILETYPE,CB_GETLBTEXT,i,(LPARAM)Extstr);                    

                    if(i == 1) // dds
                    {
                        EnableWindow(GetDlgItem(hWnd, IDC_COMBO_DDSTYPE), true);                        
                    }
                    else
                    {
                        EnableWindow(GetDlgItem(hWnd, IDC_COMBO_DDSTYPE), false);
                    }
                    
                    
                    break;
                }

                break;
            }
        case IDC_COMBO_DDSTYPE:
            {
                switch ( HIWORD(wParam) ) 
                {

                case CBN_SELCHANGE:
                    int i;
                    i=SendDlgItemMessage(hWnd, IDC_COMBO_DDSTYPE,CB_GETCURSEL,0,0);
                    SendDlgItemMessage(hWnd, IDC_COMBO_DDSTYPE,CB_GETLBTEXT,i,(LPARAM)FormatStr);                    
                    break;
                }

                break;
            }

		case IDC_BUTTON_NORMAL:
			{
				g_AnimFrameAge.m_dwEndFrame = 2;
				_stprintf(buff,_T("%d"), g_AnimFrameAge.m_dwEndFrame );
				SetDlgItemText( hWnd, IDC_EDIT_FRAME_RANGE_END, buff );

				_stprintf( buff,_T("%s"), "Normal" );
				SetDlgItemText( hWnd, IDC_EDIT_ANINAME, buff );

				g_DlgOptions.m_bSaveAnimationData = true;
				CheckDlgButton(hWnd, IDC_CHECK_INCLUDE_ANIDATA, BST_CHECKED );
				CheckDlgButton(hWnd, IDC_CHECK_OPTIMIZE, BST_UNCHECKED );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), true);
			} break;

		case IDC_BUTTON_DEFAULT:
			{
				Interval interval;
				DWORD dwTicksPerFrame;
				DWORD dwEndTime;
				TCHAR drive[MAX_PATH];
				TCHAR dir[MAX_PATH];
				TCHAR fname[MAX_PATH];
				TCHAR ext[MAX_PATH];

				interval		= g_DlgOptions.m_pInterface->GetAnimRange();
				dwTicksPerFrame	= GetTicksPerFrame();
				dwEndTime		= interval.End();

				g_AnimFrameAge.m_dwEndFrame = dwEndTime/dwTicksPerFrame;
				_stprintf(buff,_T("%d"), g_AnimFrameAge.m_dwEndFrame );
				SetDlgItemText( hWnd, IDC_EDIT_FRAME_RANGE_END, buff );

				_tsplitpath( g_DlgOptions.m_strOrgAnimName, drive, dir, fname, ext);
				_stprintf( buff,_T("%s"), fname );
				SetDlgItemText( hWnd, IDC_EDIT_ANINAME, buff );

				g_DlgOptions.m_bSaveAnimationData = true;
				CheckDlgButton(hWnd, IDC_CHECK_INCLUDE_ANIDATA, BST_CHECKED );
				CheckDlgButton(hWnd, IDC_CHECK_OPTIMIZE, BST_UNCHECKED );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), true);
			} break;

		case IDC_BUTTON_EQUIP:
			{
				g_DlgOptions.m_bObjectByYExtension = false;
				g_DlgOptions.m_bAddSkinWeight = true;
				g_DlgOptions.m_bSaveAnimationData = false;
				g_DlgOptions.m_bOptimize = false;
				g_DlgOptions.m_bSmoothNormal = true;
				CheckDlgButton(hWnd, IDC_CHECK_INCLUDE_ANIDATA, BST_UNCHECKED );
				CheckDlgButton(hWnd, IDC_CHECK_OPTIMIZE, BST_UNCHECKED );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), false );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), false );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), false );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), false );
			} break;

		case IDC_BUTTON_WEAPON:
			{
				g_DlgOptions.m_bObjectByYExtension = false;
				g_DlgOptions.m_bAddSkinWeight = true;
				g_DlgOptions.m_bSaveAnimationData = true;
				g_DlgOptions.m_bOptimize = false;
				g_DlgOptions.m_bSmoothNormal = true;
				CheckDlgButton(hWnd, IDC_CHECK_INCLUDE_ANIDATA, BST_CHECKED );
				CheckDlgButton(hWnd, IDC_CHECK_OPTIMIZE, BST_UNCHECKED );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), true);
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), true);

				g_AnimFrameAge.m_dwEndFrame = 2;
				_stprintf(buff,_T("%d"), g_AnimFrameAge.m_dwEndFrame );
				SetDlgItemText( hWnd, IDC_EDIT_FRAME_RANGE_END, buff );

				_stprintf( buff,_T("%s"), "Normal" );
				SetDlgItemText( hWnd, IDC_EDIT_ANINAME, buff );
			} break;

		case IDC_BUTTON_ACCESSORY:
			{
				g_DlgOptions.m_bObjectByYExtension = true;
				g_DlgOptions.m_bAddSkinWeight = true;
				g_DlgOptions.m_bSaveAnimationData = false;
				g_DlgOptions.m_bOptimize = true;
				g_DlgOptions.m_bSmoothNormal = true;

				CheckRadioButton( hWnd, IDC_RADIO_X_EXT, IDC_RADIO_Y_EXT, IDC_RADIO_Y_EXT );
				CheckDlgButton(hWnd, IDC_CHECK_INCLUDE_ANIDATA, BST_UNCHECKED );
				CheckDlgButton(hWnd, IDC_CHECK_OPTIMIZE, BST_CHECKED );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLING), false );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_START), false );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_FRAME_RANGE_END), false );
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_ANINAME), false );
			} break;

		default:
            break;
        }
    }
    return FALSE;
}

XExpManager::XExpManager()
{    
    m_pFile = NULL;
    
    m_frameRoot = NULL;    

    m_pxofapi = NULL;
    m_pRootData = NULL;
    m_ps.m_pxofsave = NULL;
    m_ps.m_pSkinData = NULL;
}

XExpManager::~XExpManager()
{    
    if(m_pFile)
        fclose(m_pFile);

    if(m_frameRoot)
        delete m_frameRoot;

    if(m_ps.m_pSkinData)
        delete m_ps.m_pSkinData;

    RELEASE(m_ps.m_pxofsave);
    RELEASE(m_pRootData);
    RELEASE(m_pxofapi);
}

HRESULT XExpManager::XExport(const TCHAR *aName, ExpInterface *aExpInterface,Interface *aInterface, BOOL suppressPrompts, DWORD options)
{  
    HRESULT hr = S_OK;
    BOOL bPhysique, bOptimize;
    CXExpFrame *frameRoot;
    BOOL bSaveSelection = (options & SCENE_EXPORT_SELECTED);

    TCHAR szFileNameE[MAX_PATH] = _T("");
    _tcscpy( szFileNameE, aName );
    int iNumExe = (_tcslen( szFileNameE ) - 1);
    TCHAR cExe = *( szFileNameE + iNumExe );
    if( cExe == _T('x') || cExe == _T('X') )
    {
        g_DlgOptions.m_bFileTypeX = TRUE;
    }
    else
    {
        g_DlgOptions.m_bFileTypeX = FALSE;
    }

    // setup dialog options
    g_DlgOptions.m_nTotalNode = 0;
    g_DlgOptions.m_iCurNode = 0;
    g_DlgOptions.m_nTotalVertex1 = 0;
    g_DlgOptions.m_nTotalFace1 = 0;
    g_DlgOptions.m_nTotalVertex2 = 0;
    g_DlgOptions.m_nTotalFace2 = 0;
    g_DlgOptions.m_bOptimize   = TRUE;
    g_DlgOptions.m_bSmoothNormal = TRUE;    
    g_DlgOptions.m_xFormat					= DXFILEFORMAT_BINARY | DXFILEFORMAT_COMPRESSED; 
    if(g_DlgOptions.m_bFileTypeX)
        g_DlgOptions.m_bObjectByYExtension      = FALSE;
    else
        g_DlgOptions.m_bObjectByYExtension      = TRUE;
    g_DlgOptions.m_bTextureFullPath			= FALSE;
	g_DlgOptions.m_pFileName	= aName;
    g_DlgOptions.m_bSelection = bSaveSelection;
    g_DlgOptions.m_bEmptyFrameAttach        = TRUE; //!bSaveSelection;
    g_DlgOptions.m_bConvertKOGMesh          = FALSE;
	g_DlgOptions.m_bMultiFile = FALSE;
    
    
	m_pFileName = aName;
	m_pInterface = aInterface;

    m_pInterface->ProgressStart(_T("Exporting Data"),TRUE,ProgressFn,NULL);
    m_pInterface->ProgressUpdate(0);

    m_ps.m_pSkinData = new CXExpSkinData(aInterface);
    bPhysique = m_ps.m_pSkinData->Preprocess(m_pInterface->GetRootNode());

    frameRoot = new CXExpFrame(m_pInterface, m_pFile);
    bOptimize = frameRoot->CheckOptimize(m_pInterface->GetRootNode());
    if(!bPhysique)
        g_DlgOptions.m_bOptimize   = TRUE;


    // setup dialog options
    g_DlgOptions.m_pInterface = aInterface;
    g_DlgOptions.m_bProceedWithExport       = FALSE;        
    g_DlgOptions.m_bChildrenNotAttached     = FALSE;
    if(g_DlgOptions.m_bFileTypeX)
        g_DlgOptions.m_bSaveAnimationData		= TRUE;   
    else
        g_DlgOptions.m_bSaveAnimationData		= FALSE;   
    g_DlgOptions.m_bAddSkinWeight           = TRUE;    
    g_DlgOptions.m_iAnimSamplingRate		= 30;
    g_DlgOptions.m_cMaxBonesPerVertex		= m_ps.m_pSkinData->m_ppc.m_cMaxWeightsPerVertex;
    g_DlgOptions.m_cMaxBonesPerFace			= m_ps.m_pSkinData->m_ppc.m_cMaxWeightsPerFace;    
    _tcscpy( g_DlgOptions.m_strAnimName, m_pInterface->GetCurFileName() );
	_tcscpy( g_DlgOptions.m_strOrgAnimName, m_pInterface->GetCurFileName() );

    // export 다이얼로그 생성
    if(!suppressPrompts)
    {        
        DialogBoxParam(hInstance, 
            MAKEINTRESOURCE(IDD_PANEL), 
            aInterface->GetMAXHWnd(), //GetActiveWindow(), 
            XFileExpOptionsDlgProc, 
            (LPARAM)this); //(LPARAM)GetActiveWindow());

        if(!g_DlgOptions.m_bProceedWithExport)
        {
            m_pInterface->ProgressUpdate(100);
            m_pInterface->ProgressEnd();

            return S_OK;
        }
    }
 
    // export
	hr = DoExport();

    m_pInterface->ProgressUpdate(100);
    m_pInterface->ProgressEnd();

    return hr;
}

HRESULT XExpManager::DoExport(const TCHAR *aFileName)
{
    HRESULT hr;

	TCHAR szFileNameE[MAX_PATH] = _T("");
	_tcscpy( szFileNameE, m_pFileName );

    // 디버그용 출력 파일
    if(aFileName != NULL)
    {
        m_pFile = fopen(aFileName, "w");
        if(m_pFile == NULL)
        {                      
            MessageBox(NULL, "디버깅 파일 생성 실패!", "Error!", MB_OK);
            return E_FAIL;
        }
    }    

    //m_pInterface->ProgressStart(_T("Exporting data"),TRUE,ProgressFn,NULL);
    m_pInterface->ProgressUpdate(10);
 
    hr = DirectXFileCreate(&m_pxofapi);
    if(FAILED(hr))
        return hr;

    // Register templates for d3drm.
    hr = m_pxofapi->RegisterTemplates((LPVOID)D3DRM_XTEMPLATES, D3DRM_XTEMPLATE_BYTES);
    if(FAILED(hr))
        return hr;

    hr = m_pxofapi->RegisterTemplates((LPVOID)XSKINEXP_TEMPLATES, strlen(XSKINEXP_TEMPLATES));
    if(FAILED(hr))
        return hr;

    // static mesh 일경우 Y파일로 생성준비 처리
    if( g_DlgOptions.m_bSaveAnimationData == FALSE && g_DlgOptions.m_bObjectByYExtension == TRUE )
    {
        int iNumExe = (_tcslen( szFileNameE ) - 1);
        TCHAR cExe = *( szFileNameE + iNumExe );
        if( cExe == _T('x') || cExe == _T('X') )
        {
            *( szFileNameE + iNumExe ) = _T('Y');
        }

        g_bXExt = FALSE;
    }
    else
    {
        int iNumExe = (_tcslen( szFileNameE ) - 1);
        TCHAR cExe = *( szFileNameE + iNumExe );
        if( cExe == _T('y') || cExe == _T('Y') )
        {
            *( szFileNameE + iNumExe ) = _T('X');
        }

        g_bXExt = TRUE;
    }


    // Create save object.
    hr = m_pxofapi->CreateSaveObject(szFileNameE, g_DlgOptions.m_xFormat, &(m_ps.m_pxofsave));
    if (FAILED(hr))
        return hr;

    hr = m_ps.m_pxofsave->SaveTemplates(3, aIds);
    if (FAILED(hr))
        return hr;


    m_frameRoot = new CXExpFrame(m_pInterface, m_pFile);
    m_pRootData = m_frameRoot->NodeEnum(m_pInterface->GetRootNode(), m_ps);
    if(m_pRootData == NULL)
        return E_FAIL;

    m_pInterface->ProgressUpdate(50);


    // now save that file data to the file
    hr = m_ps.m_pxofsave->SaveData(m_pRootData);
    if(FAILED(hr))
    {
        RELEASE(m_ps.m_pxofsave);
        RELEASE(m_pRootData);
        RELEASE(m_pxofapi);
        return hr;
    }

    m_pInterface->ProgressUpdate(60);

    // export animation set    
    if (g_DlgOptions.m_bSaveAnimationData)
    {
        m_ps.m_pSkinData->SetAniInfo(m_ps.m_pxofsave);
        
        hr = m_ps.m_pSkinData->ExportAnimationSet(g_DlgOptions.m_strAnimName );
        if (FAILED(hr))
        {
            RELEASE(m_ps.m_pxofsave);
            RELEASE(m_pRootData);
            RELEASE(m_pxofapi);
            return hr;
        }

        hr = m_ps.m_pxofsave->SaveData(m_ps.m_pSkinData->GetAniSet());
        if (FAILED(hr))
        {            
            RELEASE(m_ps.m_pxofsave);
            RELEASE(m_pRootData);
            RELEASE(m_pxofapi);
            return hr;
        }            
    }

	RELEASE(m_ps.m_pxofsave);
	RELEASE(m_pRootData);
	RELEASE(m_pxofapi);

	if(g_DlgOptions.m_bConvertKOGMesh)
	{
		// 게임내 Mesh구조에 맞게 미리 변환한다. (Convert)
		XMeshConvert(szFileNameE);
	}


	#pragma region Texture 출력
	char OutPutFilePath[MAX_PATH] = {0};
    char drive[MAX_PATH];
    char dir[MAX_PATH];
    char fname[MAX_PATH];
    char ext[MAX_PATH];

    _splitpath( szFileNameE, drive, dir, fname, ext);

    strcat( OutPutFilePath, drive );
    strcat( OutPutFilePath, dir );

    g_TextureConverter.ConvertToTGA( OutPutFilePath, Extstr, FormatStr );

    strcpy( Extstr, ".tga" ); // 우선 tga변경만.

    g_TextureConverter.Clear();
	#pragma endregion 텍스쳐를 변환후 저장될 x-file의 디렉토리에 같이 저장한다.
    
    return S_OK;
}

HRESULT XExpManager::XMeshConvert(const TCHAR *aName)
{
	TCHAR buf[1024];

	if(g_bXExt)
	{        
		wsprintf(buf, "D:\\ProjectX2\\Bin\\XMeshConvert\\KOGSkinMesh.exe /f /s %s",aName, aName);        
	}
	else
	{
		wsprintf(buf, "D:\\ProjectX2\\Bin\\XMeshConvert\\KOGSkinMesh.exe /f /m %s",aName, aName);
	}

	system(buf);

	return S_OK;
}