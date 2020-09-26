#ifndef __PATCHER_H__
#define __PATCHER_H__




//{{ dmlee 2008.05.26 옵션, 실행 버튼 추가
//#define DMLEE_PATCHER_TEST
//}} dmlee 2008.05.26 옵션, 실행 버튼 추가






#define LOAD_CUR_FILE_BAR_X			72
#define LOAD_CUR_FILE_BAR_Y			285
#define LOAD_CUR_FILE_BAR_WIDTH		373 - 5
#define LOAD_CUR_FILE_BAR_HEIGHT	473 - 462

#define LOAD_CUR_FILE_PER_X			449
#define LOAD_CUR_FILE_PER_Y			285
#define LOAD_CUR_FILE_PER_WIDTH		60
#define LOAD_CUR_FILE_PER_HEIGHT	30

#define LOAD_ALL_FILE_BAR_X			72
#define LOAD_ALL_FILE_BAR_Y			305
#define LOAD_ALL_FILE_BAR_WIDTH		373 - 5
#define LOAD_ALL_FILE_BAR_HEIGHT	473 - 462

#define LOAD_ALL_FILE_PER_X			449
#define LOAD_ALL_FILE_PER_Y			305
#define LOAD_ALL_FILE_PER_WIDTH		60
#define LOAD_ALL_FILE_PER_HEIGHT	30



#include "HttpWrapper.h"
#include "EnumFile.h"
#include "Registry.h"
#include "KMsgBox.h"
#include "KAssert.h"





/// 패치 프로그램의 주요 코드를 담고 있는 클래스.
class KPatcher
{
public:
    enum { IDC_PATCH_INFO_DLG = 10000, };
public:
    /// Constructor with argument.
    explicit KPatcher( HWND hWnd );
    /// Default destructor.
    ~KPatcher( void );
private:
    /// Default constructor - disabled.
    KPatcher( void )
    { ASSERT( false && _T("Do not use default constructor!") ); }
    /// Copy constructor - disabled.
    KPatcher( const KPatcher& )
    { ASSERT( false && _T("Do not use copy constructor!") ); }
    /// Assignment operator - disabled.
    const KPatcher& operator=( const KPatcher& )
    { ASSERT( false && _T("Do not use assignment operator!") ); return *this; }
public:
    /// Get window handle to main window.
    /// @param void
    /// @return handle to main window
    inline HWND GetWndHandle( void ) const
    { return m_hWnd; }
    /// Set rate of file download.
    /// @param dwRateDown the rate of file download
    /// @return void
    inline void SetRateDownload( DWORD dwRateDown )
    { m_dwRateDown = dwRateDown; }
    /// Get rate of file download.
    /// @param void
    /// @return rate of download
    inline DWORD GetRateDownload( void ) const
    { return m_dwRateDown; }
    /// Get current download file description.
    /// @param void
    /// @return current download file description

    inline const char* GetCurDownloadFileDesc( void ) const
    { return m_strCurFileDesc.c_str(); }

	void SetCurFileDownloadPer( DWORD dwDonwloadPer ) 
	{ 
		char buff[256] = {0};
		sprintf( buff, "%d%%", (int)dwDonwloadPer );
		m_strCurFileDownloadPer = buff;
	}
	inline const char* GetCurFileDownloadPer( void ) const
	{ return m_strCurFileDownloadPer.c_str(); }

	void AddCurDownAllFileSize( DWORD dwSize );
	
	inline DWORD GetCurAllFileDownloadRate() 
	{ 
		if( 0 == m_dwDownAllFileSize )
			return 100;
		else
			return (DWORD)(m_dwCurDownAllFileSize * 100.0f / m_dwDownAllFileSize); 
	}
	inline const char* GetCurDownAllFilePer() 
	{ 
		if( 0 == m_dwDownAllFileSize )
		{
			m_strCurAllFileDownloadPer = "100%";
		}
		else
		{
			int iRate = (int)(m_dwCurDownAllFileSize * 100.0f / m_dwDownAllFileSize);
			if( iRate > 100 )
				iRate = 100;
			char buff[256] = {0};
			sprintf( buff, "%d%%", iRate );
			m_strCurAllFileDownloadPer = buff;
		}
		return m_strCurAllFileDownloadPer.c_str();
	}

    /// Get current download file size
    /// @param void
    /// @return current download file size
    inline DWORD GetCurDownloadFileSize( void ) const
    { return m_dwByteFile; }
public:
    /// Check program to patch.
    bool CheckProgram( void );
    /// Launch game.
    bool Launch( void ) const;
    /// Program exit.
    void Exit( void ) const;

	std::string GetProgramPath() const { return m_strProgramPath; }

private:
    /// Create directory.
    bool CreateDirectory( const char* szPath_ ) const;
    /// Get file name except folder path.
    void ExceptFolder( char* szDest, const char* szSrc ) const;
    /// Read patch path file.
    bool ReadPatchPathFile( char* szSrcPath ) const;
    /// Read patch path file.
    bool ReadPatchPathFile( std::string& strSrcPath ) const;
    /// Delete file absolutely.
    void DeleteFileAbsolutely( const char* szFileName ) const;
    /// Update version file.
    bool UpdateVersionFile( void ) const;
    /// Open or close download window.
    void OpenDownloadWindow( bool bOpen ) const;
    /// Append backslash character to string end if it is not exist.
    void AppendBackSlash( std::string& str ) const;
    /// Append slash character to string end if it is not exist.
    void AppendSlash( std::string& str ) const;
private:
    DWORD       m_dwByteFile;       ///< Size of file.
    DWORD       m_dwRateDown;       ///< Rate of download.
    HWND        m_hWnd;             ///< Main window handle.
    bool        m_bIsPatched;       ///< Is Game Client Patched?
    std::string m_strProgramPath;   ///< Game client program path.	
    std::string m_strCurFileDesc;   ///< Current download file description.

	std::string m_strCurFileDownloadPer;
	std::string m_strCurAllFileDownloadPer;	//지금 까지 받은 전체 총파일 퍼센트
	DWORD		m_dwCurDownAllFileSize;		//지금 까지 받은 전체 총파일량
	DWORD		m_dwDownAllFileSize;		//전체 총 받아야할 파일량

};

extern KPatcher* g_pPatcher;        ///< Patcher instance.

#endif // __PATCHER_H__