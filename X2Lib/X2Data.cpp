#include "stdafx.h"
#include ".\x2data.h"

//#ifdef USER_DEFINED_KEYBOARD_SETTING
//	unsigned char CX2Data::s_KeyMappingLayer[2][256];
//#endif USER_DEFINED_KEYBOARD_SETTING


struct CharacterImageName
{
	wstring fileName;
	wstring pieceName;

	CharacterImageName() 
	: fileName( L"" )
	, pieceName( L"" )
	{
	}

	CharacterImageName( const wstring& fileName_, const wstring& pieceName_ )
	: fileName( fileName_ )
	, pieceName( pieceName_ )
	{
	}
};

// 인덱스로 접근하기 때문에 순서가 변경되면 안됩니다.


#pragma region s_CharacterImageNameMyGage
static const CharacterImageName s_CharacterImageNameMyGage[] = 
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"ELSWORD_STATE" ),		//0
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"S_KNIGHT_STATE"	),	//1
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"M_KNIGHT_STATE"	),	//2
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"AISHA_STATE"	),		//3
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"H_MAGICIAN_STATE"	),	//4
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"D_MAGICIAN_STATE"	),	//5
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"RENA_STATE"	),		//6
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"C_RANGER_STATE"	),	//7
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"S_RANGER_STATE"	),	//8
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"RAVEN_STATE"	),		//9
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"S_TAKER_STATE"	),	//10
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"O_TAKER_STATE"	),	//11
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"EVE_STATE"	),		//12
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_E_STATE"		),	//13
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_A_STATE"		),	//14
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"L_KNIGHT_STATE"	),	//15
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"R_SLAYER_STATE"	),	//16
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"W_SNEAKER_STATE"	),	//17
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"G_ARCHER_STATE"	),	//18
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"E_MASTER_STATE"	),	//19
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"V_PRINCESS_STATE"	),	//20
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"B_MASTER_STATE"	),	//21
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"R_FIST_STATE"		),	//22
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_NEMESIS_STATE"	),	//23
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_EMPRESS_STATE"	),	//24
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"CHUNG_STATE_FACE"	),	//25
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"F_GUARDIAN_FACE"	),	//26
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"S_GUARDIAN_FACE"	),	//27
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"I_PALADIN_FACE"	),	//28
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"D_CHASER_FACE"	),	//29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"T_KNIGHT_STATE" 	),	//30
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"B_MAGICIAN_STATE"	),	//31
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"T_RANGER_STATE"	),	//32
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"W_TAKER_STATE"	),	//33
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_L_STATE"		),	//34
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"SH_GUARDIAN_FACE"),	//35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture66_NEW.tga", 	L"I_SWORD_FACE" 	),	//36
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.TGA", 	L"D_WITCH_STATE" 	),	//37
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.TGA", 	L"N_ARCHER_STATE" 	),	//38
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.TGA", 	L"V_COMMANDER_STATE"),	//39
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga", 	L"B_SERAPH_STATE"	),	//40 
	CharacterImageName( L"DLG_UI_Common_Texture66_NEW.TGA", 	L"T_TROOPER_FACE"	),	//41
	#pragma endregion 신 2차전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga",		L"ARA_FACE"	),		//42
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga", 	L"ARA_LH_FACE"	),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture70_NEW.tga", 	L"ARA_SD_FACE"	),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직

	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga",		L"EL_FACE"	),		//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SSK_FACE"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SPK_FACE"	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga",		L"L_DEVIL_STATE"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga", 	L"Y_RAJA_STATE"		),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#pragma region 엘리시스 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga",		L"EL_SGM_FACE"		),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga", 	L"EL_SBH_FACE"		),	//51 블레이징 하트
#pragma endregion 엘리시스 1-2, 2-2 신 전직


#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture80_NEW.tga",		L"ADD_FACE"	),		//52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_APT_FACE"	),	//53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_ALP_FACE"	),	//54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 유저 Gage UI

#pragma region s_CharacterImageName160
static const CharacterImageName s_CharacterImageName160[] = 
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"ELSWORD_LARGE"	),	// 0
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"S_KNIGHT_LARGE"	),	// 1
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"M_KNIGHT_LARGE"	),	// 2
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"AISHA_LARGE"		),	// 3
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"H_MAGICIAN_LARGE"	),	// 4
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"D_MAGICIAN_LARGE"	),	// 5
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"RENA_LARGE"		),	// 6
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"C_RANGER_LARGE"	),	// 7
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"S_RANGER_LARGE"	),	// 8
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"RAVEN_LARGE"		),	// 9
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"S_TAKER_LARGE"	),	// 10
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"O_TAKER_LARGE"	),	// 11
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"EVE_LARGE"		),	// 12
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"CODE_E_LARGE"		),	// 13
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_A_LARGE"		),	// 14
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"L_KNIGHT_LARGE"	),	// 15
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"R_SLAYER_LARGE"	),	// 16
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"W_SNEAKER_LARGE"	),	// 17
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"G_ARCHER_LARGE"	),	// 18
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"E_MASTER_LARGE"	),	// 19
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"V_PRINCESS_LARGE"	),	// 20
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"B_MASTER_LARGE"	),	// 21
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"R_FIST_LARGE"		),	// 22
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"C_NEMESIS_LARGE"	),	// 23
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"C_EMPRESS_LARGE"	),	// 24
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"CHUNG_LARGE"	),		// 25		
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"F_GUARDIAN_LARGE"),	// 26
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"S_GUARDIAN_LARGE"),	// 27
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"I_PALADIN_LARGE"),	// 28
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"D_CHASER_LARGE"),		// 29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"T_KNIGHT_LARGE"	),	// 30
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"B_MAGICIAN_LARGE"	),	// 31
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"T_RANGER_LARGE"	),	// 32
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"W_TAKER_LARGE"	),	// 33
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"CODE_L_LARGE"		),	// 34
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"SH_GUARDIAN_LARGE"),	// 35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture69.tga",		L"I_SWORD_LARGE"	),	// 36
	CharacterImageName( L"DLG_UI_Common_Texture62.tga",		L"D_WITCH_LARGE"	),	// 37
	CharacterImageName( L"DLG_UI_Common_Texture63.tga",		L"N_WATCHER_LARGE"	),	// 38
	CharacterImageName( L"DLG_UI_Common_Texture64.tga",		L"V_COMMANDER_LARGE"), 	// 39
	CharacterImageName( L"DLG_UI_Common_Texture65.tga",		L"B_SERAPH_LARGE"	),	// 40
	CharacterImageName( L"DLG_UI_Common_Texture45.tga",		L"T_TROOPER_LARGE"	),	// 41
	#pragma endregion 신 2차 전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga",		L"ARA_LARGE"	),	//42
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga", 	L"ARA_LH_LARGE"	),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture70_NEW.tga", 	L"ARA_SD_LARGE"	),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직

	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga",		L"EL_LARGE"	),	//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SSK_LARGE"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SPK_LARGE"	),	//47 엘리시스 파이로 나이트
	
#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga",		L"L_DEVIL_LARGE"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga", 	L"Y_RAJA_LARGE"		),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND


#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#pragma region 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga",		L"EL_SGM_LARGE"	),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga", 	L"EL_SBH_LARGE"		),	//51 블레이징 하트
#pragma endregion 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture80_NEW.tga",		L"ADD_LARGE"	),		//52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_APT_LARGE"	),	//53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_ALP_LARGE"	),	//54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
	#pragma endregion 큰 초상화 이미지_160x160

#pragma region s_CharacterImageName160Gray
static const CharacterImageName s_CharacterImageName160Gray[] = 
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"ELSWORD_LARGE_GRAY"	),	// 0
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"S_KNIGHT_LARGE_GRAY"	),	// 1
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"M_KNIGHT_LARGE_GRAY"	),	// 2
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"AISHA_LARGE_GRAY"		),	// 3
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"H_MAGICIAN_LARGE_GRAY"),	// 4
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"D_MAGICIAN_LARGE_GRAY"),	// 5
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"RENA_LARGE_GRAY"		),	// 6
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"C_RANGER_LARGE_GRAY"	),	// 7
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"S_RANGER_LARGE_GRAY"	),	// 8
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"RAVEN_LARGE_GRAY"		),	// 9
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"S_TAKER_LARGE_GRAY"	),	// 10
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"O_TAKER_LARGE_GRAY"	),	// 11
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"EVE_LARGE_GRAY"		),	// 12
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"CODE_E_LARGE_GRAY"	),	// 13
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"CODE_A_LARGE_GRAY"	),	// 14
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"L_KNIGHT_LARGE_GRAY"	),	// 15
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"R_SLAYER_LARGE_GRAY"	),	// 16
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"W_SNEAKER_LARGE_GRAY"	),	// 17
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"G_ARCHER_LARGE_GRAY"	),	// 18
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"E_MASTER_LARGE_GRAY"	),	// 19
	CharacterImageName( L"DLG_UI_Common_Texture22_NEW.tga",		L"V_PRINCESS_LARGE_GRAY"),	// 20
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"B_MASTER_LARGE_GRAY"	),	// 21
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"R_FIST_LARGE_GRAY"	),	// 22
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"C_NEMESIS_LARGE_GRAY"	),	// 23
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"C_EMPRESS_LARGE_GRAY"	),	// 24
#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"CHUNG_LARGE_GRAY"	),		// 25
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"F_GUARDIAN_LARGE_GRAY"),	// 26
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"S_GUARDIAN_LARGE_GRAY"),	// 27
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"I_PALADIN_LARGE_GRAY"),	// 28
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"D_CHASER_LARGE_GRAY"),	// 29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture17_NEW.tga",		L"T_KNIGHT_LARGE_GRAY"	),	// 30
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"B_MAGICIAN_LARGE_GRAY"),	// 31
	CharacterImageName( L"DLG_UI_Common_Texture24_NEW.tga",		L"T_RANGER_LARGE_GRAY"	),	// 32
	CharacterImageName( L"DLG_UI_Common_Texture44_NEW.tga",		L"W_TAKER_LARGE_GRAY"	),	// 33
	CharacterImageName( L"DLG_UI_Common_Texture56_NEW.tga",		L"CODE_L_LARGE_GRAY"	),	// 34
	CharacterImageName( L"DLG_UI_Common_Texture62_NEW.tga",		L"SH_GUARDIAN_LARGE_GRAY"),	// 35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture69.tga",		L"I_SWORD_GRAY"		),	// 36
	CharacterImageName( L"DLG_UI_Common_Texture62.tga",		L"D_WITCH_GREY"		),	// 37
	CharacterImageName( L"DLG_UI_Common_Texture63.tga",		L"N_WATCHER_GRAY"	),	// 38
	CharacterImageName( L"DLG_UI_Common_Texture64.tga",		L"V_COMMANDER_GRAY"	),	// 39
	CharacterImageName( L"DLG_UI_Common_Texture65.tga",		L"B_SERAPH_GRAY"	),	// 40
	CharacterImageName( L"DLG_UI_Common_Texture45.tga",		L"T_TROOPER_GRAY"	),	// 41
	#pragma endregion 신 2차 전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga",		L"ARA_GRAY"	),		//42
	CharacterImageName( L"DLG_UI_Common_Texture70_NEW.tga", 	L"ARA_LH_GRAY"),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture70_NEW.tga", 	L"ARA_SD_GRAY"),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직

	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga",		L"EL_GRAY"	),	//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SSK_GRAY"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SPK_GRAY"	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga",		L"L_DEVIL_LARGE_GRAY"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga", 	L"Y_RAJA_LARGE_GRAY"	),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#pragma region 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga",		L"EL_SGM_GRAY"	),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga", 	L"EL_SBH_GRAY"	),	//51 블레이징 하트
#pragma endregion 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )

#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture80_NEW.tga",		L"ADD_GRAY"		),	//52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_APT_GRAY"	),	//53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_ALP_GRAY"	),	//54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 큰 초상화 회색 이미지_160x160

#pragma region s_CharacterImageName50
static const CharacterImageName s_CharacterImageName50[] = 
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"ELSWORD_SMALL"),		///0
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"S_KNIGHT_SMALL"	),	//1
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"M_KNIGHT_SMALL"	),	//2
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"AISHA_SMALL"	),		//3
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"H_MAGICIAN_SMALL"	),	//4
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"D_MAGICIAN_SMALL"	),	//5
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"RENA_SMALL"	),		//6
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"C_RANGER_SMALL"	),	//7
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"S_RANGER_SMALL"	),	//8
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"RAVEN_SMALL"	),		//9
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"S_TAKER_SMALL"	),	//10
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"O_TAKER_SMALL"	),	//11
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"EVE_SMALL"	),		//12
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_E_SMALL"		),	//13
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_A_SMALL"		),	//14
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"L_KNIGHT_SMALL"	),	//15
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"R_SLAYER_SMALL"	),	//16
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"W_SNEAKER_SMALL"	),	//17 
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"G_ARCHER_SMALL"	),	//18
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"E_MASTER_SMALL"	),	//19
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"V_PRINCESS_SMALL"	),	//20
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"B_MASTER_SMALL"	),	//21
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"R_FIST_SMALL"		),	//22
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_NEMESIS_SMALL"	),	//23
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_EMPRESS_SMALL"	),	//24
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"CHUNG_SMALL"		),	//25
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"F_GUARDIAN_SMALL"	),	//26
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"S_GUARDIAN_SMALL"	),	//27
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"I_PALADIN_SMALL"	),	//28
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"D_CHASER_SMALL"	),	//29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"T_KNIGHT_SMALL"	),  //30
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"B_MAGICIAN_SMALL"	),  //31
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"T_RANGER_SMALL"	),  //32
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"W_TAKER_SMALL"	),	//33
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_L_SMALL"		),	//34
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"SH_GUARDIAN_SMALL"),  //35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture69.tga", 		L"I_SWORD_MEDIUM"    ),	//36
	CharacterImageName( L"DLG_UI_Common_Texture62.tga", 		L"D_WITCH_MEDIUM"    ),	//37
	CharacterImageName( L"DLG_UI_Common_Texture63.tga", 		L"N_WATCHER_MEDIUM"  ),	//38
	CharacterImageName( L"DLG_UI_Common_Texture64.tga", 		L"V_COMMANDER_MEDIUM"),	//39
	CharacterImageName( L"DLG_UI_Common_Texture65.tga", 		L"B_SERAPH_MEDIUM"	 ),	//40
	CharacterImageName( L"DLG_UI_Common_Texture45.tga", 		L"T_TROOPER_MEDIUM"  ),	//41
	#pragma endregion 신 2차 전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga",		L"ARA_MEDIUM"	),	//42
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga", 	L"ARA_LH_MEDIUM"	),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture70_NEW.tga", 	L"ARA_SD_MEDIUM"	),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직

	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga",		L"EL_MEDIUM"	),	//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SSK_MEDIUM"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SPK_MEDIUM"	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga",		L"L_DEVIL_MEDIUM"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga", 	L"Y_RAJA_MEDIUM"	),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#pragma region 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga",		L"EL_SGM_MEDIUM"	),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga", 	L"EL_SBH_MEDIUM"	),	//51 블레이징 하트
#pragma endregion 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture80_NEW.tga",		L"ADD_MEDIUM"	),		//52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_APT_MEDIUM"	),	//53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_ALP_MEDIUM"	),	//54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 중간 초상화 회색 이미지_50x50

