// =========================================================

#include "dxstdafx.h"
#include "../X2/KOGGamePerformanceCheckDefine.h"
#include ".\x2EffectToolFileManager.h"
#include ".\X2EffectToolParticleEditor.h"
#include ".\X2EffectToolMeshEditor.h"
#include ".\EffectToolGame.h"
#include ".\WorldToolObject.h"
#include ".\CSkinAnimObject.h"
#include ".\x2EffectToolMain.h"

#include "resource.h"

using std::endl;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HWND g_hDlg = NULL;
HWND g_hDlgLineMap = NULL;
HWND g_hFrustumDlg = NULL;

CX2EffectToolMain *g_pEffectToolMain;

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

    if(g_pEffectToolMain == NULL)
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

                pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];

                g_pEffectToolMain->m_vecWorldLayer.erase(g_pEffectToolMain->m_vecWorldLayer.begin() + index);
                g_pEffectToolMain->m_pGame->GetWorld()->RemoveWordLayer(index);

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
                    CX2WorldLayer *pLayer = g_pEffectToolMain->m_pGame->GetWorld()->CreateWorldLayer();

                    if(pLayer == NULL)
                    {
                        MessageBox(hDlg, L"레이어 생성 실패", L"Error!", MB_OK);
                        return FALSE;
                    }                
                    
                    g_pEffectToolMain->DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, ofn.lpstrFile );
                    
                    string TexName;
                    ConvertWCHARToChar( TexName, wszFileNameOnly );                    
                    pLayer->SetTex(TexName.c_str());              

                    g_pEffectToolMain->m_vecWorldLayer.push_back(pLayer);

                    //InitLayerDlg(hDlg); // 새로운 레이어를 위해 레이어 다이얼로그 초기화
                    UpdateLayer(hDlg, pLayer); // 추가된 레이어를 world에 적용       
                    UpdateLayerDlg(hDlg, pLayer);                    
                    if(g_pEffectToolMain->m_vecWorldLayer.size() > 0)
                        SendDlgItemMessage(hDlg, IDC_LIST2, LB_SETCURSEL, g_pEffectToolMain->m_vecWorldLayer.size() -1, 0);

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
    
                pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];

                if( TRUE == GetOpenFileName( &ofn ) )
                {
                    if(pWorldLayer == NULL)
                    {
                        MessageBox(hDlg, L"텍스쳐 변경 실패", L"Error!", MB_OK);
                        return FALSE;
                    }                

                    g_pEffectToolMain->DecomposePathAndFileName( (WCHAR*)wszFilePathOnly, (WCHAR*)wszFileNameOnly, ofn.lpstrFile );

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
                        pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];
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
                        pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];
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
                        pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];
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
					pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];
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
                pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];
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

            for(int i=0; i<(int)g_pEffectToolMain->m_vecWorldLayer.size(); ++i)
            {
                CX2WorldLayer *pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[i];
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
        pWorldLayer = g_pEffectToolMain->m_vecWorldLayer[index];
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


	if( g_pEffectToolMain == NULL ||
		g_pEffectToolMain->m_pGame == NULL ||
		g_pEffectToolMain->m_pGame->GetWorld() == NULL )
		return FALSE;

	CKTDGLineMap *pLineMap = g_pEffectToolMain->m_pGame->GetWorld()->GetLineMap();
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

		SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER,	BM_SETCHECK, (WPARAM)!g_pEffectToolMain->m_bShowUserStartPos, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC,		BM_SETCHECK, (WPARAM)!g_pEffectToolMain->m_bShowNpcStartPos, 0);
		SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDEWORLD,	BM_SETCHECK, (WPARAM)!g_pEffectToolMain->m_bShowWorld, 0);

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
		


		// 갱신
		UpdateLIneListDlg(GetDlgItem(hDlg, LM_IDC_LIST1), pLineMap);
		UpdateCameraListDlg( hDlg, pLineMap );
		UpdateStartPosDlg(hDlg);
		UpdateTriggerDlg(hDlg, pLineMap);

		g_pEffectToolMain->m_iStartPosListIndex = -1;

		return TRUE;    

	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			// 비활성
			g_pEffectToolMain->m_iStartPosListIndex = -1;
		}
		return TRUE;

	case WM_COMMAND:
		if( HIWORD(wParam) == BN_CLICKED )
		{
			switch(LOWORD(wParam))
			{
			case LM_IDC_RADIO_RED:
				g_pEffectToolMain->iTeamColor = CX2EffectToolMain::TEAM_RED;
				return TRUE;
			case LM_IDC_RADIO_BLUE:
				g_pEffectToolMain->iTeamColor = CX2EffectToolMain::TEAM_BLUE;
				return TRUE;
			case LM_IDC_RADIO_OTHER:
				g_pEffectToolMain->iTeamColor = CX2EffectToolMain::TEAM_NONE;
				return TRUE;

			case LM_IDC_CHECK_HIDE_USER:
				{
					if( SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER, BM_GETCHECK, 0, 0) == false )
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER, BM_SETCHECK, (WPARAM)true, 0);
						g_pEffectToolMain->m_bShowUserStartPos	= true;
					}
					else
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDE_USER, BM_SETCHECK, (WPARAM)false, 0);
						g_pEffectToolMain->m_bShowUserStartPos	= false;
					}
				}
				return TRUE;
			case LM_IDC_CHECK_HIDENPC:
				{
					if( SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC, BM_GETCHECK, 0, 0) == false )
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC, BM_SETCHECK, (WPARAM)true, 0);
						g_pEffectToolMain->m_bShowNpcStartPos	= true;
					}
					else
					{
						//SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDENPC, BM_SETCHECK, (WPARAM)false, 0);
						g_pEffectToolMain->m_bShowNpcStartPos	= false;
					}
				}
				return TRUE;
			case LM_IDC_CHECK_HIDEWORLD:
				{
					if( SendDlgItemMessage(hDlg, LM_IDC_CHECK_HIDEWORLD, BM_GETCHECK, 0, 0) == false )
					{
						g_pEffectToolMain->m_bShowWorld	= true;
					}
					else
					{
						g_pEffectToolMain->m_bShowWorld	= false;
					}
				}
				return TRUE;

			}
		}

		switch(LOWORD(wParam))
		{		
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
					g_pEffectToolMain->SaveLineMapToFile(ofn.lpstrFile, true);
				}				
			}
			return TRUE;
		case LM_IDCANCEL:
			{
				// 라인맵 텍스트로 뷰
				g_pEffectToolMain->SaveLineMapToText();
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

				const D3DXVECTOR3 vEye = g_pEffectToolMain->m_pGame->GetX2Camera()->GetCamera().GetEye();
				const D3DXVECTOR3 vLookAt = g_pEffectToolMain->m_pGame->GetX2Camera()->GetCamera().GetLookAt();

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
				ModifyStartPos(hDlg, g_pEffectToolMain->m_iStartPosListIndex);
				UpdateStartPosDlg(hDlg);
				g_pEffectToolMain->m_iStartPosListIndex = -1;
			}
			return TRUE;
		case LM_IDC_BUTTON_RESETKEY:
			{				
				UpdateStartPosDlg(hDlg);
				g_pEffectToolMain->m_iStartPosListIndex = -1;
			}
			return TRUE;
		case LM_IDC_BUTTON_DELETENPC:
			{
				g_pEffectToolMain->DeleteNpcStartPos();
				UpdateStartPosDlg(hDlg);
				g_pEffectToolMain->m_iStartPosListIndex = -1;
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
				for(UINT i=0; i<g_pEffectToolMain->m_vecpWorldToolObject.size(); ++i)
				{
					CWorldToolObject* pWorldToolObject = g_pEffectToolMain->m_vecpWorldToolObject[i];
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
						if( g_pEffectToolMain != NULL )
							g_pEffectToolMain->iInitialDir = index;
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
						g_pEffectToolMain->m_iStartPosListIndex = GetStartPosIndex(1, index);
						UpdateStartPosInfo( hDlg, g_pEffectToolMain->m_iStartPosListIndex );					
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
						g_pEffectToolMain->m_iStartPosListIndex = GetStartPosIndex(2, index);
						UpdateStartPosInfo( hDlg, g_pEffectToolMain->m_iStartPosListIndex );
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
						g_pEffectToolMain->m_iStartPosListIndex = GetStartPosIndex(0, index);
						UpdateStartPosInfo( hDlg, g_pEffectToolMain->m_iStartPosListIndex );
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
			if( LOWORD(wParam) == VK_DELETE && g_pEffectToolMain->m_iStartPosListIndex >= 0 )
			{
				HWND hWnd = GetFocus();
				if( hWnd == GetDlgItem(hDlg, LM_IDC_LIST_RED) ||
					hWnd == GetDlgItem(hDlg, LM_IDC_LIST_BLUE) ||
					hWnd == GetDlgItem(hDlg, LM_IDC_LIST_OTHER) )
				{
					g_pEffectToolMain->vecTeamStartPos.erase(g_pEffectToolMain->vecTeamStartPos.begin() + g_pEffectToolMain->m_iStartPosListIndex);
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
		g_pEffectToolMain->m_bShowLineMapModifier = false;
		g_pEffectToolMain->m_bShowWorld = true;

		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			CKTDGLineMap::LineData *pLineDataInit = pLineMap->AccessLineData(i);
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
		const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
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
	CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(iIndex);

	for(int i=0; i<pLineMap->GetNumLineData(); ++i)
	{
		CKTDGLineMap::LineData *pLineDataInit = pLineMap->AccessLineData(i);
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
			CKTDGLineMap::LineData *pLineDataInit = pLineMap->AccessLineData(m_iSelectedList[i]);
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

	g_pEffectToolMain->ResetNpcStartPosKey();

	WCHAR wstr[100];
	WCHAR wstrRight[10];
	int iRed, iBlue;
	iRed = iBlue = -1;

	for(UINT i=0; i < g_pEffectToolMain->vecTeamStartPos.size(); ++i)
	{
		CX2EffectToolMain::STeamStartPos& spos = g_pEffectToolMain->vecTeamStartPos[i];
		wsprintf( wstrRight, L"(Left)" );
		if( spos.iInitialDir == CX2EffectToolMain::DIR_RIGHT )
		{
			wsprintf( wstrRight, L"(Right)" );
		}

		switch( spos.iTeamColor )
		{
		case CX2EffectToolMain::TEAM_NONE:
			{
				wsprintf(wstr, L"%d %s", spos.key, wstrRight);
				SendMessage( GetDlgItem(hDlg, LM_IDC_LIST_OTHER), LB_ADDSTRING, 0, (LPARAM)wstr );
			}
			break;
		case CX2EffectToolMain::TEAM_RED:
			{
				++iRed;
				wsprintf(wstr, L"%d %s", iRed, wstrRight);
				SendMessage( GetDlgItem(hDlg, LM_IDC_LIST_RED), LB_ADDSTRING, 0, (LPARAM)wstr );
			}
			break;
		case CX2EffectToolMain::TEAM_BLUE:
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

	for(UINT i=0; i<g_pEffectToolMain->vecTeamStartPos.size(); ++i)
	{
		switch(g_pEffectToolMain->vecTeamStartPos[i].iTeamColor)
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
		if( g_pEffectToolMain->vecTeamStartPos[i].iTeamColor == iTeam )
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
	if( index >= (int)g_pEffectToolMain->vecTeamStartPos.size() )
		return;

	CX2EffectToolMain::STeamStartPos teamInfo =  g_pEffectToolMain->vecTeamStartPos[index];


	SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_SETCURSEL, (WPARAM)teamInfo.iTeamColor, 0);
	SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_SETCURSEL, (WPARAM)teamInfo.iInitialDir, 0);
}

void ModifyStartPos(HWND hDlg, int index)
{
	if( index < 0 || index > (int)g_pEffectToolMain->vecTeamStartPos.size() )
		return;

	int iTeam = SendDlgItemMessage(hDlg, LM_IDC_COMBO_TEAMTYPE, CB_GETCURSEL, 0, 0);
	if( iTeam == 0 )
	{
		int key = g_pEffectToolMain->GetNextUniqueNPCStartPosKey();
		if( key == -1 )
			return;
		g_pEffectToolMain->vecTeamStartPos.at(index).key = key;
	}

	g_pEffectToolMain->vecTeamStartPos.at(index).iTeamColor = iTeam;
	g_pEffectToolMain->vecTeamStartPos.at(index).iInitialDir = SendDlgItemMessage(hDlg, LM_IDC_COMBO_DIR, CB_GETCURSEL, 0, 0);
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

		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(index);
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

		CKTDGLineMap::LineData *pLineData = pLineMap->AccessLineData(index);
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
	
	float fLandHeight = g_pEffectToolMain->ComputeLandHeight();
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
			const CKTDGLineMap::LineData *pLine = pLineMap->GetLineData(i);
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
	
	for(UINT i=0; i<g_pEffectToolMain->m_vecpWorldToolObject.size(); ++i)
	{
		CWorldToolObject* pWorldToolObject = g_pEffectToolMain->m_vecpWorldToolObject[i];

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
	if(g_pEffectToolMain == NULL)
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

void CALLBACK OnDialogEvent_DLG_OBJECT_PROPERTY( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_OBJECT_PROPERTY );

	vector<CWorldToolObject*>& vecpWorldToolObject = pEffectTool->GetPickedWorldToolObjectList();
    vector<CWorldToolObject*>& vecpWorldToolAllObject = pEffectTool->GetWorldToolObjectList();
    

	switch( nControlID )
	{
#ifdef COLORFACTOR_FOR_EVERY_WORLDMESH
	//{{ 09.04.25 태완
	case CX2EffectToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR:
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

						pEffectTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iLightColor[0], 3 );
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

	case CX2EffectToolMain::IDC_SLIDER_OBJECT_LIGHT_COLOR_RED:
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

				CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR );
				pEffectTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

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

	case CX2EffectToolMain::IDC_SLIDER_OBJECT_LIGHT_COLOR_GREEN:
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

				CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR );
				pEffectTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

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


	case CX2EffectToolMain::IDC_SLIDER_OBJECT_LIGHT_COLOR_BLUE:
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

				CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_OBJECT_LIGHT_COLOR );
				pEffectTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

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
    case CX2EffectToolMain::IDC_CHECK_IS_FOGWORLD:
        {
            bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetIsFog( bChecked );
            }
        }
        break;

    case CX2EffectToolMain::IDC_CHECK_IS_FOG:
        {
            bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();            
            for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
                pWorldToolObject->SetIsFog( bChecked );
            }	
        }
        break;

    case CX2EffectToolMain::IDC_EDIT_FOG_NEAR_X:
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
    case CX2EffectToolMain::IDC_EDIT_FOG_FAR_X:
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
    case CX2EffectToolMain::IDC_EDIT_FOG_NEAR_Y:
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
    case CX2EffectToolMain::IDC_EDIT_FOG_FAR_Y:
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
    case CX2EffectToolMain::IDC_EDIT_FOG_DENSITY:
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
    case CX2EffectToolMain::IDC_EDIT_FOG_DENSITY_OBJECT:
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

    case CX2EffectToolMain::IDC_EDIT_ORIGIN_FOG_COLOR:
        {
            switch( nEvent )
            {
            case EVENT_EDITBOX_STRING:
                {
                    //float fLightColor[3];
                    int iLightColor[3];

                    pEffectTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*)&iLightColor[0], 3 );
                    for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
                    {
                        CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                        pWorldToolObject->SetFogColor(iLightColor[0]/255.0f, iLightColor[1]/255.0f, iLightColor[2]/255.0f);
                    }	
                    pEffectTool->SetFogColor(iLightColor[0]/255.0f, iLightColor[1]/255.0f, iLightColor[2]/255.0f);
                    pDialog->ClearFocus();

                } break;
            }
        } break;

    case CX2EffectToolMain::IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_RED:
        {
            int iOriginLightColor[3] = { 0, };
            D3DXCOLOR coOriginLightColor = pEffectTool->GetFogColor();
            iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
            iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
            iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

            iOriginLightColor[0] = ((CDXUTSlider*)pControl)->GetValue();

            CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_ORIGIN_FOG_COLOR );
            pEffectTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
            }	
            pEffectTool->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);

        } break;



    case CX2EffectToolMain::IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_GREEN:
        {
            int iOriginLightColor[3] = { 0, };
            D3DXCOLOR coOriginLightColor = pEffectTool->GetFogColor();
            iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
            iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
            iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

            iOriginLightColor[1] = ((CDXUTSlider*)pControl)->GetValue();

            CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_ORIGIN_FOG_COLOR );
            pEffectTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
            }	
            pEffectTool->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
        } break;


    case CX2EffectToolMain::IDC_SLIDER_ORIGIN_WORLD_FOG_COLOR_BLUE:
        {
            int iOriginLightColor[3] = { 0, };
            D3DXCOLOR coOriginLightColor = pEffectTool->GetFogColor();
            iOriginLightColor[0] = (int) ( coOriginLightColor.r * 255.f );
            iOriginLightColor[1] = (int) ( coOriginLightColor.g * 255.f );
            iOriginLightColor[2] = (int) ( coOriginLightColor.b * 255.f );

            iOriginLightColor[2] = ((CDXUTSlider*)pControl)->GetValue();

            CDXUTEditBox* pEditBox_OriginLightColor	= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_ORIGIN_FOG_COLOR );
            pEffectTool->SetTextIntArrayToEditBox( pEditBox_OriginLightColor,  (int*)&iOriginLightColor, 3 );


            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);
            }	
            pEffectTool->SetFogColor(iOriginLightColor[0]/255.0f, iOriginLightColor[1]/255.0f, iOriginLightColor[2]/255.0f);

        } break;

    case CX2EffectToolMain::IDC_SLIDER_FOG_NEAR_X:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FOG_NEAR_X );
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

    case CX2EffectToolMain::IDC_SLIDER_FOG_FAR_X:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FOG_FAR_X );
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

    case CX2EffectToolMain::IDC_SLIDER_FOG_NEAR_Y:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FOG_NEAR_Y );
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

    case CX2EffectToolMain::IDC_SLIDER_FOG_FAR_Y
        :
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FOG_FAR_Y );
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

    case CX2EffectToolMain::IDC_SLIDER_FOG_DENSITY:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FOG_DENSITY );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", (float)iPos * 0.01f );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolAllObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolAllObject[i];
                pWorldToolObject->SetFogDensity((float)iPos * 0.01f);
            }	

        } break;

    case CX2EffectToolMain::IDC_SLIDER_FOG_DENSITY_OBJECT:
        {
            WCHAR wszTextOut[512] = L"";
            int iPos;            

            CDXUTEditBox* pEditBox= pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FOG_DENSITY_OBJECT );
            iPos = ((CDXUTSlider*)pControl)->GetValue();
            StringCchPrintf( wszTextOut, sizeof(wszTextOut), L"%.2f", (float)iPos * 0.01f );	
            pEditBox->SetText( wszTextOut );

            for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
            {
                CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
                pWorldToolObject->SetFogDensity((float)iPos * 0.01f);
            }	

        } break;
        
#ifdef ADD_ALPHATESTENABLE
	case CX2EffectToolMain::IDC_CHECK_IS_NOT_CULL:
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
	case CX2EffectToolMain::IDC_CHECK_IS_ALPHATEST_ENABLE:
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

	case CX2EffectToolMain::IDC_CHECK_IS_ALPHA_OBJECT:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetIsAlphaObject( bChecked );
			}			

		} break;

	case CX2EffectToolMain::IDC_CHECK_ADDITIVE_BLEND:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetAdditiveBlend( bChecked );
			}			
		} break;

	case CX2EffectToolMain::IDC_CHECK_CAN_HIDE:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetCanHide( bChecked );
			}			
		} break;

	case CX2EffectToolMain::IDC_CHECK_NOT_RENDER:
		{
			bool bChecked = ((CDXUTCheckBox*)pControl)->GetChecked();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetNotRender( bChecked );
			}			
		} break;

	case CX2EffectToolMain::IDC_CHECK_USE_WORLD_COLOR:
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


	case CX2EffectToolMain::IDC_EDIT_POS_X:
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
				} break;

			}
		} break;

	case CX2EffectToolMain::IDC_EDIT_POS_Y:
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
				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_EDIT_POS_Z:
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
				} break;
			}
		} break;


	case CX2EffectToolMain::IDC_EDIT_SCALE_X:
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
				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_EDIT_SCALE_Y:
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
				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_EDIT_SCALE_Z:
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
				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_COMBO_BILLBOARD:
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

	case CX2EffectToolMain::IDC_COMBO_LAYER:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();

			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetLayer( (int)pItem->pData );
			}

		} break;


	case CX2EffectToolMain::IDC_COMBO_OBJECT_TYPE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();

			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetObjectType( (CX2WorldObject::OBJECT_TYPE)(int)pItem->pData );
			}		

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
						CEffectToolDialog* pDialog = pEffectTool->GetDialog(CX2EffectToolMain::DLG_OBJECT_PROPERTY);
						if( pDialog != NULL )
						{
							WCHAR wszMSG[10] = L"";
							pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetText(wszMSG);
							pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetEnabled(true);
						}				
						break;
					}					
				default:
					{
						CEffectToolDialog* pDialog = pEffectTool->GetDialog(CX2EffectToolMain::DLG_OBJECT_PROPERTY);
						if( pDialog != NULL )
						{
							std::wstring wstrMSG = pItem->strText;
							wstrMSG += L" 지원안함";
							pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetText(wstrMSG.c_str(), true);
							pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_TEX0_UV_SPEED )->SetEnabled(false);
						}				
					}				
					break;
				}
			}
			 
		} break;
		
	case CX2EffectToolMain::IDC_EDIT_ROTATION_SPEED: // sky, cloud
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

				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_EDIT_OCCATIONAL_EVENT_TIME:
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

				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_EDIT_OCCATIONAL_EVENT_RATE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					bool bUseTwoNumbersForRandom = false;

					if( true == bUseTwoNumbersForRandom )
					{
						int iNumbers[2] = { 0, };
						pEffectTool->ParseIntArrayFromEditBox( ((CDXUTEditBox*)pControl), (int*) &iNumbers[0], 2 );
						pEffectTool->SetTextIntArrayToEditBox( ((CDXUTEditBox*)pControl), (int*) &iNumbers[0], 2 );

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

				} break;
			}
		} break;


	case CX2EffectToolMain::IDC_EDIT_ROTATION_DEGREE:
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

				} break;
			}

		} break;

	case CX2EffectToolMain::IDC_EDIT_TEX0_UV_SPEED:
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

				} break;
			}

		} break;

	case CX2EffectToolMain::IDC_EDIT_SKIN_ANIM_NAME:
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

				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_COMBO_SKIN_ANIM_PLAY_TYPE:
		{
			DXUTComboBoxItem *pItem = ((CDXUTComboBox*)pControl)->GetSelectedItem();
			for( UINT i=0; i<vecpWorldToolObject.size(); i++ )
			{
				CWorldToolObject* pWorldToolObject = vecpWorldToolObject[i];
				pWorldToolObject->SetPlayType( (CX2WorldObjectSkinMesh::WORLD_OBJECT_SKIN_MESH_PLAY_TYPE) (int)pItem->pData );
			}

		} break;

	//{{ 09.03.02 김태완
	case CX2EffectToolMain::IDC_EDIT_HIDE_DISTANCE:
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
				} break;
			}
		} break;
	//}}
	//{{ 09.03.02 김태완

	case CX2EffectToolMain::IDC_CHECK_FORCE_SHOW:
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
void CALLBACK OnDialogEvent_DLG_EFFECT_SET_LIST( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;

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


	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL == pDialog )
		return;

	switch( nControlID )
	{
	//////////////////////////////////////////////////////////////////////////
	//LIST
	case CX2EffectToolMain::IDC_LIST_EFFECT_SET:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					DXUTListBoxItem* pItem = (static_cast<CDXUTListBox*>(pControl))->GetSelectedItem();
					if( pItem != NULL )						
					{
						pEffectTool->UpdateEffectSetMember( pItem->strText );

						// 이펙트셋 이름 EDIT 박스에 채우기.
						CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_NAME_CHANGE );
						if( NULL != pEditBox )
						{
							pEditBox->SetText( pItem->strText );
						}
						pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SAVE_AS );
						if( NULL != pEditBox )
						{
							pEditBox->SetText( pItem->strText );
						}

						// 이펙트셋 주석
						pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_DESC );
						if( NULL != pEditBox )
						{
							pEditBox->SetText( pEffectTool->GetEffectSetDesc( pItem->strText ).c_str() );
						}

						// 내부 설정값 갱신.
						pEffectTool->UpdateSelectedEffectSetData();
						pEffectTool->UpdateSelectedEffectData();

						//메쉬 리스트 선택 해제
						pEffectTool->UnSelectMeshList();

						pEffectTool->SetDialogEvent_DelayTime(1.f);

						pEffectTool->SetReadyEffectSetResource(false);
					}
				} break; //case EVENT_LISTBOX_SELECTION:
			case EVENT_LISTBOX_ITEM_RCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
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
					}
				} break;//case EVENT_LISTBOX_ITEM_RCLK:
			} break;
		} break;
	case CX2EffectToolMain::IDC_LIST_SKIN_ANIM_NAME:
		{
			switch( nEvent )
			{				
			case EVENT_LISTBOX_SELECTION:
				{
					if( NULL != pEffectTool->m_pCharacterSkinAnim )
					{
						DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();
						if( NULL != pItem )
						{
							pEffectTool->m_pCharacterSkinAnim->ChangeAnim( pItem->strText );
							pEffectTool->m_pCharacterSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
						}
					}				
				} break;

			case EVENT_LISTBOX_ITEM_RCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
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
					}
				} break;//case EVENT_LISTBOX_ITEM_RCLK:
			} break;
		} break;
	//EDIT
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_FIND:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_FIND );
					if( NULL != pEditBox )
					{
						pEffectTool->On_IDC_BUTTON_EFFECT_SET_FIND( pEditBox->GetText() );
					}
				} break;
			}
		} break;
	// 이펙트셋 주석
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_DESC:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_DESC );
					if( NULL != pEditBox )
					{
						if( NULL != g_pData->GetGameEffectSet() &&
							NULL != pEffectTool->GetSelectedEffectSetData() )
						{
							g_pData->GetGameEffectSet()->AddEffectSet_Description( 	pEffectTool->GetSelectedEffectSetData()->m_wstrEffectSetName
																					, pEditBox->GetText() );
						}
					}
				} break;
			}
		} break;

	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_DESC_FIND:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_DESC_FIND );
					if( NULL != pEditBox )
					{
						pEffectTool->On_IDC_BUTTON_EFFECT_SET_DESC_FIND( pEditBox->GetText() );
					}
				} break;
			}
		} break;
	//BUTTON
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_NAME_CHANGE:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_NAME_CHANGE );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_EFFECT_SET_NAME_CHANGE( pEditBox->GetText() );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_FIND:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_FIND );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_EFFECT_SET_FIND( pEditBox->GetText() );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_DESC_FIND:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_DESC_FIND );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_EFFECT_SET_DESC_FIND( pEditBox->GetText() );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_DEL:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_DEL();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_ADD:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_ADD );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_EFFECT_SET_ADD( pEditBox->GetText() );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_MERGE:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_ADD );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_EFFECT_SET_MERGE( pEditBox->GetText() );
			}
		} break;

	case CX2EffectToolMain::IDC_BUTTON_FILE_OPEN_WORLD:
		{
			ShowWindow(g_hDlgLineMap, SW_HIDE);
			DestroyWindow(g_hDlgLineMap);
			g_hDlgLineMap = NULL;
			pEffectTool->m_bShowLineMapModifier = false;			

			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pEffectTool->LoadWorld( ofn.lpstrFile );
				pEffectTool->LoadLineMapInfo();
				g_pKTDXApp->SkipFrame();
			}
		} break;	
	case CX2EffectToolMain::IDC_BUTTON_LOAD_EFFECT_SET_WHOLE:
		{
			pEffectTool->On_IDC_BUTTON_LOAD_EFFECT_SET_WHOLE();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_LOAD_DAMAGE_EFFECT:
		{
			pEffectTool->On_IDC_BUTTON_LOAD_DAMAGE_EFFECT();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_SAVE_EFFECT_SET_WHOLE:
		{
			pEffectTool->On_IDC_BUTTON_SAVE_EFFECT_SET_WHOLE();				
		} break;
	case CX2EffectToolMain::IDC_BUTTON_FILE_OPEN_NPC_SKINANIM:
		{
			ofn.lpstrFilter		= L"X file\0*.x\0";

			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pEffectTool->CreateCharacterSkinMeshFromFile( ofn.lpstrFile );
				g_pKTDXApp->SkipFrame();
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_SAVE_AS:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SAVE_AS );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_EFFECT_SET_SAVE_AS( pEditBox->GetText() );
			}
		}
	}
}

