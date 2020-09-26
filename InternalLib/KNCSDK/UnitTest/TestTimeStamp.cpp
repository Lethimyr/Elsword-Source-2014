#include <boost/test/unit_test.hpp>

#include <iostream>
#include <string>
#include <dbg/dbg.hpp>
#include <KncUtil.h>

void TestTimeStamp()
{
    std::string strTimeStamp;
    std::wstring wstrTimeStamp;

    KncUtil::GetTimeStampByNameA( strTimeStamp );
    KncUtil::GetTimeStampByNameW( wstrTimeStamp );


    
    std::wcout.imbue( std::locale("KOR"));
    std::cout << strTimeStamp << std::endl;
    std::wcout << wstrTimeStamp;

}