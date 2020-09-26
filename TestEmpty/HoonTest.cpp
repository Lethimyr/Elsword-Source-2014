#include "dxstdafx.h"
#include ".\hoontest.h"

CHoonTest::CHoonTest(void)
{
	if( g_pKTDXApp->GetDeviceManager()->LoadLuaTinker(L"Enum.lua") == false )
	{
		::MessageBox(g_pKTDXApp->GetHWND(), L"Enum.lua parsing fail.!", L"waring", MB_OK);
	}
	if( g_pKTDXApp->GetDeviceManager()->LoadLuaTinker(L"StringID_def.lua") == false )
	{
		::MessageBox(g_pKTDXApp->GetHWND(), L"StringID_def.lua parsing fail.!", L"waring", MB_OK);
	}

	lua_State* L= g_pKTDXApp->GetLuaBinder()->GetLuaState();
	lua_tinker::class_add<CX2UnitManager>( L, "CX2UnitManager" );
	lua_tinker::class_def<CX2UnitManager>( L, "AddUnitTemplet",			&CX2UnitManager::AddUnitTemplet_LUA );
	lua_tinker::class_def<CX2UnitManager>( L, "AddNPCTemplet",			&CX2UnitManager::AddNPCTemplet_LUA );
	lua_tinker::class_def<CX2UnitManager>( L, "AddNPCStat",				&CX2UnitManager::AddNPCStat_LUA );
	lua_tinker::class_def<CX2UnitManager>( L, "AddNPCExtraAbility_LUA", &CX2UnitManager::AddNPCExtraAbility_LUA );

	if( m_UnitManger.OpenNPCScriptFile(L"NPCTemplet.lua") == false )
	{
		::MessageBox(g_pKTDXApp->GetHWND(), L"NPCTemplet.lua parsing fail.!", L"waring", MB_OK);
	}
}

CHoonTest::~CHoonTest(void)
{
}

HRESULT CHoonTest::OnFrameMove( double fTime, float fElapsedTime )
{

	return S_OK;
}

HRESULT CHoonTest::OnFrameRender()
{

	return S_OK;
}

HRESULT CHoonTest::OnResetDevice()
{

	return S_OK;
}

HRESULT CHoonTest::OnLostDevice()
{
	return S_OK;
}

bool CHoonTest::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( uMsg == WM_DROPFILES )
	{
		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState(), 0, false );

		if( false == g_pKTDXApp->GetDeviceManager()->LoadLuaTinker( wstrFileName.c_str() ) )
		{
			std::wstring wstrMSG = wstrFileName.c_str();
			wstrMSG += L"parsing fail.!";
			::MessageBox(g_pKTDXApp->GetHWND(), wstrMSG.c_str(), L"waring", MB_OK);

			return false;
		}

		if( luaManager.BeginTable( "test_table" ) == true )
		{
			int cCount = 1;

			while( luaManager.BeginTable( "data", cCount ) == true )
			{
				
				LUA_GET_VALUE( luaManager , 
				STAGE_DATA sStageData;
				sStageData.m_iStage = stageIndex;

				int subStageIndex = 0;
				while( luaManager.BeginTable( "SUB_STAGE", subStageIndex ) == true )
				{
					SUB_STAGE_DATA sSubStageData;
					sSubStageData.m_iSubStage = subStageIndex;

					if( luaManager.BeginTable( "NPC_GROUP" ) == true )
					{
						int npcIndex = 1;
						while( luaManager.BeginTable( npcIndex ) == true )
						{
							

							std::map<int,int>::iterator mit;
							mit = sSubStageData.m_mapNPC.find(iNpcID);
							if( mit != sSubStageData.m_mapNPC.end() )
								mit->second += 1;
							else
								sSubStageData.m_mapNPC.insert( std::make_pair(iNpcID, 1) );

							npcIndex++;			
							luaManager.EndTable();

						}
						luaManager.EndTable(); // NPC_GROUP
					}

					sStageData.m_vecSubStageData.push_back(sSubStageData);

					++subStageIndex;
					luaManager.EndTable(); //Sub Stage
				}

				vecStage.push_back(sStageData);

				++stageIndex;
				luaManager.EndTable(); // Stage(i)	
			}

			luaManager.EndTable(); // Stage list
		}
		//{{End} 던전파일 파싱.
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//{{Begin} 몬스터 정보출력
		std::vector<STAGE_DATA>::const_iterator vit;
		vit = vecStage.begin();

		std::string strFileName;
		std::wstring wstrTemp;
		wstrTemp = fname;
		wstrTemp += L".txt";
		ConvertWCHARToChar(strFileName, wstrTemp);

		//# 던전 스크립트 파일이름을 이용한 txt 파일 출력
		std::ofstream ofs( strFileName.c_str() );

		ConvertWCHARToChar(strFileName, wstrFileName);

		::SetWindowText(g_pKTDXApp->GetHWND(), wstrFileName.c_str());

		//# 파일 첫줄에 던전스크립트 파일명 출력
		ofs << strFileName << std::endl << std::endl;

		while( vit != vecStage.end() )
		{
			//# 스테이지 번호 출력
			ofs << "STAGE_" << (*vit).m_iStage << std::endl;

			std::vector<SUB_STAGE_DATA>::const_iterator svit;
			svit = (*vit).m_vecSubStageData.begin();

			while( svit != (*vit).m_vecSubStageData.end() )
			{
				//# 서브 스테이지 번호 출력
				ofs << "\t SUB_STAGE_" << (*svit).m_iSubStage << std::endl;

				std::map<int,int>::const_iterator mit;
				mit = (*svit).m_mapNPC.begin();
				while( mit != (*svit).m_mapNPC.end() )
				{
					CX2UnitManager::NPCUnitTemplet *pNPCTemplet = m_UnitManger.GetNPCUnitTemplet(CX2UnitManager::NPC_UNIT_ID(mit->first));

					std::string strNPCName;
					if( NULL != pNPCTemplet )
					{
						ConvertWCHARToChar(strNPCName, pNPCTemplet->m_Name);
					}

					//# 이름이 있는 몬스터만 출력
					if( strNPCName.empty() == false )
						ofs << "\t" << "\t" << strNPCName << "\t" << mit->first << "\t" << mit->second << std::endl;

					++mit;
				}
				++svit;
			}

			ofs << std::endl;
			++vit;
		}

		ofs.close();
		//{{End} 몬스터 정보출력
		//////////////////////////////////////////////////////////////////////////

	}

	return false;
}