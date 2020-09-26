//////////////////////////////////////////////////////////////////////////
// 이름: 최민철
// 작업시작날짜: 2013-01-04
// 패치날짜: 
// 작업내용: 게임내 정보 스트링을 엑셀파일로 출력 // '?' 버튼 클릭 시 동작 (퀘스트창, 스킬창, 캐릭터의 칭호창, 펫 창)
//#define PRINT_INGAMEINFO_TO_EXCEL
//////////////////////////////////////////////////////////////////////////

// 2013.4.10 박진웅 : 던전에서 미장착 기간제 아이템이 만료됐을 때, 아이템ID가 같은 아이템을 벗는 문제 수정
//#define ITEM_EXPIRE_USING_ITEM_UID

// 2013.4.10 박진웅 : 필드 맵에서 이동해 둔 위치가 계속 내 위치 주변으로 되돌아가던 문제 수정
//#define FIX_FIELD_MAP_SCROLLING

// 2013.4.9 박진웅 : 필드 맵에서 동료의 이름이 이상한 곳에 표시되던 문제 수정
//#define FIX_PARTY_MEMBER_POSITION


#define FIELD_CHARACTER_SCREEN_SHOT					// 김석근 [2012.02.15] 캐릭터 스크린샷 기능 구현

// 검의길, 웨폰테이커 공용 콤보트리 툴팁
//#define COMBO_TREE_TOOLTIP


//#define RAVEN_WEAPON_TAKER // 레이븐1차전직 웨폰 테이커
// patch 할 때 실제파일 체크섬 검사하지 않고 게임 시작후 별도의 스레드에서 검사하게 하는 테스트 ver.xml, *.kom 파일 필요.
// 즉, vc에서 실행하면 안됨, 클라이언트 바로 꺼짐
// fix!!! x2.exe만 다른 파일로 교체하면 nexon passport가 노출될 수도 있으므로 x2.exe는 바로 검사해야할 듯
//#define CHECKSUM_THREAD_TEST

//#define DMLEE_ONLY	// 동민 개인 컴파일 용도 입니다. 

// 달라붙는 damage effect test
//#define STICKY_DAMAGE_EFFECT_TEST



// 캐릭터가 멈추면 이펙트도 멈추게
//#define STOP_UNIT_STOP_EFFECT_TEST



// 몬스터 변신 테스트
//#define TRANSFORMER_TEST




// 오류가 발생할 수 있는 부분들을 체크해서 확인할 수 있도록 한다.
#define NO_MORE_ERROR_TEST


// 사내에서 발생한 플레이 로그, 에러 로그 등을 메일로 받아서 오류를 확인합시다~
//#define IN_HOUSE_PLAY_LOG_TEST


// 라인맵 역할을 하는 몬스터 테스트
#define MARIO_LIKE_BLOCK_TEST


// 박스대신에 터지는 기름탱크로 교체하는 테스트
//#define FUEL_TANK_TEST

// 대전게임에서 아무 맵이나 대전맵으로 로딩할 수 있게
#define FREE_SELECT_PVP_MAP



// 카메라 위치 테스트, 게임에만 구현함, 마을에는 구현 아직 안됨
//#define CAMERA_SCRIPTING_TEST

// 몬스터 트로피
//#define MONSTER_TROPHY

// 바운딩 콤보 테스트
//#define BOUNDING_COMBO_TEST

// 몬스터 무중력 상태로 띄우기 테스트
//#define REVERSE_GRAVITY_TEST

// 이브 뺨 때리면 카메라 뒤집어지게
//#define UP_SIDE_DOWN_CAMERA_TEST

// 몬스터 죽을 때 터뜨리기
//#define OVERKILL_TEST

// 몬스터 죽을 때 파티클 연출
//#define MONSTER_DIE_PARTICLE_TEST



// 절벽 타기
#define CLIFF_CLIMBING_TEST


// 아이템 치트로 사는 인터페이스
#ifndef ITEM_CHEAT_POPUP_TEST
	#define ITEM_CHEAT_POPUP_TEST
#endif //ITEM_CHEAT_POPUP_TEST


