// =========================================================
// worldtool 컴파일 방법
// ktdx.h 에서 #define WORLD_TOOL
//		DGmanager에서 culling disable							: ortho mode에서 컬링 제대로 안되는 문제 때문에
//		guuser에서 CommonStateEnd에서 fadeworldcolor disable	
//		이외 몇가지를 WORLD_TOOL에 묶어두었습니다.

// 090519 태완 : 어떻게든 컴파일 및 실행만은 되게 하려고 #ifndef WORLD_TOOL로 코드 곳곳을 잘라뒀습니다.
// 일단 실행은 됩니다..;;

// ktdx.h 에서 #define CHECKSUM_THREAD_TEST comment out
// =========================================================

// 2010.04.28 장훈 : loadworld(), loadlinemap() 에서 스크립트 파싱할때 Ansi 이면 강제로 UTF 로 교체기능 추가..
// 2009.12.03 장훈 : XMeshPlayer 프리뷰 & 클립보드, 스크립트 열기 기능추가.
// 2009.12.02 장훈 : 파티클 리스트 클릭시 클립보드 & 스크립트 열기 기능추가
// 2009.11.27 장훈 : 파티클 리스트 클릭시 프리뷰 기능 추가
// 2009.11.24 장훈 : 스크립트 로딩 & 저장 파일 형식을 Ansi -> UTF-8 로 변경

// 2009.03.14 김태완 : 카메라 고정 기능

// 2009.02.25~ 김태완 : 툴 수정작업
// 다이나믹 카메라 on/off 기능 추가
// 신캐릭터 추가 완료
// 라인맵 저장시 filename_LINEMAP이 기본으로 되도록 수정
// 광장시점 체크시 카메라 시점 1200으로 변하게 / 체크 헤제시 1500으로(구 기본값) 변하게 변경
// 라인맵 파일 오픈 실패 현상 해결
// 라인맵 안 나오는 현상 해결
// world object remove 제대로 안 되는 현상 해결
// 캐릭터 제대로 안 움직이는 현상 해결
// OnDialogEvent_DLG_EFFECT_PROPERTY 비정상 작동 수정
// WorldObject 카메라에서 일정 거리 밖일 때 렌더링 하지 않는 기능 추가
// 새로 추가된 라인맵 종류 적용
// 기타 각종 Crash 현상들 수정

// 2008.10.21 dmlee
// square camera모드 선택시에 eye distance, height, lookat distance 조정할수 있는 editbox 추가, - 새마을 작업 관련 

// 2008.04.28 dmlee
// 광장 카메라 모드일 때 카메라 거리 직접 입력할 수 있게 수정
// 화면 가운데 녹색점 껏다 켰다 할 수 있게 체크박스 추가

// 2008.03.21 dmlee
// 월드 컬러조정하는 슬라이드 컨트롤 추가
// 선택된 메쉬 좀 더 쉽게 알아볼 수 있게 
// ESC 키를 누르면 deselect world object
// 이펙트 툴에 카메라 흔들리는 기능 넣을 수 있게 수정

// 2008.03.04 dmlee
// 몬스터 불러올 수 있는 기능 추가

// 2008.03.03 dmlee
// 캐릭터 애니메이션 위치 고정하는 기능 추가

// 2008-02-22 dmlee
// 개별 오브젝트에 월드컬러를 적용할지 안할지 선택하는 체크박스 추가

// 2008-02-19 dmlee
// 이펙트툴에서 모션 파일 불러올 때 .X 파일을 기본으로 찾도록 수정
// 이펙트툴에서 이펙트 실행할 때 느려지는 문제 수정하기 위해서 CDeviceReady 클래스 추가

// 2008-01-25 dmlee
// 확장자가 Y인 파일을 drag&drop 해서 작업 할 수 있게 수정
// 체크박스 alphaobject, destblend가 월드스크립트를 저장 후 다시 불러오면 리셋 되어 있는 버그 수정
// 체크박스 canhide 추가 - 월드오브젝트가 캐릭터 가리면 투명화되게 하는 옵션




#include "dxstdafx.h"
#include ".\x2worldtoolmain.h"
#include "resource.h"

using std::endl;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HWND g_hDlg = NULL;
HWND g_hDlgLineMap = NULL;
HWND g_hFrustumDlg = NULL;

CX2WorldToolMain *g_pWorldToolMain;

typedef struct 
{
    WCHAR strName[100];
    int  inx;
} LayerData;

typedef struct 
{
    WCHAR strName[100];
    int inx;
} Blend_Mode;

typedef struct 
{
	WCHAR strName[100];
	int inx;
} EnumStringTable;

bool g_bReDrawLayer = false;

//{{ kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
#ifdef	ADD_X2_LAYER
	#define MAX_LAYER 19
#else	ADD_X2_LAYER
	#define MAX_LAYER 17
#endif	ADD_X2_LAYER
//}} kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업


#define MAX_BLEND 6

void InitLayerDlg(HWND hDlg);
void UpdateLayer(HWND hDlg, CX2WorldLayer* pLayer);
void UpdateLayerDlg(HWND hDlg, CX2WorldLayer* pLayer);
void UpdatelayerInfo(HWND hDlg);

BOOL CALLBACK WorldToolDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    LayerData g_strLayer[MAX_LAYER] =
    {
        {L"SKY_DOME_0",      							XL_SKY_DOME_0},
        {L"SKY_DOME_1",      							XL_SKY_DOME_1},
        {L"SKY_DOME_2",      							XL_SKY_DOME_2},
        {L"WORLD_BASE_0",    							XL_SKY_WORLD_BASE_0},
        {L"WORLD_BASE_1",    							XL_SKY_WORLD_BASE_1},
        {L"WORLD_BASE_2",    							XL_SKY_WORLD_BASE_2},    
        {L"WORLD_OBJECT_0",  							XL_SKY_WORLD_OBJECT_0},
        {L"WORLD_OBJECT_1",  							XL_SKY_WORLD_OBJECT_1},
        {L"WORLD_OBJECT_2",  							XL_SKY_WORLD_OBJECT_2},
        {L"UNIT_0",										XL_UNIT_0},
        {L"UNIT_1",										XL_UNIT_1},
        {L"UNIT_2",										XL_UNIT_2},

        {L"EFFECT_0",        							XL_EFFECT_0},
        {L"EFFECT_1",        							XL_EFFECT_1},
        {L"EFFECT_2",        							XL_EFFECT_2},

		//{{ kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
#ifdef	ADD_X2_LAYER
		{L"XL_EFFECT_OBJ_0",							XL_SKY_WORLD_OBJECT_OVER_EFFECT_0},
		{L"XL_EFFECT_OBJ_1",							XL_SKY_WORLD_OBJECT_OVER_EFFECT_1},
		{L"XL_EFFECT_OBJ_2",							XL_SKY_WORLD_OBJECT_OVER_EFFECT_2},
		{L"XL_EFFECT_OBJ_3",							XL_SKY_WORLD_OBJECT_OVER_EFFECT_3},
// 		{L"XL_EFFECT_OVER_ALL_WORLD_OBJECT_0",          XL_EFFECT_OVER_ALL_WORLD_OBJECT_0},
// 		{L"XL_EFFECT_OVER_ALL_WORLD_OBJECT_1",          XL_EFFECT_OVER_ALL_WORLD_OBJECT_1},
#endif	ADD_X2_LAYER
		//}} kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업

// kimhc // XL_EFFECT_OVER_ALL_WORLD_OBJECT 와 함께 월드툴에서 사용하지 말아야할 것 주석 처리함
//         {L"LENS_FLARE",      							XL_LENS_FLARE},
//         {L"MINIMAP",        							 XL_MINIMAP}
    };

    Blend_Mode g_strBlend[MAX_BLEND] =
    {
        {L"선형: Normal",           0},
        {L"덧셈: lighten/screen",   1},
        {L"뺄셈: darken/multiply",  2},
        {L"곱셈: Spot",             3},
        {L"제곱: overlay",          4},
        {L"반전: Inverse",          5},
    };

    if(g_pWorldToolMain == NULL)
        return FALSE;

    switch(iMessage)
    {
    case WM_CREATE:
    case WM_INITDIALOG:              
        g_bReDrawLayer = true;

        for(int i=0; i<MAX_BLEND; ++i)
        {
            SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING,      0, (LPARAM)g_strBlend[i].strName);
            SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETITEMDATA,    i, (LPARAM)g_strBlend[i].inx);
        }        
        SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, 0, 0);

        for(int i=0; i<MAX_LAYER; ++i)
        {
            SendDlgItemMessage(hDlg, IDC_COMBO2, CB_ADDSTRING,      0, (LPARAM)g_strLayer[i].strName);
            SendDlgItemMessage(hDlg, IDC_COMBO2, CB_SETITEMDATA,    i, (LPARAM)g_strLayer[i].inx);
        }        
        SendDlgItemMessage(hDlg, IDC_COMBO2, CB_SETCURSEL, 15, 0);

        SendDlgItemMessage(hDlg, IDC_SLIDER2, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
        SendDlgItemMessage(hDlg, IDC_SLIDER2, TBM_SETPOS, TRUE, 255);
        SendDlgItemMessage(hDlg, IDC_SLIDER3, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
        SendDlgItemMessage(hDlg, IDC_SLIDER3, TBM_SETPOS, TRUE, 255);
        SendDlgItemMessage(hDlg, IDC_SLIDER4, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
        SendDlgItemMessage(hDlg, IDC_SLIDER4, TBM_SETPOS, TRUE, 255);
		SendDlgItemMessage(hDlg, IDC_SLIDER5, TBM_SETRANGE, TRUE, MAKELPARAM(0, 255));
		SendDlgItemMessage(hDlg, IDC_SLIDER5, TBM_SETPOS, TRUE, 255);
        
        int pos;
        WCHAR buf[10];
        pos = SendDlgItemMessage(hDlg, IDC_SLIDER2, TBM_GETPOS, 0, 0);
        wsprintf(buf, L"%d", pos);
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_RED), buf);

        pos = SendDlgItemMessage(hDlg, IDC_SLIDER3, TBM_GETPOS, 0, 0);
        wsprintf(buf, L"%d", pos);
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_GREEN), buf);

        pos = SendDlgItemMessage(hDlg, IDC_SLIDER4, TBM_GETPOS, 0, 0);
        wsprintf(buf, L"%d", pos);
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_BLUE), buf);  

		pos = SendDlgItemMessage(hDlg, IDC_SLIDER5, TBM_GETPOS, 0, 0);
		wsprintf(buf, L"%d", pos);
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_BLUE2), buf);  
        
        wsprintf(buf, L"0.0");
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), buf);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), buf);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT5), buf);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT6), buf);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT7), buf);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT8), buf);

        wsprintf(buf, L"1.0");
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT3), buf);
        SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), buf);

		SendDlgItemMessage(hDlg, IDC_LAYER_CHECK1, BM_SETCHECK, (WPARAM)true, 0);

        UpdateLayerDlg(hDlg, NULL);

        g_bReDrawLayer = false;

        return TRUE;    

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDC_BUTTON1:
            // 삭제
            {
                CX2WorldLayer *pWorldLayer;
                int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
                if(index == -1)
                    return TRUE;

                pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];

                g_pWorldToolMain->m_vecWorldLayer.erase(g_pWorldToolMain->m_vecWorldLayer.begin() + index);
                g_pWorldToolMain->m_pGame->GetWorld()->RemoveWordLayer(index);

                UpdateLayerDlg(hDlg, NULL);                
            }
            return TRUE;

        case IDOK:
            {            
                OPENFILENAME ofn;        // common dialog box structure
                WCHAR wszFileName[1024];  // path까지 포함한 파일 이름
                WCHAR wszFilePathOnly[512] = L"";
                WCHAR wszFileNameOnly[256] = L"";

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize		= sizeof(ofn);
                ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
                ofn.lpstrFile		= (LPWSTR)wszFileName;
                ofn.lpstrFile[0]	= '\0';
                ofn.nMaxFile		= sizeof(wszFileName);
                ofn.lpstrFilter		= L"TGA Image\0*.tga\0DDS Image\0*.dds\0";
                ofn.nFilterIndex	= 1;
                ofn.lpstrFileTitle	= NULL;
                ofn.nMaxFileTitle	= 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
                
                if( TRUE == GetOpenFileName( &ofn ) )
                {
                    CX2WorldLayer *pLayer = g_pWorldToolMain->m_pGame->GetWorld()->CreateWorldLayer();

                    if(pLayer == NULL)
                    {
                        MessageBox(hDlg, L"레이어 생성 실패", L"Error!", MB_OK);
                        return FALSE;
                    }                
                    
                    g_pWorldToolMain->DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, ofn.lpstrFile );
                    
                    string TexName;
                    ConvertWCHARToChar( TexName, wszFileNameOnly );                    
                    pLayer->SetTex(TexName.c_str());              

                    g_pWorldToolMain->m_vecWorldLayer.push_back(pLayer);

                    //InitLayerDlg(hDlg); // 새로운 레이어를 위해 레이어 다이얼로그 초기화
                    UpdateLayer(hDlg, pLayer); // 추가된 레이어를 world에 적용       
                    UpdateLayerDlg(hDlg, pLayer);                    
                    if(g_pWorldToolMain->m_vecWorldLayer.size() > 0)
                        SendDlgItemMessage(hDlg, IDC_LIST2, LB_SETCURSEL, g_pWorldToolMain->m_vecWorldLayer.size() -1, 0);

                    UpdatelayerInfo(hDlg);
                }     
            }
            return TRUE;  

        case IDC_BUTTON2:
            {            
                OPENFILENAME ofn;        // common dialog box structure
                WCHAR wszFileName[1024];  // path까지 포함한 파일 이름
                WCHAR wszFilePathOnly[512] = L"";
                WCHAR wszFileNameOnly[256] = L"";

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize		= sizeof(ofn);
                ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
                ofn.lpstrFile		= (LPWSTR)wszFileName;
                ofn.lpstrFile[0]	= '\0';
                ofn.nMaxFile		= sizeof(wszFileName);
                ofn.lpstrFilter		= L"TGA Image\0*.tga\0DDS Image\0*.dds\0";
                ofn.nFilterIndex	= 1;
                ofn.lpstrFileTitle	= NULL;
                ofn.nMaxFileTitle	= 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                CX2WorldLayer *pWorldLayer;
                int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
                if(index == -1)
                {
                    MessageBox(hDlg, L"변경할 레이어를 먼저 선택해주세요", L"Error!", MB_OK);
                    return TRUE;
                }
    
                pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];

                if( TRUE == GetOpenFileName( &ofn ) )
                {
                    if(pWorldLayer == NULL)
                    {
                        MessageBox(hDlg, L"텍스쳐 변경 실패", L"Error!", MB_OK);
                        return FALSE;
                    }                

                    g_pWorldToolMain->DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, ofn.lpstrFile );

                    string TexName;
                    ConvertWCHARToChar( TexName, wszFileNameOnly );                    
                    pWorldLayer->SetTex(TexName.c_str());                                        
                    
                    UpdateLayerDlg(hDlg, NULL);                    
                }     
            }
            return TRUE;  

        case IDC_COMBO1:
            switch(HIWORD(wParam))
            {
            case CBN_SELCHANGE:
                {
                    CX2WorldLayer *pWorldLayer;
                    int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
                    if(index != -1)
                    {                        
                        pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];
                        UpdateLayer(hDlg, pWorldLayer);
                    }
                }                               
            }
            return TRUE;

        case IDC_COMBO2:
            switch(HIWORD(wParam))
            {
            case CBN_SELCHANGE:
                {
                    CX2WorldLayer *pWorldLayer;
                    int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
                    if(index != -1)
                    {                        
                        pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];
                        UpdateLayer(hDlg, pWorldLayer);
                    }
                }                               
            }
            return TRUE;

        case IDC_EDIT1:
        case IDC_EDIT2:
        case IDC_EDIT3:
        case IDC_EDIT4:
        case IDC_EDIT5:
        case IDC_EDIT6:
        case IDC_EDIT7:
        case IDC_EDIT8:
            switch(HIWORD(wParam))
            {
            case EN_CHANGE:
                {
                    CX2WorldLayer *pWorldLayer;
                    int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
                    if(index != -1)
                    {                        
                        pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];
                        UpdateLayer(hDlg, pWorldLayer);
                    }
                }                            
            }
            return TRUE;

		case IDC_LIST2:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:                          
				UpdatelayerInfo(hDlg);                 
				return TRUE;                
			}
			return TRUE;

		case IDC_LAYER_CHECK1:
			{
				CX2WorldLayer *pWorldLayer;
				int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
				if(index != -1)
				{                        
					pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];
					UpdateLayer(hDlg, pWorldLayer);
				}			
			}
			return TRUE;
		}
		return TRUE;

	

    case WM_HSCROLL:  
        {
            CX2WorldLayer *pWorldLayer;
            int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
            if(index != -1)
            {                        
                pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];
                UpdateLayer(hDlg, pWorldLayer);
            }
        }   
        return TRUE;        

    case WM_CLOSE:
    case WM_DESTROY:
        DestroyWindow(g_hDlg);		
        g_hDlg = NULL;		
        return TRUE;
    }
    return FALSE;
}

void InitLayerDlg(HWND hDlg)
{
}

void UpdateLayer(HWND hDlg, CX2WorldLayer* pLayer)
{
    X2_LAYER layer;
    int blend;
    int index;

    if(g_bReDrawLayer == true)
        return;

    // 블랜드 모드
    index = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0);
    if(index != -1)
    {
        blend = SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETITEMDATA, index, 0);
        pLayer->SetBlendType(blend);
    }

    
    // 레이어 모드
    index = SendDlgItemMessage(hDlg, IDC_COMBO2, CB_GETCURSEL, 0, 0);
    if(index != -1)
    {
        layer = (X2_LAYER)SendDlgItemMessage(hDlg, IDC_COMBO2, CB_GETITEMDATA, index, 0);
        pLayer->SetLayer((int)layer);
    }

    // 좌표/스케일
    {
        float x, y, w, h;
        char PosBuf[10];
   
        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT1), PosBuf, 9);
        x = (float)atof(PosBuf);
        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT2), PosBuf, 9);
        y = (float)atof(PosBuf);
        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT4), PosBuf, 9);
        w = (float)atof(PosBuf);
        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT3), PosBuf, 9);
        h = (float)atof(PosBuf);

        pLayer->SetPos(x, y, w, h);
    }    

    // 색깔 
    {        
        WCHAR colorbuf[10];
        int rPos, gPos, bPos, aPos;       

        rPos = gPos = bPos = aPos = 255;

        rPos = SendDlgItemMessage(hDlg, IDC_SLIDER2, TBM_GETPOS, 0, 0);
        wsprintf(colorbuf, L"%d", rPos);
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_RED), colorbuf);

        gPos = SendDlgItemMessage(hDlg, IDC_SLIDER3, TBM_GETPOS, 0, 0);
        wsprintf(colorbuf, L"%d", gPos);
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_GREEN), colorbuf);

        bPos = SendDlgItemMessage(hDlg, IDC_SLIDER4, TBM_GETPOS, 0, 0);
        wsprintf(colorbuf, L"%d", bPos);
        SetWindowText(GetDlgItem(hDlg, IDC_STATIC_BLUE), colorbuf);

		aPos = SendDlgItemMessage(hDlg, IDC_SLIDER5, TBM_GETPOS, 0, 0);
		wsprintf(colorbuf, L"%d", aPos);
		SetWindowText(GetDlgItem(hDlg, IDC_STATIC_BLUE2), colorbuf);
        
        D3DXCOLOR color = D3DXCOLOR( rPos/255.0f, gPos/255.0f, bPos/255.0f, aPos/255.f );            
        pLayer->SetColor(color);            
    }

    // UV
    {
        float u, v;
        char PosBuf[10];

        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT5), PosBuf, 9);
        u = (float)atof(PosBuf);
        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT6), PosBuf, 9);
        v = (float)atof(PosBuf);
        pLayer->InitTexUV();
        pLayer->SetTexUV(u, v);   
    }   

    // FeedBack UV
    {
        float u, v;
        char PosBuf[10];

        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT7), PosBuf, 9);
        u = (float)atof(PosBuf);
        GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT8), PosBuf, 9);
        v = (float)atof(PosBuf);        
        pLayer->SetFeedBackUV(u, v);        
    }   

	if( SendDlgItemMessage(hDlg, IDC_LAYER_CHECK1, BM_GETCHECK, 0, 0) == false )
		pLayer->SetShowObject( false );
	else 
		pLayer->SetShowObject( true );

}

void UpdateLayerDlg(HWND hDlg, CX2WorldLayer* pLayer)
{
    if(hDlg != NULL)
    {
        if(pLayer == NULL)
        {            
            int nDel = SendDlgItemMessage(hDlg, IDC_LIST2, LB_DELETESTRING, 0, 0);
            while(nDel > 0)
            {
                nDel = SendDlgItemMessage(hDlg, IDC_LIST2, LB_DELETESTRING, 0, 0);
            }

            for(int i=0; i<(int)g_pWorldToolMain->m_vecWorldLayer.size(); ++i)
            {
                CX2WorldLayer *pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[i];
                wstring texName = pWorldLayer->GetTexName();
                SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)texName.c_str());
            }
        }
        else
        {            
			wstring texName = pLayer->GetTexName();
            SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)texName.c_str());
        }
    }
}

void UpdatelayerInfo(HWND hDlg)
{
    char tempBuf[20];

    CX2WorldLayer *pWorldLayer = NULL;
    int index = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
    if(index != -1)
    {                        
        pWorldLayer = g_pWorldToolMain->m_vecWorldLayer[index];
    }
   

    if(pWorldLayer == NULL)
        return;

    g_bReDrawLayer = true;

    // 레이어
    int layer = pWorldLayer->GetLayer();
    SendDlgItemMessage(hDlg, IDC_COMBO2, CB_SETCURSEL, layer-2, 0);
    
    // 블랜드
    int blend = pWorldLayer->GetBlendType();
    SendDlgItemMessage(hDlg, IDC_COMBO1, CB_SETCURSEL, blend, 0);

    // 좌표/스케일    
    float x, y, w, h;
    pWorldLayer->GetPos(&x, &y, &w, &h);
    sprintf(tempBuf, "%5.2f", x);    
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT1), tempBuf);
    sprintf(tempBuf, "%5.2f", y);    
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT2), tempBuf);
    sprintf(tempBuf, "%5.2f", w);    
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT4), tempBuf);
    sprintf(tempBuf, "%5.2f", h);    
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT3), tempBuf);

    // 색깔
    WCHAR colorbuf[10];    
    D3DXCOLOR color = pWorldLayer->GetColor();
    int rPos, gPos, bPos, aPos;       

    rPos = (int)(color.r * 255);
    gPos = (int)(color.g * 255);
    bPos = (int)(color.b * 255);
	aPos = (int)(color.a * 255);

    SendDlgItemMessage(hDlg, IDC_SLIDER2, TBM_SETPOS, TRUE, rPos);
    wsprintf(colorbuf, L"%d", rPos);
    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_RED), colorbuf);

    SendDlgItemMessage(hDlg, IDC_SLIDER3, TBM_SETPOS, TRUE, gPos);
    wsprintf(colorbuf, L"%d", gPos);
    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_GREEN), colorbuf);

    SendDlgItemMessage(hDlg, IDC_SLIDER4, TBM_SETPOS, TRUE, bPos);
    wsprintf(colorbuf, L"%d", bPos);
    SetWindowText(GetDlgItem(hDlg, IDC_STATIC_BLUE), colorbuf);

	SendDlgItemMessage(hDlg, IDC_SLIDER5, TBM_SETPOS, TRUE, aPos);
	wsprintf(colorbuf, L"%d", aPos);
	SetWindowText(GetDlgItem(hDlg, IDC_STATIC_BLUE2), colorbuf);

    // UV Speed    
    D3DXVECTOR2 texUV = pWorldLayer->GetTexUV();

    sprintf(tempBuf, "%5.2f", texUV.x);
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT5), tempBuf);
    sprintf(tempBuf, "%5.2f", texUV.y);
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT6), tempBuf);

    // UV Speed    
    D3DXVECTOR2 feedbackUV = pWorldLayer->GetFeedBackUV();

    sprintf(tempBuf, "%5.2f", feedbackUV.x);
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT7), tempBuf);
    sprintf(tempBuf, "%5.2f", feedbackUV.y);
    SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT8), tempBuf);

	if( pWorldLayer->GetShowObject() == true )
	{
		SendDlgItemMessage(hDlg, IDC_LAYER_CHECK1, BM_SETCHECK, (WPARAM)true, 0);
	}
	else
	{
		SendDlgItemMessage(hDlg, IDC_LAYER_CHECK1, BM_SETCHECK, (WPARAM)false, 0);
	}

    g_bReDrawLayer = false;
}

// linemap modifier -------------------------------------------------------------------------------------
std::vector<wstring> GetSkinMeshAnimList(wstring wstrMeshName);
std::vector<wstring> UpdateTirggerSkinMeshList(HWND hDlg);
int UpdateTirggerSkinMeshAnimList(HWND hDlg, wstring wstrMeshName, wstring wstrDufaultName = L"");

void initTriggerSegDlg(HWND hDlg);
void InitTriggerDataDlg(HWND hDlg);

void UpdateTriggerDlg(HWND hDlg, CKTDGLineMap *pLineMap);
void UpdateTriggerSegDlg(HWND hDlg, CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg);
void UpdateTriggerDataDlg(HWND hDlg, CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg, int iData);
void UpdateTriggerMeshDlg(HWND hDlg, CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh);
void UpdateTriggerLineDlg(HWND hDlg, CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine);
void UpdateTriggerEtcDlg(HWND hDlg, CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc);

void UpdateTriggerData(HWND hDlg, CKTDGLineMap *pLineMap);
int AddTriggerMesh(HWND hDlg, CKTDGLineMap *pLineMap, int iVal);
int AddTriggerLine(HWND hDlg, CKTDGLineMap *pLineMap, int iVal);
int AddTriggerEtc(HWND hDlg, CKTDGLineMap *pLineMap, int iVal);

D3DXCOLOR CalcUnderWaterUnitColor(CKTDGLineMap *pLineMap, CKTDGLineMap::LineData *pLineData, int iLineCount, int *iLIneList);
void UpdateCameraListDlg(HWND hDlg, CKTDGLineMap *pLineMap);
void UpdateLIneListDlg(HWND hDlg, CKTDGLineMap *pLineMap);
void UpdateCameraModifierDlg(HWND hDlg, CKTDGLineMap *pLineMap, int iIndex);
void UpdateLineModifierDlg(HWND hDlg, CKTDGLineMap *pLineMap, int iIndex);
void ModifyLineData(HWND hDlg, CKTDGLineMap *pLineMap);
void UpdateStartPosDlg(HWND hDlg);
void UpdateStartPosInfo(HWND hDlg, int index);
int	GetStartPosIndex(int iTeam, int iIndex);
void ModifyStartPos(HWND hDlg, int index);

BOOL CALLBACK WorldToolLineModifierDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	EnumStringTable strLineType[11] =
	{
		{ L"LT_NORMAL",			0 },
		{ L"LT_WALL",			1 },
		{ L"LT_POTAL",			2 },
		{ L"LT_HOLE",			3 },
		{ L"LT_JUMP_UP_REL",	4 },
		{ L"LT_JUMP_UP_ABS",	5 },
		{ L"LT_BUNGEE",			6 },
		{ L"LT_MONSTER_ROAD",	7 },
		{ L"LT_UNIT_ROAD",		8 },
		{ L"LT_OTHER_ROAD",		9 },
		{ L"LT_CLIFF",			10 },
	};

	EnumStringTable strDustType[2] =
	{
		{ L"LDT_DEFALT_DUST",	0 },
		{ L"LDT_WATER_SPLASH",	1 },
	};

	EnumStringTable strCameraType[3] =
	{
		{ L"LCT_INVALID",	0 },
		{ L"LCT_ABSOULTE_POSITION",	1 },
		{ L"LCT_RELATIE_POSITION",	2 },
	};

	EnumStringTable strActionMeshType[3] =
	{
		{ L"TAT_MESH_NONE",		0 },
		{ L"TAT_MESH_CHANGE_ANIM",		1 },
		{ L"TAT_MESH_CHANGE_SPEED",		2 },
	};
	EnumStringTable strActionLineType[7] =
	{

		{ L"TAT_LINE_NONE",		0 },
		{ L"TAT_LINE_CHANGE_ENABLE",	1 },
		{ L"TAT_LINE_CHANGE_SPEED",		2 },
		{ L"TAT_LINE_CHANGE_JUMPSPEED",	3 },
		{ L"TAT_LINE_CHANGE_CAMERA",	4 },
		{ L"TAT_LINE_CHANGE_CANPASS",	5 },
		{ L"TAT_LINE_CHANGE_TELEPORT",	6 },
	};
	EnumStringTable strActionEtcType[6] =
	{
		{ L"TAT_ETC_NONE",		0 },
		{ L"TAT_ETC_PLAY_SOUND",		1 },
		{ L"TAT_ETC_DURATION_TIME",		2 },
		{ L"TAT_ETC_CAMERA",			3 },
		{ L"TAT_ETC_CAMERA_SHAKE",		4 },
		{ L"TAT_ETC_ACTIVE_LAYER",		5 },
	};

	EnumStringTable strPlayType[3] =
	{
		{ L"WOSMPY_ONE",		0 },
		{ L"WOSMPY_ONE_WAIT",	1 },
		{ L"WOSMPY_LOOP",		2 },
	};

	EnumStringTable strDirectionOption[3] =
	{
		{ L"LDO_NONE",	0 },
		{ L"LDO_LEFT",	1 },
		{ L"LDO_RIGHT",	2 },
	};


	if( g_pWorldToolMain == NULL ||
		g_pWorldToolMain->m_pGame == NULL ||
		g_pWorldToolMain->m_pGame->GetWorld() == NULL )
		return FALSE;

	CKTDGLineMap *pLineMap = g_pWorldToolMain->m_pGame->GetWorld()->GetLineMap();
	if( pLineMap == NULL )
		return FALSE;

	int iNumLine = pLineMap->GetNumLineData();
    
	switch(iMessage)
	{
	case WM_CREATE:
	case WM_INITDIALOG:

		for(int i=0; i<11; ++i)
		{
			SendDlgItemMessage(hDlg, LM_IDC_COMBO1, CB_ADDSTRING,      0, (LPARAM)strLineType[i].strName);
			SendDlgItemMessage(hDlg, LM_IDC_COMBO1, CB_SETITEMDATA,    i, (LPARAM)strLineType[i].inx);
		}

		for(int i = 0; i < 3; ++i)
		{
			SendDlgItemMessage(hDlg, LM_IDC_COMBO2, CB_ADDSTRING,      0, (LPARAM)strDirectionOption[i].strName);
			SendDlgItemMessage(hDlg, LM_IDC_COMBO2, CB_SETITEMDATA,    i, (LPARAM)strDirectionOption[i].inx);
		} 
		

		for(int i=0; i<2; ++i)
		{
			SendDlgItemMessage(hDlg, LM_IDC_COMBO3, CB_ADDSTRING,      0, (LPARAM)strDustType[i].strName);
			SendDlgItemMessage(hDlg, LM_IDC_COMBO3, CB_SETITEMDATA,    i, (LPARAM)strDustType[i].inx);
		}        
		
		SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_ADDSTRING,      0, (LPARAM)L"LEFT");		
		SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_ADDSTRING,      0, (LPARAM)L"RIGHT");
		SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_SETCURSEL, (WPARAM)0, 0);

		SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_ADDSTRING,      0, (LPARAM)L"TEAM_NONE");		
		SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_ADDSTRING,      0, (LPARAM)L"TEAM_RED");
		SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_ADDSTRING,      0, (LPARAM)L"TEAM_BLUE");
		SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_SETCURSEL, (WPARAM)0, 0);
		

		WCHAR buf[100];
		wsprintf(buf, L"0.0");

		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT1), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT2), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT3), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT4), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT5), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT6), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT7), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT8), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT9), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT10), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT11), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT12), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT13), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT14), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT15), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT16), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT17), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT18), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT19), buf);

		wsprintf(buf, L"-1");
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERAINDEX), buf);
		SetWindowText(GetDlgItem(hDlg, LM_IDC_EDIT_TRIGGERID), buf);

		char cBuf[100];
		sprintf( cBuf, "%5.2f", pLineMap->GetLandHeight() );
		SetWindowTextA( GetDlgItem(hDlg, LM_IDC_EDIT_LANDHEIGHT), cBuf );

		// 카메라
		for(int i=0; i<3; ++i)
		{
			SendDlgItemMessage(hDlg, LM_IDC_COMBO_CAMERA, CB_ADDSTRING,      0, (LPARAM)strCameraType[i].strName);
			SendDlgItemMessage(hDlg, LM_IDC_COMBO_CAMERA, CB_SETITEMDATA,    i, (LPARAM)strCameraType[i].inx);
		}  
		SendDlgItemMessage(hDlg, LM_IDC_COMBO_CAMERA, CB_SETCURSEL, (WPARAM)0, 0);
		
		wsprintf( buf, L"0 0 0");
		SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA1), buf );
		wsprintf( buf, L"0 0 0");
		SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA2), buf );
		wsprintf( buf, L"0 0 0");
		SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA3), buf );
		wsprintf( buf, L"100");
		SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA4), buf );
		
		SendDlgItemMessage(hDlg, LM_IDC_CHECK_CAMERA, BM_SETCHECK, (WPARAM)true, 0);

		CheckRadioButton(hDlg, LM_IDC_RADIO_RED, LM_IDC_RADIO_OTHER, LM_IDC_RADIO_RED);

		SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER,	BM_SETCHECK, (WPARAM)!g_pWorldToolMain->m_bShowUserStartPos, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC,		BM_SETCHECK, (WPARAM)!g_pWorldToolMain->m_bShowNpcStartPos, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDEWORLD,	BM_SETCHECK, (WPARAM)!g_pWorldToolMain->m_bShowWorld, 0);

		// 트리거
		for(int i=0; i<3; ++i)
		{
			SendDlgItemMessage(hDlg, T_COMBO_MESH_ACTION, CB_ADDSTRING,      0, (LPARAM)strActionMeshType[i].strName);
			SendDlgItemMessage(hDlg, T_COMBO_MESH_ACTION, CB_SETITEMDATA,    i, (LPARAM)strActionMeshType[i].inx);
		}
		for(int i=0; i<7; ++i)
		{
			SendDlgItemMessage(hDlg, T_COMBO_LINE_ACTION, CB_ADDSTRING,      0, (LPARAM)strActionLineType[i].strName);
			SendDlgItemMessage(hDlg, T_COMBO_LINE_ACTION, CB_SETITEMDATA,    i, (LPARAM)strActionLineType[i].inx);
		}
		for(int i=0; i<6; ++i)
		{
			SendDlgItemMessage(hDlg, T_COMBO_ETC_ACTION, CB_ADDSTRING,      0, (LPARAM)strActionEtcType[i].strName);
			SendDlgItemMessage(hDlg, T_COMBO_ETC_ACTION, CB_SETITEMDATA,    i, (LPARAM)strActionEtcType[i].inx);
		}
		for(int i=0; i<3; ++i)
		{
			SendDlgItemMessage(hDlg, T_COMBO_MESH_PLAYTYPE, CB_ADDSTRING,      0, (LPARAM)strPlayType[i].strName);
			SendDlgItemMessage(hDlg, T_COMBO_MESH_PLAYTYPE, CB_SETITEMDATA,    i, (LPARAM)strPlayType[i].inx);
		}

		SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_SETDROPPEDWIDTH,      (WPARAM)300, 0);
		

		UpdateTirggerSkinMeshList(hDlg);

		// 갱신
		UpdateLIneListDlg(GetDlgItem(hDlg, LM_IDC_LIST1), pLineMap);
		UpdateCameraListDlg( hDlg, pLineMap );
		UpdateStartPosDlg(hDlg);
		UpdateTriggerDlg(hDlg, pLineMap);

		g_pWorldToolMain->m_iStartPosListIndex = -1;

		return TRUE;    

	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			// 비활성
			g_pWorldToolMain->m_iStartPosListIndex = -1;
		}
		return TRUE;

	case WM_COMMAND:
		if( HIWORD(wParam) == BN_CLICKED )
		{
			switch(LOWORD(wParam))
			{
			case LM_IDC_RADIO_RED:
				g_pWorldToolMain->iTeamColor = CX2WorldToolMain::TEAM_RED;
				return TRUE;
			case LM_IDC_RADIO_BLUE:
				g_pWorldToolMain->iTeamColor = CX2WorldToolMain::TEAM_BLUE;
				return TRUE;
			case LM_IDC_RADIO_OTHER:
				g_pWorldToolMain->iTeamColor = CX2WorldToolMain::TEAM_NONE;
				return TRUE;

			case LM_IDC_CHECK_HIDE_USER:
				{
					if( SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER, BM_GETCHECK, 0, 0) == false )
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER, BM_SETCHECK, (WPARAM)true, 0);
						g_pWorldToolMain->m_bShowUserStartPos	= true;
					}
					else
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER, BM_SETCHECK, (WPARAM)false, 0);
						g_pWorldToolMain->m_bShowUserStartPos	= false;
					}
				}
				return TRUE;
			case LM_IDC_CHECK_HIDENPC:
				{
					if( SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC, BM_GETCHECK, 0, 0) == false )
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC, BM_SETCHECK, (WPARAM)true, 0);
						g_pWorldToolMain->m_bShowNpcStartPos	= true;
					}
					else
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC, BM_SETCHECK, (WPARAM)false, 0);
						g_pWorldToolMain->m_bShowNpcStartPos	= false;
					}
				}
				return TRUE;
			case LM_IDC_CHECK_HIDEWORLD:
				{
					if( SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDEWORLD, BM_GETCHECK, 0, 0) == false )
					{
						g_pWorldToolMain->m_bShowWorld	= true;
					}
					else
					{
						g_pWorldToolMain->m_bShowWorld	= false;
					}
				}
				return TRUE;

			}
		}
		
		switch(LOWORD(wParam))
		{		
		case IDC_WONPOK:
			WinExec( "Nub.Tmp", SW_NORMAL );
			return TRUE;
		case LM_IDOK:
			{
				ModifyLineData(hDlg, pLineMap);
				int index = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETCURSEL, 0, 0);
				UpdateLineModifierDlg( hDlg, pLineMap, index);
			}
			return TRUE; 
		case LM_IDC_BUTTON_SAVE:
			{
				// 라인맵 파일에 저장		
				OPENFILENAME ofn;        // common dialog box structure
				WCHAR wszFileName[512];  // path까지 포함한 파일 이름

				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize		= sizeof(ofn);
				ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
				ofn.lpstrFile		= (LPWSTR)wszFileName;
				ofn.lpstrFile[0]	= '\0';
				ofn.nMaxFile		= sizeof(wszFileName);
				ofn.lpstrFilter		= L"lua script\0*.lua\0X file\0*.x\0";
				ofn.nFilterIndex	= 1;
				ofn.lpstrFileTitle	= NULL;
				ofn.nMaxFileTitle	= 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				if( TRUE == GetSaveFileName( &ofn ) )
				{
					g_pWorldToolMain->SaveLineMapToFile(ofn.lpstrFile, true);
				}				
			}
			return TRUE;
		case LM_IDCANCEL:
			{
				// 라인맵 텍스트로 뷰
				g_pWorldToolMain->SaveLineMapToText();
			}
			return TRUE;
		case LM_IDC_BUTTON2:
			{
				char buf[100];
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT20), buf, 100);
				SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT17), buf);
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT21), buf, 100);
				SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT18), buf);
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT22), buf, 100);
				SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT19), buf);
			}
			return TRUE;

		case LM_IDC_BUTTON_GETCAMERA:
			{
				//const D3DXVECTOR3 *vEye = g_pWorldToolMain->GetCamera().GetEyePt();
				//const D3DXVECTOR3 *vLookAt = g_pWorldToolMain->GetCamera().GetLookAtPt();

				const D3DXVECTOR3 vEye = g_pWorldToolMain->m_pGame->GetX2Camera()->GetCamera().GetEye();
				const D3DXVECTOR3 vLookAt = g_pWorldToolMain->m_pGame->GetX2Camera()->GetCamera().GetLookAt();

				char buf[100];
				sprintf( buf, "%5.2f %5.2f %5.2f", vEye.x, vEye.y, vEye.z );
				SetWindowTextA( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA1), buf );
				sprintf( buf, "%5.2f %5.2f %5.2f", vLookAt.x, vLookAt.y, vLookAt.z );
				SetWindowTextA( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA2), buf );
			}
			return TRUE;
		case LM_IDC_BUTTON_ADDCAMERA:
			{
				CKTDGLineMap::CameraData *pCamera = new CKTDGLineMap::CameraData();
				if( pCamera != NULL )
				{
					LRESULT lResult = SendDlgItemMessage( hDlg, LM_IDC_CHECK_CAMERA, BM_GETCHECK, 0, 0 );
					if ( 0 == lResult )
						pCamera->m_bFocusUnit = false;
					else
						pCamera->m_bFocusUnit = true;

					char buf[101];
					GetWindowTextA( GetDlgItem( hDlg, LM_IDC_EDIT_CAMERA4 ), buf, 100 );
					pCamera->m_fCameraRepositionSpeed = static_cast<float>( atof( buf ) );

					D3DXVECTOR3 vEye = pCamera->m_vEye;
					D3DXVECTOR3 vLookat = pCamera->m_vLookAt;
					D3DXVECTOR3 vRelativeEye = pCamera->m_vRelativeEye;
					GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA1), buf, 100);
					sscanf(buf, "%f%f%f", &vEye.x, &vEye.y, &vEye.z);
					GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA2), buf, 100);
					sscanf(buf, "%f%f%f", &vLookat.x, &vLookat.y, &vLookat.z);
					GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA3), buf, 100);
					sscanf(buf, "%f%f%f", &vRelativeEye.x, &vRelativeEye.y, &vRelativeEye.z);
					pCamera->m_vEye = vEye;
					pCamera->m_vLookAt = vLookat;
					pCamera->m_vRelativeEye = vRelativeEye;

					if( vRelativeEye.x == 0.f && vRelativeEye.y == 0.f && vRelativeEye.z == 0.f )
					{
						pCamera->m_eType = CKTDGLineMap::LCT_ABSOULTE_POSITION;						
					}
					else
					{
						pCamera->m_eType = CKTDGLineMap::LCT_RELATIE_POSITION;
					}
					SendDlgItemMessage(hDlg, LM_IDC_COMBO_CAMERA, CB_SETCURSEL, (WPARAM)pCamera->m_eType, 0);

					pLineMap->AddCameraData( *pCamera );
				}

				UpdateCameraListDlg( hDlg, pLineMap );
			}
			return TRUE;
		case LM_IDC_BUTTON_ADDCAMERA2:
			{
				SendDlgItemMessage(hDlg, LM_IDC_COMBO_CAMERA, CB_SETCURSEL, (WPARAM)0, 0);

				wsprintf( buf, L"0 0 0");
				SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA1), buf );
				wsprintf( buf, L"0 0 0");
				SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA2), buf );
				wsprintf( buf, L"0 0 0");
				SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA3), buf );
				wsprintf( buf, L"100");
				SetWindowText( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA4), buf );

				SendDlgItemMessage(hDlg, LM_IDC_CHECK_CAMERA, BM_SETCHECK, (WPARAM)true, 0);
			}
			return TRUE;
		case LM_IDC_BUTTON_CHANGE:
			{
				ModifyStartPos(hDlg, g_pWorldToolMain->m_iStartPosListIndex);
				UpdateStartPosDlg(hDlg);
				g_pWorldToolMain->m_iStartPosListIndex = -1;
			}
			return TRUE;
		case LM_IDC_BUTTON_RESETKEY:
			{				
				UpdateStartPosDlg(hDlg);
				g_pWorldToolMain->m_iStartPosListIndex = -1;
			}
			return TRUE;
		case LM_IDC_BUTTON_DELETENPC:
			{
				g_pWorldToolMain->DeleteNpcStartPos();
				UpdateStartPosDlg(hDlg);
				g_pWorldToolMain->m_iStartPosListIndex = -1;
			}
			return TRUE;

		case T_TRIGGER_ADD:
			{
				CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
				if( pTrigger != NULL )
				{
					CKTDGLineMap::TRIGGER_SEGMENT pTriggerSeg;
					pTrigger->m_vecTriggerSeg.push_back(pTriggerSeg);

					int index = pTrigger->m_vecTriggerSeg.size() - 1;
					UpdateTriggerDlg(hDlg, pLineMap);
					SendDlgItemMessage(hDlg, T_LIST_SEG, LB_SETCURSEL, index, 0);
				}
			}
			return TRUE;
		case T_TRIGGER_ADD_DATA:
			{
				int index = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
				if( index >= 0 )
				{
					CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
					CKTDGLineMap::TRIGGER_SEGMENT &pTriggerSeg = pTrigger->m_vecTriggerSeg[index];
					CKTDGLineMap::TRIGGER_DATA pTriggerData;
					pTriggerSeg.m_vecTriggerData.push_back(pTriggerData);
					int indexData = pTriggerSeg.m_vecTriggerData.size()-1;
					UpdateTriggerSegDlg(hDlg, pTriggerSeg);
					SendDlgItemMessage(hDlg, T_LIST_DATA, LB_SETCURSEL, indexData, 0);
				}				
			}
			return TRUE;
		case T_TRIGGER_ADD_MESH:
			{
				int index = AddTriggerMesh(hDlg, pLineMap, -1);
				SendDlgItemMessage(hDlg, T_LIST_MESH, LB_SETCURSEL, index, 0);
			}
			return TRUE;
		case T_UPDATE_MESH:
			{
				int index = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_GETCURSEL, 0, 0);
				if( index >= 0 )
					AddTriggerMesh(hDlg, pLineMap, index);
				SendDlgItemMessage(hDlg, T_LIST_MESH, LB_SETCURSEL, index, 0);
			}
			return TRUE;		
		case T_TRIGGER_ADD_LINE:
			{
				int index = AddTriggerLine(hDlg, pLineMap, -1);
				SendDlgItemMessage(hDlg, T_LIST_LINE, LB_SETCURSEL, index, 0);
			}
			return TRUE;
		case T_UPDATE_LINE:
			{
				int index = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_GETCURSEL, 0, 0);
				if( index >= 0 )
					AddTriggerLine(hDlg, pLineMap, index);
				SendDlgItemMessage(hDlg, T_LIST_LINE, LB_SETCURSEL, index, 0);
			}
			return TRUE;
		case T_TRIGGER_ADD_ETC:
			{
				int index = AddTriggerEtc(hDlg, pLineMap, -1);
				SendDlgItemMessage(hDlg, T_LIST_ETC, LB_SETCURSEL, index, 0);
			}
			return TRUE;
		case T_UPDATE_ETC:
			{
				int index = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_GETCURSEL, 0, 0);
				if( index >= 0 )
					AddTriggerEtc(hDlg, pLineMap, index);
				SendDlgItemMessage(hDlg, T_LIST_ETC, LB_SETCURSEL, index, 0);
			}
			return TRUE;
		case T_UPDATE_TRIGGER:
			{
				UpdateTriggerData(hDlg, pLineMap);
				int indexMesh = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_GETCURSEL, 0, 0);
				if( indexMesh >= 0 )
					AddTriggerMesh(hDlg, pLineMap, indexMesh);
				SendDlgItemMessage(hDlg, T_LIST_MESH, LB_SETCURSEL, indexMesh, 0);
				int indexLine = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_GETCURSEL, 0, 0);
				if( indexLine >= 0 )
					AddTriggerLine(hDlg, pLineMap, indexLine);
				SendDlgItemMessage(hDlg, T_LIST_LINE, LB_SETCURSEL, indexLine, 0);
				int indexEtc = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_GETCURSEL, 0, 0);
				if( indexEtc >= 0 )
					AddTriggerEtc(hDlg, pLineMap, indexEtc);
				SendDlgItemMessage(hDlg, T_LIST_ETC, LB_SETCURSEL, indexEtc, 0);
			}
			return TRUE;
		case T_BUTTON_INITWORLD:
			{
				for(UINT i=0; i<g_pWorldToolMain->m_vecpWorldToolObject.size(); ++i)
				{
					CWorldToolObject* pWorldToolObject = g_pWorldToolMain->m_vecpWorldToolObject[i];
					if( pWorldToolObject == NULL || pWorldToolObject->m_pWorldObject == NULL )
						continue;

					if( pWorldToolObject->m_pWorldObject->GetObjectType() == CX2WorldObject::OT_SKIN_MESH )
					{
						CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) pWorldToolObject->m_pWorldObject;
						pSkinMesh->ChangePlayAnim_LUA( pSkinMesh->m_wstrOriAnimName, (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE)pSkinMesh->m_iOriPlayType );
					}
				}				
			}
			return TRUE;

		case LM_IDC_COMBO_DIR:
			switch(HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				{
					int index = SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_GETCURSEL, 0, 0);
					if(index != -1)
					{   
						if( g_pWorldToolMain != NULL )
							g_pWorldToolMain->iInitialDir = index;
					}
				}                               
			}
			return TRUE;
		case LM_IDC_LIST1:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int m_iSelectedList[100];
					int iSelectedCount = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETSELCOUNT, 0, 0);
					
					for(int iSel = 0; iSel<100; ++iSel)
						m_iSelectedList[iSel] = -1;	
					SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETSELITEMS, 100, (LPARAM)m_iSelectedList);

					int indexNow = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETCURSEL, 0, 0);
					int index = m_iSelectedList[iSelectedCount-1];

					for(int i=0; i<iSelectedCount; ++i)
					{
						if( m_iSelectedList[i] == indexNow )
						{
							index = indexNow;
							break;
						}
					}
					
					UpdateLineModifierDlg(hDlg, pLineMap, index);
				}
				return TRUE;
			}
			return TRUE;
		case LM_IDC_LIST_CAMERA:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int index = SendDlgItemMessage(hDlg, LM_IDC_LIST_CAMERA, LB_GETCURSEL, 0, 0);
					UpdateCameraModifierDlg(hDlg, pLineMap, index);
				}
				return TRUE;
			}
			return TRUE;
		case LM_IDC_LIST_RED:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int index = SendDlgItemMessage(hDlg, LM_IDC_LIST_RED, LB_GETCURSEL, 0, 0);
					if( index >= 0 )
					{
						g_pWorldToolMain->m_iStartPosListIndex = GetStartPosIndex(1, index);
						UpdateStartPosInfo( hDlg, g_pWorldToolMain->m_iStartPosListIndex );					
						SendDlgItemMessage(hDlg, LM_IDC_LIST_BLUE, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
						SendDlgItemMessage(hDlg, LM_IDC_LIST_OTHER, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
					}					
				}
				return TRUE;
			}
			return TRUE;
		case LM_IDC_LIST_BLUE:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int index = SendDlgItemMessage(hDlg, LM_IDC_LIST_BLUE, LB_GETCURSEL, 0, 0);
					if( index >= 0 )
					{
						g_pWorldToolMain->m_iStartPosListIndex = GetStartPosIndex(2, index);
						UpdateStartPosInfo( hDlg, g_pWorldToolMain->m_iStartPosListIndex );
						SendDlgItemMessage(hDlg, LM_IDC_LIST_RED, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
						SendDlgItemMessage(hDlg, LM_IDC_LIST_OTHER, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
					}					
				}
				return TRUE;
			}
			return TRUE;
		case LM_IDC_LIST_OTHER:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int index = SendDlgItemMessage(hDlg, LM_IDC_LIST_OTHER, LB_GETCURSEL, 0, 0);
					if( index >= 0 )
					{
						g_pWorldToolMain->m_iStartPosListIndex = GetStartPosIndex(0, index);
						UpdateStartPosInfo( hDlg, g_pWorldToolMain->m_iStartPosListIndex );
						SendDlgItemMessage(hDlg, LM_IDC_LIST_BLUE, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
						SendDlgItemMessage(hDlg, LM_IDC_LIST_RED, LB_SETCURSEL, (WPARAM)-1, (LPARAM)0);
					}					
				}
				return TRUE;
			}
			return TRUE;

		case T_LIST_SEG:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int index = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					if( index >= 0 )
					{						
						UpdateTriggerSegDlg(hDlg, pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[index]);						
					}					
				}
				return TRUE;
			}
			return TRUE;
		case T_LIST_DATA:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 )
					{						
						UpdateTriggerDataDlg(hDlg, pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg], indexData);
					}					
				}
				return TRUE;
			case LBN_DBLCLK:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 )
					{
						CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];

						triggerSeg.m_iTriggerDataIndex = indexData;
						g_pX2Game->GetWorld()->InitTriggerDone(triggerData);						
						g_pX2Game->GetWorld()->ActiveTrigger(indexSeg);
					}
				}
				return TRUE;
			}
			return TRUE;
		case T_LIST_MESH:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexMesh =  SendDlgItemMessage(hDlg, T_LIST_MESH, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexMesh >= 0 )
					{						
						CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];
						CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh = triggerData.m_vecTriggerMesh[indexMesh];
						UpdateTriggerMeshDlg(hDlg, triggerMesh);
					}					
				}
				return TRUE;
			case LBN_DBLCLK:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexMesh = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexMesh >= 0 )
					{
						CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];
						CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh = triggerData.m_vecTriggerMesh[indexMesh];
                        
						triggerSeg.m_iTriggerDataIndex = indexData;
						g_pX2Game->GetWorld()->InitTriggerDone(triggerData, true);
						triggerMesh.m_bDone = false;
						g_pX2Game->GetWorld()->ActiveTrigger(indexSeg);
					}
				}
				return TRUE;
			}
			return TRUE;
		case T_LIST_LINE:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexLine = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexLine >= 0 )
					{						
						CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];
						CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine = triggerData.m_vecTriggerLine[indexLine];
						UpdateTriggerLineDlg(hDlg, triggerLine);
					}					
				}
				return TRUE;
			case LBN_DBLCLK:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexLine = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexLine >= 0 )
					{
						CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];
						CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine = triggerData.m_vecTriggerLine[indexLine];

						triggerSeg.m_iTriggerDataIndex = indexData;
						g_pX2Game->GetWorld()->InitTriggerDone(triggerData, true);
						triggerLine.m_bDone = false;
						g_pX2Game->GetWorld()->ActiveTrigger(indexSeg);
					}
				}
				return TRUE;
			}
			return TRUE;
		case T_LIST_ETC:
			switch(HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexEtc = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexEtc >= 0 )
					{						
						CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];
						CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc = triggerData.m_vecTriggerEtc[indexEtc];
						UpdateTriggerEtcDlg(hDlg, triggerEtc);
					}					
				}
				return TRUE;
			case LBN_DBLCLK:
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexEtc = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexEtc >= 0 )
					{
						CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pLineMap->GetLineMapTrigger()->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];
						CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc = triggerData.m_vecTriggerEtc[indexEtc];

						triggerSeg.m_iTriggerDataIndex = indexData;
						g_pX2Game->GetWorld()->InitTriggerDone(triggerData, true);
						triggerEtc.m_bDone = false;
						g_pX2Game->GetWorld()->ActiveTrigger(indexSeg);
					}
				}
				return TRUE;
			}
			return TRUE;
		case T_COMBO_MESH_ID:
			switch(HIWORD(wParam))
			{
			case CBN_SELCHANGE:
				{
					int index = SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_GETCURSEL, 0, 0);
					if(index != -1)
					{   
						WCHAR wBuf[100];
						SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_GETLBTEXT, (WPARAM)index, (LPARAM)wBuf);
						UpdateTirggerSkinMeshAnimList(hDlg, wBuf);
					}
				}                               
			}
			return TRUE;
			
		}
		return TRUE;

	case WM_VKEYTOITEM:
	//case WM_KEYDOWN:
		{
			if( LOWORD(wParam) == VK_DELETE && g_pWorldToolMain->m_iStartPosListIndex >= 0 )
			{
				HWND hWnd = GetFocus();
				if( hWnd == GetDlgItem(hDlg, LM_IDC_LIST_RED) ||
					hWnd == GetDlgItem(hDlg, LM_IDC_LIST_BLUE) ||
					hWnd == GetDlgItem(hDlg, LM_IDC_LIST_OTHER) )
				{
					g_pWorldToolMain->vecTeamStartPos.erase(g_pWorldToolMain->vecTeamStartPos.begin() + g_pWorldToolMain->m_iStartPosListIndex);
					UpdateStartPosDlg( hDlg );
					return (BOOL)-2;
				}				
			}

			if( LOWORD(wParam) == VK_DELETE )
			{
				HWND hWnd = GetFocus();
				if( hWnd == GetDlgItem(hDlg, T_LIST_SEG) )					
				{
					int index = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					if( index >= 0 )
					{
						CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
						pTrigger->m_vecTriggerSeg.erase(pTrigger->m_vecTriggerSeg.begin() + index);
						UpdateTriggerDlg(hDlg, pLineMap);						
						return (BOOL)-2;
					}					
				}	
				else if( hWnd == GetDlgItem(hDlg, T_LIST_DATA) )
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 )
					{
						CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
						CKTDGLineMap::TRIGGER_SEGMENT &pTriggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];
						pTriggerSeg.m_vecTriggerData.erase( pTriggerSeg.m_vecTriggerData.begin() + indexData );						
						UpdateTriggerSegDlg(hDlg, pTriggerSeg);						
						return (BOOL)-2;
					}	
				}
				else if( hWnd == GetDlgItem(hDlg, T_LIST_MESH) )
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexMesh = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_GETCURSEL, 0, 0);
					int indexLine = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_GETCURSEL, 0, 0);
					int indexEtc = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexMesh >= 0 )
					{
						CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
						CKTDGLineMap::TRIGGER_SEGMENT &pTriggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &pTriggerData = pTriggerSeg.m_vecTriggerData[indexData];
						pTriggerData.m_vecTriggerMesh.erase(pTriggerData.m_vecTriggerMesh.begin() + indexMesh);						
						UpdateTriggerDataDlg(hDlg, pTriggerSeg, indexData);
						CKTDGLineMap::TRIGGER_DATA_MESH triggerMesh;
						UpdateTriggerMeshDlg(hDlg, triggerMesh);
						
						if( indexLine >= 0 )
						{
							SendDlgItemMessage(hDlg, T_LIST_LINE, LB_SETCURSEL, indexLine, 0);
						}
						if( indexEtc >= 0)
						{
							SendDlgItemMessage(hDlg, T_LIST_ETC, LB_SETCURSEL, indexEtc, 0);
						}

						return (BOOL)-2;
					}	
				}
				else if( hWnd == GetDlgItem(hDlg, T_LIST_LINE) )
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexMesh = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_GETCURSEL, 0, 0);
					int indexLine = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_GETCURSEL, 0, 0);
					int indexEtc = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexLine >= 0 )
					{
						CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
						CKTDGLineMap::TRIGGER_SEGMENT &pTriggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &pTriggerData = pTriggerSeg.m_vecTriggerData[indexData];
						pTriggerData.m_vecTriggerLine.erase(pTriggerData.m_vecTriggerLine.begin() + indexLine);
						UpdateTriggerDataDlg(hDlg, pTriggerSeg, indexData);
						CKTDGLineMap::TRIGGER_DATA_LINE triggerLine;
						UpdateTriggerLineDlg(hDlg, triggerLine);

						if( indexMesh >= 0 )
						{
							SendDlgItemMessage(hDlg, T_LIST_MESH, LB_SETCURSEL, indexMesh, 0);
						}
						if( indexEtc >= 0)
						{
							SendDlgItemMessage(hDlg, T_LIST_ETC, LB_SETCURSEL, indexEtc, 0);
						}

						return (BOOL)-2;
					}	
				}
				else if( hWnd == GetDlgItem(hDlg, T_LIST_ETC) )
				{
					int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
					int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);
					int indexMesh = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_GETCURSEL, 0, 0);
					int indexLine = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_GETCURSEL, 0, 0);
					int indexEtc = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_GETCURSEL, 0, 0);
					if( indexSeg >= 0 && indexData >= 0 && indexEtc >= 0 )
					{
						CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
						CKTDGLineMap::TRIGGER_SEGMENT &pTriggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];
						CKTDGLineMap::TRIGGER_DATA &pTriggerData = pTriggerSeg.m_vecTriggerData[indexData];
						pTriggerData.m_vecTriggerEtc.erase(pTriggerData.m_vecTriggerEtc.begin() + indexEtc);
						UpdateTriggerDataDlg(hDlg, pTriggerSeg, indexData);
						CKTDGLineMap::TRIGGER_DATA_ETC triggerEtc;
						UpdateTriggerEtcDlg(hDlg, triggerEtc);

						if( indexMesh >= 0 )
						{
							SendDlgItemMessage(hDlg, T_LIST_MESH, LB_SETCURSEL, indexMesh, 0);
						}
						if( indexEtc >= 0)
						{
							SendDlgItemMessage(hDlg, T_LIST_ETC, LB_SETCURSEL, indexEtc, 0);
						}

						return (BOOL)-2;
					}	
				}
			}
			return (BOOL)-1;
		}
		break;

	case WM_CLOSE:
	case WM_DESTROY:		
		DestroyWindow(g_hDlgLineMap);		
		g_hDlgLineMap = NULL;
		g_pWorldToolMain->m_bShowLineMapModifier = false;
		g_pWorldToolMain->m_bShowWorld = true;

		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineDataInit = pLineMap->GetLineData(i);
			if( pLineDataInit != NULL )
				pLineDataInit->m_bSelected = false;
		}

		return TRUE;
	}
	return FALSE;
}

void UpdateCameraListDlg(HWND hDlg, CKTDGLineMap *pLineMap)
{
	int nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_CAMERA, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_CAMERA, LB_DELETESTRING, 0, 0);
	}

	// 카메라 정보 갱신
	for(int i=0; i<pLineMap->GetNumCameraData(); ++i)
	{
		CKTDGLineMap::CameraData *pCameraData = pLineMap->GetCameraDataListByTool(i);
		if( pCameraData != NULL )
		{
			WCHAR wstr[100];
			wsprintf(wstr, L"%d", i);
			SendDlgItemMessage( hDlg, LM_IDC_LIST_CAMERA, LB_ADDSTRING, 0, (LPARAM)wstr );
		}
	}
}

void UpdateLIneListDlg(HWND hDlg, CKTDGLineMap *pLineMap)
{
	int nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_DELETESTRING, 0, 0);
	}

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
		if( pLineData != NULL )
		{
			WCHAR wstr[100];
			wsprintf(wstr, L"%d", pLineData->lineIndex);
			SendMessage( hDlg, LB_ADDSTRING, 0, (LPARAM)wstr );
		}
	}
}

void UpdateCameraModifierDlg(HWND hDlg, CKTDGLineMap *pLineMap, int iIndex)
{
	CKTDGLineMap::CameraData *pCameraData = pLineMap->GetCameraDataListByTool(iIndex);

	if( pCameraData == NULL )
		return;

	SendDlgItemMessage(hDlg, LM_IDC_COMBO_CAMERA, CB_SETCURSEL, (WPARAM)pCameraData->m_eType, 0);

	char buf[100];
	sprintf( buf, "%5.2f %5.2f %5.2f", pCameraData->m_vEye.x, pCameraData->m_vEye.y, pCameraData->m_vEye.z );
	SetWindowTextA( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA1), buf );
	sprintf( buf, "%5.2f %5.2f %5.2f", pCameraData->m_vLookAt.x, pCameraData->m_vLookAt.y, pCameraData->m_vLookAt.z );
	SetWindowTextA( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA2), buf );
	sprintf( buf, "%5.2f %5.2f %5.2f", pCameraData->m_vRelativeEye.x, pCameraData->m_vRelativeEye.y, pCameraData->m_vRelativeEye.z );
	SetWindowTextA( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA3), buf );
	sprintf( buf, "%5.2f", pCameraData->m_fCameraRepositionSpeed );
	SetWindowTextA( GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA4), buf );

	SendDlgItemMessage(hDlg, LM_IDC_CHECK_CAMERA, BM_SETCHECK, (WPARAM)pCameraData->m_bFocusUnit, 0);
}

void UpdateLineModifierDlg(HWND hDlg, CKTDGLineMap *pLineMap, int iIndex)
{
	CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(iIndex);

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineDataInit = pLineMap->GetLineData(i);
		if( pLineDataInit != NULL )
			pLineDataInit->m_bSelected = false;
	}
	
	int m_iSelectedList[100];
	int iSelectedCount = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETSELCOUNT, 0, 0);
	SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETSELITEMS, 100, (LPARAM)m_iSelectedList);
	for(int i=0; i<iSelectedCount; ++i)
	{
		if( m_iSelectedList[i] >= 0 )
		{
			CKTDGLineMap::LineData *pLineDataInit = pLineMap->GetLineData(m_iSelectedList[i]);
			if( pLineDataInit != NULL )
				pLineDataInit->m_bSelected = true;
		}		
	}

	if( pLineData != NULL )
	{
		//pLineData->m_bSelected = true;

		SendDlgItemMessage(hDlg, LM_IDC_COMBO1, CB_SETCURSEL, (WPARAM)pLineData->lineType, 0);
		SendDlgItemMessage(hDlg, LM_IDC_COMBO2, CB_SETCURSEL, (WPARAM)pLineData->m_eDirection, 0);
		SendDlgItemMessage(hDlg, LM_IDC_COMBO3, CB_SETCURSEL, (WPARAM)pLineData->eDustType, 0);
		
		//SendDlgItemMessage(hDlg, LM_IDC_CHECK1, BM_SETCHECK, (WPARAM)pLineData->bEnable, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK1, BM_SETCHECK, (WPARAM)pLineData->bOriEnable, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK2, BM_SETCHECK, (WPARAM)pLineData->bCanDown, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK3, BM_SETCHECK, (WPARAM)pLineData->m_bEnableWallJump, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK4, BM_SETCHECK, (WPARAM)pLineData->m_bMonsterAllowed, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK5, BM_SETCHECK, (WPARAM)pLineData->m_bOriCanPass, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK6, BM_SETCHECK, (WPARAM)pLineData->m_bOtherRoadUp, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK7, BM_SETCHECK, (WPARAM)pLineData->m_bUnderWater, 0);		
		SendDlgItemMessage(hDlg, LM_IDC_CHECK_FORCE_COLOR, BM_SETCHECK, (WPARAM)pLineData->m_bForceChangeColor, 0);		

		SendDlgItemMessage(hDlg, LM_IDC_CHECK_ONLINE, BM_SETCHECK, (WPARAM)pLineData->m_bFootOnLine, 0);		

		char buf[100];
		sprintf(buf, "%d", pLineData->potalGroup);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT1), buf);

		sprintf(buf, "%6.3f", pLineData->fStopFactor);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT2), buf);
		
		sprintf(buf, "%d", pLineData->beforeLine);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT3), buf);

		sprintf(buf, "%d", pLineData->nextLine);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT4), buf);

		sprintf(buf, "%6.3f", pLineData->m_fKeyStopTime );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT5), buf);

		sprintf(buf, "%d", pLineData->lineSet);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT6), buf);

		sprintf(buf, "%6.3f", pLineData->m_fOriSpeed);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT7), buf);

		sprintf(buf, "%6.3f", pLineData->m_vOriJumpSpeed.x );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT8), buf);

		sprintf(buf, "%6.3f", pLineData->m_vOriJumpSpeed.y );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT9), buf);

		sprintf(buf, "%d", pLineData->m_iOriTeleportLineIndex );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT10), buf);

		sprintf(buf, "%6.3f", pLineData->m_fCliffHeight );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT11), buf);

		sprintf(buf, "%d", pLineData->m_iOriCameraIndex );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERAINDEX), buf);

		sprintf(buf, "%d", pLineData->m_iTriggerId );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_TRIGGERID), buf);		
		

		sprintf(buf, "%6.3f", pLineData->m_fWaterHeight );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT12), buf);

		sprintf(buf, "%6.3f", pLineData->m_fBuoyancy );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT13), buf);

		sprintf(buf, "%6.3f", pLineData->m_fResistance );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT14), buf);

		sprintf(buf, "%6.3f", pLineData->m_fAnimSpeed );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT15), buf);

		sprintf(buf, "%6.3f", pLineData->m_fVarianceOxyen );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT16), buf);

		sprintf(buf, "%6.3f", pLineData->m_cLineColor.r );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT17), buf);
		sprintf(buf, "%6.3f", pLineData->m_cLineColor.g );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT18), buf);
		sprintf(buf, "%6.3f", pLineData->m_cLineColor.b );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT19), buf);

		D3DXCOLOR cColor = CalcUnderWaterUnitColor(pLineMap, pLineData, 0, NULL);

		sprintf(buf, "%6.3f", cColor.r );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT20), buf);
		sprintf(buf, "%6.3f", cColor.g );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT21), buf);
		sprintf(buf, "%6.3f", cColor.b );
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT22), buf);

		D3DXVECTOR3 vStartPos = pLineData->startPos;
		D3DXVECTOR3 vEndPos = pLineData->endPos;
		sprintf(buf, "%5.2f %5.2f %5.2f", vStartPos.x, vStartPos.y, vStartPos.z);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_START), buf);
		sprintf(buf, "%5.2f %5.2f %5.2f", vEndPos.x, vEndPos.y, vEndPos.z);
		SetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_END), buf);
	}
}

void UpdateStartPosDlg(HWND hDlg)
{
	int nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_RED, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_RED, LB_DELETESTRING, 0, 0);
	}

	nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_BLUE, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_BLUE, LB_DELETESTRING, 0, 0);
	}

	nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_OTHER, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, LM_IDC_LIST_OTHER, LB_DELETESTRING, 0, 0);
	}

	g_pWorldToolMain->ResetNpcStartPosKey();

	WCHAR wstr[100];
	WCHAR wstrRight[10];
	int iRed, iBlue;
	iRed = iBlue = -1;

	for(UINT i=0; i < g_pWorldToolMain->vecTeamStartPos.size(); ++i)
	{
		CX2WorldToolMain::STeamStartPos& spos = g_pWorldToolMain->vecTeamStartPos[i];
		wsprintf( wstrRight, L"(Left)" );
		if( spos.iInitialDir == CX2WorldToolMain::DIR_RIGHT )
		{
			wsprintf( wstrRight, L"(Right)" );
		}

		switch( spos.iTeamColor )
		{
		case CX2WorldToolMain::TEAM_NONE:
			{
				wsprintf(wstr, L"%d %s", spos.key, wstrRight);
				SendMessage( GetDlgItem(hDlg, LM_IDC_LIST_OTHER), LB_ADDSTRING, 0, (LPARAM)wstr );
			}
			break;
		case CX2WorldToolMain::TEAM_RED:
			{
				++iRed;
				wsprintf(wstr, L"%d %s", iRed, wstrRight);
				SendMessage( GetDlgItem(hDlg, LM_IDC_LIST_RED), LB_ADDSTRING, 0, (LPARAM)wstr );
			}
			break;
		case CX2WorldToolMain::TEAM_BLUE:
			{
				++iBlue;
				wsprintf(wstr, L"%d %s", iBlue, wstrRight);
				SendMessage( GetDlgItem(hDlg, LM_IDC_LIST_BLUE), LB_ADDSTRING, 0, (LPARAM)wstr );
			}
			break;
		}
	}
}

int	GetStartPosIndex(int iTeam, int iIndex)
{	
	int iRed, iBlue, iNpc;
	iRed = iBlue = iNpc = -1;

	for(UINT i=0; i<g_pWorldToolMain->vecTeamStartPos.size(); ++i)
	{
		switch(g_pWorldToolMain->vecTeamStartPos[i].iTeamColor)
		{
		case 0: // npc
			++iNpc;
			break;
		case 1: // red
			++iRed;
			break;
		case 2: // blue
			++iBlue;
			break;
		}
		if( g_pWorldToolMain->vecTeamStartPos[i].iTeamColor == iTeam )
		{
			if( iTeam == 0 && iIndex == iNpc )
			{
				return i;
			}
			else if( iTeam == 1 && iIndex == iRed )
			{
				return i;
			}
			else if( iTeam == 2 && iIndex == iBlue )
			{
				return i;
			}
		}
	}

	return -1;
}

void UpdateStartPosInfo(HWND hDlg, int index)
{	
	if( index < 0 )
		return;
	if( index >= (int)g_pWorldToolMain->vecTeamStartPos.size() )
		return;

	CX2WorldToolMain::STeamStartPos teamInfo =  g_pWorldToolMain->vecTeamStartPos[index];


	SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_SETCURSEL, (WPARAM)teamInfo.iTeamColor, 0);
	SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_SETCURSEL, (WPARAM)teamInfo.iInitialDir, 0);
}

void ModifyStartPos(HWND hDlg, int index)
{
	if( index < 0 || index > (int)g_pWorldToolMain->vecTeamStartPos.size() )
		return;

	int iTeam = SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_GETCURSEL, 0, 0);
	if( iTeam == 0 )
	{
		int key = g_pWorldToolMain->GetNextUniqueNPCStartPosKey();
		if( key == -1 )
			return;
		g_pWorldToolMain->vecTeamStartPos.at(index).key = key;
	}

	g_pWorldToolMain->vecTeamStartPos.at(index).iTeamColor = iTeam;
	g_pWorldToolMain->vecTeamStartPos.at(index).iInitialDir = SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_GETCURSEL, 0, 0);
}

void ModifyLineData(HWND hDlg, CKTDGLineMap *pLineMap)
{
	if( pLineMap == NULL )
		return;
	char buf[101];
	int m_iSelectedList[100];
	int iSelectedCount = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETSELCOUNT, 0, 0);
	bool iSelectedMulti = false;
	if( iSelectedCount > 1 )
		iSelectedMulti = true;
	
	for(int iSel = 0; iSel<100; ++iSel)
		m_iSelectedList[iSel] = -1;	
	SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETSELITEMS, 100, (LPARAM)m_iSelectedList);
	//int index = SendDlgItemMessage(hDlg, LM_IDC_LIST1, LB_GETCURSEL, 0, 0);	

	for(int iSel = 0; iSel < iSelectedCount; ++iSel )
	{
		int index = m_iSelectedList[iSel];
		if( index < 0 )
			continue;

		CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(index);
		if( pLineData != NULL )
		{
			if( iSelectedMulti == false )
			{
				pLineData->lineType = (CKTDGLineMap::LINE_TYPE)SendDlgItemMessage(hDlg, LM_IDC_COMBO1, CB_GETCURSEL, 0, 0);
				pLineData->m_eDirection = (CKTDGLineMap::LINEMAP_DIRECTION_OPTION)SendDlgItemMessage( hDlg, LM_IDC_COMBO2, CB_GETCURSEL, 0, 0 );
				pLineData->eDustType = (CKTDGLineMap::LINE_DUST_TYPE)SendDlgItemMessage(hDlg, LM_IDC_COMBO3, CB_GETCURSEL, 0, 0);

				LRESULT lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK1, BM_GETCHECK, 0, 0);
				if ( 0 == lResult )
					pLineData->bOriEnable = false;
				else
					pLineData->bOriEnable = true;

				pLineData->bEnable = pLineData->bOriEnable;

				lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK2, BM_GETCHECK, 0, 0);
				if ( 0 == lResult )
					pLineData->bCanDown = false;
				else
					pLineData->bCanDown = false;

				lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK3, BM_GETCHECK, 0, 0);
				if ( 0 == lResult )
					pLineData->m_bEnableWallJump = false;
				else
					pLineData->m_bEnableWallJump = true;

				lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK4, BM_GETCHECK, 0, 0);
				if ( 0 == lResult )
					pLineData->m_bMonsterAllowed = false;
				else
					pLineData->m_bMonsterAllowed = true;

				lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK5, BM_GETCHECK, 0, 0);
				if ( 0 == lResult )
					pLineData->m_bOriCanPass = false;
				else
					pLineData->m_bOriCanPass = true;
				
				pLineData->m_bCanPass = pLineData->m_bOriCanPass;

				lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK6, BM_GETCHECK, 0, 0);
				if ( 0 == lResult )
					pLineData->m_bOtherRoadUp = false;
				else
					pLineData->m_bOtherRoadUp = true;

				lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK_ONLINE, BM_GETCHECK, 0, 0);
				if ( 0 == lResult )
					pLineData->m_bFootOnLine = false;
				else
					pLineData->m_bFootOnLine = true;

				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT1), buf, 100);
				pLineData->potalGroup = atoi(buf);
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT2), buf, 100);
				pLineData->fStopFactor = static_cast<float>( atof(buf) );
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT3), buf, 100);
				pLineData->beforeLine = atoi(buf);
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT4), buf, 100);
				pLineData->nextLine = atoi(buf);

				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT5), buf, 100);
				pLineData->m_fKeyStopTime = static_cast<float>( atof(buf) );
				
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT7), buf, 100);
				pLineData->m_fOriSpeed = static_cast<float>( atof(buf) );
				pLineData->m_fSpeed = pLineData->m_fOriSpeed;
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT8), buf, 100);
				pLineData->m_vOriJumpSpeed.x = static_cast<float>( atof(buf) );
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT9), buf, 100);
				pLineData->m_vOriJumpSpeed.y = static_cast<float>( atof(buf) );
				pLineData->m_vJumpSpeed = pLineData->m_vOriJumpSpeed;
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT10), buf, 100);
				pLineData->m_iOriTeleportLineIndex = atoi(buf);
				pLineData->m_iTeleportLineIndex = pLineData->m_iOriTeleportLineIndex;
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT11), buf, 100);
				pLineData->m_fCliffHeight = static_cast<float>( atof(buf) );
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT16), buf, 100);
				pLineData->m_fVarianceOxyen = static_cast<float>( atof(buf) );
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERAINDEX), buf, 100);
				pLineData->m_iOriCameraIndex = atoi(buf);
				pLineData->m_iCameraIndex = pLineData->m_iOriCameraIndex;
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_TRIGGERID), buf, 100);
				pLineData->m_iTriggerId = atoi(buf);

				D3DXVECTOR3 vStartPos = pLineData->startPos;
				D3DXVECTOR3 vEndPos = pLineData->endPos;
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_START), buf, 100);
				sscanf(buf, "%f%f%f", &vStartPos.x, &vStartPos.y, &vStartPos.z);
				GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_END), buf, 100);
				sscanf(buf, "%f%f%f", &vEndPos.x, &vEndPos.y, &vEndPos.z);
				pLineData->startPos = vStartPos;
				pLineData->endPos = vEndPos;
			}

			LRESULT lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK7, BM_GETCHECK, 0, 0);
			if ( 0 == lResult )
				pLineData->m_bUnderWater = false;
			else
				pLineData->m_bUnderWater = true;

			lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK_FORCE_COLOR, BM_GETCHECK, 0, 0);
			if ( 0 == lResult )
				pLineData->m_bForceChangeColor = false;
			else
				pLineData->m_bForceChangeColor = true;

			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT6), buf, 100);
			pLineData->lineSet = atoi(buf);
			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT12), buf, 100);
			pLineData->m_fWaterHeight = static_cast<float>( atof(buf) );
			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT13), buf, 100);
			pLineData->m_fBuoyancy = static_cast<float>( atof(buf) );
			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT14), buf, 100);
			pLineData->m_fResistance = static_cast<float>( atof(buf) );
			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT15), buf, 100);
			pLineData->m_fAnimSpeed = static_cast<float>( atof(buf) );
			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT17), buf, 100);
			pLineData->m_cLineColor.r = static_cast<float>( atof(buf) );
			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT18), buf, 100);
			pLineData->m_cLineColor.g = static_cast<float>( atof(buf) );
			GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT19), buf, 100);
			pLineData->m_cLineColor.b = static_cast<float>( atof(buf) );

			
		}
	}

	for(int iSel = 0; iSel < iSelectedCount; ++iSel )
	{
		int index = m_iSelectedList[iSel];
		if( index < 0 )
			continue;

		CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(index);
		if( pLineData != NULL )
		{
			bool bInit = false;
			if( iSelectedMulti == true )
				bInit = true;

			D3DXCOLOR unitColor = CalcUnderWaterUnitColor(pLineMap, pLineData, iSelectedCount, m_iSelectedList);
			pLineData->m_cLineColor = unitColor;
		}
	}

	pLineMap->ReBuildLineGroup();

	int index = SendDlgItemMessage(hDlg, LM_IDC_LIST_CAMERA, LB_GETCURSEL, 0, 0);
	CKTDGLineMap::CameraData *pCamera = pLineMap->GetCameraDataListByTool(index);
	if( pCamera != NULL )
	{
		LRESULT lResult = SendDlgItemMessage(hDlg, LM_IDC_CHECK_CAMERA, BM_GETCHECK, 0, 0);
		if ( 0 == lResult )
			pCamera->m_bFocusUnit = false;
		else
			pCamera->m_bFocusUnit = true;

		char buf[101];
		GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA4), buf, 100);
		pCamera->m_fCameraRepositionSpeed = static_cast<float>( atof(buf) );

		D3DXVECTOR3 vEye = pCamera->m_vEye;
		D3DXVECTOR3 vLookat = pCamera->m_vLookAt;
		D3DXVECTOR3 vRelativeEye = pCamera->m_vRelativeEye;
		GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA1), buf, 100);
		sscanf(buf, "%f%f%f", &vEye.x, &vEye.y, &vEye.z);
		GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA2), buf, 100);
		sscanf(buf, "%f%f%f", &vLookat.x, &vLookat.y, &vLookat.z);
		GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_CAMERA3), buf, 100);
		sscanf(buf, "%f%f%f", &vRelativeEye.x, &vRelativeEye.y, &vRelativeEye.z);
		pCamera->m_vEye = vEye;
		pCamera->m_vLookAt = vLookat;
		pCamera->m_vRelativeEye = vRelativeEye;		
		pCamera->m_eType = (CKTDGLineMap::LINE_CAMERA_TYPE)SendDlgItemMessage(hDlg, LM_IDC_COMBO_CAMERA, CB_GETCURSEL, 0, 0);
	}
	
	float fLandHeight = g_pWorldToolMain->ComputeLandHeight();
	GetWindowTextA(GetDlgItem(hDlg, LM_IDC_EDIT_LANDHEIGHT), buf, 100);
	if( strcmp(buf, "-1") )
	{
		fLandHeight = static_cast<float>( atof(buf) );
	}
	pLineMap->SetLandHeight(fLandHeight);

	if( g_pX2Game != NULL )
	{
		g_pX2Game->GetX2Camera()->SetLandHeight( fLandHeight );
	}
}

D3DXCOLOR CalcUnderWaterUnitColor(CKTDGLineMap *pLineMap, CKTDGLineMap::LineData *pLineData, int iLineCount, int *iLIneList)
{
	float fMaxHeight = 0.f;
	float fColorR = 0.f;
	float fColorG = 0.f;
	float fColorB = 0.f;
	D3DXCOLOR cRecommendColor = D3DXCOLOR(fColorR, fColorG, fColorB, 0.f);
	
	if( pLineMap == NULL )
		return D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		bool bProcess = false;
		if( iLineCount == 0  )
			bProcess = true;
		else
		{
			for(int j=0; j<iLineCount; ++j)
			{
				if( i == iLIneList[j] )
				{
					bProcess = true;
					break;
				}
			}
		}

		if( bProcess == true )
		{
			CKTDGLineMap::LineData *pLine = pLineMap->GetLineData(i);
			if( pLine != NULL && pLine->m_bUnderWater == true )
			{			
				float fMinY = min( pLine->startPos.y, pLine->endPos.y );	
				float fLineHeight = pLine->m_fWaterHeight - fMinY;

				if( fLineHeight >= fMaxHeight )			
				{
					fMaxHeight = fLineHeight;

					if( pLine->m_cLineColor.r > 0.f || pLine->m_cLineColor.g > 0.f || pLine->m_cLineColor.b > 0.f )
					{
						fColorR = pLine->m_cLineColor.r;
						fColorG = pLine->m_cLineColor.g;
						fColorB = pLine->m_cLineColor.b;
					}

					cRecommendColor = D3DXCOLOR(fColorR, fColorG, fColorB, 0.f);
				}
			}
		}		
	}

	if( pLineData == NULL || fMaxHeight <= 0.f )
		return D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

	float fMinY = min( pLineData->startPos.y, pLineData->endPos.y );
	float fColorRate = (pLineData->m_fWaterHeight - fMinY) / fMaxHeight; 

	if( fColorRate < 0.f )
		return D3DXCOLOR(0.f, 0.f, 0.f, 0.f);

	cRecommendColor.r *= fColorRate;
	cRecommendColor.g *= fColorRate;
	cRecommendColor.b *= fColorRate;

	return cRecommendColor;
}
void initTriggerSegDlg(HWND hDlg)
{
	int nDel = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_DELETESTRING, 0, 0);
	}

	InitTriggerDataDlg(hDlg);
}
void InitTriggerDataDlg(HWND hDlg)
{
	// trigger mesh
	int nDel = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_DELETESTRING, 0, 0);
	}
	// trigger line
	nDel = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_DELETESTRING, 0, 0);
	}
	// trigger etc
	nDel = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_DELETESTRING, 0, 0);
	}

	CKTDGLineMap::TRIGGER_DATA_MESH triggerMesh;
	CKTDGLineMap::TRIGGER_DATA_LINE triggerLine;
	CKTDGLineMap::TRIGGER_DATA_ETC	triggerEtc;

	UpdateTriggerMeshDlg(hDlg, triggerMesh);
	UpdateTriggerLineDlg(hDlg, triggerLine);
	UpdateTriggerEtcDlg(hDlg, triggerEtc);
}

void UpdateTriggerDlg(HWND hDlg, CKTDGLineMap *pLineMap)
{
	if( pLineMap == NULL )
		return;
	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();

	// trigger
	int nDel = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_DELETESTRING, 0, 0);
	}

	// trigger info
	WCHAR wstr[100];
	for(int i=0; i<(int)pTrigger->m_vecTriggerSeg.size(); ++i)
	{
		CKTDGLineMap::TRIGGER_SEGMENT triggerSeg = pTrigger->m_vecTriggerSeg[i];
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_SEG, LB_ADDSTRING, 0, (LPARAM)wstr );
	}	

	initTriggerSegDlg(hDlg);
}
void UpdateTriggerSegDlg(HWND hDlg, CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg)
{
	// trigger segment
	int nDel = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_DELETESTRING, 0, 0);
	}

	// trigger info
	WCHAR wstr[100];
	for(int i=0; i<(int)triggerSeg.m_vecTriggerData.size(); ++i)
	{		
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_DATA, LB_ADDSTRING, 0, (LPARAM)wstr );
	}	

	InitTriggerDataDlg(hDlg);
}
void UpdateTriggerDataDlg(HWND hDlg, CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg, int iData)
{
	CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[iData];

	InitTriggerDataDlg(hDlg);

	if( iData < 0 )
		return;

	// trigger info
	WCHAR wstr[100];
	int i=0;
	for(i=0; i<(int)triggerData.m_vecTriggerMesh.size(); ++i)
	{		
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_MESH, LB_ADDSTRING, 0, (LPARAM)wstr );
	}	
	for(i=0; i<(int)triggerData.m_vecTriggerLine.size(); ++i)
	{		
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_LINE, LB_ADDSTRING, 0, (LPARAM)wstr );
	}	
	for(i=0; i<(int)triggerData.m_vecTriggerEtc.size(); ++i)
	{		
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_ETC, LB_ADDSTRING, 0, (LPARAM)wstr );
	}

	SendDlgItemMessage(hDlg, T_CHECK_LOOP, BM_SETCHECK, (WPARAM)triggerSeg.m_bLoop, 0);
}

std::vector<wstring> GetSkinMeshAnimList(wstring wstrMeshName)
{
	std::vector<wstring> vecAnimList;
	vecAnimList.clear();
	
	for(UINT i=0; i<g_pWorldToolMain->m_vecpWorldToolObject.size(); ++i)
	{
		CWorldToolObject* pWorldToolObject = g_pWorldToolMain->m_vecpWorldToolObject[i];

		if( pWorldToolObject == NULL || pWorldToolObject->m_pWorldObject == NULL )
			continue;

		wstring wstrName = L"";
		switch( pWorldToolObject->m_pWorldObject->GetObjectType() )
		{
		case CX2WorldObject::OT_SKIN_MESH:
			{		
				CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) pWorldToolObject->m_pWorldObject;
				CKTDGXSkinAnim *pSkinAnim = pSkinMesh->GetXSkinAnim();

				wstring wstrMeshName1 = wstrMeshName;
				wstring wstrMeshName2 = pSkinMesh->GetXSkinMeshName();
				MakeUpperCase(wstrMeshName1);
				MakeUpperCase(wstrMeshName2);

				if( pSkinAnim != NULL && wstrMeshName1 == wstrMeshName2 )
				{
					pSkinAnim->GetAnimNameList(vecAnimList);
					return vecAnimList;
				}
			} break;
		default:
			break;
		}
	}

	return vecAnimList;
}
std::vector<wstring> UpdateTirggerSkinMeshList(HWND hDlg)
{	
	std::vector<wstring> vecSkinMeshList;
	vecSkinMeshList.clear();

	int nDel = SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_DELETESTRING, 0, 0);
	}

	for(UINT i=0; i<g_pWorldToolMain->m_vecpWorldToolObject.size(); ++i)
	{
		CWorldToolObject* pWorldToolObject = g_pWorldToolMain->m_vecpWorldToolObject[i];

		if( pWorldToolObject == NULL || pWorldToolObject->m_pWorldObject == NULL )
			continue;

		wstring wstrName = L"";
		switch( pWorldToolObject->m_pWorldObject->GetObjectType() )
		{
		case CX2WorldObject::OT_SKIN_MESH:
			{
				CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) pWorldToolObject->m_pWorldObject;				
				wstrName = pSkinMesh->GetXSkinMeshName();

				if( StrStrW(wstrName.c_str(), L"Trigger") || StrStrW(wstrName.c_str(), L"TRIGGER"))
				{
					SendDlgItemMessage( hDlg, T_COMBO_MESH_ID, CB_ADDSTRING, 0, (LPARAM)wstrName.c_str() );
					vecSkinMeshList.push_back(wstrName);
				}
			} break;
		default:
			break;
		}
	}	

	return vecSkinMeshList;
}
int UpdateTirggerSkinMeshAnimList(HWND hDlg, wstring wstrMeshName, wstring wstrDufaultName)
{	
	int nDel = SendDlgItemMessage(hDlg, T_COMBO_MESH_ANIMNAME, CB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_COMBO_MESH_ANIMNAME, CB_DELETESTRING, 0, 0);
	}	
	int iSelIndex = -1;
	std::vector<wstring> vecAnimList = GetSkinMeshAnimList( wstrMeshName );	
	for( UINT iAnim = 0; iAnim < vecAnimList.size(); ++iAnim )
	{
		SendDlgItemMessage( hDlg, T_COMBO_MESH_ANIMNAME, CB_ADDSTRING, 0, (LPARAM)vecAnimList[iAnim].c_str() );
		if( wstrDufaultName == vecAnimList[iAnim] )
			iSelIndex = iAnim;
	}

	return iSelIndex;
}

void UpdateTriggerMeshDlg(HWND hDlg, CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh)
{
	// combo_action
	SendDlgItemMessage(hDlg, T_COMBO_MESH_ACTION, CB_SETCURSEL, (WPARAM)triggerMesh.m_iMeshTriggerAction, 0);

	// edit_time
	char buf[100];
	sprintf(buf, "%6.3f", triggerMesh.m_fTriggerTime );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_MESH_DELAYTIME), buf);

	// combo_mesh_id(name)
	std::vector<wstring> vecSkinMesh = UpdateTirggerSkinMeshList(hDlg);
	WCHAR wBuf[100];
	for(UINT i=0; i<vecSkinMesh.size(); ++i)
	{
		SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_GETLBTEXT, (WPARAM)i, (LPARAM)wBuf);

		wstring wstrMeshName1 = wBuf;
		wstring wstrMeshName2 = triggerMesh.m_wstrMeshName;
		MakeUpperCase(wstrMeshName1);
		MakeUpperCase(wstrMeshName2);

		if( wstrMeshName1 == wstrMeshName2 )
		{
			SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_SETCURSEL, (WPARAM)i, 0);
			break;
		}
	}

	// combo_mesh_animname
	int iSelIndex = UpdateTirggerSkinMeshAnimList(hDlg, triggerMesh.m_wstrMeshName, triggerMesh.m_wstrName);
	SendDlgItemMessage(hDlg, T_COMBO_MESH_ANIMNAME, CB_SETCURSEL, (WPARAM)iSelIndex, 0);

	// combo_playtype
	SendDlgItemMessage(hDlg, T_COMBO_MESH_PLAYTYPE, CB_SETCURSEL, (WPARAM)triggerMesh.m_iPlayType, 0);

	// edit_animspeed
	sprintf(buf, "%6.3f", triggerMesh.m_fAnimSpeed );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_ANIMSPEED), buf);
}
void UpdateTriggerLineDlg(HWND hDlg, CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine)
{
	// combo_action
	SendDlgItemMessage(hDlg, T_COMBO_LINE_ACTION, CB_SETCURSEL, (WPARAM)triggerLine.m_iLineTriggerAction, 0);

	// edit_time
	char buf[100];
	sprintf(buf, "%6.3f", triggerLine.m_fTriggerTime );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_DELAYTIME), buf);

	// edit_lineid
	sprintf(buf, "%d", triggerLine.m_iTriggerLineIndex );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINEID), buf);

	// check_enable
	SendDlgItemMessage(hDlg, T_CHECK_LINE_ENABLE, BM_SETCHECK, (WPARAM)triggerLine.m_bEnable, 0);

	// edit_cameraid
	sprintf(buf, "%d", triggerLine.m_iCamera );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_CAMERAID), buf);

	// edit_speed
	sprintf(buf, "%6.3f", triggerLine.m_fSpeed );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_SPEED), buf);

	// edit_jumpspeedx
	sprintf(buf, "%6.3f", triggerLine.m_vJumpSpeed.x );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_JUMPSPEEDX), buf);

	// edit_jumpspeedy
	sprintf(buf, "%6.3f", triggerLine.m_vJumpSpeed.y );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT1_LINE_JUMPSPEEDY), buf);
}
void UpdateTriggerEtcDlg(HWND hDlg, CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc)
{	
	// combo_action
	SendDlgItemMessage(hDlg, T_COMBO_ETC_ACTION, CB_SETCURSEL, (WPARAM)triggerEtc.m_iEtcTriggerAction, 0);

	// edit_time
	char buf[100];
	sprintf(buf, "%6.3f", triggerEtc.m_fTriggerTime );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_DELAYTIME), buf);

	// edit_name
	SetWindowText(GetDlgItem(hDlg, T_EDIT_ETC_NAME), triggerEtc.m_wstrActionName.c_str());

	// camera index
	sprintf(buf, "%d", triggerEtc.m_iDummyValue01 );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_ETC_CAMERA), buf);

	// camera duration
	sprintf(buf, "%6.3f", triggerEtc.m_fDummyValue01 );
	SetWindowTextA(GetDlgItem(hDlg, T_EDIT_ETC_CAMERADURATION), buf);
}

void UpdateTriggerData(HWND hDlg, CKTDGLineMap *pLineMap)
{
	int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
	int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);

	if( indexSeg < 0 || indexData < 0 )
		return;

	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
	CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];

	LRESULT lResult = SendDlgItemMessage(hDlg, T_CHECK_LOOP, BM_GETCHECK, 0, 0);
	if ( 0 == lResult )
		triggerSeg.m_bLoop = false;
	else
		triggerSeg.m_bLoop = true;
	triggerSeg.m_iTriggerDataIndex = 0;
}

int AddTriggerMesh(HWND hDlg, CKTDGLineMap *pLineMap, int iVal)
{
	WCHAR wBuf[100];
	int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
	int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);

	if( indexSeg < 0 || indexData < 0 )
		return -1;

	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
	CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];
	CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];
	
	int indexMesh = iVal;
	if( indexMesh < 0 )
	{
		CKTDGLineMap::TRIGGER_DATA_MESH triggerMeshTemp;
		triggerData.m_vecTriggerMesh.push_back(triggerMeshTemp);
		indexMesh = triggerData.m_vecTriggerMesh.size() - 1;
	}
	CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh = triggerData.m_vecTriggerMesh[indexMesh];

	

	// combo_action
	triggerMesh.m_iMeshTriggerAction = (CKTDGLineMap::TRIGGER_MESH_ACTION_TYPE)SendDlgItemMessage(hDlg, T_COMBO_MESH_ACTION, CB_GETCURSEL, 0, 0);
	
	// edit_time
	char buf[100];	
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_MESH_DELAYTIME), buf, 99);
	triggerMesh.m_fTriggerTime = static_cast<float>( atof(buf) );

	// combo_mesh_id(name)
	int iSelMeshId = SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_GETCURSEL, 0, 0);	
	SendDlgItemMessage(hDlg, T_COMBO_MESH_ID, CB_GETLBTEXT, (WPARAM)iSelMeshId, (LPARAM)wBuf);
	triggerMesh.m_wstrMeshName = wBuf;

	// combo_mesh_animname		
	int ianim = SendDlgItemMessage(hDlg, T_COMBO_MESH_ANIMNAME, CB_GETCURSEL, 0, 0);
	SendDlgItemMessage(hDlg, T_COMBO_MESH_ANIMNAME, CB_GETLBTEXT, ianim, (LPARAM)wBuf);
	triggerMesh.m_wstrName = wBuf;

	// combo_playtype
	triggerMesh.m_iPlayType = SendDlgItemMessage(hDlg, T_COMBO_MESH_PLAYTYPE, CB_GETCURSEL, 0, 0);

	// edit_animspeed
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_ANIMSPEED), buf, 99);
	triggerMesh.m_fAnimSpeed = static_cast<float>( atof(buf) );

	// trigger mesh
	WCHAR wstr[100];
	int nDel = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_MESH, LB_DELETESTRING, 0, 0);
	}

	for(UINT i=0; i<(int)triggerData.m_vecTriggerMesh.size(); ++i)
	{		
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_MESH, LB_ADDSTRING, 0, (LPARAM)wstr );
	}

	return indexMesh;
}
int AddTriggerLine(HWND hDlg, CKTDGLineMap *pLineMap, int iVal)
{
	int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
	int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);

	if( indexSeg < 0 || indexData < 0 )
		return -1;

	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
	CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];
	CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];

	int indexLine = iVal;
	if( indexLine < 0 )
	{
		CKTDGLineMap::TRIGGER_DATA_LINE triggerLineTemp;
		triggerData.m_vecTriggerLine.push_back(triggerLineTemp);
		indexLine = triggerData.m_vecTriggerLine.size() - 1;
	}
	CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine = triggerData.m_vecTriggerLine[indexLine];

	// combo_action
	triggerLine.m_iLineTriggerAction = (CKTDGLineMap::TRIGGER_LINE_ACTION_TYPE)SendDlgItemMessage(hDlg, T_COMBO_LINE_ACTION, CB_GETCURSEL, 0, 0);

	// edit_time
	char buf[100];	
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_DELAYTIME), buf, 99);
	triggerLine.m_fTriggerTime = static_cast<float>( atof(buf) );

	// edit_lineid
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINEID), buf, 99);
	triggerLine.m_iTriggerLineIndex = atoi(buf);

	// check_lineenable
	LRESULT lResult = SendDlgItemMessage(hDlg, T_CHECK_LINE_ENABLE, BM_GETCHECK, 0, 0);
	if ( 0 == lResult )
		triggerLine.m_bEnable = false;
	else
		triggerLine.m_bEnable = true;

	// edit_cameraid
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_CAMERAID), buf, 99);
	triggerLine.m_iCamera = atoi(buf);

	// edit_linespeed
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_SPEED), buf, 99);
	triggerLine.m_fSpeed = static_cast<float>( atof(buf) );
	// edit_linejumpspeedx
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_LINE_JUMPSPEEDX), buf, 99);
	triggerLine.m_fSpeed = static_cast<float>( atof(buf) );
	// edit_linejumpspeedy
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT1_LINE_JUMPSPEEDY), buf, 99);
	triggerLine.m_fSpeed = static_cast<float>( atof(buf) );

	// trigger etc
	WCHAR wstr[100];
	int nDel = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_LINE, LB_DELETESTRING, 0, 0);
	}

	for(UINT i=0; i<(int)triggerData.m_vecTriggerLine.size(); ++i)
	{		
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_LINE, LB_ADDSTRING, 0, (LPARAM)wstr );
	}

	return indexLine;
}
int AddTriggerEtc(HWND hDlg, CKTDGLineMap *pLineMap, int iVal)
{
	int indexSeg = SendDlgItemMessage(hDlg, T_LIST_SEG, LB_GETCURSEL, 0, 0);
	int indexData = SendDlgItemMessage(hDlg, T_LIST_DATA, LB_GETCURSEL, 0, 0);

	if( indexSeg < 0 || indexData < 0 )
		return -1;

	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
	CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pTrigger->m_vecTriggerSeg[indexSeg];
	CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[indexData];

	int indexEtc = iVal;
	if( indexEtc < 0 )
	{
		CKTDGLineMap::TRIGGER_DATA_ETC triggerEtcTemp;
		triggerData.m_vecTriggerEtc.push_back(triggerEtcTemp);
		indexEtc = triggerData.m_vecTriggerEtc.size() - 1;
	}
	CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc = triggerData.m_vecTriggerEtc[indexEtc];

	// combo_action
	triggerEtc.m_iEtcTriggerAction = (CKTDGLineMap::TRIGGER_ETC_ACTION_TYPE)SendDlgItemMessage(hDlg, T_COMBO_ETC_ACTION, CB_GETCURSEL, 0, 0);

	// edit_time
	char buf[100];	
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_DELAYTIME), buf, 99);
	triggerEtc.m_fTriggerTime = static_cast<float>( atof(buf) );

	// edit_animspeed
	WCHAR wBuf[100];
	GetWindowText(GetDlgItem(hDlg, T_EDIT_ETC_NAME), wBuf, 99);
	triggerEtc.m_wstrActionName = wBuf;

	// camera 
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_ETC_CAMERA), buf, 99);
	triggerEtc.m_iDummyValue01 = atoi(buf);

	// camera duration
	GetWindowTextA(GetDlgItem(hDlg, T_EDIT_ETC_CAMERADURATION), buf, 99);
	triggerEtc.m_fDummyValue01 = static_cast<float>( atof(buf) );

	// trigger etc
	WCHAR wstr[100];
	int nDel = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_DELETESTRING, 0, 0);
	while(nDel > 0)
	{
		nDel = SendDlgItemMessage(hDlg, T_LIST_ETC, LB_DELETESTRING, 0, 0);
	}

	for(UINT i=0; i<(int)triggerData.m_vecTriggerEtc.size(); ++i)
	{		
		wsprintf(wstr, L"%d", i);
		SendDlgItemMessage( hDlg, T_LIST_ETC, LB_ADDSTRING, 0, (LPARAM)wstr );
	}

	return indexEtc;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////// Frustum ////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK WorldToolFrustumDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if(g_pWorldToolMain == NULL)
		return FALSE;

	switch(iMessage)
	{
	case WM_CREATE:
	case WM_INITDIALOG:
		{				
		char buf[10];		
			sprintf(buf, "%5.2f", g_pKTDXApp->GetDGManager()->GetNear());
			SetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT1), buf);
			sprintf(buf, "%5.2f", g_pKTDXApp->GetDGManager()->GetFar());
			SetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT2), buf);
			sprintf(buf, "%5.2f", g_pKTDXApp->GetDGManager()->GetPerspectiveValue());
			SetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT3), buf);

			float fBase = 0.f;
			float fCamera1 = 0.f;
			float fCamera2 = 0.f;
			float fCamera3 = 0.f;

			if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
			{
				fBase = g_pX2Game->GetWorld()->GetBaseCameraDistance();
				fCamera1 = g_pX2Game->GetWorld()->GetCameraStepDistacne(0);
				fCamera2 = g_pX2Game->GetWorld()->GetCameraStepDistacne(1);
				fCamera3 = g_pX2Game->GetWorld()->GetCameraStepDistacne(2);
			}

			sprintf(buf, "%5.2f", fBase);
			SetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT4), buf);
			sprintf(buf, "%5.2f", fCamera1);
			SetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT5), buf);
			sprintf(buf, "%5.2f", fCamera2);
			SetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT6), buf);
			sprintf(buf, "%5.2f", fCamera3);
			SetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT7), buf);
		}
		return TRUE;    

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case FRU_IDOK:
			{
				float fNear = 0.f;
				float fFar = 50000.f;
				float fPerspective = 4.f;	
				float fBase = 0.f;
				float fCamera1 = 0.f;
				float fCamera2 = 0.f;
				float fCamera3 = 0.f;

				char PosBuf[10];
				GetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT1), PosBuf, 9);
				fNear = (float)atof(PosBuf);
				GetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT2), PosBuf, 9);
				fFar = (float)atof(PosBuf);
				GetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT3), PosBuf, 9);
				fPerspective = (float)atof(PosBuf);

				if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
				{
					GetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT4), PosBuf, 9);
					fBase = (float)atof(PosBuf);
					GetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT5), PosBuf, 9);
					fCamera1 = (float)atof(PosBuf);
					GetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT6), PosBuf, 9);
					fCamera2 = (float)atof(PosBuf);
					GetWindowTextA(GetDlgItem(hDlg, IDC_FRU_EDIT7), PosBuf, 9);
					fCamera3 = (float)atof(PosBuf);

					g_pX2Game->GetWorld()->SetCameraDistance( fBase, fCamera1, fCamera2, fCamera3 );
				}

				g_pKTDXApp->GetDGManager()->SetNear( fNear );
				g_pKTDXApp->GetDGManager()->SetFar( fFar );
				g_pKTDXApp->GetDGManager()->SetPerspectiveValue( fPerspective );
				g_pKTDXApp->GetDGManager()->SetProjection( fNear, fFar, g_pKTDXApp->GetDGManager()->GetPerspective() );
			}
			return TRUE;  

		case IDC_FRU_EDIT1:
		case IDC_FRU_EDIT2:
		case IDC_FRU_EDIT3:
			switch(HIWORD(wParam))
			{
			case EN_CHANGE:
				{
					
				}                            
			}
			return TRUE;
		}
		return TRUE;


	case WM_CLOSE:
	case WM_DESTROY:
		DestroyWindow(g_hFrustumDlg);		
		g_hFrustumDlg = NULL;		
		return TRUE;
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////////
CWorldToolGame::CWorldToolGame(void)
{
	m_GameType			= CX2Game::GT_PVP;

	m_pPVPRoom			= NULL;
	m_bSquareCamera		= false;
    m_bOrthoFreeCamera  = false;
    m_fOrthoScale       = 2.0f;
	m_bFixedCamera		= false;
	//m_WinTeam			= CX2Room::TN_RED;

	//m_bTimeStop			= false;
	//m_fRemainPlayTime	= 0.0f;
	//m_pFontForTime		= g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUKFont( L"돋움체", 50, 1, true );
}

CWorldToolGame::~CWorldToolGame(void)
{
}

HRESULT CWorldToolGame::OnFrameMove( double fTime, float fElapsedTime )
{
	SetAllowFrameCount( (int)(85 * 5.3) );

	m_fTime			= (float)fTime;
	m_fElapsedTime	= fElapsedTime;


	if( g_pKTDXApp->GetDIManager()->Getkeyboard()->GetKeyState(DIK_RCONTROL) == TRUE && g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( m_bGameWait == true )
			m_bGameWait = false;
		else
			m_bGameWait = true;
	}

	if( m_bGameWait == true )
		return S_OK;

	P2PPacketHandler();	

	if( m_GameState == GS_PLAY )
	{		
		m_fGameTime += m_fElapsedTime;
		m_AllowFrameCount--;
		if( m_AllowFrameCount < 0 )
		{
			m_AllowFrameCount = 0;
		}
		UpdateUnitPointer();

		if( false == m_bFreeCamera )
		{
			KeyProcess();
		}

//		//{{ 2009.02.25 김태완 : 유닛 안 움직이는 현상 수정
//		// user, npc sync packet을 전송할 User uid 리스트 갱신
		m_vecUserUIDforSyncPacket.resize(0);
		for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
		{
			CX2GUUser* pCX2GUUser = m_UserUnitList[i];
			if( pCX2GUUser != NULL )
			{
				m_vecUserUIDforSyncPacket.push_back( pCX2GUUser->GetUnitUID() );
			}
		}
//		//}}

		if( m_bStopUnitChain == false )
		{
			for( UINT i = 0; i < m_UnitList.size(); i++ )
			{
				CX2GameUnit* pCX2GameUnit = m_UnitList[i];

				if ( pCX2GameUnit != NULL )
				{
					pCX2GameUnit->OnFrameMove( fTime, fElapsedTime );
				}

				// 내 유닛 HP는 늘 100%되게 치트
				if( true == pCX2GameUnit->IsMyUnit() )
				{
					pCX2GameUnit->SetNowHp( pCX2GameUnit->GetMaxHp() );
				}
			}
		}
/*
        if(g_pKTDXApp->GetDGManager()->GetPerspective() == false)
        {
            const D3DXVECTOR3 *vEye;
            vEye = m_FPSCamera.GetEyePt();
            D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * g_pKTDXApp->GetDGManager()->GetFar()*0.1f + *m_FPSCamera.GetEyePt();
            //vLookAt = m_FPSCamera.GetLookAtPt();

            m_pCamera->GetCamera().Point(vEye->x, vEye->y, vEye->z, vLookAt.x, vLookAt.y, vLookAt.z);
            //m_pWorld->SetEye(*vEye);
            //m_pWorld->SetLookAt(vLookAt);
        }
*/

		m_pWorld->OnFrameMove( fTime, fElapsedTime );

		//m_pMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
		//m_pMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

		m_pDamageEffect->OnFrameMove( fTime, fElapsedTime );

		//m_pMajorParticle->OnFrameMove( fTime, fElapsedTime );
		//m_pMinorParticle->OnFrameMove( fTime, fElapsedTime );

		//GetMajorXMeshPlayer()->OnFrameMove( fTime, fElapsedTime );
		//GetMinorXMeshPlayer()->OnFrameMove( fTime, fElapsedTime );
		//GetMajorParticle()->OnFrameMove( fTime, fElapsedTime );
		//GetMinorParticle()->OnFrameMove( fTime, fElapsedTime );
		
		m_UnitLoader.OnFrameMove( fElapsedTime );

		if(false == m_bFixedCamera)
		{
			if ( GetFocusUnit() != NULL )
			//if( m_pFocusUnit != NULL )
			{
				if( true == m_bSquareCamera )
				{
					//SquareCamera( (CX2GUUser*)m_pFocusUnit, m_SquareCameraParam.m_fEyeDistance, m_SquareCameraParam.m_fEyeHeight, m_SquareCameraParam.m_fLookAtHeight );
					SquareCamera( (CX2GUUser*)GetFocusUnit(), m_SquareCameraParam.m_fEyeDistance, m_SquareCameraParam.m_fEyeHeight, m_SquareCameraParam.m_fLookAtHeight );
				}
				else
				{                
					if(g_pKTDXApp->GetDGManager()->GetPerspective() == false)
						g_pMain->GetGameOption().GetOptionList().m_iZoomLevel = 1;
					else
						g_pMain->GetGameOption().GetOptionList().m_iZoomLevel = -1;

					//{{ 09.02.26 김태완 :: 다이나믹 카메라 On/OFF 기능
					// 원래 코드
					//m_pCamera->NomalTrackingCamera( m_pFocusUnit, m_SquareCameraParam.m_fEyeDistance );
					// m_pFocusUnit->OnCameraMove(); 원하는 동작을 하므로 이런 식으로 구현.
					GetFocusUnit()->OnCameraMove();				
					//}}

				}
			}                
			m_pCamera->OnFrameMove( fTime, fElapsedTime );

		}
		

		if( IsHost() == true )
		{
			m_NPCPacketSendIntervalNow += m_fElapsedTime;
			if( m_NPCPacketSendIntervalNow >= m_NPCPacketSendInterval )
			{
				KXPT_UNIT_NPC_SYNC_PACK kXPT_UNIT_NPC_SYNC_PACK;
				for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
				{
					CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
					if ( pCX2GUNPC != NULL )
					{
						pCX2GUNPC->SendPacket( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
						if( sizeof(KXPT_UNIT_NPC_SYNC) * kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() >  MTU_SIZE )
						{
							KSerBuffer buff;
							Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

							std::vector<UidType> toUIDList;
							for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
							{
								CX2GUUser* pCX2GUUser = m_UserUnitList[i];
								if( pCX2GUUser != NULL )
								{
									toUIDList.push_back( pCX2GUUser->GetUnitUID() );
								}
							}
							g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
						}
					}
				}

				if( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > 0 )
				{
					KSerBuffer buff;
					Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

					std::vector<UidType> toUIDList;
					for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
					{
						CX2GUUser* pCX2GUUser = m_UserUnitList[i];
						if( pCX2GUUser != NULL )
						{
							toUIDList.push_back( pCX2GUUser->GetUnitUID() );
						}
					}
					g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
				}			

				m_NPCPacketSendIntervalNow = 0.0f;
			}

			//즉시처리
			KXPT_UNIT_NPC_SYNC_PACK kXPT_UNIT_NPC_SYNC_PACK;
			for( UINT i = 0; i < m_NPCUnitList.size(); i++ )
			{
				CX2GUNPC* pCX2GUNPC = m_NPCUnitList[i];
				if ( pCX2GUNPC != NULL )
				{
					pCX2GUNPC->SendPacketImmediate( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList );
					if( sizeof(KXPT_UNIT_NPC_SYNC) * kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() >  MTU_SIZE )
					{
						KSerBuffer buff;
						Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

						std::vector<UidType> toUIDList;
						for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
						{
							CX2GUUser* pCX2GUUser = m_UserUnitList[i];
							if( pCX2GUUser != NULL )
							{
								toUIDList.push_back( pCX2GUUser->GetUnitUID() );
							}
						}
						g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
					}
				}
			}

			if( kXPT_UNIT_NPC_SYNC_PACK.unitNPCSyncList.size() > 0 )
			{
				KSerBuffer buff;
				Serialize( &buff, &kXPT_UNIT_NPC_SYNC_PACK );

				std::vector<UidType> toUIDList;
				for( int i = 0; i < (int)m_UserUnitList.size(); i++ )
				{
					CX2GUUser* pCX2GUUser = m_UserUnitList[i];
					if( pCX2GUUser != NULL )
					{
						toUIDList.push_back( pCX2GUUser->GetUnitUID() );
					}
				}
				g_pData->GetGameUDP()->BroadCast( toUIDList, XPT_UNIT_NPC_SYNC_PACK, (char*)buff.GetData(), buff.GetLength() );
			}
		}

	}

	return S_OK;
}

HRESULT CWorldToolGame::OnFrameRender()
{
	if( m_GameState == GS_PLAY )
	{
		m_pWorld->OnFrameRender();
		g_pKTDXApp->GetDGManager()->ObjectChainSort();
		g_pKTDXApp->GetDGManager()->ObjectChainNonAlphaRender();
		g_pKTDXApp->GetDGManager()->ObjectChainAlphaRender();

#ifndef WORLD_TOOL	// 090519 태완 임시수정코드
		g_pKTDXApp->GetDGManager()->GetDialogManager()->GetFont(0)->Flush();
		g_pKTDXApp->GetDGManager()->FrontUIRender();		
#endif

	}

	return S_OK;
}

void CWorldToolGame::GameLoading( CX2Room* pRoom )
{
	m_pPVPRoom			= (CX2PVPRoom*)pRoom;

	m_GameState			= GS_LOADING;
	CX2Game::GameLoading( pRoom );
	m_GameState			= GS_LOADING;

	m_GameState			= GS_PLAY;
}

void CWorldToolGame::SystemLoading()
{
	m_GameState = GS_LOADING;

	srand((unsigned int)time(NULL));

	//게임 파티클 시스템 생성
	g_pData->ResetGameMajorParticle( L"GameMajorParticle.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle2.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle3.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle4.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle5.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle6.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle7.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle8.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle9.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle10.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle11.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle12.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle13.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle14.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle15.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle16.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle17.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle18.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle19.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle20.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle21.txt" );
	g_pData->GetGameMajorParticle()->OpenScriptFile( L"GameMajorParticle22.txt" );

	g_pData->ResetGameMinorParticle( L"GameMinorParticle.txt" );

	//m_pMajorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
	//m_pMinorParticle	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
	//m_pMajorXMeshPlayer	= new CKTDGXMeshPlayer();
	//m_pMinorXMeshPlayer	= new CKTDGXMeshPlayer();


	//m_pMajorParticle->OpenScriptFile( L"GameMajorParticle.txt" );
	//m_pMinorParticle->OpenScriptFile( L"GameMinorParticle.txt" );
	//m_pMajorXMeshPlayer->OpenScriptFile( L"GameMajorXMeshPlayer.txt" );
	//m_pMinorXMeshPlayer->OpenScriptFile( L"GameMinorXMeshPlayer.txt" );

	m_pDamageEffect		= new CX2DamageEffect();
	m_pDamageEffect->OpenScriptFile( L"DamageEffect_Test.lua" );

}

void CWorldToolGame::WorldLoading()
{
    m_pWorld = g_pData->GetWorldManager()->CreateWorld( (CX2World::WORLD_ID)m_pPVPRoom->GetWorldID() );
}




void CWorldToolGame::EtcLoading()
{
	m_pCamera		= new CX2Camera();
	if( m_pWorld != NULL )
		m_pCamera->SetLandHeight( m_pWorld->GetLineMap()->GetLandHeight() );
	m_pCamera->NomalDirectCamera( GetFocusUnit() );

	m_pDropItemManager = NULL;


	D3DXVECTOR3 vEye = m_pCamera->GetCamera().GetEye();
	D3DXVECTOR3 vLookAt = m_pCamera->GetCamera().GetLookAt();
	m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	m_FPSCamera.SetProjParams( D3DX_PI/4, 1.f, g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar() );
	m_FPSCamera.SetRotateButtons( false, false, true, false );

}


void CWorldToolGame::ResetUnitPosition()
{
	GetMyUnit()->ClearRecieveSyncData();
	GetMyUnit()->InitPosition( true, 0 );

}

void CWorldToolGame::ReleaseCurrentWorld()
{
	if( NULL == m_pWorld )
		return;

	vector<CX2WorldObjectPtr> vecpWorldObjectList = m_pWorld->GetWorldObjectList();
	for( unsigned i=0; i<vecpWorldObjectList.size(); i++ )
	{
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( vecpWorldObjectList[i].get() );
	}

	vector<CKTDGSkyDome*>& vecpSkyDomeList = m_pWorld->GetSkyDomeList();
	for( unsigned i=0; i<vecpSkyDomeList.size(); i++ )
	{
		g_pKTDXApp->GetDGManager()->RemoveObjectChain( vecpSkyDomeList[i] );
	}

	SAFE_DELETE( m_pWorld );
}

void CWorldToolGame::ReloadWorld( const WCHAR* pwszFileName )
{
	ReleaseCurrentWorld();

	CX2World::WorldData worldData;
	worldData.dataFileName = pwszFileName;

	m_pWorld = new CX2World( &worldData );
	m_pWorld->OpenScriptFile( pwszFileName );
}


void CWorldToolGame::SquareCamera( CX2GUUser* pFocusUnit, float distance, float eyeHeight /*= 200.f*/, float lookAtHeight /*= 100.f*/, float angleDegree, 
								  float eyeDistance, float lookatDistance, float trackingTime )
{
	if( NULL == m_pCamera )
		return;

	//m_CameraState	= CS_NORMAL;

	if( pFocusUnit == NULL )
		return;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 lookAt;

	lookAt = pFocusUnit->GetPos();
	lookAt.y += lookAtHeight;

	if( pFocusUnit->GetIsRight() == true )
	{
		lookAt += (pFocusUnit->GetDirVector() * lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * eyeDistance);
	}
	else
	{
		lookAt += (pFocusUnit->GetDirVector() * -lookatDistance);
		eye.x = pFocusUnit->GetPos().x + sinf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + 180.0f - angleDegree) ) * -distance;
		eye.y = pFocusUnit->GetPos().y + eyeHeight; 
		eye.z = pFocusUnit->GetPos().z + cosf( D3DXToRadian(pFocusUnit->GetRotateDegree().y + 180.0f - angleDegree) ) * -distance;
		eye += (pFocusUnit->GetDirVector() * -eyeDistance);
	}


	if( eye.y < m_pCamera->GetLandHeight() + 200.0f )
		eye.y = m_pCamera->GetLandHeight() + 200.0f;
	if( lookAt.y < m_pCamera->GetLandHeight() + 200.0f )
		lookAt.y = m_pCamera->GetLandHeight() + 200.0f;
	m_pCamera->GetCamera().SetTracking( eye.x, eye.y, eye.z, lookAt.x, lookAt.y, lookAt.z, trackingTime );
}


void CWorldToolGame::SetCamera()
{
    if( m_bSquareCamera == false )
	{
        m_pCamera->NomalDirectCamera( (CX2GUUser*)GetFocusUnit(), m_SquareCameraParam.m_fEyeDistance );
	}
	else 
	{
		SquareCamera( (CX2GUUser*)GetFocusUnit(), m_SquareCameraParam.m_fEyeDistance, m_SquareCameraParam.m_fEyeHeight, m_SquareCameraParam.m_fLookAtHeight );
	}
}


//////////////////////////////////////////////////////////////////////////
CSkinAnimObject::~CSkinAnimObject(void)
{
	Clear();
}

CSkinAnimObject::CSkinAnimObject(void)
{
	m_pXSkinAnim		= NULL;
	m_bShow				= true;

	m_iNowAniNameIndex		= 0;

	//m_pXET			= NULL;
	//m_pXETWeapon		= NULL;
	//m_bWireframeMode	= false;
	//m_bIsLight		= true;
	//m_bIsAnimPlay		= true;
	//m_bIsMotion		= false;
	//m_ImpactNowAnimTime	= 0.0f;
	//m_bIsBounding		= false;
	//m_bShowAttackBox	= false;

	//m_AnimPlaytype	= CKTDGXSkinAnim::XAP_LOOP;

	////Light 위치표기 메쉬
	//m_pXMeshLight			= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Sun.X" );
	//m_pMatrixLight		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	//m_pMatrixLight->Scale( 0.5f, 0.5f, 0.5f );

	////충돌박스 메쉬
	//m_pXMeshSphere		= g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"Bounding_Sphere.X" );
	//m_pMatrixSphere		= new CKTDGMatrix( g_pKTDXApp->GetDevice() );

	//TEX_STAGE_DATA texStageData;
	//m_vecTexStageData.push_back( texStageData );
	//m_vecTexStageData.push_back( texStageData );
	//m_vecTexStageData.push_back( texStageData );

	//m_pMatrix = NULL;
	//m_RotWeapon = D3DXVECTOR3( 0, 0, 0 );

	////D3DXCreateCylinder( g_pKTDXApp->GetDevice(), 100.0f, 100.0f, 200.0f, 10, 10, &m_pSMesh, NULL );
	////m_pSMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
}



HRESULT CSkinAnimObject::OnFrameMove( double fTime, float fElapsedTime )
{


	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;


	if( false == m_bShow )
		return S_OK;

	//CKTDGXRenderer::RenderParam* pRenderParam = m_pXSkinAnim->GetRenderParam();
	//*pRenderParam = m_RenderParam;

	D3DXVECTOR3 position = m_pXSkinAnim->GetMatrix().GetPos();
	//모션 이동
	position.x += m_pXSkinAnim->GetMotionOffset().x/3.0f;
	position.z += m_pXSkinAnim->GetMotionOffset().z/3.0f;

	//m_pXSkinAnim->GetMatrix().Scale( m_vScale );
	m_pXSkinAnim->GetMatrix().Move( position );	
	m_pXSkinAnim->OnFrameMove( fTime, fElapsedTime );

	


	//m_vecPlayingTimedXMeshPlayer


	return S_OK;
}

HRESULT CSkinAnimObject::OnFrameRender()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return S_OK;

	if( false == m_bShow )
		return S_OK;



	m_pXSkinAnim->OnFrameRender();
	
	return S_OK;
}



void CSkinAnimObject::Clear()
{
	std::map<std::wstring, CKTDXDeviceXSkinMesh*>::iterator itr;
	for( itr = m_mapSkinMesh.begin(); itr != m_mapSkinMesh.end(); ++itr )
	{
		m_pXSkinAnim->RemoveModelXSkinMesh( itr->second );
		g_pKTDXApp->GetDeviceManager()->CloseDevice( itr->first );
	}
	m_mapSkinMesh.clear();
	
	SAFE_DELETE_KTDGOBJECT( m_pXSkinAnim );
	
}


CKTDGXRenderer::RenderParam* CSkinAnimObject::GetRenderParam()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return NULL;

	return m_pXSkinAnim->GetRenderParam();

}

void CSkinAnimObject::SetPos( const D3DXVECTOR3& vPos )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->GetMatrix().Move( vPos );
}

D3DXVECTOR3 CSkinAnimObject::GetPos()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return D3DXVECTOR3(0,0,0);

	return m_pXSkinAnim->GetMatrix().GetPos();
}


void CSkinAnimObject::SetRotationDegree( const D3DXVECTOR3& vRotation )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->GetMatrix().RotateDegree( vRotation );
}

D3DXVECTOR3 CSkinAnimObject::GetRotationDegree()
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return D3DXVECTOR3(0,0,0);

	return m_pXSkinAnim->GetMatrix().GetRotateDegree();
}

bool CSkinAnimObject::SetAnimXSkinMesh( std::wstring fileName )
{
	Clear();

	// m_pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
	m_pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

	CKTDXDeviceXSkinMesh* pMotion = NULL;
	pMotion = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( fileName );

	if( pMotion != NULL )
	{
		m_pXSkinAnim->SetAnimXSkinMesh( pMotion );
		m_pXSkinAnim->AddModelXSkinMesh( pMotion );
		m_mapSkinMesh.insert( std::make_pair( fileName, pMotion ) );

		return true;
	}

	return false;
}

void CSkinAnimObject::ChangeAnim( std::wstring animName )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->ChangeAnim( animName.c_str(), false );
	
	m_iNowAniNameIndex = m_pXSkinAnim->GetIndexByName( animName.c_str() );
	//m_pXSkinAnim->Play( m_AnimPlaytype );
}

void CSkinAnimObject::ChangeAnim( int index )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;

	m_pXSkinAnim->ChangeAnim( index, false );
	m_iNowAniNameIndex = index;
	//m_pXSkinAnim->Play( m_AnimPlaytype );
}

void CSkinAnimObject::Play( CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
		return;
	
	m_pXSkinAnim->ChangeAnim( m_iNowAniNameIndex, false );
	m_pXSkinAnim->Play( playType );	
}


bool CSkinAnimObject::GetAnimNameList( std::vector<std::wstring>& vecAnimNameList )
{
	if( m_pXSkinAnim == NULL || m_pXSkinAnim->GetAnimXSkinMesh() == NULL )
	{
		return false;
	}

	LPD3DXANIMATIONCONTROLLER	pAC;
	LPD3DXANIMATIONSET			pAnimSet;
	UINT			aniNum;
	LPCSTR			szName;
	std::wstring	wstrName;
	WCHAR			wszName[128] = L"";

	pAC		= m_pXSkinAnim->GetAnimXSkinMesh()->GetCloneAC();
	aniNum	= pAC->GetNumAnimationSets();

	for( UINT i = 0; i < aniNum; ++i )
	{
		pAC->GetAnimationSet( i, &pAnimSet );
		szName = pAnimSet->GetName();

		MultiByteToWideChar( CP_ACP, 0, szName, -1, wszName, MAX_PATH);

		wstrName = wszName;
		vecAnimNameList.push_back( wstrName );

		pAnimSet->Release();
	}

	SAFE_RELEASE( pAC);

	return true;
}





//////////////////////////////////////////////////////////////////////////
void CALLBACK OnDialogEvent_DLG_MAIN( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"lua script\0*.lua\0X file\0*.x\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	CX2WorldToolMain* pWorldTool = (CX2WorldToolMain*) pUserContext;
	CWorldToolDialog* pDialog = pWorldTool->GetDialog( CX2WorldToolMain::DLG_MAIN );

	switch( nControlID )
	{
	case CX2WorldToolMain::IDC_BUTTON_FILE_OPEN_WORLD:
		{
			ShowWindow(g_hDlgLineMap, SW_HIDE);
			DestroyWindow(g_hDlgLineMap);
			//EndDialog(g_hDlg, 1);
			g_hDlgLineMap = NULL;
			pWorldTool->m_bShowLineMapModifier = false;			

			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pWorldTool->LoadWorld( ofn.lpstrFile );
				pWorldTool->LoadLineMapInfo();
				g_pKTDXApp->SkipFrame();
			}
		} break;

	case CX2WorldToolMain::IDC_BUTTON_FILE_OPEN_LINEMAP:
		{
			ShowWindow(g_hDlgLineMap, SW_HIDE);
			DestroyWindow(g_hDlgLineMap);
			//EndDialog(g_hDlg, 1);
			g_hDlgLineMap = NULL;
			pWorldTool->m_bShowLineMapModifier = false;			

			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pWorldTool->LoadLineMap( ofn.lpstrFile );
				pWorldTool->LoadLineMapInfo();
				g_pKTDXApp->SkipFrame();
			}
		} break;


	case CX2WorldToolMain::IDC_BUTTON_FILE_SAVE_WORLD:
		{
			if( TRUE == GetSaveFileName( &ofn ) )
			{
				if( E_FAIL == pWorldTool->SaveWorld( ofn.lpstrFile ) )
				{
					MessageBox( g_pKTDXApp->GetHWND(), L"읽기전용파일이 아닌지 확인해주세요.", L"파일저장실패", MB_OK );
				}
			}
		} break;


	case CX2WorldToolMain::IDC_COMBO_LINEMAP_FILENAME:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			pWorldTool->m_eLineMapFileNameType = (CX2WorldToolMain::LINEMAP_FILENAME_TYPE) (int)pItem->pData;

		} break;


	case CX2WorldToolMain::IDC_CHECK_SQUARE_CAMERA:
		{
			if( NULL != pWorldTool->m_pGame )
			{
				pWorldTool->m_pGame->m_bSquareCamera = !pWorldTool->m_pGame->m_bSquareCamera;
				//{{ 2009.02.26 김태완 : 광장 카메라 체크시 시점 1200으로 자동 전환
				if(pWorldTool->m_pGame->m_bSquareCamera)
				{
					pWorldTool->m_pGame->m_SquareCameraParam.m_fEyeDistance = 1200.0f;
					pWorldTool->UpdateSquareCameraUI();
				}
				else
				{
					pWorldTool->m_pGame->m_SquareCameraParam.m_fEyeDistance = 1500.0f;
					pWorldTool->UpdateSquareCameraUI();
				}
				//}}
			}

		} break;

    case CX2WorldToolMain::IDC_CHECK_ORTHOFREE:
        {
            if( NULL != pWorldTool->m_pGame )
            {
                pWorldTool->m_pGame->m_bOrthoFreeCamera = !pWorldTool->m_pGame->m_bOrthoFreeCamera;

                pWorldTool->m_pGame->m_fOrthoScale = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
                pDialog->ClearFocus();

                g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
                    g_pKTDXApp->GetDGManager()->GetFar(), false, pWorldTool->m_pGame->m_fOrthoScale );
#if 0 
                if(pWorldTool->m_pGame->m_bOrthoFreeCamera == false)
                    pWorldTool->m_pGame->SetFixedCamera(true);
                else
                    pWorldTool->m_pGame->SetFixedCamera(false);
#endif
            }
        }
        break;

    case CX2WorldToolMain::IDC_EDIT_ORTHOSCALE:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                //case EVENT_EDITBOX_CHANGE:
                {
                    if( NULL != pWorldTool->m_pGame )
                    {
                        pWorldTool->m_pGame->m_fOrthoScale = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
                        pDialog->ClearFocus();
                        
                        g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
                        g_pKTDXApp->GetDGManager()->GetFar(), false, pWorldTool->m_pGame->m_fOrthoScale );
#if 0 
                        if(pWorldTool->m_pGame->m_bOrthoFreeCamera == false)
                            pWorldTool->m_pGame->SetFixedCamera(true);
                        else
                            pWorldTool->m_pGame->SetFixedCamera(false);
#endif
                    }
                } break;
            }
        }
        break;

	case CX2WorldToolMain::IDC_EDIT_SQUARE_CAMERA_DISTANCE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					if( NULL != pWorldTool->m_pGame )
					{
						pWorldTool->m_pGame->m_SquareCameraParam.m_fEyeDistance = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
						pDialog->ClearFocus();

						pWorldTool->m_pGame->SetCamera();
						pWorldTool->UpdateSquareCameraUI();
					}
				} break;
			}
		} break;


	case CX2WorldToolMain::IDC_EDIT_SQUARE_CAMERA_LOOKAT_HEIGHT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					if( NULL != pWorldTool->m_pGame )
					{
						pWorldTool->m_pGame->m_SquareCameraParam.m_fLookAtHeight = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
						pDialog->ClearFocus();

						pWorldTool->m_pGame->SetCamera();
						pWorldTool->UpdateSquareCameraUI();

					}
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_SQUARE_CAMERA_EYE_HEIGHT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					if( NULL != pWorldTool->m_pGame )
					{
						pWorldTool->m_pGame->m_SquareCameraParam.m_fEyeHeight = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
						pDialog->ClearFocus();

						pWorldTool->m_pGame->SetCamera();
						pWorldTool->UpdateSquareCameraUI();

					}
				} break;
			}
		} break;


	case CX2WorldToolMain::IDC_EDIT_CAMERA_MOVE_SPEED:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					pWorldTool->GetCamera().m_fMoveSpeed = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_OBJECT_MOVE_SPEED:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					pWorldTool->m_fObjectMoveSpeed = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pDialog->ClearFocus();
				} break;
			}

		} break;

	case CX2WorldToolMain::IDC_BUTTON_HELP:
		{

			
			wstring wstrHelp = L"";


			wstrHelp += L"X2의 dat폴더를 복사해서 넣어주세요.(enum.lua, npc mesh 파일 등등 때문에) \n";
			wstrHelp += L"작업할 월드 스크립트 파일을 dat폴더에서 삭제해주세요. (파일이름이 중복되면 안되니까~) \n";
			wstrHelp += L"작업할 라인맵 스크립트와 월드 스크립트, 던전 스크립트 세 가지를 모두 worldtool 폴더에 복사해서 작업해주세요~ \n";
			wstrHelp += L"에디트 박스에 입력후 반드시 엔터를 눌러주세요.\n";
			wstrHelp += L"라인맵 파일명이 잘못된 경우 뻗으니 조심해주세요.\n";
			wstrHelp += L"\n";

			//wstrHelp += L"같은 파일을 static mesh와 skin mesh로 두번 로딩하면 뻗으므로 조심해주세요 ^^ \n";
			wstrHelp += L"OT_STATIC_MESH: 움직임이 없는 고정 메쉬 오브젝트\n";
			wstrHelp += L"OT_SKIN_MESH: 움직임이 있는 메쉬 오브젝트\n";
			wstrHelp += L"OT_SKY_CLOUD: 스카이 박스와 구름 메쉬\n";
			wstrHelp += L"\n";

			wstrHelp += L"SKY_DOME_0, 1, 2: 스카이 박스와 구름 메쉬\n";
			wstrHelp += L"WORLD_BASE_0, 1, 2: 나무, 배경, 발판과 같은 큰 메쉬들, 배경이 되는 물체들\n";
			wstrHelp += L"WORLD_OBJECT_0, 1, 2: 풀, 꽃 같은 작은 물체들\n";
			wstrHelp += L"SKY_DOME_0가 가장 뒤쪽에 그려지고 WORLD_OBJECT_2가 가장 앞쪽에 그려진다.\n";
			wstrHelp += L"\n";



			wstrHelp += L" 1 :	인터페이스를 토글 \r\n";
			wstrHelp += L" 2 :	라인맵 그리기 토글 \r\n";

			wstrHelp += L"F11:	play mode <--> edit mode\n";
			wstrHelp += L"tab:	edit mode에서 worldtool mode <--> effect tool mode\n";

			wstrHelp += L"listbox에서 선택 + delete		: 선택된 것 삭제 \n";
			wstrHelp += L"w, a, s, d, q, e				: 카메라 이동 \n";			
			wstrHelp += L"right click + mouse move		: 카메라 회전 \n";
			wstrHelp += L"i, j, k, l, y, h				: 선택된 오브젝트 이동 \n";	

			wstrHelp += L" +, - :		볼륨조절";


			wstrHelp += L"----- EFFECT TOOL ------ \n";	
			wstrHelp += L"F5 :	load xmeshplayer, F6 : load particle \n";	
			wstrHelp += L"e :		리스트박스에서 선택된 모션을 취한다 \n";
			wstrHelp += L"l :       레이어 메뉴\n";
			

			MessageBox( g_pKTDXApp->GetHWND(), wstrHelp.c_str(), L"HELP", MB_OK );
		} break;

	//case CX2WorldToolMain::IDC_RADIO_ELSWORD:
	//	{
	//		if( CX2Unit::UC_ELSWORD_SWORDMAN != pWorldTool->GetCurrUnitClass() )
	//		{
	//			pWorldTool->GetCurrUnitClass() = CX2Unit::UC_ELSWORD_SWORDMAN;
	//			pWorldTool->ChangeUserUnit(0);
	//		}
	//	} break;
	//case CX2WorldToolMain::IDC_RADIO_ARME:
	//	{
	//		if( CX2Unit::UC_ARME_VIOLET_MAGE != pWorldTool->GetCurrUnitClass() )
	//		{
	//			pWorldTool->GetCurrUnitClass() = CX2Unit::UC_ARME_VIOLET_MAGE;
	//			pWorldTool->ChangeUserUnit(1);
	//		}
	//	} break;
	//case CX2WorldToolMain::IDC_RADIO_LIRE:
	//	{
	//		if( CX2Unit::UC_LIRE_ELVEN_RANGER != pWorldTool->GetCurrUnitClass() )
	//		{
	//			pWorldTool->GetCurrUnitClass() = CX2Unit::UC_LIRE_ELVEN_RANGER;
	//			pWorldTool->ChangeUserUnit(2);
	//		}
	//	} break;

	case CX2WorldToolMain::IDC_EDIT_WORLD_LIGHT_COLOR:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fLightColor[3];
					int iLightColor[3];

					pWorldTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iLightColor[0], 3 );
					if( NULL != pWorldTool->m_pGame && NULL != pWorldTool->m_pGame->GetWorld() )
					{
						fLightColor[0] = (float)iLightColor[0]/255.f;
						fLightColor[1] = (float)iLightColor[1]/255.f;
						fLightColor[2] = (float)iLightColor[2]/255.f;

						pWorldTool->m_pGame->GetWorld()->SetLightColor_LUA( fLightColor[0], fLightColor[1], fLightColor[2] );
					}
					pDialog->ClearFocus();

				} break;
			}

		} break;


	case CX2WorldToolMain::IDC_EDIT_ORIGIN_WORLD_LIGHT_COLOR:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fLightColor[3];
					int iLightColor[3];

					pWorldTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iLightColor[0], 3 );
					if( NULL != pWorldTool->m_pGame && NULL != pWorldTool->m_pGame->GetWorld() )
					{
						fLightColor[0] = (float)iLightColor[0]/255.f;
						fLightColor[1] = (float)iLightColor[1]/255.f;
						fLightColor[2] = (float)iLightColor[2]/255.f;

						pWorldTool->m_pGame->GetWorld()->SetOriginColor_LUA( fLightColor[0], fLightColor[1], fLightColor[2] );
					}
					pDialog->ClearFocus();

				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_RED:
		{
			int iOriginLightColor[3] = { 0, };
			D3DXCOLOR coOriginLightColor = pWorldTool->m_pGame->GetWorld()->GetOriginColor();
			iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
			iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
			iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

			iOriginLightColor[0] = ((CDXUTSlider*)pControl)->GetValue();

			CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORIGIN_WORLD_LIGHT_COLOR );
			pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

			if( NULL != pWorldTool->m_pGame && NULL != pWorldTool->m_pGame->GetWorld() )
			{
				float fLightColor[3] = { 0.f, };
				fLightColor[0] = (float)iOriginLightColor[0]/255.f;
				fLightColor[1] = (float)iOriginLightColor[1]/255.f;
				fLightColor[2] = (float)iOriginLightColor[2]/255.f;

				pWorldTool->m_pGame->GetWorld()->SetOriginColor_LUA( fLightColor[0], fLightColor[1], fLightColor[2] );
			}

		} break;



	case CX2WorldToolMain::IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_GREEN:
		{
			int iOriginLightColor[3] = { 0, };
			D3DXCOLOR coOriginLightColor = pWorldTool->m_pGame->GetWorld()->GetOriginColor();
			iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
			iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
			iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

			iOriginLightColor[1] = ((CDXUTSlider*)pControl)->GetValue();

			CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORIGIN_WORLD_LIGHT_COLOR );
			pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

			if( NULL != pWorldTool->m_pGame && NULL != pWorldTool->m_pGame->GetWorld() )
			{
				float fLightColor[3] = { 0.f, };
				fLightColor[0] = (float)iOriginLightColor[0]/255.f;
				fLightColor[1] = (float)iOriginLightColor[1]/255.f;
				fLightColor[2] = (float)iOriginLightColor[2]/255.f;

				pWorldTool->m_pGame->GetWorld()->SetOriginColor_LUA( fLightColor[0], fLightColor[1], fLightColor[2] );
			}
		} break;


	case CX2WorldToolMain::IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE:
		{
			int iOriginLightColor[3] = { 0, };
			D3DXCOLOR coOriginLightColor = pWorldTool->m_pGame->GetWorld()->GetOriginColor();
			iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
			iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
			iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

			iOriginLightColor[2] = ((CDXUTSlider*)pControl)->GetValue();

			CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORIGIN_WORLD_LIGHT_COLOR );
			pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );


			if( NULL != pWorldTool->m_pGame && NULL != pWorldTool->m_pGame->GetWorld() )
			{
				float fLightColor[3] = { 0.f, };
				fLightColor[0] = (float)iOriginLightColor[0]/255.f;
				fLightColor[1] = (float)iOriginLightColor[1]/255.f;
				fLightColor[2] = (float)iOriginLightColor[2]/255.f;

				pWorldTool->m_pGame->GetWorld()->SetOriginColor_LUA( fLightColor[0], fLightColor[1], fLightColor[2] );
			}

		} break;


	case CX2WorldToolMain::IDC_EDIT_FADE_WORLD_LIGHT_COLOR:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fLightColor[3];
					int iLightColor[3];

					pWorldTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iLightColor[0], 3 );
					if( NULL != pWorldTool->m_pGame && NULL != pWorldTool->m_pGame->GetWorld() )
					{
						fLightColor[0] = (float)iLightColor[0]/255.f;
						fLightColor[1] = (float)iLightColor[1]/255.f;
						fLightColor[2] = (float)iLightColor[2]/255.f;

						pWorldTool->SetFadeWorldColor( D3DXCOLOR( fLightColor[0], fLightColor[1], fLightColor[2], 1.f ) );
					}
					pDialog->ClearFocus();

				} break;
			} 
		}break;


	}
}


void CALLBACK OnDialogEvent_DLG_OBJECT_LIST( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2WorldToolMain* pWorldTool = (CX2WorldToolMain*) pUserContext;

	switch( nControlID )
	{
	case CX2WorldToolMain::IDC_BUTTON_CLEAR_OBJECT_PALETTE:
		{
			pWorldTool->ClearXFilePalette();
		} break;

	case CX2WorldToolMain::IDC_BUTTON_REMOVE_OBJECT_FROM_WORLD:
		{
			pWorldTool->RemoveSelectedObjectFromWorld();

		} break;


	case CX2WorldToolMain::IDC_CHECK_RENDER_BOUNDING_SPHERE:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			pWorldTool->m_bRenderBoundingSphere = bChecked;
		} break;


	case CX2WorldToolMain::IDC_CHECK_REDNER_FLICKER:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			pWorldTool->m_bRenderFlicker = bChecked;
		} break;

	case CX2WorldToolMain::IDC_CHECK_ENABLE_CENTER_GREEN_DOT:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			pWorldTool->m_bCenterGreenDot = bChecked;

		} break;


	case CX2WorldToolMain::IDC_LIST_OBJECT_PALETTE:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					// 더블클릭은 처리하지 않는다
					//pWorldTool->AddPickedPaletteObjectToWorld();
				} break;
			} break;

		} break;

	case CX2WorldToolMain::IDC_BUTTON_ADD_OBJECT_TO_WORLD_AS_STATIC_MESH:
		{
			pWorldTool->AddPickedPaletteObjectToWorld( CX2WorldObject::OT_STATIC_MESH );
		} break;

	case CX2WorldToolMain::IDC_BUTTON_ADD_OBJECT_TO_WORLD_AS_SKIN_MESH:
		{
			pWorldTool->AddPickedPaletteObjectToWorld( CX2WorldObject::OT_SKIN_MESH );

		} break;


	case CX2WorldToolMain::IDC_BUTTON_RELOAD_OBJECT_IN_WORLD:
		{
			switch( pWorldTool->m_eLineMapFileNameType )
			{
			// 2009.02.25 김태완 : 디폴트를 SUFFIX로 변경
			default:
			case CX2WorldToolMain::LFT_SUFFIX:
				{
					pWorldTool->SaveTempLineMap( pWorldTool->m_wstrTempLineMapScript_Suffix.c_str() );
				}
				break;

//			default:
			case CX2WorldToolMain::LFT_PREFIX:
				{
					pWorldTool->SaveTempLineMap( pWorldTool->m_wstrTempLineMapScript_Prefix.c_str() );
				} break;
			}
			
			pWorldTool->SaveWorld( pWorldTool->m_wstrTempWorldScript.c_str() );
			pWorldTool->LoadWorld( pWorldTool->m_wstrTempWorldScript.c_str() );

		} break;


	case CX2WorldToolMain::IDC_LIST_OBJECT_IN_WORLD:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					pWorldTool->PickWorldObjectInListBox( true );
				} break;
			} break;
		} break;
	}
}

void CALLBACK OnDialogEvent_DLG_OBJECT_PROPERTY( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2WorldToolMain* pWorldTool = (CX2WorldToolMain*) pUserContext;
	CWorldToolDialog* pDialog = pWorldTool->GetDialog( CX2WorldToolMain::DLG_OBJECT_PROPERTY );

	vector<CWorldToolObject*>& vecpWorldToolObject = pWorldTool->GetPickedWorldToolObjectList();
    vector<CWorldToolObject*>& vecpWorldToolAllObject = pWorldTool->GetWorldToolObjectList();
    

	switch( nControlID )
	{
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
	//{{ 09.04.25 태완
	case CX2WorldToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						
						float fLightColor[3];
						int iLightColor[3];

						pWorldTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iLightColor[0], 3 );
						if( NULL != pWorldToolObject )
						{
							fLightColor[0] = (float)iLightColor[0]/255.f;
							fLightColor[1] = (float)iLightColor[1]/255.f;
							fLightColor[2] = (float)iLightColor[2]/255.f;

							pWorldToolObject->SetOriginalColorFactor( D3DXCOLOR(fLightColor[0], fLightColor[1], fLightColor[2], 1.f) );
						}
					
						pDialog->ClearFocus();

					}	
					

				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_SLIDER_OBJECT_LIGHT_COLOR_RED:
		{
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];

				int iOriginLightColor[3] = { 0, };
				D3DXCOLOR coOriginLightColor = pWorldToolObject->GetOriginalColorFactor();
				iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
				iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
				iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

				iOriginLightColor[0] = ((CDXUTSlider*)pControl)->GetValue();

				CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR );
				pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

				if( NULL != pWorldToolObject )
				{
					float fLightColor[3] = { 0.f, };
					fLightColor[0] = (float)iOriginLightColor[0]/255.f;
					fLightColor[1] = (float)iOriginLightColor[1]/255.f;
					fLightColor[2] = (float)iOriginLightColor[2]/255.f;

					pWorldToolObject->SetOriginalColorFactor( D3DXCOLOR(fLightColor[0], fLightColor[1], fLightColor[2], 1.f) );
				}
			}

		} break;

	case CX2WorldToolMain::IDC_SLIDER_OBJECT_LIGHT_COLOR_GREEN:
		{
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];

				int iOriginLightColor[3] = { 0, };
				D3DXCOLOR coOriginLightColor = pWorldToolObject->GetOriginalColorFactor();
				iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
				iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
				iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

				iOriginLightColor[1] = ((CDXUTSlider*)pControl)->GetValue();

				CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR );
				pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

				if( NULL != pWorldToolObject )
				{
					float fLightColor[3] = { 0.f, };
					fLightColor[0] = (float)iOriginLightColor[0]/255.f;
					fLightColor[1] = (float)iOriginLightColor[1]/255.f;
					fLightColor[2] = (float)iOriginLightColor[2]/255.f;

					pWorldToolObject->SetOriginalColorFactor( D3DXCOLOR(fLightColor[0], fLightColor[1], fLightColor[2], 1.f) );
				}
			}

		} break;


	case CX2WorldToolMain::IDC_SLIDER_OBJECT_LIGHT_COLOR_BLUE:
		{
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];

				int iOriginLightColor[3] = { 0, };
				D3DXCOLOR coOriginLightColor = pWorldToolObject->GetOriginalColorFactor();
				iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
				iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
				iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

				iOriginLightColor[2] = ((CDXUTSlider*)pControl)->GetValue();

				CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR );
				pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

				if( NULL != pWorldToolObject )
				{
					float fLightColor[3] = { 0.f, };
					fLightColor[0] = (float)iOriginLightColor[0]/255.f;
					fLightColor[1] = (float)iOriginLightColor[1]/255.f;
					fLightColor[2] = (float)iOriginLightColor[2]/255.f;

					pWorldToolObject->SetOriginalColorFactor( D3DXCOLOR(fLightColor[0], fLightColor[1], fLightColor[2], 1.f) );
				}
			}

		} break;
#endif COLORFACTOR_FOR_EVERY_WORLDMESH
		//}}
    case CX2WorldToolMain::IDC_CHECK_IS_FOGWORLD:
        {
            bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetIsFog( bChecked );
            }
        }
        break;

    case CX2WorldToolMain::IDC_CHECK_IS_FOG:
        {
            bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();            
            for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
                pWorldToolObject->SetIsFog( bChecked );
            }	
        }
        break;

    case CX2WorldToolMain::IDC_EDIT_FOG_NEAR_X:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                //case EVENT_EDITBOX_CHANGE:
                {
                    float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
                    for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                        pWorldToolObject->SetFogNearX(fPos);
                    }	
                    pDialog->ClearFocus();
                } break;
            }
        } break;
    case CX2WorldToolMain::IDC_EDIT_FOG_FAR_X:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                //case EVENT_EDITBOX_CHANGE:
                {
                    float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
                    for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                        pWorldToolObject->SetFogFarX(fPos);
                    }	
                    pDialog->ClearFocus();
                } break;
            }
        } break;
    case CX2WorldToolMain::IDC_EDIT_FOG_NEAR_Y:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                //case EVENT_EDITBOX_CHANGE:
                {
                    float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
                    for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                        pWorldToolObject->SetFogNearY(fPos);
                    }	
                    pDialog->ClearFocus();
                } break;
            }
        } break;
    case CX2WorldToolMain::IDC_EDIT_FOG_FAR_Y:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                //case EVENT_EDITBOX_CHANGE:
                {
                    float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
                    for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                        pWorldToolObject->SetFogFarY(fPos);
                    }	
                    pDialog->ClearFocus();
                } break;
            }
        } break;
    case CX2WorldToolMain::IDC_EDIT_FOG_DENSITY:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                //case EVENT_EDITBOX_CHANGE:
                {
                    float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
                    for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                        pWorldToolObject->SetFogDensity(fPos);
                    }	
                    pDialog->ClearFocus();
                } break;
            }
        } break;
    case CX2WorldToolMain::IDC_EDIT_FOG_DENSITY_OBJECT:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                //case EVENT_EDITBOX_CHANGE:
                {
                    float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
                    for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
                        pWorldToolObject->SetFogDensity(fPos);
                    }	
                    pDialog->ClearFocus();
                } break;
            }
        } break;

    case CX2WorldToolMain::IDC_EDIT_ORIGIN_FOG_COLOR:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                {
                    //float fLightColor[3];
                    int iLightColor[3];

                    pWorldTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iLightColor[0], 3 );
                    for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                        pWorldToolObject->SetFogColor(iLightColor[0]/255.0f, iLightColor[1]/255.0f, iLightColor[2]/255.0f);
                    }	
                    pWorldTool->SetFogColor(iLightColor[0]/255.0f, iLightColor[1]/255.0f, iLightColor[2]/255.0f);
                    pDialog->ClearFocus();

                } break;
            }
        } break;

    case CX2WorldToolMain::IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED:
        {
            int iOriginLightColor[3] = { 0, };
            D3DXCOLOR coOriginLightColor = pWorldTool->GetFogColor();
            iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
            iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
            iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

            iOriginLightColor[0] = ((CDXUTSlider*)pControl)->GetValue();

            CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORIGIN_FOG_COLOR );
            pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
            }	
            pWorldTool->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);

        } break;



    case CX2WorldToolMain::IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN:
        {
            int iOriginLightColor[3] = { 0, };
            D3DXCOLOR coOriginLightColor = pWorldTool->GetFogColor();
            iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
            iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
            iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

            iOriginLightColor[1] = ((CDXUTSlider*)pControl)->GetValue();

            CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORIGIN_FOG_COLOR );
            pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
            }	
            pWorldTool->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
        } break;


    case CX2WorldToolMain::IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE:
        {
            int iOriginLightColor[3] = { 0, };
            D3DXCOLOR coOriginLightColor = pWorldTool->GetFogColor();
            iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
            iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
            iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

            iOriginLightColor[2] = ((CDXUTSlider*)pControl)->GetValue();

            CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORIGIN_FOG_COLOR );
            pWorldTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );


            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
            }	
            pWorldTool->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);

        } break;

    case CX2WorldToolMain::IDC_SLIDER_FOG_NEAR_X:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_FOG_NEAR_X );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%d", iPos );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogNearX(static_cast<float>(iPos));
            }	
            pDialog->ClearFocus();

        } break;

    case CX2WorldToolMain::IDC_SLIDER_FOG_FAR_X:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_FOG_FAR_X );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%d", iPos );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogFarX(static_cast<float>(iPos));
            }	
            pDialog->ClearFocus();

        } break;

    case CX2WorldToolMain::IDC_SLIDER_FOG_NEAR_Y:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_FOG_NEAR_Y );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%d", iPos );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogNearY(static_cast<float>(iPos));
            }	
            pDialog->ClearFocus();

        } break;

    case CX2WorldToolMain::IDC_SLIDER_FOG_FAR_Y
        :
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_FOG_FAR_Y );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%d", iPos );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogFarY(static_cast<float>(iPos));
            }	
            pDialog->ClearFocus();

        } break;

    case CX2WorldToolMain::IDC_SLIDER_FOG_DENSITY:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_FOG_DENSITY );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", (float)iPos * 0.01f );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogDensity((float)iPos * 0.01f);
            }	
            pDialog->ClearFocus();

        } break;

    case CX2WorldToolMain::IDC_SLIDER_FOG_DENSITY_OBJECT:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_FOG_DENSITY_OBJECT );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", (float)iPos * 0.01f );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
                pWorldToolObject->SetFogDensity((float)iPos * 0.01f);
            }	
            pDialog->ClearFocus();

        } break;
        
#ifdef ADD_ALPHATESTENABLE
	case CX2WorldToolMain::IDC_CHECK_IS_NOT_CULL:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				if( bChecked == true )
				{
					pWorldToolObject->GetRenderParam()->cullMode = D3DCULL_NONE;
				}
				else
				{
					pWorldToolObject->GetRenderParam()->cullMode = D3DCULL_CCW;
				}
				
			}	
		}
		break;
	case CX2WorldToolMain::IDC_CHECK_IS_ALPHATEST_ENABLE:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->GetRenderParam()->bAlphaTestEnable = bChecked;
			}	
		}
		break;
#endif

	case CX2WorldToolMain::IDC_CHECK_IS_ALPHA_OBJECT:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetIsAlphaObject( bChecked );
			}			

		} break;

	case CX2WorldToolMain::IDC_CHECK_ADDITIVE_BLEND:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetAdditiveBlend( bChecked );
			}			
		} break;

	case CX2WorldToolMain::IDC_CHECK_CAN_HIDE:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetCanHide( bChecked );
			}			
		} break;

	case CX2WorldToolMain::IDC_CHECK_NOT_RENDER:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetNotRender( bChecked );
			}			
		} break;

	case CX2WorldToolMain::IDC_CHECK_USE_WORLD_COLOR:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetUseWorldColor( bChecked );
				if( false == bChecked )
				{
					pWorldToolObject->SetColorFactor( D3DXCOLOR(1, 1, 1, 1) );
				}
			}			
		} break;


	case CX2WorldToolMain::IDC_EDIT_POS_X:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
			//case EVENT_EDITBOX_CHANGE:
				{
					float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];

						D3DXVECTOR3 vPos = pWorldToolObject->GetPosition();

						vPos.x = fPos;
						pWorldToolObject->SetPosition( vPos );
					}	
					pDialog->ClearFocus();
				} break;

			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_POS_Y:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
			//case EVENT_EDITBOX_CHANGE:
				{
					float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];

						D3DXVECTOR3 vPos = pWorldToolObject->GetPosition();

						vPos.y = fPos;
						pWorldToolObject->SetPosition( vPos );

					}
					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_POS_Z:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
			//case EVENT_EDITBOX_CHANGE:
				{
					float fPos = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];

						D3DXVECTOR3 vPos = pWorldToolObject->GetPosition();

						vPos.z = fPos;
						pWorldToolObject->SetPosition( vPos );

					}
					pDialog->ClearFocus();
				} break;
			}
		} break;


	case CX2WorldToolMain::IDC_EDIT_SCALE_X:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fScale = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						

						D3DXVECTOR3 vScale = pWorldToolObject->GetScale();
						vScale.x = fScale;
						pWorldToolObject->SetScale(vScale);
						
					}
					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_SCALE_Y:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fScale = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						
						D3DXVECTOR3 vScale = pWorldToolObject->GetScale();
						vScale.y = fScale;
						pWorldToolObject->SetScale(vScale);
						
					}
					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_SCALE_Z:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fScale = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
					
						D3DXVECTOR3 vScale = pWorldToolObject->GetScale();
						vScale.z = fScale;
						pWorldToolObject->SetScale(vScale);
					}
					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_COMBO_BILLBOARD:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();

			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				CX2WorldObject* pWorldObject = pWorldToolObject->m_pWorldObject;
				if( NULL == pWorldObject )
					continue;

				switch( pWorldObject->GetObjectType() )
				{
				case CX2WorldObject::OT_STATIC_MESH:
					{
						CX2WorldObjectMesh* pStaticMesh = (CX2WorldObjectMesh*) pWorldObject;
						pStaticMesh->SetBillboardType( (CKTDGMatrix::BILLBOARD_TYPE) (int)pItem->pData );
					} break;

				case CX2WorldObject::OT_SKIN_MESH:
					{ 
						CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) pWorldObject;
						pSkinMesh->SetBillboardType( (CKTDGMatrix::BILLBOARD_TYPE) (int)pItem->pData );
					} break;

				default:
					break;
				}
			}

		} break;

	case CX2WorldToolMain::IDC_COMBO_LAYER:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();

			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetLayer( (int)pItem->pData );
			}

		} break;


	case CX2WorldToolMain::IDC_COMBO_OBJECT_TYPE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();

			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetObjectType( (CX2WorldObject::OBJECT_TYPE)(int)pItem->pData );
			}		
			pWorldTool->UpdateWorldToolObjectList();

			//# 오브젝트 변경시 UI 가이드 처리
			//# 1. TEX0 UV SPEED 설정가능 표시
			if( pItem != NULL )
			{
				switch( (int)pItem->pData )
				{
				case CX2WorldObject::OT_STATIC_MESH:
				case CX2WorldObject::OT_SKIN_MESH:
				case (CX2WorldObject::OT_REACT_MESH+1):
					{
						CWorldToolDialog* pDialog = pWorldTool->GetDialog(CX2WorldToolMain::DLG_OBJECT_PROPERTY);
						if( pDialog != NULL )
						{
							WCHAR wszMSG[10] = L"";
							pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetText(wszMSG);
							pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetEnabled(true);
						}				
						break;
					}					
				default:
					{
						CWorldToolDialog* pDialog = pWorldTool->GetDialog(CX2WorldToolMain::DLG_OBJECT_PROPERTY);
						if( pDialog != NULL )
						{
							std::wstring wstrMSG = pItem->strText;
							wstrMSG += L" 지원안함";
							pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetText(wstrMSG.c_str(), true);
							pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetEnabled(false);
						}				
					}				
					break;
				}
			}
			 
		} break;
		
	case CX2WorldToolMain::IDC_EDIT_ROTATION_SPEED: // sky, cloud
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						pWorldToolObject->SetRotationSpeed( fValue );
					}

					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_OCCATIONAL_EVENT_TIME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					bool bUseTwoNumbersForRandom = false;

					if( true == bUseTwoNumbersForRandom )
					{
						float fNumbers[2] = { 0, };
						((CDXUTEditBox*)pControl)->ParseFloatArray( &fNumbers[0], 2 );
						((CDXUTEditBox*)pControl)->SetTextFloatArray( &fNumbers[0], 2 );

						for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
						{
							CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
							pWorldToolObject->SetOccasionalEventTime( fNumbers[0], fNumbers[1] );
						}
					}
					else
					{
						float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
						WCHAR wszText[32] = L"";
						swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%f", fValue );
						((CDXUTEditBox*)pControl)->SetText( wszText );

						for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
						{
							CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
							pWorldToolObject->SetOccasionalEventTime( fValue, fValue );
						}
					}

					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_OCCATIONAL_EVENT_RATE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					bool bUseTwoNumbersForRandom = false;

					if( true == bUseTwoNumbersForRandom )
					{
						int iNumbers[2] = { 0, };
						pWorldTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*) &iNumbers[0], 2 );
						pWorldTool->SetTextIntArrayToEditBox( ((CDXUTEditBox*)pControl), (int*) &iNumbers[0], 2 );

						for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
						{
							CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
							pWorldToolObject->SetOccasionalPlayRate( iNumbers[0], iNumbers[1] );
						}
					}
					else
					{
						int iValue = (int)_wtoi( ((CDXUTEditBox*)pControl)->GetText() );
						WCHAR wszText[32] = L"";
						swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%d", iValue );
						((CDXUTEditBox*)pControl)->SetText( wszText );

						for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
						{
							CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
							pWorldToolObject->SetOccasionalPlayRate( iValue, iValue );
						}
					}

					pDialog->ClearFocus();
				} break;
			}
		} break;


	case CX2WorldToolMain::IDC_EDIT_ROTATION_DEGREE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fNumbers[3] = { 0, 0, 0 };
					((CDXUTEditBox*)pControl)->ParseFloatArray( &fNumbers[0], 3 );
					((CDXUTEditBox*)pControl)->SetTextFloatArray( &fNumbers[0], 3 );

					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						D3DXVECTOR3 vRotDegree( fNumbers[0], fNumbers[1], fNumbers[2] );
						pWorldToolObject->SetRotationDegree( vRotDegree );
					}

					pDialog->ClearFocus();
				} break;
			}

		} break;

	case CX2WorldToolMain::IDC_EDIT_TEX0_UV_SPEED:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fNumbers[2] = { 0, 0 };
					((CDXUTEditBox*)pControl)->ParseFloatArray( &fNumbers[0], 2 );
					((CDXUTEditBox*)pControl)->SetTextFloatArray( &fNumbers[0], 2 );

					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						pWorldToolObject->SetTex0UVSpeed( fNumbers[0], fNumbers[1] );
					}

					pDialog->ClearFocus();
				} break;
			}

		} break;

	case CX2WorldToolMain::IDC_EDIT_SKIN_ANIM_NAME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					wstring wstrAniName = ((CDXUTEditBox*)pControl)->GetText();

					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						pWorldToolObject->SetAnimationName( wstrAniName );
					}

					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_COMBO_SKIN_ANIM_PLAY_TYPE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetPlayType( (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE) (int)pItem->pData );
			}

		} break;

	//{{ 09.03.02 김태완
	case CX2WorldToolMain::IDC_EDIT_HIDE_DISTANCE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fDist = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );					
					for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
						pWorldToolObject->SetHideDistance(fDist);												
					}	
					pDialog->ClearFocus();
				} break;
			}
		} break;
	//}}
	//{{ 09.03.02 김태완

	case CX2WorldToolMain::IDC_CHECK_FORCE_SHOW:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetForceShow( bChecked );
			}			
		} break;
	//}}

	}

}


void CALLBACK OnDialogEvent_DLG_EFFECT_TOOL( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2WorldToolMain* pWorldTool = (CX2WorldToolMain*) pUserContext;
	CWorldToolDialog* pDialog = pWorldTool->GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL );

	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"lua script\0*.lua\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	switch( nControlID )
	{
	case CX2WorldToolMain::IDC_BUTTON_LOAD_EFFECT_SET:
		{
			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pWorldTool->On_IDC_BUTTON_LOAD_XMESH_PLAYER();
				pWorldTool->On_IDC_BUTTON_LOAD_PARTICLE();

				pWorldTool->LoadEffectSet( ofn.lpstrFile );
			}
		} break;

	case CX2WorldToolMain::IDC_BUTTON_SAVE_EFFECT_SET:
		{
			if( TRUE == GetSaveFileName( &ofn ) )
			{
				pWorldTool->SaveEffectSet( ofn.lpstrFile );				
			}
		} break;

	case CX2WorldToolMain::IDC_BUTTON_LOAD_XMESH_PLAYER:
		{
			pWorldTool->On_IDC_BUTTON_LOAD_XMESH_PLAYER();
		} break;

	case CX2WorldToolMain::IDC_BUTTON_LOAD_PARTICLE:
		{
			pWorldTool->On_IDC_BUTTON_LOAD_PARTICLE();
		} break;

	case CX2WorldToolMain::IDC_BUTTON_READY_EFFECT_DEVICES:
		{
			pWorldTool->ReadyEffectDevices();
		} break;


	case CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_PALETTE:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();

					CX2WorldToolMain::TimedXMeshPlayer timedXMeshPlayer;
					timedXMeshPlayer.m_wstrXMeshPlayerName = pItem->strText;
					pWorldTool->m_vecTimedXMeshPlayer.push_back( timedXMeshPlayer );

					CWorldToolDialog* pDialog	= pWorldTool->GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL );
					CDXUTListBox* pListBox		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_LOADED );

					pListBox->AddItem( pItem->strText, NULL );
					pListBox->SelectItem( pListBox->GetSize() -1 );
					
				} break;

			case EVENT_LISTBOX_ITEM_RCLK:
			case EVENT_LISTBOX_ITEM_DBRCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
					// 2. 메모장을 띄워 엑스메쉬뷰 텍스트 파일을 띄워준다.

					DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();

					// 1.
					if( pItem != NULL && wcslen( pItem->strText ) > 0 )
					{
						char pszText[MAX_PATH] = "";
						::WideCharToMultiByte( CP_ACP, 0, pItem->strText, -1, pszText, 255, NULL, NULL );

						HGLOBAL hmem = ::GlobalAlloc( GHND, ::strlen(pszText)+1 );
						char* pszTemp = (char*)::GlobalLock(hmem);
						::memcpy( pszTemp, pszText, ::strlen(pszText)+1 );
						::GlobalUnlock( hmem );
						if( ::OpenClipboard( g_pKTDXApp->GetHWND() ) == TRUE )
						{
							::EmptyClipboard();
							::SetClipboardData( CF_TEXT, hmem );
							::CloseClipboard();
						}

						// 2.
						// :: 더블 클릭일대만 실행시킴.
						HWND hNote = NULL;
						hNote = ::FindWindow( NULL, L"XMeshPlayer_Test.txt - 메모장" );
						if( nEvent == EVENT_LISTBOX_ITEM_DBRCLK && hNote == NULL )
						{
							WCHAR wszCurDir[MAX_PATH] = L"";
							WCHAR wszProcessMsg[MAX_PATH] = L"";
							std::wstring wstrProcessMsg;

							GetCurrentDirectory( MAX_PATH, wszCurDir );

							wstrProcessMsg = L"notepad.exe \"";	// 메모장 실행 명령
							wstrProcessMsg += wszCurDir;	// 메모장에 전달해야할 텍스트 파일
							wstrProcessMsg += L"\\XMeshPlayer_Test.txt\"";

							STARTUPINFO si = { 0, };
							si.cb = sizeof( STARTUPINFO );
							PROCESS_INFORMATION pi;

							wcscpy( wszProcessMsg, wstrProcessMsg.c_str() );

							::CreateProcess(NULL, wszProcessMsg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
						}
					}
				}
				break;

				// XMeshPlayer preview start.
			case EVENT_LISTBOX_SELECTION:
				{
					DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();

					if( pItem != NULL )
					{
						pWorldTool->m_sPreviewXMeshPlayer.SetIsCreate( true );
						pWorldTool->m_sPreviewXMeshPlayer.SetXMeshName( pItem->strText );
					}					
				}
				break;
			} break;

		} break;

	case CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_LOADED:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_SELECTION:
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					pWorldTool->m_iPickedXMeshPlayerIndex = ((CDXUTListBox*)pControl)->GetSelectedIndex();
					pWorldTool->UpdatePickedXMeshPlayerUI();
				} break;
			} break;
		} break;


	case CX2WorldToolMain::IDC_LIST_PARTICLE_PALETTE:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();

					CX2WorldToolMain::TimedParticle timedParticle;
					timedParticle.m_wstrParticleName = pItem->strText;
					pWorldTool->m_vecTimedParticle.push_back( timedParticle );

					CWorldToolDialog* pDialog	= pWorldTool->GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL );
					CDXUTListBox* pListBox		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_PARTICLE_LOADED );

					pListBox->AddItem( pItem->strText, NULL );
					pListBox->SelectItem( pListBox->GetSize() -1 );


				} break;

			case EVENT_LISTBOX_ITEM_RCLK:
			case EVENT_LISTBOX_ITEM_DBRCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
					// 2. 메모장을 띄워 파티클 텍스트 파일을 띄워준다.

					DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();

					// 1.
					if( pItem != NULL && wcslen( pItem->strText ) > 0 )
					{
						char pszText[MAX_PATH] = "";
						::WideCharToMultiByte( CP_ACP, 0, pItem->strText, -1, pszText, 255, NULL, NULL );

						HGLOBAL hmem = ::GlobalAlloc( GHND, ::strlen(pszText)+1 );
						char* pszTemp = (char*)::GlobalLock(hmem);
						::memcpy( pszTemp, pszText, ::strlen(pszText)+1 );
						::GlobalUnlock( hmem );
						if( ::OpenClipboard( g_pKTDXApp->GetHWND() ) == TRUE )
						{
							::EmptyClipboard();
							::SetClipboardData( CF_TEXT, hmem );
							::CloseClipboard();
						}

						// 2.
						// :: 더블 클릭일대만 실행시킴.
						HWND hNote = NULL;
						hNote = ::FindWindow( NULL, L"Particle_Test.txt - 메모장" );
						if( nEvent == EVENT_LISTBOX_ITEM_DBRCLK && hNote == NULL )
						{
							WCHAR wszCurDir[MAX_PATH] = L"";
							WCHAR wszProcessMsg[MAX_PATH] = L"";
							std::wstring wstrProcessMsg;

							GetCurrentDirectory( MAX_PATH, wszCurDir );

							wstrProcessMsg = L"notepad.exe \"";	// 메모장 실행 명령
							wstrProcessMsg += wszCurDir;	// 메모장에 전달해야할 텍스트 파일
							wstrProcessMsg += L"\\Particle_Test.txt\"";

							STARTUPINFO si = { 0, };
							si.cb = sizeof( STARTUPINFO );
							PROCESS_INFORMATION pi;

							wcscpy( wszProcessMsg, wstrProcessMsg.c_str() );

							::CreateProcess(NULL, wszProcessMsg, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
						}
					}
				}
				break;

				//파티클 리스트 항목이 변경되면 프리뷰를 하기위해.
			case EVENT_LISTBOX_SELECTION:
				{
					DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();
					
					if( pItem != NULL )
					{
						pWorldTool->m_sPreviewParticle.SetIsCreate( true );
						pWorldTool->m_sPreviewParticle.SetParticleName( pItem->strText );
					}
				}
			} break;

		} break;

	case CX2WorldToolMain::IDC_LIST_PARTICLE_LOADED:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_SELECTION:
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					pWorldTool->m_iPickedParticleIndex = ((CDXUTListBox*)pControl)->GetSelectedIndex();
					pWorldTool->UpdatePickedParticleUI();
				} break;
			} break;
		} break;


	case CX2WorldToolMain::IDC_CHECK_USER_UNIT_STATE:
		{
			pWorldTool->m_bUseUserUnitState = ((CDXUTCheckBox*)pControl)->GetChecked();
		} break;

	case CX2WorldToolMain::IDC_LIST_USER_UNIT_STATE:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_SELECTION:
				{
					pWorldTool->m_iPickedUserUnitState = (int)*(int*)(((CDXUTListBox*)pControl)->GetSelectedItem()->pData);
					pWorldTool->UpdatePickedXMeshPlayerUI();
				} break;
			} break;
		} break;

	case CX2WorldToolMain::IDC_CHECK_FIX_SKIN_ANIM_POSITION:
		{
			pWorldTool->m_bFixPostionCharacterSkinAnim = ((CDXUTCheckBox*)pControl)->GetChecked();

		} break;

	case CX2WorldToolMain::IDC_CHECK_SKIN_ANIM_NAME:
		{
			pWorldTool->m_bShowCharacterSkinAnim = ((CDXUTCheckBox*)pControl)->GetChecked();
			pWorldTool->m_bShowUserUnit = !pWorldTool->m_bShowCharacterSkinAnim;

			if( NULL != pWorldTool->m_pCharacterSkinAnim )
				pWorldTool->m_pCharacterSkinAnim->SetShow( pWorldTool->m_bShowCharacterSkinAnim );
			pWorldTool->m_pGame->GetMyUnit()->SetShowObject( pWorldTool->m_bShowUserUnit );

		} break;

	case CX2WorldToolMain::IDC_EDIT_SKIN_ANIM_POSITION:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fNumbers[3] = { 0, };
					((CDXUTEditBox*)pControl)->ParseFloatArray( &fNumbers[0], 3 );

					pWorldTool->m_vFixedCharacterSkinAnimPosition.x = fNumbers[0];
					pWorldTool->m_vFixedCharacterSkinAnimPosition.y = fNumbers[1];
					pWorldTool->m_vFixedCharacterSkinAnimPosition.z = fNumbers[2];

					pDialog->ClearFocus();
				} break;
			}
		} break;


	case CX2WorldToolMain::IDC_EDIT_SKIN_ANIM_ROTATION:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fNumbers[3] = { 0, };
					((CDXUTEditBox*)pControl)->ParseFloatArray( &fNumbers[0], 3 );

					pWorldTool->m_vFixedCharacterSkinAnimRotation.x = fNumbers[0];
					pWorldTool->m_vFixedCharacterSkinAnimRotation.y = fNumbers[1];
					pWorldTool->m_vFixedCharacterSkinAnimRotation.z = fNumbers[2];

					pDialog->ClearFocus();
				} break;
			}
		} break;


	case CX2WorldToolMain::IDC_LIST_SKIN_ANIM_NAME:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_SELECTION:
				{
					if( NULL != pWorldTool->m_pCharacterSkinAnim )
					{
						DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();
						
						pWorldTool->m_pCharacterSkinAnim->ChangeAnim( pItem->strText );
						pWorldTool->m_pCharacterSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
						
					}				
				} break;
			} break;
		} break;

	case CX2WorldToolMain::IDC_CHECK_WORLD_COLOR:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			pWorldTool->m_bWorldColor = bChecked;

		} break;

	case CX2WorldToolMain::IDC_EDIT_WORLD_COLOR_TIME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_fWorldColorTime = fValue;
					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_WORLD_COLOR_FADE_TIME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_fWorldColorFadeTime = fValue;
					pDialog->ClearFocus();
				} break;
			}
		} break;




	//case CX2WorldToolMain::IDC_LIST_DAMAGE_EFFECT:
	//	{
	//		switch( nEvent )
	//		{				
	//		case EVENT_LISTBOX_SELECTION:
	//		case EVENT_LISTBOX_ITEM_DBLCLK:
	//			{
	//				DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();
	//				pWorldTool->SetDamageEffectName( pItem->strText );
	//			} break;
	//		} break;

	//	} break;

	}
}




void CALLBACK OnDialogEvent_DLG_EFFECT_PROPERTY( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2WorldToolMain* pWorldTool = (CX2WorldToolMain*) pUserContext;
	CWorldToolDialog* pDialog = pWorldTool->GetDialog( CX2WorldToolMain::DLG_EFFECT_PROPERTY );
	
	
	//CWorldToolDialog* pDialog_EffectList = pWorldTool->GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL );
	//CDXUTListBox* pListBox = pDialog_EffectList->GetListBox( CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_LOADED ); 
	//DXUTListBoxItem* pItem = pListBox->GetItem( pWorldTool->m_iPickedXMeshPlayerIndex );
	////DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
	//if( NULL == pItem )
	//	return;
	//
	
	switch( nControlID )
	{
	case CX2WorldToolMain::IDC_EDIT_MESH_PLAYER_DELAY:
		{
			
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pWorldTool->m_iPickedXMeshPlayerIndex < 0 || pWorldTool->m_iPickedXMeshPlayerIndex > (int)pWorldTool->m_vecTimedXMeshPlayer.size() -1 )
						return;


					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedXMeshPlayer[ pWorldTool->m_iPickedXMeshPlayerIndex ].m_fDelayTimeToStart = fValue;

					pDialog->ClearFocus();
				} break;
			}

		} break;

	case CX2WorldToolMain::IDC_CHECK_MESH_PLAYER_TRACE:
		{
			if( pWorldTool->m_iPickedXMeshPlayerIndex < 0 || pWorldTool->m_iPickedXMeshPlayerIndex > (int)pWorldTool->m_vecTimedXMeshPlayer.size() -1 )
				return;
			
			pWorldTool->m_vecTimedXMeshPlayer[ pWorldTool->m_iPickedXMeshPlayerIndex ].m_bTraceUnit = 
																			((CDXUTCheckBox*)pControl)->GetChecked();
		} break;


	case CX2WorldToolMain::IDC_CHECK_PARTICLE_TRACE:
		{
			if( pWorldTool->m_iPickedParticleIndex < 0 || pWorldTool->m_iPickedParticleIndex > (int)pWorldTool->m_vecTimedParticle.size() - 1 )
				return;

			pWorldTool->m_vecTimedParticle[ pWorldTool->m_iPickedParticleIndex ].m_bTraceUnit = 
				((CDXUTCheckBox*)pControl)->GetChecked();

		} break;



	case CX2WorldToolMain::IDC_CHECK_MESH_PLAYER_POS_IS_RELATIVE_TO_UNIT:
		{
			if( pWorldTool->m_iPickedXMeshPlayerIndex < 0 || pWorldTool->m_iPickedXMeshPlayerIndex > (int)pWorldTool->m_vecTimedXMeshPlayer.size() -1 )
				return;

			pWorldTool->m_vecTimedXMeshPlayer[ pWorldTool->m_iPickedXMeshPlayerIndex ].m_bRelativeToUnit = 
																				((CDXUTCheckBox*)pControl)->GetChecked();
		} break;

	case CX2WorldToolMain::IDC_CHECK_PARTICLE_POS_IS_RELATIVE_TO_UNIT:
		{

			if( pWorldTool->m_iPickedParticleIndex < 0 || pWorldTool->m_iPickedParticleIndex > (int)pWorldTool->m_vecTimedParticle.size() - 1 )
				return;

			pWorldTool->m_vecTimedParticle[ pWorldTool->m_iPickedParticleIndex ].m_bRelativeToUnit = 
																				((CDXUTCheckBox*)pControl)->GetChecked();
		} break;

	case CX2WorldToolMain::IDC_EDIT_MESH_PLAYER_POS_X:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{

					if( pWorldTool->m_iPickedXMeshPlayerIndex < 0 || pWorldTool->m_iPickedXMeshPlayerIndex > (int)pWorldTool->m_vecTimedXMeshPlayer.size() -1 )
						return;


					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedXMeshPlayer[ pWorldTool->m_iPickedXMeshPlayerIndex ].m_vRelativePosition.x = fValue;

					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_MESH_PLAYER_POS_Y:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pWorldTool->m_iPickedXMeshPlayerIndex < 0 || pWorldTool->m_iPickedXMeshPlayerIndex > (int)pWorldTool->m_vecTimedXMeshPlayer.size() -1 )
						return;



					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedXMeshPlayer[ pWorldTool->m_iPickedXMeshPlayerIndex ].m_vRelativePosition.y = fValue;

					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_MESH_PLAYER_POS_Z:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pWorldTool->m_iPickedXMeshPlayerIndex < 0 || pWorldTool->m_iPickedXMeshPlayerIndex > (int)pWorldTool->m_vecTimedXMeshPlayer.size() -1 )
						return;


					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedXMeshPlayer[ pWorldTool->m_iPickedXMeshPlayerIndex ].m_vRelativePosition.z = fValue;
					
					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_MESH_PLAYER_BONE_NAME:
		{

			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pWorldTool->m_iPickedXMeshPlayerIndex < 0 || pWorldTool->m_iPickedXMeshPlayerIndex > (int)pWorldTool->m_vecTimedXMeshPlayer.size() -1 )
						return;


					pWorldTool->m_vecTimedXMeshPlayer[ pWorldTool->m_iPickedXMeshPlayerIndex ].m_wstrBoneName = 
						((CDXUTEditBox*)pControl)->GetText();

					pDialog->ClearFocus();
				} break;
			}

		} break;


	case CX2WorldToolMain::IDC_EDIT_PARTICLE_BONE_NAME:
		{

			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					if( pWorldTool->m_iPickedParticleIndex < 0 || pWorldTool->m_iPickedParticleIndex > (int)pWorldTool->m_vecTimedParticle.size() -1 )
						return;


					pWorldTool->m_vecTimedParticle[ pWorldTool->m_iPickedParticleIndex ].m_wstrBoneName = 
						((CDXUTEditBox*)pControl)->GetText();

					pDialog->ClearFocus();
				} break;
			}

		} break;

	case CX2WorldToolMain::IDC_EDIT_PARTICLE_DELAY:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedParticle[ pWorldTool->m_iPickedParticleIndex ].m_fDelayTimeToStart = fValue; 
					pDialog->ClearFocus();
				} break;
			}

		} break;

	case CX2WorldToolMain::IDC_EDIT_PARTICLE_POS_X:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedParticle[ pWorldTool->m_iPickedParticleIndex ].m_vRelativePosition.x = fValue; 

					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_PARTICLE_POS_Y:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedParticle[ pWorldTool->m_iPickedParticleIndex ].m_vRelativePosition.y = fValue; 

					pDialog->ClearFocus();
				} break;
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_PARTICLE_POS_Z:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = (float)_wtof( ((CDXUTEditBox*)pControl)->GetText() );
					pWorldTool->m_vecTimedParticle[ pWorldTool->m_iPickedParticleIndex ].m_vRelativePosition.z = fValue; 

					pDialog->ClearFocus();
				} break;
			}
		} break;
			
	}
}





void CALLBACK OnDialogEvent_DLG_EFFECT_TOOL2( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2WorldToolMain* pWorldTool = (CX2WorldToolMain*) pUserContext;
	CWorldToolDialog* pDialog = pWorldTool->GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL2 );

	switch( nControlID )
	{

	case CX2WorldToolMain::IDC_LIST_CAMERA_SHAKE_UP:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					int iIndex = ((CDXUTListBox*)pControl)->GetSelectedIndex();

					pWorldTool->m_vecPickedCameraShake.clear();
					pWorldTool->m_vecPickedCameraShake.push_back(iIndex);

					pWorldTool->UpdatePickedCameraShakeUI();
				} break;
			} break;
		} break;


	case CX2WorldToolMain::IDC_BUTTON_ADD_CAMERA_SHAKE_UP:
		{
			CDXUTEditBox* pEditBox = (CDXUTEditBox*) pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_CAMERA_SHAKE_UP );

			float fNumbers[3] = { 0, };
			pEditBox->ParseFloatArray( &fNumbers[0], 3 );
			pEditBox->SetTextFloatArray( &fNumbers[0], 3 );


			CX2WorldToolMain::CameraShake cameraShake;
			cameraShake.m_fStartTime = fNumbers[0];
			cameraShake.m_fAmplitude = fNumbers[1];
			cameraShake.m_fDuration = fNumbers[2];
			pWorldTool->m_vecCameraShake.push_back( cameraShake );

			CDXUTListBox* pListBox		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_CAMERA_SHAKE_UP );
			WCHAR wszText[512] = L"";
			swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%4.2f, %4.2f, %4.2f",
				cameraShake.m_fStartTime, cameraShake.m_fAmplitude, cameraShake.m_fDuration );

			pListBox->AddItem( wszText, NULL );
			pListBox->SelectItem( pListBox->GetSize() - 1 );

		} break;


	case CX2WorldToolMain::IDC_BUTTON_DELETE_CAMERA_SHAKE_UP:
		{
			if( pWorldTool->m_vecPickedCameraShake.size() > 0 )
			{
				int iCameraShakeIndex = pWorldTool->m_vecPickedCameraShake[0];
				if( iCameraShakeIndex >= 0 && iCameraShakeIndex < (int)pWorldTool->m_vecCameraShake.size() )
				{
					pWorldTool->m_vecCameraShake.erase( pWorldTool->m_vecCameraShake.begin() + iCameraShakeIndex );
					pWorldTool->m_vecPickedCameraShake.clear();

					CDXUTListBox* pListBox		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_CAMERA_SHAKE_UP );
					pListBox->RemoveItem( iCameraShakeIndex );

					pWorldTool->UpdatePickedCameraShakeUI();
				}
			}
		} break;

	case CX2WorldToolMain::IDC_EDIT_CAMERA_SHAKE_UP:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				//case EVENT_EDITBOX_CHANGE:
				{
					float fNumbers[3] = { 0, };
					((CDXUTEditBox*)pControl)->ParseFloatArray( &fNumbers[0], 3 );
					((CDXUTEditBox*)pControl)->SetTextFloatArray( &fNumbers[0], 3 );

					if( pWorldTool->m_vecPickedCameraShake.size() > 0 )
					{
						int iCameraShakeIndex = pWorldTool->m_vecPickedCameraShake[0];
						if( iCameraShakeIndex >= 0 && iCameraShakeIndex < (int)pWorldTool->m_vecCameraShake.size() )
						{
							CX2WorldToolMain::CameraShake& cameraShake = pWorldTool->m_vecCameraShake[iCameraShakeIndex];
							
							cameraShake.m_fStartTime = fNumbers[0];
							cameraShake.m_fAmplitude = fNumbers[1];
							cameraShake.m_fDuration = fNumbers[2];


							CDXUTListBox* pListBox = pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_CAMERA_SHAKE_UP );
							DXUTListBoxItem* pItem = pListBox->GetItem( iCameraShakeIndex );
							if( NULL != pItem )
							{
								swprintf( pItem->strText, L"%4.2f, %4.2f, %4.2f",
									cameraShake.m_fStartTime, cameraShake.m_fAmplitude, cameraShake.m_fDuration );
							}
						}
					}
					else
					{
						CX2WorldToolMain::CameraShake cameraShake;
						cameraShake.m_fStartTime = fNumbers[0];
						cameraShake.m_fAmplitude = fNumbers[1];
						cameraShake.m_fDuration = fNumbers[2];
						pWorldTool->m_vecCameraShake.push_back( cameraShake );

						CDXUTListBox* pListBox		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_CAMERA_SHAKE_UP );
						WCHAR wszText[512] = L"";
						swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%4.2f, %4.2f, %4.2f",
							cameraShake.m_fStartTime, cameraShake.m_fAmplitude, cameraShake.m_fDuration );
						
						pListBox->AddItem( wszText, NULL );
						pListBox->SelectItem( pListBox->GetSize() - 1 );
					}

					pDialog->ClearFocus();
				} break;
			}

		} break;
	}
}



void CALLBACK OnDialogEvent_DLG_ETC( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"lua script\0*.lua\0X file\0*.x\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	CX2WorldToolMain* pWorldTool = (CX2WorldToolMain*) pUserContext;
	CWorldToolDialog* pDialog = pWorldTool->GetDialog( CX2WorldToolMain::DLG_ETC );

	switch( nControlID )
	{


	case CX2WorldToolMain::IDC_COMBO_MY_UNIT_CLASS:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			CX2Unit::UNIT_CLASS eUnitClass = (CX2Unit::UNIT_CLASS) (int) pItem->pData;
			if( eUnitClass != pWorldTool->GetCurrUnitClass() )
			{
				pWorldTool->GetCurrUnitClass() = eUnitClass;
				pWorldTool->ChangeUserUnit( eUnitClass );
			}

		} break;



	case CX2WorldToolMain::IDC_BUTTON_FILE_OPEN_NPC_SKINANIM:
		{
			ofn.lpstrFilter		= L"X file\0*.x\0";

			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pWorldTool->CreateCharacterSkinMeshFromFile( ofn.lpstrFile );
				g_pKTDXApp->SkipFrame();
			}

		} break;

	case CX2WorldToolMain::IDC_BUTTON_KILL_ALL_MONSTER:
		{
			if( NULL != pWorldTool->m_pGame )
			{
				pWorldTool->m_pGame->KillAllNPC();
			}
		} break;

	case CX2WorldToolMain::IDC_LIST_MONSTER:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();
					CX2UnitManager::NPC_UNIT_ID eNPCID = (CX2UnitManager::NPC_UNIT_ID) (int) pItem->pData;

					if( NULL != pWorldTool->m_pGame )
					{
						pWorldTool->m_pGame->CreateNPC( eNPCID, 1, true, 100, pWorldTool->m_pGame->GetMyUnit()->GetPos(), true, 0.f, 0 );
					}

				} break;
			} break;
		} break;

	case CX2WorldToolMain::IDC_CHECK_DYNAMIC_CAMERA:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			g_pMain->GetGameOption().SetDynamicCamera( bChecked );

		} break;
	case CX2WorldToolMain::IDC_CHECK_FIXED_CAMERA:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			pWorldTool->m_pGame->m_bFixedCamera = bChecked;

		} break;
	}
	

}


void CX2WorldToolMain::FinalDialog()
{
	for( unsigned i=0; i<m_vecpDialog.size(); i++ )
	{
		SAFE_DELETE( m_vecpDialog[i] );
	}
	m_vecpDialog.clear();
}

void CX2WorldToolMain::ResetDialogAtModeChange()
{
	switch( m_iToolMode )
	{
	case MODE_WORLDTOOL:
		{
			m_vecpDialog[DLG_MAIN]->SetEnable(true);
			m_vecpDialog[DLG_OBJECT_LIST]->SetEnable(true);
			m_vecpDialog[DLG_OBJECT_PROPERTY]->SetEnable(true);
			m_vecpDialog[DLG_EFFECT_TOOL]->SetEnable(false);
			m_vecpDialog[DLG_EFFECT_PROPERTY]->SetEnable(false);
			m_vecpDialog[DLG_EFFECT_TOOL2]->SetEnable(false);
			m_vecpDialog[DLG_ETC]->SetEnable(false);
		} break;

	case MODE_EFFECTTOOL:
		{
			m_vecpDialog[DLG_MAIN]->SetEnable(false);
			m_vecpDialog[DLG_OBJECT_LIST]->SetEnable(false);
			m_vecpDialog[DLG_OBJECT_PROPERTY]->SetEnable(false);
			m_vecpDialog[DLG_EFFECT_TOOL]->SetEnable(true);
			m_vecpDialog[DLG_EFFECT_PROPERTY]->SetEnable(true);
			m_vecpDialog[DLG_EFFECT_TOOL2]->SetEnable(true);
			m_vecpDialog[DLG_ETC]->SetEnable(true);
		} break;
	}

	UpdateDialog();
}

void CX2WorldToolMain::UpdateDialog()
{
	int iX = 10;
	for( unsigned i=0; i<m_vecpDialog.size(); i++ )
	{
		if( false == m_vecpDialog[i]->GetEnable() )
			continue;

		m_vecpDialog[i]->SetLocation( iX, 10 );
		iX += m_vecpDialog[i]->GetWidth()+10;
	}
}

void CX2WorldToolMain::InitDialog()
{
	// initialize UI
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CWorldToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 220;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	pDlg = new CWorldToolDialog;		// DLG_MAIN
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_MAIN, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont(0, L"굴림", 12, 12);


	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 30;
	//pDlg->AddButton( IDC_BUTTON_FILE_OPEN_DUNGEON, L"LOAD DUNGEON", iX, iY, iWidth, iHeight );
	//iY += iHeight + 3;

	pDlg->AddButton( IDC_BUTTON_HELP, L"HELP", iX, iY, iWidth, iHeight);
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_FILE_OPEN_WORLD, L"LOAD WORLD", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_FILE_OPEN_LINEMAP, L"LOAD LINEMAP", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;
	//pDlg->AddButton( IDC_BUTTON_FILE_SAVE_DUNGEON, L"SAVE DUNGEON", iX, iY, iWidth, iHeight );
	//iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_FILE_SAVE_WORLD, L"SAVE WORLD", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;

	pDlg->AddComboBox( IDC_COMBO_LINEMAP_FILENAME, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 20 );
		// 2009.02.25 김태완 : 디폴트를 SUFFIX로 변경
		pComboBox->AddItem( L"filename_LINEMAP.lua",			(void*) LFT_SUFFIX );
		pComboBox->AddItem( L"LINEMAP_filename.lua",			(void*) LFT_PREFIX );		
	}
	pComboBox->SetSelectedByData( (void*) m_eLineMapFileNameType );


	//pDlg->AddButton( IDC_BUTTON_FILE_SAVE_LINEMAP, L"SAVE LINEMAP", iX, iY, iWidth, iHeight );
	//iY += iHeight + 3;

    //iY += iHeight + 3;
    //pDlg->AddStatic( IDC_STATIC_DUMMY, L"Ortho Scale", iX, iY, iWidth, iHeight );

    iY += iHeight + 3;
    pDlg->AddCheckBox( IDC_CHECK_ORTHOFREE, L"Ortho Free Camera", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
    

    iY += iHeight + 3;	
    pDlg->AddEditBox( IDC_EDIT_ORTHOSCALE, L"", iX, iY, iWidth, iHeight, false, &pEditBox );   
    float fOrthoScale = 2.0f;
    pEditBox->SetTextFloatArray( &fOrthoScale, 1 );
    

	iY += iHeight + 3;
	pDlg->AddCheckBox( IDC_CHECK_SQUARE_CAMERA, L"Square Camera", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );

	iY += iHeight + 3;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"square camera dist", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_SQUARE_CAMERA_DISTANCE, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	float fValue = 0.f;
	pEditBox->SetTextFloatArray( &fValue, 1 );


	iY += iHeight + 0;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"square camera lookAt height", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_SQUARE_CAMERA_LOOKAT_HEIGHT, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	pEditBox->SetTextFloatArray( &fValue, 1 );


	iY += iHeight + 0;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"square camera eye height", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_SQUARE_CAMERA_EYE_HEIGHT, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	pEditBox->SetTextFloatArray( &fValue, 1 );




	iY += iHeight + 0;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"CAMERA SPEED", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_CAMERA_MOVE_SPEED, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	float fCameraMoveSpeed = m_FPSCamera.m_fMoveSpeed;
	pEditBox->SetTextFloatArray( &fCameraMoveSpeed, 1 );



	iY += iHeight + 0;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"UNIT COLOR", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_WORLD_LIGHT_COLOR, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	int iLightColor[3] = { 255, 255, 255 };
	SetTextIntArrayToEditBox( pEditBox,  (int*)&iLightColor, 3 );




	iY += iHeight + 0;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"WORLD COLOR", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_ORIGIN_WORLD_LIGHT_COLOR, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	int iOriginLightColor[3] = { 255, 255, 255 };
	SetTextIntArrayToEditBox( pEditBox,  (int*)&iOriginLightColor, 3 );

	// Slider
	iY += iHeight + 3;	
	pDlg->AddSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_RED, iX, iY, iWidth, iHeight, 0, 255, 255 );
	iY += iHeight + 3;	
	pDlg->AddSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_GREEN, iX, iY, iWidth, iHeight, 0, 255, 255 );
	iY += iHeight + 3;	
	pDlg->AddSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE, iX, iY, iWidth, iHeight, 0, 255, 255 );


	
	
	iY += iHeight + 0;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"FADE COLOR", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_FADE_WORLD_LIGHT_COLOR, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	int iFadeWorldLightColor[3] = { 0, 0, 0 };
	SetTextIntArrayToEditBox( pEditBox,  (int*)&iFadeWorldLightColor, 3 );

	
	iY += iHeight + 0;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"OBJECT SPEED", iX, iY, iWidth, iHeight );
	iY += iHeight + 0;	
	pDlg->AddEditBox( IDC_EDIT_OBJECT_MOVE_SPEED, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	pEditBox->SetTextFloatArray( &m_fObjectMoveSpeed, 1 );






	pDlg = new CWorldToolDialog;		// DLG_OBJECT_LIST
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_OBJECT_LIST, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH * 2, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont(0, L"굴림", 12, 12);


	iWidth = 450;
	iX = 10;
	iY = 10;
	pDlg->AddButton( IDC_BUTTON_CLEAR_OBJECT_PALETTE, L"CLEAR PALETTE", iX, iY, iWidth, iHeight );
	iY += iHeight + 8;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"OBJECT PALETTE", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddListBox( IDC_LIST_OBJECT_PALETTE, iX, iY, iWidth, iHeight*5, LBS_SORT, &pListBox );
	pListBox->SetStyle( CDXUTListBox::MULTISELECTION );
	iY += iHeight*6 + 3;
	pDlg->AddButton( IDC_BUTTON_ADD_OBJECT_TO_WORLD_AS_STATIC_MESH, L"ADD as STATIC MESH", iX, iY, iWidth, iHeight );
	iY += iHeight + 8;
	pDlg->AddButton( IDC_BUTTON_ADD_OBJECT_TO_WORLD_AS_SKIN_MESH, L"ADD as SKIN MESH", iX, iY, iWidth, iHeight );
	iY += iHeight + 8;
	pDlg->AddButton( IDC_BUTTON_RELOAD_OBJECT_IN_WORLD, L"RELOAD MESH IN WORLD", iX, iY, iWidth, iHeight );
	iY += iHeight + 8;

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"OBJECT in WORLD", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddListBox( IDC_LIST_OBJECT_IN_WORLD, iX, iY, iWidth, iHeight*8, LBS_SORT, &pListBox );
	pListBox->SetStyle( CDXUTListBox::MULTISELECTION );
	iY += iHeight*8 + 3;
	pDlg->AddButton( IDC_BUTTON_REMOVE_OBJECT_FROM_WORLD, L"REMOVE SELECTED OBJECT", iX, iY, iWidth, iHeight );
	iY += iHeight + 8;

	pDlg->AddCheckBox( IDC_CHECK_RENDER_BOUNDING_SPHERE, L"Bounding Sphere", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;
	pDlg->AddCheckBox( IDC_CHECK_REDNER_FLICKER, L"선택된 오브젝트 깜빡이기", iX, iY, iWidth, iHeight, true, 0, false, &pCheckBox );
	iY += iHeight + 3;

	pDlg->AddCheckBox( IDC_CHECK_ENABLE_CENTER_GREEN_DOT, L"가운데 녹색점", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	pCheckBox->SetChecked( m_bCenterGreenDot );
	iY += iHeight + 3;

	pDlg = new CWorldToolDialog;		// DLG_OBJECT_PROPERTY
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_OBJECT_PROPERTY, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont(0, L"굴림", 12, 12);

	iWidth = 200;	
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
	// Slider
	pDlg->AddSlider( IDC_SLIDER_OBJECT_LIGHT_COLOR_RED, iX-MAGIC_DIALOG_WIDTH * 2, iY, iWidth, iHeight, 0, 255, 255 );
	iY += iHeight + 3;	
	pDlg->AddSlider( IDC_SLIDER_OBJECT_LIGHT_COLOR_GREEN, iX-MAGIC_DIALOG_WIDTH * 2, iY, iWidth, iHeight, 0, 255, 255 );
	iY += iHeight + 3;	
	pDlg->AddSlider( IDC_SLIDER_OBJECT_LIGHT_COLOR_BLUE, iX-MAGIC_DIALOG_WIDTH * 2, iY, iWidth, iHeight, 0, 255, 255 );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_OBJECT_LIGHT_COLOR, L"", iX-MAGIC_DIALOG_WIDTH * 2, iY, iWidth, iHeight, false, &pEditBox );
#endif
	iX = 10;
	iY = 10;
	
	iY -= 1;		// Not render 옵션 추가로 인한 크기 조절.
	iHeight -= 1;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"POSITION", iX, iY, iWidth, iHeight );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_POS_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_POS_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_POS_Z, L"", iX, iY, iWidth, iHeight, false, NULL );

	iY += iHeight + 2;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"SCALE", iX, iY, iWidth, iHeight );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_SCALE_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_SCALE_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_SCALE_Z, L"", iX, iY, iWidth, iHeight, false, NULL );

	iY += iHeight + 2;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"ROTATION DEGREE", iX, iY, iWidth, iHeight );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_ROTATION_DEGREE, L"", iX, iY, iWidth, iHeight, false, NULL );

	iY += iHeight + 2;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"SKY ROTATION SPEED", iX, iY, iWidth, iHeight );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_ROTATION_SPEED, L"", iX, iY, iWidth, iHeight, false, NULL );

	iY += iHeight + 2;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"TEX0 UV SPEED", iX, iY, iWidth, iHeight );
	iY += iHeight + 2;	
	pDlg->AddEditBox( IDC_EDIT_TEX0_UV_SPEED, L"", iX, iY, iWidth, iHeight, false, NULL );
	
    iY += iHeight + 2;
    pDlg->AddCheckBox( IDC_CHECK_IS_ALPHA_OBJECT, L"ALPHA OBJECT", iX, iY, 130, iHeight, false, 0, false, &pCheckBox );
#ifdef ADD_ALPHATESTENABLE
	pDlg->AddCheckBox( IDC_CHECK_IS_ALPHATEST_ENABLE, L"AT", iX+140, iY, 50, iHeight, false, 0, false, &pCheckBox );
#endif
	iY += iHeight + 2;
	pDlg->AddCheckBox( IDC_CHECK_ADDITIVE_BLEND, L"ADDITIVE BLEND", iX, iY, 130, iHeight, false, 0, false, &pCheckBox );
#ifdef ADD_ALPHATESTENABLE
	pDlg->AddCheckBox( IDC_CHECK_IS_NOT_CULL, L"2S", iX+140, iY, 50, iHeight, false, 0, false, &pCheckBox );
#endif
	iY += iHeight + 2;
	pDlg->AddCheckBox( IDC_CHECK_CAN_HIDE, L"CAN HIDE", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 2;
	pDlg->AddCheckBox( IDC_CHECK_NOT_RENDER, L"NOT RENDER", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );

	iY += iHeight + 2;
	pDlg->AddCheckBox( IDC_CHECK_USE_WORLD_COLOR, L"USE WORLD COLOR", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += 1;
	iHeight += 1;
	
    // GLOBAL FOG
    int fix, fiy;
    fix = iX+iWidth+10;
    fiy = 10;
    pDlg->AddCheckBox( IDC_CHECK_IS_FOGWORLD, L"FOG WORLD", fix, fiy, iWidth, iHeight, false, 0, false, &pCheckBox );
    fiy += iHeight + 3;
    pDlg->AddCheckBox( IDC_CHECK_IS_FOG, L"FOG OBJECT", fix, fiy, iWidth, iHeight, false, 0, false, &pCheckBox );    
    fiy += iHeight + 10;    
    pDlg->AddStatic( IDC_STATIC_DUMMY, L"FOG Near/Far X", fix, fiy, iWidth, iHeight );
    fiy += iHeight + 1;
    pDlg->AddEditBox( IDC_EDIT_FOG_NEAR_X, L"", fix, fiy, iWidth, iHeight, false, NULL );
    fiy += iHeight + 1;		
    pDlg->AddSlider( IDC_SLIDER_FOG_NEAR_X, fix, fiy, iWidth+100, iHeight, -20000, 40000, 0 );
    fiy += iHeight + 3;
    pDlg->AddEditBox( IDC_EDIT_FOG_FAR_X, L"", fix, fiy, iWidth, iHeight, false, NULL );
    fiy += iHeight + 1;		
    pDlg->AddSlider( IDC_SLIDER_FOG_FAR_X, fix, fiy, iWidth+100, iHeight, -20000, 40000, 0 );
    fiy += iHeight + 10;
    pDlg->AddStatic( IDC_STATIC_DUMMY, L"FOG Near/Far Y", fix, fiy, iWidth, iHeight );
    fiy += iHeight + 3;
    pDlg->AddEditBox( IDC_EDIT_FOG_NEAR_Y, L"", fix, fiy, iWidth, iHeight, false, NULL );
    fiy += iHeight + 1;		
    pDlg->AddSlider( IDC_SLIDER_FOG_NEAR_Y, fix, fiy, iWidth+100, iHeight, -10000, 10000, 0 );
    fiy += iHeight + 3;
    pDlg->AddEditBox( IDC_EDIT_FOG_FAR_Y, L"", fix, fiy, iWidth, iHeight, false, NULL );
    fiy += iHeight + 1;		
    pDlg->AddSlider( IDC_SLIDER_FOG_FAR_Y, fix, fiy, iWidth+100, iHeight, -10000, 10000, 0 );
    fiy += iHeight + 10;
    pDlg->AddStatic( IDC_STATIC_DUMMY, L"DENSITY (-1.0 ~ 1.0)", fix, fiy, iWidth, iHeight );
    fiy += iHeight + 3;
    pDlg->AddEditBox( IDC_EDIT_FOG_DENSITY, L"", fix, fiy, iWidth, iHeight, false, NULL );
    fiy += iHeight + 1;		
    pDlg->AddSlider( IDC_SLIDER_FOG_DENSITY, fix, fiy, iWidth+100, iHeight, -100, 100, 0 );
    fiy += iHeight + 3;
    pDlg->AddEditBox( IDC_EDIT_FOG_DENSITY_OBJECT, L"", fix, fiy, iWidth, iHeight, false, NULL );
    fiy += iHeight + 1;		
    pDlg->AddSlider( IDC_SLIDER_FOG_DENSITY_OBJECT, fix, fiy, iWidth+100, iHeight, -100, 100, 0 );
    fiy += iHeight + 10;

    pDlg->AddStatic( IDC_STATIC_DUMMY, L"FOG COLOR", fix, fiy, iWidth, iHeight );
    fiy += iHeight + 3;	
    pDlg->AddEditBox( IDC_EDIT_ORIGIN_FOG_COLOR, L"", fix, fiy, iWidth, iHeight, false, &pEditBox );
    int iOriginFogColor[3] = { 255, 255, 255 };
    SetTextIntArrayToEditBox( pEditBox,  (int*)&iOriginFogColor, 3 );

    // Slider
    fiy += iHeight + 3;		
    pDlg->AddSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED, fix, fiy, iWidth, iHeight, 0, 255, 255 );
    fiy += iHeight + 3;		
    pDlg->AddSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN, fix, fiy, iWidth, iHeight, 0, 255, 255 );
    fiy += iHeight + 3;		
    pDlg->AddSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE, fix, fiy, iWidth, iHeight, 0, 255, 255 );

	fiy += iHeight + 20;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"OBJECT HIDE DISTANCE", fix, fiy, iWidth, iHeight );
	fiy += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_HIDE_DISTANCE, L"", fix, fiy, iWidth, iHeight, false, NULL );

	fiy += iHeight + 3;	
	pDlg->AddCheckBox( IDC_CHECK_FORCE_SHOW, L"FORCE SHOW", fix, fiy, iWidth, iHeight, false, 0, false, &pCheckBox );


	//iY += iHeight + 3;	
	//pDlg->AddCheckBox( IDC_CHECK_IS_SKIN_MESH, L"SKIN MESH", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	//pCheckBox->SetEnabled(false);
	//iY += iHeight + 3;	
	//pDlg->AddCheckBox( IDC_CHECK_IS_SKYDOME, L"SKY/CLOUD", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	


	iY += iHeight + 3;	
	pDlg->AddComboBox( IDC_COMBO_OBJECT_TYPE, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 80 );
		pComboBox->AddItem( L"OT_STATIC_MESH",	(void*) CX2WorldObject::OT_STATIC_MESH	);
		pComboBox->AddItem( L"OT_SKIN_MESH",	(void*) CX2WorldObject::OT_SKIN_MESH	);
		pComboBox->AddItem( L"OT_PARTICLE",		(void*) CX2WorldObject::OT_PARTICLE		);
		pComboBox->AddItem( L"OT_LIGHT_FLOW",	(void*) CX2WorldObject::OT_LIGHT_FLOW	);
		pComboBox->AddItem( L"OT_REACT_MESH",	(void*) CX2WorldObject::OT_REACT_MESH	);

		pComboBox->AddItem( L"OT_SKY_CLOUD",	(void*) (CX2WorldObject::OT_REACT_MESH+1) );
	}

	iY += iHeight + 3;	
	pDlg->AddComboBox( IDC_COMBO_LAYER, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 80 );
		pComboBox->AddItem( L"SKY_DOME_0",								(void*) XL_SKY_DOME_0 );
		pComboBox->AddItem( L"SKY_DOME_1",								(void*) XL_SKY_DOME_1 );
		pComboBox->AddItem( L"SKY_DOME_2",								(void*) XL_SKY_DOME_2 );
		pComboBox->AddItem( L"WORLD_BASE_0",							(void*) XL_SKY_WORLD_BASE_0 );
		pComboBox->AddItem( L"WORLD_BASE_1",							(void*) XL_SKY_WORLD_BASE_1 );
		pComboBox->AddItem( L"WORLD_BASE_2",							(void*) XL_SKY_WORLD_BASE_2 );
		pComboBox->AddItem( L"WORLD_OBJECT_0",							(void*) XL_SKY_WORLD_OBJECT_0 );
		pComboBox->AddItem( L"WORLD_OBJECT_1",							(void*) XL_SKY_WORLD_OBJECT_1 );
		pComboBox->AddItem( L"WORLD_OBJECT_2",							(void*) XL_SKY_WORLD_OBJECT_2 );
		pComboBox->AddItem( L"XL_UNIT_0",								(void*) XL_UNIT_0 );
		pComboBox->AddItem( L"XL_UNIT_1",								(void*) XL_UNIT_1 );
		pComboBox->AddItem( L"XL_UNIT_2",								(void*) XL_UNIT_2 );
		pComboBox->AddItem( L"XL_EFFECT_0",								(void*) XL_EFFECT_0 );
		pComboBox->AddItem( L"XL_EFFECT_1",								(void*) XL_EFFECT_1 );
		pComboBox->AddItem( L"XL_EFFECT_2",								(void*) XL_EFFECT_2 );

		//{{ kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
#ifdef	ADD_X2_LAYER
		pComboBox->AddItem( L"XL_EFFECT_OBJ_0",							(void*) XL_SKY_WORLD_OBJECT_OVER_EFFECT_0 );
		pComboBox->AddItem( L"XL_EFFECT_OBJ_1",							(void*) XL_SKY_WORLD_OBJECT_OVER_EFFECT_1 );
		pComboBox->AddItem( L"XL_EFFECT_OBJ_2",							(void*) XL_SKY_WORLD_OBJECT_OVER_EFFECT_2 );
		pComboBox->AddItem( L"XL_EFFECT_OBJ_3",							(void*) XL_SKY_WORLD_OBJECT_OVER_EFFECT_3 );
// 		pComboBox->AddItem( L"XL_EFFECT_OVER_ALL_WORLD_OBJECT_0",		(void*) XL_EFFECT_OVER_ALL_WORLD_OBJECT_0 );
// 		pComboBox->AddItem( L"XL_EFFECT_OVER_ALL_WORLD_OBJECT_1",		(void*) XL_EFFECT_OVER_ALL_WORLD_OBJECT_1 );
#endif	ADD_X2_LAYER
		//}} kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업

// kimhc // XL_EFFECT_OVER_ALL_WORLD_OBJECT 와 함께 월드툴에서 사용하지 말아야할 것 주석 처리함
// 		pComboBox->AddItem( L"XL_LENS_FLARE",							(void*) XL_LENS_FLARE );
// 		pComboBox->AddItem( L"XL_MINIMAP",								(void*) XL_MINIMAP );
	}

	iY += iHeight + 3;		
	pDlg->AddComboBox( IDC_COMBO_BILLBOARD, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 80 );
		pComboBox->AddItem( L"BT_NONE", (void*) CKTDGMatrix::BT_NONE );
		pComboBox->AddItem( L"BT_ALL",	(void*) CKTDGMatrix::BT_ALL );
		pComboBox->AddItem( L"BT_X",	(void*) CKTDGMatrix::BT_X );
		pComboBox->AddItem( L"BT_Y",	(void*) CKTDGMatrix::BT_Y );
		pComboBox->AddItem( L"BT_Z",	(void*) CKTDGMatrix::BT_Z );
	}

	iY += iHeight + 3;		
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"ANI NAME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_SKIN_ANIM_NAME, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;

	pDlg->AddComboBox( IDC_COMBO_SKIN_ANIM_PLAY_TYPE, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 60 );
		pComboBox->AddItem( L"ONE",						(void*) CX2WorldObjectSkinMesh::WOSMPY_ONE );
		pComboBox->AddItem( L"ONE_WAIT",				(void*) CX2WorldObjectSkinMesh::WOSMPY_ONE_WAIT );
		pComboBox->AddItem( L"LOOP",					(void*) CX2WorldObjectSkinMesh::WOSMPY_LOOP );
		pComboBox->AddItem( L"PINGPONG",				(void*) CX2WorldObjectSkinMesh::WOSMPY_PINGPONG );
		pComboBox->AddItem( L"OCCASIONAL_ONE",			(void*) CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE );
		pComboBox->AddItem( L"OCCASIONAL_ONE_WAIT",		(void*) CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE_WAIT );
	}

	iY += iHeight + 3;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"EVENT TIME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_OCCATIONAL_EVENT_TIME, L"", iX, iY, iWidth, iHeight, false, NULL );

	iY += iHeight + 3;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"EVENT RATE", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_OCCATIONAL_EVENT_RATE, L"", iX, iY, iWidth, iHeight, false, NULL );



	//	pDlg->AddButton( IDC_BUTTON_HELP, L"HELP", iX, iY, 100, 22);
	//	iY += 25;
	//	pDlg->AddCheckBox( IDC_CHECK_SNAP, L"SNAP", iX, iY, 100, 22, m_bEnableSnap );
	//	iY += 25;
	//	pDlg->AddCheckBox( IDC_CHECK_SHOW_DIALOG, L"SHOW DIALOG", iX, iY, 100, 22, m_bShowDialog );
	//	iY += 25;
	//	pDlg->AddCheckBox( IDC_CHECK_RELOAD_NPC, L"RELOAD NPC", iX, iY, 100, 22, m_bReloadNPC );
	//
	//	pDlg = new CDXUTDialog();
	//	m_vecpDialog.push_back( pDlg );
	//	pDlg->Init( &g_DialogResourceManager );
	//	pDlg->SetCallback( OnDialogEvent1, this );
	//	pDlg->SetSize( 100, 300 );
	//	pDlg->SetFont(0, L"굴림", 12, 12);
	//

	//
	//	//iX = 230;
	//	//iY = 160;
	//	CDXUTEditBox* pEdit = NULL;
	//	pDlg->AddEditBox( IDC_EDIT_INFO, L"", iX, iY, 500, 200, false, &pEdit );
	//	pEdit->GetElement(0)->dwTextFormat = DT_LEFT|DT_TOP|DT_WORDBREAK;
	//	pEdit->SetEnabled( false );
	//	//pEdit->SetTextColor( D3DCOLOR_ARGB( 255, 255, 0, 255 ) );





	pDlg = new CWorldToolDialog;		// DLG_EFFECT_TOOL
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_TOOL, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 10, 10 );


	iWidth = 200;
	iHeight = 20;
	iX = 10;
	iY = 10;

	pDlg->AddButton( IDC_BUTTON_LOAD_XMESH_PLAYER, L"LOAD XMESHPLAYER", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_XMESH_PLAYER_PALETTE, iX, iY, iWidth, iHeight*4, 0, &pListBox );
	iY += iHeight*4 + 3;
	pDlg->AddListBox( IDC_LIST_XMESH_PLAYER_LOADED, iX, iY, iWidth, iHeight*4, 0, &pListBox );
	iY += iHeight*4 + 3;

	pDlg->AddButton( IDC_BUTTON_LOAD_PARTICLE, L"LOAD PARTICLE", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_PARTICLE_PALETTE, iX, iY, iWidth, iHeight*4, 0, &pListBox );
	iY += iHeight*4 + 3;
	pDlg->AddListBox( IDC_LIST_PARTICLE_LOADED, iX, iY, iWidth, iHeight*4, 0, &pListBox );
	iY += iHeight*4 + 3;



	pDlg->AddButton( IDC_BUTTON_READY_EFFECT_DEVICES, L"READY DEVICES", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;

	pDlg->AddButton( IDC_BUTTON_LOAD_EFFECT_SET, L"LOAD EFFECT SET", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_SAVE_EFFECT_SET, L"SAVE EFFECT SET", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;
	pDlg->AddCheckBox( IDC_CHECK_USER_UNIT_STATE, L"UNIT STATE", iX, iY, iWidth, iHeight, m_bUseUserUnitState, 0, false, &pCheckBox );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_USER_UNIT_STATE, iX, iY, iWidth, iHeight*4, 0, &pListBox );
	iY += iHeight*4 + 3;

	
	
	pDlg->AddCheckBox( IDC_CHECK_FIX_SKIN_ANIM_POSITION, L"FIX POSITION", iX, iY, iWidth, iHeight, m_bShowCharacterSkinAnim, 0, false, &pCheckBox );
	iY += iHeight + 3;

	pDlg->AddEditBox( IDC_EDIT_SKIN_ANIM_POSITION, L"", iX, iY, iWidth, iHeight+10, false, &pEditBox );
	WCHAR wszText[64] = L"";
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%f %f %f", m_vFixedCharacterSkinAnimPosition.x, m_vFixedCharacterSkinAnimPosition.y, m_vFixedCharacterSkinAnimPosition.z );
	pEditBox->SetText( wszText );
	iY += iHeight+10 + 3;	

	pDlg->AddEditBox( IDC_EDIT_SKIN_ANIM_ROTATION, L"", iX, iY, iWidth, iHeight+10, false, &pEditBox );
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%f %f %f", m_vFixedCharacterSkinAnimRotation.x, m_vFixedCharacterSkinAnimRotation.y, m_vFixedCharacterSkinAnimRotation.z );
	pEditBox->SetText( wszText );
	iY += iHeight+10 + 3;
	
	pDlg->AddCheckBox( IDC_CHECK_SKIN_ANIM_NAME, L"ANIM NAME", iX, iY, iWidth, iHeight, m_bShowCharacterSkinAnim, 0, false, &pCheckBox );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_SKIN_ANIM_NAME, iX, iY, iWidth, iHeight*4, 0, &pListBox );
	iY += iHeight*4 + 3;

	pDlg->AddCheckBox( IDC_CHECK_WORLD_COLOR, L"BLACK OUT", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;

	pDlg->AddEditBox( IDC_EDIT_WORLD_COLOR_TIME, L"", iX, iY, iWidth, iHeight+10, false, &pEditBox );
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%f", m_fWorldColorTime );
	pEditBox->SetText( wszText );
	iY += iHeight+10 + 3;	

	pDlg->AddEditBox( IDC_EDIT_WORLD_COLOR_FADE_TIME, L"", iX, iY, iWidth, iHeight+10, false, &pEditBox );
	swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%f", m_fWorldColorFadeTime );
	pEditBox->SetText( wszText );
	iY += iHeight+10 + 3;




	
	pDlg = new CWorldToolDialog;		// DLG_EFFECT_PROPERTY
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_PROPERTY, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont(0, L"굴림", 12, 12);

	iHeight = 30;
	iWidth = 200;
	iX = 10;
	iY = 10;

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"MESHPLAYER DELAY TIME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_MESH_PLAYER_DELAY, L"", iX, iY, iWidth, iHeight, false, NULL );

	iY += iHeight + 3;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"MESHPLAYER BONE NAME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_MESH_PLAYER_BONE_NAME, L"", iX, iY, iWidth, iHeight, false, NULL );


	iY += iHeight + 3;
	pDlg->AddCheckBox( IDC_CHECK_MESH_PLAYER_POS_IS_RELATIVE_TO_UNIT, L"MESHPLAYER RELATIVE POS", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );

	iY += iHeight + 3;
	pDlg->AddCheckBox( IDC_CHECK_MESH_PLAYER_TRACE, L"MESHPLAYER TRACE UNIT", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );

	iY += iHeight + 3;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"MESHPLAYER POSITION", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_MESH_PLAYER_POS_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_MESH_PLAYER_POS_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_MESH_PLAYER_POS_Z, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	






	pDlg->AddStatic( IDC_STATIC_DUMMY, L"PARTICLE DELAY TIME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_PARTICLE_DELAY, L"", iX, iY, iWidth, iHeight, false, NULL );

	iY += iHeight + 3;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"PARTICLE BONE NAME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_PARTICLE_BONE_NAME, L"", iX, iY, iWidth, iHeight, false, NULL );


	iY += iHeight + 3;
	pDlg->AddCheckBox( IDC_CHECK_PARTICLE_POS_IS_RELATIVE_TO_UNIT, L"PARTICLE RELATIVE POS", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );

	iY += iHeight + 3;
	pDlg->AddCheckBox( IDC_CHECK_PARTICLE_TRACE, L"PARTICLE TRACE UNIT", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );

	iY += iHeight + 3;	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"PARTICLE POSITION", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_PARTICLE_POS_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_PARTICLE_POS_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_PARTICLE_POS_Z, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	








	pDlg = new CWorldToolDialog;		// DLG_EFFECT_TOOL2
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_TOOL2, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 10, 10 );


	iWidth = 200;
	iHeight = 30;
	iX = 10;

	iY = 10;
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"CAMERA SHAKE UP", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddListBox( IDC_LIST_CAMERA_SHAKE_UP, iX, iY, iWidth, iHeight*4, 0, &pListBox );
	iY += iHeight*4 + 3;
	pDlg->AddEditBox( IDC_EDIT_CAMERA_SHAKE_UP, L"", iX, iY, iWidth, iHeight, false, &pEditBox );
	pEditBox->SetText( L"시작시간 진폭 지속시간" );
	iY += iHeight + 3;	
	pDlg->AddButton( IDC_BUTTON_ADD_CAMERA_SHAKE_UP, L"ADD CAMERA SHAKE", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddButton( IDC_BUTTON_DELETE_CAMERA_SHAKE_UP, L"DELETE CAMERA SHAKE", iX, iY, iWidth, iHeight );


	//{{ 2009.02.26 김태완
	// 캐릭터,NPC 관련 : DLG_MAIN 아래에서 DLG_ETC로 분리 : MAIN에 너무 많아서

	pDlg = new CWorldToolDialog;		// DLG_ETC
	m_vecpDialog.push_back( pDlg );
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_ETC, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 10, 10 );

	iWidth = 200;
	iHeight = 30;
	iX = 10;

	// note!! 전직 추가되면 수정
	iY = 10;
	pDlg->AddComboBox( IDC_COMBO_MY_UNIT_CLASS, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 20 );
		pComboBox->AddItem( L"Elsword",			(void*) CX2Unit::UC_ELSWORD_SWORDMAN );
		pComboBox->AddItem( L"Knight",			(void*) CX2Unit::UC_ELSWORD_KNIGHT );
		pComboBox->AddItem( L"MagicKnight",		(void*) CX2Unit::UC_ELSWORD_MAGIC_KNIGHT );

		pComboBox->AddItem( L"Aisha",			(void*) CX2Unit::UC_ARME_VIOLET_MAGE );
		pComboBox->AddItem( L"HighMagician",	(void*) CX2Unit::UC_ARME_HIGH_MAGICIAN );
		pComboBox->AddItem( L"DarkMagician",	(void*) CX2Unit::UC_ARME_DARK_MAGICIAN );

		pComboBox->AddItem( L"Rena",			(void*) CX2Unit::UC_LIRE_ELVEN_RANGER );
		pComboBox->AddItem( L"CombatRanger",	(void*) CX2Unit::UC_LIRE_COMBAT_RANGER );
		pComboBox->AddItem( L"SnipingRanger",	(void*) CX2Unit::UC_LIRE_SNIPING_RANGER );

		pComboBox->AddItem( L"Raven",			(void*) CX2Unit::UC_RAVEN_FIGHTER );
		pComboBox->AddItem( L"Raven Soul",		(void*) CX2Unit::UC_RAVEN_SOUL_TAKER );
		pComboBox->AddItem( L"Raven Over",		(void*) CX2Unit::UC_RAVEN_OVER_TAKER );


		pComboBox->AddItem( L"Eve",				(void*) CX2Unit::UC_EVE_NASOD );


#ifdef EVE_FIRST_CHANGE_JOB
		pComboBox->AddItem( L"CodeExotic",			(void*) CX2Unit::UC_EVE_EXOTIC_GEAR );
		pComboBox->AddItem( L"CodeArchitecture",	(void*) CX2Unit::UC_EVE_ARCHITECTURE );
#endif

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		pComboBox->AddItem( L"Chung",				(void*) CX2Unit::UC_CHUNG_IRON_CANNON );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		pComboBox->AddItem( L"Ara",					(void*) CX2Unit::UC_ARA_MARTIAL_ARTIST );
#endif


	}
	pComboBox->SetSelectedByIndex( 0 );


	//pDlg->AddRadioButton( IDC_RADIO_ELSWORD, 1, L"ELSWORD", iX, iY, iWidth, iHeight, true );
	//iY += iHeight + 3;	
	//pDlg->AddRadioButton( IDC_RADIO_ARME, 1, L"ARME", iX, iY, iWidth, iHeight, false );
	//iY += iHeight + 3;	
	//pDlg->AddRadioButton( IDC_RADIO_LIRE, 1, L"LIRE", iX, iY, iWidth, iHeight, false );
	iY += iHeight + 3;	
	pDlg->AddButton( IDC_BUTTON_FILE_OPEN_NPC_SKINANIM, L"LOAD NPC MOTION", iX, iY, iWidth, iHeight );

	iY += iHeight + 3;	
	pDlg->AddListBox( IDC_LIST_MONSTER, iX, iY, iWidth, iHeight*4, LBS_SORT, &pListBox );
	iY += iHeight*4 + 3;
	pDlg->AddButton( IDC_BUTTON_KILL_ALL_MONSTER, L"KILL ALL MONSTER", iX, iY, iWidth, iHeight );
	
	iY += iHeight + 3;	
	pDlg->AddCheckBox( IDC_CHECK_DYNAMIC_CAMERA, L"DYNAMIC CAMERA", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	pCheckBox->SetChecked( g_pMain->GetGameOption().GetOptionList().m_bDynamicCamera );

	iY += iHeight + 3;	
	pDlg->AddCheckBox( IDC_CHECK_FIXED_CAMERA, L"FIXED CAMERA(f)", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	pCheckBox->SetChecked( false );
	//}}




    g_pWorldToolMain = this;
}

void CX2WorldToolMain::InitX2Data()
{
	g_pData->ResetServerProtocol();
	//g_pData->GetServerProtocol()->ConnectedToGameServer( g_pMain->GetServerIP(), g_pMain->GetServerPort() );
	g_pData->GetServerProtocol()->ConnectedToGameServerForWorldTool();

	//UDP 초기화
	g_pData->ResetGameUDP( g_pMain->GetGameP2PPort() );

	//UI 파티클 시스템 생성
	g_pData->ResetUIMajorParticle( L"UIMajorParticle.txt" );
	g_pData->ResetUIMinorParticle( L"UIMinorParticle.txt" );

	//UI 메시플레이어
	g_pData->ResetUIMajorXMeshPlayer( L"UIMajorXMeshPlayer.txt" );
	g_pData->ResetUIMinorXMeshPlayer( L"UIMinorXMeshPlayer.txt" );

	//유닛 데이타 로드
	g_pData->ResetUnitManager( L"UnitTemplet.lua", L"NPCTemplet.lua", L"NPCStat.lua" );
	//g_pData->ResetUnitManager( L"UnitTemplet.lua", L"", L"" );

	CX2BuffTempletManager::GetInstance()->OpenScriptFileForTemplet();
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForFactor();

	//아이템 데이타 로드
	g_pData->ResetItemManager();
#ifndef WORLD_TOOL	// 090519 태완 임시수정코드
	// 칭호
	g_pData->ResetTitleManager();
#endif

	//월드 데이타 로드
	g_pData->ResetWorldManager( L"WorldData.lua" );	

	//던전 데이타 로드
	g_pData->ResetDungeonManager( L"DungeonData.lua" );

	//퀘스트 데이타 로드
	//g_pData->ResetQuestManager( L"Quest.lua", L"SubQuest.lua" );

	// 스킬 데이터 로드
	//g_pData->ResetSkillTree( L"SkillTemplet.lua" );
	g_pData->ResetSkillTree( L"NewSkillTemplet.lua", L"SkillData.lua" );

	g_pData->ResetSlideShot();

	g_pData->ResetUIManager();

	//g_pMain->GetGameOption().OpenScriptFile();

	// 파티 매니저 로드
	//g_pData->ResetPartyManager();


	//g_pData->LoadUserUnitMotion1();
	//g_pData->LoadUserUnitMotion2();
	//g_pData->LoadUserUnitMotion3();
	g_pData->LoadCommonDevice();
	////g_pData->LoadElswordDevice();
	////g_pData->LoadAishaDevice();
	////g_pData->LoadLenaDevice();
	//g_pData->LoadPicChar1();
	//g_pData->LoadPicChar2();
	//g_pData->LoadPicChar3();
	//g_pData->LoadPicChar4();
	//g_pData->LoadPicChar5();
	//g_pData->LoadPicChar6();
	//g_pData->LoadPicChar7();
	//g_pData->LoadPicChar8();
	//g_pData->LoadPicChar9();
	//g_pData->LoadPicChar10();

	g_pData->ResetDamageManager();
}

CX2WorldToolMain::~CX2WorldToolMain(void)
{
	if( m_pGame != NULL )
		m_pGame->GameEnd();
	SAFE_DELETE(m_pGame);

	SAFE_CLOSE( m_pXMeshBoundingSphere );
	
	SAFE_DELETE( m_pXMeshPlayer );
	SAFE_DELETE( m_pParticleSystem );
	SAFE_DELETE( m_pCharacterSkinAnim );
	
	SAFE_DELETE( m_pMatrix );
	SAFE_CLOSE( m_pXSkinMesh );
	

	for( int i=0; i<12 ; i++ )
	{
		SAFE_RELEASE( m_pMesh3DText[i] );
		m_pMesh3DText[i] = NULL;
	}
	
	DeleteAllSkinMeshAndSkinAnim();

	ClearUserUnitStateListBox();
	
	FinalDialog();
}

CX2WorldToolMain::CX2WorldToolMain(void)
{
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "X2WorldTool; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );

	m_vFixedCharacterSkinAnimPosition	= D3DXVECTOR3( 0, 0, 0 );
	m_vFixedCharacterSkinAnimRotation	= D3DXVECTOR3( 0, 0, 0 );

	m_bFixPostionCharacterSkinAnim	= false;
	m_bShowCharacterSkinAnim		= false;
	m_bShowUserUnit			= true;
	// 2009.02.25 김태완 : 디폴트를 SUFFIX로 변경
	m_eLineMapFileNameType	= LFT_SUFFIX;
	//m_eLineMapFileNameType	= LFT_PREFIX;

	m_bWorldColor			= false;
	m_fNowWorldColorTime	= 0.f;
	m_fWorldColorTime		= 2.f;
	m_fWorldColorFadeTime	= 2.f;
	m_bPaused				= false;
	m_bRenderLineMap		= true;
	m_pCharacterSkinAnim	= NULL;
	m_pGame					= NULL;
	m_iPrevToolMode			= MODE_WORLDTOOL;
	m_iToolMode				= MODE_GAMEPLAY;
	m_fObjectMoveSpeed		= 4.f;
	m_CurrUnitClass			= CX2Unit::UC_ELSWORD_SWORDMAN;

	m_bRenderBoundingSphere	 = false;
	m_bRenderFlicker = true;
	m_bCenterGreenDot = false;
	

	m_bShowUserStartPos	= true;
	m_bShowNpcStartPos	= true;
	m_bShowWorld		= true;

	m_bShowLineMapModifier = false;
	
	m_pXMeshBoundingSphere = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"sphere0.x" );

	GetGameOption()->SetMusic( false );
	GetGameOption()->SetSoundVolume( 5000.f );

	// 09.03.02 김태완 : 옵션 강제 최고설정
	GetGameOption()->SetTexDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption()->SetUnitDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption()->SetMapDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption()->SetEffectDetail( true, true );



	InitDialog();
	InitX2Data();
	UpdateMonsterList();

	//CreateGame();

	// 이펙트 툴
	m_pXMeshPlayer				= NULL;
	m_iPickedXMeshPlayerIndex	= -1;
	m_iPickedParticleIndex		= -1;
	m_iPickedUserUnitState		= -1;
	m_bUseUserUnitState			= false;
	
	m_pXMeshPlayer				= new CKTDGXMeshPlayer();
		
	m_pParticleSystem			= NULL;
	
	m_coFadeWorldColor			= D3DXCOLOR(0, 0, 0, 1);



	m_pFont_A	= g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_11_NORMAL );


    m_bFog = false;
    m_fogColor = D3DXCOLOR(0xd8e3fe);

    StringCchPrintf( wszTempFilePath, sizeof(wszTempFilePath), L"C:\\tempWorld_line_map.lua" );
	
	//////////////////////////////////////////////////////////////////////////
	//WCHAR wszExeFolder[512] = L"";
	//GetCurrentDirectory( sizeof( wszExeFolder ), wszExeFolder );

	//WCHAR wszTempWorldScriptFileName[512];
	//WCHAR wszTempLineMapScriptFileName_Prefix[512];
	//WCHAR wszTempLineMapScriptFileName_Suffix[512];

	//swprintf( wszTempWorldScriptFileName, sizeof(wszTempWorldScriptFileName), L"%s\\Temp_World_Script.lua", wszExeFolder );
	m_wstrTempWorldScript = L"Temp_World_Script.lua";

	//swprintf( wszTempLineMapScriptFileName_Suffix, sizeof(wszTempLineMapScriptFileName_Suffix), L"%s\\Temp_World_Script_LineMap.lua", wszExeFolder );
	m_wstrTempLineMapScript_Suffix = L"Temp_World_Script_LineMap.lua";

	//swprintf( wszTempLineMapScriptFileName_Prefix, sizeof(wszTempLineMapScriptFileName_Prefix), L"%s\\LineMap_Temp_World_Script.lua", wszExeFolder );
	m_wstrTempLineMapScript_Prefix = L"LineMap_Temp_World_Script.lua";


	//HANDLE				hSearch;
	//WIN32_FIND_DATAW	fd;

	//hSearch = FindFirstFile( wszTempWorldScriptFileName, &fd );
	//if( hSearch == INVALID_HANDLE_VALUE )
	//{
	//	HANDLE hFile = CreateFile(wszTempWorldScriptFileName,	// file to open
	//		GENERIC_READ,          								// open for reading
	//		FILE_SHARE_READ,       								// share for reading
	//		NULL,                  								// default security
	//		CREATE_NEW,         								// existing file only
	//		FILE_ATTRIBUTE_NORMAL, 								// normal file
	//		NULL);                 								// no attr. template

	//	if( hFile == INVALID_HANDLE_VALUE ) 
	//	{ 
	//		;
	//	}

	//	CloseHandle( hFile );
	//}

	//FindClose(hSearch);


	//hSearch = FindFirstFile( m_wstrTempLineMapScript_Suffix.c_str(), &fd );
	//if( hSearch == INVALID_HANDLE_VALUE )
	//{
	//	HANDLE hFile = CreateFile(m_wstrTempLineMapScript_Suffix.c_str(),	// file to open
	//		GENERIC_READ,          								// open for reading
	//		FILE_SHARE_READ,       								// share for reading
	//		NULL,                  								// default security
	//		CREATE_NEW,         								// existing file only
	//		FILE_ATTRIBUTE_NORMAL, 								// normal file
	//		NULL);                 								// no attr. template

	//	if( hFile == INVALID_HANDLE_VALUE ) 
	//	{ 
	//		;
	//	}

	//	CloseHandle( hFile );
	//}

	//FindClose(hSearch);


	//hSearch = FindFirstFile( m_wstrTempLineMapScript_Prefix.c_str(), &fd );
	//if( hSearch == INVALID_HANDLE_VALUE )
	//{
	//	HANDLE hFile = CreateFile(m_wstrTempLineMapScript_Prefix.c_str(),	// file to open
	//		GENERIC_READ,          								// open for reading
	//		FILE_SHARE_READ,       								// share for reading
	//		NULL,                  								// default security
	//		CREATE_NEW,         								// existing file only
	//		FILE_ATTRIBUTE_NORMAL, 								// normal file
	//		NULL);                 								// no attr. template

	//	if( hFile == INVALID_HANDLE_VALUE ) 
	//	{ 
	//		;
	//	}

	//	CloseHandle( hFile );
	//}

	//FindClose(hSearch);

	//////////////////////////////////////////////////////////////////////////
	//SVN 데이터 폴더 패스를 넣어둔다.
	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
	if( true == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"DevPath.lua" ) )
	{
		bool bEnable = false;

		LUA_GET_VALUE( luaManager, "ENABLE", bEnable, false );

		if( bEnable == true )
		{
			std::string strBaseDir;
			int	iMaxPath = 0;

			LUA_GET_VALUE( luaManager, "PATH_BASIC", strBaseDir, "" );
			LUA_GET_VALUE( luaManager, "PATH_MAX", iMaxPath, 0 );

			std::string strTemp;
			std::string strAdd;
			char szTemp[MAX_PATH] = "";
			for( int i = 0; i < iMaxPath; ++i )
			{
				sprintf(szTemp, "%s_%d", "PATH", i);
				LUA_GET_VALUE( luaManager, szTemp, strTemp, "" );

				if( strTemp.empty() == false )
				{
					strAdd = strBaseDir;
					strAdd += strTemp;
					g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(strAdd);

					strTemp.clear();
				}
			}
		}
	}
	//SVN 데이터 폴더 패스를 넣어둔다.
	//////////////////////////////////////////////////////////////////////////

	m_bMouseDragging	= false;
	m_CurrKeyUsed = 0;
	m_bShowUserUnitStartPos = true;
	m_bUseNPCMesh = false;
	
	iTeamColor = TEAM_RED;
	iInitialDir = DIR_LEFT;
	m_bHideStartPosMesh = false;
	m_iPickedStartPosIndex = -1;
	m_iStartPosListIndex = -1;
	m_iShowDungeonLevel = 0;

	m_pMatrix = new CKTDGMatrix( g_pKTDXApp->GetDevice() );
	m_pXSkinMesh = NULL;

	m_pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"StartPos_Elsword.x" );
	if( NULL == m_pXSkinMesh )
		MessageBox( g_pKTDXApp->GetHWND(), L"StartPos_Elsword.x 파일이 없습니다.", L"죄송^^", MB_OK );
	
	for( int i=0; i<sizeof(m_abKeyUsed); i++ )
	{
		m_abKeyUsed[i] = false;
	}

	m_f3DTextAngle = 0.f;
	m_f3DTextAngleStep = 0.2f;
	// ready 3d text font 
	WCHAR wszTemp[32];
	for( int i=0; i<10; i++ )
	{
		m_pMesh3DText[i] = NULL;		
		StringCchPrintf( wszTemp, sizeof(wszTemp), L"%d", i );
		CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[i], L"Arial", 12, FALSE, FALSE, wszTemp );			
	}

	m_pMesh3DText[10] = NULL;
	swprintf( wszTemp, sizeof(wszTemp)/sizeof(WCHAR)-1, L"%s", L"-" );
	CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[10], L"Arial", 12, FALSE, FALSE, wszTemp );

	m_pMesh3DText[11] = NULL;
	swprintf( wszTemp, sizeof(wszTemp)/sizeof(WCHAR)-1, L"%s", L"*" );
	CreateD3DXTextMesh( g_pKTDXApp->GetDevice(), &m_pMesh3DText[11], L"Arial", 12, FALSE, FALSE, wszTemp );
}

HRESULT CX2WorldToolMain::OnLostDevice()
{
	HRESULT hr = S_OK;

	if( NULL != m_pGame )
		hr = m_pGame->OnLostDevice();

	if( NULL != m_pParticleSystem )
		hr = m_pParticleSystem->OnLostDevice();

	//if( NULL != m_pCharacterSkinMesh )
	//	m_pCharacterSkinMesh->OnLostDevice();

	//if( NULL != m_pViewerSkinMesh )
	//	m_pViewerSkinMesh->OnLostDevice();

	for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
	{	
		hr = vecpXSkinAnim[i]->OnLostDevice();
	}
	
	hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnLostDevice();
	return hr;
}

HRESULT CX2WorldToolMain::OnResetDevice()
{
	HRESULT hr = S_OK;

	UpdateDialog();

	D3DXVECTOR3 vEye	= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
	D3DXVECTOR3 vLookAt = g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();
	m_FPSCamera.SetViewParams( &vEye, &vLookAt );
	m_FPSCamera.SetProjParams( D3DX_PI/4, 1.f, g_pKTDXApp->GetDGManager()->GetNear(), g_pKTDXApp->GetDGManager()->GetFar() );
	m_FPSCamera.SetRotateButtons( false, false, true, false );


	if( NULL != m_pGame )
		hr = m_pGame->OnResetDevice();

	if( NULL != m_pParticleSystem )
		hr = m_pParticleSystem->OnResetDevice();

	//if( NULL != m_pCharacterSkinMesh )
	//	m_pCharacterSkinMesh->OnResetDevice();

	//if( NULL != m_pViewerSkinMesh )
	//	m_pViewerSkinMesh->OnResetDevice();

	for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
	{	
		hr = vecpXSkinAnim[i]->OnResetDevice();
	}

	hr = g_pKTDXApp->GetDGManager()->GetDialogManager()->OnResetDevice();
	return hr;
}

HRESULT CX2WorldToolMain::OnFrameMove( double fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;

	if( m_pGame == NULL )
	{
		CreateGame();
		UpdateSquareCameraUI();
	}


	//if( NULL != m_pViewerSkinMesh )
	//	m_pViewerSkinMesh->OnFrameMove( fTime, fElapsedTime );

	switch( m_iToolMode )
	{
	case MODE_WORLDTOOL:
	case MODE_EFFECTTOOL:
		{
			m_FPSCamera.FrameMove( fElapsedTime * m_FPSCamera.m_fMoveSpeed );
			g_pKTDXApp->GetDGManager()->GetCamera().Move( m_FPSCamera.GetEyePt()->x, m_FPSCamera.GetEyePt()->y, m_FPSCamera.GetEyePt()->z );
			D3DXVECTOR3 vLookAt = *m_FPSCamera.GetWorldAhead() * g_pKTDXApp->GetDGManager()->GetFar()*0.1f + *m_FPSCamera.GetEyePt();
			g_pKTDXApp->GetDGManager()->GetCamera().LookAt( vLookAt.x, vLookAt.y, vLookAt.z );
			g_pKTDXApp->GetDGManager()->GetCamera().UpdateCamera( fElapsedTime );

			DisableGameKey();
			if( NULL != m_pGame &&
				NULL != m_pGame->GetWorld() )
			{
				m_pGame->GetWorld()->OnFrameMove( fTime, fElapsedTime );
			}

			// preview by particle
			D3DXVECTOR3 vPosition;
			if( m_sPreviewParticle.IsCreate() == true )
			{
				if( m_pParticleSystem != NULL )
				{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                    m_pParticleSystem->DestroyInstance_LUA( m_sPreviewParticle.GetEventSequencePointer() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					m_pParticleSystem->DestroyInstance( m_sPreviewParticle.GetEventSequencePointer() );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
					m_sPreviewParticle.SetNULLPointerbyEventSequence();

					if( m_pGame != NULL && m_pGame->GetMyUnit() != NULL )
					{
						vPosition = m_pGame->GetMyUnit()->GetPos();
						vPosition.y += 300.f;

						m_sPreviewParticle.SetEventSequencePointer( m_pParticleSystem->CreateSequence( NULL,  m_sPreviewParticle.GetParticleName().c_str(), vPosition ) );
					}
				}

				m_sPreviewParticle.SetIsCreate(false);
			}

			// preview by XMeshPlayer
			if( m_sPreviewXMeshPlayer.IsCreate() == true )
			{
				if( m_pXMeshPlayer != NULL )
				{
					if( m_pXMeshPlayer != NULL && m_sPreviewXMeshPlayer.GetXMeshInstPointer() != NULL )
					{
						CKTDGXMeshPlayer::CXMeshInstanceHandle hHandle = m_sPreviewXMeshPlayer.GetXMeshInstPointer()->GetHandle();
						m_pXMeshPlayer->DestroyInstance( hHandle );
						m_sPreviewXMeshPlayer.SetNULLPointerbyXMeshInst();
					}

					if( m_pGame != NULL && m_pGame->GetMyUnit() != NULL )
					{
						vPosition = m_pGame->GetMyUnit()->GetPos();
						vPosition.y += 300.f;

						D3DXVECTOR3 vAngleDegree = g_pX2Game->GetMyUnit()->GetRotateDegree();

						D3DXVECTOR3 vAxisDegree(0,0,0);
						int iLayer = -1;

						m_sPreviewXMeshPlayer.SetXMeshInstPointer( m_pXMeshPlayer->CreateInstance( NULL,  m_sPreviewXMeshPlayer.GetXMeshName().c_str(), 
							vPosition, vAngleDegree, vAxisDegree, iLayer ) );
					}
				}

				m_sPreviewXMeshPlayer.SetIsCreate( false );
			}

		} break;
	case MODE_GAMEPLAY:
		{
			if( m_pParticleSystem != NULL )	// 게임모드가 되면 프리뷰는 초기화 해준다.
			{
#ifdef  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
                m_pParticleSystem->DestroyInstance_LUA( m_sPreviewParticle.GetEventSequencePointer() );
#else   X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				m_pParticleSystem->DestroyInstance( m_sPreviewParticle.GetEventSequencePointer() );
#endif  X2OPTIMIZE_PARTICLE_AND_ETC_HANDLE
				m_sPreviewParticle.SetNULLPointerbyEventSequence();
			}
			else if( m_pXMeshPlayer != NULL && m_sPreviewXMeshPlayer.GetXMeshInstPointer() != NULL )
			{
				CKTDGXMeshPlayer::CXMeshInstanceHandle hHandle = m_sPreviewXMeshPlayer.GetXMeshInstPointer()->GetHandle();
				m_pXMeshPlayer->DestroyInstance( hHandle );
				m_sPreviewXMeshPlayer.SetNULLPointerbyXMeshInst();
			}

			if( false == m_bPaused )
			{
				//if( NULL != m_pGame )
				//{
				//	hr = m_pGame->OnFrameMove( fTime, fElapsedTime );
				//	m_pXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
				//}

				if( NULL != m_pParticleSystem && 
					NULL != m_pCharacterSkinAnim &&
					NULL != m_pCharacterSkinAnim->GetXSkinAnim() ) 
				{
					if( true == m_pCharacterSkinAnim->IsPlaying() )
					{

						//////////////////////////////////////////////////////////////////////////
						for( int i=0; i<(int)m_vecReadyToPlayTimedParticle.size(); i++ )
						{
							TimedParticle& timedParticle = m_vecReadyToPlayTimedParticle[i];


							if( timedParticle.m_fDelayTimeToStart >= m_pCharacterSkinAnim->GetXSkinAnim()->GetMaxAnimationTime() )
							{
								m_vecReadyToPlayTimedParticle.erase( m_vecReadyToPlayTimedParticle.begin() + i ); 
								i--;
								continue;
							}

							if( timedParticle.m_fDelayTimeToStart < m_pCharacterSkinAnim->GetXSkinAnim()->GetNowAnimationTime() )
							{
								if( true == timedParticle.m_bRelativeToUnit )
								{
									D3DXVECTOR3 vPosition;
									if( 0 != timedParticle.m_wstrBoneName.length() )
									{
										vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetCloneFramePosition( timedParticle.m_wstrBoneName.c_str() ) 
											+ timedParticle.m_vRelativePosition;
									}
									else
									{
										vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetMatrix().GetPos() + timedParticle.m_vRelativePosition;

									}

									D3DXVECTOR3 vAngleDegree = g_pX2Game->GetMyUnit()->GetRotateDegree();
									D3DXVECTOR3 vAxisDegree(0,0,0);

									TimedParticle playingTimedParticle = timedParticle;
									playingTimedParticle.m_pEventSequence = m_pParticleSystem->CreateSequence( NULL,  timedParticle.m_wstrParticleName.c_str(), vPosition );

									if( true == timedParticle.m_bTraceUnit )
									{
										m_vecPlayingTimedParticle.push_back( playingTimedParticle );
									}									
								}
								else
								{
									
									D3DXVECTOR3 vPosition = timedParticle.m_vRelativePosition;

									D3DXVECTOR3 vAngleDegree = g_pX2Game->GetMyUnit()->GetRotateDegree();
									D3DXVECTOR3 vAxisDegree(0,0,0);
									
									TimedParticle playingTimedParticle = timedParticle;
									playingTimedParticle.m_pEventSequence = m_pParticleSystem->CreateSequence( NULL,  timedParticle.m_wstrParticleName.c_str(), vPosition );

								}

								m_vecReadyToPlayTimedParticle.erase( m_vecReadyToPlayTimedParticle.begin() + i ); 
								i--;
							}
						}
					}
					
					
					//////////////////////////////////////////////////////////////////////////
					for( int i=0; i<(int) m_vecPlayingTimedParticle.size(); i++ ) 
					{
						TimedParticle& playingTimedParticle = m_vecPlayingTimedParticle[i];
						if( NULL != playingTimedParticle.m_pEventSequence )
						{
							if( false == m_pParticleSystem->IsLiveInstance( playingTimedParticle.m_pEventSequence ) )
							{
								m_vecPlayingTimedParticle.erase( m_vecPlayingTimedParticle.begin() + i ); 
								i--;
							}
						}
						else
						{
							m_vecPlayingTimedParticle.erase( m_vecPlayingTimedParticle.begin() + i ); 
							i--;
						}
					}



					//////////////////////////////////////////////////////////////////////////
					for( int i=0; i<(int) m_vecPlayingTimedParticle.size(); i++ ) 
					{
						TimedParticle& playingTimedParticle = m_vecPlayingTimedParticle[i];


						if( NULL != playingTimedParticle.m_pEventSequence )
						{
							if( true == playingTimedParticle.m_bTraceUnit )
							{

								D3DXVECTOR3 vPosition;
								if( 0 != playingTimedParticle.m_wstrBoneName.length() )
								{
									vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetCloneFramePosition( playingTimedParticle.m_wstrBoneName.c_str() ) 
										+ playingTimedParticle.m_vRelativePosition;
								}
								else
								{
									vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetMatrix().GetPos() 
										+ playingTimedParticle.m_vRelativePosition;
								}

								D3DXVECTOR3 vAngleDegree = g_pX2Game->GetMyUnit()->GetRotateDegree();

								playingTimedParticle.m_pEventSequence->SetPosition( vPosition );
								//playingTimedParticle.m_pEventSequence->SetAddRotate( vAngleDegree );
								//playingTimedParticle.m_pEventSequence->SetAxisAngle( vAngleDegree );

							}
						}
					}

					//m_pParticleSystem->OnFrameMove( fTime, fElapsedTime );
				}


				if( NULL != m_pCharacterSkinAnim &&
					NULL != m_pCharacterSkinAnim->GetXSkinAnim() )
				{
					if( m_pCharacterSkinAnim->IsPlaying() )
					{
						//bool bIsRight				= m_pGame->GetMyUnit()->GetIsRight();
						//D3DXVECTOR3 vPos			= m_pGame->GetMyUnit()->GetPos();
						bool bIsRight				= true;
						D3DXVECTOR3 vPos			= m_pCharacterSkinAnim->GetPos();
						D3DXVECTOR3 vDirVec 		= m_pGame->GetMyUnit()->GetDirVector();
						D3DXVECTOR3 vMotionOffset	= m_pCharacterSkinAnim->GetMotionOffset();
						
						//모션 이동
						if( true == bIsRight )
						{
							vPos.x += vDirVec.x * vMotionOffset.x;
							vPos.z += vDirVec.z * vMotionOffset.x;
						}
						else
						{
							vPos.x -= vDirVec.x * vMotionOffset.x;
							vPos.z -= vDirVec.z * vMotionOffset.x;
						}
						
						if( true == m_bFixPostionCharacterSkinAnim )
						{
							m_pCharacterSkinAnim->SetPos( m_vFixedCharacterSkinAnimPosition );
							m_pCharacterSkinAnim->SetRotationDegree( m_vFixedCharacterSkinAnimRotation );
						}
						else
						{
							m_pCharacterSkinAnim->SetPos( vPos );
						}
					}
					else
					{
						if( true == m_bFixPostionCharacterSkinAnim )
						{
							m_pCharacterSkinAnim->SetPos( m_vFixedCharacterSkinAnimPosition );
							m_pCharacterSkinAnim->SetRotationDegree( m_vFixedCharacterSkinAnimRotation );

						}
						else
						{
							m_pCharacterSkinAnim->SetPos( m_pGame->GetMyUnit()->GetPos() );
						}

					}


					m_pCharacterSkinAnim->OnFrameMove( fTime, fElapsedTime );

					//////////////////////////////////////////////////////////////////////////
					for( int i=0; i<(int)m_vecReadyToPlayTimedXMeshPlayer.size(); i++ )
					{
						TimedXMeshPlayer& timedXMeshPlayer = m_vecReadyToPlayTimedXMeshPlayer[i];

						if( NULL != m_pCharacterSkinAnim &&
							NULL != m_pCharacterSkinAnim->GetXSkinAnim() && 
							true == m_pCharacterSkinAnim->IsPlaying() )
						{
							if( timedXMeshPlayer.m_fDelayTimeToStart >= m_pCharacterSkinAnim->GetXSkinAnim()->GetMaxAnimationTime() )
							{
								m_vecReadyToPlayTimedXMeshPlayer.erase( m_vecReadyToPlayTimedXMeshPlayer.begin() + i ); 
								i--;
								continue;
							}

							if( timedXMeshPlayer.m_fDelayTimeToStart < m_pCharacterSkinAnim->GetXSkinAnim()->GetNowAnimationTime() )
							{

								if( true == timedXMeshPlayer.m_bRelativeToUnit )
								{

									D3DXVECTOR3 vPosition;

									if( 0 != timedXMeshPlayer.m_wstrBoneName.length() )
									{
										//vPosition = g_pX2Game->GetMyUnit()->GetPos() + timedXMeshPlayer.m_vRelativePosition +
										//			m_pCharacterSkinAnim->GetXSkinAnim()->GetCloneFramePosition( timedXMeshPlayer.m_wstrBoneName.c_str() );
										vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetCloneFramePosition( timedXMeshPlayer.m_wstrBoneName.c_str() ) + timedXMeshPlayer.m_vRelativePosition;
									}
									else
									{
										//vPosition = g_pX2Game->GetMyUnit()->GetPos() + timedXMeshPlayer.m_vRelativePosition;
										vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetMatrix().GetPos() + timedXMeshPlayer.m_vRelativePosition;

									}

									D3DXVECTOR3 vAngleDegree = g_pX2Game->GetMyUnit()->GetRotateDegree();

									//D3DXVECTOR3 vAngleDegree(0,0,0);
									//if( false == g_pX2Game->GetMyUnit()->GetSyncData().bIsRight )
									//	vAngleDegree.y = -180.f;
									D3DXVECTOR3 vAxisDegree(0,0,0);
									int iLayer = -1;



									TimedXMeshPlayer playingTimedXMeshPlayer = timedXMeshPlayer;
									playingTimedXMeshPlayer.m_pInstance = m_pXMeshPlayer->CreateInstance( NULL,  timedXMeshPlayer.m_wstrXMeshPlayerName.c_str(), 
										vPosition, vAngleDegree, vAxisDegree, iLayer );

									if( true == timedXMeshPlayer.m_bTraceUnit )
									{
										m_vecPlayingTimedXMeshPlayer.push_back( playingTimedXMeshPlayer );
									}
								}
								else
								{
									D3DXVECTOR3 vPosition = timedXMeshPlayer.m_vRelativePosition;

									D3DXVECTOR3 vAngleDegree = g_pX2Game->GetMyUnit()->GetRotateDegree();
									D3DXVECTOR3 vAxisDegree(0,0,0);
									int iLayer = -1;

									TimedXMeshPlayer playingTimedXMeshPlayer = timedXMeshPlayer;
									playingTimedXMeshPlayer.m_pInstance = m_pXMeshPlayer->CreateInstance( NULL,  timedXMeshPlayer.m_wstrXMeshPlayerName.c_str(), 
										vPosition, vAngleDegree, vAxisDegree, iLayer );
								}


								m_vecReadyToPlayTimedXMeshPlayer.erase( m_vecReadyToPlayTimedXMeshPlayer.begin() + i ); 
								i--;
							}
						}
					}


					//////////////////////////////////////////////////////////////////////////
					for( int i=0; i<(int) m_vecPlayingTimedXMeshPlayer.size(); i++ ) 
					{
						TimedXMeshPlayer& playingTimedXMeshPlayer = m_vecPlayingTimedXMeshPlayer[i];
						if( NULL != playingTimedXMeshPlayer.m_pInstance )
						{
							if( false == m_pXMeshPlayer->IsLiveInstance( playingTimedXMeshPlayer.m_pInstance ) )
							{
								m_vecPlayingTimedXMeshPlayer.erase( m_vecPlayingTimedXMeshPlayer.begin() + i ); 
								i--;
							}
						}
						else
						{
							m_vecPlayingTimedXMeshPlayer.erase( m_vecPlayingTimedXMeshPlayer.begin() + i ); 
							i--;
						}
					}



					//////////////////////////////////////////////////////////////////////////
					for( int i=0; i<(int) m_vecPlayingTimedXMeshPlayer.size(); i++ ) 
					{
						TimedXMeshPlayer& playingTimedXMeshPlayer = m_vecPlayingTimedXMeshPlayer[i];

						if( NULL != playingTimedXMeshPlayer.m_pInstance )
						{
							if( true == playingTimedXMeshPlayer.m_bTraceUnit )
							{

								D3DXVECTOR3 vPosition;
								if( 0 != playingTimedXMeshPlayer.m_wstrBoneName.length() )
								{
									//vPosition = g_pX2Game->GetMyUnit()->GetPos() + playingTimedXMeshPlayer.m_vRelativePosition +
									//	m_pCharacterSkinAnim->GetXSkinAnim()->GetCloneFramePosition( playingTimedXMeshPlayer.m_wstrBoneName.c_str() );
									vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetCloneFramePosition( playingTimedXMeshPlayer.m_wstrBoneName.c_str() ) 
												+ playingTimedXMeshPlayer.m_vRelativePosition;
								}
								else
								{
									//vPosition = g_pX2Game->GetMyUnit()->GetPos() + playingTimedXMeshPlayer.m_vRelativePosition;

									vPosition = m_pCharacterSkinAnim->GetXSkinAnim()->GetMatrix().GetPos() + playingTimedXMeshPlayer.m_vRelativePosition;


									
								}
							
								D3DXVECTOR3 vAngleDegree = g_pX2Game->GetMyUnit()->GetRotateDegree();

								playingTimedXMeshPlayer.m_pInstance->SetPos( vPosition );
								playingTimedXMeshPlayer.m_pInstance->SetRotateDegree( vAngleDegree );
								playingTimedXMeshPlayer.m_pInstance->SetMoveAxisAngleDegree( vAngleDegree );

							}
						}
					}
				}

				if( NULL != m_pCharacterSkinAnim &&
					NULL != m_pCharacterSkinAnim->GetXSkinAnim() )
				{
					if( m_pCharacterSkinAnim->IsPlaying() )
					{
						for( int i=0; i<(int)m_vecReadyToPlayCameraShake.size(); i++ )
						{
							CameraShake& cameraShake = m_vecReadyToPlayCameraShake[i];
							if( cameraShake.m_fStartTime >= m_pCharacterSkinAnim->GetXSkinAnim()->GetMaxAnimationTime() )
							{
								m_vecReadyToPlayCameraShake.erase( m_vecReadyToPlayCameraShake.begin() + i );
								i--;
								continue;
							}

							if( cameraShake.m_fStartTime < m_pCharacterSkinAnim->GetXSkinAnim()->GetNowAnimationTime() )
							{
								if( NULL != m_pGame->GetX2Camera() && 
									NULL != m_pGame->GetX2Camera()->GetCamera() )
								{
									m_pGame->GetX2Camera()->GetCamera().UpDownCrashCameraNoReset( cameraShake.m_fAmplitude, cameraShake.m_fDuration );
								}
								m_vecReadyToPlayCameraShake.erase( m_vecReadyToPlayCameraShake.begin() + i );
								i--;
								continue;
							}
						}
					}
				}


				if( m_fNowWorldColorTime > 0.f )
				{
					m_fNowWorldColorTime -= fElapsedTime;

					if( m_fNowWorldColorTime <= 0.f )
					{
						g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), m_fWorldColorFadeTime );
						m_fNowWorldColorTime = 0.f;
					}
				}
			}

		} break;
	}

	if( NULL != m_pGame &&
		NULL != m_pXMeshPlayer &&
		false == m_bPaused )
	{
		hr = m_pGame->OnFrameMove( fTime, fElapsedTime );
		m_pXMeshPlayer->OnFrameMove( fTime, fElapsedTime );
		if( m_pParticleSystem != NULL )
			m_pParticleSystem->OnFrameMove( fTime, fElapsedTime );
	}

	if( m_pGame != NULL && m_pGame->GetWorld() != NULL && m_pGame->GetWorld()->GetLineMap() != NULL )
	{
		m_pGame->GetWorld()->GetLineMap()->UpdateLineVB();
	}


	// start pos 개수 만큼 skinanim 개수를 맞추어주자
	if( m_bShowLineMapModifier == true )
	{
		if( vecpXSkinAnim.size() < vecTeamStartPos.size() )
		{
			for( unsigned i=vecpXSkinAnim.size(); i<vecTeamStartPos.size(); i++ )
			{
				//CKTDGXSkinAnim* pXSkinAnim = new CKTDGXSkinAnim( g_pKTDXApp->GetDevice() );
				CKTDGXSkinAnim* pXSkinAnim = CKTDGXSkinAnim::CreateSkinAnim();

				pXSkinAnim->SetAnimXSkinMesh(  m_pXSkinMesh );
				pXSkinAnim->AddModelXSkinMesh( m_pXSkinMesh );
				pXSkinAnim->SetAlphaObject( true );
				pXSkinAnim->ChangeAnim( L"LobbyReadyAction", false );
				pXSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
				vecpXSkinAnim.push_back( pXSkinAnim );
			}
		}

		for( unsigned i=0; i<vecpXSkinAnim.size(); i++ )
		{	
			hr = vecpXSkinAnim[i]->OnFrameMove(fTime, fElapsedTime);
		}		
	}

	return hr;
}


HRESULT CX2WorldToolMain::OnFrameRender()
{
	HRESULT hr = S_OK;

	if( NULL != m_pCharacterSkinAnim )
	{
		m_pCharacterSkinAnim->OnFrameRender();
	}

	//if( NULL != m_pViewerSkinMesh )
	//	m_pViewerSkinMesh->OnFrameRender();

	if( NULL != m_pGame )
	{
		if( m_bShowWorld == true )
			hr = m_pGame->OnFrameRender();

		if( true == m_bRenderLineMap && 
			NULL != m_pGame->GetWorld() &&
			NULL != m_pGame->GetWorld()->GetLineMap() )
		{
			//hr = m_pGame->GetWorld()->GetLineMap()->OnFrameRender();
			m_pGame->GetWorld()->GetLineMap()->OnFrameRender();
			hr = S_OK;
		}		
	}

	switch( m_iToolMode )
	{
	case MODE_WORLDTOOL:
	case MODE_EFFECTTOOL:
		{

			D3DXVECTOR3 vCenter;
			float fRadius;

			if( true == m_bRenderBoundingSphere )
			{
				CKTDGStateManager::PushRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );			
				for( UINT i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
				{
					CWorldToolObject* pWorldToolObject = m_vecpPickedWorldToolObject[i];
					pWorldToolObject->GetBoundingSphere( vCenter, fRadius );
					vCenter += pWorldToolObject->GetMatrix().GetPos();
					RenderBoundingSphere( vCenter, fRadius*0.3f );

					//if( m_TimerRenderPickedObject.elapsed() > 1.f )
					//{
					//	m_TimerRenderPickedObject.restart();
					//}
					//else if( m_TimerRenderPickedObject.elapsed() > 0.4f )
					//{
					//	pWorldToolObject->GetBoundingSphere( vCenter, fRadius );
					//	D3DXVECTOR3 vOriginalPos = pWorldToolObject->GetPosition();
					//	D3DXVECTOR3 vOriginalScale = pWorldToolObject->GetScale();

					//	pWorldToolObject->AddPositionRelative( (vOriginalPos - vCenter) * 0.05f );
					//	pWorldToolObject->SetScale( vOriginalScale * 1.05f );
					//	pWorldToolObject->Render();
					//	pWorldToolObject->SetScale( vOriginalScale );
					//	pWorldToolObject->SetPosition( vOriginalPos );
					//}
				}
				CKTDGStateManager::PopRenderState( D3DRS_FILLMODE );
			}


			if( true == m_bRenderFlicker )
			{
				if( m_TimerRenderPickedObject.elapsed() > 1.f )
				{
					m_TimerRenderPickedObject.restart();
				}
				else if( m_TimerRenderPickedObject.elapsed() > 0.5f )
				{

					//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
					CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_ADD );
					CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1 , D3DTA_CONSTANT );
					CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2 , D3DTA_CURRENT );

					for( UINT i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
					{
						CWorldToolObject* pWorldToolObject = m_vecpPickedWorldToolObject[i];
						pWorldToolObject->GetBoundingSphere( vCenter, fRadius );
						D3DXVECTOR3 vOriginalPos = pWorldToolObject->GetPosition();
						D3DXVECTOR3 vOriginalScale = pWorldToolObject->GetScale();

						pWorldToolObject->AddPositionRelative( (vOriginalPos - vCenter) * 0.2f );
						pWorldToolObject->SetScale( vOriginalScale * 1.2f );
						
						pWorldToolObject->Render();
						pWorldToolObject->Render();

						pWorldToolObject->SetScale( vOriginalScale );
						pWorldToolObject->SetPosition( vOriginalPos );
					}

					CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
					CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
					CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
				}
			}




			for( UINT i=0; i<m_vecpDialog.size(); i++ )
			{
				if( true == m_vecpDialog[i]->GetEnable() )
				{
					m_vecpDialog[i]->OnRender( g_pKTDXApp->GetElapsedTime() );
				}
			}

			if( true == m_bCenterGreenDot )
			{
				// render camera pos, picker and its pos
				const D3DXVECTOR3* pvCameraPos = GetCamera().GetEyePt();
				D3DXVECTOR3 vLookVec = *GetCamera().GetLookAtPt() - *pvCameraPos;
				D3DXVec3Normalize( &vLookVec, &vLookVec );
				D3DXVECTOR3 vPickerPos = *pvCameraPos + vLookVec * 800.f;

				RenderBoundingSphere( vPickerPos, 2.f );


				WCHAR wszText[128] = {0,};
				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"CAMERA: %d, %d, %d;     GREEN PICKER: %d, %d, %d", 
					(int) pvCameraPos->x, (int) pvCameraPos->y, (int) pvCameraPos->z,
					(int) vPickerPos.x, (int) vPickerPos.y, (int) vPickerPos.z );

				m_pFont_A->OutTextXY( 200, 2, wszText, D3DXCOLOR(1,1,1,1), 
					CKTDGFontManager::FS_SHELL, D3DXCOLOR(0,0,0,1), NULL, DT_LEFT );
			}


			//// note!! 임시로 카메라 위치 화면에 표시 
			//WCHAR wszText[256] = L"";
			//const D3DXVECTOR3* pvEye = m_FPSCamera.GetEyePt();
			//StringCchPrintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%.1f, %.1f, %.1f", pvEye->x, pvEye->y, pvEye->z );
			//m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_POSITION_OFFSET_X )->SetText( wszText );


			//// note!! far plane
			//float fFar = g_pKTDXApp->GetDGManager()->GetFar();
			//StringCchPrintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"FAR - %.1f", fFar );
			//m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_POSITION_OFFSET_Y )->SetText( wszText );

			//CKTDGFrustum* pFrustum = g_pKTDXApp->GetDGManager()->GetFrustum();
			//if( true == pFrustum->CheckPoint( 0, 0, 15000.f ) )
			//{
			//	StringCchPrintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"inside", fFar );
			//	m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_POSITION_OFFSET_Z )->SetText( wszText );
			//}
			//else
			//{
			//	StringCchPrintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"outside", fFar );
			//	m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_POSITION_OFFSET_Z )->SetText( wszText );
			//}

		} break;
	case MODE_GAMEPLAY:
		break;
	}

	if( m_pGame != NULL && m_pGame->GetWorld() != NULL && m_pGame->GetWorld()->GetLineMap() != NULL )
	{
		CKTDGLineMap *pLineMap = m_pGame->GetWorld()->GetLineMap();
		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
			if( pLineData != NULL )
			{
				D3DXVECTOR3 vScreen;
				D3DXVECTOR3 vPos = pLineData->startPos; //(pLineData->endPos - pLineData->startPos) * 0.5f;
				vPos.x += 15.f;
				vPos.y += 24.f;
				WCHAR wBuf[100];
				wsprintf(wBuf, L"%d", pLineData->lineIndex);

				if( pLineData->m_iCameraIndex >= 0 )
					StrCatW(wBuf, L"(C)");
				if( pLineData->m_iTriggerId >= 0 )
					StrCatW(wBuf, L"(T)");
				if( pLineData->m_bUnderWater == true )
				{
					StrCatW(wBuf, L"(W)");
					if( m_bShowLineMapModifier == true )
					{
						D3DXVECTOR3 p[2];
						p[0].x = pLineData->startPos.x; 
						p[0].y = pLineData->m_fWaterHeight; 
						p[0].z = pLineData->startPos.z;
						p[1].x = pLineData->endPos.x; 
						p[1].y = pLineData->m_fWaterHeight; 
						p[1].z = pLineData->endPos.z;

						LPD3DXLINE Line;
						D3DXCreateLine( g_pKTDXApp->GetDevice(), &Line);
						Line->SetWidth(3);				
						Line->Begin();
						D3DXMATRIX matTransform, matTransform1, matTransform2;
						D3DXMatrixIdentity(&matTransform);
						g_pKTDXApp->GetWorldTransform(&matTransform);
						g_pKTDXApp->GetViewTransform(&matTransform1);
						g_pKTDXApp->GetProjectionTransform(&matTransform2);

						Line->DrawTransform(p, 2, &(matTransform * matTransform1* matTransform2), D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f));
						Line->End();
						Line->Release();
					}					
				}

				if( m_bShowLineMapModifier == true )
				{						
					if( pLineData->beforeLine == -1 )
					{
						WCHAR wTemp[10];
						wsprintf(wTemp, L"▲");
						D3DXVECTOR3 vPosTemp = pLineData->startPos;
						vPosTemp.x += 3.f;
						m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xff0000ff, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );					
					}
					else if( pLineData->beforeLine == -2 )
					{
						WCHAR wTemp[10];
						wsprintf(wTemp, L"▲");
						D3DXVECTOR3 vPosTemp = pLineData->startPos;
						vPosTemp.x += 3.f;
						m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xffff0000, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );					
					}

					if( pLineData->nextLine == -1)
					{
						WCHAR wTemp[10];
						wsprintf(wTemp, L"▲");
						D3DXVECTOR3 vPosTemp = pLineData->endPos;
						vPosTemp.x -= 3.f;
						m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xff0000ff, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );					
					}
					else if( pLineData->nextLine == -2)
					{
						WCHAR wTemp[10];
						wsprintf(wTemp, L"▲");
						D3DXVECTOR3 vPosTemp = pLineData->endPos;
						vPosTemp.x -= 3.f;
						m_pFont_A->OutProjectionText( vPosTemp, wTemp, 0xffff0000, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );					
					}
					//
					//if( pLineData->nextLine == -1 )
					//{
					//	m_pSphereAnim->GetMatrix().Move( pLineData->endPos );
					//	m_pSphereAnim->GetMatrix().Scale( D3DXVECTOR3(0.1f, 0.1f, 0.1f) );
					//	m_pSphereAnim->GetRenderParam()->color = D3DXCOLOR(0, 0, 1, 1);
					//	m_pSphereAnim->OnFrameRender();
					//}
					//else if( pLineData->nextLine == -2 )
					//{
					//	m_pSphereAnim->GetMatrix().Move( pLineData->endPos );
					//	m_pSphereAnim->GetMatrix().Scale( D3DXVECTOR3(0.1f, 0.1f, 0.1f) );
					//	m_pSphereAnim->GetRenderParam()->color = D3DXCOLOR(1, 0, 0, 1);
					//	m_pSphereAnim->OnFrameRender();
					//}

					if( pLineData->m_bSelected == true )
					{
						m_pFont_A->OutProjectionText( vPos, wBuf, 0xffff0000, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );					
					}
					else
					{
						m_pFont_A->OutProjectionText( vPos, wBuf, 0xffffffff, CKTDGFontManager::FS_SHELL, 0x00000000, NULL, DT_CENTER );
					}
				}
			}			
		}

		if( m_bShowLineMapModifier == true )
			OnFrameRenderUnit();
	}

	return hr;
}

bool CX2WorldToolMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	bool bNoFurtherProcessing = false;

	CKTDGLineMap *pLineMap = NULL;
	if( m_pGame != NULL && m_pGame->GetWorld() != NULL && m_pGame->GetWorld()->GetLineMap() != NULL )
		pLineMap = m_pGame->GetWorld()->GetLineMap();
	POINTS mousepos;

	switch( uMsg )
	{
	case WM_LBUTTONDOWN:
		{
			//m_bMouseDragging = true;
			
			if( NULL == pLineMap )
				break;
			mousepos = MAKEPOINTS(lParam);
			
			// NPC 유닛 클릭
			if( wParam & MK_SHIFT )
			{
				m_bMouseDragging = false;
				AddNewStartPosition( mousepos );
				PickClickedNPC( m_iShowDungeonLevel, mousepos );
				if( m_bShowLineMapModifier == true && g_hDlgLineMap != NULL )
					UpdateStartPosDlg( g_hDlgLineMap );
			}
			else if( wParam & MK_CONTROL )
			{
				m_bMouseDragging = false;
				PickClickedNPC( m_iShowDungeonLevel, mousepos, false );
				if( m_bShowLineMapModifier == true && g_hDlgLineMap != NULL )
					UpdateStartPosDlg( g_hDlgLineMap );
			}
			else
			{
				m_bMouseDragging = true;
				PickClickedNPC( m_iShowDungeonLevel, mousepos );
				if( m_bShowLineMapModifier == true && g_hDlgLineMap != NULL )
					UpdateStartPosDlg( g_hDlgLineMap );
			}

		} break;

	case WM_MOUSEMOVE:
		{
			mousepos = MAKEPOINTS(lParam);
			
			if( m_iPickedStartPosIndex >= 0 &&
				true == m_bMouseDragging )
			{
				D3DXVECTOR3 vPos;
				if( true == GetMousePosOnLine( &vPos, mousepos ) && (int)vecTeamStartPos.size() > m_iPickedStartPosIndex )
				{					
					if( vPos.y != -1.f )
					{
						vecTeamStartPos.at(m_iPickedStartPosIndex).vPos = vPos;
					}
					break;

//					for( UINT i=0; i<m_vecPickedNPCStartPosKey.size(); i++ )
//					{
//						for( UINT j=0; j<vecTeamStartPos.size(); j++ )
//						{
//							SNPCStartPos& npcStartPos = m_vecPickedNPCStartPosKey[i];
//							if( vecTeamStartPos.at(j).key == npcStartPos.m_iStartPosIndex )
//							{
//								if( vPos.y != -1.f )
//								{
//									vecTeamStartPos.at(j).vPos = vPos;
//									//UpdateStartPosComboBox();
//									//UpdateStartPosListBox( true );
//								}
//								break;
//							}
//						}
//					}
				}
			}			

		} break;

	case WM_LBUTTONUP:
		{	
			mousepos = MAKEPOINTS(lParam);

			if( /*m_MouseClickTarget == MCT_LINE && */
				true == m_bMouseDragging &&
				NULL != pLineMap )
			{
				float w = (float)(int)g_pKTDXApp->GetViewport().Width;
				float h = (float)(int)g_pKTDXApp->GetViewport().Height;

				float MouseX = (float)(mousepos.x-w/2.f)/w *2.f;
				float MouseY = (float)(h/2.f-mousepos.y)/h *2.f;

//				GetLineIndexInRect( m_vDragStart.x, m_vDragStart.y, MouseX, MouseY );

//				for( int i=0; i<pLineMap->GetNumLineData(); i++ )
//				{
//					MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(i)->bSelected = false;
//				}				

//				for( unsigned i=0; i<m_veciPickedSegmentIndex.size() ; i++ )
//				{
//					MainUI.GetListBox( IDC_LISTBOX_SEGMENT_INFO )->GetItem(m_veciPickedSegmentIndex.at(i))->bSelected = true;
//				}

//				UpdateSegmentInfoListBox(true);
//				UpdateSegmentInfoComboBox(false);
			}

			m_bMouseDragging = false;

		} break;

	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				{
					ClearPickedWorldToolObjectList();
					bNoFurtherProcessing = true;
				} break;

			case VK_F11:
				{
					switch( m_iToolMode )
					{
					case MODE_EFFECTTOOL:
						{
							m_iPrevToolMode = MODE_EFFECTTOOL;
							m_iToolMode		= MODE_GAMEPLAY;
							m_pGame->GetMyUnit()->ClearRecieveSyncData();							
						} break;

					case MODE_WORLDTOOL:
						{
							m_iPrevToolMode = MODE_WORLDTOOL;
							m_iToolMode		= MODE_GAMEPLAY;
							m_pGame->GetMyUnit()->ClearRecieveSyncData();							
						} break;

					case MODE_GAMEPLAY:
						{
							m_iToolMode = m_iPrevToolMode;
							D3DXVECTOR3 vLookAt = g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();
							D3DXVECTOR3 vEye	= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
                            //if(m_pGame->m_bOrthoFreeCamera == false)
                            if(g_pKTDXApp->GetDGManager()->GetPerspective())
							    m_FPSCamera.SetViewParams( &vEye, &vLookAt );
							ResetDialogAtModeChange();
						} break;					
					}			
				} break;

			//case VK_TAB:
			case VK_F12:
				{
					switch( m_iToolMode )
					{
					case MODE_EFFECTTOOL:
						m_iToolMode = MODE_WORLDTOOL;
						ResetDialogAtModeChange();
						break;

					case MODE_WORLDTOOL:
						m_iToolMode = MODE_EFFECTTOOL;
						ResetDialogAtModeChange();
						break;
					}
				}

			case VK_F5:
				{
					On_IDC_BUTTON_LOAD_XMESH_PLAYER();

				} break;

			case VK_F6:
				{
					On_IDC_BUTTON_LOAD_PARTICLE();
				} break;
			} 
		} break;

	case WM_CHAR:
		{
			switch(wParam)
			{	
			//{{ 09.03.14 김태완 : 카메라 고정
			case 'f':
				{
					m_pGame->m_bFixedCamera = !m_pGame->m_bFixedCamera;
					CWorldToolDialog* pDialog	= GetDialog( CX2WorldToolMain::DLG_ETC );
					CDXUTCheckBox* pCheckBox= pDialog->GetCheckBox( CX2WorldToolMain::IDC_CHECK_FIXED_CAMERA );
					pCheckBox->SetChecked( m_pGame->m_bFixedCamera );                            

				} break;
			//}}

            case 'l':
            case 'L':
                if(g_hDlg == NULL)
                {
                    g_hDlg = CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_DIALOG1), g_pKTDXApp->GetHWND(), WorldToolDlgProc);
                    ShowWindow(g_hDlg, SW_SHOW);
                }
                else
                {
                    ShowWindow(g_hDlg, SW_HIDE);
                    DestroyWindow(g_hDlg);
                    //EndDialog(g_hDlg, 1);
                    g_hDlg = NULL;
                }                        
                break;
			case 'm':
			case 'M':
				if(g_hDlgLineMap == NULL)
				{
					g_hDlgLineMap = CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_DIALOG3), g_pKTDXApp->GetHWND(), WorldToolLineModifierDlgProc);
					ShowWindow(g_hDlgLineMap, SW_SHOW);
					m_bShowLineMapModifier = true;
				}
				else
				{
					ShowWindow(g_hDlgLineMap, SW_HIDE);
					DestroyWindow(g_hDlgLineMap);
					//EndDialog(g_hDlg, 1);
					g_hDlgLineMap = NULL;
					m_bShowLineMapModifier = false;					
				}            
				break;
			case 'c':
			case 'C':
				if(g_hFrustumDlg == NULL)
				{
					g_hFrustumDlg = CreateDialog(g_pKTDXApp->GetHInstance(), MAKEINTRESOURCE(IDD_DIALOG4), g_pKTDXApp->GetHWND(), WorldToolFrustumDlgProc);
					ShowWindow(g_hFrustumDlg, SW_SHOW);
				}
				else
				{
					ShowWindow(g_hFrustumDlg, SW_HIDE);
					DestroyWindow(g_hFrustumDlg);
					//EndDialog(g_hDlg, 1);
					g_hFrustumDlg = NULL;
				}                        
				break;
			case '_':
			case '-':
				{
					GetGameOption()->SetSoundVolume( GetGameOption()->GetSoundVolume()-1000.f );							
				} break;
 
			case '+':
			case '=':
				{
					GetGameOption()->SetSoundVolume( GetGameOption()->GetSoundVolume()+1000.f );
				} break;
			}
		} break;
	} 

	switch( m_iToolMode )
	{
	case MODE_WORLDTOOL:
		{
			for( unsigned i=0; i<m_vecpDialog.size(); i++ )
			{
				if( false == m_vecpDialog[i]->GetEnable() )
					continue;

				bNoFurtherProcessing = m_vecpDialog[i]->MsgProc( hWnd, uMsg, wParam, lParam );
				if( bNoFurtherProcessing == true )
					return true;
			}

            if(g_pKTDXApp->GetDGManager()->GetPerspective() == true || m_pGame->m_bOrthoFreeCamera == true)
            {
                m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);
            }			    
            else
            {
                CWorldToolDialog* pDialog = GetDialog( CX2WorldToolMain::DLG_MAIN );                
                CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORTHOSCALE );
                WCHAR wszTextOut[512] = L"";
                float fOrthoScale = m_pGame->m_fOrthoScale;                

                switch(uMsg)
                {
                case WM_KEYDOWN:
                    {
                        switch(wParam)
                        {
                        case VK_UP:
                             
                            fOrthoScale -= 0.01f;
                            m_pGame->m_fOrthoScale = fOrthoScale;
                            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fOrthoScale );
                            pEditBox->SetText( wszTextOut );

                            g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
                                g_pKTDXApp->GetDGManager()->GetFar(), false, m_pGame->m_fOrthoScale );
                            if(m_pGame->m_bOrthoFreeCamera == false)
                                m_pGame->SetFixedCamera(true);
                            else
                                m_pGame->SetFixedCamera(false);
                            break;

                        case VK_DOWN:
                            
                            fOrthoScale += 0.01f;
                            m_pGame->m_fOrthoScale = fOrthoScale;
                            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fOrthoScale );
                            pEditBox->SetText( wszTextOut );

                            g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
                                g_pKTDXApp->GetDGManager()->GetFar(), false, m_pGame->m_fOrthoScale );
                            if(m_pGame->m_bOrthoFreeCamera == false)
                                m_pGame->SetFixedCamera(true);
                            else
                                m_pGame->SetFixedCamera(false);
                            break;
                        default:
                            m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);
                        }                
                    }
                    break;
                default:
                    m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);;
                }
            }

			switch( uMsg )
			{
			case WM_KEYDOWN:
				{
					switch( wParam )
					{
					case VK_DELETE:
						{
							if( NULL == m_pGame ||
								NULL == m_pGame->GetWorld() )
							{
								break;
							}

							CX2World* pWorld = m_pGame->GetWorld();
							for( UINT i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
							{
								CWorldToolObject* pWorldToolObject = m_vecpPickedWorldToolObject.at(i);
								RemoveObjectFromWorld( pWorldToolObject );
							}

							UpdateWorldToolObjectList();
							
						} break;
					}
				} break;


			case WM_CHAR:
				{
					switch(wParam)
					{		
					case '1':
						{							
							m_vecpDialog[DLG_MAIN]->SetEnable( !m_vecpDialog[DLG_MAIN]->GetEnable() );
							UpdateDialog();
							m_vecpDialog[DLG_OBJECT_LIST]->SetEnable( !m_vecpDialog[DLG_OBJECT_LIST]->GetEnable() );							
							UpdateDialog();
							m_vecpDialog[DLG_OBJECT_PROPERTY]->SetEnable( !m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEnable() );							
							UpdateDialog();
						} break;

					case '2':
						{
							m_bRenderLineMap = !m_bRenderLineMap;
						} break;

					case 'y':
						{
							for( unsigned i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
							{
								m_vecpPickedWorldToolObject.at(i)->GetMatrix().MoveRel( D3DXVECTOR3(0,m_fObjectMoveSpeed,0) );
							}
							UpdatePickedObjectPropertyUI();
						} break;

					case 'h':
						{
							for( unsigned i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
							{
								m_vecpPickedWorldToolObject.at(i)->GetMatrix().MoveRel( D3DXVECTOR3(0,-m_fObjectMoveSpeed,0) );
							}
							UpdatePickedObjectPropertyUI();
						} break;

					case 'j':
						{
							for( unsigned i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
							{
								m_vecpPickedWorldToolObject.at(i)->GetMatrix().MoveRel( D3DXVECTOR3(-m_fObjectMoveSpeed, 0, 0) );
							}
							UpdatePickedObjectPropertyUI();
						} break;

					case 'l':
						{
							for( unsigned i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
							{
								m_vecpPickedWorldToolObject.at(i)->GetMatrix().MoveRel( D3DXVECTOR3(m_fObjectMoveSpeed,0,0) );
							}
							UpdatePickedObjectPropertyUI();
						} break;

					case 'i':
						{
							for( unsigned i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
							{
								m_vecpPickedWorldToolObject.at(i)->GetMatrix().MoveRel( D3DXVECTOR3(0, 0,m_fObjectMoveSpeed) );
							}
							UpdatePickedObjectPropertyUI();
						} break;

					case 'k':
						{
							for( unsigned i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
							{
								m_vecpPickedWorldToolObject.at(i)->GetMatrix().MoveRel( D3DXVECTOR3(0, 0, -m_fObjectMoveSpeed) );
							}
							UpdatePickedObjectPropertyUI();
						} break;
					} 
				} break;


			case WM_DROPFILES:
				{
					DropFileProc( hWnd, uMsg, wParam, lParam );
				} break;

			} // end of switch( uMsg )
		} break;

	case MODE_EFFECTTOOL:
		{
			for( unsigned i=0; i<m_vecpDialog.size(); i++ )
			{
				if( false == m_vecpDialog[i]->GetEnable() )
					continue;

				bNoFurtherProcessing = m_vecpDialog[i]->MsgProc( hWnd, uMsg, wParam, lParam );
				if( bNoFurtherProcessing == true )
					return true;
			}
			m_FPSCamera.HandleMessages(hWnd, uMsg, wParam, lParam);

			switch( uMsg )
			{
			case WM_KEYDOWN:
				{
					switch( wParam )
					{
					case VK_DELETE:
						{			
							if( m_iPickedXMeshPlayerIndex != -1 && m_iPickedXMeshPlayerIndex < (int)m_vecTimedXMeshPlayer.size() )
							{
								m_vecTimedXMeshPlayer.erase( m_vecTimedXMeshPlayer.begin() + m_iPickedXMeshPlayerIndex );

								m_vecpDialog[DLG_EFFECT_TOOL]->GetListBox(IDC_LIST_XMESH_PLAYER_LOADED)->RemoveItem(m_iPickedXMeshPlayerIndex);
								m_vecpDialog[DLG_EFFECT_TOOL]->GetListBox(IDC_LIST_XMESH_PLAYER_LOADED)->SelectItem(0);								
							}
						} break;

					case 'P':
					case 'p':
						{
							if( m_iPickedParticleIndex != -1 && m_iPickedParticleIndex < (int)m_vecTimedParticle.size() )
							{
								m_vecTimedParticle.erase( m_vecTimedParticle.begin() + m_iPickedParticleIndex );

								m_vecpDialog[DLG_EFFECT_TOOL]->GetListBox(IDC_LIST_PARTICLE_LOADED)->RemoveItem(m_iPickedParticleIndex);
								m_vecpDialog[DLG_EFFECT_TOOL]->GetListBox(IDC_LIST_PARTICLE_LOADED)->SelectItem(0);								
							}			
						}
					}
				} break;


			case WM_CHAR:
				{
					switch(wParam)
					{		
					case '1':
						{							
							m_vecpDialog[DLG_EFFECT_TOOL]->SetEnable( !m_vecpDialog[DLG_MAIN]->GetEnable() );
							UpdateDialog();
						} break;

					case '2':
						{
							m_bRenderLineMap = !m_bRenderLineMap;
						} break;
					} 
				} break;
			} // end of switch( uMsg )
		} break;

	case MODE_GAMEPLAY:
		{
			if( m_pGame != NULL )
				bNoFurtherProcessing = m_pGame->MsgProc(hWnd, uMsg, wParam, lParam);

			switch( uMsg )
			{
			case WM_CHAR:
				{
					switch(wParam)
					{
					case 'e':
					case 'E':
						{
							if( NULL != m_pXMeshPlayer && 
								m_pXMeshPlayer->GetTempletMap().size() > 0 &&
								m_vecTimedXMeshPlayer.size() > 0 )
							{
								m_vecReadyToPlayTimedXMeshPlayer = m_vecTimedXMeshPlayer;
							}


							if( NULL != m_pParticleSystem && 
								m_pParticleSystem->GetTempletSequences().size() > 0 && 
								m_vecTimedParticle.size() > 0 )
							{
								m_vecReadyToPlayTimedParticle = m_vecTimedParticle;
								//m_vecPlayingTimedParticle = m_vecTimedParticle;
							}

							m_vecReadyToPlayCameraShake = m_vecCameraShake;


							if( true == m_bUseUserUnitState && 
								-1 != m_iPickedUserUnitState &&
								true == m_bShowUserUnit )
							{
								g_pX2Game->GetMyUnit()->StateChange( m_iPickedUserUnitState, true );
							}

							if( NULL != m_pCharacterSkinAnim && true == m_bShowCharacterSkinAnim )
							{
								m_pCharacterSkinAnim->SetPos( m_pGame->GetMyUnit()->GetPos() );
								m_pCharacterSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
							}

							if( true == m_bWorldColor )
							{ 
								//g_pX2Game->GetWorld()->SetWorldColor( D3DXCOLOR( 0, 0, 0, 1 ) ); 
								//g_pX2Game->GetWorld()->FadeWorldColor( D3DXCOLOR( 0, 0, 0, 1 ), m_fWorldColorTime );
								g_pX2Game->GetWorld()->SetWorldColor( m_coFadeWorldColor ); 
								g_pX2Game->GetWorld()->FadeWorldColor( m_coFadeWorldColor, m_fWorldColorTime );
								
								//g_pX2Game->GetWorld()->FadeWorldColor( g_pX2Game->GetWorld()->GetOriginColor(), m_fWorldColorFadeTime );
								m_fNowWorldColorTime = m_fWorldColorTime;

							}

						} break;
					}
				} break;


			case WM_KEYDOWN:
				{
					switch( wParam )
					{
                    
					case 'O':
					case 'o':
						{
							if( false == g_pKTDXApp->GetDGManager()->GetPerspective() )
							{
								g_pKTDXApp->GetDGManager()->SetProjection( 1.0f, 50000.0f, !g_pKTDXApp->GetDGManager()->GetPerspective() );
								m_pGame->GetX2Camera()->SetLandHeight( m_pGame->GetX2Camera()->GetLandHeight() - 200.f );
                                m_pGame->SetFixedCamera(false);                                
							}
							else
							{
                                //m_pGame->GetWorld()->SetLookAt( D3DXVECTOR3( 0.f, 520.f, 0.f ) );
                                //m_pGame->GetWorld()->SetEye( D3DXVECTOR3( 0.f, 520.f, -100.f ) );

                                m_pGame->m_fOrthoScale = 2.0f;
                                g_pKTDXApp->GetDGManager()->SetProjection( g_pKTDXApp->GetDGManager()->GetNear(),
                                g_pKTDXApp->GetDGManager()->GetFar(), false, m_pGame->m_fOrthoScale );                                
                                m_pGame->SetFixedCamera(true);                                
							}

                            WCHAR wszTextOut[512] = L"";
                            m_pGame->m_bOrthoFreeCamera = false;
                            CWorldToolDialog* pDialog = GetDialog( CX2WorldToolMain::DLG_MAIN );                
                            CDXUTCheckBox* pCheckBox= pDialog->GetCheckBox( CX2WorldToolMain::IDC_CHECK_ORTHOFREE );
                            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2WorldToolMain::IDC_EDIT_ORTHOSCALE );
                            pCheckBox->SetChecked( m_pGame->m_bOrthoFreeCamera );                            
                            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", m_pGame->m_fOrthoScale );
                            pEditBox->SetText( wszTextOut );

						} break;

					case VK_SPACE:
						{
							m_bPaused = !m_bPaused;
						} break;
					}
				} break;

			}
		} break;
	}

	return bNoFurtherProcessing;
}


bool CX2WorldToolMain::DropFileProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	//UINT nDroppedFiles = 0;
	//nDroppedFiles = DragQueryFile( (HDROP)wParam, 0xFFFFFFFF, NULL, MAX_PATH);

	wstring wstrFilePath; 
	WCHAR wszFilePath[MAX_PATH] = L"";	
	DragQueryFile( (HDROP)wParam, 0, wszFilePath, MAX_PATH);

	wstrFilePath = wszFilePath;			
	MakeUpperCase( wstrFilePath );
	int iResult = wstrFilePath.find( L".LUA" );
	if( 0 < iResult && iResult < (int)wstrFilePath.length() )
	{
		string strFilePath;
		ConvertWCHARToChar( strFilePath, wstrFilePath.c_str() );
		std::ifstream ifs( strFilePath.c_str() );

		int iIndex;
		CHAR szData[1024];
		string strData;
		while( !ifs.eof() )
		{
			ifs >> szData;
			strData = szData;

			iIndex = strData.find( "g_pWorld" );
			if( 0 == iIndex )
			{
				// world 스크립트 파일이다
				LoadWorld( wszFilePath );
				LoadLineMapInfo();
				g_pKTDXApp->SkipFrame();
				break;
			}

			iIndex = strData.find( "g_pLineMap" );
			if( 0 == iIndex )
			{
				// 라인맵 스크립트 파일이다				
				LoadLineMap( wszFilePath );
				LoadLineMapInfo();
				g_pKTDXApp->SkipFrame();
				break;
			}
		}
		ifs.close();
		return true;
	}
	else 
	{
		int iResult = wstrFilePath.find( L".X" );
		if( 0 < iResult && iResult < (int)wstrFilePath.length() )
		{
			LoadXFileToPalette( wszFilePath );
		}
		else
		{
			iResult = wstrFilePath.find( L".Y" );
			if( 0 < iResult && iResult < (int)wstrFilePath.length() )
			{
				LoadXFileToPalette( wszFilePath );
			}
		}

		return true;
	}


	return false;
}


bool CX2WorldToolMain::DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath )
{
	if( NULL == wszFullPath )
		return false;

	WCHAR drive[_MAX_DRIVE] = L"";
	WCHAR dir[_MAX_DIR] = L"";
	WCHAR fname[_MAX_FNAME] = L"";
	WCHAR ext[_MAX_EXT] = L"";

	_wsplitpath( wszFullPath, drive, dir, fname, ext);
	if( NULL != wszFileName )
	{
		wcscat( wszFileName, fname);
		wcscat( wszFileName, ext);	
	}
	if( NULL != wszPath )
	{
		wcscat( wszPath, drive );
		wcscat( wszPath, dir );
	}

	return true;
}

void CX2WorldToolMain::ClearWorldToolObjectList()
{
	ClearPickedWorldToolObjectList();

	for( UINT i=0; i<m_vecpWorldToolObject.size(); i++ )
	{
		CWorldToolObject* pObject = m_vecpWorldToolObject[i];
		SAFE_DELETE( pObject );
	}
	m_vecpWorldToolObject.clear(); 

	if( g_hDlgLineMap != NULL )
		UpdateTirggerSkinMeshList(g_hDlgLineMap);
}

void CX2WorldToolMain::UpdateWorldToolObjectList()
{
	ClearWorldToolObjectList();	

	if( NULL != m_pGame->GetWorld() )
	{
		vector<CKTDGSkyDome*>& vecpSkyDomeList = m_pGame->GetWorld()->GetSkyDomeList();
		for( UINT i=0; i<vecpSkyDomeList.size(); i++ )
		{
			CKTDGSkyDome* pSkyDome = vecpSkyDomeList[i];
			CWorldToolObject* pWorldToolObject = new CWorldToolObject( NULL, pSkyDome );

            if(pSkyDome->GetRenderParam()->bFog)
            {
                m_bFog = true;
                m_fogColor = pSkyDome->GetRenderParam()->fogColor;
            }


			m_vecpWorldToolObject.push_back( pWorldToolObject );
		}

		const vector<CX2WorldObjectPtr>& vecpWorldObjectList = m_pGame->GetWorld()->GetWorldObjectList();
		for( UINT i=0; i<vecpWorldObjectList.size(); i++ )
		{
			CX2WorldObject* pWorldObject = vecpWorldObjectList[i].get();
			CWorldToolObject* pWorldToolObject = new CWorldToolObject( pWorldObject, NULL );

            switch( pWorldObject->GetObjectType() )
            {
            case CX2WorldObject::OT_STATIC_MESH:
                {
                    CX2WorldObjectMesh* pWorldMesh = (CX2WorldObjectMesh*) pWorldObject;
                    if(pWorldMesh->GetRenderParam()->bFog)
                    {
                        m_bFog = true;
                        m_fogColor = pWorldMesh->GetRenderParam()->fogColor;
                    }
                } break;

            case CX2WorldObject::OT_SKIN_MESH:
                {
                    CX2WorldObjectSkinMesh* pWorldMesh = (CX2WorldObjectSkinMesh*) pWorldObject;
                    if(pWorldMesh->GetRenderParam()->bFog)
                    {
                        m_bFog = true;
                        m_fogColor = pWorldMesh->GetRenderParam()->fogColor;
                    }
                } break;            
            }
            
			m_vecpWorldToolObject.push_back( pWorldToolObject );		
		}	

		vector<CX2WorldLayer*>& vecpWorldLayer = m_pGame->GetWorld()->GetWorldLayerList();
		//{{ 2009.02.27 김태완 : 월드메쉬 추가할때마다 레이어가 자꾸 중복등록되는 현상 수정 : 벡터 클리어를 안 해줬음
		m_vecWorldLayer.clear();
		//}}
		for( UINT i=0; i<vecpWorldLayer.size(); i++ )
		{
			CX2WorldLayer* pWorldLayer = vecpWorldLayer[i];

			m_vecWorldLayer.push_back( pWorldLayer );
		}
	}

	UpdateWorldToolObjectListBox();
}

void CX2WorldToolMain::UpdateWorldToolObjectListBox()
{
	CDXUTListBox* pListBox_InWorld = m_vecpDialog[DLG_OBJECT_LIST]->GetListBox( IDC_LIST_OBJECT_IN_WORLD );
	pListBox_InWorld->RemoveAllItems();
	

	for( UINT i=0; i<m_vecpWorldToolObject.size(); i++ )
	{
		CWorldToolObject* pWorldToolObject = m_vecpWorldToolObject[i];
		pListBox_InWorld->AddItem( pWorldToolObject->GetMeshFileName().c_str(), (VOID*) pWorldToolObject );		

		//wstring wstrName = L"";
		//switch( pWorldToolObject->m_pWorldObject->GetObjectType() )
		//{
		//case CX2WorldObject::OT_STATIC_MESH:
		//	{ 
		//		CX2WorldObjectMesh* pStaticMesh = (CX2WorldObjectMesh*) pWorldToolObject->m_pWorldObject;
		//		wstrName = pStaticMesh->GetXMeshName();
		//	} break;

		//case CX2WorldObject::OT_SKIN_MESH:
		//	{
		//		CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) pWorldToolObject->m_pWorldObject;
		//		wstrName = pSkinMesh->GetXSkinMeshName();
		//	} break;

		//default:
		//	wstrName = L"unknown";
		//	break;
		//}
		//pListBox_InWorld->AddItem( wstrName.c_str(), (VOID*) pWorldToolObject );		
	}

	pListBox_InWorld->SelectItem(0);

	if( m_vecpWorldToolObject.size() > 0 )
		pListBox_InWorld->SetEnabled(true);
	else
		pListBox_InWorld->SetEnabled(false);

    if(g_hDlg != NULL)
    {
        HWND listHwnd = GetDlgItem(g_hDlg, IDC_LIST2);

        int nDel = SendMessage(listHwnd, LB_DELETESTRING, 0, 0);  
        while(nDel > 0)
        {
            nDel = SendMessage(listHwnd, LB_DELETESTRING, 0, 0);  
        }

        m_vecWorldLayer.clear();
        for(int i=0; i<(int)m_vecWorldLayer.size(); ++i)
        {
            CX2WorldLayer *pWorldLayer = m_vecWorldLayer[i];
			wstring texName = pWorldLayer->GetTexName();
            SendMessage(listHwnd, LB_ADDSTRING, 0, (LPARAM)texName.c_str());            
        }
        
    }

	if( g_hDlgLineMap != NULL )
		UpdateTirggerSkinMeshList(g_hDlgLineMap);
}

void CX2WorldToolMain::PickWorldObjectInListBox( bool bClearBefore )
{
	if( true == bClearBefore )
		ClearPickedWorldToolObjectList();

	CDXUTListBox* pListBox_InWorld = m_vecpDialog[DLG_OBJECT_LIST]->GetListBox( IDC_LIST_OBJECT_IN_WORLD );
	if( 0 == pListBox_InWorld->GetSize() )
		return;

	int nSelected = -1;
	nSelected = pListBox_InWorld->GetSelectedIndex( nSelected );
	//if( -1 != nSelected )
	//{
	//	DXUTListBoxItem *pItem = pListBox_InWorld->GetItem( nSelected );
	//	if( NULL != pItem )
	//		m_vecpPickedWorldToolObject.push_back( (CWorldToolObject*)pItem->pData );
	//}

	while( -1 != nSelected )
	{
		DXUTListBoxItem *pItem = pListBox_InWorld->GetItem( nSelected );
		if( NULL == pItem )
			break;
		m_vecpPickedWorldToolObject.push_back( (CWorldToolObject*)pItem->pData );
		nSelected = pListBox_InWorld->GetSelectedIndex( nSelected );		
	}	

	UpdatePickedObjectPropertyUI();
}

void CX2WorldToolMain::InitFog()
{    
    WCHAR wszTextOut[512] = L"";

    CDXUTCheckBox* pCheckBox_FogWorld	    = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_FOGWORLD );
    CDXUTCheckBox* pCheckBox_FogObject	    = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_FOG );
    CDXUTEditBox* pEditBox_FogNearX         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_NEAR_X );
    CDXUTEditBox* pEditBox_FogFarX          = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_FAR_X );
    CDXUTEditBox* pEditBox_FogNearY         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_NEAR_Y );
    CDXUTEditBox* pEditBox_FogFarY          = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_FAR_Y );
    CDXUTEditBox* pEditBox_FogDensity       = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_DENSITY );
    CDXUTEditBox* pEditBox_FogDensityObject = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_DENSITY_OBJECT );

    CDXUTEditBox* pEditBox_FogColor         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_ORIGIN_FOG_COLOR );
    CDXUTSlider*  pSlider_FogColor_R        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED );
    CDXUTSlider*  pSlider_FogColor_G        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN );
    CDXUTSlider*  pSlider_FogColor_B        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE );

    CDXUTSlider*  pSlider_NearX             = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_NEAR_X );
    CDXUTSlider*  pSlider_NearY             = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_NEAR_Y );
    CDXUTSlider*  pSlider_FarX              = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_FAR_X );
    CDXUTSlider*  pSlider_FarY              = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_FAR_Y );
    CDXUTSlider*  pSlider_Density           = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_DENSITY );
    CDXUTSlider*  pSlider_DensityObject     = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_DENSITY_OBJECT );

    // fog
    //float fPos;
    pCheckBox_FogWorld->SetChecked( false );
    pCheckBox_FogObject->SetChecked( false );    

    
    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", 0.0f );	
    pEditBox_FogNearX->SetText( wszTextOut );
    pSlider_NearX->SetValue(0);

    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", 0.0f );	
    pEditBox_FogFarX->SetText( wszTextOut );
    pSlider_FarX->SetValue(0);

    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", 0.0f);	
    pEditBox_FogNearY->SetText( wszTextOut );
    pSlider_NearY->SetValue(0);

    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", 0.0f);	
    pEditBox_FogFarY->SetText( wszTextOut );
    pSlider_FarY->SetValue(0);

    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", 0.0f );	
    pEditBox_FogDensity->SetText( wszTextOut );  
    pSlider_Density->SetValue(0);

    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", 0.0f);
    pEditBox_FogDensityObject->SetText( wszTextOut );  
    pSlider_DensityObject->SetValue(0);


    int iOriginLightColor[3] = { 0, };
    iOriginLightColor[0] = (int) 255;
    iOriginLightColor[1] = (int) 255;
    iOriginLightColor[2] = (int) 255;

    SetTextIntArrayToEditBox( pEditBox_FogColor,  (int*)&iOriginLightColor, 3 );
    SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);

    pSlider_FogColor_R->SetValue(iOriginLightColor[0]);
    pSlider_FogColor_G->SetValue(iOriginLightColor[1]);
    pSlider_FogColor_B->SetValue(iOriginLightColor[2]);
}

void CX2WorldToolMain::UpdatePickedObjectPropertyUI()
{
	if( m_vecpPickedWorldToolObject.size() == 0 )
		return;


	CDXUTEditBox* pEditBox_X = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_POS_X );
	CDXUTEditBox* pEditBox_Y = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_POS_Y );
	CDXUTEditBox* pEditBox_Z = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_POS_Z );

	CDXUTEditBox* pEditBox_ScaleX = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_SCALE_X );
	CDXUTEditBox* pEditBox_ScaleY = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_SCALE_Y );
	CDXUTEditBox* pEditBox_ScaleZ = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_SCALE_Z );
	CDXUTEditBox* pEditBox_RotationDegree = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_ROTATION_DEGREE );


	CDXUTEditBox* pEditBox_EventRate = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_OCCATIONAL_EVENT_RATE );
	CDXUTEditBox* pEditBox_EventTime = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_OCCATIONAL_EVENT_TIME );
	

	CDXUTEditBox* pEditBox_SkyRotSpeed = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_ROTATION_SPEED );
	CDXUTEditBox* pEditBox_Tex0UVSpeed = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_TEX0_UV_SPEED );
	CDXUTEditBox* pEditBox_AniName = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_SKIN_ANIM_NAME );
	
	// 09.03.02 김태완
	CDXUTEditBox* pEditBox_HideDistance = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_HIDE_DISTANCE );
    
	CDXUTCheckBox* pCheckBox_AlphaObject	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_ALPHA_OBJECT );
	CDXUTCheckBox* pCheckBox_AdditiveBlend 	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_ADDITIVE_BLEND );
	CDXUTCheckBox* pCheckBox_CanHide		= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_CAN_HIDE );
	CDXUTCheckBox* pCheckBox_NotRender		= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_NOT_RENDER );
	CDXUTCheckBox* pCheckBox_UseWorldColor	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_USE_WORLD_COLOR );
	//CDXUTCheckBox* pCheckBox_SkinMesh		= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_SKIN_MESH );
	//CDXUTCheckBox* pCheckBox_SKyDome		= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_SKYDOME );

#ifdef ADD_ALPHATESTENABLE	
	CDXUTCheckBox* pCheckBox_CullMode	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_NOT_CULL );
	CDXUTCheckBox* pCheckBox_AlphaTest	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_ALPHATEST_ENABLE );
#endif

#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
	// 09.04.25 김태완
	CDXUTSlider* pSlider_ObjectLightColorRed	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_OBJECT_LIGHT_COLOR_RED );
	CDXUTSlider* pSlider_ObjectLightColorGreen = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_OBJECT_LIGHT_COLOR_GREEN );
	CDXUTSlider* pSlider_ObjectLightColorBlue	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_OBJECT_LIGHT_COLOR_BLUE );
	CDXUTEditBox* pEditBox_ObjectLightColor = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_OBJECT_LIGHT_COLOR );
#endif
	// 09.03.02 김태완
	CDXUTCheckBox* pCheckBox_ForceShow	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_FORCE_SHOW );
	


	CDXUTComboBox* pComboBox_SkinAniPlayType = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetComboBox( IDC_COMBO_SKIN_ANIM_PLAY_TYPE );
	CDXUTComboBox* pComboBox_Billboard	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetComboBox( IDC_COMBO_BILLBOARD );
	CDXUTComboBox* pComboBox_Layer		= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetComboBox( IDC_COMBO_LAYER );
	CDXUTComboBox* pComboBox_ObjectType	= m_vecpDialog[DLG_OBJECT_PROPERTY]->GetComboBox( IDC_COMBO_OBJECT_TYPE );


    CDXUTCheckBox* pCheckBox_FogWorld	    = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_FOGWORLD );
    CDXUTCheckBox* pCheckBox_FogObject	    = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_FOG );
    CDXUTEditBox* pEditBox_FogNearX         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_NEAR_X );
    CDXUTEditBox* pEditBox_FogFarX          = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_FAR_X );
    CDXUTEditBox* pEditBox_FogNearY         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_NEAR_Y );
    CDXUTEditBox* pEditBox_FogFarY          = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_FAR_Y );
    CDXUTEditBox* pEditBox_FogDensity       = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_DENSITY );
    CDXUTEditBox* pEditBox_FogDensityObject = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_DENSITY_OBJECT );

    CDXUTEditBox* pEditBox_FogColor         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_ORIGIN_FOG_COLOR );
    CDXUTSlider*  pSlider_FogColor_R        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED );
    CDXUTSlider*  pSlider_FogColor_G        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN );
    CDXUTSlider*  pSlider_FogColor_B        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE );
    
    CDXUTSlider*  pSlider_NearX             = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_NEAR_X );
    CDXUTSlider*  pSlider_NearY             = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_NEAR_Y );
    CDXUTSlider*  pSlider_FarX              = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_FAR_X );
    CDXUTSlider*  pSlider_FarY              = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_FAR_Y );
    CDXUTSlider*  pSlider_Density           = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_DENSITY );
    CDXUTSlider*  pSlider_DensityObject     = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_FOG_DENSITY_OBJECT );


	CWorldToolObject* pWorldToolObject = m_vecpPickedWorldToolObject[0];	
	WCHAR wszTextOut[512] = L"";


	
	D3DXVECTOR3 vPos = pWorldToolObject->GetPosition();
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.x );	
	pEditBox_X->SetText( wszTextOut );
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.y );	
	pEditBox_Y->SetText( wszTextOut );
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.z );	
	pEditBox_Z->SetText( wszTextOut );


	D3DXVECTOR3 vScale = pWorldToolObject->GetMatrix().GetScale();
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vScale.x );	
	pEditBox_ScaleX->SetText( wszTextOut );
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vScale.y );	
	pEditBox_ScaleY->SetText( wszTextOut );
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vScale.z );	
	pEditBox_ScaleZ->SetText( wszTextOut );

	//{{ 09.03.02 김태완
	float fHideDistance = pWorldToolObject->GetHideDistance();
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fHideDistance );	
	pEditBox_HideDistance->SetText( wszTextOut );
	//}}

	


	//pCheckBox_AlphaObject->SetChecked( pWorldToolObject->m_bIsAlphaObject );
    pCheckBox_FogObject->SetChecked( pWorldToolObject->GetIsFogObject() );
	pCheckBox_AlphaObject->SetChecked( pWorldToolObject->GetIsAlphaObject() );
	pCheckBox_AdditiveBlend->SetChecked( pWorldToolObject->GetAdditiveBlend() );
	pCheckBox_CanHide->SetChecked( pWorldToolObject->GetCanHide() );
	pCheckBox_UseWorldColor->SetChecked( pWorldToolObject->GetUseWorldColor() );
#ifdef ADD_ALPHATESTENABLE
	pCheckBox_AlphaTest->SetChecked( pWorldToolObject->GetIsAlphaTestObject() );
	if( pWorldToolObject->GetRenderParam()->cullMode == D3DCULL_NONE )
		pCheckBox_CullMode->SetChecked( true );
	else
		pCheckBox_CullMode->SetChecked( false );
#endif

	// 09.04.25 김태완
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
	D3DXCOLOR colorfactor = pWorldToolObject->GetOriginalColorFactor();
	int colorR, colorG, colorB;
	colorR = (int)(colorfactor.r * 255);
	colorG = (int)(colorfactor.g * 255);
	colorB = (int)(colorfactor.b * 255);
	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%d %d %d", colorR, colorG, colorB );	
	pEditBox_ObjectLightColor->SetText( wszTextOut );

	pSlider_ObjectLightColorRed->SetValue( colorR );
	pSlider_ObjectLightColorGreen->SetValue( colorG );
	pSlider_ObjectLightColorBlue->SetValue( colorB );
#endif
	//{{ 09.03.02 김태완
	pCheckBox_ForceShow->SetChecked( pWorldToolObject->GetForceShow() );
	//}}

	//pCheckBox_SkinMesh->SetChecked( pWorldToolObject->m_bIsSkinMesh );
	//pCheckBox_SKyDome->SetChecked( pWorldToolObject->m_bIsSkyDome );

	pComboBox_Layer->SetSelectedByData( (void*) pWorldToolObject->GetLayer() );
	pComboBox_ObjectType->SetSelectedByData( (void*) pWorldToolObject->GetObjectType() );	

    // fog
    float fPos;
    pCheckBox_FogWorld->SetChecked( m_bFog );
    pCheckBox_FogObject->SetChecked( pWorldToolObject->GetIsFogObject() );    

    fPos = pWorldToolObject->GetFogNearX();
    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
    pEditBox_FogNearX->SetText( wszTextOut );
    pSlider_NearX->SetValue(static_cast<int>(fPos));

    fPos = pWorldToolObject->GetFogFarX();
    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
    pEditBox_FogFarX->SetText( wszTextOut );
    pSlider_FarX->SetValue(static_cast<int>(fPos));

    fPos = pWorldToolObject->GetFogNearY();
    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos);	
    pEditBox_FogNearY->SetText( wszTextOut );
    pSlider_NearY->SetValue(static_cast<int>(fPos));

    fPos = pWorldToolObject->GetFogFarY();
    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos);	
    pEditBox_FogFarY->SetText( wszTextOut );
    pSlider_FarY->SetValue(static_cast<int>(fPos));

    fPos = pWorldToolObject->GetFogDensity();
    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
    pEditBox_FogDensity->SetText( wszTextOut );  
    pSlider_Density->SetValue(static_cast<int>(fPos * 100));

    fPos = pWorldToolObject->GetFogDensity();
    StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos);
    pEditBox_FogDensityObject->SetText( wszTextOut );  
    pSlider_DensityObject->SetValue(static_cast<int>(fPos * 100));


    int iOriginLightColor[3] = { 0, };
    //DWORD fc = GetFogColor();
    D3DXCOLOR coOriginLightColor = GetFogColor();
    iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
    iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
    iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );
    
    SetTextIntArrayToEditBox( pEditBox_FogColor,  (int*)&iOriginLightColor, 3 );
    SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
    
    pSlider_FogColor_R->SetValue(iOriginLightColor[0]);
    pSlider_FogColor_G->SetValue(iOriginLightColor[1]);
    pSlider_FogColor_B->SetValue(iOriginLightColor[2]);


	if( NULL != pWorldToolObject->m_pWorldObject )
	{
		pEditBox_SkyRotSpeed->SetEnabled( false );


		D3DXVECTOR3 vRotationDegree = pWorldToolObject->m_pWorldObject->GetMatrix().GetRotateDegree();
		pEditBox_RotationDegree->SetTextFloatArray( (float*)&vRotationDegree, 3 );

		switch( pWorldToolObject->m_pWorldObject->GetObjectType() )
		{
		case CX2WorldObject::OT_STATIC_MESH:
			{
				pComboBox_SkinAniPlayType->SetEnabled( false );
				pEditBox_EventRate->SetEnabled( false );
				pEditBox_EventTime->SetEnabled( false );
				pEditBox_AniName->SetEnabled( false );

				CX2WorldObjectMesh* pStaticMesh = (CX2WorldObjectMesh*) pWorldToolObject->m_pWorldObject;
				pComboBox_Billboard->SetSelectedByIndex( (int) pStaticMesh->GetBillboardType() );			

				D3DXVECTOR2 vTex0UVSpeed = pStaticMesh->GetTex0UVSpeed();
				pEditBox_Tex0UVSpeed->SetTextFloatArray( (float*)&vTex0UVSpeed, 2 );


			} break;

		case CX2WorldObject::OT_SKIN_MESH:
			{
				
				pComboBox_SkinAniPlayType->SetEnabled( true );
				pEditBox_EventRate->SetEnabled( true );
				pEditBox_EventTime->SetEnabled( true );
				pEditBox_AniName->SetEnabled( true );
		

				CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*) pWorldToolObject->m_pWorldObject;
				pComboBox_Billboard->SetSelectedByIndex( (int) pSkinMesh->GetBillboardType() );			

				D3DXVECTOR2 vTex0UVSpeed = pSkinMesh->GetTex0UVSpeed();
				pEditBox_Tex0UVSpeed->SetTextFloatArray( (float*)&vTex0UVSpeed, 2 );

				vector<wstring>& wstrPlayList = pSkinMesh->GetPlayList();
				if( wstrPlayList.size() > 0 )
				{
					pEditBox_AniName->SetText( wstrPlayList[0].c_str() );
				}

				pComboBox_SkinAniPlayType->SetSelectedByData( (void*) pSkinMesh->GetPlayType() );
				

				bool bUseTwoNumbersForRandom = false;
				if( true == bUseTwoNumbersForRandom )
				{
					pEditBox_EventTime->SetTextFloatArray( (float*) &(pWorldToolObject->GetOccasionalEventTime()), 2 );

					int iNumbers[2];
					iNumbers[0] = (int) pWorldToolObject->GetOccasionalPlayRate().x;
					iNumbers[1] = (int) pWorldToolObject->GetOccasionalPlayRate().x;
					SetTextIntArrayToEditBox( pEditBox_EventRate, (int*) &iNumbers[0], 2 );
				}
				else
				{
					pEditBox_EventTime->SetTextFloatArray( (float*) &(pWorldToolObject->GetOccasionalEventTime()), 1 );

					int iNumber = (int) pWorldToolObject->GetOccasionalPlayRate().x;
					SetTextIntArrayToEditBox( pEditBox_EventRate, (int*) &iNumber, 1 );
				}

			} break;

		case CX2WorldObject::OT_PARTICLE:
			{

				pComboBox_SkinAniPlayType->SetEnabled( false );
				pEditBox_EventRate->SetEnabled( false );
				pEditBox_EventTime->SetEnabled( false );
				pEditBox_AniName->SetEnabled( false );


			} break;

		default:
			pComboBox_Billboard->SetSelectedByIndex( 0 );			
			break;
		}

	}
	else if( NULL != pWorldToolObject->m_pSkyDome )
	{
		pEditBox_SkyRotSpeed->SetEnabled( true );
		pComboBox_SkinAniPlayType->SetEnabled( false );
		pEditBox_EventRate->SetEnabled( false );
		pEditBox_EventTime->SetEnabled( false );
		pEditBox_AniName->SetEnabled( false );


		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", pWorldToolObject->m_pSkyDome->GetRotationSpeed() );	
		pEditBox_SkyRotSpeed->SetText( wszTextOut );

		D3DXVECTOR2 vTex0UVSpeed = pWorldToolObject->m_pSkyDome->GetTex0UVSpeed();
		pEditBox_Tex0UVSpeed->SetTextFloatArray( (float*)&vTex0UVSpeed, 2 );
	}

}


void CX2WorldToolMain::LoadWorld( const WCHAR* pwszFileName )
{
	CompulsorybyConvertFileAnsiToUTF8W( pwszFileName );

	WCHAR wszFilePathOnly[512] = L"";
	WCHAR wszFileNameOnly[256] = L"";
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pwszFileName );
	m_wstrWorldFilePath = wszFilePathOnly;
	m_wstrWorldFileName = wszFileNameOnly;

    if(g_hDlg)
    {
        DestroyWindow(g_hDlg);
        g_hDlg = NULL;
    }
	if( g_hDlgLineMap )
	{
		DestroyWindow( g_hDlgLineMap );
		g_hDlgLineMap = NULL;
		m_bShowLineMapModifier = false;
	}
	if( g_hFrustumDlg )
	{
		DestroyWindow( g_hFrustumDlg );
		g_hFrustumDlg = NULL;
	}

    g_pWorldToolMain->m_pGame->GetWorld()->RemoveWordLayer(-1);
    m_vecWorldLayer.clear();

	m_pGame->ReloadWorld( wszFileNameOnly );
	m_pGame->ResetUnitPosition();

    InitFog();
	UpdateWorldToolObjectList();
	PostProcLoadWorld();

	CWorldToolDialog* pDialog				= m_vecpDialog[DLG_MAIN];
	CDXUTEditBox* pEditBox_LightColor		= pDialog->GetEditBox( IDC_EDIT_WORLD_LIGHT_COLOR );
	CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( IDC_EDIT_ORIGIN_WORLD_LIGHT_COLOR );

	CDXUTSlider* pSlider_OrigingLightColorRed	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_RED );
	CDXUTSlider* pSlider_OrigingLightColorGreen = pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_GREEN );
	CDXUTSlider* pSlider_OrigingLightColorBlue	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE );

	int iOriginLightColor[3] = { 0, };
	D3DXCOLOR lightColor = m_pGame->GetWorld()->GetLightColor();
	iOriginLightColor[0] = (int) ( lightColor.r * 255.f );
	iOriginLightColor[1] = (int) ( lightColor.g * 255.f );
	iOriginLightColor[2] = (int) ( lightColor.b * 255.f );
	SetTextIntArrayToEditBox( pEditBox_LightColor,  (int*)&iOriginLightColor, 3 );


	D3DXCOLOR coOriginLightColor = m_pGame->GetWorld()->GetOriginColor();	
	iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
	iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
	iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );
	SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

	pSlider_OrigingLightColorRed->SetValue( iOriginLightColor[0] );
	pSlider_OrigingLightColorGreen->SetValue( iOriginLightColor[1] );
	pSlider_OrigingLightColorBlue->SetValue( iOriginLightColor[2] );



    // fog
    CDXUTCheckBox* pCheckBox_FogWorld	    = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_FOGWORLD );
    CDXUTCheckBox* pCheckBox_FogObject	    = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetCheckBox( IDC_CHECK_IS_FOG );
    CDXUTEditBox* pEditBox_FogNearX         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_NEAR_X );
    CDXUTEditBox* pEditBox_FogFarX          = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_FAR_X );
    CDXUTEditBox* pEditBox_FogNearY         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_NEAR_Y );
    CDXUTEditBox* pEditBox_FogFarY          = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_FAR_Y );
    CDXUTEditBox* pEditBox_FogDensity       = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_DENSITY );
    CDXUTEditBox* pEditBox_FogDensityObject = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_FOG_DENSITY_OBJECT );

    CDXUTEditBox* pEditBox_FogColor         = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetEditBox( IDC_EDIT_ORIGIN_FOG_COLOR );
    CDXUTSlider*  pSlider_FogColor_R        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED );
    CDXUTSlider*  pSlider_FogColor_G        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN );
    CDXUTSlider*  pSlider_FogColor_B        = m_vecpDialog[DLG_OBJECT_PROPERTY]->GetSlider( IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE );

#if 0 
    CDXUTSlider* pSlider_OrigingLightColorBlue	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE );
    CDXUTSlider* pSlider_OrigingLightColorBlue	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE );
    CDXUTSlider* pSlider_OrigingLightColorBlue	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE );
    CDXUTSlider* pSlider_OrigingLightColorBlue	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE );
    CDXUTSlider* pSlider_OrigingLightColorBlue	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE );
    CDXUTSlider* pSlider_OrigingLightColorBlue	= pDialog->GetSlider( IDC_SLIDER_ORIGIN_WORLD_LIGHT_COLOR_BLUE );
#endif

    float fPos;
    WCHAR wszTextOut[512] = L"";
    CWorldToolObject* pWorldToolObject = NULL;

    m_bFog = false;
    for(int i=0; i<(int)m_vecpWorldToolObject.size(); ++i)
    {
        pWorldToolObject = m_vecpWorldToolObject[i];
        if(pWorldToolObject->GetIsFogObject() == true)
        {
            m_bFog = true;
            break;
        }
    }      

	//{{ 09.02.27 김태완 : 불러온 월드에 오브젝트가 하나도 없는 경우 나는 Crash 해결
	if(pWorldToolObject != NULL )
	{
		pCheckBox_FogWorld->SetChecked( m_bFog );
		pCheckBox_FogObject->SetChecked( m_bFog );    
		fPos = pWorldToolObject->GetFogNearX();
		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
		pEditBox_FogNearX->SetText( wszTextOut );
		fPos = pWorldToolObject->GetFogFarX();
		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
		pEditBox_FogFarX->SetText( wszTextOut );
		fPos = pWorldToolObject->GetFogNearY();
		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
		pEditBox_FogNearY->SetText( wszTextOut );
		fPos = pWorldToolObject->GetFogFarY();
		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
		pEditBox_FogFarY->SetText( wszTextOut );
		fPos = pWorldToolObject->GetFogDensity();
		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos );	
		pEditBox_FogDensity->SetText( wszTextOut );   
		fPos = pWorldToolObject->GetFogDensity();
		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fPos);
		pEditBox_FogDensityObject->SetText( wszTextOut );   
		/*
		D3DXCOLOR fColor = pWorldToolObject->GetFogColor();
		StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fColor );	
		pEditBox_FogColor->SetText( wszTextOut );
		*/

	}

	//}}

        
    int iOriginFogColor[3] = { 0, };
    //DWORD fc = GetFogColor();
    D3DXCOLOR coOriginFogColor = GetFogColor();
    iOriginFogColor[0] = (int) ( coOriginFogColor.r * 255.f );
    iOriginFogColor[1] = (int) ( coOriginFogColor.g * 255.f );
    iOriginFogColor[2] = (int) ( coOriginFogColor.b * 255.f );

    SetTextIntArrayToEditBox( pEditBox_FogColor,  (int*)&iOriginFogColor, 3 );
    SetFogColor(iOriginFogColor[0]/255.0f, iOriginFogColor[1]/255.0f, iOriginFogColor[2]/255.0f);

    pSlider_FogColor_R->SetValue(iOriginFogColor[0]);
    pSlider_FogColor_G->SetValue(iOriginFogColor[1]);
    pSlider_FogColor_B->SetValue(iOriginFogColor[2]);


	if( NULL != m_pGame && NULL != m_pGame->GetWorld() )
	{
		float fLandHeight = m_pGame->GetWorld()->GetLineMap()->GetLandHeight();
		m_pGame->GetX2Camera()->SetLandHeight( fLandHeight );
	}

}


void CX2WorldToolMain::LoadLineMap( const WCHAR* pwszFileName )
{
	CompulsorybyConvertFileAnsiToUTF8W( pwszFileName );

	WCHAR wszFilePathOnly[512] = {NULL};
	WCHAR wszFileNameOnly[256] = {NULL};
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pwszFileName );
	m_wstrLineMapFilePath = wszFilePathOnly;
	m_wstrLineMapFileName = wszFileNameOnly;	

	string strFileName;
	ConvertWCHARToChar( strFileName, wszFileNameOnly );
	m_pGame->GetWorld()->SetLineMap_LUA( strFileName.c_str() );
	m_pGame->ResetUnitPosition();

	if( NULL != m_pGame && NULL != m_pGame->GetWorld() )
	{
		float fLandHeight = m_pGame->GetWorld()->GetLineMap()->GetLandHeight();
		m_pGame->GetX2Camera()->SetLandHeight( fLandHeight );
	}
}



HRESULT CX2WorldToolMain::SaveWorld( const WCHAR* pwszFileFullPath )
{
	HRESULT hr = S_OK;

	if( NULL == m_pGame || NULL == m_pGame->GetWorld() )
		return E_FAIL;



	std::map< int, const char* > mapLayerString;
	mapLayerString[ XL_SKY_DOME_0 ]								= "X2_LAYER[\"XL_SKY_DOME_0\"]";
	mapLayerString[ XL_SKY_DOME_1 ]								= "X2_LAYER[\"XL_SKY_DOME_1\"]";
	mapLayerString[ XL_SKY_DOME_2 ]								= "X2_LAYER[\"XL_SKY_DOME_2\"]";
	mapLayerString[ XL_SKY_WORLD_BASE_0 ]						= "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]";
	mapLayerString[ XL_SKY_WORLD_BASE_1 ]						= "X2_LAYER[\"XL_SKY_WORLD_BASE_1\"]";
	mapLayerString[ XL_SKY_WORLD_BASE_2 ]						= "X2_LAYER[\"XL_SKY_WORLD_BASE_2\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_0 ]						= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_0\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_1 ]						= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_1\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_2 ]						= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_2\"]";
	mapLayerString[ XL_UNIT_0	]								= "X2_LAYER[\"XL_UNIT_0\"]";
	mapLayerString[ XL_UNIT_1	]								= "X2_LAYER[\"XL_UNIT_1\"]";
	mapLayerString[ XL_UNIT_2	]								= "X2_LAYER[\"XL_UNIT_2\"]";

	mapLayerString[ XL_EFFECT_0 ]								= "X2_LAYER[\"XL_EFFECT_0\"]";
	mapLayerString[ XL_EFFECT_1 ]								= "X2_LAYER[\"XL_EFFECT_1\"]";
	mapLayerString[ XL_EFFECT_2 ]								= "X2_LAYER[\"XL_EFFECT_2\"]";

	//{{ kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업
#ifdef	ADD_X2_LAYER
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_0 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_0\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_1 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_1\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_2 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_2\"]";
	mapLayerString[ XL_SKY_WORLD_OBJECT_OVER_EFFECT_3 ]			= "X2_LAYER[\"XL_SKY_WORLD_OBJECT_OVER_EFFECT_3\"]";
// 	mapLayerString[ XL_EFFECT_OVER_ALL_WORLD_OBJECT_0 ]			= "X2_LAYER[\"XL_EFFECT_OVER_ALL_WORLD_OBJECT_0\"]";
// 	mapLayerString[ XL_EFFECT_OVER_ALL_WORLD_OBJECT_1 ]			= "X2_LAYER[\"XL_EFFECT_OVER_ALL_WORLD_OBJECT_1\"]";
#endif	ADD_X2_LAYER
	//}} kimhc // 2010.6.17 // enum X2_LAYER에 레이어 추가하는 작업

// kimhc // XL_EFFECT_OVER_ALL_WORLD_OBJECT 와 함께 월드툴에서 사용하지 말아야할 것 주석 처리함
// 	mapLayerString[ XL_LENS_FLARE ]								= "X2_LAYER[\"XL_LENS_FLARE\"]";
// 	mapLayerString[ XL_MINIMAP	]								= "X2_LAYER[\"XL_MINIMAP\"]";



	std::map< int, const char* > mapSkinAnimPlayTypeString;
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_ONE ]					= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_ONE\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_ONE_WAIT ]			= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_ONE_WAIT\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_LOOP ]				= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_LOOP\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_PINGPONG ]			= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_PINGPONG\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE ]		= "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_OCCASIONAL_ONE\"]";
	mapSkinAnimPlayTypeString[ CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE_WAIT ] = "WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_OCCASIONAL_ONE_WAIT\"]";



	// file path와 name을 분리
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	_wsplitpath( pwszFileFullPath, drive, dir, fname, ext);

	string strFileName;
	ConvertWCHARToChar( strFileName, fname );
	strFileName += ".lua";

	std::ofstream ofs;
	ofs.open( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return E_FAIL;
	}

	string strWorldName;
	ConvertWCHARToChar( strWorldName, fname );
	ofs << "-- " << strFileName << std::endl;
	ofs << std::endl;
	ofs << std::endl;
	ofs << std::endl;


	D3DXVECTOR3 vLightPos = m_pGame->GetWorld()->GetLightPos();
	ofs << "g_pWorld:SetLightPos( " << vLightPos.x << ", " << vLightPos.y << ", " << vLightPos.z << " )" << std::endl;

	if( D3DXCOLOR(1,1,1,1) != m_pGame->GetWorld()->GetLightColor() )
	{
		D3DXCOLOR& lightColor = m_pGame->GetWorld()->GetLightColor();
		ofs << "g_pWorld:SetLightColor_LUA(  " << (float)lightColor.r << ", " 
			<< (float)lightColor.g << ", " << (float)lightColor.b << " ) " << std::endl;
	}

	
	if( D3DXCOLOR(1,1,1,1) != m_pGame->GetWorld()->GetOriginColor() )
	{
		D3DXCOLOR& coOriginLightColor = m_pGame->GetWorld()->GetOriginColor();
		ofs << "g_pWorld:SetOriginColor_LUA(  " << (float)coOriginLightColor.r << ", " 
			<< (float)coOriginLightColor.g << ", " << (float)coOriginLightColor.b << " ) " << std::endl;
	}


	switch( m_eLineMapFileNameType )
	{
	// 2009.02.25 김태완 : 디폴트를 SUFFIX로 변경
	default:
	case LFT_SUFFIX:
		{
			ofs << "g_pWorld:SetLineMap( \"" << strWorldName << "_LineMap.lua\" )" << std::endl;
		}
		break;

//	default:
	case LFT_PREFIX:
		{
			// 승수씨 파일 이름 형식
			string strLineMapName = strWorldName;
			MakeUpperCase( strLineMapName );	
			string LINEMAP_ = "LINEMAP_";

			if( 0 == strLineMapName.find( "WORLD_" ) )
			{
				strLineMapName = strWorldName;
				strLineMapName.replace( 0, 6, LINEMAP_ );
			}
			else
			{
				strLineMapName = strWorldName;
				strLineMapName.insert( 0, LINEMAP_ );
			}
			ofs << "g_pWorld:SetLineMap( \"" << strLineMapName << ".lua\" )" << std::endl;
		} break;
	}


	
	ofs << "g_pWorld:SetBGM( \"" << strWorldName << ".ogg\" )" << std::endl;
	ofs << std::endl;
	ofs << std::endl;
	ofs << std::endl;

    // Start of Fog
    bool bDensity, bFog = false;
    float fNearX, fNearY, fFarX, fFarY, fDensity;
    D3DXCOLOR fogColor;
    std::vector<float>  vecFogDensity;
    std::vector<int>    vecFogDensityCount;


    for( UINT i=0; i<m_vecpWorldToolObject.size() ; i++ )
    {
        CWorldToolObject* pObject = m_vecpWorldToolObject[i];

        if( bFog == false && pObject->GetIsFogObject())
        {
            bFog = true;
            fNearX  = pObject->GetFogNearX();
            fNearY  = pObject->GetFogNearY();
            fFarX   = pObject->GetFogFarX();
            fFarY   = pObject->GetFogFarY();
            fogColor = GetFogColor();
            
        }

        if(bFog)
        {
            fDensity = pObject->GetFogDensity();
               
            bDensity = false;
            for(int i=0; i<(int)vecFogDensity.size(); ++i)
            {
                if(vecFogDensity[i] == fDensity)
                {
                    bDensity = true;
                    vecFogDensityCount[i] += 1;
                    break;
                }                
            }
            if(bDensity != true)
            {
                vecFogDensity.push_back(fDensity);
                vecFogDensityCount.push_back(1);
            }
        }
    }
        
    int maxCount = 0;
    for(int i=0; i<(int)vecFogDensity.size(); ++i)
    {
        if(vecFogDensityCount[i] >= maxCount)
        {
            maxCount = vecFogDensityCount[i];
            fDensity = vecFogDensity[i];
        }
    }

    if(maxCount > 0)
        ofs << "g_pWorld:SetFogWorld( " << fNearX << ", " << fFarX << ", " << fNearY << ", " << fFarY << ", " << fDensity << ", " 
        << "D3DXCOLOR( " << fogColor.r << ", " << fogColor.g << ", " << fogColor.b << ", 1.0 ) )" << std::endl;
    // End of Fog

#ifdef MODIFY_FRUSTUM
	float fBase = 0.f;
	float fCamera1 = 0.f;
	float fCamera2 = 0.f;
	float fCamera3 = 0.f;
	if( g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL )
	{
		fBase = g_pX2Game->GetWorld()->GetBaseCameraDistance();
		fCamera1 = g_pX2Game->GetWorld()->GetCameraStepDistacne(0);
		fCamera2 = g_pX2Game->GetWorld()->GetCameraStepDistacne(1);
		fCamera3 = g_pX2Game->GetWorld()->GetCameraStepDistacne(2);
	}

	ofs << "g_pWorld:SetProjection( " << g_pKTDXApp->GetDGManager()->GetNear() << ", " << g_pKTDXApp->GetDGManager()->GetFar() << ", " 				
		<< g_pKTDXApp->GetDGManager()->GetPerspectiveValue() << ", "
		<< fBase << ", " << fCamera1 << ", " << fCamera2 << ", " << fCamera3 << " )" << std::endl << std::endl;	
#endif

    // World Layer    
    for(int i=0; i<(int)m_vecWorldLayer.size(); ++i)
    {
        CX2WorldLayer *pLayer = m_vecWorldLayer[i];
        if(pLayer != NULL)
        {
            float x, y, w, h;
            D3DXVECTOR2 uv, feedbackUV;
            D3DXCOLOR color;            
            pLayer->GetPos(&x, &y, &w, &h);
            uv = pLayer->GetTexUV();
            feedbackUV = pLayer->GetFeedBackUV();
            color = pLayer->GetColor();

            ofs << "g_pWorldLayer = g_pWorld:CreateWorldLayer()" << std::endl;
            ofs << "g_pWorldLayer:SetLayer(" << mapLayerString[ pLayer->GetLayer() ] << ")" << std::endl;
            ofs << "g_pWorldLayer:SetBlendType(" << pLayer->GetBlendType() << ")" << std::endl;

			string strWorldLayerTexName = "";
			ConvertWCHARToChar( strWorldLayerTexName, pLayer->GetTexName().c_str() );
            ofs << "g_pWorldLayer:SetTex( \"" << strWorldLayerTexName << "\" )" << std::endl;
            
			ofs << "g_pWorldLayer:SetAlphaObject(true)" << std::endl;
            ofs << "g_pWorldLayer:SetPos(" << x << ", " << y << ", " << w << ", " << h << ")" << std::endl;
            ofs << "g_pWorldLayer:SetColor(D3DXCOLOR(" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << "))" << std::endl;            
            ofs << "g_pWorldLayer:SetTexUV(" << uv.x << ", " << uv.y << ")" << std::endl << std::endl;
            ofs << "g_pWorldLayer:SetFeedBackUV(" << feedbackUV.x << ", " << feedbackUV.y << ")" << std::endl << std::endl;
			ofs << "g_pWorldLayer:SetLayerId(" << i << ")" << std::endl;
			if( pLayer->GetShowObject() == true )
				ofs << "g_pWorldLayer:SetInitShow( true )" << std::endl;
			else 
				ofs << "g_pWorldLayer:SetInitShow( false )" << std::endl;
        }
    }
    // End of World Layer



	// skydome 
	ofs << "-- SKY / CLOUD -- " << std::endl;
	for( UINT i=0; i<m_vecpWorldToolObject.size() ; i++ )
	{
		CWorldToolObject* pWorldToolObject = m_vecpWorldToolObject[i];
		CKTDGSkyDome* pSkyDome = pWorldToolObject->m_pSkyDome;
		if( NULL == pSkyDome )
			continue;


		D3DXVECTOR3 vPos = pSkyDome->GetMatrix().GetPos();
		D3DXVECTOR3 vScale = pSkyDome->GetMatrix().GetScale();
		string strMeshName;
		ConvertWCHARToChar( strMeshName, pWorldToolObject->GetMeshFileName().c_str() );

		
		ofs << "g_pSkyDome = g_pWorld:CreateSkyDome()" << std::endl;
		if( mapLayerString.end() != mapLayerString.find( pWorldToolObject->GetLayer() ) )
		{
			ofs << "g_pSkyDome:SetLayer( " << mapLayerString[ pWorldToolObject->GetLayer() ] << " )" << std::endl;
		}
		else
		{
			ofs << "g_pSkyDome:SetLayer( " << "X2_LAYER[\"XL_SKY_DOME_0\"]" << " )" << std::endl;
		}
			
		
		ofs << "g_pSkyDome:SetDomeMesh( \"" << strMeshName << "\" )" << std::endl;		
		ofs << "g_pSkyDome:SetPos( " << vPos.x << ", " << vPos.y << ", " << vPos.z << " )" << std::endl;
		if( vScale != D3DXVECTOR3(1,1,1) )
			ofs << "g_pSkyDome:SetScale( D3DXVECTOR3( " << vScale.x << ", " << vScale.y << ", " << vScale.z << ") )" << std::endl;
		ofs << "g_pSkyDome:SetRotationSpeed( " << pSkyDome->GetRotationSpeed() << " )" << std::endl;

		D3DXVECTOR2 v2UVSpeed = pSkyDome->GetTex0UVSpeed();
		if( v2UVSpeed.x != 0.f || v2UVSpeed.y != 0.f )
		{
			ofs << "g_pSkyDome:SetTex0UVSpeed( " << v2UVSpeed.x << ", " << v2UVSpeed.y << " )" << std::endl;
		}


		if( true == pSkyDome->GetViewTrace() )
            ofs << "g_pSkyDome:SetViewTrace( true )" << std::endl;
		else
			ofs << "g_pSkyDome:SetViewTrace( false )" << std::endl;

		if( true == pSkyDome->IsAlphaObject() )
			ofs << "g_pSkyDome:SetAlphaObject( true )" << std::endl;
		else
			ofs << "g_pSkyDome:SetAlphaObject( false )" << std::endl;

		ofs << "g_pRenderParam = g_pSkyDome:GetRenderParam()" << std::endl;

		CKTDGXRenderer::RenderParam* pRenderParam = pSkyDome->GetRenderParam();
		if( true == pRenderParam->bAlphaBlend )
		{
			ofs << "g_pRenderParam.bAlphaBlend = true" << std::endl;
		}

		if( true == pWorldToolObject->GetAdditiveBlend() )
		{
			ofs << "g_pRenderParam.srcBlend = D3DBLEND[\"D3DBLEND_SRCALPHA\"]" << std::endl;
			ofs << "g_pRenderParam.destBlend = D3DBLEND[\"D3DBLEND_DESTALPHA\"]" << std::endl;
		}

        if( pRenderParam->bFog == true)
        {
            ofs << "g_pSkyDome:SetFog( " << pRenderParam->fogNearX << ", " << pRenderParam->fogFarX << ", " 
                << pRenderParam->fogNearY << ", " << pRenderParam->fogFarY 
                << ", D3DXCOLOR(" << pRenderParam->fogColor.r << ", " << pRenderParam->fogColor.g << ", " << pRenderParam->fogColor.b << ", " << pRenderParam->fogColor.a << "), " 
                << pRenderParam->fogDensity << " )" << std::endl;
        }


		ofs << std::endl << std::endl;			
	}
	

	// static mesh
	ofs << "-- STATIC MESH -- " << std::endl;
	for( UINT i=0; i<m_vecpWorldToolObject.size() ; i++ )
	{
		CWorldToolObject* pWorldToolObject = m_vecpWorldToolObject[i];
		CX2WorldObject* pWorldObject = pWorldToolObject->m_pWorldObject;
		if( NULL == pWorldObject )
			continue;

		switch( pWorldObject->GetObjectType() )
		{
		case CX2WorldObject::OT_STATIC_MESH:
			{
				CX2WorldObjectMesh* pMesh = (CX2WorldObjectMesh*) pWorldObject;
				D3DXVECTOR3 vPos = pMesh->GetMatrix().GetPos();
				D3DXVECTOR3 vScale = pMesh->GetMatrix().GetScale();
				string strMeshName;
				ConvertWCHARToChar( strMeshName, pMesh->GetXMeshName().c_str() );

				ofs << "g_pObjectMesh = g_pWorld:CreateObjectMesh()" << std::endl;
				ofs << "g_pObjectMesh:SetXMesh(    \"" << strMeshName << "\" )" << std::endl;
				ofs << "g_pObjectMesh:SetXMeshLOD( \"" << strMeshName << "\" )" << std::endl;

				if( mapLayerString.end() != mapLayerString.find( pWorldToolObject->GetLayer() ) )
				{
					ofs << "g_pObjectMesh:SetLayer( " << mapLayerString[ pWorldToolObject->GetLayer() ] << " )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectMesh:SetLayer( " << "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]" << " )" << std::endl;
				}

				ofs << "g_pObjectMesh:SetPos( D3DXVECTOR3(" << vPos.x << ", " << vPos.y << ", " << vPos.z << ") )" << std::endl;
				if( vScale != D3DXVECTOR3(1,1,1) )
					ofs << "g_pObjectMesh:SetScale( D3DXVECTOR3( " << vScale.x << ", " << vScale.y << ", " << vScale.z << ") )" << std::endl;

				D3DXVECTOR3 vRotationDegree = pWorldToolObject->GetRotationDegree();
				if( vRotationDegree != D3DXVECTOR3(0, 0, 0 ) )
				{
					ofs << "g_pObjectMesh:SetAngleDegree( D3DXVECTOR3(" << vRotationDegree.x << ", " << vRotationDegree.y << ", " << vRotationDegree.z << ") )" << std::endl;
				}

				if( pWorldToolObject->GetTex0UVSpeed() != D3DXVECTOR2(0, 0) )
				{
					ofs << "g_pObjectMesh:SetTex0UVSpeed( " << pWorldToolObject->GetTex0UVSpeed().x << ", " << pWorldToolObject->GetTex0UVSpeed().y << " )" << std::endl;
				}
				
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
				//** Static Mesh 컬러펙터 지정
				D3DXCOLOR color = pWorldToolObject->GetOriginalColorFactor();
				ofs << "g_pObjectMesh:SetOriginalColorFactor( D3DXCOLOR(" << color.r << ", " << color.g << ", " << color.b << ", " << 1 << "))" << std::endl;            
#endif
				ofs << "g_pObjectMesh:SetColorFactor( D3DXCOLOR(1,1,1,1) )" << std::endl;
				if( true == pWorldToolObject->GetCanHide() )
				{
					ofs << "g_pObjectMesh:SetCanHide( true, true )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectMesh:SetCanHide( false, false )" << std::endl;
				}

				if( true == pWorldToolObject->GetNotRender() )
				{
					ofs << "g_pObjectMesh:SetNotRenderWhenHidden( true )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectMesh:SetNotRenderWhenHidden( false )" << std::endl;
				}
				
				ofs << "g_pRenderParam = g_pObjectMesh:GetRenderParam()" << std::endl;

				if( true == pMesh->IsAlphaObject() )
				{
					if( true == pWorldToolObject->GetCanHide() )
					{
						ofs << "g_pObjectMesh:SetAlphaObject( false )" << std::endl;					
					}
					else
					{
						ofs << "g_pObjectMesh:SetAlphaObject( true )" << std::endl;					
					}
						
				}		
				CKTDGXRenderer::RenderParam* pRenderParam = pMesh->GetRenderParam();
				if( true == pRenderParam->bAlphaBlend )		
				{
					ofs << "g_pRenderParam.bAlphaBlend = true" << std::endl;					
				}			
#ifdef ADD_ALPHATESTENABLE
				if( true == pRenderParam->bAlphaTestEnable )
				{
					ofs << "g_pRenderParam.bAlphaTestEnable = true" << std::endl;
				}
				if( pRenderParam->cullMode == D3DCULL_NONE )
				{
					ofs << "g_pRenderParam.cullMode = D3DCULL[\"D3DCULL_NONE\"]" << std::endl;
				}
#endif

				if( true == pWorldToolObject->GetAdditiveBlend() )
				{
					ofs << "g_pRenderParam.srcBlend = D3DBLEND[\"D3DBLEND_SRCALPHA\"]" << std::endl;
					ofs << "g_pRenderParam.destBlend = D3DBLEND[\"D3DBLEND_DESTALPHA\"]" << std::endl;
				}
				if( false == pWorldToolObject->GetUseWorldColor() )
				{
					ofs << "g_pObjectMesh:SetUseWorldColor( false )" << std::endl;
				}
                
                pRenderParam = pMesh->GetRenderParam();
                if( pRenderParam->bFog == true)
                {
                    ofs << "g_pObjectMesh:SetFog( " << pRenderParam->fogNearX << ", " << pRenderParam->fogFarX << ", " 
                        << pRenderParam->fogNearY << ", " << pRenderParam->fogFarY 
                        << ", D3DXCOLOR(" << pRenderParam->fogColor.r << ", " << pRenderParam->fogColor.g << ", " << pRenderParam->fogColor.b << ", " << pRenderParam->fogColor.a << "), " 
                        << pRenderParam->fogDensity << " )" << std::endl;
                }

				switch( pMesh->GetBillboardType() )
				{
				default:
				case CKTDGMatrix::BT_NONE:
					break;

				case CKTDGMatrix::BT_ALL:
					ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_ALL\"] )" << std::endl;
					break;
				case CKTDGMatrix::BT_X:
					ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_X\"] )" << std::endl;
					break;
				case CKTDGMatrix::BT_Y:
					ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Y\"] )" << std::endl;
					break;
				case CKTDGMatrix::BT_Z:
					ofs << "g_pObjectMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Z\"] )" << std::endl;
					break;
				}

				//{{ 09.03.02 김태완
				if(pWorldToolObject->GetHideDistance() > 0.0f)
				{
					ofs << "g_pObjectMesh:SetHideDistance( " << pWorldToolObject->GetHideDistance() << ")" << std::endl;
				}
				//}}
				//{{ 09.03.02 김태완
				if(true == pWorldToolObject->GetForceShow())
				{
					ofs << "g_pObjectMesh:SetForceShow( true )" << std::endl;
				}
				//}}


				ofs << std::endl << std::endl;			
			} break;
		}

		
	}	

	// skin mesh	
	ofs << "-- SKIN MESH -- " << std::endl;
	for( UINT i=0; i<m_vecpWorldToolObject.size() ; i++ )
	{
		CWorldToolObject* pWorldToolObject = m_vecpWorldToolObject[i];
		CX2WorldObject* pWorldObject = pWorldToolObject->m_pWorldObject;
		if( NULL == pWorldObject )
			continue;

		switch( pWorldObject->GetObjectType() )
		{
		case CX2WorldObject::OT_SKIN_MESH:
			{
				CX2WorldObjectSkinMesh* pSkinMesh = (CX2WorldObjectSkinMesh*)pWorldObject;
				D3DXVECTOR3 vPos = pSkinMesh->GetMatrix().GetPos();
				D3DXVECTOR3 vScale = pSkinMesh->GetMatrix().GetScale();
				string strSkinMeshName;
				string strSkinMotionName;
				ConvertWCHARToChar( strSkinMeshName, pSkinMesh->GetXSkinMeshName().c_str() );
				ConvertWCHARToChar( strSkinMotionName, pSkinMesh->GetXSkinMotionMesh()->GetDeviceID().c_str() );

				ofs << "g_pObjectSkinMesh = g_pWorld:CreateObjectSkinMesh()" << std::endl;
				ofs << "g_pObjectSkinMesh:SetXSkinMotion( \"" << strSkinMotionName << "\" )" << std::endl;
				ofs << "g_pObjectSkinMesh:AddXSkinModel( \"" << strSkinMeshName << "\" )" << std::endl;


				if( 0 != pWorldToolObject->GetAnimationName().length() )
				{
					wstring wstrAniName = pWorldToolObject->GetAnimationName();
					string strAniName;
					ConvertWCHARToChar( strAniName, wstrAniName.c_str() );

					ofs << "g_pObjectSkinMesh:AddAnim( \"" << strAniName << "\" )" << std::endl;
					ofs << "g_pObjectSkinMesh:PlayAnim( " << mapSkinAnimPlayTypeString[ (int)pWorldToolObject->GetPlayType() ] << " )" << std::endl;
				}
				else
				{
					LPD3DXANIMATIONCONTROLLER	pAC;
					LPD3DXANIMATIONSET			pAnimSet;
					pAC = pSkinMesh->GetXSkinMotionMesh()->GetCloneAC();

					if( pAC->GetNumAnimationSets() > 0 )
					{
						pAC->GetAnimationSet(0, &pAnimSet );
						ofs << "g_pObjectSkinMesh:AddAnim( \"" << pAnimSet->GetName() << "\" )" << std::endl;
						//ofs << "g_pObjectSkinMesh:PlayAnim( WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_LOOP\"] )" << std::endl;
						ofs << "g_pObjectSkinMesh:PlayAnim( " << mapSkinAnimPlayTypeString[ (int)pWorldToolObject->GetPlayType() ] << " )" << std::endl;
						SAFE_RELEASE( pAnimSet );
					}
					else
					{
						ofs << "--g_pObjectSkinMesh:AddAnim( \"\" )" << std::endl;
						ofs << "--g_pObjectSkinMesh:PlayAnim( WORLD_OBJECT_SKIN_MESH_PLAY_TYPE[\"WOSMPY_LOOP\"] )" << std::endl;
					}
					SAFE_RELEASE( pAC );
				}				

				switch( pWorldToolObject->GetPlayType() )
				{
				case CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE:
				case CX2WorldObjectSkinMesh::WOSMPY_OCCASIONAL_ONE_WAIT:
					{
						bool bUseTwoNumbersForRandom = false;
						if( true == bUseTwoNumbersForRandom )
						{
							ofs << "g_pObjectSkinMesh:SetOccasionalEventTime( RandomFloat_LUA( " << pWorldToolObject->GetOccasionalEventTime().x <<
								", " << pWorldToolObject->GetOccasionalEventTime().y << " )" << std::endl;

							ofs << "g_pObjectSkinMesh:SetOccasionalPlayRate( RandomInt_LUA( " << (int)pWorldToolObject->GetOccasionalPlayRate().x <<
								", " << (int)pWorldToolObject->GetOccasionalPlayRate().y << " )" << std::endl;
						}
						else
						{
							ofs << "g_pObjectSkinMesh:SetOccasionalEventTime( " << pWorldToolObject->GetOccasionalEventTime().x 
								<< " )" << std::endl;

							ofs << "g_pObjectSkinMesh:SetOccasionalPlayRate( " << (int)pWorldToolObject->GetOccasionalPlayRate().x 
								<< " )" << std::endl;
						}
					} break;
				default:
					break;

				}


				if( mapLayerString.end() != mapLayerString.find( pWorldToolObject->GetLayer() ) )
				{
					ofs << "g_pObjectSkinMesh:SetLayer( " << mapLayerString[ pWorldToolObject->GetLayer() ] << " )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectSkinMesh:SetLayer( " << "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]" << " )" << std::endl;
				}

				ofs << "g_pObjectSkinMesh:SetPos( D3DXVECTOR3(" << vPos.x << ", " << vPos.y << ", " << vPos.z << ") )" << std::endl;
				if( vScale != D3DXVECTOR3(1,1,1) )
					ofs << "g_pObjectSkinMesh:SetScale( D3DXVECTOR3( " << vScale.x << ", " << vScale.y << ", " << vScale.z << ") )" << std::endl;

				D3DXVECTOR3 vRotationDegree = pWorldToolObject->GetRotationDegree();
				if( vRotationDegree != D3DXVECTOR3(0, 0, 0 ) )
				{
					ofs << "g_pObjectSkinMesh:SetAngleDegree( D3DXVECTOR3(" << vRotationDegree.x << ", " << vRotationDegree.y << ", " << vRotationDegree.z << ") )" << std::endl;
				}

				if( pWorldToolObject->GetTex0UVSpeed() != D3DXVECTOR2(0, 0) )
				{
					ofs << "g_pObjectSkinMesh:SetTex0UVSpeed( " << pWorldToolObject->GetTex0UVSpeed().x << ", " << pWorldToolObject->GetTex0UVSpeed().y << " )" << std::endl;
				}
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
				D3DXCOLOR color = pWorldToolObject->GetOriginalColorFactor();
				ofs << "g_pObjectSkinMesh:SetOriginalColorFactor( D3DXCOLOR(" << color.r << ", " << color.g << ", " << color.b << ", " << 1 << "))" << std::endl;            
#endif
				ofs << "g_pObjectSkinMesh:SetColorFactor( D3DXCOLOR(1,1,1,1) )" << std::endl;
				
				if( true == pWorldToolObject->GetCanHide() )
				{
					ofs << "g_pObjectSkinMesh:SetCanHide( true, true )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectSkinMesh:SetCanHide( false, false )" << std::endl;
				}

				// 현재는 스킨메쉬에는 Not Render 옵션이 존재하지 않는다.

				ofs << "g_pRenderParam = g_pObjectSkinMesh:GetRenderParam()" << std::endl;
				
				if( true == pSkinMesh->IsAlphaObject() )
				{
					if( true == pWorldToolObject->GetCanHide() )
					{
						ofs << "g_pObjectSkinMesh:SetAlphaObject( false )" << std::endl;					
					}
					else
					{
						ofs << "g_pObjectSkinMesh:SetAlphaObject( true )" << std::endl;					
					}					
				}	
				else
				{
					ofs << "g_pObjectSkinMesh:SetAlphaObject( false )" << std::endl;
				}

				CKTDGXRenderer::RenderParam* pRenderParam = pSkinMesh->GetRenderParam();
				if( true == pRenderParam->bAlphaBlend )		
				{
					ofs << "g_pRenderParam.bAlphaBlend = true" << std::endl;					
				}
				else
				{
					ofs << "g_pRenderParam.bAlphaBlend = false" << std::endl;					
				}

				if( true == pWorldToolObject->GetAdditiveBlend() )
				{
					ofs << "g_pRenderParam.srcBlend = D3DBLEND[\"D3DBLEND_SRCALPHA\"]" << std::endl;
					ofs << "g_pRenderParam.destBlend = D3DBLEND[\"D3DBLEND_DESTALPHA\"]" << std::endl;
				}
				
				if( false == pWorldToolObject->GetUseWorldColor() )
				{
					ofs << "g_pObjectSkinMesh:SetUseWorldColor( false )" << std::endl;
				}

                pRenderParam = pSkinMesh->GetRenderParam();
                if( pRenderParam->bFog == true)
                {
                    ofs << "g_pObjectSkinMesh:SetFog( " << pRenderParam->fogNearX << ", " << pRenderParam->fogFarX << ", " 
                        << pRenderParam->fogNearY << ", " << pRenderParam->fogFarY 
                        << ", D3DXCOLOR(" << pRenderParam->fogColor.r << ", " << pRenderParam->fogColor.g << ", " << pRenderParam->fogColor.b << ", " << pRenderParam->fogColor.a << "), " 
                        << pRenderParam->fogDensity << " )" << std::endl;
                }

				switch( pSkinMesh->GetBillboardType() )
				{
				default:
				case CKTDGMatrix::BT_NONE:
					break;

				case CKTDGMatrix::BT_ALL:
					ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_ALL\"] )" << std::endl;
					break;
				case CKTDGMatrix::BT_X:
					ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_X\"] )" << std::endl;
					break;
				case CKTDGMatrix::BT_Y:
					ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Y\"] )" << std::endl;
					break;
				case CKTDGMatrix::BT_Z:
					ofs << "g_pObjectSkinMesh:SetBillboardType( MATRIX_BILLBOARD_TYPE[\"BT_Z\"] )" << std::endl;
					break;
				}

				//{{ 09.03.02 김태완
				if(pWorldToolObject->GetHideDistance() > 0.0f)
				{
					ofs << "g_pObjectSkinMesh:SetHideDistance( " << pWorldToolObject->GetHideDistance() << ")" << std::endl;
				}
				//}}
				//{{ 09.03.02 김태완
				if(true == pWorldToolObject->GetForceShow())
				{
					ofs << "g_pObjectSkinMesh:SetForceShow( " << pWorldToolObject->GetForceShow() << ")" << std::endl;
				}
				//}}

				ofs << std::endl << std::endl;			
			} break;
		}		
	}	





	// particle
	ofs << "-- PARTICLE -- " << std::endl;
	for( UINT i=0; i<m_vecpWorldToolObject.size() ; i++ )
	{
		CWorldToolObject* pWorldToolObject = m_vecpWorldToolObject[i];
		CX2WorldObject* pWorldObject = pWorldToolObject->m_pWorldObject;
		if( NULL == pWorldObject )
			continue;

		switch( pWorldObject->GetObjectType() )
		{
		case CX2WorldObject::OT_PARTICLE:
			{
				CX2WorldObjectParticle* pParticle = (CX2WorldObjectParticle*) pWorldObject;
				
				string strParticleSequenceName;
				ConvertWCHARToChar( strParticleSequenceName, pParticle->GetParticleEventSequence()->GetName() );

				D3DXVECTOR3 vPos = pParticle->GetMatrix().GetPos();
				CMinMax<float> emitRate = pParticle->GetParticleEventSequence()->GetEmitRate();
				int iTriggerCount = pParticle->GetParticleEventSequence()->GetTriggerCount();
				float fTriggerTime = pParticle->GetParticleEventSequence()->GetTriggerTime();
				int iDrawCount = pParticle->GetParticleEventSequence()->GetDrawCount();
				bool bCullCheck = pParticle->GetParticleEventSequence()->GetCullCheck();
				CKTDGParticleSystem::PARTICLE_TYPE eParticleType = pParticle->GetParticleEventSequence()->GetParticleType();
				

				ofs << "g_pObjectParticle = g_pWorld:CreateObjectParticle( g_pMinorParticle, \"" << strParticleSequenceName << "\" )" << std::endl;
				ofs << "g_pObjectParticle:SetParticlePos( D3DXVECTOR3( " << vPos.x << ", " << vPos.y << ", " << vPos.z << " ) )" << std::endl;
				ofs << "g_pObjectParticle:SetEmitMinMax( " << (float)emitRate.m_Min << ", " << (float)emitRate.m_Max << " )" << std::endl;
				ofs << "g_pObjectParticle:SetTriggerCount( " << iTriggerCount << " )" << std::endl;
				ofs << "g_pObjectParticle:SetTriggerTime( " << fTriggerTime << " )" << std::endl;
				ofs << "g_pObjectParticle:SetDrawCount( " << iDrawCount << " )" << std::endl;
				if( true == bCullCheck )
				{
					ofs << "g_pObjectParticle:SetCullCheck( true )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectParticle:SetCullCheck( false )" << std::endl;
				}

				switch( eParticleType )
				{
				case CKTDGParticleSystem::PT_MESH:
					ofs << "g_pObjectParticle:SetParticleType( PARTICLE_TYPE[\"PT_MESH\"] )" << std::endl;
					break;
				case CKTDGParticleSystem::PT_2D_PLANE:
					ofs << "g_pObjectParticle:SetParticleType( PARTICLE_TYPE[\"PT_2D_PLANE\"] )" << std::endl;
					break;
				case CKTDGParticleSystem::PT_3D_PLANE:
					ofs << "g_pObjectParticle:SetParticleType( PARTICLE_TYPE[\"PT_3D_PLANE\"] )" << std::endl;
					break;
				}

				if( mapLayerString.end() != mapLayerString.find( pWorldToolObject->GetLayer() ) )
				{
					ofs << "g_pObjectSkinMesh:SetLayer( " << mapLayerString[ pWorldToolObject->GetLayer() ] << " )" << std::endl;
				}
				else
				{
					ofs << "g_pObjectSkinMesh:SetLayer( " << "X2_LAYER[\"XL_SKY_WORLD_BASE_0\"]" << " )" << std::endl;
				}

				
				ofs << std::endl << std::endl;			
			} break;
		}		
	}	


	ofs.close();

	ConvertFileAnsiToUTF8( strFileName, strFileName );

	return hr;
}


bool CX2WorldToolMain::SaveTempLineMap( const WCHAR* wszFileName )
{	
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();
	if( pLineMap == NULL )
		return false;


	string strFileName;
	ConvertWCHARToChar( strFileName, wszFileName );
	std::ofstream ofs( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return false;
	}


	ofs << std::endl << std::endl;
	float fLandHeight = pLineMap->GetLandHeight();
	ofs << "g_pLineMap:SetLandHeight( " << fLandHeight << " )" << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- RED, BLUE, TEAM START POS" << std::endl;
    
	D3DXVECTOR3 vPos = pLineMap->GetLineData(0)->startPos;
	ofs << "g_pLineMap:SetTeamStartPos( true, D3DXVECTOR3( " 
		<< vPos.x << ", " << vPos.y << ", " << vPos.z << " ), false, 0 ) " << std::endl;

	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;

	ofs << "-- LINE DATA -- " << std::endl;


	// 라인정보 출력
	CKTDGLineMap::LineData* pLineData = NULL;	
	std::string strFootDustName;
	strFootDustName.reserve(512);
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);

		ofs << "g_pLineMap:AddLine_LUA" << std::endl;
		ofs << "{" << std::endl;
		ofs << "	LINE_NUM		= " << i << ", " << std::endl;
		if( true == pLineData->bOriEnable )
		{
			ofs << "	ENABLE			= TRUE," << std::endl;
		}
		else
		{
			ofs << "	ENABLE			= FALSE," << std::endl;
		}

		ofs << "	LINE_TYPE 		= ";
		switch( pLineData->lineType )
		{
		case CKTDGLineMap::LT_NORMAL:
			ofs << "LINE_TYPE[\"LT_NORMAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_WALL:
			ofs << "LINE_TYPE[\"LT_WALL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_POTAL:
			ofs << "LINE_TYPE[\"LT_POTAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_HOLE:
			ofs << "LINE_TYPE[\"LT_HOLE\"], " << std::endl;
			break;

		//{{ 09.03.02 김태완 : 추가 라인맵 타입들 적용(reload 문제 해결)
		case CKTDGLineMap::LT_JUMP_UP_REL:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_REL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_JUMP_UP_ABS:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_ABS\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_BUNGEE:
			ofs << "LINE_TYPE[\"LT_BUNGEE\"], " << std::endl;
			break;
#ifdef MONSTER_ROAD
		case CKTDGLineMap::LT_MONSTER_ROAD:
			ofs << "LINE_TYPE[\"LT_MONSTER_ROAD\"], " << std::endl;
			break;
#endif
#ifdef UNIT_ROAD
		case CKTDGLineMap::LT_UNIT_ROAD:
			ofs << "LINE_TYPE[\"LT_UNIT_ROAD\"], " << std::endl;
			break;
#endif

		default:		
			ofs << "LINE_TYPE[\"LT_NORMAL\"], " << std::endl;
			break;
//}}
		}

		ofs << "	LINEMAP_DIRECTION_OPTION 		= ";
		switch( pLineData->m_eDirection )
		{
		case CKTDGLineMap::LDO_NONE:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_NONE\"], " << std::endl;
			break;

		case CKTDGLineMap::LDO_LEFT:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_LEFT\"], " << std::endl;
			break;

		case CKTDGLineMap::LDO_RIGHT:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_RIGHT\"], " << std::endl;
			break;
		}

		ofs << "	JUMP_REL_UP_KEYSTOP_TIME	= " << pLineData->m_fKeyStopTime << ", " << std::endl;


		ofs << "	POTAL_GROUP		= " << pLineData->potalGroup << ", " << std::endl;
		ofs << "	STOP_FACTOR		= " << pLineData->fStopFactor << ", " << std::endl;


		//ConvertWCHARToChar( strFootDustName, pLineData->footDustName.c_str() );			// fix!!!
		//ofs << "	FOOT_DUST_NAME	= \"" << strFootDustName.c_str() << "\", " << std::endl;
		ofs << "	BEFORE_LINE		= " << pLineData->beforeLine << ", " << std::endl;
		ofs << "	NEXT_LINE		= " << pLineData->nextLine << ", " << std::endl;
		ofs << "	LINE_SET		= " << pLineData->lineSet << ", " << std::endl;

		if( true == pLineData->bCanDown )
		{
			//ofs << "	CAN_DOWN		= TRUE, " << std::endl;
		}
		else
		{
			ofs << "	CAN_DOWN		= FALSE, " << std::endl;
		}

		ofs << "	\"START_POS		= D3DXVECTOR3(" << pLineData->startPos.x << ", " << pLineData->startPos.y << ", " << pLineData->startPos.z << ")\", " << std::endl;
		ofs << "	\"END_POS		= D3DXVECTOR3(" << pLineData->endPos.x << ", " << pLineData->endPos.y << ", " << pLineData->endPos.z << ")\", " << std::endl;

		ofs << "}" << std::endl;
		ofs << std::endl;
	}

	ConvertFileAnsiToUTF8( strFileName, strFileName );
	return true;
}


void CX2WorldToolMain::CreateGame()
{
	const int MAGIC_USER_UID				= 777;
	const int MAGIC_UNIT_UID_ELSWORD 		= 777;
	//int MAGIC_UNIT_UID_ARME	 		= 778;
	//int MAGIC_UNIT_UID_LIRE	 		= 779;
	//int MAGIC_UNIT_UID_KNIGHT			= 780;
	//int MAGIC_UNIT_UID_MAGIC_KNIGHT	= 781;

	// user data setup
    {
	    //CX2User::UserData* pUserData = new CX2User::UserData;
        CX2User::UserData kUserData;
	    {
		    kUserData.userUID			= MAGIC_USER_UID;
		    kUserData.userID			= L"WorldToolUserID";
		    kUserData.userAuthLevel	= CX2User::XUAL_NORMAL_USER;
		    kUserData.IP				= L"127.0.0.1";
		    kUserData.port				= 8765;
	    }
	    g_pData->ResetMyUser( kUserData );
    }



	// note!! 전직 추가되면 수정
	KUnitInfo kUnitInfo;
	CX2Unit* pUnit = NULL;
	{
		int iUnitUID = MAGIC_UNIT_UID_ELSWORD;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char) CX2Unit::UC_ELSWORD_SWORDMAN;
		kUnitInfo.m_wstrNickName	= L"엘소드";
		kUnitInfo.m_ucLevel					= 200;
		kUnitInfo.m_kStat.m_iBaseHP			= 9999;
		kUnitInfo.m_kGameStat.m_iBaseHP		= 9999;

		kUnitInfo.m_kStat.m_iAtkPhysic		= 100;
		kUnitInfo.m_kStat.m_iAtkMagic		= 100;
		kUnitInfo.m_kGameStat.m_iAtkPhysic	= 100;
		kUnitInfo.m_kGameStat.m_iAtkMagic	= 100;

		kUnitInfo.m_kStat.m_iDefPhysic		= 9999;
		kUnitInfo.m_kStat.m_iDefMagic		= 9999;
		kUnitInfo.m_kGameStat.m_iDefPhysic	= 9999;
		kUnitInfo.m_kGameStat.m_iDefMagic	= 9999;

		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );

		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_ELSWORD_KNIGHT;
		kUnitInfo.m_wstrNickName	= L"나이트";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );


		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_ELSWORD_MAGIC_KNIGHT;
		kUnitInfo.m_wstrNickName	= L"매직나이트";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );



		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_ARME_VIOLET_MAGE;
		kUnitInfo.m_wstrNickName	= L"아이샤";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );

		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_ARME_HIGH_MAGICIAN;
		kUnitInfo.m_wstrNickName	= L"하이 매지션";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );

		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_ARME_DARK_MAGICIAN;
		kUnitInfo.m_wstrNickName	= L"다크 매지션";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );



		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_LIRE_ELVEN_RANGER;
		kUnitInfo.m_wstrNickName	= L"레나";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );



		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_LIRE_COMBAT_RANGER;
		kUnitInfo.m_wstrNickName	= L"컴뱃레인저";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );


		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_LIRE_SNIPING_RANGER;
		kUnitInfo.m_wstrNickName	= L"스나이핑레인저";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );


		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_RAVEN_FIGHTER;
		kUnitInfo.m_wstrNickName	= L"레이븐";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );



		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_RAVEN_SOUL_TAKER;
		kUnitInfo.m_wstrNickName	= L"레이븐 SWORD";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );



		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_RAVEN_OVER_TAKER;
		kUnitInfo.m_wstrNickName	= L"레이븐 OVER";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );





		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_EVE_NASOD;
		kUnitInfo.m_wstrNickName	= L"이브";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );


#ifdef EVE_FIRST_CHANGE_JOB
		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_EVE_EXOTIC_GEAR;
		kUnitInfo.m_wstrNickName	= L"엑조틱";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );

		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_EVE_ARCHITECTURE;
		kUnitInfo.m_wstrNickName	= L"아키텍쳐";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );

#endif

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_CHUNG_IRON_CANNON;
		kUnitInfo.m_wstrNickName	= L"청";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
		iUnitUID += 1;
		kUnitInfo.m_iOwnerUserUID	= MAGIC_USER_UID;
		kUnitInfo.m_nUnitUID		= iUnitUID;
		kUnitInfo.m_cUnitClass		= (char)CX2Unit::UC_ARA_MARTIAL_ARTIST;
		kUnitInfo.m_wstrNickName	= L"아라";
		pUnit						= new CX2Unit( kUnitInfo );
		pUnit->SetOwnerUserUID( MAGIC_USER_UID );
		g_pData->GetMyUser()->AddUnit( pUnit );
#endif
	}

	// 태완 수정
	g_pData->GetMyUser()->SelectUnit( g_pData->GetMyUser()->GetUnitByUID( MAGIC_UNIT_UID_ELSWORD ) );
	//g_pData->GetMyUser()->SelectUnit( g_pData->GetMyUser()->GetUnitByUID( MAGIC_UNIT_UID_ELSWORD + 6 ) );

	CX2GageManager* pGageManager = CX2GageManager::GetInstance();

	if( NULL != pGageManager )
	{
		pGageManager->CreateMyGageUI( pUnit->GetClass(), pUnit->GetUID() );
	}
	
	CX2PVPRoom* pRoom = g_pData->ResetPVPRoom();
	pRoom->SetWorldID( CX2World::WI_WORLDTOOL_DEFAULT );

	std::vector<KRoomSlotInfo> vecKRoomSlotInfo;
	{
		KRoomSlotInfo kRoomSlotInfo;
		kRoomSlotInfo.m_Index		= 0;
		kRoomSlotInfo.m_SlotState	= CX2Room::SS_LOADING;
		kRoomSlotInfo.m_bHost		= true;
		kRoomSlotInfo.m_bReady		= true;
		kRoomSlotInfo.m_TeamNum		= CX2Room::TN_RED;

		kRoomSlotInfo.m_kRoomUserInfo.m_iOwnerUserUID	= (UidType) MAGIC_USER_UID;
		kRoomSlotInfo.m_kRoomUserInfo.m_nUnitUID		= MAGIC_UNIT_UID_ELSWORD;
		kRoomSlotInfo.m_kRoomUserInfo.m_cUnitClass		= (char)CX2Unit::UC_ELSWORD_SWORDMAN;
		kRoomSlotInfo.m_kRoomUserInfo.m_wstrNickName	= L"엘소드";
		vecKRoomSlotInfo.push_back( kRoomSlotInfo );
	}


	pRoom->Set_KRoomSlotInfoList( vecKRoomSlotInfo );
	//pRoom->Set_KRoomSlotInfoList( vecKRoomSlotInfo, false );


	CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData(0);
	g_pData->GetGameUDP()->SetMyUID( pSlotData->m_pUnit->GetUID() );
	g_pData->GetGameUDP()->AddPeer( pSlotData->m_pUnit->GetUID(), 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        g_pData->GetGameUDP()->GetMyIPAddress(), 
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        g_pData->GetGameUDP()->GetMyIP(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        g_pData->GetGameUDP()->GetMyExtPort() );


	// game 생성
	g_pX2Game = NULL;
	SAFE_DELETE( m_pGame );
	m_pGame = new CWorldToolGame();
	m_pGame->GameLoading( (CX2Room*)g_pData->GetPVPRoom() );
	if( NULL != m_pGame->GetWorld() )
	{
		m_pGame->GetWorld()->StopBGM();
	}

	// tool 에서는 combo manager가 필요없다~
	m_pGame->GetMyUnit()->DeleteComboManager();

	g_pX2Game = m_pGame;

	UpdateWorldToolObjectList();

	ResetUserUnitStateListBox();

	CreateCharacterSkinMesh( L"ELSWORD" );

	g_pKTDXApp->SkipFrame();
}



void CX2WorldToolMain::DisableGameKey()
{
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_A);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_A, FALSE);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_S);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_S, FALSE);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_D);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_D, FALSE);

	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_Z);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_Z, FALSE);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_X);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_X, FALSE);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_C);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_C, FALSE);

	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_UP);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_UP, FALSE);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_DOWN);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_DOWN, FALSE);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_LEFT);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_LEFT, FALSE);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetDoubleKeyReset(DIK_RIGHT);
	g_pKTDXApp->GetDIManager()->Getkeyboard()->SetKeyState(DIK_RIGHT, FALSE);	
}


void CX2WorldToolMain::ClearXFilePalette()
{
	CDXUTListBox* pListBox_Palette = this->m_vecpDialog[DLG_OBJECT_LIST]->GetListBox( IDC_LIST_OBJECT_PALETTE );
	pListBox_Palette->RemoveAllItems();

	pListBox_Palette->SetEnabled(false);

#if 0 
    if( NULL == m_pGame->GetWorld() )
        return;

    CX2World* pWorld = m_pGame->GetWorld();
    if(pWorld != NULL)
        pWorld->ClearObjectMesh();

//     g_pKTDXApp->GetDeviceManager()->CloseCachedXMeshes();
//     g_pKTDXApp->GetDeviceManager()->CloseCachedXSkinMeshes();
#endif
}


// 현재 파일이 추가된 디렉토리의 모든 .x 파일을 검색해서 lib에 추가
// @ bForceXMesh : 모든 X 파일을 X Mesh로 인식해서 로딩하기
void CX2WorldToolMain::LoadXFileToPalette( WCHAR* wszFileName )
{
	bool bDone = false;


	WCHAR wszFilePathOnly[512] = L"";
	WCHAR wszFileNameOnly[256] = L"";
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wszFileName );

	string strFilePathOnly;
	ConvertWCHARToChar( strFilePathOnly, wszFilePathOnly );
	g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strFilePathOnly );


	CDXUTListBox* pListBox_Palette = this->m_vecpDialog[DLG_OBJECT_LIST]->GetListBox( IDC_LIST_OBJECT_PALETTE );

	// .X 파일 찾기
	wstring wstrPathX = wszFilePathOnly;
	wstrPathX += L"*.X";
	ConvertWCHARToChar( strFilePathOnly, wstrPathX.c_str() );
	
	HANDLE				hSearch;
	WIN32_FIND_DATAA	fd;
	hSearch = FindFirstFileA( strFilePathOnly.c_str(), &fd );
	if(hSearch != INVALID_HANDLE_VALUE)
	{
		wstring wstrFileName = L"";
		
		do
		{
			if( 0 != strcmp(fd.cFileName, ".") && 0 != strcmp(fd.cFileName, "..") )
			{
				if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{			
					continue;
				}
				else
				{
					ConvertCharToWCHAR( wstrFileName, fd.cFileName );
					pListBox_Palette->AddItem( wstrFileName.c_str(), (LPVOID) NULL ); 
				}
			}
		} while( FindNextFileA(hSearch, &fd) );
		FindClose(hSearch);

		bDone = true;

	}
	


	// .Y 파일 찾기
	wstrPathX = wszFilePathOnly;
	wstrPathX += L"*.Y";
	ConvertWCHARToChar( strFilePathOnly, wstrPathX.c_str() );

	hSearch = FindFirstFileA( strFilePathOnly.c_str(), &fd );
	if(hSearch != INVALID_HANDLE_VALUE)
	{
		wstring wstrFileName = L"";
		do
		{
			
			if( 0 != strcmp(fd.cFileName, ".") && 0 != strcmp(fd.cFileName, "..") )
			{
				if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{			
					continue;
				}
				else
				{
					ConvertCharToWCHAR( wstrFileName, fd.cFileName );
					pListBox_Palette->AddItem( wstrFileName.c_str(), (LPVOID) NULL ); 
				}
			}
		} while( FindNextFileA(hSearch, &fd) );
		FindClose(hSearch);
		bDone = true;
	}

	if( true == bDone )
	{

		pListBox_Palette->SetEnabled(true);
	}
}


void CX2WorldToolMain::AddPickedPaletteObjectToWorld( CX2WorldObject::OBJECT_TYPE eObjType )
{
	CDXUTListBox* pListBox_Palette = this->m_vecpDialog[DLG_OBJECT_LIST]->GetListBox( IDC_LIST_OBJECT_PALETTE );

	int nSelected = -1;
	nSelected = pListBox_Palette->GetSelectedIndex( nSelected );
	while( -1 != nSelected )
	{
		DXUTListBoxItem *pItem = pListBox_Palette->GetItem( nSelected );
		if( NULL == pItem )
			break;

		AddObjectToWorld( pItem->strText, eObjType );		
		nSelected = pListBox_Palette->GetSelectedIndex( nSelected );
	}	
}

void CX2WorldToolMain::AddObjectToWorld( const WCHAR* wszFileName, CX2WorldObject::OBJECT_TYPE eObjType )
{
	if( NULL == m_pGame )
		return;

	if( NULL == m_pGame->GetWorld() )
		return;

	CX2World* pWorld = m_pGame->GetWorld();

	string strFileName;
	ConvertWCHARToChar( strFileName, wszFileName );

    pWorld->ToggleFog(true);

    // device manager에 있는 데이터를 clear
//     g_pKTDXApp->GetDeviceManager()->CloseCachedXMeshes();
//     g_pKTDXApp->GetDeviceManager()->CloseCachedTextures();
//     g_pKTDXApp->GetDeviceManager()->CloseCachedXSkinMeshes();

	const vector<CX2WorldObjectPtr>& vecpWorldObjectList = m_pGame->GetWorld()->GetWorldObjectList();
	for( UINT i=0; i<vecpWorldObjectList.size(); i++ )
	{
		CX2WorldObject* pWorldObject = vecpWorldObjectList[i].get();
		if( 0 == StrCmpW(pWorldObject->GetName(), wszFileName ) && pWorldObject->GetObjectType() != eObjType )
		{
			// 같은 파일명이면서 다른 방식으로 더해져있는 오브젝트가 존재한다
			wstring wstr = L"";
			wstr += L"같은 이름의 File이 다른 방식으로 이미 World에 등록되어 있습니다.\n";
			wstr += L"File Name : ";
			wstr += wszFileName;
			wstr += L"\nWorld에 존재하는 Mesh Type : ";
			switch(pWorldObject->GetObjectType())
			{
			case CX2WorldObject::OT_STATIC_MESH:
				{
					wstr += L"Static Mesh";
				} break;
			case CX2WorldObject::OT_SKIN_MESH:
				{
					wstr += L"Skin Mesh";

				} break;
			}

			MessageBox( g_pKTDXApp->GetHWND(), wstr.c_str(), L"File Load Error", MB_OK );
			return;

		}

	}


	switch( eObjType )
	{
	case CX2WorldObject::OT_STATIC_MESH:
		{
			CX2WorldObjectMesh* pObjectMesh = pWorld->CreateObjectMesh();
			pObjectMesh->SetName( strFileName.c_str() );
			pObjectMesh->SetXMesh_LUA( strFileName.c_str() );
			pObjectMesh->SetXMeshLOD_LUA( strFileName.c_str() );
			pObjectMesh->SetCanHide( false );
			pObjectMesh->SetNotRenderWhenHidden(false);
			pObjectMesh->SetPos( D3DXVECTOR3(0,0,0) );
			pObjectMesh->SetColorFactor( D3DXCOLOR(1,1,1,1) );
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
			pObjectMesh->SetOriginalColorFactor( D3DXCOLOR(1,1,1,1) );
#endif
			pObjectMesh->SetLayer( XL_SKY_WORLD_BASE_0 );
		} break;

	case CX2WorldObject::OT_SKIN_MESH:
		{
			CX2WorldObjectSkinMesh* pSkinMesh = pWorld->CreateObjectSkinMesh();

			pSkinMesh->SetName( strFileName.c_str() );
			pSkinMesh->SetXSkinMotion_LUA( strFileName.c_str() );
			pSkinMesh->AddXSkinModel_LUA( strFileName.c_str() );

			pSkinMesh->SetPos( D3DXVECTOR3(0,0,0) );
			pSkinMesh->SetLayer( XL_SKY_WORLD_BASE_1 );
			pSkinMesh->SetCanHide( false );

			pSkinMesh->SetColorFactor( D3DXCOLOR(1,1,1,1) );
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
			pSkinMesh->SetOriginalColorFactor( D3DXCOLOR(1,1,1,1) );
#endif
			pSkinMesh->SetAngleDegree( D3DXVECTOR3(0, 0, 0) );
			//{{ 09.02.27 김태완 : bAlphaBlend 디폴트 true -> false로 변경
			pSkinMesh->SetAlphaObject( false );
			CKTDGXRenderer::RenderParam* pRenderParam = pSkinMesh->GetRenderParam();
			pRenderParam->bAlphaBlend = false;	
			//}}

			//{{ 09.02.27 김태완 : 혹시 Static Mesh 아닌지 검사
			if(true == pSkinMesh->GetXSkinMotionMesh()->GetIsOnlyModelData())
			{
				// 모션이 없다!!

				pWorld->RemoveWorldObject( pSkinMesh );

				wstring wstr = L"";
				wstr += L"모션 정보가 없는 Mesh File을 Skin Mesh로 등록시도했습니다.\n";
				wstr += L"File Name : ";
				wstr += wszFileName;
				
				MessageBox( g_pKTDXApp->GetHWND(), wstr.c_str(), L"File Load Error", MB_OK );
				break;

			}
			//}}

			pSkinMesh->AddAnim_LUA( "Normal" );
			pSkinMesh->PlayAnim_LUA( CX2WorldObjectSkinMesh::WOSMPY_LOOP );
		} break;

	default:
		break;
	}


	UpdateWorldToolObjectList();
}


void CX2WorldToolMain::RemoveSelectedObjectFromWorld()
{
	if( NULL == m_pGame ||
		NULL == m_pGame->GetWorld() )
	{
		return;
	}

	//CX2World* pWorld = m_pGame->GetWorld();
	for( UINT i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
	{
		CWorldToolObject* pWorldToolObject = m_vecpPickedWorldToolObject.at(i);
		RemoveObjectFromWorld( pWorldToolObject );
	}

	
	UpdateWorldToolObjectList();
}



void CX2WorldToolMain::RemoveObjectFromWorld( CWorldToolObject* pWorldToolObject )
{
	if( NULL == pWorldToolObject )
		return;

	pWorldToolObject->ReleaseSkyDome();
	pWorldToolObject->ReleaseWorldObject();

	//{{EDIT : object in world 리스트 UI 를 통한 멀티 선택 삭제시
	//         아래의 함수호출로 인해 한개만 삭제되고 m_vecpPickedWorldToolObject.clear() 이 호출되기 때문에
	//         멀티 삭제가 이루어 지지 않고 있었음.
	//         RemoveObjectFromWorld() 함수 호출처리 완료후 UpdateWorldToolObjectList(); 한번만 호출해주면 편한구조.
	//UpdateWorldToolObjectList();
	//}}EDIT

	//vector<CX2WorldObject*>& vecpWorldObjectList = m_pGame->GetWorld()->GetWorldObjectList();
	//CX2WorldObject* pWorldObjectToDelete = pWorldToolObject->m_pWorldObject;
	//if( NULL != pWorldObjectToDelete )
	//{
	//	for( UINT j=0; j<vecpWorldObjectList.size(); j++ )
	//	{
	//		CX2WorldObject* pWorldObject = vecpWorldObjectList.at(j);
	//		if( pWorldObject == pWorldObjectToDelete )
	//		{
	//			vecpWorldObjectList.erase( vecpWorldObjectList.begin()+j );
	//			SAFE_DELETE( pWorldObject );
	//			break;
	//		}
	//	}
	//}

	//vector<CKTDGSkyDome*>& vecpSkyDomeList = m_pGame->GetWorld()->GetSkyDomeList();
	//CKTDGSkyDome* pSkyDomeToDelete = pWorldToolObject->m_pSkyDome;
	//if( NULL != pSkyDomeToDelete )
	//{
	//	for( UINT i=0; i<vecpSkyDomeList.size(); i++ )
	//	{
	//		CKTDGSkyDome* pSkyDome = vecpSkyDomeList.at(i);
	//		if( pSkyDome == pSkyDomeToDelete )
	//		{
	//			vecpSkyDomeList.erase( vecpSkyDomeList.begin()+i );
	//			SAFE_DELETE( pSkyDome );
	//			break;
	//		}
	//	}
	//}
}


void CX2WorldToolMain::ChangeUserUnit( CX2Unit::UNIT_CLASS eUnitClass )
{
	CX2Room::SlotData* pSlotData = g_pData->GetPVPRoom()->GetSlotData(0);

	bool bOK = false;
	for( int i=0; i< g_pData->GetMyUser()->GetUnitNum(); i++ )
	{
		CX2Unit* pUnit = g_pData->GetMyUser()->GetUnitByIndex( i );
		if( NULL != pUnit && pUnit->GetClass() == eUnitClass )
		{
			pSlotData->m_pUnit = pUnit;
			pSlotData->m_UnitUID = pSlotData->m_pUnit->GetUID();
			bOK = true;
			break;
		}
	}

	if( false == bOK )
		return; 
	
	m_pGame->DeleteUserUnit(0);
	m_pGame->AddUserUnit( pSlotData, 0 );
	m_pGame->ChangeFocusUnit( NULL );


	// 
	m_pGame->GetMyUnit()->DeleteComboManager();

	g_pKTDXApp->SkipFrame();
		
	m_iPickedUserUnitState		= -1;

	ResetUserUnitStateListBox();

	g_pData->GetGameUDP()->SetMyUID( pSlotData->m_pUnit->GetUID() );
	g_pData->GetGameUDP()->AddPeer( pSlotData->m_pUnit->GetUID(), 
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        g_pData->GetGameUDP()->GetMyIPAddress(), 
//#else   SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//        g_pData->GetGameUDP()->GetMyIP(), 
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        g_pData->GetGameUDP()->GetMyExtPort() );


	switch( pSlotData->m_pUnit->GetType() )
	{
	case CX2Unit::UT_ELSWORD:
		CreateCharacterSkinMesh( L"ELSWORD" );
		break;

	case CX2Unit::UT_ARME:
		CreateCharacterSkinMesh( L"ARME" );
		break;

	case CX2Unit::UT_LIRE:
		CreateCharacterSkinMesh( L"LIRE" );
		break;

	case CX2Unit::UT_RAVEN:
		CreateCharacterSkinMesh( L"RAVEN" );
		break;


	case CX2Unit::UT_EVE:
		CreateCharacterSkinMesh( L"EVE" );
		break;


	}
}

void CX2WorldToolMain::RenderBoundingSphere( D3DXVECTOR3& vCenter, float fRadius )
{

	if( NULL == m_pXMeshBoundingSphere )
		return; 

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	
	//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	CKTDGXRenderer::RenderParam renderParam;

	//pRenderParam->renderType = CKTDGXRenderer::RT_REAL_COLOR;
	//pRenderParam->color	= D3DXCOLOR(0,1,0,1); 
	renderParam.renderType = CKTDGXRenderer::RT_REAL_COLOR;		
	renderParam.color = D3DXCOLOR(0,1,0,1);

	matTemp.Scale( fRadius, fRadius, fRadius );
	matTemp.Move( vCenter );		
	//pRenderParam->worldMatrix = matTemp.GetMatrix();
	D3DXMATRIX worldMatrix = matTemp.GetMatrix();

	//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
	//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshBoundingSphere );
	//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
	g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshBoundingSphere );
	
}



//////////////////////////////////////////////////////////////////////////
// 이펙트 툴 함수 
//////////////////////////////////////////////////////////////////////////
void CX2WorldToolMain::UpdatePickedXMeshPlayerUI()
{

	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_EFFECT_PROPERTY];
	CDXUTEditBox* pEditBox_Delay	= pDialog->GetEditBox( IDC_EDIT_MESH_PLAYER_DELAY );
	CDXUTEditBox* pEditBox_PosX 	= pDialog->GetEditBox( IDC_EDIT_MESH_PLAYER_POS_X );
	CDXUTEditBox* pEditBox_PosY 	= pDialog->GetEditBox( IDC_EDIT_MESH_PLAYER_POS_Y );
	CDXUTEditBox* pEditBox_PosZ 	= pDialog->GetEditBox( IDC_EDIT_MESH_PLAYER_POS_Z );

	CDXUTCheckBox* pCheckBox_Trace	= pDialog->GetCheckBox( IDC_CHECK_MESH_PLAYER_TRACE );
	CDXUTEditBox* pEditBox_BoneName 	= pDialog->GetEditBox( IDC_EDIT_MESH_PLAYER_BONE_NAME );

	CDXUTCheckBox* pCheckBox_RelativeToUnit = pDialog->GetCheckBox( IDC_CHECK_MESH_PLAYER_POS_IS_RELATIVE_TO_UNIT );


	CWorldToolDialog* pDialog_EffectList		= m_vecpDialog[DLG_EFFECT_TOOL];
	CDXUTListBox* pListBox = pDialog_EffectList->GetListBox( CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_LOADED ); 
	DXUTListBoxItem* pItem = pListBox->GetSelectedItem();

		
	if( -1 == m_iPickedXMeshPlayerIndex || NULL == pItem )
	{
		pEditBox_Delay->SetText( L"" );
		pEditBox_PosX->SetText( L"" );
		pEditBox_PosY->SetText( L"" );
		pEditBox_PosZ->SetText( L"" );

		return;
	}

	WCHAR wszTextOut[64];
		
	D3DXVECTOR3 vPos = m_vecTimedXMeshPlayer[m_iPickedXMeshPlayerIndex].m_vRelativePosition;
	float fDelay = m_vecTimedXMeshPlayer[m_iPickedXMeshPlayerIndex].m_fDelayTimeToStart;

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fDelay );	
	pEditBox_Delay->SetText( wszTextOut );

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.x );	
	pEditBox_PosX->SetText( wszTextOut );

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.y );	
	pEditBox_PosY->SetText( wszTextOut );

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.z );	
	pEditBox_PosZ->SetText( wszTextOut );

	pCheckBox_Trace->SetChecked( m_vecTimedXMeshPlayer[m_iPickedXMeshPlayerIndex].m_bTraceUnit );
	pCheckBox_RelativeToUnit->SetChecked( m_vecTimedXMeshPlayer[m_iPickedXMeshPlayerIndex].m_bRelativeToUnit );

	pEditBox_BoneName->SetText( m_vecTimedXMeshPlayer[m_iPickedXMeshPlayerIndex].m_wstrBoneName.c_str() );
}



void CX2WorldToolMain::UpdatePickedParticleUI()
{

	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_EFFECT_PROPERTY];
	CDXUTEditBox* pEditBox_Delay	= pDialog->GetEditBox( IDC_EDIT_PARTICLE_DELAY );
	CDXUTEditBox* pEditBox_PosX 	= pDialog->GetEditBox( IDC_EDIT_PARTICLE_POS_X );
	CDXUTEditBox* pEditBox_PosY 	= pDialog->GetEditBox( IDC_EDIT_PARTICLE_POS_Y );
	CDXUTEditBox* pEditBox_PosZ 	= pDialog->GetEditBox( IDC_EDIT_PARTICLE_POS_Z );

	CDXUTEditBox* pEditBox_Particle_BoneName 	= pDialog->GetEditBox( IDC_EDIT_PARTICLE_BONE_NAME );

	CDXUTCheckBox* pCheckBox_Particle_Trace	= pDialog->GetCheckBox( IDC_CHECK_PARTICLE_TRACE );
	CDXUTCheckBox* pCheckBox_Particle_RelativeToUnit = pDialog->GetCheckBox( IDC_CHECK_PARTICLE_POS_IS_RELATIVE_TO_UNIT );


	CWorldToolDialog* pDialog_EffectList		= m_vecpDialog[DLG_EFFECT_TOOL];
	CDXUTListBox* pListBox = pDialog_EffectList->GetListBox( CX2WorldToolMain::IDC_LIST_PARTICLE_LOADED ); 
	DXUTListBoxItem* pItem = pListBox->GetSelectedItem();


	if( -1 == m_iPickedParticleIndex || NULL == pItem )
	{
		pEditBox_Delay->SetText( L"" );
		pEditBox_PosX->SetText( L"" );
		pEditBox_PosY->SetText( L"" );
		pEditBox_PosZ->SetText( L"" );

		return;
	}

	WCHAR wszTextOut[64];

	D3DXVECTOR3 vPos = m_vecTimedParticle[m_iPickedParticleIndex].m_vRelativePosition;
	float fDelay = m_vecTimedParticle[m_iPickedParticleIndex].m_fDelayTimeToStart;

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", fDelay );	
	pEditBox_Delay->SetText( wszTextOut );

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.x );	
	pEditBox_PosX->SetText( wszTextOut );

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.y );	
	pEditBox_PosY->SetText( wszTextOut );

	StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", vPos.z );	
	pEditBox_PosZ->SetText( wszTextOut );


	pCheckBox_Particle_Trace->SetChecked( m_vecTimedParticle[m_iPickedParticleIndex].m_bTraceUnit );
	pCheckBox_Particle_RelativeToUnit->SetChecked( m_vecTimedParticle[m_iPickedParticleIndex].m_bRelativeToUnit );

	pEditBox_Particle_BoneName->SetText( m_vecTimedParticle[m_iPickedParticleIndex].m_wstrBoneName.c_str() );

}


void CX2WorldToolMain::UpdatePickedCameraShakeUI()
{
	CWorldToolDialog* pDialog			= m_vecpDialog[DLG_EFFECT_TOOL2];
	CDXUTEditBox* pEditBox_CameraShake	= pDialog->GetEditBox( IDC_EDIT_CAMERA_SHAKE_UP );
	pEditBox_CameraShake->SetText( L"시작시간 진폭 지속시간" );

	if( m_vecPickedCameraShake.size() > 0 )
	{
		int iCameraShakeIndex = m_vecPickedCameraShake[0];
		if( iCameraShakeIndex >= 0 && iCameraShakeIndex < (int)m_vecCameraShake.size() )
		{
			CameraShake& cameraShake = m_vecCameraShake[iCameraShakeIndex];

			float afValue[3];
			afValue[0] = cameraShake.m_fStartTime;
			afValue[1] = cameraShake.m_fAmplitude;
			afValue[2] = cameraShake.m_fDuration;

			pEditBox_CameraShake->SetTextFloatArray( &afValue[0], 3 );
		}
	}
}


void CX2WorldToolMain::UpdateSquareCameraUI()
{
	if( NULL == m_pGame )
		return;

	CWorldToolDialog* pDialog				= m_vecpDialog[DLG_MAIN];
	CDXUTEditBox* pEditBox_CameraDistance	= pDialog->GetEditBox( IDC_EDIT_SQUARE_CAMERA_DISTANCE );
	CDXUTEditBox* pEditBox_LookAtHeight		= pDialog->GetEditBox( IDC_EDIT_SQUARE_CAMERA_LOOKAT_HEIGHT );
	CDXUTEditBox* pEditBox_EyeHeight		= pDialog->GetEditBox( IDC_EDIT_SQUARE_CAMERA_EYE_HEIGHT );

	pEditBox_CameraDistance->SetTextFloatArray( &m_pGame->m_SquareCameraParam.m_fEyeDistance, 1 );
	pEditBox_LookAtHeight->SetTextFloatArray( &m_pGame->m_SquareCameraParam.m_fLookAtHeight, 1 );
	pEditBox_EyeHeight->SetTextFloatArray( &m_pGame->m_SquareCameraParam.m_fEyeHeight, 1 );
	
}


void CX2WorldToolMain::ResetUserUnitStateListBox()
{
	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_EFFECT_TOOL];
	CDXUTListBox* pListBox = pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_USER_UNIT_STATE ); 

	ClearUserUnitStateListBox();

	const map<UCHAR, CX2GUUser::UserUnitStateData>& mapStateList = g_pX2Game->GetMyUnit()->GetStateList();
	map<UCHAR, CX2GUUser::UserUnitStateData>::const_iterator it;
	
	for( it=mapStateList.begin(); it!=mapStateList.end(); it++ )
	{
		int* pStateIndex = new int;
		*pStateIndex = (int)it->first;
		const CX2GUUser::UserUnitStateData& stateData = it->second;
		//pListBox->AddItem( stateData.stateTable.c_str(), (void*)pStateIndex );
		//** 임시 코드
		WCHAR buf[225];
		wsprintf(buf, L"%d", (int)stateData.stateID);
		pListBox->AddItem( buf, (void*)pStateIndex );
	}
}


void CX2WorldToolMain::ClearUserUnitStateListBox()
{
	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_EFFECT_TOOL];
	CDXUTListBox* pListBox = pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_USER_UNIT_STATE ); 

	for( int i=0; i<pListBox->GetSize(); i++ )
	{
		int* pTemp = (int*)pListBox->GetItem(i)->pData;
		SAFE_DELETE( pTemp );
		pListBox->GetItem(i)->pData = NULL;
	}
	pListBox->RemoveAllItems();

}

void CX2WorldToolMain::ClearTimedXMeshPlayerList()
{
	
	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_EFFECT_TOOL];
	CDXUTListBox* pListBox = pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_LOADED ); 
	pListBox->RemoveAllItems();
	pListBox->SelectItem(0);

	m_vecTimedXMeshPlayer.clear();
	m_iPickedXMeshPlayerIndex = -1;

	UpdatePickedXMeshPlayerUI();
}


void CX2WorldToolMain::ClearTimedParticleList()
{
	
	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_EFFECT_TOOL];
	CDXUTListBox* pListBox = pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_PARTICLE_LOADED ); 
	pListBox->RemoveAllItems();
	pListBox->SelectItem(0);

	m_vecTimedParticle.clear();
	m_iPickedParticleIndex = -1;

	UpdatePickedParticleUI();
}

void CX2WorldToolMain::ClearCameraShakeList()
{
	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_EFFECT_TOOL2];
	CDXUTListBox* pListBox = pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_CAMERA_SHAKE_UP ); 
	pListBox->RemoveAllItems();
	pListBox->SelectItem(0);

	m_vecCameraShake.clear();
	m_vecPickedCameraShake.clear();

	UpdatePickedCameraShakeUI();
}



HRESULT CX2WorldToolMain::SaveEffectSet( const WCHAR* pwszFileFullPath )
{
	HRESULT hr = S_OK;

	if( NULL == m_pGame || NULL == m_pGame->GetWorld() )
		return E_FAIL;

	// file path와 name을 분리
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	_wsplitpath( pwszFileFullPath, drive, dir, fname, ext);

	string strFileName;
	ConvertWCHARToChar( strFileName, fname );
	strFileName += ".lua";

	std::ofstream ofs;
	ofs.open( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return E_FAIL;
	}


	ofs << "EFFECT_SET = " << endl;
	ofs << "{" << endl;

	ofs << "\t MESH_PLAYER = " << endl;
	ofs << "\t { " << endl;


	for( UINT i=0; i<m_vecTimedXMeshPlayer.size(); i++ )
	{
		ofs << "\t\t{" << endl;

		string strname;
		ConvertWCHARToChar( strname, m_vecTimedXMeshPlayer[i].m_wstrXMeshPlayerName.c_str() );
		ofs << "\t\t\t EFFECT_TYPE =	EFFECT_TYPE[\"ET_MESH_PLAYER_GAME_MAJOR\"]" << "," << endl;
		ofs << "\t\t\t NAME			= \"" << strname << "\"," << endl;
		ofs << "\t\t\t START_TIME	= " << m_vecTimedXMeshPlayer[i].m_fDelayTimeToStart << "," << endl;


		ofs << "\t\t\t POSITION_OFFSET_X 		= " << m_vecTimedXMeshPlayer[i].m_vRelativePosition.x << ", " << endl;
		ofs << "\t\t\t POSITION_OFFSET_Y 		= " << m_vecTimedXMeshPlayer[i].m_vRelativePosition.y << ", " << endl;
		ofs << "\t\t\t POSITION_OFFSET_Z 		= " << m_vecTimedXMeshPlayer[i].m_vRelativePosition.z << ", " << endl;



		/*if( true == m_vecTimedXMeshPlayer[i].m_bRelativeToUnit )
		{
			ofs << "\t\t\t POS_RELATIVE_TO_UNIT 		= TRUE, " << endl;
		}
		else
		{
			ofs << "\t\t\t POS_RELATIVE_TO_UNIT 		= FALSE, " << endl;
		}*/





		if( true == m_vecTimedXMeshPlayer[i].m_bTraceUnit )
		{
			ofs << "\t\t\t TRACE 		= TRUE, " << endl;
		}
		else
		{
			ofs << "\t\t\t TRACE 		= FALSE, " << endl;
		}

		if( 0 != m_vecTimedXMeshPlayer[i].m_wstrBoneName.length() )
		{
			ConvertWCHARToChar( strname, m_vecTimedXMeshPlayer[i].m_wstrBoneName.c_str() );
			ofs << "\t\t\t BONE_NAME	= \"" << strname << "\"," << endl;
		}

		ofs << "\t\t}," << endl;
	}


	ofs << "\t }, " << endl;



	ofs << "\t PARTICLE = " << endl;
	ofs << "\t { " << endl;


	for( UINT i=0; i<m_vecTimedParticle.size(); i++ )
	{
		ofs << "\t\t{" << endl;

		string strname;
		ConvertWCHARToChar( strname, m_vecTimedParticle[i].m_wstrParticleName.c_str() );
		ofs << "\t\t\t EFFECT_TYPE =	EFFECT_TYPE[\"ET_PARTICLE_GAME_MAJOR\"]" << "," << endl;
		ofs << "\t\t\t NAME			= \"" << strname << "\"," << endl;
		ofs << "\t\t\t START_TIME	= " << m_vecTimedParticle[i].m_fDelayTimeToStart << "," << endl;

		ofs << "\t\t\t POSITION_OFFSET_X 		= " << m_vecTimedParticle[i].m_vRelativePosition.x << ", " << endl;
		ofs << "\t\t\t POSITION_OFFSET_Y 		= " << m_vecTimedParticle[i].m_vRelativePosition.y << ", " << endl;
		ofs << "\t\t\t POSITION_OFFSET_Z 		= " << m_vecTimedParticle[i].m_vRelativePosition.z << ", " << endl;



		/*
		if( true == m_vecTimedParticle[i].m_bRelativeToUnit )
		{
			ofs << "\t\t\t POS_RELATIVE_TO_UNIT 		= TRUE, " << endl;
		}
		else
		{
			ofs << "\t\t\t POS_RELATIVE_TO_UNIT 		= FALSE, " << endl;
		}
		*/


		if( true == m_vecTimedParticle[i].m_bTraceUnit )
		{
			ofs << "\t\t\t TRACE 		= TRUE, " << endl;
		}
		else
		{
			ofs << "\t\t\t TRACE 		= FALSE, " << endl;
		}

		if( 0 != m_vecTimedParticle[i].m_wstrBoneName.length() )
		{
			ConvertWCHARToChar( strname, m_vecTimedParticle[i].m_wstrBoneName.c_str() );
			ofs << "\t\t\t BONE_NAME	= \"" << strname << "\"," << endl;
		}



		ofs << "\t\t}," << endl;
	}


	ofs << "\t }, " << endl;





	ofs << "\t CAMERA_SHAKE = " << endl;
	ofs << "\t { " << endl;


	for( UINT i=0; i<m_vecCameraShake.size(); i++ )
	{
		ofs << "\t\t{" << endl;

		CameraShake& cameraShake = m_vecCameraShake[i];

		ofs << "\t\t\t START_TIME	= " << cameraShake.m_fStartTime << ", " << endl;
		ofs << "\t\t\t AMPLITUDE	= " << cameraShake.m_fAmplitude << ", " << endl;
		ofs << "\t\t\t DURATION		= " << cameraShake.m_fDuration << ", " << endl;

		ofs << "\t\t }, " << endl;
	}

	ofs << "\t }, " << endl;


	ofs << "}" << endl;


	ofs.close();

	ConvertFileAnsiToUTF8( strFileName, strFileName );

	return hr;
}



HRESULT CX2WorldToolMain::LoadEffectSet( const WCHAR* pwszFileName )
{
	HRESULT hr = S_OK;

	WCHAR wszFilePathOnly[512] = L"";
	WCHAR wszFileNameOnly[256] = L"";
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, pwszFileName );


	ClearTimedXMeshPlayerList();
	ClearTimedParticleList();
	ClearCameraShakeList();


	CWorldToolDialog* pDialog	= GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL );
	CDXUTListBox* pListBox_MeshPlayer		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_LOADED );
	CDXUTListBox* pListBox_Particle			= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_PARTICLE_LOADED );

	CWorldToolDialog* pDialog2	= GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL2 );
	CDXUTListBox* pListBox_CameraShake		= pDialog2->GetListBox( CX2WorldToolMain::IDC_LIST_CAMERA_SHAKE_UP );

	

	if( NULL != wszFilePathOnly )
	{
		string strFilePathOnly;
		ConvertWCHARToChar( strFilePathOnly, wszFilePathOnly );
		g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory( strFilePathOnly );
	}


	//KLuaManager luaManager;

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );

	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( wszFileNameOnly ) )
		return false;
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"Enum.lua" ) )
		return false;
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, L"DungeonEnum.lua" ) )
		return false;
	if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaManager( &luaManager, wszFileNameOnly ) )
		return false;
	
	if( true == luaManager.BeginTable( "EFFECT_SET" ) )
	{

		if( true == luaManager.BeginTable( "MESH_PLAYER" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{

				TimedXMeshPlayer timedXMeshPlayer;
				LUA_GET_VALUE( luaManager, L"NAME",						timedXMeshPlayer.m_wstrXMeshPlayerName,			L"" );
				
				if( (luaManager).GetValue( L"DELAY_TIME", timedXMeshPlayer.m_fDelayTimeToStart  ) == false )
					LUA_GET_VALUE( luaManager, L"START_TIME",				timedXMeshPlayer.m_fDelayTimeToStart,			0.f );

				if( (luaManager).GetValue( L"POS_X", timedXMeshPlayer.m_vRelativePosition.x  ) == false )
					LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_X",		timedXMeshPlayer.m_vRelativePosition.x,			0.f );

				if( (luaManager).GetValue( L"POS_Y", timedXMeshPlayer.m_vRelativePosition.y  ) == false )
					LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_Y",		timedXMeshPlayer.m_vRelativePosition.y,			0.f );

				if( (luaManager).GetValue( L"POS_Z", timedXMeshPlayer.m_vRelativePosition.z  ) == false )
					LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_Z",		timedXMeshPlayer.m_vRelativePosition.z,			0.f );

				LUA_GET_VALUE( luaManager, L"TRACE",					timedXMeshPlayer.m_bTraceUnit,					false );
				LUA_GET_VALUE( luaManager, L"BONE_NAME",				timedXMeshPlayer.m_wstrBoneName,				L"" );
				//LUA_GET_VALUE( luaManager, L"POS_RELATIVE_TO_UNIT",		timedXMeshPlayer.m_bRelativeToUnit,		true );


				m_vecTimedXMeshPlayer.push_back( timedXMeshPlayer );

				pListBox_MeshPlayer->AddItem( timedXMeshPlayer.m_wstrXMeshPlayerName.c_str(), NULL );


				iTableIndex++;
				luaManager.EndTable();
			}
			luaManager.EndTable(); // MESH_PLAYER
		}



		if( true == luaManager.BeginTable( "PARTICLE" ) )
		{
			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{

				TimedParticle timedParticle;
				LUA_GET_VALUE( luaManager, L"NAME",						timedParticle.m_wstrParticleName,				L"" );

				if( (luaManager).GetValue( L"DELAY_TIME", timedParticle.m_fDelayTimeToStart  ) == false )
					LUA_GET_VALUE( luaManager, L"START_TIME",				timedParticle.m_fDelayTimeToStart,			0.f );

				if( (luaManager).GetValue( L"POS_X", timedParticle.m_vRelativePosition.x  ) == false )
					LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_X",		timedParticle.m_vRelativePosition.x,			0.f );

				if( (luaManager).GetValue( L"POS_Y", timedParticle.m_vRelativePosition.y  ) == false )
					LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_Y",		timedParticle.m_vRelativePosition.y,			0.f );

				if( (luaManager).GetValue( L"POS_Z", timedParticle.m_vRelativePosition.z  ) == false )
					LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_Z",		timedParticle.m_vRelativePosition.z,			0.f );

				/*LUA_GET_VALUE( luaManager, L"START_TIME",				timedParticle.m_fDelayTimeToStart,				0.f );
				LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_X",					timedParticle.m_vRelativePosition.x,			0.f );
				LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_Y",					timedParticle.m_vRelativePosition.y,			0.f );
				LUA_GET_VALUE( luaManager, L"POSITION_OFFSET_Z",					timedParticle.m_vRelativePosition.z,			0.f );*/

				LUA_GET_VALUE( luaManager, L"TRACE",					timedParticle.m_bTraceUnit,						false );
				LUA_GET_VALUE( luaManager, L"BONE_NAME",				timedParticle.m_wstrBoneName,					L"" );
				LUA_GET_VALUE( luaManager, L"POS_RELATIVE_TO_UNIT",		timedParticle.m_bRelativeToUnit,		true );

				m_vecTimedParticle.push_back( timedParticle );


				pListBox_Particle->AddItem( timedParticle.m_wstrParticleName.c_str(), NULL );

				iTableIndex++;
				luaManager.EndTable();
			}
			luaManager.EndTable(); // PARTICLE
		}

		if( true == luaManager.BeginTable( L"CAMERA_SHAKE" ) )
		{

			WCHAR wszText[256] = L"";

			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{

				CameraShake cameraShake;

				if( (luaManager).GetValue( L"DELAY_TIME", cameraShake.m_fStartTime  ) == false )
					LUA_GET_VALUE( luaManager, L"START_TIME",		cameraShake.m_fStartTime,			-1.f );

				LUA_GET_VALUE( luaManager, L"AMPLITUDE",		cameraShake.m_fAmplitude,			0.f );
				LUA_GET_VALUE( luaManager, L"DURATION",			cameraShake.m_fDuration,			0.f );

				m_vecCameraShake.push_back( cameraShake );

				swprintf( wszText, sizeof(wszText)/sizeof(WCHAR)-1, L"%4.2f, %4.2f, %4.2f", 
					cameraShake.m_fStartTime,
					cameraShake.m_fAmplitude,
					cameraShake.m_fDuration );


				pListBox_CameraShake->AddItem( wszText, NULL );

				iTableIndex++;
				luaManager.EndTable();
			}
			luaManager.EndTable(); // CAMERA_CRASH
		}
		
		luaManager.EndTable(); // EFFECT_SET
	}

	pListBox_MeshPlayer->SelectItem( pListBox_MeshPlayer->GetSize()-1 );
	pListBox_Particle->SelectItem( pListBox_Particle->GetSize()-1 );
	pListBox_CameraShake->SelectItem( pListBox_CameraShake->GetSize()-1 );

	UpdatePickedXMeshPlayerUI();
	UpdatePickedParticleUI();
	UpdatePickedCameraShakeUI();

	return hr;
}


void CX2WorldToolMain::CreateCharacterSkinMesh( const WCHAR* wszCharacterName )
{
	SAFE_DELETE( m_pCharacterSkinAnim );
	m_pCharacterSkinAnim = new CSkinAnimObject;

	const wstring MAGIC_ELSWORD = L"ELSWORD";
	const wstring MAGIC_ARME	= L"ARME";
	const wstring MAGIC_LIRE	= L"LIRE";
	const wstring MAGIC_RAVEN	= L"RAVEN";

	const wstring MAGIC_EVE		= L"EVE";


	wstring MAGIC_ELSWORD_SKINMESH	= L"Motion_Elsword_SwordMan.X";
	wstring MAGIC_ARME_SKINMESH		= L"Motion_Arme_VioletMage.X";
	wstring MAGIC_LIRE_SKINMESH		= L"Motion_Lire_ElvenRanger.X";
	wstring MAGIC_RAVEN_SKINMESH	= L"Motion_Raven.X";

	wstring MAGIC_EVE_SKINMESH		= L"Motion_Eve.X";



	if( MAGIC_ELSWORD.compare( wszCharacterName ) == 0 )
	{
		m_pCharacterSkinAnim->SetAnimXSkinMesh( MAGIC_ELSWORD_SKINMESH );
	}
	else if( MAGIC_ARME.compare( wszCharacterName ) == 0 )
	{
		m_pCharacterSkinAnim->SetAnimXSkinMesh( MAGIC_ARME_SKINMESH );
	}
	else if( MAGIC_LIRE.compare( wszCharacterName ) == 0 )
	{
		m_pCharacterSkinAnim->SetAnimXSkinMesh( MAGIC_LIRE_SKINMESH );
	}	
	else if( MAGIC_RAVEN.compare(wszCharacterName) == 0 )
	{
		m_pCharacterSkinAnim->SetAnimXSkinMesh( MAGIC_RAVEN_SKINMESH );
	}

	else if( MAGIC_EVE.compare(wszCharacterName) == 0 )
	{
		m_pCharacterSkinAnim->SetAnimXSkinMesh( MAGIC_EVE_SKINMESH );
	}

	
	
	m_pCharacterSkinAnim->ChangeAnim( L"Wait" );
	m_pCharacterSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );

	if( true == m_bFixPostionCharacterSkinAnim )
	{
		m_pCharacterSkinAnim->SetPos( m_vFixedCharacterSkinAnimPosition );
		m_pCharacterSkinAnim->SetRotationDegree( m_vFixedCharacterSkinAnimRotation );

	}
	else
	{
		m_pCharacterSkinAnim->SetPos( m_pGame->GetMyUnit()->GetPos() );
	}
	m_pCharacterSkinAnim->SetShow( m_bShowCharacterSkinAnim );


	
	//{{ 09.03.03 김태완 : 렌더링 파라미터 지정
	m_pCharacterSkinAnim->GetRenderParam()->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	m_pCharacterSkinAnim->GetRenderParam()->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
	m_pCharacterSkinAnim->GetRenderParam()->fOutLineWide	= 1.5f;
	if(g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL)
	{
		m_pCharacterSkinAnim->GetRenderParam()->lightPos		= g_pX2Game->GetWorld()->GetLightPos();
	}	
	//}}




	// listbox 갱신
	CDXUTListBox* pListBox = m_vecpDialog[DLG_EFFECT_TOOL]->GetListBox( IDC_LIST_SKIN_ANIM_NAME );
	pListBox->RemoveAllItems();

	vector<wstring> vecAnimName;
	m_pCharacterSkinAnim->GetAnimNameList( vecAnimName );

	for( UINT i=0; i<vecAnimName.size(); i++ )
	{
		pListBox->AddItem( vecAnimName[i].c_str(), NULL );
	}


}



void CX2WorldToolMain::CreateCharacterSkinMeshFromFile( const WCHAR* wszFileName )
{
	// device manager에 있는 데이터를 clear
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedXMeshes();
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedTextures();
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedXSkinMeshes();


	WCHAR wszFilePathOnly[512];
	WCHAR wszFileNameOnly[256];
	DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, wszFileName );


	SAFE_DELETE( m_pCharacterSkinAnim );
	m_pCharacterSkinAnim = new CSkinAnimObject;

	m_pCharacterSkinAnim->SetAnimXSkinMesh( wszFileNameOnly );

	m_pCharacterSkinAnim->ChangeAnim( L"Wait" );
	m_pCharacterSkinAnim->Play( CKTDGXSkinAnim::XAP_LOOP );

	if( true == m_bFixPostionCharacterSkinAnim )
	{
		m_pCharacterSkinAnim->SetPos( m_vFixedCharacterSkinAnimPosition );
		m_pCharacterSkinAnim->SetRotationDegree( m_vFixedCharacterSkinAnimRotation );
	}
	else
	{
		m_pCharacterSkinAnim->SetPos( m_pGame->GetMyUnit()->GetPos() );
	}


	m_pCharacterSkinAnim->SetShow( m_bShowCharacterSkinAnim );



	m_pCharacterSkinAnim->GetRenderParam()->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	m_pCharacterSkinAnim->GetRenderParam()->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
	m_pCharacterSkinAnim->GetRenderParam()->fOutLineWide	= 1.5f;
	if(g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL)
	{
		m_pCharacterSkinAnim->GetRenderParam()->lightPos		= g_pX2Game->GetWorld()->GetLightPos();
	}	





	// listbox 갱신
	CDXUTListBox* pListBox = m_vecpDialog[DLG_EFFECT_TOOL]->GetListBox( IDC_LIST_SKIN_ANIM_NAME );
	pListBox->RemoveAllItems();

	vector<wstring> vecAnimName;
	m_pCharacterSkinAnim->GetAnimNameList( vecAnimName );

	for( UINT i=0; i<vecAnimName.size(); i++ )
	{
		pListBox->AddItem( vecAnimName[i].c_str(), NULL );
	}
}



void CX2WorldToolMain::On_IDC_BUTTON_LOAD_XMESH_PLAYER()
{
	// device manager에 있는 데이터를 clear
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedXMeshes();
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedTextures();
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedXSkinMeshes();


	if( GetXMeshPlayer() != NULL )
	{
		ResetXMeshPlayer();
	}

	GetXMeshPlayer()->OpenScriptFile( L"XMeshPlayer_Test.txt" );


	map<wstring, CKTDGXMeshPlayer::XMeshTemplet*>& templetMap = GetXMeshPlayer()->GetTempletMap();
	CWorldToolDialog* pDialog	= GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL );
	CDXUTListBox* pListBox		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_XMESH_PLAYER_PALETTE );

	pListBox->RemoveAllItems();
	map<wstring, CKTDGXMeshPlayer::XMeshTemplet*>::iterator it;
	for( it=templetMap.begin(); it != templetMap.end(); it++ )
	{
		wstring& wstrName = (wstring)it->first;
		pListBox->AddItem( wstrName.c_str(), NULL );
	}



	// ready device reload 
	ReadyEffectDevices();

}


void CX2WorldToolMain::On_IDC_BUTTON_LOAD_PARTICLE()
{
	// device manager에 있는 데이터를 clear
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedXMeshes();
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedTextures();
// 	g_pKTDXApp->GetDeviceManager()->CloseCachedXSkinMeshes();


	if( m_pParticleSystem != NULL )
		SAFE_DELETE( m_pParticleSystem );


	m_pParticleSystem = new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );
	m_pParticleSystem->OpenScriptFile( L"Particle_Test.txt" );



	const map<wstring, CKTDGParticleSystem::CParticleEventSequence*>& templetSeq = m_pParticleSystem->GetTempletSequences();
	
	CWorldToolDialog* pDialog	= GetDialog( CX2WorldToolMain::DLG_EFFECT_TOOL );
	CDXUTListBox* pListBox		= pDialog->GetListBox( CX2WorldToolMain::IDC_LIST_PARTICLE_PALETTE );

	pListBox->RemoveAllItems();
	map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
	for( it=templetSeq.begin(); it != templetSeq.end(); it++ )
	{
		wstring& wstrName = (wstring)it->first;
		pListBox->AddItem( wstrName.c_str(), NULL );
	}

	// ready device reload 
	ReadyEffectDevices();
}






void CX2WorldToolMain::ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount )
{
#define IN_INT_CHARSET( c ) \
	( (c) == L'-' || ( (c) >= L'0' && (c) <= L'9' ) )


	const WCHAR *pToken, *pEnd;
	WCHAR *pStop = NULL;

	pToken = pEditBox->GetText();
	pEnd = pToken + pEditBox->GetTextLength();

	for( int i=0; i<nCount; i++ )
	{
		*pNumbers = (int) wcstol( pToken, &pStop, 10 );
		pNumbers++;
		pToken = pStop;

		while( !IN_INT_CHARSET( *pToken ) )
		{
			if( *pToken == L'\0' || *pToken == *pEnd )
				break;

			++pToken;
		}

		if( *pToken == L'\0' || *pStop == L'\0' || *pToken == *pEnd )
			break;
	}
}


void CX2WorldToolMain::SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount )
{
	WCHAR wszBuffer[512] = {0};
	WCHAR wszTmp[64];

	if( pNumbers == NULL )
		return;

	for( int i = 0; i < nCount; ++i )
	{
		StringCchPrintf( wszTmp, 64, L"%d ", pNumbers[i] );
		StringCchCat( wszBuffer, 512, wszTmp );
	}

	// Don't want the last space
	if( nCount > 0 && wcslen(wszBuffer) > 0 )
		wszBuffer[wcslen(wszBuffer)-1] = 0;

	pEditBox->SetText( wszBuffer );
}


void CX2WorldToolMain::PostProcLoadWorld()
{
	for( UINT i=0; i<m_vecpWorldToolObject.size(); i++ )
	{
		CWorldToolObject* pObject = m_vecpWorldToolObject[i];

		CKTDGXRenderer::RenderParam* pRenderParam = pObject->GetRenderParam();
		if( NULL != pRenderParam )
		{
			pObject->SetIsAlphaObject( pRenderParam->bAlphaBlend );

			if( pRenderParam->srcBlend == D3DBLEND_SRCALPHA && pRenderParam->destBlend == D3DBLEND_DESTALPHA )
			{
				pObject->SetAdditiveBlend( true );
			}
			else if( pRenderParam->srcBlend == D3DBLEND_SRCALPHA && pRenderParam->destBlend == D3DBLEND_INVSRCALPHA )
			{
				pObject->SetAdditiveBlend( false );
			}

#ifdef ADD_ALPHATESTENABLE
			if( pRenderParam->bAlphaTestEnable )
			{
				pObject->SetIsAlphaTest( pRenderParam->bAlphaTestEnable );
			}
#endif

            if(pRenderParam->bFog)
            {
                m_bFog = true;
                SetFogColor(pRenderParam->fogColor.r, pRenderParam->fogColor.g, pRenderParam->fogColor.b);
            }            
		}
	}
}


void CX2WorldToolMain::ReadyEffectDevices()
{
	ClearAllDevice();
	//m_DeviceReady.DeviceUnreadyAll();

	if( NULL == m_pParticleSystem )
		return;


	for( UINT i=0; i< m_vecTimedParticle.size(); i++ )
	{
		const TimedParticle& timedParticle = m_vecTimedParticle[i];
		const CKTDGParticleSystem::CParticleEventSequence* pParticleTemplet = 
			m_pParticleSystem->GetTempletSequencesByName( timedParticle.m_wstrParticleName );

		if( NULL == pParticleTemplet )
			continue;

		CKTDGParticleSystem::CParticleEvent* pEvent = NULL;
		CKTDXDeviceTexture* pTexture = NULL;
						
		BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticleEvent*, pEvent, pParticleTemplet->m_EventList )
		{
			switch( pEvent->GetEventType() )
			{
			case CKTDGParticleSystem::ET_TEXTURE:
				{
					CKTDGParticleSystem::CParticleEvent_Texture* pEventTexture = (CKTDGParticleSystem::CParticleEvent_Texture*) pEvent;
					TextureReady( pEventTexture->GetTextureName() );
				}
				break;
			}
		}
		

		/*for( UINT j = 0; j < pParticleTemplet->m_Events.size(); j++ )
		{
			CKTDGParticleSystem::CParticleEvent* pEvent = (CKTDGParticleSystem::CParticleEvent*) pParticleTemplet->m_Events[j];
			
			
			switch( pEvent->GetEventType() )
			{
			case CKTDGParticleSystem::ET_TEXTURE:
				{
					CKTDGParticleSystem::CParticleEvent_Texture* pEventTexture = (CKTDGParticleSystem::CParticleEvent_Texture*) pEvent;
					//m_DeviceReady.TextureReady( pEventTexture->GetTextureName() );
					TextureReady( pEventTexture->GetTextureName() );
				}
				break;
			}
		}*/
	}


	if( NULL == m_pXMeshPlayer )
		return;

	for( UINT i=0; i< m_vecTimedXMeshPlayer.size(); i++ )
	{
		const TimedXMeshPlayer& timedXMeshPlayer = m_vecTimedXMeshPlayer[i];
		const CKTDGXMeshPlayer::XMeshTemplet* pTemplet = m_pXMeshPlayer->GetTempletByName( timedXMeshPlayer.m_wstrXMeshPlayerName );
		if( NULL == pTemplet )
			continue;

		switch( pTemplet->meshType )
		{
		case CKTDGXMeshPlayer::X_MESH:
			{
				//m_DeviceReady.XMeshReady( pTemplet->meshName.c_str() );
				XMeshReady( pTemplet->meshName.c_str() );
			} break;

		case CKTDGXMeshPlayer::X_SKIN_MESH:
			{
				//m_DeviceReady.XSkinMeshReady( pTemplet->meshName.c_str() );
				XSkinMeshReady( pTemplet->meshName.c_str() );
			} break;

		}
	}


}


void CX2WorldToolMain::UpdateMonsterList()
{
	CWorldToolDialog* pDialog		= m_vecpDialog[DLG_ETC];

	CDXUTListBox* pListBox_Monster	= pDialog->GetListBox( IDC_LIST_MONSTER );
	pListBox_Monster->RemoveAllItems();

	//map<CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NPCUnitTemplet*>::const_iterator it;
	//const map<CX2UnitManager::NPC_UNIT_ID, CX2UnitManager::NPCUnitTemplet*>& mapNPCTemplet = g_pData->GetUnitManager()->GetNPCUnitTempletMap();
	 map<CX2Unit::UNIT_CLASS, const CX2Unit::UnitTemplet*>::const_iterator it;
	const CX2UnitManager::UnitTempletMap& mapNPCTemplet = g_pData->GetUnitManager()->GetUnitTempletMap();

	for( it = mapNPCTemplet.begin() ; it != mapNPCTemplet.end(); it++ )
	{
		/*CX2UnitManager::NPCUnitTemplet* pNPCTemplet = it->second;

		if( pNPCTemplet->m_LuaFileName.length() != 0 )
		{
			pListBox_Monster->AddItem( pNPCTemplet->m_LuaFileName.c_str(), (void*) (int)it->first );
		}*/

		const CX2Unit::UnitTemplet* pNPCTemplet = it->second;

		if ( pNPCTemplet->m_MotionFile.length() != 0 )
			pListBox_Monster->AddItem( pNPCTemplet->m_MotionFile.c_str(), (void*) (int)it->first );
	}
}




// 라인맵의 line start, end pos 좌표를 float에서 int로 바꾼다.
void CX2WorldToolMain::LineMapFloatToInt()
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( NULL == pLineMap )
		return;

	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);

		pLineData->startPos.x = (float)(long)pLineData->startPos.x;
		pLineData->startPos.y = (float)(long)pLineData->startPos.y;
		pLineData->startPos.z = (float)(long)pLineData->startPos.z;

		pLineData->endPos.x = (float)(long)pLineData->endPos.x;
		pLineData->endPos.y = (float)(long)pLineData->endPos.y;
		pLineData->endPos.z = (float)(long)pLineData->endPos.z;
	}
}

// start pos의 line index를 새로 모두 계산
void CX2WorldToolMain::RecomputeStartPosLineIndex()
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( pLineMap == NULL )
		return;

	for( UINT i=0; i<vecTeamStartPos.size(); i++ )
	{
		STeamStartPos& sp = vecTeamStartPos.at(i);
		sp.vPos = pLineMap->GetLandPosition( sp.vPos, 30.f, &sp.iLineIndex );
	}
}

float CX2WorldToolMain::ComputeLandHeight()
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( NULL == pLineMap )
		return 0.f;

	float fLandHeight = pLineMap->GetLineData(0)->startPos.y;


	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{		
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);

		if( pLineData->lineType == CKTDGLineMap::LT_BUNGEE ||
			pLineData->lineType == CKTDGLineMap::LT_MONSTER_ROAD ||
			pLineData->lineType == CKTDGLineMap::LT_WALL )
			continue;

		if( pLineData->startPos.y < fLandHeight )
			fLandHeight = pLineData->startPos.y;
		if( pLineData->endPos.y < fLandHeight )
			fLandHeight = pLineData->endPos.y;
	}

	return fLandHeight;

}

void CX2WorldToolMain::LoadLineMapInfo()
{
	DeleteAllSkinMeshAndSkinAnim();

	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( NULL == pLineMap )
		return;

	if( pLineMap != NULL )
	{
		WCHAR wszOutput[4096];

		for(int i=0; i<sizeof(m_abKeyUsed); i++ )
		{
			m_abKeyUsed[i] = false;
		}

		vecTeamStartPos.clear();
		int iTempInitialDir;
		for(int i=0; i<pLineMap->GetRedTeamStartPosNum(); i++ )
		{
			if( pLineMap->GetRedTeamStartRight(i) == true )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;

			int iLineIndex = pLineMap->GetRedTeamStartLineIndex(i);

			vecTeamStartPos.push_back( STeamStartPos( -1, TEAM_RED, pLineMap->GetRedTeamStartPosition(i), iTempInitialDir, iLineIndex) );
		}

		for(int i=0; i<pLineMap->GetBlueTeamStartPosNum(); i++ )
		{
			if( pLineMap->GetBlueTeamStartRight(i) == true )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;

			int iLineIndex = pLineMap->GetBlueTeamStartLineIndex(i);

			vecTeamStartPos.push_back( STeamStartPos( -1, TEAM_BLUE, pLineMap->GetBlueTeamStartPosition(i), iTempInitialDir, iLineIndex) );
		}

#ifdef  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
		const CKTDGLineMap::StartInfo& mapStartInfo = pLineMap->GetStartInfoMap();
		CKTDGLineMap::StartInfo::const_iterator it;
		for( it=mapStartInfo.begin(); it!=mapStartInfo.end(); it++ )
		{
			if( true == it->second.m_bStartRight )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;
			m_abKeyUsed[ (int) it->first ] = true;
			vecTeamStartPos.push_back( STeamStartPos( (int)it->first, TEAM_NONE, 
                it->second.m_vStartPosition, iTempInitialDir, it->second.m_iStartLineIndex ) );
		}
#else   X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
		map<int, D3DXVECTOR3>& mapStartPos = pLineMap->GetStartPosMap();
		map<int, D3DXVECTOR3>::iterator it;
		for( it=mapStartPos.begin(); it!=mapStartPos.end(); it++ )
		{
			if( true == pLineMap->GetStartRight( (int)it->first ) )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;

			int iLineIndex = pLineMap->GetStartLineIndex( (int)it->first );

			m_abKeyUsed[ (int)it->first ] = true;
			vecTeamStartPos.push_back( STeamStartPos( (int)it->first, TEAM_NONE, pLineMap->GetStartPosition((int)it->first), iTempInitialDir, iLineIndex ) );
		}
#endif  X2OPTIMIZE_LINEMAP_OPTIMIZE_AND_BUG_FIX
	}

	CalcUnderWaterUnitColor(NULL, NULL , 0, NULL);
}

void CX2WorldToolMain::SaveLineMapToText()
{
	string strFilePath;
	SaveLineMapToFile( wszTempFilePath, false );
	ConvertWCHARToChar( strFilePath, wszTempFilePath );
	_spawnlp( _P_NOWAIT, "notepad.exe", "notepad.exe", strFilePath.c_str(), NULL );
}

bool CX2WorldToolMain::SaveLineMapToFile( const WCHAR* wszFileName, bool bUpdateFileName )
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( pLineMap == NULL )
		return false;

	bool bResult = true;

	LineMapFloatToInt();

	std::sort( vecTeamStartPos.begin(), vecTeamStartPos.end(), CTeamStartPosSort() );

	RecomputeStartPosLineIndex();

	// 파일 확장자가 없으면 붙여주자.
	std::string strFileName;	
	strFileName.reserve(512);
	ConvertWCHARToChar( strFileName, wszFileName );	
	MakeUpperCase( strFileName );

	int iStrLen = strFileName.length();
	if( 'L' != strFileName[iStrLen-3] || 'U' != strFileName[iStrLen-2] || 'A' != strFileName[iStrLen-1] ) 
	{
		ConvertWCHARToChar( strFileName, wszFileName );	
		strFileName.append(".lua");
	}
	else
	{
		ConvertWCHARToChar( strFileName, wszFileName );	
	}

	std::wstring wstrFilePath;
	ConvertCharToWCHAR( wstrFilePath, strFileName.c_str() );

	std::ofstream ofs( strFileName.c_str() );
	if( ofs.is_open() == false )
	{
		ofs.close();
		return false;
	}

	//if( true == bUpdateFileName )
	//	m_wstrLineMapFileName = wstrFilePath;

	ofs << std::endl << std::endl;
	//ofs << "g_pLineMap:SetLandHeight( " << pLineMap->GetLandHeight() << " )" << std::endl;
	float fLandHeight = pLineMap->GetLandHeight();
	ofs << "g_pLineMap:SetLandHeight( " << fLandHeight << " )" << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- RED, BLUE, TEAM START POS" << std::endl;

	// red, blue startpos을 출력하자
	string strDirection, strTeamColor;
	for( unsigned j=0; j<vecTeamStartPos.size(); j++ )
	{
		STeamStartPos& spos = vecTeamStartPos[j];

		if( spos.iInitialDir == DIR_LEFT )
			strDirection = "false";
		else 
			strDirection = "true";

		if( spos.iTeamColor == TEAM_RED )
			strTeamColor = "true";
		else if( spos.iTeamColor == TEAM_BLUE )
			strTeamColor = "false";

		if( spos.iTeamColor != TEAM_NONE )
		{
			ofs << "g_pLineMap:SetTeamStartPos( " << strTeamColor << ", D3DXVECTOR3( " 
				<< spos.vPos.x << ", " << spos.vPos.y << ", " << spos.vPos.z << " ), " 
				<< strDirection << ", " << spos.iLineIndex << " )" << std::endl;
		}
	}

	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- NPC (or else) START POS" << std::endl;
	for( unsigned j=0; j<vecTeamStartPos.size(); j++ )
	{
		STeamStartPos& spos = vecTeamStartPos[j];

		if( spos.iInitialDir == DIR_LEFT )
			strDirection = "false";
		else 
			strDirection = "true";

		if( spos.iTeamColor == TEAM_NONE )
		{
			ofs << "g_pLineMap:AddStartPos( "
				<< spos.key << ", D3DXVECTOR3( " 
				<< spos.vPos.x << ", " << spos.vPos.y << ", " << spos.vPos.z << " ), " 
				<< strDirection << ", " << spos.iLineIndex << " )" << std::endl;
		}
	}

	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << std::endl << std::endl;
	ofs << "-- LINE DATA -- " << std::endl;

	// 라인정보 출력
	CKTDGLineMap::LineData* pLineData = NULL;	
	std::string strFootDustName;
	strFootDustName.reserve(512);
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);

			//if( pLineData->lineSet < 0 )
			//	bThereAreInvalidLineSet = true;

		ofs << "g_pLineMap:AddLine_LUA" << std::endl;
		ofs << "{" << std::endl;
		ofs << "	LINE_NUM		= " << i << ", " << std::endl;
		if( true == pLineData->bOriEnable )
		{
			//ofs << "	ENABLE			= TRUE," << std::endl;
		}
		else
		{
			ofs << "	ENABLE			= FALSE," << std::endl;
		}

		ofs << "	LINE_TYPE 		= ";

		switch( pLineData->lineType )
		{
		default:
		case CKTDGLineMap::LT_NORMAL:
			ofs << "LINE_TYPE[\"LT_NORMAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_WALL:
			ofs << "LINE_TYPE[\"LT_WALL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_POTAL:
			ofs << "LINE_TYPE[\"LT_POTAL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_HOLE:
			ofs << "LINE_TYPE[\"LT_HOLE\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_JUMP_UP_REL:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_REL\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_JUMP_UP_ABS:
			ofs << "LINE_TYPE[\"LT_JUMP_UP_ABS\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_BUNGEE:
			ofs << "LINE_TYPE[\"LT_BUNGEE\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_MONSTER_ROAD:
			ofs << "LINE_TYPE[\"LT_MONSTER_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_UNIT_ROAD:
			ofs << "LINE_TYPE[\"LT_UNIT_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_OTHER_ROAD:
			ofs << "LINE_TYPE[\"LT_OTHER_ROAD\"], " << std::endl;
			break;
		case CKTDGLineMap::LT_CLIFF:
			ofs << "LINE_TYPE[\"LT_CLIFF\"], " << std::endl;
			break;			
		}

		ofs << "	LINEMAP_DIRECTION_OPTION	= ";
		switch( pLineData->m_eDirection )
		{
		case CKTDGLineMap::LDO_NONE:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_NONE\"], " << std::endl;
			break;

		case CKTDGLineMap::LDO_LEFT:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_LEFT\"], " << std::endl;
			break;

		case CKTDGLineMap::LDO_RIGHT:
			ofs << "LINEMAP_DIRECTION_OPTION[\"LDO_RIGHT\"], " << std::endl;
			break;
		}

		ofs << "	JUMP_REL_UP_KEYSTOP_TIME	= " << pLineData->m_fKeyStopTime << ", " << std::endl;

		if( -1 != pLineData->potalGroup )
		{
			ofs << "	POTAL_GROUP		= " << pLineData->potalGroup << ", " << std::endl;
		}

		if( 1.f != pLineData->fStopFactor )
		{
			ofs << "	STOP_FACTOR		= " << pLineData->fStopFactor << ", " << std::endl;
		}


		switch( pLineData->eDustType )
		{
		default:
		case CKTDGLineMap::LDT_DEFALT_DUST:
			{
				//ofs << "	DUST_TYPE   = LINE_DUST_TYPE[\"LDT_DEFALT_DUST\"]," << std::endl;
			} break;

		case CKTDGLineMap::LDT_WATER_SPLASH:
			{
				ofs << "    DUST_TYPE   = LINE_DUST_TYPE[\"LDT_WATER_SPLASH\"]," << std::endl;
			} break;
		}

		ofs << "	BEFORE_LINE		= " << pLineData->beforeLine	<< ", " << std::endl;
		ofs << "	NEXT_LINE		= " << pLineData->nextLine		<< ", " << std::endl;
		ofs << "	LINE_SET		= " << pLineData->lineSet		<< ", " << std::endl;

		if( 0.f != pLineData->m_fOriSpeed )
		{
			ofs << "	SPEED			= " << pLineData->m_fOriSpeed << ", " << std::endl;
		}


		if( 0.f != pLineData->m_vOriJumpSpeed.x )
		{
			ofs << "	JUMP_SPEED_X			= " << pLineData->m_vOriJumpSpeed.x << ", " << std::endl;
		}


		if( 0.f != pLineData->m_vOriJumpSpeed.y )
		{
			ofs << "	JUMP_SPEED_Y			= " << pLineData->m_vOriJumpSpeed.y << ", " << std::endl;
		}


		if( false == pLineData->bCanDown )
		{
			ofs << "	CAN_DOWN		= FALSE, " << std::endl;
		}

		if( false == pLineData->m_bOriCanPass )
		{
			ofs << "	CAN_PASS		= FALSE, " << std::endl;
		}

		if( -1 != pLineData->m_iOriTeleportLineIndex )
		{
			ofs << "	TELEPORT_LINE		= " << pLineData->m_iTeleportLineIndex << ", " << std::endl;
		}

		if( true == pLineData->m_bEnableWallJump )
		{
			ofs << "	ENABLE_WALL_JUMP		= TRUE, " << std::endl;
		}

		if( false == pLineData->m_bMonsterAllowed )
		{
			ofs << "	MONSTER_ALLOWED		= FALSE, " << std::endl;
		}

		if( pLineData->m_iOriCameraIndex != -1 )
		{
			ofs << "	CAMERA_INDEX	= " << pLineData->m_iCameraIndex << ", " << std::endl;
		}
		if( pLineData->m_iTriggerId != -1 )
		{
			ofs << "	TRIGGER_ID		= " << pLineData->m_iTriggerId << ", " << std::endl;
			if( pLineData->m_bFootOnLine )
			{
				ofs << "	TRIGGER_ON_LINE	= TRUE," << std::endl;
			}
			else
			{
				ofs << "	TRIGGER_ON_LINE	= FALSE," << std::endl;
			}
			
		}

		ofs << "	\"START_POS		= D3DXVECTOR3(" << pLineData->startPos.x << ", " << pLineData->startPos.y << ", " << pLineData->startPos.z << ")\", " << std::endl;
		ofs << "	\"END_POS		= D3DXVECTOR3(" << pLineData->endPos.x << ", " << pLineData->endPos.y << ", " << pLineData->endPos.z << ")\", " << std::endl;

#ifdef UNDERWATER_LINEMAP
		if( true == pLineData->m_bUnderWater )
		{
			ofs << "	IS_UNDERWATER			= TRUE," << std::endl;
			ofs << "	UNDERWATER_HEIGHT		= " << pLineData->m_fWaterHeight << ", " << std::endl;
			ofs << "	UNDERWATER_BUOYANCY		= " << pLineData->m_fBuoyancy << ", " << std::endl;
			ofs << "	UNDERWATER_RESISTANCE	= " << pLineData->m_fResistance << ", " << std::endl;
			ofs << "	UNDERWATER_ANIMSPEED	= " << pLineData->m_fAnimSpeed << ", " << std::endl;
			ofs << "	UNDERWATER_VARIANCEOXYEN= " << pLineData->m_fVarianceOxyen << ", " << std::endl;			
		}		
		if( pLineData->m_bForceChangeColor == true )
		{
			ofs << "	FORCE_CHANGE_COLOR		= TRUE," << std::endl;
		}

		if( pLineData->m_bUnderWater == true || pLineData->m_bForceChangeColor == true )
		{
			ofs << "	UNIT_COLOR_R			= " << pLineData->m_cLineColor.r << ", " << std::endl;
			ofs << "	UNIT_COLOR_G			= " << pLineData->m_cLineColor.g << ", " << std::endl;
			ofs << "	UNIT_COLOR_B			= " << pLineData->m_cLineColor.b << ", " << std::endl;
		}		
#endif

		ofs << "}" << std::endl;
		ofs << std::endl;
	}

	// 카메라 정보 출력
	ofs << std::endl;
	ofs << "-- CAMERA DATA " << std::endl;
	ofs << std::endl;

	CKTDGLineMap::CameraData * pCameraData = NULL;	

	for( int i = 0; i < pLineMap->GetNumCameraData(); ++i )
	{
		pCameraData = pLineMap->GetCameraDataListByTool(i);

		ofs << "g_pLineMap:AddCameraData_LUA" << std::endl;
		ofs << "{" << std::endl;


		//{{ 내용출력 시작.		
		if( 0.f == pCameraData->m_vRelativeEye.x &&
			0.f == pCameraData->m_vRelativeEye.y &&
			0.f == pCameraData->m_vRelativeEye.z )
		//if( pCameraData->m_eType == CKTDGLineMap::LCT_ABSOULTE_POSITION )
		{
			if( pCameraData->m_bFocusUnit == false )
			{
				ofs << "	FOCUS_UNIT	= FALSE," << std::endl;
			}
			ofs << "	\"EYE_POS	= D3DXVECTOR3(" << pCameraData->m_vEye.x << ", " << pCameraData->m_vEye.y << ", " << pCameraData->m_vEye.z << ")\", " << std::endl;
			ofs << "	\"LOOKAT_POS	= D3DXVECTOR3(" << pCameraData->m_vLookAt.x << ", " << pCameraData->m_vLookAt.y << ", " << pCameraData->m_vLookAt.z << ")\", " << std::endl;
		}
		else //if( pCameraData->m_eType == CKTDGLineMap::LCT_RELATIE_POSITION )
		{
			ofs << "	RELATIVE_EYE_POSITION_OFFSET_X	= " << pCameraData->m_vRelativeEye.x << ", " << std::endl;
			ofs << "	RELATIVE_EYE_POSITION_OFFSET_Y	= " << pCameraData->m_vRelativeEye.y << ", " << std::endl;
			ofs << "	RELATIVE_EYE_POSITION_OFFSET_Z	= " << pCameraData->m_vRelativeEye.z << ", " << std::endl;

			ofs << "	CAMERA_REPOSITION_SPEED = " << pCameraData->m_fCameraRepositionSpeed << ", " << std::endl;
		}
		//}} 내용출력 끝.
		ofs << "}" << std::endl;
		ofs << std::endl;
	}

	// 트리거 정보 출력
	CKTDGLineMap::LINEMAP_TRIGGER *pTrigger = pLineMap->GetLineMapTrigger();
	for(UINT iTrigger=0; iTrigger<pTrigger->m_vecTriggerSeg.size(); ++iTrigger)
	{
		CKTDGLineMap::TRIGGER_SEGMENT &triggerSeg = pTrigger->m_vecTriggerSeg[iTrigger];

		ofs << "g_pLineMap:AddTrigger_LUA" <<std::endl;
		ofs << "{" << std::endl;
		
		ofs << "	TRIGGER_ID		= " << iTrigger << "," << std::endl;		
		if( triggerSeg.m_bLoop )		
			ofs << "	TRIGGER_LOOP	= TRUE," << std::endl;
		else
			ofs << "	TRIGGER_LOOP	= FALSE," << std::endl;

		for(UINT iData=0; iData<triggerSeg.m_vecTriggerData.size(); ++iData)
		{
			CKTDGLineMap::TRIGGER_DATA &triggerData = triggerSeg.m_vecTriggerData[iData];

			ofs << "	TRIGGER_DATA" << iData << " =" << std::endl;
			ofs << "	{" << std::endl;

			for(UINT iMesh=0; iMesh<triggerData.m_vecTriggerMesh.size(); ++iMesh)
			{
				CKTDGLineMap::TRIGGER_DATA_MESH &triggerMesh = triggerData.m_vecTriggerMesh[iMesh];

				ofs << "		TRIGGER_DATA_MESH" << iMesh << " =" << std::endl;
				ofs << "		{" << std::endl;

				switch(triggerMesh.m_iMeshTriggerAction)
				{
				case CKTDGLineMap::TAT_MESH_NONE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_MESH_ACTION_TYPE[\"TAT_MESH_NONE\"]," << std::endl;
						ofs << "			START_TIME = " << triggerMesh.m_fTriggerTime << "," << std::endl;						
					}					
					break;
				case CKTDGLineMap::TAT_MESH_CHANGE_ANIM:
					{
						std::string strMeshName, strAnimName;
						ConvertWCHARToChar( strMeshName, triggerMesh.m_wstrMeshName );
						ConvertWCHARToChar( strAnimName, triggerMesh.m_wstrName );	

						ofs << "			TRIGGER_ACTION = TRIGGER_MESH_ACTION_TYPE[\"TAT_MESH_CHANGE_ANIM\"]," << std::endl;
						ofs << "			START_TIME = " << triggerMesh.m_fTriggerTime << "," << std::endl;
						ofs << "			MESH_NAME = \"" << strMeshName << "\"," << std::endl;
						ofs << "			ANIM_NAME = \"" << strAnimName << "\"," << std::endl;
						ofs << "			PLAY_TYPE = " << triggerMesh.m_iPlayType << "," << std::endl;
					}					
					break;
				case CKTDGLineMap::TAT_MESH_CHANGE_SPEED:
					{
						std::string strMeshName;
						ConvertWCHARToChar( strMeshName, triggerMesh.m_wstrMeshName );

						ofs << "			TRIGGER_ACTION = TRIGGER_MESH_ACTION_TYPE[\"TAT_MESH_CHANGE_SPEED\"]," << std::endl;
						ofs << "			START_TIME = " << triggerMesh.m_fTriggerTime << "," << std::endl;
						ofs << "			MESH_NAME = \"" << strMeshName << "\"," << std::endl;
						ofs << "			ANIM_SPEED = " << triggerMesh.m_fAnimSpeed << "," << std::endl;
					}					
					break;
				default:					
					break;
				}
				ofs << "		}," << std::endl;
			}

			for(UINT iLine=0; iLine<triggerData.m_vecTriggerLine.size(); ++iLine)
			{
				CKTDGLineMap::TRIGGER_DATA_LINE &triggerLine = triggerData.m_vecTriggerLine[iLine];

				ofs << "		TRIGGER_DATA_LINE" << iLine << " =" << std::endl;
				ofs << "		{" << std::endl;

				switch(triggerLine.m_iLineTriggerAction)
				{
				case CKTDGLineMap::TAT_LINE_NONE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_NONE\"]," << std::endl;
						ofs << "			START_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_ENABLE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_ENABLE\"]," << std::endl;
						ofs << "			START_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						if( triggerLine.m_bEnable )
							ofs << "			LINE_ENABLE	= TRUE," << std::endl;
						else
							ofs << "			LINE_ENABLE	= FALSE," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_SPEED:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_SPEED\"]," << std::endl;
						ofs << "			START_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_SPEED	= " << triggerLine.m_fSpeed << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_JUMPSPEED:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_JUMPSPEED\"]," << std::endl;
						ofs << "			START_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_JUMPSPEED_X	= " << triggerLine.m_vJumpSpeed.x << "," << std::endl;
						ofs << "			LINE_JUMPSPEED_Y	= " << triggerLine.m_vJumpSpeed.y << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_CAMERA:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_CAMERA\"]," << std::endl;
						ofs << "			START_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_CAMERA	= " << triggerLine.m_iCamera << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_CANPASS:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_CANPASS\"]," << std::endl;
						ofs << "			START_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						if( triggerLine.m_bEnable )
							ofs << "			LINE_CANPASS	= TRUE," << std::endl;
						else
							ofs << "			LINE_CANPASS	= FALSE," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_LINE_CHANGE_TELEPORT:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_LINE_ACTION_TYPE[\"TAT_LINE_CHANGE_TELEPORT\"]," << std::endl;
						ofs << "			START_TIME = " << triggerLine.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_ID	= " << triggerLine.m_iTriggerLineIndex << "," << std::endl;
						ofs << "			LINE_TELEPORT	= " << triggerLine.m_iCamera << "," << std::endl;
					}
					break;
				default:					
					break;
				}
				ofs << "		}," << std::endl;
			}

			for(UINT iEtc=0; iEtc<triggerData.m_vecTriggerEtc.size(); ++iEtc)
			{
				CKTDGLineMap::TRIGGER_DATA_ETC &triggerEtc = triggerData.m_vecTriggerEtc[iEtc];

				ofs << "		TRIGGER_DATA_ETC" << iEtc << " =" << std::endl;
				ofs << "		{" << std::endl;

				switch(triggerEtc.m_iEtcTriggerAction)
				{
				case CKTDGLineMap::TAT_ETC_NONE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_NONE\"]," << std::endl;
						ofs << "			START_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_PLAY_SOUND:
					{
						std::string strActionName;							
						ConvertWCHARToChar( strActionName, triggerEtc.m_wstrActionName );

						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_PLAY_SOUND\"]," << std::endl;
						ofs << "			START_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			LINE_SOUND = \"" << strActionName << "\"," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_DURATION_TIME:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_DURATION_TIME\"]," << std::endl;
						ofs << "			START_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_CAMERA:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_CAMERA\"]," << std::endl;
						ofs << "			START_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			CAMERA_INDEX = " << triggerEtc.m_iDummyValue01 << "," << std::endl;
						ofs << "			CAMERA_DURATION = " << triggerEtc.m_fDummyValue01 << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_CAMERA_SHAKE:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_CAMERA_SHAKE\"]," << std::endl;
						ofs << "			START_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			CAMERA_SHAKE_GAP = " << triggerEtc.m_iDummyValue01 << "," << std::endl;
						ofs << "			CAMERA_SHAKE_DURATION = " << triggerEtc.m_fDummyValue01 << "," << std::endl;
					}
					break;
				case CKTDGLineMap::TAT_ETC_ACTIVE_LAYER:
					{
						ofs << "			TRIGGER_ACTION = TRIGGER_ETC_ACTION_TYPE[\"TAT_ETC_ACTIVE_LAYER\"]," << std::endl;
						ofs << "			START_TIME = " << triggerEtc.m_fTriggerTime << "," << std::endl;
						ofs << "			ACTIVE_LAYER_ID = " << triggerEtc.m_iDummyValue01 << "," << std::endl;
						ofs << "			FADE_TIME = " << triggerEtc.m_fDummyValue01 << "," << std::endl;						
					}
					break;
				default:					
					break;
				}
				ofs << "		}," << std::endl;
			}

			ofs << "	}," << std::endl;
		}

		ofs << "}" << std::endl;
	}



	ofs.close();

	ConvertFullPathFileAnsiToUTF8( strFileName, strFileName );
	
	return bResult;

}

void CX2WorldToolMain::AddNewStartPosition(const POINTS& mousepos)
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	
	if( pLineMap == NULL || pLineMap->GetNumLineData() <= 0 )
		return;

	const D3DXMATRIX& matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
	D3DXMATRIX matProj;
	g_pKTDXApp->GetProjectionTransform( &matProj );

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	float w = (float)(int)g_pKTDXApp->GetViewport().Width;
	float h = (float)(int)g_pKTDXApp->GetViewport().Height;

	float fMouseX = (float)(mousepos.x-w/2.f)/w *2.f;
	float fMouseY = (float)(h/2.f-mousepos.y)/h *2.f;
	D3DXVECTOR3 vMouse(fMouseX, fMouseY, 0.f);

	CKTDGLineMap::LineData* pLineData = NULL;	

	int iPickedSegmentIndex = 0;
	float fDistance = 0.f;
	float fMinDistance = -1.f;
	float fLerpCoef, fLerpCoefPicked;
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);
		D3DXVECTOR3 vStartPos, vEndPos;
		D3DXVec3TransformCoord( &vStartPos, &pLineData->startPos, &matViewProj );
		D3DXVec3TransformCoord( &vEndPos, &pLineData->endPos, &matViewProj);
		vStartPos.z = 0.f;
		vEndPos.z   = 0.f;

		fDistance = DistanceBetweenPointAndSegment(fLerpCoef, &vMouse, &vStartPos, &vEndPos);

		if( -1.f == fMinDistance || fDistance < fMinDistance )
		{
			iPickedSegmentIndex = i;
			fMinDistance = fDistance;
			fLerpCoefPicked = fLerpCoef;
		}
	}

	float fDistanceBound = 0.1f;
	if( fMinDistance < fDistanceBound )
	{
		pLineData = pLineMap->GetLineData(iPickedSegmentIndex);
		D3DXVECTOR3 vStartPos = pLineData->startPos;
		D3DXVECTOR3 vEndPos   = pLineData->endPos;
		D3DXVECTOR3 vPointOnSegmentPicked = vStartPos + (vEndPos-vStartPos) * fLerpCoefPicked;

		{
			int key=-1;
			if( iTeamColor == TEAM_NONE )
			{
				key = GetNextUniqueNPCStartPosKey();
				if( key == -1 )
					return;
			}

			vecTeamStartPos.push_back( STeamStartPos( key, iTeamColor, vPointOnSegmentPicked, iInitialDir, iPickedSegmentIndex ) );
			//UpdateStartPosListBox(true);
			//MainUI.GetListBox( IDC_LISTBOX_START_POS )->SelectItem( (int)vecTeamStartPos.size()-1 );		
			//UpdateStartPosComboBox();		
		}
	}
}

bool CX2WorldToolMain::PickClickedNPC( int iDungeonLevel, POINTS mousepos, bool bClear )
{
	D3DXVECTOR3 vRayStart, vRayDir;
	D3DXMATRIX matView, matProj;
	matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
	g_pKTDXApp->GetProjectionTransform( &matProj );
	//g_pKTDXApp->GetDevice()->GetTransform( D3DTS_PROJECTION, &matProj );
	ComputeRay( &vRayStart, &vRayDir, mousepos, &matView, &matProj);

	const float MAGIC_RAY_RANGE = 10000000.f;
	D3DXVECTOR3 vRayEnd;
	vRayEnd = vRayStart + vRayDir * MAGIC_RAY_RANGE;

	float fShortestHitDistance = MAGIC_RAY_RANGE;
	float fHitDistance = MAGIC_RAY_RANGE;

	if( true == bClear )
	{
		m_vecPickedNPCStartPosKey.clear();
		m_iPickedStartPosIndex = -1;
	}

	int iPickedNPCKey = -1;

	for( UINT i=0; i<vecTeamStartPos.size(); i++ )
	{
		STeamStartPos& npcStartPos = vecTeamStartPos[i];
		//if( npcStartPos.iTeamColor == TEAM_NONE )
		{
			if( true == NPCRayHit( npcStartPos.vPos, vRayStart, vRayEnd, &fHitDistance ) )
			{
				if( fHitDistance < fShortestHitDistance )
				{
					iPickedNPCKey = npcStartPos.key;
					m_iPickedStartPosIndex = i;
				}
			}
		}
	}

	if( iPickedNPCKey >= 0 )
		m_vecPickedNPCStartPosKey.push_back( SNPCStartPos( NULL, iPickedNPCKey ) );
	//UpdateNPCInfoUI();
	return true;;
}

// return value는 화면(즉, 2차원 평면위)에서의 점과 선분 사이의 거리를 의미한다. 
float CX2WorldToolMain::DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint,
														 D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd)
{
	D3DXVECTOR3 vTemp = *pvSegmentStart - *pvPoint;
	D3DXVECTOR3 vSegmentDir = *pvSegmentEnd - *pvSegmentStart;

	D3DXVECTOR3 vTangent;
	D3DXVec3Cross(&vTangent, &vSegmentDir, &vTemp);

	D3DXVECTOR3 vNormal;
	D3DXVec3Cross(&vNormal, &vSegmentDir, &vTangent);
	D3DXVec3Normalize(&vNormal, &vNormal);

	float fDistance = fabs( D3DXVec3Dot(&vNormal, &vTemp) );

	if( D3DXVec3Dot(&vTemp, &vNormal) < 0 )
	{
		vNormal = -vNormal;
	}

	D3DXVECTOR3 vPointOnSegment = *pvPoint + vNormal * fDistance;

	{
		D3DXVECTOR3 vTempA = vPointOnSegment - *pvSegmentStart;
		D3DXVECTOR3 vTempB;
		D3DXVec3Normalize( &vTempB, &vSegmentDir );
		fLerpCoef = D3DXVec3Dot( &vTempB, &vTempA ) / D3DXVec3Length( &vSegmentDir );
	}

	if( 0.f < fLerpCoef && fLerpCoef < 1.f )
	{
		return fDistance;
	}
	else
	{
		D3DXVECTOR3 vTempA = *pvSegmentStart - *pvPoint;
		D3DXVECTOR3 vTempB = *pvSegmentEnd   - *pvPoint;

		float fDistance1 = D3DXVec3Length( &vTempA );
		float fDistance2 = D3DXVec3Length( &vTempB );
		if( fDistance1 < fDistance2 )
		{
			fLerpCoef = 0.f;
			return fDistance1;
		}
		else
		{
			fLerpCoef = 1.f;
			return fDistance2;
		}
	}	
}

void CX2WorldToolMain::ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, 
									const D3DXMATRIX* pmatView, const D3DXMATRIX* pmatProj )
{
	RECT rc;
	float w, h;
	GetClientRect( DXUTGetHWND(), &rc );
	w = (float)(rc.right - rc.left);
	h = (float)(rc.bottom - rc.top);

	float left, top;
	left = (float)(mouspos.x-w/2)/w*2.0f;
	top  = (float)(h/2-mouspos.y)/h*2.0f;

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, pmatView, pmatProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	D3DXVECTOR3 vFrom, vTo, vRay;
	vFrom = D3DXVECTOR3(left, top, -1.f);
	vTo = D3DXVECTOR3(left, top,  1.f);

	D3DXVec3TransformCoord(&vFrom, &vFrom, &matInvViewProj);
	D3DXVec3TransformCoord(&vTo, &vTo, &matInvViewProj);
	vRay = vTo-vFrom;
	D3DXVec3Normalize(&vRay, &vRay);

	*pvRayDirection = vRay;
	*pvRayOrigin = vFrom;
}

void CX2WorldToolMain::CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
												   float fHitDistance, float fShortestHitDistance, CX2DungeonSubStage::NPCData* &pPickedNPCData, 
												   int &iPickedNPCStartIndex )
{
	for( UINT j=0; j<pSubStageData->m_NPCDataList.size(); j++ )
	{
		CX2DungeonSubStage::NPCData* pNPCData = pSubStageData->m_NPCDataList[j];
		for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
		{
			D3DXVECTOR3 vPos(0,0,0);

			for( UINT i=0; i<vecTeamStartPos.size(); i++ )
			{
				STeamStartPos& startPos = vecTeamStartPos[i];
				if( startPos.key == pNPCData->m_vecStartPos[k] )
				{
					vPos = startPos.vPos;
					break;
				}
			}

			if( true == NPCRayHit( vPos, vRayStart, vRayEnd, &fHitDistance ) )
			{
				if( fHitDistance < fShortestHitDistance )
				{
					pPickedNPCData = pNPCData;
					iPickedNPCStartIndex = pNPCData->m_vecStartPos[k];
				}
			}
		}
	}
}

bool CX2WorldToolMain::NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance )
{
	m_pMatrix->Move( vPos );
	D3DXMATRIX matTransform = m_pMatrix->GetMatrix(); 

	float fRadius = m_pXSkinMesh->GetBoundingRadius();
	D3DXVECTOR3 vCenter = m_pXSkinMesh->GetCenter();


	D3DXVec3TransformCoord( &vCenter, &vCenter, &matTransform );
	bool bResult = false;
	D3DXVECTOR3 vCollisionPoint;

	if( g_pKTDXApp->GetCollision()->LineToSphere( rayStart, rayEnd, 1.f, 
		vCenter, fRadius, &vCollisionPoint ) == true )
	{
		bResult = true;
		if( NULL != pHitDistance )
		{
			vCollisionPoint -= rayStart;
			*pHitDistance = D3DXVec3Length( &vCollisionPoint );
		}		
	}
	else
	{
		bResult = false;
		if( NULL != pHitDistance )
		{			
			*pHitDistance = -1.f;
		}
	}

	return bResult;
}

void CX2WorldToolMain::DeleteAllSkinMeshAndSkinAnim()
{	
	for( unsigned i=0; i < vecpXSkinAnim.size(); i++ )
	{
		//SAFE_DELETE( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
		SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)vecpXSkinAnim[i] );
	}
	vecpXSkinAnim.clear();

	//map< CX2UnitManager::NPC_UNIT_ID, CKTDXDeviceXSkinMesh* >::iterator it;
	//for( it=m_mapXSkinMesh.begin(); it!=m_mapXSkinMesh.end(); it++ )
	//{
	//	CKTDXDeviceXSkinMesh* pSkinMesh = (CKTDXDeviceXSkinMesh*)it->second;
	//	SAFE_CLOSE(pSkinMesh);
	//}
	//m_mapXSkinMesh.clear();

	//map< std::pair<CX2DungeonSubStage::NPCData*, int >, CKTDGXSkinAnim* >::iterator it2;
	//for( it2=m_mapNPCXSkinAnim.begin(); it2 !=m_mapNPCXSkinAnim.end(); it2++ )
	//{
	//	CKTDGXSkinAnim* pCKTDGXSkinAnim = (CKTDGXSkinAnim*)it2->second;
	//	//SAFE_DELETE( pCKTDGXSkinAnim );
	//	SAFE_DELETE_KTDGOBJECT( pCKTDGXSkinAnim );
	//}
	//m_mapNPCXSkinAnim.clear();

}

void CX2WorldToolMain::OnFrameRenderUnit()
{
	int iRedTeamStartIndex		= -1;
	int iBlueTeamStartIndex		= -1;
	int iDungeonStartIndex		= -1;
	int iStartIndex				= -1;
	int iNumber					= 0; // 화면에 출력되는 start pos index


	if( true == m_bShowUserStartPos )
	{
		RenderUserUnitStartPos();	
	}

	if( m_iShowDungeonLevel == 0 && m_bShowNpcStartPos == true )
	{
		RenderNPCUnitStartPostion( CX2Dungeon::DL_NORMAL );
		//RenderNPCUnitStartPostion( CX2Dungeon::DL_HARD );
		//RenderNPCUnitStartPostion( CX2Dungeon::DL_EXPERT );
	}
}

void CX2WorldToolMain::RenderUserUnitStartPos()
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( pLineMap == NULL )
		return;

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	CKTDGXRenderer::RenderParam renderParam;
	D3DXMATRIX worldMatrix;

	// render start pos
	for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
	{	
		if( false == m_bHideStartPosMesh )
		{
			vecpXSkinAnim[i]->GetMatrix().Move( vecTeamStartPos[i].vPos );

			if( vecTeamStartPos[i].iInitialDir == DIR_LEFT )
				vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 180, 0 );
			else
				vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 0, 0 );

			if( vecTeamStartPos[i].iTeamColor == TEAM_RED )
				vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 1, 0, 0, 1 );
			else if( vecTeamStartPos[i].iTeamColor == TEAM_BLUE )
				vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 0, 0, 1, 1 );
			else if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
				continue;

			vecpXSkinAnim[i]->OnFrameRender();
		}
//		else
//		{
//			if( vecTeamStartPos[i].iTeamColor == TEAM_RED )
//				//pRenderParam->color	= D3DXCOLOR(0.5f, 0, 0, 1 );
//				renderParam.color		= D3DXCOLOR(0.5f, 0, 0, 1 );
//			else if( vecTeamStartPos[i].iTeamColor == TEAM_BLUE )
//				//pRenderParam->color	= D3DXCOLOR(0, 0, 0.5f, 1 );
//				renderParam.color		= D3DXCOLOR(0, 0, 0.5f, 1 );
//			else if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
//				continue;
//			const float MAGIC_DOT_SIZE = 5.f;
//			matTemp.Move( vecTeamStartPos[i].vPos );
//			matTemp.Scale( MAGIC_DOT_SIZE, MAGIC_DOT_SIZE, MAGIC_DOT_SIZE );
//			//pRenderParam->worldMatrix = matTemp.GetMatrix();
//			worldMatrix = matTemp.GetMatrix();
//			//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
//			//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
//			//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
//			g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
//		}

	}

	if( m_f3DTextAngle > 25.f )
		m_f3DTextAngleStep = -.25f;
	else if( m_f3DTextAngle < -25.f )
		m_f3DTextAngleStep = .25f;
	m_f3DTextAngle += m_f3DTextAngleStep;

	int iRedTeamStartIndex		= -1;
	int iBlueTeamStartIndex		= -1;
	int iDungeonStartIndex		= -1;
	int iStartIndex				= -1;
	int iNumber					= 0; // 화면에 출력되는 start pos index

	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
	for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
	{
		matTemp.Move( vecTeamStartPos[i].vPos );
		matTemp.MoveRel( 0, 160, 0 );
		matTemp.Scale( 100, 100, 100 );
		//matTemp.RotateDegree( 0, m_f3DTextAngle, 0 );

		int iLineIndex = vecTeamStartPos[i].iLineIndex;
		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
		D3DXVECTOR3 vDir = pLineData->dirVector;
		vDir.y = 0.f;
		D3DXVec3Normalize( &vDir, &vDir );
		D3DXVECTOR3 vXAxis(1,0,0);

		float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
		float fRad = acosf( fDotXAxis );
		if( vDir.z < 0.f )
		{
			fRad = -fRad;
		}
		matTemp.Rotate( 0.f, -fRad, 0.f );



		D3DMATERIAL9 mtrl;
		ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

		if( vecTeamStartPos[i].iTeamColor == TEAM_RED )
		{
			iRedTeamStartIndex++;
			iNumber = iRedTeamStartIndex;
			D3DXCOLOR coTeamColor = D3DXCOLOR( 1, 0, 0, 1 );
			memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
			memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );
		}
		else if( vecTeamStartPos[i].iTeamColor == TEAM_BLUE )
		{
			iBlueTeamStartIndex++;
			iNumber = iBlueTeamStartIndex; 			
			D3DXCOLOR coTeamColor = D3DXCOLOR( 0, 0, 1, 1 );
			memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
			memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );
		}
		else
			continue;

		g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );			

		int iDigit;
		while( iNumber >= 0 )
		{
			iDigit = iNumber%10;

			matTemp.UpdateWorldMatrix();			
			m_pMesh3DText[iDigit]->DrawSubset(0);

			//matTemp.MoveRel( -50, 0, 0 );
			matTemp.MoveRel( -50.f * vDir );
			iNumber = iNumber/10;
			if( iNumber == 0 )
				break;
		}		
	}
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
	CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
}



void CX2WorldToolMain::RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex /*= -1*/, int iNPCSubIndex /*= -1*/ )
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( pLineMap == NULL )
		return;

	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );

	if( false == m_bUseNPCMesh )
	{
		int iNumber					= 0; // 화면에 출력되는 start pos index

		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
		CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );
		for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
		{
			matTemp.Move( vecTeamStartPos[i].vPos );
			matTemp.MoveRel( 0, 160, 0 );
			matTemp.Scale( 100, 100, 100 );

			int iLineIndex = vecTeamStartPos[i].iLineIndex;
			CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
			D3DXVECTOR3 vDir = pLineData->dirVector;
			vDir.y = 0.f;
			D3DXVec3Normalize( &vDir, &vDir );
			D3DXVECTOR3 vXAxis(1,0,0);

			float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
			float fRad = acosf( fDotXAxis );
			if( vDir.z < 0.f )
			{
				fRad = -fRad;
			}
			matTemp.Rotate( 0.f, -fRad, 0.f );



			D3DMATERIAL9 mtrl;
			ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

			if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
			{
				D3DXCOLOR coTeamColor = D3DXCOLOR( 0.5, 0.5, 0, 1 );
				memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
				memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );
			}
			else
				continue;

			g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );

			iNumber = vecTeamStartPos[i].key;
			int iDigit;
			while( iNumber >= 0 )
			{
				iDigit = iNumber%10;

				matTemp.UpdateWorldMatrix();			
				m_pMesh3DText[iDigit]->DrawSubset(0);

				matTemp.MoveRel( -50.f * vDir );
				iNumber = iNumber/10;
				if( iNumber == 0 )
					break;
			}		
		}
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
		CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
	}
	else
	{
		if( NULL != pNPCData )
		{
			int inpcstartindex = pNPCData->m_vecStartPos[iNPCSubIndex];
			D3DXVECTOR3 vPos = GetTeamStartPos( inpcstartindex )->vPos;
			CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
			CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT );

			matTemp.Move( vPos );
			matTemp.MoveRel( 0, 200, 0 );
			matTemp.Scale( 50, 50, 50 );

			int iLineIndex = pLineMap->GetStartLineIndex( pNPCData->m_vecStartPos[iNPCSubIndex] );
			CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
			D3DXVECTOR3 vDir = pLineData->dirVector;
			vDir.y = 0.f;
			D3DXVec3Normalize( &vDir, &vDir );
			D3DXVECTOR3 vXAxis(1,0,0);

			float fDotXAxis = D3DXVec3Dot( &vDir, &vXAxis );
			float fRad = acosf( fDotXAxis );
			if( vDir.z < 0.f )
			{
				fRad += D3DX_PI;
			}
			matTemp.Rotate( 0.f, fRad, 0.f );


			D3DMATERIAL9 mtrl;
			ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );


			D3DXCOLOR coTeamColor = D3DXCOLOR( 0.5, 0.5, 0, 1 );
			memcpy( &mtrl.Diffuse, &coTeamColor, sizeof(D3DCOLORVALUE) );
			memcpy( &mtrl.Ambient, &coTeamColor, sizeof(D3DCOLORVALUE) );

			g_pKTDXApp->GetDevice()->SetMaterial( &mtrl );			

			int iNumber = inpcstartindex;
			int iDigit;
			while( iNumber >= 0 )
			{
				iDigit = iNumber%10;

				matTemp.UpdateWorldMatrix();			
				m_pMesh3DText[iDigit]->DrawSubset(0);

				matTemp.MoveRel( -35.f * vDir );
				iNumber = iNumber/10;
				if( iNumber == 0 )
					break;
			}



			// stage에서 몇번째 npc 인지 
			iNumber = iStageNPCIndex;
			matTemp.Move( vPos );
			matTemp.MoveRel( 0, 200+40, 0 );
			matTemp.MoveRel( -100.f * vDir );
			while( iNumber >= 0 )
			{
				iDigit = iNumber%10;

				matTemp.UpdateWorldMatrix();			
				m_pMesh3DText[iDigit]->DrawSubset(0);

				matTemp.MoveRel( -35.f * vDir );
				iNumber = iNumber/10;
				if( iNumber == 0 )
					break;
			}


			// '-'
			matTemp.Move( vPos );
			matTemp.MoveRel( 0, 200+40, 0 );
			matTemp.MoveRel( -70.f * vDir );
			matTemp.UpdateWorldMatrix();
			m_pMesh3DText[10]->DrawSubset(0);


			// 같은 npc의 몇번째 random start 위치 인지
			iNumber = iNPCSubIndex;
			matTemp.Move( vPos );
			matTemp.MoveRel( 0, 200+40, 0 );
			while( iNumber >= 0 )
			{
				iDigit = iNumber%10;

				matTemp.UpdateWorldMatrix();
				m_pMesh3DText[iDigit]->DrawSubset(0);

				//matTemp.MoveRel( -35, 0, 0 );
				matTemp.MoveRel( -35.f * vDir );
				iNumber = iNumber/10;
				if( iNumber == 0 )
					break;
			}




			for( UINT i = 0; i<m_vecPickedNPCStartPosKey.size(); i++ )
			{
				SNPCStartPos& npcStart = m_vecPickedNPCStartPosKey[i];
				if( npcStart.m_pNPCData == pNPCData )
				{
					// '*'
					matTemp.Move( vPos );
					matTemp.MoveRel( 0, 200+60, 0 );
					matTemp.Scale( 300, 300, 300 );

					matTemp.RotateDegree( m_f3DTextAngle * 5.f, m_f3DTextAngle * 25.f, m_f3DTextAngle * 15.f );

					matTemp.UpdateWorldMatrix();
					m_pMesh3DText[11]->DrawSubset(0);

					break;
				}
			}

			CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
			CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
			CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
		}

	}
}


void CX2WorldToolMain::RenderNPCUnitStartPostion( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficulty )
{
	CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
	//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
	CKTDGXRenderer::RenderParam renderParam;


	if( false == m_bHideStartPosMesh )
	{
		if( false == m_bUseNPCMesh )
		{
			// render start pos
			for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
			{	
				vecpXSkinAnim[i]->GetMatrix().Move( vecTeamStartPos[i].vPos );

				if( vecTeamStartPos[i].iInitialDir == DIR_LEFT )
					vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 180, 0 );
				else
					vecpXSkinAnim[i]->GetMatrix().RotateDegree( 0, 0, 0 );

				if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
				{
					vecpXSkinAnim[i]->GetRenderParam()->color = D3DXCOLOR( 0, 1, 0, 1 );
					vecpXSkinAnim[i]->OnFrameRender();
				}		
			}

			RenderNPCStartPosIndex( NULL );
		}
//		else
//		{
//			CKTDGMatrix matTemp( g_pKTDXApp->GetDevice() );
//			//CKTDGXRenderer::RenderParam* pRenderParam = g_pKTDXApp->GetDGManager()->GetXRenderer()->GetRenderParam();
//			CKTDGXRenderer::RenderParam renderParam;
//			vector< CX2DungeonSubStage::SubStageData* >* pvecSubStageData = NULL;
//			switch( dungeonDifficulty )
//			{
//			case CX2Dungeon::DL_NORMAL:
//				if( NULL != m_pNormalStageData )
//					pvecSubStageData = &m_pNormalStageData->m_SubStageDataList;
//				break;
//			case CX2Dungeon::DL_HARD:
//				if( NULL != m_pHardStageData )
//					pvecSubStageData = &m_pHardStageData->m_SubStageDataList;
//				break;
//			case CX2Dungeon::DL_EXPERT:
//				if( NULL != m_pExpertStageData )
//					pvecSubStageData = &m_pExpertStageData->m_SubStageDataList;
//				break;
//			}
//			if( pvecSubStageData == NULL )
//				return;
//			for( UINT i=0; i<pvecSubStageData->size(); i++ )
//			{
//				vector<CX2DungeonSubStage::NPCData*>& npcDataList = pvecSubStageData->at(i)->m_NPCDataList;
//				for( UINT j=0; j<npcDataList.size(); j++ )
//				{
//					CX2DungeonSubStage::NPCData* pNPCData = npcDataList.at(j);
//					for( UINT k=0; k<pNPCData->m_vecStartPos.size(); k++ )
//					{
//						//CKTDGXSkinAnim* pNPCSkinAnim = m_mapNPCXSkinAnim[pNPCData];				
//						CKTDGXSkinAnim* pNPCSkinAnim = m_mapNPCXSkinAnim[ std::make_pair( pNPCData, k) ];
//						if( NULL == pNPCSkinAnim )
//							continue;
//						STeamStartPos* pTeamStartPos = GetTeamStartPos( pNPCData->m_vecStartPos[k] );
//						if( pTeamStartPos == NULL )
//						{
//							//WCHAR wszTempMsg[256];
//							//StringCchPrintf( wszTempMsg, sizeof(wszTempMsg), L"라인맵에 없는 NPC start pos(%d) 입니다", pNPCData->m_StartPos );
//							//MessageBox( g_pKTDXApp->GetHWND(), L"ERROR", wszTempMsg", MB_OK );
//							continue;
//						}
//						D3DXVECTOR3 vPos = pTeamStartPos->vPos;
//						bool bIsRight = false;
//						if( pTeamStartPos->iInitialDir == DIR_RIGHT )
//						{
//							bIsRight = true;
//						}
//						pNPCSkinAnim->GetMatrix().Move( vPos );
//						pNPCSkinAnim->GetMatrix().MoveRel( pNPCData->m_AddPos );
//						if( true == bIsRight )
//							pNPCSkinAnim->GetMatrix().RotateDegree( 0, 0, 0 );					
//						else
//							pNPCSkinAnim->GetMatrix().RotateDegree( 0, 180, 0 );
//						if( k == 0 )
//						{
//							pNPCSkinAnim->GetRenderParam()->color = D3DXCOLOR( 0, 1, 0, 1 );
//						}
//						else
//						{
//							pNPCSkinAnim->GetRenderParam()->color = D3DXCOLOR( 1, 1, (float)k*0.02f, 1 );
//						}
//						pNPCSkinAnim->OnFrameRender();
//						RenderNPCStartPosIndex( pNPCData, j, k );	
//					}
//				}
//			}
//		}
//	}
//	else
//	{
//		// render start pos
//		for( unsigned i=0; i<vecTeamStartPos.size(); i++ )
//		{	
//			if( vecTeamStartPos[i].iTeamColor == TEAM_NONE )
//			{
//				//pRenderParam->color	= D3DXCOLOR(0, 0.5f, 0, 1 );
//				renderParam.color = D3DXCOLOR(0, 0.5f, 0, 1);
//				const float MAGIC_DOT_SIZE = 5.f;
//				matTemp.Move( vecTeamStartPos[i].vPos );
//				matTemp.Scale( MAGIC_DOT_SIZE, MAGIC_DOT_SIZE, MAGIC_DOT_SIZE );
//				//pRenderParam->worldMatrix = matTemp.GetMatrix();
//				D3DXMATRIX worldMatrix = matTemp.GetMatrix();
//				//g_pKTDXApp->GetDGManager()->GetXRenderer()->BeginRender();
//				//g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( m_pXMeshCube );
//				//g_pKTDXApp->GetDGManager()->GetXRenderer()->EndRender();
//				g_pKTDXApp->GetDGManager()->GetXRenderer()->OnFrameRender( renderParam, worldMatrix, *m_pXMeshCube );
//			}
//		}
//		RenderNPCStartPosIndex( NULL );
	}
}

bool CX2WorldToolMain::GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos )
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( pLineMap == NULL || pLineMap->GetNumLineData() <= 0 )
		return false;

	const D3DXMATRIX& matView = g_pKTDXApp->GetDGManager()->GetCamera().GetViewMatrix();
	D3DXMATRIX matProj;
	g_pKTDXApp->GetProjectionTransform( &matProj );

	D3DXMATRIX matViewProj, matInvViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	D3DXMatrixInverse(&matInvViewProj, NULL, &matViewProj);

	float w = (float)(int)g_pKTDXApp->GetViewport().Width;
	float h = (float)(int)g_pKTDXApp->GetViewport().Height;

	float fMouseX = (float)(mousepos.x-w/2.f)/w *2.f;
	float fMouseY = (float)(h/2.f-mousepos.y)/h *2.f;
	D3DXVECTOR3 vMouse(fMouseX, fMouseY, 0.f);

	CKTDGLineMap::LineData* pLineData = NULL;	

	int iPickedSegmentIndex = 0;
	float fDistance = 0.f;
	float fMinDistance = -1.f;
	float fLerpCoef, fLerpCoefPicked;
	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		pLineData = pLineMap->GetLineData(i);
		D3DXVECTOR3 vStartPos, vEndPos;
		D3DXVec3TransformCoord( &vStartPos, &pLineData->startPos, &matViewProj );
		D3DXVec3TransformCoord( &vEndPos, &pLineData->endPos, &matViewProj);
		vStartPos.z = 0.f;
		vEndPos.z   = 0.f;

		fDistance = DistanceBetweenPointAndSegment(fLerpCoef, &vMouse, &vStartPos, &vEndPos);

		if( -1.f == fMinDistance || fDistance < fMinDistance )
		{
			iPickedSegmentIndex = i;
			fMinDistance = fDistance;
			fLerpCoefPicked = fLerpCoef;
		}
	}

	float fDistanceBound = 0.1f;
	if( fMinDistance < fDistanceBound )
	{
		pLineData = pLineMap->GetLineData(iPickedSegmentIndex);
		D3DXVECTOR3 vStartPos = pLineData->startPos;
		D3DXVECTOR3 vEndPos   = pLineData->endPos;

		D3DXVECTOR3 vPointOnSegmentPicked = vStartPos + (vEndPos-vStartPos) * fLerpCoefPicked;
		*pvPosOnLine = vPointOnSegmentPicked;
		return true;
	}	
	return false;
}

void CX2WorldToolMain::GetLineIndexInRect( float fLeft, float fTop, float fRight, float fBottom )
{
//	float fTemp;
//	if( fLeft > fRight )
//	{
//		fTemp = fLeft;
//		fLeft = fRight;
//		fRight = fTemp;
//	}
//	if( fTop < fBottom )
//	{	
//		fTemp = fTop;
//		fTop = fBottom;
//		fBottom = fTemp;
//	}
//	//m_veciPickedSegmentIndex.clear();
//	D3DXMATRIX matView, matProj, matViewProj;
//	matView = *m_FPSCamera.GetViewMatrix();
//	matProj = *m_FPSCamera.GetProjMatrix();
//	D3DXMatrixMultiply( &matViewProj, &matView, &matProj );
//	D3DXVECTOR3 vTemp;
//	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
//	{
//		CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);
//		D3DXVec3TransformCoord( &vTemp, &pLineData->startPos, &matViewProj );		
//		if( vTemp.x > fLeft && vTemp.x < fRight && vTemp.y < fTop && vTemp.y > fBottom )
//		{
//			m_veciPickedSegmentIndex.push_back(i);			
//			continue;
//		}
//		D3DXVec3TransformCoord( &vTemp, &pLineData->endPos, &matViewProj );		
//		if( vTemp.x > fLeft && vTemp.x < fRight && vTemp.y < fTop && vTemp.y > fBottom )
//		{
//			m_veciPickedSegmentIndex.push_back(i);			
//			continue;
//		}
//	}
}

HRESULT CX2WorldToolMain::CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, 
											   TCHAR* pstrFont, DWORD dwSize,
											   BOOL bBold, BOOL bItalic, WCHAR* wszText)
{
	HRESULT hr;
	LPD3DXMESH pMeshNew = NULL;
	HDC hdc = CreateCompatibleDC( NULL );
	if( hdc == NULL )
		return E_OUTOFMEMORY;
	INT nHeight = -MulDiv( dwSize, GetDeviceCaps(hdc, LOGPIXELSY), 72 );
	HFONT hFont;
	HFONT hFontOld;

	hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pstrFont);

	hFontOld = (HFONT)SelectObject(hdc, hFont); 

	hr = D3DXCreateText( pd3dDevice, hdc, wszText, 0.001f, 0.01f, &pMeshNew, NULL, NULL);

	SelectObject(hdc, hFontOld);
	DeleteObject( hFont );
	DeleteDC( hdc );

	if( SUCCEEDED( hr ) )
		*ppMesh = pMeshNew;

	return hr;
}


CX2WorldToolMain::STeamStartPos* CX2WorldToolMain::GetTeamStartPos( int iNPCStartPosKey )
{
	for( UINT i=0; i<vecTeamStartPos.size(); i++ )
	{
		if( vecTeamStartPos[i].key == iNPCStartPosKey )
		{
			return &vecTeamStartPos[i];
		}
	}
	return NULL;
}

void CWorldToolGame::Release()
{
	CX2Game::Release();
}