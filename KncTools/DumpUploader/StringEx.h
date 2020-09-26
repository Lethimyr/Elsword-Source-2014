//----------------------------------------------------------------
// Name: StringEx.h v2.0
// Version: 2.0
// Desc: more advanced string manupulation class named 'CStringEx'
//       which supports 
//             1) string edit distance
//             2) exact KMP string matching
//             3) 2D, 3D approximate pattern matching(not yet)
//             4) advanced parsing functions
//       (c) 2000 KOGSoft by seojt@kogsoft.com
//       modified on 14 August, new Millennium
//       last modified on 21 March, 2001
//----------------------------------------------------------------
// added on Version 2.0
// 
// 1) some bugs fixed
// 2) FormatVS();
// 3) edit distance functions
//    call EditDistance();
// added on Version 2.1
//
// 1) GetCfgToken() modified
//
// on October 25, 2001
//  - namespace for function grouping
//  - friend functions are converted to inline for efficiency
//
//  NOTE: NOT TESTED after this conversion
//

#pragma once
#pragma warning(disable: 4995)
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#if !defined(_CHAR_DEFINED_)
#define _CHAR_DEFINED_
  typedef char        CHAR;   // you can redefine this for Wide type char
  typedef char*       LPCHAR;
#endif // !defined(_CHAR_DEFINED_)


class CStringEx;
CStringEx StringConcat(const CHAR* lpsz1, const CHAR* lpsz2);


//-----------------------------------------------------------------------
// Name: class 'CStringEx'
// Desc: near same to MFC's CString, but added advanced member functions
//       (c) 2000 KOG by seojt@kogsoft.com
//
class CStringEx
{
private:
//public:
    CHAR*       m_lpszBuffer; // string buffer
    int         m_nLength;    // length of string, not size of buffer

    //members for FindFirst(), FindNext()
    int*        m_pFail;      // used for storing failure link in FindFirst()
    int         m_nNextIndex; // used for FindNext()

    // members for GetToken()
    int         m_nTokenIndex;// next token start index

private:
//public:
    // I use underscore(_) for private member functions
    int   _SafeAlloc(int nLength); // allocate buffer safely
    void  _SafeDelete();           // delete buffer safely

    inline CHAR* CStringEx::_StringCopy(CHAR* dest, const CHAR* src)
    {
        if ( _StringLength(src) == 0 )
            return NULL;

        return strcpy( dest, src );
    }//CStringEx::_StringCopy()

    inline void _SetString(const CHAR* lpsz) { 
        strcpy( m_lpszBuffer, lpsz );
    }
    inline int _StringLength(const CHAR* lpsz) const {
        if (lpsz == NULL) return 0;
        return (int)strlen( lpsz );
    }
    inline int _StringCompare(const CHAR* lpsz1, const CHAR* lpsz2) const {
        if (lpsz1 == NULL && lpsz2 == NULL) return 0;
        if (lpsz1 == NULL && lpsz2 != NULL) return -1;
        if (lpsz1 != NULL && lpsz2 == NULL) return 1;
        return stricmp( lpsz1, lpsz2 );
    }

    //............................................................
    // KMP matching functions for 'Find()'
    void  _KMPSetup(const CHAR* p, int* fail);
    int   _KMPMatch(const CHAR* p, const CHAR* t, const int* fail, 
                    int nStart);
    int   _KMPMatchCount(const CHAR* p, const CHAR* t, const int* fail, 
                    int nStart, int* pMark = NULL, int nMark = 1);

public:
    // how many constructor there are!
    CStringEx();                        // default constructor
    CStringEx(const CStringEx& string); // copy constructor
    CStringEx(const CHAR* lpsz);
    CStringEx(const CHAR  ch);
    CStringEx(long n);
    CStringEx(int n);
    CStringEx(double f);
    CStringEx(float f);
    ~CStringEx(); // it need not be virtual because this is stand alone class

