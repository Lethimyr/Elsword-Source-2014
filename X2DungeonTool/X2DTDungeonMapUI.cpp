#include "dxstdafx.h"
#include ".\x2dtdungeonmapui.h"

KX2DTDungeonMapUI::KX2DTDungeonMapUI( CX2Dungeon* pDungeon )
{
	m_bShow				= false;	
	m_vPos				= D3DXVECTOR2(250, 250);

	m_fCurrStageAlpha	= 0.f;

	m_mapDungeonMapList = NULL;
	m_pDungeon			= pDungeon;
	m_pairPickMiniMap	= std::make_pair( -1, -1 );
	
	m_pBGTexture		= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"FadeInOut.tga" );
	m_pStageTexture 	= g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Dungeon_Map_Stage_Mark.tga" );
	m_pBossStageTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Dungeon_Map_Boss_Stage_Mark.tga" );
	m_pCurrStageTexture = g_pKTDXApp->GetDeviceManager()->OpenTexture( L"DLG_Dungeon_Map_Current_Stage_Mark.tga" );

	m_LineManager.OnCreatedDevice( g_pKTDXApp->GetDevice() );

	// 반드시 트리거가 있는 스크립트만 MakeLine()을 호출할것!!
	//MakeLine( pDungeon );
}

KX2DTDungeonMapUI::~KX2DTDungeonMapUI()
{
	SAFE_CLOSE( m_pBGTexture );	
	SAFE_CLOSE( m_pStageTexture );
	SAFE_CLOSE( m_pBossStageTexture );
	SAFE_CLOSE( m_pCurrStageTexture );
}

HRESULT KX2DTDungeonMapUI::OnResetDevice()
{
	m_LineManager.OnResetDevice();
	return S_OK;
}

HRESULT KX2DTDungeonMapUI::OnLostDevice()
{
	m_LineManager.OnLostDevice();
	return S_OK;
}

void KX2DTDungeonMapUI::OnFrameRender()
{
	if( false == m_bShow )
		return;

	if( NULL == m_pDungeon || NULL == m_mapDungeonMapList )
		return;


	if( NULL != m_pBGTexture )
	{

		m_pBGTexture->Draw( (int)m_vPos.x, (int)m_vPos.y, 
			m_pBGTexture->GetWidth()+550, m_pBGTexture->GetHeight()+300, D3DCOLOR_RGBA( 255, 255, 255, 255 ) );
	}

	// 일단은 라인 출력 안함!!
	//m_LineManager.OnRender();

	m_fCurrStageAlpha -= 0.01f;
	if( m_fCurrStageAlpha < 0.f )
	{
		m_fCurrStageAlpha = 1.2f;
	}

	const map< std::pair<int,int>, CX2Dungeon::DungeonMapData >& mapDungeonMapData = *m_mapDungeonMapList;
	map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::const_iterator it;

	for( it = mapDungeonMapData.begin(); it != mapDungeonMapData.end(); it++ )
	{
		const CX2Dungeon::DungeonMapData& dungeonMapData = it->second;


		D3DCOLOR coStageColor = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		if( false == m_pDungeon->GetDungeonMapOpen( (int)it->first.first, (int)it->first.second ) )
		{
			coStageColor = D3DCOLOR_RGBA( 128, 128, 128, 255 );
		}

		if( true == dungeonMapData.m_bIsBossStage )
		{
			m_pBossStageTexture->Draw( (int) (m_vPos.x + dungeonMapData.m_vPos.x - m_pBossStageTexture->GetWidth()*0.5f),
				(int) (m_vPos.y + dungeonMapData.m_vPos.y - m_pBossStageTexture->GetHeight()*0.5f ), 
				m_pBossStageTexture->GetWidth(), m_pBossStageTexture->GetHeight(), coStageColor );
		}
		else
		{
			m_pStageTexture->Draw( (int) (m_vPos.x + dungeonMapData.m_vPos.x - m_pStageTexture->GetWidth()*0.5f),
				(int) (m_vPos.y + dungeonMapData.m_vPos.y - m_pStageTexture->GetHeight()*0.5f ), 
				m_pStageTexture->GetWidth(), m_pStageTexture->GetHeight(), coStageColor );
		}
		
		if( it->first.first == m_pairPickMiniMap.first && it->first.second == m_pairPickMiniMap.second )
		{
			m_pCurrStageTexture->Draw( (int) (m_vPos.x + dungeonMapData.m_vPos.x - m_pCurrStageTexture->GetWidth()*0.5f),
				(int) (m_vPos.y + dungeonMapData.m_vPos.y - m_pCurrStageTexture->GetHeight()*0.5f ), 
				m_pCurrStageTexture->GetWidth(), m_pCurrStageTexture->GetHeight(), D3DCOLOR_RGBA( 255, 255, 255, min(255, (int)(m_fCurrStageAlpha*255) )) );
		}
	}
}

