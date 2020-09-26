#pragma once

#define MAX_KIND_OF_NPC 12

class CX2BattleFieldGame : public CX2Game
{
public:

	typedef std::map<int, KAttribEnchantNpcInfo> KAttribEnchantNpcInfoMap;

	CX2BattleFieldGame();
	virtual ~CX2BattleFieldGame();

	virtual void			Release();

	virtual HRESULT			OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT			OnFrameRender();

	virtual void			GameLoading( CX2Room* pRoom );

	virtual void			WorldLoading();
	
	virtual bool			DetermineLastKill() { return false; }

	virtual	void			ResurrectOtherUser();

	virtual bool			Handler_EGS_NPC_UNIT_DIE_NOT( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	bool					Handler_EGS_JOIN_BATTLE_FIELD_NOT( KEGS_JOIN_ROOM_NOT& kPacket_ );


	void					CreateNpcCreatedInfo( const vector<KNPCUnitReq>& vecNpcData_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfoMap& mapAttribNpcInfo_ 
								, bool bSetNPCLoadComplete
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		);
	//virtual bool			Handler_EGS_LEAVE_ROOM_ACK( KEGS_LEAVE_ROOM_ACK& kEGS_LEAVE_ROOM_ACK );
	//virtual bool			Handler_EGS_LEAVE_ROOM_NOT( KEGS_LEAVE_ROOM_NOT& kEGS_LEAVE_ROOM_NOT );

	//virtual bool			Handler_EGS_END_GAME_NOT( KEGS_END_GAME_NOT& kKEGS_END_GAME_NOT );

	//virtual void			UserUnitDieNot( KEGS_USER_UNIT_DIE_NOT& kEGS_USER_UNIT_DIE_NOT );

/*	void					CreateNpcFromDungeonIntrudeSyncPacket( const KEGS_DUNGEON_INTRUDE_SYNC_NOT& kPacket_ );*/
	void					SetBattleFieldId( UINT uiBattleFieldId_ ) { m_pBattleFieldRoom->SetBattleFieldId( uiBattleFieldId_ ); }

	void					SwapWorld();

	UINT					GetWorldId() const { return m_uiWorldId; }
	void					SetWorldId( const UINT uiWorldId_ ) { m_uiWorldId = uiWorldId_; }

	int	GetMyGUUserLastTouchedLineMapIndex( const bool bFutureState_ ) const 
	{
		ASSERT( GetMyUnit() );
		return GetMyUnit()->GetLastTouchLineIndex( bFutureState_ );
	}

	bool					IsNearPortalLineMap();
	
	void					CreateBattleFieldNpcAtStart( const std::vector< KNPCUnitReq >& vecNPCDataList_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfoMap& mapAttribNpcInfo_ 
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD		
		);
	void					CreateBattleFieldNpcByNotData( const std::vector< KNPCUnitNot >& vecNPCDataList_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfoMap& mapAttribNpcInfo_ 
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD	
		);

	void					CreateBattleFieldNpc( const KNPCUnitReq& kNpcUnitData_
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
								, const KAttribEnchantNpcInfo* pkAttribNpcInfo_
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
		);

	void					ClearNpcCreatedInfoList();
	bool					DeleteNpcInCreatedInfoList( const UINT uiNpcUid_ );
	virtual bool			DeleteNPCUnitByUID( const UINT uiNpcUid_ );
	bool					GetIsNpcLoadComplete() const {return m_bIsNpcLoadComplete;}

	void					Send_EGS_UPDATE_BATTLE_FIELD_USER_POS_NOT();
	virtual bool			Handler_EGS_LEAVE_BATTLE_FIELD_NOT( const KEGS_LEAVE_BATTLE_FIELD_NOT& kPacket );

	virtual void			AddUserUnit( CX2Room::SlotData* pSlotData_, int iSlotIndex_ );
	CX2GUUser*				CreateAndInsertUserUnit(  CX2Room::SlotData* pSlotData_, int iSlotIndex_ );
	
#ifndef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	void					SetKAttribEnchantNpcInfoMap( KAttribEnchantNpcInfoMap& mapAttribNpcInfo_ )
	{
		m_mapAttribNpcInfo.swap( mapAttribNpcInfo_ );
	}
	void					ClearKAttribEnchantNpcInfoMap() { m_mapAttribNpcInfo.clear(); }
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD

#ifdef QUEST_GUIDE
	CX2NPCIndicator*		GetNpcIndicator() const {return m_pNPCIndicator;}
#endif QUEST_GUIDE

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CX2GUNPC *				GetChainedBossGroup ( CX2GUNPC * pGUNPC_ );		// 필드 내의 GroupID 가 같은 보스를 검색
	void					SetShowMonsterIndicator (bool bVal ) { if( NULL != m_pMonsterIndicator) m_pMonsterIndicator->SetShow(bVal); } // 몬스터 알림판을 보이게 함
	CX2MonsterIndicator*	GetMonsterIndicator() const { return m_pMonsterIndicator; }
	void					SetMonsterIndicator(CX2MonsterIndicator* val) { m_pMonsterIndicator = val; }
	void					UpdateCreateUnitDisplayEffect ( CX2GUNPC * pNPC_, bool bIsBossSummoned_ );		// 몬스터 등장 및 모두 사망 시, 해당 몬스터 그룹 이름에 대한 연출
	void					SetIndicatorToNpc ( CX2GUNPC * pNPC_ );		// NPC 에 Indicator 를 할당 및 설정
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef SERV_CHECK_EXIST_MONSTER_UID
	void					StartElapsedTimeSinceGettingFirstNpcSyncPacket();
	bool					GetCanCheckGettingFirstNpcSyncPacket() const { return m_bCanCheckGettingFirstNpcSyncPacket; }
	void					SetCanCheckGettingFirstNpcSyncPacket(bool val) { m_bCanCheckGettingFirstNpcSyncPacket = val; }
	void					Handler_EGS_CHECK_EXIST_MONSTER_UID_REQ( const KEGS_CHECK_EXIST_MONSTER_UID_REQ& kPacket_ );
	bool					Handler_EGS_CHECK_EXIST_MONSTER_UID_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_CHECK_EXIST_MONSTER_UID

#ifdef FIELD_BOSS_RAID
	void					CreateRaidPotal();
	void					DestoryRaidPotalParticle();
	void					CreateLoadingUI();
	bool					GetShowLoadUi() const { if(m_pDLGLoadingState == NULL) return false; return true; }
#endif // FIELD_BOSS_RAID

#ifdef SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX
	void					ResetTimerForSendingPositionInfo(){ m_TimerForSendingPositionInfo.ResetSumOfElapsedTime();}
#endif // SERV_OPTIMIZE_MOVE_TO_BATTLEFIELD_LOGIC_FIX

protected:

	virtual void			UpdateUnitPointer();	
	void					CreateBattleFieldNpcByCreatedInfo( const CNpcCreatedInfoPtr NpcCreatedInfoPtr_ );
	virtual void			ResurrectionKeyProcess();

#ifdef SERV_CHECK_EXIST_MONSTER_UID
	void					CheckNpcNotReceiveFirstSyncPacketAndRequestToCheckNpcUidList();
	void					CheckNpcNotReceiveFirstSyncPacketAndCollectNpcUid( vector<int>& vecMonsterUidToCheck_ ) const;
#endif // SERV_CHECK_EXIST_MONSTER_UID

#ifdef NEW_CHARACTER_EL
	virtual bool			GetIsExsitancePartyMemberHPLessThanPer( const float fHPRate_, const CX2GUUser* pGUUserWhoCallFunction_ ) const;
#endif // NEW_CHARACTER_EL

#ifdef FIELD_BOSS_RAID
	void					OnFrameMove_LoadingUI(double fTime, float fElapsedTime);
#endif // FIELD_BOSS_RAID

private:

	void					OpenBattleFieldName();
	void					CreatePotalParticle();
#ifdef SERV_COMMON_AUTO_OPEN_HARD_CODE
	void					CreatePotalParticle( const int& iLineIdx ) const;
#endif // SERV_COMMON_AUTO_OPEN_HARD_CODE
	UINT					GetEmpyIndexNpcUnitList();	
protected:
	CX2BattleFieldRoom*		m_pBattleFieldRoom;
	CKTDGUIDialogType		m_pDlgBattleFieldName;
	CKTDXCheckElapsedTime	m_TimerForSendingPositionInfo;
	const int				m_iMAX_NUM_OF_NPC;
	UINT					m_uiWorldId;
#ifdef	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::deque<CNpcCreatedInfoPtr>			m_deqNpcCreatedInfo;
#else	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	std::vector<CNpcCreatedInfoPtr>			m_vecNpcCreatedInfo;
	KAttribEnchantNpcInfoMap	m_mapAttribNpcInfo;
#endif	X2OPTIMIZE_FIELD_NPC_BACKGROUND_LOAD
	bool					m_bIsNpcLoadComplete;

#ifdef QUEST_GUIDE
	CX2NPCIndicator*		m_pNPCIndicator;
#endif

#ifdef SERV_BATTLEFIELD_MIDDLE_BOSS
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hBossDanger;		// 중간 보스 출현 위험 연출
	CKTDGParticleSystem::CParticleEventSequenceHandle m_hBossDangerAdd;		// 중간 보스 출현 위험 연출

	CX2MonsterIndicator	*		m_pMonsterIndicator;		// 필드 중보 Indicator, 차후 확장 가능
	wstring 					m_wstrBossGroupName;		// 필드 보스 그룹 이름
#endif // SERV_BATTLEFIELD_MIDDLE_BOSS

#ifdef	SERV_CHECK_EXIST_MONSTER_UID
	CKTDXCheckElapsedTime		m_ElapsedTimeSinceGettingFirstNpcSyncPacket;	/// NPC로 부터 첫 싱크 패킷을 받은 이후로 부터의 경과 시간
	bool						m_bCanCheckGettingFirstNpcSyncPacket;			/// NPC로 부터 첫 싱크 패킷을 받았는지 체크 하려면 true
#endif	//	SERV_CHECK_EXIST_MONSTER_UID

#ifdef FIELD_BOSS_RAID
	CKTDGParticleSystem::CParticleEventSequenceHandle			m_hRaidGateParticle;			// 레이드 게이트 파티클 핸들
	CKTDGParticleSystem::CParticleEventSequenceHandle			m_hRaidPortalParticle;			// 레이드 포탈 파티클 핸들
	vector<CKTDGParticleSystem::CParticleEventSequenceHandle>	m_vecRaidGateEffectParticle;	// 레이드 게이트 이펙트 파티클 핸들

	CKTDGUIDialogType		m_pDLGLoadingState;		/// 워프로 이동 했을 때 생성 되는 로딩창

	float					m_fLoadUIReaminTime;
#endif // FIELD_BOSS_RAID

//#ifdef SERV_COMMON_AUTO_OPEN_HARD_CODE
	mutable bool						m_bOpenPotalParticle;
//#endif //SERV_COMMON_AUTO_OPEN_HARD_CODE

};
