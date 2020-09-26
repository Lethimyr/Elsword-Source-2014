//----------------------------------------------------------------
// Name: StringEx.cpp
// Desc: more advanced string manupulation class named 'CStringEx'
//       which supports                       
//             1) string edit distance using 'EditDistance()'
//             2) exact KMP string matching using 'Find()'
//             3) 2D approximate pattern matching 'EditDistance2D()'
//             4) 3D approximate pattern matching 'EditDistance2D()'
//             5) advanced parsing functions for example 'GetCFGToken()'
//       (c) 2000 KOGSoft by seojt@kogsoft.com
//----------------------------------------------------------------



#include "stdafx.h"

#include "StringEx.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#pragma warning(disable: 4995)


CStringEx StringConcat(const CHAR* lpsz1, const CHAR* lpsz2)
{
    if (lpsz1 == NULL && lpsz2 == NULL)
        return CStringEx();
    if (lpsz1 == NULL && lpsz2 != NULL)
        return CStringEx(lpsz2);
    if (lpsz1 != NULL && lpsz2 == NULL)
        return CStringEx(lpsz1);

    
    CHAR* lpszNew;
    int   len1 = (int)strlen( lpsz1 );
    int   len2 = (int)strlen( lpsz2 );

    lpszNew = new CHAR[ len1 + len2 + 1 ];
    // concatenate this and string to 'lpszNew'
    strcpy( lpszNew, lpsz1 );
    strcpy( lpszNew + len1, lpsz2 );

    // construct temporary object
    CStringEx stringRet( lpszNew );
    delete[] lpszNew;

    return stringRet;
      // Note: there is copying problem
}//StringConcat()


//----------------------------------------------------------------
// private members

int CStringEx::_SafeAlloc(int nLength)
{
    _SafeDelete();
    if (nLength <= 0)
        return 0;
    m_nLength    = nLength;
    m_lpszBuffer = new CHAR[ m_nLength + 1];
      //                               ^ + 2 for WCHAR or UNICODE string
    return 1;
}//CStringEx::_SafeAlloc()

void CStringEx::_SafeDelete()
{
    if ( m_lpszBuffer )
    {
        delete[] m_lpszBuffer;
        m_lpszBuffer = NULL; // you must pointer to 0, when deleted for safety
        m_nLength    = 0;    // it's good habit!
    }//if
    if ( m_pFail )
    {
        delete[] m_pFail;
        m_pFail = NULL;
    }//if
}//CStringEx::_SafeDelete()


/* inline void CStringEx::_SetString(const CHAR* lpsz)
{
    strcpy( m_lpszBuffer, lpsz );
}//CStringEx::_SetString() */

/* inline int CStringEx::_StringLength(const CHAR* lpsz) const
{
    if (lpsz == NULL) 
        return 0;

    return strlen( lpsz );
}//CStringEx::_StringLength() */

/* inline int CStringEx::_StringCompare(const CHAR* lpsz1, const CHAR* lpsz2) const
{
    if (lpsz1 == NULL && lpsz2 == NULL)
        return 0;
    if (lpsz1 == NULL && lpsz2 != NULL)
        return -1;
    if (lpsz1 != NULL && lpsz2 == NULL)
        return 1;
    return strcmp( lpsz1, lpsz2 );
}//CStringEx::_StringCompare() */

//----------------------------------------------------------------
// public members

//default constructor
CStringEx::CStringEx() 
{
    m_lpszBuffer = NULL;
    m_nLength    = 0;

    // members for FindFirst(), FindNext()
    m_pFail      = NULL;
}//CStringEx::CStringEx()

//copy constructor
CStringEx::CStringEx(const CStringEx& string)
{
    m_lpszBuffer = NULL;
    m_nLength    = 0;
    m_pFail      = NULL;
    if ( _SafeAlloc( string.GetLength() ) )
        _StringCopy( m_lpszBuffer, string.GetBuffer() );
}//CStringEx::CStringEx()

CStringEx::CStringEx(const CHAR* lpsz)
{
    m_lpszBuffer = NULL;
    m_nLength    = 0;
    m_pFail      = NULL;
    if ( _SafeAlloc( _StringLength(lpsz) ) )
        _StringCopy( m_lpszBuffer, lpsz );
}//CStringEx::CStringEx()

CStringEx::CStringEx(const CHAR ch)
{
    m_lpszBuffer = NULL;
    m_nLength    = 0;
    m_pFail      = NULL;
    _SafeAlloc( 1 );
    m_lpszBuffer[0] = ch;
    m_lpszBuffer[1] = 0;
}//CStringEx::CStringEx()

CStringEx::CStringEx(long n)
{
    char buffer[20];

    _ltoa(n,buffer,10);
    m_lpszBuffer = NULL;
    m_nLength    = 0;
    m_pFail      = NULL;
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
}//CStringEx::CStringEx()

CStringEx::CStringEx(int n)
{
    char buffer[20];

    _itoa(n,buffer,10);
    m_lpszBuffer = NULL;
    m_nLength    = 0;
    m_pFail      = NULL;
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
}//CStringEx::CStringEx()

CStringEx::CStringEx(double n)
{
    char buffer[20];

    sprintf(buffer, "%g", n);
    m_lpszBuffer = NULL;
    m_nLength    = 0;
    m_pFail      = NULL;
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
}//CStringEx::CStringEx()

CStringEx::CStringEx(float n)
{
    char buffer[20];

    sprintf(buffer, "%g", n);
    m_lpszBuffer = NULL;
    m_nLength    = 0;
    m_pFail      = NULL;
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
}//CStringEx::CStringEx()

CStringEx::~CStringEx()
{
    _SafeDelete();
    if (m_pFail) {
        delete[] m_pFail;
        m_pFail = NULL;
    }//if
}////CStringEx::~CStringEx()

/* int CStringEx::GetLength() const
{
    return m_nLength;
}//CStringEx::GetLength() */

/* int   CStringEx::IsEmpty() const
{
    return m_nLength <= 0;
}//CStringEx::IsEmpty() */

/* CHAR  CStringEx::GetAt(int nIndex) const
{
    return m_lpszBuffer[ nIndex ];
}//CStringEx::GetAt() */

