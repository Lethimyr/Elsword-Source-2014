#include "dxstdafx.h"
#include ".\eventscriptparsing.h"

CEventScriptParsing::CEventScriptParsing(void)
{
}

CEventScriptParsing::~CEventScriptParsing(void)
{
}

void CEventScriptParsing::OpenScriptFile( IN const WCHAR * pFileName )
{
	WCHAR wszPath[MAX_PATH] = L"";
	WCHAR wszFileName[MAX_PATH] = L"";

	// 풀패스를 경로와 파일이름으로 분리
	if( true == _EST_OBJECT_::DecomposePathAndFileName(wszPath, wszFileName, pFileName) )
	{
		KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
		Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( wszFileName );
		// 파일이름으로 로딩에 실패하면
		if( Info == NULL )
		{
			std::wstring wstrDirectory = wszPath;

			// 경로를 추가함
			g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->AddDataDirectory(wstrDirectory);

			// 새로운 경로 추가를 통해 다시 파일오픈.
			Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( wszFileName );
			if( Info == NULL )
			{
				std::wstring wstrMsg = L"파일 열기 실패.! ";
				wstrMsg += pFileName;
				ERRORMSG(wstrMsg.c_str());
				return;
			}
			else
			{
				ASSERT( NULL != Info->pRealData );
				if( NULL == Info->pRealData )
				{
					ERRORMSG(L"스크립트 데이터 내용이 없음.!");
					return;
				}

				if( false == Parsing(Info->pRealData, Info->size) )
				{
					std::wstring wstrMsg = L"스크립트 분석실패.! ";
					wstrMsg += pFileName;
					ERRORMSG(wstrMsg.c_str());
					return;
				}
			}
		}
		else
		{
			if( false == Parsing(Info->pRealData, Info->size) )
			{
				std::wstring wstrMsg = L"스크립트 분석실패.! ";
				wstrMsg += pFileName;
				ERRORMSG(wstrMsg.c_str());
				return;
			}
		}
	}
	else
	{
		std::wstring wstrMsg = L"파일 경로가 잘못되었음.! ";
		wstrMsg += pFileName;
		ERRORMSG(wstrMsg.c_str());
	}
}

void CEventScriptParsing::GetEventIDList( _EST_OBJECT_::EventIDList & eventIDList )
{
	_EST_OBJECT_::EventSceneData::iterator mit;

	mit = m_mapEventSceneData.begin();
	while( mit != m_mapEventSceneData.end() )
	{
		eventIDList.push_back(mit->first);
		++mit;
	}
}

bool CEventScriptParsing::GetUTF8byEventScriptData( IN std::wstring wstrEventID, OUT std::wstring &wstrScript )
{
	char charBuf[255] = {0,};
	::WideCharToMultiByte( CP_ACP, 0, wstrEventID.c_str(), -1, charBuf, 255, NULL, NULL );

	_EST_OBJECT_::EventSceneData::iterator mit;
	mit = m_mapEventSceneData.find(charBuf);

	if( mit == m_mapEventSceneData.end() )
		return false;

	WCHAR *pwszScript = new WCHAR[mit->second.size()];

	::MultiByteToWideChar( CP_UTF8, 0, mit->second.c_str(), -1, pwszScript, mit->second.size() );

	wstrScript = pwszScript;

	delete[] pwszScript;

	return true;
}

bool CEventScriptParsing::ScriptRefresh( IN const WCHAR *pScrpt, IN const long iLength )
{
	int iBufferSize = iLength * 2;
	//int iResultSize = 0;

	char *pcBuffer = new char[iBufferSize]; // 주석으로 한글이 끼여있을경우 2바이트가 필요하기 때문에 *2 를 함. 

	
	/*iResultSize = */::WideCharToMultiByte( CP_UTF8, 0, pScrpt, -1, pcBuffer, iBufferSize, NULL, NULL );

	bool bResult = Parsing(pcBuffer, strlen(pcBuffer)/*iResultSize*/);

	if( false == bResult )
		ERRORMSG(L"Script 파싱실패.!");

	// Lua refresh
	// Refresh 가 성공했으면 루아도 새롭게 파싱해준다.
	if( true == bResult )
	{
		lua_tinker::decl( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "g_pEventScene", GetRenderFrm()->GetX2EventScene() );
		bResult = lua_tinker::dostring(g_pKTDXApp->GetLuaBinder()->GetLuaState(), pcBuffer);
		//bResult = g_pKTDXApp->GetLuaBinder()->DoMemoryNotEncript(pTemp, iResultSize + 3);

		if( false == bResult )
			ERRORMSG(L"Script 루아 재로딩 실패.!");
	}

	SAFE_DELETE_ARRAY(pcBuffer);

	return bResult;
}

void CEventScriptParsing::Clear()
{
	m_mapEventSceneData.clear();
}

