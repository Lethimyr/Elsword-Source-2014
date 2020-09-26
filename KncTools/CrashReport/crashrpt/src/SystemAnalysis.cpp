#include "StdAfx.h"
#include "SystemAnalysis.h"
#include <tchar.h>
#include <d3d8.h>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>




KSystemAnalysis* g_Dlg = NULL; ///< KSystemAnalysis 객체에 대한 전역 포인터
HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );
HRESULT PrintContainerAndChildren( WCHAR* wszParentName, IDxDiagContainer* pDxDiagContainer, FILE* fp );
void DxDiagFileOut( const char* szFileName );

INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName, VOID* pContext );

KSystemAnalysis::KSystemAnalysis()
{
    g_Dlg = this;
}
KSystemAnalysis::~KSystemAnalysis()
{
    g_Dlg = NULL;  // 전역 포인터 초기화 
}

bool KSystemAnalysis::StoreMySystemAnalysis()
{
    FILE* pFile;
    std::string fileName;
    std::string allSystemData;
    std::string filePath;
    allSystemData = "=============================================================================";
    allSystemData += "\r\n                  나의 컴퓨터 시스템 정보 \r\n";
    allSystemData +="=============================================================================";
    allSystemData += "\r\n\r\n컴퓨터 구성 \t         내 컴퓨터 사양";
    allSystemData += "\r\n\r\nCPU                      ";
    allSystemData += GetSystemCpuData().c_str();
    allSystemData += "\r\n\r\nOS                       ";
    allSystemData += GetSystemOsData().c_str();
    allSystemData += "\r\n\r\n남은 하드 공간           ";
    allSystemData += GetSystemFreeHardCapacity().c_str();
    allSystemData += "\r\n\r\nRAM                      ";
    allSystemData += GetSystemRamData().c_str();
    allSystemData += "\r\n\r\n그래픽카드               ";
    allSystemData += GetSystemGraphicCardData().c_str();
    allSystemData += "\r\n\r\n드라이버 버젼            ";
    allSystemData += GetSystemDriverVersion().c_str();
    allSystemData += "\r\n\r\nDirect X 버전            ";
    allSystemData += GetSystemDirectXVersion().c_str();
    allSystemData += "\r\n\r\n사운드 드라이버          ";
    allSystemData += GetSystemSoundData().c_str();
    allSystemData += "\r\n\r\n";
    allSystemData += "\0";
    
    fileName = "HardwareInfo.txt"; 
    pFile = fopen( fileName.c_str(), "w+" );
    fwrite( allSystemData.c_str(), sizeof( char ), allSystemData.size(), pFile );
    fclose(pFile);


	//fileName = "DxDiagReport.txt";
	//DxDiagFileOut( fileName.c_str() );
    
    return true;
}

std::string KSystemAnalysis::GetSystemCpuData()
{
    std::string strSystemCpuData;
    
    CPUInfo cpu;
    
    if (cpu.DoesCPUSupportCPUID())
    {
        strSystemCpuData = cpu.GetExtendedProcessorName();
    }
    else
    {
        strSystemCpuData = "ERROR! NOT FOUND CPU DATA";
    }
    
    return strSystemCpuData;
}

std::string KSystemAnalysis::GetSystemDirectXVersion()
{
    std::string strSystemDirectXVersion;
    HRESULT hr;
    TCHAR strResult[128];
    
    DWORD dwDirectXVersion = 0;
    TCHAR strDirectXVersion[10];
    
    hr = GetDXVersion( &dwDirectXVersion, strDirectXVersion, 10 );
    if( SUCCEEDED(hr) )
    {
        if( dwDirectXVersion > 0 )
            _sntprintf( strResult, 128, TEXT("DirectX %s"), strDirectXVersion );
        else
            _tcsncpy( strResult, TEXT(""), 128 );
        strResult[127] = 0;
    }
    else
    {
        _sntprintf( strResult, 128, TEXT(""), hr );
        strResult[127] = 0;
        
    }
    
    strSystemDirectXVersion = strResult;

    return strSystemDirectXVersion;
}

std::string KSystemAnalysis::GetSystemDriverVersion()
{
    std::string strDriverVersion;
    D3DADAPTER_IDENTIFIER8 identifier;
    
    IDirect3D8* pD3d;
    pD3d = Direct3DCreate8( D3D_SDK_VERSION ); 
    
    pD3d->GetAdapterIdentifier( D3DADAPTER_DEFAULT  ,D3DENUM_NO_WHQL_LEVEL , &identifier );
    
    strDriverVersion = identifier.Driver;
    
    pD3d->Release();

    return strDriverVersion;
}

std::string KSystemAnalysis::GetSystemFreeHardCapacity()
{
    std::string strSystemFreeHardCapacity;
    ULARGE_INTEGER AvailableToCaller; 
    ULARGE_INTEGER Disk, Free;
    std::string strMyFreeHardCapacity;
    char HardDiskLetters[4]  = { "c:\\" };
    int iFreeMB;
    char buffer[200];
    
    GetDiskFreeSpaceEx( HardDiskLetters, &AvailableToCaller, &Disk, &Free);

    iFreeMB = Free.QuadPart /1024/1024;
    _itoa( iFreeMB, buffer, 10);

    strSystemFreeHardCapacity = buffer;
    strSystemFreeHardCapacity += " MB";

    return strSystemFreeHardCapacity;
}