#pragma region s_CharacterImageName20
static const CharacterImageName s_CharacterImageName20[] = 
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"ELSWORD_ICON"		), //0
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"S_KNIGHT_ICON"	), //1
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"M_KNIGHT_ICON"	), //2
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"AISHA_ICON"		), //3
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"H_MAGICIAN_ICON"	), //4
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"D_MAGICIAN_ICON"	), //5
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"RENA_ICON"		), //6
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"C_RANGER_ICON"	), //7
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"S_RANGER_ICON"	), //8
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"RAVEN_ICON"		), //9
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"S_TAKER_ICON"		), //10
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"O_TAKER_ICON"		), //11
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"EVE_ICON"			), //12
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_E_ICON"		), //13
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_A_ICON"		), //14
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"L_KNIGHT_ICON"	), //15
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"R_SLAYER_ICON"	), //16
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"W_SNEAKER_ICON"	), //17
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"G_ARCHER_ICON"	), //18
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"E_MASTER_ICON"	), //19
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"V_PRINCESS_ICON"	), //20
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"B_MASTER_ICON"	), //21
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"R_FIST_ICON"		), //22
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_NEMESIS_ICON"	), //23
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_EMPRESS_ICON"	), //24
#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"CHUNG_ICON"		), //25
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"F_GUARDIAN_ICON"	), //26
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"S_GUARDIAN_ICON"	), //27
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"I_PALADIN_ICON"	), //28
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"D_CHASER_ICON"	), //29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",    	L"T_KNIGHT_ICON"	), //30
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"B_MAGICIAN_ICON"	), //31
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"T_RANGER_ICON"	), //32
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"W_TAKER_ICON"		), //33
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_L_ICON"		), //34
	CharacterImageName( L"DLG_UI_Common_Texture61_NEW.tga",		L"SH_GUARDIAN_ICON" ), //35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture69.tga",			L"I_SWORD_ICON"		), //36
	CharacterImageName( L"DLG_UI_Common_Texture62.tga",			L"D_WITCH_ICON"		), //37
	CharacterImageName( L"DLG_UI_Common_Texture63.tga",			L"N_WATCHER_ICON"	), //38
	CharacterImageName( L"DLG_UI_Common_Texture64.tga",			L"V_COMMANDER_ICON"	), //39
	CharacterImageName( L"DLG_UI_Common_Texture65.tga",			L"B_SERAPH_ICON"	), //40
	CharacterImageName( L"DLG_UI_Common_Texture45.tga",			L"T_TROOPER_ICON"	), //41
	#pragma endregion 신 2차 전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga",		L"ARA_ICON"	),	//42
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga", 	L"ARA_LH_ICON"	),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture70_NEW.tga", 	L"ARA_SD_ICON"	),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직

	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga",		L"EL_ICON"	),	//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SSK_ICON"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SPK_ICON"	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga",		L"L_DEVIL_ICON"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga", 	L"Y_RAJA_ICON"	),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#pragma region 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga",		L"EL_SGM_ICON"	),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga", 	L"EL_SBH_ICON"	),	//51 블레이징 하트 
#pragma endregion 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture80_NEW.tga",		L"ADD_ICON"	),		//52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_APT_ICON"	),	//53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_ALP_ICON"	),	//54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 작인 초상화 이미지_20x20

#pragma region s_CharacterImageNameDungeonClear
// 던전 clear 하고 나오는 던전 대사 나올때 큰 캐릭터 이미지
// D:\ProjectX2_SVN\resource\KR\Trunk\dat\Texture\Effect\CutIn
CharacterImageName s_CharacterImageNameDungeonClear[] = 
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"HQ_Dungeon_Clear_Elsword.tga",				L""			),	 //0
	CharacterImageName( L"HQ_Dungeon_Clear_Elsword_Sword.tga",			L""			),	 //1
	CharacterImageName( L"HQ_Dungeon_Clear_Elsword_Magic.tga",			L""			),	 //2
	CharacterImageName( L"HQ_Dungeon_Clear_Aisha.tga",					L""			),	 //3
	CharacterImageName( L"HQ_Dungeon_Clear_Aisha_High.tga",				L""			),	 //4
	CharacterImageName( L"HQ_Dungeon_Clear_Aisha_Dark.tga",				L""			),	 //5
	CharacterImageName( L"HQ_Dungeon_Clear_Rena.tga",					L""			),	 //6
	CharacterImageName( L"HQ_Dungeon_Clear_Rena_Combat.tga",			L""			),	 //7
	CharacterImageName( L"HQ_Dungeon_Clear_Rena_Sniping.tga",			L""			),	 //8
	CharacterImageName( L"HQ_Dungeon_Clear_Raven.tga",					L""			),	 //9
	CharacterImageName( L"HQ_Dungeon_Clear_Raven_SoulTaker.tga",		L""			),	 //10
	CharacterImageName( L"HQ_Dungeon_Clear_Raven_OverTaker.tga",		L""			),	 //11
	CharacterImageName( L"HQ_Dungeon_Clear_Eve.tga",					L""			),	 //12
	CharacterImageName( L"HQ_Dungeon_Clear_Eve_Exotic.tga",				L""			),	 //13
	CharacterImageName( L"HQ_Dungeon_Clear_Eve_Architecture.tga",		L""			),	 //14
	CharacterImageName( L"HQ_Dungeon_Clear_Elsword_LordKnight.tga",		L""			),	 //15
	CharacterImageName( L"HQ_Dungeon_Clear_Elsword_RuneSlayer.tga",		L""			),	 //16
	CharacterImageName( L"HQ_Dungeon_Clear_Rena_Wind_Sneaker.tga",		L""			),	 //17
	CharacterImageName( L"HQ_Dungeon_Clear_Rena_Grand_Archer.tga",		L""			),	 //18		
	CharacterImageName( L"HQ_Dungeon_Clear_Aisha_Elemental_Master.tga",	L""			),	 //19
	CharacterImageName( L"HQ_Dungeon_Clear_Aisha_Void_Princess.tga",	L""			),	 //20
	CharacterImageName( L"HQ_Dungeon_Clear_Raven_BladeMaster.tga",		L""			),	 //21
	CharacterImageName( L"HQ_Dungeon_Clear_Raven_RecklessFist.tga",		L""			),	 //22
	CharacterImageName( L"HQ_Dungeon_Clear_Eve_Nemesis.tga",			L""			),	 //23
	CharacterImageName( L"HQ_Dungeon_Clear_Eve_Empress.tga",			L""			),	 //24
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"HQ_Dungeon_Clear_Chung_Iron_Cannon_Normal.TGA", L""		),	//25
	CharacterImageName( L"HQ_Dungeon_Clear_Chung_CFG_Normal.TGA",			L""		),	//26
	CharacterImageName( L"HQ_Dungeon_Clear_Chung_CSG_Normal.TGA",			L""		),	//27
	CharacterImageName( L"DDS_Chung_CIP_Clear_Normal.tga",			L""			),		//28
	CharacterImageName( L"DDS_Chung_CDC_Clear_Normal.tga",			L""			),		//29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"HQ_Dungeon_Clear_Elsword_Sheath.tga",			L""			),	//30
	CharacterImageName( L"HQ_Dungeon_Clear_Aisha_Battle_Magician.tga",			L""	),	//31
	CharacterImageName( L"HQ_Dungeon_Clear_Rena_Trapping_Ranger.tga",			L""	),	//32
	CharacterImageName( L"HQ_Dungeon_Clear_Raven_Weapon_Taker.tga",		L""			),	//33
	CharacterImageName( L"HQ_Dungeon_Clear_Eve_ELECTRA.tga",			L""			),	//34
	CharacterImageName( L"HQ_Dungeon_Clear_Chung_SHG_Normal.tga",			L""		),	//35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"HQ_Dungeon_Clear_Elsword_InfinitySword.tga",		L""		),	//36
	CharacterImageName( L"HQ_Dungeon_Clear_Aisha_DimensionWitch.tga",		L""		),	//37
	CharacterImageName( L"HQ_Dungeon_Clear_Rena_Night_Watcher_1.tga",		L""		),	//38
	CharacterImageName( L"HQ_Dungeon_Clear_Raven_Veteran_Commander.tga",		L""	),	//39
	CharacterImageName( L"HQ_Dungeon_Clear_Eve_Battle_Seraph.tga",			L""		),	//40
	CharacterImageName( L"HQ_Dungeon_Clear_Chung_TT_Normal.tga",		L""			),	//41
	#pragma endregion 신 2차 전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"HQ_Dungeon_Clear_Ara_Martial_Artist_Normal.tga",	L""	),	//42
	CharacterImageName( L"HQ_Dungeon_Clear_Ara_LITTLE_HSIEN_Normal.tga", 	L""	),	//43 소선
	CharacterImageName( L"HQ_Dungeon_Clear_Ara_Sakra_Devanam_Normal.tga", 	L""	),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직

	CharacterImageName( L"HQ_Dungeon_Clear_Elesis.tga",		L""	),				//45 엘리시스 노전
	CharacterImageName( L"HQ_Dungeon_Clear_Elesis_Saber_Knight.tga", 	L""	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"HQ_Dungeon_Clear_Elesis_Pyro_Knight.tga", 	L""	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"HQ_Dungeon_Clear_Ara_Little_Devil_Normal.tga",	L""	),	//48 소마
	CharacterImageName( L"HQ_Dungeon_Clear_Ara_Yama_Raja_Normal.tga",		L""	),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND
	
#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#pragma region 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"HQ_Dungeon_Clear_Elesis_Grand_Master_Normal.tga",		L""	),	//50 그랜드 마스터
	CharacterImageName( L"HQ_Dungeon_Clear_Elesis_Blazing_Heart_Normal.tga",	L""	),	//51 블레이징 하트
#pragma endregion 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"HQ_Dungeon_Clear_Add.tga",	L""			),	 //52 애드 기본 전직
	CharacterImageName( L"HQ_Dungeon_Clear_Add_APT.tga",	L""		),	 //53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"HQ_Dungeon_Clear_Add_ALP.tga",	L""		),	 //54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 던전 클리어 이미지

#pragma region s_CharacterImageNameInitial
CharacterImageName s_CharacterImageNameInitial[] =
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L""			),	 //0
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_S"  ),	 //1
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_M"  ),	 //2
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L""   		),	 //3
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_H"  ),	 //4
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_D"  ),	 //5
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L""			),	 //6
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_C"  ),	 //7
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_S"  ),	 //8
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L""			),	 //9
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_S"  ),	 //10
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_O"  ),	 //11
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"" 		),	 //12
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_E"	),	 //13
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_A"	),	 //14
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_L"	),	 //15
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_R"	),	 //16
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"Class_W"	),	 //17			
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"Class_G"	),	 //18	
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"CLASS_E" 	),	 //19
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"Class_V" 	),	 //20
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"Class_B" 	),	 //21
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"Class_R" 	),	 //22
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_N"	),	 //23
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_E"	),	 //24
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L""			),	//25
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_F"	),	//26
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_S"	),	//27
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_I"	),	//28
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_D"	),	//29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_T"   ),	//30
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"Class_B"	), 	//31
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_T"	), 	//32
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"Class_W"	),	//33		
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_E"	), 	//34
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_H"	), 	//35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_I"	),	//36
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_D"	),	//37
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_N"	),	//38
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_V"	),	//39
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_B"	),	//40
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_T"	),	//41
	#pragma endregion 신 2차 전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L""	),	//42
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_L"	),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_S"	),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직

	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L""	),		//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_S"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_P"	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_D"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_Y"	),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_G"	),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture11.tga", 	L"CLASS_H"	),	//51 블레이징 하트
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"" 		),	 //52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"CLASS_P" 	),	 //53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture11.tga",		L"CLASS_L" 	),	 //54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 직업 관련 글자

#pragma region s_CharacterImageNameParty
static const CharacterImageName s_CharacterImageNameParty[] = 
{
	#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"ELSWORD_PARTY"	), //0
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"S_KNIGHT_PARTY"	), //1
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"M_KNIGHT_PARTY"	), //2
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"AISHA_PARTY"		), //3
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"H_MAGICIAN_PARTY"	), //4
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"D_MAGICIAN_PARTY"	), //5
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"RENA_PARTY"		), //6
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"C_RANGER_PARTY"	), //7
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"S_RANGER_PARTY"	), //8
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"RAVEN_PARTY"		), //9
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"S_TAKER_PARTY"	), //10
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"O_TAKER_PARTY"	), //11
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"EVE_PARTY"		), //12
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_E_PARTY"		), //13
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_A_PARTY"		), //14
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"L_KNIGHT_PARTY"	), //15
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga",		L"R_SLAYER_PARTY"	), //16
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"W_SNEAKER_PARTY"	), //17
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"G_ARCHER_PARTY"	), //18
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"E_MASTER_PARTY"	), //19
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"V_PRINCESS_PARTY"	), //20
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"B_MASTER_PARTY"	), //21
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"R_FIST_PARTY"		), //22
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_NEMESIS_PARTY"	), //23
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"C_EMPRESS_PARTY"	), //24
	#pragma endregion 노전직, 1차전직, 2차전직
	
	#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture63_NEW.tga",		L"CHUNG_PARTY"		), //25
	CharacterImageName( L"DLG_UI_Common_Texture63_NEW.tga",		L"F_GUARDIAN_PARTY"	), //26
	CharacterImageName( L"DLG_UI_Common_Texture63_NEW.tga",		L"S_GUARDIAN_PARTY"	), //27
	CharacterImageName( L"DLG_UI_Common_Texture63_NEW.tga",		L"I_PALADIN_PARTY"	), //28
	CharacterImageName( L"DLG_UI_Common_Texture63_NEW.tga",		L"D_CHASER_PARTY"	), //29
	#pragma endregion 노전직, 1차전직, 2차전직

	#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture16_NEW.tga", 	L"T_KNIGHT_PARTY" 	), //30
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.tga",		L"B_MAGICIAN_PARTY"	), //31
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.tga",		L"T_RANGER_PARTY"	), //32
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.tga",		L"W_TAKER_PARTY"	), //33
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga",		L"CODE_L_PARTY"		), //34
	CharacterImageName( L"DLG_UI_Common_Texture63_NEW.tga",		L"SH_GUARDIAN_PARTY"), //35
	#pragma endregion 신 1차전직

	#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture66_NEW.tga", 	L"I_SWORD_PARTY" 	),	//36
	CharacterImageName( L"DLG_UI_Common_Texture21_NEW.TGA", 	L"D_WITCH_PARTY" 	),	//37
	CharacterImageName( L"DLG_UI_Common_Texture23_NEW.TGA", 	L"N_ARCHER_PARTY" 	),	//38
	CharacterImageName( L"DLG_UI_Common_Texture25_NEW.TGA", 	L"V_COMMANDER_PARTY"),	//39
	CharacterImageName( L"DLG_UI_Common_Texture45_NEW.tga", 	L"B_SERAPH_PARTY"	),	//40 
	CharacterImageName( L"DLG_UI_Common_Texture66_NEW.TGA", 	L"T_TROOPER_PARTY"	),	//41
	#pragma endregion 신 2차 전직

	#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga",		L"ARA_PARTY"	),	//42
	CharacterImageName( L"DLG_UI_Common_Texture68_NEW.tga",		L"ARA_LH_PARTY"	),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture70_NEW.tga",		L"ARA_SD_PARTY"	),	//44 제천
	#pragma endregion 노전직, 1차전직, 2차전직


	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga",		L"EL_PARTY"	),	//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SSK_PARTY"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture74_NEW.tga", 	L"EL_SPK_PARTY"	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga", 	L"L_DEVIL_STATE_PARTY"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture77_NEW.tga", 	L"Y_RAJA_STATE_PARTY"	),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#pragma region 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga", 	L"EL_SGM_PARTY"	),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture76_NEW.tga", 	L"EL_SBH_PARTY"	),	//51 블레이징 하트
#pragma endregion 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture80_NEW.tga",		L"ADD_PARTY"		), //52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_APT_PARTY"	), //53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture81_NEW.tga",		L"ADD_ALP_PARTY"	), //54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 파티원 이미지