void CALLBACK OnDialogEvent_DLG_EFFECT_SET_MEMBER( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	if( NULL == pControl )
		return;

	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if ( NULL == pDialog )
		return;

	switch( nControlID )
	{
	//COMBO
	case CX2EffectToolMain::IDC_COMBO_EFFECT_SET_TYPE:
		{
			CDXUTComboBox* pComboBox = static_cast<CDXUTComboBox*>(pControl);
			CX2EffectSet::EFFECT_SET_TIMER_TYPE eEffectType = 
				static_cast<CX2EffectSet::EFFECT_SET_TIMER_TYPE>( reinterpret_cast<int>(pComboBox->GetSelectedData()) );

			pEffectTool->On_IDC_COMBO_EFFECT_SET_TYPE(eEffectType);
		} break;

	//CHECK
	case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_AFFECTED_BY_UNIT_SCALE:
		{
			pEffectTool->On_IDC_CHECK_EFFECT_SET_AFFECTED_BY_UNIT_SCALE( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
		} break;
	//EDIT
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_DELETE_SHAKE_COUNT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iValue = static_cast<int>(_wtoi( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->On_IDC_EDIT_EFFECT_SET_DELETE_SHAKE_COUNT( iValue );
				} break;
			}
		} break;
	//LIST
	case CX2EffectToolMain::IDC_LIST_EFFECT_SET_PARTICLE:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					if( NULL != pListBox )
					{
						DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
						if( pItem != NULL )						
						{
							pEffectTool->On_IDC_LIST_EFFECT_SET_PARTICLE_SELECTION( pItem->strText );
						}
					}
				} break;
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					// 몇번 파티클 파일에 있는 지 확인하기.
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					if( NULL != pListBox )
					{
						DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
						if( pItem != NULL )						
						{
							pEffectTool->On_IDC_LIST_EFFECT_SET_PARTICLE_DBLCLK( pItem->strText );
						}
					}

				} break;
			case EVENT_LISTBOX_ITEM_RCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
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
					}
				} break;//case EVENT_LISTBOX_ITEM_RCLK:
			}
		} break;
	case CX2EffectToolMain::IDC_LIST_EFFECT_SET_MESH:
		{
			switch( nEvent )
			{
				case EVENT_LISTBOX_SELECTION:
				{
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					if( NULL != pListBox )
					{
						DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
						if( pItem != NULL )						
						{
							pEffectTool->On_IDC_LIST_EFFECT_SET_MESH_SELECTION(pItem->strText);
						}
					}
				} break;
				case EVENT_LISTBOX_ITEM_DBLCLK:
					{
						// 메시플레이어의 어느 위치에 있는지 찾기
						CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
						if( NULL != pListBox )
						{
							DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
							if( pItem != NULL )						
							{
								pEffectTool->On_IDC_LIST_EFFECT_SET_MESH_DBLCLK( pItem->strText );
							}
						}
					} break;
				case EVENT_LISTBOX_ITEM_RCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
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
					}
				} break;//case EVENT_LISTBOX_ITEM_RCLK:
			}
		} break;
	case CX2EffectToolMain::IDC_LIST_EFFECT_SET_CAMERA_SHAKE:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					pEffectTool->UpdateSelectedCameraShake(true);
					int iSelectedIndex = (static_cast<CDXUTListBox*>(pControl))->GetSelectedIndex();
					pEffectTool->UpdateEffectSetCameraData( iSelectedIndex );
				}
				break;
			} break;
		} break;
	//BUTTON
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_PARTICLE:
		{
			pEffectTool->ChangeToolMode( CX2EffectToolMain::MODE_EFFECT_SET_PARTICLE_MESH_EDIT );
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_CAMERA:
		{
			pEffectTool->ChangeToolMode( CX2EffectToolMain::MODE_EFFECT_SET_CAMERA_EDIT );
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_HIDE:
		{
			pEffectTool->ChangeToolMode( CX2EffectToolMain::MODE_EFFECT_SET );
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_LIST_SHOW_TOGGLE:
		{
			pEffectTool->ToggleShowEffectSetList();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_CAMERA_ADD:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_CAMERA_ADD();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_CAMERA_DEL:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_CAMERA_DEL();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_PARTICLE_ADD:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_PARTICLE_ADD();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_PARTICLE_DEL:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_PARTICLE_DEL();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_MESH_ADD:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_MESH_ADD();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_CREATE_EFFECT_SET_BY_DAMAGE_EFFECT:
		{
			pEffectTool->On_IDC_BUTTON_CREATE_EFFECT_SET_BY_DAMAGE_EFFECT();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_MESH_DEL:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_MESH_DEL();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_CUSTOM_PARTICLE_ADD:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_CUSTOM_PARTICLE_ADD();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_EFFECT_SET_CUSTOM_MESH_ADD:
		{
			pEffectTool->On_IDC_BUTTON_EFFECT_SET_CUSTOM_MESH_ADD();
		} break;
	}
}

void CALLBACK OnDialogEvent_DLG_EFFECT_SET_PARTICLE_MESH_EDIT( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_PARTICLE_MESH_EDIT );
	if( NULL == pDialog )
		return;

	switch( nControlID )
	{
		//edit
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_START_TIME:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_START_TIME( fValue );
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_BONE_NAME:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_BONE_NAME( ( static_cast<CDXUTEditBox*>(pControl) )->GetText());
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_RIDING_BONE_NAME:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_RIDING_BONE_NAME( ( static_cast<CDXUTEditBox*>(pControl) )->GetText());
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_POSITION_OFFSET_X:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_X( fValue );
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Y:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Y( fValue );
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Z:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Z( fValue );
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_HYPER_NAME:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_HYPER_NAME( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() );
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_WEAPON_BONE_NAME:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_NAME( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() );
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_WEAPON_BONE_INDEX:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						int iValue = static_cast<int>(_wtoi( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_INDEX( iValue );
					} break;
				}
			} break;
		case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_WEAPON_BONE_MODEL_INDEX:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						int iValue = static_cast<int>(_wtoi( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
						pEffectTool->ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_MODEL_INDEX( iValue );
					} break;
				}
			} break;

#ifdef MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE // 오현빈
		case CX2EffectToolMain::IDC_EDIT_EFFECT_LIMIT_DISTANCE:
			{
				switch( nEvent )
				{
				case EVENT_EDITBOX_STRING:
					{
						int iValue = static_cast<int>(_wtoi( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
						pEffectTool->ON_IDC_EDIT_EFFECT_LIMIT_DISTANCE( iValue );
					} break;
				}
			} break;

#endif //MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE 
		//check
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_TRACE:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_TRACE( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_TRACE_MATRIX:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_TRACE_MATRIX( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_TRACE_MORE:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_TRACE_MORE( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_REVERSE_Y:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_REVERSE_Y( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_USE_LAND_POSITION:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_USE_LAND_POSITION( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_APPLY_INSTANCE_SCALE:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_APPLY_INSTANCE_SCALE( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_START_WHEN_TRACE_TARGET_DEAD:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_START_WHEN_TRACE_TARGET_DEAD( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_PASSIVE_EFFECT:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_PASSIVE_EFFECT( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_USE_SUB_ATTACK_LIST_SET:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_USE_SUB_ATTACK_LIST_SET( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
#ifdef MODIFY_EFFECT_SET_TRACE_BONE // 오현빈
		case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH:
			{
				pEffectTool->ON_IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
			} break;
#endif //MODIFY_EFFECT_SET_TRACE_BONE 
	}
}

void CALLBACK OnDialogEvent_DLG_EFFECT_SET_PARTICLE_MESH_EDIT2( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_PARTICLE_MESH_EDIT2 );

	if( NULL == pDialog )
		return;

	switch( nControlID )
	{
		//combo
	case CX2EffectToolMain::IDC_COMBO_EFFECT_SET_EFFECT_TYPE:
		{
			CDXUTComboBox* pComboBox  = static_cast<CDXUTComboBox*>(pControl);
			if( NULL != pComboBox )
			{
				CX2EffectSet::EFFECT_TYPE eEffectType = 
					static_cast<CX2EffectSet::EFFECT_TYPE>( reinterpret_cast<int>(pComboBox->GetSelectedData()) );
				pEffectTool->ON_IDC_COMBO_EFFECT_SET_EFFECT_TYPE( eEffectType );
			}
		} break;
		//edit
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_ROTATE_X:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_ROTATE_X( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_ROTATE_Y:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_ROTATE_Y( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_ROTATE_Z:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_ROTATE_Z( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SCALE_X:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_SCALE_X( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SCALE_Y:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_SCALE_Y( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SCALE_Z:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_SCALE_Z( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_X:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_X( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Y:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Y( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Z:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Z( fValue );
				} break;
			}
		} break;
	//LIST
	case CX2EffectToolMain::IDC_LIST_MOTION_BONE_LIST:
		{
			switch( nEvent )
			{						
			//case EVENT_LISTBOX_ITEM_DBLCLK:
			case EVENT_LISTBOX_ITEM_RCLK:
			//case EVENT_LISTBOX_ITEM_DBRCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
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
					}
				} break;
			case EVENT_LISTBOX_SELECTION:
				{
// 					if( NULL != pWorldTool->m_pCharacterSkinAnim )
// 					{
// 						DXUTListBoxItem* pItem = ((CDXUTListBox*)pControl)->GetSelectedItem();
// 						if( NULL != pItem )
// 						{
// 							pWorldTool->m_pCharacterSkinAnim->ChangeAnim( pItem->strText );
// 							pWorldTool->m_pCharacterSkinAnim->Play( CKTDGXSkinAnim::XAP_ONE_WAIT );
// 						}
// 					}				
				} break;
			} break;
		} break;
	}
}

void CALLBACK OnDialogEvent_DLG_EFFECT_SET_CAMERA_EDIT( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_CAMERA_EDIT );

	if( NULL == pDialog )
		return;

	switch( nControlID )
	{
		//combo
	case CX2EffectToolMain::IDC_COMBO_EFFECT_SET_CAMERA_SHAKE_TYPE:
		{
			CDXUTComboBox* pComboBox = static_cast<CDXUTComboBox*>(pControl);
			if( NULL != pComboBox )
			{
				CKTDGCamera::SHAKE_TYPE eShakeType = 
					static_cast<CKTDGCamera::SHAKE_TYPE>( reinterpret_cast<int>(pComboBox->GetSelectedData()) );
				pEffectTool->ON_IDC_COMBO_EFFECT_SET_CAMERA_SHAKE_TYPE( eShakeType );
			}
		} break;
		//edit
	case CX2EffectToolMain::IDC_CHECK_EFFECT_SET_FIXED_LOOCK_VEC:
		{
			pEffectTool->ON_IDC_CHECK_EFFECT_SET_FIXED_LOOCK_VEC( static_cast<CDXUTCheckBox*>(pControl)->GetChecked() );
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_CAMERA_START_TIME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_CAMERA_START_TIME( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_LIMIT_DISTANCE:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iValue = static_cast<int>(_wtoi( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_LIMIT_DISTANCE( iValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_TWO_DIR_GAP:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_TWO_DIR_GAP( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_TWO_DIR_TIME:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_TWO_DIR_TIME( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iValue = static_cast<int>(_wtoi( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT( iValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					int iValue = static_cast<int>(_wtoi( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT( iValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX( fValue );
				} break;
			}
		} break;
	case CX2EffectToolMain::IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					float fValue = static_cast<float>(_wtof( ( static_cast<CDXUTEditBox*>(pControl) )->GetText() ));
					pEffectTool->ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX( fValue );
				} break;
			}
		} break;
	}
}
void CALLBACK OnDialogEvent_DLG_PARTICLE_LIST( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"txt\0*.txt\0lua script\0*.lua\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );

	switch( nControlID )
	{
	//editor
	case CX2EffectToolMain::IDC_EDIT_PARTICLE_LIST_FIND:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_PARTICLE_LIST_FIND );
					if( NULL != pEditBox )
					{
						pEffectTool->On_IDC_BUTTON_PARTICLE_LIST_FIND( pEditBox->GetText() );
					}
				} break;
			}
		} break;
	//combo
	case CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE:
		{
			CDXUTComboBox* pComboBox = static_cast<CDXUTComboBox*>(pControl);
			UINT uiIndex = reinterpret_cast<int>(pComboBox->GetSelectedData()) ;
			
			pEffectTool->UpdateParticleListByIndex( uiIndex );
		} break;
	//button
	case CX2EffectToolMain::IDC_BUTTON_PARTICLE_ADD_OTHER_FILE:
		{
			pEffectTool->On_IDC_BUTTON_PARTICLE_ADD_OTHER_FILE();
		}
	case CX2EffectToolMain::IDC_BUTTON_PARTICLE_LIST_FIND:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_PARTICLE_LIST_FIND );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_PARTICLE_LIST_FIND( pEditBox->GetText() );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_LOAD_BASE_PARTICLE_FILE:
		{
			pEffectTool->ON_IDC_BUTTON_LOAD_BASE_PARTICLE_FILE();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_LOAD_PARTICLE_FILE:
		{
			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pEffectTool->ON_IDC_BUTTON_LOAD_PARTICLE_FILE( ofn.lpstrFile );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_SAVE_PARTICLE_FILE:
		{
			pEffectTool->ON_IDC_BUTTON_SAVE_PARTICLE_FILE();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_DEL_PARTICLE:
		{
			pEffectTool->On_IDC_BUTTON_DEL_PARTICLE();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_INIT_PARTICLE_FILE:
		{
			pEffectTool->ON_IDC_BUTTON_INIT_PARTICLE_FILE();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_PARTICLE_BASE_EDITOR:
		{
			pEffectTool->ON_IDC_BUTTON_PARTICLE_BASE_EDITOR();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_PARTICLE_ETC_EDITOR:
		{
			pEffectTool->ON_IDC_BUTTON_PARTICLE_ETC_EDITOR();
		} break;
	//list
	case CX2EffectToolMain::IDC_LIST_PARTICLE_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{	//미리보기
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
					if( pItem != NULL )						
					{
						// 다이얼로그 갱신
						CKTDGParticleSystem::CParticleEventSequence* pParticleSequence = 
							pEffectTool->GetParticleByNameAndIndex( pItem->strText );
						if( NULL != pParticleSequence )
						{
							pEffectTool->UpdateDialogParticleEditor( pParticleSequence );

							// 범위형태로 지정된 값 갱신.
							pEffectTool->SetRandomValue( pParticleSequence );
						}

						// 파티클 미리보기
						pEffectTool->PreViewParticle( pItem->strText );
					}
				} break;
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{	//이펙트셋에 파티클 추가
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
					if( pItem != NULL )						
						pEffectTool->AddParticleAtCurrentEffectset( pItem->strText );
				} break;

			case EVENT_LISTBOX_ITEM_RCLK:
				{
					// 1. 클릭된 항목을 클립보드에 넣어주고 
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
					}
				} break;//case EVENT_LISTBOX_ITEM_RCLK:
			}
		} break;
	}
}
void CALLBACK OnDialogEvent_DLG_PARTICLE_LIST_FIND_TEXTURE( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST_FIND_TEXTURE );

	switch( nControlID )
	{
		//editor
	case CX2EffectToolMain::IDC_EDIT_FIND_TEXTURE_NAME:
		{
 			switch( nEvent )
 			{
 			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FIND_TEXTURE_NAME );
					if( NULL != pEditBox )
					{
						pEffectTool->On_IDC_BUTTON_FIND_TEXTURE( pEditBox->GetText() );
					}
 				} break;
 			}
		} break;
		//button
	case CX2EffectToolMain::IDC_BUTTON_FIND_TEXTURE:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_FIND_TEXTURE_NAME );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_FIND_TEXTURE( pEditBox->GetText() );
			}
		}
	case CX2EffectToolMain::IDC_LIST_PARTICLE_LIST_FIND_TEXTURE:
		{
 			switch( nEvent )
 			{
 				case EVENT_LISTBOX_ITEM_DBLCLK:
				{
					// 몇번 파티클 파일에 있는 지 확인하기.
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					if( NULL != pListBox )
					{
						DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
						if( pItem != NULL )						
						{
							pEffectTool->On_IDC_LIST_EFFECT_SET_PARTICLE_DBLCLK( pItem->strText );
						}
					}
				} break;
				case EVENT_LISTBOX_ITEM_RCLK:
					{
						// 1. 클릭된 항목을 클립보드에 넣어주고 
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
						}
					} break;//case EVENT_LISTBOX_ITEM_RCLK:
			}
		} break;
	}
}
void CALLBACK OnDialogEvent_DLG_MESH_LIST( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	OPENFILENAME ofn;        // common dialog box structure
	WCHAR wszFileName[512];  // path까지 포함한 파일 이름

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize		= sizeof(ofn);
	ofn.hwndOwner		= g_pKTDXApp->GetHWND(); 
	ofn.lpstrFile		= (LPWSTR)wszFileName;
	ofn.lpstrFile[0]	= '\0';
	ofn.nMaxFile		= sizeof(wszFileName);
	ofn.lpstrFilter		= L"txt\0*.txt\0lua script\0*.lua\0";
	ofn.nFilterIndex	= 1;
	ofn.lpstrFileTitle	= NULL;
	ofn.nMaxFileTitle	= 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	CX2EffectToolMain* pEffectTool = (CX2EffectToolMain*) pUserContext;
	CEffectToolDialog* pDialog = pEffectTool->GetDialog( CX2EffectToolMain::DLG_MESH_LIST );

	switch( nControlID )
	{
		//editor
	case CX2EffectToolMain::IDC_EDIT_MESH_LIST_FIND:
		{
			switch( nEvent )
			{
			case EVENT_EDITBOX_STRING:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_MESH_LIST_FIND );
					if( NULL != pEditBox )
					{
						pEffectTool->On_IDC_BUTTON_MESH_LIST_FIND( pEditBox->GetText() );
					}
				} break;
			}
		} break;
		//button
	case CX2EffectToolMain::IDC_BUTTON_MESH_LIST_FIND:
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_MESH_LIST_FIND );
			if( NULL != pEditBox )
			{
				pEffectTool->On_IDC_BUTTON_MESH_LIST_FIND( pEditBox->GetText() );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_LOAD_BASE_MESH_FILE:
		{
			pEffectTool->ON_IDC_BUTTON_LOAD_MESH_FILE( L"GameMajorXMeshPlayer.txt" ) ;
		} break;
	case CX2EffectToolMain::IDC_BUTTON_SAVE_MESH_FILE:
		{
			pEffectTool->ON_IDC_BUTTON_SAVE_MESH_FILE() ;
		} break;
	case CX2EffectToolMain::IDC_BUTTON_DEL_MESH:
		{
			pEffectTool->On_IDC_BUTTON_DEL_MESH();
		} break;
	case CX2EffectToolMain::IDC_BUTTON_LOAD_MESH_FILE:
		{
			if( TRUE == GetOpenFileName( &ofn ) )
			{
				pEffectTool->ON_IDC_BUTTON_LOAD_MESH_FILE( ofn.lpstrFile );
			}
		} break;
	case CX2EffectToolMain::IDC_BUTTON_MESH_BASE_EDITOR:
		{
			pEffectTool->ON_IDC_BUTTON_MESH_BASE_EDITOR();
		}
		//list
	case CX2EffectToolMain::IDC_LIST_MESH_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{	//미리보기
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
					if( pItem != NULL )						
					{
						pEffectTool->PreVieweXMesh( pItem->strText );

						// 다이얼로그 갱신
						CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet =
							pEffectTool->GetMeshByName( pItem->strText );
						pEffectTool->UpdateDialogMeshEditor( pMeshTemplet );
					}
				} break;
			case EVENT_LISTBOX_ITEM_DBLCLK:
				{	//이펙트셋에 파티클 추가
					CDXUTListBox* pListBox = static_cast<CDXUTListBox*>(pControl);
					DXUTListBoxItem* pItem = pListBox->GetSelectedItem();
					if( pItem != NULL )						
						pEffectTool->AddMeshAtCurrentEffectset( pItem->strText );
				} break;
			}
		} break;
	}
}

void CX2EffectToolMain::FinalDialog()
{
	for( unsigned i=0; i<m_vecpDialog.size(); i++ )
	{
		SAFE_DELETE( m_vecpDialog[i] );
	}
	m_vecpDialog.clear();
}

void CX2EffectToolMain::ResetDialogAtModeChange()
{
	for( int i = DLG_OBJECT_PROPERTY; i < DLG_END; ++i )
		m_vecpDialog[i]->SetEnable(false);

	switch( m_iToolMode )
	{
	case MODE_EFFECT_SET:
		{
			m_vecpDialog[DLG_EFFECT_SET_LIST]->SetEnable(m_bShowEffectSetList);
			m_vecpDialog[DLG_EFFECT_SET_MEMBER]->SetEnable(true);
		} break;
	case MODE_EFFECT_SET_PARTICLE_MESH_EDIT:
		{
			m_vecpDialog[DLG_EFFECT_SET_LIST]->SetEnable(m_bShowEffectSetList);
			m_vecpDialog[DLG_EFFECT_SET_MEMBER]->SetEnable(true);
			m_vecpDialog[DLG_EFFECT_SET_PARTICLE_MESH_EDIT]->SetEnable(true);
			m_vecpDialog[DLG_EFFECT_SET_PARTICLE_MESH_EDIT2]->SetEnable(true);
		} break;
	case MODE_EFFECT_SET_CAMERA_EDIT:
		{
			m_vecpDialog[DLG_EFFECT_SET_LIST]->SetEnable(m_bShowEffectSetList);
			m_vecpDialog[DLG_EFFECT_SET_MEMBER]->SetEnable(true);
			m_vecpDialog[DLG_EFFECT_SET_CAMERA_EDIT]->SetEnable(true);
		} break;
	case MODE_EFFECT_SET_PARTICLE_ADD:
		{
			m_vecpDialog[DLG_EFFECT_SET_LIST]->SetEnable(m_bShowEffectSetList);
			m_vecpDialog[DLG_EFFECT_SET_MEMBER]->SetEnable(true);
			m_vecpDialog[DLG_PARTICLE_LIST]->SetEnable(true);
			m_vecpDialog[DLG_PARTICLE_LIST_FIND_TEXTURE]->SetEnable(true);
		} break;
	case MODE_EFFECT_SET_MESH_ADD:
		{
			m_vecpDialog[DLG_EFFECT_SET_LIST]->SetEnable(m_bShowEffectSetList);
			m_vecpDialog[DLG_EFFECT_SET_MEMBER]->SetEnable(true);
			m_vecpDialog[DLG_MESH_LIST]->SetEnable(true);
		} break;
	}
	UpdateDialog();
}

void CX2EffectToolMain::UpdateDialog()
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

void CX2EffectToolMain::InitDialog()
{
	// initialize UI
	// 벡터로 관리하기 때문에 순서 보장 필요합니다.
	InitDialogObjectProperty();			//DLG_OBJECT_PROPERTY,


	InitDialogEffectSet_List();			/// DLG_EFFECT_SET_LIST,
	InitDialogEffectSet_Member();		/// DLG_EFFECT_SET_MEMBER,			//파티클, 메쉬, 카메라
	InitDialogEffectSet_ParticleMeshEdit();	/// DLG_EFFECT_SET_PARTICLE_MESH_EDIT,
	InitDialogEffectSet_ParticleMeshEdit2();/// DLG_EFFECT_SET_PARTICLE_MESH_EDIT2
	InitDialogEffectSet_CameraEdit();	/// DLG_EFFECT_SET_CAMERA_EDIT,

	InitDialogParticle_List();				/// DLG_PARTICLE_LIST,
	InitDialogParticle_List_Find_Texture();	/// DLG_PARTICLE_LIST_FIND_TEXTURE,
	InitDialogMesh_List();					// DLG_MESH_LIST

    g_pEffectToolMain = this;
}
void CX2EffectToolMain::InitDialogObjectProperty()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 220;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 30;

	pDlg = new CEffectToolDialog;		// DLG_OBJECT_PROPERTY
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
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

}
void CX2EffectToolMain::InitDialogEffectSet_List()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 320;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 300;
	int iHeight = 20;

	pDlg = new CEffectToolDialog;		// DLG_EFFECT_SET_LIST
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_SET_LIST, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );

	pDlg->AddButton( IDC_BUTTON_FILE_OPEN_WORLD, L"월드 스크립트 열기", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_LOAD_EFFECT_SET_WHOLE, L"이펙트셋 열기", iX, iY, iWidth-150, iHeight );
	pDlg->AddButton( IDC_BUTTON_LOAD_DAMAGE_EFFECT, L"데미지이펙트 열기", iWidth-140, iY, iWidth-150, iHeight );
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_SAVE_EFFECT_SET_WHOLE, L"이펙트셋 저장", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;


	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_DESC, L"", iX, iY, iWidth, iHeight + 10, false, &pEditBox );   
	iY += iHeight + 10;

	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_DESC_FIND, L"", iX, iY, iWidth - 70, iHeight + 10, false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_DESC_FIND, L"주석 찾기", iX + iWidth - 60 , iY, 60 , iHeight + 10 );
	iY += (iHeight + 10) + 3;

	pDlg->AddListBox( IDC_LIST_EFFECT_SET, iX, iY, iWidth, iHeight * 23, 0, &pListBox );	
	pListBox->SetScrollBarWidth( 23 );
	iY += iHeight * 23 + 3;

	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_FIND, L"", iX, iY, iWidth - 40, iHeight + 10, false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_FIND, L"찾기", iX + iWidth - 30 , iY, 30 , iHeight + 10 );
	iY += (iHeight + 10) + 3;

	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_NAME_CHANGE, L"", iX, iY, iWidth - 80, iHeight + 10 , false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_NAME_CHANGE, L"변경", iX + iWidth - 70 , iY, 30, iHeight + 10 );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_DEL, L"제거", iX + iWidth - 30 , iY, 30 , iHeight + 10 );
	iY += (iHeight + 10) + 3;

	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_SAVE_AS, L"", iX, iY, iWidth - 80, iHeight + 10 , false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_SAVE_AS, L"다른이름저장", iX + iWidth - 70 , iY, 70, iHeight + 10 );
	iY += (iHeight + 10) + 3;


	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_ADD, L"", iX, iY, iWidth - 80, iHeight + 10 , false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_ADD, L"추가", iX + iWidth - 70 , iY, 30, iHeight + 10 );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_MERGE, L"통합", iX + iWidth - 30 , iY, 30 , iHeight + 10 );
	iY += iHeight + 10;
	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"모션 목록", iX, iY, (iWidth - 80), iHeight );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_SKIN_ANIM_NAME, iX, iY, iWidth, iHeight*8, 0, &pListBox );
	iY += iHeight*8 + 3;
	pDlg->AddButton( IDC_BUTTON_FILE_OPEN_NPC_SKINANIM, L"모션 파일 열기", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
}
void CX2EffectToolMain::InitDialogEffectSet_Member() //DLG_EFFECT_SET_MEMBER
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 220;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 30;

	pDlg = new CEffectToolDialog;		// DLG_EFFECT_Set
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_SET_MEMBER, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );

	pDlg->AddComboBox( IDC_COMBO_EFFECT_SET_TYPE, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 40 );
		pComboBox->AddItem( L"ESTT_ANIM",	(void*) CX2EffectSet::ESTT_ANIM );
		pComboBox->AddItem( L"ESTT_CUSTOM",	(void*) CX2EffectSet::ESTT_CUSTOM );
	}
	iY += iHeight + 3;

	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_AFFECTED_BY_UNIT_SCALE, L"AFFECTED_BY_UNIT_SCALE", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"DELETE_SHAKE_COUNT", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_DELETE_SHAKE_COUNT, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"파티클 이펙트" , iX, iY, (iWidth - 70), iHeight );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_PARTICLE_ADD, L"목록", iX + (iWidth - 90), iY, 50, iHeight );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_PARTICLE_DEL, L"제거", iX + (iWidth - 40), iY, 40, iHeight );
	iY += iHeight + 3;

	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_CUSTOM_PARTICLE_ADD, L"", iX, iY, iWidth - 40, iHeight , false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_CUSTOM_PARTICLE_ADD, L"추가", iX + iWidth - 30 , iY, 30 , iHeight );
	iY += iHeight + 10;


	pDlg->AddListBox( IDC_LIST_EFFECT_SET_PARTICLE, iX, iY, iWidth, iHeight * 5, 0, &pListBox );
	iY += iHeight * 5 + 3;

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"메쉬 이펙트", iX, iY, (iWidth - 70), iHeight );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_MESH_ADD, L"목록", iX + (iWidth - 90), iY, 50, iHeight );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_MESH_DEL, L"제거", iX + (iWidth - 40), iY, 40, iHeight );
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_CREATE_EFFECT_SET_BY_DAMAGE_EFFECT, L"데미지이펙트 to 이펙트셋", iX	, iY, iWidth, iHeight );
	iY += iHeight + 3;

	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_CUSTOM_MESH_ADD, L"", iX, iY, iWidth - 40, iHeight , false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_CUSTOM_MESH_ADD, L"추가", iX + iWidth - 30 , iY, 30 , iHeight );
	iY += iHeight + 10;

	pDlg->AddListBox( IDC_LIST_EFFECT_SET_MESH, iX, iY, iWidth, iHeight * 5, 0, &pListBox );
	iY += iHeight * 5 + 3;

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"카메라 쉐이크", iX, iY, (iWidth - 80), iHeight );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_CAMERA_ADD, L"추가", iX + (iWidth - 80), iY, 40, iHeight );
	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_CAMERA_DEL, L"제거", iX + (iWidth - 40), iY, 40, iHeight );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_EFFECT_SET_CAMERA_SHAKE, iX, iY, iWidth, iHeight * 5, 0, &pListBox );
	iY += iHeight * 5 + 3;

	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_PARTICLE, L"파티클&메쉬 설정(F5)", iX, iY, iWidth , iHeight );
	iY += iHeight + 3;

	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_CAMERA, L"카메라 설정(F6)", iX, iY, iWidth , iHeight );
	iY += iHeight + 3;

	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_HIDE, L"UI 숨기기(F7)", iX, iY, iWidth , iHeight );
	iY += iHeight + 3;

	pDlg->AddButton( IDC_BUTTON_EFFECT_SET_LIST_SHOW_TOGGLE, L"이펙트셋UI 보기/숨기기(F8)", iX, iY, iWidth , iHeight );
	iY += iHeight + 3;
}
void CX2EffectToolMain::InitDialogEffectSet_ParticleMeshEdit()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 220;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 30;
	int iHeightEx = 20;

	pDlg = new CEffectToolDialog;		// DLG_EFFECT_ParticleMeshEdit
	
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_SET_PARTICLE_MESH_EDIT, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );
	pDlg->SetFont( 1, L"굴림", 10, 10 );

	//pData->m_fStartAnimTime
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"START TIME", iX, iY, iWidth, iHeight );
	iY += iHeightEx + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_START_TIME, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	

	//pData->m_PositionBoneName
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"BONE NAME", iX, iY, iWidth, iHeight );
	iY += iHeightEx + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_BONE_NAME, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;

	
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"RIDING BONE NAME", iX, iY, iWidth, iHeight );
	iY += iHeightEx + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_RIDING_BONE_NAME, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;
	
	
	//pData->m_vPositionOffset
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"POSITION OFFSET", iX, iY, iWidth, iHeight );
	iY += iHeightEx + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_POSITION_OFFSET_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Z, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	