void KX2DTDungeonMapUI::MakeLine( CX2Dungeon* pDungeon )
{
	if( NULL == pDungeon )
		return;

	KGCMassFileManager::CMassFile::MASSFILE_MEMBERFILEINFO_POINTER Info;
	Info = g_pKTDXApp->GetDeviceManager()->GetMassFileManager()->LoadDataFile( pDungeon->GetDungeonData()->m_DataFileName );
	if( Info == NULL )
		return;

	wstring wstrDungeonScript;
	ConvertCharToWCHAR( wstrDungeonScript, Info->pRealData );

	int iFirstFunctionIndex = wstrDungeonScript.find( L"function" );
	wstrDungeonScript = wstrDungeonScript.substr( iFirstFunctionIndex, wstrDungeonScript.length()-iFirstFunctionIndex );



	map< std::pair<int, int>, SubStageNode>::iterator it;
	m_mapStageGraph.clear();

	vector<CX2DungeonStage::StageData*>& vecStageList = pDungeon->GetStageDataList();
	for( UINT i=0; i<vecStageList.size(); i++ )
	{
		CX2DungeonStage::StageData* pStageData = vecStageList[i];

		for( UINT j=0; j<pStageData->m_SubStageDataList.size(); j++ )
		{
			CX2DungeonSubStage::SubStageData* pSubStageData = pStageData->m_SubStageDataList[j];

			it = m_mapStageGraph.find( std::make_pair(i,j) );
			if( it == m_mapStageGraph.end() )
			{
				SubStageNode subStageNode;
				m_mapStageGraph[ std::make_pair(i,j) ] = subStageNode;
			}

			it = m_mapStageGraph.find( std::make_pair(i,j) );
			SubStageNode& nowSubStageNode = it->second;



			for( UINT k=0; k<pSubStageData->m_TriggerList.size(); k++ )
			{
				CX2DungeonSubStage::Trigger* pTrigger = pSubStageData->m_TriggerList[k];

				wstring wstrReactFuncName = L"function ";
				wstrReactFuncName += pTrigger->m_ReactFunc;

				int iFoundIndex = wstrDungeonScript.find( wstrReactFuncName );
				if( iFoundIndex != std::wstring::npos )
				{
					int iNextFunctionOrFileEndIndex = wstrDungeonScript.find( L"function", iFoundIndex+1 );
					if( iNextFunctionOrFileEndIndex == std::wstring::npos )
					{
						iNextFunctionOrFileEndIndex = wstrDungeonScript.length();
					}

					wstring wstrReactFunc = wstrDungeonScript.substr( iFoundIndex, iNextFunctionOrFileEndIndex-iFoundIndex );
					int iClearSubStageIndex = 0;
					int iLastClosingParenthesisIndex = 0;
					while( true )
					{

						iClearSubStageIndex = wstrReactFunc.find( L"ClearSubStage", iLastClosingParenthesisIndex );
						if( iClearSubStageIndex != std::wstring::npos && 
							iClearSubStageIndex != iLastClosingParenthesisIndex )
						{


							int iFirstCommaIndex	= wstrReactFunc.find( L",", iClearSubStageIndex+1 );
							int iLastCommaIndex		= wstrReactFunc.find( L",", iFirstCommaIndex+1 );
							iLastClosingParenthesisIndex = wstrReactFunc.find( L")", iLastCommaIndex+1 );

							wstring wstrStageIndex		= wstrReactFunc.substr( iFirstCommaIndex+1, iLastCommaIndex-iFirstCommaIndex-1);
							wstring wstrSubStageIndex	= wstrReactFunc.substr( iLastCommaIndex+1, iLastClosingParenthesisIndex-iLastCommaIndex-1 );

							int iStageIndex		= (int) wcstol( wstrStageIndex.c_str(), NULL, 10 );
							int iSubStageIndex	= (int) wcstol( wstrSubStageIndex.c_str(), NULL , 10 );


							CX2DungeonSubStage::CLEAR_TYPE eClearType = CX2DungeonSubStage::CT_GAME;

							int iClearTypeIndex = wstrReactFunc.find( L"CT_STAGE", iClearSubStageIndex );
							if( std::wstring::npos != iClearTypeIndex &&
								iClearTypeIndex < iFirstCommaIndex )
							{
								eClearType = CX2DungeonSubStage::CT_STAGE;
								iSubStageIndex = 0;
							}

							iClearTypeIndex = wstrReactFunc.find( L"CT_SUB_STAGE", iClearSubStageIndex );
							if( std::wstring::npos != iClearTypeIndex &&
								iClearTypeIndex < iFirstCommaIndex )
							{
								eClearType = CX2DungeonSubStage::CT_SUB_STAGE;
							}

							if( eClearType == CX2DungeonSubStage::CT_GAME )
								continue;


							nowSubStageNode.m_vecNextSubStageIndex.push_back( std::make_pair( iStageIndex, iSubStageIndex) );

							it = m_mapStageGraph.find( std::make_pair( iStageIndex, iSubStageIndex ) );
							if( it == m_mapStageGraph.end() )
							{
								SubStageNode nextSubStageNode;
								m_mapStageGraph[ std::make_pair(iStageIndex, iSubStageIndex) ] = nextSubStageNode;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}


	// make line
	m_LineManager.RemoveAllLines();
	D3DXVECTOR2 vVertexPos[2];

	const map< std::pair<int,int>, CX2Dungeon::DungeonMapData >& mapDungeonMapData = m_pDungeon->GetMapDungeonMapData();
	map< std::pair<int,int>, CX2Dungeon::DungeonMapData >::const_iterator it2;


	for( it=m_mapStageGraph.begin(); it!=m_mapStageGraph.end(); it++ )
	{
		SubStageNode& subStageNode = it->second;
		it2 = mapDungeonMapData.find( it->first );
		if( it2 == mapDungeonMapData.end() )
			continue;

		const CX2Dungeon::DungeonMapData& dungeonMapData = it2->second;
		for( UINT j=0; j< subStageNode.m_vecNextSubStageIndex.size(); j++ )
		{
			SubStageNode& nextSubStageNode = m_mapStageGraph[ subStageNode.m_vecNextSubStageIndex[j] ];
			it2 = mapDungeonMapData.find( subStageNode.m_vecNextSubStageIndex[j] );
			if( it2 == mapDungeonMapData.end() )
				continue;

			const CX2Dungeon::DungeonMapData& nextDungeonMapData = it2->second;

			vVertexPos[0] = m_vPos + dungeonMapData.m_vPos;
			vVertexPos[1] = m_vPos + nextDungeonMapData.m_vPos;

			m_LineManager.AddLine( NULL, (D3DXVECTOR2*)&vVertexPos[0], 2, D3DCOLOR_RGBA( 255, 255, 255, 255 ), 
				4.f, 1.f, false );
		}
	}

}