#pragma region s_CharacterImageUnitSlot
static const CharacterImageName s_CharacterImageUnitSlot[] = 
{
#pragma region 엘소드, 아이샤, 레나, 레이븐, 이브
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELSWORD"	), //0
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELSWORD_SW_KNIGHT"	), //1
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELSWORD_M_KNIGHT"	), //2
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_AISHA"		), //3
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_AISHA_H_MAGICIAN"	), //4
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_AISHA_D_MAGICIAN"	), //5
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RENA"		), //6
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RENA_C_RANGER"	), //7
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RENA_S_RANGER"	), //8
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RAVEN"		), //9
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RAVEN_S_TAKER"	), //10
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RAVEN_O_TAKER"	), //11
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_EVE"		), //12
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_EVE_EXOTIC"		), //13
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_EVE_ARCHITECTURE"		), //14
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELSWORD_LO_KNIGHT"	), //15
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELSWORD_R_SLAYER"	), //16
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RENA_W_SNEAKER"	), //17
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RENA_G_ARCHER"	), //18
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_AISHA_E_MASTER"	), //19
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_AISHA_V_PRINCESS"	), //20
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RAVEN_B_MASTER"	), //21
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RAVEN_R_FIST"		), //22
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_EVE_NEMESIS"	), //23
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_EVE_EMPRESS"	), //24
#pragma endregion 노전직, 1차전직, 2차전직

#pragma region 청 (아이언 캐논, 퓨리 가디언, 슈팅 가디언, 아이언 팔라딘, 데들리 체이서)
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_CHUNG"		), //25
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_CHUNG_F_GUARDIAN"	), //26
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_CHUNG_S_GUARDIAN"	), //27
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_CHUNG_I_PALADIN"	), //28
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_CHUNG_D_CHASER"	), //29
#pragma endregion 노전직, 1차전직, 2차전직

#pragma region 시쓰 나이트, 배틀 매지션, 트래핑 레인저, 웨폰 테이커, 쉘링 가디언
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELSWORD_S_KNIGHT" 	), //30
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_AISHA_B_MAGICIAN"	), //31
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RENA_T_RANGER"	), //32
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_RAVEN_W_TAKER"	), //33
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_EVE_ELECTRA"		), //34
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_CHUNG_SHEL_GUARDIAN"), //35
#pragma endregion 신 1차전직

#pragma region 인피니티 소드, 디멘션 위치, 나이트 와쳐, 베터랑 코만도, 배틀 세라프, 택티컬 트루퍼
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELSWORD_I_SWORD" 	),	//36
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.TGA",		L"list_AISHA_D_WITCH" 	),	//37
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.TGA",		L"list_RENA_N_WATCHER" 	),	//38
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.TGA",		L"list_RAVEN_V_C0MMENDER"),	//39
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_EVE_B_SERAPH"	),	//40 
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.TGA",		L"list_CHUNG_T_TROOPER"	),	//41
#pragma endregion 신 2차 전직

#pragma region 아라(마샬 아티스트, )
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ARA"	),	//42
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ARA_L_Hsien"	),	//43 소선
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ARA_S_Devanam"	),	//44 제천
#pragma endregion 노전직, 1차전직, 2차전직


	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELESIS"	),	//45 엘리시스 노전
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga", 		L"list_ELESIS_S_KNIGHT"	),	//46 엘리시스 세이버 나이트
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELESIS_P_KNIGHT"	),	//47 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
#pragma region 아라 (소마, 명왕)
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ARA_L_DEVIL"	),	//48 소마
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ARA_Y_RAJA"	),	//49 명왕
#pragma endregion 아라 2번째 전직
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
#pragma region 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",	 	L"list_ELESIS_G_MASTER"	),	//50 그랜드 마스터
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ELESIS_B_HEART"	),	//51 블레이징 하트
#pragma endregion 엘리시스 2차 전직 ( 그랜드 마스터, 블레이징 하트 )
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ADD"	), //52 애드 기본 전직
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ADD_P_Tracer"	), //53 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	CharacterImageName( L"DLG_UI_Common_Texture_GUI_character_01.tga",		L"list_ADD_ALP"), //54 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

};
#pragma endregion 유닛 슬롯 이미지

CX2Data::CX2Data(void)
//{{ kimhc // 2009-12-15 // 이전에 플레이 했던 서버군 저장
#ifdef	ADD_SERVER_GROUP
: m_wstrServerGroupFileName( L"SelectedServerGroup.lua" )
#endif	ADD_SERVER_GROUP
//}} kimhc // 2009-12-15 // 이전에 플레이 했던 서버군 저장
//{{ 허상형 : [2011/3/14/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
, m_pWorldMissionManager( NULL )
#endif SERV_INSERT_GLOBAL_SERVER
//}} 허상형 : [2011/3/14/] //	월드 미션
, m_BattleFieldManager()
, m_pBattleFieldRoom( NULL )
#ifdef SERV_RECOMMEND_LIST_EVENT
, m_bShowRecommendUI( false )
#endif //SERV_RECOMMEND_LIST_EVENT
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
, m_UIEffectSet( 0 )
, m_GameEffectSet( 1 )
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
#ifdef SERV_GLOBAL_MISSION_MANAGER
, m_pGlobalMissionManager( NULL )
#endif SERV_GLOBAL_MISSION_MANAGER
{

	m_pUIMajorParticle		= NULL;
	m_pUIMinorParticle		= NULL;

	m_pUIMajorXMeshPlayer	= NULL;
	m_pUIMinorXMeshPlayer	= NULL;

	m_pGameMajorParticle	= NULL;
	m_pGameMinorParticle	= NULL;

	m_pGameMajorXMeshPlayer	= NULL;
	m_pGameMinorXMeshPlayer	= NULL;
	
	m_pItemManager			= NULL;	
	m_pUnitManager			= NULL;
	m_pUIUnitManager		= NULL;
	m_pWorldManager			= NULL;
	m_pDungeonManager		= NULL;
	m_pChannelManager		= NULL;
	m_pQuestManager			= NULL;
	m_pSkillTree			= NULL;
#ifdef COMBO_TREE_UI_TEST
	m_pComboTree = NULL;
#endif COMBO_TREE_UI_TEST
	m_pPartyManager			= NULL;
	m_pTrainingCenterTable	= NULL;

	m_pMyUser				= NULL;
	m_pServerProtocol		= NULL;

	m_pPVPRoom				= NULL;
	m_pDungeonRoom			= NULL;

	m_pGameUDP				= NULL;

	m_pPVPResultInfo		= NULL;
	m_pDungeonResultInfo	= NULL;

	m_pDamageManager		= NULL;

	m_pPicChar				= NULL;
	m_pPicCharRed			= NULL;
	m_pPicCharBlue			= NULL;
	m_pPicCharGreen			= NULL;
	m_pPicCharYellow		= NULL;
	m_pPicCharGameScore		= NULL;
	m_pPicCharTechPoint		= NULL;

	m_pScoreBonusPicChar	= NULL;
	m_pComboPicChar			= NULL;
	m_pComboPicCharBig		= NULL;	

	m_pLocationManager		= NULL;
	m_pMessenger			= NULL;
	m_pSlideShot			= NULL;

#ifdef EVENT_SCENE
	m_pEventScene			= NULL;
#endif EVENT_SCENE

	m_pCX2SocketItem		= NULL;
	m_pCX2EnchantItem		= NULL;
	//{{ 2008. 10. 1  최육사
//#ifdef TITLE_SYSTEM
	m_pCX2TitleManager		= NULL;
//#endif
#ifdef NEW_VILLAGE_UI
	m_pRewardTable			= NULL;
    m_pUIManager            = NULL;
	m_pSlotBeforeDragging	= NULL;
	m_DraggingItemUID		= -1;

	m_pCharPopUpMenu		= NULL;

#ifdef CHARACTER_MOUSE_RBUTTON
	m_pUserListPopUp = NULL;
#endif CHARACTER_MOUSE_RBUTTON

#endif
	//}}
	m_pCashShop				= NULL;

	m_ArrivedPresentNum		= 0;

	m_ModelDetailPercent	= 100;

	m_bToggleRenderEtc = false;
	m_StateArg = 0;

	//{{ 허상형 : [2009/9/16] //	길드 매니저 객체 초기화
#ifdef GUILD_MANAGEMENT
	m_pGuildManager			= NULL;
#endif
	//}} 허상형 : [2009/9/16] //	길드 매니저 객체 초기화

#ifdef SERV_PET_SYSTEM
	m_pPetManager			= NULL;
#endif

	//{{ JHKang / 강정훈 / 2011.6.3 / 프리미엄 버프(휴면, PC방)
#ifdef COME_BACK_REWARD
	m_pPremiumBuffManager	= NULL;
#endif
	//}}

#ifdef DEPARTURE_PREVENTION_SYSTEM
	m_pDeparturePrevention	= NULL;
#endif

	//{{ 오현빈 // 2012-05-15 // 플레이 가이드 Class
	m_pPlayGuide				= NULL;
	//}}

	CX2BuffTempletManager::GetInstance();
	CX2GageManager::GetInstance();

// #ifdef RIDING_SYSTEM 왜 여기에서 초기화하면 dlg_gameedit.lua에서 파싱 오류가 나는지 모르겠다
// 	CX2RidingPetManager::GetInstance();
// #endif //RIDING_SYSTEM

//#ifdef USER_DEFINED_KEYBOARD_SETTING
//	for( int j=0; j<2; j++ )
//	{
//		for( int i=0; i<256; i++ )
//		{
//			s_KeyMappingLayer[j][i] = i;
//		}
//	}
//#endif USER_DEFINED_KEYBOARD_SETTING

	m_TickCountAtGetTime = 0;

#ifdef SERV_LOCAL_RANKING_SYSTEM //지인시스템
	m_pProfileManager = NULL;
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef CUBE_OPEN_IMAGE_MANAGER
	m_pCubeOpenImageManager = NULL;
#endif // CUBE_OPEN_IMAGE_MANAGER

#ifdef ADDED_RELATIONSHIP_SYSTEM
	m_pRelationshipManager = NULL;
	m_pRelationshipEffectManager = NULL;
#endif // ADDED_RELATIONSHIP_SYSTEM
}

CX2Data::~CX2Data(void)
{
	CX2BuffTempletManager::DestroyInstance();
	CX2GageManager::DestroyInstance();

    //{{ 2008. 10. 1  최육사
//#ifdef TITLE_SYSTEM
    SAFE_DELETE( m_pCX2TitleManager );
//#endif
    //}}	

	//{{ 2009. 6. 24  최육사
	SAFE_DELETE( m_pRewardTable );
	//}}

	SAFE_DELETE( m_pPVPRoom );
	SAFE_DELETE( m_pDungeonRoom );
	SAFE_DELETE( m_pBattleFieldRoom );

	SAFE_DELETE( m_pItemManager );
	SAFE_DELETE( m_pUnitManager );
	SAFE_DELETE( m_pUIUnitManager );	
	SAFE_DELETE( m_pWorldManager );
	SAFE_DELETE( m_pDungeonManager );
	SAFE_DELETE( m_pChannelManager );
	SAFE_DELETE( m_pQuestManager );
	SAFE_DELETE( m_pSkillTree );
#ifdef COMBO_TREE_UI_TEST
	SAFE_DELETE( m_pComboTree );
#endif COMBO_TREE_UI_TEST
	SAFE_DELETE( m_pPartyManager );
	SAFE_DELETE( m_pTrainingCenterTable );
	
	SAFE_DELETE( m_pMyUser );
	SAFE_DELETE( m_pServerProtocol );

	SAFE_DELETE( m_pGameUDP );	

	SAFE_DELETE( m_pPVPResultInfo );
	SAFE_DELETE( m_pDungeonResultInfo );
	
	SAFE_DELETE( m_pCashShop );
	SAFE_DELETE( m_pDamageManager );

#ifdef NEW_VILLAGE_UI
	SAFE_DELETE( m_pUIManager );
	SAFE_DELETE( m_pCharPopUpMenu );
#ifdef CHARACTER_MOUSE_RBUTTON
	SAFE_DELETE( m_pUserListPopUp );
#endif CHARACTER_MOUSE_RBUTTON
#endif

	SAFE_DELETE( m_pPicChar	);
	SAFE_DELETE( m_pPicCharRed );
	SAFE_DELETE( m_pPicCharBlue	);
	SAFE_DELETE( m_pPicCharGreen );
	SAFE_DELETE( m_pPicCharYellow );
	SAFE_DELETE( m_pPicCharGameScore );
	SAFE_DELETE( m_pPicCharTechPoint );


	SAFE_DELETE( m_pScoreBonusPicChar );
	SAFE_DELETE( m_pComboPicChar );
	SAFE_DELETE( m_pComboPicCharBig );

	SAFE_DELETE( m_pLocationManager );
	SAFE_DELETE( m_pMessenger );
	SAFE_DELETE( m_pSlideShot );

#ifdef EVENT_SCENE
	SAFE_DELETE( m_pEventScene );
#endif EVENT_SCENE

	//{{ 오현빈 // 2012-05-15 // 플레이 가이드 Class
	SAFE_DELETE ( m_pPlayGuide );

	DeleteUIEffectSet();
	DeleteGameEffectSet();

	DeleteUIMajorParticle();
	DeleteUIMinorParticle();
	DeleteGameMajorParticle();
	DeleteGameMinorParticle();

	DeleteUIMajorXMeshPlayer();
	DeleteUIMinorXMeshPlayer();
	DeleteGameMajorXMeshPlayer();
	DeleteGameMinorXMeshPlayer();

	SAFE_DELETE( m_pCX2SocketItem );
	SAFE_DELETE( m_pCX2EnchantItem );	

	//m_ServerSetList.clear();

	//{{ 허상형 : [2009/9/16] //	길드 매니저 객체 메모리 해제
#ifdef GUILD_MANAGEMENT
	SAFE_DELETE( m_pGuildManager );
#endif	//	GUILD_MANAGEMENT
	//}} 허상형 : [2009/9/16] //	길드 매니저 객체 메모리 해제

#ifdef SERV_PET_SYSTEM
	SAFE_DELETE( m_pPetManager );
#endif

	//{{ 허상형 : [2011/3/14/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
	SAFE_DELETE( m_pWorldMissionManager );
#endif SERV_INSERT_GLOBAL_SERVER
	//}} 허상형 : [2011/3/14/] //	월드 미션

	//{{ JHKang / 강정훈 / 2011.6.3 / 프리미엄 버프(휴면, PC방)
#ifdef COME_BACK_REWARD
	SAFE_DELETE( m_pPremiumBuffManager );
#endif
	//}}

#ifdef DEPARTURE_PREVENTION_SYSTEM
	SAFE_DELETE( m_pDeparturePrevention );
#endif

#ifdef SERV_LOCAL_RANKING_SYSTEM //지인시스템
	SAFE_DELETE( m_pProfileManager );
#endif //SERV_LOCAL_RANKING_SYSTEM

#ifdef CUBE_OPEN_IMAGE_MANAGER
	SAFE_DELETE( m_pCubeOpenImageManager );
#endif // CUBE_OPEN_IMAGE_MANAGER

#ifdef ADDED_RELATIONSHIP_SYSTEM
	SAFE_DELETE ( m_pRelationshipManager );
	SAFE_DELETE ( m_pRelationshipEffectManager );
#endif // 

#ifdef SERV_GLOBAL_MISSION_MANAGER
	SAFE_DELETE( m_pGlobalMissionManager );
#endif SERV_GLOBAL_MISSION_MANAGER
}

HRESULT CX2Data::OnFrameMove( double fTime, float fElapsedTime )
{
	KTDXPROFILE();

	if( m_pUIMajorParticle != NULL )
		m_pUIMajorParticle->OnFrameMove( fTime, fElapsedTime );

	if( m_pUIMinorParticle != NULL )
		m_pUIMinorParticle->OnFrameMove( fTime, fElapsedTime );

	if( m_pUIMajorXMeshPlayer != NULL )
		m_pUIMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

	if( m_pUIMinorXMeshPlayer != NULL )
		m_pUIMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

	if( m_pGameMajorParticle != NULL )
		m_pGameMajorParticle->OnFrameMove( fTime, fElapsedTime );

	if( m_pGameMinorParticle != NULL )
		m_pGameMinorParticle->OnFrameMove( fTime, fElapsedTime );

	if( m_pGameMajorXMeshPlayer != NULL )
		m_pGameMajorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );

	if( m_pGameMinorXMeshPlayer != NULL )
		m_pGameMinorXMeshPlayer->OnFrameMove( fTime, fElapsedTime );


	m_UIEffectSet.OnFrameMove( fTime, fElapsedTime );
	m_GameEffectSet.OnFrameMove( fTime, fElapsedTime );


	if( m_pGameUDP != NULL )
		m_pGameUDP->OnFrameMove( fTime, fElapsedTime );

#ifdef EVENT_SCENE
	if( NULL != m_pEventScene )
		m_pEventScene->OnFrameMove( fTime, fElapsedTime );
