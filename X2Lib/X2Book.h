#pragma once

class CX2Book
{
public:

	// 생성하면 바로 책이 뜨고, Object 자체를 Delete 하면 깔끔하게 사라지게.
	// EXIT로 닫았을 때 어떻게 처리해야 할까..
	// 확장하기 쉽게.. -> 이후 책시스템 들어가면 대부분의 코드가 재사용 가능하도록 해보자
	// 책 data를 최대한 스크립트로 뽑아두자 : 이후 뭔가 추가할때 코드는 최대한 수정 적게 할 수 있도록.

	enum BOOK_UI_CUSTOM_MESSAGE
	{
		BUCM_EXIT			= 28000,
		BUCM_PREV_PAGE,
		BUCM_NEXT_PAGE,
		BUCM_GOTO_PAGE,       
	};

	struct PageData
	{
		wstring m_PageLuaFileName;
		//wstring m_string;
		//wstring m_CoverLuaFileName;
	};
	typedef map<int, PageData>::iterator PageDataIterator;

	CX2Book( const char* bookTableName );
	CX2Book();
	virtual ~CX2Book(void);
	
	bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool OnMouseWheel( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	bool GetShow() { return m_bShow; }
	void SetShow(bool bOpen);

	bool SetBookTable( const char* bookTableName );

	void NextPage();
	void PrevPage();
	void GotoPage(int iTargetPage);

private:

	bool			m_bShow;
	CKTDGUIDialogType	m_pDLGBook;
	CKTDGUIDialogType	m_pDLGBookCoverDefault;

	int				m_iCurrentPage;
	map<int, PageData> m_mapPageData;

};