/* CHAR& CStringEx::operator[](int nIndex)
{
    return m_lpszBuffer[ nIndex ];
}//CStringEx::operator[]() */

/* void  CStringEx::SetAt(int nIndex, CHAR ch)
{
    m_lpszBuffer[ nIndex ] = ch;
}//CStringEx::SetAt() */

//----------------------------------------------------------------
// get buffer pointer of this string
/* CStringEx::operator CHAR*() const
{
    return m_lpszBuffer;
}//CStringEx::operator CHAR*() */

//----------------------------------------------------------------
// get integer value of this string
/* CStringEx::operator int() const
{
    return atoi(m_lpszBuffer);
}//CStringEx::operator CHAR*() */

//----------------------------------------------------------------
// get long value of this string
/* CStringEx::operator long() const
{
    return atol(m_lpszBuffer);
}//CStringEx::operator CHAR*() */

//----------------------------------------------------------------
// get float value of this string
/* CStringEx::operator float() const
{
    return (float)atof(m_lpszBuffer);
}//CStringEx::operator CHAR*() */

//----------------------------------------------------------------
// get double value of this string
/* CStringEx::operator double() const
{
    return atof(m_lpszBuffer);
}//CStringEx::operator CHAR*() */

/* CStringEx& CStringEx::operator=(const CStringEx& stringSrc)
{
    if ( _SafeAlloc( stringSrc.GetLength() ) )
        _StringCopy( m_lpszBuffer, stringSrc.GetBuffer() );
    return *this;
}//CStringEx::operator=() */

/* CStringEx& CStringEx::operator=(const CHAR* lpsz)
{
    if ( _SafeAlloc( _StringLength(lpsz) ) )
        _StringCopy( m_lpszBuffer, lpsz );
    return *this;
}//CStringEx::operator=() */

/* CStringEx& CStringEx::operator=(const CHAR ch)
{
    _SafeAlloc( 1 );
    m_lpszBuffer[0] = ch;
    m_lpszBuffer[1] = 0;
    return *this;
}//CStringEx::operator=() */

/* CStringEx& CStringEx::operator=(const int    n)
{
    _SafeDelete();

    char buffer[20];

    _itoa(n,buffer,10);
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
    return *this;
}//CStringEx::operator=(const int    n) */

/* CStringEx& CStringEx::operator=(const long   n)
{
    _SafeDelete();

    char buffer[20];

    _ltoa(n,buffer,10);
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
    return *this;
}//CStringEx::operator=(const long   n) */

/* CStringEx& CStringEx::operator=(const float  f)
{
    _SafeDelete();

    char buffer[20];

    sprintf(buffer, "%g", f);
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
    return *this;
}//CStringEx::operator=(const float  f) */

/* CStringEx& CStringEx::operator=(const double f)
{
    _SafeDelete();

    char buffer[20];

    sprintf(buffer, "%g", f);
    if ( _SafeAlloc( _StringLength(buffer) ) )
        _StringCopy( m_lpszBuffer, buffer );
    return *this;
}//CStringEx::operator=(const double f) */

/* CStringEx& CStringEx::operator+=(const CStringEx& string)
{
    return operator+=( string.GetBuffer() );
}//CStringEx::operator+=() */

/* CStringEx& CStringEx::operator+=(const CHAR* lpsz)
{
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
}//CStringEx::operator+=() */

/* CStringEx& CStringEx::operator+=(const CHAR ch)
{
    char buffer[2]= { ch, 0 };
    return operator+=( buffer );
}//CStringEx::operator+=() */

/* int CStringEx::Compare(const CStringEx& string) const
{
    return _StringCompare( m_lpszBuffer, string.GetBuffer() );
}//CStringEx::Compare() */

/* int CStringEx::Compare(const CHAR* lpsz) const
{
    return _StringCompare( m_lpszBuffer, lpsz );
}//CStringEx::Compare() */

// when nCount is 0, return suffix of this string
CStringEx CStringEx::Mid(int nFirst, int nCount) const
{
    CHAR* lpszBuffer = new CHAR[ m_nLength + 1];

    if (nCount>=0)
    {
        memcpy(lpszBuffer, m_lpszBuffer+nFirst, nCount);
        lpszBuffer[nCount] = 0; // stuff EOS
    }
    else
    {
        strcpy( lpszBuffer, m_lpszBuffer+nFirst );
    }//if.. else..
    
    CStringEx stringRet( lpszBuffer );

    delete[] lpszBuffer;
    
    return stringRet;
}//CStringEx::Mid()

/* CStringEx CStringEx::Left(int nCount) const
{
    return Mid(0, nCount);
}//CStringEx::Left() */

/* CStringEx CStringEx::Right(int nCount) const
{
    return Mid(m_nLength-nCount, nCount);
}//CStringEx::Right() */

/* void CStringEx::MakeUpper()
{
    if (m_nLength>0)
        _strupr( m_lpszBuffer );
}//CStringEx::MakeUpper() */

/* void CStringEx::MakeLower()
{
    if (m_nLength>0)
        _strlwr( m_lpszBuffer );
}//CStringEx::MakeLower() */

void CStringEx::MakeReverse()
{
    int i = 0,
        j = m_nLength - 1;
    CHAR ch;

    while (i < j)
    {
        ch              = m_lpszBuffer[i];
        m_lpszBuffer[i] = m_lpszBuffer[j];
        m_lpszBuffer[j] = ch;
        ++i;
        --j;
    }//while
}//CStringEx::MakeReverse()

//----------------------------------------------------------------
// this function internally uses vsprintf()
// wow! it's simple and you will learn technique
//  passing variable arguments
void CStringEx::FormatVS(const CHAR* lpszFormat, ... )
{
    va_list args;
    char    buffer[255]={0,};
    
    va_start(args, lpszFormat);
    vsprintf(buffer, lpszFormat, args);
      // you can't use sprintf() why?
    va_end(args);

    if ( _SafeAlloc( _StringLength(buffer) ) )
    {
        _StringCopy( m_lpszBuffer, buffer );
    }//if
}//CStringEx::FormatVS(const CHAR* lpszFormat, ... )