#endif EVENT_SCENE

	if( NULL != m_pSlideShot )
		m_pSlideShot->OnFrameMove( fElapsedTime );

	if( GetMyUser() != NULL && g_pKTDXApp->GetIsNowVeryfy() == true )
	{
		for ( int i = 0; i < GetMyUser()->GetUnitNum(); i++ )
		{
			CX2Unit* pUnit = GetMyUser()->GetUnitByIndex(i);
			if ( pUnit == NULL )
				continue;
			pUnit->GetUnitData().Verify();
		}
	}


	g_pData->GetClassChangePopup().OnFrameMove( fTime, fElapsedTime );

#ifdef SERV_PET_SYSTEM
	if( m_pPetManager != NULL && g_pX2Game == NULL )
		m_pPetManager->OnFrameMove( fTime, fElapsedTime );
#endif


//#ifndef X2OPTIMIZE_PARTY_UDP_CONNECTION_BUG_FIX
//	//{{ 2013. 1. 9	박세훈	Merge 공인IP 연결 실패시 내부IP로 시도( 박진웅 )
////#ifdef SERV_KTDX_RETRY_USING_INTERNAL_IP
//	if( ( g_pData->GetPVPRoom() == NULL ) && ( g_pData->GetDungeonRoom() == NULL ) )
//	{
//		if( m_pPartyManager != NULL )
//		{
//			m_pPartyManager->OnFrameMove( fTime, fElapsedTime );
//		}
//	}
////#endif SERV_KTDX_RETRY_USING_INTERNAL_IP
//	//}}
//#endif  X2OPTIMIZE_PARTY_UDP_CONNECTION_BUG_FIX

	return S_OK;
}

HRESULT CX2Data::OnResetDevice()
{
	if( m_pUIMajorParticle != NULL )
		m_pUIMajorParticle->OnResetDevice();

	if( m_pUIMinorParticle != NULL )
		m_pUIMinorParticle->OnResetDevice();

	if( m_pGameMajorParticle != NULL )
		m_pGameMajorParticle->OnResetDevice();

	if( m_pGameMinorParticle != NULL )
		m_pGameMinorParticle->OnResetDevice();

	return S_OK;
}

HRESULT CX2Data::OnLostDevice()
{
	if( m_pUIMajorParticle != NULL )
		m_pUIMajorParticle->OnLostDevice();

	if( m_pUIMinorParticle != NULL )
		m_pUIMinorParticle->OnLostDevice();

	if( m_pGameMajorParticle != NULL )
		m_pGameMajorParticle->OnLostDevice();

	if( m_pGameMinorParticle != NULL )
		m_pGameMinorParticle->OnLostDevice();

	return S_OK;
}

void CX2Data::LoadUserUnitMotion1()
{
	//XSkinMeshReady( L"Motion_Elsword_SwordMan.x" );

#ifdef SEPARATION_MOTION
	XSkinMeshReady( L"Motion_Elsword_Common.x" );
	XSkinMeshReady( L"Motion_Elsword_Field.x" );
	XSkinMeshReady( L"Motion_Elsword_Emotion.x" );
#else
	XSkinMeshReady( L"Motion_Field_Elsword.x" );
#endif

	XSkinMeshReady( L"Motion_Elsword_Emotion_BIGBANG.X" );
	XSkinMeshReady( L"Motion_Elsword_Emotion_DeepBow.X" );
	XSkinMeshReady( L"Motion_Elsword_Emotion_APINK.X" );
	XSkinMeshReady( L"Motion_Elsword_Emotion_APINK_LOVE.X" );
	XSkinMeshReady( L"Motion_Elsword_Emotion_CRAYONPOP_Sorted.X" );

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_Elsword_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION
}

void CX2Data::LoadUserUnitMotion2()
{
	XSkinMeshReady( L"Motion_Lire.x" );

#ifdef SEPARATION_MOTION
	XSkinMeshReady( L"Motion_Lire_Common.x" );
	XSkinMeshReady( L"Motion_Lire_Field.x" );
	XSkinMeshReady( L"Motion_Lire_Emotion.x" );
#else
	XSkinMeshReady( L"Motion_Field_Lire.x" );
#endif

	XSkinMeshReady( L"Motion_Lire_Emotion_BIGBANG.X" );
	XSkinMeshReady( L"Motion_Lire_Emotion_DeepBow.X" );
	XSkinMeshReady( L"Motion_Lire_Emotion_APINK.X" );
	XSkinMeshReady( L"Motion_Lire_Emotion_APINK_LOVE.X" );
	XSkinMeshReady( L"Motion_Lire_Emotion_CRAYONPOP_Sorted.X" );

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_LIRE_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION
}

void CX2Data::LoadUserUnitMotion3()
{
	XSkinMeshReady( L"Motion_Arme.x" );

#ifdef SEPARATION_MOTION
	XSkinMeshReady( L"Motion_Arme_Common.x" );
	XSkinMeshReady( L"Motion_Arme_Field.x" );
	XSkinMeshReady( L"Motion_Arme_Emotion.x" );
#else
	XSkinMeshReady( L"Motion_Field_Arme.x" );
#endif

	XSkinMeshReady( L"Motion_Arme_Emotion_BIGBANG.X" );
	XSkinMeshReady( L"Motion_Arme_Emotion_DeepBow.X" );
	XSkinMeshReady( L"Motion_Arme_Emotion_APINK.X" );
	XSkinMeshReady( L"Motion_Arme_Emotion_APINK_LOVE.X" );
	XSkinMeshReady( L"Motion_Arme_Emotion_CRAYONPOP_Sorted.X" );

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_ARME_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION
}

void CX2Data::LoadUserUnitMotion4()
{
	XSkinMeshReady( L"Motion_Raven.x" );

#ifdef SEPARATION_MOTION
	XSkinMeshReady( L"Motion_Raven_Common.x" );
	XSkinMeshReady( L"Motion_Raven_Field.x" );
	XSkinMeshReady( L"Motion_Raven_Emotion.x" );
#else
	XSkinMeshReady( L"Motion_Field_Raven.x" );
#endif

	XSkinMeshReady( L"Motion_Raven_Emotion_BIGBANG.X" );
	XSkinMeshReady( L"Motion_Raven_Emotion_DeepBow.X" );
	XSkinMeshReady( L"Motion_Raven_Emotion_APINK.X" );
	XSkinMeshReady( L"Motion_Raven_Emotion_APINK_LOVE.X" );
	XSkinMeshReady( L"Motion_Raven_Emotion_CRAYONPOP_Sorted.X" );

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_RAVEN_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION
}

void CX2Data::LoadUserUnitMotion5()
{
	XSkinMeshReady( L"Motion_Eve.x" );

#ifdef SEPARATION_MOTION
	XSkinMeshReady( L"Motion_EVE_Common.x" );
	XSkinMeshReady( L"Motion_EVE_Field.x" );
	XSkinMeshReady( L"Motion_EVE_Emotion.x" );
#else
	XSkinMeshReady( L"Motion_Field_Eve.x" );
	XSkinMeshReady( L"Motion_Eve_Emotion_DeepBow.X" );
#endif

	XSkinMeshReady( L"Motion_EVE_Emotion_BIGBANG.X" );
	XSkinMeshReady( L"Motion_Eve_Emotion_DeepBow.X" );
	XSkinMeshReady( L"Motion_EVE_Emotion_APINK.X" );
	XSkinMeshReady( L"Motion_EVE_Emotion_APINK_LOVE.X" );
	XSkinMeshReady( L"Motion_Eve_Emotion_CRAYONPOP_Sorted.X" );

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_EVE_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION
}

#ifdef NEW_CHARACTER_CHUNG
void CX2Data::LoadUserUnitMotion6()
{
	XSkinMeshReady( L"Motion_CHUNG.x" );

	XSkinMeshReady( L"Motion_CHUNG_Common.x" );
	XSkinMeshReady( L"Motion_CHUNG_Field.x" );
	XSkinMeshReady( L"Motion_CHUNG_Emotion.x" );

	XSkinMeshReady( L"Motion_CHUNG_Emotion_BIGBANG.X" );
	XSkinMeshReady( L"Motion_Chung_Emotion_DeepBow.X" );
	XSkinMeshReady( L"Motion_CHUNG_Emotion_APINK.X" );
	XSkinMeshReady( L"Motion_CHUNG_Emotion_APINK_LOVE.X" );
	XSkinMeshReady( L"Motion_Chung_Emotion_CRAYONPOP_Sorted.X" );

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_CHUNG_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION
}
#endif

#ifdef ARA_CHARACTER_BASE
void CX2Data::LoadUserUnitMotion7()
{
	XSkinMeshReady( L"Motion_ARA.x" );
	XSkinMeshReady( L"Motion_ARA_Common.x" );
	XSkinMeshReady( L"Motion_ARA_Field.x" );
	XSkinMeshReady( L"Motion_ARA_Emotion.x" );

#ifdef ADD_ARA_EMOTION		/// 아라 아바타 이모션 추가
	XSkinMeshReady( L"Motion_Ara_Emotion_DeepBow.X" );
	XSkinMeshReady( L"Motion_Ara_Emotion_CRAYONPOP_Sorted.X" );
#endif ADD_ARA_EMOTION

#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_ARA_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION
}
#endif
#ifdef NEW_CHARACTER_EL
void CX2Data::LoadUserUnitMotion8()
{
	XSkinMeshReady( L"Motion_EL.x" );
	XSkinMeshReady( L"Motion_EL_Common.x" );
	XSkinMeshReady( L"Motion_EL_Field.x" );
	XSkinMeshReady( L"Motion_EL_Emotion.x" );


#ifdef CRAYONPOP_SECOND_EMOTION // 김태환
	XSkinMeshReady( L"Motion_EL_Emotion_BbaBbaBba.X" );
#endif // CRAYONPOP_SECOND_EMOTION

#ifdef ADD_KOREAN_CLOTHES_EMOTION // 김태환
	XSkinMeshReady( L"Motion_EL_Emotion_DeepBow.X" );
#endif // ADD_KOREAN_CLOTHES_EMOTION
}
#endif // NEW_CHARACTER_EL
#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
void CX2Data::LoadUserUnitMotion9()		/// 변경이 필요 하다.
{
	XSkinMeshReady( L"Motion_ADD.x" );
	XSkinMeshReady( L"Motion_Add_Common.x" );
	XSkinMeshReady( L"Motion_ADD_Field.x" );
	XSkinMeshReady( L"Motion_Add_Emotion.x" );

#ifdef ADD_KOREAN_CLOTHES_EMOTION // 김태환
	XSkinMeshReady( L"Motion_Add_Emotion_DeepBow.X" );
#endif // ADD_KOREAN_CLOTHES_EMOTION
}
#endif //SERV_9TH_NEW_CHARACTER