#ifdef MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LIMIT_DISTANCE", iX, iY, iWidth, iHeight );
	iY += iHeightEx + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_LIMIT_DISTANCE, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
#endif // MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE

	//pData->m_bTrace
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_TRACE, L"TRACE", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;		

#ifndef EXCLUDE_EFFECT_SET_FUNCTION
	//pData->m_bTraceMatrix
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_TRACE_MATRIX, L"TRACE_MATRIX", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	
	//pData->m_bTraceMore
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_TRACE_MORE, L"TRACE_MORE", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	
	//m_bReverseY
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_REVERSE_Y, L"REVERSE_Y", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	
	//m_bLandPosition
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_USE_LAND_POSITION, L"USE_LAND_POSITION", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	
	//m_bApplyUnitRotation
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION, L"APPLY_UNIT_ROTATION", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
	//m_bApplyUnitRotationEx
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR, L"APPLY_UNIT_ROTATION_MIRROR", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	pCheckBox->GetElement(0)->SetFont( 1 );
	iY += iHeight + 3;	
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	//m_bApplyInstanceScale
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_APPLY_INSTANCE_SCALE, L"APPLY_INSTANCE_SCALE", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	
	//m_bStartWhenTraceTargetDead
	//pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_START_WHEN_TRACE_TARGET_DEAD, L"START_WHEN_TRACE_TARGET_DEAD", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	//iY += iHeight + 3;	
	//m_bPassiveEffect.
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_PASSIVE_EFFECT, L"PASSIVE_EFFECT", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	
	//m_bUseSubAttackListSet		
	//pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_USE_SUB_ATTACK_LIST_SET, L"USE_SUB_ATTACK_LIST_SET", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );	
	//iY += iHeight + 3;	
#ifdef MODIFY_EFFECT_SET_TRACE_BONE // 오현빈
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH, L"BONE_TRACE_TARGET_MESH", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	pCheckBox->GetElement(0)->SetFont( 1 );
	iY += iHeight + 3;	
#endif // MODIFY_EFFECT_SET_TRACE_BONE

	//pData->m_EffectNameHyper
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"HYPER_NAME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_HYPER_NAME, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;

	//pData->m_PositionWeaponBoneName
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"WEAPON_BONE_NAME", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_WEAPON_BONE_NAME, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;

	//pData->m_iWeaponIndex
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"WEAPON_BONE_WEAPON_INDEX", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_WEAPON_BONE_INDEX, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;

	//pData->m_iModelIndex
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"WEAPON_BONE_MODEL_INDEX", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_WEAPON_BONE_MODEL_INDEX, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;
#endif //EXCLUDE_EFFECT_SET_FUNCTION
}
void CX2EffectToolMain::InitDialogEffectSet_ParticleMeshEdit2()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 400;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 30;

	pDlg = new CEffectToolDialog;		// DLG_EFFECT_ParticleMeshEdit
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_SET_PARTICLE_MESH_EDIT2, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );


	//pData->m_eEffectType
	pDlg->AddComboBox( IDC_COMBO_EFFECT_SET_EFFECT_TYPE, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 140 );
		pComboBox->AddItem( L"ET_INVALID",					(void*)CX2EffectSet::ET_INVALID);
		pComboBox->AddItem( L"ET_DAMAGE_EFFECT",			(void*)CX2EffectSet::ET_DAMAGE_EFFECT);
		pComboBox->AddItem( L"ET_MESH_PLAYER_UI_MAJOR",		(void*)CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR);
		pComboBox->AddItem( L"ET_MESH_PLAYER_UI_MINOR",		(void*)CX2EffectSet::ET_MESH_PLAYER_UI_MINOR);
		pComboBox->AddItem( L"ET_MESH_PLAYER_GAME_MAJOR",	(void*)CX2EffectSet::ET_MESH_PLAYER_GAME_MAJOR);
		pComboBox->AddItem( L"ET_MESH_PLAYER_GAME_MINOR",	(void*)CX2EffectSet::ET_MESH_PLAYER_GAME_MINOR);
		pComboBox->AddItem( L"ET_PARTICLE_UI_MAJOR",		(void*)CX2EffectSet::ET_PARTICLE_UI_MAJOR);
		pComboBox->AddItem( L"ET_PARTICLE_UI_MINOR",		(void*)CX2EffectSet::ET_PARTICLE_UI_MINOR);
		pComboBox->AddItem( L"ET_PARTICLE_GAME_MAJOR",		(void*)CX2EffectSet::ET_PARTICLE_GAME_MAJOR);
		pComboBox->AddItem( L"ET_PARTICLE_GAME_MINOR",		(void*)CX2EffectSet::ET_PARTICLE_GAME_MINOR);
	}
	iY += iHeight + 3;	

#ifndef EXCLUDE_EFFECT_SET_FUNCTION
	//pData->m_vRotateRel
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"ROTATE", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_ROTATE_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_ROTATE_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_ROTATE_Z, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;


	//pData->pData->m_vScale
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"SCALE", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_SCALE_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_SCALE_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_SCALE_Z, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	


	//pData->m_vScaleOnLeft
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"SCALE_ON_LEFT", iX, iY, iWidth, iHeight );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_X, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Y, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Z, L"", iX, iY, iWidth, iHeight, false, NULL );
	iY += iHeight + 3;
#endif //EXCLUDE_EFFECT_SET_FUNCTION

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"본 목록", iX, iY, (iWidth - 80), iHeight );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_MOTION_BONE_LIST, iX, iY, iWidth, iHeight*8, 0, &pListBox );
	iY += iHeight*8 + 3;
}
void CX2EffectToolMain::InitDialogEffectSet_CameraEdit()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 300;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 20;

	pDlg = new CEffectToolDialog;		// DLG_EFFECT_CameraEdit
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_EFFECT_SET_CAMERA_EDIT, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );


	pDlg->AddComboBox( IDC_COMBO_EFFECT_SET_CAMERA_SHAKE_TYPE, iX, iY, iWidth, iHeight, 0, false, &pComboBox );
	{
		pComboBox->SetDropHeight( 140 );
		pComboBox->AddItem( L"DECT_INVALID",			(void*) CKTDGCamera::DECT_INVALID);
		pComboBox->AddItem( L"DECT_UP",					(void*) CKTDGCamera::DECT_UP);
		pComboBox->AddItem( L"DECT_DOWN",				(void*) CKTDGCamera::DECT_DOWN);
		pComboBox->AddItem( L"DECT_UP_DOWN",			(void*) CKTDGCamera::DECT_UP_DOWN);
		pComboBox->AddItem( L"DECT_UP_DOWN_NO_RESET",	(void*) CKTDGCamera::DECT_UP_DOWN_NO_RESET);
		pComboBox->AddItem( L"DECT_LEFT",				(void*) CKTDGCamera::DECT_LEFT);
		pComboBox->AddItem( L"DECT_RIGHT",				(void*) CKTDGCamera::DECT_RIGHT);
		pComboBox->AddItem( L"DECT_LEFT_RIGHT",			(void*) CKTDGCamera::DECT_LEFT_RIGHT);
		pComboBox->AddItem( L"DECT_LEFT_RIGHT_NO_RESET",(void*) CKTDGCamera::DECT_LEFT_RIGHT_NO_RESET);
		pComboBox->AddItem( L"DECT_RANDOM",				(void*) CKTDGCamera::DECT_RANDOM);
	}
	iY += iHeight + 3;



	//pData->m_fStartAnimTime
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"시작 시간", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_CAMERA_START_TIME, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;


	//pData->m_iLimitDistanceSq
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"적용 거리 제한", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_LIMIT_DISTANCE, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//pData->m_fOneDirSpeed
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"한 방향_진폭", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;
	
	//pData->m_fOneDirAccel
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"한방향_가속도", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_fTwoDirGap
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"두 방향_진폭", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_GAP, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_fTwoDirTime
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"두 방향_속도", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_TIME, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_fTwoDirTimeGap
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"두 방향_방향전환 속도", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_bFixedLookVec
	pDlg->AddCheckBox( IDC_CHECK_EFFECT_SET_FIXED_LOOCK_VEC, L"시선벡터 고정 ", iX, iY, iWidth, iHeight, false, 0, false, &pCheckBox );
	iY += iHeight + 3;	


	//m_iEyeShakeCount
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"EYE_방향전환 횟수", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_EyeMoveSpeed.min
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"EYE_방향전환 속도_최소값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;
	
	//m_EyeMoveSpeed
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"EYE_방향전환 속도_최대값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_EyeMoveRange
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"EYE_방향전환 범위_최소값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_EyeMoveRange
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"EYE_방향전환 범위_최대값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_iLookAtShakeCount
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LOOK_AT_방향전환 횟수", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;
	
	//m_LookAtMoveSpeed
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LOOK_AT_방향전환 속도_최소값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;

	//m_LookAtMoveSpeed
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LOOK_AT_방향전환 속도_최대값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;
	
	//m_LookAtMoveRange
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LOOK_AT_방향전환 범위_최소값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;
	
	//m_LookAtMoveRang
	pDlg->AddStatic( IDC_STATIC_DUMMY, L"LOOK_AT_방향전환 범위_최대값", iX, iY, iWidth, iHeight );
	iY += iHeight;	
	pDlg->AddEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX, L"", iX, iY, iWidth, iHeight+10, false, NULL );
	iY += iHeight+10;
}
void CX2EffectToolMain::InitDialogParticle_List()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 300;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 20;

	pDlg = new CEffectToolDialog;		// DLG_PARTICLE_LIST
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_PARTICLE_LIST, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );


	pDlg->AddComboBox( IDC_COMBO_PARTICLE_FILE, iX, iY, iWidth + 20, iHeight+10, 0, false, &pComboBox );

	iY += iHeight + 3;
	pComboBox->SetDropHeight( 80 );
		
	pDlg->AddButton( IDC_BUTTON_LOAD_BASE_PARTICLE_FILE, L"기존 파일 모두 열기", iX, iY, iWidth, iHeight);
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_LOAD_PARTICLE_FILE, L"다른 파일 열기", iX, iY, iWidth, iHeight);
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_INIT_PARTICLE_FILE, L"초기화", iX, iY, iWidth, iHeight);
	iY += iHeight + 3;

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"파티클 목록", iX, iY, (iWidth - 80), iHeight );
	pDlg->AddButton( IDC_BUTTON_DEL_PARTICLE, L"제거", iX + (iWidth - 80), iY, 40, iHeight );
	pDlg->AddButton( IDC_BUTTON_SAVE_PARTICLE_FILE, L"저장", iX + (iWidth - 40), iY, 40, iHeight );
	pDlg->AddEditBox( IDC_EDIT_PARTICLE_LIST_COUNT, L"", iX + iWidth, iY - 10, 50, 30, false, NULL );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_PARTICLE_LIST, iX, iY, iWidth, iHeight * 20, 0, &pListBox );
	iY += iHeight * 20 + 3;
	pDlg->AddButton( IDC_BUTTON_PARTICLE_BASE_EDITOR, L"파티클 기본 설정(F3)", iX, iY, iWidth, iHeight);
	iY += iHeight + 3;
	pDlg->AddButton( IDC_BUTTON_PARTICLE_ETC_EDITOR, L"파티클 기타 설정", iX, iY, iWidth, iHeight);
	iY += iHeight + 3;

	pDlg->AddEditBox( IDC_EDIT_PARTICLE_LIST_FIND, L"", iX, iY, iWidth - 40, iHeight + 10, false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_PARTICLE_LIST_FIND, L"찾기", iX + iWidth - 30 , iY, 30 , iHeight + 10 );
	iY += (iHeight + 10) + 3;

	pDlg->AddButton( IDC_BUTTON_PARTICLE_ADD_OTHER_FILE, L"마지막 파일로 파티클 이동", iX, iY, iWidth, iHeight);


}
void CX2EffectToolMain::InitDialogParticle_List_Find_Texture()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 300;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 20;

	pDlg = new CEffectToolDialog;		// DLG_PARTICLE_LIST
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_PARTICLE_LIST_FIND_TEXTURE, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"특정 텍스쳐 사용 파티클 목록", iX, iY, iWidth , iHeight );
	iY += iHeight + 3;

	pDlg->AddListBox( IDC_LIST_PARTICLE_LIST_FIND_TEXTURE, iX, iY, iWidth, iHeight * 20, 0, &pListBox );
	iY += iHeight * 20 + 3;

	pDlg->AddEditBox( IDC_EDIT_FIND_TEXTURE_NAME, L"", iX, iY, iWidth - 40, iHeight + 10, false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_FIND_TEXTURE, L"찾기", iX + iWidth - 30 , iY, 30 , iHeight + 10 );
	iY += (iHeight + 10) + 3;
}
void CX2EffectToolMain::InitDialogMesh_List()
{
	extern CDXUTDialogResourceManager g_DialogResourceManager;
	CEffectToolDialog* pDlg		= NULL;
	CDXUTEditBox* pEditBox		= NULL;
	CDXUTComboBox* pComboBox	= NULL;
	CDXUTListBox* pListBox		= NULL;
	CDXUTCheckBox* pCheckBox	= NULL;

	const int MAGIC_DIALOG_WIDTH	= 300;
	const int MAGIC_DIALOG_HEIGHT	= 400;

	int iX = 10;
	int iY = 10;
	int iWidth = 200;
	int iHeight = 20;

	pDlg = new CEffectToolDialog;		// DLG_PARTICLE_LIST
	m_vecpDialog.push_back( pDlg );
	pDlg->EnableKeyboardInput(true);
	pDlg->Init( &g_DialogResourceManager );
	pDlg->SetCallback( OnDialogEvent_DLG_MESH_LIST, this );
	pDlg->SetSize( MAGIC_DIALOG_WIDTH, MAGIC_DIALOG_HEIGHT );
	pDlg->SetFont( 0, L"굴림", 12, 12 );


	pDlg->AddButton( IDC_BUTTON_LOAD_BASE_MESH_FILE, L"GameMajorXMeshPlayer 열기", iX, iY, iWidth , iHeight);
	iY += iHeight + 3;

	pDlg->AddStatic( IDC_STATIC_DUMMY, L"메쉬 목록", iX, iY, (iWidth - 80), iHeight );
	pDlg->AddButton( IDC_BUTTON_DEL_MESH, L"제거", iX + (iWidth - 80), iY, 40, iHeight );
	pDlg->AddButton( IDC_BUTTON_SAVE_MESH_FILE, L"저장", iX + (iWidth - 40), iY, 40, iHeight );
	iY += iHeight + 3;
	pDlg->AddListBox( IDC_LIST_MESH_LIST, iX, iY, iWidth, iHeight * 20, 0, &pListBox );
	iY += iHeight * 20 + 3;
	pDlg->AddButton( IDC_BUTTON_MESH_BASE_EDITOR, L"메쉬 기본 설정(F4)", iX, iY, iWidth, iHeight);
	iY += iHeight + 3;

	pDlg->AddEditBox( IDC_EDIT_MESH_LIST_FIND, L"", iX, iY, iWidth - 40, iHeight + 10, false, &pEditBox );   
	pDlg->AddButton( IDC_BUTTON_MESH_LIST_FIND, L"찾기", iX + iWidth - 30 , iY, 30 , iHeight + 10 );
	iY += (iHeight + 10) + 3;

}
void CX2EffectToolMain::InitX2Data()
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
	g_pData->ResetUnitManager( L"UnitTemplet_EffectTool.lua", L"NPCTemplet_EffectTool.lua", L"NPCStat_EffectTool.lua" );

	CX2BuffTempletManager::GetInstance()->OpenScriptFileForTemplet();
	CX2BuffTempletManager::GetInstance()->OpenScriptFileForFactor();

	//아이템 데이타 로드
	g_pData->ResetItemManagerEffectTool();

	//월드 데이타 로드
	g_pData->ResetWorldManager( L"WorldData.lua" );	

	//던전 데이타 로드
	g_pData->ResetDungeonManager( L"DungeonData_EffectTool.lua" );

	//퀘스트 데이타 로드
	//g_pData->ResetQuestManager( L"Quest.lua", L"SubQuest.lua" );

	// 스킬 데이터 로드
	//g_pData->ResetSkillTree( L"SkillTemplet.lua" );
	g_pData->ResetSkillTree( L"NewSkillTemplet.lua", L"SkillData.lua" );

	g_pData->ResetSlideShot();


	g_pData->ResetUIManager();
	g_pData->ResetDamageManager();
}

CX2EffectToolMain::~CX2EffectToolMain(void)
{
	if( m_pGame != NULL )
		m_pGame->GameEnd();
	SAFE_DELETE(m_pGame);

	SAFE_CLOSE( m_pXMeshBoundingSphere );
	
	SAFE_DELETE( m_pXMeshPlayer );
	SAFE_DELETE( m_pParticleSystem );
	
	SAFE_DELETE( m_pMinorxMeshPlayer );
	SAFE_DELETE( m_pMinorParticleSystem );
	SAFE_DELETE( m_pCharacterSkinAnim );
	
	SAFE_DELETE( m_pMatrix );
	SAFE_DELETE( m_pFileManager );
	SAFE_DELETE( m_pParticleEditor );
	SAFE_DELETE( m_pMeshEditor );
	SAFE_CLOSE( m_pXSkinMesh );
	

	for( int i=0; i<12 ; i++ )
	{
		SAFE_RELEASE( m_pMesh3DText[i] );
		m_pMesh3DText[i] = NULL;
	}
	
	DeleteAllSkinMeshAndSkinAnim();
#ifndef EFFECT_TOOL
	ClearUserUnitStateListBox();
#endif //EFFECT_TOOL
	FinalDialog();
}