// 사내에서 몬스터 테스트용 던전 들어갈 수 있게 
#define MONSTER_TEST_DUNGEON



// 게임중 카메라 위치, 각도 지정해서 플레이 할 수 있게 하는 기능 
#define IN_GAME_MANUAL_CAMERA_POSITION_TEST


// 캐릭터, 몬스터 위치가 잘못 되었을 때, 위치를 리셋하는 기능
#define RESET_INVALID_UNIT_POSITION_TEST


// 잡아서 던지기
//#define GRAPPLING_TEST


// 사내버전에서만 로그인창에서 메세지 팝업
//#define IN_HOUSE_ONLY_NOTICE_MESSAGE_POPUP_TEST


// 게임중에 파일 드롭하면 해당 파일이 있는 폴더를 데이타 디렉토리에 추가해준다
#define DROP_FILE_TEST



// meshplayer에서 move axis angle이 0이 아닐때 중력값이 잘못 적용되던 오류 수정하기
//#define FIX_XMESH_PLAYER_GRAVITY_ERROR_TEST


// precompile header 사용하는 소스코드 줄이기 테스트 
#define REDUCED_PRECOMPILED_HEADER_TEST






// p2p 패킷 사이즈 줄인 버전
//#define OPTIMIZED_P2P


// 캐릭터가 라인맵 사이를 점프, #define OPTIMIZED_P2P를 comment out 해야 합니다.
//#define LINKED_LINEMAP_JUMP_USER_TEST
#ifdef LINKED_LINEMAP_JUMP_USER_TEST
#undef OPTIMIZED_P2P
#endif LINKED_LINEMAP_JUMP_USER_TEST


// 몬스터가 라인맵 사이를 점프
//#define LINKED_LINEMAP_JUMP_MONSTER_TEST

// 레나 시즈모드 허리꺽기
//#define RENA_SIEGE_MODE_ANGLE_TEST


// 캐릭터 머리에 카메라 붙이기
//#define HEAD_ATTACHED_CAMERA_TEST


// m_FrameDataFuture.stateParam.bLandConnect 기능 제거하기 위해서, m_NPCFrameData.stateParam.bLandConnect 찾아서 사용하는 부분 삭제하거나, 캐릭터에서만 삭제하고 몬스터는 그대로 남겨둬도 될듯
//#define REMOVE_LAND_CONNECT_CONSTRAINTS


// 몬스터 위에 올라가기
//#define STEP_ON_MONSTER_TEST

// 몬스터 collsion box 밟고 서기
//#define STEP_ON_MONSTER_COLLISION_BOX_TEST



// FPS 화살 쏘기
//#define FPS_SHOOTING_TEST


// 얼굴 돌리기 테스트
//#define HEAD_INVERSE_KINEMATICS_TEST



// 마을에서 개발자는 캐릭터 빨리 달릴 수 있게
#define VILLAGE_FLASH_MAN_DEV_ONLY










// 게임 시작후 검은 화면 나오는것 일부 줄이기 테스트 
#define FIRST_SCREEN_TEST

// 불필요한 시스템 메세지 팝업 줄이기 테스트
#define HIDE_SYSTEM_MESSAGE


// 던전게임에서 필살기 쓸 때 시간 멈추는거 줄이기 테스트
//#define SPECIAL_ATTACK_DELAY_TEST

// CHECK opened device list, 리소스툴과 연계해서 매 200frame마다 열려있는 리소스 목록 체크
//#define CHECK_OPENED_DEVICE



// 열려있는 리소스 목록을 파일로 출력
#define NEW_CHECK_OPENED_DEVICE
//'H' 키를 누르면 현재 열려있는 리소스 목록을 파일로 저장해줍니다. opened resource list.txt
//마지막으로 H를 눌렀을 때와 바뀐 파일 목록을 리스트 해줍니다.  
//@@@@@@@@@@@@@@@@@@@@			<--- 새로 열린 파일 
//ref(1) size(1) 				<---- 이전에 'H' 키를 눌렀을 때의 referece count와 device size
//---------released ----------   		<---- 이전에 'H' 키를 눌렀을 때는 열려있었으나 지금은 닫힌 device list