void CX2Data::LoadPicChar1()
{
	m_pPicChar			= new CKTDGPicChar( L"PC_", m_pUIMajorParticle->CreateSequence( NULL,  L"PicCharDamage", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );
	m_pPicChar->SetWidth( 30 );
}

void CX2Data::LoadPicChar2()
{
	m_pPicCharRed		= new CKTDGPicChar( m_pPicChar, m_pUIMajorParticle->CreateSequence( NULL,  L"PicCharRed", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );
	m_pPicCharRed->SetWidth( 26 );

}

void CX2Data::LoadPicChar3()
{
	m_pPicCharBlue		= new CKTDGPicChar( m_pPicChar, m_pUIMajorParticle->CreateSequence( NULL,  L"PicCharBlue", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );
	m_pPicCharBlue->SetWidth( 30 );

}

void CX2Data::LoadPicChar4()
{
	m_pPicCharGreen		= new CKTDGPicChar( m_pPicChar, m_pUIMajorParticle->CreateSequence( NULL,  L"PicCharGreen", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );
	m_pPicCharGreen->SetWidth( 30 );

}

void CX2Data::LoadPicChar5()
{
	m_pPicCharYellow	= new CKTDGPicChar( m_pPicChar, m_pUIMajorParticle->CreateSequence( NULL,  L"PicCharYellow", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f ) );
	m_pPicCharYellow->SetWidth( 30 );

}

void CX2Data::LoadPicChar6()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pUIMajorParticle->CreateSequence( NULL,  L"GameScorePicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharGameScore = new CKTDGPicChar( L"HQ_CPC_", pSeq, L"0123456789" );
	m_pPicCharGameScore->SetWidth( 11.0f );
}

void CX2Data::LoadPicChar7()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pUIMajorParticle->CreateSequence( NULL,  L"ScoreBonusPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pPicCharTechPoint = new CKTDGPicChar( m_pPicCharGameScore, pSeq );
	m_pPicCharTechPoint->SetColor( 0xffff0000 );
	m_pPicCharTechPoint->SetWidth( 11.0f );
}

void CX2Data::LoadPicChar8()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pUIMajorParticle->CreateSequence( NULL,  L"ScoreBonusPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pScoreBonusPicChar	= new CKTDGPicChar( m_pPicCharGameScore, pSeq );
}

void CX2Data::LoadPicChar9()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pUIMajorParticle->CreateSequence( NULL,  L"ComboPicCharBig", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pComboPicCharBig	= new CKTDGPicChar( L"HQ_Combo_", pSeq, L"0123456789;" );
}

void CX2Data::LoadPicChar10()
{
	CKTDGParticleSystem::CParticleEventSequence* pSeq = m_pUIMajorParticle->CreateSequence( NULL,  L"ComboPicChar", 0.0f, 0.0f, 0.0f,  0.0f, 0.0f );
	pSeq->SetDynamicChain( false );
	m_pComboPicChar	= new CKTDGPicChar( m_pComboPicCharBig, pSeq );
}

void CX2Data::LoadPicCharAll_LUA()
{
#ifndef _SERVICE_
	
	if ( g_pData->GetMyUser()->GetAuthLevel() >= CX2User::XUAL_OPERATOR )
	{
		if( NULL == m_pPicChar )
			LoadPicChar1();
		if( NULL == m_pPicCharRed )
			LoadPicChar2();
		if( NULL == m_pPicCharBlue )
			LoadPicChar3();
		if( NULL == m_pPicCharGreen )
			LoadPicChar4();
		if( NULL == m_pPicCharYellow )
			LoadPicChar5();

		if( NULL == m_pPicCharGameScore )
			LoadPicChar6();
		if( NULL == m_pPicCharTechPoint )
			LoadPicChar7();
		if( NULL == m_pScoreBonusPicChar )
			LoadPicChar8();
		if( NULL == m_pComboPicCharBig )
			LoadPicChar9();
		if( NULL == m_pComboPicChar )
			LoadPicChar10();
	}

#endif 
}

void CX2Data::LoadEXPTable()
{
	m_EXPTable.OpenScriptFile( L"ExpTable.lua" );
	XSkinMeshReady( L"LobbyLevelUp01-1.X" );
	XSkinMeshReady( L"LobbyLevelUp01.X" );
	XSkinMeshReady( L"LobbyLevelUp02.X" );
	XSkinMeshReady( L"LobbyLevelUp03.X" );
	XSkinMeshReady( L"LobbyLevelUp04.X" );
	TextureReady( L"Explosion_Sphere.dds" );
}

void CX2Data::LoadPVPResultTable()
{
	m_PVPResultTable.OpenScriptFile( L"PVPResultTable.lua" );
}

#ifdef ADD_SERVER_GROUP
void CX2Data::LoadItemManager()
{
	m_pItemManager->ClearItemExchangeData();
	m_pItemManager->ClearManufactureData();
	m_pItemManager->ClearManufactureVillageData();
	m_pItemManager->ClearManufactureResultGroupData();

	m_pItemManager->OpenScriptFile( L"ManufactureItem.lua" );
	m_pItemManager->OpenScriptFile( L"ItemExchangeTable.lua" );
}
#endif

#ifdef SERVER_CURRENT_TIME_INT64
__int64 CX2Data::GetServerCurrentTime64()
{
	DWORD diffTickCount = ::GetTickCount() - m_TickCountAtGetTime;
	return m_tServerCurrent64Time + (diffTickCount/1000);
}
void CX2Data::SetServerCurrentTimeInt64( __int64 tTime )
{
	m_tServerCurrent64Time = tTime;
}
#endif //SERVER_CURRENT_TIME_INT64

//{{ 임홍락 [2012.05.16] 클라이언트 단일화 // CLIENT_USE_NATION_FLAG 필수 포함임
#ifdef UNITED_CLIENT_EU
std::wstring CX2Data::GetWstrItemLuaName()
{
	std::wstring wstrFile = L"";
	switch(g_pMain->GetNationFlag())
	{
	case CX2Main::NF_DE:	wstrFile = L"Item_DE.lua";	break;
	case CX2Main::NF_FR:	wstrFile = L"Item_FR.lua";	break;
	case CX2Main::NF_IT:	wstrFile = L"Item_IT.lua";	break;
	case CX2Main::NF_PL:	wstrFile = L"Item_PL.lua";	break;
	case CX2Main::NF_ES:	wstrFile = L"Item_ES.lua";	break;
	case CX2Main::NF_EN:	wstrFile = L"Item_EN.lua";	break;
	default:
		wstrFile = L"Item.lua";
		break;
	}

	return wstrFile;
}

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
std::wstring CX2Data::GetWstrItemTransLuaName()
{
	std::wstring wstrFile = L"";
	switch(g_pMain->GetNationFlag())
	{
	case CX2Main::NF_DE:	wstrFile = L"ItemTrans_DE.lua";	break;
	case CX2Main::NF_FR:	wstrFile = L"ItemTrans_FR.lua";	break;
	case CX2Main::NF_IT:	wstrFile = L"ItemTrans_IT.lua";	break;
	case CX2Main::NF_PL:	wstrFile = L"ItemTrans_PL.lua";	break;
	case CX2Main::NF_ES:	wstrFile = L"ItemTrans_ES.lua";	break;
	case CX2Main::NF_EN:	wstrFile = L"ItemTrans_EN.lua";	break;
	default:
		wstrFile = L"ItemTrans.lua";
		break;
	}

	return wstrFile;
}
#endif SERV_ITEM_LUA_TRANS_DEVIDE

#endif UNITED_CLIENT_EU
//}}

CX2ItemManager* CX2Data::ResetItemManager()
{
	SAFE_DELETE( m_pItemManager );
	m_pItemManager = new CX2ItemManager();

#ifdef  X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef IN_ITEM_KIM_USE_HIDE_SET_DESC
	m_pItemManager->OpenScriptFile( L"ItemEtcOption.lua" );
#endif //IN_ITEM_KIM_USE_HIDE_SET_DESC

#ifdef UNITED_CLIENT_EU
	std::wstring strItemLuaName = GetWstrItemLuaName();
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	std::wstring strItemTransLuaName = GetWstrItemTransLuaName();
	const wchar_t*  apwszArray[] = { strItemLuaName.c_str(), L"SetItem.lua", strItemTransLuaName.c_str(), L"SetItemTrans.lua" };
#else //SERV_ITEM_LUA_TRANS_DEVIDE
	const wchar_t*  apwszArray[] = { strItemLuaName.c_str(), L"SetItem.lua" };
#endif //SERV_ITEM_LUA_TRANS_DEVIDE
    m_pItemManager->OpenItemScriptFile( L"Item.kim", ARRAY_SIZE( apwszArray ), apwszArray );
	
#else UNITED_CLIENT_EU

#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
	const wchar_t*  apwszArray[] = { L"Item.lua", L"SetItem.lua", L"ItemTrans.lua", L"SetItemTrans.lua" };
#else //SERV_ITEM_LUA_TRANS_DEVIDE
    const wchar_t*  apwszArray[] = { L"Item.lua", L"SetItem.lua" };
#endif //SERV_ITEM_LUA_TRANS_DEVIDE

    m_pItemManager->OpenItemScriptFile( L"Item.kim", ARRAY_SIZE( apwszArray ), apwszArray );
#endif UNITED_CLIENT_EU
	//}}	
#else   //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
#ifdef UNITED_CLIENT_EU
	std::wstring strItemLuaName = GetWstrItemLuaName();
	m_pItemManager->OpenScriptFile( strItemLuaName.c_str() );
#else UNITED_CLIENT_EU
	m_pItemManager->OpenScriptFile( L"Item.lua" );
#endif UNITED_CLIENT_EU
	//}}
#ifdef SERV_ITEM_LUA_TRANS_DEVIDE
#ifdef UNITED_CLIENT_EU
	std::wstring strItemTransLuaName = GetWstrItemTransLuaName();
	m_pItemManager->OpenScriptFile( strItemTransLuaName.c_str() );
#else // UNITED_CLIENT_EU
	m_pItemManager->OpenScriptFile( L"ItemTrans.lua" );
#endif // UNITED_CLIENT_EU
#endif SERV_ITEM_LUA_TRANS_DEVIDE	
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	m_pItemManager->OpenScriptFile( L"ShopItem.lua" );
	m_pItemManager->OpenScriptFile( L"RandomItem.lua" );
#ifndef ADD_SERVER_GROUP
	m_pItemManager->OpenScriptFile( L"ManufactureItem.lua" );
#endif
	m_pItemManager->OpenScriptFile( L"PackageItemData.lua" );
#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	m_pItemManager->OpenScriptFile( L"SetItem.lua" );
#ifdef LUA_TRANS_DEVIDE
	m_pItemManager->OpenScriptFile( L"SetItemTrans.lua" );
#endif LUA_TRANS_DEVIDE	
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
#ifndef ADD_SERVER_GROUP
	m_pItemManager->OpenScriptFile( L"ItemExchangeTable.lua" );
#endif

#ifdef ICE_HEATER_PRE_VIEW
	m_pItemManager->OpenScriptFile( L"IceHeaterPreView.lua" );
#endif //ICE_HEATER_PRE_VIEW
#ifdef PET_PREVIEW
	m_pItemManager->OpenScriptFile( L"PetPreView.lua" );
#endif // PET_PREVIEW


#ifndef X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING
	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	// SetItemData에 구해준 MaxLevel(셋트효과를 구성하는 아이템 중 제일 높은 레벨) 을 넣어줌
	m_pItemManager->UpdateMaxLevelToSetItemData();
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
#endif  //X2OPTIMIZE_ITEM_TEMPLET_PREPROCESSING

#ifdef COOLTIME_SHARE_GROUP_ITEM
	m_pItemManager->OpenScriptFile( L"CoolTimeGroupItem.lua" );
#endif COOLTIME_SHARE_GROUP_ITEM
	//{{ 2013.01.26 최민철 큐브패키지아이템 미리보기기능
#ifdef PACKAGE_IN_QUBE_PREVIEW
	m_pItemManager->OpenScriptFile( L"AddCubePackageData.lua" );
#endif PACKAGE_IN_QUBE_PREVIEW
	//}}

#ifdef SERV_NEW_ITEM_SYSTEM_2013_05
	m_pItemManager->OpenScriptFile( L"ItemConvertTable.lua" );
#endif //SERV_NEW_ITEM_SYSTEM_2013_05

#ifdef MODIFY_FIND_SHOP_IMAGE_FILE_NAME
	m_pItemManager->CreateShopImageNameList();
#endif // MODIFY_FIND_SHOP_IMAGE_FILE_NAME

	return m_pItemManager;
}
#ifdef WORLD_TOOL
CX2ItemManager* CX2Data::ResetItemManagerEffectTool()
{
	SAFE_DELETE( m_pItemManager );
	m_pItemManager = new CX2ItemManager();

	m_pItemManager->OpenScriptFile( L"Item_EffectTool.lua" );
	return m_pItemManager;
}
#endif //WORLD_TOOL
CX2UnitManager* CX2Data::ResetUnitManager( const WCHAR* pFileName, const WCHAR* pNPCFileName, const WCHAR* pNPCStatFileName )
{
	SAFE_DELETE( m_pUnitManager );

	m_pUnitManager = new CX2UnitManager();
#ifdef USE_DIFFERENT_SOUND_WHEN_IN_SPECIAL
	m_pUnitManager->OpenDifferentSoundScriptFile(L"MappingCharSoundChange.lua");
#endif //USE_DIFFERENT_SOUND_WHEN_IN_SPECIAL
	m_pUnitManager->OpenScriptFile( pFileName );
	m_pUnitManager->OpenNPCFiles( pNPCFileName, pNPCStatFileName, L"NpcExtraAbility.lua" );

#ifdef REFORM_SKILL_NOTE_UI
	// 특정 클래스의 하위 클래스가 설정된 스크립트 파싱
	m_pUnitManager->OpenDataScriptFile( L"CorrelateUnitClassTemplet.lua");
#endif // REFORM_SKILL_NOTE_UI

	return m_pUnitManager;
}

CX2UIUnitManager* CX2Data::ResetUIUnitManager()
{
	SAFE_DELETE( m_pUIUnitManager );
	m_pUIUnitManager = new CX2UIUnitManager();

	return m_pUIUnitManager;
}

CX2WorldManager* CX2Data::ResetWorldManager( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pWorldManager );
	m_pWorldManager = new CX2WorldManager();
	m_pWorldManager->OpenScriptFile( pFileName );

	return m_pWorldManager;
}

CX2DungeonManager* CX2Data::ResetDungeonManager( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pDungeonManager );
	m_pDungeonManager = new CX2DungeonManager;
	m_pDungeonManager->OpenScriptFile( pFileName );

	return m_pDungeonManager;
}

CX2ChannelManager* CX2Data::ResetChannelManager()
{
	SAFE_DELETE( m_pChannelManager );
	m_pChannelManager = new CX2ChannelManager();

	return m_pChannelManager;
}

#ifdef SERV_ACCOUNT_MISSION_SYSTEM
CX2QuestManager* CX2Data::ResetQuestManager( const WCHAR* pQuestFileName, const WCHAR* pSubQuestFileName, const WCHAR* pAccountQuestFileName )
{
	SAFE_DELETE( m_pQuestManager );
	m_pQuestManager = new CX2QuestManager();
	m_pQuestManager->OpenScriptFile( pQuestFileName );
	m_pQuestManager->OpenScriptFile( pSubQuestFileName );
	//계정 공용 퀘스트 데이타 로드
	m_pQuestManager->OpenScriptFile( pAccountQuestFileName );
	
#ifdef LUA_TRANS_DEVIDE
	m_pQuestManager->OpenScriptFile( L"QuestTrans.lua" );
	m_pQuestManager->OpenScriptFile( L"SubQuestTrans.lua" );
#endif LUA_TRANS_DEVIDE	

	return m_pQuestManager;
}
#else
CX2QuestManager* CX2Data::ResetQuestManager( const WCHAR* pQuestFileName, const WCHAR* pSubQuestFileName )
{
	SAFE_DELETE( m_pQuestManager );
	m_pQuestManager = new CX2QuestManager();
	m_pQuestManager->OpenScriptFile( pQuestFileName );
	m_pQuestManager->OpenScriptFile( pSubQuestFileName );

#ifdef LUA_TRANS_DEVIDE
	m_pQuestManager->OpenScriptFile( L"QuestTrans.lua" );
	m_pQuestManager->OpenScriptFile( L"SubQuestTrans.lua" );
#endif LUA_TRANS_DEVIDE

	return m_pQuestManager;
}
#endif //SERV_ACCOUNT_MISSION_SYSTEM
#ifdef DAY_OF_THE_WEEK_QUEST
void CX2Data::OpenDayOfWeekQuestScript( const WCHAR* pQuestFileName_ )
{
	if( NULL != m_pQuestManager )
	{
		m_pQuestManager->ClearDayOfTheWeekQuest();
		m_pQuestManager->OpenScriptFile( pQuestFileName_ );
	}
}
#endif //DAY_OF_THE_WEEK_QUEST
CX2SkillTree* CX2Data::ResetSkillTree( const WCHAR* pFileName, const WCHAR* pSkillDataFileName )
{
	SAFE_DELETE( m_pSkillTree );
	m_pSkillTree = new CX2SkillTree;
	m_pSkillTree->OpenScriptFile( pFileName, pSkillDataFileName );

	return m_pSkillTree;
}

#ifdef COMBO_TREE_UI_TEST

	CX2ComboTree* CX2Data::ResetComboTree( const wstring& filename )
	{
		SAFE_DELETE( m_pComboTree );
		m_pComboTree = new CX2ComboTree;
		m_pComboTree->OpenScriptFile( filename );

		return m_pComboTree;
	}

#endif COMBO_TREE_UI_TEST



CX2PartyManager* CX2Data::ResetPartyManager()
{
	SAFE_DELETE( m_pPartyManager );
	m_pPartyManager = new CX2PartyManager;
	return m_pPartyManager;
}

CX2TrainingCenterTable* CX2Data::ResetTrainingCenterTable( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pTrainingCenterTable );
	m_pTrainingCenterTable = new CX2TrainingCenterTable;
	m_pTrainingCenterTable->OpenScriptFile( pFileName );

	return m_pTrainingCenterTable;
}

CX2ServerProtocol* CX2Data::ResetServerProtocol()
{
	SAFE_DELETE( m_pServerProtocol );

	bool bIsSERVICE = false;
#ifndef _SERVICE_
	bIsSERVICE = false;
#else
	bIsSERVICE = true;
#endif

	m_pServerProtocol = new CX2ServerProtocol( KTDXSendGameMessage, g_pKTDXApp->GetHWND(), XGM_RECEIVE_FROM_SERVER, CX2ServerEvent::SERVER_EVENT_ID_STR, EGS_CLIENT_EVENT_ID_END, bIsSERVICE );

	return m_pServerProtocol;
}

//CX2User* CX2Data::ResetMyUser( CX2User::UserData* pUserData )
CX2User* CX2Data::ResetMyUser( const CX2User::UserData& kUserData )
{
	SAFE_DELETE( m_pMyUser );
	m_pMyUser = new CX2User( kUserData );

	return m_pMyUser;
}

CX2PVPRoom* CX2Data::ResetPVPRoom()
{
#ifdef HEAP_BROKEN_BY_ROOM
	SAFE_DELETE( m_pPVPRoom );
	m_pPVPRoom = new CX2PVPRoom();
#else	HEAP_BROKEN_BY_ROOM
	if ( NULL == m_pPVPRoom )
		m_pPVPRoom = new CX2PVPRoom();
	else
		g_pX2Room = m_pPVPRoom;	
#endif // HEAP_BROKEN_BY_ROOM
	

	return m_pPVPRoom;
}

CX2DungeonRoom* CX2Data::ResetDungeonRoom()
{
#ifdef HEAP_BROKEN_BY_ROOM
	SAFE_DELETE( m_pDungeonRoom );
	m_pDungeonRoom = new CX2DungeonRoom();
#else	HEAP_BROKEN_BY_ROOM
	if ( NULL == m_pDungeonRoom )
		m_pDungeonRoom = new CX2DungeonRoom();
	else
		g_pX2Room = m_pDungeonRoom;
#endif // HEAP_BROKEN_BY_ROOM

	return m_pDungeonRoom;
}

CX2BattleFieldRoom* CX2Data::ResetBattleFieldRoom()
{
#ifdef HEAP_BROKEN_BY_ROOM
	SAFE_DELETE( m_pBattleFieldRoom );
	m_pBattleFieldRoom = new CX2BattleFieldRoom();
#else	HEAP_BROKEN_BY_ROOM
	if ( NULL == m_pBattleFieldRoom )
		m_pBattleFieldRoom = new CX2BattleFieldRoom();
	else
		g_pX2Room = m_pBattleFieldRoom;
#endif // HEAP_BROKEN_BY_ROOM

	return m_pBattleFieldRoom;
}

CKTDGParticleSystem* CX2Data::ResetUIMajorParticle( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pUIMajorParticle );
	m_pUIMajorParticle = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK        
        );
	m_pUIMajorParticle->OpenScriptFile( pFileName );

	return m_pUIMajorParticle;
}

CKTDGParticleSystem* CX2Data::ResetUIMinorParticle( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pUIMinorParticle );
	m_pUIMinorParticle = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 1
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
	m_pUIMinorParticle->OpenScriptFile( pFileName );

	return m_pUIMinorParticle;
}

CKTDGParticleSystem* CX2Data::ResetGameMajorParticle( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pGameMajorParticle );
	m_pGameMajorParticle = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 2
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK          
        );
	m_pGameMajorParticle->OpenScriptFile( pFileName );

	return m_pGameMajorParticle;
}

CKTDGParticleSystem* CX2Data::ResetGameMinorParticle( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pGameMinorParticle );
	m_pGameMinorParticle = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 3
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK          
        );
	m_pGameMinorParticle->OpenScriptFile( pFileName );

	return m_pGameMinorParticle;
}

