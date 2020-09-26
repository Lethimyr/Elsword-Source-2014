#pragma once

#pragma warning(disable:4996)
#include "../X2ServerProtocol/X2ServerProtocolLib.h"
#pragma warning(disable:4996)

#include "KTDX.h"
#pragma warning(disable:4996)
#include "../../KncWX2Server/Common/Enum/DungeonEnum.h"
#include "../../KncWX2Server/Common/Enum/Enum.h"

#include "./X2Define.h"
#ifdef _NEXON_KR_
#include "../NexonModule/NMClass/NMPCH.h"
#include "../NexonModule/NMClass/NMManager.h"
#include "../NexonModule/NMClass/NMEventHandler.h"
#endif _NEXON_KR_

#ifdef CLIENT_COUNTRY_ID
#pragma comment(lib, "../X2Lib/OnlyGlobal/ID/NMClientAuthLib/NMClientAuthDLL_MT.lib")
#include "../X2Lib/OnlyGlobal/ID/NMClientAuthLib/NMClientAuthLib.h"
#include "../X2Lib/OnlyGlobal/ID/NMRunParamLib/NMRunParamLib.h"
#include "../X2Lib/OnlyGlobal/ID/SSOWebLib/SSOWebLib.h"
#endif // CLIENT_COUNTRY_ID

//{{ kimhc // 2010.3.3 // 광고 노출도 적용
#ifdef	IGA_TEST
#include "../IGA/include/iga.h"
#include "./X2IGA.h"
#endif	IGA_TEST
//}} kimhc // 2010.3.3 // 광고 노출도 적용

//{{ 허상형 : [2009/8/4] //	핵실드
#ifdef HACK_SHIELD 
#include "./HShield.h"
#include "./X2HSGame.h"
#endif
//}} 허상형 : [2009/8/4] //	핵실드

//-- 2010. 12. 13.  지헌 : XTRAP 클라이언트 - 해더
#ifdef CLIENT_USE_XTRAP
#if(defined _MT)
#if(defined _DLL)
#pragma comment(lib, "XTrap4Client_ClientPart_mtDll.lib")
#pragma comment(lib, "XTrap4Client_mtDll.lib")
#pragma comment(lib, "XTrap4Client_ServerPart_mtDll.lib")
#pragma comment(lib, "XTrap_Unipher_mtDll.lib")
#else	// _DLL
#pragma comment(lib, "XTrap4Client_ClientPart_mt.lib")
#pragma comment(lib, "XTrap4Client_mt.lib")
#pragma comment(lib, "XTrap4Client_ServerPart_mt.lib")
#pragma comment(lib, "XTrap_Unipher_mt.lib")
#endif	// _DLL
#else	// _MT
#pragma comment(lib, "XTrap4Client_ClientPart_st.lib")
#pragma comment(lib, "XTrap4Client_st.lib")
#pragma comment(lib, "XTrap4Client_ServerPart_st.lib")
#pragma comment(lib, "XTrap_Unipher_st.lib")
#endif	// _MT
#include "./Xtrap_C_Interface.h"
#endif	// CLIENT_USE_XTRAP

#ifdef SERV_STEAM
#pragma comment(lib, "steam_api.lib")
#pragma comment(lib, "sdkencryptedappticket.lib")
#include "./X2Steam.h"
#endif //SERV_STEAM

// korea nProtect

#ifdef	CHECK_KOM_FILE_ON_LOAD
#include "./define.h"
#include "./HttpSession.h"
#endif	CHECK_KOM_FILE_ON_LOAD

#include "./NPGameLib.h"
#include "./X2KNPGame.h"

#include "./X2ServerEvent.h"
#include "./X2ServerPacket.h"
#include "./X2Struct.h"
#include "./X2NetError.h"
#include "./X2PacketLog.h"
#include "./X2GameOption.h"
#include "./X2CheckSumManager.h"

#include "./X2PVPEmblem.h"
#include "./X2EXPTable.h"
#include "./X2Stat.h"

#include "./X2BuffFactor.h"

#ifdef NEW_SKILL_TREE
	#include "./X2SkillTree.h"
	#include "./X2UserSkillTree.h"
#endif NEW_SKILL_TREE

#include "./X2Unit_PreHeader.h"
#include "./X2Item_PreHeader.h"
#include "./X2Inventory.h"
#include "./X2Unit.h"

#include "./X2UnitManager.h"
#include "./X2PVPResultTable.h"

#include "./X2DamageManager.h"
#include "./X2EnchantItem.h"
#include "./X2SocketItem.h"
#include "./X2Item.h"
#include "./X2ItemManager.h"
#include "./X2SubEquip.h"
#include "./X2Eqip.h"
#include "./X2UnitViewerUI.h"
#include "./X2NPCUnitViewerUI.h"
#include "./X2User.h"

#include "./X2ComboGuide.h"




#include "./X2WorldObject.h"
#include "./X2WorldObjectMesh.h"
#include "./X2WorldObjectSkinMesh.h"
#include "./X2WorldObjectLightFlow.h"
#include "./X2WorldObjectParticle.h"
#include "./X2WorldLayer.h"
#include "./X2World.h"
#include "./X2WorldManager.h"


#include "./X2Channel.h"
#include "./X2ChannelManager.h"
#include "./X2Room.h"
#include "./X2PVPRoom.h"


#include "./X2Packet.h"
#include "./X2Camera.h"
#include "./X2StringFilter.h"
#include "./X2DamageEffect.h"
#include "./X2EffectSet.h"

#include "./X2BuffBehaviorTemplet.h"
#include "./X2BuffFinalizerTemplet.h"
#include "./X2BuffDisplayerTemplet.h"
#include "./X2BuffTemplet.h"
#include "./X2BuffTempletManager.h"

#include "./X2ComboAndSkillChainSystem.h"

#include "./X2GageData.h"
#include "./X2GageUI.h"
#include "./X2GageManager.h"
#include "./X2ComboManager.h"
#include "./X2ChatWindow.h"
#include "./x2unitslashtracemanager.h"

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
#include "./X2FrameUDPPack.h"
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#include "./X2GameUnit.h"

#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#else REDUCED_PRECOMPILED_HEADER_TEST
//{{AFX
#include "./X2GUUser.h"
//}}AFX
#endif REDUCED_PRECOMPILED_HEADER_TEST

#include "./X2GUNPC.h"


#include "./X2SimplePathFinder.h"
#include "./X2NPCAI.h"
#include "./X2AllyNPCAI.h"
#include "./X2GUNPCAI.h"
#include "./X2BattleFieldNpcAi.h"

#include "./X2GUNPCFunc.h"

#ifdef REDUCED_PRECOMPILED_HEADER_TEST
#else REDUCED_PRECOMPILED_HEADER_TEST
//{{AFX
#include "./X2GUElsword_SwordMan.h"
#include "./X2GUArme_VioletMage.h"
#include "./X2GULire_ElvenRanger.h"
#include "./X2GURaven.h"
#include "./X2GuEve.h"

//{{ kimhc // 2010.11.25 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
#include "./X2GUChung.h"
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.25 //  2010-12-23 New Character CHUNG

#ifdef ARA_CHARACTER_BASE
#include "./X2GUAra.h"
#endif

#ifdef NEW_CHARACTER_EL
#include "./X2GUEl.h"
#endif // NEW_CHARACTER_EL

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
#include "./X2GUAdd.h"
#endif //SERV_9TH_NEW_CHARACTER
//}}AFX
#endif REDUCED_PRECOMPILED_HEADER_TEST


#include "./X2Curtain.h"
#include "./X2DropItemManager.h"


#include "./X2TalkBoxManagerImp.h"
#include "./X2TalkBoxManagerImpPShop.h"
#include "./X2TalkBoxManagerImpPartyLeader.h"
#include "./X2TalkBoxManager.h"
#include "./X2TalkBoxManagerUI.h"
#include "./X2UnitLoader.h"
#include "./X2TextManager.h"
#include "./X2NoticeManager.h"
#include "./X2GameEdit.h"
#include "./X2SlotManager.h"
#include "./X2SlotItem.h"
#include "./X2ItemSlotManager.h"
#include "./X2PageManager.h"
#include "./X2PageMgrItem.h"

//{{ JHKang / 강정훈 / 2012.04.12 / 팝업 UI Class
#include "./X2PopupUIBase.h"
//}}

//{{ 오현빈 // 2012-05-15 // 플레이 가이드 Class
#include "./X2PlayGuide.h"
//}}

#ifdef SERV_PET_SYSTEM

#include "./X2Pet.h"
#include "./X2PetAi.h"
#include "./X2PetManager.h"

#endif


#ifdef RIDING_SYSTEM
#include "./X2RidingPetSkillSlot.h"
#include "./X2RidingPet.h"
#include "./X2RidingPetManager.h"
#endif //RIDING_SYSTEM

#ifdef EVENT_SCENE
#include "./X2EventScene.h"
#endif EVENT_SCENE


#include "./X2SlideShot.h"
#include "./X2Game.h"
#include "./X2GameLoadingTip.h"
#include "./X2PVPGame.h"
#include "./X2PVPGameTeam.h"
#include "./X2PVPGameTeamDeath.h"
#include "./X2PVPGameSurvival.h"
#include "./X2UiUnit.h"
#include "./X2UIUnitManager.h"
#include "./X2SquareUnit.h"
#include "./X2SquareGame.h"

#include "./X2SkillTree.h"
#include "./X2ComboTree.h"
#include "./X2PartyManager.h"
#include "./X2Community.h"

#ifdef SERV_HERO_PVP_MANAGE_LIST //영웅대전 UI
#include "./X2UIHeroMatch.h"
#endif //SERV_HERO_PVP_MANAGE_LIST

#ifdef SERV_LOCAL_RANKING_SYSTEM //지인시스템
#include "./X2ProfileManager.h"
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef SERV_EVENT_BINGO
#include "./X2UIBingo.h"
#endif  //SERV_EVENT_BINGO

#ifdef COUPON_SYSTEM
#include "X2UICouponBox.h"
#endif // COUPON_SYSTEM

#include "./X2CashShop.h"

#ifdef NEW_VILLAGE_UI
#include "./X2UIDragable.h"
#endif

#ifdef POSTBOX
#include "./X2PostBox.h"
#endif

#ifdef SERV_SECOND_SECURITY
#include "./X2SecurityPad.h"
#endif SERV_SECOND_SECURITY

#include "./X2KeyPad.h"

#include "./X2CharPopupMenu.h"

#ifdef CHARACTER_MOUSE_RBUTTON
#include "./X2UserListPopUp.h"
#endif CHARACTER_MOUSE_RBUTTON

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
	#include "./X2UISkillTreeNew.h"
#else // UPGRADE_SKILL_SYSTEM_2013
	#include "./X2UISkillTree.h"
#endif // UPGRADE_SKILL_SYSTEM_2013

//{{  [11/11/2009 : oasis907 ]
#include "./X2UIGuildSkillTree.h"
//}}  [11/11/2009 : oasis907 ]

//{{ oasis907 : 김상윤 [2010.4.5] // 
#ifdef SERV_SOCKET_NEW
#include "./X2UISocketItem.h"
#endif SERV_SOCKET_NEW
//}}
#ifdef SERV_PET_SYSTEM
#include "./X2UIPetInventory.h"
#endif SERV_PET_SYSTEM

#include "./X2UIComboTree.h"

#ifdef NEW_VILLAGE_UI
#include "./X2UIEnduranceChecker.h"
#include "./X2UIInventory.h"
#include "./X2UICharInfo.h"
#include "./X2UIWatch.h"
#include "./X2UIPersonalShop.h"
#include "./X2UIPersonalTrade.h"
#include "./X2UIShop.h"
#include "./X2UIManufacture.h"
#include "./X2UIQuickSlot.h"
#ifdef SERV_PET_SYSTEM
#include "./X2UIPetInfo.h"
#endif
#ifdef SERV_EPIC_QUEST
#include "./X2UIQuestNew.h"
#endif SERV_EPIC_QUEST
#include "./X2UIQuestReceive.h"
#include "./X2UIQuest.h"
#include "./X2Book.h"
//{{ kimhc	// 실시간 엘소드 중 실시간 아이템 획득 과 관련한 임시 인벤토리 클래스
// 던전 내에서 아이템 획득시 인벤토리가 가득 찼을 경우 임시 인벤토리를 생성함
#ifdef	REAL_TIME_ELSWORD
#include "./X2UITempInventory.h"
#endif	REAL_TIME_ELSWORD
//}} kimhc	// 실시간 엘소드 중 실시간 아이템 획득 과 관련한 임시 인벤토리 클래스

//{{ kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업
#ifdef	RANKING_INFO_UI
#include	"./X2UIRankingInfo.h"
#endif	RANKING_INFO_UI
//}} kimhc // 2009-07-09 // 헤니르의 시공에 추가되는 Ranking UI 작업

//{{ 2011.9.16 이준호 반디캠 작업
#ifdef	BANDICAM_CAPTURE
#include "bandicap.h"
#endif	BANDICAM_CAPTURE
//}}



//{{ oasis907 : 김상윤 [2009.12.14] //
#ifdef	GUILD_BOARD
#include "./X2UIGuildSkillTreeInfo.h"

#include "./X2UIGuildBoard.h"
#endif	GUILD_BOARD
//}}

//{{ oasis907 : 김상윤 [2010.3.12] // 
#ifdef DEF_TRADE_BOARD
#include "./X2UIPersonalShopBoard.h"
#endif DEF_TRADE_BOARD
//}}


//{{ kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업
#ifdef	ITEM_EXCHANGE_SHOP
#include	"./X2UIItemExchangeShop.h"
#endif	ITEM_EXCHANGE_SHOP
//}} kimhc // 2009-07-25 // 헤니르의 시공에 추가되는 ITEM_EXCHANGE_SHOP 작업

//{{ kimhc // 2009-08-04 // 캐릭터별 은행
#ifdef	PRIVATE_BANK
#include	"./X2UIPrivateBank.h"
#endif	PRIVATE_BANK
//}} kimhc // 2009-08-04 // 캐릭터별 은행

//{{ 허상형 : [2009/9/8] //	길드 창단과 관리
#ifdef GUILD_MANAGEMENT
#include "./X2GuildManager.h"
#include "./X2UIGuild.h"
#endif	//	GUILD_MANAGEMENT
//}} 허상형 : [2009/9/8] //	길드 창단과 관리

#ifdef ADDED_RELATIONSHIP_SYSTEM
#include "./X2RelationshipManager.h"
#include "./X2UIRelationship.h"
#endif // ADDED_RELATIONSHIP_SYSTEM


