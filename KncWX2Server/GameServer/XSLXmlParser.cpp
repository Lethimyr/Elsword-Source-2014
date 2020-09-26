#include "XSLXmlParser.h"
#include "TinyXml/tinyxml.h"


ImplementRefreshSingleton( CXSLXmlParser );

CXSLXmlParser::CXSLXmlParser(void)
{
}

CXSLXmlParser::~CXSLXmlParser(void)
{
}

ImplementLuaScriptParser( CXSLXmlParser )
{
}

bool CXSLXmlParser::ConvertXMLStringToValue( IN std::map< unsigned short, KPropertyValue >& mapProperty, IN int iPolicyResultNo, OUT int& iOutUserRetaining )
{
	std::map< unsigned short, KPropertyValue >::iterator mit = mapProperty.begin();
	if( mit == mapProperty.end() )
	{
		START_LOG( cerr, L"넥슨 세션시스템에서 받은 PropertyValue 값이 하나도 없다!" );
		return false;
	}

	KPropertyValue kProperty;

	// 넥슨에서 받은 값 가공하기
	mit = mapProperty.find( 18 );        // 이탈 방지 관련 정책의 값
	if( mit != mapProperty.end() )
	{
		kProperty = mit->second;

		//kProperty.m_wstrPropertyValue = L"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n<PolicyResults>\n<PolicyResult>\n<No>1</No>\n<Name>셧다운제</Name>\n<Result>2</Result>\n<Option></Option>\n</PolicyResult>\n<PolicyResult>\n<No>3</No>\n<Name>이탈</Name>\n<Result>4</Result>\n<Option></Option>\n</PolicyResult>\n</PolicyResults>";

		USES_CONVERSION;  
		std::string strPropertyValue(W2A(kProperty.m_wstrPropertyValue.c_str()));

		iOutUserRetaining = XMLStringToWString( strPropertyValue, iPolicyResultNo );
	}

	START_LOG( clog, L"이탈방지 세션시스템에서 받은 PropertyValue 값이다!" )
		<< BUILD_LOG( iPolicyResultNo ) 
		<< BUILD_LOG( iOutUserRetaining ) 
		<< END_LOG;

	return true;
}

int CXSLXmlParser::XMLStringToWString( IN std::string& strPropertyValue, IN int iPolicyResultNo )
{
	int iRet = 0;

	TiXmlDocument temp;

	temp.Parse( strPropertyValue.c_str() );

	TiXmlElement* pMainElement = temp.FirstChildElement("PolicyResults");


	const char*		szRootName;
	TiXmlElement*	pPolicyResult	= NULL;
	TiXmlNode*		pChild			= NULL;
	TiXmlNode*		pNode			= NULL;

	int iNo = 0;

	for( pPolicyResult = pMainElement->FirstChildElement("PolicyResult") ; pPolicyResult != NULL ; pPolicyResult = pPolicyResult->NextSiblingElement() )
	{
		if( pPolicyResult )
		{
			pNode = pPolicyResult->FirstChild();

			if( !pNode )
				continue;
			szRootName = pNode->Value();		// <NO>
			pChild = pNode->FirstChild();

			if( !pChild )
				continue;
			szRootName = pChild->Value();		// NO의 값

			iNo = atoi( szRootName );
			if( iPolicyResultNo == iNo )
			{
				pNode = pNode->NextSibling();
				if( !pNode )
					continue;
				szRootName = pNode->Value();		// <Name>
				pChild = pNode->FirstChild();

				if( !pChild )
					continue;
				szRootName = pChild->Value();		// Name의 값

				pNode = pNode->NextSibling();
				if( !pNode )
					continue;
				szRootName = pNode->Value();		// <Result>
				pChild = pNode->FirstChild();

				if( !pChild )
					continue;
				szRootName = pChild->Value();		// Result의 값

				iRet = atoi( szRootName );
				return iRet;

////////////////////////////////////////////////////////////
// 일단은 필요 없는 부분이므로 주석
//
// 				pNode = pNode->NextSibling();
//				if( !pNode )
// 					continue;
// 				szRootName = pNode->Value();		// <Option>
// 				pChild = pNode->FirstChild();
// 
// 				if( !pChild )
// 					continue;
// 				szRootName = pChild->Value();		// Option의 값
//////////////////////////////////////////////////////////////////////////
				
			}

		}
	}

	return iRet;
}