// 게임 시작시 로딩 애니메이션 skip
//#define SKIP_INTRO_ANIMATION

// 게임 중간 로딩시 애니메이션
//#define LOADING_ANIMATION_TEST

// 게임 처음 시작할 때 로딩 별도 스레드로
//#define FIRST_GAME_LOADING_THREAD_TEST	


//{{ seojt // 2008-10-23, 18:48
// 정적 도움말 함수를 클래스에 자동으로 붙이는 기능의 테스트
// - jintaeks on 2008-10-23, 18:49
#define KHELP_SYSTEM_TEST
//}} seojt // 2008-10-23, 18:48

//#define MEMORY_LEAK_DETECTION_TEST







// damageeffect lua 테이블 한번에 다 로드해서 메모리에 올려두기, 
// 캐릭터와 몬스터용 damage effect를 구분 하자 
// 대전게임, 던전게임에서 필요한 damage effect만 불러오도록 하자
//#define DAMAGE_EFFECT_LOAD_TEST

// lua 테스트2
//#define LUA_TEST_TEST
// lua 테스트 
//#define LUA_TEST
// lua gameunit 테스트 
//#define LUA_GAMEUNIT_TEST


// 로그를 많이 남겨서 버그를 잡아보자~
//#define HEAVY_LOG_TEST




// 레이븐 공중 필살기
//#define RAVEN_AIR_SPECIAL_ATTACK_TEST




// 기존의 hand slash trace 없애기
//#define OLD_HAND_SLASH_TRACE












// singleton_pool test
#define BOOST_SINGLETON_POOL_TEST


// metal slug
//#define SHOOTING_TEST

// 파일 열기 실패 에러로그 남기기
//#define LOADREALFILE_ERROR_LOG

#ifdef LOADREALFILE_ERROR_LOG
#define MASS_FILE_FIRST
#endif 

// 던전 전체에 몬스터 정보 파일로 출력하기, 기획팀 테스트 용도 
// 사용방법: 아무던전이나 한번 들어가면 파일 출력됨~
//#define EXTRACT_ALL_DUNGEON_NPC_INFO




// EVE 통상기 여러가지 테스트
//#define EVE_COMBO_TEST_TEST


// 에러날 수 있는 코드 부분 잡기 위해서 로그 남기는 것
#define LEAVE_LAST_ERROR_LOG_TEST




// 유저가 키보드 입력 설정할 수 있게
//#define USER_DEFINED_KEYBOARD_SETTING





// 라인맵 바람 테스트 
//#define LINEMAP_SLOW_WIND_TEST
//#define LINEMAP_FAST_WIND_TEST


// 라인맵에 못 들어가는 사각형 영역 지정할 수 있게
//#define LINEMAP_RECT_TEST






// 오늘 하루 전적
//#define TODAY_RECORD_TEST


// 장비 탈의 테스트
//#define EQUIP_STRIP_TEST





// tag 테스트
//#define TAG_MATCH_TEST



// 대장전
//#define PVP_BOSS_COMBAT_TEST









// 어택 딜레이 게이지를 사용하는 곳이 없는 것 같아서 코드 삭제함
//#define ATTACK_DELAY_GAGE




// .kom 파일 사용하지 않고 클라이언트 테스트 할 때 사용, vld.h를 사용하거나, 디버그로 오픈테섭, 서비스 버전에 접속할 때 사용
// .kom파일이 없으면, 개당 5~10초를 기다리기 때문에 매우 느리다.
//#define WITHOUT_MASS_FILE_TEST





//주태욱

//KGCMassFileManager.cpp에 자동 버퍼 매니저 시스템을 포기해야지 사용할 수..?
//#define INTRUDE_THREAD_LOADING		// 진택이 형이 해주시니까 필요 없을듯
//#define DUNGEON_WORLD_THREAD_LOADING	// 진택이 형이 해주시니까 필요 없을듯
//#define MAKE_GGOBONG_TEST				// 몬스터 우리 편으로 만들게 끔 하는 테스트.
//#define DASH_CAMERA_ANGLE_TEST		// 대시 해서 가게 되면 카메라 앵글 조금씩 변화
//#define MONSTER_BABO_TEST				// 몬스터 반대로 보고 공격하게끔 



