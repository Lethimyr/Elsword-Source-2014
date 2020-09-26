#pragma once

class CX2ChatWindow
{
	public:

		struct ChatWindowData
		{
			D3DXVECTOR2 pos;
			D3DXVECTOR2 size;

			D3DXVECTOR2 fontSize;

			int			maxChatDataNum;
			float		fChatTime;
			float		fVanishTime;

			ChatWindowData& operator=(const ChatWindowData& rhs)
			{
				if ( this == &rhs)
				{
					return *this;
				}

				pos					= rhs.pos;
				size				= rhs.size;
				fontSize			= rhs.fontSize;
				maxChatDataNum		= rhs.maxChatDataNum;
				fChatTime			= rhs.fChatTime;
				fVanishTime			= rhs.fVanishTime;

				return *this;
			}

		};

		enum CHAT_TYPE
		{
			CT_NONE = 0,
			CT_KILL,
			CT_NORMAL,
			CT_CHAT,
			CT_PICK_ITEM,
			CT_GET_EXP,
			CT_LEVEL_UP,
			CT_MISC,
		};

		struct ChatData
		{
			CHAT_TYPE	chatType;
			wstring		senderName;
			wstring		message;
			float		fTime;
			bool		bVanishing;
			D3DXCOLOR	textColor;
			int			nRow;

			ChatData()
			{
				chatType	= CT_NONE;
				senderName	= L"";
				message		= L"";
				fTime		= 0.0f;
				bVanishing	= false;
				textColor	= D3DXCOLOR(1,1,1,1);
				nRow		= 0;
			}
		};

	public:

		CX2ChatWindow(ChatWindowData& noticeManagerData);
		virtual ~CX2ChatWindow(void);

		HRESULT OnFrameMove( double fTime, float fElapsedTime );
		HRESULT OnFrameRender();

		void	PushChatData( ChatData* pNotice );

		bool GetShow() const { return m_bShow; }
		void SetShow(bool val) { m_bShow = val; }


		CX2ChatWindow::ChatWindowData* GetChatWindowData() { return &m_ChatWindowData; }



	protected:
		bool								m_bShow;
		
		ChatWindowData						m_ChatWindowData;


		CKTDGFontManager::CUKFont*			m_pFont;
#ifndef KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT
		CKTDGFontManager::CKTDGFont*		m_pFontForUniBuffer;
#endif
		std::deque< ChatData* >				m_ChatDataList;

		//CKTDGUIUniBuffer					m_UniBuffer;
};
