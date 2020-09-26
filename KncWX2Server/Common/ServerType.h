#pragma once

enum ServerType
{
    ST_GUILD            = 0x00000001,   // 길드관련 기능 가능.
    ST_BROADCASTING     = 0x00000002,   // 방송용 서버. 서버에서의 차이점은 없으나, 클라이언트에 알려줌.
    ST_OPEN_TESOP       = 0x00000004,   // Exp, GP의 상한선 제한을 3배로 늘린다.
    ST_INTERNAL_TESOP   = 0x00000008,   // play time 확인 패스, 체크섬 확인 패스, db에 모드카운트 기록 안함.
    ST_NOUSE_01         = 0x00000010,   // 
    ST_INDIGO           = 0x00000020,   // 용사의 섬
    ST_NPGG_ON          = 0x00000040,   // NProtect 활성화
    ST_NPGG_KILL        = 0x00000080,   // NProtect 문제유저 적발시 접속 종료 여부.
    ST_DEBUG_AUTH       = 0x00000100,   // '디버그 인증'이라고 읽도록 하자. DB의 PASSWD란과 곧바로 비교.
    ST_AHNHS_ON         = 0x00000200,   // HShield 활성화
    ST_AHNHS_KILL       = 0x00000400,   // HShield 문제 유저 적발시 접속 종료 여부.
    ST_FORCE_DWORD      = 0xFFFFFFFF,
};