//{{ 허상형 : [2011/3/14/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
#include "./X2WorldMissionManager.h"
#include "./X2UIWorldMission.h"
#endif SERV_INSERT_GLOBAL_SERVER
//}} 허상형 : [2011/3/14/] //	월드 미션

#ifdef SERV_GLOBAL_MISSION_MANAGER
#include "./X2GlobalMissionManager.h"
#include "./X2UIGlobalMission.h"
#endif SERV_GLOBAL_MISSION_MANAGER

//{{ JHKang / 강정훈 / 2011.6.3 / 휴면 복귀 유저 보상 및 특수 보상에 대한 UI 및 버프 처리
#ifdef COME_BACK_REWARD
#include "./X2PremiumBuffManager.h"
#endif
//}}

//{{ JHKang / 강정훈 / 2011.8.8 / 이탈 유저 방지 시스템
#ifdef DEPARTURE_PREVENTION_SYSTEM
#include "./X2DeparturePrevention.h"
#endif
//}}

#ifdef SERV_SKILL_NOTE
#include "./X2UISkillNote.h"
#endif

#ifdef CUBE_OPEN_IMAGE_MANAGER
#include "./X2CubeOpenImageManager.h"
#endif // CUBE_OPEN_IMAGE_MANAGER

//{{ 2011.03.16   임규수 아바타 분해 시스템
#ifdef SERV_MULTI_RESOLVE
#include "./X2UIResolveItem.h"
#endif SERV_MULTI_RESOLVE
//}}
//{{ 2011.05.04   임규수 아바타 합성 시스템
#ifdef SERV_SYNTHESIS_AVATAR
#include "./X2UISynthesis.h"
#endif SERV_SYNTHESIS_AVATAR
//}}
#include "./X2UIManager.h"
#endif

#include "./X2MiniMapUI.h"

#ifdef QUEST_GUIDE
#include "./X2NPCIndicator.h"
#endif //QUEST_GUIDE


#include "./X2BattleFieldManager.h"

#include "./X2MonsterIndicator.h"
#include "./X2DungeonSubStage.h"
#include "./X2DungeonStage.h"
#include "./X2Dungeon.h"
#include "./X2DungeonManager.h"
#include "./X2DungeonRoom.h"
#include "./X2DungeonGame.h"

#include "./X2BattleFieldRoom.h"
#include "./X2BattleFieldGame.h"

#include "./X2QuestManager.h"
#include "./X2Cursor.h"
#include "./X2LocationManager.h"
#include "./X2TrainingCenterTable.h"

//#ifdef TITLE_SYSTEM
//{{ 2008. 10. 1  최육사
#include "./X2TitleSystem.h"
#include "./X2TitleManager.h"
//}}
//#endif

//{{ 2009. 6. 24  최육사	보상테이블
#include "./X2RewardTable.h"
//}}

#ifdef QUEST_GUIDE
#include "./X2NPCIndicator.h"
#endif //QUEST_GUIDE

#include "./x2TFieldNpcShop.h"
#include "./X2TFieldNpc.h"
#include "./X2NoviceGuide.h"
#include "./X2TFieldGame.h"

#ifdef ELSWORD_NEW_BEGINNING
#include "./X2BeginningGame.h"
#endif ELSWORD_NEW_BEGINNING

#include "./X2Informer.h"
#include "./X2InformerSkill.h"
#include "./X2InformerQuest.h"
#include "./X2InformerInven.h"
#include "./X2InformerCashShop.h"
#ifdef	GUILD_MANAGEMENT
#include "./X2InformerGuildExp.h"
#endif	GUILD_MANAGEMENT
#include "./X2InformerManager.h"
#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
#include "./X2ItemStatCalculator.h"
#endif //SERV_NEW_ITEM_SYSTEM_2013_05
#include "./X2Data.h"
#include "./X2InstanceData.h"

#include "./X2MemoryHolder.h"
#include "./X2LVUpEventMgr.h"

#ifndef COUPON_SYSTEM // 이전 UI 제거
#include "./X2CouponBox.h"
#endif // COUPON_SYSTEM


#include "./X2TutorSystem.h"


#include "./X2LocalMapUI.h"
#include "./X2ChatBox.h"
#include "./X2PartyUI.h"





#include "./X2State.h"
#include "./X2StateOption.h"
#include "./X2StateCommonBG.h"
#include "./X2StateMenu.h"
#include "./X2StateStartUp.h"

#include "./X2StateLogin.h"
#include "./X2StateLoading.h"
#include "./X2StateJoin.h"
#ifndef ELSWORD_NEW_BEGINNING
#include "./X2StateCreateUnit.h"
#endif ELSWORD_NEW_BEGINNING

#include "./X2StateBattleField.h"

#include "./X2StatePVPLobby.h"
#include "./X2StatePVPRoom.h"
#include "./X2StatePVPGame.h"
#include "./X2StateDungeonRoom.h"
#include "./X2StateDungeonGame.h"
//#include "./X2StateArcadeGame.h"
#include "./X2StateSquareGame.h"

#include "./X2StateField.h"

#ifdef ELSWORD_NEW_BEGINNING
#include "./X2StateBeginning.h"
#endif ELSWORD_NEW_BEGINNING

#include "./X2StatePVPResult.h"
#include "./X2StateDungeonResult.h"
#include "./X2StateTrainingResult.h"
//#include "./X2StateArcadeResult.h"
#ifndef ELSWORD_NEW_BEGINNING
#include "./X2StateServerSelect.h"
#endif ELSWORD_NEW_BEGINNING
#include "./X2StateTrainingSchool.h"
#include "./X2StateTrainingGame.h"
#include "./X2StateClassChange.h"
//#include "./X2StateArcadeLobby.h"
#include "./X2StateAutoChanger.h"

#include "./X2NexonMsgHandler.h"

#include "./X2SystemInfo.h"

#ifdef ADD_COLLECT_CLIENT_INFO
#include "./X2CollectHackInfo.h"
#endif

//{{ JHKang / 강정훈 / 2010.09.06 / 숫자 출력 형태 변경
#ifdef NUMBER_TO_LANGUAGE
#include "./X2NumbersToLanguage.h"
#endif NUMBER_TO_LANGUAGE
//}} JHKang / 강정훈 / 2010.09.06 / 숫자 출력 형태 변경

#include "../X2/KOGGamePerformanceCheckDefine.h"

#if defined(CLIENT_GLOBAL_LINEBREAK) || defined(ELLIPSE_CN)
#include "./WordWrap.h"
#endif //defined(CLIENT_GLOBAL_LINEBREAK) || defined(ELLIPSE_CN)
#ifdef SERV_KOM_FILE_CHECK_ADVANCED
#include <boost/random.hpp>
using boost::mt19937;
#endif SERV_KOM_FILE_CHECK_ADVANCED

#ifdef ADDED_RELATIONSHIP_SYSTEM
#include "./X2StateWeddingGame.h"
#include "./X2WeddingManager.h"
#include "./X2WeddingGame.h"
#endif //ADDED_RELATIONSHIP_SYSTEM

#ifdef NEW_EMBLEM_MANAGER
#include "./X2EmblemManager.h"
#endif //NEW_EMBLEM_MANAGER

#ifdef NEW_MAIL_LOG
#include "./X2MailLog.h"
#endif // NEW_MAIL_LOG

#ifdef FIELD_BOSS_RAID
#include "./X2BossRaidManager.h"
#endif // FIELD_BOSS_RAID
#ifdef REFORM_SKILL_NOTE_UI
#include "./X2SkillNoteManager.h"
#endif // REFORM_SKILL_NOTE_UI
#ifdef ENUM_CHILD_WINDOW
BOOL CALLBACK EnumChildProc_Thread(HWND , LPARAM);
#endif

#if defined( SERV_HACKING_TOOL_LIST ) 
BOOL CALLBACK EnumWindowsProc_Thread(HWND , LPARAM);
BOOL Pesudo_IsDebuggerPresent();
#endif

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
	#include <boost/random.hpp>
	using boost::mt19937;
#endif // SERV_KOM_FILE_CHECK_ADVANCED

template < typename T >
inline void Serialize( KSerBuffer* pBuff, T* pData )
{
	KTDXPROFILE();
	KSerializer ks;
	ks.BeginWriting( pBuff );
	ks.Put( *pData );
	ks.EndWriting();
}

template < typename T >
inline void DeSerialize( KSerBuffer* pBuff, T* pData )
{
	KTDXPROFILE();
	KSerializer ks;
	ks.BeginReading( pBuff );
	ks.Get( *pData );
	ks.EndReading();
}




#ifdef LOADING_ANIMATION_TEST

	class CX2LoadingAnimation
	{
	public:
		CX2LoadingAnimation();
		~CX2LoadingAnimation();

		void OnFrameMove( double fTime, float fElapsedTime );
		void OnFrameRender();

		bool GetShow() const { return m_bShow; }
		void SetShow(bool val) { m_bShow = val; }

	private:
		bool				m_bShow;
		CKTDXTimer			m_timerAni;
		int					m_iCurrTextureIndex;
		vector<CKTDXDeviceTexture*> m_vecpTextureToAnimate;

		D3DXVECTOR2			m_vPos;
	};

#endif LOADING_ANIMATION_TEST


#ifdef REFACTORING_BY_TOOL_TEAM
#	ifdef SERV_HACKING_TOOL_LIST
	BOOL Pesudo_IsDebuggerPresent();
	class DebuggerChecker
	{
	public: 
		DebuggerChecker() 
			: m_ElapsedTime( 0.f ) 
		{
		}

		void OnFrameMove( const float elapsedTime );

	private: 
		float m_ElapsedTime;

	};


#	endif SERV_HACKING_TOOL_LIST
#endif REFACTORING_BY_TOOL_TEAM


#ifdef CHECK_THREAD_WND
	struct THREAD_WND_INFO
	{
		wstring m_wstrName;
		wstring m_wstrClass;
	};
#endif //CHECK_THREAD_WND



class CX2Main : public CKTDXStage
{
	public:
#ifdef CLIENT_USE_NATION_FLAG
		enum NationFlag
		{
			NF_NONE			= 0x00000000,
			NF_DE			= 0x00000001,
			NF_FR			= 0x00000002,
			NF_IT			= 0x00000003,
			NF_PL			= 0x00000004,
			NF_ES			= 0x00000005,
			NF_TR			= 0x00000006,
			NF_EN			= 0x00000007,

			NF_KR			= 0x00000100,
			NF_JP			= 0x00000200,
			NF_TW			= 0x00000300,
			NF_HK			= 0x00000301,
			NF_US			= 0x00000400,
			NF_CN			= 0x00000600,
			NF_VN			= 0x00000700,
			NF_TH			= 0x00000800,
			NF_ID			= 0x00000900,
			NF_BR			= 0x00001000,
			NF_PH			= 0x00001100,
			NF_IN			= 0x00001200,
		};
#endif CLIENT_USE_NATION_FLAG

		enum X2_STATE
		{
			XS_INVALID = 0,
			XS_START_UP,
			XS_LOGIN,
			XS_JOIN,
			XS_LOADING,
#ifdef ELSWORD_NEW_BEGINNING
			XS_BEGINNING,
#else
			XS_SERVER_SELECT,
#endif ELSWORD_NEW_BEGINNING
			XS_CREATE_UNIT,
			XS_PVP_LOBBY,
			XS_PVP_ROOM,
			XS_PVP_GAME,
			XS_DUNGEON_ROOM,
			XS_DUNGEON_GAME,
			XS_PVP_RESULT,
			XS_DUNGEON_RESULT,
			XS_VILLAGE_MAP,
			XS_SQUARE_GAME,
			XS_TRAINING_SCHOOL,
			XS_TRAINING_GAME,
			XS_TRAINING_RESULT,
			XS_CLASS_CHANGE,			
			XS_ARCADE_RESULT,	/// FieldFix: 삭제하자
			XS_ARCADE_GAME,		/// FieldFix: 삭제하자
			XS_ARCADE_LOBBY,	/// FieldFix: 삭제하자
			XS_ARCADE_ROOM,		/// FieldFix: 삭제하자
			XS_BATTLE_FIELD,
//#ifdef ADDED_RELATIONSHIP_SYSTEM
			XS_WEDDING_GAME,
//#endif // ADDED_RELATIONSHIP_SYSTEM

			XS_END,
		};

		enum X2_PUBLISHER
		{
			XP_NEXON_KOREA = 0,
			XP_GAMANIA_TW = 1,
			XP_GAMANIA_HK = 2,
			XP_NHN_JP = 3,
			XP_GIANT_CN = 4,
			XP_GAMEFORGE_EU = 5,
			XP_ASIASOFT_TH = 6,
			XP_K3RCOMBO_US = 7,
			XP_CJ_ID = 8,
			XP_LEVELUP_BR = 9,
			XP_GARENA_PH = 10,
			XP_FUNIZEN_IN = 11,
		};


		struct ReservedServerPacket
		{
			SERVER_EVENT_ID		m_eServerEventID;
			void*				m_pEvent;
		};

		struct WaitServerPacket
		{
			SERVER_EVENT_ID		waitACK;
			float				fReamainTimeOut;
		};

		struct ReservedMessagePopUp
		{
			vector<X2_STATE>	m_vecTargetState;
			wstring				m_Message;
			int					m_iOKMsg;

			ReservedMessagePopUp()
			{
				m_iOKMsg = -1;
			}
			ReservedMessagePopUp( const ReservedMessagePopUp& t ) { *this = t; }    // copy constructor
			ReservedMessagePopUp& operator=( const ReservedMessagePopUp& t )        // assign operator
			{
				m_vecTargetState = t.m_vecTargetState;
				m_Message = t.m_Message;
				m_iOKMsg = t.m_iOKMsg;

				return *this;
			}
		};


		struct TimedMessagePopUp
		{
			enum MESSAGE_BOX_TYPE
			{
				MBT_NO_BUTTON,
				MBT_OK,
				MBT_OK_CANCEL,
			};
			
