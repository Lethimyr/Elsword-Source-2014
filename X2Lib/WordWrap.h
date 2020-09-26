#pragma once


/*
* LineBreak 의 cutting 지점은 공백 문자 다음, 또는 지정한 특수문자 다음 지점을 기준으로 잡는다.
*
*/

class CWordLineHandler
{
public:
	CWordLineHandler(void);
	~CWordLineHandler(void);

	//static wstring GetStrByLineBreak( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, const bool bCutRedundantEmptyLine = true );

	//--------//
	// 묶음 1 //
	//--------//
	//static int LineBreakInChatBox( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor );
	//static int LineBreakInDungeonRoom( wstring& wstrText, CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor );
	// 일관성 위해 첫번째 인자와 2번째 인자 순서 바꿈 //
	static int LineBreakInX2Main( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, const bool bCutRedundantEmptyLine = true );

	
	//--------//
	// 묶음 2 //
	//--------//
	//static int LineBreakInChatWindow(wstring ChatDataMessage, CKTDGFontManager::CUKFont* pFont, int limitX );
	//static int LineBreakInSlideShot( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
	//static int LineBreakInEventScene( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
	//static int LineBreakInTrainingGameUI( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );

#ifdef LINE_BREAK_IN_SPEECH
	//static int LineBreakInDungeonSpeech( CKTDGFontManager::CUKFont* pFont, int iWidth, wstring& wstrSpeech, const WCHAR* pNextLineString, wstring wstrColor = L"" );
#endif



	// 일단 정리 안되서 코드 복사해옴
	// 기본 LineBreak 합쳐야 할듯
	//3
	static void LineBreakInTalkBoxManagerImp( CKTDGFontManager::CUKFont* pFont,  D3DXVECTOR2 m_Size, int& nRow, int& iColumnSize, wstring& wstrSpeech, bool bTrade 
#ifdef CLIENT_GLOBAL_EU_WORDWRAP
												, D3DXVECTOR2 m_ResSize
#endif CLIENT_GLOBAL_EU_WORDWRAP
		);

	
	//4
//==============================================//
	// 함수 이름 Lien 오타 있어서 Line으로 수정함
	static std::wstring	GetStrByLineBreakInX2Main( const WCHAR* pOrgStr, int width, int fontIndex );
	static std::wstring	BasicLineBreak( const WCHAR* pOrgStr, int width, int fontIndex );
	
	//5
#ifdef FIX_TOOLTIP
	static std::wstring	GetStrByLineBreakColorInX2Main( const WCHAR* pOrgStr, int width, int fontIndex );
#endif FIX_TOOLTIP
	//6
#if defined(ELLIPSE_GLOBAL) || defined(ELLIPSE_CN) //...기능 
	static std::wstring GetStrByLineBreakInX2MainWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse);
#ifdef ELLIPSE_FIX
	static std::wstring CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse, const WCHAR* pEllipseStr = L"..."); //단어단위로 짜르지 않고 ... 기능 사용하고 싶을때. ex 아이템 분해창
	static std::wstring CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, const WCHAR* pEllipseStr = L"...");
#else // ELLIPSE_FIX
	static std::wstring CutStringWithEllipse( const WCHAR* pOrgStr, int width, int fontIndex, const int nLineNum, bool& bEllipse); //단어단위로 짜르지 않고 ... 기능 사용하고 싶을때. ex 아이템 분해창
#endif // ELLIPSE_FIX
#endif //defined(ELLIPSE_GLOBAL) || defined(ELLIPSE_CN)

//=============================================//
	//7
	static void ProcessQuestDescInUIQuestNew( const wstring& wstrDesc, const int iLineWidth, CKTDGFontManager::CUKFont* pFont,D3DXVECTOR2 m_vQuestDescSize, vector<wstring>&  m_vecQuestDesc);

	//static void UpdateQuestDescInX2TFieldNpcShop( const wstring& wstrDesc,CKTDGFontManager::CUKFont* pFont, D3DXVECTOR2 m_vQuestDescSize, vector<wstring>&  m_vecQuestDesc);


	//8
	static void LineBreakInX2Community(wstring tempChatMsg, CKTDGFontManager::CUKFont* pFont, wstring chatColor, vector<wstring>& m_vecTalk, CKTDGUIListBox* pListBoxMessenger, bool  bApplyColor, bool bIsSameChatSession);
	
	//9
	static int LineBreakInX2MainMsgBox(wstring& tempText, CKTDGFontManager::CUKFont* pFont, const int constTextMaxLen);

	//10
	static bool MegaLineBreakAdd( wstring wstrText, CKTDGFontManager::CUKFont* pFont, int iTextWidth, CKTDGUIListBox* pListBox , int iMegaID, wstring wstrSenderName);

//=============================================//
	
	static bool IsSpaceLatter( WCHAR wszCharBuf );
	static bool IsSplitLatter( WCHAR wszCharBuf );

//=============================================//
	
	
};
