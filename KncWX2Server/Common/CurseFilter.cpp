#pragma   warning( disable :4018 )
#include <set>
#include <string>
#include <windows.h>
#include <KncUtil.h>
#include <algorithm> // Unique를 사용하기 위해.
#include <iostream>  // std::cout 사용하기 위해.
#include "CurseFilter.h"

#define MAX_CURSE_STRING    32768
#define HUGE_NUMBER     MAX_CURSE_STRING
#define CURSE_FILE_NAME     "table/Curses"
static char curse_string[MAX_CURSE_STRING];
static bool bInit = false;

#define MAX_LENGTH  64
std::set<std::wstring>   g_setNickName;

void LoadNickNameFilter( const char* szFilename )
{
    static wchar_t comment_chars[] = { L"/;*" };
    static wchar_t swcNMask = 0xFF9D; // 카타카나 반각응
    static wchar_t swcSoMask = 0xFF7F; // 카타카나 반각소


    g_setNickName.clear();

    std::cout << " * Unicode Curse Filter Load Start *" << std::endl;

    std::wstring strFileName = KncUtil::toWideString(szFilename);
    FILE* fp = _wfopen( strFileName.c_str(), L"rb" );
    if( !fp )   return;

    do
    {
        wchar_t szStringSrc[MAX_LENGTH] = {0, };
        wchar_t szStringDes[MAX_LENGTH] = {0, };
        //fgets(szString, MAX_LENGTH, fp);
        fwscanf( fp, L"%s", szStringSrc );
        if( !wcschr( comment_chars, szStringSrc[0] ) )
        {
            // 060106. kkurrung.반각으로 수정.
            int iRet = ::LCMapStringW( LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH | LCMAP_LOWERCASE, szStringSrc, MAX_LENGTH, szStringDes, MAX_LENGTH );
            
            while( true )// 060106. kkurrung.반각 응을 반각 소로 변경.
            {
                wchar_t* pCh = wcsrchr( szStringDes, swcNMask );
                if( pCh == NULL )
                    break;
                *pCh = swcSoMask;
            }//end while

            g_setNickName.insert( szStringDes );
        }//end if
    } while( !feof(fp) );

}

bool IsNickNameString( const wchar_t* szStringSrc )
{
    if( g_setNickName.empty() )
    {
        LoadNickNameFilter( "Curse.txt" );
    }

	#define MaxMaskRow      4

    const wchar_t* pszStringSrc = szStringSrc;
    wchar_t szStringBuff[MAX_LENGTH] = {0, };
    wchar_t* pszStringBuff = szStringBuff;
    static wchar_t swcMask[MaxMaskRow] =
    {
        { 0x30FC }, //카타카나 전각 짝데기
        { 0xFF70 }, //카타카나 반각 짝데기
        { 0xFFE3 }, //반각 페이지에 있는 괘선 짝데기
        { 0xFF0D }  //반각 페이에 있는 마이너스표시
        //{ 0x83, 0x93 }  //일본어 'ㅇ' 받침 코드, { 0x83, 0x5C } //일본어 '소'
    };

	static wchar_t swcNMask[2] = 
	{
		{ 0x30F3 }, // 전각 '응'
		{ 0xFF9D }	// 반각 '응'
	};

    do
    {
        for( int i = 0 ; i < MaxMaskRow ; ++i ) //짝데기 문자를 찾아서 패스하는 루틴
        {
            if( *pszStringSrc == swcMask[i] )
            {
                //fprintf( stderr, "0x%X, 0x%X, 0x%X, 0x%X\\n", *pszStringSrc, suiMask[i][0], *(pszStringSrc + 1), suiMask[i][1] );
                ++pszStringSrc; // 2바이트 문자이기 때문에. +2를 하지 않는다.
                break;
            }//end if
        }//end for

        if( i == MaxMaskRow ) // 짝데기 문자가 아니라면 '응'인지 확인하고 '소'로 변경 한다.
        {
            if( *pszStringSrc == swcNMask[0] )
            {
                *pszStringBuff = 0x30BD;
            }
            else if( *pszStringSrc == swcNMask[1] )
            {
				*pszStringBuff = 0xFF7F;
            }
			else
			{
				*pszStringBuff = *pszStringSrc;
			}//end if ... else ... if

            pszStringBuff++;
            pszStringSrc++;        
        }//end if
    } while(*pszStringSrc != L'\0');

    wchar_t szStringDes[MAX_LENGTH] = {0, };
    int iRet = ::LCMapStringW( LOCALE_SYSTEM_DEFAULT, LCMAP_HALFWIDTH | LCMAP_LOWERCASE, szStringBuff, MAX_LENGTH, szStringDes, MAX_LENGTH );
    std::wstring strString = szStringDes;

    std::set<std::wstring>::const_iterator   sit;
    for( sit = g_setNickName.begin(); sit != g_setNickName.end(); sit++ )
    {
        //fprintf(stderr, "%s/%s(%d) ", sit->c_str(), strString.c_str(), strString.find( *sit ));
        if( strString.find( *sit ) != -1 )
        {
            return true;
        }
    }
    return false;
}

