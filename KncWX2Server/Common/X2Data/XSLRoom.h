#pragma once

#include "XSLUnit.h"

class CXSLRoom
{
	public:
		enum ROOM_TYPE
		{
			//{{ 2012. 04. 20	박세훈	어둠의 문 개편
//#ifdef SERV_REFORM_THE_GATE_OF_DARKNESS
			RT_NONE				= -1,	// UserBuffManager에서 RoomType을 가리지 않고 모든 버프를 선택할 때 사용하고 있음.
//#endif SERV_REFORM_THE_GATE_OF_DARKNESS
			//}}
			RT_PVP				= 0,
			RT_DUNGEON			= 1,
			RT_SQUARE			= 2,
			RT_TRADE			= 3,
			RT_TRAININGCENTER	= 4,
			RT_PERSONAL_SHOP	= 5,
			//{{ 2011. 04. 06	최육사	대리 상인
//#ifdef SERV_PSHOP_AGENCY
			RT_PSHOP_AGENCY		= 6,
//#endif SERV_PSHOP_AGENCY
			//}}
			//{{ 2011. 10. 27	최육사	배틀필드 시스템
//#ifdef SERV_BATTLE_FIELD_SYSTEM
			RT_BATTLE_FIELD		= 7,
//#endif SERV_BATTLE_FIELD_SYSTEM
			//}}
			//{{ 2013. 04. 01	 인연 시스템 - 김민성
//#ifdef SERV_RELATIONSHIP_SYSTEM
			RT_WEDDING_HALL		= 8,
//#endif SERV_RELATIONSHIP_SYSTEM
			//}

            RT_TOTAL_NUM,
		};

		enum ROOM_STATE
		{
			RS_INIT		= 1,
			RS_CLOSED,
			RS_WAIT,
			RS_LOADING,
			RS_PLAY,
		};

		enum PVP_ROOM_STATE
		{
			PRS_WAIT = 0,
			PRS_PLAY,
		};

		enum PVP_GAME_TYPE
		{
			PGT_NULL = -1,
			PGT_TEAM = 0,
			PGT_TEAM_DEATH,
			PGT_SURVIVAL,
			PGT_TAG,
			PGT_END,
		};

		enum TEAM_NUM
		{
			TN_NONE		= -1,
			TN_RED		=  0,
			TN_BLUE		=  1,
			TN_MONSTER	=  2,
		};

		enum DUNGEON_GET_ITEM_TYPE
		{
			DGIT_NONE	= 0,
			DGIT_RANDOM,
			DGIT_PERSON,
			DGIT_END,
		};

		struct PVPRoomSimpleData
		{
			int					roomNum;
			std::wstring		roomName;

			PVP_ROOM_STATE		roomState;
			PVP_GAME_TYPE		gameType;
			int					worldID;

			int					maxSlot;
			int					joinSlot;
		};

		enum SLOT_STATE
		{
			SS_EMPTY	= 1,
			SS_CLOSE,
			SS_WAIT,
			SS_LOADING,
			SS_PLAY,
		};

		enum SLOT_EVENT
		{
			SE_NO_CHANGE = 0,//슬롯 정보 변경 없음.
			SE_CHANGE,		 //슬롯 정보가 변경됨. 
			SE_NEW_JOIN,	 //새로운 유저가 입장함.
			SE_EXIT_ROOM,	 //유저가 방에서 나감. 
			SE_EXIT_GAME,	//유저가 게임에서 나가 방으로 감.
			SE_DISCONNECT,	 //유저 접속이 종료됨. 
			SE_OPEN_CLOSE,	 //슬롯을 열거나 닫음. 
			SE_READY,		 //준비 하거나 안함.
			SE_TEAM_CHANGE,	 //팀 바꿈.
			SE_MOTION_CHANGE, //모션 바꿈.
		}; 

		enum SLOT_UNIT_SPECIAL_MOTION
		{
			SUSM_NONE = 0,
			SUSM_WAIT1,
			SUSM_WAIT2,
			SUSM_END,
		};

		enum ROOM_CHAT_TYPE
		{
			RCT_ALL = 0,
			RCT_TEAM,
			RCT_WHISPER,
		};

		//{{ 2009. 7. 21  최육사	비밀스테이지
		enum ENTER_SECRET_STAGE
		{
			ESS_NONE = 0,
			ESS_NOT_AGREE,
			ESS_AGREE,
		};
		//}}

		struct PVPSlotData
		{
			SLOT_STATE		slotState;
			int				teamNum;
			CXSLUnit*		pUnit;
		};

		struct PVPRoomData
		{
			int						roomNum;
			std::wstring			roomName;

			PVP_ROOM_STATE			roomState;
			PVP_GAME_TYPE			gameType;

			int						winGameNum;
			int						winKillNum;

			int						worldID;
			std::vector<PVPSlotData>	slotDataList;
		};

	public:
		CXSLRoom(void);
		~CXSLRoom(void);

	static ROOM_TYPE GetRoomType( UidType iRoomUID ) { return static_cast<ROOM_TYPE>(KncUid::ExtractReservedID( iRoomUID )); }
	//{{ 2009. 10. 22  최육사	대전방 버그수정	
	static bool IsValidWinKillNum( char cPvpGameType, char cWinKillNum );
	static bool IsValidPvpPlayTime( char cPvpGameType, int iPlayTime );
	//}}
	//{{ 2011. 06. 20	최육사	대전 개편
#ifdef SERV_PVP_NEW_SYSTEM
	static bool IsValidPvpGameType( const PVP_GAME_TYPE ePvpGameType );
#endif SERV_PVP_NEW_SYSTEM
	//}}
};