			enum MESSAGE_TYPE
			{
				MT_NONE,
				MT_REQUESTING_JOIN_PARTY,					// 파티 참가 요청자가 기다리는 팝업
				MT_PARTY_LEADER_DECIDE_JOIN_REQUEST,		// 파티 요청을 받을 파티장이 수락, 최소 결정하는 팝업
				MT_PARTY_LEADER_INVITE_PARTY,				// 파티장이 파티 초대후 기다리는 팝업
				MT_DECIDE_INVITE_REQUEST,					// 파티장의 초대를 받은 유저가 수락, 취소 결정하는 팝업
				MT_PVP_INVITE,								// 대련장에서 다른 유저를 초대한 경우
				MT_PVP_DECIDE_INVITE_REQUEST,				// 대련장에서 다른 유저의 초대를 받는 경우
				MT_WANT_TO_ENTER_SECRET_STATE,				// 비밀 스테이지에 입장하고 싶은지 아닌지 묻는 팝업
#ifdef SERV_PVP_NEW_SYSTEM
				MT_PVP_START,
#endif
				MT_PARTY_READY_TO_PLAY_DUNGEON_REQUEST,		// 던전을 시작할 준비가 되었는지 묻는 팝업
				MT_DUNGEON_MATCH_START_REQUEST,				// 매칭에 의한 던전을 시작할 준비가 되었는지 묻는 팝업
				MT_DUNGEON_MATCH_REMAKING,					// 던전 매칭중 다른 유저가 취소하여 매칭을 다시 한다는 팝업

// #ifdef ADDED_RELATIONSHIP_SYSTEM								
				MT_WAIT_CHOICE_COUPLE,						// 커플 성사 대기!
				MT_AGREE_COUPLE,							// 커플 성사 성공!
// #endif // ADDED_RELATIONSHIP_SYSTEM

#ifdef SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC // 적용날짜: 2013-04-22
				MT_ENTER_GATE_OF_DARKNESS,					// 어둠의 문 입장 여부를 묻는 팝업
#endif // SERV_NEW_DEFENCE_DUNGEON_ENTER_LOGIC

#ifdef ADDED_RELATIONSHIP_SYSTEM
				MT_WAIT_SUMMON_MARRIAGED_USER,				// 배우자 소환 요청중 팝업
				MT_SELECT_SUMMON_TO_MARRIAGED_USER,			// 자신을 배우자의 위치로 소환시킬 지 묻는 팝업
#endif // ADDED_RELATIONSHIP_SYSTEM
				MT_OK_CANCEL,
			};

			// TimedMessagePopUp를 식별하기 위한 uid
			struct TimedPopupUserData
			{
				UidType		iMyUID;
				UidType		iOpponentUID;
				int			iTargetPVPChannel;
			
				TimedPopupUserData()
				{
					iMyUID = -1;
					iOpponentUID = -1;
					iTargetPVPChannel = -1;
				}

				bool operator ==( const TimedPopupUserData& uniqueID )
				{
					if( iMyUID == uniqueID.iMyUID &&
						iOpponentUID == uniqueID.iOpponentUID )		//kimhc // 2010-05-06 // iOppoentUID 와 uniqueID.iMyUID를 비교하던것을 수정
					{
						return true;
					}

					return false;
				}
			};


		public:
			TimedPopupUserData		m_UserData;			// TimedMessagePopUp을 구분하기 위한 ID로도 사용되고, 실제 필요한 userdata를 저장해두는 변수로도 사용한다.
			MESSAGE_TYPE			m_eMessageType;
			CKTDGUIDialogType			m_pDialog;
			float					m_fTimeLeft;
			wstring					m_wstrMsg;
			int						m_iTimeOutMsg;
			//void (*CBOnTimeOut)( const void* );

			
			TimedMessagePopUp()
			{
				m_eMessageType = MT_NONE;
				m_pDialog	= NULL;
				m_fTimeLeft = 0.f;
				m_wstrMsg	= L"";
				//CBOnTimeOut = NULL;
				m_iTimeOutMsg = -1;
			}

			~TimedMessagePopUp()
			{
				if( m_pDialog != NULL )
				{
					g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDialog, NULL, false );
					m_pDialog = NULL;
				}
			}

			void OnFrameMove( double fTime, float fElapsedTime );
		};

		
#if defined( SERV_HACKING_TOOL_LIST )
		class CheckWindowInfo : public KJohnThread
		{
		public:

			CheckWindowInfo( CKTDXApp* pKTDXApp, CX2InstanceData* pInstanceData
#ifdef	CHECK_KOM_FILE_ON_LOAD
                , CX2Main* pMain 
#endif	CHECK_KOM_FILE_ON_LOAD
                )
                : m_pKTDXApp( pKTDXApp )
                , m_pInstanceData( pInstanceData )
#ifdef	CHECK_KOM_FILE_ON_LOAD
				, m_pMain( NULL )
#endif	CHECK_KOM_FILE_ON_LOAD
			{
				m_bLoop = true;				
#ifdef ENUM_CHILD_WINDOW
				m_bSendedMail_Thread = false;
				m_bFindChild_Thread = false;
#endif

#ifdef DESK_THREAD_WINDOW
				m_vecDeskThreadInfo_Thread.clear();
#ifdef CHECK_ALL_WINDOW
				m_vecTopWndInfo_Thread.clear();
#endif
#endif
				m_bForceExitThread = false;

#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
                m_fGetCheckKomTime_Thread = 200.f;
                m_vecChangeCheckKom_Thread.clear();
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
			}
			virtual ~CheckWindowInfo()
			{	
				m_bForceExitThread = true;
				EndThread(9000);
			}

			void SetNullToKtdxappAndInstanceData() { m_pKTDXApp = NULL; m_pInstanceData = NULL; }
			void SetForceExitThread( const bool bForceExitThread_ ) { m_bForceExitThread = bForceExitThread_; }

			bool GetIsLoop() { return m_bLoop; }
			bool CheckProcess_Thread();
			void ClearWindowInfo_Thread() { m_vecWindowInfo_Thread.clear(); }
			void PushWindowInfo_Thread( const KHackingToolInfo& windowInfo) { m_vecWindowInfo_Thread.push_back(windowInfo); }
#ifdef ENUM_CHILD_WINDOW
			void SetChildWindow_Thread(bool bVal) { m_bFindChild_Thread = bVal; }
#endif

			virtual bool BeginThread()
			{
				m_bLoop = true;
				m_bForceExitThread = false;
				return KJohnThread::BeginThread();
			}//BeginThread()

			virtual void EndThread(DWORD dwTimeout_ = INFINITE)
			{
				m_bLoop = false;				
				KJohnThread::EndThread(dwTimeout_);
			}//EndThread()

			virtual DWORD RunThread()
			{
#if defined( _SERVICE_ )
				ELSWORD_VIRTUALIZER_START
#endif

#ifdef  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY
                g_CKTDXLog.RegisterCurrentThread();
#endif  X2OPTIMIZE_KTDXLOG_ENFORCE_THREAD_SAFETY

				while(1)
				{	
					THEMIDA_ENCODE_START

					if( m_bLoop.Verify() == false || m_bForceExitThread.Verify() == false )
						break;

					if( m_pInstanceData != NULL && m_pKTDXApp != NULL )
					{
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
                        if ( m_pInstanceData->GetChangedHackList_ThreadSafe( m_vecHackList_Thread ) == true )
                        {
							// 주의 : CheckProcess 내부에 발생하는 연산은 오래 걸린다. 
							// 멀티쓰레드 환경에서 오래 걸리는 연산은
							// 게임종료시점에서 CheckProcess 이후에 객체 참조는 위험할수도 있다.
							CheckProcess_Thread();
                        }
#else   X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
						if( m_pInstanceData->GetChangeHackList() == true )
						{
							m_pInstanceData->SetChangeHackList(false);

							// 주의 : CheckProcess 내부에 발생하는 연산은 오래 걸린다. 
							// 멀티쓰레드 환경에서 오래 걸리는 연산은
							// 게임종료시점에서 CheckProcess 이후에 객체 참조는 위험할수도 있다.
							CheckProcess_Thread();
						}
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

						if( m_bForceExitThread == true )
							break;

#ifdef	CHECK_KOM_FILE_ON_LOAD
						if(m_pMain != NULL)
						{

#ifdef SERV_KOM_FILE_CHECK_ADVANCED
							static boost::mt19937 rng;
							static boost::uniform_real<float> six(0.1f,0.9f);
							static boost::variate_generator< boost::mt19937&, boost::uniform_real<float> > rand(rng,six);
							float fRandomizeTimeCount = rand();

#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
                            m_fGetCheckKomTime_Thread = m_fGetCheckKomTime_Thread + 5.0f + fRandomizeTimeCount;
                            if ( m_fGetCheckKomTime_Thread > 200.f )
                            {
                                m_pMain->GetCheckKomFileList_Thread( m_vecChangeCheckKom_Thread, m_fGetCheckKomTime_Thread );
                            }
                            m_pMain->CompareCheckKomList_Thread( m_vecChangeCheckKom_Thread );
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
							m_pMain->SetCheckKomTime( m_pMain->GetCheckKomTime() + 5.0f + fRandomizeTimeCount );
							if( m_pMain->GetCheckKomTime() > 200.0f )
							{
								m_pMain->GetCheckKomFileList();
							}
							m_pMain->CompareCheckKomList();
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX

#else
#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
                            m_pMain->CompareCheckKomList_Thread( m_vecCheckKomFileList_Thread );
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
							m_pMain->CompareCheckKomList();
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
#endif // SERV_KOM_FILE_CHECK_ADVANCED
						}
#endif	CHECK_KOM_FILE_ON_LOAD
					
					}

					Sleep(5000);

					if( m_bForceExitThread == true )
						break;

					THEMIDA_ENCODE_END
				}

				m_bLoop = false;
#if defined( _SERVICE_ ) 				
				ELSWORD_VIRTUALIZER_END
#endif
				return 0;
			}//RunThread()



		private:

			CKTDXApp* m_pKTDXApp;
			CX2InstanceData *m_pInstanceData;
#ifdef	CHECK_KOM_FILE_ON_LOAD
			CX2Main* m_pMain;
#endif	CHECK_KOM_FILE_ON_LOAD
			std::vector<KHackingToolInfo>	m_vecWindowInfo_Thread;
#ifdef  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX
            std::vector<KHackingToolInfo>   m_vecHackList_Thread;
#endif  X2OPTIMIZE_HACKLIST_CHECK_MULTITHREAD_CRASH_BUG_FIX

			KProtectedType<bool> m_bLoop;
			KProtectedType<bool> m_bForceExitThread;
#ifdef ENUM_CHILD_WINDOW
			bool m_bSendedMail_Thread;
			bool m_bFindChild_Thread;
#endif

#if defined(DESK_THREAD_WINDOW) && defined(CHECK_THREAD_WND)
			vector<THREAD_WND_INFO> m_vecDeskThreadInfo_Thread;
#ifdef CHECK_ALL_WINDOW
			vector<THREAD_WND_INFO> m_vecTopWndInfo_Thread;
#endif
#endif //DESK_THREAD_WINDOW

#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
            float   m_fGetCheckKomTime_Thread;
            std::vector< pair<string, string> >	m_vecChangeCheckKom_Thread;
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX

		};//class TThread : public KJohnThread
#endif


#ifdef EXPAND_DEVELOPER_SCRIPT	  // 김종훈, 개발자 스크립트 확장 기능 추가
		struct DeveloperScriptSet
		{
			enum DEVELOPER_SCRIPT_TYPE
			{
				DST_GAME_EFFECT_SET = 0,		// 게임 이펙트 셋
				DST_DAMAGE_EFFECT,				// 데미지 이펙트
				DST_GAME_MAJOR_XMESH_PLAYER,	// 게임 메이저 XMesh 
				DST_END,
			};
			enum DEVELOPER_SCRIPT_REFRESH_TYPE
			{
				DSCRT_MERGE = 0,		// 머지 시켜줌 ( 우선권은 가장 늦게 불러오는 순 ), 부하가 심할 수 있다. 필요한 부분만 골라서 넣자.
				DSCRT_SWAP,				// 해당 스크립트로 바꿔치기 함
				DSCRT_END,
			};
			struct DeveloperScript
			{
				DeveloperScript () :
					m_wstrDeveloperScriptFileName ( L"" ), 
					m_eDeveloperScriptType ( DST_END ),
					m_eDeveloperScriptRefreshType ( DSCRT_MERGE )	{ }
				
				wstring m_wstrDeveloperScriptFileName;	
				DEVELOPER_SCRIPT_TYPE m_eDeveloperScriptType;
				DEVELOPER_SCRIPT_REFRESH_TYPE m_eDeveloperScriptRefreshType;
			};


		public:
			DeveloperScriptSet () { }

			void Clear() { m_vecDeveloperScriptSet.clear(); }		// 초기화
			bool OpenScriptFile( const wstring& wstrFileName )		// 스크립트 파일 불러오기
			{
				lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pDevScript", this );	

				if( true == wstrFileName.empty() )
					return false;

				return g_pKTDXApp->LoadLuaTinker( wstrFileName.c_str() );
			}
			bool AddScriptSet ( DEVELOPER_SCRIPT_TYPE eDevScriptType )	// 타입 지정 스크립트 추가
			{
				KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
				TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


				DeveloperScript cDeveloperScript;
				LUA_GET_VALUE(				luaManager,		"NAME",			cDeveloperScript.m_wstrDeveloperScriptFileName,		L""	);
				LUA_GET_VALUE_ENUM(			luaManager,		"REFRESH_TYPE",	cDeveloperScript.m_eDeveloperScriptRefreshType,		DEVELOPER_SCRIPT_REFRESH_TYPE,	DSCRT_MERGE );

				cDeveloperScript.m_eDeveloperScriptType = eDevScriptType;

				m_vecDeveloperScriptSet.push_back( cDeveloperScript );
				return true;
			}
			
			bool AddEffectSetDevFileList_LUA ()	// 이펙트셋 타입 지정 스크립트 추가
			{
				return AddScriptSet ( DST_GAME_EFFECT_SET );
			}
			
			bool AddDamageEffectDevFileList_LUA ()	// 데미지이펙트 타입 지정 스크립트 추가
			{
				return AddScriptSet ( DST_DAMAGE_EFFECT );
			}
			bool AddGameMajorXMeshPlayerDevFileList_LUA ()	// XMeshPlayer 타입 지정 스크립트 추가
			{
				return AddScriptSet ( DST_GAME_MAJOR_XMESH_PLAYER );
			}
			
			bool AddDeveloperScript_LUA ()	// 스크립트 추가 기본형
			{				
				KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
				TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING


				DeveloperScript cDeveloperScript;
				LUA_GET_VALUE(				luaManager,		"NAME",			cDeveloperScript.m_wstrDeveloperScriptFileName,		L""	);
				LUA_GET_VALUE_ENUM(			luaManager,		"SCRIPT_TYPE",	cDeveloperScript.m_eDeveloperScriptType,			DEVELOPER_SCRIPT_TYPE,	DST_END );
				LUA_GET_VALUE_ENUM(			luaManager,		"REFRESH_TYPE",	cDeveloperScript.m_eDeveloperScriptRefreshType,		DEVELOPER_SCRIPT_REFRESH_TYPE,	DSCRT_MERGE );
				

				m_vecDeveloperScriptSet.push_back( cDeveloperScript );
				return true;
			}

			vector <DeveloperScript> & GetDeveloperScriptSet () { return m_vecDeveloperScriptSet; }
		private :
			vector <DeveloperScript> m_vecDeveloperScriptSet;				
		};
