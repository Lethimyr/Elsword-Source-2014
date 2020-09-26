#pragma once


#ifdef MONSTER_TROPHY
	class CX2NPCUnitViewerUI;
#endif MONSTER_TROPHY



class CX2UICharInfo : public CX2UIDragable
{
public:
 
	enum CHARACTER_INFORMATION_MSG
	{		
		CIM_EXIT	= 10000,
		CIM_TITLE,
//#ifdef MANUAL_STANDING_WAIT
		CIM_MOTION_SELECT,
//#endif
//#ifdef VIEW_SPECIAL_OPTIONDATA
		CIM_SPECIAL_OPTIONDATA_TOGGLE,
//#endif VIEW_SPECIAL_OPTIONDATA
#ifdef SERV_LOCAL_RANKING_SYSTEM
		CIM_PROFILE,
		CIM_PASSWORD_OK,
		CIM_PASSWORD_CANCLE,
#endif //SERV_LOCAL_RANKING_SYSTEM
	};

	CX2UICharInfo( CKTDXStage* pNowStage, const WCHAR* pFileName );
	virtual ~CX2UICharInfo(void);
	
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	virtual bool MouseDown( D3DXVECTOR2 mousePos );
	virtual bool MouseUp( D3DXVECTOR2 mousePos );
	virtual bool MouseRButtonUp( D3DXVECTOR2 mousePos );

	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    void SetShow(bool val);
	bool GetShow(){ return m_bShow; }

	virtual void SetNowDragItemReturn();

	void SetPosition(D3DXVECTOR2 vec);
	D3DXVECTOR2 GetDLGSize(){ return m_DLGSize; }
	void SetLayer(X2_DIALOG_LAYER layer);

	void ResetStat();
	void ResetNowEquipUI();
	void ResetEDnCashnVP();
	void UpdateLevel();
	void ResetResurrectionStoneUI();
	
	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	inline int	GetMyLevel() const;
	inline float GetDamageDecrement( const float fTotalDefValue_ ) const;	// 방

	void UpdateBasicStatUI( const CX2SocketItem::SocketData& socketData_ );		// HP, 물공, 마공, 물방, 마방 UI 갱신
	void UpdateOptionStatUI( const CX2SocketItem::SocketData& socketData_ );	// 크리티컬, 추가데미지, 명중, 각성충전속도, 각성지속시간, 회피, 데미지감소, 동작속도, 이동력, 점프력
	void UpdateOptionStatFromSpecialSkill( CX2SocketItem::SocketData& socketData_ ); // 스킬에 의한 추가 스탯(크리티컬, 추가데미지, 명중, 각성충전속도, 각성지속시간, 회피, 데미지감소, 동작속도, 이동력, 점프력)

#ifdef MODIFY_CHARACTER_STAT_INFO
	void UpdateOptionStatFromSpecialSkill_EVE(OUT CX2SocketItem::SocketData& socketData_, const CX2UserSkillTree& UserSkillTree_ );
	void UpdateOptionStatFromSpecialSkill_LENA(OUT CX2SocketItem::SocketData& socketData_, const CX2UserSkillTree& UserSkillTree_ );
#endif // MODIFY_CHARACTER_STAT_INFO

	#ifdef VIEW_SPECIAL_OPTIONDATA
	void UpdateResistanceStatUI(const CX2SocketItem::SocketData& socketData_ );	// 불, 물, 자연, 바람, 빛, 어둠 속성 저항 UI 갱신
	#else
	void UpdateResistanceStatUI( );	// 불, 물, 자연, 바람, 빛, 어둠 속성 저항 UI 갱신
	#endif //VIEW_SPECIAL_OPTIONDATA

	void UpdateBasicStatFromSpecialSkill();
	void SetStatInfoStatic( const WCHAR* wszStaticName_, const WCHAR* wszMsg_ );
	void SetStatInfoGuideDesc( const WCHAR* wszButtonName_, const WCHAR* wszGuideMsg_ );
	void SumSocketOption( const vector<int>& vecOptions_, OUT CX2SocketItem::SocketData& sumSocketData_, const int iSocketLevel_ );	
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

	CX2SocketItem::SocketData GetMyEquippedSocketData();
	
	virtual bool OnRClickedItem( D3DXVECTOR2 mousePos );
	virtual bool OnDropAnyItem( D3DXVECTOR2 mousePos );