    inline int   GetLength() const { return m_nLength; }
    inline int   IsEmpty() const { return m_nLength <= 0; }
    inline CHAR  GetAt(int nIndex) const { return m_lpszBuffer[ nIndex ]; }
    inline CHAR& operator[](int nIndex) { return m_lpszBuffer[ nIndex ]; }
    inline void  SetAt(int nIndex, CHAR ch) { m_lpszBuffer[ nIndex ] = ch; }
    
    inline operator CHAR*() const { return m_lpszBuffer; }
    inline CHAR* c_str() { return m_lpszBuffer; }
    inline operator int() const { return atoi(m_lpszBuffer); }   // get integer value of this string
    inline operator long() const { return atol(m_lpszBuffer); }  // get long value of this string
    inline operator float() const { return (float)atof(m_lpszBuffer); } // get float value of this string
    inline operator double() const { return atof(m_lpszBuffer); }// get double value of this string

    inline CStringEx& operator=(const CStringEx& stringSrc) {
        if ( _SafeAlloc( stringSrc.GetLength() ) )
            _StringCopy( m_lpszBuffer, stringSrc.GetBuffer() );
        return *this;
    }
    inline CStringEx& operator=(const CHAR* lpsz) {
        if ( _SafeAlloc( _StringLength(lpsz) ) )
            _StringCopy( m_lpszBuffer, lpsz );
        return *this;
    }
    inline CStringEx& operator=(const CHAR ch) {
        _SafeAlloc( 1 );
        m_lpszBuffer[0] = ch;
        m_lpszBuffer[1] = 0;
        return *this;
    }
    inline CStringEx& operator=(const int n) {
        _SafeDelete();

        char buffer[20];

        _itoa(n,buffer,10);
        if ( _SafeAlloc( _StringLength(buffer) ) )
            _StringCopy( m_lpszBuffer, buffer );
        return *this;
    }
    inline CStringEx& operator=(const long n) {
        _SafeDelete();

        char buffer[20];

        _ltoa(n,buffer,10);
        if ( _SafeAlloc( _StringLength(buffer) ) )
            _StringCopy( m_lpszBuffer, buffer );
        return *this;
    }
    inline CStringEx& operator=(const float f) {
        _SafeDelete();

        char buffer[20];

        sprintf(buffer, "%g", f);
        if ( _SafeAlloc( _StringLength(buffer) ) )
            _StringCopy( m_lpszBuffer, buffer );
        return *this;
    }
    inline CStringEx& operator=(const double f) {
        _SafeDelete();

        char buffer[20];

        sprintf(buffer, "%g", f);
        if ( _SafeAlloc( _StringLength(buffer) ) )
            _StringCopy( m_lpszBuffer, buffer );
        return *this;
    }

    inline CStringEx& operator+=(const CStringEx& string) {
        return operator+=( string.GetBuffer() );
    }
    inline CStringEx& operator+=(const CHAR* lpsz) {
        CHAR* lpszNew;
        int   len = _StringLength( lpsz );
        int   lenNew = m_nLength + len;

        lpszNew = new CHAR[ lenNew + 1 ];
        // concatenate this and string to 'lpszNew'
        _StringCopy( lpszNew, m_lpszBuffer );
        _StringCopy( lpszNew + m_nLength, lpsz );

        _SafeDelete();
        m_lpszBuffer = lpszNew;
        m_nLength    = lenNew;
        return *this;
    }
    inline CStringEx& operator+=(const CHAR ch) {
        char buffer[2]= { ch, 0 };
        return operator+=( buffer );
    }

    // return -1, 0 or 1 in lexicographical order
    inline int Compare(const CStringEx& string) const {
        return _StringCompare( m_lpszBuffer, string.GetBuffer() );
    }
    inline int Compare(const CHAR* lpsz) const {
        return _StringCompare( m_lpszBuffer, lpsz );
    }

    // substring functions
    CStringEx Mid(int nFirst, int nCount = -1) const;
    inline CStringEx Left(int nCount) const { return Mid(0, nCount); }
    inline CStringEx Right(int nCount) const { return Mid(m_nLength-nCount, nCount); }