#endif // EXPAND_DEVELOPER_SCRIPT	  // 김종훈, 개발자 스크립트 확장 기능 추가


	public:
		CX2Main(void);
		~CX2Main(void);

		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
		virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

//{{ robobeg : 2013-09-17
		//CX2GameOption*	GetGameOption(){ return	&m_GameOption; }
        CX2GameOption&	GetGameOption(){ return	m_GameOption; }
//}} robobeg : 2013-09-17
		//CX2UnitLoader*	GetUnitLoader(){ return	&m_UnitLoader; }
		CX2Data*		GetX2Data(){ return m_pData; }
		CKTDXStage*		GetNowState(){ return m_pNowState; }
		X2_STATE		GetNowStateID() const { return m_NowStateID; }
#ifdef X2TOOL
		void SetNowStateID(X2_STATE eState) { m_NowStateID = eState; }
#endif
		

#ifdef ADDED_MESSAGEBOX_USING_CUSTOM_LUA
	#ifdef GOOD_ELSWORD //김창한. 정훈이형이 수정하신 기능 okmsgbox에도 적용.
		#ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh		
		CKTDGUIDialogType	KTDGUIOKMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState, int iOKMsg = -1, float fTimeLeft = -1.f, wstring wstrFileName = L"",IN const D3DXVECTOR2 vSize_ = D3DXVECTOR2( 0, 0 ), wstring wstrPopupSoundFileName = L"" );	
		#else  // REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
		CKTDGUIDialogType	KTDGUIOKMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState, int iOKMsg = -1, float fTimeLeft = -1.f, wstring wstrFileName = L"",IN const D3DXVECTOR2 vSize_ = D3DXVECTOR2( 0, 0 ));	
		#endif // REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	
	#else //GOOD_ELSWORD
		CKTDGUIDialogType	KTDGUIOKMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState, int iOKMsg = -1, float fTimeLeft = -1.f, wstring wstrFileName = L"" );
	#endif //GOOD_ELSWORD
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
		CKTDGUIDialogType	KTDGUIMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState, wstring wstrFileName = L"", wstring wstrPopupSoundFileName = L"" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
		CKTDGUIDialogType	KTDGUIMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState, wstring wstrFileName = L""  );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
#ifdef GOOD_ELSWORD //JHKang
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
		CKTDGUIDialogType	KTDGUIOkAndCancelMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, int okMsg, CKTDXStage* pNowState, int cancelMsg = -1,	
			wstring wstrFileName = L"", IN const D3DXVECTOR2 vSize_ = D3DXVECTOR2( 0, 0 ), wstring wstrPopupSoundFileName = L"" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
		CKTDGUIDialogType	KTDGUIOkAndCancelMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, int okMsg, CKTDXStage* pNowState, int cancelMsg = -1,
			wstring wstrFileName = L"", IN const D3DXVECTOR2 vSize_ = D3DXVECTOR2( 0, 0 ) );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편

#else //GOOD_ELSWORD
		CKTDGUIDialogType	KTDGUIOkAndCancelMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, int okMsg, CKTDXStage* pNowState, int cancelMsg = -1, wstring wstrFileName = L"" );
#endif //GOOD_ELSWORD

#else  // ADDED_MESSAGEBOX_USING_CUSTOM_LUA
		CKTDGUIDialogType	KTDGUIOKMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState, int iOKMsg = -1, float fTimeLeft = -1.f );
		CKTDGUIDialogType	KTDGUIMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState );
		CKTDGUIDialogType	KTDGUIOkAndCancelMsgBox( D3DXVECTOR2 pos, const WCHAR* pText, int okMsg, CKTDXStage* pNowState, int cancelMsg = -1 );
#endif // ADDED_MESSAGEBOX_USING_CUSTOM_LUA

		CKTDGUIDialogType	KTDGUIOKMsgBoxPlus( D3DXVECTOR2 pos, const WCHAR* pText, CKTDXStage* pNowState, int iOKMsg = -1, float fTimeLeft = -1.f );

#ifdef SERV_SKILL_NOTE
		CKTDGUIDialogType	KTDGUIInfoBox( D3DXVECTOR2 vPos, const WCHAR* pText, CKTDXStage* pNowState, float fLifeTime = -1.f );
		void				DeleteInfoBox();
		CKTDGUIDialogType	GetInfoBox() { return m_pDlgInfoBox; }
#endif

		//{{ 허상형 : [2009/10/1] //	EditBox가 포함된 Dialog 생성 함수. iLimitLength = 입력 길이 제한, bIsByte = 길이를 byte로 할지 여부. 디폴트로 글자 갯수로 판단
		CKTDGUIDialogType	KTDGUIOkAndCancelEditBox( D3DXVECTOR2 vPos, const WCHAR* pText, int iOkMsg, CKTDXStage* pNowState, int iLimitLength = 40, bool bIsByte = false, int iCancelMsg = -1 );
#ifdef SERV_LOCAL_RANKING_SYSTEM
		CKTDGUIDialogType	KTDGUIOkAndCancelEditBox_Hide( D3DXVECTOR2 vPos, const WCHAR* pText, int iOkMsg, CKTDXStage* pNowState, int iLimitLength = 40, bool bIsByte = false, int iCancelMsg = -1 );
#endif //SERV_LOCAL_RANKING_SYSTEM
		CKTDGUIDialog*	KTDGUIOKMsgBox_LUA( D3DXVECTOR2 pos, const char* pText, CKTDXStage* pNowState );
		CKTDGUIDialog*	KTDGUIOkAndCancelMsgBox_LUA( D3DXVECTOR2 pos, const char* pText, int okMsg, CKTDXStage* pNowState );


		CKTDGUIDialogType		KTDGUIOkAndCancelEditBox2( D3DXVECTOR2 vPos, const WCHAR* pText, int iOkMsg, CKTDXStage* pNowState, int iLimitLength, bool bIsByte, int iCancelMsg );


#ifdef DISABLE_REDUDANT_PACKET_TEST
		bool			IsWaitingServerPacket( const SERVER_EVENT_ID waitACK );
#endif DISABLE_REDUDANT_PACKET_TEST

		void			AddServerPacket( SERVER_EVENT_ID waitACK, float fReamainTimeOut = 0.0f );
		bool			DeleteServerPacket( SERVER_EVENT_ID waitACK );
		void			AllDeleteServerPacket();

#ifdef SERV_GLOBAL_AUTH
		void			AddChannelServerIP( const WCHAR* pServerIP );
		void			AddChannelServerIP_LUA( const char* pServerIP, int iServerPort = 9400 );
#else //SERV_GLOBAL_AUTH
		void			AddChannelServerIP( const WCHAR* pServerIP )
		{
			if( NULL == pServerIP )
				return;

			m_vecChannelServerIP.push_back( pServerIP );
		};		

		void			AddChannelServerIP_LUA( const char* pServerIP )
		{
			if( NULL == pServerIP )
				return;

#ifdef DOMAIN_TO_IP
			struct hostent* host = NULL;
			wstring wstrChannelServerIP = L"";
			host = gethostbyname(pServerIP);

			if( host != NULL )
			{
				ConvertUtf8ToWCHAR( wstrChannelServerIP, inet_ntoa(*(struct in_addr*)host->h_addr_list[0]) );
			}
			else
			{
				ConvertUtf8ToWCHAR( wstrChannelServerIP, pServerIP );
			}			
#else
			wstring wstrChannelServerIP = L"";
			ConvertUtf8ToWCHAR( wstrChannelServerIP, pServerIP );
#endif DOMAIN_TO_IP

#ifdef	USE_CHANNEL_SERVER_IP_AT_SCRIPT
			m_DefaultChannelServerInfo.m_kServerIP = wstrChannelServerIP;
#else	USE_CHANNEL_SERVER_IP_AT_SCRIPT
			m_vecChannelServerIP.push_back( wstrChannelServerIP );
#endif	USE_CHANNEL_SERVER_IP_AT_SCRIPT
		}
#endif //SERV_GLOBAL_AUTH

		int GetPickedChannelServerIPIndex() const { return m_iPickedChannelServerIPIndex; }
		void SetPickedChannelServerIPIndex(int val) { m_iPickedChannelServerIPIndex = val; }
		int GetDefaultChannelServerIPIndex() const { return m_iDefaultChannelServerIPIndex; }
		void SetDefaultChannelServerIPIndex(int val) { m_iDefaultChannelServerIPIndex = val; }

		int GetChannelServerPort() const { return m_iChannelServerPort; }
		void SetChannelServerPort(int val) {
#ifdef	USE_CHANNEL_SERVER_IP_AT_SCRIPT
			m_DefaultChannelServerInfo.m_usMasterPort = val;
#else	USE_CHANNEL_SERVER_IP_AT_SCRIPT
			m_iChannelServerPort = val;
#endif	USE_CHANNEL_SERVER_IP_AT_SCRIPT
		}

		// 09.07.03 태완 : 사내에서 계정생성 되게
#if !defined(_SERVICE_) || defined(SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER)
		int GetGameServerPortForCreateID() const { return m_iGameServerPortForCreateID; }
		void SetGameServerPortForCreateID(int val) { m_iGameServerPortForCreateID = val; }
		void AddGameServerIPForCreateID_LUA( const char* pServerIP )
		{
			if( NULL == pServerIP )
				return;

			m_wstrGameServerIPForCreateID = L"";
			ConvertUtf8ToWCHAR( m_wstrGameServerIPForCreateID, pServerIP );
			
		}
		wstring GetGameServerIPForCreateID(){ return m_wstrGameServerIPForCreateID; }
#endif //!defined(_SERVICE_) || defined(SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER)

#ifdef SERVER_GROUP_UI_ADVANCED
		KServerGroupInfo GetPickedChannelServer() const;
#else
		const wstring& GetPickedChannelServerIP()
		{ 
			if( m_iPickedChannelServerIPIndex < 0 ||
				m_iPickedChannelServerIPIndex >= (int) m_vecChannelServerIP.size() )
			{
				m_iPickedChannelServerIPIndex = m_iDefaultChannelServerIPIndex;
			}
			
			return m_vecChannelServerIP[ m_iPickedChannelServerIPIndex ];
		}
#endif SERVER_GROUP_UI_ADVANCED

#ifdef SERV_INTEGRATION
		UidType         ExtractServerGroupID( UidType iUID_ )
		{
#ifdef EXTEND_SERVER_GROUP_MASK
			return ( iUID_ >> 52 ) & ( UidType )0x00000000000003ff;
#else
			return ( iUID_ >> 56 ) & ( UidType )0x000000000000003f;
#endif // EXTEND_SERVER_GROUP_MASK
		}
#endif SERV_INTEGRATION

		// 게임서버 관련, p2p 관련
		const WCHAR*	GetConnectedGameServerIP() const { return m_ConnectedGameServerIP.c_str(); }
		void			SetConnectedGameServerIP(const std::wstring& val) { m_ConnectedGameServerIP = val; }
		int				GetConnectedGameServerPort() const { return m_iConnectedGameServerPort; }
		void			SetConnectedGameServerPort(int val) { m_iConnectedGameServerPort = val; }

		void			SetServerUDPPort( int port ){ m_ServerUDPPort = port; }		// 게임서버 udp relay port
		const int		GetServerUDPPort(){ return m_ServerUDPPort; }

		void			SetGameP2PPort( int port ){ m_GameP2PPort = port; }
		const int		GetGameP2PPort(){ return m_GameP2PPort; }

		void			SetServerTimeOut_LUA(float fTime) { m_fServerTimeOut = fTime; } 
		float			GetServerTimeOut() { return m_fServerTimeOut; }

	
		


		const WCHAR*	GetTRServerIp() { return m_TRServerIp.c_str(); }
		void			SetTRServerIp( const wstring &wstrTRServerIp ) { m_TRServerIp = wstrTRServerIp; };

		int				GetFrameBufferNum(){ return m_FrameBufferNum; }

		void			SetAutoLoginUnitIndex(int index) { m_AutoLoginUnitIndex = index; }
		void			SetSafetyKey_LUA( const char* pKey );
		wstring			GetSafetyKey(){ return m_SafetyKey.c_str(); }

		bool				m_bAutoStart;
		int					m_AutoLoginUnitIndex;

		void			ResetFullScreenResolution(int iWidth, int iHeight);
		bool			IsValidPacket( int enumID );

		void			SetTutorialRandomLoadingBG( int iRandIndex ) { m_iRandLoadingBG = iRandIndex; }
		int				GetTutorialRandomLoadingBG() { return m_iRandLoadingBG; }
		const bool		GetIsPlayingTutorial() { return m_bIsPlayingTutorial; }
		void			SetIsPlayingTutorial( bool bPlaying ) { m_bIsPlayingTutorial = bPlaying; }
		const bool		GetIsExitingTutorial() { return m_bIsExitingTutorial; }
		void			SetIsExitingTutorial( bool bExiting ) { m_bIsExitingTutorial = bExiting; }
		X2_STATE		GetStateIDBeforeTutorial() { return m_StateIDBeforeTutorial; }
		void			SetStateIDBeforeTutorial( X2_STATE stateID ) { m_StateIDBeforeTutorial = stateID; }

		void			SetPlayFieldDungeon( bool bPlay) { m_bIsPlayFieldDungeon = bPlay; }
		const bool		GetPlayFieldDungeon() { return m_bIsPlayFieldDungeon; }
		void			SetExitFieldDungeon( bool bPlay) { m_bIsExitFieldDungeon = bPlay; }
		const bool		GetExitFieldDungeon() { return m_bIsExitFieldDungeon; }


		CX2StateAutoChanger& GetStateAutoChanger() { return m_AutoStateChanger; }

		CX2StringFilter* GetStringFilter() { return m_pStringFilter; }
		
		bool			InstallFont( const char* pValueName, const char* pFontFileName );
		X2_PUBLISHER	GetPublisher(){ return m_X2Publisher; }
		bool			GetManualLogin(){ return m_bManualLogin; }

		void			SetClientVersion_LUA( char* pVersion );
		const WCHAR*	GetClientVersion(){ return m_ClientVersion.c_str(); }
		void			SetSubClientVersion_LUA( char* pVersion );
		const WCHAR*	GetSubClientVersion_LUA(){ return m_SubClientVersion.c_str(); }

