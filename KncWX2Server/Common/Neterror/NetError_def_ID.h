#pragma once

enum NetErrorEnum
{
	NET_OK = 0,             //성공
	ERR_UNKNOWN,             //알 수 없는 에러입니다.
	ERR_CLIENT_STATE,             //유저 상태 오류입니다.
	ERR_CONNECT_00,             //버전이 맞지 않습니다. 패치 후 재접속 하세요.
	ERR_CONNECT_04,             //서버 수용 인원이 가득 찼습니다.
	ERR_CONNECT_05,             //게임서버 점검중입니다. 점검시간이 끝난후에 접속해주세요.
	ERR_VERIFY_00,             //동일서버간 이중 접속 시도입니다.
	ERR_VERIFY_01,             //타서버간 이중접속 시도입니다.
	ERR_VERIFY_02,             //서버 IP 검증 실패입니다.
	ERR_VERIFY_03,             //서버 UID 충돌입니다.
	ERR_VERIFY_04,             //유저가 존재하지 않습니다.
	ERR_VERIFY_05,             //삭제된 유저입니다.
	ERR_VERIFY_06,             //패스워드가 다릅니다.
	ERR_VERIFY_08,             //서버 작동 플래그 설정 실패입니다.
	ERR_VERIFY_10,             //NEXON 인증 실패입니다.
	ERR_VERIFY_11,             //차단된 계정입니다.
	ERR_VERIFY_12,             //운영자 이상 레벨만 요청 가능합니다.
	ERR_VERIFY_13,             //프록시 ID가 없습니다.
	ERR_VERIFY_14,             //허용되지 않은 IP 입니다.
	ERR_VERIFY_15,             //게임서버의 해당 채널 ID정보가 없습니다.
	ERR_VERIFY_16,             //ID의 길이가 이상합니다.
	ERR_NEW_ACCOUNT_02,             //트랜잭션 에러입니다.
	ERR_NEW_ACCOUNT_03,             //ID 길이가 잘못되었습니다.
	ERR_NEW_ACCOUNT_04,             //PASSWORD 길이가 잘못되었습니다.
	ERR_NEW_ACCOUNT_06,             //이름 길이가 잘못되었습니다.
	ERR_CREATE_UNIT_00,             //삭제된 유저가 캐릭터 생성을 시도하였습니다.
	ERR_CREATE_UNIT_01,             //캐릭터 닉네임이 이미 존재합니다.
	ERR_CREATE_UNIT_02,             //트랜잭션 에러입니다.
	ERR_CREATE_UNIT_03,             //캐릭터 데이터 로드 실패입니다.
	ERR_CREATE_UNIT_04,             //캐릭터 닉네임 길이가 잘못되었습니다.
	ERR_CREATE_UNIT_05,             //더 이상의 캐릭터를 생성할 수 없습니다.
	ERR_CREATE_UNIT_06,             //해당 닉네임은 일정 기간 동안 사용할 수 없습니다.
	ERR_CREATE_UNIT_07,             //넥슨 메신저용 CharacterSN을 DB에 생성하지 못했습니다.
	ERR_CREATE_UNIT_08,             //사용할 수 없는 닉네임입니다.
	ERR_CREATE_UNIT_09,             //캐릭터는 하루에 네번만 생성할 수 있습니다.
	ERR_SELECT_UNIT_00,             //선택한 캐릭터가 게임 서버에 없습니다.
	ERR_SELECT_UNIT_01,             //선택한 캐릭터의 클래스가 이상합니다.
	ERR_SELECT_UNIT_03,             //부활석 정보를 DB에서 받아오지 못했습니다.
	ERR_SELECT_UNIT_04,             //넥슨 메신저용 CharacterSN을 DB에서 받아오지 못했습니다.
	ERR_SELECT_UNIT_05,             //캐릭터 선택 과정에서 DB 오류가 발생하였습니다.
	ERR_SELECT_UNIT_06,             //닉네임을 변경해야 합니다.
	ERR_CHANNEL_00,             //레벨에 맞는 PVP채널을 찾지 못했습니다.
	ERR_CHANNEL_02,             //채널에서 삭제할 유저를 찾지 못했습니다.
	ERR_CHANNEL_04,             //채널 이동 실패하였습니다.
	ERR_CHANNEL_05,             //VP가 부족하여 입장할 수 없습니다.
	ERR_CHANNEL_06,             //광장 채널에 입장하지 못했습니다.
	ERR_CHANNEL_07,             //채널 입장하지 못했습니다.
	ERR_CHANNEL_09,             //진행중인 대회가 없습니다.
	ERR_ROOM_LIST_00,             //파티 리스트를 불러올 수 없습니다.
	ERR_ROOM_LIST_01,             //던전 게이트에서만 파티 리스트를 볼 수 있습니다.
	ERR_ROOM_LIST_02,             //해당 던전 게이트에서 볼 수 있는 파티리스트가 아닙니다.
	ERR_ROOM_LIST_03,             //3레벨부터 대전을 할 수 있습니다.
	ERR_ROOM_00,             //방 서버에 더 이상 방을 만들 수 없습니다.
	ERR_ROOM_01,             //해당 슬롯에서 유저를 발견할 수 없습니다.
	ERR_ROOM_02,             //해당 캐릭터가 있는 슬롯을 찾을 수 없습니다.
	ERR_ROOM_03,             //방이 꽉 찼습니다.
	ERR_ROOM_04,             //비밀 번호가 틀립니다.
	ERR_ROOM_06,             //선행던전을 클리어하지 않았습니다.
	ERR_ROOM_07,             //퀵조인 할 수 있는 방이 없습니다.
	ERR_ROOM_08,             //방장만 요청할 수 있습니다.
	ERR_ROOM_09,             //패킷이 허용되는 방 상태가 아닙니다.
	ERR_ROOM_10,             //준비 상태가 아닌 유저가 있습니다.
	ERR_ROOM_11,             //팀 인원수가 같지 않습니다.
	ERR_ROOM_12,             //한 명 이상 READY 해야 시작 할 수 있습니다.
	ERR_ROOM_13,             //로딩 상태를 변경하지 못했습니다.
	ERR_ROOM_14,             //방이 생성되었습니다.
	ERR_ROOM_15,             //방 정보가 변경되었습니다.
	ERR_ROOM_16,             //방이 삭제되었습니다.
	ERR_ROOM_17,             //방에서 유저를 검색하지 못했습니다.
	ERR_ROOM_18,             //난입 실패하였습니다.
	ERR_ROOM_19,             //난입 가능 방에서 나가지 못했습니다.
	ERR_ROOM_20,             //죽은 NPC UID가 잘못되었습니다.
	ERR_ROOM_21,             //이미 죽은 NPC입니다.
	ERR_ROOM_22,             //몬스터 데이터가 잘못되어 로딩 실패하였습니다.
	ERR_ROOM_23,             //대상 유저를 찾을 수 없습니다.
	ERR_ROOM_24,             //채팅 타입이 이상합니다.
	ERR_ROOM_26,             //자기 자신을 강퇴 시킬수 없습니다.
	ERR_ROOM_27,             //근성도가 부족합니다.
	ERR_ROOM_28,             //추가 스탯을 지원하지 않는 게임 타입입니다.
	ERR_ROOM_29,             //게임 종료중이라 난입할 수 없습니다.
	ERR_ROOM_30,             //방에 입장할 수 없습니다.
	ERR_ROOM_31,             //입장 중인 방을 알 수 없습니다.
	ERR_ROOM_32,             //거래 중에는 게임 준비를 할 수 없습니다.
	ERR_ROOM_33,             //게임 시작 중이라 난입할 수 없습니다. 잠시 후에 다시 시도해주세요.
	ERR_ROOM_34,             //게임 시작 아이템이 인벤토리에 없습니다.
	ERR_ROOM_35,             //권한이 없는 유저가 옵저버 방을 만들려고 시도했습니다.
	ERR_ROOM_36,             //잘못된 대전맵 정보입니다.
	ERR_ROOM_39,             //오프라인 대회 참여권을 가지고 있는 유저는 대회 채널을 이용하실 수 없습니다.
	ERR_ROOM_41,             //해당 던전을 플레이 하기 위한 아이템이 없습니다. 게임 시작 시 방에서 자동으로 이탈하게 됩니다.
	ERR_ROOM_43,             //대전 방에서만 초대 기능을 사용하실 수 있습니다.
	ERR_ROOM_44,             //초대하려는 상대가 대전방에 입장할 수 없는 상태입니다.
	ERR_ROOM_45,             //대전 채널 로비에 있는 유저만 초대할 수 있습니다.
	ERR_ROOM_46,             //파티에 속해 있어야 게임이 시작가능합니다.
	ERR_ROOM_47,             //킬 수 세팅이 이상합니다.
	ERR_ITEM_00,             //습득하려는 아이템이 없습니다.
	ERR_ITEM_01,             //다른 사람이 이미 습득하였습니다.
	ERR_ITEM_03,             //랜덤으로 아이템 주기가 실패하였습니다.
	ERR_ITEM_04,             //아이템 정보를 찾지 못했습니다.
	ERR_ITEM_05,             //아이템 ID가 잘못되었습니다.
	ERR_ITEM_06,             //아이템을 주지 못했습니다.
	ERR_ITEM_07,             //아이템을 수리하지 못했습니다.
	ERR_ITEM_08,             //퀵슬롯에 있는 아이템을 사용하지 못했습니다.
	ERR_ITEM_09,             //이벤트 던전에서는 퀵슬롯을 사용할 수 없습니다.
	ERR_ITEM_10,             //아이템을 획득하지 못했습니다.
	ERR_ITEM_11,             //임시 인벤토리의 아이템을 가져오지 못했습니다.
	ERR_ITEM_12,             //임시 인벤토리 아이템을 인벤토리에 넣을 공간이 부족합니다.
	ERR_ITEM_13,             //확성기 메시지 길이가 너무 깁니다.
	ERR_SLOT_01,             //팀을 변경하지 못했습니다.
	ERR_SLOT_02,             //슬롯 상태를 변경하지 못했습니다.
	ERR_SLOT_03,             //준비 상태를 변경하지 못했습니다.
	ERR_SLOT_04,             //슬롯에서 나가지 못했습니다.
	ERR_SLOT_05,             //강제 퇴장 시키지 못했습니다.
	ERR_SLOT_07,             //자신을 죽인 유저 데이터를 저장하지 못했습니다.
	ERR_SLOT_09,             //상태를 변경하지 못했습니다.
	NOT_LEAVE_ROOM_REASON_00,             //유저에 의한 퇴장입니다.
	NOT_LEAVE_ROOM_REASON_01,             //접속이 종료되었습니다.
	NOT_LEAVE_ROOM_REASON_02,             //강퇴로 인한 퇴장입니다.
	NOT_LEAVE_ROOM_REASON_04,             //레벨 업으로 인하여 해당 채널에 머무를 수 없습니다.
	NOT_LEAVE_ROOM_REASON_06,             //개인 거래가 완료되었습니다.
	NOT_LEAVE_ROOM_REASON_07,             //개인 거래 참가 실패입니다.
	NOT_LEAVE_ROOM_REASON_08,             //개인 거래 참가 지연으로 인한 퇴장입니다.
	NOT_LEAVE_ROOM_REASON_09,             //개인 거래가 취소되었습니다.
	NOT_LEAVE_ROOM_REASON_10,             //개인 거래 인원 수가 이상합니다.
	NOT_LEAVE_ROOM_REASON_11,             //개인 거래 중 오류가 발생하였습니다.
	NOT_LEAVE_ROOM_REASON_12,             //인벤토리 공간이 부족하거나 등록된 아이템 정보가 이상합니다.
	NOT_LEAVE_ROOM_REASON_14,             //근성도가 부족하여 던전을 진행 할 수 없습니다.
	NOT_LEAVE_ROOM_REASON_15,             //거래할 물품이 없어서 거래가 취소되었습니다.
	NOT_LEAVE_ROOM_REASON_16,             //거래가 취소되었습니다.
	NOT_LEAVE_ROOM_REASON_17,             //개인상점 물품등록 지연으로 상점이 종료되었습니다.
	NOT_LEAVE_ROOM_REASON_18,             //상점거래가 취소되었습니다.
	NOT_LEAVE_ROOM_REASON_19,             //개인 상점의 모든 물품이 판매 완료 되어 상점이 종료되었습니다.
	NOT_LEAVE_ROOM_REASON_20,             //방에서 퇴장하여 거래가 취소되었습니다.
	NOT_LEAVE_ROOM_REASON_21,             //개인 상점이 판매 종료 되었습니다.
	NOT_LEAVE_ROOM_REASON_23,             //클라이언트 크래쉬입니다.
	NOT_LEAVE_ROOM_REASON_24,             //대회 채널에 더 이상 머무를 수 없습니다.
	NOT_LEAVE_PARTY_REASON_01,             //대전 게임 진입으로 인한 파티 탈퇴입니다.
	NOT_LEAVE_PARTY_REASON_02,             //던전 게임 중단으로 인한 파티 탈퇴입니다.
	NOT_LEAVE_PARTY_REASON_03,             //마켓 진입으로 인한 파티 탈퇴입니다.
	NOT_LEAVE_PARTY_REASON_04,             //훈련소 진입으로 인한 파티 탈퇴입니다.
	ERR_CLIENT_QUIT_00,             //정상 종료입니다.
	ERR_CLIENT_QUIT_01,             //해킹 감지로 인한 종료입니다.
	ERR_CLIENT_QUIT_02,             //클라이언트 크래쉬로 인한 종료입니다.
	ERR_ADMIN_COMMAND_00,             //캐릭터 경험치 수정 실패입니다.
	ERR_ADMIN_COMMAND_01,             //요청한 캐릭터의 클래스를 DB에서 알아내지 못했습니다.
	ERR_ADMIN_COMMAND_03,             //권한 레벨이 잘못되었습니다.
	ERR_ADMIN_COMMAND_04,             //해당 유저가 없습니다.
	ERR_ADMIN_COMMAND_05,             //트랜잭션 에러입니다.
	ERR_ADMIN_COMMAND_06,             //해당 권한 레벨의 유저가 없음.
	ERR_ADMIN_COMMAND_07,             //운영자에 의해 접속이 종료되었습니다.
	ERR_ADMIN_COMMAND_08,             //이 명령어는 사내에서만 실행할 수 있습니다.
	ERR_ADMIN_COMMAND_09,             //이 명령어는 방에서 실행할 수 없습니다.
	ERR_BUY_ED_ITEM_05,             //아이템을 구입할 수 없습니다.
	ERR_SELL_ED_ITEM_05,             //아이템을 되팔 수 없습니다.
	ERR_QUEST_01,             //수행 중인 퀘스트입니다.
	ERR_QUEST_02,             //수행 중이지 않은 퀘스트를 지우려고 하였습니다.
	ERR_QUEST_03,             //존재하지 않는 퀘스트입니다.
	ERR_QUEST_04,             //완료한 퀘스트입니다.
	ERR_QUEST_05,             //이 캐릭터는 해당 퀘스트를 수행할 수 없는 캐릭터입니다.
	ERR_QUEST_06,             //이전의 필요한 퀘스트를 완료하지 않았습니다.
	ERR_QUEST_07,             //NPC에게 잘못된 퀘스트를 받았습니다.
	ERR_QUEST_08,             //퀘스트 템플릿이 존재하지 않습니다.
	ERR_QUEST_09,             //완료 조건이 만족되지 않았습니다.
	ERR_QUEST_10,             //퀘스트 관련 DB처리가 실패하였습니다.
	ERR_QUEST_11,             //퀘스트는 최대 10개까지만 수행이 가능합니다.
	ERR_QUEST_12,             //레벨이 낮아서 아직 퀘스트를 수락할 수 없습니다.
	ERR_QUEST_13,             //인벤토리 공간이 부족하여 퀘스트를 완료할 수 없습니다. 인벤토리를 먼저 정리해 주세요.
	ERR_QUEST_14,             //퀘스트 선택 보상 처리 중 오류가 발생하였습니다.
	ERR_QUEST_15,             //선택 보상 아이템 검색이 실패하였습니다.
	ERR_QUEST_17,             //이미 전직 퀘스트를 수행 중입니다.
	ERR_QUEST_18,             //전직 퀘스트 보상 처리 과정 중 오류가 발생하였습니다.
	ERR_QUEST_19,             //퀘스트 타입이 이상합니다.
	ERR_CHAT_01,             //채팅 입력 제한 시간보다 짧게 입력하였습니다.
	ERR_CHAT_02,             //자기 자신에게는 귓속말을 할 수 없습니다.
	ERR_CHAT_03,             //채팅 종류가 이상합니다.
	ERR_CHAT_04,             //현재 접속중이 아닙니다.
	ERR_ODBC_00,             //--/;/따옴표/쉼표/공백 등은 사용할 수 없습니다.
	ERR_REFRESH_01,             //유저 리스트 갱신시간보다 짧게 입력하였습니다.
	ERR_SEARCH_UNIT_01,             //유저정보 검색 제한시간이 지나지 않았습니다.
	ERR_SEARCH_UNIT_03,             //현재 접속 중이 아닙니다.
	ERR_SEARCH_UNIT_04,             //존재하는 닉네임이 아닙니다.
	ERR_SQUARE_01,             //광장 입장 실패입니다.
	ERR_SQUARE_02,             //광장 나가기 실패입니다.
	ERR_SKILL_00,             //이미 최고 레벨까지 배운 스킬입니다.
	ERR_SKILL_01,             //존재하지 않는 스킬입니다.
	ERR_SKILL_03,             //레벨이 낮아서 아직 스킬을 배울 수 없습니다.
	ERR_SKILL_04,             //스킬 포인트가 부족합니다.
	ERR_SKILL_05,             //습득 조건 아이템이 없습니다.
	ERR_SKILL_06,             //습득 조건 퀘스트를 클리어 하십시오.
	ERR_SKILL_07,             //선행 습득 스킬을 배우지 않았습니다.
	ERR_SKILL_08,             //이미 배운 스킬입니다.
	ERR_SKILL_09,             //변경할 스킬 슬롯 아이디가 잘못되었습니다.
	ERR_SKILL_10,             //배우지 않은 스킬을 등록하려고 하였습니다.
	ERR_SKILL_12,             //스킬을 초기화하지 못했습니다.
	ERR_SKILL_13,             //스킬 슬롯 B를 사용하실 수 없습니다.
	ERR_SKILL_14,             //스킬 슬롯 B 확장 중 DB오류가 발생하였습니다.
	ERR_SKILL_15,             //봉인 해제되지 않은 스킬은 배울 수 없습니다.
	ERR_SKILL_16,             //배울 수 없는 계층에 있는 스킬입니다.
	ERR_SKILL_17,             //스킬 포인트가 부족해서 스킬을 획득할 수 없습니다.
	ERR_SKILL_18,             //스킬 포인트가 부족해서 스킬을 획득할 수 없습니다.
	ERR_SKILL_19,             //존재하지 않는 스킬의 정보를 변경하려고 하였습니다.
	ERR_SKILL_20,             //스킬 레벨 정보가 이상합니다.
	ERR_SKILL_21,             //스킬 제한 레벨보다 높은 레벨로 설정하려고 하였습니다.
	ERR_SKILL_22,             //캐시 스킬포인트 기간 만료 처리가 실패하였습니다.
	ERR_SKILL_23,             //캐시 스킬포인트 등록이 실패하였습니다.
	ERR_SKILL_24,             //캐시 스킬포인트 정보 갱신이 실패하였습니다.
	ERR_SKILL_25,             //캐시 스킬포인트 정보 초기화가 실패하였습니다.
	ERR_SKILL_26,             //기본 스킬은 초기화 할 수 없습니다.
	ERR_PARTY_00,             //파티를 생성하지 못했습니다.
	ERR_PARTY_01,             //파티에서 탈퇴하지 못했습니다.
	ERR_PARTY_03,             //파티 정보가 잘못되었습니다.
	ERR_PARTY_07,             //초대할 수 없는 상태입니다.
	ERR_PARTY_09,             //소속된 파티가 없습니다.
	ERR_PARTY_12,             //이미 파티에 가입되어 있습니다.
	ERR_PARTY_13,             //대전에서는 파티를 생성할 수 없습니다.
	ERR_PARTY_14,             //대전에서는 파티에 입장할 수 없습니다.
	ERR_PARTY_15,             //파티 인원이 다 찼습니다.
	ERR_PARTY_16,             //파티에 가입하지 못했습니다.
	ERR_PARTY_17,             //삭제된 파티입니다.
	ERR_PARTY_18,             //파티 입장이 거절되었습니다.
	ERR_PARTY_19,             //파티 초대를 거부하였습니다.
	ERR_PARTY_20,             //준비 할 수 없는 장소입니다.
	ERR_PARTY_21,             //준비 상태 변경 실패입니다.
	ERR_PARTY_22,             //던전 선택이 잘못되었습니다.
	ERR_PARTY_23,             //게임 시작이 실패하였습니다.
	ERR_PARTY_25,             //게임 타입이 잘못되었습니다.
	ERR_PARTY_26,             //파티장이 아닙니다.
	ERR_PARTY_27,             //파티장을 변경하지 못했습니다.
	ERR_PARTY_28,             //파티 인원을 변경하지 못했습니다.
	ERR_PARTY_29,             //이 던전 게이트 소속 던전이 아닙니다.
	ERR_PARTY_30,             //공개 설정을 변경하지 못했습니다.
	ERR_PARTY_31,             //새로 임명될 파티장이 현재의 던전을 수행할 수 없습니다.
	ERR_PARTY_32,             //파티장은 레디 상태 변경을 할 수 없습니다.
	ERR_PARTY_33,             //채팅 상대자가 없습니다.
	ERR_PARTY_34,             //파티 리스트 요청 타입이 이상합니다.
	ERR_PARTY_35,             //레벨 제한 설정을 변경하지 못했습니다.
	ERR_PARTY_38,             //비공개 파티입니다.
	ERR_RESURRECT_00,             //부활석이 없습니다.
	ERR_RESURRECT_01,             //이미 부활 하였습니다.
	ERR_RESURRECT_02,             //부활 실패입니다.
	ERR_RESURRECT_03,             //이 던전에서는 부활석을 사용할 수 없습니다.
	ERR_COMMUNITY_OPT_01,             //없는 상태 값을 업데이트 하려고 하였습니다.
	ERR_COMMUNITY_OPT_02,             //커뮤니티 옵션 값을 받아오지 못했습니다.
	ERR_BLACKLIST_00,             //더이상 블랙리스트에 등록할 수 없습니다.
	ERR_BLACKLIST_01,             //이미 블랙리스트에 등록된 유저입니다.
	ERR_BLACKLIST_02,             //이미 블랙리스트에서 삭제된 유저입니다.
	ERR_BLACKLIST_03,             //존재하지 않는 유저입니다.
	ERR_BLACKLIST_04,             //블랙리스트에서 삭제하지 못했습니다.
	ERR_BLACKLIST_05,             //자기 자신을 블랙리스트에 등록할 수 없습니다.
	ERR_BLACKLIST_06,             //자기 자신은 블랙리스트에서 삭제할 수 없습니다.
	ERR_BLACKLIST_07,             //현재 접속 중인 유저만 블랙리스트에 등록할 수 있습니다.
	ERR_KNM_00,             //자신의 캐릭터를 추가할 수 없습니다.
	ERR_KNM_01,             //친구 추가 제한 시간이 지나지 않았습니다.
	ERR_KNM_02,             //캐릭터를 찾을 수 없습니다.
	ERR_KNM_03,             //내 캐릭터가 초대할 수 없는 상태입니다.
	ERR_KNM_04,             //상대방 캐릭터가 초대 받을 수 없는 상태입니다.
	ERR_KNM_05,             //상대방 캐릭터가 같이 할 수 없는 상태입니다.
	ERR_KNM_06,             //친구로 초대할 수 없습니다.
	ERR_TRADE_00,             //개인 거래를 생성하지 못했습니다.
	ERR_TRADE_01,             //개인 거래에서 이탈하지 못했습니다.
	ERR_TRADE_02,             //개인 거래 유저를 검색하지 못했습니다.
	ERR_TRADE_03,             //등록하려는 아이템 정보가 이상합니다.
	ERR_TRADE_04,             //등록하려는 ED 정보가 이상합니다.
	ERR_TRADE_05,             //거래 아이템이 존재하지 않습니다.
	ERR_TRADE_06,             //거래 ED가 부족합니다.
	ERR_TRADE_07,             //인벤토리에 있는 아이템만 등록할 수 있습니다.
	ERR_TRADE_09,             //인벤토리 공간이 부족합니다.
	ERR_TRADE_11,             //귀속 아이템은 거래할 수 없습니다.
	ERR_TRADE_12,             //거래채팅을 할 수 없는 장소입니다.
	ERR_TRADE_13,             //자기 자신과 거래할 수 없습니다.
	ERR_TRADE_14,             //내구도가 소모된 아이템은 거래할 수 없습니다. 아이템을 수리하고 거래해주세요.
	ERR_TRADE_15,             //이미 등록한 아이템입니다.
	ERR_TRADE_16,             //등록되지 않은 아이템을 삭제하려고 하였습니다.
	ERR_TRADE_17,             //거래 가능한 아이템 종류는 최대 10가지입니다.
	ERR_TRADE_18,             //거래가 성사되면 상대방의 ED 보유량이 최대치를 초과합니다. 거래 ED량을 줄여주세요.
	NOT_REQUEST_TRADE_REPLY_00,             //개인 거래 요청을 수락하였습니다.
	NOT_REQUEST_TRADE_REPLY_01,             //이미 다른 사람과 거래 중입니다.
	NOT_REQUEST_TRADE_REPLY_02,             //개인 거래 요청을 거절하였습니다.
	NOT_REQUEST_TRADE_REPLY_03,             //거래할 수 없는 상태입니다.
	NOT_REQUEST_TRADE_REPLY_04,             //서버 오류로 거래를 할 수 없습니다.
	ERR_DUNGEON_REQUIRE_ITEM_00,             //던전 입장에 필요한 아이템을 가지고 있지 않습니다.
	ERR_TC_01,             //훈련소 입장 조건을 만족하지 않습니다.
	ERR_TC_03,             //훈련소를 진행할 던전이 ID가 없습니다.
	ERR_TC_04,             //보상 처리 중 오류가 발생하였습니다.
	ERR_RANDOM_ITEM_00,             //선택한 아이템은 인벤토리에 없는 아이템입니다.
	ERR_RANDOM_ITEM_01,             //선택한 아이템은 큐브 아이템이 아닙니다.
	ERR_RANDOM_ITEM_02,             //이 캐릭터로는 사용할 수 없는 아이템입니다.
	ERR_RANDOM_ITEM_04,             //열쇠가 부족하여 큐브를 열 수 없습니다.
	ERR_RANDOM_ITEM_05,             //큐브를 열 수 없습니다.
	ERR_RANDOM_ITEM_06,             //아이템을 생성하지 못했습니다.
	ERR_RANDOM_ITEM_07,             //해당 큐브를 열기위한 인벤토리의 공간이 부족합니다.
	ERR_USER_REGISTRATION_00,             //이중 접속이 감지되었습니다.
	ERR_USER_REGISTRATION_01,             //유저 UID가 이상합니다.
	ERR_USER_REGISTRATION_02,             //유저 ID가 이상합니다.
	ERR_USER_REGISTRATION_03,             //캐릭터 UID가 이상합니다.
	ERR_USER_REGISTRATION_04,             //캐릭터 닉네임이 이상합니다.
	ERR_USER_REGISTRATION_05,             //게임 서버 UID가 이상합니다.
	ERR_USER_REGISTRATION_06,             //유저 정보 포인터가 이상합니다.
	ERR_USER_REGISTRATION_07,             //게임 서버 포인터가 이상합니다.
	ERR_USER_REGISTRATION_08,             //유저 UID 검색이 실패하였습니다.
	ERR_USER_REGISTRATION_09,             //메신져 고유 번호가 이상합니다.
	ERR_MANUFACTURE_00,             //아이템 제조를 할 수 없습니다.
	ERR_MANUFACTURE_02,             //주문서 아이템이 아닙니다.
	ERR_MANUFACTURE_03,             //ED가 부족하여 제조를 할 수 없습니다.
	ERR_MANUFACTURE_05,             //재료 아이템이 부족합니다.
	ERR_MANUFACTURE_10,             //제조를 하기 위해서는 해당 인벤토리에 빈칸이 1개 이상 있어야 합니다.
	ERR_MANUFACTURE_11,             //아이템을 생성하지 못했습니다.
	ERR_KNP_00,             //HackShield 인증이 실패하였습니다.
	ERR_KNP_01,             //HackShield 업데이트로 인하여 재시작해야 합니다.
	ERR_KNP_02,             //HackShield 인증이 실패하였습니다.
	ERR_NX_WEB_SERVICE_00,             //이미 사용한 쿠폰이거나 쿠폰 번호가 잘못 입력되었습니다. 쿠폰은 계정당 1회만 사용 가능합니다.
	ERR_NX_WEB_SERVICE_01,             //쿠폰 번호 30자를 입력해주세요.
	ERR_NX_COUPON_00,             //이미 사용된 쿠폰입니다.
	ERR_NX_COUPON_01,             //기간이 만료된 쿠폰입니다.
	ERR_NX_COUPON_02,             //쿠폰 번호가 올바르지 않습니다.
	ERR_NX_COUPON_03,             //이 쿠폰은 계정당 한 번만 사용 가능합니다.
	ERR_NX_COUPON_04,             //쿠폰을 사용하지 못했습니다.
	ERR_NX_COUPON_05,             //재구매 일수 미만입니다.
	ERR_NX_SHOP_00,             //아이템 리스트를 불러올 수 없습니다.
	ERR_NX_SHOP_01,             //가져오려는 아이템 정보가 이상합니다.
	ERR_NX_SHOP_02,             //인벤토리에 공간이 부족합니다.
	ERR_NX_SHOP_03,             //구매한 캐쉬 아이템을 가져오지 못했습니다.
	ERR_NX_SHOP_04,             //잔액 정보를 조회하지 못했습니다.
	ERR_NX_SHOP_05,             //회원 정보를 찾을 수 없습니다.
	ERR_NX_SHOP_06,             //차단된 계정입니다.
	ERR_NX_SHOP_07,             //다른 캐릭터 아이템은 인벤토리로 가져올 수 없습니다.
	ERR_NX_SHOP_08,             //동일한 기간제 아이템을 이미 가지고 있습니다.
	ERR_NX_SHOP_09,             //아이템을 가져오는 중입니다. 잠시만 기다려 주세요.
	ERR_STATISTICS_00,             //잘못된 프레임 평균 값입니다.
	ERR_RESTORE_NICK_NAME_00,             //해당 캐릭터가 존재하지 않습니다.
	ERR_RESTORE_NICK_NAME_01,             //닉네임 복구 대상이 아닙니다.
	ERR_RESTORE_NICK_NAME_02,             //동일한 닉네임이 이미 존재합니다.
	ERR_RESTORE_NICK_NAME_03,             //이 닉네임은 일정 기간 사용할 수 없습니다.
	ERR_RESTORE_NICK_NAME_04,             //닉네임 복구 작업이 실패하였습니다.
	ERR_RESTORE_NICK_NAME_05,             //유효하지 않은 닉네임입니다.
	ERR_RESTORE_NICK_NAME_06,             //닉네임 길이가 올바르지 않습니다.
	ERR_CHANGE_NICK_NAME_00,             //닉네임을 변경하지 못했습니다.
	ERR_PERSONAL_SHOP_00,             //개인 상점을 생성하지 못했습니다.
	ERR_PERSONAL_SHOP_01,             //광장에서만 상점을 생성할 수 있습니다.
	ERR_PERSONAL_SHOP_03,             //광장 유저 정보를 찾을 수 없어서 상점을 시작 할 수 없습니다.
	ERR_PERSONAL_SHOP_04,             //개인 상점 참여 인원이 많으므로 잠시 후에 이용해 주세요.
	ERR_PERSONAL_SHOP_05,             //개인 상점에 참여하지 못했습니다.
	ERR_PERSONAL_SHOP_06,             //판매 등록 하려는 아이템의 수량 정보가 잘못되었습니다.
	ERR_PERSONAL_SHOP_07,             //아이템이 이미 판매 등록된 상태입니다.
	ERR_PERSONAL_SHOP_08,             //개설자가 아닌데 판매 물품을 등록하려고 하였습니다.
	ERR_PERSONAL_SHOP_09,             //판매할 수 있는 물품 개수보다 등록된 아이템 개수가 많습니다.
	ERR_PERSONAL_SHOP_10,             //상점 개설 비용이 부족하므로 상점을 개설할 수 없습니다.
	ERR_PERSONAL_SHOP_11,             //상점 개설 준비가 아직 안되었습니다.
	ERR_PERSONAL_SHOP_12,             //상점에서 퇴장하지 못했습니다.
	ERR_PERSONAL_SHOP_14,             //구입하려는 아이템 수량이 판매 물품보다 많습니다.
	ERR_PERSONAL_SHOP_15,             //구입하려는 아이템이 없습니다.
	ERR_PERSONAL_SHOP_16,             //ED가 부족하여 물품을 구매할 수 없습니다.
	ERR_PERSONAL_SHOP_17,             //인벤토리에 공간이 없어서 구매할 수 없습니다.
	ERR_PERSONAL_SHOP_20,             //개설자를 찾지 못하여 거래하지 못했습니다.
	ERR_PERSONAL_SHOP_21,             //거래 중에는 상점을 열 수 없습니다.
	ERR_PERSONAL_SHOP_22,             //이미 개인 상점이 열려있습니다.
	ERR_PERSONAL_SHOP_23,             //최대 등록할 수 있는 ED량을 초과하였습니다.
	ERR_PERSONAL_SHOP_24,             //최대 구매할 수 있는 ED량을 초과하였습니다.
	ERR_PERSONAL_SHOP_25,             //구매하려는 아이템 정보가 이상합니다.
	ERR_RESOLVE_ITEM_00,             //선택한 아이템은 인벤토리에 없는 아이템입니다.
	ERR_RESOLVE_ITEM_01,             //분해할 수 있는 아이템이 아닙니다.
	ERR_RESOLVE_ITEM_02,             //분해 실패하였습니다. 다시 분해를 해주세요.
	ERR_RESOLVE_ITEM_03,             //장비와 악세사리만 분해가 가능합니다.
	ERR_RESOLVE_ITEM_04,             //분해를 할 수 없습니다.
	ERR_RESOLVE_ITEM_05,             //아이템 생성이 실패하였습니다.
	ERR_RESOLVE_ITEM_06,             //인벤토리 공간이 부족하여 분해를 할 수 없습니다.
	ERR_RESOLVE_ITEM_08,             //기간제 아이템은 분해할 수 없습니다.
	ERR_ENCHANT_ITEM_00,             //강화할 아이템이 인벤토리에 없습니다.
	ERR_ENCHANT_ITEM_01,             //아이템 정보가 존재하지 않습니다.
	ERR_ENCHANT_ITEM_02,             //무기나 방어구만 강화할 수 있습니다.
	ERR_ENCHANT_ITEM_03,             //강화할 수 있는 아이템이 아닙니다.
	ERR_ENCHANT_ITEM_04,             //더 이상 강화할 수 없습니다.
	ERR_ENCHANT_ITEM_05,             //강화석이 존재하지 않습니다.
	ERR_ENCHANT_ITEM_06,             //강화를 진행할 수 없습니다.
	ERR_ENCHANT_ITEM_07,             //강화에 필요한 ED가 부족합니다.
	ERR_ENCHANT_ITEM_08,             //장착중인 아이템은 강화할 수 없습니다.
	ERR_ENCHANT_ITEM_09,             //플루오르 스톤이 인벤토리에 없습니다.
	ERR_ENCHANT_ITEM_10,             //플루오르 스톤을 사용할 수 없습니다.
	ERR_ENCHANT_ITEM_11,             //강화 레벨 10부터는 강화시 플루오르 스톤을 사용할 수 없습니다.
	ERR_ENCHANT_ITEM_12,             //사용불가 아이템은 강화할 수 없습니다.
	ERR_ENCHANT_ITEM_13,             //강화 업데이트가 실패하였습니다.
	ERR_RESTORE_ITEM_00,             //복구할 아이템이 인벤토리에 없습니다.
	ERR_RESTORE_ITEM_01,             //아이템 정보가 존재하지 않습니다.
	ERR_RESTORE_ITEM_02,             //복구할 수 있는 아이템이 아닙니다.
	ERR_RESTORE_ITEM_03,             //사용불가 아이템만 복구가 가능합니다.
	ERR_RESTORE_ITEM_04,             //복구석이 부족하여 복구할 수 없습니다.
	ERR_RESTORE_ITEM_05,             //장착중인 아이템은 복구할 수 없습니다.
	ERR_RESTORE_ITEM_06,             //사용 불가 아이템에는 이용할 수 없는 기능입니다.
	ERR_ENCHANT_RESULT_00,             //강화 성공입니다.
	ERR_ENCHANT_RESULT_01,             //변화 없음입니다.
	ERR_ENCHANT_RESULT_02,             //1단계 하락입니다.
	ERR_ENCHANT_RESULT_03,             //강화 단계 초기화입니다.
	ERR_ENCHANT_RESULT_04,             //아이템 사용 불가입니다.
	ERR_SOCKET_ITEM_00,             //인벤토리에 없는 아이템입니다.
	ERR_SOCKET_ITEM_01,             //아이템 정보가 존재하지 않습니다.
	ERR_SOCKET_ITEM_02,             //수수료가 부족합니다.
	ERR_SOCKET_ITEM_03,             //무기나 방어구만 소켓이 존재합니다.
	ERR_SOCKET_ITEM_04,             //해당 장비에 대한 마법석 정보가 존재하지 않습니다.
	ERR_SOCKET_ITEM_05,             //인벤토리에 마법석이 없습니다.
	ERR_SOCKET_ITEM_06,             //소켓을 사용할 수 없습니다.
	ERR_SOCKET_ITEM_07,             //소켓에 이미 마법석이 꽂혀있습니다.
	ERR_SOCKET_ITEM_08,             //소켓에 이미 마법석이 제거되어 있습니다.
	ERR_SOCKET_ITEM_09,             //존재하지 않는 소켓 옵션입니다.
	ERR_RECOMMEND_USER_00,             //존재하지 않는 닉네임 입니다.
	ERR_RECOMMEND_USER_01,             //자기 자신은 추천할 수 없습니다.
	ERR_REWARD_TO_POST_00,             //보상을 받을 대상이 존재하지 않습니다.
	ERR_TUTORIAL_00,             //지우려는 사제 데이터가 DB에 없습니다.
	ERR_TUTORIAL_01,             //제자 리스트를 받아오지 못했습니다.
	ERR_TUTORIAL_02,             //레벨이 낮아 정보 제공을 할 수 없습니다.
	ERR_TUTORIAL_03,             //레벨이 낮아 제자를 받을 수 없습니다.
	ERR_TUTORIAL_04,             //제자는 3명까지 등록 가능합니다.
	ERR_TUTORIAL_05,             //이미 스승이 있습니다.
	ERR_TUTORIAL_06,             //이미 제자로 등록되어 있습니다.
	ERR_BUY_CASH_ITEM_00,             //구입하려는 상품 정보가 이상합니다.
	ERR_BUY_CASH_ITEM_01,             //상품을 구매할 수 없습니다.
	ERR_BUY_CASH_ITEM_02,             //중복 결제입니다.
	ERR_BUY_CASH_ITEM_03,             //상품 총 구매 수량을 초과하였습니다.
	ERR_BUY_CASH_ITEM_04,             //상품 구매 개수를 초과하였습니다.
	ERR_BUY_CASH_ITEM_05,             //이 상품을 아직은 다시 구매할 수 없습니다.
	ERR_BUY_CASH_ITEM_06,             //카트가 없습니다.
	ERR_BUY_CASH_ITEM_07,             //사용 제한된 게임입니다.
	ERR_BUY_CASH_ITEM_08,             //총 판매 수량 제한으로 인하여 구매할 수 없습니다.
	ERR_BUY_CASH_ITEM_09,             //주문 수량이 제한되었습니다.
	ERR_BUY_CASH_ITEM_10,             //이 상품을 다시 구매할 수 있는 시기가 아직 안되었습니다.
	ERR_BUY_CASH_ITEM_11,             //선물할 수 없는 상품입니다.
	ERR_BUY_CASH_ITEM_12,             //주문 수량이 제한되어 있습니다.
	ERR_BUY_CASH_ITEM_13,             //받는 사람이 이 상품을 다시 구매할 수 있는 시기가 아직 안되었습니다.
	ERR_BUY_CASH_ITEM_14,             //블록된 유저입니다.
	ERR_BUY_CASH_ITEM_15,             //회원 정보를 찾을 수 없습니다.
	ERR_BUY_CASH_ITEM_16,             //잔액이 부족합니다.
	ERR_BUY_CASH_ITEM_17,             //점검중입니다.
	ERR_BUY_CASH_ITEM_18,             //소지할 수 있는 최대 부활석 개수를 초과하게 됩니다.
	ERR_BUY_CASH_ITEM_19,             //인벤토리 슬롯 개수를 더 이상 확장할 수 없습니다.
	ERR_BUY_CASH_ITEM_20,             //자기 자신에게는 선물할 수 없습니다.
	ERR_BUY_CASH_ITEM_21,             //잘못된 비밀 번호를 입력하였습니다.
	ERR_BUY_CASH_ITEM_22,             //존재하지 않는 닉네임입니다.
	ERR_BUY_CASH_ITEM_23,             //체험 ID 유저에게는 선물할 수 없습니다.
	ERR_BUY_CASH_ITEM_24,             //선물할 수 있는 한도액이 초과되었습니다.
	ERR_BUY_CASH_ITEM_25,             //비밀 번호를 입력해주세요.
	ERR_BUY_CASH_ITEM_26,             //넥슨 계정 인증 실패입니다.
	ERR_BUY_CASH_ITEM_27,             //선물할 수 없는 상품입니다.
	ERR_BUY_CASH_ITEM_28,             //만 14세 미만은 이 기능을 사용할 수 없습니다.
	ERR_BUY_CASH_ITEM_30,             //캐릭터 슬롯 확장 상품을 구매할 수 없습니다. 슬롯 확장은 9개까지 가능합니다.
	ERR_BUY_CASH_ITEM_31,             //캐릭터 슬롯을 확장하지 못했습니다.
	ERR_BUY_CASH_ITEM_32,             //이미 해당 기능을 사용 중입니다.
	ERR_RESET_SKILL_00,             //존재하지 않는 스킬입니다.
	ERR_RESET_SKILL_01,             //스킬 초기화 아이템이 존재하지 않습니다.
	ERR_RESET_SKILL_02,             //스킬 초기화 아이템을 DB에서 삭제하지 못했습니다.
	ERR_RESET_SKILL_03,             //스킬 초기화 DB쿼리 결과 실패가 반환되었습니다.
	ERR_RESET_SKILL_04,             //초기화 할 수 없는 스킬입니다.
	ERR_RESET_SKILL_05,             //스킬 슬롯에 장착 중인 스킬은 초기화 할 수 없습니다.
	ERR_WISH_LIST_00,             //찜 리스트에 등록할 수 없는 상품입니다.
	ERR_WISH_LIST_02,             //찜 리스트에 더 이상 상품을 등록할 수 없습니다.
	ERR_GUEST_USER_00,             //체험 ID에서는 이용 하실 수 없는 기능입니다.
	ERR_GUEST_USER_01,             //체험 ID 유저는 추천인에 등록 할 수 없습니다.
	ERR_GUEST_USER_02,             //체험 ID를 이용하실 수 있는 레벨인 20렙이 되셨습니다. 계속 게임을 하시려면 넥슨ID로 전환해주세요.
	ERR_GUEST_USER_03,             //체험 ID 유저는 메신저 친구로 등록할 수 없습니다.
	ERR_HACK_USER_00,             //해킹 리스트에 등록된 유저가 아닙니다.
	ERR_HACK_USER_01,             //해킹 유저 동의 데이터를 디비 업데이트 하지 못했습니다.
	ERR_HACK_USER_02,             //해킹 신고 유저인데 동의를 하지 않았습니다.
	ERR_ATTRIB_ENCHANT_00,             //인벤토리에 존재하지 않는 아이템입니다.
	ERR_ATTRIB_ENCHANT_01,             //속성 강화를 할 수 없습니다.
	ERR_ATTRIB_ENCHANT_02,             //이미 속성 강화된 슬롯입니다.
	ERR_ATTRIB_ENCHANT_03,             //이미 속성 강화가 제거된 슬롯입니다.
	ERR_ATTRIB_ENCHANT_04,             //속성 강화를 위한 수수료가 부족합니다.
	ERR_ATTRIB_ENCHANT_05,             //속성 강화를 위한 엘의 조각이 부족합니다.
	ERR_ATTRIB_ENCHANT_06,             //아이템 정보가 이상합니다.
	ERR_ATTRIB_ENCHANT_07,             //무기와 방어구만 속성강화를 할 수 있습니다.
	ERR_IDENTIFY_00,             //선택된 아이템은 판별할 수 없는 아이템입니다.
	ERR_IDENTIFY_01,             //판별기가 부족하여 아이템을 감정할 수 없습니다.
	ERR_IDENTIFY_02,             //해당 아이템을 판별하기 위한 인벤토리 공간이 부족합니다.
	ERR_IDENTIFY_03,             //해당 아이템을 판별할 수 없습니다.
	ERR_IDENTIFY_04,             //아이템 생성 실패.
	ERR_POST_LETTER_00,             //우편함 정보를 얻을 수 없습니다.
	ERR_POST_LETTER_01,             //선택하신 아이템은 편지에 첨부할 수 없습니다.
	ERR_POST_LETTER_02,             //입력하신 ED는 편지에 첨부할 수 없습니다.
	ERR_POST_LETTER_03,             //편지를 전송하지 못했습니다.
	ERR_POST_LETTER_04,             //존재하지 않는 편지입니다.
	ERR_POST_LETTER_05,             //인벤토리에 여유공간이 없어서 첨부된 물품을 이동할 수 없습니다. 여유공간을 확보 하신 후 다시 시도 해 주시기 바랍니다.
	ERR_POST_LETTER_06,             //첨부 물품을 가져오지 못했습니다.
	ERR_POST_LETTER_07,             //존재하지 않는 아이템을 첨부하려고 하였습니다.
	ERR_POST_LETTER_08,             //완전히 수리된 아이템만 첨부할 수 있습니다.
	ERR_POST_LETTER_10,             //존재하는 닉네임이 아닙니다.
	ERR_POST_LETTER_11,             //동일한 캐릭터 간에는 편지를 보낼 수 없습니다.
	ERR_POST_LETTER_12,             //수수료 부족으로 편지를 발송할 수 없습니다.
	ERR_POST_LETTER_13,             //상대방의 우편함에 여유 공간이 없어 편지를 보낼 수 없습니다.
	ERR_POST_LETTER_14,             //이미 삭제된 편지를 선택하였습니다.
	ERR_POST_LETTER_15,             //상대방이 편지 받기를 거부하였습니다.
	ERR_POST_LETTER_16,             //귀속 아이템은 편지에 첨부할 수 없습니다.
	ERR_POST_LETTER_17,             //체험 ID 유저에게는 편지를 보내실 수 없습니다.
	ERR_POST_BLACK_LIST_00,             //존재하지 않는 닉네임입니다.
	ERR_POST_BLACK_LIST_01,             //이미 블랙리스트에 등록된 닉네임입니다.
	ERR_POST_BLACK_LIST_02,             //블랙리스트에 등록하지 못했습니다.
	ERR_POST_BLACK_LIST_03,             //이미 삭제된 닉네임입니다.
	ERR_POST_BLACK_LIST_04,             //블랙리스트에서 삭제하지 못했습니다.
	ERR_TITLE_00,             //보유하고 있는 칭호가 아닙니다.
	ERR_TITLE_01,             //이미 장착중인 칭호입니다.
	ERR_TITLE_02,             //사용 가능한 기간이 지난 칭호입니다.
	ERR_TITLE_03,             //존재하지 않는 칭호입니다.
	ERR_TITLE_04,             //칭호를 획득하지 못했습니다.
	ERR_TITLE_05,             //이미 보유 중인 칭호입니다.
	ERR_USE_ITEM_IN_INVENTORY_00,             //아이템을 사용할 수 없습니다.
	ERR_USE_ITEM_IN_INVENTORY_01,             //존재하지 않는 아이템입니다.
	ERR_USE_ITEM_IN_INVENTORY_02,             //캐릭터 레벨이 부족하여 아이템을 사용할 수 없습니다.
	ERR_USE_ITEM_IN_INVENTORY_03,             //인벤토리에서 사용 불가능한 아이템입니다.
	ERR_ATTACH_ITEM_00,             //인벤토리에 없는 아이템 입니다.
	ERR_ATTACH_ITEM_01,             //부여 아이템이 아닙니다.
	ERR_ATTACH_ITEM_02,             //부여가 가능한 아이템이 아닙니다.
	ERR_ATTACH_ITEM_03,             //강화 레벨 부여중 업데이트가 실패하였습니다.
	ERR_ITEM_EXCHANGE_00,             //아이템을 교환할 수 없습니다.
	ERR_ITEM_EXCHANGE_01,             //인벤토리에 없는 아이템 입니다.
	ERR_ITEM_EXCHANGE_02,             //이 아이템은 교환대상이 아닙니다.
	ERR_ITEM_EXCHANGE_04,             //교환 아이템 생성되지 않았습니다.
	NOT_LEAVE_FIELD_REASON_00,             //다른 필드로 이동하기 위한 이탈입니다.
	NOT_LEAVE_FIELD_REASON_01,             //방으로 입장하기 위한 이탈입니다.
	NOT_LEAVE_FIELD_REASON_02,             //시스템에서 유저를 이탈 처리하였습니다.
	ERR_FIELD_00,             //마을에 입장할 수 없습니다.
	ERR_FIELD_01,             //마을에서 나가지 못했습니다.
	ERR_FIELD_04,             //유저 리스트를 받아올 수 없습니다.
	ERR_FIELD_05,             //레벨이 낮아 마을에 입장할 수 없습니다.
	ERR_FIELD_06,             //마을에 입장하기 위한 던전을 클리어하지 않았습니다.
	ERR_CHANNEL_CHANGE_01,             //존재하지 않는 채널입니다.
	ERR_CHANNEL_CHANGE_03,             //채널 이동 성공으로 인한 접속 종료입니다.
	ERR_CHANNEL_CHANGE_05,             //채널이동을 위한 유저 등록이 실패하였습니다.
	ERR_CHANNEL_CHANGE_06,             //채널이동을 위한 유저 등록이 되어 있지 않습니다.
	ERR_CHANNEL_CHANGE_07,             //이동하려는 채널과 현재 채널이 같습니다.
	ERR_CHANNEL_CHANGE_08,             //수용 인원이 가득 차서 이동 할 수 없습니다.
	ERR_CHANNEL_CHANGE_09,             //마을 또는 필드에서만 채널 이동 기능을 이용하실 수 있습니다.
	ERR_CHANNEL_CHANGE_10,             //잠시 후에 채널 이동을 이용해주시기 바랍니다.
	ERR_GM_CHAR_DENY_00,             //GM 캐릭터는 사용할 수 없는 기능입니다.
	ERR_UPDATE_FRIEND_00,             //캐릭터가 선택되지 않았습니다.
	ERR_UPDATE_FRIEND_01,             //친구가 접속 중이 아닙니다.
	ERR_WRONG_STATE_00,             //잘못된 상태입니다.
	ERR_TIME_DROP_EVENT_00,             //이벤트로 드롭된 아이템을 DB업데이트하지 못했습니다.
	ERR_TIME_DROP_EVENT_01,             //이미 시간 이벤트 보상을 받았습니다.
	ERR_MESSENGER_00,             //이미 친구로 등록되어있는 캐릭터입니다.
	ERR_MESSENGER_01,             //친구 캐릭터의 상태가 올바르지 않습니다.
	ERR_MESSENGER_02,             //나를 친구로 초대한 캐릭터가 아닙니다.
	ERR_MESSENGER_03,             //이 캐릭터는 친구가 아닙니다.
	ERR_MESSENGER_04,             //그룹이 존재하지 않습니다.
	ERR_MESSENGER_05,             //메세지의 길이가 너무 깁니다.
	ERR_MESSENGER_06,             //현재 그룹과 이동시키려고 하는 그룹이 같습니다.
	ERR_MESSENGER_07,             //그룹을 더 이상 생성할 수 없습니다.
	ERR_MESSENGER_08,             //동일한 그룹이 이미 있습니다.
	ERR_MESSENGER_09,             //동일한 그룹 이름입니다.
	ERR_TUTORIAL_07,             //제자가 7일동안 접속을 하지 않아 사제관계가 종료 되었습니다.
	ERR_TUTORIAL_08,             //제자가 레벨업을 완료하여 사제관계가 종료 되었습니다.
	ERR_TUTORIAL_09,             //캐릭터가 삭제되어 사제관계가 종료 되었습니다.
	ERR_TUTORIAL_10,             //유저에 의해 사제관계가 종료되었습니다.
	ERR_TUTORIAL_11,             //사제관계 종료를 실패하였습니다.
	ERR_MESSENGER_10,             //친구 기능 관련 DB 작업 오류입니다.
	ERR_MESSENGER_11,             //그룹 이름이 너무 깁니다.
	ERR_MESSENGER_12,             //친구 리스트가 가득 차서 친구로 초대할 수 없습니다.
	ERR_MESSENGER_13,             //같은 계정의 캐릭터에게는 친구 초대를 할 수 없습니다.
	ERR_MESSENGER_14,             //상대방의 친구 리스트가 가득 차서 친구로 초대할 수 없습니다.
	ERR_MESSENGER_15,             //상대방이 친구 초대를 거부한 상태입니다.
	ERR_ODBC_01,             //쿼리 호출 실패입니다.
	ERR_ROOM_37,             //대회채널에서는 난입할 수 없습니다.
	ERR_ROOM_38,             //대회기간중에는 방에 인원이 가득차야만 게임이 시작 가능합니다.
	ERR_POST_LETTER_18,             //제목의 길이가 너무 깁니다.
	ERR_POST_LETTER_19,             //편지 내용의 길이가 너무 깁니다.
	ERR_PARTY_39,             //초대한 유저는 현재의 던전을 수행할 수 없습니다.
	ERR_USE_ITEM_IN_INVENTORY_04,             //이미 봉인이 해제된 스킬입니다.
	ERR_SKILL_27,             //스킬 봉인 해제를 실패하였습니다.
	ERR_BUY_CASH_ITEM_33,             //이 상품을 구매하실 수 없는 캐릭터 직업입니다.
	ERR_BUY_CASH_ITEM_34,             //이미 봉인이 해제된 스킬의 상품입니다.
	ERR_BUY_CASH_ITEM_35,             //선택하신 상품과 같은 기능의 아이템이 인벤토리에 있어 상품을 구매하실 수 없습니다.
	ERR_GUEST_USER_04,             //체험 ID 유저는 헤니르 시공 던전 파티에 초대할 수 없습니다.
	ERR_VERIFY_17,             //인증 방식이 맞지 않습니다.
	ERR_BANK_00,             //은행에 보관된 아이템은 분해할 수 없습니다.
	ERR_BANK_01,             //은행에 보관된 아이템은 소켓을 추가 할 수 없습니다.
	ERR_BANK_02,             //은행에서는 사용할 수 없는 기능입니다.
	ERR_BUY_CASH_ITEM_36,             //은행 슬롯 갯수를 더이상 확장할 수 없습니다.
	ERR_BUY_CASH_ITEM_37,             //은행 회원권은 1개이상 구매할 수 없습니다.
	ERR_BUY_CASH_ITEM_38,             //구매할 수 없는 은행 회원권 입니다.
	ERR_BUY_CASH_ITEM_39,             //인벤토리에 루리엘 신용보증서가 있으면 플래티넘 회원권 구매를 할 수 없습니다. 루리엘 신용보증서를 사용해주세요.
	ERR_VERIFY_18,             //패스워드가 존재하지 않습니다.
	ERR_VERIFY_19,             //패스워드 갱신을 할 수 없습니다.
	ERR_VERIFY_20,             //아이디 또는 패스워드가 올바르지 않습니다.
	ERR_VERIFY_21,             //인증 과정에서 시스템 오류가 발생하였습니다.
	ERR_SEAL_ITEM_00,             //존재하지 않는 아이템입니다.
	ERR_SEAL_ITEM_01,             //아이템 정보를 얻을 수 없습니다.
	ERR_SEAL_ITEM_02,             //봉인할 수 없는 아이템 종류입니다.
	ERR_SEAL_ITEM_03,             //장착중인 아이템은 봉인할 수 없습니다.
	ERR_SEAL_ITEM_04,             //이미 봉인된 아이템 입니다.
	ERR_SEAL_ITEM_05,             //수리되지 않은 아이템은 봉인할 수 없습니다.
	ERR_SEAL_ITEM_06,             //기간제 아이템은 봉인할 수 없습니다.
	ERR_SEAL_ITEM_07,             //파괴된 아이템은 봉인할 수 없습니다.
	ERR_SEAL_ITEM_08,             //거래가능 아이템은 봉인할 수 없습니다.
	ERR_SEAL_ITEM_09,             //봉인 가능한 횟수가 남지 않아 봉인할 수 없습니다.
	ERR_SEAL_ITEM_10,             //선택한 봉인주문서 정보가 이상합니다.
	ERR_SEAL_ITEM_11,             //봉인을 실패하였습니다.
	ERR_SEAL_ITEM_12,             //봉인 주문서가 부족하여 봉인할 수 없습니다.
	ERR_SEAL_ITEM_13,             //장착중인 아이템은 봉인해제를 할 수 없습니다.
	ERR_SEAL_ITEM_14,             //이미 봉인해제된 아이템 입니다.
	ERR_SEAL_ITEM_15,             //봉인해제를 실패하였습니다.
	ERR_SEAL_ITEM_16,             //봉인정보 DB업데이트 실패입니다.
	ERR_ENCHANT_ITEM_14,             //봉인된 아이템은 강화할 수 없습니다.
	ERR_GASH_00,             //캐쉬 포인트 조회 중에 시스템 에러가 발생하였습니다.
	ERR_GASH_01,             //이 계정은 활동이 제한된 계정입니다.
	ERR_GASH_02,             //계정이 존재하지 않습니다.
	ERR_GASH_03,             //계정이 올바르지 않습니다.
	ERR_GASH_04,             //쿠폰 번호가 올바르지 않습니다.
	ERR_GASH_05,             //이미 사용된 쿠폰입니다.
	ERR_GASH_06,             //이 종류의 쿠폰은 한 번만 사용할 수 있습니다.
	ERR_GASH_07,             //사용 기한이 지난 쿠폰입니다.
	ERR_GASH_08,             //쿠폰 사용 중에 시스템 오류가 발생하였습니다.
	ERR_GASH_09,             //포인트가 부족합니다.
	ERR_GASH_10,             //아이템 구입 중에 시스템 오류가 발생하였습니다.
	ERR_BUY_CASH_ITEM_40,             //15 레벨 이상 부터 구매가 가능합니다.
	ERR_GUILD_00,             //이미 가입한 길드가 존재합니다.
	ERR_GUILD_01,             //이미 존재 하는 길드 이름입니다.
	ERR_GUILD_02,             //길드 창단 아이템이 인벤토리에 없습니다.
	ERR_GUILD_03,             //길드 창단 아이템 정보가 이상합니다.
	ERR_GUILD_04,             //길드 창단에 실패하였습니다.
	ERR_GUILD_05,             //길드 마스터 등록에 실패하였습니다.
	ERR_GUILD_06,             //길드 정보를 찾을 수 없습니다.
	ERR_GUILD_07,             //길드 초대를 할 수 없습니다.
	ERR_GUILD_08,             //현재 오프라인인 유저 입니다.
	ERR_GUILD_09,             //체험ID 유저는 길드에 초대할 수 없습니다.
	ERR_GUILD_10,             //이미 길드 초대한 유저입니다.
	ERR_GUILD_11,             //다른 길드에서 이미 초대한 유저입니다.
	ERR_GUILD_12,             //길드 초대를 할 수 있는 권한이 없습니다.
	ERR_GUILD_13,             //길드 인원을 더 이상 늘릴 수 없습니다. 길드 인원 확장을 위해서는 '길드 인원 확장 아이템'이 필요합니다.
	ERR_GUILD_14,             //초대하려는 유저는 이미 길드멤버입니다.
	ERR_GUILD_15,             //이미 길드에 가입되어있는 유저입니다.
	ERR_GUILD_16,             //길드 유저 리스트를 얻을 수 없습니다.
	ERR_GASH_11,             //쿠폰으로 획득하려는 아이템 정보가 올바르지 않습니다.
	ERR_GUILD_17,             //유저가 길드 초대를 거부한 상태입니다.
	ERR_GUILD_18,             //길드 가입 실패.
	ERR_GUILD_19,             //길드에 소속되어있지 않습니다.
	ERR_GUILD_20,             //길드원 등급변경 권한이 없습니다.
	ERR_GUILD_21,             //길드원 등급변경에 실패하였습니다.
	ERR_GUILD_22,             //길드마스터와 길드관리자만 길드메시지의 변경이 가능합니다.
	ERR_GUILD_23,             //길드 메시지 변경에 실패하였습니다.
	ERR_GUILD_24,             //길드원 강퇴 권한이 없습니다.
	ERR_GUILD_25,             //길드 마스터는 길드에서 탈퇴 할 수 없습니다.
	ERR_GUILD_26,             //길드 탈퇴를 실패하였습니다.
	ERR_GUILD_27,             //길드원 인사말 글자 길이 제한.
	ERR_GUILD_28,             //해당 길드원 인사말 변경에 대한 권한이 없습니다.
	ERR_GUILD_29,             //길드원 인사말 변경을 실패하였습니다.
	ERR_GUILD_30,             //길드마스터만 길드 해체 기능을 사용할 수 있습니다.
	ERR_GUILD_31,             //길드원이 남아 있으므로 길드 해체가 불가능합니다.
	ERR_GUILD_32,             //길드 해체를 실패하였습니다.
	ERR_GUILD_33,             //이미 변경된 등급입니다.
	ERR_GUILD_34,             //더 이상 길드 관리자를 임명 할 수 없습니다.\n새로운 길드 관리자를 임명 하려면\n현재 길드 관리자 등급을 조정해야 합니다.
	ERR_GUILD_35,             //길드마스터는 캐릭터 삭제를 할 수 없습니다. 길드마스터 등급을 다른 길드원에게 위임하세요.
	ERR_GUILD_36,             //자기 자신의 등급은 변경할 수 없습니다.
	ERR_GUILD_37,             //이미 길드에 소속 되어 있어 길드 창단 허가서를 구매할 수 없습니다.
	ERR_GUILD_38,             //인벤토리에 길드 창단 허가서가 존재합니다.
	ERR_GUILD_39,             //길드 인원 확장을 실패하였습니다.
	ERR_GUILD_40,             //길드 마스터와 길드 관리자만 길드 인원 확장권을 사용할 수 있습니다.
	ERR_GUILD_41,             //길드 인원을 더 이상 늘릴 수 없습니다.
	ERR_ITEM_14,             //거래중인 상태에서는 해당 기능을 이용할 수 없습니다.
	ERR_USE_ITEM_IN_INVENTORY_05,             //이동 할수없는 지역입니다.
	ERR_GUILD_42,             //이 길드 이름은 7일간 사용할 수 없습니다.
	ERR_GUILD_43,             //획득 가능한 최대 길드 경험치 입니다.
	ERR_GUILD_44,             //이 아이템을 구입하기 위해서는 관련 퀘스트를 수행 해야 합니다.
	ERR_GUILD_45,             //길드 이름의 길이가 너무 깁니다.
	ERR_ATTRIB_ENCHANT_08,             //속성 제거를 위한 수수료가 부족합니다.
	ERR_BUY_CASH_ITEM_41,             //35 레벨 이상 부터 구매가 가능합니다.
	ERR_GASH_12,             //게임을 종료하고 빈펀 계정으로 업그레이드 하시기 바랍니다. 빈펀 사이트는 tw.beanfun.gamania.com 입니다.
	ERR_GASH_13,             //이 계정은 이미 빈펀 계정으로 업그레이드 된 계정입니다. 빈펀 프로그램으로 접속해주세요.
	ERR_GASH_14,             //더 이상 아이템을 구입할 수 없습니다. 임시 보관함에 있는 아이템을 인벤토리로 가져오세요.
	ERR_GUILD_SKILL_00,             //길드 마스터 등급만 GSP를 투자할 수 있습니다.
	ERR_GUILD_SKILL_01,             //이미 최고 레벨까지 배운 길드스킬입니다.
	ERR_GUILD_SKILL_02,             //해당 길드 스킬 정보가 존재하지 않습니다.
	ERR_GUILD_SKILL_03,             //선행 길드스킬을 아직 최고레벨까지 배우지 못했습니다.
	ERR_GUILD_SKILL_04,             //충분한 GSP를 투자하지 않았습니다.
	ERR_GUILD_SKILL_05,             //GSP가 부족합니다.
	ERR_GUILD_SKILL_06,             //길드 스킬 배우기가 실패하였습니다.
	ERR_GUILD_SKILL_07,             //길드 마스터 등급만 길드 스킬을 되돌릴 수 있습니다.
	ERR_GUILD_SKILL_08,             //망각의 피리 아이템이 인벤토리에 없습니다.
	ERR_GUILD_SKILL_09,             //길드 스킬을 되돌릴 수 없습니다.
	ERR_GUILD_SKILL_10,             //길드 스킬 되돌리기가 실패하였습니다.
	ERR_GUILD_SKILL_11,             //길드 마스터 등급만 길드 스킬 초기화를 할 수 있습니다.
	ERR_GUILD_SKILL_12,             //망각의 뿔피리 아이템이 인벤토리에 없습니다.
	ERR_GUILD_SKILL_13,             //길드 스킬 초기화를 할 수 없습니다.
	ERR_GUILD_SKILL_14,             //길드 스킬 초기화를 실패하였습니다.
	ERR_GUILD_SKILL_15,             //길드 마스터 등급만 사용 가능 합니다.
	ERR_GUILD_SKILL_16,             //기간제 길드 스킬 포인트 획득 실패하였습니다.
	ERR_GUILD_SKILL_17,             //이미 그노시스의 대축복 아이템을 사용 중입니다.
	ERR_CUMULATIVE_TIME_EVENT_00,             //아직 이벤트 보상을 받을 수 없습니다.
	ERR_CUMULATIVE_TIME_EVENT_01,             //인벤토리 공간이 부족하여 이벤트 보상을 받을 수 없습니다.
	ERR_CUMULATIVE_TIME_EVENT_02,             //이벤트 보상 받기가 실패하였습니다.
	ERR_DUNGEON_ITEM_00,             //아이템 모드가 아닙니다.
	ERR_DUNGEON_ITEM_01,             //드롭된 아이템이 아직 남아있습니다.
	ERR_DUNGEON_ITEM_02,             //대전 드롭아이템 얻어오기 실패.
	ERR_CREATE_UNIT_10,             //케릭터 카드 사용 후 생성 가능한 클래스입니다.
	ERR_GASH_15,             //본 계정은 이벤트에 참여할 수 없습니다.
	ERR_GUILD_BOARD_00,             //길드 마스터 또는 길드 관리자만 이용할 수 있는 기능입니다.
	ERR_PC_BANG_00,             //PC방 전용 아이템은 이용할 수 없는 기능입니다.
	ERR_GUILD_BOARD_01,             //길드 광고가 존재하지 않습니다.
	ERR_GUILD_BOARD_02,             //글자 길이가 너무 깁니다.
	ERR_GUILD_BOARD_03,             //잘못된 광고 등록 기간입니다.
	ERR_GUILD_BOARD_04,             //광고 등록 비용이 부족하므로 광고를 등록 할 수 없습니다.
	ERR_GUILD_BOARD_05,             //이미 등록한 광고가 있습니다.
	ERR_GUILD_BOARD_06,             //길드 광고 등록을 실패하였습니다.
	ERR_GUILD_BOARD_07,             //가입 신청자가 너무 많아서 가입 신청을 할 수 없습니다.
	ERR_GUILD_BOARD_08,             //길드 가입 신청을 실패하였습니다.
	ERR_GUILD_BOARD_09,             //이미 가입 신청이 되어 있습니다.
	ERR_GUILD_BOARD_10,             //가입 신청 정보가 존재하지 않습니다.
	ERR_GUILD_BOARD_11,             //길드 가입 수락을 실패하였습니다.
	ERR_GUILD_BOARD_12,             //길드 가입 신청 삭제를 실패하였습니다.
	ERR_GUILD_BOARD_13,             //기간이 다된 광고에는 가입 신청을 할 수 없습니다.
	ERR_GUILD_BOARD_14,             //길드 가입 신청 리스트를 볼수 있는 권한이 없습니다.
	ERR_GASH_16,             //자물쇠를 열지 않았습니다.
	ERR_GASH_17,             //이 게임 계정은 정지 당했습니다.
	ERR_QUEST_16,             //PC방 전용 퀘스트는 PC방에서만 수락할 수 있습니다.
	ERR_QUEST_20,             //PC방 전용 퀘스트는 PC방에서만 완료할 수 있습니다.
	NOT_LEAVE_ROOM_REASON_25,             //해킹 감지로 인한 방 이탈 입니다.
	ERR_GUILD_BOARD_15,             //길드 초대 거부 옵션이 켜져있어서 가입 신청을 할 수 없습니다.
	ERR_GUILD_BOARD_16,             //길드 가입 신청자가 많아 더 이상 가입 신청을 할 수 없습니다.
	ERR_QUEST_21,             //현재 진행중인 이벤트 퀘스트가 아닙니다.
	ERR_BUY_CASH_ITEM_42,             //해당 아이템의 구매 한도가 초과되었습니다.
	ERR_BUY_CASH_ITEM_43,             //넥슨캐시 사용이 불가능하도록 설정돼 있습니다. 넥슨포털 내정보의 넥슨캐시 보안설정 메뉴에서 설정을 변경해주세요.
	ERR_WEB_POINT_EVENT_00,             //이미 오늘 출석 체크는 완료된 상태 입니다.
	ERR_WEB_POINT_EVENT_01,             //웹 포인트 증가 DB쿼리 호출 실패.
	ERR_WEB_POINT_EVENT_02,             //출석 체크 실패.
	NOT_LEAVE_PARTY_REASON_05,             //중복 파티 가입으로 인한 파티 이탈 처리.
	ERR_PARTY_40,             //파티 가입 대기중인 상태입니다.
	ERR_PERSONAL_SHOP_28,             //아이템 리스트 삭제.
	ERR_PERSONAL_SHOP_26,             //아이템 리스트 추가.
	ERR_PERSONAL_SHOP_27,             //아이템 리스트 수정.
	ERR_PERSONAL_SHOP_29,             //카테고리 선택이 잘못 되었습니다.
	ERR_PERSONAL_SHOP_30,             //검색하려는 아이템 정보가 없습니다.
	ERR_PARTY_41,             //초대 대상자의 근성도가 부족합니다.
	ERR_PERSONAL_SHOP_31,             //잠시후에 검색을 시도해 주세요.
	ERR_PERSONAL_SHOP_32,             //판매종료된 상점입니다.
	ERR_BUY_CASH_ITEM_44,             //구매 불가능한 상태의 사용자 입니다.
	ERR_SIM_ROOM_00,             //방 생성에 실패했습니다.
	ERR_SKILL_NOTE_00,             //존재하지 않는 아이템입니다.
	ERR_SKILL_NOTE_01,             //존재 하지 않는 페이지입니다.
	ERR_SKILL_NOTE_02,             //이미 등록되어 있는 메모입니다.
	ERR_SKILL_NOTE_03,             //메모 등록을 실패 하였습니다.
	ERR_SKILL_NOTE_04,             //기술의 노트를 보유하고 있지 않습니다.
	ERR_SKILL_NOTE_05,             //기술의 노트 페이지 확장 실패.
	ERR_SKILL_NOTE_06,             //이미 기술의 노트를 보유하고 있습니다.
	ERR_SKILL_NOTE_07,             //20레벨이 되어야 구입할 수 있습니다.
	ERR_DIF_SERVER_GROUP_00,             //다른 서버의 유저와는 개인거래를 할 수 없습니다.
	ERR_DIF_SERVER_GROUP_01,             //다른 서버의 유저의 개인상점은 이용 할 수 없습니다.
	ERR_DIF_SERVER_GROUP_02,             //다른 서버의 유저는 길드로 초대 할 수 없습니다.
	ERR_DIF_SERVER_GROUP_03,             //다른 서버의 유저는 제자로 등록 할 수 없습니다.
	ERR_DIF_SERVER_GROUP_04,             //다른 서버의 유저는 친구로 등록 할 수 없습니다.
	ERR_DIF_SERVER_GROUP_05,             //헤니르의 시공 도전 모드는 다른 서버 유저와 함께 플레이 할 수 없습니다.
	ERR_ACCOUNT_BLOCK_00,             //해킹 의심 유저 접속 제한 조치.
	ERR_CLASS_CHANGE,             //[홍콩] 해당 직업으로는 전직을 할 수 없습니다.
	ERR_ITEM_15,             //버리려는 아이템 갯수 입력이 잘 못 되었습니다.
	ERR_PET_00,             //펫 생성을 실패 하였습니다.
	ERR_PET_01,             //이미 사용중인 펫 이름 입니다.
	ERR_PET_02,             //펫 이름은 최대 한글 6자(영문 12자)까지 가능합니다.
	ERR_PET_03,             //이미 펫 생성이 진행 중 입니다.
	ERR_PET_04,             //펫 소환을 실패 하였습니다.
	ERR_PET_05,             //여기서는 펫을 소환 할 수 없습니다.
	ERR_PET_06,             //소환된 펫이 없습니다.
	ERR_PET_07,             //이미 소환 되어 있는 펫 입니다.
	ERR_PET_08,             //펫 소환 해제를 실패 하였습니다.
	ERR_PET_09,             //펫 명령을 실패 하였습니다.
	ERR_PET_10,             //포만도가 ‘가득참’ 상태입니다. 더 이상 먹이를 먹을 수 없습니다.
	ERR_PET_11,             //펫 먹이 주기가 실패하였습니다.
	ERR_PET_12,             //펫이 '특별한'상태 입니다. 엘의 나무 열매/씨앗 만 먹습니다.
	ERR_PET_13,             //이미 저장 되어 있는 내용입니다.
	ERR_PET_14,             //더 이상 진화 할 수 없습니다.
	ERR_PET_15,             //친밀도가 100%가 되었을 때만 진화가 가능합니다.
	ERR_PET_16,             //장비아이템 또는 전용먹이만 섭취합니다.
	ERR_PET_17,             //장비의 레벨이 너무 낮습니다.
	ERR_GAMEFORGE_00,             //패스워드가 틀렸습니다.
	ERR_GAMEFORGE_01,             //계정이 존재하지 않습니다.
	ERR_GAMEFORGE_02,             //계정 승인이 되지 않았습니다. (User not validated)
	ERR_PET_18,             //펫에게 줄 수 있는 먹이 아이템이 아닙니다.
	ERR_PARTY_42,             //해당 유저가 다른 용무 중 입니다.
	ERR_GIANT_VERIFY_01,             //[CN] 매트릭스 카드가 바인딩되어 있습니다.
	ERR_GIANT_VERIFY_02,             //[CN] OPT번호가 틀렸습니다. 다시 입력해 주십시요.
	ERR_GIANT_VERIFY_03,             //[CN] 보안카드 번호를 잘못 입력 하셨습니다. 다시 입력해 주십시요.
	ERR_GIANT_STATE_01,             //[CN] 계정 정보가 수정 과정에 있습니다. 의문 사항이 있으시면 서비스 센터로 연락 바랍니다.
	ERR_GIANT_STATE_02,             //[CN] 자이언트 패스워드 보안이 바인딩 해제 상태에 있습니다. 의문사항이 있으시면 서비스 센터로 연락 바랍니다.
	STR_GM_NICKNAME_1,             //[TW]ServiceCode 가 잘못되었습니다.
	STR_GM_NICKNAME_2,             //GM리엘
	STR_DELETED_NICKNAME,             //[발신자 정보가 없습니다]
	ERR_BUY_CASH_ITEM_45,             //지금은 청약철회 기능을 이용 하실 수 없습니다.
	ERR_BUY_CASH_ITEM_46,             //청약철회 가능 개수보다 요청 갯수가 커서 청약철회를 실패하였습니다.
	ERR_BUY_CASH_ITEM_47,             //구매한지 7일이 초과된 상품으로 청약철회가 불가능 합니다.
	ERR_BUY_CASH_ITEM_48,             //패키지 중 일부 상품을 수령하여 청약철회가 불가능 합니다.
	ERR_BUY_CASH_ITEM_49,             //상품 가격이 0원으로 청약철회가 불가능한 상품 입니다.
	ERR_BUY_CASH_ITEM_50,             //중복된 RequestID입니다.
	ERR_BUY_CASH_ITEM_51,             //존재하지 않는 상품 구매 정보입니다.
	ERR_BUY_CASH_ITEM_52,             //존재하지 않는 상품 정보 입니다.
	ERR_BUY_CASH_ITEM_53,             //청약철회가 불가능한 상품 입니다.
	ERR_BUY_CASH_ITEM_54,             //선물 받은 상품이어서 청약철회가 불가능 합니다.
	ERR_ITEM_16,             //기간제 아이템은 버릴 수 없습니다.
	ERR_ITEM_17,             //되팔기 불능 아이템은 버릴 수 없습니다.
	ERR_BUY_CASH_ITEM_55,             //1 EL coins 이벤트 구매 제한을 초과 하여 더 이상 구매 할 수 없습니다. 
	ERR_RANDOM_ITEM_08,             //레벨이 부족하여 아이템을 사용 할 수 없습니다.
	ERR_ENCHANT_ITEM_15,             //강화 레벨 11부터는 강화시 플루오르 스톤을 사용할 수 없습니다.
	ERR_QUEST_22,             //이미 보상을 받아서 퀘스트 수락을 할 수 없습니다.
	ERR_QUEST_23,             //수락 보상을 받을 인벤토리 공간이 부족하여 퀘스트를 수락할 수 없습니다.
	ERR_SOCKET_ITEM_10,             //마법석 사용을 실패하였습니다.
	ERR_BLOCK_TRADE_00,             //해당 기능은 현재 사용할 수 없습니다.\n 불편을 드려 죄송합니다.
	ERR_IP_BLOCK_00,             //접속이 제한된 IP입니다.
	ERR_CHAT_BLOCK_00,             //채팅 기능이 제한 되었습니다.
	ERR_BLOCK_TRADE_01,             //거래 기능이 제한 되었습니다.
	NOT_LEAVE_ROOM_REASON_22,             //' 아이템이 없어 게임을 시작 할 수 없습니다.
	ERR_KNP_03,             //다른 응용프로그램과의 충돌이 있습니다.
	ERR_GIANT_BILLING_01,             //[CN] 해당 유저가 존재하지 않습니다.
	ERR_GIANT_BILLING_02,             //[CN] 잔액이 부족합니다.
	ERR_GIANT_BILLING_03,             //[CN] 존재하지 않는 아이템 카드입니다.
	ERR_GIANT_BILLING_04,             //[CN] 동일한 쿠폰은 중복사용 할 수 없습니다.
	ERR_GIANT_BILLING_05,             //[CN] 이미 사용한 아이템 카드입니다.
	ERR_GIANT_BILLING_06,             //[CN] 잘못된 전용 카드 ID입니다.
	ERR_GIANT_BILLING_07,             //[CN] 잘못된 전용 카드 패스워드입니다.
	ERR_GIANT_BILLING_08,             //[CN] DB 에러
	ERR_BLOCK_TRADE_02,             //거래 제한 상태라서 편지 보내기 기능을 사용 하실 수 없습니다.
	ERR_BLOCK_TRADE_03,             //거래 제한 상태라서 개인 상점 개설을 할 수 없습니다.
	ERR_BLOCK_TRADE_04,             //거래 제한 상태라서 개인 상점을 볼 수 없습니다.
	ERR_BLOCK_TRADE_05,             //거래 제한 상태라서 물품 검색을 할 수 없습니다.
	ERR_BLOCK_TRADE_06,             //거래 제한 상태라서 거래 광장에 입장 할 수 없습니다.
	ERR_LIMITED_CASHITEM,             //캐시 인벤토리에 아이템이 존재합니다.(HK 이벤트에 사용)
	ERR_WORLD_MISSION_01,             //파티인원이 2인 이상이어야 합니다.
	ERR_WORLD_MISSION_02,             //던전이 닫혀 입장 할 수 없습니다.
	ERR_DIF_SERVER_GROUP_06,             //다른 서버 유저와 월드 미션 던전을 플레이 할 수 없습니다.
	ERR_EXIST_LETTER_MONITORING,             //우편 감시 로그에 중복된 값이 있습니다.
	ERR_EXIST_TRADE_MONITORING,             //거래 감시 로그에 중복된 값이 있습니다.
	ERR_INSERT_FAILD_LETTER_MONITORING,             //우편 감시 로그 추가 실패!
	ERR_INSERT_FAILD_TRADE_MONITORING,             //거래 감시로그 추가 실패!
	ERR_PERSONAL_SHOP_33,             //아직 판매중인 물품이 있어 대리상인을 종료할 수 없습니다!
	ERR_PERSONAL_SHOP_34,             //대리상인이 개설되어 있지 않습니다.
	ERR_PERSONAL_SHOP_35,             //판매 중지를 할 수 없습니다.
	ERR_PERSONAL_SHOP_36,             //판매 중지된 개인상점입니다. 물건을 구매할 수 없습니다.
	ERR_PERSONAL_SHOP_37,             //대리 판매인이 물품을 판매 하고 있어 상점을 개설 할 수 없습니다.
	ERR_WARNING_BLOCK_TRADE,             //(TW)비정상적인 방법으로 게임을 플레이 하고 있습니다. 계속 하시면 거래 정지 됩니다.
	ERR_PERSONAL_SHOP_38,             //대리상인을 종료할 수 없습니다.
	ERR_PAYLETTER_BILLING_01,             //Unregisted User.
	ERR_PAYLETTER_BILLING_02,             //It is already canceled.
	ERR_PAYLETTER_BILLING_03,             //Invalid coupon number.
	ERR_PAYLETTER_BILLING_04,             //This coupon can not be canceled.
	ERR_PAYLETTER_BILLING_05,             //The used user information and cancel the requested user information does not match.
	ERR_PAYLETTER_BILLING_06,             //The issued cash by coupons  already had used . so can not be canceled.
	ERR_PAYLETTER_BILLING_07,             //requested price should be equal to billing items price.
	ERR_PAYLETTER_BILLING_08,             //The balance short.
	ERR_PAYLETTER_BILLING_09,             //Billing account that is unavailable.
	ERR_PAYLETTER_BILLING_10,             //You can not register any more today.
	ERR_PAYLETTER_BILLING_11,             //This coupon can not verify the information.
	ERR_PAYLETTER_BILLING_12,             //Coupon is already used.
	ERR_PAYLETTER_BILLING_13,             //Coupon use is stopped.
	ERR_PAYLETTER_BILLING_14,             //Coupon is expired.
	ERR_PAYLETTER_BILLING_15,             //Coupon is not available in the game.
	ERR_PAYLETTER_BILLING_16,             //Issued to certain customers who can use coupons, you do not have permission.
	ERR_PAYLETTER_BILLING_17,             //Buying is a restricted user.
	ERR_PAYLETTER_BILLING_18,             //Your use is limited to the payment service.
	ERR_PAYLETTER_BILLING_19,             //Your use is limited to the purchase service.
	ERR_PAYLETTER_BILLING_20,             //User cannot be verified.
	ERR_PAYLETTER_BILLING_21,             //Registration has exceeded the number of coupons available.
	ERR_K3RCOMBO_AUTH_01,             //Unable to login due to IP restriction.
	ERR_PAYLETTER_BILLING_22,             //The coupon code has already been redeemed.
	ERR_PAYLETTER_BILLING_23,             //You cannot purchase the item at the moment. Your account have reached the daily limit for purchasing the item.
	ERR_VERIFY_22,             //2회이상 비밀번호를 틀려 로그인할 수 없습니다.
	ERR_PERSONAL_SHOP_39,             //대리 판매인에게 물품을 맡기려면 대리판매 보증서가 있어야 합니다.
	ERR_SECOND_SECURITY_AUTH_FAILED,             //입력하신 보안패드 암호가 서로 같지 않습니다.
	ERR_SECOND_SECURITY_AUTH_FAILED_LIMIT,             //보안패드 암호가 10회 실패하였습니다.\n[확인]버튼을 누르면 접속이 종료됩니다.\n재활성화 및 해제는 웹사이트에서 개인 인증 후 가능합니다.
	ERR_SECOND_SECURITY_DEFECT_PW,             //보안패드 암호 생성에 실패하였습니다.
	ERR_SECOND_SECURITY_DIFFER_PW,             //보안패드 암호가 일치하지 않습니다.
	ERR_PERSONAL_SHOP_40,             //상점 이름이 너무 깁니다.
	ERR_PERSONAL_SHOP_41,             //대리상점 판매가 종료된 상태입니다.\n물품받기 후 새롭게 상점 개설이 가능합니다.
	ERR_PERSONAL_SHOP_42,             //대리 판매인이 물품을 판매 중입니다.\n물품이나 판매수익을 받기 위해서는\n대리 판매인 상점을 종료 해야 합니다.
	ERR_PERSONAL_SHOP_43,             //대리 상인 보증서 DB쿼리 실패
	ERR_PERSONAL_SHOP_44,             //대리 상인을 생성을 실패하였습니다.
	ERR_PERSONAL_SHOP_45,             //대리 상인 물품 등록을 실패 하였습니다.
	ERR_PERSONAL_SHOP_46,             //대리 상점 개설자만 이용할 수 있습니다.
	ERR_PERSONAL_SHOP_47,             //대리 상점 물품 받기가 실패하였습니다.
	ERR_PERSONAL_SHOP_48,             //현재 구매가 이루어지고 있는 물품이어서 물품 받기가 실패하였습니다.
	ERR_PERSONAL_SHOP_49,             //이미 물품 받기가 진행중인 상태입니다.
	ERR_PERSONAL_SHOP_50,             //보유 가능한 ED량을 초과하여 물품받기가 실패하였습니다.
	ERR_PERSONAL_SHOP_51,             //인벤토리 공간이 부족하여 물품을 받을 수 없습니다.
	ERR_PERSONAL_SHOP_52,             //보관 중인 물품이 없습니다.
	ERR_PERSONAL_SHOP_53,             //동일한 물품에 대해서는 동시에 가져올 수 없습니다.
	ERR_MIN_LEVEL_JOIN_PARTY_FAILE,             //레벨 제한 조건을 만족하지 않습니다.\n파티 요청을 할 수 없습니다.
	ERR_PERSONAL_SHOP_54,             //아직 대리상인 기간이 남아서 상품을 구매할 수 없습니다.
	ERR_PERSONAL_SHOP_55,             //찾아가지 않은 물품이 있어 개인상점을 개설 할 수 없습니다.\n물품 받기를 확인 하세요!
	NOT_LEAVE_ROOM_REASON_26,             //대리 판매인의 판매 물품이 모두 판매되어 상점을 종료합니다.
	ERR_QUEST_24,             //이미 수락하여 보상을 받은 퀘스트 입니다. 퀘스트 수락을 할 수 없습니다.
	ERR_QUEST_25,             //인벤토리 슬롯이 부족하여 퀘스트 수락을 할 수 없습니다.
	ERR_PARTY_43,             //파티 최소 레벨 입장 제한을 변경하지 못했습니다.
	ERR_PERSONAL_SHOP_56,             //대리상점의 모든 물품이 판매될 경우 소지 한도 금액을 초과하게 되기 때문에 상점을 개설할 수 없습니다.
	ERR_ENCHANT_ITEM_16,             //(TW/HK)강화 보조 아이템 사용 불가
	ERR_ENCHANT_ITEM_17,             //(TW/HK)강화 보조 아이템이 없습니다.
	ERR_ENCHANT_ITEM_18,             //(TW/HK)파괴 방지 아이템 사용 불가
	ERR_ENCHANT_ITEM_19,             //(TW/HK)파괴 방지 아이템이 없습니다.
	ERR_CHANGE_DUNGEON_MAP_FAILE,             //레벨 제한 조건을 만족하지 않습니다.\n다른 던전을 선택하시거나 레벨제한을 해제하여야 합니다.
	ERR_CHANGE_PARTY_MIN_LEVEL_CHECK_FAILE,             //레벨 제한 조건을 만족하지 않습니다.\n레벨 제한을 활성화 할 수 없습니다.
	ERR_PARTY_INVITE_FAILE,             //레벨 제한 조건을 만족하지 않습니다.\n파티 초대를 할 수 없습니다.
	ERR_CREATE_PARTY_FAILE,             //레벨 조건을 만족하지 않습니다.\n다른 던전을 선택하시거나 레벨제한을 해제하여야 합니다.
	ERR_SECOND_SECURITY_DEFECT_PW_1,             //반복되는 숫자는 암호로 사용하실 수 없습니다.\n(0000, 1111, 999999 등)
	ERR_SECOND_SECURITY_DEFECT_PW_2,             //연속되는 숫자는 암호로 사용하실 수 없습니다.\n(1234, 9876, 123456 등)
	ERR_SECOND_SECURITY_DELETE_FAILE,             //2차 보안패드 삭제가 실패하였습니다.
	ERR_SECOND_SECURITY_NO_USE,             //2차 보안패드를 사용중이 아닙니다.
	ERR_SECOND_SECURITY_DEFECT_PW_3,             //이전 비밀번호와 동일한 비밀번호로\n변경 할 수 없습니다.
	ERR_BUY_ED_ITEM_06,             //상점에서 팔고있지 않는 아이템을 구매하려고 하였습니다.
	NOT_LEAVE_MATCH_REASON_00,             //대전 게임 시작으로 인한 매치 종료.
	NOT_LEAVE_MATCH_REASON_01,             //대전 매칭 대상자 중 누군가가 참여하지 않았습니다.
	NOT_LEAVE_MATCH_REASON_02,             //대전 매칭이 취소되었습니다.
	NOT_LEAVE_MATCH_REASON_03,             //파티원이 대전에 참여하지 않아서 대전 매칭이 취소되었습니다.
	NOT_LEAVE_MATCH_WAIT_REASON_00,             //매치가 성공하여 대기자 리스트에서 빠짐.
	NOT_LEAVE_MATCH_WAIT_REASON_01,             //유저가 매치 신청 취소를 하여 대기자 리스트에서 빠짐.
	NOT_LEAVE_MATCH_WAIT_REASON_02,             //파티원의 파티 탈퇴로 인해 매치 신청이 취소되었습니다.
	ERR_MATCH_MAKING_00,             //이미 매치 중인 상태입니다.
	ERR_MATCH_MAKING_01,             //대전 인원 정보가 이상합니다.
	ERR_MATCH_MAKING_02,             //대전 타입 정보가 이상합니다.
	ERR_MATCH_MAKING_03,             //공식 대전은 10레벨부터 가능합니다.
	ERR_MATCH_MAKING_04,             //대전 파티는 10레벨부터 이용 가능 합니다.
	ERR_MATCH_MAKING_05,             //이미 변경된 파티 타입 입니다.
	ERR_MATCH_MAKING_06,             //던전파티 인원이 4명일때는 대전파티로 변경이 불가능 합니다.
	ERR_MATCH_MAKING_07,             //파티 타입 변경 실패.
	ERR_MATCH_MAKING_08,             //파티원중에 10레벨이 안되는 멤버가 있어 대전파티로 변경이 불가능합니다.
	ERR_MATCH_MAKING_09,             //파티원중에 10레벨이 안되는 멤버가 있어 공식대전을 이용할 수 없습니다.
	ERR_MATCH_MAKING_10,             //10레벨 이상의 캐릭터만 대전파티에 초대 할 수 있습니다.
	ERR_MATCH_MAKING_11,             //10레벨 이상의 캐릭터만 대전 파티의 파티장으로 임명될 수 있습니다.
	ERR_EXCHANGE_CASH,             //[CN]전환이 정상적으로 이루어지지 않았습니다. 다시 시도하여 주시기 바랍니다.
	ERR_SORT_CATEGORY,             //[EU]연속으로 정렬할 수 없습니다.\n잠시 후 다시 시도해주세요.
	ERR_MATCH_MAKING_12,             //대전 매칭을 신청한 상태가 아닙니다.
	ERR_MATCH_MAKING_13,             //이미 대전방에 입장한 상태입니다.
	ERR_MATCH_MAKING_14,             //대전방이 아닙니다.
	ERR_MATCH_MAKING_15,             //대전 매칭 요청이 실패하였습니다.
	ERR_MATCH_MAKING_16,             //대전 매칭 신청한 유저는 파티에 초대 할 수 없습니다.
	ERR_MATCH_MAKING_17,             //대전 매칭중인 유저는 파티에 초대 할 수 없습니다.
	ERR_BUY_ED_ITEM_07,             //대전 랭크가 낮아 해당 아이템을 구매할 수 없습니다.
	ERR_MATCH_MAKING_18,             //대전 매칭 신청을 한 상태에서는 다른 파티에 가입 신청을 할 수 없습니다.
	ERR_MATCH_MAKING_19,             //대전 매칭중인 상태에서는 다른 파티에 가입 신청을 할 수 없습니다.
	ERR_MATCH_MAKING_20,             //대전 매칭 상태에서는 채널이동 기능을 사용하실 수 없습니다.
	ERR_CHECK_MACHINE_ID_00,             //머신 ID 인증 실패.
	ERR_USER_REGISTRATION_10,             //머신ID로 인한 이중접속 감지.
	ERR_QUEST_26,             //오늘 수락 할 수 있는 랜덤 퀘스트가 아닙니다.
	ERR_K3RCOMBO_AUTH_02,             //약관에 동의하지 않았습니다.
	ERR_ODBC_02,             //SP 출력 값에 문제가 있습니다.
	ERR_ODBC_03,             //SP 입력 값에 문제가 있습니다. 
	ERR_ODBC_04,             //목적 DB 이름이 잘못 되었습니다.
	ERR_ODBC_05,             //데이터 검증 실패
	ERR_CONTENT_00,             //죄송합니다. 캐쉬샵 점검으로 인해 이용하실 수 없습니다.
	ERR_SHUT_DOWN_00,             //00시부터 06시까지 게임 접속이 제한됩니다.
	ERR_GM_CHAR_DENY_01,             //정보를 가져오지 못했습니다.
	ERR_TRADE_BLOCK_UNIT_00,             //캐릭터 생성한지 5일이 지난 후부터 거래가 가능합니다
	ERR_TRADE_BLOCK_UNIT_01,             //캐릭터 생성한지 5일이 지난 후부터 우편이 가능합니다.
	ERR_GIANT_BILLING_09,             //[CN]사용실패!
	NOT_LEAVE_ROOM_REASON_27,             //배틀필드 입장을 위한 방 이탈 입니다.
	ERR_SHARING_BANK_01,             //Lv.15가 되어야 구입하실 수 있습니다.
	ERR_SERVERSN_01,             //ServerSN과 MachineID가 매칭되지 않는다.
	ERR_SERVERSN_02,             //MachineID와 ServerSN이 매칭되지 않는다.
	ERR_SERVERSN_03,             //DB에 존재 하지 않는 ServerSN이 있다.
	ERR_SERVERSN_04,             //DB에 존재 하지 않는 MachineID가 있다.
	ERR_SERVERSN_05,             //클라가 준 SN은 없는데 DB에는 MachineID에 해당하는 SN 존재.
	ERR_SERVERSN_06,             //제재된 SN입니다.
	ERR_SERVERSN_07,             //접속 제한된 ServerSN 입니다.
	ERR_VERIFY_23,             //이 계정은 테스트서버의 계정이 아닙니다.
	NOT_LEAVE_PARTY_REASON_06,             //파티 병합을 위한 자동 파티 이탈입니다.
	ERR_BUY_CASH_ITEM_56,             //이미 스킬 슬롯 확장을 사용 중입니다. 스킬 슬롯 확장(기간제) 상품을 가져올 수 없습니다.
	ERR_BUY_CASH_ITEM_57,             //이미 스킬 슬롯 확장을 기간 무제한으로 사용 중입니다. 상품을 가져올 수 없습니다.
	ERR_GIANT_VERIFY_04,             //[CN] 귀하의 계정이 정지되었습니다. 고객 센터에 문의하여 주십시요.
	ERR_GIANT_VERIFY_05,             //[CN] 비밀번호를 잘못 입력 하셨습니다. 다시 입력해 주십시요.
	ERR_GIANT_VERIFY_06,             //[CN] 해당 계정은 존재하지 않습니다.
	ERR_GIANT_VERIFY_07,             //[CN] 불명확인 시스템 에러
	ERR_RANDOM_ITEM_09,             //이미 동일한 아이템을 사용 중 입니다.
	ERR_BUY_CASH_ITEM_58,             //레벨이 부족하여 구매할 수 없습니다.
	ERR_BUY_CASH_ITEM_59,             //레벨이 부족하여 상품을 가져올 수 없습니다.
	NOT_CLOSE_PARTY_REASON_00,             //파티 생성 실패.
	NOT_CLOSE_PARTY_REASON_01,             //파티 인원 수 부족.
	NOT_CLOSE_PARTY_REASON_02,             //파티 초대에 대한 초대 대상자의 응답 지연.
	ERR_BUY_CASH_ITEM_60,             //[JP] 판매하지 않는 상품입니다.
	ERR_BUY_CASH_ITEM_61,             //[JP] 한게임 빌링 오류로 구매할수 없습니다.
	ERR_SHARE_BANK_01,             //공유은행 아이템은 우편으로 보낼 수 없습니다.
	ERR_SHARE_BANK_02,             //공유은행 아이템은 거래 할 수 없습니다.
	ERR_BUY_CASH_ITEM_62,             //[US] 레벨 20이 되어야 구입하실 수 있습니다.
	ERR_ITEM_EXCHANGE_ONLY_GUILD_MEMBER_00,             //[EU] 길드에 소속된 캐릭터만 교환 할 수 있습니다.
	ERR_BUY_CASH_ITEM_63,             //[US] 레벨 30이 되어야 구입하실 수 있습니다.
	ERR_PARTY_44,             //[US] PVP파티에는 풀패치일때만 참가요청을 할 수 있습니다.
	ERR_CHECK_DLL_01,             //블랙리스트에 등록된 DLL 이 존재합니다.
	ERR_QUIZ_00,             //[CN] 잠시 후 다시 시도해 주시거나 캐릭터를 재접속 해주시기 바랍니다.
	ERR_QUIZ_01,             //[CN] 오늘 퀴즈 이용 횟수를 초과 하셨습니다.
	ERR_QUIZ_02,             //[CN] 알 수 없는 에러 입니다. (퀴즈)
	ERR_BUY_CASH_ITEM_64,             //[JP] 거래제한기간중에는구매할수 없습니다.
	ERR_BUY_CASH_ITEM_65,             //[JP] 거래제한기간중에는 사용할 수 없습니다.
	ERR_BUY_CASH_ITEM_66,             //더 이상 퀵 슬롯을 확장 할 수 없습니다.
	ERR_DELETE_UNIT_01,             //[CN] 이미 삭제되었거나 삭제 대기 상태의 캐릭입니다.
	ERR_DELETE_UNIT_02,             //[CN] 캐릭터 삭제 가능한 기간이 아닙니다.
	ERR_DELETE_UNIT_03,             //[CN] 캐릭터 최종 삭제 가능 기간이 아닙니다.
	ERR_DELETE_UNIT_04,             //[CN] 최종 삭제 가능한 캐릭터가 아닙니다.
	ERR_RESTORE_UNIT_01,             //[CN] 삭제 복구 대상 캐릭터가 아닙니다.
	ERR_RESTORE_UNIT_02,             //[CN] 캐릭터 복구 가능 기간이 아닙니다.
	ERR_SORT_CATEGORY_01,             //잠시후에 정렬을 시도해주세요.
	ERR_DELETE_UNIT_00,             //해당 캐릭터를 찾을 수 없습니다.
	ERR_DELETE_UNIT_05,             //캐릭터 삭제를 실패 하였습니다.
	ERR_PET_19,             //펫 이름 변경에 실패 하였습니다.
	ERR_PET_20,             //이미 펫 이름 변경이 진행 중입니다.
	ERR_SHUT_DOWN_01,             //게임 이용이 차단 됩니다.
	ERR_GUILD_46,             //길드마스터가 아닙니다.
	ERR_GUILD_47,             //아이템 정보가 이상합니다.
	ERR_GUILD_48,             //길드 이름 변경 실패!
	ERR_GUILD_49,             //이미 진행 중인 작업입니다.
	ERR_GUILD_50,             //존재하지 않는 아이템을 삭제 시도하였습니다.
	ERR_FISH_01,             //[TW] 낚시대를 장착하지 않았거나, 미끼가 없습니다.
	ERR_FISH_02,             //[TW] 이미 사용중인 낚시터 입니다.
	ERR_FISH_03,             //[TW] 낚시터에 문제가 있습니다. 다른 곳을 이용해 주세요.
	ERR_FISH_04,             //[TW] 낚시 보상에 문제가 발생했습니다.
	ERR_ITEM_18,             //아이템 삭제를 실패 하였습니다.
	ERR_BUY_CASH_ITEM_67,             //길드 이름 변경권은 길드 마스터만 사용 가능합니다.
	ERR_PET_21,             //펫이 있을 때만 사용이 가능합니다. 펫이 없을 경우 일주일 이내에 청약 철회를 하셔야 합니다.
	ERR_GUILD_51,             //처리 시간이 지연되어 작업이 무효화 되었습니다. 재시도 해주시기 바랍니다.
	ERR_GUILD_52,             //유저가 길드 이름 변경을 취소하였습니다.
	ERR_BATTLEFIELD_00,             //해당 지역으로 이동할 수 있는 상태가 아닙니다.
	ERR_BATTLEFIELD_01,             //이미 해당 지역에 입장한 상태입니다.
	ERR_BATTLEFIELD_02,             //이미 해당 지역은 가득차 있어 입장할 수 없습니다.
	ERR_BATTLEFIELD_03,             //해당 지역에는 이미 다른 파티들로 인해 입장할 공간이 없습니다.
	ERR_BATTLEFIELD_04,             //해당 지역에 입장을 실패하였습니다.
	ERR_BATTLEFIELD_05,             //입장하려는 지역 정보가 잘못되었습니다.
	ERR_BATTLEFIELD_06,             //해당 지역에 입장하려는 중 오류가 발생하였습니다.
	ERR_BATTLEFIELD_07,             //해당 지역에서 유저 정보 찾기가 실패하였습니다.
	ERR_AUTO_PARTY_00,             //자동 파티로 던전 게임 시작을 실패하였습니다.
	ERR_BATTLEFIELD_08,             //던전 게임 시작을 실패하였습니다.
	ERR_AUTO_PARTY_01,             //자동 파티 신청을 할 수 없는 상태입니다.
	ERR_AUTO_PARTY_02,             //재집결을 위한 휴식 버프로 인해 자동 파티 신청을 할 수 없습니다. 버프가 풀릴때까지 기다려주세요.
	ERR_AUTO_PARTY_03,             //이미 자동 파티 신청이 된 상태입니다.
	ERR_AUTO_PARTY_04,             //잘못된 정보로 인해 자동 파티 신청이 실패하였습니다.
	ERR_AUTO_PARTY_05,             //자동 파티 대기자로 등록이 실패하였습니다.
	ERR_AUTO_PARTY_06,             //자동 파티 신청 상태가 아닙니다.
	ERR_BATTLEFIELD_09,             //파티원 수가 1명뿐이어서 게임을 시작할 수 없습니다.
	NOT_PARTY_GAME_START_FAILED_00,             //파티구성원 중 취소한 유저가 있습니다.
	NOT_PARTY_GAME_START_FAILED_01,             //파티 게임 시작에 대한 응답시간이 지나 파티 게임을 시작하지 않습니다.
	ERR_BATTLEFIELD_10,             //이미 파티에 소속된 상태입니다.
	ERR_BATTLEFIELD_11,             //초대 대상자가 이미 다른 파티 가입을 기다리는 중입니다.
	ERR_BATTLEFIELD_12,             //파티에 소속된 상태가 아닙니다.
	ERR_USE_ITEM_IN_INVENTORY_06,             //영약 또는 비약은 중첩하여 사용할 수 없습니다.
	ERR_SN_OVERLAP_01,             //ServerSN이 중복 접속 중이다.
	ERR_SN_OVERLAP_02,             //ServerSN 존재하지 않는데 삭제하려한다.
	ERR_INVENTORY_LOCK_00,             //현재 사용할 수 없는 기능입니다.
	ERR_NX_AUTH_00,             //넥슨 통합 맵버쉽 계정 인증 상향 조정 필요.
	ERR_USE_ITEM_IN_INVENTORY_07,             //육체의 한계 버프가 켜져 있습니다. 비약을 사용할 수 없습니다.
	ERR_DUNGEON_GAME_START_00,             //레벨 제한 조건을 만족하지 않습니다. 던전 게임을 시작할 수 없습니다.
	ERR_RECOMMEND_USER_02,             //[US] 추천받을 대상이 아닙니다.
	ERR_RECOMMEND_USER_03,             //[US] 추천인이 이미 3인을 추천하였습니다.
	ERR_RECOMMEND_USER_04,             //[US] 추천 가능한 유저가 아닙니다.
	ERR_RECOMMEND_USER_05,             //[US] 맞지 않는 아이템을 사용하였습니다.
	ERR_ITEM_21,             //[US] 캐릭터 슬롯을 확장할 수 없습니다. 슬롯 확장은 9개까지 가능합니다.
	NOT_LEAVE_AUTO_PARTY_REASON_00,             //던전 게임 시작으로 인한 자동 파티 종료.
	NOT_LEAVE_AUTO_PARTY_REASON_01,             //자동 파티 대상자 중 누군가가 게임 시작에 동의하지 않았습니다.
	NOT_LEAVE_AUTO_PARTY_REASON_02,             //자동 파티 취소 요청에 의해 성공적으로 종료되었습니다.
	NOT_LEAVE_AUTO_PARTY_REASON_03,             //파티원이 자동파티 게임시작에 동의하지 않아 자동파티가 취소되었습니다.
	NOT_LEAVE_AUTO_PARTY_WAIT_REASON_00,             //자동파티가 성공하여 대기자 리스트에서 빠짐.
	NOT_LEAVE_AUTO_PARTY_WAIT_REASON_01,             //유저가 자동파티 신청 취소를 하여 대기자 리스트에서 빠짐.
	NOT_LEAVE_AUTO_PARTY_WAIT_REASON_02,             //파티원의 파티 탈퇴로 인해 자동파티 신청이 취소되었습니다.
	ERR_DUNGEON_GAME_START_01,             //아이템 레벨 제한 조건을 만족하지 않습니다. 던전 게임을 시작할 수 없습니다.
	ERR_KEYBOARD_MAPPING_01,             //키보드 맵핑 정보를 저장하지 못하였습니다.
	ERR_KEYBOARD_MAPPING_02,             //모든 키보드 맵핑 정보를 저장하는데 실패하였습니다.
	ERR_KEYBOARD_MAPPING_03,             //키보드 맵핑 정보를 읽어오지 못하였습니다.
	ERR_KEYBOARD_MAPPING_04,             //모든 키보드 맵핑 정보를 읽어오는데 실패하였습니다.
	ERR_PARTY_45,             //파티 던전 게임 시작을 위한 응답이 지연되었습니다.
	ERR_RECOMMEND_USER_06,             //[US] 추천인 테이블에서 캐릭터 삭제를 실패하였습니다.
	ERR_K3RCOMBO_AUTH_03,             //[US] 보안패드 리셋용 질문 및 답변을 입력하지 않았습니다.
	ERR_SEARCH_UNIT_05,             //같은 파티원이 아닙니다.
	ERR_CHAT_OPTION_01,             //채팅 옵션 정보를 저장하는데 실패하였습니다.
	ERR_CHAT_OPTION_02,             //채팅 옵션 정보를 읽어오는데 실패하였습니다.
	ERR_CHAT_OPTION_03,             //일부 채팅 옵션 정보를 읽어오는데 실패하였습니다.
	ERR_KEYBOARD_MAPPING_05,             //테스트 
	ERR_PAYLETTER_BILLING_24,             //[US] 한달에 $1,000 까지만 선물이 가능합니다.
	ERR_SHUT_DOWN_02,             //나이정보를 확인 중 오류가 발생하였습니다. 잠시 후 다시 로그인하여 주십시오.
	ERR_SHUT_DOWN_03,             //실명인증을 받지 않은 회원이거나 넥슨닷컴 회원이 아닙니다.
	ERR_SHUT_DOWN_04,             //셧다운제에 의하여 게임이용이 차단 됩니다.
	ERR_SHUT_DOWN_05,             //선택적 셧다운제의 의해 게임이용이 차단됩니다.
	ERR_VERIFY_24,             //통합 맴버십 ATL 2.0 유저가 아니므로 게임이용이 차단 됩니다.
	ERR_RANDOM_ITEM_10,             //오픈 가능 시간이 되지 않았습니다.
	ERR_RANDOM_ITEM_11,             //오픈 가능 시간이 지나 오픈 할 수 없습니다.
	ERR_AS_BILLING_00,             //ServiceID가 잘못 되었습니다.
	ERR_AS_BILLING_01,             //캐시 잔액 정보를 찾을 수 없습니다.
	ERR_GASH_18,             //[TW]ServiceCode 가 올바르지 않습니다.
	ERR_GASH_19,             //[TW]정의되지 않은 애러 발생했습니다.
	ERR_GASH_20,             //[TW]시간형식이 잘못되었습니다.
	ERR_GASH_21,             //[TW]Type정보가 잘못되었습니다.
	ERR_GASH_22,             //[TW]이전 상품 구매가 진행 중입니다.
	ERR_QUEST_27,             //레벨이 높아 퀘스트를 수락 받지 못합니다.
	ERR_AS_BILLING_02,             //파라매터 갯수 입력이 잘못되었습니다.
	ERR_SYNTHESIS_01,             //[JP] 합성제가 없습니다.
	ERR_SYNTHESIS_02,             //[JP] 없는 아바타를 합성하려고 했습니다.
	ERR_PAYLETTER_BILLING_25,             //[US]월 선물 가능 횟수를 초과하였습니다.
	ERR_PARTY_46,             //[CN]1인 플레이 던전 입니다.
	ERR_PARTY_47,             //[CN]던전 오픈 시간이 아닙니다.
	ERR_SELECT_UNIT_07,             //정상적인 캐릭터 선택이 아닙니다.
	ERR_POST_LETTER_20,             //캐릭터 자신에게 일반우편을 보낼려고 했습니다.
	ERR_PET_25,             //펫 정보가 이상합니다.
	ERR_PET_22,             //펫이 소환된 상태에서만 사용 가능합니다.
	ERR_PET_24,             //성체 상태의 펫만 사용 가능합니다.
	ERR_PET_23,             //유체 상태의 펫만 사용 가능합니다.
	ERR_ROOM_48,             //태그 횟수 세팅이 이상합니다.
	ERR_ROOM_49,             //팀원이 최소 2명 이상이어야 시작할 수 있습니다.
	ERR_PVP_01,             //대전 아이템은 이용할 수 없는 기능입니다.
	ERR_PVP_02,             //대전 아이템 사용 횟수가 남지 않았습니다.
	ERR_PVP_03,             //대전 아이템 사용 횟수가 남지 않았습니다.
	ERR_PVP_04,             //대전 아이템 사용 횟수가 남지 않았습니다.
	NOT_LEAVE_ROOM_REASON_28,             //던전 게임 입장을 위한 배틀필드 이탈입니다.
	NOT_LEAVE_ROOM_REASON_32,             //대전 게임 입장을 위한 이탈입니다.
	NOT_LEAVE_ROOM_REASON_29,             //채널 이동에 의한 이탈입니다.
	NOT_LEAVE_ROOM_REASON_31,             //같은 파티원 찾아가기에 의한 이탈입니다.
	NOT_LEAVE_ROOM_REASON_30,             //이동 아이템 사용에 의한 이탈입니다.
	NOT_LEAVE_ROOM_REASON_33,             //비정상종료로 인한 이탈.
	ERR_PARTY_48,             //던전 게임중에는 파티원을 초대 할 수 없습니다.
	ERR_PARTY_54,             //이미 다른 파티에 소속되어 있어 초대 할 수 없습니다.
	ERR_PARTY_51,             //대상이 대전 게임중이라 초대할 수 없습니다.
	ERR_PARTY_50,             //대상이 던전 게임중이라 초대할 수 없습니다.
	ERR_PARTY_53,             //동일한 서버에서 해당 유저를 찾을 수 없습니다.
	ERR_PARTY_52,             //더 이상 파티원을 초대 할 수 없습니다.
	ERR_PARTY_49,             //대전 게임중에는 파티원을 초대 할 수 없습니다.
	ERR_BATTLEFIELD_13,             //필드에 입장할 수 있는 레벨이 부족합니다.
	ERR_BATTLEFIELD_14,             //필드에 입장할 수 있는 조건의 던전을 클리어 하지 않았습니다.
	ERR_QUEST_28,             //퀘스트는 최대 30개까지만 수행이 가능합니다.
	ERR_USE_ITEM_IN_INVENTORY_08,             //보유한 ED가 부족합니다.
	ERR_PARTY_55,             //자동 파티 매칭중인 유저는 파티 초대를 할 수 없습니다.
	ERR_PARTY_59,             //자동 파티 매칭중일때는 파티 초대를 할 수 없습니다.
	ERR_PARTY_57,             //대전 로비에서는 파티 초대를 할 수 없습니다.
	ERR_PARTY_56,             //훈련중에는 파티원을 초대할 수 없습니다.
	ERR_MATCH_MAKING_22,             //대전 매칭 중에는 파티 초대를 할 수 없습니다.
	ERR_PARTY_58,             //자동 파티 신청을 등록한 상태에서는 파티 초대를 할 수 없습니다.
	ERR_MATCH_MAKING_21,             //대전 매칭 신청을 등록한 상태에서는 파티 초대를 할 수 없습니다.
	ERR_BATTLEFIELD_15,             //현재 필드가 파티가 소속된 필드 입니다.
	ERR_PARTY_60,             //이미 다른 파티 초대 대기자 상태여서 파티 초대를 할 수 없습니다.
	ERR_PAYLETTER_BILLING_26,             //이 이벤트 쿠폰은 계정당 한 번만 받을 수 있습니다.
	ERR_CJ_01,             //[ID] ID 또는 비밀번호가 잘못되었습니다.
	ERR_HERO_PVP_USER_LIST_02,             //등록되어있지 않은 닉네임입니다.
	ERR_HERO_PVP_USER_LIST_01,             //서버 그룹 아이디가 잘못되었습니다.
	ERR_SHARE_BANK_03,             //[TW] 공유 은행 처리중인 아이템 입니다. 은행을 닫고 다시 시도해 주세요.
	ERR_ROOM_50,             //온라인 토너먼트 참가 신청권이 없는 유저는 대회 채널을 이용하실 수 없습니다.
	ERR_RANDOM_ITEM_12,             //큐브를 개봉 하기 위해서는 아이템과 ed 가 필요합니다.
	ERR_RANDOM_ITEM_13,             //ED가 부족해서 큐브를 개봉 할 수 없습니다.
	ERR_PARTY_61,             //현재 인원으로 시작합니다.
	ERR_GUILD_CN01,             //길드 생성은 하루에 한 번 가능합니다.
	ERR_CJ_02,             //[ID] ID 는 최소 4자에서 최대 20자 까지 입력 가능 합니다.
	ERR_CJ_03,             //[ID] PW 는 최소 6자에서 최대 15자 까지 입력 가능합니다.
	ERR_ITEM_22,             //이벤트 기간이 끝났습니다.
	ERR_NX_SHOP_10,             //아라 캐릭터로는 가져올 수 없는 아이템입니다.
	ERR_PAYLETTER_BILLING_27,             //금일 선물하기 제한을 초과하였습니다.
	ERR_ITEM_EXCHANGE_05,             //아쉽게도 교환 수향이 모두 소진 되었습니다. 1월 17일 점검 이후 교환 수량이 추가 됩니다.
	ERR_CJ_04,             //[ID] 인증실패 : 서버와의 연결을 확인해 주십시오.
	ERR_PET_26,             //존재하지 않는 펫입니다.
	ERR_PET_28,             //이미 사용 중인 기능입니다.
	ERR_PET_27,             //유체 이상일 경우에만 사용 가능합니다.
	ERR_SKILL_28,             //게임 대기 중인 상태에서는 스킬 슬롯을 변경할 수 없습니다.
	ERR_BATTLEFIELD_16,             //현재 방에 입장한 상태가 아니라서 필드 몬스터 정보 불러오기를 실패하였습니다.
	ERR_BATTLEFIELD_17,             //필드방 상태가 아니어서 필드 몬스터 정보 불러오기를 실패하였습니다.
	ERR_BUY_CASH_ITEM_68,             //캐릭터 슬롯 확장 상품을 구매할 수 없습니다. 슬롯 확장은 30개까지 가능합니다.
	NOT_LEAVE_ROOM_REASON_34,             //던전 강퇴 투표에 의한 이탈입니다.
	ERR_BAD_ATTITUDE_00,             //투표자가 정상적인 유저가 아닙니다.
	ERR_BAD_ATTITUDE_01,             //투표자가 정상적인 유저가 아닙니다.
	ERR_PAYLETTER_BILLING_28,             //빌링 서버가 초기화 되지 않았습니다.
	ERR_PAYLETTER_BILLING_30,             //상품 구매중 문제가 발생하였습니다.
	ERR_PAYLETTER_BILLING_29,             //구매 처리 중입니다. 잠시 후에 다시 시도해 주세요.
	ERR_BINGO_01,             //아직 처리 중인 작업이 있습니다. 잠시 후에 다시 시도 해주세요.
	ERR_BINGO_02,             //아직 빙고판이 완성되지 않았습니다.
	ERR_BINGO_03,             //일시적인 오류입니다. 잠시 후에 다시 시도 해주세요.
	ERR_GUILD_FIND_BOARD_01,             //초대 메시지 등록 비용이 부족하므로 등록할 수 없습니다.
	ERR_GUILD_FIND_BOARD_07,             //길드 초대 메시지 등록을 실패하였습니다.
	ERR_GUILD_FIND_BOARD_04,             //길드구함 광고 등록을 실패하였습니다.
	ERR_GUILD_FIND_BOARD_03,             //길드 초대 요청이 실패하였습니다.
	ERR_GUILD_FIND_BOARD_06,             //이미 등록한 길드구함 광고가 있습니다.
	ERR_GUILD_FIND_BOARD_05,             //이미 등록한 길드 초대 메시지가 있습니다.
	ERR_GUILD_FIND_BOARD_08,             //길드구함 광고 등록 비용이 부족하므로 등록할 수 없습니다.
	ERR_GUILD_FIND_BOARD_02,             //해당 길드가 최대 인원에 도달하여 가입하실 수 없습니다.
	ERR_GUILD_FIND_BOARD_10,             //길드 초대 메시지가 존재하지 않습니다.
	ERR_GUILD_FIND_BOARD_09,             //길드구함 광고가 존재하지 않습니다.
	ERR_GUILD_FIND_BOARD_11,             //존재하지 않는 길드입니다.
	ERR_GUILD_FIND_BOARD_12,             //초대 유저 목록을 볼 수 있는 권한이 없습니다.
	ERR_DUNGEON_ENTER_LIMIT_00,             //[US] 던전 입장 회수 제한을 초과하였습니다.
	ERR_LOCALRANKING_04,             //로컬 랭킹 유저 정보 초기화 오류
	ERR_LOCALRANKING_01,             //로컬 랭킹 유저 정보가 초기화 되어있지 않습니다.
	ERR_LOCALRANKING_03,             //프로필 정보는 20자까지만 입력 가능합니다.
	ERR_LOCALRANKING_06,             //로컬 랭킹 게시판 조회 오류
	ERR_LOCALRANKING_05,             //로컬 랭킹 유저 정보 쓰기 오류
	ERR_LOCALRANKING_02,             //다음 정보 수정은 익일 06시 이후에 가능합니다.
	ERR_PARTY_62,             //[TW]2인 플레이 던전입니다.
	ERR_RECOMMEND_USER_07,             //더 이상 추천 할 수 없는 유저입니다.
	ERR_LOCALRANKING_07,             //비밀번호가 잘 못 되었습니다
	ERR_RECOMMEND_USER_09,             //투니랜드 계정이거나, 계정 인증 이후 추천 가능 합니다.
	ERR_RECOMMEND_USER_08,             //동일 명의의 계정이므로 추천 할 수 없습니다.
	ERR_RECOMMEND_USER_10,             //추천을 실패 하였습니다.
	ERR_LOCALRANKING_08,             //PASSWORD 길이가 잘못되었습니다.
	ERR_LOCALRANKING_09,             //프로필 정보에 사용할 수 없는 단어가 들어있습니다.
	ERR_HERO_PVP_USER_LIST_06,             //영웅 대전 후보 리스트에 이미 존재하는 유닛입니다.
	ERR_HERO_PVP_USER_LIST_03,             //닉네임 길이가 잘못되었습니다.
	ERR_HERO_PVP_USER_LIST_05,             //영웅 대전 후보 리스트에 존재하지 않는 유닛입니다.
	ERR_HERO_PVP_USER_LIST_04,             //--/;/따옴표/쉼표/공백 등은 사용할 수 없습니다.
	ERR_EVENT_NOTE_02,             //필드 100% 즐기기 필수 가이드북를\n 보유하고 있지 않습니다.
	ERR_EVENT_NOTE_01,             //챕터 등록을 실패 하였습니다.
	ERR_EVENT_NOTE_00,             //이미 가이드 북에 기록되어 있습니다.
	ERR_LOCALRANKING_10,             //해당 유닛은 존재하지 않습니다.
	ERR_UDP_01,             //UNKNOWN
	ERR_RELATIONSHIP_06,             //벌써 임자가 있습니다.
	ERR_RELATIONSHIP_03,             //양다리는 안됩니다!
	ERR_RELATIONSHIP_00,             //마을, 필드에서만 커플 신청을 할 수 있습니다.
	ERR_RELATIONSHIP_08,             //접속중인 유저가 아닙니다.
	ERR_RELATIONSHIP_02,             //인벤토리에 커플링이 존재하지 않습니다.
	ERR_RELATIONSHIP_05,             //동성 캐릭터간의 커플은 한국에서는 안됩니다.
	ERR_RELATIONSHIP_04,             //해당 서버군에 존재하지 않는 캐릭터 명입니다.
	ERR_RELATIONSHIP_07,             //커플 신청 받는 유저의 정보가 이상합니다.
	ERR_RELATIONSHIP_01,             //정상적인 캐릭터 명이 아닙니다.
	ERR_LOCALRANKING_12,             //알 수 없는 에러입니다.
	ERR_LOCALRANKING_11,             //존재하지 않는 유저입니다.
	ERR_RELATIONSHIP_10,             //상대방이 실수로 커플링을 떨어트렸어요! 얼른 찾아오라고 말해주세요!
	ERR_RELATIONSHIP_09,             //상대방이 수락했지만, 내 인벤토리에 커플링이 없어요! 인벤토리를 확인해 주세요!
	ERR_RELATIONSHIP_11,             //인연 시스템 사용 아이템 삭제 실패
	ERR_RELATIONSHIP_12,             //요청한 연인 정보의 캐릭터 정보가 이상합니다.
	ERR_RELATIONSHIP_13,             //당신은 솔로입니다. 연인이 없어 드릴 정보가 없습니다.
	ERR_RELATIONSHIP_17,             //선택한 옵션값이 이상합니다.
	ERR_RELATIONSHIP_14,             //인벤토리에 결혼반지가 존재하지 않습니다.
	ERR_RELATIONSHIP_16,             //선택한 옵션에 필요한 캐시 아이템이 인벤토리에 존재하지 않습니다.
	ERR_RELATIONSHIP_18,             //결혼을 하기 위해서는 커플이 되고 72시간이 지나야 합니다.
	ERR_RELATIONSHIP_15,             //결혼을 하려면 커플 상태여야 합니다.
	ERR_FREE_FIRST_CLASS_CHANGE_EVENT_01,             //무료 1차 전직 이벤트를 진행할 수 있는 상태가 아닙니다.
	ERR_RELATIONSHIP_19,             //인벤토리에 결혼에 필요한 아이템이 존재하지 않습니다.
	ERR_RELATIONSHIP_20,             //결혼하기 아이템 사용 실패 하였습니다.
	ERR_RELATIONSHIP_21,             //결혼식에 초대하려는 인원 수보다 인벤토리에 있는 초대권 수가 부족합니다.
	ERR_FREE_FIRST_CLASS_CHANGE_EVENT_02,             //이벤트 기간이 종료되어 전직이 불가능합니다.
	ERR_RIDING_PET_03,             //탈 것을 더 이상 등록 할 수 없습니다. \n사용하지 않는 탈 것을 놓아주세요!
	ERR_RIDING_PET_02,             //탈 것 생성 아이템을 사용할 수 없습니다.
	ERR_RIDING_PET_04,             //탈 것 정보를 가져오지 못했습니다.
	ERR_RIDING_PET_01,             //탈 것을 생성할 수 있는 아이템이 아닙니다.
	ERR_RIDING_PET_05,             //탈 것 생성에 실패 하였습니다.
	ERR_RIDING_PET_07,             //이미 다른 탈 것이 소환되어 있습니다.
	ERR_RIDING_PET_06,             //존재하지 않는 탈 것입니다.
	ERR_RIDING_PET_09,             //해당 탈 것은 소환되어 있지 않습니다.
	ERR_RIDING_PET_08,             //소환되어 있는 탈 것이 없습니다.
	ERR_RIDING_PET_11,             //탈 것 놓아주기에 실패하였습니다.
	ERR_RIDING_PET_10,             //소환되어 있는 탈 것은 놓아줄 수 없습니다.
	ERR_RIDING_PET_12,             //탈 것을 소환할 수 없는 지역입니다.
	ERR_RIDING_PET_13,             //보유 중인 탈 것이 없습니다.
	ERR_RELATIONSHIP_22,             //존재하지 않는 아이템에 대한 정보를 요청하였습니다.
	ERR_RELATIONSHIP_23,             //웨딩 관련 아이템이 아닙니다.
	ERR_RELATIONSHIP_24,             //웨딩 관련 아이템의 정보가 없습니다.
	ERR_RELATIONSHIP_25,             //예약된 결혼식장의 정보가 없습니다.
	ERR_RELATIONSHIP_26,             //결혼식을 예약한 유저의 닉네임을 찾을 수 없습니다.
	ERR_RELATIONSHIP_27,             //사용가능한 커플, 결혼 상태가 아닙니다.
	ERR_RELATIONSHIP_28,             //던전에서는 소환하기를 사용 할 수 없습니다.
	ERR_GLOBAL_BILLING_01,             //[INT] 지원하지 않는 캐시 타입 입니다.
	ERR_RELATIONSHIP_29,             //소환하기를 사용 할 준비가 되지 않았습니다.
	ERR_RELATIONSHIP_30,             //동일 채널에서만 소환하기가 가능합니다.
	NOT_LEAVE_ROOM_REASON_35,             //연인 소환을 위한 필드 이탈
	ERR_RELATIONSHIP_31,             //아이템 정보가 존재하지 않습니다.
	ERR_RELATIONSHIP_33,             //결혼식장에 입장 가능한 위치가 아닙니다.
	ERR_RELATIONSHIP_32,             //결혼식장에 입장 가능한 아이템이 아닙니다.
	NOT_LEAVE_ROOM_REASON_36,             //결혼식장 입장을 위한 필드 이탈
	ERR_RELATIONSHIP_34,             //결혼식이 완료 되었거나 존재하지 않는 결혼식입니다.
	ERR_RELATIONSHIP_36,             //커플 상태가 아닙니다.
	ERR_RELATIONSHIP_37,             //결혼식장에 입장이 실패하였습니다.
	ERR_RELATIONSHIP_38,             //현재 필드에 소환이 불가능 합니다.
	ERR_RELATIONSHIP_39,             //이별이 가능한 위치가 아닙니다.
	ERR_RELATIONSHIP_40,             //위자료를 지불 할 수 없습니다.
	ERR_RELATIONSHIP_41,             //신랑이나 신부가 아닙니다.
	ERR_RELATIONSHIP_42,             //신랑 신부가 모두 방에 존재해야 합니다.
	ERR_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT_01,             //사랑의 리퀘스트를 가지고 오세요~뽀~
	ERR_RELATIONSHIP_SYSTEM_LAUNCHING_EVENT_02,             //새콤달콤 뽀피트 미니미가 우편으로 지급되었습니다.
	ERR_REQUEST_OF_LOVE_01,             //새콤달콤 뽀피트 미니미가 우편으로 지급되었습니다.
	ERR_REQUEST_OF_LOVE_02,             //사랑의 리퀘스트를 가지고 오세요~뽀~
	ERR_REQUEST_OF_LOVE_03,             //뽀루의 러브 레터를 발송하려면 1만 ED가 필요합니다.
	ERR_REQUEST_OF_LOVE_04,             //뽀루의 러브 레터를 발송하지 못했습니다.
	ERR_RELATIONSHIP_43,             //인연 상태가 아닙니다.
	ERR_RELATIONSHIP_44,             //애칭 변경권의 정보가 이상합니다.
	ERR_DELETE_UNIT_06,             //인연 상태에서는 삭제 할 수 없습니다.
	ERR_COBO_EXPRESS_01,             //[ES_INT] VIP 기간 갱신 중 문제가 발생했습니다.
	ERR_SYNTHESIS_03,             //[JP] 선택 할 수 없는 옵션입니다.
	ERR_SYNTHESIS_04,             //[JP] 합성 부위가 다릅니다.
	ERR_RELATIONSHIP_45,             //결혼 상태가 아닙니다.
	ERR_RELATIONSHIP_46,             //연인이 필드 입장에 실패 하였습니다.
	ERR_EVALUATE_ITEM_05,             //미라클 큐브의 아이템 레벨이 맞지 않습니다.
	ERR_EVALUATE_ITEM_03,             //이미 감정이 된 아이템 입니다.
	ERR_EVALUATE_ITEM_00,             //연인이 필드 입장에 실패 하였습니다.
	ERR_EVALUATE_ITEM_02,             //아이템 감정을 위한 수수료가 부족합니다.
	ERR_EVALUATE_ITEM_04,             //아이템 감정을 실패 하였습니다.
	ERR_EVALUATE_ITEM_06,             //미라클 큐브의 수량이 부족합니다.
	ERR_EVALUATE_ITEM_01,             //해당 아이템은 감정할 수 없습니다.
	ERR_RED_VS_BLUE_EVENT_00,             //[ES_INT]이미 같은 계정의 다른 캐릭터로 신청하셨습니다.
	ERR_RED_VS_BLUE_EVENT_01,             //[ES_INT]이미 해당 캐릭터로 신청하셨습니다.
	ERR_EVALUATE_ITEM_07,             //변경 대상 아이템이 아닙니다.
	ERR_EVALUATE_ITEM_08,             //아이템 변경에 필요한 수수료가 부족합니다.
	ERR_DANO_EVENT_01,             //[ES_INT]금일 이벤트에 응모할 응모권이 없습니다.
	ERR_DANO_EVENT_02,             //[ES_INT]금일 이벤트 응모에 실패하였습니다. 다시 시도해 주세요.
	ERR_DANO_EVENT_03,             //[ES_INT]이벤트 기간이 끝났습니다.
	ERR_SKILL_29,             //동일 등급의 스킬을 동시에 습득하려고 합니다.
	ERR_EXCHANGE_LIMIT_02,             //교환 가능한 수량이 모두 소진되었습니다.
	ERR_EXCHANGE_LIMIT_01,             //아이템 교환에 실패하였습니다.
	ERR_EXCHANGE_LIMIT_03,             //오늘의 붉은 기사단 초대 확정권은\n모두 소진되었습니다.\n일일 수량은 06시에 갱신됩니다.\n내일 다시 도전해주세요!
	ERR_TRADE_BLOCK_UNIT_02,             //새로 생성한 캐릭터는 3일간 거래를 이용할 수 없습니다.
	ERR_ITEM_EXCHANGE_06,             //기간제 아이템은 재료나 교환 아이템으로\n사용할 수 없습니다.
	ERR_MANUFACTURE_12,             //기간제 아이템은 재료나 교환 아이템으로\n사용할 수 없습니다.
	ERR_ATTACH_ITEM_04,             //요구 조건을 만족하지 않습니다. 확인해주세요.
	ERR_LEVELUP_01,             //[BR] 레벨업 인증 에러
	ERR_RELATIONSHIP_47,             //엘소드와 엘리시스는 남매이므로 커플 요청을 할 수 없습니다.
	ERR_NOT_USE_01,             //현재 레벨에서는 사용할 수 없습니다.
	ERR_CREATE_UNIT_11,             //선택한 유닛 클래스 정보가 이상합니다.
	ERR_JUMPING_04,             //해당되는 점핑 이벤트가 없습니다.
	ERR_JUMPING_01,             //점핑 이벤트 대상자가 아닙니다.
	ERR_JUMPING_07,             //점핑 하려는 대상 직업이 이상합니다.
	ERR_JUMPING_06,             //점핑은 마을이나 필드에서만 가능합니다.
	ERR_JUMPING_03,             //점핑 이벤트 정보가 초기화 되지 않았습니다.
	ERR_JUMPING_08,             //점핑 캐릭터 오류입니다.
	ERR_JUMPING_02,             //기본 직업 대상자만 점핑할 수 있습니다.
	ERR_JUMPING_05,             //점핑 이벤트 대상자가 아닙니다.
	ERR_EXTRACT_06,             //아이템을 생성하지 못했습니다.
	ERR_EXTRACT_03,             //기간제 아이템은 추출 할 수 없습니다.
	ERR_EXTRACT_00,             //추출 가능한 아이템이 아닙니다.
	ERR_EXTRACT_02,             //아이템 수량이 이상합니다.
	ERR_EXTRACT_05,             //인벤토리 공간이 부족합니다.
	ERR_EXTRACT_04,             //아이템 추출에 실패하였습니다.
	ERR_EXTRACT_01,             //추출 아이템 정보가 이상합니다.
	ERR_FINALITY_SKILL_03,             //아이템 사용에 실패하였습니다.
	ERR_FINALITY_SKILL_00,             //궁극기 사용 아이템이 아닙니다.
	ERR_FINALITY_SKILL_02,             //아이템 수량이 이상합니다.
	ERR_FINALITY_SKILL_01,             //아이템 정보가 이상합니다.
	ERR_ODBC_06,             //해당 데이터 없음
	ERR_BLOCK_TRADE_07,             //거래 정지 상태의 계정은 개인 거래를 할 수 없습니다.
	ERR_USE_ITEM_IN_QUICKSLOT_00,             //배틀필드에서는 NPC카드를 사용할 수 없습니다
	ERR_EVENT_PET_02,             //이 열매는 '베르드의 마수'에게만 먹일 수 있습니다.
	ERR_EVENT_PET_01,             //'정화의 열매'만 먹일 수 있습니다.
	ERR_NX_COUPON_11,             //취소된 쿠폰입니다.
	ERR_NX_COUPON_08,             //PC방에서만 사용할 수 있는 쿠폰입니다.
	ERR_NX_COUPON_07,             //쿠폰 오류입니다.
	ERR_NX_COUPON_13,             //사용 제한 수량을 초과하였습니다.
	ERR_NX_COUPON_10,             //쿠폰 등록에 실패하였습니다.
	ERR_NX_COUPON_09,             //쿠폰 정보 읽기에 실패하였습니다.
	ERR_NX_COUPON_12,             //쿠폰핀 규칙이 맞지 않습니다.
	ERR_NX_COUPON_06,             //잘못된 쿠폰 박스 타입입니다.
	ERR_NX_COUPON_14,             //해당 쿠폰의 할인 적용 가능 아이템 목록 읽기에 실패하였습니다.
	ERR_CLIENT_QUIT_03,             //CheckKom.xml 파싱 오류로 인한 종료입니다.
	ERR_GARENA_BILLING_01,             //
	ERR_ROOM_51,             //스테이지 정보가 잘못되었습니다.
	ERR_RELATIONSHIP_EVENT_00,             //인벤토리에 가상 결혼 신청서가 존재하지 않습니다.
	ERR_RELATIONSHIP_EVENT_03,             //인벤토리에 가상 결혼 계약서가 존재하지 않습니다.
	ERR_RELATIONSHIP_EVENT_02,             //가상 결혼 상대방이 존재하지 않습니다.
	ERR_RELATIONSHIP_EVENT_04,             //가상 결혼 계약서 삭제 실패
	ERR_RELATIONSHIP_EVENT_01,             //가상 결혼 신청서 삭제 실패
	ERR_HW_PET_03,             //조건이 맞지 않습니다. 펫이 소환되어 있는지 확인 바랍니다.
	ERR_HW_PET_02,             //조건이 맞지 않습니다. 사용 가능한 펫 인지 확인 바랍니다.
	ERR_HW_PET_04,             //축하합니다! 할로윈 호박 요정의 묘약을 성공적으로 사용 하셨습니다.
	ERR_HW_PET_01,             //조건이 맞지 않습니다. 펫 친밀도가 100% 인지 확인 바랍니다.
	NOT_CREATE_CHARACTER_BY_COUNT,             //캐릭터 생성 횟수제한에 의해 캐릭터를 생성할 수 없습니다.
	ERR_RELATIONSHIP_EVENT_05,             //상대방이 수락했지만, 내 인벤토리에 가상 결혼 신청서가 없어요! 인벤토리를 확인해 주세요!
	ERR_RELATIONSHIP_EVENT_06,             //상대방이 실수로 가상 결혼 신청서를 잃어버렸어요! 얼른 찾아오라고 말해주세요!
	ERR_RELATIONSHIP_EVENT_07,             //가상 부부 상대방이 있어야 시작할 수 있습니다.
	ERR_MATCH_MAKING_23,             //던전 게임 신청 중에는 사용할 수 없는 기능입니다.
	ERR_AUTO_PARTY_07,             //공식 대전 신청 중에는 사용할 수 없는 기능입니다.
	ERR_AUTO_PARTY_08,             //룸 리스트 조회 중에는 사용할 수 없는 기능입니다.
	ERR_DUNGEON_01,             //올바르지 않은 SubStageID 입니다.
	ERR_ROOM_52,             //플레이 가능 횟수를 초과하였습니다.
	ERR_EVENT_PET_03,             //브라질 쁘띠 뽀루에게만 사용 할 수 있습니다.
	ERR_ROOM_53,             //공식 대전에는 난입할 수 없습니다.
	ERR_ROOM_54,             //잘못된 스테이지 정보 요청입니다.
	ERR_NX_COUPON_17,             //적용 가능 하한 금액 미만입니다.
	ERR_NX_COUPON_16,             //적용 가능 상한 금액 초과입니다.
	ERR_NX_COUPON_19,             //이미 등록된 쿠폰입니다.
	ERR_NX_COUPON_18,             //할인 대상 아이템이 아닙니다.
	ERR_NX_COUPON_15,             //PC방이 아닙니다.
	ERR_PERSONAL_SHOP_57,             //현재 대리 판매 보증서(VIP)를 이용 중입니다
	ERR_PERSONAL_SHOP_58,             //대리상인에 등록된 아이템이 있어서 다른 종류의 보증서를 등록할 수 없습니다.
	NOT_REQUEST_TRADE_REPLY_05,             //해당 유저는 거래가 제한된 상태입니다.
	ERR_EXPAND_INVENTORY_ED_01,             //ED 가 부족합니다
	ERR_EXPAND_INVENTORY_ED_02,             //더 이상 슬롯을 확장할 수 없습니다.
	ERR_EXPAND_INVENTORY_ED_03,             //확장에 필요한 금액이 잘못되었습니다.
	ERR_BATTLEFIELD_19,             //해당 필드는 열려있지 않습니다.
	ERR_BATTLEFIELD_18,             //현재 필드에서는 입장할 수 없습니다.
	ERR_BATTLEFIELD_20,             //재도전을 위한 휴식 버프로 인해 시간의 포탈에 입장 할 수 없습니다. 다음 포탈 생성시까지 기다려 주세요.
	ERR_CLIENT_QUIT_04,             //Kom 파일의 변조로 인한 종료입니다.
	ERR_NX_COUPON_20,             //쿠폰 총 사용 횟수를 초과하여 사용하실 수 없습니다.
	ERR_ROOM_55,             //보스 필드 입장에 실패하였습니다. 다시 시도 해주세요.
	ERR_ADMIN_COMMAND_10,             //사용할 수 없는 권한입니다.
	ERR_FIELD_BOSS_02,             //보스 필드 쿨타임 상태입니다. 닫은 후에 다시 시도해주세요.
	ERR_FIELD_BOSS_01,             //해당 필드에서는 보스 필드 게이트가 생성되지 않습니다.
	ERR_FIELD_BOSS_04,             //사용 권한이 없습니다.
	ERR_FIELD_BOSS_03,             //보스 필드가 아닙니다.
	ERR_FIELD_BOSS_05,             //쿨 타임 상태입니다.
	ERR_FIELD_BOSS_06,             //보스 필드정보가 존재하지 않습니다.
	ERR_DELETE_UNIT_07,             //사전 생성한 캐릭터는 삭제할 수 없습니다
	ERR_FIELD_BOSS_07,             //보스 필드의 제한 시간이 종료되었습니다.
	ERR_FIELD_BOSS_13,             //무기와 방어구 아이템에만 사용 할 수 있습니다.
	ERR_FIELD_BOSS_19,             //소켓 슬롯 추가에 실패하였습니다.
	ERR_FIELD_BOSS_16,             //인벤토리의 재료 아이템만 사용할 수 있습니다.
	ERR_FIELD_BOSS_10,             //파괴된 아이템에는 사용할 수 없습니다.
	ERR_FIELD_BOSS_18,             //방어구 소켓 슬롯을 추가하기 위한 재료 아이템을 가지고 있지 않습니다.
	ERR_FIELD_BOSS_15,             //소켓 슬롯 추가에 필요한 재료 아이템이 존재하지 않습니다.
	ERR_FIELD_BOSS_09,             //인벤토리의 아이템에만 사용할 수 있는 기능입니다.
	ERR_FIELD_BOSS_12,             //장착 불가 아이템에는 사용할 수 없습니다.
	ERR_FIELD_BOSS_11,             //PC방 프리미엄 아이템에는 사용할 수 없습니다.
	ERR_FIELD_BOSS_17,             //무기 소켓 슬롯을 추가하기 위한 재료 아이템을 가지고 있지 않습니다.
	ERR_FIELD_BOSS_14,             //더 이상 소켓 슬롯을 추가 할 수 없습니다.
	ERR_FIELD_BOSS_08,             //소켓 슬롯 추가에 필요한 대상 아이템이 존재하지 않습니다.
	ERR_HACKSHIELD_00,             //hsb 파일이 클라이언트와 맞지 않습니다.
	ERR_SKILL_PAGE_04,             //활성화 중인 스킬 페이지 번호가 이상 합니다.
	ERR_SKILL_PAGE_01,             //클라이언트에서 보내준 활성화된 스킬 페이지 번호가 서버의 정보와 다릅니다.
	ERR_SKILL_PAGE_03,             //사용하기로 선택한 스킬 페이지가 이상 합니다.
	ERR_SKILL_PAGE_05,             //추가된 페이지에 기본을 스킬 등록 하지 못 했습니다.
	ERR_SKILL_PAGE_02,             //더 이상 스킬 페이지를 확장 할 수 없습니다.
	ERR_SKILL_PAGE_06,             //스킬 페이지를 확장한 적이 없는데 페이지 변경을 시도함
	ERR_SKILL_PAGE_07,             //스킬 페이지 변경 업데이트 실패
	ERR_SKILL_PAGE_08,             //마을 및 휴식처에서만 구매 가능 합니다.
	ERR_SKILL_PAGE_09,             //1개권 만 구입 가능 합니다.
	ERR_CLASS_CHANGE_EVENT_01,             //더 이상 구입 할 수 없습니다.
	ERR_ITEM_INT_01,             //버프 효과가 지속되고 있는 동안 동일한 버프 효과의 아이템을 다시 사용할 수 없습니다.
	ERR_AUTO_PARTY_09,             //레벨 그룹이 다른 유저와 전장을 같이 플레이 할 수 없습니다.
	NOT_LEAVE_PARTY_REASON_07,             //불량 유저 검사 시스템에 의한 파티 이탈입니다.
	NOT_LEAVE_PARTY_REASON_08,             //파티 재결합에 의한 파티 이탈입니다.
	NOT_LEAVE_PARTY_REASON_09,             //유효하지 않은 파티원 검사에 의한 파티 이탈입니다.
	NOT_LEAVE_PARTY_REASON_10,             //어둠의 문 진입 전 이탈로 인한 파티 이탈입니다.
	ERR_ACCOUNT_BLOCK_03,             //당신의 계정은 차단되었습니다(LE3) 웹사이트를 방문하여 문의 하세요.
	ERR_ACCOUNT_BLOCK_02,             //로그인이 불가능합니다(LE2). 웹사이트를 방문하여 문의 하세요.
	ERR_ACCOUNT_BLOCK_04,             //당신의 계정은 차단되었습니다(LE4) 웹사이트를 방문하여 문의 하세요.
	ERR_ACCOUNT_BLOCK_01,             //로그인이 불가능합니다(LE1). 웹사이트를 방문하여 문의 하세요.
	ERR_AUTO_PARTY_10,             //PVE게임에 존재하지 않는 던전 모드입니다.

	ERR_SENTINEL
};

static wchar_t* szErrorStr[] = {
	L"성공",
	L"알 수 없는 에러입니다.",
	L"유저 상태 오류입니다.",
	L"버전이 맞지 않습니다. 패치 후 재접속 하세요.",
	L"서버 수용 인원이 가득 찼습니다.",
	L"게임서버 점검중입니다. 점검시간이 끝난후에 접속해주세요.",
	L"동일서버간 이중 접속 시도입니다.",
	L"타서버간 이중접속 시도입니다.",
	L"서버 IP 검증 실패입니다.",
	L"서버 UID 충돌입니다.",
	L"유저가 존재하지 않습니다.",
	L"삭제된 유저입니다.",
	L"패스워드가 다릅니다.",
	L"서버 작동 플래그 설정 실패입니다.",
	L"NEXON 인증 실패입니다.",
	L"차단된 계정입니다.",
	L"운영자 이상 레벨만 요청 가능합니다.",
	L"프록시 ID가 없습니다.",
	L"허용되지 않은 IP 입니다.",
	L"게임서버의 해당 채널 ID정보가 없습니다.",
	L"ID의 길이가 이상합니다.",
	L"트랜잭션 에러입니다.",
	L"ID 길이가 잘못되었습니다.",
	L"PASSWORD 길이가 잘못되었습니다.",
	L"이름 길이가 잘못되었습니다.",
	L"삭제된 유저가 캐릭터 생성을 시도하였습니다.",
	L"캐릭터 닉네임이 이미 존재합니다.",
	L"트랜잭션 에러입니다.",
	L"캐릭터 데이터 로드 실패입니다.",
	L"캐릭터 닉네임 길이가 잘못되었습니다.",
	L"더 이상의 캐릭터를 생성할 수 없습니다.",
	L"해당 닉네임은 일정 기간 동안 사용할 수 없습니다.",
	L"넥슨 메신저용 CharacterSN을 DB에 생성하지 못했습니다.",
	L"사용할 수 없는 닉네임입니다.",
	L"캐릭터는 하루에 네번만 생성할 수 있습니다.",
	L"선택한 캐릭터가 게임 서버에 없습니다.",
	L"선택한 캐릭터의 클래스가 이상합니다.",
	L"부활석 정보를 DB에서 받아오지 못했습니다.",
	L"넥슨 메신저용 CharacterSN을 DB에서 받아오지 못했습니다.",
	L"캐릭터 선택 과정에서 DB 오류가 발생하였습니다.",
	L"닉네임을 변경해야 합니다.",
	L"레벨에 맞는 PVP채널을 찾지 못했습니다.",
	L"채널에서 삭제할 유저를 찾지 못했습니다.",
	L"채널 이동 실패하였습니다.",
	L"VP가 부족하여 입장할 수 없습니다.",
	L"광장 채널에 입장하지 못했습니다.",
	L"채널 입장하지 못했습니다.",
	L"진행중인 대회가 없습니다.",
	L"파티 리스트를 불러올 수 없습니다.",
	L"던전 게이트에서만 파티 리스트를 볼 수 있습니다.",
	L"해당 던전 게이트에서 볼 수 있는 파티리스트가 아닙니다.",
	L"3레벨부터 대전을 할 수 있습니다.",
	L"방 서버에 더 이상 방을 만들 수 없습니다.",
	L"해당 슬롯에서 유저를 발견할 수 없습니다.",
	L"해당 캐릭터가 있는 슬롯을 찾을 수 없습니다.",
	L"방이 꽉 찼습니다.",
	L"비밀 번호가 틀립니다.",
	L"선행던전을 클리어하지 않았습니다.",
	L"퀵조인 할 수 있는 방이 없습니다.",
	L"방장만 요청할 수 있습니다.",
	L"패킷이 허용되는 방 상태가 아닙니다.",
	L"준비 상태가 아닌 유저가 있습니다.",
	L"팀 인원수가 같지 않습니다.",
	L"한 명 이상 READY 해야 시작 할 수 있습니다.",
	L"로딩 상태를 변경하지 못했습니다.",
	L"방이 생성되었습니다.",
	L"방 정보가 변경되었습니다.",
	L"방이 삭제되었습니다.",
	L"방에서 유저를 검색하지 못했습니다.",
	L"난입 실패하였습니다.",
	L"난입 가능 방에서 나가지 못했습니다.",
	L"죽은 NPC UID가 잘못되었습니다.",
	L"이미 죽은 NPC입니다.",
	L"몬스터 데이터가 잘못되어 로딩 실패하였습니다.",
	L"대상 유저를 찾을 수 없습니다.",
	L"채팅 타입이 이상합니다.",
	L"자기 자신을 강퇴 시킬수 없습니다.",
	L"근성도가 부족합니다.",
	L"추가 스탯을 지원하지 않는 게임 타입입니다.",
	L"게임 종료중이라 난입할 수 없습니다.",
	L"방에 입장할 수 없습니다.",
	L"입장 중인 방을 알 수 없습니다.",
	L"거래 중에는 게임 준비를 할 수 없습니다.",
	L"게임 시작 중이라 난입할 수 없습니다. 잠시 후에 다시 시도해주세요.",
	L"게임 시작 아이템이 인벤토리에 없습니다.",
	L"권한이 없는 유저가 옵저버 방을 만들려고 시도했습니다.",
	L"잘못된 대전맵 정보입니다.",
	L"오프라인 대회 참여권을 가지고 있는 유저는 대회 채널을 이용하실 수 없습니다.",
	L"해당 던전을 플레이 하기 위한 아이템이 없습니다. 게임 시작 시 방에서 자동으로 이탈하게 됩니다.",
	L"대전 방에서만 초대 기능을 사용하실 수 있습니다.",
	L"초대하려는 상대가 대전방에 입장할 수 없는 상태입니다.",
	L"대전 채널 로비에 있는 유저만 초대할 수 있습니다.",
	L"파티에 속해 있어야 게임이 시작가능합니다.",
	L"킬 수 세팅이 이상합니다.",
	L"습득하려는 아이템이 없습니다.",
	L"다른 사람이 이미 습득하였습니다.",
	L"랜덤으로 아이템 주기가 실패하였습니다.",
	L"아이템 정보를 찾지 못했습니다.",
	L"아이템 ID가 잘못되었습니다.",
	L"아이템을 주지 못했습니다.",
	L"아이템을 수리하지 못했습니다.",
	L"퀵슬롯에 있는 아이템을 사용하지 못했습니다.",
	L"이벤트 던전에서는 퀵슬롯을 사용할 수 없습니다.",
	L"아이템을 획득하지 못했습니다.",
	L"임시 인벤토리의 아이템을 가져오지 못했습니다.",
	L"임시 인벤토리 아이템을 인벤토리에 넣을 공간이 부족합니다.",
	L"확성기 메시지 길이가 너무 깁니다.",
	L"팀을 변경하지 못했습니다.",
	L"슬롯 상태를 변경하지 못했습니다.",
	L"준비 상태를 변경하지 못했습니다.",
	L"슬롯에서 나가지 못했습니다.",
	L"강제 퇴장 시키지 못했습니다.",
	L"자신을 죽인 유저 데이터를 저장하지 못했습니다.",
	L"상태를 변경하지 못했습니다.",
	L"유저에 의한 퇴장입니다.",
	L"접속이 종료되었습니다.",
	L"강퇴로 인한 퇴장입니다.",
	L"레벨 업으로 인하여 해당 채널에 머무를 수 없습니다.",
	L"개인 거래가 완료되었습니다.",
	L"개인 거래 참가 실패입니다.",
	L"개인 거래 참가 지연으로 인한 퇴장입니다.",
	L"개인 거래가 취소되었습니다.",
	L"개인 거래 인원 수가 이상합니다.",
	L"개인 거래 중 오류가 발생하였습니다.",
	L"인벤토리 공간이 부족하거나 등록된 아이템 정보가 이상합니다.",
	L"근성도가 부족하여 던전을 진행 할 수 없습니다.",
	L"거래할 물품이 없어서 거래가 취소되었습니다.",
	L"거래가 취소되었습니다.",
	L"개인상점 물품등록 지연으로 상점이 종료되었습니다.",
	L"상점거래가 취소되었습니다.",
	L"개인 상점의 모든 물품이 판매 완료 되어 상점이 종료되었습니다.",
	L"방에서 퇴장하여 거래가 취소되었습니다.",
	L"개인 상점이 판매 종료 되었습니다.",
	L"클라이언트 크래쉬입니다.",
	L"대회 채널에 더 이상 머무를 수 없습니다.",
	L"대전 게임 진입으로 인한 파티 탈퇴입니다.",
	L"던전 게임 중단으로 인한 파티 탈퇴입니다.",
	L"마켓 진입으로 인한 파티 탈퇴입니다.",
	L"훈련소 진입으로 인한 파티 탈퇴입니다.",
	L"정상 종료입니다.",
	L"해킹 감지로 인한 종료입니다.",
	L"클라이언트 크래쉬로 인한 종료입니다.",
	L"캐릭터 경험치 수정 실패입니다.",
	L"요청한 캐릭터의 클래스를 DB에서 알아내지 못했습니다.",
	L"권한 레벨이 잘못되었습니다.",
	L"해당 유저가 없습니다.",
	L"트랜잭션 에러입니다.",
	L"해당 권한 레벨의 유저가 없음.",
	L"운영자에 의해 접속이 종료되었습니다.",
	L"이 명령어는 사내에서만 실행할 수 있습니다.",
	L"이 명령어는 방에서 실행할 수 없습니다.",
	L"아이템을 구입할 수 없습니다.",
	L"아이템을 되팔 수 없습니다.",
	L"수행 중인 퀘스트입니다.",
	L"수행 중이지 않은 퀘스트를 지우려고 하였습니다.",
	L"존재하지 않는 퀘스트입니다.",
	L"완료한 퀘스트입니다.",
	L"이 캐릭터는 해당 퀘스트를 수행할 수 없는 캐릭터입니다.",
	L"이전의 필요한 퀘스트를 완료하지 않았습니다.",
	L"NPC에게 잘못된 퀘스트를 받았습니다.",
	L"퀘스트 템플릿이 존재하지 않습니다.",
	L"완료 조건이 만족되지 않았습니다.",
	L"퀘스트 관련 DB처리가 실패하였습니다.",
	L"퀘스트는 최대 10개까지만 수행이 가능합니다.",
	L"레벨이 낮아서 아직 퀘스트를 수락할 수 없습니다.",
	L"인벤토리 공간이 부족하여 퀘스트를 완료할 수 없습니다. 인벤토리를 먼저 정리해 주세요.",
	L"퀘스트 선택 보상 처리 중 오류가 발생하였습니다.",
	L"선택 보상 아이템 검색이 실패하였습니다.",
	L"이미 전직 퀘스트를 수행 중입니다.",
	L"전직 퀘스트 보상 처리 과정 중 오류가 발생하였습니다.",
	L"퀘스트 타입이 이상합니다.",
	L"채팅 입력 제한 시간보다 짧게 입력하였습니다.",
	L"자기 자신에게는 귓속말을 할 수 없습니다.",
	L"채팅 종류가 이상합니다.",
	L"현재 접속중이 아닙니다.",
	L"--/;/따옴표/쉼표/공백 등은 사용할 수 없습니다.",
	L"유저 리스트 갱신시간보다 짧게 입력하였습니다.",
	L"유저정보 검색 제한시간이 지나지 않았습니다.",
	L"현재 접속 중이 아닙니다.",
	L"존재하는 닉네임이 아닙니다.",
	L"광장 입장 실패입니다.",
	L"광장 나가기 실패입니다.",
	L"이미 최고 레벨까지 배운 스킬입니다.",
	L"존재하지 않는 스킬입니다.",
	L"레벨이 낮아서 아직 스킬을 배울 수 없습니다.",
	L"스킬 포인트가 부족합니다.",
	L"습득 조건 아이템이 없습니다.",
	L"습득 조건 퀘스트를 클리어 하십시오.",
	L"선행 습득 스킬을 배우지 않았습니다.",
	L"이미 배운 스킬입니다.",
	L"변경할 스킬 슬롯 아이디가 잘못되었습니다.",
	L"배우지 않은 스킬을 등록하려고 하였습니다.",
	L"스킬을 초기화하지 못했습니다.",
	L"스킬 슬롯 B를 사용하실 수 없습니다.",
	L"스킬 슬롯 B 확장 중 DB오류가 발생하였습니다.",
	L"봉인 해제되지 않은 스킬은 배울 수 없습니다.",
	L"배울 수 없는 계층에 있는 스킬입니다.",
	L"스킬 포인트가 부족해서 스킬을 획득할 수 없습니다.",
	L"스킬 포인트가 부족해서 스킬을 획득할 수 없습니다.",
	L"존재하지 않는 스킬의 정보를 변경하려고 하였습니다.",
	L"스킬 레벨 정보가 이상합니다.",
	L"스킬 제한 레벨보다 높은 레벨로 설정하려고 하였습니다.",
	L"캐시 스킬포인트 기간 만료 처리가 실패하였습니다.",
	L"캐시 스킬포인트 등록이 실패하였습니다.",
	L"캐시 스킬포인트 정보 갱신이 실패하였습니다.",
	L"캐시 스킬포인트 정보 초기화가 실패하였습니다.",
	L"기본 스킬은 초기화 할 수 없습니다.",
	L"파티를 생성하지 못했습니다.",
	L"파티에서 탈퇴하지 못했습니다.",
	L"파티 정보가 잘못되었습니다.",
	L"초대할 수 없는 상태입니다.",
	L"소속된 파티가 없습니다.",
	L"이미 파티에 가입되어 있습니다.",
	L"대전에서는 파티를 생성할 수 없습니다.",
	L"대전에서는 파티에 입장할 수 없습니다.",
	L"파티 인원이 다 찼습니다.",
	L"파티에 가입하지 못했습니다.",
	L"삭제된 파티입니다.",
	L"파티 입장이 거절되었습니다.",
	L"파티 초대를 거부하였습니다.",
	L"준비 할 수 없는 장소입니다.",
	L"준비 상태 변경 실패입니다.",
	L"던전 선택이 잘못되었습니다.",
	L"게임 시작이 실패하였습니다.",
	L"게임 타입이 잘못되었습니다.",
	L"파티장이 아닙니다.",
	L"파티장을 변경하지 못했습니다.",
	L"파티 인원을 변경하지 못했습니다.",
	L"이 던전 게이트 소속 던전이 아닙니다.",
	L"공개 설정을 변경하지 못했습니다.",
	L"새로 임명될 파티장이 현재의 던전을 수행할 수 없습니다.",
	L"파티장은 레디 상태 변경을 할 수 없습니다.",
	L"채팅 상대자가 없습니다.",
	L"파티 리스트 요청 타입이 이상합니다.",
	L"레벨 제한 설정을 변경하지 못했습니다.",
	L"비공개 파티입니다.",
	L"부활석이 없습니다.",
	L"이미 부활 하였습니다.",
	L"부활 실패입니다.",
	L"이 던전에서는 부활석을 사용할 수 없습니다.",
	L"없는 상태 값을 업데이트 하려고 하였습니다.",
	L"커뮤니티 옵션 값을 받아오지 못했습니다.",
	L"더이상 블랙리스트에 등록할 수 없습니다.",
	L"이미 블랙리스트에 등록된 유저입니다.",
	L"이미 블랙리스트에서 삭제된 유저입니다.",
	L"존재하지 않는 유저입니다.",
	L"블랙리스트에서 삭제하지 못했습니다.",
	L"자기 자신을 블랙리스트에 등록할 수 없습니다.",
	L"자기 자신은 블랙리스트에서 삭제할 수 없습니다.",
	L"현재 접속 중인 유저만 블랙리스트에 등록할 수 있습니다.",
	L"자신의 캐릭터를 추가할 수 없습니다.",
	L"친구 추가 제한 시간이 지나지 않았습니다.",
	L"캐릭터를 찾을 수 없습니다.",
	L"내 캐릭터가 초대할 수 없는 상태입니다.",
	L"상대방 캐릭터가 초대 받을 수 없는 상태입니다.",
	L"상대방 캐릭터가 같이 할 수 없는 상태입니다.",
	L"친구로 초대할 수 없습니다.",
	L"개인 거래를 생성하지 못했습니다.",
	L"개인 거래에서 이탈하지 못했습니다.",
	L"개인 거래 유저를 검색하지 못했습니다.",
	L"등록하려는 아이템 정보가 이상합니다.",
	L"등록하려는 ED 정보가 이상합니다.",
	L"거래 아이템이 존재하지 않습니다.",
	L"거래 ED가 부족합니다.",
	L"인벤토리에 있는 아이템만 등록할 수 있습니다.",
	L"인벤토리 공간이 부족합니다.",
	L"귀속 아이템은 거래할 수 없습니다.",
	L"거래채팅을 할 수 없는 장소입니다.",
	L"자기 자신과 거래할 수 없습니다.",
	L"내구도가 소모된 아이템은 거래할 수 없습니다. 아이템을 수리하고 거래해주세요.",
	L"이미 등록한 아이템입니다.",
	L"등록되지 않은 아이템을 삭제하려고 하였습니다.",
	L"거래 가능한 아이템 종류는 최대 10가지입니다.",
	L"거래가 성사되면 상대방의 ED 보유량이 최대치를 초과합니다. 거래 ED량을 줄여주세요.",
	L"개인 거래 요청을 수락하였습니다.",
	L"이미 다른 사람과 거래 중입니다.",
	L"개인 거래 요청을 거절하였습니다.",
	L"거래할 수 없는 상태입니다.",
	L"서버 오류로 거래를 할 수 없습니다.",
	L"던전 입장에 필요한 아이템을 가지고 있지 않습니다.",
	L"훈련소 입장 조건을 만족하지 않습니다.",
	L"훈련소를 진행할 던전이 ID가 없습니다.",
	L"보상 처리 중 오류가 발생하였습니다.",
	L"선택한 아이템은 인벤토리에 없는 아이템입니다.",
	L"선택한 아이템은 큐브 아이템이 아닙니다.",
	L"이 캐릭터로는 사용할 수 없는 아이템입니다.",
	L"열쇠가 부족하여 큐브를 열 수 없습니다.",
	L"큐브를 열 수 없습니다.",
	L"아이템을 생성하지 못했습니다.",
	L"해당 큐브를 열기위한 인벤토리의 공간이 부족합니다.",
	L"이중 접속이 감지되었습니다.",
	L"유저 UID가 이상합니다.",
	L"유저 ID가 이상합니다.",
	L"캐릭터 UID가 이상합니다.",
	L"캐릭터 닉네임이 이상합니다.",
	L"게임 서버 UID가 이상합니다.",
	L"유저 정보 포인터가 이상합니다.",
	L"게임 서버 포인터가 이상합니다.",
	L"유저 UID 검색이 실패하였습니다.",
	L"메신져 고유 번호가 이상합니다.",
	L"아이템 제조를 할 수 없습니다.",
	L"주문서 아이템이 아닙니다.",
	L"ED가 부족하여 제조를 할 수 없습니다.",
	L"재료 아이템이 부족합니다.",
	L"제조를 하기 위해서는 해당 인벤토리에 빈칸이 1개 이상 있어야 합니다.",
	L"아이템을 생성하지 못했습니다.",
	L"HackShield 인증이 실패하였습니다.",
	L"HackShield 업데이트로 인하여 재시작해야 합니다.",
	L"HackShield 인증이 실패하였습니다.",
	L"이미 사용한 쿠폰이거나 쿠폰 번호가 잘못 입력되었습니다. 쿠폰은 계정당 1회만 사용 가능합니다.",
	L"쿠폰 번호 30자를 입력해주세요.",
	L"이미 사용된 쿠폰입니다.",
	L"기간이 만료된 쿠폰입니다.",
	L"쿠폰 번호가 올바르지 않습니다.",
	L"이 쿠폰은 계정당 한 번만 사용 가능합니다.",
	L"쿠폰을 사용하지 못했습니다.",
	L"재구매 일수 미만입니다.",
	L"아이템 리스트를 불러올 수 없습니다.",
	L"가져오려는 아이템 정보가 이상합니다.",
	L"인벤토리에 공간이 부족합니다.",
	L"구매한 캐쉬 아이템을 가져오지 못했습니다.",
	L"잔액 정보를 조회하지 못했습니다.",
	L"회원 정보를 찾을 수 없습니다.",
	L"차단된 계정입니다.",
	L"다른 캐릭터 아이템은 인벤토리로 가져올 수 없습니다.",
	L"동일한 기간제 아이템을 이미 가지고 있습니다.",
	L"아이템을 가져오는 중입니다. 잠시만 기다려 주세요.",
	L"잘못된 프레임 평균 값입니다.",
	L"해당 캐릭터가 존재하지 않습니다.",
	L"닉네임 복구 대상이 아닙니다.",
	L"동일한 닉네임이 이미 존재합니다.",
	L"이 닉네임은 일정 기간 사용할 수 없습니다.",
	L"닉네임 복구 작업이 실패하였습니다.",
	L"유효하지 않은 닉네임입니다.",
	L"닉네임 길이가 올바르지 않습니다.",
	L"닉네임을 변경하지 못했습니다.",
	L"개인 상점을 생성하지 못했습니다.",
	L"광장에서만 상점을 생성할 수 있습니다.",
	L"광장 유저 정보를 찾을 수 없어서 상점을 시작 할 수 없습니다.",
	L"개인 상점 참여 인원이 많으므로 잠시 후에 이용해 주세요.",
	L"개인 상점에 참여하지 못했습니다.",
	L"판매 등록 하려는 아이템의 수량 정보가 잘못되었습니다.",
	L"아이템이 이미 판매 등록된 상태입니다.",
	L"개설자가 아닌데 판매 물품을 등록하려고 하였습니다.",
	L"판매할 수 있는 물품 개수보다 등록된 아이템 개수가 많습니다.",
	L"상점 개설 비용이 부족하므로 상점을 개설할 수 없습니다.",
	L"상점 개설 준비가 아직 안되었습니다.",
	L"상점에서 퇴장하지 못했습니다.",
	L"구입하려는 아이템 수량이 판매 물품보다 많습니다.",
	L"구입하려는 아이템이 없습니다.",
	L"ED가 부족하여 물품을 구매할 수 없습니다.",
	L"인벤토리에 공간이 없어서 구매할 수 없습니다.",
	L"개설자를 찾지 못하여 거래하지 못했습니다.",
	L"거래 중에는 상점을 열 수 없습니다.",
	L"이미 개인 상점이 열려있습니다.",
	L"최대 등록할 수 있는 ED량을 초과하였습니다.",
	L"최대 구매할 수 있는 ED량을 초과하였습니다.",
	L"구매하려는 아이템 정보가 이상합니다.",
	L"선택한 아이템은 인벤토리에 없는 아이템입니다.",
	L"분해할 수 있는 아이템이 아닙니다.",
	L"분해 실패하였습니다. 다시 분해를 해주세요.",
	L"장비와 악세사리만 분해가 가능합니다.",
	L"분해를 할 수 없습니다.",
	L"아이템 생성이 실패하였습니다.",
	L"인벤토리 공간이 부족하여 분해를 할 수 없습니다.",
	L"기간제 아이템은 분해할 수 없습니다.",
	L"강화할 아이템이 인벤토리에 없습니다.",
	L"아이템 정보가 존재하지 않습니다.",
	L"무기나 방어구만 강화할 수 있습니다.",
	L"강화할 수 있는 아이템이 아닙니다.",
	L"더 이상 강화할 수 없습니다.",
	L"강화석이 존재하지 않습니다.",
	L"강화를 진행할 수 없습니다.",
	L"강화에 필요한 ED가 부족합니다.",
	L"장착중인 아이템은 강화할 수 없습니다.",
	L"플루오르 스톤이 인벤토리에 없습니다.",
	L"플루오르 스톤을 사용할 수 없습니다.",
	L"강화 레벨 10부터는 강화시 플루오르 스톤을 사용할 수 없습니다.",
	L"사용불가 아이템은 강화할 수 없습니다.",
	L"강화 업데이트가 실패하였습니다.",
	L"복구할 아이템이 인벤토리에 없습니다.",
	L"아이템 정보가 존재하지 않습니다.",
	L"복구할 수 있는 아이템이 아닙니다.",
	L"사용불가 아이템만 복구가 가능합니다.",
	L"복구석이 부족하여 복구할 수 없습니다.",
	L"장착중인 아이템은 복구할 수 없습니다.",
	L"사용 불가 아이템에는 이용할 수 없는 기능입니다.",
	L"강화 성공입니다.",
	L"변화 없음입니다.",
	L"1단계 하락입니다.",
	L"강화 단계 초기화입니다.",
	L"아이템 사용 불가입니다.",
	L"인벤토리에 없는 아이템입니다.",
	L"아이템 정보가 존재하지 않습니다.",
	L"수수료가 부족합니다.",
	L"무기나 방어구만 소켓이 존재합니다.",
	L"해당 장비에 대한 마법석 정보가 존재하지 않습니다.",
	L"인벤토리에 마법석이 없습니다.",
	L"소켓을 사용할 수 없습니다.",
	L"소켓에 이미 마법석이 꽂혀있습니다.",
	L"소켓에 이미 마법석이 제거되어 있습니다.",
	L"존재하지 않는 소켓 옵션입니다.",
	L"존재하지 않는 닉네임 입니다.",
	L"자기 자신은 추천할 수 없습니다.",
	L"보상을 받을 대상이 존재하지 않습니다.",
	L"지우려는 사제 데이터가 DB에 없습니다.",
	L"제자 리스트를 받아오지 못했습니다.",
	L"레벨이 낮아 정보 제공을 할 수 없습니다.",
	L"레벨이 낮아 제자를 받을 수 없습니다.",
	L"제자는 3명까지 등록 가능합니다.",
	L"이미 스승이 있습니다.",
	L"이미 제자로 등록되어 있습니다.",
	L"구입하려는 상품 정보가 이상합니다.",
	L"상품을 구매할 수 없습니다.",
	L"중복 결제입니다.",
	L"상품 총 구매 수량을 초과하였습니다.",
	L"상품 구매 개수를 초과하였습니다.",
	L"이 상품을 아직은 다시 구매할 수 없습니다.",
	L"카트가 없습니다.",
	L"사용 제한된 게임입니다.",
	L"총 판매 수량 제한으로 인하여 구매할 수 없습니다.",
	L"주문 수량이 제한되었습니다.",
	L"이 상품을 다시 구매할 수 있는 시기가 아직 안되었습니다.",
	L"선물할 수 없는 상품입니다.",
	L"주문 수량이 제한되어 있습니다.",
	L"받는 사람이 이 상품을 다시 구매할 수 있는 시기가 아직 안되었습니다.",
	L"블록된 유저입니다.",
	L"회원 정보를 찾을 수 없습니다.",
	L"잔액이 부족합니다.",
	L"점검중입니다.",
	L"소지할 수 있는 최대 부활석 개수를 초과하게 됩니다.",
	L"인벤토리 슬롯 개수를 더 이상 확장할 수 없습니다.",
	L"자기 자신에게는 선물할 수 없습니다.",
	L"잘못된 비밀 번호를 입력하였습니다.",
	L"존재하지 않는 닉네임입니다.",
	L"체험 ID 유저에게는 선물할 수 없습니다.",
	L"선물할 수 있는 한도액이 초과되었습니다.",
	L"비밀 번호를 입력해주세요.",
	L"넥슨 계정 인증 실패입니다.",
	L"선물할 수 없는 상품입니다.",
	L"만 14세 미만은 이 기능을 사용할 수 없습니다.",
	L"캐릭터 슬롯 확장 상품을 구매할 수 없습니다. 슬롯 확장은 9개까지 가능합니다.",
	L"캐릭터 슬롯을 확장하지 못했습니다.",
	L"이미 해당 기능을 사용 중입니다.",
	L"존재하지 않는 스킬입니다.",
	L"스킬 초기화 아이템이 존재하지 않습니다.",
	L"스킬 초기화 아이템을 DB에서 삭제하지 못했습니다.",
	L"스킬 초기화 DB쿼리 결과 실패가 반환되었습니다.",
	L"초기화 할 수 없는 스킬입니다.",
	L"스킬 슬롯에 장착 중인 스킬은 초기화 할 수 없습니다.",
	L"찜 리스트에 등록할 수 없는 상품입니다.",
	L"찜 리스트에 더 이상 상품을 등록할 수 없습니다.",
	L"체험 ID에서는 이용 하실 수 없는 기능입니다.",
	L"체험 ID 유저는 추천인에 등록 할 수 없습니다.",
	L"체험 ID를 이용하실 수 있는 레벨인 20렙이 되셨습니다. 계속 게임을 하시려면 넥슨ID로 전환해주세요.",
	L"체험 ID 유저는 메신저 친구로 등록할 수 없습니다.",
	L"해킹 리스트에 등록된 유저가 아닙니다.",
	L"해킹 유저 동의 데이터를 디비 업데이트 하지 못했습니다.",
	L"해킹 신고 유저인데 동의를 하지 않았습니다.",
	L"인벤토리에 존재하지 않는 아이템입니다.",
	L"속성 강화를 할 수 없습니다.",
	L"이미 속성 강화된 슬롯입니다.",
	L"이미 속성 강화가 제거된 슬롯입니다.",
	L"속성 강화를 위한 수수료가 부족합니다.",
	L"속성 강화를 위한 엘의 조각이 부족합니다.",
	L"아이템 정보가 이상합니다.",
	L"무기와 방어구만 속성강화를 할 수 있습니다.",
	L"선택된 아이템은 판별할 수 없는 아이템입니다.",
	L"판별기가 부족하여 아이템을 감정할 수 없습니다.",
	L"해당 아이템을 판별하기 위한 인벤토리 공간이 부족합니다.",
	L"해당 아이템을 판별할 수 없습니다.",
	L"아이템 생성 실패.",
	L"우편함 정보를 얻을 수 없습니다.",
	L"선택하신 아이템은 편지에 첨부할 수 없습니다.",
	L"입력하신 ED는 편지에 첨부할 수 없습니다.",
	L"편지를 전송하지 못했습니다.",
	L"존재하지 않는 편지입니다.",
	L"인벤토리에 여유공간이 없어서 첨부된 물품을 이동할 수 없습니다. 여유공간을 확보 하신 후 다시 시도 해 주시기 바랍니다.",
	L"첨부 물품을 가져오지 못했습니다.",
	L"존재하지 않는 아이템을 첨부하려고 하였습니다.",
	L"완전히 수리된 아이템만 첨부할 수 있습니다.",
	L"존재하는 닉네임이 아닙니다.",
	L"동일한 캐릭터 간에는 편지를 보낼 수 없습니다.",
	L"수수료 부족으로 편지를 발송할 수 없습니다.",
	L"상대방의 우편함에 여유 공간이 없어 편지를 보낼 수 없습니다.",
	L"이미 삭제된 편지를 선택하였습니다.",
	L"상대방이 편지 받기를 거부하였습니다.",
	L"귀속 아이템은 편지에 첨부할 수 없습니다.",
	L"체험 ID 유저에게는 편지를 보내실 수 없습니다.",
	L"존재하지 않는 닉네임입니다.",
	L"이미 블랙리스트에 등록된 닉네임입니다.",
	L"블랙리스트에 등록하지 못했습니다.",
	L"이미 삭제된 닉네임입니다.",
	L"블랙리스트에서 삭제하지 못했습니다.",
	L"보유하고 있는 칭호가 아닙니다.",
	L"이미 장착중인 칭호입니다.",
	L"사용 가능한 기간이 지난 칭호입니다.",
	L"존재하지 않는 칭호입니다.",
	L"칭호를 획득하지 못했습니다.",
	L"이미 보유 중인 칭호입니다.",
	L"아이템을 사용할 수 없습니다.",
	L"존재하지 않는 아이템입니다.",
	L"캐릭터 레벨이 부족하여 아이템을 사용할 수 없습니다.",
	L"인벤토리에서 사용 불가능한 아이템입니다.",
	L"인벤토리에 없는 아이템 입니다.",
	L"부여 아이템이 아닙니다.",
	L"부여가 가능한 아이템이 아닙니다.",
	L"강화 레벨 부여중 업데이트가 실패하였습니다.",
	L"아이템을 교환할 수 없습니다.",
	L"인벤토리에 없는 아이템 입니다.",
	L"이 아이템은 교환대상이 아닙니다.",
	L"교환 아이템 생성되지 않았습니다.",
	L"다른 필드로 이동하기 위한 이탈입니다.",
	L"방으로 입장하기 위한 이탈입니다.",
	L"시스템에서 유저를 이탈 처리하였습니다.",
	L"마을에 입장할 수 없습니다.",
	L"마을에서 나가지 못했습니다.",
	L"유저 리스트를 받아올 수 없습니다.",
	L"레벨이 낮아 마을에 입장할 수 없습니다.",
	L"마을에 입장하기 위한 던전을 클리어하지 않았습니다.",
	L"존재하지 않는 채널입니다.",
	L"채널 이동 성공으로 인한 접속 종료입니다.",
	L"채널이동을 위한 유저 등록이 실패하였습니다.",
	L"채널이동을 위한 유저 등록이 되어 있지 않습니다.",
	L"이동하려는 채널과 현재 채널이 같습니다.",
	L"수용 인원이 가득 차서 이동 할 수 없습니다.",
	L"마을 또는 필드에서만 채널 이동 기능을 이용하실 수 있습니다.",
	L"잠시 후에 채널 이동을 이용해주시기 바랍니다.",
	L"GM 캐릭터는 사용할 수 없는 기능입니다.",
	L"캐릭터가 선택되지 않았습니다.",
	L"친구가 접속 중이 아닙니다.",
	L"잘못된 상태입니다.",
	L"이벤트로 드롭된 아이템을 DB업데이트하지 못했습니다.",
	L"이미 시간 이벤트 보상을 받았습니다.",
	L"이미 친구로 등록되어있는 캐릭터입니다.",
	L"친구 캐릭터의 상태가 올바르지 않습니다.",
	L"나를 친구로 초대한 캐릭터가 아닙니다.",
	L"이 캐릭터는 친구가 아닙니다.",
	L"그룹이 존재하지 않습니다.",
	L"메세지의 길이가 너무 깁니다.",
	L"현재 그룹과 이동시키려고 하는 그룹이 같습니다.",
	L"그룹을 더 이상 생성할 수 없습니다.",
	L"동일한 그룹이 이미 있습니다.",
	L"동일한 그룹 이름입니다.",
	L"제자가 7일동안 접속을 하지 않아 사제관계가 종료 되었습니다.",
	L"제자가 레벨업을 완료하여 사제관계가 종료 되었습니다.",
	L"캐릭터가 삭제되어 사제관계가 종료 되었습니다.",
	L"유저에 의해 사제관계가 종료되었습니다.",
	L"사제관계 종료를 실패하였습니다.",
	L"친구 기능 관련 DB 작업 오류입니다.",
	L"그룹 이름이 너무 깁니다.",
	L"친구 리스트가 가득 차서 친구로 초대할 수 없습니다.",
	L"같은 계정의 캐릭터에게는 친구 초대를 할 수 없습니다.",
	L"상대방의 친구 리스트가 가득 차서 친구로 초대할 수 없습니다.",
	L"상대방이 친구 초대를 거부한 상태입니다.",
	L"쿼리 호출 실패입니다.",
	L"대회채널에서는 난입할 수 없습니다.",
	L"대회기간중에는 방에 인원이 가득차야만 게임이 시작 가능합니다.",
	L"제목의 길이가 너무 깁니다.",
	L"편지 내용의 길이가 너무 깁니다.",
	L"초대한 유저는 현재의 던전을 수행할 수 없습니다.",
	L"이미 봉인이 해제된 스킬입니다.",
	L"스킬 봉인 해제를 실패하였습니다.",
	L"이 상품을 구매하실 수 없는 캐릭터 직업입니다.",
	L"이미 봉인이 해제된 스킬의 상품입니다.",
	L"선택하신 상품과 같은 기능의 아이템이 인벤토리에 있어 상품을 구매하실 수 없습니다.",
	L"체험 ID 유저는 헤니르 시공 던전 파티에 초대할 수 없습니다.",
	L"인증 방식이 맞지 않습니다.",
	L"은행에 보관된 아이템은 분해할 수 없습니다.",
	L"은행에 보관된 아이템은 소켓을 추가 할 수 없습니다.",
	L"은행에서는 사용할 수 없는 기능입니다.",
	L"은행 슬롯 갯수를 더이상 확장할 수 없습니다.",
	L"은행 회원권은 1개이상 구매할 수 없습니다.",
	L"구매할 수 없는 은행 회원권 입니다.",
	L"인벤토리에 루리엘 신용보증서가 있으면 플래티넘 회원권 구매를 할 수 없습니다. 루리엘 신용보증서를 사용해주세요.",
	L"패스워드가 존재하지 않습니다.",
	L"패스워드 갱신을 할 수 없습니다.",
	L"아이디 또는 패스워드가 올바르지 않습니다.",
	L"인증 과정에서 시스템 오류가 발생하였습니다.",
	L"존재하지 않는 아이템입니다.",
	L"아이템 정보를 얻을 수 없습니다.",
	L"봉인할 수 없는 아이템 종류입니다.",
	L"장착중인 아이템은 봉인할 수 없습니다.",
	L"이미 봉인된 아이템 입니다.",
	L"수리되지 않은 아이템은 봉인할 수 없습니다.",
	L"기간제 아이템은 봉인할 수 없습니다.",
	L"파괴된 아이템은 봉인할 수 없습니다.",
	L"거래가능 아이템은 봉인할 수 없습니다.",
	L"봉인 가능한 횟수가 남지 않아 봉인할 수 없습니다.",
	L"선택한 봉인주문서 정보가 이상합니다.",
	L"봉인을 실패하였습니다.",
	L"봉인 주문서가 부족하여 봉인할 수 없습니다.",
	L"장착중인 아이템은 봉인해제를 할 수 없습니다.",
	L"이미 봉인해제된 아이템 입니다.",
	L"봉인해제를 실패하였습니다.",
	L"봉인정보 DB업데이트 실패입니다.",
	L"봉인된 아이템은 강화할 수 없습니다.",
	L"캐쉬 포인트 조회 중에 시스템 에러가 발생하였습니다.",
	L"이 계정은 활동이 제한된 계정입니다.",
	L"계정이 존재하지 않습니다.",
	L"계정이 올바르지 않습니다.",
	L"쿠폰 번호가 올바르지 않습니다.",
	L"이미 사용된 쿠폰입니다.",
	L"이 종류의 쿠폰은 한 번만 사용할 수 있습니다.",
	L"사용 기한이 지난 쿠폰입니다.",
	L"쿠폰 사용 중에 시스템 오류가 발생하였습니다.",
	L"포인트가 부족합니다.",
	L"아이템 구입 중에 시스템 오류가 발생하였습니다.",
	L"15 레벨 이상 부터 구매가 가능합니다.",
	L"이미 가입한 길드가 존재합니다.",
	L"이미 존재 하는 길드 이름입니다.",
	L"길드 창단 아이템이 인벤토리에 없습니다.",
	L"길드 창단 아이템 정보가 이상합니다.",
	L"길드 창단에 실패하였습니다.",
	L"길드 마스터 등록에 실패하였습니다.",
	L"길드 정보를 찾을 수 없습니다.",
	L"길드 초대를 할 수 없습니다.",
	L"현재 오프라인인 유저 입니다.",
	L"체험ID 유저는 길드에 초대할 수 없습니다.",
	L"이미 길드 초대한 유저입니다.",
	L"다른 길드에서 이미 초대한 유저입니다.",
	L"길드 초대를 할 수 있는 권한이 없습니다.",
	L"길드 인원을 더 이상 늘릴 수 없습니다. 길드 인원 확장을 위해서는 '길드 인원 확장 아이템'이 필요합니다.",
	L"초대하려는 유저는 이미 길드멤버입니다.",
	L"이미 길드에 가입되어있는 유저입니다.",
	L"길드 유저 리스트를 얻을 수 없습니다.",
	L"쿠폰으로 획득하려는 아이템 정보가 올바르지 않습니다.",
	L"유저가 길드 초대를 거부한 상태입니다.",
	L"길드 가입 실패.",
	L"길드에 소속되어있지 않습니다.",
	L"길드원 등급변경 권한이 없습니다.",
	L"길드원 등급변경에 실패하였습니다.",
	L"길드마스터와 길드관리자만 길드메시지의 변경이 가능합니다.",
	L"길드 메시지 변경에 실패하였습니다.",
	L"길드원 강퇴 권한이 없습니다.",
	L"길드 마스터는 길드에서 탈퇴 할 수 없습니다.",
	L"길드 탈퇴를 실패하였습니다.",
	L"길드원 인사말 글자 길이 제한.",
	L"해당 길드원 인사말 변경에 대한 권한이 없습니다.",
	L"길드원 인사말 변경을 실패하였습니다.",
	L"길드마스터만 길드 해체 기능을 사용할 수 있습니다.",
	L"길드원이 남아 있으므로 길드 해체가 불가능합니다.",
	L"길드 해체를 실패하였습니다.",
	L"이미 변경된 등급입니다.",
	L"더 이상 길드 관리자를 임명 할 수 없습니다.\n새로운 길드 관리자를 임명 하려면\n현재 길드 관리자 등급을 조정해야 합니다.",
	L"길드마스터는 캐릭터 삭제를 할 수 없습니다. 길드마스터 등급을 다른 길드원에게 위임하세요.",
	L"자기 자신의 등급은 변경할 수 없습니다.",
	L"이미 길드에 소속 되어 있어 길드 창단 허가서를 구매할 수 없습니다.",
	L"인벤토리에 길드 창단 허가서가 존재합니다.",
	L"길드 인원 확장을 실패하였습니다.",
	L"길드 마스터와 길드 관리자만 길드 인원 확장권을 사용할 수 있습니다.",
	L"길드 인원을 더 이상 늘릴 수 없습니다.",
	L"거래중인 상태에서는 해당 기능을 이용할 수 없습니다.",
	L"이동 할수없는 지역입니다.",
	L"이 길드 이름은 7일간 사용할 수 없습니다.",
	L"획득 가능한 최대 길드 경험치 입니다.",
	L"이 아이템을 구입하기 위해서는 관련 퀘스트를 수행 해야 합니다.",
	L"길드 이름의 길이가 너무 깁니다.",
	L"속성 제거를 위한 수수료가 부족합니다.",
	L"35 레벨 이상 부터 구매가 가능합니다.",
	L"게임을 종료하고 빈펀 계정으로 업그레이드 하시기 바랍니다. 빈펀 사이트는 tw.beanfun.gamania.com 입니다.",
	L"이 계정은 이미 빈펀 계정으로 업그레이드 된 계정입니다. 빈펀 프로그램으로 접속해주세요.",
	L"더 이상 아이템을 구입할 수 없습니다. 임시 보관함에 있는 아이템을 인벤토리로 가져오세요.",
	L"길드 마스터 등급만 GSP를 투자할 수 있습니다.",
	L"이미 최고 레벨까지 배운 길드스킬입니다.",
	L"해당 길드 스킬 정보가 존재하지 않습니다.",
	L"선행 길드스킬을 아직 최고레벨까지 배우지 못했습니다.",
	L"충분한 GSP를 투자하지 않았습니다.",
	L"GSP가 부족합니다.",
	L"길드 스킬 배우기가 실패하였습니다.",
	L"길드 마스터 등급만 길드 스킬을 되돌릴 수 있습니다.",
	L"망각의 피리 아이템이 인벤토리에 없습니다.",
	L"길드 스킬을 되돌릴 수 없습니다.",
	L"길드 스킬 되돌리기가 실패하였습니다.",
	L"길드 마스터 등급만 길드 스킬 초기화를 할 수 있습니다.",
	L"망각의 뿔피리 아이템이 인벤토리에 없습니다.",
	L"길드 스킬 초기화를 할 수 없습니다.",
	L"길드 스킬 초기화를 실패하였습니다.",
	L"길드 마스터 등급만 사용 가능 합니다.",
	L"기간제 길드 스킬 포인트 획득 실패하였습니다.",
	L"이미 그노시스의 대축복 아이템을 사용 중입니다.",
	L"아직 이벤트 보상을 받을 수 없습니다.",
	L"인벤토리 공간이 부족하여 이벤트 보상을 받을 수 없습니다.",
	L"이벤트 보상 받기가 실패하였습니다.",
	L"아이템 모드가 아닙니다.",
	L"드롭된 아이템이 아직 남아있습니다.",
	L"대전 드롭아이템 얻어오기 실패.",
	L"케릭터 카드 사용 후 생성 가능한 클래스입니다.",
	L"본 계정은 이벤트에 참여할 수 없습니다.",
	L"길드 마스터 또는 길드 관리자만 이용할 수 있는 기능입니다.",
	L"PC방 전용 아이템은 이용할 수 없는 기능입니다.",
	L"길드 광고가 존재하지 않습니다.",
	L"글자 길이가 너무 깁니다.",
	L"잘못된 광고 등록 기간입니다.",
	L"광고 등록 비용이 부족하므로 광고를 등록 할 수 없습니다.",
	L"이미 등록한 광고가 있습니다.",
	L"길드 광고 등록을 실패하였습니다.",
	L"가입 신청자가 너무 많아서 가입 신청을 할 수 없습니다.",
	L"길드 가입 신청을 실패하였습니다.",
	L"이미 가입 신청이 되어 있습니다.",
	L"가입 신청 정보가 존재하지 않습니다.",
	L"길드 가입 수락을 실패하였습니다.",
	L"길드 가입 신청 삭제를 실패하였습니다.",
	L"기간이 다된 광고에는 가입 신청을 할 수 없습니다.",
	L"길드 가입 신청 리스트를 볼수 있는 권한이 없습니다.",
	L"자물쇠를 열지 않았습니다.",
	L"이 게임 계정은 정지 당했습니다.",
	L"PC방 전용 퀘스트는 PC방에서만 수락할 수 있습니다.",
	L"PC방 전용 퀘스트는 PC방에서만 완료할 수 있습니다.",
	L"해킹 감지로 인한 방 이탈 입니다.",
	L"길드 초대 거부 옵션이 켜져있어서 가입 신청을 할 수 없습니다.",
	L"길드 가입 신청자가 많아 더 이상 가입 신청을 할 수 없습니다.",
	L"현재 진행중인 이벤트 퀘스트가 아닙니다.",
	L"해당 아이템의 구매 한도가 초과되었습니다.",
	L"넥슨캐시 사용이 불가능하도록 설정돼 있습니다. 넥슨포털 내정보의 넥슨캐시 보안설정 메뉴에서 설정을 변경해주세요.",
	L"이미 오늘 출석 체크는 완료된 상태 입니다.",
	L"웹 포인트 증가 DB쿼리 호출 실패.",
	L"출석 체크 실패.",
	L"중복 파티 가입으로 인한 파티 이탈 처리.",
	L"파티 가입 대기중인 상태입니다.",
	L"아이템 리스트 삭제.",
	L"아이템 리스트 추가.",
	L"아이템 리스트 수정.",
	L"카테고리 선택이 잘못 되었습니다.",
	L"검색하려는 아이템 정보가 없습니다.",
	L"초대 대상자의 근성도가 부족합니다.",
	L"잠시후에 검색을 시도해 주세요.",
	L"판매종료된 상점입니다.",
	L"구매 불가능한 상태의 사용자 입니다.",
	L"방 생성에 실패했습니다.",
	L"존재하지 않는 아이템입니다.",
	L"존재 하지 않는 페이지입니다.",
	L"이미 등록되어 있는 메모입니다.",
	L"메모 등록을 실패 하였습니다.",
	L"기술의 노트를 보유하고 있지 않습니다.",
	L"기술의 노트 페이지 확장 실패.",
	L"이미 기술의 노트를 보유하고 있습니다.",
	L"20레벨이 되어야 구입할 수 있습니다.",
	L"다른 서버의 유저와는 개인거래를 할 수 없습니다.",
	L"다른 서버의 유저의 개인상점은 이용 할 수 없습니다.",
	L"다른 서버의 유저는 길드로 초대 할 수 없습니다.",
	L"다른 서버의 유저는 제자로 등록 할 수 없습니다.",
	L"다른 서버의 유저는 친구로 등록 할 수 없습니다.",
	L"헤니르의 시공 도전 모드는 다른 서버 유저와 함께 플레이 할 수 없습니다.",
	L"해킹 의심 유저 접속 제한 조치.",
	L"[홍콩] 해당 직업으로는 전직을 할 수 없습니다.",
	L"버리려는 아이템 갯수 입력이 잘 못 되었습니다.",
	L"펫 생성을 실패 하였습니다.",
	L"이미 사용중인 펫 이름 입니다.",
	L"펫 이름은 최대 한글 6자(영문 12자)까지 가능합니다.",
	L"이미 펫 생성이 진행 중 입니다.",
	L"펫 소환을 실패 하였습니다.",
	L"여기서는 펫을 소환 할 수 없습니다.",
	L"소환된 펫이 없습니다.",
	L"이미 소환 되어 있는 펫 입니다.",
	L"펫 소환 해제를 실패 하였습니다.",
	L"펫 명령을 실패 하였습니다.",
	L"포만도가 ‘가득참’ 상태입니다. 더 이상 먹이를 먹을 수 없습니다.",
	L"펫 먹이 주기가 실패하였습니다.",
	L"펫이 '특별한'상태 입니다. 엘의 나무 열매/씨앗 만 먹습니다.",
	L"이미 저장 되어 있는 내용입니다.",
	L"더 이상 진화 할 수 없습니다.",
	L"친밀도가 100%가 되었을 때만 진화가 가능합니다.",
	L"장비아이템 또는 전용먹이만 섭취합니다.",
	L"장비의 레벨이 너무 낮습니다.",
	L"패스워드가 틀렸습니다.",
	L"계정이 존재하지 않습니다.",
	L"계정 승인이 되지 않았습니다. (User not validated)",
	L"펫에게 줄 수 있는 먹이 아이템이 아닙니다.",
	L"해당 유저가 다른 용무 중 입니다.",
	L"[CN] 매트릭스 카드가 바인딩되어 있습니다.",
	L"[CN] OPT번호가 틀렸습니다. 다시 입력해 주십시요.",
	L"[CN] 보안카드 번호를 잘못 입력 하셨습니다. 다시 입력해 주십시요.",
	L"[CN] 계정 정보가 수정 과정에 있습니다. 의문 사항이 있으시면 서비스 센터로 연락 바랍니다.",
	L"[CN] 자이언트 패스워드 보안이 바인딩 해제 상태에 있습니다. 의문사항이 있으시면 서비스 센터로 연락 바랍니다.",
	L"[TW]ServiceCode 가 잘못되었습니다.",
	L"GM리엘",
	L"[발신자 정보가 없습니다]",
	L"지금은 청약철회 기능을 이용 하실 수 없습니다.",
	L"청약철회 가능 개수보다 요청 갯수가 커서 청약철회를 실패하였습니다.",
	L"구매한지 7일이 초과된 상품으로 청약철회가 불가능 합니다.",
	L"패키지 중 일부 상품을 수령하여 청약철회가 불가능 합니다.",
	L"상품 가격이 0원으로 청약철회가 불가능한 상품 입니다.",
	L"중복된 RequestID입니다.",
	L"존재하지 않는 상품 구매 정보입니다.",
	L"존재하지 않는 상품 정보 입니다.",
	L"청약철회가 불가능한 상품 입니다.",
	L"선물 받은 상품이어서 청약철회가 불가능 합니다.",
	L"기간제 아이템은 버릴 수 없습니다.",
	L"되팔기 불능 아이템은 버릴 수 없습니다.",
	L"1 EL coins 이벤트 구매 제한을 초과 하여 더 이상 구매 할 수 없습니다. ",
	L"레벨이 부족하여 아이템을 사용 할 수 없습니다.",
	L"강화 레벨 11부터는 강화시 플루오르 스톤을 사용할 수 없습니다.",
	L"이미 보상을 받아서 퀘스트 수락을 할 수 없습니다.",
	L"수락 보상을 받을 인벤토리 공간이 부족하여 퀘스트를 수락할 수 없습니다.",
	L"마법석 사용을 실패하였습니다.",
	L"해당 기능은 현재 사용할 수 없습니다.\n 불편을 드려 죄송합니다.",
	L"접속이 제한된 IP입니다.",
	L"채팅 기능이 제한 되었습니다.",
	L"거래 기능이 제한 되었습니다.",
	L"' 아이템이 없어 게임을 시작 할 수 없습니다.",
	L"다른 응용프로그램과의 충돌이 있습니다.",
	L"[CN] 해당 유저가 존재하지 않습니다.",
	L"[CN] 잔액이 부족합니다.",
	L"[CN] 존재하지 않는 아이템 카드입니다.",
	L"[CN] 동일한 쿠폰은 중복사용 할 수 없습니다.",
	L"[CN] 이미 사용한 아이템 카드입니다.",
	L"[CN] 잘못된 전용 카드 ID입니다.",
	L"[CN] 잘못된 전용 카드 패스워드입니다.",
	L"[CN] DB 에러",
	L"거래 제한 상태라서 편지 보내기 기능을 사용 하실 수 없습니다.",
	L"거래 제한 상태라서 개인 상점 개설을 할 수 없습니다.",
	L"거래 제한 상태라서 개인 상점을 볼 수 없습니다.",
	L"거래 제한 상태라서 물품 검색을 할 수 없습니다.",
	L"거래 제한 상태라서 거래 광장에 입장 할 수 없습니다.",
	L"캐시 인벤토리에 아이템이 존재합니다.(HK 이벤트에 사용)",
	L"파티인원이 2인 이상이어야 합니다.",
	L"던전이 닫혀 입장 할 수 없습니다.",
	L"다른 서버 유저와 월드 미션 던전을 플레이 할 수 없습니다.",
	L"우편 감시 로그에 중복된 값이 있습니다.",
	L"거래 감시 로그에 중복된 값이 있습니다.",
	L"우편 감시 로그 추가 실패!",
	L"거래 감시로그 추가 실패!",
	L"아직 판매중인 물품이 있어 대리상인을 종료할 수 없습니다!",
	L"대리상인이 개설되어 있지 않습니다.",
	L"판매 중지를 할 수 없습니다.",
	L"판매 중지된 개인상점입니다. 물건을 구매할 수 없습니다.",
	L"대리 판매인이 물품을 판매 하고 있어 상점을 개설 할 수 없습니다.",
	L"(TW)비정상적인 방법으로 게임을 플레이 하고 있습니다. 계속 하시면 거래 정지 됩니다.",
	L"대리상인을 종료할 수 없습니다.",
	L"Unregisted User.",
	L"It is already canceled.",
	L"Invalid coupon number.",
	L"This coupon can not be canceled.",
	L"The used user information and cancel the requested user information does not match.",
	L"The issued cash by coupons  already had used . so can not be canceled.",
	L"requested price should be equal to billing items price.",
	L"The balance short.",
	L"Billing account that is unavailable.",
	L"You can not register any more today.",
	L"This coupon can not verify the information.",
	L"Coupon is already used.",
	L"Coupon use is stopped.",
	L"Coupon is expired.",
	L"Coupon is not available in the game.",
	L"Issued to certain customers who can use coupons, you do not have permission.",
	L"Buying is a restricted user.",
	L"Your use is limited to the payment service.",
	L"Your use is limited to the purchase service.",
	L"User cannot be verified.",
	L"Registration has exceeded the number of coupons available.",
	L"Unable to login due to IP restriction.",
	L"The coupon code has already been redeemed.",
	L"You cannot purchase the item at the moment. Your account have reached the daily limit for purchasing the item.",
	L"2회이상 비밀번호를 틀려 로그인할 수 없습니다.",
	L"대리 판매인에게 물품을 맡기려면 대리판매 보증서가 있어야 합니다.",
	L"입력하신 보안패드 암호가 서로 같지 않습니다.",
	L"보안패드 암호가 10회 실패하였습니다.\n[확인]버튼을 누르면 접속이 종료됩니다.\n재활성화 및 해제는 웹사이트에서 개인 인증 후 가능합니다.",
	L"보안패드 암호 생성에 실패하였습니다.",
	L"보안패드 암호가 일치하지 않습니다.",
	L"상점 이름이 너무 깁니다.",
	L"대리상점 판매가 종료된 상태입니다.\n물품받기 후 새롭게 상점 개설이 가능합니다.",
	L"대리 판매인이 물품을 판매 중입니다.\n물품이나 판매수익을 받기 위해서는\n대리 판매인 상점을 종료 해야 합니다.",
	L"대리 상인 보증서 DB쿼리 실패",
	L"대리 상인을 생성을 실패하였습니다.",
	L"대리 상인 물품 등록을 실패 하였습니다.",
	L"대리 상점 개설자만 이용할 수 있습니다.",
	L"대리 상점 물품 받기가 실패하였습니다.",
	L"현재 구매가 이루어지고 있는 물품이어서 물품 받기가 실패하였습니다.",
	L"이미 물품 받기가 진행중인 상태입니다.",
	L"보유 가능한 ED량을 초과하여 물품받기가 실패하였습니다.",
	L"인벤토리 공간이 부족하여 물품을 받을 수 없습니다.",
	L"보관 중인 물품이 없습니다.",
	L"동일한 물품에 대해서는 동시에 가져올 수 없습니다.",
	L"레벨 제한 조건을 만족하지 않습니다.\n파티 요청을 할 수 없습니다.",
	L"아직 대리상인 기간이 남아서 상품을 구매할 수 없습니다.",
	L"찾아가지 않은 물품이 있어 개인상점을 개설 할 수 없습니다.\n물품 받기를 확인 하세요!",
	L"대리 판매인의 판매 물품이 모두 판매되어 상점을 종료합니다.",
	L"이미 수락하여 보상을 받은 퀘스트 입니다. 퀘스트 수락을 할 수 없습니다.",
	L"인벤토리 슬롯이 부족하여 퀘스트 수락을 할 수 없습니다.",
	L"파티 최소 레벨 입장 제한을 변경하지 못했습니다.",
	L"대리상점의 모든 물품이 판매될 경우 소지 한도 금액을 초과하게 되기 때문에 상점을 개설할 수 없습니다.",
	L"(TW/HK)강화 보조 아이템 사용 불가",
	L"(TW/HK)강화 보조 아이템이 없습니다.",
	L"(TW/HK)파괴 방지 아이템 사용 불가",
	L"(TW/HK)파괴 방지 아이템이 없습니다.",
	L"레벨 제한 조건을 만족하지 않습니다.\n다른 던전을 선택하시거나 레벨제한을 해제하여야 합니다.",
	L"레벨 제한 조건을 만족하지 않습니다.\n레벨 제한을 활성화 할 수 없습니다.",
	L"레벨 제한 조건을 만족하지 않습니다.\n파티 초대를 할 수 없습니다.",
	L"레벨 조건을 만족하지 않습니다.\n다른 던전을 선택하시거나 레벨제한을 해제하여야 합니다.",
	L"반복되는 숫자는 암호로 사용하실 수 없습니다.\n(0000, 1111, 999999 등)",
	L"연속되는 숫자는 암호로 사용하실 수 없습니다.\n(1234, 9876, 123456 등)",
	L"2차 보안패드 삭제가 실패하였습니다.",
	L"2차 보안패드를 사용중이 아닙니다.",
	L"이전 비밀번호와 동일한 비밀번호로\n변경 할 수 없습니다.",
	L"상점에서 팔고있지 않는 아이템을 구매하려고 하였습니다.",
	L"대전 게임 시작으로 인한 매치 종료.",
	L"대전 매칭 대상자 중 누군가가 참여하지 않았습니다.",
	L"대전 매칭이 취소되었습니다.",
	L"파티원이 대전에 참여하지 않아서 대전 매칭이 취소되었습니다.",
	L"매치가 성공하여 대기자 리스트에서 빠짐.",
	L"유저가 매치 신청 취소를 하여 대기자 리스트에서 빠짐.",
	L"파티원의 파티 탈퇴로 인해 매치 신청이 취소되었습니다.",
	L"이미 매치 중인 상태입니다.",
	L"대전 인원 정보가 이상합니다.",
	L"대전 타입 정보가 이상합니다.",
	L"공식 대전은 10레벨부터 가능합니다.",
	L"대전 파티는 10레벨부터 이용 가능 합니다.",
	L"이미 변경된 파티 타입 입니다.",
	L"던전파티 인원이 4명일때는 대전파티로 변경이 불가능 합니다.",
	L"파티 타입 변경 실패.",
	L"파티원중에 10레벨이 안되는 멤버가 있어 대전파티로 변경이 불가능합니다.",
	L"파티원중에 10레벨이 안되는 멤버가 있어 공식대전을 이용할 수 없습니다.",
	L"10레벨 이상의 캐릭터만 대전파티에 초대 할 수 있습니다.",
	L"10레벨 이상의 캐릭터만 대전 파티의 파티장으로 임명될 수 있습니다.",
	L"[CN]전환이 정상적으로 이루어지지 않았습니다. 다시 시도하여 주시기 바랍니다.",
	L"[EU]연속으로 정렬할 수 없습니다.\n잠시 후 다시 시도해주세요.",
	L"대전 매칭을 신청한 상태가 아닙니다.",
	L"이미 대전방에 입장한 상태입니다.",
	L"대전방이 아닙니다.",
	L"대전 매칭 요청이 실패하였습니다.",
	L"대전 매칭 신청한 유저는 파티에 초대 할 수 없습니다.",
	L"대전 매칭중인 유저는 파티에 초대 할 수 없습니다.",
	L"대전 랭크가 낮아 해당 아이템을 구매할 수 없습니다.",
	L"대전 매칭 신청을 한 상태에서는 다른 파티에 가입 신청을 할 수 없습니다.",
	L"대전 매칭중인 상태에서는 다른 파티에 가입 신청을 할 수 없습니다.",
	L"대전 매칭 상태에서는 채널이동 기능을 사용하실 수 없습니다.",
	L"머신 ID 인증 실패.",
	L"머신ID로 인한 이중접속 감지.",
	L"오늘 수락 할 수 있는 랜덤 퀘스트가 아닙니다.",
	L"약관에 동의하지 않았습니다.",
	L"SP 출력 값에 문제가 있습니다.",
	L"SP 입력 값에 문제가 있습니다. ",
	L"목적 DB 이름이 잘못 되었습니다.",
	L"데이터 검증 실패",
	L"죄송합니다. 캐쉬샵 점검으로 인해 이용하실 수 없습니다.",
	L"00시부터 06시까지 게임 접속이 제한됩니다.",
	L"정보를 가져오지 못했습니다.",
	L"캐릭터 생성한지 5일이 지난 후부터 거래가 가능합니다",
	L"캐릭터 생성한지 5일이 지난 후부터 우편이 가능합니다.",
	L"[CN]사용실패!",
	L"배틀필드 입장을 위한 방 이탈 입니다.",
	L"Lv.15가 되어야 구입하실 수 있습니다.",
	L"ServerSN과 MachineID가 매칭되지 않는다.",
	L"MachineID와 ServerSN이 매칭되지 않는다.",
	L"DB에 존재 하지 않는 ServerSN이 있다.",
	L"DB에 존재 하지 않는 MachineID가 있다.",
	L"클라가 준 SN은 없는데 DB에는 MachineID에 해당하는 SN 존재.",
	L"제재된 SN입니다.",
	L"접속 제한된 ServerSN 입니다.",
	L"이 계정은 테스트서버의 계정이 아닙니다.",
	L"파티 병합을 위한 자동 파티 이탈입니다.",
	L"이미 스킬 슬롯 확장을 사용 중입니다. 스킬 슬롯 확장(기간제) 상품을 가져올 수 없습니다.",
	L"이미 스킬 슬롯 확장을 기간 무제한으로 사용 중입니다. 상품을 가져올 수 없습니다.",
	L"[CN] 귀하의 계정이 정지되었습니다. 고객 센터에 문의하여 주십시요.",
	L"[CN] 비밀번호를 잘못 입력 하셨습니다. 다시 입력해 주십시요.",
	L"[CN] 해당 계정은 존재하지 않습니다.",
	L"[CN] 불명확인 시스템 에러",
	L"이미 동일한 아이템을 사용 중 입니다.",
	L"레벨이 부족하여 구매할 수 없습니다.",
	L"레벨이 부족하여 상품을 가져올 수 없습니다.",
	L"파티 생성 실패.",
	L"파티 인원 수 부족.",
	L"파티 초대에 대한 초대 대상자의 응답 지연.",
	L"[JP] 판매하지 않는 상품입니다.",
	L"[JP] 한게임 빌링 오류로 구매할수 없습니다.",
	L"공유은행 아이템은 우편으로 보낼 수 없습니다.",
	L"공유은행 아이템은 거래 할 수 없습니다.",
	L"[US] 레벨 20이 되어야 구입하실 수 있습니다.",
	L"[EU] 길드에 소속된 캐릭터만 교환 할 수 있습니다.",
	L"[US] 레벨 30이 되어야 구입하실 수 있습니다.",
	L"[US] PVP파티에는 풀패치일때만 참가요청을 할 수 있습니다.",
	L"블랙리스트에 등록된 DLL 이 존재합니다.",
	L"[CN] 잠시 후 다시 시도해 주시거나 캐릭터를 재접속 해주시기 바랍니다.",
	L"[CN] 오늘 퀴즈 이용 횟수를 초과 하셨습니다.",
	L"[CN] 알 수 없는 에러 입니다. (퀴즈)",
	L"[JP] 거래제한기간중에는구매할수 없습니다.",
	L"[JP] 거래제한기간중에는 사용할 수 없습니다.",
	L"더 이상 퀵 슬롯을 확장 할 수 없습니다.",
	L"[CN] 이미 삭제되었거나 삭제 대기 상태의 캐릭입니다.",
	L"[CN] 캐릭터 삭제 가능한 기간이 아닙니다.",
	L"[CN] 캐릭터 최종 삭제 가능 기간이 아닙니다.",
	L"[CN] 최종 삭제 가능한 캐릭터가 아닙니다.",
	L"[CN] 삭제 복구 대상 캐릭터가 아닙니다.",
	L"[CN] 캐릭터 복구 가능 기간이 아닙니다.",
	L"잠시후에 정렬을 시도해주세요.",
	L"해당 캐릭터를 찾을 수 없습니다.",
	L"캐릭터 삭제를 실패 하였습니다.",
	L"펫 이름 변경에 실패 하였습니다.",
	L"이미 펫 이름 변경이 진행 중입니다.",
	L"게임 이용이 차단 됩니다.",
	L"길드마스터가 아닙니다.",
	L"아이템 정보가 이상합니다.",
	L"길드 이름 변경 실패!",
	L"이미 진행 중인 작업입니다.",
	L"존재하지 않는 아이템을 삭제 시도하였습니다.",
	L"[TW] 낚시대를 장착하지 않았거나, 미끼가 없습니다.",
	L"[TW] 이미 사용중인 낚시터 입니다.",
	L"[TW] 낚시터에 문제가 있습니다. 다른 곳을 이용해 주세요.",
	L"[TW] 낚시 보상에 문제가 발생했습니다.",
	L"아이템 삭제를 실패 하였습니다.",
	L"길드 이름 변경권은 길드 마스터만 사용 가능합니다.",
	L"펫이 있을 때만 사용이 가능합니다. 펫이 없을 경우 일주일 이내에 청약 철회를 하셔야 합니다.",
	L"처리 시간이 지연되어 작업이 무효화 되었습니다. 재시도 해주시기 바랍니다.",
	L"유저가 길드 이름 변경을 취소하였습니다.",
	L"해당 지역으로 이동할 수 있는 상태가 아닙니다.",
	L"이미 해당 지역에 입장한 상태입니다.",
	L"이미 해당 지역은 가득차 있어 입장할 수 없습니다.",
	L"해당 지역에는 이미 다른 파티들로 인해 입장할 공간이 없습니다.",
	L"해당 지역에 입장을 실패하였습니다.",
	L"입장하려는 지역 정보가 잘못되었습니다.",
	L"해당 지역에 입장하려는 중 오류가 발생하였습니다.",
	L"해당 지역에서 유저 정보 찾기가 실패하였습니다.",
	L"자동 파티로 던전 게임 시작을 실패하였습니다.",
	L"던전 게임 시작을 실패하였습니다.",
	L"자동 파티 신청을 할 수 없는 상태입니다.",
	L"재집결을 위한 휴식 버프로 인해 자동 파티 신청을 할 수 없습니다. 버프가 풀릴때까지 기다려주세요.",
	L"이미 자동 파티 신청이 된 상태입니다.",
	L"잘못된 정보로 인해 자동 파티 신청이 실패하였습니다.",
	L"자동 파티 대기자로 등록이 실패하였습니다.",
	L"자동 파티 신청 상태가 아닙니다.",
	L"파티원 수가 1명뿐이어서 게임을 시작할 수 없습니다.",
	L"파티구성원 중 취소한 유저가 있습니다.",
	L"파티 게임 시작에 대한 응답시간이 지나 파티 게임을 시작하지 않습니다.",
	L"이미 파티에 소속된 상태입니다.",
	L"초대 대상자가 이미 다른 파티 가입을 기다리는 중입니다.",
	L"파티에 소속된 상태가 아닙니다.",
	L"영약 또는 비약은 중첩하여 사용할 수 없습니다.",
	L"ServerSN이 중복 접속 중이다.",
	L"ServerSN 존재하지 않는데 삭제하려한다.",
	L"현재 사용할 수 없는 기능입니다.",
	L"넥슨 통합 맵버쉽 계정 인증 상향 조정 필요.",
	L"육체의 한계 버프가 켜져 있습니다. 비약을 사용할 수 없습니다.",
	L"레벨 제한 조건을 만족하지 않습니다. 던전 게임을 시작할 수 없습니다.",
	L"[US] 추천받을 대상이 아닙니다.",
	L"[US] 추천인이 이미 3인을 추천하였습니다.",
	L"[US] 추천 가능한 유저가 아닙니다.",
	L"[US] 맞지 않는 아이템을 사용하였습니다.",
	L"[US] 캐릭터 슬롯을 확장할 수 없습니다. 슬롯 확장은 9개까지 가능합니다.",
	L"던전 게임 시작으로 인한 자동 파티 종료.",
	L"자동 파티 대상자 중 누군가가 게임 시작에 동의하지 않았습니다.",
	L"자동 파티 취소 요청에 의해 성공적으로 종료되었습니다.",
	L"파티원이 자동파티 게임시작에 동의하지 않아 자동파티가 취소되었습니다.",
	L"자동파티가 성공하여 대기자 리스트에서 빠짐.",
	L"유저가 자동파티 신청 취소를 하여 대기자 리스트에서 빠짐.",
	L"파티원의 파티 탈퇴로 인해 자동파티 신청이 취소되었습니다.",
	L"아이템 레벨 제한 조건을 만족하지 않습니다. 던전 게임을 시작할 수 없습니다.",
	L"키보드 맵핑 정보를 저장하지 못하였습니다.",
	L"모든 키보드 맵핑 정보를 저장하는데 실패하였습니다.",
	L"키보드 맵핑 정보를 읽어오지 못하였습니다.",
	L"모든 키보드 맵핑 정보를 읽어오는데 실패하였습니다.",
	L"파티 던전 게임 시작을 위한 응답이 지연되었습니다.",
	L"[US] 추천인 테이블에서 캐릭터 삭제를 실패하였습니다.",
	L"[US] 보안패드 리셋용 질문 및 답변을 입력하지 않았습니다.",
	L"같은 파티원이 아닙니다.",
	L"채팅 옵션 정보를 저장하는데 실패하였습니다.",
	L"채팅 옵션 정보를 읽어오는데 실패하였습니다.",
	L"일부 채팅 옵션 정보를 읽어오는데 실패하였습니다.",
	L"테스트 ",
	L"[US] 한달에 $1,000 까지만 선물이 가능합니다.",
	L"나이정보를 확인 중 오류가 발생하였습니다. 잠시 후 다시 로그인하여 주십시오.",
	L"실명인증을 받지 않은 회원이거나 넥슨닷컴 회원이 아닙니다.",
	L"셧다운제에 의하여 게임이용이 차단 됩니다.",
	L"선택적 셧다운제의 의해 게임이용이 차단됩니다.",
	L"통합 맴버십 ATL 2.0 유저가 아니므로 게임이용이 차단 됩니다.",
	L"오픈 가능 시간이 되지 않았습니다.",
	L"오픈 가능 시간이 지나 오픈 할 수 없습니다.",
	L"ServiceID가 잘못 되었습니다.",
	L"캐시 잔액 정보를 찾을 수 없습니다.",
	L"[TW]ServiceCode 가 올바르지 않습니다.",
	L"[TW]정의되지 않은 애러 발생했습니다.",
	L"[TW]시간형식이 잘못되었습니다.",
	L"[TW]Type정보가 잘못되었습니다.",
	L"[TW]이전 상품 구매가 진행 중입니다.",
	L"레벨이 높아 퀘스트를 수락 받지 못합니다.",
	L"파라매터 갯수 입력이 잘못되었습니다.",
	L"[JP] 합성제가 없습니다.",
	L"[JP] 없는 아바타를 합성하려고 했습니다.",
	L"[US]월 선물 가능 횟수를 초과하였습니다.",
	L"[CN]1인 플레이 던전 입니다.",
	L"[CN]던전 오픈 시간이 아닙니다.",
	L"정상적인 캐릭터 선택이 아닙니다.",
	L"캐릭터 자신에게 일반우편을 보낼려고 했습니다.",
	L"펫 정보가 이상합니다.",
	L"펫이 소환된 상태에서만 사용 가능합니다.",
	L"성체 상태의 펫만 사용 가능합니다.",
	L"유체 상태의 펫만 사용 가능합니다.",
	L"태그 횟수 세팅이 이상합니다.",
	L"팀원이 최소 2명 이상이어야 시작할 수 있습니다.",
	L"대전 아이템은 이용할 수 없는 기능입니다.",
	L"대전 아이템 사용 횟수가 남지 않았습니다.",
	L"대전 아이템 사용 횟수가 남지 않았습니다.",
	L"대전 아이템 사용 횟수가 남지 않았습니다.",
	L"던전 게임 입장을 위한 배틀필드 이탈입니다.",
	L"대전 게임 입장을 위한 이탈입니다.",
	L"채널 이동에 의한 이탈입니다.",
	L"같은 파티원 찾아가기에 의한 이탈입니다.",
	L"이동 아이템 사용에 의한 이탈입니다.",
	L"비정상종료로 인한 이탈.",
	L"던전 게임중에는 파티원을 초대 할 수 없습니다.",
	L"이미 다른 파티에 소속되어 있어 초대 할 수 없습니다.",
	L"대상이 대전 게임중이라 초대할 수 없습니다.",
	L"대상이 던전 게임중이라 초대할 수 없습니다.",
	L"동일한 서버에서 해당 유저를 찾을 수 없습니다.",
	L"더 이상 파티원을 초대 할 수 없습니다.",
	L"대전 게임중에는 파티원을 초대 할 수 없습니다.",
	L"필드에 입장할 수 있는 레벨이 부족합니다.",
	L"필드에 입장할 수 있는 조건의 던전을 클리어 하지 않았습니다.",
	L"퀘스트는 최대 30개까지만 수행이 가능합니다.",
	L"보유한 ED가 부족합니다.",
	L"자동 파티 매칭중인 유저는 파티 초대를 할 수 없습니다.",
	L"자동 파티 매칭중일때는 파티 초대를 할 수 없습니다.",
	L"대전 로비에서는 파티 초대를 할 수 없습니다.",
	L"훈련중에는 파티원을 초대할 수 없습니다.",
	L"대전 매칭 중에는 파티 초대를 할 수 없습니다.",
	L"자동 파티 신청을 등록한 상태에서는 파티 초대를 할 수 없습니다.",
	L"대전 매칭 신청을 등록한 상태에서는 파티 초대를 할 수 없습니다.",
	L"현재 필드가 파티가 소속된 필드 입니다.",
	L"이미 다른 파티 초대 대기자 상태여서 파티 초대를 할 수 없습니다.",
	L"이 이벤트 쿠폰은 계정당 한 번만 받을 수 있습니다.",
	L"[ID] ID 또는 비밀번호가 잘못되었습니다.",
	L"등록되어있지 않은 닉네임입니다.",
	L"서버 그룹 아이디가 잘못되었습니다.",
	L"[TW] 공유 은행 처리중인 아이템 입니다. 은행을 닫고 다시 시도해 주세요.",
	L"온라인 토너먼트 참가 신청권이 없는 유저는 대회 채널을 이용하실 수 없습니다.",
	L"큐브를 개봉 하기 위해서는 아이템과 ed 가 필요합니다.",
	L"ED가 부족해서 큐브를 개봉 할 수 없습니다.",
	L"현재 인원으로 시작합니다.",
	L"길드 생성은 하루에 한 번 가능합니다.",
	L"[ID] ID 는 최소 4자에서 최대 20자 까지 입력 가능 합니다.",
	L"[ID] PW 는 최소 6자에서 최대 15자 까지 입력 가능합니다.",
	L"이벤트 기간이 끝났습니다.",
	L"아라 캐릭터로는 가져올 수 없는 아이템입니다.",
	L"금일 선물하기 제한을 초과하였습니다.",
	L"아쉽게도 교환 수향이 모두 소진 되었습니다. 1월 17일 점검 이후 교환 수량이 추가 됩니다.",
	L"[ID] 인증실패 : 서버와의 연결을 확인해 주십시오.",
	L"존재하지 않는 펫입니다.",
	L"이미 사용 중인 기능입니다.",
	L"유체 이상일 경우에만 사용 가능합니다.",
	L"게임 대기 중인 상태에서는 스킬 슬롯을 변경할 수 없습니다.",
	L"현재 방에 입장한 상태가 아니라서 필드 몬스터 정보 불러오기를 실패하였습니다.",
	L"필드방 상태가 아니어서 필드 몬스터 정보 불러오기를 실패하였습니다.",
	L"캐릭터 슬롯 확장 상품을 구매할 수 없습니다. 슬롯 확장은 30개까지 가능합니다.",
	L"던전 강퇴 투표에 의한 이탈입니다.",
	L"투표자가 정상적인 유저가 아닙니다.",
	L"투표자가 정상적인 유저가 아닙니다.",
	L"빌링 서버가 초기화 되지 않았습니다.",
	L"상품 구매중 문제가 발생하였습니다.",
	L"구매 처리 중입니다. 잠시 후에 다시 시도해 주세요.",
	L"아직 처리 중인 작업이 있습니다. 잠시 후에 다시 시도 해주세요.",
	L"아직 빙고판이 완성되지 않았습니다.",
	L"일시적인 오류입니다. 잠시 후에 다시 시도 해주세요.",
	L"초대 메시지 등록 비용이 부족하므로 등록할 수 없습니다.",
	L"길드 초대 메시지 등록을 실패하였습니다.",
	L"길드구함 광고 등록을 실패하였습니다.",
	L"길드 초대 요청이 실패하였습니다.",
	L"이미 등록한 길드구함 광고가 있습니다.",
	L"이미 등록한 길드 초대 메시지가 있습니다.",
	L"길드구함 광고 등록 비용이 부족하므로 등록할 수 없습니다.",
	L"해당 길드가 최대 인원에 도달하여 가입하실 수 없습니다.",
	L"길드 초대 메시지가 존재하지 않습니다.",
	L"길드구함 광고가 존재하지 않습니다.",
	L"존재하지 않는 길드입니다.",
	L"초대 유저 목록을 볼 수 있는 권한이 없습니다.",
	L"[US] 던전 입장 회수 제한을 초과하였습니다.",
	L"로컬 랭킹 유저 정보 초기화 오류",
	L"로컬 랭킹 유저 정보가 초기화 되어있지 않습니다.",
	L"프로필 정보는 20자까지만 입력 가능합니다.",
	L"로컬 랭킹 게시판 조회 오류",
	L"로컬 랭킹 유저 정보 쓰기 오류",
	L"다음 정보 수정은 익일 06시 이후에 가능합니다.",
	L"[TW]2인 플레이 던전입니다.",
	L"더 이상 추천 할 수 없는 유저입니다.",
	L"비밀번호가 잘 못 되었습니다",
	L"투니랜드 계정이거나, 계정 인증 이후 추천 가능 합니다.",
	L"동일 명의의 계정이므로 추천 할 수 없습니다.",
	L"추천을 실패 하였습니다.",
	L"PASSWORD 길이가 잘못되었습니다.",
	L"프로필 정보에 사용할 수 없는 단어가 들어있습니다.",
	L"영웅 대전 후보 리스트에 이미 존재하는 유닛입니다.",
	L"닉네임 길이가 잘못되었습니다.",
	L"영웅 대전 후보 리스트에 존재하지 않는 유닛입니다.",
	L"--/;/따옴표/쉼표/공백 등은 사용할 수 없습니다.",
	L"필드 100% 즐기기 필수 가이드북를\n 보유하고 있지 않습니다.",
	L"챕터 등록을 실패 하였습니다.",
	L"이미 가이드 북에 기록되어 있습니다.",
	L"해당 유닛은 존재하지 않습니다.",
	L"UNKNOWN",
	L"벌써 임자가 있습니다.",
	L"양다리는 안됩니다!",
	L"마을, 필드에서만 커플 신청을 할 수 있습니다.",
	L"접속중인 유저가 아닙니다.",
	L"인벤토리에 커플링이 존재하지 않습니다.",
	L"동성 캐릭터간의 커플은 한국에서는 안됩니다.",
	L"해당 서버군에 존재하지 않는 캐릭터 명입니다.",
	L"커플 신청 받는 유저의 정보가 이상합니다.",
	L"정상적인 캐릭터 명이 아닙니다.",
	L"알 수 없는 에러입니다.",
	L"존재하지 않는 유저입니다.",
	L"상대방이 실수로 커플링을 떨어트렸어요! 얼른 찾아오라고 말해주세요!",
	L"상대방이 수락했지만, 내 인벤토리에 커플링이 없어요! 인벤토리를 확인해 주세요!",
	L"인연 시스템 사용 아이템 삭제 실패",
	L"요청한 연인 정보의 캐릭터 정보가 이상합니다.",
	L"당신은 솔로입니다. 연인이 없어 드릴 정보가 없습니다.",
	L"선택한 옵션값이 이상합니다.",
	L"인벤토리에 결혼반지가 존재하지 않습니다.",
	L"선택한 옵션에 필요한 캐시 아이템이 인벤토리에 존재하지 않습니다.",
	L"결혼을 하기 위해서는 커플이 되고 72시간이 지나야 합니다.",
	L"결혼을 하려면 커플 상태여야 합니다.",
	L"무료 1차 전직 이벤트를 진행할 수 있는 상태가 아닙니다.",
	L"인벤토리에 결혼에 필요한 아이템이 존재하지 않습니다.",
	L"결혼하기 아이템 사용 실패 하였습니다.",
	L"결혼식에 초대하려는 인원 수보다 인벤토리에 있는 초대권 수가 부족합니다.",
	L"이벤트 기간이 종료되어 전직이 불가능합니다.",
	L"탈 것을 더 이상 등록 할 수 없습니다. \n사용하지 않는 탈 것을 놓아주세요!",
	L"탈 것 생성 아이템을 사용할 수 없습니다.",
	L"탈 것 정보를 가져오지 못했습니다.",
	L"탈 것을 생성할 수 있는 아이템이 아닙니다.",
	L"탈 것 생성에 실패 하였습니다.",
	L"이미 다른 탈 것이 소환되어 있습니다.",
	L"존재하지 않는 탈 것입니다.",
	L"해당 탈 것은 소환되어 있지 않습니다.",
	L"소환되어 있는 탈 것이 없습니다.",
	L"탈 것 놓아주기에 실패하였습니다.",
	L"소환되어 있는 탈 것은 놓아줄 수 없습니다.",
	L"탈 것을 소환할 수 없는 지역입니다.",
	L"보유 중인 탈 것이 없습니다.",
	L"존재하지 않는 아이템에 대한 정보를 요청하였습니다.",
	L"웨딩 관련 아이템이 아닙니다.",
	L"웨딩 관련 아이템의 정보가 없습니다.",
	L"예약된 결혼식장의 정보가 없습니다.",
	L"결혼식을 예약한 유저의 닉네임을 찾을 수 없습니다.",
	L"사용가능한 커플, 결혼 상태가 아닙니다.",
	L"던전에서는 소환하기를 사용 할 수 없습니다.",
	L"[INT] 지원하지 않는 캐시 타입 입니다.",
	L"소환하기를 사용 할 준비가 되지 않았습니다.",
	L"동일 채널에서만 소환하기가 가능합니다.",
	L"연인 소환을 위한 필드 이탈",
	L"아이템 정보가 존재하지 않습니다.",
	L"결혼식장에 입장 가능한 위치가 아닙니다.",
	L"결혼식장에 입장 가능한 아이템이 아닙니다.",
	L"결혼식장 입장을 위한 필드 이탈",
	L"결혼식이 완료 되었거나 존재하지 않는 결혼식입니다.",
	L"커플 상태가 아닙니다.",
	L"결혼식장에 입장이 실패하였습니다.",
	L"현재 필드에 소환이 불가능 합니다.",
	L"이별이 가능한 위치가 아닙니다.",
	L"위자료를 지불 할 수 없습니다.",
	L"신랑이나 신부가 아닙니다.",
	L"신랑 신부가 모두 방에 존재해야 합니다.",
	L"사랑의 리퀘스트를 가지고 오세요~뽀~",
	L"새콤달콤 뽀피트 미니미가 우편으로 지급되었습니다.",
	L"새콤달콤 뽀피트 미니미가 우편으로 지급되었습니다.",
	L"사랑의 리퀘스트를 가지고 오세요~뽀~",
	L"뽀루의 러브 레터를 발송하려면 1만 ED가 필요합니다.",
	L"뽀루의 러브 레터를 발송하지 못했습니다.",
	L"인연 상태가 아닙니다.",
	L"애칭 변경권의 정보가 이상합니다.",
	L"인연 상태에서는 삭제 할 수 없습니다.",
	L"[ES_INT] VIP 기간 갱신 중 문제가 발생했습니다.",
	L"[JP] 선택 할 수 없는 옵션입니다.",
	L"[JP] 합성 부위가 다릅니다.",
	L"결혼 상태가 아닙니다.",
	L"연인이 필드 입장에 실패 하였습니다.",
	L"미라클 큐브의 아이템 레벨이 맞지 않습니다.",
	L"이미 감정이 된 아이템 입니다.",
	L"연인이 필드 입장에 실패 하였습니다.",
	L"아이템 감정을 위한 수수료가 부족합니다.",
	L"아이템 감정을 실패 하였습니다.",
	L"미라클 큐브의 수량이 부족합니다.",
	L"해당 아이템은 감정할 수 없습니다.",
	L"[ES_INT]이미 같은 계정의 다른 캐릭터로 신청하셨습니다.",
	L"[ES_INT]이미 해당 캐릭터로 신청하셨습니다.",
	L"변경 대상 아이템이 아닙니다.",
	L"아이템 변경에 필요한 수수료가 부족합니다.",
	L"[ES_INT]금일 이벤트에 응모할 응모권이 없습니다.",
	L"[ES_INT]금일 이벤트 응모에 실패하였습니다. 다시 시도해 주세요.",
	L"[ES_INT]이벤트 기간이 끝났습니다.",
	L"동일 등급의 스킬을 동시에 습득하려고 합니다.",
	L"교환 가능한 수량이 모두 소진되었습니다.",
	L"아이템 교환에 실패하였습니다.",
	L"오늘의 붉은 기사단 초대 확정권은\n모두 소진되었습니다.\n일일 수량은 06시에 갱신됩니다.\n내일 다시 도전해주세요!",
	L"새로 생성한 캐릭터는 3일간 거래를 이용할 수 없습니다.",
	L"기간제 아이템은 재료나 교환 아이템으로\n사용할 수 없습니다.",
	L"기간제 아이템은 재료나 교환 아이템으로\n사용할 수 없습니다.",
	L"요구 조건을 만족하지 않습니다. 확인해주세요.",
	L"[BR] 레벨업 인증 에러",
	L"엘소드와 엘리시스는 남매이므로 커플 요청을 할 수 없습니다.",
	L"현재 레벨에서는 사용할 수 없습니다.",
	L"선택한 유닛 클래스 정보가 이상합니다.",
	L"해당되는 점핑 이벤트가 없습니다.",
	L"점핑 이벤트 대상자가 아닙니다.",
	L"점핑 하려는 대상 직업이 이상합니다.",
	L"점핑은 마을이나 필드에서만 가능합니다.",
	L"점핑 이벤트 정보가 초기화 되지 않았습니다.",
	L"점핑 캐릭터 오류입니다.",
	L"기본 직업 대상자만 점핑할 수 있습니다.",
	L"점핑 이벤트 대상자가 아닙니다.",
	L"아이템을 생성하지 못했습니다.",
	L"기간제 아이템은 추출 할 수 없습니다.",
	L"추출 가능한 아이템이 아닙니다.",
	L"아이템 수량이 이상합니다.",
	L"인벤토리 공간이 부족합니다.",
	L"아이템 추출에 실패하였습니다.",
	L"추출 아이템 정보가 이상합니다.",
	L"아이템 사용에 실패하였습니다.",
	L"궁극기 사용 아이템이 아닙니다.",
	L"아이템 수량이 이상합니다.",
	L"아이템 정보가 이상합니다.",
	L"해당 데이터 없음",
	L"거래 정지 상태의 계정은 개인 거래를 할 수 없습니다.",
	L"배틀필드에서는 NPC카드를 사용할 수 없습니다",
	L"이 열매는 '베르드의 마수'에게만 먹일 수 있습니다.",
	L"'정화의 열매'만 먹일 수 있습니다.",
	L"취소된 쿠폰입니다.",
	L"PC방에서만 사용할 수 있는 쿠폰입니다.",
	L"쿠폰 오류입니다.",
	L"사용 제한 수량을 초과하였습니다.",
	L"쿠폰 등록에 실패하였습니다.",
	L"쿠폰 정보 읽기에 실패하였습니다.",
	L"쿠폰핀 규칙이 맞지 않습니다.",
	L"잘못된 쿠폰 박스 타입입니다.",
	L"해당 쿠폰의 할인 적용 가능 아이템 목록 읽기에 실패하였습니다.",
	L"CheckKom.xml 파싱 오류로 인한 종료입니다.",
	L"",
	L"스테이지 정보가 잘못되었습니다.",
	L"인벤토리에 가상 결혼 신청서가 존재하지 않습니다.",
	L"인벤토리에 가상 결혼 계약서가 존재하지 않습니다.",
	L"가상 결혼 상대방이 존재하지 않습니다.",
	L"가상 결혼 계약서 삭제 실패",
	L"가상 결혼 신청서 삭제 실패",
	L"조건이 맞지 않습니다. 펫이 소환되어 있는지 확인 바랍니다.",
	L"조건이 맞지 않습니다. 사용 가능한 펫 인지 확인 바랍니다.",
	L"축하합니다! 할로윈 호박 요정의 묘약을 성공적으로 사용 하셨습니다.",
	L"조건이 맞지 않습니다. 펫 친밀도가 100% 인지 확인 바랍니다.",
	L"캐릭터 생성 횟수제한에 의해 캐릭터를 생성할 수 없습니다.",
	L"상대방이 수락했지만, 내 인벤토리에 가상 결혼 신청서가 없어요! 인벤토리를 확인해 주세요!",
	L"상대방이 실수로 가상 결혼 신청서를 잃어버렸어요! 얼른 찾아오라고 말해주세요!",
	L"가상 부부 상대방이 있어야 시작할 수 있습니다.",
	L"던전 게임 신청 중에는 사용할 수 없는 기능입니다.",
	L"공식 대전 신청 중에는 사용할 수 없는 기능입니다.",
	L"룸 리스트 조회 중에는 사용할 수 없는 기능입니다.",
	L"올바르지 않은 SubStageID 입니다.",
	L"플레이 가능 횟수를 초과하였습니다.",
	L"브라질 쁘띠 뽀루에게만 사용 할 수 있습니다.",
	L"공식 대전에는 난입할 수 없습니다.",
	L"잘못된 스테이지 정보 요청입니다.",
	L"적용 가능 하한 금액 미만입니다.",
	L"적용 가능 상한 금액 초과입니다.",
	L"이미 등록된 쿠폰입니다.",
	L"할인 대상 아이템이 아닙니다.",
	L"PC방이 아닙니다.",
	L"현재 대리 판매 보증서(VIP)를 이용 중입니다",
	L"대리상인에 등록된 아이템이 있어서 다른 종류의 보증서를 등록할 수 없습니다.",
	L"해당 유저는 거래가 제한된 상태입니다.",
	L"ED 가 부족합니다",
	L"더 이상 슬롯을 확장할 수 없습니다.",
	L"확장에 필요한 금액이 잘못되었습니다.",
	L"해당 필드는 열려있지 않습니다.",
	L"현재 필드에서는 입장할 수 없습니다.",
	L"재도전을 위한 휴식 버프로 인해 시간의 포탈에 입장 할 수 없습니다. 다음 포탈 생성시까지 기다려 주세요.",
	L"Kom 파일의 변조로 인한 종료입니다.",
	L"쿠폰 총 사용 횟수를 초과하여 사용하실 수 없습니다.",
	L"보스 필드 입장에 실패하였습니다. 다시 시도 해주세요.",
	L"사용할 수 없는 권한입니다.",
	L"보스 필드 쿨타임 상태입니다. 닫은 후에 다시 시도해주세요.",
	L"해당 필드에서는 보스 필드 게이트가 생성되지 않습니다.",
	L"사용 권한이 없습니다.",
	L"보스 필드가 아닙니다.",
	L"쿨 타임 상태입니다.",
	L"보스 필드정보가 존재하지 않습니다.",
	L"사전 생성한 캐릭터는 삭제할 수 없습니다",
	L"보스 필드의 제한 시간이 종료되었습니다.",
	L"무기와 방어구 아이템에만 사용 할 수 있습니다.",
	L"소켓 슬롯 추가에 실패하였습니다.",
	L"인벤토리의 재료 아이템만 사용할 수 있습니다.",
	L"파괴된 아이템에는 사용할 수 없습니다.",
	L"방어구 소켓 슬롯을 추가하기 위한 재료 아이템을 가지고 있지 않습니다.",
	L"소켓 슬롯 추가에 필요한 재료 아이템이 존재하지 않습니다.",
	L"인벤토리의 아이템에만 사용할 수 있는 기능입니다.",
	L"장착 불가 아이템에는 사용할 수 없습니다.",
	L"PC방 프리미엄 아이템에는 사용할 수 없습니다.",
	L"무기 소켓 슬롯을 추가하기 위한 재료 아이템을 가지고 있지 않습니다.",
	L"더 이상 소켓 슬롯을 추가 할 수 없습니다.",
	L"소켓 슬롯 추가에 필요한 대상 아이템이 존재하지 않습니다.",
	L"hsb 파일이 클라이언트와 맞지 않습니다.",
	L"활성화 중인 스킬 페이지 번호가 이상 합니다.",
	L"클라이언트에서 보내준 활성화된 스킬 페이지 번호가 서버의 정보와 다릅니다.",
	L"사용하기로 선택한 스킬 페이지가 이상 합니다.",
	L"추가된 페이지에 기본을 스킬 등록 하지 못 했습니다.",
	L"더 이상 스킬 페이지를 확장 할 수 없습니다.",
	L"스킬 페이지를 확장한 적이 없는데 페이지 변경을 시도함",
	L"스킬 페이지 변경 업데이트 실패",
	L"마을 및 휴식처에서만 구매 가능 합니다.",
	L"1개권 만 구입 가능 합니다.",
	L"더 이상 구입 할 수 없습니다.",
	L"버프 효과가 지속되고 있는 동안 동일한 버프 효과의 아이템을 다시 사용할 수 없습니다.",
	L"레벨 그룹이 다른 유저와 전장을 같이 플레이 할 수 없습니다.",
	L"불량 유저 검사 시스템에 의한 파티 이탈입니다.",
	L"파티 재결합에 의한 파티 이탈입니다.",
	L"유효하지 않은 파티원 검사에 의한 파티 이탈입니다.",
	L"어둠의 문 진입 전 이탈로 인한 파티 이탈입니다.",
	L"당신의 계정은 차단되었습니다(LE3) 웹사이트를 방문하여 문의 하세요.",
	L"로그인이 불가능합니다(LE2). 웹사이트를 방문하여 문의 하세요.",
	L"당신의 계정은 차단되었습니다(LE4) 웹사이트를 방문하여 문의 하세요.",
	L"로그인이 불가능합니다(LE1). 웹사이트를 방문하여 문의 하세요.",
	L"PVE게임에 존재하지 않는 던전 모드입니다.",

	L""
};
//ID버전 출력
static wchar_t* szErrorStrF[] = {
	L"Berhasil",
	L"Unknown error",
	L"User status error.",
	L"Versi salah. Silakan login kembali setelah melakukan Patching.",
	L"Server Penuh",
	L"Checking the game server. Silakan login setelah check selesai.",
	L"Overlapped logins between the same server.",
	L"Overlapped logins between different servers.",
	L"Verifikasi IP server gagal.",
	L"UID server crash.",
	L"User name tidak ada.",
	L"The user has been deleted.",
	L"Password salah",
	L"Flag set-up for server working failed.",
	L"Nermarble authorization failed.",
	L"Account telah diblok.",
	L"Only the administrator level or higher may request.",
	L"Tidak ada proxy ID",
	L"IP tidak diapprove",
	L"There is no Channel ID information in the game server.",
	L"There is no problem with the ID length.",
	L"Transaksi gagal",
	L"Incorrect ID length .",
	L"Incorrect PASSWORD length.",
	L"Incorrect name length.",
	L"A deleted user has attempted to generate a character.",
	L"Nickname sudah terpakai",
	L"Transaksi gagal",
	L"Gagal memuat data karakter",
	L"Incorrect character nickname length.",
	L"No more characters may be generated.",
	L"The nickname cannot be used for a certain period of time.",
	L"CharacterSN for netmarble messenger has not been generated in the DB.",
	L"Nickname tidak dapat digunakan",
	L"Characters may be generated four times a day.",
	L"Karakter yang dipilih tidak ada di dalam server",
	L"There is a problem with the class of your selected character.",
	L"Information on Resurrection Stones has not been secured in the DB.",
	L"CharacterSN for Netmarble messenger has not been recieved from the DB.",
	L"A DB error has occurred in the process of selecting a character.",
	L"Kamu harus mengganti nickname kamu",
	L"A PVP channel fit for this level has not been found.",
	L"The user to be deleted could not be found in the channel.",
	L"Pergantian channel gagal",
	L"AP tidak mencukupi, kamu tidak dapat memasuki",
	L"You have not entered the market square channel.",
	L"Kamu tidak memasuki channel",
	L"Tidak ada kompetisi yang sedang berlangsung",
	L"Party list cannot be called.",
	L"Party lists may be checked only at the dungeon gate.",
	L"The party list cannot be seen at this dungeon gate.",
	L"Level 3 ke atas diperbolehkan memasuki pertarungan",
	L"Kamu tidak dapat membuat room baru di server room",
	L"User tidak dapat ditemukan di dalam slot.",
	L"Slot karakter tidak dapat ditemukan.",
	L"Room Penuh",
	L"Password salah",
	L"Kamu belum menyelesaikan dungeon sebelumnya",
	L"Tidak ada room untuk Quick Join",
	L"Hanya room leader yang dapat mengajukan",
	L"The room status does not allow packets.",
	L"Semua user belum ready",
	L"Jumlah member team tidak sama",
	L"Semua pemain harus menekan tombol READY untuk memulai",
	L"The loading status has not been changed.",
	L"Room telah berhasil dibuat",
	L"Informasi room telah diubah",
	L"Room sudah dihapus",
	L"User tidak ditemukan di dalam room",
	L"Intrusion has failed.",
	L"You never left the room that allows an intrusion.",
	L"The dead NPC UID is not correct.",
	L"NPC yang bersangkutan sudah mati",
	L"Data monster salah dan  loading gagal",
	L"User tidak ditemukan",
	L"Chat error",
	L"You cannot forcefully remove yourself.",
	L"Jumlah stamina tidak mencukupi",
	L"This is a game type which does not support additional stats.",
	L"Intrusions are not allowed as the game is closing.",
	L"Kamu tidak dapat memasuki room",
	L"The room you are entering has not been identified.",
	L"Kamu tidak dapat menekan tombol READY saat melakukan trading",
	L"Intrusions are not allowed as the match is just starting. Try again later.",
	L"The Start Game item is not in the inventory.",
	L"An unauthorized user has attempted to generate an Observer Room.",
	L"Match map information is not correct.",
	L"Kamu tidak dapat menggunakan Kompetisi Channel jika kamu memiliki tiket turnamen.",
	L"Kamu tidak memiliki item yang dibutuhkan untuk memasuki dungeon ini. Kamu akan otomatis meninggalkan room ketika permainan dimulai",
	L"You may use invitation functions only in a match room.",
	L"Party yang diinvite tidak dapat memasuki room",
	L"Hanya user yang berada di lobby pertandingan yang dapat diinvite",
	L"Kamu hanya dapat memulai permainan jika tergabung sebagai party",
	L"There is a problem with the set-up for the number of kills.",
	L"Item tidak dapat diambil",
	L"Other people have already acquired the skill.",
	L"Gagal memberikan item random",
	L"Informasi item tidak ditemukan",
	L"Incorrect item ID.",
	L"Item belum diberikan",
	L"Item belum direpair",
	L"Item masih dalam cooldown",
	L"Kamu tidak dapat menggunakan quick slot dalam event dungeon",
	L"Item belum diambil",
	L"Items in the temporary inventory has not been received.",
	L"The inventory space for temporary inventory items is not sufficient.",
	L"Message untuk Megaphone terlalu panjang",
	L"Team tidak dirubah",
	L"Slot status has not been changed.",
	L"Status Ready belum berubah",
	L"Kamu gagal keluar dari slot",
	L"Tidak dapat mengeluarkan player",
	L"User data who killed you has not been saved.",
	L"Status belum berubah",
	L"Dikeluarkan oleh user",
	L"Koneksi terputus",
	L"Dikeluarkan dengan paksa",
	L"Kamu tidak diperbolehkan berada di room ini karena jarak levelmu",
	L"Transaksi personal telah berhasil dilakukan",
	L"Participation in personal trades has failed.",
	L"Exit due to delayed participation in personal trades.",
	L"Transaksi personal dibatalkan",
	L"There is a problem with the number of personal trades.",
	L"An error has occurred during the personal trade.",
	L"Inventory tidak cukup atau informasi item bermasalah",
	L"Dungeon tidak dapat dimasuki karena stamina tidak cukup Bermain dalam party agar stamina tidak cepat habis Kamu juga tetap bisa bermain PVP (Ladder/Battle) saat stamina habis.",
	L"Transaksi dibatalkan dan tidak ada item yang dipindahkan",
	L"Transaksi dibatalkan",
	L"Toko telah ditutup karena registrasi item pribadi yang terlambat",
	L"Transaksi toko dibatalkan",
	L"Toko telah ditutup karena semua item telah habis terjual",
	L"Kamu meninggalkan room dan traksaksi telah dibatalkan",
	L"Toko pribadi telah ditutup",
	L"Client Crash",
	L"Kamu tidak dapat berada di channel kompetisi",
	L"Meninggalkan party karena kamu memasuki pertandingan",
	L"Meninggalkan party karena tidak menyelesaikan dungeon",
	L"Meninggalkan party karena  kamu memasuki Market ",
	L"Meninggalkan party karena kamu memasuki training camp",
	L"Normal Shutdown.",
	L"Ditutup karena terdeteksi hacking",
	L"Ditutup karena client crash",
	L"Perubahan experience point gagal",
	L"The class of the requested character has not been identified in the DB.",
	L"Incorrect authority level.",
	L"User tidak dapat ditemukan",
	L"Transaksi gagal",
	L"User does not exist in the authority level.",
	L"Koneksi telah diputus oleh admin",
	L"This command can be carried out only in the company.",
	L"This command cannot be executed in a room.",
	L"Item gagal dibeli",
	L"Item tidak dapat dijual",
	L"Quest sedang dijalankan",
	L"Attempted to delete the quests that are not being executed.",
	L"Quest tidak tersedia",
	L"Quest selesai",
	L"Karakter ini tidak dapat memulai quest",
	L"Quest sebelumnya belum diselesaikan",
	L"Quest yang diperoleh dari NPC tidak cocok",
	L"Quest template tidak tersedia",
	L"Syarat untuk menyelesaikan quest belum diselesaikan",
	L"DB processing related to quests has failed.",
	L"Maksimal 10 standard quest yang bisa dijalankan sekaligus.",
	L"Levelmu terlalu rendah dan kamu belum bisa menerima quest ini",
	L"Quest tidak dapat diselesaikan. Inventory penuh",
	L"Terjadi error pada saat proses pemilihan quest reward",
	L"Searching the selected reward items has failed.",
	L"Quest pergantian job sedang dilakukan",
	L"Terjadi error pada saat proses pemilihan quest reward pergantian job",
	L"There is a problem with the quest types.",
	L"You have entered a time that is shorter than the limited time for a chat.",
	L"Kamu tidak dapat berbisik pada dirimu sendiri",
	L"Terjadi masalah pada type chat",
	L"Karakter offline",
	L"Kamu tidak dapat menggunakan --/;/tanda kutip/koma/spasi, etc.",
	L"You have entered s time that is shorter than the limited time for a user list renewal.",
	L"There is a limited time to search user information.",
	L"Karakter offline",
	L"Nickname tidak dapat digunakan",
	L"Kamu gagal memasuki square",
	L"Kamu gagal keluar dari square",
	L"Kamu sudah mencapai level tertinggi dari skill ini",
	L"Skill tidak tersedia",
	L"Level kamu terlalu rendah dan kamu belum bisa mempelajari skill ini",
	L"Skill point tidak mencukupi",
	L"Kamu tidak memiliki item untuk mengambilnya",
	L"Selesaikan quest untuk memenuhi syarat",
	L"Kamu telah mempelajari skill yang dibutuhkan",
	L"Kamu telah mempelajari skill ini",
	L"The skill slot ID to be changed is wrong.",
	L"You have tried to register unlearned skills.",
	L"Skill belum di",
	L"Kamu tidak dapat menggunakan Skill slot B",
	L"A DB error has occurred while expanding the Skill Slot B.",
	L"Kamu harus menghilangkan seal untuk mempelajari skill",
	L"Skill ini terdapat di jalur yang belum dapat dipelajari",
	L"Skill point tidak mencukupi dan kamu tidak dapat mempelajari skill",
	L"Skill point tidak mencukupi dan kamu tidak dapat mempelajari skill",
	L"You have tried to change the information of a skill that does not exist.",
	L"There is a problem with the skill level information.",
	L"You have tried to set up a level that is higher than the skill limit level.",
	L"Processing the time limit of a cash skill point period has failed.",
	L"Registration of cash skill points has failed.",
	L"Renewal of cash skill point information has failed.",
	L"Reset of cash skill point information has failed.",
	L"Skill basic tidak dapat direset",
	L"Party belum terbentuk",
	L"Kamu gagal meninggalkan party",
	L"Informasi party salah",
	L"The current state does not allow an invitation.",
	L"Kamu tidak berada di dalam party",
	L"Kamu sudah tergabung ke dalam party",
	L"Party tidak dapat dibuat di dalam pertandingan",
	L"Kamu tidak dapat memasuki party di dalam pertandingan",
	L"Party sudah penuh",
	L"Kamu gagal bergabung dengan party",
	L"Party sudah dibubarkan",
	L"Permintaan bergabung partymu ditolak",
	L"Kamu telah menolak permintaan bergabung dalam party",
	L"Kamu tidak dapat menekan tombol READY di lokasi ini",
	L"Changing the ready state has failed.",
	L"Incorrect dungeon selection.",
	L"Gagal memulai permainan",
	L"Incorrect game type.",
	L"Kamu bukan party leader",
	L"Party leader belum diganti",
	L"Jumlah anggota party belum berubah",
	L"Dungeon tidak berada di gerbang dungeon ini",
	L"Kamu gagal mengubah privacy party",
	L"Party leader yang baru tidak dapat memulai dungeon ini",
	L"Party leader tidak dapat mengubah status READY",
	L"Tidak ada teman chat",
	L"There is a problem with the type of party list requests.",
	L"The level limit set-up has not been changed.",
	L"Party ini private",
	L"Tidak ada Resurrection Stone yang bisa digunakan",
	L"Sudah dihidupkan kembali",
	L"Resurrection gagal",
	L"Kamu tidak dapat menggunakan Resurrection Stone di dungeon ini",
	L"You've tried to update a non-existent state value.",
	L"Community option values have not been received.",
	L"You cannot register to the black list any longer.",
	L"User berhasil dimasukkan kedalam black list",
	L"User berhasil dihilangkan dari black list",
	L"User tersebut tidak ada",
	L"It has not been deleted from the black list.",
	L"Kamu tidak dapat menambahkan dirimu sendiri ke dalam black list",
	L"You cannot delete yourself from the black list.",
	L"Only the currently logged-in users may be registered to the black list.",
	L"Kamu tidak dapat menambahkan karaktermu sendiri",
	L"Ada jeda waktu untuk menambahkan teman",
	L"Karakter tidak dapat ditemukan",
	L"Character cannot invite in current state.",
	L"The other party's character cannot be invited in this state.",
	L"You cannot party with other players in this state.",
	L"Friend request tidak diperbolehkan",
	L"Transaksi personal belum diadakan",
	L"Did not leave from the personal trade.",
	L"Personal trade users have not been searched.",
	L"There is a problem with the item information to be registered.",
	L"There is a problem with ED information to be registered.",
	L"Tidak item untuk ditransaksikan",
	L"ED tidak mencukupi",
	L"Only the items in your inventory may be registered.",
	L"Inventory tidak cukup",
	L"Bind item tidak dapat ditransaksikan",
	L"Permintaan trade tidak dapat dilakukan disini",
	L"Kamu tidak dapat melakukan trade dengan dirimu sendiri",
	L"Item dengan durability rendah tidak dapat ditransaksikan, silakan repair dahulu item tersebut",
	L"Item sudah terdaftar",
	L"Try to delete unregistered items.",
	L"Batas item untuk ditransaksikan tidak boleh melebihi 10 item",
	L"The other party's ED will exceed the maximum limit. Please reduce the ED amount for trades.",
	L"Permintaan trade telah diterima",
	L"Saat ini kamu sedang melakukan transaksi dengan user lain",
	L"Permintaan trade telah ditolak",
	L"Cannot trade in this current state.",
	L"Permintaan trade gagal karena server bermasalah",
	L"Kamu tidak memiliki item yang dibutuhkan untuk memasuki dungeon ini",
	L"Persyaratan untuk memasuki training camp belum diselesaikan",
	L"The dungeon to carry out the training camp has no ID.",
	L"An error has occurred while processing a reward.",
	L"Item yang dipilih tidak ada di dalam inventory",
	L"Item yang dipilih bukan item 'cube'",
	L"Item ini tidak dapat digunakan oleh karakter ini",
	L"Kamu membutuhkan kunci lebih banyak untuk membuka kotak ini",
	L"Kotak ini tidak bisa dibuka",
	L"The item has not been generated.",
	L"Inventorymu tidak cukup untuk menyimpan isi kotak",
	L"Double login telah terdeteksi",
	L"User UID bermasalah",
	L"User D bermasalah",
	L"Karakter UID bermasalah",
	L"Nickname karakter bermasalah",
	L"Game server UID bermasalah",
	L"There is a problem with the user information pointer.",
	L"There is a problem with the game server pointer.",
	L"Searching the user UID has failed.",
	L"A problem has occurred in the messenger's serial number.",
	L"Item tidak dapat dibuat",
	L"Item ini bukan magic scroll",
	L"Kamu tidak memiliki ED yang cukup untuk biaya crafting",
	L"Bahan crafting tidak cukup",
	L"Kamu harus menyediakan minimal 1 slot di inventorymu untuk melakukan crafting",
	L"Item belum dibuat",
	L"Sertifikasi Hackshield gagal",
	L"Hackshield telah diupdate harap restart game",
	L"Sertifikasi Hackshield gagal",
	L"Kupon ini sudah digunakan atau kamu salah memasukkan nomor kupon, kupon hanya bisa digunakan sekali di 1 account.",
	L"Silakan masukkan kode 20 digit alpa numerik",
	L"Kupon sudah digunakan",
	L"Kupon bisa digunakan pada",
	L"Kode kupon salah",
	L"Kupon ini hanya bisa digunakan sekali di 1 account",
	L"Kupon tidak dapat ditebus",
	L"Hanya beberapa hari tersisa untuk membeli ulang",
	L"The item list cannot be retrieved.",
	L"There is a problem with the item information to be recieved.",
	L"Inventory tidak cukup. Inventory tambahan tersedia di Item Mall",
	L"Unable to retrieve purchased cash item.",
	L"Unable to retrieve K-Ching balance.",
	L"User info tidak ditemukan",
	L"Account telah dibanned",
	L"Kamu tidak bisa mengirim item karakter lain\nke inventory kamu",
	L"Karaktermu sudah menggunakan item ini",
	L"Item sedang ditransfer, harap tunggu.",
	L"This is a wrong frame mean value.",
	L"Karakter tidak  ada",
	L"This is not subject to nickname restoration.",
	L"Nickname sudah terpakai",
	L"Nickname ini tdak dapat digunakan untuk beberapa waktu",
	L"Restore nickname gagal",
	L"Nickname invalid",
	L"Panjang nickname salah",
	L"Nickname belum diganti",
	L"Toko pribadi belum dibuat",
	L"Kamu hanya dapat mendirikan toko di market square",
	L"Informasi user tidak dapat ditemukan di market square, toko tidak dapat dibuka",
	L"Terlalu banyak user yang sedang membuka toko, coba lagi nanti",
	L"You've failed to take part in a personal shop.",
	L"There is a problem with the quantity information of items to be registered for sale.",
	L"Item sudah terdaftar untuk dijual",
	L"A person, not an installer, has tried to register an item for sale.",
	L"The number of registered items is higher than the number of items that can be sold.",
	L"Kamu tidak memiliki cukup ED untuk membuka toko",
	L"Kamu belum siap untuk membuka toko",
	L"Kamu belum meninggalkan toko",
	L"Jumlah item yang dibeli lebih besar dari item yang akan dijual",
	L"Tidak ada item untuk dibeli",
	L"ED tidak mencukupi untuk membeli item",
	L"Kamu tidak dapat membeli item karena inventorymu tidak mencukupi",
	L"The installer has not been found and trade has not taken place.",
	L"Kamu tidak dapat membuka toko saat melakukan trade",
	L"Toko pribadi sudah dibuka",
	L"Jumlah maksimal ED yang didaftarkan sudah melebihi batas",
	L"Jumlah maksimal ED yang dibeli sudah melebihi batas",
	L"There is a problem with the item information to be purchased.",
	L"Item yang dipilih tidak ada di dalam inventory",
	L"Item ini tidak dapat di-dismantle",
	L"Dismantling gagal, harap coba lagi",
	L"Hanya perlengkapan dan aksesoris yang dapat didismantle",
	L"Gagal mendismantle",
	L"Pembuatan item gagal",
	L"Inventory tidak cukup untuk mendismantle item",
	L"Item berdurasi waktu tidak dapat didismantle",
	L"Item yang akan dienhance tidak ada di dalam inventory",
	L"Tidak ada informasi item",
	L"Hanya senjata dan armor yang dapat dienhance",
	L"Item ini tidak dapat dienhance",
	L"Tidak dapat dienhance lagi",
	L"Tidak ada Enhancement Stone",
	L"Enhancement tidak dapat dilakukan",
	L"ED tidak cukup untuk melakukan enhancement",
	L"An installed item cannot be enhanced.",
	L"Tidak ada Flourite Ore di dalam inventory",
	L"Kamu tidak dapat menggunakan Flourite Ore",
	L"Flourite Ore tidak dapat digunakan dari enhancement level 10 pada saat enhancement berlangsung",
	L"Item yang tidak diperbolehkan untuk dipakai tidak dapat dienhance",
	L"Enhancement gagal",
	L"Tidak ada item yang akan direstore  di dalam inventory",
	L"Tidak ada informasi item",
	L"Ini bukan item untuk restorasi",
	L"Hanya item yang tidak dapat dipakai yang dapat direstorasi",
	L"Restoration Stone tidak cukup dan proses restorasi tidak dapat dilakukan",
	L"Item yang diinstal tidak dapat direstore",
	L"This function cannot be used for items that cannot be used.",
	L"Enhancement Berhasil",
	L"There is no change.",
	L"A one step fall",
	L"Enhancement stage initialization",
	L"Item tidak dapat digunakan",
	L"Item tidak ada di inventory.",
	L"Tidak ada informasi item",
	L"Biaya tidak mencukupi",
	L"Hanya senjata dan armor yang memiliki socket",
	L"There is no magic stone information as to the equipment.",
	L"Tidak ada magic stone di dalam inventory",
	L"Socket tidak dapat digunakan.",
	L"Magic stone sudah terpasang di socket.",
	L"Magic stone sudah dilepaskan dari socket.",
	L"Pilihan socket tidak tersedia.",
	L"Nama ini tidak ada.",
	L"Kamu tidak dapat mereferensikan diri sendiri.",
	L"Tidak ada tujuan penerima hadiah.",
	L"Data Master-Apprentice yang dihapus tidak ada di DB.",
	L"Daftar apprentice belum diterima.",
	L"Level rendah dan informasi tidak dapat diberikan.",
	L"Level rendah dan apprentice tidak dapat diterima.",
	L"Sampai 3 apprentice dapat didaftarkan.",
	L"Master sudah ada.",
	L"Kamu telah didaftarkan sebagai apprentice.",
	L"Ada masalah dengan informasi item yang dibeli.",
	L"Item tidak dapat dibeli.",
	L"Overlapped settlement",
	L"Jumlah item yang dapat dibeli telah terlampaui.",
	L"Jumlah item yang dapat dibeli telah terlampaui.",
	L"Item tidak tersedia untuk dibeli lagi.",
	L"Tidak ada cart.",
	L"Ini adalah game dengan penggunaan terbatas.",
	L"Kamu tidak dapat membeli karena jumlah penjualan terbatas.",
	L"Jumlah pesanan terbatas.",
	L"Periode pembelian kembali item ini akan segera tersedia.",
	L"Item ini tidak dapat diberikan sebagai hadiah.",
	L"Jumlah pesanan terbatas.",
	L"Periode pembelian kembali item ini akan segera tersedia.",
	L"User diblok.",
	L"Informasi member tidak dapat ditemukan.",
	L"Saldo tidak cukup.",
	L"Memeriksa.",
	L"Jumlah maksimum Ressurection Stones yang bisa dimiliki akan terlampaui",
	L"Kamu tidak dapat memperluas jumlah inventori.",
	L"Tidak dapat memberikan hadiah kepada diri sendiri.",
	L"Password salah.",
	L"Nama ini tidak ada.",
	L"Kamu tidak dapat memberikan hadiah ke experience ID user.",
	L"Jumlah limit hadiah terlampaui.",
	L"Masukkan password.",
	L"Netmarble account certification failed.",
	L"Item ini tidak dapat diberikan sebagai hadiah.",
	L"User di bawah 14 tahun tidak dapat menggunakan fungsi ini.",
	L"Item Penambahan slot karakter tidak dapat dibeli. Penambahan Slot Hanya diperbolehkan sampai 6 karakter.",
	L"Karakter slot tidak diperluas.",
	L"Kamu sudah menggunakan fungsi ini.",
	L"Slkill tidak tersedia.",
	L"Tidak ada item skill initialization.",
	L"Item skill initialization belum dihapus dari DB.",
	L"Kesalah dikembalikan sebagai hasil DB skill initialization.",
	L"Skill ini bukan untuk initialization.",
	L"Skill yang dipakai di slot skill tidak dapat menggunakan skill restore.",
	L"Item ini tidak dapat didaftarkan ke daftar reserved item.",
	L"Kamu tidak dapat mendaftarkan item ke daftar reserved item.",
	L"Fungsi ini tidak dapat digunakan dengan experience ID.",
	L"Experience ID tidak dapat didaftarkan sebagai recommender.",
	L"Kamu mencapai level 20 sehingga bisa memakai experience ID. Jika ingin tetap bermain, silahkan convert ke Netmarble ID.",
	L"User experience ID tidak dapat didaftarkan sebagai teman messenger.",
	L"User ini tidak didaftarkan di daftar hacking.",
	L"Data hacking user agreement belum diupdate di DB.",
	L"User ini dilaporkan hacking dan user belum setuju.",
	L"Item tidak ada  di inventori.",
	L"Attribute enhancement tidak dapat dilakukan.",
	L"Ini adalah slot yang attribute sudah pernah enhanced.",
	L"Ini adalah slot yang attribute enchancement sudah pernah dilepaskan",
	L"Biaya attribute enhancement tidak cukup.",
	L"El pieces untuk attribute enhancement tidak cukup.",
	L"Terdapat masalah dengan informasi item.",
	L"Hanya senjata dan armor yang dapat enhance attribute.",
	L"Item yang dipilih tidak dapat diidentifikasi.",
	L"Kamu membutuhkan discriminator lebih untuk evaluasi item.",
	L"Space inventori tidak cukup untuk identifikasi item.",
	L"Item tidak dapat diidentifikasi.",
	L"Item creation GAGAL euy.",
	L"Kamu tidak dapat memperoleh informasi mailbox.",
	L"Item yang dipilih tidak dapat dilampirkan di surat.",
	L"ED yang dimasukkan tidak dapat dilampirkan di surat.",
	L"Pengiriman surat gagal.",
	L"Surat ini tidak ada.",
	L"Tidak ada ruangan di inventori dan item yang dilampirkan tidak dapat dipindahkan. Harap cari tempat.",
	L"Item yang dilampirkan tidak bisa diimpor.",
	L"Kamu mencoba melampirkan item yang tidak ada.",
	L"Hanya item yang sudah diperbaiki bisa dilampirkan.",
	L"Nama ini tidak ada.",
	L"Tidak dapat mengirimkan surat ke diri sendiri.",
	L"Surat tidak dapat dikirimkan karena dana tidak cukup.",
	L"Surat tidak dapat dikirimkan karena mailbox penerima penuh.",
	L"Kamu memilih surat yang sudah dihapus.",
	L"Pihak lain menolak menerima surat.",
	L"Atrribute item tidak dapat dilampirkan dalam surat.",
	L"Kamu tidak dapat mengirimkan surat ke experience ID user.",
	L"Nama ini tidak ada.",
	L"Nama sudah didaftarkan ke daftar hitam.",
	L"Pendaftaran ke blacklist gagal.",
	L"Nama sudah dihapus.",
	L"Belum dihapus dari blacklist.",
	L"Kamu tidak punya title ini.",
	L"Title sudah dipakai.",
	L"Title kadaluarsa dan tidak bisa digunakan.",
	L"Title tidak ada.",
	L"Kamu belum memperoleh title.",
	L"Kamu sudah punya title ini.",
	L"Item tidak dapat digunakan.",
	L"Item tidak ada.",
	L"Level karakter kurang dan kamu tidak dapat menggunakan item.",
	L"Item tidak dapat digunakan dalam inventori.",
	L"Item tidak ada di inventory.",
	L"Item ini bukan untuk enchanment.",
	L"Item tidak bisa enchanted.",
	L"Update gagal ketika melakukan enhancement level.",
	L"Item tidak dapat ditukar.",
	L"Item tidak ada di inventory.",
	L"Item ini bukan untuk untuk ditukar.",
	L"Item exchange belum dibuat.",
	L"Untuk pindah ke field lain.",
	L"Untuk masuk ke room lain.",
	L"Sistem sudah memproses bahwa user sudah keluar.",
	L"Kamu tidak dapat masuk ke desa.",
	L"Kamu gagal keluar dari desa.",
	L"Daftar user tidak dapat diperoleh.",
	L"Level terlalu cupu dan tidak dapat masuk.",
	L"Kamu belum membereskan dungeon untuk memasuki desa.",
	L"Channel tidak ada.",
	L"Koneksi telah terputus karena channel movement sukses.",
	L"Pendaftaran user untuk channel movement gagal.",
	L"Pendaftaran user untuk channel movement belum dilakukan.",
	L"Channel tujuan dan channel sekarang sama.",
	L"Jumlah sudah terpenuhi dan kamu tidak bisa pindah.",
	L"Kamu bisa menggunakan fungsi channel movement ketika di desa dan field.",
	L"Gunakan channel movement nanti.",
	L"Kamu tidak bisa menggunakan fungsi ini ke karakter GM.",
	L"Kamu belum memilih karakter.",
	L"Teman belum login.",
	L"Ada masalah dengan keadaan.",
	L"Item yang drop sebagai event belum diupdate di DB.",
	L"Kamu sudah menerima item time event.",
	L"Karakter ini sudah didaftarkan sebagai teman.",
	L"Status karakter teman invalid.",
	L"Ini bukan karakter yang kamu undang sebagai teman.",
	L"Karakter ini bukan teman kamu.",
	L"Grup tidak ada.",
	L"Pesan terlalu panjang.",
	L"Grup saat ini dan grup tujuan sama.",
	L"Tidak dapat membuat grup lagi.",
	L"Grup yang sama sudah ada.",
	L"Nama grup sama.",
	L"Apprentice belum login selama 7 hari, hubungan Master-Apprentice putus.",
	L"Apprentice sudah menyelesaikan level up, hubungan Master-Apprentice putus.",
	L"Karakter telah dihapus dan hubungan Master-Apprentice putus.",
	L"Hubungan Master-Apprentice diputuskan user.",
	L"Memutuskan hubungan Master-Apprentice gagal.",
	L"Fitur friends yang berhubungan dengan operasi DB gagal.",
	L"Nama grup terlalu panjang.",
	L"Daftar teman sudah penuh dan kamu tidak dapat mengundang teman lagi.",
	L"Kamu tidak dapat mengundang karakter teman dengan account yang sama.",
	L"Daftar teman sudah penuh dan kamu tidak dapat mengundang teman lagi.",
	L"Pihak lain menolak permintaan teman.",
	L"Query call gagal.",
	L"Kamu tidak dapat mengganggu channel kompetisi.",
	L"Kamu dapat memulai game ketika room penuh saat kompetisi.",
	L"Title terlalu panjang.",
	L"Surat terlalu panjang.",
	L"User yang diundang belum bisa masuk dungeon sekarang.",
	L"Skill sealing dibatalkan.",
	L"Gagal membuka skill yang disegel.",
	L"Class karakter tidak dapat membeli item ini.",
	L"Skill yang disegel telah dibuka.",
	L"Item dengan fungsi sama telah ada di inventori.",
	L"Experience ID user tidak dapat diundang ke Henir's Time and Space dungeon party.",
	L"Metode otorisasi salah.",
	L"Item yang disimpan di bank tidak dapat dipecahkan.",
	L"Soket tidak dapat ditambahkan ke item yang disimpan di bank.",
	L"Fungsi ini tidak dapat digunakan di bank.",
	L"Kamu tidak dapat menambah slot di bank.",
	L"Kamu hanya dapat membeli 1 tiket keanggotaan bank.",
	L"Tiket keanggotaan bank tidak dapat dibeli.",
	L"Ketika kamu memiliki Luriel's Credit Guarantee di inventori, kamu tidak dapat membeli platinum membership card. Harap gunakan Luriel's Credit Guarantee.",
	L"Password tidak ada.",
	L"Kamu tidak dapat mengganti password.",
	L"ID dan password salah.",
	L"Terdapat system error ketika proses otorisasi.",
	L"Item tidak ada.",
	L"Kamu tidak dapat memperoleh informasi item.",
	L"Tipe item ini tidak dapat disegel.",
	L"Item yang terpakai tidak disegel.",
	L"Item telah disegel.",
	L"Item yang belum diperbaiki tidak dapat disegel.",
	L"Time-limited item tidak dapat disegel.",
	L"Item rusak tidak dapat disegel.",
	L"Item yang dapat ditransaksi tidak dapat disegel.",
	L"Tidak dapat segel karena jumlah segel yang diperbolehkan telah terlampaui.",
	L"Informasi error pada spellbox yang dipilih.",
	L"Sealing gagal.",
	L"Spellbox lebih banyak diperlukan untuk menyegel item.",
	L"Segel item yang diinstal tidak dapat dibatalkan.",
	L"Sealing item ini telah dibatalkan.",
	L"Pembatalan sealing item gagal.",
	L"Update informasi sealing ke DB gagal.",
	L"Item sealing tidak dapat dienhance",
	L"Sistem error ketika memeriksa cash point.",
	L"Aktifitas terbatas dengan account ini.",
	L"Account tidak ada.",
	L"Account salah.",
	L"Nomor kupon salah.",
	L"Kupon sudah digunakan.",
	L"Tipe kupon ini hanya bisa digunakan sekali.",
	L"Kupon sudah kadaluarsa.",
	L"Sistem error ketika menggunakan kupon.",
	L"Point tidak cukup.",
	L"Sistem error ketika membeli item.",
	L"Level 15 ke atas bisa membeli.",
	L"Kamu sedang dalam Guild.",
	L"Nama Guild sudah ada.",
	L"Tidak ada item Guild establisment di dalam inventori.",
	L"Terdapat masalah dengan informasi item Guild establishment.",
	L"Guild establishment gagal.",
	L"Pendaftaran Guild master gagal.",
	L"Informasi Guild tidak dapat ditemukan.",
	L"Kamu tidak dapat mengundang Guild.",
	L"User ini sedang offline.",
	L"Experience ID user tdak dapat diundang ke dalam Guild.",
	L"User ini telah diundang ke dalam Guild.",
	L"Guild lain telah mengundang user.",
	L"Kamu tidak bisa mengundang anggota Guild.",
	L"Jumlah anggota Guild tidak dapat ditingkatkan. Untuk meningkatkan jumlah anggota gGuild, kamu butuh 'item penambah jumlah anggota Guild'.",
	L"User yang diundang sudah merupakan anggota guild.",
	L"User sedang dalam Guild.",
	L"Daftar anggota Guild tidak dapat diperoleh.",
	L"Informasi item yang diperoleh melalui kupon salah.",
	L"User menolak undangan Guild.",
	L"Gagal bergabung ke Guild.",
	L"Kamu tidak tergabung dalam Guild.",
	L"Kamu tidak bisa mengganti ranking anggota Guild.",
	L"Mengganti ranking anggota Guild gagal.",
	L"Hanya Guild Masters dan Guild Administrators bisa mengganti pesan Guild.",
	L"Kamu gagal mengganti pesan Guild.",
	L"Kamu tidak bisa memecat anggota Guild.",
	L"Guild Master tidak diijinkan keluar dari Guild.",
	L"Kamu gagal keluar dari Guild.",
	L"Jumlah kata pesan untuk salam anggota Guild terbatas.",
	L"Kamu tidak bisa mengganti salam anggota Guild.",
	L"Kamu gagal mengganti salam anggota Guild.",
	L"Hanya Guild Master yang bisa membubarkan Guild.",
	L"Tidak bisa membubarkan Guild karena masih ada anggota Guild.",
	L"Membubarkan Guild gagal.",
	L"Level telah diganti.",
	L"Tidak bisa menunjuk Guild Administrator lagi.   jika kamu ingin menunjuk Guild Administrator baru,  kamu perlu mengatur level Guild Administrator.",
	L"Guild Master tidak dapat menghapus karakter. Transfer ranking Guild Master ke anggota Guild lain.",
	L"Kamu tidak mengubah level sendiri.",
	L"Kamu sudah tergabung dalam Guild dan tidak dapat membeli Guild Establishment Approval.",
	L"Terdapat Guild Establishment Approval di inventori.",
	L"Kamu gagal menambah jumlah anggota Guild.",
	L"Hanya Guild Master dan Guild Administrator yang bisa menggunakan Guild Member Expansion Card.",
	L"Jumlah anggota Guild tidak dapat ditingkatkan lagi.",
	L"Fungsi tidak dapat digunakan ketika transaksi.",
	L"Kamu tidak dapat pindah ke area ini.",
	L"Nama Guild tidak dapat digunakan selama 7 hari.",
	L"Ini adalah maksimum Guild experience point yang dapat diperoleh.",
	L"Kamu perlu melakukan quest yang berkaitan untuk membeli item ini.",
	L"Nama Guild terlalu panjang.",
	L"Biaya untuk menghapus attribute tidak cukup.",
	L"Level 35 ke atas bisa membeli.",
	L"Complete the game and upgrade with the BeanFun account. The BeanFun website is tw.beanfun.gamania.com.",
	L"This is a BeanFun account and has been upgraded. Log in to the BeanFun program.",
	L"Kamu tidak dapat membeli item lagi. Taruh item di temporary box ke inventori.",
	L"Hanya Guild Master yang bisa investasi GSP.",
	L"Skill Guild sudah ditingkatkan ke level tertinggi.",
	L"Informasi skill Guild tidak ada.",
	L"Guild skill ini belum dipelajari hingga level tertinggi.",
	L"Kamu belum investasi GSP dengan cukup.",
	L"GSP tidak cukup.",
	L"Mempelajari skill Guild gagal.",
	L"Hanya Guild Master yang bisa mengembalikan skill Guild.",
	L"Item Pipe of Oblivion tidak ada di inventori.",
	L"Skill Guild tidak bisa dikembalikan.",
	L"Pengembalian skill Guild gagal.",
	L"Hanya Guild Master yang bisa reset skill Guild.",
	L"Item Bugle of Oblivion tidak ada di inventori.",
	L"Reset skill Guild tidak bisa dilakukan.",
	L"Reset skill Guild gagal.",
	L"Hanya Guild Master yang bisa menggunakan ini.",
	L"Akuisisiskill Guild  time-limited gagal.",
	L"Kamu sedang menggunakan item ini.",
	L"Kamu belum bisa menerima hadiah.",
	L"Inventori tidak cukup dan kamu tidak bisa menerima hadiah event.",
	L"Kamu gagal menerima hadiah event.",
	L"Ini bukan item mode.",
	L"Item drop masih ada.",
	L"Gagal memperoleh item yang drop dari pertandingan.",
	L"Class ini dapat digunakan setelah menggunakan kartu karakter.",
	L"User ini tidak diizinkan ikut event.",
	L"Hanya Guild Master atau Guild Administrator yang bisa menggunakan fungsi ini.",
	L"Item hanya untuk Warnet tidak dapat menggunakan fungsi ini.",
	L"Iklan Guild tidak ada.",
	L"Panjang surat terlalu panjang.",
	L"Periode pendaftaran iklan salah.",
	L"Biaya pendaftaran iklan kurang dan kamu tidak dapat mendaftarkan iklan.",
	L"Kamu sudah memiliki iklan.",
	L"Kamu gagal mendaftarkan iklan Guild.",
	L"Terlalu banyak pendaftar dan kamu tidak bisa ikut.",
	L"Pendaftaran untuk masuk Guild gagal.",
	L"Kamu sudah mendaftar untuk bergabung.",
	L"Tidak ada informasi pendaftaran bergabung.",
	L"Gagal bergabung ke Guild.",
	L"Penghapusan aplikasi bergabung ke Guild gagal.",
	L"Kamu tidak dapat daftar bergabung iklan yang periode kadaluarsa.",
	L"Kamu tidak bisa melihat daftar aplikasi bergabung ke Guild.",
	L"Kamu belum membuka kunci.",
	L"Account ini sudah diblok.",
	L"Quests only for PC Gaming Centers can only be accepted at PC Gaming Centers.",
	L"Quests only for PC Gaming Centers can only be completed at PC Gaming Centers.",
	L"Karena terdeteksi program ilegal.",
	L"Kamu tidak dapat mengisi aplikasi ketika pilihan block Guild invitation tidak diperiksa.  Harap uncheck Options Menu.",
	L"Terlalu banyak pendaftar dan kamu tidak bisa ikut.",
	L"Saat ini tidak ada quest event.",
	L"Batas pembelian item terlampaui",
	L"It has been set up not to use Kill3rcombo cash. Change the setup from the Kill3rcombo Cash security setup menu in My Information, Kill3rcombo Portal.",
	L"Kehadiran hari ini selesai.",
	L"DB query call untuk meningkatkan web point gagal.",
	L"Cek kehadiran gagal.",
	L"Processed as a withdrawal from a party due to overlapped party subscriptions.",
	L"Kamu dalam status menunggu bergabung dalam party.",
	L"Menghapus daftar item.",
	L"Menambah daftar item.",
	L"Merevisi daftar item.",
	L"Pemilihan kategori salah.",
	L"Item tidak ada hasil pencarian atau sudah tidak ada.",
	L"Stamina user yang diundang tidak cukup.",
	L"Coba cari lagi nanti.",
	L"Toko selesai penjualan.",
	L"User ini tidak dapat membeli.",
	L"Pembuatan room gagal.",
	L"Item tidak ada.",
	L"Halaman tidak ada.",
	L"Memo sudah didaftarkan.",
	L"Pendaftaran memo gagal.",
	L"Kamu tidak memiliki Note of Techniques.",
	L"Halaman ekspansi dari Note of Techniques gagal.",
	L"Kamu sudah memiliki Note of Techniques.",
	L"Item ini tersedia untuk pembelian untuk level 20 ke atas.",
	L"Kamu tidak dapat transaksi dengan user di server berbeda.",
	L"Kamu tidak dapat menggunakan Personal Shops di server berbeda.",
	L"Kamu tidak dapat mengudnang user bermain di server berbeda dengan Guild.",
	L"Kamu tidak dapat mendaftarkan user di server berbeda.",
	L"Kamu tidak dapat mendaftarkan user di server berbeda sebagai teman.",
	L"Kamut tidak dapat bermain mode Henir's Time and Space Challenge dengan user di server berbeda.",
	L"Account ini sudah diblok.",
	L"[홍콩] 해당 직업으로는 전직을 할 수 없습니다.",
	L"Jumlah item untuk dibuang salah.",
	L"Gagal membuat pet.",
	L"Nama telah terpakai.",
	L"Kamu dapat menggunakan maksimum 12 huruf untuk nama pet.",
	L"Pet telah dibuat saat ini.",
	L"Gagal summon pet.",
	L"Tidak dapat summon pet di area ini.",
	L"Kamu tidak memiliki pet untuk di summon.",
	L"Pet sudah disummon.",
	L"Gagal mengembalikan pet.",
	L"Gagal memerintah pet.",
	L"Petmu sudah kenyang dan tidak dapat makan lagi.",
	L"Gagal memberi makan pet.",
	L"Petmu dalam keadaan 'Special' dan hanya makan El Tree Fruits atau Seeds.",
	L"Data telah tersimpan.",
	L"Pet tidak dapat berkembang lagi.",
	L"Pet dapat berkembang setelah mencapai affinity 100%.",
	L"Pet dapat hanya mengkonsumsi equipment dan pet food item.",
	L"Level equipment terlalu rendah.",
	L"Passwords tidak cocok.",
	L"Account tidak terdaftar.",
	L"Account belum divalidasi.",
	L"Item itu tidak dapat digunakan sebagai makanan pet.",
	L"User sedang sibuk saat ini.",
	L"[CN] 매트릭스 카드가 바인딩되어 있습니다.",
	L"[CN] OPT번호가 틀렸습니다. 다시 입력해 주십시요.",
	L"[CN] 보안카드 번호를 잘못 입력 하셨습니다. 다시 입력해 주십시요.",
	L"[CN] 계정 정보가 수정 과정에 있습니다. 의문 사항이 있으시면 서비스 센터로 연락 바랍니다.",
	L"[CN] 자이언트 패스워드 보안이 바인딩 해제 상태에 있습니다. 의문사항이 있으시면 서비스 센터로 연락 바랍니다.",
	L"GM Kieran",
	L"GM Lielle",
	L"[Tidak ada informasi pengirim]",
	L"지금은 청약철회 기능을 이용 하실 수 없습니다.",
	L"The quantity of the item that you requested is greater than that of available refunds; failed to refund the item.",
	L"The item has passed 7 days of its refund period from the date of purchase; cannot refund the item.",
	L"Paket telah terbuka dan telah terpakai sebagian; tidak dapat mengembalikan paket.",
	L"Harga item 0; tidak dapat mengembalikan item.",
	L"Duplicate RequestID.",
	L"Tidak ada informasi pembelian seperti itu.",
	L"Tidak ada informasi produk seperti itu.",
	L"청약철회가 불가능한 상품 입니다.",
	L"선물 받은 상품이어서 청약철회가 불가능 합니다.",
	L"Batas waktu item tidak dapat dihilangkan.",
	L"Item tidak dapat dijual dan dibuang.",
	L"Kamu tidak dapat membeli lagi karena sudah melebihi 1 EL coins event dari batas maksimal.",
	L"Level kamu terlalu rendah untuk menggunakan item ini.",
	L"Kamu tidak dapat menggunakan fluorite ore ketika level enchantment sudah 11 atau lebih.",
	L"Kamu tidak dapat menerima quest lagi karena kamu telah menerima hadiah.",
	L"Kamu tidak dapat melapor quest karena tidak memiliki cukup ruang di inventory.",
	L"Telah gagal menggunakan magic stone.",
	L"Kamu tidak dapat menggunakan fungsi ini.  Sorry untuk ketidak nyamanan ini.",
	L"IP telah diblok.",
	L"Chatting telah diblok.",
	L"Trade telah diblok.",
	L"tidak ditemukan; Kamu tidak bisa memulai game.",
	L"Terjadi konflik dengan program lain.",
	L"[CN] 해당 유저가 존재하지 않습니다.",
	L"[CN] 잔액이 부족합니다.",
	L"[CN] 존재하지 않는 아이템 카드입니다.",
	L"[CN] 동일한 쿠폰은 중복사용 할 수 없습니다.",
	L"[CN] 이미 사용한 아이템 카드입니다.",
	L"[CN] 잘못된 전용 카드 ID입니다.",
	L"[CN] 잘못된 전용 카드 패스워드입니다.",
	L"[CN] DB 에러",
	L"Kamu tidak bisa mengirim mail karena trade tidak diperbolehkan.",
	L"Kamu tidak dapat membuka toko karena trade tidak diperbolehkan….",
	L"Kamu tidak dapat mengunjungi toko karena trade tidak diperbolehkan....",
	L"Kamu tidak dapat mencari toko karena trade tidak diperbolehkan....",
	L"Kamu tidak dapat memasuki market square karena trade tidak diperbolehkan....",
	L"캐시 인벤토리에 아이템이 존재합니다.(HK 이벤트에 사용)",
	L"파티인원이 2인 이상이어야 합니다.",
	L"던전이 닫혀 입장 할 수 없습니다.",
	L"다른 서버 유저와 월드 미션 던전을 플레이 할 수 없습니다.",
	L"우편 감시 로그에 중복된 값이 있습니다.",
	L"거래 감시 로그에 중복된 값이 있습니다.",
	L"우편 감시 로그 추가 실패!",
	L"거래 감시로그 추가 실패!",
	L"아직 판매중인 물품이 있어 대리상인을 종료할 수 없습니다!",
	L"대리상인이 개설되어 있지 않습니다.",
	L"Kamu tidak dapat berhenti berjualan.",
	L"Toko ini tidak lagi menjual barang. Kamu tidak dapat membeli dari toko ini.",
	L"대리 판매인이 물품을 판매 하고 있어 상점을 개설 할 수 없습니다.",
	L"(TW)비정상적인 방법으로 게임을 플레이 하고 있습니다. 계속 하시면 거래 정지 됩니다.",
	L"대리상인을 종료할 수 없습니다.",
	L"User tidak terdaftar.",
	L"Ini sudah dibatalkan.",
	L"Nomor kupon salah.",
	L"Kupon ini tidak dapat dibatalkan.",
	L"The used user information and cancel the requested user information does not match.",
	L"Cash dari kupon sudah pernah digunakan. Sehingga tidak dapat dibatalkan.",
	L"Harga yang diminta haru sama dengan harga item tersebut.",
	L"Balance kamu tidak cukup.",
	L"Billing account tidak cukup.",
	L"Kamu tidak dapat register lagi hari ini.",
	L"Kupon ini tidak dapat memverifikasi informasi.",
	L"Kupon telah dipakai.",
	L"Penggunaan kupon dihentikan.",
	L"Kupon telah expired.",
	L"Kupon tidak tersedia di dalam game.",
	L"Hanya diberikan kepada pelanggan yang menggunakan kupon, kamu tidak diizinkan menggunakan ini.",
	L"Buying is a restricted user.",
	L"Pembatasan penggunaan terhadap payment service.",
	L"Pembatasan penggunaan terhadap purchase service.",
	L"User tidak dapat di verifikasi..",
	L"Registration telah melebihi jumlah kupon yang ada.",
	L"We're sorry but Elsword Online is not available in your area. Service is restricted to North America and Oceania.",
	L"Kode kupon telah digunakan.",
	L"Kamu tidak dapat membeli item saat ini. Acoount kamu telah mencapai batas pembelian item harian.",
	L"Kamu tidak dapat memasukan Pin Code yang salah sebanyak dua kali.",
	L"대리 판매인에게 물품을 맡기려면 대리판매 보증서가 있어야 합니다.",
	L"Pin Code yang kamu masukan salah.",
	L"Kamu telah gagal memasukan Pin Code yang benar. Tekan tombol [Ok] untuk keluar. Silahkan kunjungi website kami untuk pengaktifan ulang.",
	L"Gagal membuat Security Pin Code",
	L"Pin Code tidak cocok.",
	L"Nama toko terlalu panjang.",
	L"대리상점 판매가 종료된 상태입니다. 물품받기 후 새롭게 상점 개설이 가능합니다.",
	L"대리 판매인이 물품을 판매 중입니다. 물품이나 판매수익을 받기 위해서는 대리 판매인 상점을 종료 해야 합니다.",
	L"대리 상인 보증서 DB쿼리 실패",
	L"대리 상인을 생성을 실패하였습니다.",
	L"대리 상인 물품 등록을 실패 하였습니다.",
	L"대리 상점 개설자만 이용할 수 있습니다.",
	L"대리 상점 물품 받기가 실패하였습니다.",
	L"Pembelian sedang berlangsung sehingga barang tidak dapat diterima.",
	L"Kamu akan mendapatkan barangnya sesaat lagi.",
	L"Barang gagal untuk dijual karena kamu memiliki jumlah E",
	L"Tidak dapat bertransaksi karena inventori penuh.",
	L"Tidak ada barang tersimpan.",
	L"Kamu tidak dapat membawa barang yang sama sekaligus.",
	L"Kamu belum mencapai batas level. Kamu tidak dapat join party.",
	L"Sisa waktu masih berjalan dari Deputy Merchant sehingga barang tidak dapat dibeli.",
	L"Barang yang belum diklaim tidak dapat dibuka. Silahkan periksa barang kamu.",
	L"대리 판매인의 판매 물품이 모두 판매되어 상점을 종료합니다.",
	L"Kamu sudah menerima hadiah quest. Kamu tidak menerima quest ini.",
	L"Tidak dapat menerima quest jika inventori penuh.",
	L"Batas minimum level untuk party tidak dapat diubah.",
	L"대리상점의 모든 물품이 판매될 경우 소지 한도 금액을 초과하게 되기 때문에 상점을 개설할 수 없습니다.",
	L"(TW/HK)강화 보조 아이템 사용 불가",
	L"(TW/HK)강화 보조 아이템이 없습니다.",
	L"(TW/HK)파괴 방지 아이템 사용 불가",
	L"(TW/HK)파괴 방지 아이템이 없습니다.",
	L"Kamu belum mencapai batas level. Silahkan memilih dungeon lain atau matikan batas level.",
	L"Kamu belum mencapai batas level. Batas Level tidak dapat diaktifkan.",
	L"Kamu belum mencapai batas level. Kamu tidak bisa mengundang orang.",
	L"Kamu belum mencapai batas level. Silahkan memilih dungeon lain atau matikan batas level.",
	L"Kamu tidak dapat menggunakan nomor yang sama.",
	L"Kamu tidak dapat menggunakan nomor seri sebagai kode pinmu.",
	L"Security Pin Code belum dihilangkan.",
	L"Kamu tidak menggunakan Security Pin Code.",
	L"Kamu tidak dapat menggunakan Pin Code lama sebagai Pin Code baru.",
	L"Kamu telah berusaha untuk membeli item yang tidak terdaftar pada item di toko.",
	L"Arena match telah selesai.",
	L"Player telah meninggalkan Arena match.",
	L"Arena match telah dibatalkan.",
	L"Salah satu anggota party tidak bergabung ke auto matching Arena, sehingga dibatalkan.",
	L"Matching berhasil dan kamu akan keluar dari waiting list.",
	L"Player membatalkan match, akan dikeluarkan dari waiting list.",
	L"Pertandingan telah dibatalkan karena player keluar dari party.",
	L"Arena Match sedang berlangsung.",
	L"Informasi player di dalam Arena tidak tersedia.",
	L"Informasi tentang tipe Arena tidak tersedia.",
	L"Kamu perlu mencapai minimal level 10 untuk bergabung ke Official Arena Match.",
	L"Kamu perlu mencapai minimal level 10 untuk Arena Match.",
	L"Tipe Party telah diubah.",
	L"Party tidak dapat diubah jika sudah ada 4 player.",
	L"Gagal mengubah tipe party.",
	L"Arena party match tidak mungkin untuk diubah jika ada player dibawah level 10 di dalam party.",
	L"Kamu tidak dapat berpartisipasi di Official Arena Match jika ada player dibawah level 10 di dalam party.",
	L"Kamu hanya dapat mengundang player diatas level 10.",
	L"Hanya level 10 atau diatasnya yang bisa menjadi party leader.",
	L"[CN]전환이 정상적으로 이루어지지 않았습니다. 다시 시도하여 주시기 바랍니다.",
	L"Kamu tidak dapat menekan tombol sort berkali-kali.",
	L"Kamu belum tergabung kedalam Arena Match.",
	L"Sudah masuk kedalam match room.",
	L"Ini bukan Arena room.",
	L"Permintaan untuk join match gagal.",
	L"Tidak dapat join party ketika sedang menunggu pertandingan.",
	L"Player sedang menunggu giliran bertanding dan tidak dapat di undang ke dalam party.",
	L"Kamu belum mencapai rank yang diperlukan untuk membeli item itu.",
	L"Tidak dapat join party lain ketika sedang menunggu bertanding.",
	L"Tidak dapat join party lain ketika sedang menunggu bertanding.",
	L"Tidak dapat mengganti channels ketika menunggu pertandingan.",
	L"머신 ID 인증 실패.",
	L"머신ID로 인한 이중접속 감지.",
	L"Kamu tidak dapat memilih quest random hari ini.",
	L"OUR TERMS OF USE HAVE CHANGED Klik 'OK' dan login ulang ke dalam game.",
	L"SP 출력 값에 문제가 있습니다.",
	L"SP 입력 값에 문제가 있습니다. ",
	L"목적 DB 이름이 잘못 되었습니다.",
	L"데이터 검증 실패",
	L"Tidak dapat membeli Item Mall karena sedang maintenance.",
	L"Kamu tidak dapat bermain game dari 00.00 s.d 06.00",
	L"Tidak dapat mengambil informasi.",
	L"Transaksi dibatasi untuk 5 hari pertama untuk setiap karakter baru.",
	L"Mailing dibatasi untuk 5 hari pertama untuk setiap karakter baru.",
	L"[CN]사용실패!",
	L"Meninggalkan ruang untuk memasuki medan pertempuran.",
	L"Kamu harus lv.15 untuk dapat membeli.",
	L"ServerSN과 MachineID가 매칭되지 않는다.",
	L"MachineID와 ServerSN이 매칭되지 않는다.",
	L"DB에 존재 하지 않는 ServerSN이 있다.",
	L"DB에 존재 하지 않는 MachineID가 있다.",
	L"클라가 준 SN은 없는데 DB에는 MachineID에 해당하는 SN 존재.",
	L"제재된 SN입니다.",
	L"접속 제한된 ServerSN 입니다.",
	L"Ini bukan akun test server",
	L"파티 병합을 위한 자동 파티 이탈입니다.",
	L"Skill Slot Expansion sedang digunakan. Kamu tidak dapat menggunakan Skill Slot Expansion (batas waktu).",
	L"Skill Slot Expansion dalam penggunaan tak terbatas. Kamu tidak dapat menggunakan ini.",
	L"[CN] 귀하의 계정이 정지되었습니다. 고객 센터에 문의하여 주십시요.",
	L"[CN] 비밀번호를 잘못 입력 하셨습니다. 다시 입력해 주십시요.",
	L"[CN] 해당 계정은 존재하지 않습니다.",
	L"[CN] 불명확인 시스템 에러",
	L"Cube tidak dapat dibuka karena Blessed Medal sudah ada di inventori.",
	L"Tidak dapat membeli karena pembatasan level.",
	L"Tidak dapat membawa item ini karena kamu belum mencapai level persyaratan.",
	L"Membuat party gagal.",
	L"Jumlah party kurang.",
	L"Undang party tertunda.",
	L"[JP] 판매하지 않는 상품입니다.",
	L"[JP] 한게임 빌링 오류로 구매할수 없습니다.",
	L"Item yang diberikan melalui bank tidak dapat di kirim melalui mail.",
	L"공유은행 아이템은 거래 할 수 없습니다.",
	L"Tidak dapat membeli karena kamu belum mencapai persyaratan Lv. 21.",
	L"[EU] 길드에 소속된 캐릭터만 교환 할 수 있습니다.",
	L"Tidak dapat membeli karena kamu belum mencapai persyaratan Lv. 31.",
	L"Kamu hanya dapat berpartisipasi di PVP party dengan full patch.",
	L"블랙리스트에 등록된 DLL 이 존재합니다.",
	L"Silahkan coba lagi nanti atau reconnect ulang karaktermu.",
	L"Kamu telah menjawab Quiz Problem untuk hari ini.",
	L"Ini adalah error yang tidak diketahui. (Quiz)",
	L"Tidak dapat membeli item karena adanya pembatasan pembelian.",
	L"Tidak dapat menggunakan item karena adanya pembatasan pembelian.",
	L"Quick Slot tidak dapat diperpanjang.",
	L"Karakter sudah dihapus atau akan dihapus.",
	L"Tidak bisa hapus karakter sekarang.",
	L"Hapus karakter tidak memungkinkan saat ini.",
	L"Hapus karakter tidak memungkinkan.",
	L"Karakter ini tidak dapat dikembalikan.",
	L"Karakter ini tidak dapat dikembalikan sekarang.",
	L"Kamu tidak dapat menekan tombol sort berulang-ulang.",
	L"Karakter tidak dapat ditemukan.",
	L"Tidak dapat menghapus karakter.",
	L"Ubah nama pet gagal.",
	L"Ubah nama pet sedang berlangsung.",
	L"Penggunaan game akan diblok.",
	L"Kamu bukan ketua Guild.",
	L"Informasi item tidak jelas.",
	L"Ubah nama Guild gagal!",
	L"Pengerjaan sedang berlangsung.",
	L"존재하지 않는 아이템을 삭제 시도하였습니다.",
	L"[TW] 낚시대를 장착하지 않았거나, 미끼가 없습니다.",
	L"[TW] 이미 사용중인 낚시터 입니다.",
	L"[TW] 낚시터에 문제가 있습니다. 다른 곳을 이용해 주세요.",
	L"[TW] 낚시 보상에 문제가 발생했습니다.",
	L"Gagal menghapus item.",
	L"Hanya ketua Guild yang dapat mengubah nama Guild.",
	L"Hanya dapat digunakan jika memiliki Pet. Jika tidak memilikinya, kamu harus membatalkan kurang dari seminggu.",
	L"Waktu pembuatan tertunda dan pengerjaan telah gagal. Silahkan coba lagi.",
	L"User telah membatalkan untuk ubah nama Guild.",
	L"Tidak dapat berpindah sekarang.",
	L"Kamu sudah berpindah ke tempat kamu.",
	L"Area sudah penuh dan kamu tidak dapat masuk.",
	L"Tidak ada tempat lagi karena banyak party lain.",
	L"Gagal berpindah ke tempat lain.",
	L"Informasi yang diberikan salah.",
	L"Terjadi error saat memasuki area.",
	L"Gagal menemukan info user di areamu.",
	L"Auto Party dungeon play telah gagal.",
	L"Memulai bermain dungeon gagal.",
	L"Tidak dapat memulai Auto Party sekarang.",
	L"Tidak dapat memulai Auto Party dengan resting buff. Tunggu sampai buff habis.",
	L"Sudah memulai Auto Party.",
	L"Auto Party gagal karena kesalahan informasi.",
	L"Gagal menjadi Auto Party wait list.",
	L"Tidak dalam Auto Party status.",
	L"Tidak dapat mulai dengan party 1 orang.",
	L"Salah satu anggota party telah membatalkan.",
	L"Game tidak dapat dimulai karena party tidak merespon dalam waktu yang diberikan.",
	L"Sudah dalam party.",
	L"Yang diundang sedang menunggu untuk gabung party lain.",
	L"Tidak dalam party.",
	L"Tidak dapat memakai Elixir dan Potion dalam waktu yang bersamaan.",
	L"ServerSN이 중복 접속 중이다.",
	L"ServerSN 존재하지 않는데 삭제하려한다.",
	L"Fungsi ini sementara tidak dapat digunakan.",
	L"넥슨 통합 맵버쉽 계정 인증 상향 조정 필요.",
	L"Body's Limit buff aktif. Potion tidak dapat digunakan.",
	L"Kamu tidak memenuhi batas level. Permainan tidak dapat dimulai.",
	L"Kamu tidak memenuhi syarat untuk menjadi El Recruit.",
	L"El Recruiter telah merekrut 3 El Recruits.",
	L"User tidak memenuhi syarat untuk El Recruiter.",
	L"Item ini tidak dapat digunakan.",
	L"Item Character slot expansion tidak dapat digunakan. Penambahan 8 slot diperbolehkan.",
	L"Auto Party batal karena sudah memulai dungeon.",
	L"Salah satu anggota Auto Party belum setuju untuk memulai game.",
	L"Auto Party telah berakhir karena request.",
	L"Auto Party dibatalkan karena anggota party tidak setuju untuk memulai game.",
	L"Auto Party sukses dan keluar dari wait list.",
	L"Keluar dari waiting list setelah membatalkan Auto Party.",
	L"Auto Party telah dibatalkan karena anggota party keluar.",
	L"Tidak memenuhi batas level. Game tidak dapat dimulai.",
	L"Gagal untuk save keyboard setting.",
	L"Gagal untuk save semua keyboard setting.",
	L"Gagal untuk memuat keyboard setting",
	L"Gagal untuk memuat semua keyboard setting",
	L"Perbedaan waktu telah menghambat mulainya dungeon.",
	L"Gagal menghapus karakter dari list El Recruiter.",
	L"Kamu belum mengatur security question dan answer.",
	L"Anggota member yang tidak sama.",
	L"Gagal untuk save chat option.",
	L"Gagal untuk membuat chat option.",
	L"Gagal untuk membuat beberapa chat option.",
	L"Test",
	L"Batas kredit bulanan atau Kartu Prabayar kamu tidak bisa digunakan.",
	L"나이정보를 확인 중 오류가 발생하였습니다. 잠시 후 다시 로그인하여 주십시오.",
	L"실명인증을 받지 않은 회원이거나 넥슨닷컴 회원이 아닙니다.",
	L"셧다운제에 의하여 게임이용이 차단 됩니다.",
	L"선택적 셧다운제의 의해 게임이용이 차단됩니다.",
	L"통합 맴버십 ATL 2.0 유저가 아니므로 게임이용이 차단 됩니다.",
	L"Item tidak dapat dibuka sekarang.",
	L"Waktu buka item sudah expired.",
	L"ServiceID가 잘못 되었습니다.",
	L"Informasi jumlah NC tidak dapat ditemukan.",
	L"[TW]ServiceCode 가 올바르지 않습니다.",
	L"[TW]정의되지 않은 애러 발생했습니다.",
	L"[TW]시간형식이 잘못되었습니다.",
	L"[TW]Type정보가 잘못되었습니다.",
	L"[TW]이전 상품 구매가 진행 중입니다.",
	L"Quest level terlalu tinggi untuk diambil.",
	L"파라매터 갯수 입력이 잘못되었습니다.",
	L"[JP] 합성제가 없습니다.",
	L"[JP] 없는 아바타를 합성하려고 했습니다.",
	L"[US]월 선물 가능 횟수를 초과하였습니다.",
	L"[CN]1인 플레이 던전 입니다.",
	L"[CN]던전 오픈 시간이 아닙니다.",
	L"Memilih karakter gagal.",
	L"Kamu mencoba untuk mengirim mail biasa ke karakter kamu.",
	L"Informasi pet salah.",
	L"Hanya dapat digunakan jika pet di summon.",
	L"Hanya dapat digunakan jika pet dalam keadaan teen.",
	L"Hanya dapat digunakan jika pet dalam keadaan infant.",
	L"Pengaturan nomer tag tidak dikenal.",
	L"Harus ada minimal 2 anggota untuk memulai.",
	L"Match item  tidak dapat digunakan.",
	L"Tidak ada lagi item pertandingan tersisa.",
	L"Tidak ada lagi item pertandingan tersisa.",
	L"Tidak ada lagi item pertandingan tersisa.",
	L"Keluar dari battlefield untuk masuk ke permainan dungeon.",
	L"Keluar untuk masuk ke pertandingan.",
	L"Keluar karena perubahan channel.",
	L"Keluar untuk mencari anggota party yang sama.",
	L"Keluar untuk menggunakan item teleport.",
	L"Keluar untuk berhenti ",
	L"Tidak bisa undang pemain ketika di dungeon.",
	L"Already in a party, cannot invite.",
	L"Tidak bisa undang pemain ketika sedang dalam pertandingan.",
	L"Tidak bisa undang pemain ketika sedang di dalam dungeon.",
	L"Tidak dapat menemukan pemain yang berada di server yang sama.",
	L"Kamu tidak dapat lagi mengundang anggota party.",
	L"Kamu tidak dapat mengundang pemain ketika dalam pertandingan.",
	L"Level kamu terlalu rendah untuk masuk field.",
	L"Kamu tidak menyelesaikan dungeon, yang dibutuhkan untuk masuk ke field.",
	L"Quest aktif hanya bisa diambil hingga 30.",
	L"ED yang kamu miliki tidak mencukupi.",
	L"Tidak bisa undang pemain yang sedang mencari auto party.",
	L"Tidak bisa undang pemain ketika kamu sedang mencari auto party.",
	L"Tidak bisa undang pemain yang sedang di lobby pertandingan.",
	L"Tidak bisa undang anggota party ketika training.",
	L"Tidak bisa undang pemain ketika berada di pertandingan.",
	L"Tidak bisa undang party jika kamu sudah meminta untuk cari auto party.",
	L"Tidak bisa undang pemain setelah kamu meminta untuk cari pertandingan.",
	L"Party ini ditugaskan untuk field yang sedang berlangsung",
	L"Tidak bisa undang party karena telah diundang untuk party",
	L"이 이벤트 쿠폰은 계정당 한 번만 받을 수 있습니다.",
	L"ID atau Password Salah",
	L"Nama tidak tersedia",
	L"ID grup server salah",
	L"[TW] 공유 은행 처리중인 아이템 입니다. 은행을 닫고 다시 시도해 주세요.",
	L"Kamu tidak dapat menggunakan Competition Channel jika kamu tidak memiliki tiket turnamen.",
	L"Kamu butuh item dan ED untuk membuka cube.",
	L"Tidak dapat membuka cube karena ED tidak mencukupi.",
	L"Mulai dengan member yang ada saat ini.",
	L"길드 생성은 하루에 한 번 가능합니다.",
	L"ID Minimal 4~Maksimal 20 Digit",
	L"Password Minimal 6~Maksimal 15 Digit",
	L"Waktu Event telah berakhir",
	L"아라 캐릭터로는 가져올 수 없는 아이템입니다.",
	L"금일 선물하기 제한을 초과하였습니다.",
	L"아쉽게도 교환 수향이 모두 소진 되었습니다. 1월 17일 점검 이후 교환 수량이 추가 됩니다.",
	L"Login Gagal. Tidak dapat terhubung dengan server.",
	L"Pet tidak tersedia",
	L"Fungsi ini telah digunakan.",
	L"Harus evolusi bayi atau lebih tinggi.",
	L"Menunggu permainan, tidak dapat mengganti Skill Slot.",
	L"Gagal menerima informasi monster karena saat ini tidak berada di field.",
	L"Gagal menerima informasi monster karena status field tidak tetap",
	L"Tidak bisa membeli ekspansi karakter. Ekspansi ini dimungkinkan sampai 30.",
	L"Kamu telah dikeluarkan oleh voter",
	L"Voter adalah bukan user biasa",
	L"Target voter adalah bukan user biasa.",
	L"빌링 서버가 초기화 되지 않았습니다.",
	L"상품 구매중 문제가 발생하였습니다.",
	L"구매 처리 중입니다. 잠시 후에 다시 시도해 주세요.",
	L"아직 처리 중인 작업이 있습니다. 잠시 후에 다시 시도 해주세요.",
	L"아직 빙고판이 완성되지 않았습니다.",
	L"일시적인 오류입니다. 잠시 후에 다시 시도 해주세요.",
	L"초대 메시지 등록 비용이 부족하므로 등록할 수 없습니다.",
	L"길드 초대 메시지 등록을 실패하였습니다.",
	L"길드구함 광고 등록을 실패하였습니다.",
	L"길드 초대 요청이 실패하였습니다.",
	L"이미 등록한 길드구함 광고가 있습니다.",
	L"이미 등록한 길드 초대 메시지가 있습니다.",
	L"길드구함 광고 등록 비용이 부족하므로 등록할 수 없습니다.",
	L"해당 길드가 최대 인원에 도달하여 가입하실 수 없습니다.",
	L"길드 초대 메시지가 존재하지 않습니다.",
	L"길드구함 광고가 존재하지 않습니다.",
	L"존재하지 않는 길드입니다.",
	L"초대 유저 목록을 볼 수 있는 권한이 없습니다.",
	L"Batasan waktu untuk memasuki dungeon sudah melampaui batas.",
	L"로컬 랭킹 유저 정보 초기화 오류",
	L"로컬 랭킹 유저 정보가 초기화 되어있지 않습니다.",
	L"프로필 정보는 20자까지만 입력 가능합니다.",
	L"로컬 랭킹 게시판 조회 오류",
	L"로컬 랭킹 유저 정보 쓰기 오류",
	L"다음 정보 수정은 익일 06시 이후에 가능합니다.",
	L"[TW]2인 플레이 던전입니다.",
	L"더 이상 추천 할 수 없는 유저입니다.",
	L"비밀번호가 잘 못 되었습니다",
	L"투니랜드 계정이거나, 계정 인증 이후 추천 가능 합니다.",
	L"동일 명의의 계정이므로 추천 할 수 없습니다.",
	L"추천을 실패 하였습니다.",
	L"PASSWORD 길이가 잘못되었습니다.",
	L"프로필 정보에 사용할 수 없는 단어가 들어있습니다.",
	L"영웅 대전 후보 리스트에 이미 존재하는 유닛입니다.",
	L"닉네임 길이가 잘못되었습니다.",
	L"영웅 대전 후보 리스트에 존재하지 않는 유닛입니다.",
	L"--/;/따옴표/쉼표/공백 등은 사용할 수 없습니다.",
	L"Kamu tidak memiliki Guide Book: Cara Menikmati Field 100%",
	L"Gagal merekam kedalam Chapter.",
	L"Sudah terekam didalam Guide Book.",
	L"해당 유닛은 존재하지 않습니다.",
	L"UNKNOWN",
	L"벌써 임자가 있습니다.",
	L"양다리는 안됩니다!",
	L"마을, 필드에서만 커플 신청을 할 수 있습니다.",
	L"접속중인 유저가 아닙니다.",
	L"인벤토리에 커플링이 존재하지 않습니다.",
	L"동성 캐릭터간의 커플은 한국에서는 안됩니다.",
	L"해당 서버군에 존재하지 않는 캐릭터 명입니다.",
	L"커플 신청 받는 유저의 정보가 이상합니다.",
	L"정상적인 캐릭터 명이 아닙니다.",
	L"알 수 없는 에러입니다.",
	L"존재하지 않는 유저입니다.",
	L"상대방이 실수로 커플링을 떨어트렸어요! 얼른 찾아오라고 말해주세요!",
	L"상대방이 수락했지만, 내 인벤토리에 커플링이 없어요! 인벤토리를 확인해 주세요!",
	L"인연 시스템 사용 아이템 삭제 실패",
	L"요청한 연인 정보의 캐릭터 정보가 이상합니다.",
	L"당신은 솔로입니다. 연인이 없어 드릴 정보가 없습니다.",
	L"선택한 옵션값이 이상합니다.",
	L"인벤토리에 결혼반지가 존재하지 않습니다.",
	L"선택한 옵션에 필요한 캐시 아이템이 인벤토리에 존재하지 않습니다.",
	L"결혼을 하기 위해서는 커플이 되고 72시간이 지나야 합니다.",
	L"결혼을 하려면 커플 상태여야 합니다.",
	L"무료 1차 전직 이벤트를 진행할 수 있는 상태가 아닙니다.",
	L"인벤토리에 결혼에 필요한 아이템이 존재하지 않습니다.",
	L"결혼하기 아이템 사용 실패 하였습니다.",
	L"결혼식에 초대하려는 인원 수보다 인벤토리에 있는 초대권 수가 부족합니다.",
	L"이벤트 기간이 종료되어 전직이 불가능합니다.",
	L"탈 것을 더 이상 등록 할 수 없습니다. \n사용하지 않는 탈 것을 놓아주세요!",
	L"탈 것 생성 아이템을 사용할 수 없습니다.",
	L"탈 것 정보를 가져오지 못했습니다.",
	L"탈 것을 생성할 수 있는 아이템이 아닙니다.",
	L"탈 것 생성에 실패 하였습니다.",
	L"이미 다른 탈 것이 소환되어 있습니다.",
	L"존재하지 않는 탈 것입니다.",
	L"해당 탈 것은 소환되어 있지 않습니다.",
	L"소환되어 있는 탈 것이 없습니다.",
	L"탈 것 놓아주기에 실패하였습니다.",
	L"소환되어 있는 탈 것은 놓아줄 수 없습니다.",
	L"탈 것을 소환할 수 없는 지역입니다.",
	L"보유 중인 탈 것이 없습니다.",
	L"존재하지 않는 아이템에 대한 정보를 요청하였습니다.",
	L"웨딩 관련 아이템이 아닙니다.",
	L"웨딩 관련 아이템의 정보가 없습니다.",
	L"예약된 결혼식장의 정보가 없습니다.",
	L"결혼식을 예약한 유저의 닉네임을 찾을 수 없습니다.",
	L"사용가능한 커플, 결혼 상태가 아닙니다.",
	L"던전에서는 소환하기를 사용 할 수 없습니다.",
	L"[INT] 지원하지 않는 캐시 타입 입니다.",
	L"소환하기를 사용 할 준비가 되지 않았습니다.",
	L"동일 채널에서만 소환하기가 가능합니다.",
	L"연인 소환을 위한 필드 이탈",
	L"아이템 정보가 존재하지 않습니다.",
	L"결혼식장에 입장 가능한 위치가 아닙니다.",
	L"결혼식장에 입장 가능한 아이템이 아닙니다.",
	L"결혼식장 입장을 위한 필드 이탈",
	L"결혼식이 완료 되었거나 존재하지 않는 결혼식입니다.",
	L"커플 상태가 아닙니다.",
	L"결혼식장에 입장이 실패하였습니다.",
	L"현재 필드에 소환이 불가능 합니다.",
	L"이별이 가능한 위치가 아닙니다.",
	L"위자료를 지불 할 수 없습니다.",
	L"신랑이나 신부가 아닙니다.",
	L"신랑 신부가 모두 방에 존재해야 합니다.",
	L"사랑의 리퀘스트를 가지고 오세요~뽀~",
	L"새콤달콤 뽀피트 미니미가 우편으로 지급되었습니다.",
	L"새콤달콤 뽀피트 미니미가 우편으로 지급되었습니다.",
	L"사랑의 리퀘스트를 가지고 오세요~뽀~",
	L"뽀루의 러브 레터를 발송하려면 1만 ED가 필요합니다.",
	L"뽀루의 러브 레터를 발송하지 못했습니다.",
	L"인연 상태가 아닙니다.",
	L"애칭 변경권의 정보가 이상합니다.",
	L"인연 상태에서는 삭제 할 수 없습니다.",
	L"Terjadi error saat memperbarui periode VIP.",
	L"[JP] 선택 할 수 없는 옵션입니다.",
	L"[JP] 합성 부위가 다릅니다.",
	L"결혼 상태가 아닙니다.",
	L"연인이 필드 입장에 실패 하였습니다.",
	L"Blessed Time and Space Scroll tidak dapat dipakai pada Level ini.",
	L"Item telah diidentifikasi.",
	L"Tidak ada Item di Inventorymu.",
	L"Uangmu tidak cukup untuk mengidentifikasi Item.",
	L"Item identifikasi gagal.",
	L"Kamu tidak mempunyai Blessed Time and Space Scroll yang sesuai.",
	L"Item tidak dapat diidentifikasi.",
	L"",
	L"",
	L"Item tidak dapat ditukarkan.",
	L"Uangmu tidak cukup untuk menukar Item.",
	L"",
	L"",
	L"",
	L"",
	L"Penukaran barang telah melebihi batas yang ditentukan.",
	L"Penukaran barang gagal.",
	L"",
	L"",
	L"Item durasi tidak dapat digunakan untuk Material atau ditukarkan.",
	L"Item durasi tidak dapat digunakan untuk Material atau ditukarkan.",
	L"Kamu belum memenuhi persyaratan. Harap cek kembali.",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"Account yang dikenakan Trade block tidak bisa melakukan transaksi.",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"Informasi stage tidak benar.",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"Persyaratan belum terpenuhi. Pastikan pet yang di inginkan sudah dipanggil.",
	L"Persyaratan belum terpenuhi. Pastikan pet yang dimaksud tersedia.",
	L"Selamat! Kamu berhasil mendapatkan Halloween Pumpkin Fairy Potion.",
	L"Persyaratan belum terpenuhi. Pastikan bahwa affinity pet telah mencapai 100%.",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"Kemungkinan bermain dungeon sudah melebihi batasannya.",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"File KOM telah dimodifikasi. Menutup Client.",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",

	L""
};