//----------------------------------------------------------------
// this function only support escape sequence like
//   %d,  %ld,  %f,    %g,     %c,       %s
// each means
//   int, long, float, double, char, and string
// and support
//   %b,     %x,  %o,   %i
// each means
//   binary, hex, octa, integer
// Note: escape sequence like %2.3f isn't supported
//       you can do it. code it yourself!
void CStringEx::Format(const CHAR* lpszFormat, ... )
{
    _SafeDelete();

    int       i   = 0;
    int       len = _StringLength( lpszFormat );
    int       ch;
    va_list   ap;
    // temporaries for Escape sequence
    int       d;  
    long      ld;
    //float     f;
    double    g;
    char*     s;
    char      c;
    // temporary string buffer for parsing escape sequence
    char      lpsz[33]; // support 32 length
    int       lenlpsz;
    // variables for string construction
    CStringEx str;
    int       indexStr     = 0;
    int       bEscSequence = 0;

    va_start(ap, lpszFormat);

    while ( i < len )
    {
        ch = lpszFormat[i];
        if ( ch == '%' )
        {
            bEscSequence = 0;
            ++i;
            ch = lpszFormat[i];
            switch ( ch ) {
            case 'B':
            case 'b': // binary, int
                d = va_arg(ap, int);
                _itoa(d, lpsz, 2);
                bEscSequence = 1;
                break;
            case 'X':
            case 'x': // hexa decimal, int
                d = va_arg(ap, int);
                _itoa(d, lpsz, 16);
                bEscSequence = 1;
                break;
            case 'O':
            case 'o': // octa decimal, int
                d = va_arg(ap, int);
                _itoa(d, lpsz, 8);
                bEscSequence = 1;
                break;
            case 'I':
            case 'i':
            case 'D':
            case 'd': // decimal, int
                d = va_arg(ap, int);
                _itoa(d, lpsz, 10);
                bEscSequence = 1;
                break;
            case 'l': // long
                ++i;
                ch = lpszFormat[i];
                if (ch == 'd')
                {
                    ld = va_arg(ap, long);
                    _ltoa(ld, lpsz, 10);
                    bEscSequence = 1;
                }
                else
                {
                    --i;
                    ch = lpszFormat[i];
                    lpsz[0] = (char)ch;
                    lpsz[1] = 0;
                    str.Insert( indexStr, lpsz );
                    indexStr += 1;
                }//if
                break;
            case 'F':
            case 'f': // float
                g = va_arg(ap, double);
                sprintf( lpsz, "%g", g );
                bEscSequence = 1;
                break;
            case 'G':
            case 'g': // double why 'g'?
                g = va_arg(ap, double);
                sprintf( lpsz, "%g", g );
                bEscSequence = 1;
                break;
            case 'C':
            case 'c': // character
                c = va_arg(ap, char);
                lpsz[0] = c;
                lpsz[1] = 0;
                bEscSequence = 1;
                break;
            case 'S':
            case 's': // string, i.e. char*
                s = va_arg(ap, char*);
                lenlpsz = _StringLength( s );
                str.Insert( indexStr, s );
                indexStr += lenlpsz;
                break;
            default: // '%'
                str.Insert( indexStr, (char)ch );
                indexStr += 1;
                break;
            }//switch
            if ( bEscSequence )
            {
                lenlpsz = _StringLength( lpsz );
                str.Insert( indexStr, lpsz );
                indexStr += lenlpsz;
            }//if
        }
        else
        {
            str.Insert( indexStr, (char)ch );
            indexStr += 1;
        }//if.. else..
        ++i;
    }//for

    va_end(ap);
    if ( _SafeAlloc( str.GetLength() ) )
    {
        _StringCopy( m_lpszBuffer, str.GetBuffer() );
    }//if
}//CStringEx::Format()

void CStringEx::TrimLeft(CHAR ch)
{
    if (m_nLength <= 0) return;

    int i = 0;

    while (m_lpszBuffer[i] == ch)
    {
        ++i;
    }//while
    int lenNew = m_nLength - i;
    memmove(m_lpszBuffer, m_lpszBuffer + i, lenNew+1);
    m_nLength = lenNew;
}//CStringEx::TrimLeft()

void CStringEx::TrimRight(CHAR ch)
{
    if (m_nLength <= 0) return;

    int i = m_nLength - 1;
    int count = 0;

    while (m_lpszBuffer[i] == ch)
    {
        --i;
        ++count;
    }//while
    m_lpszBuffer[i+1] = 0;
    m_nLength = m_nLength - count;
}//CStringEx::TrimRight()

/* void CStringEx::Trim(CHAR ch)
{
    TrimLeft(ch);
    TrimRight(ch);
}//CStringEx::Trim() */

//----------------------------------------------------------------
// Name: _KMPSetup()
// Desc: Knuth,Morris,Pratt string matching algorithm
//       Setup KMP failure link
void CStringEx::_KMPSetup(const CHAR* p, int* fail)
{
    int k, r;
    int plen = _StringLength( p );

    fail[0] = -1; // -1 means read next character
    for (k = 1; k < plen; ++k)
    {
        r = fail[k-1];
        while ( r > -1 && p[r] != p[k-1] )
        {
            r = fail[r]; // follow failure link
        }//while
        fail[k] = r+1;
    }//for
}//CStringEx::_KMPSetup()

//----------------------------------------------------------------
// p : pattern
// t : text
// fail : failure link
// Order : O(len(p)+len(t))
// return -1 when no match
int CStringEx::_KMPMatch(const CHAR* p, const CHAR* t, 
                         const int* fail, int nStart)
{
    if (nStart < 0) return -1;

    int j = nStart, //j indexes text charchters
        k = 0,      //k indexes the pattern and fail array
        plen = _StringLength( p ),
        tlen = _StringLength( t );

    while (j < tlen && k < plen)
    {
        if ( k == -1 ) // -1 means read next character
        {
            ++j;
            ++k;
        }
        else if ( p[k] == t[j] )
        {
#if defined(_DEBUG) // 2003-12-18 ¿ÀÀü 9:40:20
            assert( k >= 0 );
            assert( j >= 0 );
#endif // defined(_DEBUG)
            ++j;
            ++k;
        // follow fail arrow
        }
        else
        {
            k = fail[k];
        }//if.. else if.. else..
    }//while

    // j-strlen(p), match found
    // j = -1, no match
    return (k >= plen) ? j - plen : -1;
}//CStringEx::_KMPMatch()

