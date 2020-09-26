/** @File: CustomUI.cpp
    Starting point for new Direct3D applications

    Copyright (c) Microsoft Corporation. All rights reserved.
    @date   jintaeks on 2008-10-09, 11:04
            - Ods306팀의 서진택이 메시 변환기의 수정 작업을 진행하다.
            - 이미 변환된 메시가 .x_old 혹은 .y_old로 변환되지 않도록 수정하다.
            - 작업 정보를 JLog.html로 남기도록 수정하다.
*/


#include <StdAfx.h>
#include "resource.h"
#include "KJHtmlLog.h" // 정보를 log로 남기기 위해 추가 - jintaeks on 2008-10-09, 10:40


//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 

#define NO_WIN32_LEAN_AND_MEAN

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*              g_pFont = NULL;         // Font for drawing text
ID3DXSprite*            g_pTextSprite = NULL;   // Sprite for batching draw text calls
ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
CDXUTMesh               g_Mesh;                 // Background mesh
D3DXMATRIXA16           g_mView;
CModelViewerCamera      g_Camera;               // A model viewing camera
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg         g_SettingsDlg;          // Device settings dialog
CDXUTDialog             g_HUD;                  // dialog for standard controls
CDXUTDialog             g_SampleUI;             // dialog for sample specific controls

struct CONVERTINFO
{
    std::wstring m_strPath;
    std::wstring m_strFileName;
    BOOL  m_bSkinMesh;
};

std::vector<CONVERTINFO> g_vecFileList;
BOOL g_bProcess = TRUE;
BOOL g_bRestore = FALSE;
BOOL g_bFileConvert = FALSE;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_EDITBOX1            5
#define IDC_EDITBOX2            6
#define IDC_ENABLEIME           7
#define IDC_DISABLEIME          8
#define IDC_COMBOBOX            9
#define IDC_STATIC              10
#define IDC_OUTPUT              12
#define IDC_SLIDER              13
#define IDC_CHECKBOX            14
#define IDC_CLEAREDIT           15
#define IDC_RADIO1A             16
#define IDC_RADIO1B             17
#define IDC_RADIO1C             18
#define IDC_RADIO2A             19
#define IDC_RADIO2B             20
#define IDC_RADIO2C             21
#define IDC_LISTBOX             22
#define IDC_LISTBOXM            23


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice( void* pUserContext );
void    CALLBACK OnDestroyDevice( void* pUserContext );

void    InitApp();
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );
void    RenderText();

BOOL CALLBACK ProgressDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
void AddFolder(wchar_t *strPath);
int  InsertRow(HWND, int, CONVERTINFO, std::wstring);

enum    EKSMOptions
{
    KSMOPTIONS_NONE,
    KSMOPTIONS_SKINMESH,
    KSMOPTIONS_MESH,
};//enum    EKSMOptions

static EKSMOptions s_eksmOptions = KSMOPTIONS_NONE;
static LPSTR   s_psz1stArgument = NULL;
static LPSTR   s_psz2ndArgument = NULL;

