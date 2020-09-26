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
	ERR_TRADE_BLOCK_UNIT_00,             //한사람의 보험자로써 등록되기 전까지 거래 기능을 이용할 수 없습니다.
	ERR_TRADE_BLOCK_UNIT_01,             //한사람의 보험자로써 등록되기 전까지 편지를 전송할 수 없습니다.
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
	ERR_BATTLEFIELD_15,             //현재 필드가 파티원이 가장 많이 있는 필드입니다.
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
	ERR_ITEM_22,             //[CN] 이벤트 기간이 끝났습니다.
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
	L"한사람의 보험자로써 등록되기 전까지 거래 기능을 이용할 수 없습니다.",
	L"한사람의 보험자로써 등록되기 전까지 편지를 전송할 수 없습니다.",
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
	L"현재 필드가 파티원이 가장 많이 있는 필드입니다.",
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
	L"[CN] 이벤트 기간이 끝났습니다.",
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
//TH버전 출력
static wchar_t* szErrorStrF[] = {
	L"สำเร็จ",
	L"Errorที่ไม่สามารถทราบได้",
	L"สภาพ User ผิดพลาด",
	L"Versionไม่ถูกต้อง หลังจากPatchกรุณาเชื่อมต่ออีกครั้ง",
	L"จำนวนผู้ใช้เซิฟเวอร์เต็มแล้ว",
	L"กำลัง MA เกมส์เซิฟเวอร์ กรุณาเชื่อมต่อหลังจาก MA เสร็จแล้ว",
	L"เขตแดนที่เชื่อมต่อกันในช่วงรวมเซิฟเวอร์",
	L"การทดลองเชื่อมต่อเซิร์ฟเวอร์ซ้ำ",
	L"การตรวจสอบ IP Server ล้มเหลว",
	L"UID เซิฟเวอร์ขัดแย้งกัน",
	L"User นี้ไม่อยู่",
	L"Userที่ถูกลบ",
	L"พาสเวิร์ดไม่ตรงกัน",
	L"การตั้งค่า Flag ของเซิฟเวอร์ทำงานล้มเหลว",
	L"NEXON การรับรองล้มเหลว",
	L"เป็นบัญชีที่ไม่สามารถแบนได้",
	L"ระดับผู้ดูแลขึ้นไปเท่านั้นที่สามารถร้องขอได้",
	L"ไม่มีProxy ID ",
	L"IPนี้ไม่ได้รับอนุญาต",
	L"ข้อมูลIDไม่ตรงกับเซิฟเวอร์เกม",
	L"ความยาวของPASSWORDแปลกประหลาด",
	L"Transaction Error",
	L"ความยาวของPASSWORDไม่ถูกต้อง",
	L"ความยาวของPASSWORDไม่ถูกต้อง",
	L"ความยาวของชื่อผิด",
	L"Userที่ถูกลบพยายามจะสร้าตัวละคร",
	L"ชื่อเล่นตัวละครนี้มีอยู่แล้ว",
	L"Transaction Error",
	L"การดาวน์โหลดข้อมูลตัวละครล้มเหลว",
	L"ความยาวของชื่อเล่นตัวละครไม่ถูกต้อง",
	L"ไม่สามารถสร้างตัวละครได้อีก",
	L"ชื่อเล่นนี้ไม่สามารถใช้ได้ระหว่างการตรวจสอบ",
	L"ไม่สามารถสร้าง CharacterSN ที่ใช้สำหรับ Nexon Messenger ใน DB ได้",
	L"ไม่สามารถใช้ชื่อเล่นนี้ได้",
	L"สามารถสร้างตัวละครได้วันละสี่ครั้ง",
	L"ตัวละครที่เลือกไม่มีในเซิฟเวอร์เกม",
	L"คลาสตัวละครที่เลือกผิดปกติ",
	L"ไม่สามารับข้อมูลหินชุบชีวิตจากDB",
	L"ไม่สามารถรับ CharacterSN ที่ใช้สำหรับ Nexon Messenger ใน DB ได้",
	L"DB ผิดพลาดในขั้นตอนเลือกตัวละคร",
	L"ต้องเปลี่ยนชื่อเล่น",
	L"ไม่พบ PVP ชาแนลที่ตรงกับเลเวล",
	L"ไม่สามารถค้นหา ผู้เล่นที่จะลบจากแชแนลได้ ",
	L"การย้ายแชแนลล้มเหลว",
	L"VPไม่เพียงพอจึงไม่สามารถเข้าไปได้ครับ",
	L"ไม่สามารถเข้าไปยังที่โล่งแจ้งได้",
	L"ไม่สามารถเข้าแชแนลได้",
	L"ไม่มีการแข่งขันตอนนี้",
	L"ไม่สามารถเรียกปารี้ลิสต์ได้",
	L"สามารถดูลิสต์ปาร์ตี้ได้ที่เกทดันเจี้ยนเท่านั้น",
	L"ไม่ใช่ปาร์ตี้ลิสต์ที่สามารถดูได้จากเกทดันเจี้ยนนี้",
	L"ตั้งแต่เลเวล3 ถึงจะสามารถเข้าแข่งขันได้ครับ",
	L"ไม่สามารถสร้างห้องได้ที่ห้องเซิร์ฟเวอร์อีกต่อไป",
	L"ไม่สามารถเผยยูเซอร์ที่Slotนี้ได้",
	L"ไม่สามารถหาSlotที่มีตัวละครนี้ได้",
	L"ห้องเต็ม",
	L"รหัสผ่านไม่ถูกต้อง",
	L"ยังไม่ได้เคลียร์ดันเจี้ยนก่อนหน้านี้",
	L"ไม่มีห้องที่สามารถใช้ Quick Join ได้",
	L"หัวหน้าห้องเท่านั้นที่สามารถร้องขอได้",
	L"ไม่ได้อยู่ในสถานะที่อนุญาตให้ใช้ packet ได้",
	L"มี ผู้เล่นที่ยังไม่อยู่ในสถานะเตรียมพร้อม",
	L"จำนวนทีมไม่ตรง",
	L"ต้องมีคนมากกว่าหนึ่งคน Ready จึงจะสามารถเริ่มได้",
	L"ไม่สามารถเปลี่ยนแปลงสถานะLoadingได้",
	L"ห้องถูกสร้างแล้ว",
	L"ข้อมูลห้องถูกปรับเปลี่ยนเรียบร้อย",
	L"ห้องถูกลบแล้ว",
	L"หาUserในห้องไม่พบ",
	L"การบุกรุกล้มเหลว",
	L"ไม่สามารถออกจากห้องบุกรุกได้",
	L"NPC ที่ตายแล้ว  UID ผิดพลาด",
	L"NPC ตายแล้ว",
	L"ข้อมูลของมอนสเตอร์ไม่ถูกต้อง การดาวน์โหลดข้อมูลล้มเหลว",
	L"ไม่สามารถหาผู้เล่นของฝ่ายตรงข้ามได้",
	L"Chating Type ผิดปกติ",
	L"ไม่สามารถบังคับปิดตัวเองได้",
	L"ระดับstaminaไม่เพียงพอ",
	L"Game Typeที่ไม่สนับสนุนStatเสริม",
	L"ไม่สามารถเข้าได้เพราะกำลังยุติเกมส์",
	L"ไม่สามารถเข้าห้องได้",
	L"ไม่สามารถรับทราบห้องที่กำลังเข้า",
	L"ขณะที่ทำการซื้อขายไม่สามารถเตรียมเกมได้",
	L"ไม่สามารถเข้าได้เพราะกำลังเริ่มเกมส์ กรุณาลองใหม่อีกครั้งหลังจากนี้",
	L"ไอเท็มเริ่มเกมส์ไม่อยู่ในอินเวนทอรี่(Inventory)",
	L"ผู้ใช้ที่ไม่มีสิทธิ์ ลองทำห้องobserver ",
	L"ข้อมูลดันเจี้ยนแมพไม่ถูกต้อง",
	L"คุณไม่สามารถใช้ Competition Channel ได้ ถ้าคุณมี tournament ticket.",
	L"ไม่มีไอเทมที่ใช้ในการเล่นดันเจี้ยนนี้ เมื่อเริ่มเกมจะถูกถอนตัวจากห้องโดยอัตโนมัติ",
	L"สามารถใช้งานฟังก์ชั่นเชิญเฉพาะห้องต่อสู้เท่านั้น",
	L"อยู่ในสถานะไม่สามารถเข้าห้อง PvPที่ฝ่ายตรงข้ามเป็นผู้เชิญได้",
	L"สามารถเชิญได้เฉพาะผู้เล่นที่อยู่ในล็อบบี้ Channel ต่อสู้เท่านั้น",
	L"ต้องอยู่ในปาร์ตี้จึงจะสามารถเริ่มเกมได้",
	L"จำนวน kill ผิดปกติ",
	L"ไม่มีไอเทมที่ต้องการจะได้รับ",
	L"คนอื่นๆเรียนรู้เรียบร้อยแล้ว",
	L"การให้ไอเท็มโดยการสุ่มล้มเหลว",
	L"ไม่สามารถหาข้อมูลไอเทมได้",
	L"Item ID ไม่ถูกต้อง",
	L"ไม่สามารถมอบไอเทมให้ได้",
	L"ไม่สามารถซ่อมหรือแก้ไขไอเทมได้",
	L"ไม่สามารถใช้ไอเทมที่อยู่ใน Quick Slot",
	L"ไม่สามารถใช้ควิกสล็อตที่ดันเจี้ยนอีเวนท์",
	L"ไม่สามารถครอบครองไอเทมได้",
	L"ไม่สามารถเอาไอเทม Inventoryชั่วคราว มาได้",
	L"ช่องว่างที่จะใส่ไอเทม Inventory ชั่วคราวลงไปใน Inventory ไม่เพียงพอ",
	L"ความยาวของ Message เครื่องขยายเสียงยาวเกินไป",
	L"ไม่สามารถเปลี่ยนทีมได้",
	L"ไม่สามารถเปลี่ยนแปลงสถานะ Slot ได้",
	L"ไม่ได้เปลี่ยนสถานะเตรียมพร้อม",
	L"ไม่สามารถออกจาก Slot ได้",
	L"ไม่สามารถบังคับให้ออกได้",
	L"ไม่สามารถบันทึกข้อมูลผู้เล่นที่ฆ่าตัวตาย",
	L"ไม่สามารถเปลี่ยนแปลงสถานะได้",
	L"ผู้เล่นถอนตัว",
	L"การเชื่อมต่อถูกยุติแล้ว",
	L"ออกจากการแข่งขันโดยการบังคับ",
	L"ไม่สามารถอยู่ในชาแนลนี้ได้เนื่องจากเลเวลอัพ",
	L"การซื้อขายแบบส่วนตัวเสร็จสมบูรณ์",
	L"การเข้าร่วมซื้อขายแบบส่วนตัวล้มเหลว",
	L"ออกจากการแข่งขันเนื่องจากความล่าช้าในการเข้าร่วมการซื้อขายแบบส่วนตัว",
	L"การซื้อขายแบบส่วนตัวถูกยกเลิก",
	L"จำนวนคนที่ค้าขายส่วนตัวผิดปกติ",
	L"เกิดความผิดพลาดในระหว่างการซื้อขายแบบส่วนตัว",
	L"ช่องว่างใน Inventory ไม่เพียงพอหรือข้อมูลไอเทมที่ลงทะเบียนผิดปกติ",
	L"หากมีระดับstaminaไม่เพียงพอ ก็จะไม่สามารถดำเนินการดันเจี้ยนได้",
	L"ไม่มีสินค้าที่ทำการซื้อขาย การซื้อขายจึงถูกยกเลิกแล้ว",
	L"การซื้อขายถูกยกเลิก",
	L"ด้วยความล่าในการลงทะเบียนสินค้าของร้านค้าส่วนตัวร้านค้าจึงถูกยุติ",
	L"การซื้อขายถูกยกเลิก",
	L"การจำหน่ายสินค้าทั้งหมดของร้านค้าส่วนตัวเสร็จสมบูรณ์ ร้านค้ายุติ",
	L"เมื่อออกจากห้องการซื้อขายถูกยกเลิก",
	L"ร้านค้าส่วนตัวยุติการหน่าย",
	L"client crash",
	L"ไม่สามารถอยู่ที่แชแนลการแข่งขันได้อีกต่อไป",
	L"ถอนตัวจากปาร์ตี้เนื่องจากเข้าร่วมเกมการต่อสู้",
	L"ถอนตัวจากดันเจี้ยนเนื่องจากเกมดันเจี้ยนถูกระงับ",
	L"ออกจาปาร์ตี้ที่เป็นทางเข้ามาร์เก็ต",
	L"เนื่องจากปาร์ตี้ที่เข้าไปยังสถานฝึกถอนตัว",
	L"เป็นประเภทปกติ",
	L"สิ้นสุดลงเนื่องจากมีการHacking",
	L"สิ้นสุดลงเพราะ client crash",
	L"การแก้ไขค่าประสบการณ์ของตัวละครล้มเหลว",
	L"ไม่พบคลาสของตัวละครที่ร้องขอใน DB",
	L"เลเวลที่มีสิทธิ์ผิดพลาด",
	L"ไม่มียูเซอร์นี้",
	L"Transaction Error",
	L"ไม่มียูเซอร์ของเลเวลที่เชิญชวน",
	L"ตัดการเชื่อมต่อโดยผู้ดูแลระบบ",
	L"คำสั่งนี้ใช้ได้แต่ในเมืองเท่านั้น",
	L"คำสั่งนี้ไม่สามารถใช้ในห้องได้",
	L"ไม่สามารถซื้อไอเทมได้",
	L"ไม่สามารถขายไอเทมซ้ำได้",
	L"เป็นเควสที่อยู่ในระหว่างดำเนินการ",
	L"ลบเควสที่ไม่ได้ดำเนินการอยู่เรียบร้อยแล้ว",
	L"เป็นเควสที่ไม่มีอยู่",
	L"เควสที่เสร็จสมบูรณ์",
	L"ตัวละครนี้เป็นตัวละครที่ไม่สามารถปฏิบัติภารกิจดังกล่าวได้",
	L"เควสที่จำเป็นก่อนหน้านี้ยังไม่เสร็จสมบูรณ์",
	L"รับเควสที่ผิดจากNPC",
	L"ไม่มี Quest Template",
	L"ยังไม่พอใจเงื่อนไขของการสิ้นสุด",
	L"การดำเนินการ DB ที่เกี่ยวข้องกับเควสล้มเหลว",
	L"สามารถรับเควสได้สูงสุด 10 เควส",
	L"เลเวลต่ำเกินไป ไม่สามารถรับเควสได้",
	L"ช่องว่างใน Inventory ไม่เพียงพอจึงไม่สามารถทำเควสให้สมบูรณ์ได้ กรุณาจัดการ Inventory ก่อน",
	L"เกิดความผิดพลาดขณะดำเนินการของรางวัลเลือกเควส",
	L"การค้นหาไอเทมรางวัลที่เลือกล้มเหลว",
	L"กำลังทำเควสเปลี่ยนอาชีพ",
	L"เกิดข้อผิดพลาดในขณะดำเนินการจัดส่งรางวัลเควสเปลี่ยนอาชีพ",
	L"ชนิดของเควสไม่สมบูรณ์",
	L"ใส่ข้อมูลสั้นกว่าตอนเวลาใส่ข้อมูลแชทที่กำหนด",
	L"ไม่สามารถกระซิบตัวเองได้",
	L"ประเภทการแชทผิดปกติ",
	L"ไม่ได้เชื่อมต่ออยู่ในปัจจุบัน",
	L"--/;/เครื่องหมายฟันหนู/จุลภาค/ช่องว่าง ไม่สามารถใช้ได้",
	L"กรอกข้อมูลสั้นกว่าเวลาการทำ User Listใหม่",
	L"ยังไม่ถึงเวลาที่จำกัดในการค้นหาข้อมูลของ User",
	L"ไม่ได้เชื่อมต่ออยู่ในปัจจุบัน",
	L"ไม่ใช่ชื่อเล่นที่มีอยู่",
	L"การเข้าไปยังที่โล่งล้มเหลว",
	L"การออกไปกลางแจ้งล้มเหลว",
	L"สกิลที่ได้เรียนจนถึงเลเวลสูงสุด",
	L"สกิลนี้ไม่มีอยู่",
	L"เลเวลต่ำเกินไป ไม่สามารถเรียนรู้สกิลได้",
	L"ขาดแคลน สกิล Point",
	L"ไม่มีไอเทมเงื่อนไขที่รับ",
	L"กรุณาเคลียร์เควสเงื่อนไขที่รับ",
	L"ยังไม่ได้เรียนรู้สกิลที่ฝึกในเควสก่อนหน้านี้",
	L"สกิลที่เรียนแล้ว",
	L"Skill Slot Idที่เปลี่ยน ไม่ถูกต้อง",
	L"ตั้งใจที่จะลงทะเบียนสกิลที่ยังไม่ได้เรียน",
	L"ไม่สามารถรีเซ็ทสกิลได้",
	L"ไม่สามารถใช้สกิล Slot B ได้",
	L"เกิดข้อผิดพลาด DB ขณะกำลังขยายสกิล Slot",
	L"สกิลที่Unsealไม่สามารถเรียนรู้ได้",
	L"สกิลที่อยู่ในระดับที่ไม่สามารถเรียนรู้ได้",
	L"ไม่สามารถรับสกิลได้เนื่องจากสกิล Point ไม่เพียงพอ ",
	L"ไม่สามารถรับสกิลได้เนื่องจากสกิล Point ไม่เพียงพอ ",
	L"จะเปลี่ยนข้อมูลสกิลที่ไม่มีอยู่",
	L"ข้อมูลสกิลเลเวลผิดปกติ",
	L"ตั้งค่าเป็นเลเวลที่สูงกว่าที่สูงกว่าเลเวลสกิลที่จำกัดแล้ว",
	L"การจัดการวันหมดอายุของ cash skill point ล้มเหลว",
	L"การลงทะเบียน cash skill point ล้มเหลว",
	L"การต่ออายุ cash skill point ล้มเหลว",
	L"การทำให้ cash skill point เริ่มต้นใหม่ล้มเหลว",
	L"ทักษะพื้นฐานไม่สามารถกลับไปเป็นเหมือนเดิมได้",
	L"ไม่สามารถสร้างปาร์ตี้ได้",
	L"ไม่สามารถออกจากปาร์ตี้ได้",
	L"ข้อมูลปาร์ตี้ผิดพลาด",
	L"สถานะไม่สามารถเชิญได้",
	L"ไม่มีปาร์ตี้ที่สังกัด",
	L"เข้าร่วมปาร์ตี้แล้ว",
	L"ไม่สามารถสร้างปาร์ตี้ภายในการต่อสู้ได้",
	L"ไม่สามารถเข้าร่วมปาร์ตี้ภายในการต่อสู้ได้",
	L"สมาชิกปาร์ตี้เต็มแล้ว",
	L"ไม่สามารถเข้าร่วมปาร์ตี้ได้",
	L"ปาร์ตี้ถูกลบไปแล้ว",
	L"ปฏิเสธการเข้าร่วมปาร์ตี้",
	L"ปฏิเสธการเชิญเข้าร่วมปาร์ตี้",
	L"เป็นสถานที่ที่ไม่สามารถเตรียมได้",
	L"การเปลี่ยนสถานะเตรียมพร้อมผิดพลาด",
	L"เลือกดันเจี้ยนไม่ถูกต้อง",
	L"การเริ่มเกมส์ล้มเหลว",
	L"เกิดข้อผิดพลาดกับประเภทเกมส์(Game Type)",
	L"ไม่ใช่หัวหน้าปาร์ตี้",
	L"ไม่สามารถเปลี่ยนหัวหน้าปาร์ตี้ได้",
	L"ไม่สามารถเปลี่ยนแปลงสมาชิกปาร์ตี้ได้",
	L"เกทดันเจี้ยนนี้ ไม่ใช่ดันเจี้ยนที่สังกัด",
	L"ไม่สามารถเปลี่ยนแปลงการเปิดเผยได้",
	L"หัวหน้าปาร์ตี้ที่จะถูกแต่งตั้งใหม่ไม่สามารถทำดันเจี้ยนปัจจุบันได้",
	L"หัวหน้าปาร์ตี้ไม่สามารถเปลี่ยนเป็นสถานะ Ready ได้",
	L"ไม่มีฝั่งตรงข้ามที่จะ Chating ด้วย",
	L"ประเภทการร้องขอปาร์ตี้ลิสต์ไม่สมบูรณ์",
	L"ไม่สามารถเปลี่ยนแปลงการตั้งค่าจำกัดเลเวลได้",
	L"ปาร์ตี้ส่วนตัว",
	L"ไม่มีหินคืนสภาพ",
	L"ฟื้นคืนชีพแล้ว",
	L"การคืนชีพล้มเหลว",
	L"ไม่สามารถใช้หินสกัดในดันเจี้ยนแห่งนี้ได้",
	L"ตั้งใจจะอัพเดทมูลค่าสถานะที่ไม่มี",
	L"ไม่สามารถดาวน์โหลดค่าตัวเลือกคอมมูนิตี้ได้",
	L"ไม่สามารถลงทะเบียนแบล็คลิสต์ได้อีกต่อไปแล้ว",
	L"เป็นผู้เล่นที่ถูกเข้าบัญชีแบล็คลิสต์แล้ว",
	L"เป็นผู้เล่นที่ถูกลบจากแบล็คลิสต์แล้ว",
	L"เป็นผู้ใช้ที่ไม่มีอยู่",
	L"ไม่สามารถลบออกจากBlacklistได้",
	L"ไม่สามารถใส่ตัวเองเข้าใน Black list ได้",
	L"ไม่สามารถลบตัวเองออกจาก Black list ได้",
	L"ยูเซอร์ที่เชื่อมต่อในปัจจุบันเท่านั้นที่สามารถลงทะเบียนแบล็กลิสต์ได้",
	L"ไม่สามารถเพิ่มตัวละครของตัวเองได้",
	L"ยังไม่หมดเวลาการเพิ่มเพื่อน",
	L"ไม่สามารถค้นหาตัวละครได้",
	L"คาแรกเตอร์ของฉันเป็นสภาพที่ไม่สามารถเชิญชวนท่านได้",
	L"ตัวละครฝ่ายตรงข้ามอยู่ในสถานะที่ไม่สามารถตอบรับการเชื้อเชิญได้",
	L"ตัวละครฝ่ายตรงข้ามอยู่ในสถานะที่ไม่สามารถไปด้วยกันได้",
	L"ไม่สามารถเชิญเป็นเพื่อนได้",
	L"ไม่สามารถเกิดการซื้อขายแบบส่วนตัวได้",
	L"ไม่สามารถถอนตัวจากการซื้อขายแบบส่วนตัวได้",
	L"ไม่สามารถสืบค้นยูสเซอร์ที่ค้าขายแบบส่วนตัวได้",
	L"ข้อมูลไอเท็มที่จะทำการลงทะเบียนผิดปกติ",
	L"ข้อมูล ED ที่จะทำการลงทะเบียนผิดปกติ",
	L"ไม่ปรากฏไอเท็มซื้อขาย",
	L"ED ในการซื้อขายไม่เพียงพอ",
	L"สามารถลงทะเบียนได้เฉพาะไอเทมที่มีใน Inventory ",
	L"ช่องว่างใน Inventory ไม่เพียงพอ",
	L"ไม่สามารถนำไอเท็มที่ครอบครองอยู่มาขายได้",
	L"เป็นสถานที่ที่ไม่สามารถแช็ทซื้อขายได้",
	L"ไม่สามารถซื้อขายกับตัวเองได้",
	L"ไม่สามารถแลกเปลี่ยนไอเทมที่ระดับความคงทนใกล้จะหมดได้ กรุณาซ่อมแซมไอเทมก่อนนำมาแลกเปลี่ยน",
	L"ไอเทมที่ลงทะเบียนแล้ว",
	L"จะทำการลบไอเท็มที่ไม่ได้ทำการลงทะเบียน",
	L"ประเภทไอเท็มที่สามารถทำการซื้อขายได้สูงสุด10ประเภท",
	L"เมื่อการซื้อขายสำเร็จแล้วจำนวน ED ของฝ่ายตรงข้ามจะถูกใช้ในราคาที่สูงที่สุด กรุณาลดปริมาณEDการซื้อขายด้วย",
	L"ยอมรับคำร้องการค้าขายแบบส่วนตัว",
	L"อยู่ในระหว่างค้าขายกับผู้อื่น",
	L"ปฏิเสธคำร้องการค้าขายแบบส่วนตัว",
	L"อยู่ในสภาวะที่ไม่สามารถทำการซื้อขายได้",
	L"เกิดข้อผิดพลาดที่เซิฟเวอร์ จึงไม่สามารถ Trade ได้",
	L"ไม่ได้พกไอเทมที่จำเป็นต่อการเข้าดันเจี้ยน",
	L"เงื่อนไขการเข้าสถานฝึกไม่เพียงพอ",
	L"ไม่มี ID ดันเจี้ยนที่กำลังจะดำเนินที่สถานฝึก",
	L"เกิดความผิดพลาดในระหว่างดำเนินการให้รางวัล",
	L"ไอเทมที่เลือกเป็นไอเทมที่ไม่มีใน Inventory",
	L"ไอเทมที่เลือกมาไม่ใช่Cubeไอเทม",
	L"เป็นไอเทมที่ตัวละครนี้ไม่สามารถใช้ได้",
	L"กุญแจไม่ครบ ไม่สามารถเปิด Cube ได้",
	L"ไม่สามารถเปิด Cube ได้",
	L"ไม่สามารถผลิตไอเทมใหม่ได้",
	L"พื้นที่InventoryในการเปิดCubeนี้ไม่เพียงพอ",
	L"รู้สึกถึงการประชิดเป็นสองเท่า",
	L"User UID ผิดปกติ",
	L"User ID ผิดปกติ",
	L"ตัวละคร UID ไม่สมบูรณ์",
	L"ชื่อเล่นตัวละครไม่สมบูรณ์",
	L"UID เซิฟเวอร์เกมขัดข้อง",
	L"Pointer ข้อมูล User ผิดปกติ",
	L"พ้อยเตอร์(Pointer)เซิฟเวอร์เกมขัดข้อง",
	L"การค้นหา User UID ล้มเหลว",
	L"หมายเลขเมสเซนเจอร์เดิมผิดปกติ",
	L"ไม่สามารถผลิตไอเทมได้",
	L"ไม่ใช่ไอเท็มใบคำสั่ง",
	L"EDไม่เพียงพอจึงไม่สามารถผลิตได้",
	L"ไอเทมวัตถุดิบไม่เพียงพอ",
	L"เพื่อที่จะผลิต ต้องมีช่องว่าง Inventory ที่เกี่ยวข้อง 1 ช่องขึ้นไป",
	L"ไม่สามารถผลิตไอเทมใหม่ได้",
	L"การรับรองHackShieldล้มเหลว",
	L"เพราะโดยHackShieldอัพเดต ต้องเริ่มใหม่อีกครั้ง",
	L"การรับรองHackShieldล้มเหลว",
	L"ใส่คูปองที่ใช้แล้วหรือไม่ก็หมายเลขคูปองผิด คูปองสามารถใช้ได้แค่1ครั้งต่อ 1 ID",
	L"กรุณาใส่หมายเลขคูปอง 15 หลัก",
	L"คูปองที่ถูกใช้แล้ว",
	L"คูปองหมดอายุ",
	L"หมายเลขคูปองไม่ถูกต้อง",
	L"คูปองนี้ใช้ได้ 1 ครั้ง ต่อ 1 ID",
	L"ไม่สามารถใช้คูปองได้",
	L"การซื้อใหม่จำนวนวันต่ำ",
	L"ไม่สามารถเรียกดูลิสต์ไอเทมได้",
	L"มีข่าวแปลกๆเกี่ยวกับไอเท็มที่พยายามจะเอามา",
	L"ช่องว่างของ Inventory ไม่เพียงพอ",
	L"ไม่สามารถนำไอเท็มที่ซื้อด้วยแคชมาได้",
	L"ไม่สามารถสอบถามข้อมูลจำนวนเงินที่เหลืออยู่ได้",
	L"ไม่สามารถค้นหาข้อมูลสมาชิกได้",
	L"เป็นบัญชีที่ไม่สามารถแบนได้",
	L"ไม่สามารถนำไอเทมของตัวละครอื่นไปยัง Inventory ได้",
	L"มีไอเทมรวมชั่วคราวอยู่แล้ว",
	L"กำลังนำไอเทมมา กรุณารอสักครู่",
	L"ค่าเฉลี่ย flame ไม่ถูกต้อง",
	L"ไม่ปรากฏตัวละครนี้",
	L"ไม่ใช่เป้าหมายที่จะกู้ Nickname",
	L"มีชื่อเล่นรวมอยู่แร้ว",
	L"ชื่อเล่นนี้ไม่สามารถใช้ได้ในระยะเวลาที่กำหนด",
	L"การกู้ Nickname ล้มเหลว",
	L"เป็นชื่อที่ไม่สามารถใช้ได้",
	L"ความยาวของชื่อเล่นไม่ถูกต้อง",
	L"ไม่สามารถเปลี่ยนชื่อเล่นได้",
	L"ไม่สามารถเกิดร้านค้าส่วนตัวได้",
	L"สามารถสร้างร้านค้าได้ในที่โล่งแจ้งเท่านั้น",
	L"ไม่สามารถหาข้อมูลผู้ใช้ที่โล่งได้",
	L"เพราะว่ามีจำนวนคนที่เข้าร่วมเยอะมากกรุณาใช้บริการหลังจากนี้อีกสักครู่",
	L"ไม่สามารถเข้าร่วมที่ร้านค้าส่วนตัวได้",
	L"ข้อมูลจำนวนไอเทมที่ต้องการขายไม่ถูกต้อง",
	L"ไอเทมอยู่ในสภาวะลงทะเบียนการค้าขายแล้ว",
	L"ไม่ใช่นักบัญชีหรอกแต่ก็ตั้งใจลงทะเบียนสิ่งของเพื่อขาย",
	L"จำนวนไอเทมที่ลงทะเบียนมากกว่าจำนวนไอเทมที่สามารถขายได้",
	L"จำนวนเงินไม่พอตั้งร้านค้า ไม่สามารถตั้งร้านค้าได้",
	L"การเตรียมตั้งร้านค้ายังไม่เสร็จ",
	L"ไม่สามารถออกจากร้านค้าได้",
	L"ไอเท็มที่จะซื้อมีมากกว่าของที่ขาย",
	L"ไม่มีไอเท็มให้ซื้อ",
	L"EDไม่เพียงพอจึงไม่สามารถซื้อสิ่งของได้",
	L"เนื่องจาก Inventory  ไม่มีช่องว่างจึงไม่สามารถซื้อได้",
	L"เพราะหานักบัญชีไม่เจอก็เลยทำการค้าไม่ได้",
	L"ขณะที่ทำการซื้อขายไม่สามารถเปิดร้านค้าได้",
	L"ร้านค้าส่วนตัวเปิดแล้ว",
	L"เกินปริมาณEDที่สามารถลงทะเบียนได้สูงสุด",
	L"เกินปริมาณEDที่สามารถซื้อได้สูงสุด",
	L"ข้อมูลไอเท็มที่จะซื้อแปลกๆ ",
	L"ไอเทมที่เลือกเป็นไอเทมที่ไม่มีใน Inventory",
	L"ไม่ใช่ไอเทมที่สามารถยิ้นส่วนได้",
	L"การแยกชิ้นส่วนล้มเหลว กรุณาแยกชิ้นส่วนใหม่อีกครั้ง",
	L"สามารถแยกส่วนได้เฉพาะอุปกรณ์และAcceserie เท่านั้น",
	L"ไม่สามารถแยกชิ้นส่วนได้",
	L"เกิดความล้มเหลวในการผลิตไอเทม",
	L"ช่องว่างใน Inventory ไม่เพียงพอจึงไม่สามารถแยกชิ้นส่วนได้",
	L"ไม่สามารถแบ่งไอเทมออกได้เมื่อหมดเวลา",
	L"ไม่มีไอเทมที่จะอัพเกรดใน Inventory",
	L"ไม่ปรากฏข้อมูลไอเทม",
	L"อาวุธหรือเกราะเท่านั้นที่สามารถอัพเกรดได้",
	L"ไม่ใช่ไอเทมที่สามารถอัพเกรดได้",
	L"ไม่สามารถเพิ่มพลังได้อีกแล้ว",
	L"ไม่มี Upgrade Stone ",
	L"ไม่สามารถดำเนินการอัพเกรดได้",
	L"ED ที่จำเป็นต่อการอัพเกรดไม่เพียงพอ",
	L"ไอเทมที่อยู่ในระหว่างติดตั้ง ไม่สามารถเสริมพลังได้",
	L"ไม่มีFluor Stoneที่ช่องสัมภาระ",
	L"ไม่สามารถใช้Fluor Stoneได้",
	L"ตั้งแต่เลเวล 10 เมื่ออัพเกรดไม่สามารถใช้หินฟลูโอรัลได้",
	L"ไอเทมที่ไม่อนุญาตให้ใช้ไม่สามารถเสริมพลังได้",
	L"การอัพเดทอัพเกรดล้มเหลว",
	L"ไอเทมที่จะซ่อมใหม่ไม่มีอยู่ในช่องสัมภาระ",
	L"ไม่ปรากฏข้อมูลไอเทม",
	L"ไม่ใช่ไอเทมที่สามารถซ่อมใหม่ได้",
	L"ไอเทมที่ไม่อนุญาตให้ใช้เท่านั้นที่สามารถเรียกคืนได้",
	L"หินฟื้นฟูมีไม่เพียงพอจึงไม่สามารถฟื้นฟูได้",
	L"ไอเทมที่อยู่ในระหว่างติดตั้ง ไม่สามารถซ่อมใหม่ได้",
	L"ไม่สามารถใช้ในไอเทมที่ไม่อนุญาตให้ใช้ได้",
	L"การอัพเกรดสำเร็จ",
	L"ไม่มีการเปลี่ยนแปลง",
	L"ลดลง 1ระดับครับ",
	L"ขั้นตอนต้นการอัพเกรด",
	L"ไม่สามารถใช้ไอเทมได้",
	L"เป็นไอเทมที่ไม่มีใน Inventory ",
	L"ไม่ปรากฏข้อมูลไอเทม",
	L"ค่าบริการไม่เพียงพอ",
	L"อาวุธหรือเกราะเท่านั้นที่มีSocket",
	L"ไม่ปรากฏข้อมูลหินเวทย์มนต์ที่เกี่ยวกับอุปกรณ์นี้",
	L"ไม่มีหินเวทย์ใน Inventory ",
	L"ไม่สามารถใช้ Socket ได้",
	L"Magic stone เสียบอยู่ที่ Socket แล้ว",
	L"Magic stone ถูกเอาออกจาก Socket แล้ว",
	L"Socket option นี้ไม่มีอยู่",
	L"เป็นชื่อเล่นที่ไม่มีอยู่",
	L"ไม่สามารถเชิญตัวเองได้",
	L"เป้าหมายที่จะรับรางวัลไม่มีตัวตน",
	L"ดาต้าครูและศิษย์ที่จะลบไม่มีใน DB",
	L"ไม่สามารถรับลิสต์ผู้ติดตามได้",
	L"เลเวลต่ำเกินไป ไม่สามารถให้ข้อมูลได้",
	L"เลเวลต่ำเกินไปไม่สามารถรับศิษย์ได้",
	L"สามารถลงทะเบียนลูกศิษย์ได้ถึง 3 คน",
	L"มีอาจารย์แล้ว",
	L"กำลังลงทะเบียนเป็นศิษย์",
	L"ข้อมูลของสิ่งที่จะซื้อผิดปกติ",
	L"ไม่สามารถซื้อสินค้าได้",
	L"เป็นการตัดสินซ้ำซ้อน",
	L"จำนวนสินค้ารวมที่ซื้อมากเกินไป",
	L"จำนวนสินค้าที่ซื้อมากเกินไป",
	L"ยังไม่สามารถซื้อสินค้านี้ได้",
	L"ไม่มี cart",
	L"เกมถูกจำกัดการใช้",
	L"เนื่องจากจำกัดปริมาณในการขายปืนจึงไม่สามารถซื้อได้",
	L"ปริมาณคำสั่งถูกกำหนดแล้ว",
	L"ยังไม่ถึงเวลาที่สามารถซื้อสินค้านี้ได้อีกครั้ง",
	L"เป็นของที่ไม่สามารถให้เป็นของขวัญได้!",
	L"ปริมาณคำสั่งถูกกำหนดแล้ว",
	L"คนที่ได้รับยังไม่ถึงเวลาที่จะสามารถซื้อสินค้านี้ได้อีกครั้ง",
	L"เป็นผู้เล่นที่ถูกบล็อก",
	L"ไม่สามารถค้นหาข้อมูลสมาชิกได้",
	L"จำนวนเงินที่เหลืออยู่ไม่เพียงพอ",
	L"อยู่ในระหว่างตรวจสอบ",
	L"ซ่อมแซมหินชุบชีวิตที่อยู่ในครอบครองมากเกินไป",
	L"ไม่สามารถขยายจำนวน Iventory Slot เพิ่มได้อีก",
	L"ไม่สามารถให้ของขวัญแก่ตัวเองได้",
	L"ใส่รหัสผ่านไม่ถูกต้อง",
	L"เป็นชื่อเล่นที่ไม่มีอยู่",
	L"ไม่สามารถให้ของขวัญผู้เล่น IDประสบการณ์",
	L"คุณส่งของขวัญเกินยอดเงินที่กำหนด",
	L"กรุณาใส่รหัสผ่าน",
	L"การรับรองบัญชีเน็กซอนล้มเหลว",
	L"เป็นของที่ไม่สามารถให้เป็นของขวัญได้!",
	L"เด็กอายุต่ำกว่า14ปีห้ามใช้",
	L"ไม่สามารถซื้อสินค้าขยายสล็อตตัวละครได้ การขยายสล็อตทำได้สูงสุด 8 ช่อง",
	L"ไม่สามารถขยายสล็อตตัวละครได้",
	L"กำลังใช้งานฟังก์ชั่นดังกล่าว",
	L"สกิลนี้ไม่มีอยู่",
	L"ไม่มีไอเทมรีเซ็ทสกิล",
	L"ไม่สามารถลบไอเทมรีเซ็ทสกิลจาก DB ได้",
	L"การล้มเหลวของ DB Query ที่รีเซ็ทสกิลถูกส่งกลับ",
	L"สกิลที่ไม่สามารถรีเซ็ทได้",
	L"ไม่สามารถรีเซ็ทสกิลที่กำลังอยู่ในช่วงติดตั้ง สกิล slot ได้",
	L"เป็นสิ่งของที่ไม่สามารถลงทะเบียนในList ได้",
	L"ไม่สามารถลงทะเบียนสิ่งของใน List ได้อีก",
	L"เป็นเทคนิคที่ไม่สามารถใช้ได้กับIDประสบการณ์",
	L"ผู้เล่นIDประสบการณ์ไม่สามารถลงทะเบียนที่ผู้แนะนำได้",
	L"ได้Rep20ที่เป็นเลเวลซึ่งสามารถใช้ผู้เล่นIDประสบการณ์ได้ ถ้าหากจะดำเนินเกมต่อไปกรุณาเปลี่ยนเป็นเน็กซันID",
	L"ผู้เล่นIDประสบการณ์ไม่สามารถลงทะเบียนโดยเพื่อนผู้ส่งได้",
	L"ไม่ใช่ยูเซอร์ที่ถูกบันทึกในHacking List",
	L"ไม่สามารถทำการ DV Update ข้อมูลในการยอมรับHacking User",
	L"ไม่สามารถทำการยอมรับได้เนื่องจากเป็นUserที่ถูกแจ้งเรื่องการHacking",
	L"เป็นไอเทมที่ไม่มีใน Inventory ",
	L"ไม่สามารถอัพเกรดธาตุได้",
	L"Slot ที่ถูกอัพเกรดธาตุแล้ว",
	L"เป็น Slot ที่ยกเลิกการอัพเกรดธาตุแล้ว",
	L"ค่าบริการเพื่ออัพเกรดพลังธาตุไม่เพียงพอ",
	L"ชิ้นส่วนของElสำหรับอัพเกรดธาตุไม่เพียงพอ",
	L"ข้อมูลไอเทมผิดปกติ",
	L"อาวุธและเกราะเท่านั้นที่สามารถอัพเกรดธาตุได้",
	L"ไอเทมที่ถูกเลือกเป็นไอเทมที่ไม่สามารถแบ่งแยกได้",
	L"เครื่องคัดแยกไม่เพียงพอ ไม่สามารถประเมิณค่าไอเทมได้",
	L"พื้นที่Inventoryไม่เพียงพอสำหรับการแยกไอเทมนี้",
	L"ไม่สามารถแยกไอเทมที่นี้ได้",
	L"การผลิตไอเทมล้มเหลว",
	L"ไม่สามารถรับข้อมูลตู้จดหมายได้",
	L"ไอเทมที่เลือกไม่สามารถแนบจดหมายได้",
	L"ไม่สามารถแนบ ED ที่กรอกในจดหมายได้",
	L"ไม่สามารถส่งจดหมายได้",
	L"เป็นจดหมายที่ไม่มีอยุ่",
	L"ไม่มีช่องว่างที่ใช้ได้ในInventory  ไม่สามาถย้ายสิ่งของที่แนบมาได้ หลังจากที่ขยายช่องที่ใช้ได้แล้ว กรุณาลองอีกครั้ง",
	L"ไม่สามารถเอาสิ่งของที่แนบมาด้วยได้",
	L"กำลังแนบไอเท็มที่ไม่มีอยู่",
	L"สามารถแนบได้เฉพาะไอเทมที่ซ่อมแซมสมบูรณ์แล้วเท่านั้น",
	L"ไม่ใช่ชื่อเล่นที่มีอยู่",
	L"ในช่วงระหว่างรวมคาแล็กเตอร์ไม่สามารถส่งจดหมายได้",
	L"ไม่สามารถส่งจดหมายได้เพราะค่าบริการไม่เพียงพอ",
	L"กล่องจดหมายของฝ่ายตรงข้ามเต็ม ไม่สามารถส่งจดหมายได้",
	L"เลือกจดหมายที่ถูกลบแล้ว",
	L"ฝ่ายตรงข้ามปฏิเสธการรับจดหมาย",
	L"ไม่สามารถนำไอเท็มที่ครอบครองแนบเข้าไปในจดหมายได้",
	L"ไม่สามารถส่งจดหมายให้กับผู้เล่น IDประสบการณ์ได้",
	L"เป็นชื่อเล่นที่ไม่มีอยู่",
	L"เป็นชื่อเล่นที่ถูกเข้าบัญชีแบล็คลิสต์แล้ว",
	L"ไม่สามารถลงทะเบียนในBlacklistได้",
	L"เป็นชื่อเล่นที่ถูกลบแล้ว",
	L"ไม่สามารถลบออกจากBlacklistได้",
	L"ไม่ใช่ฉายาเป็นเจ้าของ",
	L"อยู่ในระหว่างใส่ฉายา",
	L"เป็นฉายาที่หมดอายุใช้งานแล้ว",
	L"เป็นฉายาที่ไม่มีอยู่",
	L"ไม่สามารถรับฉายาได้",
	L"ฉายามีเจ้าของแล้ว",
	L"ไม่สามารถใช้ไอเทมได้",
	L"ไอเท็มที่ไม่มีอยู่",
	L"เลเวลตัวละครไม่เพียงพอ ไม่สามารถใช้ไอเทมได้",
	L"เป็นไอเทมที่ไม่สามารถใช้ได้จากInventory ",
	L"เป็นไอเทมที่ไม่มีใน Inventory ",
	L"ไม่ใช่ไอเทมที่มอบให้",
	L"ไม่ใช่ไอเทมที่สามารถมอบให้ได้",
	L"การอัพเดทอัพเกรดล้มเหลว",
	L"ไม่สามารถแลกเปลี่ยนไอเทมได้",
	L"เป็นไอเทมที่ไม่มีใน Inventory ",
	L"ไอเทมนี้ไม่ใช่เป้าหมายในการแลกเปลี่ยน",
	L"ไม่สามารถแลกเปลี่ยนไอเท็มได้",
	L"เป็นการถอนตัวเพื่อย้ายไปยังฟิลด์อื่น",
	L"ถอนตัวเพื่อเข้าไปยังห้อง",
	L"ผู้เล่นถูกเตะออกจากระบบ",
	L"ไม่สามารถเข้าเมืองได้",
	L"ออกจากเมืองไม่ได้",
	L"ไม่สามารถรับ User List มาได้",
	L"เลเวลต่ำเกินไป ไม่สามารถเข้าเมืองได้",
	L"ไม่ได้เคลียร์ดันเจี้ยนสำหรับเข้าไปในเมือง",
	L"แชแนลนี้ไม่มีอยู่",
	L"การย้ายแชแนลสามารถเข้าถึงได้สำเร็จ",
	L"การลงทะเบียนผู้เล่นที่จะย้ายแชแนลล้มเหลว",
	L"ผู้เล่นที่จะย้ายแชแนลไม่ได้ลงทะเบียน",
	L"Channel ปัจจุบันเหมือนกับ Channel ที่ต้องการวาร์ปไป",
	L"เพราะจำนวนคนที่รับเต็ม จึงไม่สามารถเคลื่อนย้ายได้",
	L"สามารถใช้ฟังก์ชั่นวาร์ป Channel ได้เฉพาะใน Filed และเมืองเท่านั้น",
	L"อีกสักครู่ช่วยย้าย Channel ด้วย",
	L"ความสามารถที่ตัวละครGM ไม่สามารถใช้ได้",
	L"ตัวละครไม่ได้ถูกเลือก",
	L"ไม่ได้กำลังเชื่อมต่อกับเพื่อน",
	L"อยู่ในสถานะไม่ถูกต้อง",
	L"ไม่สามารถอัพเดทDB ไอเทมที่ถูกดรอปจากอีเวนท์",
	L"ได้รับรางวัลอีเวนท์เวลาแล้ว",
	L"ตัวละครถูกลงทะเบียนเป็นเพื่อนแล้ว",
	L"สถานะของตัวละครเพื่อนไม่ถูกต้อง",
	L"ฉันไม่ใช่ตัวที่ถูกเชิญมาให้เป็นเพื่อน",
	L"ตัวละครนี้ไม่ใช่เพื่อน",
	L"ไม่มีกลุ่มอยู่",
	L"ข้อความยาวเกินไป",
	L"คล้ายกับกรุ๊ปสำหรับสั่งให้เคลื่อนที่ไปกับกรุ๊ปปัจจุบัน",
	L"ไม่สามารถสร้างกลุ่มเพิ่มได้อีก",
	L"มีกลุ่มที่รวมตัวกันอยู่แล้ว",
	L"ชื่อกลุ่มที่รวมตัวกัน",
	L"หากไม่ได้ติดต่อกัBlueกศิษย์ภายใน 7 วัน การเป็นศิษย์อาจารย์จะถูกยกเลิก",
	L"ลูกศิษย์อัพเลเวลสมบูรณ์แล้ว ความสัมพันธ์ศิษย์อาจารย์จึงถูกยุติ",
	L"ตัวละครถูกลบ ทำให้ความสัมพันธ์ถูกยกเลิก",
	L"ผู้เล่นยกเลิกความสัมันธ์ศิษย์อาจารย์",
	L"ล้มเหลวในความสัมพันธ์ของศิษย์อาจารย์",
	L"อาชีพ DB ที่เกี่ยวข้องความสามารถของเพื่อนผิดพลาด",
	L"ชื่อกลุ่มยาวเกินไป",
	L"รายชื่อเพื่อนเต็ม ไม่สามารถเชื้อเชิญเป็นเพื่อนได้",
	L"ตัวละครของบัญชีเดียวกันจะไม่สามารถเชิญเพื่อนได้",
	L"รายชื่อเพื่อนของฝ่ายตรงข้ามเต็ม ไม่สามารถชวนเป็นเพื่อนได้",
	L"ฝ่ายตรงข้ามอยู่ในสถานะปฏิเสธการชวนเป็นเพื่อน",
	L"การอัญเชิญ Wally ล้มเหลว",
	L"ไม่สามารถบุกจากแชแนลการแข่งขันได้",
	L"ในช่วงเวลาการแข่งขันนั้นในห้องต้องมีคนเต็มเท่านั้นเกมจึงสามารถเริ่มได้",
	L"ความยาวของชื่อยาวเกินไป",
	L"ข้อความในจดหมายยาวเกินไป",
	L"ยูสเซอร์ที่เชิญไม่สามารถเล่นดันเจี้ยนปัจจุบันได้",
	L"สกิลที่ถูกปลดผนึกแล้ว",
	L"การปลดผนึกสกิลล้มเหลว",
	L"เป็นอาชีพตัวละครที่ไม่สามารถซื้อสินค้านี้ได้",
	L"รางวัลของสกิลที่ถูกปลดผนึกแล้ว",
	L"ไอเทมที่เลือก มีอยู่ใน Inventoryแล้ว จึงไม่สามารถซื้อได้",
	L"ผู้เล่นIDประสบการณ์ไม่สามารถรับเชิญไปที่ปาร์ตี้ดันเจี้ยนห้วงอวากาศHoenirได้",
	L"วิธีการรับรองไม่ถูกต้อง",
	L"ไอเทมที่ถูกเก็บไว้ที่ธนาคารไม่สามารถแยกชิ้นส่วนได้",
	L"ไอเทมที่ถูกเก็บไว้ที่ธนาคารไม่สามารถเพิ่มใน Socket ได้",
	L"ไม่สามารถใช้ได้ในธนาคาร",
	L"จำนวน Slot ธนาคาร ไม่สามารถเพิ่มได้อีก",
	L"บัตรสมาชิกธนาคารไม่สามารถซื้อมากกว่า 1 อันได้",
	L"บัตรสมาชิกธนาคารที่ไม่สามารถซื้อได้ ",
	L"ถ้าหากว่ามีใบรับรองเครดิตของ Ariel ใน Inventory จะไม่สามารถซื้อบัตรสมาชิกแพลตตินัมได้ กรุณาใช้ใบรับรองเครดิตของ Ariel",
	L"ไม่มีพาสเวิร์ดนี้",
	L"ไม่สามารถแก้ไขรหัสผ่านได้",
	L"ไอดีหรือรหัสผ่านไม่ถูกต้อง",
	L"เกิดความผิดพลาดในระบบขั้นตอนการรับรอง",
	L"ไอเท็มที่ไม่มีอยู่",
	L"ไม่สามารถรับข้อมูลไอเทมได้",
	L"ประเภทไอเทมที่ไม่สามารถผนึกได้",
	L"ไอเทมที่อยู่ในระหว่างติดตั้ง ไม่สามารถผนึกได้",
	L"ไอเทมที่ถูกผนึกแล้ว",
	L"ไอเทมที่ไม่ได้รับการซ่อมแซมไม่สามารถผนึกได้",
	L"ไม่สามารถประทับตราไอเทมได้เมื่อหมดเวลา",
	L"ไม่สามารถผนึกไอเทมที่ถูกทำลายได้",
	L"ไอเท็มที่สามารถซื้อขายได้ไม่สามารถประทับตราได้",
	L"ไม่เหลือจำนวนที่สามารถผนึก จึงไม่สามารถผนึกได้",
	L"ข้อมูลใบสั่งผนึกที่เลือกผิดปกติ",
	L"การผนึกได้ล้มเหลวแล้ว",
	L"ใบคำสั่งผนึกไม่เพียงพอไม่สามารถผนึกได้",
	L"ไอเทมที่อยู่ในระหว่างติดตั้งไม่สามารถเปิดผนึกได้",
	L"ไอเทมที่ถูกปลดผนึกแล้ว",
	L"Unsealได้ล้มเหลวแล้ว",
	L"ข้อมูลการผนึกDB Updateล้มเหลว",
	L"ไอเทมที่ถูกผนึกไม่สามารถอัพเกรดได้",
	L"เกิดความผิดพลาดขึ้นขณะตรวจสอบ cash point",
	L"ID นี้เป็น ID ที่ถูกจำกัดการเคลื่อนไหว",
	L"ไม่มีชื่อบัญชีผู้ใช้นี้",
	L"บัญชีผู้ใช้ไม่ถูกต้อง",
	L"หมายเลขคูปองไม่ถูกต้อง",
	L"คูปองที่ถูกใช้แล้ว",
	L"คูปองชนิดนี้สามารถใช้ได้เพียงครั้งเดียว",
	L"คูปองหมดอายุใช้งานแล้ว",
	L"เกิดความผิดพลาดขณะใช้คูปอง",
	L"พ้อยท์ไม่เพียงพอ",
	L"เกิดความพิดพลาดขณะที่กำลังทำการซื้อไอเทม",
	L"ตั้งแต่เลเวล15ขึ้นไปจึงสามารถซื้อได้",
	L"กิลด์ที่เข้าร่วมมีอยู่แล้ว",
	L"เป็นชื่อกิลด์ที่มีแล้ว",
	L"ไม่มีไอเทมกลุ่มกิลด์ใหม่ในอินเวนทอรี่(Inventory)",
	L"ข้อมูลไอเทมกลุ่มกิลด์ใหม่ไม่สมบูรณ์",
	L"กลุ่มกิลด์ใหม่ล้มเหลว",
	L"การลงทะเบียนมาสเตอร์กิลด์ล้มเหลว",
	L"ไม่สามารถหาข้อมูลกิลด์ได้",
	L"ไม่สามารถส่งคำเชิญกิลด์ได้",
	L"ยูเซอร์ออฟไลน์อยู่ในตอนนี้",
	L"ผู้เล่นIDประสบการณ์ไม่สามารถเชิญเข้ากิลด์ได้",
	L"เป็นผู้เล่นที่เชิญเข้ากิลด์",
	L"มีผู้เล่นจากกิลด์อื่นเชิญผู้เล่นนี้แล้ว",
	L"ไม่มีอำนาจที่สามารถเชิญกิลด์ได้",
	L"ไม่สามารถเพิ่มสมาชิกกิลด์ได้อีก ต้องใช้ไอเทมเพิ่มสมาชิกกิลด์เพื่อเพิ่มเท่านั้น",
	L"ยูสเซอร์ที่อยากเชิญเป็นกิลด์เมมเบอร์แล้ว",
	L"เป็นผู้เล่นที่เข้ากิลด์แล้ว",
	L"ไม่สามารถรับยูสเซอร์ลิสท์(User list)กิลด์ได้",
	L"ข้อมูลไอเทมในการรับด้วยคูปองไม่ถูกต้อง",
	L"User ปฎิเสธการเชิญเข้ากิลด์",
	L"การเข้ากิลด์ล้มเหลว",
	L"ไม่ได้อยู่ในสังกัดกิลด์",
	L"ไม่มีอำนาจเปลี่ยนแปลงระดับสมาชิกกิลด์",
	L"การเปลี่ยนแปลงระดับสมาชิกกิลด์ล้มเหลว",
	L"เฉพาะมาสเตอร์กิลด์และผู้ควบคุมกิลด์เท่านั้นที่สามารถปรับเปลี่ยนข้อความกิลด์ได้",
	L"การเปลี่ยนแปลงข้อความกิลด์ล้มเหลว",
	L"ไม่มีอำนาจไล่สมาชิกกิลด์",
	L"มาสเตอร์กิลด์ไม่สามารถถอนตัวจากกิลด์ได้",
	L"การออกจากกิลด์ล้มเหลว",
	L"กำหนดข้อความทักทายสมาชิกกิลด์",
	L"ไม่มีสิทธิในการเปลี่ยนคำทักทายของสมาชิกกิลด์นี้",
	L"การเปลี่ยนแปลงคำทักทายสมาชิกกิลด์ล้มเหลว",
	L"เฉพาะมาสเตอร์กิลด์เท่านั้นที่สามารถใช้เงื่อนไขยกเลิกกิลด์ได้",
	L"การยกเลิกกิลด์ไม่สามารถทำได้หากมีกลุ่มกิลด์อยู่",
	L"การยกเลิกกิลด์ล้มเหลว",
	L"ระดับที่ถูกเปลี่ยนแปลงแล้ว",
	L"ไม่สามารถแต่งตั้งผู้ดูแลกิลด์ได้อีกแล้ว\nหากต้องการที่จะแต่งตั้งผู้ดูแลกิลด์ใหม่ \nต้องแก้ไขระดับของผู้ดูแลกิลด์ปัจจุบัน",
	L"มาสเตอร์กิลด์ไม่สามารถลบคาแร็คเตอร์ได้ กรุณามอบอำนาจมาสเตอร์กิลด์ให้สมาชิกกิลด์คนอื่นๆ",
	L"ไม่สามารถเปลี่ยนระดับของตัวเองได้",
	L"ไม่สามารถซื้อใบอนุญาตก่อตั้งกิลด์ขณะที่สังกัดในกิลด์แล้ว",
	L"มีใบอนุญาติการตั้งกิลด์ใน Inventory ",
	L"การเพิ่มสมาชิกกิลด์ล้มเหลว",
	L"เฉพาะมาสเตอร์กิลด์และผู้ควบคุมกิลด์เท่านั้นที่สามารถใช้ใบเพิ่มจำนวนกิลด์ได้",
	L"ไม่สามารถเพิ่มสมาชิกกิลด์ได้อีก",
	L"ขณะที่ทำการซื้อขายใช้สิ่งที่เกี่ยวข้องได้",
	L"พื้นที่ที่ไม่สามารถเคลื่อนที่ได้",
	L"ชื่อกิลด์นี้ไม่สามารถใช้ได้เป็นเวลา 7 วัน",
	L"เป็นค่าประสบการณ์กิลด์ที่มากที่สุดที่สามารถได้รับ",
	L"ต้องทำเควสที่เกี่ยวข้องก่อนจึงจะสามารถซื้อไอเทมนี้ได้",
	L"ชื่อกิลด์ยาวเกินไป",
	L"ค่าบริการในการยกเลิกธาตุไม่เพียงพอ",
	L"สามารถซื้อได้เมื่อเลเวล 35 ขึ้นไป",
	L"กรุณายกเลิกเกมส์แล้วอัพเกรดบัญชี Beanfun ที่เว็บไซต์ tw.beanfun.gamania.com",
	L"account นี้เป็น account ที่ถูกอัพเกรดเป็นบัญชี Bean Fun กรุณาเชื่อมต่อด้วยโปรแกรม Bean Fun",
	L"ไม่สามารถซื้อไอเทมได้อีกแล้ว กรุณาย้ายไอเทมที่อยู่ที่ห้องเก็บของชั่วคราวไปยังInventory",
	L"เฉพาะระดับมาสเตอร์กิลด์เท่านั้นที่สามารถลงทุน GSPได้",
	L"กิลด์สกิลที่ได้เรียนจนถึงเลเวลสูงสุด",
	L"ไม่ปรากฏข้อมูลสกิลกิลด์นี้",
	L"กิลด์สกิลที่ได้มา ยังไม่ถึงเลเวลที่ดีที่สุด ไม่สามารถเรียนรู้ได้",
	L"ลง GSP ไม่เพียงพอ",
	L"GSPไม่เพียงพอ",
	L"การเรียนสกิล(skill)กิลด์ล้มเหลว",
	L"เฉพาะระดับมาสเตอร์กิลด์เท่านั้นที่สามารถคืนสกิลกิลด์ได้",
	L"ไม่มีไอเทม Oblivion Pipe ใน Inventory",
	L"ไม่สามารถฟื้นคืนสกิล(Skill)กิลด์ได้",
	L"การฟื้นคืนสกิล(Skill)กิลด์ล้มเหลว",
	L"เฉพาะระดับมาสเตอร์กิลด์เท่านั้นที่สามารถเริ่มสกิลกิลด์ได้",
	L"ไม่มีไอเทมOblivion Bugleใน Inventory",
	L"ไม่สามารถเริ่มต้นสกิล(Skill)กิลด์ได้",
	L"การเริ่มต้นสกิล(Skill)กิลด์ล้มเหลว",
	L"เฉพาะระดับมาสเตอร์กิลด์เท่านั้นที่สามารถใช้ได้",
	L"ไม่สามารถทำคะแนนกิลสกิล(Guild Skill)ได้ในเวลาที่กำหนด",
	L"อยู่ในระหว่างใช้ไอเทมพรครั้งใหญ่ของGnosis",
	L"ยังไม่สามารถรับของตอบแทนจากอีเวนท์ได้",
	L"ช่องว่างใน Inventory ไม่เพียงพอจึงไม่สามารถรับของรางวัลกิจกรรมได้",
	L"การรับรางวัลอีเวนท์ล้มเหลว",
	L"ไม่ได้อยู่ในโหมดไอเทม",
	L"ยังเหลือไอเท็มที่ดรอปอยู่",
	L"การรับเอาดรอปไอเทมของการต่อสู้ล้มเหลว",
	L"เป็นคลาสที่สามารถผลิตได้หลังใช้การ์ดตัวละคร",
	L"บัญชีหลักไม่สามารถเข้าร่วมอีเวนท์ได้",
	L"เฉพาะมาสเตอร์กิลด์และผู้ควบคุมกิลด์เท่านั้นที่สามารถใช้ได้",
	L"ไอเทมที่นำไปใช้ห้องPCเป็นทักษะที่ไม่สามารถนะไปใช้ได้",
	L"ไม่พบโฆษณากิลด์",
	L"ความยาวของตัวอักษรยาวมาก",
	L"ช่วงเวลาการสมัครประชาสัมพันธ์ที่ไม่ถูกต้อง",
	L"เนื่องจากค่าใช้จ่ายการลงทะเบียนประกาศไม่เพียงพอจึงไม่สามารถลงทะเบียนได้",
	L"มีโฆษณาที่ลงทะเบียนแล้ว",
	L"การลงทะเบียนโฆษณากิลด์ล้มเหลว",
	L"ไม่สามารถสมัครเข้าร่วมได้ เนื่องจากมีผู้สมัครเข้าร่วมมากเกินไป",
	L"การสมัครเข้ากิลด์ล้มเหลว",
	L"สมัครเข้าสมาคมแล้ว",
	L"ไม่มีข้อมูลการสมัครเข้าร่วม",
	L"คำขอเข้ากิลด์ล้มเหลว",
	L"การลบคำร้องขอเข้ากิลด์ล้มเหลว",
	L"ไม่สามารถสมัครโฆษณาที่เลยระยะเวลาแล้วได้",
	L"ไม่อยู่ในรายชื่อการสมัครเข้ากิลด์",
	L"แม่กุญแจไม่เปิด",
	L"Game Account นี้ถูกยกเลิก",
	L"เควสที่นำไปใช้ในห้องPCอนุญาตให้ใช้แค่ในห้องPCเท่านั้น",
	L"เควสที่นำไปใช้ในห้องPCทำให้สำเร็จได้แค่ในห้องPCเท่านั้น",
	L"ถอนตัวออกจากห้องเนื่องจากมีการHacking",
	L"ไม่สามารถเข้าสมัครได้เพราะอ็อบชั่นปฏิเสธคำเชิญกิลด์เปิดอยู่",
	L"ผู้สมัครเข้ากิลด์มีมากเกินไป ไม่สามารถสมัครได้อีก",
	L"ไม่ใช่เควสอีเวนท์ที่กำลังดำเนินการในปัจจุบัน",
	L"เกินขอบเขตการซื้อของไอเทมนี้",
	L"การตั้งค่าปัจจุบันกำหนดให้ไม้สามารถใช้งาน Nexon Cash ได้. กรุณาเปลี่ยนการตั้งค่าในเมนู Nexon Cash Security Setting ที่อยู่ใน Nexon Portal ข้อมูลของฉัน",
	L"สถานะCheck Attendanceวันนี้เสร็จสมบูรณ์แล้ว",
	L"เว็บพ้อยท์เพิ่มขึ้น การเรียก DB quarry ล้มเหลว",
	L"การตรวจสอบการเข้าร่วมล้มเหลว",
	L"ดำเนินการถอนตัวออกจากปาร์ตี้ เนื่องจากเข้าร่วมปาร์จี้ซ้ำ",
	L"อยู่ในสถานะกำลังรอเข้าร่วมปาร์ตี้",
	L"ลบลิสต์ไอเทม",
	L"เพิ่มลิสต์ไอเทม",
	L"แก้ไขลิสต์ไอเทม",
	L"เลือก category ผิด",
	L"ไม่มีข้อมูลของไอเท็มที่ต้องการหาได้",
	L"Spiritของผู้ชดใช้คนแรกไม่เพียงพอ",
	L"กรุณาลองค้นหาใหม่ภายหลัง",
	L"เป็นร้านค้าที่เสร็จสิ้นการขายแล้ว",
	L"เป็นผู้ใช้ที่อยู่ในสภาพไม่สามารถซื้อของได้",
	L"การสร้างห้องล้มเหลว",
	L"ไอเท็มที่ไม่มีอยู่",
	L"เป็นเพจที่ไม่มีอยู่",
	L"เมโมได้ถูกลงทะเบียนแล้ว",
	L"การบันทึก Memo ล้มเหลว",
	L"ไม่ได้เป็นเจ้าของตำราวิทยายุทธ",
	L"การเพิ่มหน้าตำราวิทยายุทธล้มเหลว",
	L"มีสกิลโน้ตแล้ว",
	L"ต้องเลเวล20ถึงจะสามารถซื้อได้",
	L"ไม่สามารถทำการค้าส่วนตัวกับผู้เล่นของเซอร์เวอร์อื่นได้",
	L"ไม่สามารถใช้ร้านค้าส่วนตัวของผู้เล่นของเซอร์เวอร์อื่นได้",
	L"ไม่สามารถเชิญผู้เล่นของเซิฟเวอร์อื่นเข้ากิลด์ได้",
	L"ไม่สามารถลงทะเบียนผู้เล่นของเซอร์เวอร์อื่นเป็นลูกศิษย์ได้",
	L"ไม่สามารถลงทะเบียนผู้เล่นของเซอร์เวอร์อื่นเป็นเพื่อนได้",
	L"โหมดท้าทางสถานที่ของHeonirนั้นไม่สามารถเล่นกับยูเซอร์เซอเวอร์อื่นได้",
	L"เราต้องปกป้อง User จากการ Hacking",
	L"[ฮ่องกง] ไม่สามารถเปลี่ยนอาชีพในเวลาเดียวกันได้",
	L"ใส่จำนวนไอเทมที่ต้องการจะทิ้งไม่ถูกต้อง",
	L"การสร้างสัตว์เลี้ยงล้มเหลว",
	L"ชื่อสัตว์เลีย้งที่ถูกใช้แล้ว",
	L"ชื่อสัตว์เลี้ยงภาษาเกาหลีสูงสุด 6 ตัวอักษร(ภาษาอังกฤษ 12 ตัวอักษร)",
	L"อยู่ในระหว่างสร้างสัตว์เลี้ยง",
	L"การเรียกสัตว์เลี้ยงล้มเหลว",
	L"ที่นี่ไม่สามารถเรียกสัตว์เลี้ยงได้",
	L"คุณไม่มี Summoned Pet",
	L"สัตว์เลี้ยงที่ถูกเรียกมาแล้ว",
	L"การยกเลิกการเรียกสัตว์เลี้ยงล้มเหลว",
	L"การสั่งสัตว์เลี้ยงล้มเหลว",
	L"ระดับความอิ่มอยู่ในสถานะ 'อิ่ม' ไม่สามารถกินอาหารได้อีก",
	L"การให้อาหารสัตว์เลี้ยงล้มเหลว",
	L"สัตว์เลี้ยงอยู่ในสถานะ 'พิเศษ' กินแต่เมล็ด/ผลของต้นไม้แห่งElเท่านั้น",
	L"Data Save เรียบร้อยแล้ว",
	L"ไม่สามารถพัฒนาได้อีกต่อไปแล้ว",
	L"ระดับความสัมพันธ์ต้องถึง 100% จึงจะสามารถพัฒนาได้",
	L"ดูดเฉพาะของกินส่วนตัวหรือไอเทมอุปกรณ์เท่านั้น",
	L"เลเวลของอุปกรณ์ต่ำเกินไป",
	L"พาสเวิร์ดไม่ถูกต้อง",
	L"ไม่มีชื่อบัญชีผู้ใช้นี้",
	L"ระบบไม่ตอบรับชื่อบัญชีผู้ใช้",
	L"ไม่ใช่ไอเทมอาหารที่สามารถให้สัต์เลี้ยงได้",
	L"ยูเซอร์นี้กำลังติดภารกิจอื่น",
	L"[CN] ผูกมัดเมทริกซ์การ์ดเรียบร้อยแล้ว",
	L"หมายเลข@Keyผิดพลาด  กรุณาใส่อีกครั้ง ",
	L"[CN] ใส่หมายเลขการ์ดรักษาความปลอดภัยผิด กรุณาใส่อีกครั้ง",
	L"[CN] ข้อมูลอยู่ในตำราคริสตัล หากมีข้อสงสัยกรุณาติดต่อเซอร์วิสเซ็นเตอร์ ",
	L"[CN] การป้องกันพาสเวิร์ดไจแอนท์อยู่ในสภาวะยกเลิกความสัมพันธ์  สอบถามข้อสงสัยได้ที่เซอร์วิสเซ็นเตอร์",
	L"เคียรันGM",
	L"GM เรียล",
	L"[ไม่มีข้อมูลของผู้ส่ง]",
	L"ไม่สามารถล้มเลิกการทำสัญญาได้ในตอนนี้",
	L"จำนวนร้องขอมากกว่าจำนวนที่สามารถยกเลิกการลงชื่อได้ การยกเลิกการลงชื่อล้มเหลว",
	L"ไม่สามารถทำการยกเลิกการสั่งซื้อล่วงหน้าของที่ซื้อไปเกิน 7 วันแล้ว",
	L"หากได้รับสินค้าส่วนหนึ่งในแพคเกจแล้ว จะไม่สามารถยกเลิกสัญญาล่วงหน้าได้",
	L"เป็นสินค้าที่ไม่สามารถลงทะเบียนด้วยราคา 0wonได้",
	L"RequestID ซ้ำ",
	L"เป็นข้อมูลการซื้อของที่ไม่มีอยู่",
	L"เป็นข้อมูลของของที่ไม่มีอยู่",
	L"เป็นสิ่งของที่ไม่สามารถยกเลิกการลงชื่อได้",
	L"เนื่องจากเป็นของที่ได้รับมาเป็นของขวัญ จึงไม่สามารถ Cooling Off ได้",
	L"Item ที่มีเวลาไม่สามารถทำลายได้",
	L"ไอเทมที่ไม่สามารถนำมาขายใหม่ ไม่สามารถทิ้งได้",
	L"เพราะมีการจำกัดการซื้ออีเว้นท์เหรียญ1 ELมากเกินไป จึงทำให้ไม่สามารถซื้อได้อีกครับ",
	L"เลเวลไม่เพียงพอ ไม่สามารถใช้ไอเทมได้",
	L"ตั้งแต่เลเวล 11 เมื่ออัพเกรดไม่สามารถใช้หินฟลูโอรัลได้",
	L"เนื่องจากได้รับรางวัลแล้วจะไม่สามารถยอมรับเควสได้",
	L"พื้นที่ในช่องสัมภาระที่จะรับรางวัลไม่เพียงพอ ไม่สามารถรับเควสได้",
	L"ไม่สามารถใช้Magic Stone ",
	L"ไม่สามารถใช้ความสามารถนี้ได้\n ขออภัยในความสะดวก",
	L"IPที่ถูกจำกัดการเชื่อมต่อ",
	L"การแชทถูกจำกัด",
	L"ความสามารถในการซื้อขายถูกควบคุม",
	L"มีข้อผิดพลาด ไม่สามารถเริ่มเกมได้",
	L"มีความขัดแย้งกับโปรแกรมประยุกษ์อื่นอยู่",
	L"ไม่ปรากฏยูเซอร์นี้",
	L"จำนวนเงินที่เหลืออยู่ไม่เพียงพอ",
	L"เป็นการ์ดไอเท็มที่ไม่มีอยู่",
	L"ไม่สามารถใช้การ์ดนี้ที่พื้นที่นี้ได้",
	L"ไอเทมการ์ดที่ใช้แล้ว",
	L"ID cardส่วนตัวไม่ถูกต้อง",
	L"password ID cardส่วนตัวไม่ถูกต้อง",
	L"DB เออเร่อ",
	L"คุณไม่สามารถ mail ได้เนื่องจากการจำกัดทางการค้า",
	L"อยู่ในสภาวะบังคับการซื้อขายไม่สามารถสร้างร้านค้าส่วนตัวได้",
	L"อยู่ในสภาวะบังคับการซื้อขายไม่สามารถดูร้านค้าส่วนตัวได้",
	L"อยู่ในสภาวะบังคับการซื้อขายไม่สามารถค้นหาสินค้าได้",
	L"อยู่ในสภาวะบังคับการซื้อขายไม่สามารถเข้าไปที่ลานการค้าได้",
	L"มีไอเทมอยู่ในช่อง Cash inventory (ใช้ในอีเวนท์ HK) ",
	L"ต้องมีสมาชิกในปาร์ตี้ 2 คนขึ้นไป",
	L"ดันเจี้ยนปิดแล้วไม่สามารถเข้าไปได้",
	L"ไม่สามารถเล่นดันเจี้ยน World Mission กับผู้เล่นเซิฟเวอร์อื่นได้",
	L"มีค่าใช้จ่ายซ้ำเกี่ยวกับ Log ตรวจจดหมาย",
	L"มีราคาซ้ำในบันทึกควบคุมการซื้อขาย",
	L"การเพิ่ม Log การตรวจจดหมายล้มเหลว",
	L"การเพิ่มบันทึกควบคุมการซื้อขายล้มเหลว!",
	L"ยังมีสินค้าที่กำลังทำการขายอยู่ไม่สามารถสำเร็จตัวแทนการขายได้!",
	L"ไม่มีพ่อค้าตัวแทนที่แต่งตั้งอยู่",
	L"ไม่สามารถยกเลิกการขายได้",
	L"ร้านค้าส่วนตัวที่ถูกยกเลิกการขาย ไม่สามารถซื้อสินค้าได้",
	L"ผู้ค้าขายตัวแทนกำลังทำการขายสินค้าจึงไม่สามารถติดตั้งร้านค้าได้",
	L"(TW)비정상적인 방법으로 게임을 플레이 하고 있습니다. 계속 하시면 거래 정지 됩니다.",
	L"ไม่สามรถยกเลิกพ่อค้าตัวแทนได้",
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
	L"ถ้าใส่รหัสลับผิดมากกว่า2ครั้งจะไม่สามารถล็อกอินได้ครับ",
	L"หากต้องการจะฝากสินค้าไว้ที่ผู้จำหน่ายแทนจะต้องมีใบรับรองตัวแทนการจำหน่าย",
	L"รหัสลับ Security Pad ที่กรอกไม่เหมือนกัน",
	L"รหัสลับSecurity Passwordล้มเหลว 10ครั้ง\nถ้ากดปุ่ม[ยืนยัน]การเชื่อมต่อจะถูกยุติ\nRe-Activate กับการยกเลิกจะสามารถใช้ได้หลังจากยืนยันตนเองเว็บไซต์",
	L"การสร้างรหัสลับSecurity Passwordล้มเหลว",
	L"ไม่ยินยอมรหัสลับSecurity Password",
	L"ชื่อร้านค้ายาวเกินไป",
	L"อยู่ในสภาวะที่การขายของร้านค้าตัวแทนถูกยกเลิก \n หลังจากรับสินค้าแล้วจึงจะสามารถตั้งร้านค้าใหม่ได้",
	L"ผู้จำหน่ายแทนกำลังจำหน่ายสินค้าอยู่ \n ต้องปิดร้านจำหน่ายแทน\n  เพื่อรับสินค้าหรือรายได้ที่จำหน่าย ",
	L" DBQuarry ใบรับรองพ่อค้าตัวแทนล้มเหลว",
	L"การสร้างพ่อค้าตัวแทนล้มเหลว",
	L"พ่อค้าตัวแทนลงทะเบียนสินค้าล้มเหลว",
	L"ผู้ก่อตั้งเท่านั้นที่สามารถใช้งานร้านค้าตัวแทนได้",
	L"การรับสินค้าร้านค้าตัวแทนล้มเหลว",
	L"เนื่องจากเป็นสินค้าที่กำลังอยู่ในการขายในปัจจุบันการรับสินค้าจึงเกิดการล้มเหลว",
	L"อยู่ในสถานะกำลังรับของ",
	L"ปริมาณEDเกินกว่าที่จะสามารถมีได้ การรับสินค้าล้มเหลว",
	L"ช่องว่างใน Inventory ไม่เพียงพอจึงไม่สามารถได้รับสิ่งของได้",
	L"ไม่มีสิ่งของในบรรดาของที่ฝาก",
	L"สามารถมีสินค้ารวมในเวลาเดียวกันได้",
	L"เลเวลไม่ตรงเงื่อนไขที่กำหนด\nไม่สามารถขอเข้าร่วมปาร์ตี้ได้",
	L"ช่วงเวลาตัวแทนการขายยังเหลืออยู่จึงไม่สามารถซื้อสินค้าได้",
	L"มีสิ่งของที่ไม่ได้หา ไม่สามารถสร้างร้านส่วนตัวได้ \n กรุณาตรวจสอบการรับสิ่งของ",
	L"สิ้นสุดการจำหน่าย เนื่องจากสินค้าที่จำหน่ายของผู้จำหน่ายแทนถูกจำหน่ายหมดแล้ว",
	L"ไม่สามารถรับเควสได้. เป็นเควสที่รับและได้รับรางวัลแล้ว. ",
	L"ไม่สามารถรับเควสได้เนื่องจาก Iventory Slot ไม่เพียงพอ",
	L"ไม่สามารถเปลี่ยนแปลงเลเวลขั้นต่ำในการเข้าปาร์ตี้ได้",
	L"ในกรณีที่สินค้าในร้านค้าตัวแทนถูกขายจนหมดทำให้มีเงินมากเกินไปจึงไม่สามารถตั้งร้านได้",
	L"(TW/HK)강화 보조 아이템 사용 불가",
	L"(TW/HK)강화 보조 아이템이 없습니다.",
	L"(TW/HK)파괴 방지 아이템 사용 불가",
	L"(TW/HK)파괴 방지 아이템이 없습니다.",
	L"เลเวลไม่ตรงเงื่อนไขที่กำหนด\nต้องยกเลิกการจำกัดเลเวลหรือเลือกดันเจี้ยนอื่น",
	L"เลเวลไม่ตรงเงื่อนไขที่กำหนด\nไม่สามารถเปลี่ยนแปลงการจำกัดเลเวลได้",
	L"เลเวลไม่ตรงเงื่อนไขที่กำหนด\nไม่สามารถเชิญเข้าปาร์ตี้ได้",
	L"เลเวลไม่ตรงเงื่อนไขที่กำหนด\nต้องยกเลิกการจำกัดเลเวลหรือเลือกดันเจี้ยนอื่น",
	L"ไม่สามารถใช้ตัวเลขซ้ำสร้าง Password ได้ \n (เช่น 0000, 1111, 999999)",
	L"ตัวเลขที่เรียงกันไม่สามารถใช้เป็นรหัสลับได้\n(เช่น 1234, 9876, 123456)",
	L"การลบ Security Password ครั้งที่ 2 ล้มเหลว",
	L"ไม่ใช่ Security Password ครั้งที่ 2 ที่ใช้งานอยู่",
	L"ไม่สามารถเปลี่ยนเป็น Password เดียวกัน \n กับ Password ก่อนหน้านี้",
	L"ต้องการซื้อไอเทมที่ไม่ได้กำลังขายในร้าน",
	L"เนื่องจากเริ่มเกมการต่อสู้ ดังนั้นการ Match จึงสิ้นสุดลง",
	L"ไม่มีผู้เข้าร่วมที่จะ Matching กันในการต่อสู้ได้",
	L"การMatchingการต่อสู้ถูกยกเลิกแล้ว",
	L"เนื่องจากสมาชิกปาร์ตี้ไม่ได้เข้าร่วมสงคราม การจับคู่สงครามถูกยกเลิก",
	L"ถ้าแมทช์สำเร็จก็จะหลุดออกจากwaiting list",
	L"User ยกเลิกคำเชิญทำให้ไม่อยู่ใน List ",
	L"การเข้าร่วมการแข่งขันถูกยกเลิกเนื่องจากสมาชิกปาร์ตี้กถอนตัว",
	L"อยู่ในสถานะกำลัง Match ",
	L"ข้อมูลจำนวนสมาชิกที่ต่อสู้ผิดปรกติ",
	L"ข้อมูลประเภทการต่อสู้ผิดปกติ",
	L"สามารถเริ่ม PVP ได้ตั้งแต่เลเวล 10 เป็นต้นไป",
	L"สามารถใช้งานปาร์ตี้ต่อสู้ได้ตั้งแต่เลเวล 10",
	L"รูปแบบปาร์ตี้ที่ถูกเปลี่ยนแปลงแล้ว",
	L"เมื่อจำนวนสมาชิกของปาร์ตี้ดันเจี้ยนมี 4 คนจะไม่สามารถเปลี่ยนไปยังปาร์ตี้การต่อสู้ได้",
	L"การเปลี่ยนแปลงประเภทปาร์ตี้ล้มเหลว",
	L"มีสามาชิกในปาร์ตี้ที่มีเลเวลไม่ถึง 20 ไม่สามารถเปลี่ยนเป็นปาร์ตี้สงครามได้",
	L"มีสามาชิกในปาร์ตี้ที่มีเลเวลไม่ถึง 10 ไม่สามารถใช้ Official War ได้",
	L"ตัวละครที่มากกว่าเลเวล10เท่านั้นที่รับเชิญให้ไปปาร์ตี้มหาสงครามครับ",
	L"ตัวละครที่มากกว่าเลเวล10เท่านั้นที่สามารถได้เป็นลานปาร์ตี้ของปาร์ตี้มหาสงครามครับ",
	L"[CN]การแลกเปลี่ยนไม่สำเร็จ กรุณาลองอีกครั้ง",
	L"คุณไม่สามารถ กดปุ่ม หลายครั้งในเวลาเดียวกันได้",
	L"ไม่ได้อยู่ในสถานะที่สมัครMatchingการต่อสู้",
	L"อยู่ในสถานะเข้าสู่ห้อง PvPแล้ว",
	L"ไม่ใช่ห้องต่อสู้",
	L"คำร้องขอ Matching ต่อสู้ล้มเหลว",
	L"อันดับการต่อสู้ไม่เพียงพอไม่สามารถซื้อไอเทมได้",
	L"ไม่สามารถเชิญผู้เล่นที่กำลัง Matching ต่อสู้เข้าร่วมปาร์ตี้ได้",
	L"เนื่องจากอยู่ในอันดับการต่อสู้ที่ต่ำจึงไม่สามรุซื้อไอเทมได้",
	L"ไม่สามารถสมัครเข้าร่วมปาร์ตี้อื่นขณะที่อยู่ในสถานะสมัคร Matching ต่อสู้",
	L"ไม่สามารถสมัครเข้าร่วมปาร์ตี้อื่นขณะที่อยู่ในสถานะสมัคร Matching ต่อสู้",
	L"ไม่สามารถใช้งานฟังก์ชั่นย้าย Channel ได้ขณะที่อยู่ในสถานะ Matching การต่อสู้",
	L"การรับรองIDของเครื่องล้มเหลว",
	L"มีการใช้IDของเครื่องซ้ำต่อเนื่อง",
	L"ไม่ใช่แรนดอมเควสที่สามารถรับได้ในวันนี้",
	L"ไม่เห็นด้วยตามข้อตกลง",
	L"มีปัญหากับค่าการจ่ายSP",
	L"มีปัญหากับค่าการใส่SP",
	L"ชื่อ DB เป้าหมายไม่ถูกต้อง",
	L"ตรวจสอบdataล้มเหลว",
	L"ขออภัย ไม่สามารถใช้ได้เนื่องจากกำลังตรวจสอบแคชช็อป ",
	L"ถูกบังคับให้เชื่องโยงเกมส์ตั้งแต่เมือง00จนถึงเมิอง06ครับ",
	L"ไม่สามารถเอาข้อมูลมาได้",
	L"ไม่สามารถใช้การขายได้จนกว่าจะถูกลงทะเบียนเป็นนักผจญภัย",
	L"ไม่สามารถส่งจดหมายได้จนกว่าจะถูกลงทะเบียนเป็นนักผจญภัย",
	L"[CN]การใช้ล้มเหลว!",
	L"ออกจากห้องเพื่อเข้าสู่ Battle field",
	L"คุณจะต้องมี LV. 15 เพื่อทำการค้าขาย",
	L"ServerSNกับ MachineIDไม่แมชกัน",
	L"MachineIDกับ ServerSN ไม่แมช",
	L"มีServerSNที่ไม่มีตัวตนอยู่ที่DB",
	L"มีMachineIDที่ไม่มีตัวตนอยู่ที่DB",
	L"ไม่มี SN ที่ Cra ให้ แต่มี SN ที่เกี่ยวข้องกับ MachineID ที่ DB",
	L"เป็น SN ที่ถูกจำกัด",
	L"ServerSN ที่ถูกจำกัดการเชื่อมต่อ",
	L"นี่ไม่ใช่ Tes Server's Account",
	L"สลายปาร์ตี้อัตโนมัติเพื่อรวมปาร์ตี้",
	L"Skill Slot Expansion กำลังถูกเปิดใช้ คุณไม่สามารถใช้ Skill Slot Expansion (แบบจำกัดเวลา) ได้ในขณะนี้",
	L"กำลังใช้งานฟังก์ชั่นขยายช่อง Slot สกิลแบบไม่จำกัดเวลา. ไม่สามารถนำไอเทมออกมาได้",
	L"[CN] บันทึกเรียบร้อยแล้ว กรุณาสอบถามที่ศูนย์บริการลูกค้า",
	L"[CN] ใส่รหัสผ่านผิด  กรุณาใส่อีกครั้ง",
	L"[CN] ไม่ปรากฏบัญชีในเวลาเดียวกัน",
	L"[CN] system error",
	L"อยู่ในระหว่างใช้ไอเทมเดียวกัน",
	L"เลเวลไม่เพียงพอ ไม่สามารถซื้อได้",
	L"เลเวลไม่เพียงพอไม่สามารถรับรางวัลได้",
	L"การสร้างปาร์ตี้ล้มเหลว",
	L"จำนวนสมาชิกปาร์ตี้ไม่เพียงพอ",
	L"ฝ่ายตรงข้ามไม่ตอบรับการเชิญปาร์ตี้",
	L"[JP] เป็นสินค้าที่ไม่มีการจำหน่าย",
	L"[JP] 한게임 빌링 오류로 구매할수 없습니다.",
	L"ไม่สามารถส่งไอเทม Sharing Bank ไปที่ Mailbox ได้",
	L"ไม่สามารถแลกเปลี่ยนไอเทม Sharing Bank ได้",
	L"ไม่สามารถทำการค้าจานได้เนื่องจากคุณต้องมี LV. อย่างน้อย 21",
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
	L"เป็นตัวละครที่อยู่ในสถานะถูกลบไปแล้วหรือกำลังรอลบ",
	L"ไม่ใช่ช่วงเวลาที่สามารถลบตัวละครได้",
	L"ไม่ใช่ชวงเวลาที่สามารถลบตัวละครครั้งสุดท้ายได้",
	L"ไม่ใช่ตัวละครที่สามารถลบเป็นครั้งสุดท้ายได้",
	L"ไม่ใช่ตัวละครที่จะ Rstore การลบได้.",
	L"ไม่ใช่ช่วงเวลาที่สามารถ Restore ตัวละครได้.",
	L"กรุณาลองจัดเรียงใหม่อีกครั้งในภายหลัง.",
	L"ไม่สามารถค้นหาตัวละครดังกล่าวได้.",
	L"ลบตัวละครล้มเหลว.",
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
	L"เกิดข้อผิดพลาดระหว่างเข้าไปพื้นที่",
	L"การค้นหาข้อมูลผู้เล่นในพื้นที่ล้มเหลว",
	L"การเริ่มเล่นดันเจี้ยนด้วยปาร์ตี้อัตโนมัติล้มเหลว",
	L"การเริ่มเล่นดันเจี้ยนล้มเหลว",
	L"ไม่สามารถเริ่มปาร์ตี้อัตโนมัติในขณะนี้",
	L"ไม่สามารถเริ่มปาร์ตี้อัตโนมัติเนื่องจากResting Buff กรุณารอจนกระทั่งBuffหายไป",
	L"คุณได้เริ่มปาร์ตี้อัตโนมัติแล้ว",
	L"การเริ่มปาร์ตี้ล้มเหลวเนื่องจากข้อมูลที่ผิดพลาด",
	L"การลงทะเบียนเป็นผู้รอปาร์ตี้อัตโนมัติล้มเหลว",
	L"ไม่อยู่ในสถานะที่ใช้ปาร์ตี้อัตโนมัติได้",
	L"สมาชิกปาร์ตี้มีเพียง 1คนจึงไม่สามารถเริ่มเกมได้",
	L"มีผู้เล่นยกเลิกการเป็นสมาชิกปาร์ตี้",
	L"หมดเวลาของการตอบรับการเริ่มเล่นปาร์ตี้ไปแล้ว จึงไม่สามารถเริ่มเกมได้",
	L"เข้าร่วมปาร์ตี้แล้ว",
	L"ผู้ที่ถูกเชิญกำลังรอเข้าปาร์ตี้อื่น",
	L"ยังไม่ได้เข้าร่วมปาร์ตี้",
	L"ไม่สามารถใช้น้ำทิพย์และยาพิษในเวลาเดียวกันได้",
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
	L"หยุดเล่นปาร์ตี้เนื่องจากการเริ่มเล่นดันเจี้ยน",
	L"มีสมาชิกปาร์ตี้อัตโนมัติบางคนยังไม่ยืนยันการเริ่มเล่นเกม",
	L"คำขอยกเลิกปาร์ตีอัตโนมัติสำเร็จแล้ว",
	L"สมาชิกปาร์ตี้ไม่ตกลงเริ่มเกม ปาร์ตี้อัตโนมัติจึงถูกยกเลิก ",
	L"ปาร์ตีอัตมัติสมบูรณ์และถูกนำออกจากรายชื่อผู้รอ",
	L"ผู้เล่นยกเลือกการขอปาร์ตี้อัตโนมัติ และออกจากรายชื่อผู้รอ ",
	L"ปาร์ตี้อัตโนมัติถูกเลิกเนื่องจากสมาชิกออกจากปาร์ตี้",
	L"เลเวลไอเทมไม่ตรงกับเงื่อนไขที่กำหนด ไม่สามารถเล่นเกมดันเจี้ยนได้",
	L"ไม่สามารถจัดเก็บข้อมูลตั้งค่าคีย์บอร์ดได้",
	L"การจัดเก็บข้อมูลตั้งค่าคีย์บอร์ดทั้งหมดล้มเหลว",
	L"ไม่สามารถอ่านข้อมูลตั้งค่าคีย์บอร์ดได้",
	L"การอ่านข้อมูลตั้งค่าคีย์บอร์ดทั้งหมดล้มเหลว",
	L"การตอบสนองช้าลงเนื่องจากการเริ่มเล่นดันเจี้ยน",
	L"[US] 추천인 테이블에서 캐릭터 삭제를 실패하였습니다.",
	L"[US] 보안패드 리셋용 질문 및 답변을 입력하지 않았습니다.",
	L"ไม่ใช่สมาชิกปาร์ตี้เดียวกัน",
	L"การจัดเก็บตัวเลือกสนทนาล้มเหลว",
	L"การอ่านข้อมูลตัวเลือกสนทนาล้มเหลว",
	L"การอ่านข้อมูลตัวเลือกสนทนาบางส่วนล้มเหลว",
	L"Test",
	L"[US] 한달에 $1,000 까지만 선물이 가능합니다.",
	L"나이정보를 확인 중 오류가 발생하였습니다. 잠시 후 다시 로그인하여 주십시오.",
	L"실명인증을 받지 않은 회원이거나 넥슨닷컴 회원이 아닙니다.",
	L"셧다운제에 의하여 게임이용이 차단 됩니다.",
	L"선택적 셧다운제의 의해 게임이용이 차단됩니다.",
	L"통합 맴버십 ATL 2.0 유저가 아니므로 게임이용이 차단 됩니다.",
	L"ยังไม่ถึงเวลาที่สามารถ Open ได้",
	L"เวลาที่สามารถ Open ผ่านไปแล้ว จึงไม่สามารถ Open ได้",
	L"ServiceID ไม่ถูกต้อง.",
	L"ไม่สามารถค้นหาข้อมูลแคชที่เหลืออยู่ได้.",
	L"[TW]ServiceCode 가 올바르지 않습니다.",
	L"[TW]정의되지 않은 애러 발생했습니다.",
	L"[TW]시간형식이 잘못되었습니다.",
	L"[TW]Type정보가 잘못되었습니다.",
	L"[TW]이전 상품 구매가 진행 중입니다.",
	L"ไม่สามารถรับเควสเลเวลระดับสูงได้",
	L"ใส่จำนวน Parameter ไม่ถูกต้อง.",
	L"[JP] 합성제가 없습니다.",
	L"[JP] 없는 아바타를 합성하려고 했습니다.",
	L"[US]월 선물 가능 횟수를 초과하였습니다.",
	L"[CN]1인 플레이 던전 입니다.",
	L"[CN]던전 오픈 시간이 아닙니다.",
	L"ไม่ได้เลือกตัวละครปรกติ.",
	L"ส่งจดหมายธรรมดาให้ตัวละครของตัวเอง.",
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
	L"Leaving to enter the dungeon game.",
	L"Leaving to enter the PVP match.",
	L"Leaving due to changing channel.",
	L"Leaving due to moving to a party member.",
	L"Leaving due to using teleport item.",
	L"Leaving due to quitting.",
	L"คุณไม่สามารถเชิญสมาชิก Party ได้ขณะอยู่ใน Dungeon",
	L"อยู่ใน Party อยู่แล้ว ไม่สามารถเชิญซ้ำได้",
	L"ในขณะ PVP ไม่สามารถเชิญได้",
	L"ในขณะอยู่ใน Dungeon ไม่สามารถเชิญได้",
	L"ไม่พบผู้เล่นที่ต้องการ จาก Server เดียวกัน",
	L"ไม่สามารถเชิญสมาชิก Party ได้มากกว่านี้",
	L"คุณไม่สามารถเชิญสมาชิก Party ได้ขณะทำการ PVP",
	L"ระดับ Level ของอุปกรณไม่เหมาะสมที่จะเข้าสู่ Field",
	L"ยังไม่สามารถเข้าสู่ Field ได้เนื่องจากคุณยังไม่ได้เคลียร์ Dungeon ที่กำหนดไว้",
	L"คุณสามารถรับเควสได้สูงสุด 30 เควส",
	L"ED ไม่เพียงพอ",
	L"ผู้เล่น Auto Party ไม่สามารถส่งเชิญ Party ได้",
	L"หากคุณกำลังอยู่ในขณะ รอ Auto Party คุณไม่สามาถ ส่งคำเชิญถึงผู้อื่นได้",
	L"ไม่สามารถส่งคำเชิญ Party ได้ขณะกำลังอยู่ใน Sparring Area",
	L"ไม่สามารถส่งคำเชิญ Party ได้ขณะกำลังอยู่ใน Training",
	L"ไม่สามารถส่งคำเชิญ Party ได้ขณะกำลังรอการแข่งขัน PVP",
	L"ในขณะที่คุณลงทะเบียนสำหรับ Auto Party คุณจะไม่สามารถทำการส่งการเชิญ Party ได้",
	L"ในขณะที่คุณคุณลงทะเบียนสำหรับแข่งขัน PVP คุณจะไม่สามารถทำการ เชิญ Party ได้",
	L"Field ในปัจจุบันเป็นของ Party พวกคุณ",
	L"ผู้ถูกเชิญมี Party อยู่แล้วไม่สามารถเชิญซ้ำได้",
	L"이 이벤트 쿠폰은 계정당 한 번만 받을 수 있습니다.",
	L"[ID] ID 또는 비밀번호가 잘못되었습니다.",
	L"ไม่มีชื่อดังกล่าว",
	L"Server group ID ไม่ถูกต้อง",
	L"[TW] 공유 은행 처리중인 아이템 입니다. 은행을 닫고 다시 시도해 주세요.",
	L"คุณไม่สามารถใช้ Competition Channel ได้ ถ้าคุณไม่มี tournament ticket.",
	L"คุณจำเป็นต้องมี ED และ Item สำหรับการเปิด Cube",
	L"ED ไม่เพียงพอสำหรับการเปิด Cube",
	L"เริ่มด้วย สมาชิกในปัจจุบัน",
	L"길드 생성은 하루에 한 번 가능합니다.",
	L"[ID] ID 는 최소 4자에서 최대 20자 까지 입력 가능 합니다.",
	L"[ID] PW 는 최소 6자에서 최대 15자 까지 입력 가능합니다.",
	L"이벤트 기간이 끝났습니다.",
	L"아라 캐릭터로는 가져올 수 없는 아이템입니다.",
	L"금일 선물하기 제한을 초과하였습니다.",
	L"아쉽게도 교환 수향이 모두 소진 되었습니다. 1월 17일 점검 이후 교환 수량이 추가 됩니다.",
	L"[ID] 인증실패 : 서버와의 연결을 확인해 주십시오.",
	L"ไม่มี Pet ในขณะนี้",
	L"Function กำลังเปิดใช้งาน",
	L"ต้องการระดับเริ่มต้น หรือสูงกว่า",
	L"กำลังเริ่มเกม ไม่สามารถเปลี่ยน Skill Slot",
	L"ล้มเหลวในการรับข้อมูล Field Monster เนื่องจากสถานะของ Field Room ไม่ใช่ห้องปัจจุบัน",
	L"ล้มเหลวในการรับข้อมูล Field Monster เนื่องจากสถานะของ Field Room ไม่ถูกเปิดใช้",
	L"ไม่สามารถขยายช่องเก็บของได้ เนื่องจากขยายได้สูงสุด 30 ช่อง",
	L"คุณถูกโหวตออกจากห้อง",
	L"Voter จะไม่ใช่ผู้เล่นธรรมดา",
	L"Voted target จะไม่ใช่ผู้เล่นธรรมดา",
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
	L"จำนวนครั้งที่คุณสามารถเข้าสู่ดันเจี้ยนได้นั้น เกินขีดจำกัดที่ตั้งไว้",
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
	L"ไม่มีField 100% Enjoy Guidebook\n ในครอบครอง",
	L"การบันทึกChapterล้มเหลว",
	L"บันทึกในGuidebookแล้ว",
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
	L"เกิดปัญหาขึ้นระหว่างการต่ออายุVIP Period",
	L"[JP] 선택 할 수 없는 옵션입니다.",
	L"[JP] 합성 부위가 다릅니다.",
	L"결혼 상태가 아닙니다.",
	L"연인이 필드 입장에 실패 하였습니다.",
	L"The Blessed Time and Space Scroll ไม่สามารถใช้ได้ในระดับเลเวลนี้",
	L"ไอเทมได้ทำการ 'ตรวจสอบ' เรียบร้อยแล้ว",
	L"ไม่มีไอเทมใน Inventory ของคุณ",
	L"คุณขาดเงินสำหรับ 'ตรวจสอบ' ไอเทม",
	L"ไอเทมนี้ไม่สามารถ 'ตรวจสอบ' ได้",
	L"คุณมี Blessed Time and Space Scrolls ไม่ครบ",
	L"ไอเทมนี้ไม่สามารถ 'ตรวจสอบ' ได้",
	L"[ES_INT]이미 같은 계정의 다른 캐릭터로 신청하셨습니다.",
	L"[ES_INT]이미 해당 캐릭터로 신청하셨습니다.",
	L"ไอเทมนี้ไม่สามารถแลกเปลี่ยนได้",
	L"คุณมีเงินไม่เพียงพอต่อการ 'ตรวจสอบ'ไอเทม",
	L"",
	L"",
	L"",
	L"",
	L"เลยระยะเวลาสำหรับการแลกเปลี่ยนไอเทมแล้ว",
	L"ล้มเหลวในการแลกเปลี่ยนไอเทม",
	L"",
	L"",
	L"ไอเทมที่มีกำหนดระยะเวลา ไม่สามารถใช้เป็นวัตถุดิบในการแลกเปลี่ยนได้",
	L"ไอเทมที่มีกำหนดระยะเวลา ไม่สามารถใช้เป็นวัตถุดิบในการแลกเปลี่ยนได้",
	L"คุณขาดคุณสมบัติบางอย่างอยู่ โปรดตรวจสอบอีกครั้ง",
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
	L"Account ที่ระงับการซื้อขาย ไม่สามารถทำการซื้อขายส่วนตัวได้",
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
	L"ข้อมูล Stage ไม่ถูกต้อง",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"ไม่ตรงกับเงื่อนไข กรุณาตรวจสอบว่า pet ถูกเรียกแล้วหรือยัง",
	L"ไม่ตรงกับเงื่อนไข  โปรดเช็คว่า pet ดังกล่าวสามารถใช้ได้หรือไม่",
	L"ขอแสดงความยินดีด้วย! ท่านใช้ Halloween Pumpkin Fairy Potion ได้สำเร็จแล้ว",
	L"ไม่ตรงกับเงื่อนไข ลองครวจสอบว่า ระดับความสนิทสนม  pet เป็น 100% หรือไม่",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"เวลาเล่นเกินกว่าที่อนุญาติไว้",
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
	L"พบ KOM ไฟล์ท่มีการดัดแปลงเสียหาย ระบบทำการปิดตัวเอง",
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