//----------------------------------------------------------------
// p : pattern
// t : text
// fail : failure link
// Order : O(len(p)+len(t))
// return occurence of 'p' in 't'
// Note: marks 'pMark' with 'nMark' when pMark isn't NULL
//       pMark must be properly allocated before call this function
//       first used for 'GetCFGToken()' for preprocessing
int CStringEx::_KMPMatchCount(const CHAR* p, const CHAR* t, 
                              const int* fail, int nStart,
                              int* pMark, 
                              int nMark)
{
    int j      = nStart, //j indexes text charchters
        k      = 0,      //k indexes the pattern and fail array
        plen   = _StringLength( p ),
        tlen   = _StringLength( t );
    int nCount = 0;

labelCount:
    while (j < tlen && k < plen)
    {
        if (k == -1 || p[k] == t[j])
        {
            //-1 means read next character
            ++j;
            ++k;
        } else //follow fail arrow
            k = fail[k];
    }//while
    if (k >= plen)
    {
        k = 0;
        ++nCount;
        if (pMark)
            pMark[ j-plen ] = nMark;
        goto labelCount;
    }//if
    return nCount;
}//CStringEx::_KMPMatchCount()

//----------------------------------------------------------------
// find lpszSub in this string
// using KMP string matching algorithm
// return -1 when not found
int CStringEx::Find(const CHAR* lpszSub, int nStart)
{
    int* pFail;
    int  nRet;

    pFail = new int[ _StringLength(lpszSub) ];

    _KMPSetup(lpszSub, pFail);
    nRet = _KMPMatch(lpszSub, m_lpszBuffer, pFail, nStart);

    delete[] pFail;

    return nRet;
}//CStringEx::Find()

int CStringEx::FindCount(const CHAR* lpszSub, int nStart)
{
    int* pFail;
    int  nRetCount;

    pFail = new int[ _StringLength(lpszSub) ];

    _KMPSetup(lpszSub, pFail);
    nRetCount = _KMPMatchCount(lpszSub, m_lpszBuffer, pFail, nStart);

    delete[] pFail;

    return nRetCount;
}//CStringEx::FindCount(const CHAR* lpszSub, int nStart = 0)

//----------------------------------------------------------------
// find position of 'lpszSub'
// this functions uses KMP string matching algorithm
// return -1 when not found
int CStringEx::FindFirst(const CHAR* lpszSub)
{
    int  nRet;

    if (m_pFail)
    {
        delete[] m_pFail;
        m_pFail = NULL;
    }//if

    m_pFail      = new int[ _StringLength(lpszSub) ];
    m_nNextIndex = 0;

    _KMPSetup(lpszSub, m_pFail);
    nRet = _KMPMatch(lpszSub, m_lpszBuffer, m_pFail, m_nNextIndex);
    m_nNextIndex = nRet + _StringLength( lpszSub );

    return nRet;
}//CStringEx::FindFirst(const CHAR* lpszSub)

//----------------------------------------------------------------
// Note: 'lpszSub' must be same to FindFirst()'s one
//       because this function used same failure link
//       which is constructed on 'FindFirst()'
// return -1 when not found
int CStringEx::FindNext(const CHAR* lpszSub)
{
    int i;

    i = _KMPMatch(lpszSub, m_lpszBuffer, m_pFail, m_nNextIndex);
    m_nNextIndex = i + _StringLength( lpszSub );
    if (i == -1) // for ultimate safety
    {
        delete[] m_pFail;
        m_pFail = NULL;
    }//
    return i;
}//CStringEx::FindNext()

int CStringEx::ReverseFind(const CHAR* lpszSub, int nRevStart)
{
    // disable warning
    lpszSub;
    nRevStart;
    //............................................................
    // how about implementing this function by yourself
    // it's easy. just use Find(), and Reverse()
    assert( 1 == 0 );
    return 0;
}//CStringEx::ReverseFind()

/* CHAR* CStringEx::GetBuffer() const
{
    return m_lpszBuffer;
}//CStringEx::GetBuffer() */

void CStringEx::ReleaseBuffer(int nNewLength)
{
    _SafeDelete();
    if (nNewLength > 0)
        _SafeAlloc(nNewLength);
}//CStringEx::ReleaseBuffer()


//________________________________________________________________
// parsing functions
//----------------------------------------------------------------

//----------------------------------------------------------------
// return index of ch on 'lpszCharSet'
// return -1 when there is no 'ch'
int CStringEx::_IsIncluded(const CHAR* lpszCharSet, CHAR ch) const
{
    int i;
    int len = _StringLength( lpszCharSet );

    for (i=0; i<len; ++i)
    {
        if (ch == lpszCharSet[i])
            return i;
    }//for
    return -1;
}//CStringEx::_IsIncluded(CHAR* lpszCharSet, CHAR ch)

void CStringEx::_MoveString(CHAR* lpsz, int nDest, int nSrc)
{
#ifdef _DEBUG
    assert( nDest < nSrc );
#endif
    int len = _StringLength( lpsz );
    memmove(lpsz + nDest, lpsz + nSrc, len - nSrc + 1 );
}//CStringEx::_MoveString(int nDest, int nSrc)

//----------------------------------------------------------------
// return index of ch on string
// return -1 when there is no 'ch'
/* int CStringEx::IsIncluded(CHAR ch) const
{
    return _IsIncluded( m_lpszBuffer, ch );
}//CStringEx::IsIncluded(CHAR ch) */

