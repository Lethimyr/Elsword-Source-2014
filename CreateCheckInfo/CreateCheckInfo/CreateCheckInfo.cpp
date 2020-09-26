// CreateCheckInfo.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "SHA1.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/test/utils/foreach.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>

#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment( lib, "libxml_mtd.lib" )
#else
	#pragma comment( lib, "libxml_mt.lib" )
#endif


using namespace std;
using namespace boost;
typedef tokenizer<char_separator<char>> Tokenizer;

struct CheckInfo
{
	std::string m_strFileName;
	std::string m_strSha1;

	CheckInfo( const char* szFileName_ )
		: m_strFileName( szFileName_ )
	{}

	CheckInfo( const std::string& strFileName_ )
		: m_strFileName( strFileName_ )
	{}

	CheckInfo( const char* szFileName_, const char* szSha1_ )
		: m_strFileName( szFileName_ ), m_strSha1( szSha1_ )
	{}

	CheckInfo( const std::string& strFileName_, const std::string& strSha1_ )
		: m_strFileName( strFileName_ ), m_strSha1( strSha1_ )
	{}

	// 주의!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// CheckInfo의 == 판단은 m_strFileName만 가지고 판단함
	// 그러나 = 연산자의 경우 m_strFileName, m_strSha1 모두를 assign 함

	bool operator == ( const CheckInfo& rhs_ )
	{
		// 파일명이 같으면 true를 리턴
		if ( 0 == m_strFileName.compare( rhs_.m_strFileName ) )
			return true;
		else
			return false;
	}

	bool operator == ( const string& strFileName_ )
	{
		// 파일명이 같으면 true를 리턴
		if ( 0 == m_strFileName.compare( strFileName_ ) )
			return true;
		else
			return false;
	}
};

struct CompareOnlyFileNameFunc
{
	bool operator()( const CheckInfo& lhs, const CheckInfo& rhs )
	{
		if ( 0 == lhs.m_strFileName.compare( rhs.m_strFileName ) )
			return true;
		else
			false;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	// 인자가 3개 보다 작으면 실패
	if ( 3 > argc )
	{
		std::cerr << "Argument's size is wrong!" << std::endl;
		return -1;
	}
	
	// SHA를 구할 kom 파일들 이름
	std::string strKomFiles	= argv[2];

	// 분리
	char_separator<char> sep( "/" );
	Tokenizer tok( strKomFiles, sep );
	
	std::vector<CheckInfo> vecCheckInfoFromLocalFiles;
	for ( Tokenizer::iterator tItr = tok.begin(); tItr != tok.end(); ++tItr )
	{
		//std::cout << *tItr << std::endl;
		vecCheckInfoFromLocalFiles.push_back( CheckInfo( *tItr ) );	// 분리한 것을 vector에 넣음
	}

	// kom 파일들이 있는 경로
	std::string strPath		= argv[1];
	// Local에 있는 KOM 파일의 sha를 얻어냄
	BOOST_TEST_FOREACH( CheckInfo&, val, vecCheckInfoFromLocalFiles )
	{
		std::string strFullFileName = strPath;
		strFullFileName += "\\data\\";
		strFullFileName += val.m_strFileName;

		CSHA1 Sha1;
		if ( Sha1.HashFile( strFullFileName.c_str() ) )
		{
			char aReport[50] = { 0, };
			Sha1.Final();
			Sha1.ReportHash( aReport, CSHA1::REPORT_HEX, true );
			
			//std::cout << aReport << std::endl;
			val.m_strSha1 = aReport;
		}
		// Local에 Sha를 조사하고자 하는 kom이 없다면 sha를 empty 상태로 하여 vector에 넣어둠
	}

	std::string strPathCheckInfo = strPath;
	strPathCheckInfo += "\\checkkom.xml";
	
	/*
	// xml 파일 읽어오기
	std::vector<CheckInfo> vecCheckInfoFromXml;
	stringstream streamXml;
	ifstream inputFileStream( strPathCheckInfo, ios_base::in );
	if ( true == inputFileStream.is_open() )
	{
		streamXml << inputFileStream.rdbuf();
		inputFileStream.close();

		// xml로 변경
		xmlDoc* doc = xmlReadMemory( streamXml.str().data(), streamXml.str().size(), "checksum.xml", NULL, 0 );

		xmlNode* files = xmlDocGetRootElement(doc);
		if(files->type == XML_ELEMENT_NODE && strcmp((char*)files->name,"Files")==0)
		{			
			xmlNode * file = files->children;
			while ( file )
			{
				if ( file->type == XML_ELEMENT_NODE && strcmp((char*)file->name,"File")==0 )
				{	
					std::string strFileName;
					std::string strSha1;
					xmlAttr * attribute =  file->properties;

					while ( attribute )
					{
						if ( strcmp ( (char*)attribute->name,"Name" ) == 0 && attribute->children )
						{
							strFileName = (char*)attribute->children->content;						
						}
						else if ( strcmp ( (char*)attribute->name,"Sha" ) == 0 && attribute->children )
						{
							strSha1 = (char*)attribute->children->content;
						}
						attribute = attribute->next;
					}
					vecCheckInfoFromXml.push_back( CheckInfo( strFileName, strSha1 ) );
				}
				file = file->next;
			}
		}			
		xmlFreeDoc(doc);
	}	
	
	BOOST_TEST_FOREACH( CheckInfo&, localFileCheckInfo, vecCheckInfoFromLocalFiles )
	{
		if ( localFileCheckInfo.m_strSha1.empty() )
		{
			vector<CheckInfo>::iterator vItrFromXml = find( vecCheckInfoFromXml.begin(), vecCheckInfoFromXml.end(), localFileCheckInfo );

			if ( vecCheckInfoFromXml.end() != vItrFromXml )
			{
				localFileCheckInfo.m_strSha1 = vItrFromXml->m_strSha1;
			}
		}
	}
	//*/

	// xml 파일 만들기
	std::stringstream stream;
	stream << "<?xml version=\"1.0\"?>" << std::endl << "<Files>" << std::endl;

	BOOST_TEST_FOREACH( CheckInfo&, localFileCheckInfo, vecCheckInfoFromLocalFiles )
	{
		if ( !( localFileCheckInfo.m_strSha1.empty() ) )
		{
			stream << "<File";
			stream << " Name=\"" << localFileCheckInfo.m_strFileName << "\"";
			stream << " Sha=\"" << localFileCheckInfo.m_strSha1 << "\"";
			stream << " />" << std::endl;			

			std::cout << "KomName: " << localFileCheckInfo.m_strFileName << "\t" << "SHA-1: " << localFileCheckInfo.m_strSha1 << std::endl;
		}
		else
		{
			std::cout << localFileCheckInfo.m_strFileName << " does not exist." << std::endl;
		}
	}
	stream << "</Files>";

	ofstream outputFileStream( strPathCheckInfo, ios_base::out );
	if ( true == outputFileStream.is_open() )
	{
		outputFileStream << stream.rdbuf();
		outputFileStream.close();
	}	

	return 0;
}

