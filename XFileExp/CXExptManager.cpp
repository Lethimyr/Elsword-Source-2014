#include "XFileExp.h"
#include "CXExpManager.h"




BOOL CALLBACK XFileExpOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) 
{
    static XFileExp *imp = NULL;

    switch(message) 
    {
        case WM_INITDIALOG:
            imp = (XFileExp *)lParam;
            CenterWindow(hWnd,GetParent(hWnd));
            return TRUE;

        case WM_CLOSE:
            EndDialog(hWnd, 0);
            return TRUE;
    }
    return FALSE;
}


HRESULT XExpManager::XExport(const TCHAR *aName, ExpInterface *aExpInterface,Interface *aInterface, BOOL suppressPrompts, DWORD options)
{
    HWND pDialog = GetActiveWindow();
    

    // 
    if(!suppressPrompts)
    {        
        DialogBoxParam(hInstance, 
            MAKEINTRESOURCE(IDD_PANEL), 
            GetActiveWindow(), 
            XFileExpOptionsDlgProc, (LPARAM)pDialog);

        if(!g_bExport)
        {
            if(g_exportInfo)
            {                
                delete g_exportInfo;
                g_exportInfo = NULL;
            }

            return S_OK;
        }
    }

    // export
    g_exportInfo->DoExport(i, name);


    // release
    delete g_exportInfo;

    return S_OK;
}

HRESULT XExpManager::DoExport(Interface *aInterface, const TCHAR *aFileName)
{
    HRESULT hr;
    int numChildren;    

    if(aFileName != NULL)
    {
        m_pFile = fopen(aFileName, "w");
        if(m_pFile == NULL)
        {                      
            MessageBox(NULL, "디버깅 파일 생성 실패!", "Error!", MB_OK);
            return E_FAIL;
        }
    }

    m_pInterface = aInterface; 

    numChildren = m_pInterface->GetRootNode()->NumberOfChildren();
       

    ExportRootTransform(m_pInterface->GetRootNode());

    for (int idx=0; idx<numChildren; ++idx) 
    {
        if (m_pInterface->GetCancel())
            break;

        hr = nodeEnum(m_pInterface->GetRootNode()->GetChildNode(idx));
        if(FAILED(hr))
        {
            return E_FAIL;
        }
    }

    return S_OK;
}