#ifdef _NEXON_KR_
		void			SetNMVirtualKey( const NMGameCode uGameCode_, const UINT32 uVirtualIDCode_ )
		{
			m_NMKeyOfMineToBeDeleted.uGameCode = uGameCode_;
			m_NMKeyOfMineToBeDeleted.uVirtualIDCode = uVirtualIDCode_;
		}

		NMVirtualKey	GetNMVirtualKey() const { return m_NMKeyOfMineToBeDeleted; }

		void			SetNexonLogin( bool bLogin ){ m_bNexonLogin = bLogin; }
		bool			GetNexonLogin(){ return m_bNexonLogin; }

		void			SetNexonLoginMessenger( bool bLogin ){ m_bNexonLoginMessenger = bLogin; }
		bool			GetNexonLoginMessenger(){ return m_bNexonLoginMessenger; }

		void			SetNexonVirtualLogin( bool bVirtualLogin ){ m_bNexonVirtualLogin = bVirtualLogin; }
		bool			GetNexonVirtualLogin(){ return m_bNexonVirtualLogin; }
#endif _NEXON_KR_

		void			ChangeKoreanAndEnglishKey( bool bKorean );

//{{ kimhc // 2010.3.12 //	채팅창 개편
#ifdef	CHAT_WINDOW_IMPROV
		bool			GetNativeAndEnglisKeyState();//현재 한영 모드 알아내기 // TRUE면 한글
#endif	CHAT_WINDOW_IMPROV	
//}} kimhc // 2010.3.12 //	채팅창 개편

//{{ kimhc // 2010.9.14 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트
#ifdef	AVATAR_IMAGE_TEST
		bool			ScreenCaptureAndSendToWeb( IN double fTime, IN float fElapsedTime );
		bool			ScreenCaptureForWebAvatar( IN double fTime, IN float fElapsedTime, OUT wstring& wstrFileName );	// 웹에서 쓰일 아바타용 파일을 갭쳐
		bool			SendFileCapturedToWeb( IN const wstring& wstrFileName );	// 캡쳐한 파일을 웹으로 전송함
#endif	AVATAR_IMAGE_TEST
//}} kimhc // 2010.9.14 // 홈페이지 개편에서 아바타 이미지 표현으로 사용될 캐릭터 이미지 캡쳐 기능 테스트

		void			ResetCrashReport( const WCHAR* wszAddMsg );

		CKTDGFontManager::CUKFont* GetFontForDebuf() { return m_pFontForDebug; }

		UINT			GetConnectedChannelID() { return m_iConnectChannelID; }
		void			SetConnectedChannelID( UINT channelID ) { m_iConnectChannelID = channelID; }

		std::vector<KPVPChannelInfo>& GetChannelList() { return m_vecChannel; } 
		void			SetChannelList( std::vector<KPVPChannelInfo>& vecChannel ) { m_vecChannel = vecChannel; }



		// note: 대전채널에 입장한 경우에만 사용해야 함~
		KPVPChannelInfo::PVP_CHANNEL_CLASS GetPVPChannelClass( const UINT iChannelID )
		{
			for( UINT i=0; i<m_vecChannel.size(); i++ )
			{
				KPVPChannelInfo& channelInfo = m_vecChannel[i];
				if( channelInfo.m_iChannelID == iChannelID )
				{
					return (KPVPChannelInfo::PVP_CHANNEL_CLASS) channelInfo.m_cPVPChannelClass;
				}
			}

			return KPVPChannelInfo::PCC_NONE;
		}



		bool			SendQuitMsgToServer();
		int				GetServerUserCount() const		{ return m_iServerUserCount; }
		void			SetServerUserCount( int val )	{ m_iServerUserCount = val; }
		map<int, int>&	GetMapServerPVPRoomCount()		{ return m_mapServerPVPRoomCount; }
		void			SetMapServerPVPRoomCount(map<int, int> val) { m_mapServerPVPRoomCount = val; }
		//{{ 2009. 4. 1  최육사		파티정보
		int				GetCurPartyCount() const		{ return m_iCurPartyCount; }
		void			SetCurPartyCount(int val)		{ m_iCurPartyCount = val; }
		int				GetPlayGamePartyCount() const	{ return m_iPlayGamePartyCount; }
		void			SetPlayGamePartyCount(int val)	{ m_iPlayGamePartyCount = val; }
		//}}

#ifdef SERV_CHANNELING_USER_MANAGER
		std::map<int,int>		GetServerChannelingUserCount() const		{ return m_mapChannelingUserList; }
		void					SetServerChannelingUserCount( std::map<int,int> mapChannelingUserList )	{ m_mapChannelingUserList = mapChannelingUserList; }
#endif SERV_CHANNELING_USER_MANAGER


		bool			DeleteFile_LUA( const char* pFileName );
#ifdef CLIENT_USE_NATION_FLAG
		void			SetNationFlag_LUA( DWORD dwFlag ) { m_dwNationFlag = dwFlag; }
		DWORD			GetNationFlag() { return m_dwNationFlag; }
#endif CLIENT_USE_NATION_FLAG

		CX2GameEdit*	GetGameEdit() { return m_pGameEdit; }
		CX2TextManager*	GetTextManager() { return m_pTextManager; }
		CX2NoticeManager* GetNoticeManager() { return m_pNoticeManager; }

		void			SetQuitType( int quitType ) { m_QuitType = quitType; }
		void			SetBetweenCharToCharSize( HDC hdc, int sizeValue = 0 ) { SetTextCharacterExtra( hdc, sizeValue ); }

		void			BuyItemSuper_LUA( int itemID, int iQuantity = 1, short sPeriod = 0 );

		void			SetAuthLevel_LUA( const CHAR* userID, CX2User::X2_USER_AUTH_LEVEL level );
		void			GetAuthUserList_LUA( CX2User::X2_USER_AUTH_LEVEL level );

		void			BuySuperEqipLevel_LUA( int level );
		//{{ 2008. 1. 2  최육사  근성도 치트
		void			ChangeSpirit( int iChangeSpirit );
		//}}
		//{{ 2008. 1. 25  최육사  VP 치트
#ifdef SERV_PVP_NEW_SYSTEM
		void			ChangeVSPoint( int iRating, int iRPoint, int iAPoint );
#else
		void			ChangeVSPoint( int iChangeVSPoint, int iChangeVSPointMax );
#endif

		//}}
		//{{ 2008. 2. 18  최육사  유닛 클래스 치트
		void			ChangeUnitClass( char cUnitClass );
		void			ModifyChangeUnitClass( const char* UnitClassName );
		//}}
		//{{ 2008. 10. 24  최육사	타이틀 치트
		void			GetTitleCheat( int iTitleID, int iPeriod = 0 );
		//}}

		void			ChangeCashSkillPointDate( const int iYear_, const int iMonth_, const int iDay_, const int iHour_, const int iMinute_ );

		void 			InitMyUnitSkillTree();
		void 			SetMyUnitSP( int iSP );

#ifdef SERV_ADMIN_CHEAT_GET_ALL_SKILL 
		void			FullMyUnitSkillTree();
#endif //SERV_ADMIN_CHEAT_GET_ALL_SKILL 

		//{{ 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력
#ifdef PRINT_INGAMEINFO_TO_EXCEL
		bool			IsInGameInfoToExcel() { return m_bInGameInfoToExcel; };
		void			SetInGameInfoToExcel(bool bInGameInfoToExcel){ m_bInGameInfoToExcel = bInGameInfoToExcel; };
#endif PRINT_INGAMEINFO_TO_EXCEL

#ifdef UPGRADE_SKILL_SYSTEM_2013 // 김태환 - 스킬 시스템 변경
		void			FullMyUnitSkillTreeByLevel();
		void			SetMyUnitSkillLevel( const int iSkillID, const int iSkillLevel );
#endif // UPGRADE_SKILL_SYSTEM_2013


		void 			SendStatisticsLocLog();
		void 			SendStatisticsDBLog();


		void			AddDungeonRewardItem( std::vector<KItemInfo>& vecDungeonRewardItem );
		const std::vector<KItemInfo>& GetDungeonRewardItem() { return m_vecDungeonRewardItem; }
		void			ResetDungeonRewardItem() 
		{ 
			m_vecDungeonRewardItem.resize(0);
		}

		bool			GetCenterServerDie() { return m_bCenterServerDie; }
		void			SetCenterServerDie(bool bCheck) { m_bCenterServerDie = bCheck;  }

		void			Handler_EGS_CHECK_SUM_REQ( bool bFileCheck = false );
		//{{김준환 서버시간 받아오기
#ifdef	SERV_SERVER_TIME_GET
		bool			Handler_ECH_GET_SERVER_TIME_REQ();
		bool			Handler_ECH_GET_SERVER_TIME_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif  SERV_SERVER_TIME_GET

		int				GetTrainingRewardED() const { return m_iTrainingRewardED; }
		void			SetTrainingRewardED(int val) { m_iTrainingRewardED = val; }
		int				GetTrainingRewardEXP() const { return m_iTrainingRewardEXP; }
		void			SetTrainingRewardEXP(int val) { m_iTrainingRewardEXP = val; }
		bool			GetTrainingSuccess() const { return m_bTrainingGameSuccess; }
		void			SetTrainingSuccess(bool val) { m_bTrainingGameSuccess = val; }

#ifdef COUPON_SYSTEM
		CX2UICouponBox& GetUICouponBox()  { return m_UICouponBox; }
#else	// COUPON_SYSTEM
		CX2CouponBox*	GetCouponBox() const { return m_pCouponBox; }
#endif // COUPON_SYSTEM

		
		CX2TutorSystem* GetTutorSystem() const 
		{
#ifdef SERV_NO_DISCIPLE
			return NULL;
#else SERV_NO_DISCIPLE
			return m_pTutorSystem;
#endif SERV_NO_DISCIPLE
		}

#ifdef POSTBOX
        CX2PostBox*     GetPostBox() const { return m_pPostBox; }
#endif
#ifdef SERV_SECOND_SECURITY
        CX2SecurityPad*	GetSecurityPad() const { return m_pSecurityPad; }
#endif SERV_SECOND_SECURITY
		wstring			GetEDString( int edToChange );
		wstring			GetEDString( const WCHAR* pEDToChange );
		int				GetEDFromString( const WCHAR* wszED );
#ifdef PRICE_COLOR
		D3DXCOLOR		GetPriceColor( __int64 iPrice );
#endif	//	PRICE_COLOR



		bool 			GetLastStageWasTrainingGame() const { return m_bLastStageWasTrainingGame; }
		void 			SetLastStageWasTrainingGame(bool val) { m_bLastStageWasTrainingGame = val; }
		int				GetNowTrainingID() const { return m_iNowTrainingID; }
		void			SetNowTrainingID(int val) { m_iNowTrainingID = val; }
		bool 			GetAutoStartNextTraining() const { return m_bAutoStartNextTraining; }
		void 			SetAutoStartNextTraining(bool val) { m_bAutoStartNextTraining = val; }

		bool 			GetIsPlayingFreeTraining() const { return m_bIsPlayingFreeTraining; }
		void 			SetIsPlayingFreeTraining(bool val) { m_bIsPlayingFreeTraining = val; }


		CX2PVPEmblem*	GetPVPEmblem() { return m_pX2PVPEmblem; }
		CX2InformerManager* GetInformerManager() { return m_pInformerManager; }
		CX2MemoryHolder*	GetMemoryHolder() { return m_pMemoryHolder; }

		void			IntentionalCrash();	// 고의로 crash 내는 함수
		void			SetFreeCamera( bool bEnable );
		bool			GetFreeCamera() const ;
#ifdef KEYFRAME_CAMERA
		void			SetWorldCameraEdit( bool bEnable);
		void			PlayWorldCamera( int iWorldCameraID );
		void			EndWorldCamera();
		void			SetProjectionFov( float fFov );
#endif KEYFRAME_CAMERA
		D3DXVECTOR2		GetWindowPos( D3DXVECTOR2 windowSize, D3DXVECTOR2 wantMousePoint );
		void			OpenURL( WCHAR* pURL );
#ifdef REFORM_ENTRY_POINT		// 13-11-11, kimjh 진입 구조 개편
		void			CreateStateChangeDLG( const WCHAR* szExplanation, wstring wstrCustomLuaFileName = L"", wstring wstrPlaySoundFileName = L"" );
#else	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
		void			CreateStateChangeDLG( const WCHAR* szExplanation );
#endif	// REFORM_ENTRY_POINT	// 13-11-11, kimjh 진입 구조 개편
		void			DeleteStateChangeDLG();

		void			DeleteOtherFile( WCHAR* pExtention, vector<wstring>& fileList );

		void			SetShowUnitViewerUI( bool bShow ) { CX2UnitViewerUI::SetShowUnitViewerUI( bShow ); }

		wstring			GetStrByLienBreak( const WCHAR* pOrgStr, int width, int fontIndex );
#ifdef FIX_TOOLTIP
		wstring			GetStrByLineBreakColor( const WCHAR* pOrgStr, int width, int fontIndex );
#endif FIX_TOOLTIP

		void			SetSquareMaxUserNum( int userNum ) { m_SquareMaxUserNum = userNum; }
		int				GetSquareMaxUserNum() { return m_SquareMaxUserNum; }

		KBrowserWrapper* GetBrowserWrapper() { return &m_BrowserWrapper; }
#ifdef CJ_ID_WEB_BILLING
		KNMBrowserWrapper* GetNMBrowserWrapper() { return &m_NMBrowserWrapper; }
#endif //CJ_ID_WEB_BILLING

		CX2LVUpEventMgr* GetLVUpEventMgr() { return m_pLVUpEventMgr; }

		//void			ResetLuaBinder( KLuabinder* pKLuabinder );

		void			AddMemoryInfo( stringstream& strstm );

		void			AddSystemInfo(stringstream& strstm);
		void			AddDLLInfo(stringstream& strstm);
		void			AddProcessInfo(stringstream& strstm, bool bThreadInfo = true);
		void 			UpdateProcessList();
		void 			SetMailNameToFindHack( const WCHAR* wszMailName ) { m_MailNameToFindHack = wszMailName; }
