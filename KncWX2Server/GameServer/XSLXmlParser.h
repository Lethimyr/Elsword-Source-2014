#pragma once

#define IN
#define OUT

#include <Windows.h>
#include "BaseServer.h"

#include "RefreshSingleton.h"
#include <map>
#include <vector>


struct KPropertyValue;
class CXSLXmlParser
{
	DeclareRefreshSingleton( CXSLXmlParser );
	DeclareLuaScriptParser;

public:
	CXSLXmlParser(void);
	~CXSLXmlParser(void);

	bool ConvertXMLStringToValue( IN std::map< unsigned short, KPropertyValue >& mapProperty, IN int iPolicyResultNo, OUT int& iOutUserRetaining );		// XML 형식의 string 에서 value 추출
	int XMLStringToWString( IN std::string& strPropertyValue, IN int iPolicyResultNo );

	
};

DefRefreshSingletonInline( CXSLXmlParser );
