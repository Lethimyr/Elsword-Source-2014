#pragma once

class   CX2Unit;
class   CX2Unit_PreHeader
{
public:

	enum EQIP_POSITION
	{
		EP_NONE		= 0,
		EP_QUICK_SLOT,			//퀵슬롯 아이템

		//무기
		EP_WEAPON_HAND,			//무기
		EP_WEAPON_TEMP1,		//임시1
		EP_WEAPON_TEMP2,		//임시2
		EP_WEAPON_TEMP3,		//임시3

		//방어구
		EP_DEFENCE_HAIR,		//헤어스타일
		EP_DEFENCE_FACE,		//얼굴
		EP_DEFENCE_BODY,		//상의
		EP_DEFENCE_LEG,			//하의
		EP_DEFENCE_HAND,		//장갑
		EP_DEFENCE_FOOT,		//신발
		EP_DEFENCE_TEMP1,		//임시1
		EP_DEFENCE_TEMP2,		//임시2
		EP_DEFENCE_TEMP3,		//임시3

		//액세서리
		EP_AC_TITLE,			//칭호
		EP_AC_HAIR,				//헤어
		EP_AC_FACE1,			//얼굴(상)
		EP_AC_FACE2,			//얼굴(중)
		EP_AC_FACE3,			//얼굴(하)
		EP_AC_BODY,				//상의
		EP_AC_LEG,				//다리
		EP_AC_ARM,				//팔
		EP_AC_RING,				//반지
		EP_AC_NECKLESS,			//목걸이
		EP_AC_WEAPON,			// 무기 악세사리
		EP_AC_TEMP2,			//임시2
		EP_AC_TEMP3,			//임시3
		EP_AC_TEMP4,			//임시4
		EP_AC_TEMP5,			//임시5

		//필살기
		EP_SKILL_1,				//1단계 필살기
		EP_SKILL_2,				//2단계 필살기
		EP_SKILL_3,				//3단계 필살기
		EP_SKILL_TEMP1,			//임시1
		EP_SKILL_TEMP2,			//임시2
		EP_SKILL_TEMP3,			//임시3
			
		EP_RAVEN_LEFT_ARM,		// dmlee 2008.07.31 - 레이븐 왼팔, 오른팔 나누면서	
		EP_WEAPON_SECOND,		// dmlee 2008.12.12 - 두 번째 무기

#ifdef SERV_NEW_ONE_PIECE_AVATAR_SLOT
		EP_ONEPIECE_FASHION,	// 원피스 아바타
#endif //SERV_NEW_ONE_PIECE_AVATAR_SLOT

		EP_END,
	};
};