bool CEventScriptParsing::SaveScript()
{
	std::wstring wstrFileName;

	if( true == _EST_OBJECT_::GetDlgFileName(g_pKTDXApp->GetHWND(), wstrFileName, false) )
	{
		WCHAR wszBuffer[MAX_SCRIPT_BUFFER] = L"";
		char szBuffer[MAX_SCRIPT_BUFFER] = "";

		::WideCharToMultiByte( CP_ACP, 0, wstrFileName.c_str(), -1, szBuffer, MAX_PATH, NULL, NULL );

		FILE *file = NULL;
		file = fopen(szBuffer, "w");

		if( NULL == file )
		{
			ERRORMSG(L"파일저장 실패.!")
				return false;
		}

		fputc( 0xef, file );
		fputc( 0xbb, file );
		fputc( 0xbf, file );

		wcscpy(wszBuffer, L"-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.");
		::WideCharToMultiByte( CP_UTF8, 0, wszBuffer, -1, szBuffer, MAX_SCRIPT_BUFFER, NULL, NULL);
		fprintf( file, "%s", szBuffer );

		fprintf( file, "\n" );
		fprintf( file, "\n" );

		_EST_OBJECT_::EventSceneData::iterator mit;
		mit = m_mapEventSceneData.begin();

		size_t sizeWrote = 0;
		while( mit != m_mapEventSceneData.end() )
		{
			//::MultiByteToWideChar( CP_ACP, 0, mit->second.c_str(), -1, wszBuffer, MAX_SCRIPT_BUFFER );
			//::WideCharToMultiByte( CP_ACP, 0, wszBuffer, -1, szBuffer, MAX_SCRIPT_BUFFER, NULL, NULL);
			//fprintf( file, "%s", szBuffer );
			fprintf( file, "%s", mit->second.c_str() );

			fprintf( file, "\n" );
			fprintf( file, "\n" );

			++mit;
		}

		fclose(file);
	}

	return true;
}

bool CEventScriptParsing::Parsing( IN const char * pScript, IN const long iLength )
{
	const char* p = pScript;

	//{{begin} 한블럭의 스크립트 파싱이 끝나면 초기화 되어야 할 변수들..
	std::string strID;
	std::string strScript;
	// true : pScript 내용이 g_pEventScene: 이면
	// false : pScript 가 } 로 마무리 되었다면.
	bool bIsParsing = false;
	int iBlockCount = -1;
	//{{end} 한블럭의 스크립트 파싱이 끝나면 초기화 되어야 할 변수들..

	//{{begin} 아이디 파싱이 끝나면 초기화 되어야 할 변수들(스크립트가 파싱시작후에 동작)
	std::string strIDName;
	bool bWaitforIDParsing = false;
	bool bBeginforIsIDParsing = false;
	//{{end} 아이디 파싱이 끝나면 초기화 되어야 할 변수들(스크립트가 파싱시작후에 동작)

	while( (*p) != 0 ) // check EOF
	{
		// 스크립트 파싱 시작체크.
		if( false == bIsParsing &&	// 아직 파싱전이고
			(*p) == 'g' )	// 소문자 g 이면 체크시작.
		{
			std::string strOrg, strPar;
			strOrg = "g_pEventScene:";

			const char * pTemp = p;

			while( strOrg.size() > strPar.size() )
			{
				if( (*p) != 0 )
				{
					strPar += (*pTemp);
				}
				else
					break;	// while end for EOF

				++pTemp;
			}

			if( strOrg.compare(strPar) == 0 )
				bIsParsing = true;
		}

		// 파싱시작이면..
		if( bIsParsing == true )
		{
			strScript += (*p);

			//{{begin} 스크립트 파싱 종료 체크
			if( (*p) == '{' ) 
			{	
				++iBlockCount; if( iBlockCount == 0 ) ++iBlockCount; 
			}
			else
			if( (*p) == '}' ) 
			{
				--iBlockCount;
			}
			//{{end} 스크립트 파싱 종료 체크

			//{{begin} EVENT_SCENE_ID 값 체크.
			if( iBlockCount == 1 && strID.empty() == true )	// 아이디 파싱 시작체크
			{
				if( isupper((*p)) != 0 || islower((*p)) != 0 || (*p) == '_' ) // 대,소문자 또는 '_' 일경우
				{
					strIDName += (*p);
				}
				else
				if( isspace((*p)) != 0 || 
					(*p) == ':' || 
					(*p) == '{' || 
					(*p) == '}' ||
					(*p) == '=' || 
					(*p) == ',' ||
					(*p) == '-'
					)
				{
					if( strIDName.empty() == false && strIDName.compare("EVENT_SCENE_ID") == 0 )
					{
						bWaitforIDParsing = true;
						strIDName.clear();
					}
				}
			}

			if( (*p) == '\"' ) // 아이디 파싱 시작,종료 체크.
			{
				if( bWaitforIDParsing == true && bBeginforIsIDParsing == true )	// 따옴표(") 가 끝나는 부분이면 꺼주기.
				{
					bWaitforIDParsing = false;
					bBeginforIsIDParsing = false;
				}

				if( bWaitforIDParsing == true && bBeginforIsIDParsing == false )
					bBeginforIsIDParsing = true;
			}

			if( bWaitforIDParsing == true && bBeginforIsIDParsing == true )
			{
				if( (*p) != '\"' )
					strID += (*p);
			}
			//{{end} EVENT_SCENE_ID 값 체크.

			if( iBlockCount == 0 ) // 스크립트 파싱후 모든블록이 닫히면 파싱종료.
			{
				bIsParsing = false;
				iBlockCount = -1;

				strScript += '\0';

				// Refresh 용으로도 함수가 사용되기 때문에 같은 아이디가 있으면 삭제후 넣어준다.
				_EST_OBJECT_::EventSceneData::iterator mit;
				mit = m_mapEventSceneData.find(strID);
				if( mit != m_mapEventSceneData.end() )
					m_mapEventSceneData.erase(mit);

				// 모든 처리가 끝났으면 넣어준다.
				m_mapEventSceneData.insert(std::make_pair(strID, strScript));

				strID.clear();
				strScript.clear();
			}
		}

		++p;
	}

	return true;
}