std::string KSystemAnalysis::GetSystemGraphicCardData()
{
    std::string strSystemGraphicCardData;

    D3DADAPTER_IDENTIFIER8 identifier;
    
    IDirect3D8* pD3d;
    pD3d = Direct3DCreate8( D3D_SDK_VERSION ); 
    
    
    pD3d->GetAdapterIdentifier( D3DADAPTER_DEFAULT  ,D3DENUM_NO_WHQL_LEVEL , &identifier );
    
    strSystemGraphicCardData = identifier.Description;
    
    pD3d->Release();

    return strSystemGraphicCardData;
}

std::string KSystemAnalysis::GetSystemOsData()
{
    OSVERSIONINFO osVersion;
    std::string strSystemOsData;
    
    osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    
    if ( GetVersionEx( &osVersion ) )
    {
        switch( osVersion.dwPlatformId )
        {
        case VER_PLATFORM_WIN32s:
            {
                strSystemOsData = "Windows 3.1 ";
                break;
            }
        case VER_PLATFORM_WIN32_WINDOWS:
            {
                switch( osVersion.dwMinorVersion )
                {
                case 0:
                    strSystemOsData = "windows 95 ";
                    break;
                case 10:
                    strSystemOsData = "windows 98 ";
                    break;
                case 90:
                    strSystemOsData = "Windows Me ";
                default:
                    strSystemOsData = "Windows ?? ";
                    break;
                }
                break;
            }
        case VER_PLATFORM_WIN32_NT:
            {
                switch( osVersion.dwMajorVersion )
                {
                case 3:
                    strSystemOsData = "Windows  NT 3.51 ";
                    break;
                case 4:
                    strSystemOsData = "Windows NT 4.0 ";
                    break;
                case 5:
                    switch( osVersion.dwMinorVersion)
                    {
                    case 0:
                        strSystemOsData = "Windows 2000 ";
                        break;
                    case 1:
                        strSystemOsData = "Windows XP ";
                        break;
                    case 2:
                        strSystemOsData = "Windows Server 2003 family ";
                        break;
                    default:
                        strSystemOsData = "Windows NT 계열 ";
                        break;
                    }
                    break;
                    default:
                        strSystemOsData = "Windows NT 계열 ";
                        break;
                }
                break;
            }
        default:
            {
                strSystemOsData = "OS를 찾울수 없습니다.";
                break;
            }
        } // end switch
        
        strSystemOsData += osVersion.szCSDVersion;
        strSystemOsData += "(Build ";
        
        std::string buildNumber;
        char buffer[100];
        _itoa( osVersion.dwBuildNumber, buffer, 10 );
        buildNumber = buffer;
        
        strSystemOsData += buildNumber;
        strSystemOsData += ")";
    } // end if 
    else
        strSystemOsData = "OS정보를 읽어올수 없습니다.";
    
   return strSystemOsData;
}

std::string KSystemAnalysis::GetSystemRamData()
{
    char buffer[200];
    std::string strSystemRamData;
    MEMORYSTATUS memoryStatus;
    memoryStatus.dwLength = sizeof (MEMORYSTATUS);
    GlobalMemoryStatus(&memoryStatus);
    
    double tempTotalMemory = memoryStatus.dwTotalPhys;
    
    tempTotalMemory /= 1024; //byte를 KB로 
    tempTotalMemory /= 1024; //KB를 MB로
    
    int totalMemory = (int )( tempTotalMemory + 0.999 ); // 소숫점이하는  올림으로 

    _itoa( totalMemory, buffer, 10 );

    strSystemRamData = buffer;
    strSystemRamData += " MB";
        
    return strSystemRamData;
}

std::string KSystemAnalysis::GetSystemSoundData()
{
    HRESULT hr;
    
    HWND hSoundDeviceCombo = NULL;
    
    if( FAILED( hr = DirectSoundEnumerate( (LPDSENUMCALLBACK)DSoundEnumCallback, //api함수 사용한것 
        (VOID*)hSoundDeviceCombo ) ) )
    {
        m_strSystemSoundData = "사운드를 찾을수 없습니다. ";
    }

    return m_strSystemSoundData;
}

/// 사운드카드의 정보를 호출하는 함수이다. 이함수가 1번불리면 "주사운드카드"글자가 strDesc에 저장 
/// 2번재 불릴때는 strDesc에 내 컴퓨터의 사운드 카드 정보가 저장된다. 
/// @param pGUID  뭐하는것인지 잘 모른다. 
/// @param strDesc 사운드카드의 정보를 가진다.
/// @param strDrvName 뭐하는것인지 잘 모른다. 
/// @param strDrvname 뭐하는 것인지 잘 모른다.
/// @return void
INT_PTR CALLBACK DSoundEnumCallback( GUID* pGUID, LPSTR strDesc, LPSTR strDrvName, VOID* pContext )
{
    // Set aside static storage space for 20 audio drivers
    static int nCounter =0;
    static GUID  AudioDriverGUIDs[20];
    static DWORD dwAudioDriverIndex = 0;
    
    GUID* pTemp  = NULL;
    
    if( pGUID )
    {
        if( dwAudioDriverIndex >= 20 )
            return TRUE;
        
        pTemp = &AudioDriverGUIDs[dwAudioDriverIndex++];
        memcpy( pTemp, pGUID, sizeof(GUID) );
    }
    
    HWND hSoundDeviceCombo = (HWND)pContext;
    
    if ( nCounter == 1)
    {
        g_Dlg->m_strSystemSoundData = strDesc;
    }
    
    nCounter++;
    return TRUE;
}