static inline void filterChar(const char *src, char *dst, const char *chars)
{
    char *srcp = (char *) src;
    char *dstp = dst;

    while (*srcp) {
    if (strchr(chars, *srcp) == NULL) {
        *dstp = *srcp;   dstp += 1;
    } else {
        int msb, lsb;
        msb = *(unsigned char *)(srcp-1);
        lsb = *(unsigned char *)srcp;
        if (srcp > src && (
        (msb == 0x82 && lsb > 0x9e && lsb < 0xf2)   // hirakana
        || (msb == 0x83 && lsb > 0x3f && lsb < 0x97)    // katakana
            || (msb >= 0x88 && msb <= 0x9f && msb != 0x7f   // kanji 1 group
            && lsb > 0x3f && lsb < 0xfd)
        || (msb >= 0xe0 && msb <= 0xfc && msb != 0x7f   // kanji 2 group
            && lsb > 0x3f && lsb < 0xfd))) {
        *dstp = *srcp;   dstp += 1;
        }
    }
    srcp += 1;
    }
    *dstp = 0;
}

static inline char *getField(const char *buf, int delim, char *field)
{
    unsigned char *src = (unsigned char *)buf;
    while ((int)*src == delim && *src != 0) src++;
    while ((int)*src != delim && *src != 0) *field ++ = *src ++;

    *field = '\0';
    return (char *)src;
}

void LoadCurses(const char *filename)
{
    
    static char comment_chars[] = { "$;\n" };
    char buf[128];
    char *ptr = curse_string;
    FILE *file;

    if (bInit) return;


    if (file = fopen(filename, "r"), !file) {
    fprintf(stderr, "! can't open [%s]\n", filename);
    return;
    }

    while (fgets(buf, sizeof(buf), file)) {
    if (strchr(comment_chars, buf[0])) continue;

        ptr += sprintf(ptr, buf);

        if (ptr - curse_string >= MAX_CURSE_STRING) {
            fprintf(stderr, "! %s is too long, pass.\n", filename);
            curse_string[MAX_CURSE_STRING - 1] = 0;
            break;
        }
    }
    fclose(file);

    fprintf(stderr, "* parsing [%s]. %d bytes read\n", filename, ptr - curse_string);

    return;
}

bool IsCurse(const char * str)
{
    char string[HUGE_NUMBER], line[HUGE_NUMBER], word[HUGE_NUMBER];
    char *ptr = curse_string;
    const char *sub, *subs, *subw;

    if (!str) return false;

    // filter characters to keep users from
    // speaking curses mixed with white spaces 
    // or punctuaion mark

    filterChar(str, string, "\t _-.:^");


    // optimized for multibyte code set.

    while (ptr = getField(ptr, '\n', line), line[0])
    {
    getField(line, '\t', word); 
    sub = string;
    while (*sub && *(sub + 1)) {
        subs = sub; // current comparison pointer in string
        subw = word;    // current comparison pointer in word
        while (*subs && (*subs == *subw)) {
        subs++; subw++;
        if (*subw == 0) {
            return true;        // reach to null, curse
        }
        }
        sub += *sub < 0 ? 2 : 1;
    }
    }

    return false;
}

char *ConvertString(char * str, int max_len)
{
    char result[MAX_CURSE_STRING], string[MAX_CURSE_STRING];
    char line[128], curse[128], replace[128];
    char *ptr = curse_string, *lptr, *cptr;

    if (!str) return NULL;

    if (!max_len || max_len >= sizeof(result)) return str;

    // skip converting, if no curse is found.
    // in fact, 'isCurse' and 'convert' are redundant.
    // but it can be ignored and more efficient 
    // because users speak a normal sentence 
    // much more than a curse

    if (!IsCurse(str)) return str;


    // filter characters to keep users from
    // speaking curses mixed with white spaces 
    // or punctuaion mark
    filterChar(str, string, "\t _-.:^");

    while (ptr = getField(ptr, '\n', line), line[0]) {

    lptr = line;
    lptr = getField(lptr, '\t', curse);     // get the first field  
    lptr = getField(lptr, '\t', replace);   // get the second field
    cptr = string;
    strcpy(result, string);

    while (cptr = strstr(string, curse), cptr) {
        // if there is no matching replacement of curse word
        if (!replace[0]) {
        str[0] = NULL;
        return str;
        }

        if ((max_len - 1) < (cptr - string) + strlen(replace) + strlen(cptr + strlen(curse))) {
                // string too long, remove the curses that linger around
                strncpy(result, string, cptr - string);
                strcpy(result + (cptr - string), cptr + strlen(curse));
            } else {                    
            strncpy(result, string, cptr - string);
            strcpy(result + (cptr - string), replace);
            strcpy(result + (cptr - string + strlen(replace)), cptr + strlen(curse));
            }
        // refresh intermediate result
        strcpy(string, result);
    }
    }

    result[max_len] = 0;        // prevent overflow
    strcpy(str, result);

    return str;
}
