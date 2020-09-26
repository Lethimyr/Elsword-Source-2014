#include "StdAfx.h"
#include ".\x2chatwindow.h"

CX2ChatWindow::CX2ChatWindow(ChatWindowData& chatWindowData)
{
	m_bShow = true;
	m_ChatWindowData = chatWindowData;
	
	m_pFont = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_13_SEMIBOLD );
	//m_pFontForUniBuffer = g_pKTDXApp->GetDGManager()->GetDialogManager()->GetUKFont( XUF_DODUM_13_SEMIBOLD );
	m_ChatWindowData.fontSize.y = (float)m_pFont->GetHeight();
	
	//m_pFont = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewUKFont(L"±¼¸²", (int)m_ChatWindowData.fontSize.y, 1, true, FW_BOLD );
	//m_pFontForUniBuffer = g_pKTDXApp->GetDGManager()->GetFontManager()->CreateNewFont( L"±¼¸²", (int)m_ChatWindowData.fontSize.y );
	
	//m_UniBuffer.SetFontNode( m_pFontForUniBuffer );
}

CX2ChatWindow::~CX2ChatWindow(void)
{
	for ( UINT i = 0; i < (UINT)m_ChatDataList.size(); i++ )
	{
		ChatData* pChatData = m_ChatDataList[i];
		SAFE_DELETE( pChatData );
	}
	m_ChatDataList.clear();

	//g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyUKFont( m_pFont );
	//g_pKTDXApp->GetDGManager()->GetFontManager()->DestroyFont( m_pFontForUniBuffer );
}


HRESULT CX2ChatWindow::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();
	for ( UINT i = 0; i < (UINT)m_ChatDataList.size(); i++ )
	{
		ChatData* pChatData = m_ChatDataList[i];
		pChatData->fTime += fElapsedTime;

		if ( pChatData->bVanishing == true && pChatData->fTime >= m_ChatWindowData.fVanishTime )
		{			
			SAFE_DELETE( pChatData );
			m_ChatDataList.erase( m_ChatDataList.begin() + i );
			i--;
			continue;
		}

		if ( pChatData->bVanishing == false && pChatData->fTime >= m_ChatWindowData.fChatTime )
		{
			pChatData->bVanishing = true;
			pChatData->fTime = 0.0f;
		}
	}

	return S_OK;
}

HRESULT CX2ChatWindow::OnFrameRender()
{
	KTDXPROFILE();

	if( false == m_bShow )
		return S_OK;


	D3DXVECTOR2 pos = m_ChatWindowData.pos;

	for ( UINT i = 0; i < (UINT)m_ChatDataList.size(); i++ )
	{
		ChatData* pChatData = m_ChatDataList[i];

		D3DXCOLOR fontColor						= D3DXCOLOR(1,1,1,1);
		D3DXCOLOR fontOutLintColor				= D3DXCOLOR(1,1,1,1);
		CKTDGFontManager::FONT_STYLE fontStyle	= CKTDGFontManager::FS_NONE;

		//wstring ChatDataMessage;

		switch( pChatData->chatType )
		{
		case CT_CHAT:
			{
				fontColor			= D3DXCOLOR(1,1,1,1);
				fontOutLintColor	= D3DXCOLOR(0,0,0,1);

				fontStyle			= CKTDGFontManager::FS_SHELL;
			}
			break;
		case CT_KILL:
			{
				fontColor			= D3DXCOLOR(0.8f,0,0,1);
				fontOutLintColor	= D3DXCOLOR(0,0,0,1);

				fontStyle			= CKTDGFontManager::FS_SHELL;
			}
			break;

		case CT_NORMAL:
			{
				fontColor			= D3DXCOLOR(0,1,0,1);
				fontOutLintColor	= D3DXCOLOR(0,0,0,1);

				fontStyle			= CKTDGFontManager::FS_SHELL;
			}
			break;

		case CT_PICK_ITEM:
			{
				fontColor			= D3DXCOLOR(0.9f,0.9f,0.5f,1);
				fontOutLintColor	= D3DXCOLOR(0,0,0,1);

				fontStyle			= CKTDGFontManager::FS_SHELL;
			}
			break;

		case CT_GET_EXP:
			{
				fontColor			= D3DXCOLOR(0.3f,0.7f,0.8f,1);
				fontOutLintColor	= D3DXCOLOR(0,0,0,1);

				fontStyle			= CKTDGFontManager::FS_SHELL;
			}
			break;

		case CT_LEVEL_UP:
			{
				fontColor			= D3DXCOLOR(1,1,0,1);
				fontOutLintColor	= D3DXCOLOR(0,0,0,1);

				fontStyle			= CKTDGFontManager::FS_SHELL;
			}
			break;

		case CT_MISC:
			{
				fontColor			= pChatData->textColor; 
				fontOutLintColor	= D3DXCOLOR(0,0,0,1);

				fontStyle			= CKTDGFontManager::FS_SHELL;
			} break;
		}



		//if ( pChatData->senderName.compare(L"") != 0 )
		//{
		//	ChatDataMessage = pChatData->senderName + L" : " + pChatData->message;
		//}
		//else
		//{
		//	ChatDataMessage = pChatData->message;
		//}

		if ( pChatData->bVanishing == true )
		{
			fontColor.a				= ( m_ChatWindowData.fVanishTime - pChatData->fTime ) / m_ChatWindowData.fVanishTime;
			fontOutLintColor.a		= ( m_ChatWindowData.fVanishTime - pChatData->fTime ) / m_ChatWindowData.fVanishTime;
		}

		//int nowLineSize = 0;
		//int iRaw = 0;
		//wstring wstrNextLine = L"\n";

		//for ( int i = 0; i < (int)ChatDataMessage.size(); i++ )
		//{
		//	int iUniCharSize = 0;

		//	WCHAR wChar = ChatDataMessage[i];
		//	m_UniBuffer.InsertChar(0, wChar);
		//	m_UniBuffer.CPtoX(m_UniBuffer.GetTextSize(), FALSE, &iUniCharSize);		//	'a' - 7, '°¡' - 14
		//	m_UniBuffer.Clear();
		//	
		//	nowLineSize += iUniCharSize;

		//	if ( nowLineSize >= m_ChatWindowData.size.x )
		//	{
		//		ChatDataMessage.insert( ChatDataMessage.begin()+i, wstrNextLine.begin(), wstrNextLine.end() );
		//		nowLineSize = 0;
		//		iRaw++;
		//	}
		//}

		for ( int i = 1; i < pChatData->nRow ; i++ )
		{
			pos.y -= m_ChatWindowData.fontSize.y;
		}

		//m_pFont->Draw2DText( ChatDataMessage.c_str(), (int)pos.x, (int)pos.y, fontColor, fontOutLintColor, DT_LEFT, fontStyle );
		m_pFont->OutTextMultiline( (int)pos.x, (int)pos.y, pChatData->message.c_str(), fontColor, 
									CKTDGFontManager::FS_SHELL, fontOutLintColor, 0.9f );

		pos.y -= m_ChatWindowData.fontSize.y;
		
	}

	return S_OK;
}