static void _AnalyzeArgument( LPCSTR pszInArg_ )
{
    s_eksmOptions = KSMOPTIONS_NONE;
    s_psz1stArgument = NULL;
    s_psz2ndArgument = NULL;

    if ( pszInArg_ == NULL )
        return;

    int iNumArgs = 0;

    for( ; *pszInArg_ != 0; pszInArg_++ )
    {
        if ( *pszInArg_ == ' ' || *pszInArg_ == '\t' )
            continue;
        if ( *pszInArg_ == '/' )
        {
            pszInArg_++;
            switch( *pszInArg_ )
            {
            case 0:
                return;
            case 's':
            case 'S':
                s_eksmOptions = KSMOPTIONS_SKINMESH;
                break;
            case 'm':
            case 'M':
                s_eksmOptions = KSMOPTIONS_MESH;
                break;
            case 'r':
            case 'R':
                g_bRestore = TRUE;
                break;
            case 'f':
            case 'F':
                g_bFileConvert = TRUE;
            }//switch
        }
        else
        {
            LPCSTR pszFirst = pszInArg_;
            DWORD       dwSize = 1;
            for( pszInArg_++; *pszInArg_ != 0 /* && *pszInArg_ != ' ' */ && *pszInArg_ != '\t'; pszInArg_++, dwSize++ );
            if ( iNumArgs == 0 )
            {
                s_psz1stArgument = new char[dwSize+1];
                CopyMemory( s_psz1stArgument, pszFirst, dwSize );
                s_psz1stArgument[dwSize] = 0;
                iNumArgs = 1;
            }
            else if ( iNumArgs == 1 )
            {
                s_psz2ndArgument = new char[dwSize+1];
                CopyMemory( s_psz2ndArgument, pszFirst, dwSize );
                s_psz2ndArgument[dwSize] = 0;
                iNumArgs = 2;
            }//if.. else..
            if ( *pszInArg_ == 0 )
                return;
        }//if.. else..
    }//while
}//_AnalyzeArgument()


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR pszArguments, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions. These functions allow DXUT to notify
    // the application about device changes, user input, and windows messages.  The 
    // callbacks are optional so you need only set callbacks for events you're interested 
    // in. However, if you don't handle the device reset/lost callbacks then the sample 
    // framework won't be able to reset your device since the application must first 
    // release all device resources before resetting.  Likewise, if you don't handle the 
    // device created/destroyed callbacks then DXUT won't be able to 
    // recreate your device resources.
    //DXUTSetCallbackDeviceCreated( OnCreateDevice );
    //DXUTSetCallbackDeviceReset( OnResetDevice );
    //DXUTSetCallbackDeviceLost( OnLostDevice );
    //DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    //DXUTSetCallbackMsgProc( MsgProc );
    //DXUTSetCallbackKeyboard( KeyboardProc );
    //DXUTSetCallbackFrameRender( OnFrameRender );
    //DXUTSetCallbackFrameMove( OnFrameMove );

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );

    InitApp();

    // Initialize DXUT and create the desired Win32 window and Direct3D 
    // device for the application. Calling each of these functions is optional, but they
    // allow you to set several options which control the behavior of the framework.
    DXUTInit( true, true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
    DXUTCreateWindow( L"KOGSkinMesh", hInstance );
    DXUTCreateDevice( D3DADAPTER_DEFAULT, true, 640, 480, IsDeviceAcceptable, ModifyDeviceSettings );

    int                     iErrorCode = 1;
    int                     nErrCount = 0;
    HRESULT                 hr;
    CKTDXDeviceXSkinMesh*   pSkinMesh = NULL;
    CKTDXDeviceXMesh*       pMesh = NULL;
    std::wstring            wstrFilename, wPath;
    std::string             tempFile;
    CONVERTINFO             convertFile;
    wchar_t                 tempBuf[MAX_PATH];
    wchar_t                 countBuf[100];
    wchar_t*                wstrErr;

    _AnalyzeArgument( pszArguments );

    //{{ seojt // 2008-10-8, 17:03
    // 특정한 폴더에 대해서 파일 converson을 테스트하기 위해 삽인한 코드 - jintaeks on 2008-10-08, 17:22
    //
//    s_psz1stArgument = new char[2048];
//    strcpy( s_psz1stArgument, "C:\\p4_116.120.238.511666_SEOJT\\X2Project\\X2\\dat\\Model\\Unit\\npc" );
    //}} seojt // 2008-10-8, 17:03

    if ( s_psz1stArgument == NULL )
    {
        // Open Dlg
        BROWSEINFO BrowsingInfo;
        wchar_t DirPath[MAX_PATH];
        wchar_t FolderName[MAX_PATH];
        LPITEMIDLIST ItemID;
        memset( &BrowsingInfo, 0, sizeof(BROWSEINFO) );
        memset( DirPath, 0, MAX_PATH );
        memset( FolderName, 0, MAX_PATH );
        BrowsingInfo.hwndOwner         = NULL; //Handle;
        BrowsingInfo.pszDisplayName = FolderName;
        BrowsingInfo.lpszTitle         = L"폴더를 선택하세요.";
        BrowsingInfo.ulFlags            = BIF_RETURNONLYFSDIRS;
        ItemID = SHBrowseForFolder( &BrowsingInfo );
        SHGetPathFromIDList(ItemID, DirPath );
        GlobalFreePtr( ItemID );
        //::MessageBox( NULL, (LPWSTR)DirPath, L"선택된 디렉토리", MB_OK );

        if(wcscmp(DirPath, L"") != 0)
            AddFolder(DirPath);
        else 
            return 0;
    }
    else
    {
        if(g_bFileConvert == FALSE)
        {
            ConvertCharToWCHAR(wPath, s_psz1stArgument);

            SAFE_DELETE_ARRAY( s_psz1stArgument );
            SAFE_DELETE_ARRAY( s_psz2ndArgument );
            AddFolder((wchar_t *)wPath.c_str());
        }
        else
        {
            wchar_t strDrive[10], strDir[1024], strFileName[1024], strExt[24]; 
            wchar_t strPath[1024], strFile[1024];
            CONVERTINFO convertMesh;
            
            ConvertCharToWCHAR(wPath, s_psz1stArgument);

            _wsplitpath( wPath.c_str(), strDrive, strDir, strFileName, strExt);

            wsprintf(strPath, L"%s%s", strDrive, strDir);
            wsprintf(strFile, L"%s%s", strFileName, strExt);

            convertMesh.m_strFileName = strFile;
            convertMesh.m_strPath = strPath;

            if(s_eksmOptions == KSMOPTIONS_SKINMESH)
                convertMesh.m_bSkinMesh = true;
            else
                convertMesh.m_bSkinMesh = false;

            g_vecFileList.push_back(convertMesh);   
        }
        
    }//if
    
    s_psz1stArgument = NULL; 
    
    InitCommonControls();

    //HWND hWnd;
    HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, ProgressDlgProc);
    ShowWindow(hDlg, SW_SHOW);
    HWND hStatic = CreateWindow(WC_STATICW, NULL, WS_CHILD | WS_VISIBLE, 
        10, 420, 630, 16, hDlg, NULL, NULL, NULL);
    HWND hList = CreateWindow(WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS, 
        10, 10, 600, 410, hDlg, NULL, hInstance, NULL);
    LVCOLUMN COL;
    COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    COL.fmt = LVCFMT_LEFT;
    COL.cx = 150;
    COL.pszText = L"Name";
    COL.iSubItem = 0;
    SendMessage(hList, LVM_INSERTCOLUMN, 0, (LPARAM)&COL);

    COL.pszText = L"Status";
    COL.iSubItem = 1;
    SendMessage(hList, LVM_INSERTCOLUMN, 1, (LPARAM)&COL);

    COL.cx = 300;
    COL.pszText = L"Path";
    COL.iSubItem = 2;
    SendMessage(hList, LVM_INSERTCOLUMN, 2, (LPARAM)&COL);

    ShowWindow(hStatic, SW_SHOW);
    ShowWindow(hList, SW_SHOW);


    // 수집된 모든 파일에 대해서 작업을 진행한다.
    //
    int     iNumAlreadyConverted    = 0;    /// 이미 변환되어 있는 메시의 개수
    int     iNumConverted           = 0;    /// 변환된 메시의 개수
    int     iNumProcessing          = 0;    /// 처리한 파일(메시)의 개수
    int     iNumLoadOrOpenError     = 0;    /// open이나 load가 실패한 메시의 개수
    int     iNumWritingError        = 0;    /// 변환하기 위해 write에 실패한 메시의 개수
    int     iNumXFileWhichContainedNoAnimationData = 0; /// .x 파일 메시가 animation 데이터를 포함하고 있지 않은 메시의 개수

    for(int i=0; i<g_vecFileList.size(); ++i)
    {          
        KFileSerializer kOutFile;

        // 이미 "*.[xy]_old"형식의 파일이 존재하는지 검사해서 g_bRestore가 true이면
        // 원본 파일을 복구한다. - jintaeks on 2008-10-09, 10:02
        //
        wstrErr = L"완료";
        s_psz1stArgument = new char[MAX_PATH];  // .x_old
        s_psz2ndArgument = new char[MAX_PATH];  // .x

        wmemset(tempBuf, 0, MAX_PATH);
        convertFile = g_vecFileList[i];
        wsprintf(tempBuf, L"%s\\%s", convertFile.m_strPath.c_str(), convertFile.m_strFileName.c_str()); 
        ConvertWCHARToChar(tempFile, tempBuf);
        strcpy(s_psz2ndArgument, tempFile.c_str());

        wsprintf(tempBuf, L"%s\\%s_old", convertFile.m_strPath.c_str(), convertFile.m_strFileName.c_str()); 
        ConvertWCHARToChar(tempFile, tempBuf);
        strcpy(s_psz1stArgument, tempFile.c_str());
        
        wsprintf(countBuf, L"(%d / %d) %s\\%s", i+1, g_vecFileList.size(), convertFile.m_strPath.c_str(), convertFile.m_strFileName.c_str());
        SetWindowText(hStatic, countBuf);

        //g_bRestore = true;
        if(g_bRestore)
        {
            BOOL result = true;

            HANDLE hSrch;
            WIN32_FIND_DATA wfd;

            wmemset(tempBuf, 0, MAX_PATH);
            wsprintf(tempBuf, L"%s\\%s_old", convertFile.m_strPath.c_str(), convertFile.m_strFileName.c_str());
            hSrch = FindFirstFile(tempBuf, &wfd);
            
            if(wcscmp(tempBuf, L"") == 0)
                result = false;
            else
                result = true;

            if(result)
            {           
                wmemset(tempBuf, 0, MAX_PATH);
                wsprintf(tempBuf, L"%s\\%s", convertFile.m_strPath.c_str(), convertFile.m_strFileName.c_str());

                // delete .x
                DeleteFile(tempBuf);

                // rename .x_old .x
                rename(s_psz1stArgument, s_psz2ndArgument);

                SAFE_DELETE_ARRAY( s_psz1stArgument );
                SAFE_DELETE_ARRAY( s_psz2ndArgument );  

                continue;
            }
        }

        
#if 1   
        JLOG( "Processing %s...", s_psz2ndArgument );
        ++iNumProcessing;

        KFileSerializer kFile2;

        // 이미 변환된 메시인지를 검사한다.
        //
        KFileSerializer kFileTemp;
        hr = kFileTemp.LoadFile( s_psz2ndArgument, sizeof(DWORD) );
        if ( SUCCEEDED( hr ) &&  kFileTemp.GetSize() >= sizeof(DWORD) )
        {
            DWORD dwMagic = *( (const DWORD*)kFileTemp.Lock() );
            if ( dwMagic == KOGSKINNEDMESH_FORMAT_MAGIC || dwMagic == KOGMESH_FORMAT_MAGIC )
            {
                kFileTemp.Unlock();
                wstrErr = L"File format is already converted.";
                JLOG_ERROR( wstrErr );

                ++iNumAlreadyConverted;

                goto end;
            }//if
            kFileTemp.Unlock();
        }//if


        // 원본 .x 파일 이름을 .x_old 파일 이름으로 변화해 보존해 둔다.
        //
        if( rename(s_psz2ndArgument, s_psz1stArgument) != 0 )
        {
            wstrErr = L"Cannot rename(Maybe file format is already converted).";
            JLOG_ERROR( wstrErr );

            ++iNumLoadOrOpenError;

            goto end;
        }

        if(convertFile.m_bSkinMesh)
            s_eksmOptions = KSMOPTIONS_SKINMESH;
        else
            s_eksmOptions = KSMOPTIONS_MESH;        

        // .x_old로 변환된 원본 파일을 읽는다.
        hr = kFile2.LoadFile( s_psz1stArgument, sizeof(DWORD) );
        if ( FAILED( hr ) )
        {
            //MessageBox( DXUTGetHWND(), L"File cannot be opened", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
            rename(s_psz1stArgument, s_psz2ndArgument);
            wstrErr = L"File cannot be opened";
            JLOG_ERROR( wstrErr );
            ++nErrCount;

            ++iNumLoadOrOpenError;

            goto end;
        }//if
//        if ( kFile2.GetSize() < sizeof(DWORD) )
//        {
//            //MessageBox( DXUTGetHWND(), L"Invalid file", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
//            rename(s_psz1stArgument, s_psz2ndArgument);
//            wstrErr = L"Invalid file";
//            JLOG_ERROR( wstrErr );
//            ++nErrCount;
//            goto end;
//        }//if
//        DWORD dwMagic = *( (const DWORD*) kFile2.Lock() );
//        if ( dwMagic == KOGSKINNEDMESH_FORMAT_MAGIC || dwMagic == KOGMESH_FORMAT_MAGIC )
//        {
//            //MessageBox( DXUTGetHWND(), L"File format is already converted.", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
//            kFile2.Unlock();
//            rename(s_psz1stArgument, s_psz2ndArgument);
//            wstrErr = L"File format is already converted.";
//            JLOG_ERROR( wstrErr );
//            goto end;
//        }//if
//          kFile2.Unlock();

   
	    ConvertCharToWCHAR( wstrFilename, tempFile.c_str()); //s_psz1stArgument );


        /* //{{ seojt on 2008-10-8
        // original code commented by jintaeks on 2008-10-08, 17:19
        //
        pSkinMesh = new CKTDXDeviceXSkinMesh( DXUTGetD3DDevice(), wstrFilename.c_str() );
        //ASSERT( pSkinMesh != NULL );
        
        if(pSkinMesh == NULL)
        {
            wstrErr = L"pSkinMesh is NULL";
            ++nErrCount;
            goto end;
        }

        hr = pSkinMesh->Load();
        if ( FAILED( hr ) )
        {
            //MessageBox( DXUTGetHWND(), L"File loading failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
            wstrErr = L"File loading failed";
            ++nErrCount;
            goto end;
        }//if

        if ( !pSkinMesh->GetIsOnlyModelData() )
        {
            if ( s_eksmOptions == KSMOPTIONS_MESH )
            {
                //MessageBox( DXUTGetHWND(), L"Animation information will be lost!!!", DXUTGetWindowTitle(), MB_ICONWARNING|MB_OK );
                wstrErr = L"완료(AniInfo 제거)";
            }
            else
                s_eksmOptions = KSMOPTIONS_SKINMESH;
        }
        else
        {
            if ( s_eksmOptions == KSMOPTIONS_NONE )
                s_eksmOptions = KSMOPTIONS_MESH;
        }//if.. else..


        if ( s_eksmOptions == KSMOPTIONS_SKINMESH )
        {
            LPD3DXANIMATIONCONTROLLER pClone = pSkinMesh->GetCloneAC();
            //ASSERT( pClone != NULL );
            
            SAFE_RELEASE( pClone );
        
            hr = pSkinMesh->WriteToFileSerializer( kOutFile );
            if ( FAILED( hr ) )
            {
                //MessageBox( DXUTGetHWND(), L"File format conversion failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
                wstrErr = L"File format conversion failed";
                ++nErrCount;
                goto end;
            }//if
        }
        else
        {
            pMesh = new CKTDXDeviceXMesh( DXUTGetD3DDevice(), wstrFilename.c_str() );
            //ASSERT( pMesh != NULL );
            if(pMesh == NULL)
            {
                wstrErr = L"pMesh is NULL";
                ++nErrCount;
                goto end;
            }

            HRESULT hr = pMesh->Load();
            if ( FAILED( hr ) )
            {
                //MessageBox( DXUTGetHWND(), L"File loading failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
                wstrErr = L"File loading failed";
                ++nErrCount;
                goto end;
            }//if
            hr = pMesh->WriteToFileSerializer( kOutFile );
            if ( FAILED( hr ) )
            {
                //MessageBox( DXUTGetHWND(), L"File format conversion failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
                wstrErr = L"File format conversion failed";
                ++nErrCount;
                goto end;
            }//if
        }//if.. else..
        /**/ //}} seojt on 2008-10-8


        // modified code by jintaeks on 2008-10-08, 17:19
        //
        // .x 파일인 경우, skinned mesh로 conversion을 시도한다.
        if ( convertFile.m_bSkinMesh )
        {
            pSkinMesh = new CKTDXDeviceXSkinMesh( DXUTGetD3DDevice(), wstrFilename.c_str() );
            //ASSERT( pSkinMesh != NULL );
            
            if(pSkinMesh == NULL)
            {
                wstrErr = L"pSkinMesh is NULL";
                JLOG_ERROR( wstrErr );
                ++nErrCount;

                ++iNumLoadOrOpenError;

                goto end;
            }

            hr = pSkinMesh->Load();
            if ( FAILED( hr ) )
            {
                //MessageBox( DXUTGetHWND(), L"File loading failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
                wstrErr = L"File loading failed";
                JLOG_ERROR( wstrErr );
                ++nErrCount;

                +iNumLoadOrOpenError;

                goto end;
            }//if

            if ( pSkinMesh->GetIsOnlyModelData() )
            {
                WCHAR   wbuffer[1024];
                wsprintf( wbuffer, L"warn: %s는 animation data를 포함하고 있지 않음\r\n", wstrFilename.c_str() );
                JLOG_INFO( wbuffer );
                OutputDebugStringW( wbuffer );

                ++iNumXFileWhichContainedNoAnimationData;

            }//if


            LPD3DXANIMATIONCONTROLLER pClone = pSkinMesh->GetCloneAC();
            //ASSERT( pClone != NULL );
            
            SAFE_RELEASE( pClone );
        
            hr = pSkinMesh->WriteToFileSerializer( kOutFile );
            if ( FAILED( hr ) )
            {
                //MessageBox( DXUTGetHWND(), L"File format conversion failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
                wstrErr = L"File format conversion failed";
                JLOG_ERROR( wstrErr );
                ++nErrCount;

                ++iNumWritingError;

                goto end;
            }//if
        }
        // .y 파일인 경우 일반 mesh로 conversion을 시도한다.
        else
        {
            pMesh = new CKTDXDeviceXMesh( DXUTGetD3DDevice(), wstrFilename.c_str() );
            //ASSERT( pMesh != NULL );
            if(pMesh == NULL)
            {
                wstrErr = L"pMesh is NULL";
                JLOG_ERROR( wstrErr );
                ++nErrCount;

                ++iNumLoadOrOpenError;

                goto end;
            }//if

            HRESULT hr = pMesh->Load();
            if ( FAILED( hr ) )
            {
                //MessageBox( DXUTGetHWND(), L"File loading failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
                wstrErr = L"File loading failed";
                JLOG_ERROR( wstrErr );
                ++nErrCount;

                ++iNumLoadOrOpenError;

                goto end;
            }//if
            hr = pMesh->WriteToFileSerializer( kOutFile );
            if ( FAILED( hr ) )
            {
                //MessageBox( DXUTGetHWND(), L"File format conversion failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
                wstrErr = L"File format conversion failed";
                JLOG_ERROR( wstrErr );
                ++nErrCount;

                ++iNumWritingError;

                goto end;
            }//if
        }//if.. else..


        // 변환된 파일을 write한다.
        //
        if ( s_psz2ndArgument == NULL )
        {
            CHAR    szFilename[MAX_PATH];
            //CHAR    szDirname[MAX_PATH];
            CHAR    szDrive[_MAX_DRIVE];
            CHAR    szDir[_MAX_DIR];
            CHAR    szFName[ _MAX_FNAME];
            CHAR    szExt[ _MAX_EXT ];

            _splitpath( s_psz1stArgument, szDrive, szDir, szFName, szExt );
            StringCchPrintfA( szFilename, ARRAY_SIZE( szFilename )
                , "%s%s%s.%s", szDrive, szDir, szFName, ( s_eksmOptions == KSMOPTIONS_SKINMESH ) ? "ksm" : "kme" );

            hr = kOutFile.SaveFile( szFilename );
            if ( SUCCEEDED(hr) ) ++iNumConverted;
            JLOG( "saving %s", szFilename );
        }
        else
        {
            hr = kOutFile.SaveFile( s_psz2ndArgument );
            if ( SUCCEEDED(hr) ) ++iNumConverted;
            JLOG( "saving %s", s_psz2ndArgument );
        }//if.. else..
        //kOutFile.Unlock();

        if ( FAILED( hr ) )
        {
            //MessageBox( DXUTGetHWND(), L"FIle saving failed", DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
            wstrErr = L"FIle saving failed";
            JLOG_ERROR( wstrErr );
            ++nErrCount;

            ++iNumWritingError;

            goto end;
        }//if
        iErrorCode = 0;

    end:        

        SAFE_DELETE( pSkinMesh );
        SAFE_DELETE( pMesh );
        SAFE_DELETE_ARRAY( s_psz1stArgument );
        SAFE_DELETE_ARRAY( s_psz2ndArgument );  
#endif        
        InsertRow(hList, i, convertFile, wstrErr);
        
        //Sleep(100);

        if(!g_bProcess)
            break;
        
    } // end of for (g_vecFileList)    
    
    JLOG_INFO( "iNumAlreadyConverted = %d", iNumAlreadyConverted );
    JLOG_INFO( "iNumConverted = %d", iNumConverted );
    JLOG_INFO( "iNumProcessing = %d", iNumProcessing );
    JLOG_INFO( "iNumLoadOrOpenError = %d", iNumLoadOrOpenError );
    JLOG_INFO( "iNumWritingError = %d", iNumWritingError );
    JLOG_INFO( "iNumXFileWhichContainedNoAnimationData = %d", iNumXFileWhichContainedNoAnimationData );


    if(nErrCount > 0)
    {
        MSG msg;

        while(GetMessage(&msg, NULL, 0, 0)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;

    // Pass control to DXUT for handling the message pump and 
    // dispatching render calls. DXUT will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    //DXUTMainLoop();
    

    // Perform any application-level cleanup here. Direct3D device resources are released within the
    // appropriate callback functions and therefore don't require any cleanup code here.

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    // Initialize dialogs
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );

    g_HUD.SetCallback( OnGUIEvent ); int iY = 10; 
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

    g_SampleUI.SetCallback( OnGUIEvent );
    g_SampleUI.SetFont( 1, L"Comic Sans MS", 24, FW_NORMAL );
    g_SampleUI.SetFont( 2, L"Courier New", 16, FW_NORMAL );

    // Static
    g_SampleUI.AddStatic( IDC_STATIC, L"This is a static control.", 0, 0, 200, 30 );
    g_SampleUI.AddStatic( IDC_OUTPUT, L"This static control provides feedback for your action.  It will change as you interact with the UI controls.", 20, 50, 620, 300 );
    g_SampleUI.GetStatic( IDC_OUTPUT )->SetTextColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) ); // Change color to red
    g_SampleUI.GetStatic( IDC_STATIC )->SetTextColor( D3DCOLOR_ARGB( 255, 0, 255, 0 ) ); // Change color to green
    g_SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->dwTextFormat = DT_LEFT|DT_TOP|DT_WORDBREAK;
    g_SampleUI.GetControl( IDC_OUTPUT )->GetElement( 0 )->iFont = 2;
    g_SampleUI.GetControl( IDC_STATIC )->GetElement( 0 )->dwTextFormat = DT_CENTER|DT_VCENTER|DT_WORDBREAK;

    // Buttons
    g_SampleUI.AddButton( IDC_ENABLEIME, L"Enable (I)ME", 30, 390, 80, 35, L'I' );
    g_SampleUI.AddButton( IDC_DISABLEIME, L"Disable I(M)E", 30, 430, 80, 35, L'M' );

    // Edit box
    g_SampleUI.AddEditBox( IDC_EDITBOX1, L"Edit control with default styles. Type text here and press Enter", 20, 440, 600, 32 );

    // IME-enabled edit box
    CDXUTIMEEditBox *pIMEEdit;
    if( SUCCEEDED( g_SampleUI.AddIMEEditBox( IDC_EDITBOX2, L"IME-capable edit control with custom styles. Type and press Enter", 20, 390, 600, 45, false, &pIMEEdit ) ) )
    {
        pIMEEdit->GetElement( 0 )->iFont = 1;
        pIMEEdit->GetElement( 1 )->iFont = 1;
        pIMEEdit->GetElement( 9 )->iFont = 1;
        pIMEEdit->GetElement( 0 )->TextureColor.Init( D3DCOLOR_ARGB( 128, 255, 255, 255 ) );  // Transparent center
        pIMEEdit->SetBorderWidth( 7 );
        pIMEEdit->SetTextColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
        pIMEEdit->SetCaretColor( D3DCOLOR_ARGB( 255, 64, 64, 64 ) );
        pIMEEdit->SetSelectedTextColor( D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
        pIMEEdit->SetSelectedBackColor( D3DCOLOR_ARGB( 255, 40, 72, 72 ) );
    }

    // Slider
    g_SampleUI.AddSlider( IDC_SLIDER, 200, 450, 200, 24, 0, 100, 50, false );

    // Checkbox
    g_SampleUI.AddCheckBox( IDC_CHECKBOX, L"This is a checkbox with hotkey. Press 'C' to toggle the check state.",
                            170, 450, 350, 24, false, L'C', false );
    g_SampleUI.AddCheckBox( IDC_CLEAREDIT, L"This checkbox controls whether edit control text is cleared when Enter is pressed. (T)",
                            170, 460, 450, 24, false, L'T', false );

    // Combobox
    CDXUTComboBox *pCombo;
    g_SampleUI.AddComboBox( IDC_COMBOBOX, 0, 0, 200, 24, L'O', false, &pCombo );
    if( pCombo )
    {
        pCombo->SetDropHeight( 100 );
        pCombo->AddItem( L"Combobox item (O)", (LPVOID)0x11111111 );
        pCombo->AddItem( L"Placeholder (O)", (LPVOID)0x12121212 );
        pCombo->AddItem( L"One more (O)", (LPVOID)0x13131313 );
        pCombo->AddItem( L"I can't get enough (O)", (LPVOID)0x14141414 );
        pCombo->AddItem( L"Ok, last one, I promise (O)", (LPVOID)0x15151515 );
    }

    // Radio buttons
    g_SampleUI.AddRadioButton( IDC_RADIO1A, 1, L"Radio group 1 Amy (1)", 0, 50, 220, 24, false, L'1' );
    g_SampleUI.AddRadioButton( IDC_RADIO1B, 1, L"Radio group 1 Brian (2)", 0, 50, 220, 24, false, L'2' );
    g_SampleUI.AddRadioButton( IDC_RADIO1C, 1, L"Radio group 1 Clark (3)", 0, 50, 220, 24, false, L'3' );

    g_SampleUI.AddRadioButton( IDC_RADIO2A, 2, L"Single (4)", 0, 50, 90, 24, false, L'4' );
    g_SampleUI.AddRadioButton( IDC_RADIO2B, 2, L"Double (5)", 0, 50, 90, 24, false, L'5' );
    g_SampleUI.AddRadioButton( IDC_RADIO2C, 2, L"Triple (6)", 0, 50, 90, 24, false, L'6' );

    // List box
    g_SampleUI.AddListBox( IDC_LISTBOX, 30, 400, 200, 150, 0 );
    for( int i = 0; i < 15; ++i )
    {
        WCHAR wszText[50];
        StringCchPrintf( wszText, 50, L"Single-selection listbox item %d", i );
        g_SampleUI.GetListBox( IDC_LISTBOX )->AddItem( wszText, (LPVOID)(size_t)i );
    }
    g_SampleUI.AddListBox( IDC_LISTBOXM, 30, 400, 200, 150, CDXUTListBox::MULTISELECTION );
    for( int i = 0; i < 30; ++i )
    {
        WCHAR wszText[50];
        StringCchPrintf( wszText, 50, L"Multi-selection listbox item %d", i );
        g_SampleUI.GetListBox( IDC_LISTBOXM )->AddItem( wszText, (LPVOID)(size_t)i );
    }
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3DObject(); 
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
                    D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    // Must support pixel shader 1.1
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 1, 1 ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
         pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
    {
        pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
        pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif
    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning();
    }

    return true;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;


    V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         L"Arial", &g_pFont ) );

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;
    #ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
    #ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"CustomUI.fx" ) );

    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags, 
                                        NULL, &g_pEffect, NULL ) );

    g_Mesh.Create( pd3dDevice, L"misc\\cell.x" );

    // Setup the camera's view parameters
    D3DXVECTOR3 vecEye(0.0f, 1.5f, -7.0f);
    D3DXVECTOR3 vecAt (0.0f, 0.2f, 0.0f);
    D3DXVECTOR3 vecUp (0.0f, 1.0f, 0.0f );
    g_Camera.SetViewParams( &vecEye, &vecAt );
    D3DXMatrixLookAtLH( &g_mView, &vecEye, &vecAt, &vecUp );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also optimizes the 