    // case functions
    inline void MakeUpper()
    {
        if (m_nLength>0)
            _strupr( m_lpszBuffer );
    }
    inline void MakeLower()
    {
        if (m_nLength>0)
            _strlwr( m_lpszBuffer );
    }
    void MakeReverse();

    //............................................................
    // how difficult this is!
    void FormatVS(const CHAR* lpszFormat, ... );
    void Format(const CHAR* lpszFormat, ... );

    void TrimLeft(CHAR ch = ' ');
    void TrimRight(CHAR ch = ' ');
    inline void Trim(CHAR ch = ' ') {
        TrimLeft(ch);
        TrimRight(ch);
    }

    int Find(const CHAR* lpszSub, int nStart = 0);
    int FindCount(const CHAR* lpszSub, int nStart = 0);
    int FindFirst(const CHAR* lpszSub);
    int FindNext(const CHAR* lpszSub);
    int ReverseFind(const CHAR* lpszSub, int nRevStart = 0);

    inline CHAR* GetBuffer() const { return m_lpszBuffer; }
    inline CHAR* GetString() const { return m_lpszBuffer; }
    void ReleaseBuffer(int nNewLength = -1);


    //------------------------------------------------------------
    // general parsing functions
    //------------------------------------------------------------
private:
//public:
    int _IsIncluded(const CHAR* lpszCharSet, CHAR ch) const; // return index of ch on 'lpszCharSet'
    void _MoveString(CHAR* lpsz, int nDest, int nSrc);

public:
    // return index of ch on string
    // return -1 when there is no 'ch'
    inline int IsIncluded(CHAR ch) const
    {   // return index of ch on string
        return _IsIncluded( m_lpszBuffer, ch );
    }
    CStringEx SpanIncluding(const CHAR* lpszCharSet, int nIndex = 0) const;
    CStringEx SpanExcluding(const CHAR* lpszCharSet, int nIndex = 0) const;
    int FillChar(int nIndex, int nCount, CHAR ch);
    int Replace(CHAR chOld, CHAR chNew);
    int Replace(const CHAR* lpszOld, const CHAR* lpszNew);
    int Remove(CHAR ch);
    int Remove(const CHAR* lpsz, CHAR chSentinal = 127);
    inline int Insert(int nIndex, CHAR ch) {
        char str[2] = { ch, 0 };

        return Insert(nIndex, str);
    }
    int Insert(int nIndex, const CHAR* lpsz);
    int Delete(int nIndex, int nCount = 1);
    int FindOneOf(const CHAR* lpszCharSet) const;

public:
    //------------------------------------------------------------
    // advanced parsing functions
    //------------------------------------------------------------
    CStringEx GetToken(const CHAR* lpszDelimiter, 
                       int nStart = 0, int bInclude = 0);
    void GetToken(const CHAR* lpszDelimiter, CStringEx& strOut, 
                  int nStart = 0, int bInclude = 0);
    void PrepareGetNextTokenStart() { m_nTokenIndex = 0; }
    CStringEx GetNextToken(const CHAR* lpszDelimiter, 
                           int bInclude = 0);
    void GetNextToken(const CHAR* lpszDelimiter, CStringEx& strOut,
                      int bInclude = 0);
    CStringEx GetCFGToken(const CHAR* lpszStart, const CHAR* lpszEnd, 
                          int* ipStartIndex = NULL,
                          int* ipEndIndex = NULL,
                          int nStart = 0, int bBalancing = 1);
    void GetCFGToken(const CHAR* lpszStart, const CHAR* lpszEnd, 
                     CStringEx& strOut,
                     int* ipStartIndex = NULL,
                     int* ipEndIndex = NULL,
                     int nStart = 0, int bBalancing = 1);
    CStringEx GetCFGTokenExcluding(const CHAR* lpszStart, const CHAR* lpszEnd, 
                                   int* ipStartIndex = NULL,
                                   int* ipEndIndex = NULL,
                                   int nStart = 0, int bBalancing = 1);   


//------------------------------------------------------------
// string edit distance functions
//------------------------------------------------------------
public:
    int m_nSimilarity;   // local edit distance

