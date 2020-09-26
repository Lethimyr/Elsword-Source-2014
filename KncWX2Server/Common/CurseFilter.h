#ifndef _CurseFilter_h_
#define _CurseFilter_h_
//#ifndef bool
//#define bool int
//#define false 0
//#define true (!false)
//#endif
//{{ 060103. kkurrung.
/*
--커스 필터에서 일본어 부분 추가 설명--
일본어 중에 '소'와 '응' 발음은 모양이 비슷하다.
욕중에 '소'가 들어가는 말을 '응'으로 바꿔서 사용하거나
반대로도 사용하는 경우가 있기 때문에 커스 필터에서는 모든 
'소' 나 '응' 을 '소'로 바꾸어서 필터링한다.
또 하이픈이나 마이너스 또는 전각 반각의 장음 표현 문자로 구획을
나누어 사용하는 사람들도 있기 때문에 이러한 '-' 와 유사한 문자들을
모두 제거하고 '응'과 '소'를 '소'로 교체해서 필터링 한다.
*/
//}} kkurrung.

void LoadNickNameFilter( const char* szFilename );
bool IsNickNameString( const wchar_t* szStringSrc );

//extern void LoadCurses(const char *filename);
//extern bool IsCurse(const char *str);
//extern char *ConvertString(char *str, int max_len);


#endif