void CX2ChatWindow::PushChatData( ChatData* pChatData )
{
	if ( (int)m_ChatDataList.size() >= m_ChatWindowData.maxChatDataNum )
	{
		ChatData* pChatDataForDelete = m_ChatDataList.back();
		m_ChatDataList.pop_back();
		SAFE_DELETE( pChatDataForDelete );

		//ChatData* pChatDataForDelete = m_ChatDataList[ m_ChatDataList.size() - 1 ];
		//SAFE_DELETE( pChatDataForDelete );
		//m_ChatDataList.erase( m_ChatDataList.end() - 1 );
	}

	wstring ChatDataMessage = L"";

	if ( pChatData->senderName.compare(L"") != 0 )
	{
		ChatDataMessage = pChatData->senderName + L" : " + pChatData->message;
	}
	else
	{
		ChatDataMessage = pChatData->message;
	}

#ifdef CLIENT_GLOBAL_LINEBREAK

int iRaw = CWordLineHandler::LineBreakInX2Main(ChatDataMessage, m_pFont, (int)m_ChatWindowData.size.x, L"", true, false);

#else //#ifdef CLIENT_GLOBAL_LINEBREAK
	// line break;
	int nowLineSize = 0;
	int iRaw = 1;
	wstring wstrNextLine = L"\n";
	
	int iStringLength = (int) ChatDataMessage.length();

	for ( int i = 0; i < iStringLength; i++ )
	{
		int iUniCharSize = 0;

		WCHAR wChar = ChatDataMessage[i];
		//m_UniBuffer.InsertChar(0, wChar);
		//m_UniBuffer.CPtoX(m_UniBuffer.GetTextSize(), FALSE, &iUniCharSize);		//	'a' - 7, '°¡' - 14
		//m_UniBuffer.Clear();

		iUniCharSize = m_pFont->GetWidth( wChar );
		nowLineSize += iUniCharSize;

		if ( nowLineSize >= m_ChatWindowData.size.x )
		{
			ChatDataMessage.insert( ChatDataMessage.begin()+i, wstrNextLine.begin(), wstrNextLine.end() );
			iStringLength += 1;
			i += 1;
			nowLineSize = 0;
			iRaw++;
		}
	}
#endif //CLIENT_GLOBAL_LINEBREAK


	pChatData->message = ChatDataMessage;
	pChatData->nRow = iRaw;
	m_ChatDataList.push_front( pChatData );
}