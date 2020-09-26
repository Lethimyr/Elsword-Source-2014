
#pragma once

#ifndef _KSTRINGUTIL_H
#define _KSTRINGUTIL_H



// dmlee 2008.05.14
// note!! boost 라이브러리 대신 임시로 코드 대체


//#include    "KJohnGen.h"
//#include    <boost/algorithm/string/predicate.hpp>
#include    <string>


namespace   KStringUtil
{
    inline bool    CaseInsensitiveLess( const std::string& strIn1_, const std::string& strIn2_ )
    {
		//return  boost::algorithm::ilexicographical_compare( strIn1_, strIn2_ );
		
		std::string strIn1 = strIn1_;
		std::string strIn2 = strIn2_;

		MakeUpperCase( strIn1 );
		MakeUpperCase( strIn2 );

		if( strcmp( strIn1.c_str(), strIn2.c_str() ) < 0 )
			return true;
		else 
			return false;
        
    }//CaseInsensitiveLess()

    inline bool    CaseInsensitiveLess( const std::wstring& wstrIn1_, const std::wstring& wstrIn2_ )
    {
        //return  boost::algorithm::ilexicographical_compare( wstrIn1_, wstrIn2_ );
		std::wstring strIn1 = wstrIn1_;
		std::wstring strIn2 = wstrIn2_;

		MakeUpperCase( strIn1 );
		MakeUpperCase( strIn2 );

		if( wcscmp( strIn1.c_str(), strIn2.c_str() ) < 0 )
			return true;
		else 
			return false;
    }//CaseInsensitiveLess()

    inline bool    CaseInsensitiveEqual( const std::string& strIn1_, const std::string& strIn2_ )
    {
        //return  boost::algorithm::iequals( strIn1_, strIn2_ );

		std::string strIn1 = strIn1_;
		std::string strIn2 = strIn2_;

		MakeUpperCase( strIn1 );
		MakeUpperCase( strIn2 );

		if( strcmp( strIn1.c_str(), strIn2.c_str() ) == 0 )
			return true;
		else 
			return false;


    }//CaseInsensitiveEqual()

    inline bool    CaseInsensitiveEqual( const std::wstring& wstrIn1_, const std::wstring& wstrIn2_ )
    {
        //return  boost::algorithm::iequals( wstrIn1_, wstrIn2_ );

		std::wstring strIn1 = wstrIn1_;
		std::wstring strIn2 = wstrIn2_;

		MakeUpperCase( strIn1 );
		MakeUpperCase( strIn2 );

		if( wcscmp( strIn1.c_str(), strIn2.c_str() ) == 0 )
			return true;
		else 
			return false;

    }//CaseInsensitiveEqual()

}//namespace   KStringUtil


struct  KCaseInsensitiveLessA : public std::binary_function<std::string,std::string,bool>
{
    result_type operator () ( const first_argument_type& strIn1_, const second_argument_type& strIn2_ ) const
    {
        return  KStringUtil::CaseInsensitiveLess( strIn1_, strIn2_ );
    }//
};//struct  KCaseInsensitiveLessA


struct  KCaseInsensitiveLessW : public std::binary_function<std::wstring,std::wstring,bool>
{
    result_type operator () ( const first_argument_type& wstrIn1_, const second_argument_type& wstrIn2_ ) const
    {
        return  KStringUtil::CaseInsensitiveLess( wstrIn1_, wstrIn2_ );
    }//
};//struct  KCaseInsensitiveLessA


struct  KCaseInsensitiveEqualA : public std::binary_function<std::string,std::string,bool>
{
    result_type operator () ( const first_argument_type& strIn1_, const second_argument_type& strIn2_ ) const
    {
        return  KStringUtil::CaseInsensitiveEqual( strIn1_, strIn2_ );
    }//
};//struct  KCaseInsensitiveLessA


struct  KCaseInsensitiveEqualW : public std::binary_function<std::wstring,std::wstring,bool>
{
    result_type operator () ( const first_argument_type& wstrIn1_, const second_argument_type& wstrIn2_ ) const
    {
        return  KStringUtil::CaseInsensitiveEqual( wstrIn1_, wstrIn2_ );
    }//
};//struct  KCaseInsensitiveLessA


#endif // #define _KSTRINGUTIL_H