//----------------------------------------------------------------
// return a substring that contains characters in the string 
// that are in 'lpszCharSet'
CStringEx CStringEx::SpanIncluding(const CHAR* lpszCharSet, 
                                   int nIndex) const
{
    int i;
    int state       = 0;
    int nStartIndex = nIndex;
    int nCount      = 0;
    
    for (i=nIndex; i<m_nLength; ++i)
    {
        switch ( state)
        {
        case 0:
            if ( -1 != _IsIncluded(lpszCharSet, m_lpszBuffer[i]) )
            {
                state = 1;
                nStartIndex = i;
                nCount = 1;
            }//if
            break;
        case 1:
            if ( -1 != _IsIncluded(lpszCharSet, m_lpszBuffer[i]) )
            {
                ++nCount;
            } else
                state = 2;
            break;
        }//switch
        if (state == 2) 
            break;
    }//for
    if (state == 1 || state == 2)
        return Mid(nStartIndex, nCount);
    else
        return CStringEx();
}//CStringEx::SpanIncluding(CHAR* lpszCharSet, int nIndex = 0) const

//----------------------------------------------------------------
// return a substring that does not contains characters in the string 
// that are in 'lpszCharSet'
CStringEx CStringEx::SpanExcluding(const CHAR* lpszCharSet, 
                                   int nIndex) const
{
    int i;
    int state       = 0;
    int nStartIndex = nIndex;
    int nCount      = 0;
    
    for (i=nIndex; i<m_nLength; ++i)
    {
        switch ( state)
        {
        case 0:
            if ( -1 == _IsIncluded(lpszCharSet, m_lpszBuffer[i]) )
            {
                state = 1;
                nStartIndex = i;
                nCount = 1;
            }//if
            break;
        case 1:
            if ( -1 == _IsIncluded(lpszCharSet, m_lpszBuffer[i]) )
            {
                ++nCount;
            } else
                state = 2;
            break;
        }//switch
        if (state == 2) 
            break;
    }//for
    if (state == 1 || state == 2)
        return Mid(nStartIndex, nCount);
    else
        return CStringEx();
}//CStringEx::SpanExcluding(CHAR* lpszCharSet, int nIndex = 0) const

//----------------------------------------------------------------
// fill character from nIndex to nIndex+nCount-1
int CStringEx::FillChar(int nIndex, int nCount, CHAR ch)
{
    int i;

    for (i=0; i<nCount; ++i)
    {
        if (i+nIndex >= m_nLength) break; // added...
        m_lpszBuffer[i+nIndex] = ch;
    }//for
    return nCount;
}//CStringEx::FillChar(int nIndex, int nCount, CHAR ch)

//----------------------------------------------------------------
// replace chOld with chNew in this string
int CStringEx::Replace(CHAR chOld, CHAR chNew)
{
    int i;
    int nCount = 0;
    for (i=0; i<m_nLength; ++i)
    {
        if (chOld == m_lpszBuffer[i])
        {
            ++nCount;
            m_lpszBuffer[i] = chNew;
        }//if
    }//for
    return nCount;
}//Replace(CHAR chOld, CHAR chNew)

//----------------------------------------------------------------
// replace 'lpszOld' with 'lpszNew' in this string
int CStringEx::Replace(const CHAR* lpszOld, const CHAR* lpszNew)
{
    int t;
    int lenOld = _StringLength( lpszOld );
    int nCount = 0;

    t = Find( lpszOld );
    while (t != -1)
    {
        ++nCount;
        Delete( t, lenOld );
        Insert( t, lpszNew );
        t = Find( lpszOld );
    }//if
    return nCount;
}//Replace(CHAR* lpszOld, CHAR* lpszNew)

//----------------------------------------------------------------
// remove ch in this string
int CStringEx::Remove(CHAR ch)
{
    int i;
    int nCount = 0;

    for (i=0; i<m_nLength; ++i)
    {
        if (ch == m_lpszBuffer[i])
        {
            ++nCount;
            _MoveString(m_lpszBuffer, i, i+1);
            --i; // Note: restart from this
        }//if
    }//for
    m_nLength -= nCount;
    return nCount;
}//Remove(CHAR ch)

//----------------------------------------------------------------
// remove 'lpsz' in this string
// Note: chSentinal must be out in string character set
// we assume character 127 isn't used in string
// if you use 127th character in this string
int CStringEx::Remove(const CHAR* lpsz, CHAR chSentinal)
{
    int len = _StringLength( lpsz );
    int nCount = FindCount( lpsz );
    int i, t;

    if (nCount == 0) return 0;

    t = FindFirst( lpsz );
    FillChar(t, len, chSentinal);
    for (i=1; i<nCount; ++i)
    {
        t = FindNext( lpsz );
        FillChar(t, len, chSentinal);
    }//for
    Remove( chSentinal );
    return nCount;
}//Remove(CHAR* lpsz)

//----------------------------------------------------------------
// insert ch in m_lpszBuffer[nIndex]
/* int CStringEx::Insert(int nIndex, CHAR ch)
{
    char str[2] = { ch, 0 };

    return Insert(nIndex, str);
}//Insert(int nIndex, CHAR ch) */

//----------------------------------------------------------------
// insert 'lpsz' in m_lpsz[ nIndex ]
// return modified string length
// return -1 when insertion failed
int CStringEx::Insert(int nIndex, const CHAR* lpsz)
{
    if (nIndex > m_nLength) return -1;

    CHAR* lpszNew;
    int   lenNew;
    int   len = _StringLength( lpsz );
    int   i;

    lenNew  = m_nLength + len;
    lpszNew = new CHAR[ lenNew + 1 ];
    _StringCopy( lpszNew, m_lpszBuffer );
    memcpy(lpszNew + (nIndex + len), lpszNew + nIndex, m_nLength - nIndex );
    lpszNew[ lenNew ] = 0; // stuff EOS mark
    for (i=0; i<len; ++i)
    {
        lpszNew[ i+nIndex ] = lpsz[i];
    }//for
    //_SafeDelete();
    delete[] m_lpszBuffer;

    m_lpszBuffer = lpszNew;
    m_nLength    = lenNew;
    
    return m_nLength;
}//Insert(int nIndex, CHAR* lpsz)

//----------------------------------------------------------------
// delete characters for nCount times
int CStringEx::Delete(int nIndex, int nCount)
{
    if (nCount <= 0) return 0;

    _MoveString( m_lpszBuffer, nIndex, nIndex+nCount );
    m_nLength -= nCount;
    return nCount;
}//Delete(int nIndex, int nCount = 1)