CX2EffectToolMain::CX2EffectToolMain(void)
: m_pSelectedEffectSetData ( NULL )
,m_pSelectedEffectData ( NULL )
,m_pSelectedCameraData ( NULL )
,m_iSelectedCameraIndex( -1 )
,m_iSelectedEffectSetDataIndex ( -1 )
,m_iSelectedParticleMeshIndex( -1 )
,m_bShowEffectSetList(true)
,m_uiParticleNumCurrentFile(0)
,m_uiParticleFileNum(0)
,m_bIsReadyEffectSetResource(false)
,m_bPreScriptLoad(false)
{
	char szWindowTitle[1024];
	sprintf( szWindowTitle, "X2EffectTool; compiled at %s, %s", __DATE__, __TIME__ );
	SetWindowTextA( g_pKTDXApp->GetHWND(), szWindowTitle );
#ifdef CHECK_VERSION
	ConvertCharToWCHAR( m_wstrVersion, szWindowTitle );
#endif //CHECK_VERSION

#ifdef PRE_UPDATE_SCRIPT
	wstring wstrResourcePath = L"";
	GetMajorResourcePath(wstrResourcePath);

	wstring wstrCommand = L"/command:update /path:\"";
	wstrCommand += wstrResourcePath;
	wstrCommand += L"\"";
	ShellExecute(NULL, NULL, L"tortoiseproc.exe", wstrCommand.c_str(), L"", SW_SHOWNORMAL );
#endif // PRE_UPDATE_SCRIPT


	m_vFixedCharacterSkinAnimPosition	= D3DXVECTOR3( 0, 0, 0 );
	m_vFixedCharacterSkinAnimRotation	= D3DXVECTOR3( 0, 0, 0 );

	m_bFixPostionCharacterSkinAnim	= false;
	m_bShowCharacterSkinAnim		= true;
	m_bShowUserUnit			= false;
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
	m_iPrevToolMode			= MODE_EFFECT_SET;
	m_iToolMode				= MODE_GAMEPLAY;
	m_fObjectMoveSpeed		= 4.f;
	m_CurrUnitClass			= CX2Unit::UC_ELSWORD_SWORDMAN;

	m_bRenderBoundingSphere	 = false;
	m_bRenderFlicker = false;	//월드툴 기능. 제거되어야 함.
	m_bCenterGreenDot = false;
	

	m_bShowUserStartPos	= true;
	m_bShowNpcStartPos	= true;
	m_bShowWorld		= true;

	m_bShowLineMapModifier = false;
	
	m_pXMeshBoundingSphere = g_pKTDXApp->GetDeviceManager()->OpenXMesh( L"sphere0.x" );

	GetGameOption().SetMusic( false );
	GetGameOption().SetSoundVolume( 5000.f );

	// 09.03.02 김태완 : 옵션 강제 최고설정
	GetGameOption().SetTexDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption().SetUnitDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption().SetMapDetail( CX2GameOption::OL_HIGH, true );
	GetGameOption().SetEffectDetail( CX2GameOption::OL_HIGH, true );


	g_pMain->ResetCrashReport( L"" );

	InitDialog();
	InitX2Data();

	//CreateGame();

	// 이펙트 툴
	//m_pXMeshPlayerTest			= NULL;
	m_iPickedUserUnitState		= -1;
	m_bUseUserUnitState			= false;

	m_pXMeshPlayer				= new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
	m_pParticleSystem			= new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );

	// UI 플레이어.
	m_pMinorxMeshPlayer			= new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        1
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
	m_pMinorxMeshPlayer->OpenScriptFile( L"GameMinorXMeshPlayer.txt" );		

	m_pMinorParticleSystem			= new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 1
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
	m_pMinorParticleSystem->OpenScriptFile( L"GameMinorParticle.txt" );
	//m_pParticleSystemTest		= NULL;

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

	m_pXSkinMesh = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( L"Dummy_Ani_Wait.x" );
	if( NULL == m_pXSkinMesh )
		MessageBox( g_pKTDXApp->GetHWND(), L"Dummy_Ani_Wait.x 파일이 없습니다.", L"죄송^^", MB_OK );
	
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


	m_pParticleEditor = new CX2EffectToolParticleEditor(this);
	m_pMeshEditor = new CX2EffectToolMeshEditor(this);
	m_pFileManager	= new CX2EffectToolFileManager(this);
	m_fDialogEvent_DelayTime = -1.f;
}

HRESULT CX2EffectToolMain::OnLostDevice()
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

HRESULT CX2EffectToolMain::OnResetDevice()
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

void CX2EffectToolMain::OnFrameMove_Mesh( double fTime, float fElapsedTime )
{
	if( NULL == m_pParticleSystem ) 
		return;

	if( NULL != m_pCharacterSkinAnim &&
		NULL != m_pCharacterSkinAnim->GetXSkinAnim() )
	{
		if( m_pCharacterSkinAnim->IsPlaying() )
		{
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
	}
}
void CX2EffectToolMain::DestroyPreViewInstance()
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
	if( m_pXMeshPlayer != NULL && m_sPreviewXMeshPlayer.GetXMeshInstPointer() != NULL )
	{
		CKTDGXMeshPlayer::CXMeshInstanceHandle hHandle = m_sPreviewXMeshPlayer.GetXMeshInstPointer()->GetHandle();
		m_pXMeshPlayer->DestroyInstance( hHandle );
		m_sPreviewXMeshPlayer.SetNULLPointerbyXMeshInst();
	}

}
HRESULT CX2EffectToolMain::OnFrameMove( double fTime, float fElapsedTime )
 {
	HRESULT hr = S_OK;

	if( NULL != m_pParticleEditor)
		m_pParticleEditor->OnFrameMove_HotKey();

 	if( NULL != m_pMeshEditor )
 		m_pMeshEditor->OnFrameMove_HotKey();

	if( m_pGame == NULL )
	{
		CreateGame();
	}
	else
	{
#ifdef PRE_UPDATE_SCRIPT
		if( false == m_bPreScriptLoad )
		{
			m_bPreScriptLoad = true;
			//if( MessageBox( NULL, L"관련 스크립트 모두 열기 하겠습니까?\n업데이트 완료된 후에 확인 눌러주세요.", L"", MB_OKCANCEL ) == IDOK )
			{
				// 이펙트셋 열기
				On_IDC_BUTTON_LOAD_EFFECT_SET_WHOLE();

				// 메이저 파티클, 메쉬플레이어 열기.

				ON_IDC_BUTTON_LOAD_BASE_PARTICLE_FILE();
				ON_IDC_BUTTON_LOAD_MESH_FILE( L"GameMajorXMeshPlayer.txt" );
			}
		}
#endif // PRE_UPDATE_SCRIPT
	}

	if( m_fDialogEvent_DelayTime > 0.f )
		m_fDialogEvent_DelayTime -= fElapsedTime;


	if( false == m_bPaused )
	{
		OnFrameMove_Mesh(fTime, fElapsedTime);
	}
	switch( m_iToolMode )
	{
	case MODE_GAMEPLAY:
		{
			DestroyPreViewInstance();
		} break;
#pragma region MODE_TOOL
	case MODE_EFFECT_SET:
	default:
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
						vPosition.y += 200.f;

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
						vPosition.y += 200.f;

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
#pragma endregion MODE_TOOL
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
void CX2EffectToolMain::OnFrameMove_CameraShake()
{
	if( NULL == m_pCharacterSkinAnim )
		return;

	CKTDGXSkinAnim* pXSkiinAnim = m_pCharacterSkinAnim->GetXSkinAnim();
	if( NULL == pXSkiinAnim )
		return;

	if( false == m_pCharacterSkinAnim->IsPlaying() )
		return;

	if( NULL == m_pGame || NULL == m_pGame->GetX2Camera() )
		return;


	CKTDGCamera& pCamera = m_pGame->GetX2Camera()->GetCamera();

	vector<CX2EffectSet::CameraShakeData*>::iterator it = m_vecReadyToPlayCameraShake.begin();
	for( ; it != m_vecReadyToPlayCameraShake.end(); ++it )
	{
		CX2EffectSet::CameraShakeData* pShakeData = *it;
		if( NULL == pShakeData )
			continue;

		if( pShakeData->m_fStartAnimTime >= pXSkiinAnim->GetMaxAnimationTime() )
		{
			m_vecReadyToPlayCameraShake.erase( it-- );
			continue;
		}

		if( pShakeData->m_fStartAnimTime < pXSkiinAnim->GetNowAnimationTime() )
		{
			switch( pShakeData->m_eShakeType )
			{
			case CKTDGCamera::DECT_UP:
				{
					pCamera.UpCrashCamera( pShakeData->m_fOneDirSpeed, pShakeData->m_fOneDirAccel );
				} break;
			case CKTDGCamera::DECT_DOWN:
				{
					pCamera.DownCrashCamera( pShakeData->m_fOneDirSpeed, pShakeData->m_fOneDirAccel );
				} break;
			case CKTDGCamera::DECT_UP_DOWN:
				{
					pCamera.ShakeUpDown( pShakeData->m_fTwoDirGap, pShakeData->m_fTwoDirTime, pShakeData->m_fTwoDirTimeGap );
				} break;
			case CKTDGCamera::DECT_UP_DOWN_NO_RESET:
				{
					pCamera.UpDownCrashCameraNoReset( pShakeData->m_fTwoDirGap, pShakeData->m_fTwoDirTime );
				} break;

			case CKTDGCamera::DECT_LEFT:
				{
					pCamera.LeftCrashCamera( pShakeData->m_fOneDirSpeed, pShakeData->m_fOneDirAccel );
				} break;
			case CKTDGCamera::DECT_RIGHT:
				{
					pCamera.RightCrashCamera( pShakeData->m_fOneDirSpeed, pShakeData->m_fOneDirAccel );
				} break;
			case CKTDGCamera::DECT_LEFT_RIGHT:
				{
					pCamera.ShakeLeftRight( pShakeData->m_fTwoDirGap, pShakeData->m_fTwoDirTime, pShakeData->m_fTwoDirTimeGap );
				} break;
			case CKTDGCamera::DECT_LEFT_RIGHT_NO_RESET:
				{
					pCamera.ShakeLeftRight( pShakeData->m_fTwoDirGap, pShakeData->m_fTwoDirTime );
				} break;
			case CKTDGCamera::DECT_RANDOM:
				{
					pCamera.ShakeRandom( pShakeData->m_iEyeShakeCount, pShakeData->m_EyeMoveSpeed.m_Min, pShakeData->m_EyeMoveSpeed.m_Max,
										pShakeData->m_EyeMoveRange.m_Min, pShakeData->m_EyeMoveRange.m_Max,
										pShakeData->m_iLookAtShakeCount, pShakeData->m_LookAtMoveSpeed.m_Min, pShakeData->m_LookAtMoveSpeed.m_Max,
										pShakeData->m_LookAtMoveRange.m_Min, pShakeData->m_LookAtMoveRange.m_Max,
										pShakeData->m_bFixedLookVec);
				} break;
			}

			m_vecReadyToPlayCameraShake.erase( it-- );
			continue;
		}
	}
}
HRESULT CX2EffectToolMain::OnFrameRender()
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
	case MODE_EFFECT_SET:
	default:
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
				}
				CKTDGStateManager::PopRenderState( D3DRS_FILLMODE );
			}


// 			if( true == m_bRenderFlicker )
// 			{
// 				if( m_TimerRenderPickedObject.elapsed() > 1.f )
// 				{
// 					m_TimerRenderPickedObject.restart();
// 				}
// 				else if( m_TimerRenderPickedObject.elapsed() > 0.5f )
// 				{
// 
// 					//CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
// 					CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_ADD );
// 					CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG1 , D3DTA_CONSTANT );
// 					CKTDGStateManager::PushTextureStageState( 0, D3DTSS_COLORARG2 , D3DTA_CURRENT );
// 
// 					for( UINT i=0; i<m_vecpPickedWorldToolObject.size(); i++ )
// 					{
// 						CWorldToolObject* pWorldToolObject = m_vecpPickedWorldToolObject[i];
// 						pWorldToolObject->GetBoundingSphere( vCenter, fRadius );
// 						D3DXVECTOR3 vOriginalPos = pWorldToolObject->GetPosition();
// 						D3DXVECTOR3 vOriginalScale = pWorldToolObject->GetScale();
// 
// 						pWorldToolObject->AddPositionRelative( (vOriginalPos - vCenter) * 0.2f );
// 						pWorldToolObject->SetScale( vOriginalScale * 1.2f );
// 						
// 						pWorldToolObject->Render();
// 						pWorldToolObject->Render();
// 
// 						pWorldToolObject->SetScale( vOriginalScale );
// 						pWorldToolObject->SetPosition( vOriginalPos );
// 					}
// 
// 					CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG2 );
// 					CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLORARG1 );
// 					CKTDGStateManager::PopTextureStageState( 0, D3DTSS_COLOROP );
// 				}
// 			}




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
		} break;
	case MODE_GAMEPLAY:
		break;
	}

	if( m_pGame != NULL && m_pGame->GetWorld() != NULL && m_pGame->GetWorld()->GetLineMap() != NULL )
	{
		CKTDGLineMap *pLineMap = m_pGame->GetWorld()->GetLineMap();
		for(int i=0; i<pLineMap->GetNumLineData(); ++i)
		{
			const CKTDGLineMap::LineData *pLineData = pLineMap->GetLineData(i);
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

bool CX2EffectToolMain::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
			}

			m_bMouseDragging = false;

		} break;
	case WM_KEYDOWN:
		{
			if( MODE_GAMEPLAY != m_iToolMode )
			{
				switch( wParam )
				{
				case VK_F2: ToggleGameMode();break;
				case VK_F3: g_pEffectToolMain->ON_IDC_BUTTON_PARTICLE_BASE_EDITOR(); break;
				case VK_F4: g_pEffectToolMain->ON_IDC_BUTTON_MESH_BASE_EDITOR(); break;
				case VK_F5: g_pEffectToolMain->ChangeToolMode( CX2EffectToolMain::MODE_EFFECT_SET_PARTICLE_MESH_EDIT );break;
				case VK_F6: g_pEffectToolMain->ChangeToolMode( CX2EffectToolMain::MODE_EFFECT_SET_CAMERA_EDIT ); break;
				case VK_F7: g_pEffectToolMain->ChangeToolMode( CX2EffectToolMain::MODE_EFFECT_SET ); break;
				case VK_F8: g_pEffectToolMain->ToggleShowEffectSetList(); break;
				} 
			}
			else
			{
				if( wParam == VK_F2 )
				{
					ToggleGameMode();
				}
			}
#ifdef SHOW_ATTACK_BOX_DUMMY
			switch( wParam )
			{
			case VK_F11:
				{
					if( NULL != g_pEffectToolMain &&
						NULL != g_pEffectToolMain->m_pCharacterSkinAnim )
					{
						g_pEffectToolMain->m_pCharacterSkinAnim->ToggleShowAttackBox();
					}
				} break;
			case VK_F12:
				{
					if( NULL != g_pEffectToolMain &&
						NULL != g_pEffectToolMain->m_pXMeshPlayer )
					{
						g_pEffectToolMain->m_pXMeshPlayer->ToggleShowAttackBox();
					}
				} break;
			}
#endif //SHOW_ATTACK_BOX_DUMMY
		} break;
	} 

	switch( m_iToolMode )
	{		
	case MODE_EFFECT_SET:
	default:
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
					case VK_F1:
						{
							ReadyCurrentEffectSet();
							PlayEffectSet();
						} break;
					}
				} break;
			case WM_CHAR:
				{
					switch(wParam)
					{	
					case 'F':
					case 'f':
						{
							m_pGame->m_bFixedCamera = !m_pGame->m_bFixedCamera;
						} break;
						// 모두 저장
					case '[':
						{
							SaveWhole( true );
						} break;
						// 커밋
					case ']':
						{
							SaveWhole( true );
							CommitSVN();
						} break;
#ifdef CREATE_EFFECT_SET_BY_DAMAGE_EFFECT // 오현빈
					case 'r':
					case 'R':
						{
							CreateEffectSetByDamageEffect();
						} break;
#endif // CREATE_EFFECT_SET_BY_DAMAGE_EFFECT

					case 'm':
					case 'M':
						{
							OutputUsedResource();
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
			case WM_KEYDOWN:
				{
					switch( wParam )
					{
					case VK_F1:
						{
							PlayEffectSet();
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


bool CX2EffectToolMain::DropFileProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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

		return true;
	}


	return false;
}


bool CX2EffectToolMain::DecomposePathAndFileName( WCHAR* wszPath, WCHAR* wszFileName, const WCHAR* wszFullPath )
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
void CX2EffectToolMain::InitFog()
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

void CX2EffectToolMain::LoadWorld( const WCHAR* pwszFileName )
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

    g_pEffectToolMain->m_pGame->GetWorld()->RemoveWordLayer(-1);
    m_vecWorldLayer.clear();

	m_pGame->ReloadWorld( wszFileNameOnly );
	m_pGame->ResetUnitPosition();

    InitFog();
	PostProcLoadWorld();

	if( NULL != m_pGame && NULL != m_pGame->GetWorld() )
	{
		float fLandHeight = m_pGame->GetWorld()->GetLineMap()->GetLandHeight();
		m_pGame->GetX2Camera()->SetLandHeight( fLandHeight );
	}

}


void CX2EffectToolMain::LoadLineMap( const WCHAR* pwszFileName )
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

void CX2EffectToolMain::CreateGame()
{
	const int MAGIC_USER_UID				= 777;
	const int MAGIC_UNIT_UID_ELSWORD 		= 777;
	//int MAGIC_UNIT_UID_ARME	 		= 778;
	//int MAGIC_UNIT_UID_LIRE	 		= 779;
	//int MAGIC_UNIT_UID_KNIGHT			= 780;
	//int MAGIC_UNIT_UID_MAGIC_KNIGHT	= 781;

	// user data setup
    {
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
	m_pGame = new CEffectToolGame(this);
	m_pGame->GameLoading( (CX2Room*)g_pData->GetPVPRoom() );
	if( NULL != m_pGame->GetWorld() )
	{
		m_pGame->GetWorld()->StopBGM();
	}

	// tool 에서는 combo manager가 필요없다~
	m_pGame->GetMyUnit()->DeleteComboManager();
	m_pGame->GetMyUnit()->SetShowObject(false);
	g_pX2Game = m_pGame;
	CreateCharacterSkinMesh( L"ELSWORD" );

	g_pKTDXApp->SkipFrame();
}



void CX2EffectToolMain::DisableGameKey()
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

void CX2EffectToolMain::AddObjectToWorld( const WCHAR* wszFileName, CX2WorldObject::OBJECT_TYPE eObjType )
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

}
void CX2EffectToolMain::ChangeUserUnit( CX2Unit::UNIT_CLASS eUnitClass )
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
#ifndef EFFECT_TOOL
	ResetUserUnitStateListBox();
#endif //EFFECT_TOOL

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
	}
}

void CX2EffectToolMain::RenderBoundingSphere( D3DXVECTOR3& vCenter, float fRadius )
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
void CX2EffectToolMain::On_IDC_BUTTON_ADD_DAMAGE_EFFECT( const WCHAR* pwszFileFullPath )
{
	// 존재하지 않는 DamageEffect 이름이라면 DamageEffect.lua에 추가
	if ( NULL == m_pFileManager )
		return;

	ResetDamageEffect();

	if( false == IsValidDamageEffectByName(L"SDFKLJSDF") )
	{
		m_pFileManager->AddDamageEffect( pwszFileFullPath, L"SDFKLJSDF");
	}
}
void CX2EffectToolMain::ResetDamageEffect()
{
	if( NULL == g_pX2Game )
		return;

	static_cast<CEffectToolGame*>(g_pX2Game)->ResetDamageEffect();
}
bool CX2EffectToolMain::IsValidDamageEffectByName( const WCHAR* wcEffectName_ )
{
	if( NULL == g_pX2Game )
		return false;

	if( NULL == g_pX2Game->GetDamageEffect() )
	{
		ResetDamageEffect();
	}

	if( NULL == g_pX2Game->GetDamageEffect() )
		return false;


	return g_pX2Game->GetDamageEffect()->IsValidDamageEffectByName( wcEffectName_ );
}
void CX2EffectToolMain::On_IDC_BUTTON_LOAD_DAMAGE_EFFECT()
{
	if( NULL != g_pX2Game )
	{
		g_pX2Game->RefreshDamageEffectScript();
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_LOAD_EFFECT_SET_WHOLE()
{
	if( NULL == g_pData )
		return;

	if( NULL == g_pData->GetGameEffectSet() )
		return;

	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();
	if( NULL == pEffectset )
		return;

	pEffectset->ClearTempletAndInstance();
	pEffectset->OpenScriptFile( L"EffectSet.lua");
	pEffectset->OpenScriptFile( L"EffectSet_Description.lua" );

#ifdef CHECK_VERSION
	if( 0 != pEffectset->GetEffectToolVersion().compare( m_wstrVersion.c_str() ) )
	{
#ifndef DEBUG	
		MessageBox(NULL, L"최신 버전의 툴로 업데이트 해주세요.", L"Error!", MB_OK);
//		DestroyWindow( g_pKTDXApp->GetHWND() );
#endif //DEBUG
	}	
#endif //CHECK_VERSION
	UpdateEffectList();
// #pragma NOTE("검수")
// 	CX2EffectSet EffectTemp;
// 	EffectTemp.ClearTempletAndInstance();
// 	EffectTemp.OpenScriptFile( L"123_test.lua" ) ;
// 
// 
// 	std::vector< CX2EffectSet::EffectSetData* > vec1 = pEffectset->GetVecEffectSetTemplet();
// 	std::vector< CX2EffectSet::EffectSetData* > vec2 = EffectTemp.GetVecEffectSetTemplet();
// 
// 	std::vector< CX2EffectSet::EffectSetData* >::iterator it1 = vec1.begin();
// 	std::vector< CX2EffectSet::EffectSetData* >::iterator it2 = vec2.begin();
// 
// 
// 	for( ; it1 != vec1.end(); ++it1 )
// 	{
// 		//if( true == ( (*it1)->operator ==( **it2 )) )
// 		if( (**it1) == (**it2) )
// 		{
// 			int i=0;
// 		}
// 		else
// 		{
// 			ASSERT("안돼!!");
// 			break;
// 		}
// 
// 		++it2;
// 	}
// 
// 	EffectTemp.ClearTempletAndInstance();	
// 
// 
// 	int i=0;
}
void CX2EffectToolMain::On_IDC_BUTTON_SAVE_EFFECT_SET_WHOLE( bool bHotKey_ )
{
	if( NULL == m_pFileManager )
		return;

	if( NULL == g_pData )
		return;

	if( NULL == g_pData->GetGameEffectSet() )
		return;

	// 파일 저장 경로 & 파일 이름 설정
	wstring wsFilePathAndName;
	g_pEffectToolMain->GetMajorResourcePath( wsFilePathAndName );
	wsFilePathAndName += L"EffectSet.lua";

	// 저장할 이펙트셋 얻기.
	std::vector< CX2EffectSet::EffectSetData* >* pEffectSetTemplet = 
		g_pData->GetGameEffectSet()->GetVecEffectSetTemplet();

	// 저장
	if( NULL != pEffectSetTemplet )
		m_pFileManager->SaveEffectSetWhole( wsFilePathAndName.c_str(), pEffectSetTemplet );

	// 주석 파일 저장
	g_pEffectToolMain->GetMajorResourcePath( wsFilePathAndName );
	wsFilePathAndName += L"EffectSet_Description.lua";

	if( NULL != pEffectSetTemplet )
		m_pFileManager->SaveEffectSet_Desc( wsFilePathAndName.c_str(), pEffectSetTemplet );

#ifdef SAVE_AND_SVN_UPDATE
	if( false == bHotKey_ )
	{
		if( true == MessageBox(NULL, L"저장이 완료 되었습니다.\n\n스크립트 업데이트 받으시겠습니까?\n\n※업데이트 하지 않고 커밋하면\n스크립트 충돌 위험이 있습니다.", L"Update", MB_OKCANCEL ) )
		{
			wstring wstrResourcePath = L"";
			GetMajorResourcePath(wstrResourcePath);

			wstring wstrCommand = L"/command:update /path:\"";
			wstrCommand += wstrResourcePath;
			wstrCommand += L"\"";
			ShellExecute(NULL, NULL, L"tortoiseproc.exe", wstrCommand.c_str(), L"", SW_SHOWNORMAL );
		}
	}
#endif // SAVE_AND_SVN_UPDATE

}

HRESULT CX2EffectToolMain::LoadEffectSet( const WCHAR* pwszFileName )
{
	HRESULT hr = S_OK;

	return hr;
}


void CX2EffectToolMain::CreateCharacterSkinMesh( const WCHAR* wszCharacterName )
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
	CDXUTListBox* pListBox = m_vecpDialog[DLG_EFFECT_SET_LIST]->GetListBox( IDC_LIST_SKIN_ANIM_NAME );
	pListBox->RemoveAllItems();

	vector<wstring> vecAnimName;
	m_pCharacterSkinAnim->GetAnimNameList( vecAnimName );

	for( UINT i=0; i<vecAnimName.size(); i++ )
	{
		pListBox->AddItem( vecAnimName[i].c_str(), NULL );
	}
	// listbox 갱신
	pListBox = m_vecpDialog[DLG_EFFECT_SET_PARTICLE_MESH_EDIT2]->GetListBox( IDC_LIST_MOTION_BONE_LIST );
	pListBox->RemoveAllItems();

	m_pCharacterSkinAnim->GetFrameNameList( vecAnimName );
		
	for( UINT i=0; i<vecAnimName.size(); i++ )
	{
		pListBox->AddItem( vecAnimName[i].c_str(), NULL );
	}
}

void CX2EffectToolMain::CreateCharacterSkinMeshFromFile( const WCHAR* wszFileName )
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

	if( false == m_pCharacterSkinAnim->SetAnimXSkinMesh( wszFileNameOnly ) )
	{
		MessageBox( NULL, L"모션 파일 열기 실패\nDevPath.lua에서 경로를 설정해주세요", L"Error" , NULL );
		return;
	}
	
	// 본체 모션 바꾸기
	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetMyUnit() )
	{
		g_pX2Game->GetMyUnit()->SetXSkinAnim_EffectTool( m_pCharacterSkinAnim->GetXSkinAnim() );
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



	m_pCharacterSkinAnim->GetRenderParam()->renderType		= CKTDGXRenderer::RT_CARTOON_BLACK_EDGE;
	m_pCharacterSkinAnim->GetRenderParam()->cartoonTexType	= CKTDGXRenderer::CTT_NORMAL;
	m_pCharacterSkinAnim->GetRenderParam()->fOutLineWide	= 1.5f;
	if(g_pX2Game != NULL && g_pX2Game->GetWorld() != NULL)
	{
		m_pCharacterSkinAnim->GetRenderParam()->lightPos		= g_pX2Game->GetWorld()->GetLightPos();
	}	

	// listbox 갱신
	CDXUTListBox* pListBox = m_vecpDialog[DLG_EFFECT_SET_LIST]->GetListBox( IDC_LIST_SKIN_ANIM_NAME );
	pListBox->RemoveAllItems();

	vector<wstring> vecAnimName;
	m_pCharacterSkinAnim->GetAnimNameList( vecAnimName );

	for( UINT i=0; i<vecAnimName.size(); i++ )
	{
		pListBox->AddItem( vecAnimName[i].c_str(), NULL );
	}
	// listbox 갱신
	pListBox = m_vecpDialog[DLG_EFFECT_SET_PARTICLE_MESH_EDIT2]->GetListBox( IDC_LIST_MOTION_BONE_LIST );
	pListBox->RemoveAllItems();

	m_pCharacterSkinAnim->GetFrameNameList( vecAnimName );

	for( UINT i=0; i<vecAnimName.size(); i++ )
	{
		pListBox->AddItem( vecAnimName[i].c_str(), NULL );
	}
}
// 콤보박스 중 선택된 파티클 파일로 파티클 리스트 갱신 하는 함수.
void CX2EffectToolMain::UpdateParticleListByIndex( UINT uiIndex_ )
{
	if( m_vecParticleFileName.size() > uiIndex_ )
	{				
		vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
		if( true == m_pParticleSystem->GetVecTempletSequences( m_vecParticleFileName[uiIndex_].c_str() , vecParticleTemplet ) )
		{
			m_wstrCurrentParticleFileName = m_vecParticleFileName[uiIndex_];
			CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
			if( NULL != pDialog )
			{
				CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
				if( NULL != pListBox )
				{
					pListBox->RemoveAllItems();
					BOOST_FOREACH(CKTDGParticleSystem::CParticleEventSequence* pData, vecParticleTemplet )
					{
						pListBox->AddItem( pData->GetName(), NULL );
					}

					// 파티클 갯수 표시
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_PARTICLE_LIST_COUNT );
					if( NULL != pEditBox )
					{
						WCHAR wszTextOut[64];
						m_uiParticleNumCurrentFile = vecParticleTemplet.size();
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", m_uiParticleNumCurrentFile );	
						pEditBox->SetText( wszTextOut );
					}

					CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
					if( NULL != pComboBox )
					{
						pComboBox->SetSelectedByIndex( uiIndex_ );
					}
				}
			}
		}
	}
}
void CX2EffectToolMain::UpdateParticleListByFileName( const WCHAR* strFileName_ )
{	
	if( strFileName_ == L"")
		return;

	int iIndex = 0;
	BOOST_FOREACH( wstring strData, m_vecParticleFileName )
	{
		if( 0 == strData.compare( strFileName_ ) )
		{
			vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
			if( true == m_pParticleSystem->GetVecTempletSequences( strData.c_str() , vecParticleTemplet ) )
			{
				m_wstrCurrentParticleFileName = strData;
				CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
				if( NULL != pDialog )
				{
					CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
					if( NULL != pListBox )
					{
						pListBox->RemoveAllItems();
						BOOST_FOREACH(CKTDGParticleSystem::CParticleEventSequence* pData, vecParticleTemplet )
						{
							pListBox->AddItem( pData->GetName(), NULL );
						}
						
						// 파티클 갯수 표시
						CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_PARTICLE_LIST_COUNT );
						if( NULL != pEditBox )
						{
							WCHAR wszTextOut[64];
							m_uiParticleNumCurrentFile = vecParticleTemplet.size();
							StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", vecParticleTemplet.size() );	
							pEditBox->SetText( wszTextOut );
						}
					}

					CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
					if( NULL != pComboBox )
					{
						pComboBox->SetSelectedByIndex( iIndex );
					}
				}
			}
			break;
		}
		iIndex++;
	}

}
void CX2EffectToolMain::SelectParticleListByIndex( int iIndex_ )
{				
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
		if( NULL != pListBox )
		{
			pListBox->SelectItem( iIndex_ );
		}
	}
}
void CX2EffectToolMain::SelectParticleListLastIndex()
{
	vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;	
	if( true == m_pParticleSystem->GetVecTempletSequences( m_wstrCurrentParticleFileName.c_str(), vecParticleTemplet ) )
	{
		SelectParticleListByIndex( vecParticleTemplet.size()-1 );
	}
}
void CX2EffectToolMain::SelectMeshListByIndex( int iIndex_ )
{				
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_MESH_LIST );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_MESH_LIST);
		if( NULL != pListBox )
		{
			pListBox->SelectItem( iIndex_ );
		}
	}
}
void CX2EffectToolMain::SelectMeshListLastIndex()
{
	SelectMeshListByIndex( m_pXMeshPlayer->GetVecTemplet().size() -1 );
}
//선택된 이펙트셋에 파티클 추가
void CX2EffectToolMain::AddParticleAtCurrentEffectset( const WCHAR* strName_ )
{
	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		CX2EffectSet::EffectData* pEffectData = new CX2EffectSet::EffectData;
		pEffectData->m_EffectName = strName_;
		pEffectData->m_eEffectType = CX2EffectSet::ET_PARTICLE_GAME_MAJOR;
		pEffectSetData->m_vecpEffectData.push_back( pEffectData );

		UpdateEffectSetMember( pEffectSetData->m_wstrEffectSetName.c_str() );
	}
}
void CX2EffectToolMain::AddMeshAtCurrentEffectset( const WCHAR* strName_ )
{
	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		CX2EffectSet::EffectData* pEffectData = new CX2EffectSet::EffectData;
		pEffectData->m_EffectName = strName_;
		pEffectData->m_eEffectType = CX2EffectSet::ET_MESH_PLAYER_GAME_MAJOR;
		pEffectSetData->m_vecpEffectData.push_back( pEffectData );

		UpdateEffectSetMember( pEffectSetData->m_wstrEffectSetName.c_str() );
	}
}
CKTDGXMeshPlayer::XMeshTemplet* CX2EffectToolMain::GetMeshByName( const WCHAR* strMeshName_ )
{
	if( NULL != m_pXMeshPlayer )
	{
		BOOST_FOREACH( CKTDGXMeshPlayer::XMeshTemplet* pData, m_pXMeshPlayer->GetVecTemplet() )
		{
			if( 0 == pData->templetName.compare( strMeshName_ ) )
			{
				return pData;
			}
		}
	}

	return NULL;
}
void CX2EffectToolMain::UpdateDialogMeshEditor( CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet_ )
{
	if( NULL == pMeshTemplet_ )
		return;

 	if( NULL != m_pMeshEditor )
 		m_pMeshEditor->UpdateMeshData( pMeshTemplet_ );
}
void CX2EffectToolMain::UpdateDialogParticleEditor( CKTDGParticleSystem::CParticleEventSequence* pParticle_ )
{ 
	if( NULL == pParticle_ )
		return;

	if( NULL != m_pParticleEditor ) 
		m_pParticleEditor->UpdateParticleData(pParticle_, m_wstrCurrentParticleFileName ); 
}
CKTDGParticleSystem::CParticleEventSequence* CX2EffectToolMain::CreateParticleEventSequence( const WCHAR* strParticleName_, const WCHAR* strParticleFileName_ )
{
	bool bIsExistence = false;
	BOOST_FOREACH( wstring wstrFileName, m_vecParticleFileName )
	{
		if( 0 == wstrFileName.compare( strParticleFileName_) )
		{
			bIsExistence = true;
		}
	}

	if( false == bIsExistence )
	{
		m_vecParticleFileName.push_back( strParticleFileName_ );
		AddParticleFileComboBox( strParticleFileName_ );
	}

	return m_pParticleSystem->CreateEventSequence( strParticleName_, strParticleFileName_ );
}

