#pragma once

#include <sstream>
#include <iostream>
// std::ostream을 입력으로 받아 자신의 내용을 string으로 dump하고 리턴하는함수 ToString을 정의, 구현.
#define DeclToStringW               public : virtual std::wostream& ToString( std::wostream& stm ) const
#define ImplToStringW( classname )  std::wostream& classname##::ToString( std::wostream& stm_ ) const
#define DeclToStringA               public : virtual std::ostream& ToString( std::ostream& stm ) const
#define ImplToStringA( classname )  std::ostream& classname##::ToString( std::ostream& stm_ ) const

#define DeclDump    void Dump() { ToString( std::wcout ); }

// 멤버함수 ToString을 이용해 std::ostream의 operator<<() 을 정의, 구현.
#define DeclOstmOperatorW( classname )      std::wostream& operator<<( std::wostream& stm, const classname& kObj )
#define ImplOstmOperatorW( classname )      std::wostream& operator<<( std::wostream& stm_, const classname& kObj_ ) \
                                            { return kObj_.ToString( stm_ ); }
#define ImplOstmOperatorW2A( classname )    std::ostream& operator<<( std::ostream& stm_, const classname& kObj_ ) \
                                            { std::wostringstream stm; kObj_.ToString( stm ); return stm_ << KncUtil::toNarrowString( stm.str() ); }
#define DeclOstmOperatorA( classname )      std::ostream& operator<<( std::ostream& stm, const classname& kObj )
#define ImplOstmOperatorA( classname )      std::ostream& operator<<( std::ostream& stm_, const classname& kObj_ ) \
                                            { return kObj_.ToString( stm_ ); }
#define ImplOstmOperatorA2W( classname )    std::wostream& operator<<( std::wostream& stm_, const classname& kObj_ ) \
                                            { std::ostringstream stm; kObj_.ToString( stm ); return stm_ << KncUtil::toWideString( stm.str() ); }

// macro for 귀차니즘;
#define START_TOSTRINGW                     stm_ << L"-> " << m_RTTI.GetName() << std::endl
#define START_TOSTRINGA                     stm_ << "-> " << m_RTTI.GetName() << std::endl
#define START_TOSTRING2W( classname )       stm_ << L"-> " << L#classname << std::endl
#define START_TOSTRING_PARENTW( parent )    parent##::ToString( stm_ ) << L"-> " << m_RTTI.GetName() << std::endl    
#define TOSTRINGA( data )                   "    " #data " : " << data << std::endl
#define TOSTRINGW( data )                   L"    " L#data L" : " << data << std::endl
#define TOSTRINGWb( data )                  L"    " L#data L" : " << std::boolalpha << data << std::noboolalpha << std::endl
#define TOSTRINGWt( data )                  L"    " L#data L" : " << (LPCWSTR)data.Format( KNC_TIME_FORMAT ) << std::endl
#define TOSTRINGWc( data )                  L"    " L#data L" : " << (int)data << std::endl
#define TOSTRINGA2W( data )                 L"    " L#data L" : " << KncUtil::toWideString( data ) << std::endl

