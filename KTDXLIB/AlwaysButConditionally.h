#pragma once 








#ifdef IN_TESTING_THEMIDA_BY_TOOL_TEAM
#	pragma message( "THEMIDA_BY_TOOL_TEAM, REFACTORING_BY_TOOL_TEAM, CPPUNIT_BY_TOOL_TEAM, NO_ALL_KILL_HACKING_TEST 등이 적용되고 있습니다. 본섭에 패치되면 안됩니다!")
// 툴팀에서 themida 적용 테스트 중 입니다.
#	define THEMIDA_BY_TOOL_TEAM
// themida를 적용하기 위해 필요한 refactoring 작업 진행중입니다. 
#	define REFACTORING_BY_TOOL_TEAM
// cppunit unittest library 
//#	define CPPUNIT_BY_TOOL_TEAM
// 몬스터 모두 죽이는 해킹 막기 테스트, REFACTORING_BY_TOOL_TEAM와 같이 정의 되어야 합니다.
//#	define NO_ALL_KILL_HACKING_TEST  
#endif IN_TESTING_THEMIDA_BY_TOOL_TEAM




#ifdef IN_TESTING_THEMIDA_BY_TOOL_TEAM
#else IN_TESTING_THEMIDA_BY_TOOL_TEAM
#	pragma message( "DAMAGE_HISTORY, VIRTUALIZER_CODE가 적용됩니다.")
#	define DAMAGE_HISTORY	// 데미지 히스토리(핵방지용)
#	define VERIFY_NPC_HP	// 핵 표본 몬스터 HP 유효성 검사 (데미지 히스토리와 같이 사용됨)
//{{ hoon.2009-10-26. 코드 부분함호화 라이브러리.
#	define VIRTUALIZER_CODE
//}} hoon.2009-10-26.
#endif IN_TESTING_THEMIDA_BY_TOOL_TEAM













#ifdef VIRTUALIZER_CODE
#	include "../X2Lib/VirtualizerSDK.h"
#	define ELSWORD_VIRTUALIZER_START	VIRTUALIZER_START
#	define ELSWORD_VIRTUALIZER_END		VIRTUALIZER_END
#else VIRTUALIZER_CODE
#	define ELSWORD_VIRTUALIZER_START
#	define ELSWORD_VIRTUALIZER_END
#endif VIRTUALIZER_CODE






#ifdef THEMIDA_BY_TOOL_TEAM
#	pragma message( "툴팀에서 themida 적용 테스트 중 입니다." )
#	include "../X2Lib/ThemidaSDK.h"
#	define THEMIDA_VM_START					VM_START
#	define THEMIDA_VM_END					VM_END
#	define THEMIDA_CODEREPLACE_START		CODEREPLACE_START
#	define THEMIDA_CODEREPLACE_END			CODEREPLACE_END
//#	define THEMIDA_ENCODE_START				ENCODE_START // 크래시 자꾸 생겨서 적용 안시킴
//#	define THEMIDA_ENCODE_END				ENCODE_END
#	define THEMIDA_ENCODE_START			
#	define THEMIDA_ENCODE_END
#	define THEMIDA_CLEAR_START				CLEAR_START
#	define THEMIDA_CLEAR_END				CLEAR_END            
#	define THEMIDA_CHECK_PROTECTION			CHECK_PROTECTION
#	define THEMIDA_CHECK_CODE_INTEGRITY		CHECK_CODE_INTEGRITY
#	define THEMIDA_REPORT_IF_TAMPERED( variableName, variableValue, reportString ) if( variableValue != variableName ) { g_pKTDXApp->SetFindHacking( true, reportString ); }
#else THEMIDA_BY_TOOL_TEAM
#	define THEMIDA_VM_START				
#	define THEMIDA_VM_END				
#	define THEMIDA_CODEREPLACE_START	
#	define THEMIDA_CODEREPLACE_END		
#	define THEMIDA_ENCODE_START			
#	define THEMIDA_ENCODE_END
#	define THEMIDA_CLEAR_START
#	define THEMIDA_CLEAR_END
#	define THEMIDA_CHECK_PROTECTION( a, b )
#	define THEMIDA_CHECK_CODE_INTEGRITY( a, b )
#	define THEMIDA_REPORT_IF_TAMPERED( variableName, variableValue, reportString )
#endif THEMIDA_BY_TOOL_TEAM


#ifdef CPPUNIT_BY_TOOL_TEAM
#	if defined(DEBUG) || defined(_DEBUG)
#		pragma comment( lib, "cppunitd.lib" )
#	else
#		pragma comment( lib, "cppunit.lib" )
#	endif 
#endif CPPUNIT_BY_TOOL_TEAM