// mesh for the graphics card's vertex cache, which improves performance by organizing 
// the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh )
{
    ID3DXMesh* pMesh = NULL;
    WCHAR str[MAX_PATH];
    HRESULT hr;

    // Load the mesh with D3DX and get back a ID3DXMesh*.  For this
    // sample we'll ignore the X file's embedded materials since we know 
    // exactly the model we're loading.  See the mesh samples such as
    // "OptimizedMesh" for a more generic mesh loading example.
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, strFileName ) );

    V_RETURN( D3DXLoadMeshFromX(str, D3DXMESH_MANAGED, pd3dDevice, NULL, NULL, NULL, NULL, &pMesh) );

    DWORD *rgdwAdjacency = NULL;

    // Make sure there are normals which are required for lighting
    if( !(pMesh->GetFVF() & D3DFVF_NORMAL) )
    {
        ID3DXMesh* pTempMesh;
        V( pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                  pMesh->GetFVF() | D3DFVF_NORMAL, 
                                  pd3dDevice, &pTempMesh ) );
        V( D3DXComputeNormals( pTempMesh, NULL ) );

        SAFE_RELEASE( pMesh );
        pMesh = pTempMesh;
    }

    // Optimize the mesh for this graphics card's vertex cache 
    // so when rendering the mesh's triangle list the vertices will 
    // cache hit more often so it won't have to re-execute the vertex shader 
    // on those vertices so it will improve perf.     
    rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
    if( rgdwAdjacency == NULL )
        return E_OUTOFMEMORY;
    V( pMesh->GenerateAdjacency(1e-6f,rgdwAdjacency) );
    V( pMesh->OptimizeInplace(D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL) );
    delete []rgdwAdjacency;

    *ppMesh = pMesh;

    return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, 
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnResetDevice() );
    V_RETURN( g_SettingsDlg.OnResetDevice() );

    if( g_pFont )
        V_RETURN( g_pFont->OnResetDevice() );
    if( g_pEffect )
        V_RETURN( g_pEffect->OnResetDevice() );

    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pTextSprite ) );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 1000.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width-170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( 0, 0 );
    g_SampleUI.SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );

    g_SampleUI.GetControl( IDC_STATIC )->SetSize( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height * 6 / 10 );
    g_SampleUI.GetControl( IDC_OUTPUT )->SetSize( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height / 4 );
    g_SampleUI.GetControl( IDC_EDITBOX1 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 230 );
    g_SampleUI.GetControl( IDC_EDITBOX1 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 32 );
    g_SampleUI.GetControl( IDC_EDITBOX2 )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 280 );
    g_SampleUI.GetControl( IDC_EDITBOX2 )->SetSize( pBackBufferSurfaceDesc->Width - 40, 45 );
    g_SampleUI.GetControl( IDC_ENABLEIME )->SetLocation( 130, pBackBufferSurfaceDesc->Height - 80 );
    g_SampleUI.GetControl( IDC_DISABLEIME )->SetLocation( 220, pBackBufferSurfaceDesc->Height - 80 );
    g_SampleUI.GetControl( IDC_SLIDER )->SetLocation( 10, pBackBufferSurfaceDesc->Height - 140 );
    g_SampleUI.GetControl( IDC_CHECKBOX )->SetLocation( 120, pBackBufferSurfaceDesc->Height - 50 );
    g_SampleUI.GetControl( IDC_CLEAREDIT )->SetLocation( 120, pBackBufferSurfaceDesc->Height - 25 );
    g_SampleUI.GetControl( IDC_COMBOBOX )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 180 );
    g_SampleUI.GetControl( IDC_RADIO1A )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 100 );
    g_SampleUI.GetControl( IDC_RADIO1B )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 124 );
    g_SampleUI.GetControl( IDC_RADIO1C )->SetLocation( pBackBufferSurfaceDesc->Width - 160, 148 );
    g_SampleUI.GetControl( IDC_RADIO2A )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 100 );
    g_SampleUI.GetControl( IDC_RADIO2B )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 76 );
    g_SampleUI.GetControl( IDC_RADIO2C )->SetLocation( 20, pBackBufferSurfaceDesc->Height - 52 );
    g_SampleUI.GetControl( IDC_LISTBOX )->SetLocation( pBackBufferSurfaceDesc->Width - 400, pBackBufferSurfaceDesc->Height - 180 );
    g_SampleUI.GetControl( IDC_LISTBOX )->SetSize( 190, 96 );
    g_SampleUI.GetControl( IDC_LISTBOXM )->SetLocation( pBackBufferSurfaceDesc->Width - 200, pBackBufferSurfaceDesc->Height - 180 );
    g_SampleUI.GetControl( IDC_LISTBOXM )->SetSize( 190, 124 );
    g_Mesh.RestoreDeviceObjects( pd3dDevice );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    D3DXMATRIXA16 m;
    D3DXMatrixRotationY( &m, D3DX_PI * fElapsedTime / 40.0f );
    D3DXMatrixMultiply( &g_mView, &m, &g_mView );
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }

    HRESULT hr;
    D3DXMATRIXA16 mWorld;
    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    D3DXMATRIXA16 mWorldViewProjection;
    
    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
        // Get the projection & view matrix from the camera class
        mWorld = *g_Camera.GetWorldMatrix();       
        mProj = *g_Camera.GetProjMatrix();       
        mView = g_mView;

        mWorldViewProjection = mWorld * mView * mProj;

        // Update the effect's variables.  Instead of using strings, it would 
        // be more efficient to cache a handle to the parameter by calling 
        // ID3DXEffect::GetParameterByName
        V( g_pEffect->SetMatrix( "g_mWorldViewProjection", &mWorldViewProjection ) );
        V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );
        V( g_pEffect->SetFloat( "g_fTime", (float)fTime ) );

        g_pEffect->SetTechnique( "RenderScene" );
        UINT cPasses;
        g_pEffect->Begin( &cPasses, 0 );
        ID3DXMesh *pMesh = g_Mesh.GetLocalMesh();
        for( UINT p = 0; p < cPasses; ++p )
        {
            g_pEffect->BeginPass( p );
            for( UINT m = 0; m < g_Mesh.m_dwNumMaterials; ++m )
            {
                g_pEffect->SetTexture( "g_txScene", g_Mesh.m_pTextures[m] );
                g_pEffect->CommitChanges();
                pMesh->DrawSubset( m );
            }
            g_pEffect->EndPass();
        }
        g_pEffect->End();

        RenderText();
        V( g_HUD.OnRender( fElapsedTime ) );
        V( g_SampleUI.OnRender( fElapsedTime ) );

        V( pd3dDevice->EndScene() );
    }
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves performance.
    CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    txtHelper.DrawTextLine( DXUTGetFrameStats() );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );

    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    
    // Draw help
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.DrawTextLine( L"Press ESC to quit" );
    txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );

    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    WCHAR wszOutput[1024];

    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:     g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
        case IDC_ENABLEIME:
            CDXUTIMEEditBox::EnableImeSystem( true );
            g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( L"You clicked the 'Enable IME' button.\nIME text input is enabled for IME-capable edit controls." );
            break;
        case IDC_DISABLEIME:
            CDXUTIMEEditBox::EnableImeSystem( false );
            g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( L"You clicked the 'Disable IME' button.\nIME text input is disabled for IME-capable edit controls." );
            break;
        case IDC_EDITBOX1:
        case IDC_EDITBOX2:
            switch( nEvent )
            {
                case EVENT_EDITBOX_STRING:
                {
                    StringCchPrintf( wszOutput, 1024, L"You have pressed Enter in edit control (ID %u).\nThe content of the edit control is:\n\"%s\"",
                                nControlID, ((CDXUTEditBox*)pControl)->GetText() );
                    g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    // Clear the text if needed
                    if( g_SampleUI.GetCheckBox( IDC_CLEAREDIT )->GetChecked() )
                        ((CDXUTEditBox*)pControl)->SetText( L"" );
                    break;
                }

                case EVENT_EDITBOX_CHANGE:
                {
                    StringCchPrintf( wszOutput, 1024, L"You have changed the content of an edit control (ID %u).\nIt is now:\n\"%s\"",
                                nControlID, ((CDXUTEditBox*)pControl)->GetText() );
                    g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );

                    break;
                }
            }
            break;
        case IDC_SLIDER:
            StringCchPrintf( wszOutput, 1024, L"You adjusted the slider control.\nThe new value reported is %d",
                        ((CDXUTSlider*)pControl)->GetValue() );
            g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_CHECKBOX:
            StringCchPrintf( wszOutput, 1024, L"You %s the upper check box.",
                ((CDXUTCheckBox*)pControl)->GetChecked() ? L"checked" : L"cleared" );
            g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_CLEAREDIT:
            StringCchPrintf( wszOutput, 1024, L"You %s the lower check box.\nNow edit controls will %s",
                        ((CDXUTCheckBox*)pControl)->GetChecked() ? L"checked" : L"cleared",
                        ((CDXUTCheckBox*)pControl)->GetChecked() ? L"be cleared when you press Enter to send the text" : L"retain the text context when you press Enter to send the text" );
            g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_COMBOBOX:
        {
            DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
            if( pItem )
            {
                StringCchPrintf( wszOutput, 1024, L"You selected a new item in the combobox.\nThe new item is \"%s\" and the associated data value is 0x%p",
                            pItem->strText, pItem->pData );
                g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            }
            break;
        }
        case IDC_RADIO1A:
        case IDC_RADIO1B:
        case IDC_RADIO1C:
            StringCchPrintf( wszOutput, 1024, L"You selected a new radio button in the UPPER radio group.\nThe new button is \"%s\"",
                ((CDXUTRadioButton*)pControl)->GetText() );
            g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;
        case IDC_RADIO2A:
        case IDC_RADIO2B:
        case IDC_RADIO2C:
            StringCchPrintf( wszOutput, 1024, L"You selected a new radio button in the LOWER radio group.\nThe new button is \"%s\"",
                ((CDXUTRadioButton*)pControl)->GetText() );
            g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
            break;

        case IDC_LISTBOX:
            switch( nEvent )
            {
                case EVENT_LISTBOX_ITEM_DBLCLK:
                {
                    DXUTListBoxItem *pItem = ((CDXUTListBox *)pControl)->GetItem( ((CDXUTListBox *)pControl)->GetSelectedIndex( -1 ) );

                    StringCchPrintf( wszOutput, 1024, L"You double clicked an item in the left list box.  The item is\n\"%s\"",
                        pItem ? pItem->strText : L"" );
                    g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }

                case EVENT_LISTBOX_SELECTION:
                {
                    StringCchPrintf( wszOutput, 1024, L"You changed the selection in the left list box.  The selected item is %d", ((CDXUTListBox *)pControl)->GetSelectedIndex() );
                    g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }
            }
            break;

        case IDC_LISTBOXM:
            switch( nEvent )
            {
                case EVENT_LISTBOX_ITEM_DBLCLK:
                {
                    DXUTListBoxItem *pItem = ((CDXUTListBox *)pControl)->GetItem( ((CDXUTListBox *)pControl)->GetSelectedIndex( -1 ) );

                    StringCchPrintf( wszOutput, 1024, L"You double clicked an item in the right list box.  The item is\n\"%s\"",
                        pItem ? pItem->strText : L"" );
                    g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }

                case EVENT_LISTBOX_SELECTION:
                {
                    StringCchPrintf( wszOutput, 1024, L"You changed the selection in the right list box.  The selected item(s) are\n" );
                    int nSelected = -1;
                    while( ( nSelected = ((CDXUTListBox *)pControl)->GetSelectedIndex( nSelected ) ) != -1 )
                    {
                        StringCchPrintf( wszOutput + lstrlenW( wszOutput ), 1024 - lstrlenW( wszOutput ), L"%d,", nSelected );
                    }
                    // Remove the trailing comma if one exists.
                    if( wszOutput[lstrlenW(wszOutput)-1] == L',' )
                        wszOutput[lstrlenW(wszOutput)-1] = L'\0';
                    g_SampleUI.GetStatic( IDC_OUTPUT )->SetText( wszOutput );
                    break;
                }
            }
            break;
    }
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
    g_DialogResourceManager.OnLostDevice();
    g_SettingsDlg.OnLostDevice();
    g_Mesh.InvalidateDeviceObjects();

    if( g_pFont )
        g_pFont->OnLostDevice();
    if( g_pEffect )
        g_pEffect->OnLostDevice();
    SAFE_RELEASE(g_pTextSprite);
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnDestroyDevice();
    g_SettingsDlg.OnDestroyDevice();
    g_Mesh.Destroy();

    SAFE_RELEASE(g_pEffect);
    SAFE_RELEASE(g_pFont);
}


