#include "StdAfx.h"
#include ".\kprotectreftable.h"
DWORD ProtectTable::ProtectTableHelper::g_dwProtectTableChkSum = 0;
bool ProtectTable::ProtectTableHelper::g_bHackSafe = true;
std::wstring ProtectTable::ProtectTableHelper::g_strHackComment = L"";
#ifdef _DEBUG
DWORD ProtectTable::ProtectDebugHelper::g_dwUpdateCall = 0;
DWORD ProtectTable::ProtectDebugHelper::g_dwVerifyCall = 0;
#endif

//#include <limits.h>
//#include <vector>
//#include <algorithm>
//#include "Support/PrimeSearch/PrimeSearch.h"
//#include "ProtectedType.h"
//#include <boost/bind.hpp>
//
//#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
//
//KGCProtectRefTable::KGCProtectRefTable(void) : m_aSeedTable(NULL)
//{
//    ::srand( static_cast<unsigned int>( ::time( NULL ) ) );
//    // Must be Call!!
//    GenerateTable();
//
//#ifdef _DEBUG
//    m_dwCall = 0;
//#endif
//}
//
//KGCProtectRefTable::~KGCProtectRefTable(void)
//{
//    SAFE_DELETE_ARRAY( m_aSeedTable );
//}
//
//// 항상 srand( time(NULL) ) 부분 뒤에 위치해야 함.
//void KGCProtectRefTable::GenerateTable()
//{
//    do
//    {
//        m_ucTableSize = static_cast<UCHAR>( (::rand() % UCHAR_MAX) );
//    } while( m_ucTableSize < SCHAR_MAX );
//
//    m_aSeedTable = new UCHAR[ m_ucTableSize ];
//
//    PrimeSearch search( m_ucTableSize );
//    for( int i = 0; i < m_ucTableSize; ++i )
//    {
//        m_aSeedTable[i] = search.GetNext() + 1;
//    }
//}