CKTDGXMeshPlayer* CX2Data::ResetUIMajorXMeshPlayer( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pUIMajorXMeshPlayer );
	m_pUIMajorXMeshPlayer = new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
    ASSERT( m_pUIMajorXMeshPlayer != NULL );
	m_pUIMajorXMeshPlayer->OpenScriptFile( pFileName );

	return m_pUIMajorXMeshPlayer;
}

CKTDGXMeshPlayer* CX2Data::ResetUIMinorXMeshPlayer( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pUIMinorXMeshPlayer );
	m_pUIMinorXMeshPlayer = new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        1
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK        
        );
    ASSERT( m_pUIMinorXMeshPlayer != NULL );
	m_pUIMinorXMeshPlayer->OpenScriptFile( pFileName );

	return m_pUIMinorXMeshPlayer;
}

CKTDGXMeshPlayer* CX2Data::ResetGameMajorXMeshPlayer( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pGameMajorXMeshPlayer );
	m_pGameMajorXMeshPlayer = new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        2
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
    ASSERT( m_pGameMajorXMeshPlayer != NULL );
	m_pGameMajorXMeshPlayer->OpenScriptFile( pFileName );

	return m_pGameMajorXMeshPlayer;
}

CKTDGXMeshPlayer* CX2Data::ResetGameMinorXMeshPlayer( const WCHAR* pFileName )
{
	SAFE_DELETE( m_pGameMinorXMeshPlayer );
	m_pGameMinorXMeshPlayer = new CKTDGXMeshPlayer(
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        3
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK         
        );
    ASSERT( m_pGameMinorXMeshPlayer != NULL );
	m_pGameMinorXMeshPlayer->OpenScriptFile( pFileName );

	return m_pGameMinorXMeshPlayer;
}


CX2EffectSet* CX2Data::ResetUIEffectSet( const WCHAR* pFileName )
{
	if( NULL == pFileName )
		return &m_UIEffectSet;

	DeleteUIEffectSet();
	m_UIEffectSet.OpenScriptFile( pFileName );

	return &m_UIEffectSet;
}

CX2EffectSet* CX2Data::ResetGameEffectSet( const WCHAR* pFileName )
{
	if( NULL == pFileName )
		return &m_GameEffectSet;

	DeleteGameEffectSet();
	m_GameEffectSet.OpenScriptFile( pFileName );

	return &m_GameEffectSet;
}



CKTDNUDP* CX2Data::ResetGameUDP( int port )
{
	SAFE_DELETE( m_pGameUDP );
	m_pGameUDP = new CKTDNUDP( port );

	return m_pGameUDP;
}

//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
CX2SocketItem* CX2Data::ResetSocketItem( const WCHAR* pFileName,const WCHAR* pFormulaFileName_ )
//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업
{
	SAFE_DELETE( m_pCX2SocketItem );
	m_pCX2SocketItem = new CX2SocketItem();

	//{{ kimhc // 2011-07-05 // 옵션데이타 수치화 작업
	m_pCX2SocketItem->OpenScriptFile( pFileName, pFormulaFileName_ );
	//}} kimhc // 2011-07-05 // 옵션데이타 수치화 작업

	return m_pCX2SocketItem;
}

CX2EnchantItem* CX2Data::ResetEnchantItem()
{
	SAFE_DELETE( m_pCX2EnchantItem );
	m_pCX2EnchantItem = new CX2EnchantItem();

	return m_pCX2EnchantItem;
}

//{{ 2008. 10. 1  최육사
//#ifdef TITLE_SYSTEM
CX2TitleManager* CX2Data::ResetTitleManager()
{
	SAFE_DELETE( m_pCX2TitleManager );
	m_pCX2TitleManager = new CX2TitleManager();
	m_pCX2TitleManager->OpenScriptFile( L"TitleTable.lua" );
    m_pCX2TitleManager->OpenScriptFile( L"TitleMission.lua" );
    m_pCX2TitleManager->OpenScriptFile( L"SubTitleMission.lua" );
#ifdef LUA_TRANS_DEVIDE
	m_pCX2TitleManager->OpenScriptFile( L"TitleTableTrans.lua" );
	m_pCX2TitleManager->OpenScriptFile( L"TitleMissionTrans.lua" );
	m_pCX2TitleManager->OpenScriptFile( L"SubTitleMissionTrans.lua" );
#endif LUA_TRANS_DEVIDE

    m_pCX2TitleManager->InitRecord();

	return m_pCX2TitleManager;
}
//#endif
//}}

//{{ 2009. 6. 24  최육사	보상테이블
CX2RewardTable* CX2Data::ResetRewardTable()
{
    SAFE_DELETE( m_pRewardTable );
	m_pRewardTable = new CX2RewardTable();
#ifdef SERV_EVENT_DB_CONTROL_SYSTEM
#else //SERV_EVENT_DB_CONTROL_SYSTEM
	m_pRewardTable->OpenScriptFile( L"RewardTable.lua" );
#endif //SERV_EVENT_DB_CONTROL_SYSTEM

	return m_pRewardTable;
}
//}}

//{{ 허상형 : [2009/9/16] //	길드 매니저
#ifdef GUILD_MANAGEMENT
CX2GuildManager* CX2Data::ResetGuildManager()
{
	SAFE_DELETE( m_pGuildManager );
	m_pGuildManager = new CX2GuildManager;
	m_pGuildManager->OpenScriptFile( L"GuildData.lua" );
	
	return m_pGuildManager;
}
#endif	//	GUILD_MANAGEMENT
//}} 허상형 : [2009/9/16] //	길드 매니저

#ifdef NEW_VILLAGE_UI
CX2UIManager* CX2Data::ResetUIManager()
{
    SAFE_DELETE( m_pUIManager );
    m_pUIManager = new CX2UIManager();    
#ifndef WORLD_TOOL
	m_pUIManager->ResetUIManager();
#endif //WORLD_TOOL
    return m_pUIManager;
}
CX2CharPopupMenu* CX2Data::ResetUserMenu()
{
	SAFE_DELETE(m_pCharPopUpMenu);
	m_pCharPopUpMenu = new CX2CharPopupMenu();
	return m_pCharPopUpMenu;
}

//{{ 김상훈 : 캐릭터 우클릭 팝업메뉴 UI 개선
#ifdef CHARACTER_MOUSE_RBUTTON


CX2UserListPopUp* CX2Data::ResetUserListPopUp()
{
	SAFE_DELETE(m_pUserListPopUp);
    m_pUserListPopUp = new CX2UserListPopUp();
	return m_pUserListPopUp;
}

#endif CHARACTER_MOUSE_RBUTTON
//}} 김상훈 : 캐릭터 우클릭 팝업메뉴 UI 개선

// void CX2Data::ToggleSDUnit() 
// { 
// #ifdef NEW_VILLAGE_AUTO_SD_UNIT
// 	if(g_pKTDXApp->GetFrameMoveFPS() <= 10.f)
// 	{
// 		m_bShowSDUnit = false;
// 		return;
// 	}
// #endif
// 
// 	m_bShowSDUnit = !m_bShowSDUnit; 
// }
#endif

CX2CashShop* CX2Data::ResetCashShop( CKTDXStage* pNowStage, const WCHAR* wszFileName )
{
	SAFE_DELETE( m_pCashShop );
	m_pCashShop = new CX2CashShop( pNowStage, wszFileName );

	return m_pCashShop;
}


//CX2PVPGame::PVPResultInfo* CX2Data::ResetPVPTeamResultInfo( KEGS_END_GAME_PVP_NORMAL_TEAM_RESULT_DATA_NOT& kPacket )
//{
//	SAFE_DELETE( m_pPVPResultInfo );
//	m_pPVPResultInfo = new CX2PVPGame::PVPResultInfo( kPacket );
//
//	return m_pPVPResultInfo;
//}
//CX2PVPGame::PVPResultInfo* CX2Data::ResetPVPDeathResultInfo( KEGS_END_GAME_PVP_DEATH_SURVIVAL_RESULT_DATA_NOT& kPacket )
//{
//	SAFE_DELETE( m_pPVPResultInfo );
//	m_pPVPResultInfo = new CX2PVPGame::PVPResultInfo( kPacket );
//
//	return m_pPVPResultInfo;
//}
//
//CX2PVPGame::PVPResultInfo* CX2Data::ResetPVPTeamDeathResultInfo( KEGS_END_GAME_PVP_TEAM_DEATH_RESULT_DATA_NOT& kPacket )
//{
//	SAFE_DELETE( m_pPVPResultInfo );
//	m_pPVPResultInfo = new CX2PVPGame::PVPResultInfo( kPacket );
//
//	return m_pPVPResultInfo;
//}

CX2PVPGame::PVPResultInfo* CX2Data::ResetPVPResultInfo( KEGS_END_GAME_PVP_RESULT_DATA_NOT& kPacket )
{
	SAFE_DELETE( m_pPVPResultInfo );
	m_pPVPResultInfo = new CX2PVPGame::PVPResultInfo( kPacket );

	return m_pPVPResultInfo;
}


CX2DungeonGame::DungeonResultInfo* CX2Data::ResetDungeonResultInfo( KEGS_END_GAME_DUNGEON_RESULT_DATA_NOT& kPacket )
{
	SAFE_DELETE( m_pDungeonResultInfo );
	m_pDungeonResultInfo = new CX2DungeonGame::DungeonResultInfo( kPacket );

	return m_pDungeonResultInfo;
}

CX2LocationManager*	CX2Data::ResetLocationManager()
{
	SAFE_DELETE( m_pLocationManager );
	m_pLocationManager = new CX2LocationManager();

	return m_pLocationManager;
}

CX2Community* CX2Data::ResetMessenger()
{
	SAFE_DELETE( m_pMessenger );
	m_pMessenger = new CX2Community();

	return m_pMessenger;
}

CX2DamageManager* CX2Data::ResetDamageManager()
{
	SAFE_DELETE( m_pDamageManager );
	m_pDamageManager = new CX2DamageManager();

	return m_pDamageManager;
}


#ifdef EVENT_SCENE
CX2EventScene* CX2Data::ResetEventScene(const WCHAR* pSceneObjectFileName, const WCHAR* pEventSceneFileName)
{
	SAFE_DELETE( m_pEventScene );
	m_pEventScene = new CX2EventScene();

	m_pEventScene->OpenScriptFile( pSceneObjectFileName );
	m_pEventScene->OpenScriptFile( pEventSceneFileName );

	return m_pEventScene;
}
#endif EVENT_SCENE

CX2SlideShot* CX2Data::ResetSlideShot()
{
	SAFE_DELETE( m_pSlideShot );
	m_pSlideShot = new CX2SlideShot();

	return m_pSlideShot;
}

#ifdef SERV_PET_SYSTEM
CX2PetManager* CX2Data::ResetPetManager()
{
	SAFE_DELETE( m_pPetManager );
	m_pPetManager = new CX2PetManager();
	m_pPetManager->OpenScriptFile( L"PetTemplet.lua" );

	return m_pPetManager;
}
#endif

#ifdef RIDING_SYSTEM
CX2RidingPetManager* CX2Data::ResetRidingPetManager()
{
	CX2RidingPetManager::DestroyInstance();
	CX2RidingPetManager::GetInstance();
	CX2RidingPetManager::GetInstance()->OpenScriptFile( L"RidingPetTemplet.lua" );

	return CX2RidingPetManager::GetInstance();
}
#endif //RIDING_SYSTEM

//{{ 허상형 : [2011/3/17/] //	월드미션
#ifdef SERV_INSERT_GLOBAL_SERVER
CX2WorldMissionManager* CX2Data::ResetWorldMissionManager()
{
	SAFE_DELETE( m_pWorldMissionManager );
	m_pWorldMissionManager = new CX2WorldMissionManager();

	return m_pWorldMissionManager;
}
#endif SERV_INSERT_GLOBAL_SERVER
//}} 허상형 : [2011/3/17/] //	월드미션

//{{ JHKang / 강정훈 / 2011.6.3 / 프리미엄 버프(휴면, PC방)
#ifdef COME_BACK_REWARD
CX2PremiumBuffManager* CX2Data::ResetPremiumBuffManager()
{
	SAFE_DELETE( m_pPremiumBuffManager );
	m_pPremiumBuffManager = new CX2PremiumBuffManager();

	return m_pPremiumBuffManager;
}
#endif
//}}

#ifdef DEPARTURE_PREVENTION_SYSTEM
CX2DeparturePrevention* CX2Data::ResetDeparturePrevention()
{
	SAFE_DELETE( m_pDeparturePrevention );
	m_pDeparturePrevention = new CX2DeparturePrevention();

	return m_pDeparturePrevention;
}
#endif

//{{ 오현빈 // 2012-05-15 // 플레이 가이드 Class
CX2PlayGuide*	CX2Data::ResetPlayGuide()
{
	SAFE_DELETE(m_pPlayGuide);
	m_pPlayGuide	= new CX2PlayGuide;

	return m_pPlayGuide;
}
//}}

#ifdef CUBE_OPEN_IMAGE_MANAGER
CX2CubeOpenImageManager* CX2Data::ResetCubeOpenImageManager()
{
	SAFE_DELETE( m_pCubeOpenImageManager);
	m_pCubeOpenImageManager = new CX2CubeOpenImageManager();
	m_pCubeOpenImageManager->OpenScriptFile(L"CubeOpenImageInfo.lua");

	return m_pCubeOpenImageManager;
}
#endif // CUBE_OPEN_IMAGE_MANAGER

wstring CX2Data::GetComboString( CX2DungeonRoom::RANK_TYPE rankType )
{

	wstring comboString = L"Error";

	switch( rankType )
	{
	case CX2DungeonRoom::RT_F:
		comboString = L"F";
		break;

	case CX2DungeonRoom::RT_E:
		comboString = L"E";
		break;

	case CX2DungeonRoom::RT_D:
		comboString = L"D";
		break;

	case CX2DungeonRoom::RT_C:
		comboString = L"C";
		break;

	case CX2DungeonRoom::RT_B:
		comboString = L"B";
		break;

	case CX2DungeonRoom::RT_A:
		comboString = L"A";
		break;

	case CX2DungeonRoom::RT_S:
		comboString = L"S";
		break;

		//{{ kimhc // 2011-02-22 // 던전랭크 SS 등급 추가
#ifdef	DUNGEON_RANK_NEW
	case CX2DungeonRoom::RT_SS:
		comboString = L"SS";
		break;
#endif	DUNGEON_RANK_NEW	
		//}} kimhc // 2011-02-22 // 던전랭크 SS 등급 추가
	}

	return comboString;
}

void CX2Data::PlayLobbyBGM( const WCHAR* wszName, bool bPlay )
{	
	if( g_pKTDXApp->GetDSManager()->GetInit() == false || g_pKTDXApp->GetDSManager()->GetMP3Play() == NULL )
		return;

	g_pKTDXApp->GetDSManager()->GetMP3Play()->Stop();

	if( true == bPlay )
	{
		g_pKTDXApp->GetDSManager()->GetMP3Play()->Play( wszName );
	}
	else
	{
		return;
	}
}

void CX2Data::LoadCommonDevice()
{
//{{ robobeg : 2008-10-28
	//KLuaManager luaManager;
    KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, true );
//}} robobeg : 2008-10-28
	g_pKTDXApp->LoadAndDoMemory( &luaManager, L"Unit_Common_Device.lua" );

	wstring fileName;

	if( luaManager.BeginTable( "INIT_COMMON_DEVICE" ) == true )
	{
		if( luaManager.BeginTable( "READY_COMMON_TEXTURE" ) == true )
		{
			int tableIndex = 1;
			while( luaManager.GetValue( tableIndex, fileName ) == true )
			{
				TextureReady( fileName.c_str() );
				tableIndex++;
			}

			luaManager.EndTable();
		}


		if( luaManager.BeginTable( "READY_COMMON_SOUND" ) == true )
		{
			int tableIndex = 1;
			while( luaManager.GetValue( tableIndex, fileName ) == true )
			{
				SoundReady( fileName.c_str() );
				tableIndex++;
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( "READY_COMMON_XSKIN_MESH" ) == true )
		{
			int tableIndex = 1;
			while( luaManager.GetValue( tableIndex, fileName ) == true )
			{
				XSkinMeshReady( fileName.c_str() );
				tableIndex++;
			}

			luaManager.EndTable();
		}

		if( luaManager.BeginTable( "READY_COMMON_XMESH" ) == true )
		{
			int tableIndex = 1;
			while( luaManager.GetValue( tableIndex, fileName ) == true )
			{
				XMeshReady( fileName.c_str() );
				tableIndex++;
			}

			luaManager.EndTable();
		}

		luaManager.EndTable();
	}
}