#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
        void            SetUserIdToFindHack_ThreadSafe( const std::string& strUserId )
        {
            CSLock      lock(m_csUserIdToFindHack);
            m_strUserIdToFindHack_CS = strUserId;
        }
        void            GetUserIdToFindHackIfNotEmpty_ThreadSafe( std::string& strID )
        {
            CSLock      lock(m_csUserIdToFindHack);
            if ( m_strUserIdToFindHack_CS.empty() == false )
                strID = m_strUserIdToFindHack_CS;
        }
        bool            IsSameUserIdToFindHack_ThreadSafe( const std::string& strID )
        {
            CSLock      lock(m_csUserIdToFindHack);
            return strID.compare( m_strUserIdToFindHack_CS ) == 0;
        }
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		void			SetUserIdToFindHack( const WCHAR* wszUserId ) { m_strUserIdToFindHack = wszUserId; }
		const std::wstring&	GetUserIdToFindHack() { return m_strUserIdToFindHack; }
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		void			SetHackingUser( bool b ) { m_bHackingUser = b; }
		void 			SendHackMail();
		void			SendHackMail_DamageHistory(const char *strComm);
		void			SendHackMail_DLLInjection(const char *strComm);
#ifdef USER_MONITORING_MAIL
		void			SendUserMonitoringMail(const char *strComm);
#endif

#ifdef SERV_PROTECT_AUTO_HACK
		void			SendHackMail_HackUserNot(const char *strComm);
#endif

#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
#ifdef HACK_USER_ATTACH_FILE		
		void			SendHackMail_AttachFile(const char *strComm, const string& userId);
#endif
#ifdef SERACH_FOLDER_FILE
		void			SendHackMail_FileList(const char *strComm, const string& userId);
#endif
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
#ifdef HACK_USER_ATTACH_FILE		
		void			SendHackMail_AttachFile(const char *strComm, wstring userId);
#endif
#ifdef SERACH_FOLDER_FILE
		void			SendHackMail_FileList(const char *strComm, wstring userId);
#endif
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		void			SendHackMail_VERIFYVALUE(const char *strComm);
		void			SendHackMailGameStart(int iVal, bool bVal = false);

#ifdef SERV_ED_MONITORING_IN_GAME
		void			SendMail_MoneyMonitoring(const char *strComm);
#endif

#ifdef SERV_CLIENT_PUBLIC_IP
		void			SetPublicIp(wstring wstrPublicIp) { m_wstrPublicIp = wstrPublicIp; }
#endif

		void			SendCheckMail( const char* pRecipient_, const char* pSubject_, const char* pMessageBody_ );

		bool			GetPresentArrival( CX2StateMenu::PresentArrival& presentArrival );
		void			PushPresentArrival( KEGS_PRESENT_MESSAGE_TO_RECEIVER_NOT& kEvent );


		CX2PartyUI*		ResetPartyUI()
		{
			DeletePartyUI();
			m_pPartyUI = new CX2PartyUI((CKTDXStage*) GetNowState());
			return m_pPartyUI;
		}
		CX2PartyUI*		GetPartyUI() 
		{ 
			if( NULL == m_pPartyUI )
			{
				ResetPartyUI();
			}

			return m_pPartyUI; 
		}
		void DeletePartyUI() { SAFE_DELETE( m_pPartyUI ); }


		void ClearReservedMessagePopup() 
		{
			m_vecReservedMessagePopUp.resize(0);
		}
		void AddReservedMessagePopup( ReservedMessagePopUp& reservedMsg )
		{
			m_vecReservedMessagePopUp.push_back( reservedMsg );
		}
		void PopupReservedMessage();


		void ClearTimedMessagePopUp() { m_listTimedMessagePopUp.clear(); }
		TimedMessagePopUp* GetTimedMessagePopup( CKTDGUIDialogType pDialog );

		//void ClearTimedMessagePopup();
		void RemoveTimedMessagePopup( TimedMessagePopUp::MESSAGE_TYPE eMessageType, const TimedMessagePopUp::TimedPopupUserData& userData );
		void AddTimedMessagePopup( TimedMessagePopUp::MESSAGE_TYPE eMessageType, 
			TimedMessagePopUp::MESSAGE_BOX_TYPE eBoxType, 
			const TimedMessagePopUp::TimedPopupUserData& userData, const float fTime,
			const wstring& wstrMsg, CKTDXStage* pStage, int okMsg, int cancelMsg, int timeOutMsg = -1 );
		

		bool IsThereTimedMessagePopup( const TimedMessagePopUp::MESSAGE_TYPE eMessageType )
		{
			BOOST_TEST_FOREACH( const TimedMessagePopUp&, timedMessage, m_listTimedMessagePopUp )
			{
				if( eMessageType == timedMessage.m_eMessageType )
				{
					return true;
				}
			}

			return false;
		}

		

#ifdef LOADING_ANIMATION_TEST
		CX2LoadingAnimation* GetLoadingAnimation() { return m_pLoadingAnimation; }
#endif LOADING_ANIMATION_TEST


#ifdef CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		bool GetReceived_EGS_KNPROTECT_AUTH_CHECK_NOT() const { return m_bReceived_EGS_KNPROTECT_AUTH_CHECK_NOT; }
		void SetReceived_EGS_KNPROTECT_AUTH_CHECK_NOT(bool val) { m_bReceived_EGS_KNPROTECT_AUTH_CHECK_NOT = val; }
#endif CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT


#ifdef SHOOTING_TEST
		int GetShootingLevel() const { return m_iShootingLevel; }
		void SetShootingLevel(int val) { m_iShootingLevel = val; }
#endif SHOOTING_TEST
		
		bool			KeyProcess();

		static int LineBreak( wstring& wstrText, const CKTDGFontManager::CUKFont* pFont, const int iTextWidth, const wstring& wstrTextColor, const bool bEnableColor, bool bCutRedundantEmptyLine = true );
		static bool IsInvalidNickName( const wstring& wstrNickName, wstring& wstrErrorMessage );



		void AddReservedServerPacket( SERVER_EVENT_ID eServerEventID, void* pEvent )
		{
			ReservedServerPacket reservedServerPacket;
			reservedServerPacket.m_eServerEventID = eServerEventID;
			reservedServerPacket.m_pEvent = pEvent;

			m_vecReservedServerPacket.push_back( reservedServerPacket );
		}

		//bool PopReservedServerPacket( ReservedServerPacket& reservedServerPacket )
		//{
		//	if( false == m_vecReservedServerPacket.empty() )
		//	{
		//		reservedServerPacket = m_vecReservedServerPacket[0];
		//		m_vecReservedServerPacket.erase( m_vecReservedServerPacket.begin() );
		//		return true;
		//	}

		//	return false;
		//}

		//bool PeepReservedServerPacket( ReservedServerPacket& reservedServerPacket )
		//{
		//	if( false == m_vecReservedServerPacket.empty() )
		//	{
		//		reservedServerPacket = m_vecReservedServerPacket[0];
		//		return true;
		//	}
		//	
		//	return false;
		//}


		bool IsMyAuthLevelHigherThan( CX2User::X2_USER_AUTH_LEVEL eAuthLevel )
		{
			if( false == GetEnabledDeveloperCheatCode() )
				return false;

			if( NULL == m_pData )
				return false;

			if( NULL == m_pData->GetMyUser() )
				return false;

			if( m_pData->GetMyUser()->GetAuthLevel() < eAuthLevel )
				return false;

			return true;
		}

		bool GetEnabledDeveloperCheatCode() const { return m_bEnabledDeveloperCheatCode; }
		void SetEnabledDeveloperCheatCode(const bool val) { m_bEnabledDeveloperCheatCode = val; }


		//{{ 2009. 7. 26  최육사	party fever cheat
		void IncreasePartyFeverPoint( int iCount );
		//}}

		//{{ 2009. 8. 5  최육사		체험ID치트
		void SetGuestUserMode( bool bVal );
		//}}

		//{{ 허상형 : [2010/3/12/] //	마을 이동 워프 치트
		void WarpByCommand( int iPosition );
		//}} 허상형 : [2010/3/12/] //	
		
		//{{ 오현빈 // 2012-08-22 // 필드 이동 워프 치트
#ifdef BATTLE_FIELD_WARP_EDIT
		void FieldWarpByCommand( int iPosition ) ;
#endif //BATTLE_FIELD_WARP_EDIT
		//}} 오현빈 // 2012-08-22 // 필드 이동 워프 치트

#ifdef CLIENT_COUNTRY_TWHK
		//** 09/08.04 태완 : 대만 접속 임시코드
		wstring GetArgvID(){ return m_wstrID; }
		wstring GetArgvPass(){ return m_wstrPass; }
#endif CLIENT_COUNTRY_TWHK

#ifdef MAKE_SCREENSHOT
		void MakeHackScreenShotTemp( const char *pFileName );
#endif
		void MakeHackScreenShot( const char *pFileName );


#if defined( SERV_HACKING_TOOL_LIST )
		CheckWindowInfo *GetCheckWindowInfo() { return m_threadCheckWindowInfo; }
#endif

#ifdef DLL_MANAGER
		void			DllManagerReport();
#endif

//{{ megagame / 박교현 / 2010.05.12 / 시스템 정보
#ifdef SYS_INFO
		CX2SystemInfo*	GetSystemInfo() const	{ return m_pSystemInfo; }
#endif SYS_INFO
//}} megagame / 박교현 / 2010.05.12 / 시스템 정보

#ifdef BROKEN_CODE
		void BrokenCode();
#endif

		//{{ JHKang // 강정훈 // 2010/10/06
#ifdef ON_CHAT_REMAIN_FOCUS
		bool IsPopUpOnChat(void) { return m_bIsPopUpOnChat; }
		void SetPopUpOnChat( bool bSet ) { m_bIsPopUpOnChat = bSet; }
#endif ON_CHAT_REMAIN_FOCUS
		//}} JHKang // 강정훈 // 2010/10/06

#ifdef MACHINE_ID
		std::string GetMachineId();
#endif
#ifdef IS_PCROOM
		bool IsPcRoom();
#endif

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void    SetUDPMode_LUA( const char* pszGameType, const char* pszUDPMode );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
        void    SetUDPMode( CX2Game::GAME_TYPE eGameType, CKTDNUDP::EForceConnectMode eUDPMode );
        CKTDNUDP::EForceConnectMode GetUDPMode( CX2Game::GAME_TYPE eGameType );
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK



#ifdef	CHECK_KOM_FILE_ON_LOAD
	public:
		void ProcessSession();
		bool CreateSession();
		bool OpenSession( const char* pSessionName_ ) { return m_pSession->OpenSession( pSessionName_ ); }
		void CloseSession() { m_pSession->CloseSession(); }
		bool ConnectAndOpen( const char* szLocationPatchPathData_ );
#ifdef CHINA_PATH_SERVER_CONNECT
		string GetCheckKomLocation( string strSrvList );
#endif CHINA_PATH_SERVER_CONNECT
		bool GetCheckInfoAndDisconnect();
#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
        void CompareCheckKomList_Thread( std::vector< std::pair<string, string> >& vecChangeCheckKom );
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		void CompareCheckKomList();
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		bool CompareCheckKom( const pair<string, string>& pairCheckSum_ ) const;
		void Disconnect() { m_pSession->Disconnect(); }
		void DestroySession() { SAFE_DELETE( m_pSession ) }
		//void ClearCheckKomList() { m_vecCheckKom.clear(); }
#ifdef SERV_KOM_FILE_CHECK_ADVANCED

