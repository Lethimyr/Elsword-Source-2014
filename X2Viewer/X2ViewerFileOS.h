#pragma once
/**	@file	: X2ViewerFileOS.h
	@desc	: 파일 입출력
	@author	: Anomymous

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#pragma region CX2ViewerFileOS
/** @class : CX2ViewerFileOS
	@brief : 파일 시스템
	@date  : 2013-09-17
*/
class CX2ViewerFileOS
{
	public:
		enum FILE_STATE
		{
			FS_NONE,	/// 아무 생태도 아님
			FS_XFILE,	/// XFile 상태
			FS_LUA,		/// Scene Fiile 상태(Lua)
		};

	public:
		CX2ViewerFileOS(void);
		~CX2ViewerFileOS(void);

		FILE_STATE		FileOpen( WCHAR* wsFilter );
		FILE_STATE		FileSave( WCHAR* wsFilter );

		std::wstring	GetPullFileName();
		std::wstring	GetTitleFileName();
		std::wstring	GetDirName();

	private:
		OPENFILENAME	m_OFN;		/// 파일명
		HWND			m_hWnd;		/// 윈도우 핸들

		std::wstring	m_wsPULLFileName,	/// 파일명 전체
						m_wsTITLEFileName,	/// 타이틀 파일명
						m_wsDirName;		/// 디렉토리명
};
#pragma endregion class