void AddFolder(wchar_t *strPath)
{
    BOOL ret = true;;
    HANDLE hSrch;
    WIN32_FIND_DATA wfd;    

    wchar_t efound[MAX_PATH], efront[MAX_PATH], *epath, tempPath[MAX_PATH];  //배열선언
    
    wcscpy(tempPath, strPath);
    wcscat(tempPath, L"\\*.*");
    int count;
    int ei = 0, ecount, enchanged=1;
    CONVERTINFO convertMesh;

    count = ecount = 0;  

    hSrch = FindFirstFile(tempPath, &wfd);
    while(ret)
    {
        if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {

            // directory
            if(wcsstr(wfd.cFileName, L".") == NULL)
            {            
                wsprintf(efront, L"%s", wfd.cFileName);
                wsprintf(tempPath, L"%s\\%s", strPath, efront);
                
                AddFolder(tempPath);
            }
        }
        else
        {
            // file
            wsprintf(efront, L"%s", wfd.cFileName);

            if(wcscmp(efront + wcslen(efront)-2, L".x") == 0 ||
               wcscmp(efront + wcslen(efront)-2, L".X") == 0)
            {
                convertMesh.m_strFileName = efront;
                convertMesh.m_strPath = strPath;
                convertMesh.m_bSkinMesh = true;
                
                g_vecFileList.push_back(convertMesh);         
            }
            else if(wcscmp(efront + wcslen(efront)-2, L".y") == 0 ||
               wcscmp(efront + wcslen(efront)-2, L".Y") == 0)       
            {           
                convertMesh.m_strFileName = efront;
                convertMesh.m_strPath = strPath;
                convertMesh.m_bSkinMesh = false;

                g_vecFileList.push_back(convertMesh);         
            }
        }

        ret = FindNextFile(hSrch, &wfd);
    }

    FindClose(hSrch);

}

int  InsertRow(HWND hList, int rownum, CONVERTINFO fInfo, std::wstring fStatus)
{
    LVITEM LI;
    
    LI.mask = LVIF_TEXT;
    LI.state = 0;
    LI.stateMask = 0;

    LI.iSubItem = 0;
    LI.iItem = rownum;
    LI.pszText = (LPWSTR)fInfo.m_strFileName.c_str();
    SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LI);

    LI.iSubItem = 1;
    LI.pszText = (LPWSTR)(fStatus.c_str());
    SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

    LI.iSubItem = 2;
    LI.pszText = (LPWSTR)fInfo.m_strPath.c_str();
    SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

    SendMessage( hList, LB_SETCURSEL, (WPARAM)rownum, 0L );//
    return 0;
}


BOOL CALLBACK ProgressDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    switch(iMessage)
    {
    case WM_INITDIALOG:
        return true;
    case WM_COMMAND:
        switch(wParam)
        {
        case IDOK:
        case IDCANCEL:
            g_bProcess = FALSE;
            EndDialog(hDlg, 0);
            return true;
        }
        break;
    }
    return false;
}