#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
        void GetCheckKomFileList_Thread( std::vector< std::pair<string, string> >& vecChangeCheckKom, float& fGetCheckKomTime );
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		inline void SetCheckKomTime( float GetTime ) { m_fGetCheckKomTime = GetTime; }
		inline float GetCheckKomTime() { return m_fGetCheckKomTime; }
		void GetCheckKomFileList();														// 변조 검사가 끝난 Kom 정보는 m_vecChangeCheckKom에서 제거되므로 모두 제거되면 다시 추가해주는 함수
		void SetChangeCheckKom( std::vector< std::pair<string, string> > vecChangeCheckKom );
		void GetChangeCheckKom( std::vector< std::pair<string, string> >& vecChangeCheckKom );
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX

		void Handler_EGS_KOM_FILE_CHECK_LOG_REQ( const std::wstring wstrInvalidKomName );
		bool Handler_EGS_KOM_FILE_CHECK_LOG_ACK( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
#endif // SERV_KOM_FILE_CHECK_ADVANCED
#endif	CHECK_KOM_FILE_ON_LOAD

		
		//{{ 2011.9.16	이준호  반디캠 동영상 캡쳐 지원
#ifdef BANDICAM_RECORDING
		bool					Bandi_StartVideoCapture();
		void					Bandi_Pause_Restart_VidieCapture(bool bPause);	// bPause : true이면 일시 정지, false 일시 정지 해제
		void					Bandi_EndVidioCapture();	
		void					Bandi_EnoughDiskSpace(float fElapsedTime);
		bool					IsCapturing()
		{ 
			if( m_BandiCaptureLibrary.IsCapturing() == TRUE )
				return true;
			return false;
		};

#endif BANDICAM_RECORDING
		//}}

		//{{ 2011.9.16	이준호  반디캠 스크린샷 캡쳐 지원
#ifdef BANDICAM_CAPTURE
		bool					Bandi_ScreenCapture();
		float					m_fCaptureDelay;
		CBandiCaptureLibrary	m_BandiCaptureLibrary;
#endif BANDICAM_CAPTURE
		//}}

#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
		void			SetHSStatusTime(float fVal) { m_fCheckStatusTime = fVal; }
#endif
#ifdef SERV_HACKING_TOOL_LIST
		void			SetHackToolTime(float fVal) { m_fCheckHackToolTime = fVal; }
#endif

#ifdef ADD_COLLECT_CLIENT_INFO
		CX2CollectHackInfo*		m_pCollectHackInfo;

		bool SendHackInfo1( unsigned short usType, const char *strHackInfo, bool bExit, bool bOnlyOnce = false );
		bool SendHackInfo2( unsigned short usType, const char *strHackInfo, bool bExit, bool bOnlyOnce = false );
		bool SendHackInfo3( unsigned short usType, const char *strHackInfo, bool bExit, bool bOnlyOnce = false );
		bool SendHackInfo4( unsigned short usType, const char *strHackInfo, bool bExit, bool bOnlyOnce = false );
		bool SendHackInfo5( unsigned short usType, const char *strHackInfo, bool bExit, bool bOnlyOnce = false );

		void CreateCollectHackInfo()
		{
			if( m_pCollectHackInfo == NULL )
				m_pCollectHackInfo = new CX2CollectHackInfo();
			if( m_pCollectHackInfo != NULL && m_pCollectHackInfo->GetThreadHandle() == NULL )
				m_pCollectHackInfo->BeginThread();
		}
		CX2CollectHackInfo*	GetCollectHackInfo()	{ return m_pCollectHackInfo; }

		bool GetSendHackPacket() 
		{ 
			if( m_pCollectHackInfo != NULL )
				return m_pCollectHackInfo->m_bSendPacket; 
			return false;
		}
		void SetSendHackPacket(bool bVal) 
		{ 
			if( m_pCollectHackInfo != NULL )
				m_pCollectHackInfo->m_bSendPacket = bVal; 
		}
		
		void initSendHackInfoIndex() { m_iSendHackInfo = 0; }
		int GetSendHackInfoIndex() { return m_iSendHackInfo; }
		int m_iSendHackInfo;

#endif

#ifdef CHECK_PLAY_TIME_INFORMATION
		void CreatePlayTimeInformation();														//플레이 시간 안내창 생성
		void CreateShutDownInformation();							//셧다운 안내창 생성

		void FadePlayTimeInformation( float fShowTime );	//플레이 시간 안내창 Fade 효과
		void DeletePlayTimeInformation();					//플레이 시간 안내창 삭제

		void FadeShutDownInformation( float fShowTime );	//셧다운 안내창 Fade 효과
		void DeleteShutDownInformation();					//셧다운 안내창 삭제

		void SetShutDownString( wstring wstrShutDownString ) { m_wstrShutDownString = wstrShutDownString; }		//셧다운 안내 문구 설정
		void SetShutDownInformation( bool bShutDownInfo ) { m_bProcessShutDownInformation = bShutDownInfo; }	//셧다운 생성 설정
#endif CHECK_PLAY_TIME_INFORMATION
		
		CX2KeyPad*	GetKeyPad() const { return m_pKeyPad; }
		bool GetSurveyUser() const { return m_bSurveyUser; }
		void SetSurveyUser(bool val) { m_bSurveyUser = val; }


#ifdef CHECK_INHERIT_STATE_MENU
		//CX2StateMenu를 상속받지 않는 스테이트 체크 
		inline bool IsInheritStateMenu()
		{
			switch( m_NowStateID )
			{
			case XS_START_UP:
			case XS_LOGIN:
			case XS_JOIN:
			case XS_LOADING:
			case XS_SERVER_SELECT:
			case XS_CREATE_UNIT:
			case XS_PVP_GAME:
			case XS_PVP_RESULT:
			case XS_DUNGEON_RESULT:
				return false;
				break;
			default:
				return true;
				break;
			}
		}
#endif //CHECK_INHERIT_STATE_MENU


#ifdef NEXON_QA_CHEAT_REQ
		void SetNoCoolTime( bool bNoCoolTime_ ) { m_bNoCoolTime = bNoCoolTime_; }
		bool GetNoCoolTime() const { return m_bNoCoolTime; }
		void InventoryClear( int iSelect );
#endif //NEXON_QA_CHEAT_REQ

#ifdef SERV_CLIENT_DIRECT_CONNECT_AUTH_PCBANG
		bool CheckPcBangModule();
		bool IsStartPcBang();
#endif //CLIENT_DIRECT_CONNECT_AUTH_PCBANG

#ifdef FESTIVAL_UI
		const CKTDGParticleSystem::CParticleEventSequenceHandle GetFestivalSeq(){ return m_SeqFestivalUI; }
		void SetFestivalSeq( const CKTDGParticleSystem::CParticleEventSequenceHandle Seq_ ){ m_SeqFestivalUI = Seq_; }
#endif //FESTIVAL_UI

#ifdef SEND_HACKING_PROCESS_CHECK_RESULT 
		void Send_EGS_HACKING_USER_PROCESS_NAME_RECORD_NOT( char cHackInfoFlag_, const wstring& wstrHackingName );
#endif // SEND_HACKING_PROCESS_CHECK_RESULT

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터

		bool GetIsJumpingCharacter() const { return m_bIsJumpingCharacter; }	// 점핑 캐릭터 해당 캐릭터인가?
		void SetIsJumpingCharacter(bool val) { m_bIsJumpingCharacter = val; }	// 점핑 캐릭터에 해당한다면 설정

#endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터		



#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT
		bool GetIsValideCheckKomScript() const { return m_bIsValideCheckKomScript; }
		void SetIsValideCheckKomScript(bool val) { m_bIsValideCheckKomScript = val; }
#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT		

#ifdef EXPAND_DEVELOPER_SCRIPT	  // 김종훈, 개발자 스크립트 확장 기능 추가
		bool ResetDeveloperScriptSet ( const WCHAR* pFileName );	// DevScriptTable.lua 파싱
		bool ResetDeveloperScriptSet_GameEffectSet ( DeveloperScriptSet::DeveloperScript devScript );
		bool ResetDeveloperScriptSet_DamageEffect ( DeveloperScriptSet::DeveloperScript devScript );
		bool ResetDeveloperScriptSet_GameMajorXMeshPlayer ( DeveloperScriptSet::DeveloperScript devScript );
#endif // EXPAND_DEVELOPER_SCRIPT  // 김종훈, 개발자 스크립트 확장 기능 추가

#ifdef BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
		// Wstring 도 GET_REPLACED_STRING 사용 할 수 있도록 추가
		// 스킬 설명 확장이 필요한 경우의 예외 처리, 에너지 스퍼트의 @1 ( 현재 레벨 표기 ) 때문에 추가				
		wstring GetReplacedWstring( wstring & wstrModify, char* szParamTypeList, ... );
#endif // BALANCE_PATCH_20131107					// 김종훈 / 13-10-16, 2013년 후반기 밸런스 개편
	private:
		void			RegisterLuabind();
        bool			OpenScriptFile( const WCHAR* pFileName );
		bool			StateChange( int stateID, int iDetailStateID );
		void			PostStateChange();
		void			ReservedServerEventProc();



		bool			DeleteFile( const WCHAR* pFileName );	
		
		void			StateInLog();
		
		
		void			DeleteOtherFile( WCHAR* pExtention, vector<wstring>& fileList, WCHAR* wstrFolder );

		void			DeleteNMCachData();
		void			LoadLuaEnum();
	
#ifdef IN_HOUSE_PLAY_LOG_TEST
		void SendInHousePlayLogMail( char* pReceiverAddress );
#endif IN_HOUSE_PLAY_LOG_TEST



#ifdef REFACTORING_BY_TOOL_TEAM
		void ExitClientIfHackingFound( const float fElapsedTime ); 
#endif REFACTORING_BY_TOOL_TEAM



#ifdef SERV_HACKING_TOOL_LIST
#	ifdef REFACTORING_BY_TOOL_TEAM
		void CreateCheckWindowInfoThreadIfNotExist();
#	endif REFACTORING_BY_TOOL_TEAM
#endif SERV_HACKING_TOOL_LIST

		bool GetWallPaperDirectory( char * pszDir, int iDirSize );

		//{{ 2013.02.27 조효진	해외 기본 구조 작업 (SERV_GLOBAL_BASE)
		std::wstring GetWstrBasicConfigFile();
		//}}
		//{{김준환 서버시간 받아오기
#ifdef	SERV_SERVER_TIME_GET
		void StateChangeTimeSleep(float nSleepTime);
#endif  SERV_SERVER_TIME_GET

#ifdef CLOSE_ON_START_FOR_GAMEGUARD
	public:
		bool IsCloseOnStart() { return m_bCloseOnStart; }
		void SetCloseOnStart(bool b) { m_bCloseOnStart = b; }
#endif CLOSE_ON_START_FOR_GAMEGUARD
#ifdef ALWAYS_INTERNAL_NPGE_PACKING
		void SetClientArg(string TempArg) { ClientArg = TempArg; }
		string GetClinetArg(void) { return ClientArg; }
#endif ALWAYS_INTERNAL_NPGE_PACKING
#ifdef CASH_CHARGE_URL_JP
		void MSCashChargeURL_LUA( const char* pCashChargeURL )
		{
			if( NULL == pCashChargeURL )
				return;
			m_wstrMSCashChargeURL = L"";
			ConvertUtf8ToWCHAR( m_wstrMSCashChargeURL, pCashChargeURL );
		}
		std::wstring	GetMSCashChargeURL(){ return m_wstrMSCashChargeURL; }

		void NHNCashChargeURL_LUA( const char* pCashChargeURL )
		{
			if( NULL == pCashChargeURL )
				return;
			m_wstrNHNCashChargeURL = L"";
			ConvertUtf8ToWCHAR( m_wstrNHNCashChargeURL, pCashChargeURL );
		}
		std::wstring	GetNHNCashChargeURL(){ return m_wstrNHNCashChargeURL; }

		void NICOCashChargeURL_LUA( const char* pCashChargeURL )
		{
			if( NULL == pCashChargeURL )
				return;
			m_wstrNICOCashChargeURL = L"";
			ConvertUtf8ToWCHAR( m_wstrNICOCashChargeURL, pCashChargeURL );
		}
		std::wstring	GetNICOCashChargeURL(){ return m_wstrNICOCashChargeURL; }

		void NEXONCashChargeURL_LUA( const char* pCashChargeURL )
		{
			if( NULL == pCashChargeURL )
				return;
			m_wstrNEXONCashChargeURL = L"";
			ConvertUtf8ToWCHAR( m_wstrNEXONCashChargeURL, pCashChargeURL );
		}
		std::wstring	GetNEXONCashChargeURL(){ return m_wstrNEXONCashChargeURL; }
#endif //CASH_CHARGE_URL_JP

#ifdef TOGGLE_UNLIMITED_SKILL_USE
		bool ToggleUnlimitedSkillUse();
		bool IsUnlimitedSkillUse() { return m_bUnlimitedSkillUse; }
#endif //TOGGLE_UNLIMITED_SKILL_USE

	private:
#ifdef TOGGLE_UNLIMITED_SKILL_USE
		bool				m_bUnlimitedSkillUse;
#endif //TOGGLE_UNLIMITED_SKILL_USE

		wstring				m_ClientVersion;
		wstring				m_SubClientVersion;
#ifdef _NEXON_KR_		
		NMVirtualKey		m_NMKeyOfMineToBeDeleted;		/// 삭제할 캐릭터의 넥슨메신저VirtualKey
		bool				m_bNexonLogin;
		bool				m_bNexonLoginMessenger;
		bool				m_bNexonVirtualLogin;
#endif

		X2_STATE			m_NowStateID;
		CX2State*			m_pNowState;

		int					m_iChannelServerPort;
		std::vector<wstring> m_vecChannelServerIP;
		int					m_iPickedChannelServerIPIndex;
		int					m_iDefaultChannelServerIPIndex;
#ifdef SERVER_GROUP_UI_ADVANCED
		KServerGroupInfo	m_DefaultChannelServerInfo;
#endif SERVER_GROUP_UI_ADVANCED
		
		std::wstring		m_ConnectedGameServerIP;		// 접속된 게임서버 ip
		int					m_iConnectedGameServerPort;		// 접속된 게임서버 port
		int					m_ServerUDPPort;				// 접속된 게임서버 udp relay port
		int					m_GameP2PPort;
		int					m_FrameBufferNum;

		CX2Data*			m_pData;
		wstring				m_SafetyKey;

		float				m_fServerTimeOut;

		vector<WaitServerPacket*>	m_WaitServerPacketList;
		map<int, int>				m_mapWaitServerPacketList;	// only for debugging; <eventid, count>


		bool				m_bIsPlayFieldDungeon;
		bool				m_bIsExitFieldDungeon;
		bool				m_bIsPlayingTutorial;
		bool				m_bIsExitingTutorial;
		X2_STATE			m_StateIDBeforeTutorial;
		int					m_iRandLoadingBG;



		CX2StringFilter*	m_pStringFilter;	
		CX2GameOption		m_GameOption;
		//CX2UnitLoader		m_UnitLoader;
		
		X2_PUBLISHER		m_X2Publisher;
		bool				m_bManualLogin;
#ifdef _NEXON_KR_
		CX2NexonMsgHandler	m_CX2NexonMsgHandler;
#endif
		
		LPVOID				m_pCrashReportState;

		CKTDGFontManager::CUKFont* m_pFontForDebug;

		UINT							m_iConnectChannelID;
		std::vector<KPVPChannelInfo>	m_vecChannel;

		float						m_fServerDataTime;
		int							m_iServerUserCount;
		map<int, int>				m_mapServerPVPRoomCount;
		//{{ 2009. 4. 1  최육사		파티정보
		int							m_iCurPartyCount;
		int							m_iPlayGamePartyCount;
		//}}		

		CX2GameEdit*				m_pGameEdit;
		CX2TextManager*				m_pTextManager;
		CX2NoticeManager*			m_pNoticeManager;

		int							m_QuitType;

		std::vector<KItemInfo>		m_vecDungeonRewardItem;

		bool						m_bCenterServerDie;

		CX2StateAutoChanger			m_AutoStateChanger;
		CX2CheckSumManager*			m_pCheckSumManager;

		wstring						m_PlannerServerIP;

		int							m_iTrainingRewardED;
		int							m_iTrainingRewardEXP;
		bool						m_bTrainingGameSuccess;
		bool						m_bLastStageWasTrainingGame;
		bool						m_bIsPlayingFreeTraining;

		bool						m_bAutoStartNextTraining;
		int							m_iNowTrainingID;

		
		CX2TutorSystem*				m_pTutorSystem;
		
#ifndef COUPON_SYSTEM // 이전 UI 제거
		CX2CouponBox*				m_pCouponBox;		
#endif // COUPON_SYSTEM

		
		CX2PVPEmblem*				m_pX2PVPEmblem;

#ifdef POSTBOX
        CX2PostBox*                 m_pPostBox;
#endif
#ifdef SERV_SECOND_SECURITY
		CX2SecurityPad*				m_pSecurityPad;
#endif SERV_SECOND_SECURITY
		bool						m_bAutoQuit;
		float						m_fAutoQuitTime;
		float						m_fAutoQuitCoolTime;
		
		CX2InformerManager*			m_pInformerManager;
		CX2MemoryHolder*			m_pMemoryHolder;

		HINTERNET					m_hInternet;

		CKTDGUIDialogType				m_pDLGStateChange;
		vector<wstring>				m_KOMList;
		vector<wstring>				m_DLLList;
		vector<wstring>				m_MP3List;

		vector<wstring>				m_UseFontNameList;

		int							m_SquareMaxUserNum;


		KBrowserWrapper				m_BrowserWrapper;		// 넥슨 캐시결제 홈페이지 띄우기 위한 browser

#ifdef CJ_ID_WEB_BILLING
		KNMBrowserWrapper			m_NMBrowserWrapper;
#endif //CJ_ID_WEB_BILLING

		CX2LVUpEventMgr*			m_pLVUpEventMgr;

		float						m_fTimeToUpdateProcessList;

		wstring						m_MailNameToFindHack;
#ifdef  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
        MemberCriticalSection       m_csUserIdToFindHack;
        std::string                 m_strUserIdToFindHack_CS;
#else   X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		wstring						m_strUserIdToFindHack;
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		bool						m_bHackingUser;
#ifdef  X2OPTIMIZE_PROCESS_LIST_MULTITHREAD_CRASH_BUG_FIX
        MemberCriticalSection       m_csProcessNameToFindHack;
		set<wstring>				m_setProcessNameToFindHack_CS;
        MemberCriticalSection       m_csSendedProcessList;
		set<wstring>				m_setSendedProcessList_CS;
#else   X2OPTIMIZE_PROCESS_LIST_MULTITHREAD_CRASH_BUG_FIX
		set<wstring>				m_setProcessNameToFindHack;
		set<wstring>				m_setSendedProcessList;
#endif  X2OPTIMIZE_PROCESS_LIST_MULTITHREAD_CRASH_BUG_FIX
		
		queue< CX2StateMenu::PresentArrival > m_quePresentArrival;

		vector<ReservedMessagePopUp>	m_vecReservedMessagePopUp;
		
		//vector<TimedMessagePopUp>		m_vecTimedMessagePopUp;
		list<TimedMessagePopUp>			m_listTimedMessagePopUp; // kimhc // 2010.01.22 // list로 수정


		CX2PartyUI*					m_pPartyUI;

			

#ifdef LOADING_ANIMATION_TEST
		CX2LoadingAnimation*		m_pLoadingAnimation;
#endif LOADING_ANIMATION_TEST


#ifdef CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT
		bool						m_bReceived_EGS_KNPROTECT_AUTH_CHECK_NOT;		// nprotect 인증 패킷을 받은 후에 게임로딩 시작
#endif CHECK_RECEIVED_EGS_KNPROTECT_AUTH_CHECK_NOT


#ifdef SHOOTING_TEST
		int							m_iShootingLevel;
#endif SHOOTING_TEST

		wstring						m_TRServerIp; // 현재 접속한 채널서버 IP


		
		std::vector< ReservedServerPacket > m_vecReservedServerPacket;
//{{ 09.07.03 태완 : 사내에서 계정생성 되게
#if !defined(_SERVICE_) || defined(SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER)
		int m_iGameServerPortForCreateID;
		wstring m_wstrGameServerIPForCreateID;
#endif //!defined(_SERVICE_) || defined(SERV_JOIN_IN_CLIENT_FOR_TW_TEST_SERVER)
//}}
#if defined(CLIENT_COUNTRY_TWHK) || defined(SERV_STEAM)
		wstring m_wstrID;
		wstring m_wstrPass;
#endif //defined(CLIENT_COUNTRY_TWHK) || defined(SERV_STEAM)

		bool m_bEnabledDeveloperCheatCode;	// 개발자만 제한된 기능을 사용할지 말지

#ifdef SERV_SKILL_NOTE
		CKTDGUIDialogType m_pDlgInfoBox;
		float m_fInfoBoxLifeTime;
#endif

#if defined( SERV_HACKING_TOOL_LIST )
		CheckWindowInfo					*m_threadCheckWindowInfo;


#	ifdef REFACTORING_BY_TOOL_TEAM
		DebuggerChecker m_DebuggerChecker;
#	else REFACTORING_BY_TOOL_TEAM
		float			m_fCheckDebugeTimer;
#	endif REFACTORING_BY_TOOL_TEAM

#endif // SERV_HACKING_TOOL_LIST

//{{ megagame / 박교현 / 2010.05.12 / 시스템 정보
#ifdef SYS_INFO
		CX2SystemInfo*					m_pSystemInfo;
#endif SYS_INFO
//}} megagame / 박교현 / 2010.05.12 / 시스템 정보

		//{{ JHKang // 강정훈 // 2010/10/06
#ifdef ON_CHAT_REMAIN_FOCUS
		bool		m_bIsPopUpOnChat;
#endif ON_CHAT_REMAIN_FOCUS
		//}} JHKang // 강정훈 // 2010/10/06

#ifdef SERV_CLIENT_PUBLIC_IP
		wstring		m_wstrPublicIp;
#endif

#ifdef	CHECK_KOM_FILE_ON_LOAD
		KHttpSession*	m_pSession;						/// 세션 객체
		vector<pair<string, string>> m_vecCheckKom;	/// first: kom 파일명, second: SHA-1
#ifdef SERV_KOM_FILE_CHECK_ADVANCED
		// vector pair의 first로 검색하기 위해 추가
		struct stCompareFirst
		{
			stCompareFirst( std::string const& strKomFileName ) : m_strKomFileName(strKomFileName) { }

			bool operator() ( std::pair< std::string, std::string > const& pairKomInfo )
			{
				return ( pairKomInfo.first == m_strKomFileName );
			}

			std::string m_strKomFileName;
		};

		vector< pair<string, string> >	m_vecImportantCheckKom;
#ifndef X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		float							m_fGetCheckKomTime;
		mutable KncCriticalSection		m_csData_vec;
		vector< pair<string, string> >	m_vecChangeCheckKom;	// first: kom 파일명, second: SHA-1
#endif  X2OPTIMIZE_GENERAL_MULTITHREAD_RACE_BUG_FIX
		//std::wstring					m_wstrInvaildKomName;

#endif // SERV_KOM_FILE_CHECK_ADVANCED
#endif	CHECK_KOM_FILE_ON_LOAD

#ifdef CLIENT_USE_NATION_FLAG
		DWORD		m_dwNationFlag;
#endif CLIENT_USE_NATION_FLAG

#ifdef ADD_CHKOPT_ABNORMAL_FUNCTION_CALL_V2
		float m_fCheckStatusTime;
#endif
#ifdef SERV_HACKING_TOOL_LIST
		float m_fCheckHackToolTime;
#endif
#ifdef BANDICAM_RECORDING
		float m_fFileSizeCheckTime;
		__int64 m_iOldCaptureFileSize;
#endif //BANDICAM_RECORDING

#ifdef CHECK_PLAY_TIME_INFORMATION
		int				  m_iGameHour;						//클라이언트 시작 후 지난 시간 반환

		CKTDGUIDialogType m_pDlgPlayTimeInformation;		//플레이 시간 안내용 Dialog
		float			  m_fPlayTimeInfoShowTime;			//안내창 유지 시간
		bool			  m_bProcessPlayTimeInformation;	//안내창 Fade 기능 적용 여부


		CKTDGUIDialogType m_pDlgShutDownInformation;		//셧다운 안내용 Dialog
		float			  m_fShutDownInfoShowTime;			//셧다운 안내창 유지 시간
		bool			  m_bProcessShutDownInformation;	//셧다운 안내창 Fade 기능 적용 여부

		wstring			  m_wstrShutDownString;				//셧다운 안내창용 문구
#endif CHECK_PLAY_TIME_INFORMATION

#ifdef USER_REGULATION_NOTICE
		boost::timer                    m_TimerUserRegulationNotice;
		// 방침미 신분인증
#ifdef IDENTITY_CONFIRM
		boost::timer                    m_TimerIdentityConfirmChat;
#endif IDENTITY_CONFIRM
#endif //USER_REGULATION_NOTICE

#ifdef CLOSE_ON_START_FOR_GAMEGUARD
		bool m_bCloseOnStart;
#endif CLOSE_ON_START_FOR_GAMEGUARD

#ifdef ALWAYS_INTERNAL_NPGE_PACKING
		string ClientArg;
#endif ALWAYS_INTERNAL_NPGE_PACKING

#ifdef CASH_CHARGE_URL_JP
		std::wstring		m_wstrMSCashChargeURL;
		std::wstring		m_wstrNHNCashChargeURL;
		std::wstring		m_wstrNICOCashChargeURL;
		std::wstring		m_wstrNEXONCashChargeURL;
#endif //CASH_CHARGE_URL_JP

#ifdef SERV_CHANNELING_USER_MANAGER
		std::map<int,int>			m_mapChannelingUserList;
#endif SERV_CHANNELING_USER_MANAGER

#ifdef CHANGE_MAILSTRING
		string		m_strMailAddress;		/// 클라이언트 메일 발송 주소 스트링
#endif

		CX2KeyPad*		m_pKeyPad;
		bool		m_bSurveyUser;			/// 설문조사 대상 유저


#ifdef NEXON_QA_CHEAT_REQ
		bool	m_bNoCoolTime;
#endif //NEXON_QA_CHEAT_REQ

#ifdef FESTIVAL_UI
		CKTDGParticleSystem::CParticleEventSequenceHandle m_SeqFestivalUI;
#endif //FESTIVAL_UI

#ifdef SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT
		bool m_bIsValideCheckKomScript;	// CheckKom.xml 유효 여부
#endif // SERV_VALIDITY_CHECK_CEHCKKOM_SCRIPT


#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
public:
	void OccurLag_LUA( float fLagTime, int iLagType );

private:
	void _InitKOGGamePerformanceCheck();
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef ACTIVE_KOG_GAME_PERFORMANCE_CHECK
public:
	void ForceHost_LUA( bool bEnable );
	void SwitchConnect_LUA( int iMode );
#endif//ACTIVE_KOG_GAME_PERFORMANCE_CHECK

#ifdef UDP_PACKET_ANALYSIS_LOG
public:
	void UdpPacketAnalysisLog_LUA( bool bEnable );
#endif//UDP_PACKET_ANALYSIS_LOG

#ifdef X2OPTIMIZE_UDP_RELAY_OVERHEAD_TEST
public:
	void UdpRelayOverHeadTest_LUA( int iRoomNum );
#endif//X2OPTIMIZE_UDP_RELAY_OVERHEAD_TEST
//{{김준환 서버시간 받아오기
#ifdef	SERV_SERVER_TIME_GET
		bool				m_bMappingResult;
		int					m_SumCount;
		float				m_GetTimeCount;
		std::wstring		m_TempTime;
		bool				m_bOneChange;
#endif  SERV_SERVER_TIME_GET

//#ifdef  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK
public:
	void UdpPacketOverlapTest_LUA( bool bEnable );

	bool IsEnableUdpPacketOverlap() { return m_bUdpPacketOverlap; }

private:
	bool m_bUdpPacketOverlap;

    CKTDNUDP::EForceConnectMode m_aeUDPMode[CX2Game::GT_NUMS];
//#endif  SERV_KTDX_OPTIMIZE_UDP_PACKET_PACK

#ifdef X2OPTIMIZE_ONE_SIMUL_ONE_RENDER_TEST
public:
	void OneSimulOneRenderTest_LUA( bool bEnable );
#endif//X2OPTIMIZE_ONE_SIMUL_ONE_RENDER_TEST

#ifdef X2OPTIMIZE_DAMAGE_EFFECT_TEST
public:
	void DamageEffectTest_LUA( bool bEnable );
	bool GetDamageEffectTest() { return m_bEnableDamageEffectTest; }

private:
	bool m_bEnableDamageEffectTest;
#endif//X2OPTIMIZE_DAMAGE_EFFECT_TEST

#ifdef X2OPTIMIZE_VIEWDISTANCE_TEST
public:
	void ViewDistanceTest_LUA( int iFar1, int iFar2 );
#endif//X2OPTIMIZE_VIEWDISTANCE_TEST

	//{{ 최민철 [2013/1/4]  게임내 정보 스트링을 엑셀파일로 출력
#ifdef PRINT_INGAMEINFO_TO_EXCEL
	bool			m_bInGameInfoToExcel;
#endif PRINT_INGAMEINFO_TO_EXCEL

#ifdef COUPON_SYSTEM
	CX2UICouponBox		m_UICouponBox;
#endif // COUPON_SYSTEM

#ifdef ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터
	bool				m_bIsJumpingCharacter;			// 점핑 캐릭터 대상 캐릭터인가?
#endif // ADDED_EVENT_JUMPING_CHARACTER	// 김종훈, 여름방학 이벤트 점핑 캐릭터

#ifdef EXPAND_DEVELOPER_SCRIPT	  // 김종훈, 개발자 스크립트 확장 기능 추가
	DeveloperScriptSet			m_DeveloperScriptSet;
#endif // EXPAND_DEVELOPER_SCRIPT  // 김종훈, 개발자 스크립트 확장 기능 추가

#ifdef PLAY_PROMOTION_MOVIE //JHKang
public:
	bool	m_bPlayIntroMovie;
#endif //PLAY_PROMOTION_MOVIE

}; // CX2Main

