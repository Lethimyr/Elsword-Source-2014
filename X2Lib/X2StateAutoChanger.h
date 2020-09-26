#pragma once

class CX2StateAutoChanger
{
public:
	struct TARGET_DETAIL
	{
		int m_iTrainingSchoolMode;
		//CX2StateTrainingSchool::TRAINING_SCHOOL_MODE m_eTrainingSchoolMode;
		int m_iLocalMapID;
		int m_iVillageID;
		int m_iChannelID;
		int m_iHouseID;
		int m_iDungeonID;
		UidType m_iRoomUID;
#ifdef SERV_NEW_PVPROOM_PROCESS
		bool m_bInvitation;
#endif SERV_NEW_PVPROOM_PROCESS

	public:
		TARGET_DETAIL()
		{
			//m_eTrainingSchoolMode = CX2StateTrainingSchool::TSM_INTRO;
			m_iTrainingSchoolMode = 0;
			m_iLocalMapID = -1;
			m_iVillageID = -1;
			m_iChannelID = -1;
			m_iHouseID = -1;
			m_iRoomUID = -1;
			m_iDungeonID = -1;
#ifdef SERV_NEW_PVPROOM_PROCESS
			m_bInvitation = false;
#endif SERV_NEW_PVPROOM_PROCESS
		}
	};


public:
	CX2StateAutoChanger();
	virtual ~CX2StateAutoChanger();

	void EndStateChange();
	void StartStateChange( int iNowStateID, int iTargetStateID, TARGET_DETAIL targetDetail, UidType myUnitUID = -1 );
	void DoStateChange( int iNowStateID );
	

	void StateChangeAfterConstructor();

	bool GetIsAutoChangingState() const { return m_bIsAutoChangingState; }
	void SetIsAutoChangingState(bool val) { m_bIsAutoChangingState = val; }
	int GetTargetStateID() const { return m_iTargetStateID; }
	UidType GetMyUnitUID() const { return m_iMyUnitUID; }

	int GetTryEnterRoomCount() const { return m_iTryEnterRoomCount; }
	void SetTryEnterRoomCount(int val) { m_iTryEnterRoomCount = val; }


	//void TryEnterDungeonChannel();
	//void TryEnterDungeonRoom( bool bIsPublic = true );
	void TryEnterPVPRoom( bool bIsPublic = true );
	void TryEnterArcadeRoom( bool bIsPublic = true );


	TARGET_DETAIL& GetTargetDetail() { return m_TargetDetail; }


protected:
#ifndef ELSWORD_NEW_BEGINNING
	void InCase_XS_CREATE_UNIT();
#endif ELSWORD_NEW_BEGINNING
	void InCase_XS_SERVER_SELECT();

	//void InCase_XS_WORLD_MAP();
	//void InCase_XS_LOCAL_MAP();
	
	void InCase_XS_VILLAGE_MAP();
	void InCase_XS_PVP_LOBBY();
	void InCase_XS_PVP_ROOM();
	void InCase_XS_DUNGEON_ROOM();
	void InCase_XS_DUNGEON_RESULT();
	//void InCase_XS_SHOP();
	//void InCase_XS_NPC_HOUSE();
	void InCase_XS_SQUARE_GAME();
	void InCase_XS_TRAINING_SCHOOL();
	void InCase_XS_ALCHEMIST_HOUSE();
	void InCase_XS_DUNGEON_GAME();
	void InCase_XS_BATTLE_FIELD();
	//void InCase_XS_CLASS_CHANGE();

	//void InCase_XS_ARCADE_LOBBY();
	//void InCase_XS_ARCADE_ROOM();
	//void InCase_XS_ARCADE_GAME();
	

private:
	bool		m_bIsAutoChangingState; 
	UidType		m_iMyUnitUID;

	int			m_iTargetStateID;
	TARGET_DETAIL m_TargetDetail;		// target 상태에 대한 detail 정보
	int			m_iTryEnterRoomCount;	// 비밀방인지 알기위해 방에 조인 시도를 2번까지 할 수 있다

	CKTDGUIDialogType		m_pDLGLoadingBG;
	int					m_iLoadingBGIndex;		// 스테이트 이동중에 보여줄 random loading background
	
};