//#define THROW_WOODEN_PIECE_TEST				// 나무 조각 던지는 부분
//#define PROGRESSIVE_X_SKIN_MESH				// 프로그레시브 x스킨 메시 적용
//#define PROGRESSIVE_X_MESH					// 프로그레시브 메시 적용
//#define RIDE_MACHINE_GUN						// 탱크 올라타기
//#define FRAME_MOVE_CULL						// 멀어질 수록 프레임 무브 줄여서 부르게끔.
//#define UNIT_SYNC_PACKET_DUNGEON_FOR_TEST		// 퀵슬롯 아이템 관련해서 서브 스테이트 들어가는 부분
//#define LEAP_TEST								// 높이 점프 해서 공격
//#define	RIDE_HORSE							// 몬스터가 몬스터 올라타는거
//#define BEE_FLY								// 벌 날아가기




//#define SKILL_SHORT_CUT_KEY_TEST			// 스킬 단축키 작업

//#define X2VIEWER  // x2viewer 툴용 디파인 

// 김태완	
//#define WORLD_TOOL
//#define EFFECT_TOOL					// 오현빈 // 2013-02-20 // 월드툴에서 이펙트툴 기능 분리 및 이펙트 툴 개선 작업
//#ifdef EFFECT_TOOL
#if defined( EFFECT_TOOL ) || defined( X2VIEWER )
	#define WORLD_TOOL
#endif

//#define HALLOWEEN_EVENT			// 할로윈 이벤트 던전 데미지 보정 코드
//#define NEW_YEAR_EVENT_DUNGEON				// 설날 이벤트 던전 데미지 보정 코드
#define AUTH_AUTO_SELL_ITEM					// 운영자용 해당 카데고리 아이템 다 팔아버리기 기능. 테스트 용도로 급히 제작. 안정성 보장 못함!!

//#define X2TOOL

// 몇가지 치명적인 오류가 있어서 막음...
//#define SCRIPT_SELECT						// 스크립트 툴로 특정 던전/지역의 ON/OFF 설정가능하게 함. 해당 툴에 대응하는 코드 몇부분 수정.

//#define NEW_MESSENGER

//{{ robobeg : 2008-10-18	

// 레이어 테스트 오버헤드를 줄이기 위해, 사용하지 않을 때는 아래 매크로를 커맨트 아웃한다.
//    #define KTDGMANAGER_USE_DISABLE_LAYER  
//}} robobeg : 2008-10-18


// hoons // 2009-09-04 // 자동 콤보 & 자동 달리기 테스트
//#define TEST_MACRO_COMBO

//#define POST_QUANTITY_NEW				// 우편 수량 UI 개편

// cheat command 입력 shortcut key, '/' 키를 누르면 마지막으로 입력한 치트명령어가 실행됨
#define CHEAT_COMMAND_SHORT_CUT_KEY_TEST

// 몬스터 테스트용 몬스터 리스트 UI
#define CREATE_MONSTER_LIST_TEST


// 사운드 작업용 몬스터 상태 리스트 UI
#define MONSTER_STATE_LIST_TEST




// 캐릭터 위치를 강제로 이동시켜주는 함수, 현재는 사내테스트용으로만 사용하고 있어서 서비스버전에 포함 안되게 분리합니다. 
#define ENABLE_SET_UNIT_POSITION_TEST

//{{ 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용
//#define EVENT_SCENE_TEXT_USED_INDEX
//}} 허상형 : [2010/11/5/] //	에픽 퀘스트 텍스트를 인덱스를사용해 출력할때 사용

//#define RENDER_SKIP_TEST

//#define DOWN_FRAME_TEST // 대전/던전 프레임을 65 fps로 테스트 하기위해
//#define NUM_TEST_FRAME (65.f)