__time64_t CX2Data::GetServerCurrentTime()
{
	DWORD diffTickCount = ::GetTickCount() - m_TickCountAtGetTime;
	return m_ServerCurrentTime.GetTime() + (diffTickCount/1000);
}

void CX2Data::SetServerCurrentTime( wstring wstrTime )
{
	KncUtil::ConvertStringToCTime( wstrTime, m_ServerCurrentTime );
	m_TickCountAtGetTime = ::GetTickCount();
}

/*static*/ bool CX2Data::GetCharacterImageName( OUT wstring& fileName, OUT wstring& pieceName, const CX2Unit::UNIT_CLASS eUnitClass, const CHARACTER_IMAGE_TYPE eImageType )
{
	int iImageIndex = -1;
	switch( eUnitClass )
	{
	case CX2Unit::UC_ELSWORD_SWORDMAN:					iImageIndex = 0;		break;			
	case CX2Unit::UC_ELSWORD_KNIGHT:					iImageIndex = 1;		break;	
	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:				iImageIndex = 2;		break;
	case CX2Unit::UC_ARME_VIOLET_MAGE:					iImageIndex = 3;		break;
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:				iImageIndex = 4;		break;
	case CX2Unit::UC_ARME_DARK_MAGICIAN:				iImageIndex = 5;		break;
	case CX2Unit::UC_LIRE_ELVEN_RANGER:					iImageIndex = 6;		break;
	case CX2Unit::UC_LIRE_COMBAT_RANGER:				iImageIndex = 7;		break;
	case CX2Unit::UC_LIRE_SNIPING_RANGER:				iImageIndex = 8;		break;
	case CX2Unit::UC_RAVEN_FIGHTER:						iImageIndex = 9;		break;
	case CX2Unit::UC_RAVEN_SOUL_TAKER:					iImageIndex = 10;		break;
	case CX2Unit::UC_RAVEN_OVER_TAKER:					iImageIndex = 11;		break;
	case CX2Unit::UC_EVE_NASOD:							iImageIndex = 12;		break;
	case CX2Unit::UC_EVE_EXOTIC_GEAR:					iImageIndex = 13;		break;	
	case CX2Unit::UC_EVE_ARCHITECTURE:					iImageIndex = 14;		break;
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:				iImageIndex = 15;		break;	
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:				iImageIndex = 16;		break;	
	case CX2Unit::UC_LIRE_WIND_SNEAKER:					iImageIndex = 17;		break;	
	case CX2Unit::UC_LIRE_GRAND_ARCHER:					iImageIndex = 18;		break;	
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:				iImageIndex = 19;		break;	
	case CX2Unit::UC_ARME_VOID_PRINCESS:				iImageIndex = 20;		break;
#ifdef RAVEN_SECOND_CLASS_CHANGE
	case CX2Unit::UC_RAVEN_BLADE_MASTER:				iImageIndex = 21;		break;	
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:				iImageIndex = 22;		break;
#endif

//{{ kimhc // 2010.2.17 //	이브 2차 전직
#ifdef	EVE_SECOND_CLASS_CHANGE
	case CX2Unit::UC_EVE_CODE_NEMESIS:					iImageIndex = 23;		break;
	case CX2Unit::UC_EVE_CODE_EMPRESS:					iImageIndex = 24;		break;
#endif	EVE_SECOND_CLASS_CHANGE
//}} kimhc // 2010.2.17 //	이브 2차 전직

//{{ kimhc // 2010.11.24 // 2010-12-23 New Character CHUNG
#ifdef	NEW_CHARACTER_CHUNG
	case CX2Unit::UC_CHUNG_IRON_CANNON:					iImageIndex = 25;		break;
#endif	NEW_CHARACTER_CHUNG
//}} kimhc // 2010.11.24 //  2010-12-23 New Character CHUNG

//{{ kimhc // 2011.1.3 // 청 1차 전직
#ifdef	CHUNG_FIRST_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:				iImageIndex = 26;		break;
	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:			iImageIndex = 27;		break;
#endif	CHUNG_FIRST_CLASS_CHANGE
//}} kimhc // 2011.1.3 // 청 1차 전직

		//{{ JHKang / 강정훈 / 2011.6.13 / 청 2차 전직
#ifdef CHUNG_SECOND_CLASS_CHANGE
	case CX2Unit::UC_CHUNG_IRON_PALADIN:				iImageIndex = 28;		break;
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:				iImageIndex = 29;		break;
#endif
			//}}
#ifdef ELSWORD_SHEATH_KNIGHT
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:				iImageIndex = 30;		break;	
#endif ELSWORD_SHEATH_KNIGHT

#ifdef SERV_ADD_ARME_BATTLE_MAGICIAN
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:				iImageIndex = 31;		break;
#endif

#ifdef	SERV_TRAPPING_RANGER_TEST
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:				iImageIndex = 32;		break;
#endif	SERV_TRAPPING_RANGER_TEST

#ifdef RAVEN_WEAPON_TAKER
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:				iImageIndex = 33;		break;
#endif RAVEN_WEAPON_TAKER
#ifdef EVE_ELECTRA
	case CX2Unit::UC_EVE_ELECTRA:						iImageIndex = 34;		break;
#endif EVE_ELECTRA
#ifdef SERV_ADD_CHUNG_SHELLING_GUARDIAN
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:			iImageIndex = 35;		break;
#endif
#ifdef SERV_ELSWORD_INFINITY_SWORD
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:			iImageIndex = 36;		break;
#endif
#ifdef SERV_ARME_DIMENSION_WITCH
	case CX2Unit::UC_ARME_DIMENSION_WITCH:				iImageIndex = 37;		break;
#endif
#ifdef SERV_RENA_NIGHT_WATCHER
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:				iImageIndex = 38;		break;
#endif		
#ifdef SERV_RAVEN_VETERAN_COMMANDER
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:			iImageIndex = 39;		break;
#endif
#ifdef SERV_EVE_BATTLE_SERAPH
	case CX2Unit::UC_EVE_BATTLE_SERAPH:					iImageIndex = 40;		break;
#endif
#ifdef SERV_CHUNG_TACTICAL_TROOPER
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:			iImageIndex = 41;		break;
#endif
#ifdef ARA_CHARACTER_BASE
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:				iImageIndex = 42;		break;
#endif
#ifdef ARA_CHANGE_CLASS_FIRST
	case CX2Unit::UC_ARA_LITTLE_HSIEN:					iImageIndex = 43;		break; //소선
	case CX2Unit::UC_ARA_SAKRA_DEVANAM:					iImageIndex = 44;		break; //제천
#endif //ARA_CHANGE_CLASS_FIRST

	case CX2Unit::UC_ELESIS_KNIGHT:						iImageIndex = 45;		break; // 엘리시스 노전직
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:				iImageIndex = 46;		break; // 엘리시스 세이버 나이트
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:				iImageIndex = 47;		break; // 엘리시스 파이로 나이트

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	case CX2Unit::UC_ARA_LITTLE_DEVIL:					iImageIndex = 48;		break;	// 아라 2-1 소마
	case CX2Unit::UC_ARA_YAMA_RAJA:						iImageIndex = 49;		break;	// 아라 2-2 명왕
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	case CX2Unit::UC_ELESIS_GRAND_MASTER:				iImageIndex = 50;		break; // 엘리시스 노전직
	case CX2Unit::UC_ELESIS_BLAZING_HEART:				iImageIndex = 51;		break; // 엘리시스 세이버 나이트
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환 ( 캐릭터 추가용 )
	case CX2Unit::UC_ADD_NASOD_RULER:					iImageIndex = 52;		break; // 애드 기본 전직
	case CX2Unit::UC_ADD_PSYCHIC_TRACER:				iImageIndex = 53;		break; // 애드 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:				iImageIndex = 54;		break; // 애드 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER

	default:
		break;
	}

	if( -1 == iImageIndex )
		return false;
	
	switch( eImageType )
	{
	case CIT_DungeonClear:
		{
			fileName = s_CharacterImageNameDungeonClear[ iImageIndex ].fileName;
			pieceName = s_CharacterImageNameDungeonClear[ iImageIndex ].pieceName;
			return true;
		} break;

	case CIT_160by160:
		{
			fileName = s_CharacterImageName160[ iImageIndex ].fileName;
			pieceName = s_CharacterImageName160[ iImageIndex ].pieceName;
			return true;
		} break;

	case CIT_160by160Gray:
		{
			fileName = s_CharacterImageName160Gray[ iImageIndex ].fileName;
			pieceName = s_CharacterImageName160Gray[ iImageIndex ].pieceName;
			return true;
		} break;
	case CIT_50by50:
		{
			fileName = s_CharacterImageName50[ iImageIndex ].fileName;
			pieceName = s_CharacterImageName50[ iImageIndex ].pieceName;
			return true;
		} break;
	case CIT_20by20:
		{
			fileName = s_CharacterImageName20[ iImageIndex ].fileName;
			pieceName = s_CharacterImageName20[ iImageIndex ].pieceName;
			return true;
		} break;

	case CIT_ClassInitial:
		{
			fileName = s_CharacterImageNameInitial[ iImageIndex ].fileName;
			pieceName = s_CharacterImageNameInitial[ iImageIndex ].pieceName;
			return true;
		} break;

	case CIT_MyGage:
		{
			fileName = s_CharacterImageNameMyGage[ iImageIndex ].fileName;
			pieceName = s_CharacterImageNameMyGage[ iImageIndex ].pieceName;
			return true;
		} break;

	case CIT_Party:
		{
			fileName = s_CharacterImageNameParty[ iImageIndex ].fileName;
			pieceName = s_CharacterImageNameParty[ iImageIndex ].pieceName;
			return true;
		} break;
// #ifdef REFORM_ENTRY_POINT	 	// 13-11-11, 진입 구조 개편, kimjh
	case CIT_UnitSlot:
		{
			fileName = s_CharacterImageUnitSlot[ iImageIndex ].fileName;
			pieceName = s_CharacterImageUnitSlot[ iImageIndex ].pieceName;
			return true;
		} break;
// #endif // REFORM_ENTRY_POINT	// 13-11-11, 진입 구조 개편, kimjh

	}

	return false;
}