    int m_nPatternStart; // aligned pattern index
    int m_nPatternEnd;   // aligned pattern index
    int m_nTextStart;    // aligned text index
    int m_nTextEnd;      // aligned text index
    
//private:
public:
    // we use score value 2 and -1, each for match and mismatch
    int _Weight(int i, int j, const CHAR* text, const CHAR* pattern) {
        return (pattern[i] == text[j]) ? 2 : -1;
          //                             ^ you can modify score
          //                             ^ match score
          //                                 ^penalty score
    }
    int _Max(int a, int b, int c, char* dir);
    // edit distance main function
    int _Construct(const CStringEx& strText, const CStringEx& strPattern);
    
public:
    inline int EditDistance(const CStringEx& strPattern)
    {
        CStringEx strTemp = m_lpszBuffer;
        return _Construct(strTemp, strPattern);
    }
    inline int EditDistance(const CStringEx& strText, const CStringEx& strPattern) {
        return _Construct(strText, strPattern);
    }
    inline int GetSimilarity() { return m_nSimilarity; }
    inline int GetSimilarityInfo(int* pTextBegin, int* pTextEnd, 
        int* pPatternBegin, int* pPatternEnd) {
        *pTextBegin    = m_nTextStart;
        *pTextEnd      = m_nTextEnd;
        *pPatternBegin = m_nPatternStart;
        *pPatternEnd   = m_nPatternEnd;
        return m_nSimilarity;
    }



    //------------------------------------------------------------
    // 2D, 3D approximate pattern matching(yet supported)
    //------------------------------------------------------------
    // yet supported
    // i will support this on future 3.0 version

};//class CStringEx

inline CStringEx operator+(CStringEx& string1, CStringEx& string2)
{
    return StringConcat( string1.GetBuffer(), string2.GetBuffer() );
}
inline CStringEx operator+(CStringEx& string1, const CHAR* lpsz)
{
    return StringConcat(string1.GetBuffer(), lpsz);
}
inline CStringEx operator+(CStringEx& string1, CHAR ch)
{
    char lpsz[2] = { ch, 0 };
    return StringConcat(string1.GetBuffer(), lpsz);
}
inline CStringEx operator+(const CHAR* lpsz, CStringEx& string)
{
    return StringConcat(lpsz, string.GetBuffer());
}
inline CStringEx operator+(CHAR ch, CStringEx& string)
{
    char lpsz[2] = { ch, 0 };
    return StringConcat(lpsz, string.GetBuffer());
}


inline int operator==(CStringEx& string1, CStringEx& string2)
{
    return string1.Compare( string2 ) == 0;
}
inline int operator==(const CHAR* lpsz, CStringEx& string2)
{
    return string2.Compare( lpsz ) == 0;
}
inline int operator==(CStringEx& string1, const CHAR* lpsz)
{
    return string1.Compare( lpsz ) == 0;
}
inline int operator!=(CStringEx& string1, CStringEx& string2)
{
    return string1.Compare( string2 ) != 0;
}
inline int operator!=(const CHAR* lpsz, CStringEx& string2)
{
    return string2.Compare( lpsz ) != 0;
}
inline int operator!=(CStringEx& string1, const CHAR* lpsz)
{
    return string1.Compare( lpsz ) != 0;
}

inline int operator>(CStringEx& string1, CStringEx& string2)
{
    return string1.Compare( string2 ) > 0;
}
inline int operator>(const CHAR* lpsz, CStringEx& string2)
{
    return string2.Compare( lpsz ) < 0;
}
inline int operator>(CStringEx& string1, const CHAR* lpsz)
{
    return string1.Compare( lpsz ) > 0;
}
inline int operator<(CStringEx& string1, CStringEx& string2)
{
    return string1.Compare( string2 ) < 0;
}
inline int operator<(const CHAR* lpsz, CStringEx& string2)
{
    return string2.Compare( lpsz ) > 0;
}
inline int operator<(CStringEx& string1, const CHAR* lpsz)
{
    return string1.Compare( lpsz ) < 0;
}

#pragma warning(default: 4995)