//#define FIX_BLUE_TEAM_MP_REGEN // 박진웅 : 대전 블루팀 마나 회복률 보너스 없던 문제 수정
//#define EXTRA_DAMAGE_CUT_NEGATIVE_RATE // 박진웅 : 엑스트라데미지 확률이 음수일 때 0으로 처리 (ASSERT 제거)
//#define SHOW_OPPONENT_MP_ONLY // 박진웅 : 고성능 고글 사용시 아군의 발밑에도 MP바 생기는 문제 수정
//#define RELEASE_ALL_BUTTONS_AND_KEYS_WHEN_FOCUS_LOSING	// 박진웅(북미) : 포커스를 잃을 때 입력 초기화
//#define FIX_BACKJUMP_SHOT_FREEZE // 박진웅 : 각성 상태에서 낙하 중 백덤블링샷 시 공중에서 굳는 문제 수정(hyperStateData.StateEndFuture 등록 실수)

//#define MAGIC_NEC_FIX // 이지헌(대만/홍콩) : 마법의 목걸이 개편

#define PREVENT_SUBSTR_ERROR // 2011.09.15 이준호 substr error check
//#define BANDICAM_CAPTURE // 2011.9.16	이준호  반디캠 스크린샷 캡쳐 지원
//#define BANDICAM_RECORDING // 2011.9.16 이준호 반디캠 작업
//#define FILDE_ONLY_CHARACTER //2011.11.03 이준호 X_x88 필드에서 오직 케릭터만 보입니다
//#define POSTBOX_CANCEL_QUANTITY_ITEM_FIX	/// 11.09.26 임규수 우편 수량성 아이템 첨부 취소 오류 수정

//#define FIX_SOUND_DEVICE						// 11.11.28 하형진 사운드 디바이스 문제 관련 수정
//#define REMOVE_DRAW_PRIMITIVE_UP_3				// 11.11.28 하형진 DrawPrimitiveUp 함수 수정
//#define KTDGDEVICEFONT_SIMULATE_DIRECTX_FONT	// 11.11.28 하형진 자체 폰트에서 기존 directx 폰트 기능 시뮬레이션
//#define UINPC_TO_SMARTPTR						// 11.11.28 하형진 UI NPC 스마트 포인트화
//#define CLEARTYPE_FONT							// 11.12.01 박교현 Clear Type 테스트
//#define DYNAMIC_VERTEX_BUFFER_OPT				// 11.12.12 하형진 Vertext Buffer 최적화
//#define KTDGDEVICEFONT_BACKGROUND_PRELOADING	// 11.12.12 하형진 Device Font Background Loading

// 박진웅 : 특수 아이템 사용
//#define SPECIAL_USE_ITEM

// robobeg : 하형진 [2011.12.20] //
//#define KTDGDEVICEFONT_SIZE_CACHE

//#define SLASH_TRACE_BUG_FIX			// 11.12.27 하형진 무기의 검흔 오류 수정 내용 병합
//#define REMOVE_DXUT_SAMPLE_CODE		// 11.12.27 하형진
//#define DXUT_KTDXLIB_SAFE_ON_RESET	// 11.12.27 하형진 

// 2011.12.14 박진웅 : 메디테이션달리기 수정
//#define FIX_MEDITATION_RUN

//#define DAMAGE_EFFECT_BUG_FIX

#define CAMILLA_SMASH					// 해외팀 추가 소켓

// 박진웅 : 선물하기 때 현재 넥슨 캐시량에 콤마 않찍힌 문제 수정
//#define ADD_COMMA_TO_CASH_ITEM

// 임규수 : 빈틈없는 반격 레벨업시 데미지 반영 안되는 문제 수정
//#define COMPACT_COUNTER_FIX

// 박진웅 : 브루탈스윙 메모 푸시앤푸시 첫타 데미지 적용 안되는 오류 수정
//#define  MEMO_FIX_PUSH_AND_PUSH

// 이지헌 : ALLY NPC 가 유저를 타겟 잡는 문제 수정
//#define EVOKE_TARGETING_BUG_FIX

#define NOT_USE_UDP_CHECK_INHOUSE		/// 랜선랙 사내에서는 작동 안하도록!