CKTDGParticleSystem::CParticleEventSequence* CX2EffectToolMain::GetParticleByNameAndIndex( const WCHAR* strParticleName_, int iIndex_ /*= -1*/  )
{
	//파티클 벡터
	vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;

	//인덱스를 지정하지 않으면 현재 선택되어 있는 파티클 파일의 벡터 얻기.
	if( -1 == iIndex_ )
	{
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
		if( NULL != pDialog )
		{
			CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
			if( NULL != pComboBox )
			{
				// 현재 선택되어 있는 콤보박스의 인덱스 얻기.
				UINT uiIndex = reinterpret_cast<UINT>(pComboBox->GetSelectedData());
				m_pParticleSystem->GetVecTempletSequences( m_vecParticleFileName[uiIndex].c_str(), vecParticleTemplet );
			}
		}	
	}
	else
	{
		m_pParticleSystem->GetVecTempletSequences( m_vecParticleFileName[iIndex_].c_str(), vecParticleTemplet );
	}

	//이름 같은 파티클 구해서 리턴.
	if( false == vecParticleTemplet.empty() )
	{
		BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequence* pData, vecParticleTemplet ) 
		{
			if( 0 == wcscmp( strParticleName_, pData->GetName() ) )
			{
				return pData;
			}
		}
	}

	return NULL;
}
bool CX2EffectToolMain::LoadMjoarParticleFile( const WCHAR* strFileFullPath_ )
{
	if( L"" == strFileFullPath_ )
		return false;

	WCHAR drive[_MAX_DRIVE] = {0, };
	WCHAR dir[_MAX_DIR] = {0, };
	WCHAR fname[_MAX_FNAME] = {0, };
	WCHAR ext[_MAX_EXT] = {0, };
	// 기본 파일 불러왔을 경우는, 파일명만 가지고 있음.
	_wsplitpath( strFileFullPath_, drive, dir, fname, ext);

	wstring strFileName( fname );
	strFileName += L".txt";

	BOOST_FOREACH(wstring str, m_vecParticleFileName )
	{
		//같은 파일이 있다면 추가하지 않음.
		if( 0 == str.compare( strFileName ) )
		{
			MessageBox( NULL, strFileName.c_str() , L"이미 로드된 파일입니다." , NULL );
			return false;
		}
	}

	if( NULL == m_pParticleSystem )
	{
		m_pParticleSystem = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
            , 0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK              
            );
		m_pParticleSystem->OpenScriptFile( strFileName.c_str() );
	}
	else
		m_pParticleSystem->OpenScriptFile( strFileName.c_str() );

	if( true == m_pParticleSystem->GetCurrentFileName().empty() )
	{
#ifdef BANANA_EXCEPTION
		// 경로 오류로 인해 파일 열기 실패 시/ 경로 포함된 파일명으로 다시 열기 시도
		m_pParticleSystem->OpenScriptFile( strFileFullPath_ );
		if( true == m_pParticleSystem->GetCurrentFileName().empty() )
#endif // BANANA_EXCEPTION
		{
			return false;
		}
	}

	m_vecParticleFileName.push_back( strFileName );
	AddParticleFileComboBox( strFileName.c_str() );

	// 임의 위치의 파티클을 불러왔을 경우 파티클 경로 저장.
	if( 0 != wcslen(dir) )
	{
		m_mapParticleFilePath[strFileName] = strFileFullPath_;
	}
	return true;
}
void CX2EffectToolMain::AddParticleFileComboBox( const WCHAR* strFileName_ )
{
	// 콤보박스에 목록 추가
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
	if( NULL != pDialog )
	{
		CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
		if( NULL != pComboBox )
		{
								// 파일이름 , 파일이름 저장할 벡터의 인덱스.
			pComboBox->AddItem( strFileName_, reinterpret_cast<void*>(m_vecParticleFileName.size()-1) );
			pComboBox->SetSelectedByIndex( m_vecParticleFileName.size()-1 );
		}
	}
}

void CX2EffectToolMain::LoadMjoarMeshFile( const WCHAR* strFileName_ )
{
	if( L"" == strFileName_ )
		return;

	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	_wsplitpath( strFileName_, drive, dir, fname, ext);

	wstring strFileName( fname );
	strFileName += L".txt";

	if( NULL != m_pXMeshPlayer )
	{
		SAFE_DELETE( m_pXMeshPlayer );
	}
	
	m_pXMeshPlayer = new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
	m_pXMeshPlayer->OpenScriptFile( strFileName.c_str() );		
}
void CX2EffectToolMain::ParseIntArrayFromEditBox( CDXUTEditBox* pEditBox, int *pNumbers, int nCount )
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

void CX2EffectToolMain::SetTextIntArrayToEditBox( CDXUTEditBox* pEditBox, const int *pNumbers, int nCount )
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

void CX2EffectToolMain::PostProcLoadWorld()
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

void CX2EffectToolMain::ReadyEffectDevices()
{
	ClearAllDevice();

	if( NULL == m_pParticleSystem )
		return;

	for( UINT i=0; i< m_vecReadyParticle.size(); i++ )
	{
		const TimedParticle& timedParticle = m_vecReadyParticle[i];
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
	}
	if( NULL == m_pXMeshPlayer )
		return;

	for( UINT i=0; i< m_vecReadyXMeshPlayer.size(); i++ )
	{
		const TimedXMeshPlayer& timedXMeshPlayer = m_vecReadyXMeshPlayer[i];
		const CKTDGXMeshPlayer::XMeshTemplet* pTemplet = m_pXMeshPlayer->GetTempletByName( timedXMeshPlayer.m_wstrXMeshPlayerName );
		if( NULL == pTemplet )
			continue;

		switch( pTemplet->meshType )
		{
		case CKTDGXMeshPlayer::X_MESH:
			{
				XMeshReady( pTemplet->meshName.c_str() );
			} break;

		case CKTDGXMeshPlayer::X_SKIN_MESH:
			{
				XSkinMeshReady( pTemplet->meshName.c_str() );
			} break;

		}
	}
}
// 라인맵의 line start, end pos 좌표를 float에서 int로 바꾼다.
void CX2EffectToolMain::LineMapFloatToInt()
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( NULL == pLineMap )
		return;

	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{
		CKTDGLineMap::LineData* pLineData = pLineMap->AccessLineData(i);

		pLineData->startPos.x = (float)(long)pLineData->startPos.x;
		pLineData->startPos.y = (float)(long)pLineData->startPos.y;
		pLineData->startPos.z = (float)(long)pLineData->startPos.z;

		pLineData->endPos.x = (float)(long)pLineData->endPos.x;
		pLineData->endPos.y = (float)(long)pLineData->endPos.y;
		pLineData->endPos.z = (float)(long)pLineData->endPos.z;
	}
}

// start pos의 line index를 새로 모두 계산
void CX2EffectToolMain::RecomputeStartPosLineIndex()
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

float CX2EffectToolMain::ComputeLandHeight()
{
	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( NULL == pLineMap )
		return 0.f;

	float fLandHeight = pLineMap->GetLineData(0)->startPos.y;


	for( int i=0; i<pLineMap->GetNumLineData(); i++ )
	{		
		const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);

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

void CX2EffectToolMain::LoadLineMapInfo()
{
	DeleteAllSkinMeshAndSkinAnim();

	CKTDGLineMap* pLineMap = m_pGame->GetWorld()->GetLineMap();	
	if( NULL == pLineMap )
		return;

	if( pLineMap != NULL )
	{
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
		const CKTDGLineMap::StartInfoMap& mapStartInfo = pLineMap->GetStartInfoMap();
		CKTDGLineMap::StartInfoMap::const_iterator it;
		for( it=mapStartInfo.begin(); it!=mapStartInfo.end(); it++ )
		{
			if( true == it->second.m_bStartRight )
				iTempInitialDir = DIR_RIGHT;
			else
				iTempInitialDir = DIR_LEFT;
			m_abKeyUsed[ (int) it->first ] = true;
			vecTeamStartPos.push_back( STeamStartPos( (int)it->first, TEAM_NONE, 
                it->second.m_vStartPos, iTempInitialDir, it->second.m_iStartLineIndex ) );
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

void CX2EffectToolMain::SaveLineMapToText()
{
	string strFilePath;
	SaveLineMapToFile( wszTempFilePath, false );
	ConvertWCHARToChar( strFilePath, wszTempFilePath );
	_spawnlp( _P_NOWAIT, "notepad.exe", "notepad.exe", strFilePath.c_str(), NULL );
}

bool CX2EffectToolMain::SaveLineMapToFile( const WCHAR* wszFileName, bool bUpdateFileName )
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
	const CKTDGLineMap::LineData* pLineData = NULL;	
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

void CX2EffectToolMain::AddNewStartPosition(const POINTS& mousepos)
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

	const CKTDGLineMap::LineData* pLineData = NULL;	

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

bool CX2EffectToolMain::PickClickedNPC( int iDungeonLevel, POINTS mousepos, bool bClear )
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
float CX2EffectToolMain::DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint,
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

void CX2EffectToolMain::ComputeRay( D3DXVECTOR3* pvRayOrigin, D3DXVECTOR3* pvRayDirection, const POINTS& mouspos, 
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

void CX2EffectToolMain::CollisionCheckNPCStartPos( CX2DungeonSubStage::SubStageData* pSubStageData, D3DXVECTOR3 vRayStart, D3DXVECTOR3 vRayEnd, 
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

bool CX2EffectToolMain::NPCRayHit( D3DXVECTOR3 vPos, D3DXVECTOR3 rayStart, D3DXVECTOR3 rayEnd, float* pHitDistance )
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

void CX2EffectToolMain::DeleteAllSkinMeshAndSkinAnim()
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

void CX2EffectToolMain::OnFrameRenderUnit()
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

void CX2EffectToolMain::RenderUserUnitStartPos()
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
		const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
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

void CX2EffectToolMain::RenderNPCStartPosIndex( CX2DungeonSubStage::NPCData* pNPCData, int iStageNPCIndex /*= -1*/, int iNPCSubIndex /*= -1*/ )
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
			const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
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
			const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( iLineIndex );
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

void CX2EffectToolMain::RenderNPCUnitStartPostion( CX2Dungeon::DIFFICULTY_LEVEL dungeonDifficulty )
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

bool CX2EffectToolMain::GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const POINTS& mousepos )
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

	const CKTDGLineMap::LineData* pLineData = NULL;	

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

void CX2EffectToolMain::GetLineIndexInRect( float fLeft, float fTop, float fRight, float fBottom )
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

HRESULT CX2EffectToolMain::CreateD3DXTextMesh( IDirect3DDevice9* pd3dDevice, LPD3DXMESH* ppMesh, 
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


CX2EffectToolMain::STeamStartPos* CX2EffectToolMain::GetTeamStartPos( int iNPCStartPosKey )
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

void CEffectToolGame::Release()
{
	CX2Game::Release();
}


void CX2EffectToolMain::UpdateEffectSetMember ( const WCHAR* strText )
{	
	if( L"" == strText )
		return;

	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();		
	if( NULL == pEffectset )
		return;

	CX2EffectSet::EffectSetData* pEffectSetData = pEffectset->GetEffectSetTemplet( strText );
	if( NULL != pEffectSetData )
	{
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
		if( NULL != pDialog )
		{
			CDXUTComboBox* pComboBox = pDialog->GetComboBox( IDC_COMBO_EFFECT_SET_TYPE );
			if( NULL != pComboBox )
				pComboBox->SetSelectedByIndex( static_cast<int>(pEffectSetData->m_eEventTimerType) );

			CDXUTCheckBox* pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_AFFECTED_BY_UNIT_SCALE );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectSetData->m_bAffectedByUnitScale );

			CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_DELETE_SHAKE_COUNT );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pEffectSetData->m_iDeleteShakeCount );	
				pEditBox->SetText( wszTextOut );
			}

			CDXUTListBox* pListBoxParticle = pDialog->GetListBox( IDC_LIST_EFFECT_SET_PARTICLE );
			CDXUTListBox* pListBoxMesh = pDialog->GetListBox( IDC_LIST_EFFECT_SET_MESH );
			if( NULL != pListBoxParticle && NULL != pListBoxMesh )
			{
				pListBoxParticle->RemoveAllItems();
				pListBoxMesh->RemoveAllItems();
				int iIndex = 0;
				BOOST_FOREACH( CX2EffectSet::EffectData* pData, pEffectSetData->m_vecpEffectData )
				{
					switch( pData->m_eEffectType )
					{
					case CX2EffectSet::ET_PARTICLE_UI_MAJOR:
					case CX2EffectSet::ET_PARTICLE_UI_MINOR:
					case CX2EffectSet::ET_PARTICLE_GAME_MAJOR:
					case CX2EffectSet::ET_PARTICLE_GAME_MINOR:
						{
							pListBoxParticle->AddItem( pData->m_EffectName.c_str(), reinterpret_cast<void*>(iIndex) );
						}
						break;
					case CX2EffectSet::ET_DAMAGE_EFFECT:
					case CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR:
					case CX2EffectSet::ET_MESH_PLAYER_UI_MINOR:
					case CX2EffectSet::ET_MESH_PLAYER_GAME_MAJOR:
					case CX2EffectSet::ET_MESH_PLAYER_GAME_MINOR:
						{
							pListBoxMesh->AddItem( pData->m_EffectName.c_str(), reinterpret_cast<void*>(iIndex) );
						}
						break;
					default:
						break;
					}
					iIndex++;
				}
			}
			CDXUTListBox* pListBox = pDialog->GetListBox( IDC_LIST_EFFECT_SET_CAMERA_SHAKE );
			if( NULL != pListBox )
			{
				pListBox->RemoveAllItems();
				WCHAR wszTextOut[64];
				int iIndex = 0;
				BOOST_FOREACH( CX2EffectSet::CameraShakeData* pData, pEffectSetData->m_vecpCameraShakeData )
				{
					StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"CAMERA_SHAKE%d", iIndex );	
					pListBox->AddItem( wszTextOut , reinterpret_cast<void*>(iIndex++) );
				}
			}
		}
	}
}

void CX2EffectToolMain::UpdateEffectData ()
{
 	CX2EffectSet::EffectData* pEffectData = NULL;
	pEffectData = GetSelectedEffectData();

	//메쉬or파티클 data 설정
	if( NULL != pEffectData )
	{
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_PARTICLE_MESH_EDIT );
		if( NULL != pDialog )
		{
			CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_START_TIME );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_fStartAnimTime );	
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_BONE_NAME );
			if( NULL != pEditBox )
			{				
				pEditBox->SetText( pEffectData->m_PositionBoneName.c_str() );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_RIDING_BONE_NAME );
			if( NULL != pEditBox )
			{				
				pEditBox->SetText( pEffectData->m_PosRidingPetBoneName.c_str() );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_POSITION_OFFSET_X );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vPositionOffset.x);	
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Y );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vPositionOffset.y);	
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Z );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vPositionOffset.z);	
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_HYPER_NAME );
			if( NULL != pEditBox )
			{
				pEditBox->SetText( pEffectData->m_EffectNameHyper.c_str() );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_WEAPON_BONE_NAME );
			if( NULL != pEditBox )
			{
				pEditBox->SetText( pEffectData->m_PositionWeaponBoneName.c_str() );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_WEAPON_BONE_INDEX );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pEffectData->m_iWeaponIndex );	
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_WEAPON_BONE_MODEL_INDEX );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pEffectData->m_iModelIndex );	
				pEditBox->SetText( wszTextOut );
			}

#ifdef MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE // 오현빈
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_LIMIT_DISTANCE );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.0f", sqrt(pEffectData->m_fLimitDistanceSq));	
				pEditBox->SetText( wszTextOut );
			}
#endif // MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE

			CDXUTCheckBox* pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_TRACE );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bTrace );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_TRACE_MATRIX );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bTraceMatrix );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_TRACE_MORE );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bTraceMore );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_REVERSE_Y );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bReverseY );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_USE_LAND_POSITION );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bLandPosition );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bApplyUnitRotation );

#ifdef SERV_ARA_CHANGE_CLASS_SECOND
			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bApplyUnitRotationMirror );
#endif // SERV_ARA_CHANGE_CLASS_SECOND

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_APPLY_INSTANCE_SCALE );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bApplyInstanceScale );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_START_WHEN_TRACE_TARGET_DEAD );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bStartWhenTraceTargetDead );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_PASSIVE_EFFECT );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bPassiveEffect );

			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_USE_SUB_ATTACK_LIST_SET );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bUseSubAttackListSet );

#ifdef MODIFY_EFFECT_SET_TRACE_BONE // 오현빈
			pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pEffectData->m_bBoneTraceTargetMesh );