//----------------------------------------------------------------
// The zero-based index of the first character in this string 
// that is also in lpszCharSet; -1 if there is no match
int CStringEx::FindOneOf(const CHAR* lpszCharSet) const
{
    int  i, j;
    int  len = _StringLength( lpszCharSet );
    CHAR ch;

    for (i=0; i<m_nLength; ++i)
    {
        for (j=0; j<len; ++j)
        {
            ch = lpszCharSet[j];
            if ( m_lpszBuffer[i] == ch )
                return i;
        }//for
    }//for
    return -1;
}//FindOneOf(CHAR* lpszCharSet) const

//----------------------------------------------------------------
// advanced parsing functions
//----------------------------------------------------------------

//----------------------------------------------------------------
// parse this string from nIndex
//   token is delimited by 'lpszDelimiter'
// when 'bInclude' is 1
//   lpszDelimiter is included in token
CStringEx CStringEx::GetToken(const CHAR* lpszDelimiter, int nIndex, int bInclude)
{
    int len = _StringLength( lpszDelimiter );
    int nCount;
    int nDelimiter;

    nDelimiter = Find(lpszDelimiter,nIndex);
    if (nDelimiter == -1)
    {
        m_nTokenIndex = -1;
        return Mid(nIndex); // return suffix of this string
    }//if

    nCount = nDelimiter - nIndex;
    if ( bInclude ) nCount += len;
    m_nTokenIndex = nDelimiter + len;

    return Mid(nIndex, nCount);
}//CStringEx::GetToken(const CHAR* lpszDelimeter, int nIndex)


//----------------------------------------------------------------
// parse this string from nIndex
//   token is delimited by 'lpszDelimiter'
// when 'bInclude' is 1
//   lpszDelimiter is included in token
void CStringEx::GetToken(const CHAR* lpszDelimiter, 
                                     CStringEx& strOut, int nIndex , int bInclude)
{
    int len = _StringLength( lpszDelimiter );
    int nCount;
    int nDelimiter;

    nDelimiter = Find(lpszDelimiter,nIndex);
    if (nDelimiter == -1)
    {
        m_nTokenIndex = -1;

        strOut = Mid(nIndex);
        return;// Mid(nIndex); // return suffix of this string
    }//if

    nCount = nDelimiter - nIndex;
    if ( bInclude ) nCount += len;
    m_nTokenIndex = nDelimiter + len;

    strOut = Mid(nIndex, nCount);
    return;// Mid(nIndex, nCount);
}//CStringEx::GetToken(const CHAR* lpszDelimeter, int nIndex)


//----------------------------------------------------------------
// you must call GetToken() first time
//   and then call this function
// you may freely specify 'lpszDelimiter', 
// lpszDelimiter used in this functions is independedt with it in 'GetToken()'
CStringEx CStringEx::GetNextToken(const CHAR* lpszDelimiter, int bInclude)
{
    if (m_nTokenIndex == -1) return CStringEx();

    int len = _StringLength( lpszDelimiter );
    int nCount;
    int nStart;
    int nDelimiter;

    nStart = m_nTokenIndex;
    nDelimiter = Find(lpszDelimiter, m_nTokenIndex);
    if (nDelimiter == -1)
    {
        m_nTokenIndex = -1;
        if (bInclude) // there is no token which includes delimiter
            return CStringEx();
        else // return suffix of string
            return Mid( nStart );
    }//if

    nCount = nDelimiter - nStart;
    if ( bInclude ) nCount += len;
    m_nTokenIndex = nDelimiter + len;

    if (nCount == 0)
    {
        return CStringEx(lpszDelimiter);
    }//if

    return Mid(nStart, nCount);
}//CStringEx::GetNextToken(const CHAR* lpszDelimeter)


//----------------------------------------------------------------
// you must call GetToken() first time
//   and then call this function
// you may freely specify 'lpszDelimiter', 
// lpszDelimiter used in this functions is independedt with it in 'GetToken()'
void CStringEx::GetNextToken(const CHAR* lpszDelimiter, 
                             CStringEx& strOut,
                             int bInclude)
{
    if (m_nTokenIndex == -1) 
    {
        strOut = CStringEx();
        return;
    }//if

    int len = _StringLength( lpszDelimiter );
    int nCount;
    int nStart;
    int nDelimiter;

    nStart = m_nTokenIndex;
    nDelimiter = Find(lpszDelimiter, m_nTokenIndex);
    if (nDelimiter == -1)
    {
        m_nTokenIndex = -1;
        if (bInclude) // there is no token which includes delimiter
        {
            strOut = CStringEx();
            return;// CStringEx();
        }
        else // return suffix of string
        {
            strOut = Mid( nStart );
            return;// Mid( nStart );
        }//if.. else..
    }//if

    nCount = nDelimiter - nStart;
    if ( bInclude ) nCount += len;
    m_nTokenIndex = nDelimiter + len;

    if (nCount == 0) 
    {
        strOut = CStringEx(lpszDelimiter);
        return;// CStringEx(lpszDelimiter);
    }//if

    strOut = Mid(nStart, nCount);
    return;// Mid(nStart, nCount);
}//CStringEx::GetNextToken(const CHAR* lpszDelimeter)


