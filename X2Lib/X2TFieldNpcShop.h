#pragma once

class CX2TFieldNpcShop
{   
public:
	enum NPC_MESSAGE_UI_MSG
	{
		NMUM_CLOSE				= 15000,
		NMUM_QUEST,
		NMUM_SHOP,
		NMUM_PVP,
		NMUM_MAKE,
		NMUM_FREETRAIN,
		NMUM_TRAINING,
		NMUM_PASS,
		NMUM_NEXT,
		NMUM_AGREE,
		NMUM_COMPLETE,
		NMUM_ENCHANT,
		NMUM_ATTRIBUTE,
		NMUM_RANKING,
		NMUM_EXCHANGE,			// 헤니르 시공에 들어가는 아이템 교환	// kimhc // 2009-07-25
		
		//{{ kimhc // 2009-08-06 // 캐릭터별 은행
//#ifdef PRIVATE_BANK
		NMUN_PRIVATE_BANK,		// 개인 은행							// kimhc // 2009-08-03
//#endif PRIVATE_BANK
		//}} kimhc // 2009-08-06 // 캐릭터별 은행
		NMUM_GUILD,
		NMUM_WEB_EVENT,		// oasis907 : 김상윤 [2010.2.24] // 
		NMUM_PERSONAL_SHOP,
//#ifdef SERV_PSHOP_AGENCY
		NMUM_AGENCY_TRADER_REGISTER,	// 대리상인 - 물품등록
		NMUM_AGENCY_TRADER_RECEIVE,		// 대리상인 - 물품받기
//#endif
//#ifdef SERV_PVP_NEW_SYSTEM
		NMUM_FORMAL_PVP,
//#endif
//#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT 
		NMUM_WEDDING_EVENT			= 15022,	// 결혼 시스템 런칭 이벤트 게시판 버튼 클릭
		// <cocy33 김창한> 기존 코드가 UIManager에서 해당 UI의 클래스를 가지고 있고 메세지는 해당 UI의 UICustom에서 받게 되어있는데 이벤트를 위해서 클래스를 만드는 것이 비효율적이라 생각.
		NMUM_WEDDING_EVENT_SEND_LETTER,			// 결혼 시스템 런칭 이벤트 편지지에서 편지 발송 버튼 클릭
		NMUM_WEDDING_EVENT_CLOSE,				// 결혼 시스템 런칭 이벤트 편지지 닫기
//#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		NMUM_NEW_ITEM_EXCHANGE,

//#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		NMUM_WEDDING_EVENT_FOCUS_LETTER,		// 결혼 시스템 런칭 이벤트 편지지 포커스 처리
//#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

//{{ 2011.05.04   임규수 아바타 합성 시스템
//#ifdef SERV_SYNTHESIS_AVATAR
		NMUM_SYNTHESIS,
//#endif SERV_SYNTHESIS_AVATAR
//}}
//#ifdef NPC_EVENT_BUTTON
		NMUM_EVENT1,
		NMUM_EVENT2,
		NMUM_EVENT3,
		NMUM_EVENT4,
//#endif NPC_EVENT_BUTTON
	};

