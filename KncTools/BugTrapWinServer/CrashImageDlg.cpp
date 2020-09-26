
#include "stdafx.h"
#include "BugTrapWinServer.h"
#include "CrashImageDlg.h"

#define HIMETRIC_INCH        2540

// CCrashImageDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCrashImageDlg, CDialog)
CCrashImageDlg::CCrashImageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrashImageDlg::IDD, pParent)
{
    mp_image_object = NULL;
    m_width_size = m_height_size = 0;

}

CCrashImageDlg::~CCrashImageDlg()
{
    if(mp_image_object != NULL) mp_image_object->Release();
}

void CCrashImageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PIC, m_pic);

    
}


BEGIN_MESSAGE_MAP(CCrashImageDlg, CDialog)
    ON_WM_PAINT()

END_MESSAGE_MAP()

void CCrashImageDlg::OnPaint()
{
    Draw();
}


char CCrashImageDlg::LoadImage(const char *parm_path)
{
    // 이전에 그림을 읽어서 객체가 만들어져 있다면 해당 객체를 제거한다.
    if(mp_image_object != NULL) mp_image_object->Release();

    char ok_flag = 0;
    // 지정한 그림파일을 CreateFile 함수를 사용하여 연다.
    HANDLE h_file = CreateFile(parm_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if(INVALID_HANDLE_VALUE != h_file){
        // 파일의 크기를 얻는다.
        int file_size = GetFileSize(h_file, NULL);
        if(-1 != file_size){
            // 파일의 크기만큼 전역 메모리를 할당한다.
            HGLOBAL h_global = GlobalAlloc(GMEM_MOVEABLE, file_size);
            if(NULL != h_global){
                // 핸들 형태로 할당된 전역 메모리를 사용하기 위해서 실제 포인터를 얻는다.
                void *p_data = GlobalLock(h_global);
                if(NULL != p_data){
                    unsigned long read_size = 0;
                    // 그림 파일 정보를 읽어 들인다.
                    ReadFile(h_file, p_data, file_size, &read_size, NULL);
                    GlobalUnlock(h_global);

                    LPSTREAM p_stream = NULL;
                    // 읽어들인 파일정보를 이용하여 스트림 객체를 생성한다.
                    HRESULT h_result = CreateStreamOnHGlobal(h_global, TRUE, &p_stream);
                    if(SUCCEEDED(h_result) && p_stream){
                        // 스트림 객체의 내용을 기준으로 그림(Picture) 객체를 생성한다.
                        // 생성된 그림 객체의 포인터는 mp_image_object에 저장된다.
                        ::OleLoadPicture(p_stream, file_size, FALSE,
                            IID_IPicture, (LPVOID *)&mp_image_object);
                        // 스트림 객체를 제거한다.
                        p_stream->Release();
                        if(SUCCEEDED(h_result) && mp_image_object){
                            // 그림객체가 성공적으로 생성되었다면 그림 크기정보를 얻는다
                            mp_image_object->get_Width(&m_width_size);
                            mp_image_object->get_Height(&m_height_size);
                        }
                        ok_flag = 1;
                    }
                }
            }
            GlobalFree(h_global);
        }
        CloseHandle(h_file);
    }
    return ok_flag;


}

void CCrashImageDlg::Draw()
{
    HRESULT hr;
    //CClientDC dc(&m_pic);
    CDC *dc = m_pic.GetDC();
    

    // 그림을 출력할 시작 좌표가 (100, 120)이고 폭과 높이를 이미지의 크기에 따라 출력한다.
    int x_size = MulDiv(m_width_size, dc->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
    int y_size = MulDiv(m_height_size, dc->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);

    
    hr = mp_image_object->Render(dc->m_hDC, 10, 10, x_size, y_size, 0,
        m_height_size, m_width_size, -m_height_size, NULL);

    m_pic.Invalidate();
    // 사용한 그림 객체를 정리하기 위해서 해당 클래스의 객체 파괴자나 WM_DESTROY에 아래와 같은
    // 코드를 추가하면 된다.
    //if(mp_image_object != NULL) mp_image_object->Release();

}

// CCrashImageDlg 메시지 처리기입니다.