//----------------------------------------------------------------
// parse this string from nStart, first delimiter is lpszStart
//   second delimiter is lpszEnd
// when bBalancing is 1
//   lpszStart and lpszEnd are balanced
//   if balancing fails, return (null)
// nStart: starting index of this string
CStringEx CStringEx::GetCFGToken(const CHAR* lpszStart, 
                                 const CHAR* lpszEnd,
                                 int* ipStartIndex,
                                 int* ipEndIndex,
                                 int nStart, 
                                 int bBalancing)
{
    int     nCount1, nCount2;
    int     lenStart = _StringLength(lpszStart),
            lenEnd   = _StringLength(lpszEnd);
    int*    pMarker;
    int*    pFail;

    // 1) allocate marker memory
    pMarker = new int[ m_nLength ];
    memset(pMarker, 0, sizeof(int)*m_nLength );
    
    // 2) mark start delimiter with 1
    pFail   = new int[ lenStart ];
    _KMPSetup(lpszStart, pFail);
    nCount1 = _KMPMatchCount(lpszStart, m_lpszBuffer, pFail, nStart, 
                            pMarker, 1);
    delete[] pFail;

    // 3) mark end delimiter with 2
    pFail   = new int[ lenEnd ];
    _KMPSetup(lpszEnd, pFail);
    nCount2 = _KMPMatchCount(lpszEnd, m_lpszBuffer, pFail, nStart,
                            pMarker, 2);
    delete[] pFail;

//#define _SEOJT_DEBUG 1
#if defined( _SEOJT_DEBUG )
    {
    for (int i=0; i<m_nLength; ++i)
        printf("%c", pMarker[i] );
    printf("\n");
    }
#endif // defined( _SEOJT_DEBUG )

    // 4) parse 'pMarker' using automata
    //    how powerful it is!
    int i       = 0;
    int state   = 0;
    int marker;
    int index1  = nStart;
    int index2  = nStart;

    if (nCount1 <= 0 || nCount2 <= 0) 
        goto GCT_ERROR;

    /*
    nCount1 = nCount2 = 0;
    for (i=nStart; i<m_nLength; ++i) {
        marker = pMarker[i];
        switch ( state ) {
        case 0:
            if (marker == 1) {
                nCount1 = 1;
                state   = 1;
                index1  = i;
            }//if
            break;
        case 1:
            if (marker == 1)
                ++nCount1;
            else if (marker == 2) {
                nCount2 = 1;
                if (nCount1 == nCount2 || bBalancing == 0) {
                    state = 3; // set accepting state
                    index2 = i;
                } else
                    state = 2;
            }//if.. else if..
            break;
        case 2:
            if (marker == 1)
                goto GCT_ERROR;
            else if (marker == 2) {
                ++nCount2;
                if (nCount1 == nCount2) {
                    state = 3; // set accepting state
                    index2 = i;
                }//if
            }//if.. else..
            break;
        }//switch
        if (state == 3) { // accepting state
            delete[] pMarker;
            return Mid(index1+lenStart, index2-index1-lenStart);
        }//if
    }//for
    */

    nCount1 = nCount2 = 0;
    for (i=nStart; i<m_nLength; ++i)
    {
        marker = pMarker[i];
        switch ( state )
        {
        case 0:
            if (marker == 1)
            {
                nCount1 = 1;
                state   = 1;
                index1  = i;
            }//if
            break;
        case 1:
            if (marker == 1)
                ++nCount1;
            else if (marker == 2)
            {
                --nCount1;
                if (nCount1 == 0 || bBalancing == 0)
                {
                //          ^ when balancing 
                //                             ^ when no balancing
                    state  = 2; // set accepting state
                    index2 = i;
                }//if
            }//if.. else if..
            break;
        }//switch

        if (state == 2) // accepting state
        {
            delete[] pMarker;

            if (ipStartIndex != NULL)
                *ipStartIndex = index1;
            if (ipEndIndex != NULL)
                *ipEndIndex = index2;

            return Mid(index1+lenStart, index2-index1-lenStart);
        }//if
    }//for

GCT_ERROR:
    delete[] pMarker;

    if (ipStartIndex != NULL)
        *ipStartIndex = -1;
    if (ipEndIndex != NULL)
        *ipEndIndex = -1;
   
    return CStringEx();
}//CStringEx::GetCFGToken(const CHAR* lpszStart, const CHAR* lpszEnd, int bBalancing = 1)


//----------------------------------------------------------------
// parse this string from nStart, first delimiter is lpszStart
//   second delimiter is lpszEnd
// when bBalancing is 1
//   lpszStart and lpszEnd are balanced
//   if balancing fails, return (null)
// nStart: starting index of this string
void CStringEx::GetCFGToken(const CHAR* lpszStart, 
                            const CHAR* lpszEnd, 
                            CStringEx& strOut,
                            int* ipStartIndex,
                            int* ipEndIndex,
                            int nStart, 
                            int bBalancing)
{
    int     nCount1, nCount2;
    int     lenStart = _StringLength(lpszStart),
            lenEnd   = _StringLength(lpszEnd);
    int*    pMarker;
    int*    pFail;

    // 1) allocate marker memory
    pMarker = new int[ m_nLength ];
    memset(pMarker, 0, sizeof(int)*m_nLength );
    
    // 2) mark start delimiter with 1
    pFail   = new int[ lenStart ];
    _KMPSetup(lpszStart, pFail);
    nCount1 = _KMPMatchCount(lpszStart, m_lpszBuffer, pFail, nStart, 
                            pMarker, 1);
    delete[] pFail;

    // 3) mark end delimiter with 2
    pFail   = new int[ lenEnd ];
    _KMPSetup(lpszEnd, pFail);
    nCount2 = _KMPMatchCount(lpszEnd, m_lpszBuffer, pFail, nStart,
                            pMarker, 2);
    delete[] pFail;

//#define _SEOJT_DEBUG 1
#if defined( _SEOJT_DEBUG )
    {
    for (int i=0; i<m_nLength; ++i)
        printf("%c", pMarker[i] );
    printf("\n");
    }
#endif // defined( _SEOJT_DEBUG )

    // 4) parse 'pMarker' using automata
    //    how powerful it is!
    int i       = 0;
    int state   = 0;
    int marker;
    int index1  = nStart;
    int index2  = nStart;

    if (nCount1 <= 0 || nCount2 <= 0) 
        goto GCT_ERROR;

    nCount1 = nCount2 = 0;
    for (i=nStart; i<m_nLength; ++i)
    {
        marker = pMarker[i];
        switch ( state )
        {
        case 0:
            if (marker == 1)
            {
                nCount1 = 1;
                state   = 1;
                index1  = i;
            }//if
            break;
        case 1:
            if (marker == 1)
                ++nCount1;
            else if (marker == 2)
            {
                --nCount1;
                if (nCount1 == 0 || bBalancing == 0)
                {
                //          ^ when balancing 
                //                             ^ when no balancing
                    state  = 2; // set accepting state
                    index2 = i;
                }//if
            }//if.. else if..
            break;
        }//switch
        if (state == 2) // accepting state
        {
            delete[] pMarker;

            if (ipStartIndex != NULL)
                *ipStartIndex = index1;
            if (ipEndIndex != NULL)
                *ipEndIndex = index2;

            strOut = Mid(index1+lenStart, index2-index1-lenStart);

            return;// Mid(index1+lenStart, index2-index1-lenStart);
        }//if
    }//for

GCT_ERROR:
    delete[] pMarker;

    if (ipStartIndex != NULL)
        *ipStartIndex = -1;
    if (ipEndIndex != NULL)
        *ipEndIndex = -1;

    strOut = CStringEx();

    return;// CStringEx();
}//CStringEx::GetCFGToken(const CHAR* lpszStart, const CHAR* lpszEnd, int bBalancing = 1)