#ifdef SERV_PVP_NEW_SYSTEM
/*static*/ bool CX2Data::GetPvpNpcImageName( OUT wstring& fileName, OUT wstring& pieceName, CX2UnitManager::NPC_UNIT_ID iNpcId )
{
	switch(iNpcId)
	{
	case CX2UnitManager::NUI_PVP_HERO_LOW:
		{
			fileName = L"DLG_PVP_State_Info0.tga";
			pieceName = L"LowStateSmall";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_PENENSIO:
		{
			fileName = L"DLG_PVP_State_Info0.tga";
			pieceName = L"PenensioStateSmall";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_NOA:
		{
			fileName = L"DLG_UI_Common_Texture23.tga";
			pieceName = L"E_Master_Medium";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_SPIKA:
		{
			fileName = L"DLG_UI_Common_Texture23.tga";
			pieceName = L"V_Princess_Medium";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_LIME:
		{
			fileName = L"DLG_UI_Common_Texture22.tga";
			pieceName = L"W_Sneaker_Medium";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_AMELIA:
		{
			fileName = L"DLG_UI_Common_Texture22.tga";
			pieceName = L"G_Archer_Medium";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_EDAN:
		{
			fileName = L"DLG_UI_Common_Texture24.tga";
			pieceName = L"B_MASTER_Medium";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_BALAK:
		{
			fileName = L"DLG_UI_Common_Texture24.tga";
			pieceName = L"R_FIST_Medium";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_CODE_Q_PROTO_00:
		{
			fileName = L"DLG_UI_Common_Texture25.tga";
			pieceName = L"C_NEMESIS_Medium";
		}
		break;
	case CX2UnitManager::NUI_PVP_HERO_APPLE:
		{
			fileName = L"DLG_UI_Common_Texture25.tga";
			pieceName = L"C_EMPRESS_Medium";
		}
		break;
	case CX2UnitManager::NUI_NPC_ELSWORD_01:
	case CX2UnitManager::NUI_NPC_ELSWORD_02:
	case CX2UnitManager::NUI_NPC_ELSWORD_03:
	case CX2UnitManager::NUI_NPC_ELSWORD_04:
	case CX2UnitManager::NUI_NPC_ELSWORD_05:
		{
			fileName = L"DLG_UI_Common_Texture16_NEW.tga";
			pieceName = L"ELSWORD_SMALL";
		}
		break;
	case CX2UnitManager::NUI_NPC_AISHA_01:
	case CX2UnitManager::NUI_NPC_AISHA_02:
	case CX2UnitManager::NUI_NPC_AISHA_03:
	case CX2UnitManager::NUI_NPC_AISHA_04:
	case CX2UnitManager::NUI_NPC_AISHA_05:
		{
			fileName = L"DLG_UI_Common_Texture21_NEW.tga";
			pieceName = L"AISHA_SMALL";
		}
		break;
	case CX2UnitManager::NUI_NPC_RENA_01:
	case CX2UnitManager::NUI_NPC_RENA_02:
	case CX2UnitManager::NUI_NPC_RENA_03:
	case CX2UnitManager::NUI_NPC_RENA_04:
	case CX2UnitManager::NUI_NPC_RENA_05:
		{
			fileName = L"DLG_UI_Common_Texture23_NEW.tga";
			pieceName = L"RENA_SMALL";
		}
		break;
	case CX2UnitManager::NUI_NPC_RAVEN_01:
	case CX2UnitManager::NUI_NPC_RAVEN_02:
	case CX2UnitManager::NUI_NPC_RAVEN_03:
	case CX2UnitManager::NUI_NPC_RAVEN_04:
	case CX2UnitManager::NUI_NPC_RAVEN_05:
		{
			fileName = L"DLG_UI_Common_Texture25_NEW.tga";
			pieceName = L"RAVEN_SMALL";
		}
		break;
	case CX2UnitManager::NUI_NPC_EVE_01:
	case CX2UnitManager::NUI_NPC_EVE_02:
	case CX2UnitManager::NUI_NPC_EVE_03:
	case CX2UnitManager::NUI_NPC_EVE_04:
	case CX2UnitManager::NUI_NPC_EVE_05:
		{
			fileName = L"DLG_UI_Common_Texture45_NEW.tga";
			pieceName = L"EVE_SMALL";
		}
		break;
	case CX2UnitManager::NUI_NPC_CHUNG_01:
	case CX2UnitManager::NUI_NPC_CHUNG_02:
	case CX2UnitManager::NUI_NPC_CHUNG_03:
	case CX2UnitManager::NUI_NPC_CHUNG_04:
	case CX2UnitManager::NUI_NPC_CHUNG_05:
		{
			fileName = L"DLG_UI_Common_Texture61_NEW.tga";
			pieceName = L"CHUNG_SMALL";
		}
		break;
	default:
		return false;
	}

	return true;
}

#endif

void CX2Data::ReadyCharacterMotion()
{
	// 노 전직
	//XSkinMeshReady( L"Motion_Elsword.x" );	
	//XSkinMeshReady( L"Motion_Lire.x" );	
	//XSkinMeshReady( L"Motion_Arme.x" );	
	//XSkinMeshReady( L"Motion_Raven.x" );	
	//XSkinMeshReady( L"Motion_EVE.x" );	
	//XSkinMeshReady( L"Motion_CHUNG.x" );	

// 	XSkinMeshReady( L"Motion_Mushroom_Basic.x" );	
// 	XSkinMeshReady( L"Motion_CHICKEN_ELTREE.x" );	
// 	XSkinMeshReady( L"Motion_Thief_Thin_Zero.x" );	
// 	XSkinMeshReady( L"Thief_Thin_Toto.x" );	
// 	XSkinMeshReady( L"Motion_Benders_Thief.x" );	


	XSkinMeshReady( L"Motion_Elsword_LK.x" );
	XSkinMeshReady( L"Motion_Elsword_MK.x" );
	XSkinMeshReady( L"Motion_Elsword_SK.x" );

	XSkinMeshReady( L"Motion_Lire_CR.x" );
	XSkinMeshReady( L"Motion_Lire_SR.x" );
	XSkinMeshReady( L"Motion_Lire_RTR.x" );

	XSkinMeshReady( L"Motion_Arme_HM.x" );
	XSkinMeshReady( L"Motion_Arme_DM.x" );
	XSkinMeshReady( L"Motion_Arme_BM.x" );

	XSkinMeshReady( L"Motion_Raven_ST.x" );
	XSkinMeshReady( L"Motion_Raven_OT.x" );
	XSkinMeshReady( L"Motion_Raven_WT.x" );

	XSkinMeshReady( L"Motion_EVE_EG.x" );
	XSkinMeshReady( L"Motion_EVE_AT.x" );
	XSkinMeshReady( L"Motion_EVE_PR.x" );

	XSkinMeshReady( L"Motion_CHUNG_CSG.x" );
	XSkinMeshReady( L"Motion_CHUNG_CFG.x" );
	XSkinMeshReady( L"Motion_CHUNG_CHG.x" );
// 
// 
	XSkinMeshReady( L"Motion_Elsword_RS.x" );
	XSkinMeshReady( L"Motion_Elsword_SK.x" );		
	XSkinMeshReady( L"Motion_Lire_WS.x" );
	XSkinMeshReady( L"Motion_Lire_GA.x" );	
	XSkinMeshReady( L"Motion_Arme_EM.x" );
	XSkinMeshReady( L"Motion_Arme_VP.x" );	
	XSkinMeshReady( L"Motion_Raven_BM.x" );
	XSkinMeshReady( L"Motion_Raven_RF.x" );	
	XSkinMeshReady( L"Motion_EVE_NS.x" );
	XSkinMeshReady( L"Motion_EVE_EP.x" );	
	XSkinMeshReady( L"Motion_CHUNG_CDC.x" );
	XSkinMeshReady( L"Motion_CHUNG_CIP.x" );

#ifdef ARA_CHANGE_CLASS_FIRST
	XSkinMeshReady( L"Motion_Ara_LH.x" );	/// 아라 1차 전직_소선
	XSkinMeshReady( L"Motion_Ara_SD.x" );	/// 아라 2차 전직_제천
#endif //ARA_CHANGE_CLASS_FIRST

#ifdef SERV_ARA_CHANGE_CLASS_SECOND // 김태환
	XSkinMeshReady( L"Motion_Ara_LD.x" );	/// 아라 1차 전직_소마
	XSkinMeshReady( L"Motion_Ara_YR.x" );	/// 아라 2차 전직_명왕
#endif // SERV_ARA_CHANGE_CLASS_SECOND

#ifdef SERV_ELESIS_SECOND_CLASS_CHANGE	  // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트
	XSkinMeshReady( L"Motion_EL.x" );
	XSkinMeshReady( L"Motion_EL_SK.x" );
	XSkinMeshReady( L"Motion_EL_PK.x" );
	XSkinMeshReady( L"MOTION_EL_BH.x" );
	XSkinMeshReady( L"MOTION_EL_GM.x" );
#endif // SERV_ELESIS_SECOND_CLASS_CHANGE // 김종훈, 엘리시스 1-2 그랜드 마스터, 2-2 블레이징 하트

#ifdef SERV_9TH_NEW_CHARACTER // 김태환
	XSkinMeshReady( L"Motion_ADD_PT.x" );	/// 애드 1차 전직 사이킥 트레이서
#endif //SERV_9TH_NEW_CHARACTER

#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	XSkinMeshReady( L"Motion_ADD_LP.x" );	/// 애드 2차 전직 루나틱 사이커
#endif //SERV_ADD_LUNATIC_PSYKER
}

/** @function : ResetFieldManager
	@brief : 기존의 Field정보를 clear 시키고, FieldData.lua 스크립트를 다시 파싱하여 정보를 만듦
	@param : const char* szScriptFileName_
*/
void CX2Data::ResetBattleFieldManager( const wchar_t* wszScriptFileName_ )
{
	m_BattleFieldManager.ReOpenScriptFile( wszScriptFileName_ );
}


/** @function : GetSelectUnitLevel
	@brief : 현재 선택된 캐릭터의 레벨 얻기	
*/
//오현빈 // 2012-12-14 // 널체크 코드 길이 줄이기 위해 추가 했습니다.
const int CX2Data::GetSelectUnitLevel() const
{
	if( NULL != m_pMyUser && NULL != m_pMyUser->GetSelectUnit() )
		return m_pMyUser->GetSelectUnit()->GetUnitData().m_Level;

	return 0;
}
CX2Unit::UNIT_TYPE CX2Data::GetSelectUnitType() const
{
	if( NULL != m_pMyUser &&
		NULL != m_pMyUser->GetSelectUnit() &&
		NULL != m_pMyUser->GetSelectUnit()->GetUnitTemplet() )
	{
		return m_pMyUser->GetSelectUnit()->GetUnitTemplet()->m_UnitType;
	}

	return CX2Unit::UT_NONE;
}
CX2Unit::UNIT_CLASS CX2Data::GetSelectUnitClass() const
{
	if( NULL != m_pMyUser &&
		NULL != m_pMyUser->GetSelectUnit() &&
		NULL != m_pMyUser->GetSelectUnit()->GetUnitTemplet() )
	{
		return m_pMyUser->GetSelectUnit()->GetUnitTemplet()->m_UnitClass;
	}

	return CX2Unit::UC_NONE;
}
BYTE CX2Data::GetSelectUnitMemberShipGrade() const
{
	if( NULL != m_pMyUser && NULL != m_pMyUser->GetSelectUnit() )
		return m_pMyUser->GetSelectUnit()->GetUnitData().m_byMemberShipGrade;

	return 0;
}
SEnum::SERVER_GROUP_ID CX2Data::GetSelectUnitServerGroupID() const
{
	if( NULL != m_pMyUser &&
		NULL != m_pMyUser->GetSelectUnit() )
	{
		return m_pMyUser->GetSelectUnit()->GetServerGroupID();
	}

	return SEnum::SGI_INVALID;
}
void CX2Data::SetSelectUnitED( int iED_ )
{
	if( NULL != m_pMyUser &&
		NULL != m_pMyUser->GetSelectUnit() )
	{
		m_pMyUser->GetSelectUnit()->AccessUnitData().m_ED = iED_;
	}

	if( NULL != m_pUIManager &&
		NULL != m_pUIManager->GetUIInventory() )
	{
		m_pUIManager->GetUIInventory()->SetEDString( iED_ );
	}
}

SEnum::DUNGEON_ID CX2Data::GetCurrentDungeonID()
{
	if( NULL != GetPartyManager() &&
		NULL != GetPartyManager()->GetMyPartyData() )
	{
		return static_cast<const SEnum::DUNGEON_ID>(GetPartyManager()->GetMyPartyData()->m_iDungeonID);
	}
	return SEnum::DI_NONE;
}


#ifdef SERV_LOCAL_RANKING_SYSTEM //지인시스템
CX2ProfileManager* CX2Data::ResetProfileManager()
{
	SAFE_DELETE( m_pProfileManager );
	m_pProfileManager = new CX2ProfileManager();

	return m_pProfileManager;
}


bool CX2Data::AddArea_LUA()
{
	CX2ProfileManager::Area AddArea;

	KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
#ifndef X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING

	LUA_GET_VALUE_RETURN( luaManager, "m_UpperAreaName", AddArea.UpperArea, L"", return false; );
	LUA_GET_VALUE_RETURN( luaManager, "m_UnderAreaName", AddArea.UnderArea, L"", return false; );

	if( 0 == StrCmp( AddArea.UpperArea.c_str(), L"" ) )
	{
		return false;
	}

	m_pProfileManager->GetArea().push_back( AddArea );

	return true;
}

bool CX2Data::LoadAreaData()
{
	if( NULL == m_pProfileManager )
		return false;

	if( true == m_pProfileManager->GetIsLoadComplete() )
		return false;

    if ( g_pKTDXApp->LoadLuaTinker( L"Area.lua" ) == false )
    {
		ErrorLogMsg( XEM_ERROR9, L"Area.lua" );

		return false;
    }

	//vector<wstring> AreaSort = m_pProfileManager->GetArea();
	//wstring sort = L"";

	//for(vector<wstring>::size_type i = 1;i < AreaSort.size() - 1; ++i)
	//{
	//	if( 1 == StrCmp( AreaSort[i].data(), AreaSort[i+1].data() ) )
	//	{
	//		sort = AreaSort[i];
	//		AreaSort[i] = AreaSort[i+1];
	//		AreaSort[i+1] = sort;
	//	}
	//}


	m_pProfileManager->SetIsLoadComplete( true );

	return true;
}
#endif //SERV_LOCAL_RANKING_SYSTEM 


#ifdef ADDED_RELATIONSHIP_SYSTEM
CX2RelationshipManager* CX2Data::ResetRelationshipManager()
{
	SAFE_DELETE( m_pRelationshipManager );
	m_pRelationshipManager = new CX2RelationshipManager;
//	m_pRelationshipManager->OpenScriptFile( L"RelationshipData.lua" );
//	m_pRelationshipManager->

	return m_pRelationshipManager;

}

CX2RelationshipEffectManager* CX2Data::ResetRelationshipEffectManager()
{
	SAFE_DELETE( m_pRelationshipEffectManager );
	m_pRelationshipEffectManager = new CX2RelationshipEffectManager;
	m_pRelationshipEffectManager->SetRelationEffectInfoIndex();
	return m_pRelationshipEffectManager;

}
#endif	//	ADDED_RELATIONSHIP_SYSTEM

#ifdef REFORM_ENTRY_POINT //JHKang
#pragma region RotateYSelectCharacterType
float CX2Data::RotateYSelectCharacterType( CX2Unit::UNIT_CLASS eClass_ )
{
	float fRotY = 0.f;

	switch ( eClass_ )
	{
#pragma region Elsword
	case CX2Unit::UC_ELSWORD_SWORDMAN:
		{
			fRotY = 25.f;
		} break;
	case CX2Unit::UC_ELSWORD_KNIGHT:
		{
			fRotY = 30.f;
		} break;
	case CX2Unit::UC_ELSWORD_MAGIC_KNIGHT:
		{
			fRotY = 45.f;
		} break;
	case CX2Unit::UC_ELSWORD_SHEATH_KNIGHT:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_ELSWORD_LORD_KNIGHT:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_ELSWORD_RUNE_SLAYER:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_ELSWORD_INFINITY_SWORD:
		{
			fRotY = 15.f;
		} break;
#pragma endregion 엘소드 회전 값

#pragma region Aisha
	case CX2Unit::UC_ARME_VIOLET_MAGE:
		{
			fRotY = 40.f;
		} break;
	case CX2Unit::UC_ARME_HIGH_MAGICIAN:
		{
			fRotY = 35.f;
		} break;
	case CX2Unit::UC_ARME_DARK_MAGICIAN:
		{
			fRotY = 40.f;
		} break;
	case CX2Unit::UC_ARME_BATTLE_MAGICIAN:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_ARME_ELEMENTAL_MASTER:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_ARME_VOID_PRINCESS:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_ARME_DIMENSION_WITCH:
		{
			fRotY = 30.f;
		} break;
#pragma endregion 아이샤 회전 값

#pragma region Lena
	case CX2Unit::UC_LIRE_ELVEN_RANGER:
		{
			fRotY = 30.f;
		} break;
	case CX2Unit::UC_LIRE_COMBAT_RANGER:
		{
			fRotY = 30.f;
		} break;
	case CX2Unit::UC_LIRE_SNIPING_RANGER:
		{
			fRotY = 30.f;
		} break;
	case CX2Unit::UC_LIRE_TRAPPING_RANGER:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_LIRE_WIND_SNEAKER:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_LIRE_GRAND_ARCHER:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_LIRE_NIGHT_WATCHER:
		{
			fRotY = 10.f;
		} break;
#pragma endregion 레나 회전 값

#pragma region Raven
	case CX2Unit::UC_RAVEN_FIGHTER:
		{
			fRotY = 15.f;
		} break;
	case CX2Unit::UC_RAVEN_SOUL_TAKER:
		{
			fRotY = 30.f;
		} break;
	case CX2Unit::UC_RAVEN_OVER_TAKER:
		{
			fRotY = 15.f;
		} break;
	case CX2Unit::UC_RAVEN_WEAPON_TAKER:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_RAVEN_BLADE_MASTER:
		{
			fRotY = 35.f;
		} break;
	case CX2Unit::UC_RAVEN_RECKLESS_FIST:
		{
			fRotY = 15.f;
		} break;
	case CX2Unit::UC_RAVEN_VETERAN_COMMANDER:
		{
			fRotY = 30.f;
		} break;
#pragma endregion 레이븐 회전 값

#pragma region Eve
	case CX2Unit::UC_EVE_NASOD:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_EVE_EXOTIC_GEAR:
		{
			fRotY = 15.f;
		} break;
	case CX2Unit::UC_EVE_ARCHITECTURE:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_EVE_ELECTRA:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_EVE_CODE_NEMESIS:
		{
			fRotY = 5.f;
		} break;
	case CX2Unit::UC_EVE_CODE_EMPRESS:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_EVE_BATTLE_SERAPH:
		{
			fRotY = 5.f;
		} break;
#pragma endregion 이브 회전 값

#pragma region Chung
	case CX2Unit::UC_CHUNG_IRON_CANNON:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_CHUNG_FURY_GUARDIAN:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_CHUNG_SHOOTING_GUARDIAN:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_CHUNG_SHELLING_GUARDIAN:
		{
			fRotY = 5.f;
		} break;
	case CX2Unit::UC_CHUNG_IRON_PALADIN:
		{
			fRotY = 20.f;
		} break;
	case CX2Unit::UC_CHUNG_DEADLY_CHASER:
		{
			fRotY = 25.f;
		} break;
	case CX2Unit::UC_CHUNG_TACTICAL_TROOPER:
		{
			fRotY = 25.f;
		} break;
#pragma endregion 청 회전 값

#pragma region Ara
	case CX2Unit::UC_ARA_MARTIAL_ARTIST:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_ARA_LITTLE_HSIEN:
		{
			fRotY = 15.f;
		} break;
	case CX2Unit::UC_ARA_LITTLE_DEVIL:
		{
			fRotY = 15.f;
		} break;
	case CX2Unit::UC_ARA_SAKRA_DEVANAM:
		{
			fRotY = 5.f;
		} break;
	case CX2Unit::UC_ARA_YAMA_RAJA:
		{
			fRotY = 13.f;
		} break;
#pragma endregion 아라 회전 값

#pragma region Elesis
	case CX2Unit::UC_ELESIS_KNIGHT:
		{
			fRotY = 30.f;
		} break;
	case CX2Unit::UC_ELESIS_SABER_KNIGHT:
		{
			fRotY = 25.f;
		} break;
	case CX2Unit::UC_ELESIS_PYRO_KNIGHT:
		{
			fRotY = 10.f;
		} break;
	case CX2Unit::UC_ELESIS_GRAND_MASTER:
		{
			fRotY = 15.f;
		} break;
	case CX2Unit::UC_ELESIS_BLAZING_HEART:
		{
			fRotY = 8.f;
		} break;
#pragma endregion 엘리시스 회전 값

#pragma region ADD
	case CX2Unit::UC_ADD_NASOD_RULER:
		{
			fRotY = 20.f;
		} break;
#ifdef SERV_ADD_LUNATIC_PSYKER // 김태환
	case CX2Unit::UC_ADD_LUNATIC_PSYKER:
		{
			fRotY = 3.f;
		} break;
#endif //SERV_ADD_LUNATIC_PSYKER
#pragma endregion 애드 회전 값

	default:
		{
			fRotY = 10.f;
		} break;
	}

	return fRotY;
}
#pragma endregion 유닛 회전 값 지정
#endif //REFORM_ENTRY_POINT

#ifdef SERV_GLOBAL_MISSION_MANAGER
CX2GlobalMissionManager* CX2Data::ResetGlobalMissionManager()
{
	SAFE_DELETE( m_pGlobalMissionManager );
	m_pGlobalMissionManager = new CX2GlobalMissionManager();

	return m_pGlobalMissionManager;
}
#endif SERV_GLOBAL_MISSION_MANAGER