	enum NPC_SHOP_BUTTON_TYPE
	{
		NSBT_NONE = 0,
		NSBT_QUEST,
		NSBT_SHOP,
		NSBT_PVP,
		NSBT_MAKE,
		NSBT_FREETRAIN,
		NSBT_TRAINING,
		NSBT_CLOSE,
		NSBT_PASS,
		NSBT_NEXT,
		NSBT_AGREE,
		NSBT_COMPLETE,
		NSBT_ENCHANT,
		NSBT_ATTRIBUTE,
		NSBT_RANKING,
		NSBT_EXCHANGE,			// 헤니르 시공에 들어가는 아이템 교환	// kimhc // 2009-07-25
		//{{ kimhc // 2009-08-06 // 캐릭터별 은행
//#ifdef PRIVATE_BANK
		NSBT_PRIVATE_BANK,		// 개인은행								// kimhc // 2009-08-03
//#endif PRIVATE_BANK
		//}} kimhc // 2009-08-06 // 캐릭터별 은행

//#ifdef GUILD_BOARD
		NSBT_GUILD,				// oasis907 : 김상윤 [2009.11.24] //
//#endif GUILD_BOARD
//#ifdef WEB_POINT_EVENT
		NSBT_WEB_EVENT,
//#endif WEB_POINT_EVENT
//#ifdef DEF_TRADE_BOARD
		NSBT_PERSONAL_SHOP,
//#endif DEF_TRADE_BOARD
//#ifdef SERV_PSHOP_AGENCY
		NSBT_AGENCY_TRADER_REGISTER,	// 대리상인 - 물품등록
		NSBT_AGENCY_TRADER_RECEIVE,		// 대리상인 - 물품받기
//#endif
		//{{ 2011.05.04   임규수 아바타 합성 시스템
//#ifdef SERV_SYNTHESIS_AVATAR
		NSBT_SYNTHESIS,
//#endif SERV_SYNTHESIS_AVATAR
		//}}
//#ifdef NPC_EVENT_BUTTON
		NSBT_EVENT1,
		NSBT_EVENT2,
		NSBT_EVENT3,
		NSBT_EVENT4,
//#endif NPC_EVENT_BUTTON
//#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
		NSBT_WEDDING_EVENT,				// 결혼 시스템 이벤트 버튼
//#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
//#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
		NSBT_EXCHANGE_NEW_ITEM,
//#endif // SERV_NEW_ITEM_SYSTEM_2013_05

	};

	struct NPCSHOPTYPE
	{
		bool			m_bShowNpcShop;
		NPC_SHOP_BUTTON_TYPE	m_eShopType;
	};	

    CX2TFieldNpcShop();
    virtual ~CX2TFieldNpcShop();

    void RegisterLuaBind();
	void SetStage(CKTDXStage *pStage);

	void SetNpcId(int npcId, wstring houseName);
	void SetShopType(NPC_SHOP_BUTTON_TYPE eType, bool bShow = true);
	bool GetShopType(NPC_SHOP_BUTTON_TYPE eType);
	void SetShow(bool val);
    void SetShow(NPC_SHOP_BUTTON_TYPE eType, bool val);
	bool GetShow() { return m_bShowNpcShop; }
	bool GetShow(NPC_SHOP_BUTTON_TYPE eType);
	
	void SetHouseName(wstring houseName);	
	void SetNpcMessage(const wstring &npcMsg);
#ifdef SERV_PSHOP_AGENCY
	void SetNpcMessageWithEtc(const wstring &npcMsg, const wstring &npcMsgEtc);
#endif

	void CreateHouseNPCName(CX2LocationManager::HouseTemplet* pHouse);

	HRESULT OnFrameMove( double fTime, float fElapsedTime );
	int	 UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	void LeaveField();
	void SetQuestDesc(const wstring &title, const wstring &message);

	void SetPlayGuide(int val) { m_iIsPlayGuide = val; }
	void SetKeyEvent();

#ifdef DEF_TRADE_BOARD
	void SetEnablePersonalShopBoardButton(bool bEnable);
#endif DEF_TRADE_BOARD

	
private:
	void UpdateQuestDesc( const wstring& wstrDesc );
	void UpdateQuestDescPage( int iPage );
	

protected:    

    //{{ seojt // 2009-8-18, 18:03, qff
    CKTDGUIDialogType       m_pDlgMessage;
    //}} seojt // 2009-8-18, 18:03
#ifdef SERV_READY_TO_SOSUN_EVENT
	CKTDGUIDialogType		m_pDlgReadyToSosun;
#endif SERV_READY_TO_SOSUN_EVENT

	std::vector<NPCSHOPTYPE> m_vecShopType;

private:
	int						m_iNpcID;
	bool					m_bShowNpcShop;

#ifdef DEF_TRADE_BOARD
	bool					m_bEnableButtonPersonalShop;
#endif DEF_TRADE_BOARD