// return string which excludes the result of 'GetCFGToken()'
CStringEx CStringEx::GetCFGTokenExcluding(const CHAR* lpszStart, 
                                          const CHAR* lpszEnd, 
                                          int* ipStartIndex,
                                          int* ipEndIndex,
                                          int nStart,
                                          int bBalancing)
{
    CStringEx token;
    int       i;
    int       len;

    token = GetCFGToken(lpszStart, lpszEnd, ipStartIndex, ipEndIndex, 
                        nStart, bBalancing);
    token = lpszStart + token;
    token = token + lpszEnd;
    i     = Find(token.GetBuffer(), nStart);
    len   = token.GetLength();
    if (i != -1)
    {
        //printf("%s\n", (char*)token);
        token = *this;
        token.Delete(i, len);
        //printf("%s\n", (char*)token);
        return token;
    }//if

    return CStringEx();
}//CStringEx::GetCFGTokenExcluding(const CHAR* lpszStart, const CHAR* lpszEnd, int nStart = 0)


//------------------------------------------------------------
// string edit distance functions
//------------------------------------------------------------

// we use score value 2 and -1, each for match and mismatch
/* int CStringEx::_Weight(int i, int j, 
                       const CHAR* text, const CHAR* pattern) 
{
    return (pattern[i] == text[j]) ? 2 : -1;
      //                             ^ you can modify score
      //                             ^ match score
      //                                 ^penalty score
}//CStringEx::_Weight(int i, int j, const CHAR* text, const CHAR* pattern) */

int CStringEx::_Max(int a, int b, int c, char* dir) 
{
    int r;

    if (a>b)
    {
        if (a>c)
        {
            r    = a; 
            *dir = 1;
        }
        else
        {
            r    = c; 
            *dir = 3;
        }//if.. else..
    }
    else
    {
        if (b>c)
        {
            r    = b; 
            *dir = 2;
        }
        else
        {
            r    = c; 
            *dir = 3;
        }//if.. else..
    }//if.. else..

    return (r > 0) ? r : 0;
}//CStringEx::_Max()

int CStringEx::_Construct(const CStringEx& strText, const CStringEx& strPattern) 
{
    struct
    {
        char    sim;
        char    dir;
    } d[256][256];

    int     lenPattern;
    int     lenText;
    CHAR*   lpText;
    CHAR*   lpPattern;

    lenText      = strText.GetLength();
    lenPattern   = strPattern.GetLength();
    lpText       = (CHAR*)strText;
    lpPattern    = (CHAR*)strPattern;

    
    int i, j = 0;

    m_nSimilarity   = 0;
    m_nPatternStart = 0;
    m_nPatternEnd   = 0;
    m_nTextStart    = 0;
    m_nTextEnd      = 0;

    if (lenPattern == 0 || lenText == 0 ||
        lenPattern > 255 || lenText > 255)
    {
        return 0;
    }//if

    //.....................................
    // construct distance matrix
    // it's order is O(lenText*lenPattern)
    // can you improve this time complexity? try it!
    memset(d, 0, sizeof(d));
    for (i = 1; i <= lenPattern; ++i)
    {
        for (j = 1; j <= lenText; ++j)
        {
            d[i][j].sim = (char)_Max(d[i-1][j].sim - 1,
                               d[i-1][j-1].sim + _Weight(i-1, j-1, lpText, lpPattern),
                               d[i][j-1].sim-1, &d[i][j].dir);
                               //               ^ returned direction
            if (d[i][j].sim > m_nSimilarity)
            {
                m_nSimilarity = d[i][j].sim;
                m_nPatternEnd = i;
                m_nTextEnd    = j;
            }//if
        }//for
    }//for

    //.......................
    // Trace back
    m_nPatternStart = m_nPatternEnd;
    m_nTextStart    = m_nTextEnd;
    while ( d[m_nPatternStart][m_nTextStart].sim != 0 )
    {
        switch ( d[m_nPatternStart][m_nTextStart].dir )
        {
        case 1:
            --m_nPatternStart;
            i = 1;
            j = 0;
            break;
        case 2:
            --m_nPatternStart;
            --m_nTextStart;
            i = 1;
            j = 1;
            break;
        case 3:
            --m_nTextStart;
            i = 0;
            j = 1;
            break;
        }//switch
    }//while
    m_nPatternStart += i;
    m_nTextStart    += j;

    --m_nPatternStart;
    --m_nPatternEnd;
    --m_nTextStart;
    --m_nTextEnd;
    return 1;
}//CStringEx::_Construct(CStringEx& strText, CStringEx& strPattern)

/* int CStringEx::EditDistance(const CStringEx& strPattern) 
{
    CStringEx strTemp = m_lpszBuffer;
    return _Construct(strTemp, strPattern);
}//CStringEx::EditDistance() */

/* int CStringEx::EditDistance(const CStringEx& strText, const CStringEx& strPattern) 
{
    return _Construct(strText, strPattern);
}//CStringEx::EditDistance() */

/* int CStringEx::GetSimilarity()
{
    return m_nSimilarity;
}//CStringEx::GetSimilarity() */

/* int CStringEx::GetSimilarityInfo(int* pTextBegin, int* pTextEnd, 
                      int* pPatternBegin, int* pPatternEnd)
{
    *pTextBegin    = m_nTextStart;
    *pTextEnd      = m_nTextEnd;
    *pPatternBegin = m_nPatternStart;
    *pPatternEnd   = m_nPatternEnd;
    return m_nSimilarity;
}//CStringEx::GetSimilarityInfo(int* nTextBegin, int* nTextEnd,  */