	void EquipChanged();

	// 유닛뷰어 관련
	CX2UnitViewerUI* GetUnitViewerUI() { return m_pUnitViewerUI; }
	void ResetUnitViewerUI();
	void DeleteUnitViewerUI();

	bool IsInUnitClickReact( D3DXVECTOR2 mousePos );
	void UnitHandling( double fTime, float fElapsedTime );
	
    void DrawSlotMouseOverImage();
	wstring GetSlotItemDesc();
	
	void SetTitle();

	bool GetOverMouseCharInfo();


#ifdef MONSTER_TROPHY
	void SetNPCID(const CX2UnitManager::NPC_UNIT_ID val) { m_eNPCID = val; }

	void OpenDLGNPCViewerUI( bool bOpen );
	void UpdateDLGNPCViewerUI();
#endif MONSTER_TROPHY

	void UpdateItemLevel();


protected:
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2SlotItem* pFromCX2SlotItem, CX2SlotItem* pToCX2SlotItem );
	bool Handler_EGS_CHANGE_INVENTORY_SLOT_REQ( CX2Inventory::SORT_TYPE fromSortType, int fromSlotID, CX2Inventory::SORT_TYPE toSortType, int toSlotID );

#ifdef SERV_LOCAL_RANKING_SYSTEM
	void OpenProfilePasswordDlg();
#endif // SERV_LOCAL_RANKING_SYSTEM

	static float GetIntermediateCriticalValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateCriticalValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateMoveSpeedValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateMoveSpeedValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateJumpSpeedValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateJumpSpeedValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateAnimationSpeedValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateAnimationSpeedValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateAnitEvadeValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateAntiEvadeValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateEvadeValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateEvadeValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateHyperGageChargeSpeedValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateHyperGageChargeSpeedValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateAdditionalDefenceValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateAdditionalDefenceValue", fSumValue_, iUserLevel_ );
	}

	static float GetIntermediateAdditionalAttackValue( const float fSumValue_, const int iUserLevel_ )
	{
		return lua_tinker::call<float>( g_pKTDXApp->GetLuaBinder()->GetLuaState(), "CalculateIntermediateAdditionalAttackValue", fSumValue_, iUserLevel_ );
	}

private:

    CKTDGUIDialogType       m_pDLGCharInfo;   

#ifdef SERV_LOCAL_RANKING_SYSTEM
	CKTDGUIDialogType		m_pDlgPassword;
#endif //SERV_LOCAL_RANKING_SYSTEM
//{{ robobeg : 2013-11-04
	//CX2Stat*				m_pStat;
	//CX2Stat*				m_pStatEqip;
	CX2Stat				    m_Stat;
	CX2Stat				    m_StatEqip;
//}} robobeg : 2013-11-04
	CX2UnitViewerUI*		m_pUnitViewerUI;
	D3DXVECTOR2				m_DLGPosition;
	D3DXVECTOR2				m_DLGSize;

	// 유닛뷰어관련
	float					m_fRemainRotY;
	float					m_fMouseSensitivity;
	bool					m_bRotateReturn;
	D3DXVECTOR2				m_UnitClickPos;
	D3DXVECTOR2				m_UnitClickSize;
	bool					m_bUnitClicked;

	bool					m_bShow;
    
	D3DXVECTOR2				m_MovedPosition;


#ifdef MONSTER_TROPHY
	CKTDGUIDialogType       m_pDLGMonsterTrophy;
	CX2NPCUnitViewerUI*		m_pNPCViewerUI;
	CX2UnitManager::NPC_UNIT_ID m_eNPCID;
#endif MONSTER_TROPHY

#ifdef PET_AURA_SKILL
//{{ robobeg : 2013-11-04
	//CX2Stat*				m_pStatPet;
    CX2Stat				    m_StatPet;
//}} robobeg : 2013-11-04
#endif
   
#ifdef VIEW_SPECIAL_OPTIONDATA
	bool					m_bShowPVPOption;
	bool					m_bShowDungeonOption;
#endif //VIEW_SPECIAL_OPTIONDATA

#ifdef MODIFY_CHARACTER_STAT_INFO
	float					m_fAddEvadePercent;
#endif // MODIFY_CHARACTER_STAT_INFO

};