	CKTDGUIButton*			m_pButtonQuest;
	CKTDGUIButton*			m_pButtonPvp;
	CKTDGUIButton*			m_pButtonShop;
	CKTDGUIButton*			m_pButtonMake;
	CKTDGUIButton*			m_pButtonTraining;
	CKTDGUIButton*			m_pButtonFreeTraining;
	CKTDGUIButton*			m_pButtonGuild; // oasis907 : 김상윤 [2009.11.24] // 길드 게시판
	CKTDGUIButton*			m_pButtonRanking;

	//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
	CKTDGUIButton*			m_pButtonExchange;
#endif	ITEM_EXCHANGE_SHOP
	//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
	
	//{{ kimhc // 2009-08-03 // 캐릭터별 은행
#ifdef PRIVATE_BANK
	CKTDGUIButton*			m_pButtonPrivateBank;
#endif PRIVATE_BANK
	//}} kimhc // 2009-08-03 // 캐릭터별 은행

#ifdef WEB_POINT_EVENT
	CKTDGUIButton*			m_pButtonWebEvent;
#endif WEB_POINT_EVENT
#ifdef DEF_TRADE_BOARD
	CKTDGUIButton*			m_pButtonPersonalShop;
#endif DEF_TRADE_BOARD
	//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
	CKTDGUIButton*			m_pButtonSynthesis;
#endif SERV_SYNTHESIS_AVATAR
	//}}
	CKTDGUIButton*			m_pButtonBack;
	CKTDGUIButton*			m_pButtonOut;
	CKTDGUIButton*			m_pButtonPass;
	CKTDGUIButton*			m_pButtonWindowPass;

	CKTDGUIButton*			m_pButtonNext;
	CKTDGUIButton*			m_pButtonAgree;
	CKTDGUIButton*			m_pButtonComplete;

	CKTDGUIButton*			m_pButtonEnchant;
	CKTDGUIButton*			m_pButtonAttribute;

#ifdef SERV_PSHOP_AGENCY
	CKTDGUIButton*			m_pButtonAgencyTraderRegister;
	CKTDGUIButton*			m_pButtonAgencyTraderReceive;
#endif

#ifdef SERV_PVP_NEW_SYSTEM
	CKTDGUIButton*			m_pButtonFormalPvp;
#endif

#ifdef NPC_EVENT_BUTTON
	CKTDGUIButton*			m_pButtonEvent1;
	CKTDGUIButton*			m_pButtonEvent2;
	CKTDGUIButton*			m_pButtonEvent3;
	CKTDGUIButton*			m_pButtonEvent4;
#endif NPC_EVENT_BUTTON

#ifdef SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT
	CKTDGUIButton*			m_pButtonWeddingEvent;
#endif //SERV_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	CKTDGUIButton*			m_pButtoenExchangeNewItem;
#endif // SERV_NEW_ITEM_SYSTEM_2013_05


	CKTDGUIStatic*			pStaticShopName1;
	CKTDGUIStatic*			pStaticShopName2;
	CKTDGUIStatic*			pStaticShopmessage;

	//CX2TFieldShop*			m_pShop;
	NPC_SHOP_BUTTON_TYPE	m_eNowState;

	int						m_nEnabledButton;

    //{{ seojt // 2009-8-18, 18:03
    CKTDGUIDialogType			m_pDLGNpcName;
    //}} seojt // 2009-8-18, 18:03
	int						m_iHouseID;

	D3DXVECTOR2			m_vQuestDescSize;				// 퀘스트 설명 글자가 나올 창의 크기, 글자 부분만
	wstring				m_wstrQuestTitle;				// 선택된 퀘스트의 제목
	vector<wstring>		m_vecQuestDesc;					// 선택된 퀘스트의 설명
	int					m_iNowQuestDescPage;
	int					m_iMaxQuestDescPage;

	// 초심자 가이드 진행여부를 저장한다.(가이드 단계값)
	int				m_iIsPlayGuide;

	vector<wstring>		m_vButtonNameList;

#ifdef LOCAL_RANKING_UPDATE
	bool				m_bShowLocalRankingInfo;
#endif //LOCAL_RANKING_UPDATE

};