#endif // MODIFY_EFFECT_SET_TRACE_BONE
		}

		pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_PARTICLE_MESH_EDIT2 );
		if( NULL != pDialog )
		{
			//DLG_EFFECT_SET_PARTICLE_MESH_EDIT22
			CDXUTComboBox* pComboBox = pDialog->GetComboBox( IDC_COMBO_EFFECT_SET_EFFECT_TYPE );
			if( NULL != pComboBox )
				pComboBox->SetSelectedByIndex( static_cast<int>(pEffectData->m_eEffectType) );

			CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_ROTATE_X );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vRotateRel.x );
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_ROTATE_Y );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vRotateRel.y );
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_ROTATE_Z );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vRotateRel.z );
				pEditBox->SetText( wszTextOut );
			}

			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_SCALE_X );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vScale.x );
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_SCALE_Y );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vScale.y );
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_SCALE_Z );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vScale.z );
				pEditBox->SetText( wszTextOut );
			}

			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_X );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vScaleOnLeft.x );
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Y );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vScaleOnLeft.y );
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Z );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pEffectData->m_vScaleOnLeft.z );
				pEditBox->SetText( wszTextOut );
			}
		}
	}
}
void CX2EffectToolMain::UpdateEffectSetCameraData ( int iSelectedIndex_ )
{
	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();		
	if( NULL == pEffectset )
		return;

	//현재 선택된 EffectSetData 얻기
	CX2EffectSet::EffectSetData* pEffectSetData = NULL;
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( IDC_LIST_EFFECT_SET );
		if( NULL != pListBox )
		{
			if( NULL != pListBox->GetSelectedItem() )
			{
				pEffectSetData = pEffectset->GetEffectSetTemplet( pListBox->GetSelectedItem()->strText );
			}
		}
	}
	//현재 선택된 카메라 얻기
	CX2EffectSet::CameraShakeData* pCameraShakeData = NULL;
	if( static_cast<int>(pEffectSetData->m_vecpCameraShakeData.size()) > iSelectedIndex_ && iSelectedIndex_ >= 0 )
	{
		pCameraShakeData = pEffectSetData->m_vecpCameraShakeData[iSelectedIndex_];
	}

	//카메라 data 설정
	if( NULL != pCameraShakeData )
	{
		pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_CAMERA_EDIT );
		if( NULL != pDialog )
		{
			CDXUTComboBox* pComboBox = pDialog->GetComboBox( IDC_COMBO_EFFECT_SET_CAMERA_SHAKE_TYPE );
			if( NULL != pComboBox )
				pComboBox->SetSelectedByIndex( static_cast<int>(pCameraShakeData->m_eShakeType) );

			CDXUTCheckBox* pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_FIXED_LOOCK_VEC );
			if( NULL != pCheckBox )
				pCheckBox->SetChecked( pCameraShakeData->m_bFixedLookVec );

			CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_CAMERA_START_TIME );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fStartAnimTime );	
				pEditBox->SetText( wszTextOut );
			}
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LIMIT_DISTANCE );
			if( NULL != pEditBox )
			{
				WCHAR wszTextOut[64];
				StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%f", sqrt(static_cast<float>(pCameraShakeData->m_iLimitDistanceSq)) );	
				pEditBox->SetText( wszTextOut );
			}
			// ShakeType별로 사용하지 않는 컨트롤을 구분하기 위해 	
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_GAP );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_TIME );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);
			pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX );
			if( NULL != pEditBox ) pEditBox->SetVisible(false);

			switch( pCameraShakeData->m_eShakeType )
			{
			case CKTDGCamera::DECT_UP:
			case CKTDGCamera::DECT_DOWN:
			case CKTDGCamera::DECT_LEFT:
			case CKTDGCamera::DECT_RIGHT:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fOneDirSpeed );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fOneDirAccel );	
						pEditBox->SetText( wszTextOut );
					}
				} break;
			case CKTDGCamera::DECT_UP_DOWN:
			case CKTDGCamera::DECT_LEFT_RIGHT:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_GAP );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fTwoDirGap );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_TIME );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fTwoDirTime );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fTwoDirTimeGap );	
						pEditBox->SetText( wszTextOut );
					}
				} break;
			case CKTDGCamera::DECT_UP_DOWN_NO_RESET:
			case CKTDGCamera::DECT_LEFT_RIGHT_NO_RESET:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_GAP );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fTwoDirGap );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_TWO_DIR_TIME );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_fTwoDirTime );	
						pEditBox->SetText( wszTextOut );
					}
				} break;
			case CKTDGCamera::DECT_RANDOM:
				{
					CDXUTEditBox* pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pCameraShakeData->m_iEyeShakeCount );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_EyeMoveSpeed.m_Min );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_EyeMoveSpeed.m_Max );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_EyeMoveRange.m_Min );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%.3f", pCameraShakeData->m_EyeMoveRange.m_Max );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pCameraShakeData->m_iLookAtShakeCount );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pCameraShakeData->m_LookAtMoveSpeed.m_Min );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pCameraShakeData->m_LookAtMoveSpeed.m_Max );	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pCameraShakeData->m_EyeMoveRange.m_Min);	
						pEditBox->SetText( wszTextOut );
					}
					pEditBox = pDialog->GetEditBox( IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX );
					if( NULL != pEditBox )
					{
						pEditBox->SetVisible(true);
						WCHAR wszTextOut[64];
						StringCchPrintf( wszTextOut, ARRAY_SIZE(wszTextOut), L"%d", pCameraShakeData->m_EyeMoveRange.m_Max);	
						pEditBox->SetText( wszTextOut );
					}
				} break;
			}
		}
	}
}
CX2EffectSet::EffectSetData* CX2EffectToolMain::GetEffectSetDataByName( const WCHAR* strText )
{
	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();
	if( NULL != pEffectset )
	{
		return pEffectset->GetEffectSetTemplet( strText);
	}
	return NULL;
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_FIND( const WCHAR* strText )
{
	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();
	if( NULL == pEffectset )
		return;

	std::vector< CX2EffectSet::EffectSetData* >* vecEffectData = pEffectset->GetVecEffectSetTemplet();
	if( NULL == vecEffectData )
		return;

	// 대소문자 구분을 피하기 위해 모두 대문자로 변경.
	wstring wsFindName = strText;
	wstring wsEffectSetName;
	MakeUpperCase( wsFindName );


	bool bIsFind = false;
	// 현재 위치보다 아래 목록 중 검색.
	std::vector< CX2EffectSet::EffectSetData* >::iterator it = vecEffectData->begin();
	for( int iIndex = 0 ; it != vecEffectData->end(); ++it, ++iIndex )
	{
		if( NULL == *it )
			break;

		if( iIndex <= m_iSelectedEffectSetDataIndex )
			continue;
		wsEffectSetName = (*it)->m_wstrEffectSetName;
		MakeUpperCase( wsEffectSetName );
		if( wsEffectSetName.find( wsFindName.c_str(), 0 ) != wstring::npos )
		{
			CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
			if( NULL != pDialog)
			{
				CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
				if( NULL != pListBox )
				{
					pListBox->SelectItem( -1 );
					pListBox->SelectItem( iIndex );
					UpdateSelectedEffectSetData();
					bIsFind = true;
				}
			}
			break;
		}
	}

	// 아래 방향에서 못 찾았다면 한번 더 검색.
	if( false == bIsFind )
	{
		it = vecEffectData->begin();
		for( int iIndex = 0 ; it != vecEffectData->end(); ++it, ++iIndex )
		{
			if( NULL == *it )
				break;

			wsEffectSetName = (*it)->m_wstrEffectSetName;
			MakeUpperCase( wsEffectSetName );
			if( wsEffectSetName.find( wsFindName.c_str(), 0 ) != wstring::npos )
			{
				CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
				if( NULL != pDialog)
				{
					CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
					if( NULL != pListBox )
					{
						pListBox->SelectItem( -1 );
						pListBox->SelectItem( iIndex );
						UpdateSelectedEffectSetData();
						bIsFind = true;
					}
				}
				break;
			}
		}	 
	}

	//그래도 못찾았다면 팝업.
	if( false == bIsFind )
	{
		MessageBox(NULL, L"이펙트셋에 없는 이름입니다.", L"Error!", MB_OK);
	}

}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_DESC_FIND( const WCHAR* strText )
{
	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();
	if( NULL == pEffectset )
		return;

	std::vector< CX2EffectSet::EffectSetData* >* vecEffectData = pEffectset->GetVecEffectSetTemplet();
	if( NULL == vecEffectData )
		return;

	// 대소문자 구분을 피하기 위해 모두 대문자로 변경.
	wstring wsFindName = strText;
	wstring wsEffectSetDescName;
	MakeUpperCase( wsFindName );


	bool bIsFind = false;
	// 현재 위치보다 아래 목록 중 검색.
	std::vector< CX2EffectSet::EffectSetData* >::iterator it = vecEffectData->begin();
	for( int iIndex = 0 ; it != vecEffectData->end(); ++it, ++iIndex )
	{
		if( NULL == *it )
			break;

		if( iIndex <= m_iSelectedEffectSetDataIndex )
			continue;

		wsEffectSetDescName = GetEffectSetDesc( (*it)->m_wstrEffectSetName.c_str() );
		MakeUpperCase( wsEffectSetDescName );
		if( wsEffectSetDescName.find( wsFindName.c_str(), 0 ) != wstring::npos )
		{
			CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
			if( NULL != pDialog)
			{
				CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
				if( NULL != pListBox )
				{
					pListBox->SelectItem( -1 );
					pListBox->SelectItem( iIndex );
					UpdateSelectedEffectSetData();
					bIsFind = true;
				}
			}
			break;
		}
	}

	// 아래 방향에서 못 찾았다면 한번 더 검색.
	if( false == bIsFind )
	{
		it = vecEffectData->begin();
		for( int iIndex = 0 ; it != vecEffectData->end(); ++it, ++iIndex )
		{
			if( NULL == *it )
				break;

			wsEffectSetDescName = GetEffectSetDesc( (*it)->m_wstrEffectSetName.c_str() );
			MakeUpperCase( wsEffectSetDescName );
			if( wsEffectSetDescName.find( wsFindName.c_str(), 0 ) != wstring::npos )
			{
				CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
				if( NULL != pDialog)
				{
					CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
					if( NULL != pListBox )
					{
						pListBox->SelectItem( -1 );
						pListBox->SelectItem( iIndex );
						UpdateSelectedEffectSetData();
						bIsFind = true;
					}
				}
				break;
			}
		}	 
	}

	//그래도 못찾았다면 팝업.
	if( false == bIsFind )
	{
		MessageBox(NULL, L"이펙트셋에 없는 이름입니다.", L"Error!", MB_OK);
	}

}

void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_NAME_CHANGE( const WCHAR* strText )
{
	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();
	if( NULL == pEffectset )
		return;

	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		pEffectset->ChangeNameEffectSetTemplet( pEffectSetData->m_wstrEffectSetName, strText );
	}

	UpdateEffectList();

	//선택 갱신
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog)
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
		if( NULL != pListBox )
		{
			pListBox->SelectItem( -1 );
			pListBox->SelectItem( m_iSelectedEffectSetDataIndex );
			UpdateSelectedEffectSetData();
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_ADD( const WCHAR* strText )
{
	//이펙트셋 추가
	CreateEffectSetTempletDummy( strText );

	//UI 갱신
	UpdateEffectList();

	//추가된 항목 선택
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog)
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
		if( NULL != pListBox )
		{
			pListBox->SelectItem(pListBox->GetSize());
			UpdateSelectedEffectSetData();
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_MERGE( const WCHAR* strText )
{
	CX2EffectSet::EffectSetData* pSelectedEffect = GetSelectedEffectSetData();
	if( NULL == pSelectedEffect )
		return;

	//이펙트셋 머지 할 대상 이펙트셋
	CX2EffectSet::EffectSetData* pDestEffectSet = NULL;
	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetEffectSet() )
	{
		pDestEffectSet = g_pX2Game->GetEffectSet()->GetEffectSetTemplet( strText );
	}

	if( NULL == pDestEffectSet )
		return;

	BOOST_FOREACH( CX2EffectSet::EffectData* pData, pSelectedEffect->m_vecpEffectData )
	{
		CX2EffectSet::EffectData* pTempData = new CX2EffectSet::EffectData;
		*pTempData = *pData;
		pDestEffectSet->m_vecpEffectData.push_back( pTempData );
	}
	BOOST_FOREACH( CX2EffectSet::CameraShakeData* pData, pSelectedEffect->m_vecpCameraShakeData )
	{
		CX2EffectSet::CameraShakeData* pTempData = new CX2EffectSet::CameraShakeData;
		*pTempData = *pData;
		pDestEffectSet->m_vecpCameraShakeData.push_back( pTempData );
	}

	//UI 갱신
	UpdateEffectList();

	//변경된 항목 검색.
	UINT uiIndex = 0;
	if( NULL != g_pX2Game &&
		NULL != g_pX2Game->GetEffectSet() )
	{
		BOOST_FOREACH(CX2EffectSet::EffectSetData* pData, *g_pX2Game->GetEffectSet()->GetVecEffectSetTemplet() )
		{
			if( 0 == pData->m_wstrEffectSetName.compare( strText ) )
				break;
			++uiIndex;
		}
	}
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog)
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
		if( NULL != pListBox )
		{
			pListBox->SelectItem( -1 );
			pListBox->SelectItem( uiIndex );
			UpdateSelectedEffectSetData();
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_SAVE_AS( const WCHAR* strText )
{
	CX2EffectSet::EffectSetData* pSelectedEffectSet = GetSelectedEffectSetData();
	if( NULL == pSelectedEffectSet )
		return;

	//이펙트셋 추가
	CX2EffectSet::EffectSetData* pAddEffectSet = CreateEffectSetTempletDummy( strText );
	if( NULL == pAddEffectSet )
		return;

	*pAddEffectSet = *pSelectedEffectSet;
	pAddEffectSet->m_wstrEffectSetName = strText;

	//UI 갱신
	UpdateEffectList();

	//추가된 항목 선택
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog)
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
		if( NULL != pListBox )
		{
			pListBox->SelectItem(pListBox->GetSize());
			UpdateSelectedEffectSetData();
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_DEL( )
{
	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();
	if( NULL == pEffectset )
		return;

	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		pEffectset->DeleteEffectSetTemplet( pEffectSetData->m_wstrEffectSetName );
	}

	UpdateEffectList();

	//선택 갱신
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog)
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
		if( NULL != pListBox )
		{
			pListBox->SelectItem( -1 );
			pListBox->SelectItem( m_iSelectedEffectSetDataIndex );
			UpdateSelectedEffectSetData();
		}
	}
}
void CX2EffectToolMain::On_IDC_COMBO_EFFECT_SET_TYPE( CX2EffectSet::EFFECT_SET_TIMER_TYPE eEffectType_ )
{
	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		pEffectSetData->m_eEventTimerType = eEffectType_;
	}
}
void CX2EffectToolMain::On_IDC_CHECK_EFFECT_SET_AFFECTED_BY_UNIT_SCALE( bool bVal_ )
{
	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		pEffectSetData->m_bAffectedByUnitScale = bVal_;
	}
}
void CX2EffectToolMain::On_IDC_EDIT_EFFECT_SET_DELETE_SHAKE_COUNT( int iValue_ )
{
	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		pEffectSetData->m_iDeleteShakeCount = iValue_;
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_CAMERA_ADD()
{
	CX2EffectSet::EffectSetData* pEffectSet = GetSelectedEffectSetData();
	if( NULL != pEffectSet )
	{
		CX2EffectSet::CameraShakeData* pCameraShake = new CX2EffectSet::CameraShakeData;
		pEffectSet->m_vecpCameraShakeData.push_back(pCameraShake);
		UpdateEffectSetMember( pEffectSet->m_wstrEffectSetName.c_str() );
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_CAMERA_DEL()
{
	CX2EffectSet::EffectSetData* pEffectSet = GetSelectedEffectSetData();
	if( NULL != pEffectSet )
	{
		if( -1 != m_iSelectedCameraIndex &&
			static_cast<int>(pEffectSet->m_vecpCameraShakeData.size()) > m_iSelectedCameraIndex )
		{
			pEffectSet->m_vecpCameraShakeData.erase( pEffectSet->m_vecpCameraShakeData.begin() + m_iSelectedCameraIndex );
			UpdateEffectSetMember( pEffectSet->m_wstrEffectSetName.c_str() );
			UpdateEffectSetCameraData( m_iSelectedCameraIndex );
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_PARTICLE_ADD()	/// 파티클 추가,제거
{
	ChangeToolMode( MODE_EFFECT_SET_PARTICLE_ADD );
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_PARTICLE_DEL()
{
	CX2EffectSet::EffectSetData* pEffectSet = GetSelectedEffectSetData();
	//현재 이펙트셋이 선택 되어 있는지 체크
	if( NULL != pEffectSet )
	{
		//파티클 리스트 중에 선택된 것이 있는지 체크
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
		if( NULL != pDialog )
		{
			CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET_PARTICLE );
			if( NULL != pListBox && NULL != pListBox->GetSelectedItem() )
			{	
				//선택에 오류가 없다면 제거 체크
				if( -1 != m_iSelectedParticleMeshIndex &&
					static_cast<int>(pEffectSet->m_vecpEffectData.size()) > m_iSelectedParticleMeshIndex )
				{
					int iOldIndex = m_iSelectedParticleMeshIndex;
					pEffectSet->m_vecpEffectData.erase( pEffectSet->m_vecpEffectData.begin() + m_iSelectedParticleMeshIndex );

					UpdateEffectSetMember( pEffectSet->m_wstrEffectSetName.c_str() );
					UpdateSelectedEffectData(true);
					UpdateEffectData();

					SelectParticleList_EffectMember( iOldIndex );
				}
			}
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_CREATE_EFFECT_SET_BY_DAMAGE_EFFECT()
{
	CreateEffectSetByDamageEffect();
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_MESH_ADD()		/// 메쉬 추가,제거
{
	ChangeToolMode( MODE_EFFECT_SET_MESH_ADD );
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_MESH_DEL()
{
	CX2EffectSet::EffectSetData* pEffectSet = GetSelectedEffectSetData();
	//현재 이펙트셋이 선택 되어 있는지 체크
	if( NULL != pEffectSet )
	{
		//메쉬리스트 중에 선택된 것이 있는지 체크
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
		if( NULL != pDialog )
		{
			CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET_MESH );
			if( NULL != pListBox && NULL != pListBox->GetSelectedItem() )
			{	
				//선택에 오류가 없다면 제거 체크
				if( -1 != m_iSelectedParticleMeshIndex &&
					static_cast<int>(pEffectSet->m_vecpEffectData.size()) > m_iSelectedParticleMeshIndex )
				{
					int iOldIndex = m_iSelectedParticleMeshIndex;
					pEffectSet->m_vecpEffectData.erase( pEffectSet->m_vecpEffectData.begin() + m_iSelectedParticleMeshIndex );

					UpdateEffectSetMember( pEffectSet->m_wstrEffectSetName.c_str() );
					UpdateSelectedEffectData(true);
					UpdateEffectData();

					SelectMeshList_EffectMember( iOldIndex );
				}
			}
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_CUSTOM_PARTICLE_ADD()
{
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_CUSTOM_PARTICLE_ADD );
		if( NULL != pEditBox )
		{
			AddParticleAtCurrentEffectset( pEditBox->GetText() );
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_EFFECT_SET_CUSTOM_MESH_ADD()
{
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		CDXUTEditBox* pEditBox = pDialog->GetEditBox( CX2EffectToolMain::IDC_EDIT_EFFECT_SET_CUSTOM_MESH_ADD );
		if( NULL != pEditBox )
		{
			AddMeshAtCurrentEffectset( pEditBox->GetText() );
		}
	}
}
void CX2EffectToolMain::On_IDC_LIST_EFFECT_SET_PARTICLE_SELECTION( const WCHAR* strName_ )
{
	UnSelectMeshList();			// 메쉬 리스트 박스 선택 해제하기.
	UpdateSelectedEffectData(); // 선택 항목 갱신

	UpdateEffectData();			// 수치 갱신
	PreViewParticle( strName_ ); // 파티클 재생
}
void CX2EffectToolMain::On_IDC_LIST_EFFECT_SET_PARTICLE_DBLCLK( const WCHAR* strName_ )
{
	if( NULL == m_pParticleSystem )
		return;

	g_pEffectToolMain->On_IDC_BUTTON_EFFECT_SET_PARTICLE_ADD();

	vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
	UINT iFileIndex = 0;
	UINT iParticleIndex = 0;
	BOOST_FOREACH( wstring ParticleFileName, m_vecParticleFileName )
	{
		m_pParticleSystem->GetVecTempletSequences( ParticleFileName.c_str(), vecParticleTemplet );
		iParticleIndex = 0;
		BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequence* pEventSequence, vecParticleTemplet )
		{
			if( 0 == wcscmp(pEventSequence->GetName(), strName_ ) )
			{
				UpdateParticleListByIndex( iFileIndex );
				SelectParticleListByIndex( iParticleIndex );
				return;
			}
			++iParticleIndex;
		}
		++iFileIndex;
	}
	MessageBox(NULL, L"파티클 목록에 없는 이름 입니다.", L"Error!", MB_OK);
}
void CX2EffectToolMain::On_IDC_LIST_EFFECT_SET_MESH_DBLCLK( const WCHAR* strName_ )
{
	if( NULL == m_pXMeshPlayer )
		return;

	g_pEffectToolMain->On_IDC_BUTTON_EFFECT_SET_MESH_ADD();

	wstring wstrFindName = strName_;

	if(  NULL != GetSelectedEffectData() &&
		CX2EffectSet::ET_DAMAGE_EFFECT == GetSelectedEffectData()->m_eEffectType )
	{ //데미지 이펙트라면, 해당 이펙트의 메인메쉬 네임을 이용해 탐색.

		wstrFindName = GetMainMeshName_ByDamageEffectName( strName_ );
	}

	UINT iMeshIndex = 0;
	BOOST_FOREACH( CKTDGXMeshPlayer::XMeshTemplet* pData, m_pXMeshPlayer->GetVecTemplet() )
	{
		if( 0 == pData->templetName.compare( wstrFindName.c_str()) )
		{
			SelectMeshListByIndex( iMeshIndex );
			return;
		}
		++iMeshIndex;
	}
	MessageBox(NULL, L"메쉬 목록에 없는 이름 입니다.", L"Error!", MB_OK);
}
void CX2EffectToolMain::On_IDC_LIST_EFFECT_SET_MESH_SELECTION( const WCHAR* strName_ )
{
	UnSelectParticleList();		// 파티클 리스트 박스 선택 해제하기.
	UpdateSelectedEffectData();	// 선택 항목 갱신
	UpdateEffectData();			// 수치 갱신

	if(  NULL != GetSelectedEffectData() &&
		CX2EffectSet::ET_DAMAGE_EFFECT == GetSelectedEffectData()->m_eEffectType )
		PreVieweXMesh( GetMainMeshName_ByDamageEffectName( strName_ ).c_str() );	// 메쉬 재생
	else
		PreVieweXMesh( strName_ );	// 메쉬 재생
}
CX2EffectSet::EffectSetData* CX2EffectToolMain::CreateEffectSetTempletDummy( const WCHAR* strEffectSetName_ )
{
	CX2EffectSet* pEffectSet = g_pData->GetGameEffectSet();
	if( NULL != pEffectSet )
	{
		CX2EffectSet::EffectSetData* pEffectSetData = new CX2EffectSet::EffectSetData;
		pEffectSetData->m_wstrEffectSetName = strEffectSetName_;

		pEffectSet->CreateEffectSetTemplet( pEffectSetData );
		return pEffectSetData;
	}

	return NULL;
}
void CX2EffectToolMain::UpdateSelectedEffectData( bool bForce /* = true */ )
{
	if( true == bForce )
	{
		m_pSelectedEffectData = NULL;
		m_iSelectedParticleMeshIndex = -1;
	}

	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL == pEffectSetData )
		return;

	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		//파티클중에 선택된 것이 있는지 체크
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET_PARTICLE );
		if( NULL != pListBox && NULL != pListBox->GetSelectedItem() )
		{
			if( NULL == m_pSelectedEffectData )
			{
				int iIndex = reinterpret_cast<int>(pListBox->GetSelectedItem()->pData);
				if( static_cast<int>(pEffectSetData->m_vecpEffectData.size()) > iIndex && iIndex >= 0 )
				{
					m_pSelectedEffectData = pEffectSetData->m_vecpEffectData[iIndex];
					m_iSelectedParticleMeshIndex = iIndex;
				}
				return;
			}
		}
		//파티클에서 확인 됐다면 return.
		//메쉬중에 선택된 것이 있는지 체크.
		pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET_MESH );
		if( NULL != pListBox && NULL != pListBox->GetSelectedItem() )
		{
			if( NULL == m_pSelectedEffectData )
			{
				int iIndex = reinterpret_cast<int>(pListBox->GetSelectedItem()->pData);
				if( static_cast<int>(pEffectSetData->m_vecpEffectData.size()) > iIndex && iIndex >= 0)
				{
					m_pSelectedEffectData = pEffectSetData->m_vecpEffectData[iIndex];
					m_iSelectedParticleMeshIndex = iIndex;
				}
				return;
			}
		}
	}
}

CX2EffectSet::EffectData* CX2EffectToolMain::GetEffectDataByName( const WCHAR* strName_ )
{	
	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		BOOST_FOREACH( CX2EffectSet::EffectData* pData, pEffectSetData->m_vecpEffectData) 
		{
			if( 0 == pData->m_EffectName.compare( strName_ ) )
				return pData;
		}
	}
	return NULL;
}

void CX2EffectToolMain::UpdateSelectedCameraShake( bool bForce /*= true*/ )
{
	if( true == bForce )
		m_pSelectedCameraData = NULL;

	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL == pEffectSetData )
		return;

	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		//선택된 것이 있는지 체크
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET_CAMERA_SHAKE );
		if( NULL != pListBox && NULL != pListBox->GetSelectedItem() )
		{
			if( NULL == m_pSelectedCameraData )
			{
				int iIndex = reinterpret_cast<int>(pListBox->GetSelectedItem()->pData);
				if( static_cast<int>(pEffectSetData->m_vecpCameraShakeData.size()) > iIndex)
				{
					m_pSelectedCameraData = pEffectSetData->m_vecpCameraShakeData[iIndex];
					m_iSelectedCameraIndex = iIndex;
				}
				return;
			}
		}
	}
}

CX2EffectSet::CameraShakeData* CX2EffectToolMain::GetSelectedCameraShakeData()
{
	if( NULL == m_pSelectedCameraData )
		UpdateSelectedCameraShake();

	return m_pSelectedCameraData;
}
void CX2EffectToolMain::UpdateSelectedEffectSetData(bool bForce /*= true */)
{
	if( true == bForce )
		m_pSelectedEffectSetData = NULL;

	m_iSelectedEffectSetDataIndex = -1;

	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog)
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
		if( NULL != pListBox && NULL != pListBox->GetSelectedItem() )
		{
			if( NULL == m_pSelectedEffectSetData )
				m_pSelectedEffectSetData = GetEffectSetDataByName( pListBox->GetSelectedItem()->strText );
				
			m_iSelectedEffectSetDataIndex = pListBox->GetSelectedIndex();

			SetWindowTextW( g_pKTDXApp->GetHWND(), m_pSelectedEffectSetData->m_wstrEffectSetName.c_str() );
		}
	}
}
CX2EffectSet::EffectSetData* CX2EffectToolMain::GetSelectedEffectSetData()
{
	if( NULL == m_pSelectedEffectSetData )
		UpdateSelectedEffectSetData();

	return m_pSelectedEffectSetData;
}
CX2EffectSet::EffectData* CX2EffectToolMain::GetSelectedEffectData()
{
	if( NULL == m_pSelectedEffectData )
		UpdateSelectedEffectData();

	return m_pSelectedEffectData;
}

void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_START_TIME( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_fStartAnimTime = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_BONE_NAME( const WCHAR* strValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_PositionBoneName = strValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_RIDING_BONE_NAME( const WCHAR* strValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_PosRidingPetBoneName = strValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_X( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vPositionOffset.x = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Y( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vPositionOffset.y = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_POSITION_OFFSET_Z( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vPositionOffset.z = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_TRACE( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bTrace = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_TRACE_MATRIX( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bTraceMatrix = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_TRACE_MORE( bool bVal_)
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bTraceMore = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_REVERSE_Y( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bReverseY = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_USE_LAND_POSITION( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bLandPosition = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
		if( true == bVal_ &&
			true == pEffectData->m_bApplyUnitRotationMirror )
		{
			MessageBox(NULL, L"APPLY_UNIT_ROTATION_MIRROR과 같이 사용 할 수 없습니다.", L"Error!", MB_OK);

			CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_PARTICLE_MESH_EDIT );
			if( NULL != pDialog )
			{
				CDXUTCheckBox* pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION );
				if( NULL != pCheckBox )
					pCheckBox->SetChecked( false );
			}

			return;
		}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

		pEffectData->m_bApplyUnitRotation = bVal_;
	}
}
#ifdef SERV_ARA_CHANGE_CLASS_SECOND
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		if( true == bVal_ &&
			true == pEffectData->m_bApplyUnitRotation )
		{
			MessageBox(NULL, L"APPLY_UNIT_ROTATION과 같이 사용 할 수 없습니다.", L"Error!", MB_OK);

			CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_PARTICLE_MESH_EDIT );
			if( NULL != pDialog )
			{
				CDXUTCheckBox* pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_APPLY_UNIT_ROTATION_MIRROR );
				if( NULL != pCheckBox )
					pCheckBox->SetChecked( false );
			}

			return;
		}
		pEffectData->m_bApplyUnitRotationMirror = bVal_;
	}
}
#endif // SERV_ARA_CHANGE_CLASS_SECOND

void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_APPLY_INSTANCE_SCALE( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bApplyInstanceScale = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_START_WHEN_TRACE_TARGET_DEAD( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bStartWhenTraceTargetDead = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_PASSIVE_EFFECT( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bPassiveEffect = bVal_;
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_USE_SUB_ATTACK_LIST_SET( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_bUseSubAttackListSet = bVal_;
	}
}
#ifdef MODIFY_EFFECT_SET_TRACE_BONE // 오현빈
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH( bool bVal_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		if( true == bVal_ )
		{
			bool bCanAply= true;
			switch( pEffectData->m_eEffectType ) 
			{
			case CX2EffectSet::ET_DAMAGE_EFFECT:
			case CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR:
			case CX2EffectSet::ET_MESH_PLAYER_UI_MINOR:
			case CX2EffectSet::ET_MESH_PLAYER_GAME_MAJOR:
			case CX2EffectSet::ET_MESH_PLAYER_GAME_MINOR:
				{
					bool bDuplicateCheck = false;
					// 이펙트셋을 순회 하여 이미 타겟메쉬를 지정 했는지 검사
					CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
					if( NULL != pEffectSetData )
					{
						BOOST_FOREACH( const CX2EffectSet::EffectData* pEffet, pEffectSetData->m_vecpEffectData )
						{
							if( true == pEffet->m_bBoneTraceTargetMesh )
							{

								MessageBox(NULL, L"이펙트셋 마다 1개의 메쉬에만 적용 할 수 있습니다.", L"Error!", MB_OK);
								bCanAply = false;
								break;
							}
						}
					}
					pEffectData->m_bBoneTraceTargetMesh = bVal_;
				} break;
			default:
				{
					MessageBox(NULL, L"메쉬 플레이어 이펙트에만 적용 할 수 있습니다.", L"Error!", MB_OK);
					bCanAply = false;
				}
				break;
			}

			if( false == bCanAply )
			{
				pEffectData->m_bBoneTraceTargetMesh = false;
				CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_PARTICLE_MESH_EDIT );
				if( NULL != pDialog )
				{
					CDXUTCheckBox* pCheckBox = pDialog->GetCheckBox( IDC_CHECK_EFFECT_SET_BONE_TRACE_TARGET_MESH );
					if( NULL != pCheckBox )
						pCheckBox->SetChecked( pEffectData->m_bBoneTraceTargetMesh );
				}
			}
		}
		else
		{
			pEffectData->m_bBoneTraceTargetMesh = bVal_;
		}
	}
}
#endif // MODIFY_EFFECT_SET_TRACE_BONE
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_HYPER_NAME( const WCHAR* strValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_EffectNameHyper = strValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_NAME( const WCHAR* strValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_PositionWeaponBoneName = strValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_INDEX( int iValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_iWeaponIndex = iValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_WEAPON_BONE_MODEL_INDEX( int iValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_iModelIndex = iValue_;
	}
}

#ifdef MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_LIMIT_DISTANCE( int iValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_fLimitDistanceSq = iValue_ * iValue_;
	}
}
#endif // MODIFY_CREATE_EFFECT_SET_LIMIT_DISTANCE

void CX2EffectToolMain::ON_IDC_COMBO_EFFECT_SET_EFFECT_TYPE( CX2EffectSet::EFFECT_TYPE eEffectType_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_eEffectType = eEffectType_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_ROTATE_X( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vRotateRel.x = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_ROTATE_Y( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vRotateRel.y = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_ROTATE_Z( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vRotateRel.z = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_SCALE_X( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vScale.x = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_SCALE_Y( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vScale.y = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_SCALE_Z( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vScale.z = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_X( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vScaleOnLeft.x = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Y( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vScaleOnLeft.y = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_SCALE_ON_LEFT_Z( float fValue_ )
{
	CX2EffectSet::EffectData* pEffectData = GetSelectedEffectData();
	if( NULL != pEffectData )
	{
		pEffectData->m_vScaleOnLeft.z = fValue_;
	}
}
void CX2EffectToolMain::ON_IDC_COMBO_EFFECT_SET_CAMERA_SHAKE_TYPE( CKTDGCamera::SHAKE_TYPE eShakeType_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_eShakeType = eShakeType_;
		UpdateEffectSetCameraData( m_iSelectedCameraIndex );
	}
}
void CX2EffectToolMain::ON_IDC_CHECK_EFFECT_SET_FIXED_LOOCK_VEC( bool bValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_bFixedLookVec = bValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_CAMERA_START_TIME( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_fStartAnimTime = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_LIMIT_DISTANCE( int iValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_iLimitDistanceSq = iValue_ * iValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_ONE_DIR_SPEED( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_fOneDirSpeed = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_ONE_DIR_ACCEL( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_fOneDirAccel = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_TWO_DIR_GAP( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_fTwoDirGap = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_TWO_DIR_TIME( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_fTwoDirTime = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_TWO_DIR_TIME_GAP( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_fTwoDirTimeGap = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_EYE_SHAKE_COUNT( int iValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_iEyeShakeCount = iValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MIN( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_EyeMoveSpeed.m_Min = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_SPEED_MAX( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_EyeMoveSpeed.m_Max = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MIN( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_EyeMoveRange.m_Min = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_EYE_MOVE_RANGE_MAX( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_EyeMoveRange.m_Max = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_LOOK_AT_SHAKE_COUNT( int iValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_iLookAtShakeCount = iValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MIN( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_LookAtMoveSpeed.m_Min = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_SPEED_MAX( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_LookAtMoveSpeed.m_Max = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MIX( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_LookAtMoveRange.m_Min = fValue_ ;
	}
}
void CX2EffectToolMain::ON_IDC_EDIT_EFFECT_SET_LOOK_AT_MOVE_RANGE_MAX( float fValue_ )
{
	CX2EffectSet::CameraShakeData* pCameraData = GetSelectedCameraShakeData();
	if( NULL != pCameraData )
	{
		pCameraData->m_LookAtMoveRange.m_Max = fValue_ ;
	}
}
/** @function : SelectParticleList_EffectMember
	@brief : 이펙트 멤버의 파티클 리스트를 인덱스값으로 선택 
*/
void CX2EffectToolMain::SelectParticleList_EffectMember( int iIndex )
{
	// 이펙트 멤버의 파티클 리스트 선택
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET_PARTICLE );
		if( NULL != pListBox )
		{
			pListBox->SelectItem( iIndex );
		}
	}
}
/** @function : SelectMeshList_EffectMember
	@brief : 이펙트 멤버의 메쉬 리스트를 인덱스값으로 선택 
*/
void CX2EffectToolMain::SelectMeshList_EffectMember( int iIndex )
{
	// 이펙트 멤버의 파티클 리스트 선택
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET_MESH );
		if( NULL != pListBox )
		{
			pListBox->SelectItem( iIndex );
		}
	}
}

//메쉬, 파티클 리스트 선택 해제
void CX2EffectToolMain::UnSelectParticleList()
{	// 파티클 리스트 박스 선택 해제하기.
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox(CX2EffectToolMain::IDC_LIST_EFFECT_SET_PARTICLE);
		if( NULL != pListBox && NULL != pListBox->GetSelectedItem())
		{
			pListBox->SelectItem( -1 );
		}
	}
}
void CX2EffectToolMain::UnSelectMeshList()
{	// 메쉬 리스트 박스 선택 해제하기.
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_MEMBER );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox(CX2EffectToolMain::IDC_LIST_EFFECT_SET_MESH);
		if( NULL != pListBox && NULL != pListBox->GetSelectedItem())
		{
			pListBox->SelectItem( -1 );
		}
	}
}
// 파티클 미리보기
void CX2EffectToolMain::PreViewParticle( const WCHAR* strName_ )
{
	m_sPreviewParticle.SetIsCreate(true);
	m_sPreviewParticle.SetParticleName( strName_ );
}
// 메쉬 미리보기
void CX2EffectToolMain::PreVieweXMesh( const WCHAR* strName_ )
{
	m_sPreviewXMeshPlayer.SetIsCreate(true);
	m_sPreviewXMeshPlayer.SetXMeshName( strName_ );
}

// 파티클 스크립트에서 설정된 범위형 값을 실제 사용되는 확정형 값으로 재설정.
void CX2EffectToolMain::SetRandomValue( CKTDGParticleSystem::CParticleEventSequence* pParticle_ )
{	
	if( NULL == pParticle_ )
		return;

	CMinMax<float> cTempFloat =	pParticle_->GetDrawCount_Origin();
	pParticle_->SetDrawCount( static_cast<int>(cTempFloat.GetRandomNumInRange()) );

	cTempFloat = pParticle_->GetLandPosition_Origin();
	pParticle_->SetLandPosition( cTempFloat.GetRandomNumInRange() );

	cTempFloat = pParticle_->GetTriggerCount_Origin();
	pParticle_->SetTriggerCount( static_cast<int>(cTempFloat.GetRandomNumInRange()) );

	cTempFloat = pParticle_->GetTriggerTime_Origin();
	pParticle_->SetTriggerTime( cTempFloat.GetRandomNumInRange() );

	CMinMax<D3DXVECTOR3> cTempVec = pParticle_->GetAddRotateRel_Origin();
	pParticle_->SetAddRotateRel( cTempVec.GetRandomNumInRange() );

	cTempVec = pParticle_->GetBlackHolePosition_Origin();	
	pParticle_->SetBlackHolePosition( cTempVec.GetRandomNumInRange() );

	cTempVec = pParticle_->GetLightPos_Origin();
	pParticle_->SetLightPos( cTempVec.GetRandomNumInRange() );

	cTempVec = pParticle_->GetvSphericalEmitRotation_Origin();
	pParticle_->SetSphericalEmitRotation( cTempVec.GetRandomNumInRange() );
}

wstring CX2EffectToolMain::GetMainMeshName_ByDamageEffectName( const WCHAR* strName_ )
{
	if( NULL == g_pX2Game )
		return L"";

	if( NULL == g_pX2Game->GetDamageEffect() )
		return L"";

	return g_pX2Game->GetDamageEffect()->GetMainMeshNameByDamageEffectName( strName_ );
}
void CX2EffectToolMain::SetReadyParticleList( const CX2EffectSet::EffectData* pEffectData_ )
{
	TimedParticle TempTimeParticle;

	switch( pEffectData_->m_eEffectType )
	{
	case CX2EffectSet::ET_DAMAGE_EFFECT:
		{ 
			if( NULL != g_pX2Game &&
				NULL != g_pX2Game->GetDamageEffect() )
			{
				// 데미지 이펙트에서 생성한 파티클 등록.
				vector<wstring> vecParticleList;
				g_pX2Game->GetDamageEffect()->GetParticleListByEffectName( pEffectData_->m_EffectName.c_str(), vecParticleList );

				BOOST_FOREACH( wstring wstrParticleName, vecParticleList )
				{
					TempTimeParticle.m_wstrParticleName = wstrParticleName;
					m_vecReadyParticle.push_back( TempTimeParticle );
				}

				// 데미지 이펙트에서 생성한 이펙트 등록.
				vector<wstring> vecDamageEffectList;
				g_pX2Game->GetDamageEffect()->GetDamageEffectListByEffectName( pEffectData_->m_EffectName.c_str(), vecDamageEffectList );
				BOOST_FOREACH( wstring wstrParticleName, vecDamageEffectList )
				{
					CX2EffectSet::EffectData* pTempEffectData = new CX2EffectSet::EffectData;
					pTempEffectData->m_EffectName = wstrParticleName;
					pTempEffectData->m_eEffectType = CX2EffectSet::ET_DAMAGE_EFFECT;
					// 재귀 호출.
					SetReadyParticleList( pTempEffectData );
					SetReadyMeshList( pTempEffectData );
				}
			}

		} break;

		// 일반 파티클 등록.
	case CX2EffectSet::ET_PARTICLE_UI_MAJOR:
	case CX2EffectSet::ET_PARTICLE_UI_MINOR:
	case CX2EffectSet::ET_PARTICLE_GAME_MAJOR:
	case CX2EffectSet::ET_PARTICLE_GAME_MINOR:
		{
// 			TempTimeParticle.m_bTraceUnit = pEffectData_->m_bTrace;
// 			TempTimeParticle.m_fDelayTimeToStart = pEffectData_->m_fStartAnimTime;
// 			TempTimeParticle.m_vRelativePosition = pEffectData_->m_vPositionOffset;
// 			TempTimeParticle.m_wstrBoneName = pEffectData_->m_PositionBoneName;
			TempTimeParticle.m_wstrParticleName = pEffectData_->m_EffectName;

			m_vecReadyParticle.push_back( TempTimeParticle );
		} break;
	default: 
		{
			 
		} break;
	}
}
void CX2EffectToolMain::SetReadyMeshList( const CX2EffectSet::EffectData* pEffectData_ )
{
	TimedXMeshPlayer TempMeshPlayer;
	switch( pEffectData_->m_eEffectType )
	{
	case CX2EffectSet::ET_DAMAGE_EFFECT:
		TempMeshPlayer.m_wstrXMeshPlayerName = GetMainMeshName_ByDamageEffectName( pEffectData_->m_EffectName.c_str() );
		m_vecReadyXMeshPlayer.push_back( TempMeshPlayer );
		break;
	case CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR:
	case CX2EffectSet::ET_MESH_PLAYER_UI_MINOR:
	case CX2EffectSet::ET_MESH_PLAYER_GAME_MAJOR:
	case CX2EffectSet::ET_MESH_PLAYER_GAME_MINOR:
		TempMeshPlayer.m_wstrXMeshPlayerName = pEffectData_->m_EffectName;
		m_vecReadyXMeshPlayer.push_back( TempMeshPlayer );
		break;
	default: 
		break;
	}
// 	TempMeshPlayer.m_bRelativeToUnit = true;
// 	TempMeshPlayer.m_bTraceUnit = pEffectData_->m_bTrace;
// 	TempMeshPlayer.m_fDelayTimeToStart = pEffectData_->m_fStartAnimTime;
// 	TempMeshPlayer.m_vRelativePosition = pEffectData_->m_vPositionOffset;
// 	TempMeshPlayer.m_wstrBoneName = pEffectData_->m_PositionBoneName;

}
void CX2EffectToolMain::ReadyCurrentEffectSet()
{
	if( true == m_bIsReadyEffectSetResource )
		return;

	CX2EffectSet::EffectSetData* pEffectSetData = GetSelectedEffectSetData();
	if( NULL != pEffectSetData )
	{
		// 파티클 레디 등록.
		m_vecReadyParticle.clear();
		BOOST_FOREACH( CX2EffectSet::EffectData* pData, pEffectSetData->m_vecpEffectData )
		{
			SetReadyParticleList( pData );
		}

		// 메쉬 등록
		m_vecReadyXMeshPlayer.clear();
		BOOST_FOREACH( CX2EffectSet::EffectData* pData, pEffectSetData->m_vecpEffectData )
		{
			SetReadyMeshList( pData );
		}

		// 카메라 등록
		m_vecCameraShake.clear();
		BOOST_FOREACH ( CX2EffectSet::CameraShakeData* pData, pEffectSetData->m_vecpCameraShakeData)
		{
			CX2EffectSet::CameraShakeData* pTempData =new CX2EffectSet::CameraShakeData;
			*pTempData = *pData;
			m_vecCameraShake.push_back( pTempData );
		}		
		ReadyEffectDevices();

		m_bIsReadyEffectSetResource = true;
	}
}
void CX2EffectToolMain::ON_IDC_BUTTON_LOAD_BASE_PARTICLE_FILE()
{
	if( false == LoadMjoarParticleFile( L"GameMajorParticle" ) )
		return;

	int iIndex = 2;
	while( true )
	{
		WCHAR wszFileName[512] = L"";
		StringCchPrintf( wszFileName, ARRAY_SIZE(wszFileName), L"GameMajorParticle%d.txt", iIndex++ );	
		if( false == LoadMjoarParticleFile(wszFileName) )
			break;
		m_wstrLastBaseFileName = wszFileName;
		m_uiParticleFileNum = iIndex;
	}
	// 파티클 리스트에 업데이트.;
	UpdateParticleListCurrentSelect();
}
void CX2EffectToolMain::UpdateParticleListCurrentSelect()
{	
	// 파티클 리스트에 업데이트.;
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
	if( NULL != pDialog )
	{
		CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
		if( NULL != pComboBox )
		{
			// 현재 선택되어 있는 콤보박스의 인덱스 얻기.
			UINT uiIndex = reinterpret_cast<UINT>(pComboBox->GetSelectedData());

			// 인덱스를 통해 파티클 리스트 갱신.
			UpdateParticleListByIndex( uiIndex );
		}
	}
}
void CX2EffectToolMain::ON_IDC_BUTTON_LOAD_PARTICLE_FILE( const WCHAR* strFileName_ )
{
	// 정보 로드.
	LoadMjoarParticleFile(strFileName_);

	// 파티클 리스트에 업데이트.
	UpdateParticleListCurrentSelect();
}
void CX2EffectToolMain::ON_IDC_BUTTON_SAVE_PARTICLE_FILE()
{
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
	if( NULL != pDialog )
	{
		CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
		if( NULL != pComboBox )
		{
			// 현재 선택되어 있는 콤보박스의 인덱스 얻기.
			UINT uiIndex = reinterpret_cast<UINT>(pComboBox->GetSelectedData());
			if( -1 != uiIndex )
			{
				// 파티클 정보 얻기.
				vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
				if( true == m_pParticleSystem->GetVecTempletSequences( m_vecParticleFileName[uiIndex].c_str(), vecParticleTemplet ) )
				{
					// 파일 경로 지정.
					wstring wsParticleFilePath = GetParticleFilePath(m_vecParticleFileName[uiIndex].c_str());
					m_pFileManager->SaveMajorParticle(wsParticleFilePath.c_str() , vecParticleTemplet );
				}
			}
		}
	}	
}
wstring CX2EffectToolMain::GetParticleFilePath( const WCHAR* wcParticleName_ )
{
	wstring wsFileFullPath = L"";
	map<wstring, wstring>::iterator it = m_mapParticleFilePath.find( wcParticleName_ );
	if( it != m_mapParticleFilePath.end() )
	{
		wsFileFullPath = it->second;
	}
	else
	{
		GetMajorResourcePath( wsFileFullPath );
		wsFileFullPath += wcParticleName_;
	}

	return wsFileFullPath;
}
void CX2EffectToolMain::ON_IDC_BUTTON_PARTICLE_BASE_EDITOR()
{
	if( NULL != m_pParticleEditor )
	{
		CKTDGParticleSystem::CParticleEventSequence* pParticle = NULL;

		CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet = NULL;
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
		if( NULL != pDialog )
		{
			CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
			if( NULL != pListBox )
			{
				DXUTListBoxItem* pItem = (static_cast<CDXUTListBox*>(pListBox))->GetSelectedItem();
				if( pItem != NULL )						
				{
					pParticle = GetParticleByNameAndIndex( pItem->strText );
				}
			}
		}
		m_pParticleEditor->CreateParticleEditor_Base( pParticle, m_wstrCurrentParticleFileName );
	}
}
void CX2EffectToolMain::ON_IDC_BUTTON_PARTICLE_ETC_EDITOR()
{
	if( NULL != m_pParticleEditor )
	{
		CKTDGParticleSystem::CParticleEventSequence* pParticle = NULL;

		CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet = NULL;
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
		if( NULL != pDialog )
		{
			CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
			if( NULL != pListBox )
			{
				DXUTListBoxItem* pItem = (static_cast<CDXUTListBox*>(pListBox))->GetSelectedItem();
				if( pItem != NULL )						
				{
					pParticle = GetParticleByNameAndIndex( pItem->strText );
				}
			}
		}
		m_pParticleEditor->CreateParticleEditor_Etc( pParticle, m_wstrCurrentParticleFileName );
	}
}
void CX2EffectToolMain::ON_IDC_BUTTON_INIT_PARTICLE_FILE()
{
	m_vecParticleFileName.clear();

	SAFE_DELETE(m_pParticleSystem);
	//m_pParticleSystem			= new CKTDGParticleSystem( g_pKTDXApp->GetDevice() );

	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
	if( NULL != pDialog )
	{
		CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
		if( NULL != pComboBox )
		{
			pComboBox->RemoveAllItems();
		}

		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
		if( NULL != pListBox )
		{
			pListBox->RemoveAllItems();
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_PARTICLE_ADD_OTHER_FILE()
{
	if( NULL == m_pParticleSystem )
		return;


	if( 0 == m_wstrCurrentParticleFileName.compare( m_wstrLastBaseFileName.c_str() ) )
	{
		MessageBox(NULL, L"같은 파일로 옮길 수 없습니다.", L"Error!", MB_OK);
		return ;
	}

	// 현재 파티클 파일에 있는 파티클 갯수 파악.
	vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
	m_pParticleSystem->GetVecTempletSequences( m_wstrCurrentParticleFileName.c_str(), vecParticleTemplet );
	UINT uiCurrentFileParticleNum = vecParticleTemplet.size();

	if( 0 == uiCurrentFileParticleNum )
	{
		MessageBox(NULL, L"빈 파일입니다.", L"Error!", MB_OK);
		return;
	}

	// 이동 할 파일 파티클 갯수 파악
	m_pParticleSystem->GetVecTempletSequences( m_wstrLastBaseFileName.c_str(), vecParticleTemplet );
	UINT uiLastFileParticleNum = vecParticleTemplet.size();

	// 추가 불가능 하다면 새로운 파일 추가
	if( 400 <= (uiLastFileParticleNum + uiCurrentFileParticleNum ) )
	{
		// 새로운 파일 추가
		WCHAR wszFileName[512] = L"";
		StringCchPrintf( wszFileName, ARRAY_SIZE(wszFileName), L"GameMajorParticle%d.txt", m_uiParticleFileNum++ );	
		m_wstrLastBaseFileName = wszFileName;

	
		m_vecParticleFileName.push_back( m_wstrLastBaseFileName.c_str());
		AddParticleFileComboBox( m_wstrLastBaseFileName.c_str() );
	}

	// 파일 이동.
	m_pParticleSystem->MoveParticleTempletFile( m_wstrCurrentParticleFileName.c_str(), m_wstrLastBaseFileName.c_str() );
	
	// 갱신
	UpdateParticleListByFileName( m_wstrCurrentParticleFileName.c_str() );
}

void CX2EffectToolMain::On_IDC_BUTTON_FIND_TEXTURE( const WCHAR* strText )
{
	// to do : 특정 텍스쳐명이 포함된 파티클 리스트 생성.

	ClearParticleFindList();

	// 모든 파일에 대해 검색 하기 위해 순회.
	BOOST_FOREACH( wstring wstrFileName,  m_vecParticleFileName )
	{
		// 파티클 파일 별 파티클 시퀀스 얻기.
		vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
		m_pParticleSystem->GetVecTempletSequences( wstrFileName.c_str(), vecParticleTemplet );

		// 파티클 시퀀스 순회.
		BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequence* pEventSequence, vecParticleTemplet )
		{
		
		// 사용하고 있는 텍스쳐를 얻기 위해 이벤트 리스트 순회.
			if( NULL != pEventSequence->GetEventList() )
			{
				CKTDGParticleSystem::CParticleEventList::iterator it = pEventSequence->GetEventList()->begin();
				for( ; it != pEventSequence->GetEventList()->end(); ++it )
				{
		// 텍스쳐 형태의 이벤트 리스트 체크
					if( CKTDGParticleSystem::ET_TEXTURE == (*it)->GetEventType() )
					{
						CKTDGParticleSystem::CParticleEvent_Texture* pEvent = dynamic_cast<CKTDGParticleSystem::CParticleEvent_Texture*>( *it );
						if( NULL != pEvent )
						{
		// 해당 텍스쳐를 사용하는 파티클 발견!!
							if( true == IsEqualTextureName( pEvent->GetTextureName(), strText ) )
							{
								// 목록 추가
								AddParticleFindList( pEventSequence->GetName() );
								break;
							}
						}		
						//static_cast<CKTDGParticleSystem::CParticleEventList>(*it);
					}
				}
			}
		}
	}

//	m_pParticleSystem->GetVecTempletSequences()
}
bool CX2EffectToolMain::IsEqualTextureName( const WCHAR* pSrcTexture_, const WCHAR* pDestTexture )
{
	// 대,소문자 구분 하지 않고 
	// 해당 문자열이 포함된 모든 항목을 검사.

	// 변경 필요 하다면 이 함수를 변경해야함.

	wstring wstrSrcTexture = pSrcTexture_;
	MakeUpperCase( wstrSrcTexture );
	wstring wstrDestTexture = pDestTexture;
	MakeUpperCase( wstrDestTexture );

	if( wstrSrcTexture.find( wstrDestTexture.c_str(), 0 ) != wstring::npos )
	{
		return true;
	}

	return false;
}

void CX2EffectToolMain::ClearParticleFindList()
{
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST_FIND_TEXTURE );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST_FIND_TEXTURE );
		if( NULL != pListBox )
		{
			pListBox->SelectItem(-1);
			pListBox->RemoveAllItems();
		}
	}
}

void CX2EffectToolMain::AddParticleFindList( const WCHAR* pParitcleName_ )
{
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST_FIND_TEXTURE );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST_FIND_TEXTURE );
		if( NULL != pListBox )
		{
			pListBox->AddItem( pParitcleName_, NULL );
		}
	}
}

void CX2EffectToolMain::On_IDC_BUTTON_PARTICLE_LIST_FIND( const WCHAR* strText )
{
	// 현재 선택된 파티클 인덱스
	int iSelectedParticleIndex = -1; 

	// 파티클 벡터 얻기.
	vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
	if( NULL != pDialog )
	{
		CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
		if( NULL != pComboBox )
		{
			// 현재 선택되어 있는 콤보박스의 인덱스 얻기.
			UINT uiIndex = reinterpret_cast<UINT>(pComboBox->GetSelectedData());
			m_pParticleSystem->GetVecTempletSequences( m_vecParticleFileName[uiIndex].c_str() , vecParticleTemplet );
		}

		CDXUTListBox* pListBox = pDialog->GetListBox( IDC_LIST_PARTICLE_LIST );
		if( NULL != pListBox )
		{
			iSelectedParticleIndex = pListBox->GetSelectedIndex();
		}
	}

	if( true == vecParticleTemplet.empty() )
		return;

	// 대소문자 구분을 피하기 위해 모두 대문자로 변경.
	wstring wsFindName = strText;
	MakeUpperCase( wsFindName );

	bool bIsFind = false;
	// 현재 위치보다 아래 목록 중 검색.

	vector<CKTDGParticleSystem::CParticleEventSequence*>::iterator it = vecParticleTemplet.begin();
	for( int iIndex = 0 ; it != vecParticleTemplet.end(); ++it, ++iIndex )
	{
		if( NULL == *it )
			break;

		if( iIndex <= iSelectedParticleIndex )
			continue;

		wstring wsParticleName = (*it)->GetName();
		MakeUpperCase( wsParticleName );
		if( wsParticleName.find( wsFindName.c_str(), 0 ) != wstring::npos )
		{
			if( NULL != pDialog)
			{
				CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
				if( NULL != pListBox )
				{
					pListBox->SelectItem( -1 );
					pListBox->SelectItem( iIndex );
					bIsFind = true;
				}
			}
			break;
		}
	}

	// 아래 방향에서 못 찾았다면 한번 더 검색.
	if( false == bIsFind )
	{
		it = vecParticleTemplet.begin();
		for( int iIndex = 0 ; it != vecParticleTemplet.end(); ++it, ++iIndex )
		{
			if( NULL == *it )
				break;

			wstring wsParticleName = (*it)->GetName();
			MakeUpperCase( wsParticleName );
			if( wsParticleName.find( wsFindName.c_str(), 0 ) != wstring::npos )
			{
				if( NULL != pDialog)
				{
					CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
					if( NULL != pListBox )
					{
						pListBox->SelectItem( -1 );
						pListBox->SelectItem( iIndex );
						bIsFind = true;
					}
				}
				break;
			}
		}
	}

	//그래도 못찾았다면 팝업.
	if( false == bIsFind )
	{
		MessageBox(NULL, L"현재 파티클 파일에 없는 이름입니다.", L"Error!", MB_OK);
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_DEL_PARTICLE()
{
	//파티클 벡터
	vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;

	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_PARTICLE_LIST );
	if( NULL != pDialog )
	{
		CDXUTComboBox* pComboBox = pDialog->GetComboBox( CX2EffectToolMain::IDC_COMBO_PARTICLE_FILE );
		if( NULL != pComboBox )
		{
			// 현재 선택되어 있는 콤보박스의 인덱스 얻기.
			UINT uiFileIndex = reinterpret_cast<UINT>(pComboBox->GetSelectedData());

			CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_PARTICLE_LIST );
			if( NULL != pListBox )
			{
				UINT uiPartileIndex = pListBox->GetSelectedIndex();
				m_pParticleSystem->EraseParticleSequnce( uiPartileIndex, m_vecParticleFileName[uiFileIndex].c_str() );

				UpdateParticleListByIndex( uiFileIndex );
				SelectParticleListByIndex( uiPartileIndex );
			}
		}
	}
}
void CX2EffectToolMain::On_IDC_BUTTON_DEL_MESH()
{
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_MESH_LIST );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_MESH_LIST );
		if( NULL != pListBox )
		{
			UINT uiPartileIndex = pListBox->GetSelectedIndex();
			m_pXMeshPlayer->EraseXMeshTemplet( uiPartileIndex );

			UpdateMeshList();
			SelectMeshListByIndex( uiPartileIndex );
		}
	}
}

void CX2EffectToolMain::On_IDC_BUTTON_MESH_LIST_FIND( const WCHAR* strText )
{

	// 파티클 벡터 얻기.
	vector<CKTDGXMeshPlayer::XMeshTemplet*> vecMeshTemplet = m_pXMeshPlayer->GetVecTemplet();

	// 현재 선택된 파티클 인덱스
	int iSelectedMeshIndex = -1; 
	CEffectToolDialog* pDialog = GetDialog( CX2EffectToolMain::DLG_MESH_LIST );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( IDC_LIST_MESH_LIST );
		if( NULL != pListBox )
		{
			iSelectedMeshIndex = pListBox->GetSelectedIndex();
		}
	}

	if( true == vecMeshTemplet.empty() )
		return;

	// 대소문자 구분을 피하기 위해 모두 대문자로 변경.
	wstring wsFindName = strText;
	MakeUpperCase( wsFindName );

	bool bIsFind = false;
	// 현재 위치보다 아래 목록 중 검색.

	vector<CKTDGXMeshPlayer::XMeshTemplet*>::iterator it = vecMeshTemplet.begin();
	for( int iIndex = 0 ; it != vecMeshTemplet.end(); ++it, ++iIndex )
	{
		if( NULL == *it )
			break;

		if( iIndex <= iSelectedMeshIndex )
			continue;

		wstring wsMeshName = (*it)->templetName;
		MakeUpperCase( wsMeshName );
		if( wsMeshName.find( wsFindName.c_str(), 0 ) != wstring::npos )
		{
			if( NULL != pDialog)
			{
				CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_MESH_LIST );
				if( NULL != pListBox )
				{
					pListBox->SelectItem( -1 );
					pListBox->SelectItem( iIndex );
					bIsFind = true;
				}
			}
			break;
		}
	}

	// 아래 방향에서 못 찾았다면 한번 더 검색.
	if( false == bIsFind )
	{
		it = vecMeshTemplet.begin();
		for( int iIndex = 0 ; it != vecMeshTemplet.end(); ++it, ++iIndex )
		{
			if( NULL == *it )
				break;

			wstring wsMeshName = (*it)->templetName;
			MakeUpperCase( wsMeshName );
			if( wsMeshName.find( wsFindName.c_str(), 0 ) != wstring::npos )
			{
				if( NULL != pDialog)
				{
					CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_MESH_LIST );
					if( NULL != pListBox )
					{
						pListBox->SelectItem( -1 );
						pListBox->SelectItem( iIndex );
						bIsFind = true;
					}
				}
				break;
			}
		}
	}

	//그래도 못찾았다면 팝업.
	if( false == bIsFind )
	{
		MessageBox(NULL, L"현재 메쉬 파일에 없는 이름입니다.", L"Error!", MB_OK);
	}
}
void CX2EffectToolMain::ON_IDC_BUTTON_MESH_BASE_EDITOR()
{	
 	if( NULL != m_pMeshEditor )
	{
		CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet = NULL;
		CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_MESH_LIST );
		if( NULL != pDialog )
		{
			CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_MESH_LIST );
			if( NULL != pListBox )
			{
				DXUTListBoxItem* pItem = (static_cast<CDXUTListBox*>(pListBox))->GetSelectedItem();
				if( pItem != NULL )						
				{
					pMeshTemplet  = GetMeshByName( pItem->strText );
				}
			}
		}
		m_pMeshEditor->CreateDialogMeshEditor( pMeshTemplet );
	}
}
void CX2EffectToolMain::ON_IDC_BUTTON_LOAD_MESH_FILE( const WCHAR* strFileName_ )
{
	// 정보 로드.
	LoadMjoarMeshFile(strFileName_);

	// 메쉬 리스트 업데이트
	UpdateMeshList();
}
void CX2EffectToolMain::ON_IDC_BUTTON_SAVE_MESH_FILE()
{
	wstring wsMeshFilePath;
	GetMajorResourcePath(wsMeshFilePath);
	wsMeshFilePath += L"GameMajorXMeshPlayer.txt";

	m_pFileManager->SaveMajorMeshPlayer( wsMeshFilePath.c_str(), m_pXMeshPlayer );
}

CKTDGXMeshPlayer::XMeshTemplet* CX2EffectToolMain::CreateXMeshTemplet( const WCHAR* strMeshName_ )
{
	if( NULL == m_pXMeshPlayer )
		return NULL;

	return m_pXMeshPlayer->CreateXMeshTemplet(strMeshName_);
}
void CX2EffectToolMain::UpdateMeshList()
{
	// 메쉬 리스트 업데이트
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_MESH_LIST );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_MESH_LIST );
		if( NULL != pListBox )
		{
			pListBox->RemoveAllItems();
			if( NULL != m_pXMeshPlayer )
			{
				BOOST_FOREACH( CKTDGXMeshPlayer::XMeshTemplet* pData, m_pXMeshPlayer->GetVecTemplet() )
				{
					pListBox->AddItem( pData->templetName.c_str(), NULL );
				}
			}
		}
	}
}
void CX2EffectToolMain::UpdateEffectList()
{
	CX2EffectSet* pEffectset = g_pData->GetGameEffectSet();
	if( NULL == pEffectset )
		return;

	/// IDC LIST 등록
	CEffectToolDialog* pDialog	= GetDialog( CX2EffectToolMain::DLG_EFFECT_SET_LIST );
	if( NULL != pDialog )
	{
		CDXUTListBox* pListBox = pDialog->GetListBox( CX2EffectToolMain::IDC_LIST_EFFECT_SET );
		if( NULL != pListBox )
		{
			pListBox->RemoveAllItems();
			std::vector< CX2EffectSet::EffectSetData* >* vecEffectData = pEffectset->GetVecEffectSetTemplet();			
			BOOST_FOREACH( CX2EffectSet::EffectSetData* pData, *vecEffectData ) 
			{
				if( NULL != pData )
				{
					pListBox->AddItem( pData->m_wstrEffectSetName.c_str(), NULL );
				}
			}
		}
	}
}
void CX2EffectToolMain::PlayEffectSet()
{
	if( NULL != g_pData &&
		NULL != g_pData->GetGameEffectSet() &&
		NULL != GetSelectedEffectSetData() &&
		NULL != g_pX2Game )
	{
		g_pData->GetGameEffectSet()->StopEffectSetAll();
		g_pData->GetGameEffectSet()->PlayEffectSet( GetSelectedEffectSetData()->m_wstrEffectSetName, 
													static_cast<CX2GameUnit*>( g_pX2Game->GetMyUnit() ) );
		
	}

	m_pCharacterSkinAnim->Play(CKTDGXSkinAnim::XAP_ONE_WAIT);
}
void CX2EffectToolMain::ToggleGameMode()
{
	switch( m_iToolMode )
	{
	case MODE_GAMEPLAY:
		{
			m_iToolMode = m_iPrevToolMode;
 			D3DXVECTOR3 vLookAt = g_pKTDXApp->GetDGManager()->GetCamera().GetLookAt();
 			D3DXVECTOR3 vEye	= g_pKTDXApp->GetDGManager()->GetCamera().GetEye();
 			//if(m_pGame->m_bOrthoFreeCamera == false)
 			if(g_pKTDXApp->GetDGManager()->GetPerspective())
 				m_FPSCamera.SetViewParams( &vEye, &vLookAt );
			ResetDialogAtModeChange();

			if( NULL != m_pParticleSystem )
				m_pParticleSystem->DestroyAllInstance();

			if( NULL != m_pXMeshPlayer )
				m_pXMeshPlayer->DestroyAllInstance();

			ChangeToolMode( MODE_EFFECT_SET );
		} break;
	case MODE_EFFECT_SET:
		{
			m_iPrevToolMode = MODE_EFFECT_SET;
			m_iToolMode		= MODE_GAMEPLAY;
			m_pGame->GetMyUnit()->ClearRecieveSyncData();
			//현재 선택된 이펙트셋을 실행시킬 수 있도록 준비
			ReadyCurrentEffectSet();
		} break;		
	default:
		{
			ChangeToolMode( MODE_EFFECT_SET );
		} break;
	}		
}
void CX2EffectToolMain::GetMajorResourcePath( wstring& wsResourcePath ) const
{
	WCHAR charFilePath[ _MAX_DRIVE + _MAX_DIR + _MAX_FNAME + _MAX_EXT ];
	GetModuleFileName( NULL, charFilePath, ARRAY_SIZE(charFilePath));
	PathRemoveFileSpec( charFilePath );
	wsResourcePath = charFilePath;
	
	string strResourceFilePathAndName = "";
	ConvertWCHARToChar( strResourceFilePathAndName, wsResourcePath );
	strResourceFilePathAndName += "\\ResourcePath.txt";

	FILE *fp = fopen(strResourceFilePathAndName.c_str(), "r");

	char szSearchDir[MAX_PATH] = "";	
	if( NULL != fp )
	{
		fscanf ( fp, "%s", szSearchDir );
		ConvertUtf8ToWCHAR( wsResourcePath, szSearchDir );
		fclose ( fp );
	}
	else
	{
		MessageBox(NULL, L"ResourcePath Load Fail", L"Error!", MB_OK);
	}

	//저장할 파일 경로 설정
	wsResourcePath += L"\\dat\\Script\\Major\\";
}	
const UINT CX2EffectToolMain::GetParticleNumByFileName( const WCHAR* pFileName_ )const
{
	BOOST_FOREACH( wstring strData, m_vecParticleFileName )
	{
		if( 0 == strData.compare( pFileName_ ) )
		{
			vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
			m_pParticleSystem->GetVecTempletSequences( strData.c_str() , vecParticleTemplet );
			return vecParticleTemplet.size();
		}
	}

	return 0;
}

wstring CX2EffectToolMain::GetEffectSetDesc( const WCHAR* pEffectSetName_   )
{
	if( NULL == g_pData )
		return L"";

	CX2EffectSet* pEffectSet = g_pData->GetGameEffectSet();
	if( NULL == pEffectSet )
		return L"";

	// 이펙트셋 주석
	return pEffectSet->GetEffectSetDesc( pEffectSetName_ ).c_str();
}


void CX2EffectToolMain::SaveWhole( bool bHotKey_ ) // 전체 저장
{
	// 이펙트셋 저장
	On_IDC_BUTTON_SAVE_EFFECT_SET_WHOLE( bHotKey_ );				

	// 메쉬플레이어 저장
	ON_IDC_BUTTON_SAVE_MESH_FILE();


	// 모든 파티클 파일 저장
	vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
	BOOST_FOREACH( wstring wstrFileName, m_vecParticleFileName )
	{
		if( true == m_pParticleSystem->GetVecTempletSequences( wstrFileName.c_str(), vecParticleTemplet ) )
		{
			wstring wsParticleFilePath = GetParticleFilePath(wstrFileName.c_str());
			m_pFileManager->SaveMajorParticle(wsParticleFilePath.c_str() , vecParticleTemplet );
		}
	}	
}
void CX2EffectToolMain::CommitSVN()
{	
	
	WCHAR charFilePath[ _MAX_DRIVE + _MAX_DIR + _MAX_FNAME + _MAX_EXT ];
	GetModuleFileName( NULL, charFilePath, ARRAY_SIZE(charFilePath));
	PathRemoveFileSpec( charFilePath );
	wstring wsResourcePath = charFilePath;


	wsResourcePath += L"\\SvnCommit.bat";
// 	string strResourceFilePathAndName = "";
// 	ConvertWCHARToChar( strResourceFilePathAndName, wsResourcePath );
// 	strResourceFilePathAndName += "\\SvnCommit.bat";

	ShellExecute(NULL, L"open", wsResourcePath.c_str(), L"",L"", SW_SHOWNORMAL );
}

#ifdef CREATE_EFFECT_SET_BY_DAMAGE_EFFECT // 오현빈
/** @function : CreateEffectSetByDamageEffect
	@brief : 데미지 이펙트를 이펙트셋으로 만들기
*/
void CX2EffectToolMain::CreateEffectSetByDamageEffect()
{
	wstring wstrDamageEffectName = L"";
	// 선택된 메쉬플레이어가 데미지 이펙트인지 체크
	if( NULL != m_pSelectedEffectData &&
		CX2EffectSet::ET_DAMAGE_EFFECT == m_pSelectedEffectData->m_eEffectType)
	{
		
		// 유효한 데미지 이펙트인지 체크 후 해당 이름으로 더미 이펙트셋 생성
		if( true == IsValidDamageEffectByName( m_pSelectedEffectData->m_EffectName.c_str() ) )
		{
			wstrDamageEffectName = m_pSelectedEffectData->m_EffectName;
			On_IDC_BUTTON_EFFECT_SET_ADD( wstrDamageEffectName.c_str());
		}
		else
		{
			MessageBox( g_pKTDXApp->GetHWND(), 
				L"DamageEffect.lua에 없는 데미지 이펙트입니다.", L"Error", MB_OK );
		}


		if( NULL != m_pSelectedEffectSetData &&
			NULL != g_pX2Game && 
			NULL != g_pX2Game->GetDamageEffect() )
		{
			// 데미지 이펙트에서 생성한 파티클 얻기
			vector<CX2DamageEffect::ParticleData*> vecParticleList;
			g_pX2Game->GetDamageEffect()->GetParticleListByEffectName( wstrDamageEffectName.c_str(), vecParticleList );		

			// 파티클 항목 추가
			BOOST_FOREACH ( const CX2DamageEffect::ParticleData* pParticleData, vecParticleList )
			{
				CX2EffectSet::EffectData* pEffectData = new CX2EffectSet::EffectData;

				pEffectData->m_eEffectType = CX2EffectSet::ET_PARTICLE_GAME_MAJOR;
				pEffectData->m_EffectName = pParticleData->particleName;		/// 파티클 이름(wide-character)
				pEffectData->m_PositionBoneName = pParticleData->boneName;			/// 위치 형태가 뼈대일 경우, 뼈대 이름
				pEffectData->m_bApplyUnitRotation = pParticleData->bApplyRotate;		/// 대상의 회전 변환을 적용할 것인가
				pEffectData->m_fStartAnimTime = pParticleData->fEventTime;			/// 발생 시간
				pEffectData->m_vPositionOffset.x = pParticleData->fOffsetX;			/// 오프셋 위치 X
				pEffectData->m_vPositionOffset.y = pParticleData->fOffsetY;			/// 오프셋 위치 Y
				pEffectData->m_vPositionOffset.z = pParticleData->fOffsetZ;			/// 오프셋 위치 Z
				pEffectData->m_vScale = pParticleData->vScale;				/// 크기 비율
				pEffectData->m_bTrace = pParticleData->bTrace;				/// 따라 다닐 것인가
				pEffectData->m_bTraceMore = pParticleData->bTraceMore;

				m_pSelectedEffectSetData->m_vecpEffectData.push_back( pEffectData );
			}
			UpdateEffectSetMember( m_pSelectedEffectSetData->m_wstrEffectSetName.c_str() );

			// 메인 메쉬 추가
			AddMeshAtCurrentEffectset( GetMainMeshName_ByDamageEffectName( wstrDamageEffectName.c_str() ).c_str() );

			// 사용한 파티클 제거
			BOOST_FOREACH( CX2DamageEffect::ParticleData* pParticleData, vecParticleList)
			{
				SAFE_DELETE(pParticleData);
			}
		}
	}
	else
	{
		MessageBox( g_pKTDXApp->GetHWND(), 
			L"선택한 이펙트는 데미지이펙트가 아닙니다.", L"Error", MB_OK );
		return;
	}
}
#endif // CREATE_EFFECT_SET_BY_DAMAGE_EFFECT

void CX2EffectToolMain::OutputUsedResource()
{
	if( NULL != m_pSelectedEffectSetData )
	{
		set<wstring> setResource;

		BOOST_FOREACH( const CX2EffectSet::EffectData* pEffectData, m_pSelectedEffectSetData->m_vecpEffectData )
		{
			switch ( pEffectData->m_eEffectType )
			{
			case CX2EffectSet::ET_DAMAGE_EFFECT:
				{
					wstring wstrMeshName = GetMainMeshName_ByDamageEffectName( pEffectData->m_EffectName.c_str() );
					CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet = GetMeshByName( wstrMeshName.c_str() );
					if( NULL != pMeshTemplet )
					{
						setResource.insert( pMeshTemplet->meshName );
					}
					
				} break;
			case CX2EffectSet::ET_MESH_PLAYER_UI_MAJOR:
			case CX2EffectSet::ET_MESH_PLAYER_UI_MINOR:
			case CX2EffectSet::ET_MESH_PLAYER_GAME_MAJOR:
			case CX2EffectSet::ET_MESH_PLAYER_GAME_MINOR:
				{
					CKTDGXMeshPlayer::XMeshTemplet* pMeshTemplet = GetMeshByName( pEffectData->m_EffectName.c_str() );
					if( NULL != pMeshTemplet )
					{
						setResource.insert( pMeshTemplet->meshName );
					}
				} break;
			case CX2EffectSet::ET_PARTICLE_UI_MAJOR:
			case CX2EffectSet::ET_PARTICLE_UI_MINOR:
			case CX2EffectSet::ET_PARTICLE_GAME_MAJOR:
			case CX2EffectSet::ET_PARTICLE_GAME_MINOR:
				{
					//파티클 벡터
					vector<CKTDGParticleSystem::CParticleEventSequence*> vecParticleTemplet;
					bool bFind = false;
					BOOST_FOREACH ( const wstring& wstrFileName, m_vecParticleFileName )
					{
						vecParticleTemplet.clear();
						m_pParticleSystem->GetVecTempletSequences( wstrFileName.c_str(), vecParticleTemplet );

						if( false == vecParticleTemplet.empty() )
						{
							BOOST_FOREACH( CKTDGParticleSystem::CParticleEventSequence* pData, vecParticleTemplet ) 
							{
								if( 0 == wcscmp( pEffectData->m_EffectName.c_str(), pData->GetName() ) )
								{
									BOOST_TEST_FOREACH( CKTDGParticleSystem::CParticleEvent*, pEvent, pData->m_EventList )
									{
										switch( pEvent->GetEventType() )
										{
										case CKTDGParticleSystem::ET_TEXTURE:
											{
												CKTDGParticleSystem::CParticleEvent_Texture* pEventTexture = (CKTDGParticleSystem::CParticleEvent_Texture*) pEvent;
												setResource.insert( pEventTexture->GetTextureName() );
											}
										}
									}
									bFind = true;
								}

								if( true == bFind  )
									break;
							}
						}

						if( true == bFind  )
							break;
					}
				} break;
			}
		}

		KLOG("UsedResource.txt")  << m_pSelectedEffectSetData->m_wstrEffectSetName.c_str() << fileout;
		KLOG("UsedResource.txt")  << L"Start-------------------------------------------" << fileout;
		set<wstring>::iterator it = setResource.begin() ;
		for(; it != setResource.end(); ++it)
		{
			if( 'y' == (*it).back() ||
				'Y' == (*it).back() )
			{
				KLOG("UsedResource.txt")  << L"XMeshReadyInBackground(L\"" << (*it).c_str() << "\");" << fileout;
			}
			else if( 'x' == (*it).back() ||
				'X' == (*it).back() )
			{
				KLOG("UsedResource.txt")  << L"XSkinMeshReadyInBackground(L\"" << (*it).c_str() << "\");" << fileout;
			}
			else
			{
				KLOG("UsedResource.txt")  << L"TextureReadyInBackground(L\"" << (*it).c_str() << "\");" << fileout;
			}

		}
		KLOG("UsedResource.txt")  << L"--------------------------------------------End" << fileout;


		ShellExecute(NULL, NULL, L"notepad++.exe", L"UsedResource.txt", L"", SW_SHOWNORMAL );
	}
}