extern CX2Data*			g_pData;
extern CX2InstanceData*	g_pInstanceData;
extern CX2Main*			g_pMain;
extern CX2Game*			g_pX2Game;
extern CX2Room*			g_pX2Room;
extern CX2SquareGame*	g_pSquareGame;
#ifdef ADDED_RELATIONSHIP_SYSTEM
extern CX2WeddingGame*	g_pWeddingGame;
#endif //ADDED_RELATIONSHIP_SYSTEM

#ifdef ELSWORD_NEW_BEGINNING
extern CX2BeginningGame* g_pBeginningGame;
#endif ELSWORD_NEW_BEGINNING

extern CX2TFieldGame*	g_pTFieldGame;

extern CX2ChatBox*		g_pChatBox;

extern FILE*			g_fpStream;
extern FILE*			g_fpStreamError;
extern HINTERNET		g_hInternet;

#ifdef _NEXON_KR_
extern wstring			g_pNexonPassport;	
#endif



inline void OpenGlobalURL( WCHAR* pURL )
{
	if( g_hInternet == NULL )
		return;

	// 파일이 있는 URL을 열어놓는다.
	HINTERNET hURLFile;
	if ( NULL == ( hURLFile = ::InternetOpenUrl( g_hInternet,
		pURL,
		NULL,
		0,
		INTERNET_FLAG_RELOAD |
		INTERNET_FLAG_NO_CACHE_WRITE,
		0 ) ) )
	{
		ErrorLog( XEM_ERROR107 );
		return;
	}

	::InternetCloseHandle( hURLFile );
}




__forceinline bool CX2Main::OpenScriptFile( const WCHAR* pFileName )
{

	lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pMain", this );


    if ( g_pKTDXApp->LoadLuaTinker( pFileName ) == false )
    {
		ErrorLogMsg( XEM_ERROR14, pFileName );
		return false;
    }

	return true;
}