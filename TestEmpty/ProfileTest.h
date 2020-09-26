//#pragma once
///********************************************************************
//	created:	2006/10/30
//	created:	30:10:2006   11:28
//	filename: 	d:\projectX2\X2Project\TestEmpty\ProfileTest.h
//	file path:	d:\projectX2\X2Project\TestEmpty
//	file base:	ProfileTest
//	file ext:	h
//	author:		dmlee
//	
//	purpose:	새로 만드는 코드 테스트 용
//
//*********************************************************************/
//
//
//
//
//namespace SANDBOX
//{
//	// 연결된 line을 linegroup으로 묶어두기 위한 클래스
//	// 연결된 line상에 있는지 빠르게 체크하기 위해
//	class CLineMapExt
//	{
//	public:
//		struct LineGroup
//		{
//			vector<CKTDGLineMap::LineData*> vecpLineData;			
//		};
//
//	public:
//		~CLineMapExt()
//		{ 
//			ReleaseLineGroup();
//		};
//
//		CLineMapExt()
//		{
//		};		
//
//		void ReleaseLineGroup()
//		{
//			for( unsigned i=0; i<m_vecLineGroupList.size(); i++ )
//			{
//				SAFE_DELETE( m_vecLineGroupList[i] );
//			}
//			m_vecLineGroupList.clear();
//		}
//
//		LineGroup* GetLineGroupIncludesLineData( CKTDGLineMap::LineData* pLineData )
//		{
//			map<CKTDGLineMap::LineData*, LineGroup*>::iterator it;
//			it = m_mapLineData2LineGroup.find( pLineData );
//			if( it != m_mapLineData2LineGroup.end() )
//				return it->second;
//			else
//				return NULL;			
//		}
//
//		LineGroup* GetLineGroup(int index) { return m_vecLineGroupList[index]; }
//
//		void RemoveLineData( CKTDGLineMap::LineData* pLineData )
//		{
//		}
//
//		void AddLineData( CKTDGLineMap::LineData* pLineData )
//		{
//		}
//
//		void ReBuild( CKTDGLineMap* pLineMap )
//		{
//			if( NULL == pLineMap )
//				return;
//
//			int nNodes = pLineMap->GetNumLineData();
//			if( nNodes <= 0 )
//				return;
//
//			bool* bDoneLineData = new bool[nNodes];
//			for( int i=0; i<nNodes; i++ )
//				bDoneLineData[i] = false;
//
//			ReleaseLineGroup();
//
//			// line group 생성
//			for( int i=0; i<nNodes; i++ )
//			{
//				if( bDoneLineData[i] == true )
//					continue;
//
//				bDoneLineData[i] = true;
//				LineGroup* pGroup = new LineGroup();
//				CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData(i);
//				pGroup->vecpLineData.push_back( pLineData );
//
//				CKTDGLineMap::LineData* pTempLineData = pLineData;
//				while( true )
//				{			
//					if( pTempLineData->nextLine >= 0 && false == bDoneLineData[pTempLineData->nextLine] )
//					{	
//						bDoneLineData[pTempLineData->nextLine] = true;
//						pTempLineData = pLineMap->GetLineData( pTempLineData->nextLine );
//						pGroup->vecpLineData.push_back( pTempLineData );
//					}
//					else
//					{
//						break;
//					}				
//				}
//
//				pTempLineData = pLineData;
//				while( true )
//				{			
//					if( pTempLineData->beforeLine >= 0 && false == bDoneLineData[pTempLineData->beforeLine] )
//					{						
//						bDoneLineData[pTempLineData->beforeLine] = true;
//						pTempLineData = pLineMap->GetLineData( pTempLineData->beforeLine );
//						pGroup->vecpLineData.push_back( pTempLineData );
//					}
//					else
//					{
//						break;
//					}				
//				}
//
//				m_vecLineGroupList.push_back( pGroup );
//			}
//
//			m_mapLineData2LineGroup.clear();
//			int nLineGroups = (int)m_vecLineGroupList.size(); 
//			for( int i=0; i<nLineGroups; i++ )
//			{
//				CLineMapExt::LineGroup* pGroup = m_vecLineGroupList.at(i);
//				int nLineDataInANode = (int)pGroup->vecpLineData.size();
//
//				for(int j=0; j<nLineDataInANode; j++ )
//				{					
//					CKTDGLineMap::LineData* pLineData = pGroup->vecpLineData.at(j);
//					m_mapLineData2LineGroup.insert( std::make_pair(pLineData, pGroup) );
//				}
//			}
//
//			SAFE_DELETE_ARRAY(bDoneLineData);			
//		}
//
//
//	private:
//		vector<LineGroup*> m_vecLineGroupList;
//		map<CKTDGLineMap::LineData*, LineGroup*> m_mapLineData2LineGroup;	
//
//	}; // end of class CLineMapExt
//
//
//	//////////////////////////////////////////////////////////////////////////
//	// CTestAI::OnFrameMove() -- 코드랑 완전히 똑같지는 않지만 대략 이런식의 rule
//	// if( 점프상태 ) 
//	//   목적지까지 정해진 속도로 움직인다
//	// else
//	//   if( 최종목적지가 연결된 line위에 있다 ) 달려~
//	//   else
//	//     if( 최종목적지가 위쪽 && 수직 위로 점프가능 ) 수직 위로 점프
//	//     else if( 최종목적지가 아래쪽 && 수직 아래로 점프가능 ) 수직 아래로 점프
//	//     else      
//	//       if( 라인 끝에 도착 )
//	//         if( 대각선 점프 가능 ) 대각선 점프
//	//         else 오던 반대방향으로 걷기
//	//       else 계속 걷기
//	//
//	// todo: randomness 추가
//	//       못찾는 길 찾도록 고치기.
//	//       
//	//////////////////////////////////////////////////////////////////////////
//	class CTestAI
//	{	
//	public:
//		enum MOVESTATE
//		{
//			MS_WAIT,
//			MS_LEFT,
//			MS_RIGHT, 
//			MS_JUMP,
//			MS_DOWN,
//			MS_ATTACK,
//			MS_FORCE_LEFT,
//			MS_FORCE_RIGHT,
//		};
//
//	public:
//		~CTestAI()
//		{
//		}
//
//		CTestAI()
//		{
//			m_pPrevLineGroup = NULL;
//			m_pCurrLineGroup = NULL;
//
//			m_MoveState = MS_WAIT;
//		}
//
//		void OnFrameMove( D3DXVECTOR3& vCurrPos, int& iCurrLineDataIndex,
//			const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex, 
//			const float fMaxJumpUp, const float fMaxJumpRight,
//			CKTDGLineMap* pLineMap, CLineMapExt* pLineMapExt,
//			float fElapsedTime )
//		{
//			KTDXPROFILE();
//
//			const float MAGIC_MOVE_SPEED		= 500.f * fElapsedTime;
//			const float MAGIC_COLLISION_BOUND	= 30.f;
//			const float MAGIC_LINE_RADIUS		= 30.f;
//
//			CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
//			const CLineMapExt::LineGroup* pFinalDestLineGroup = pLineMapExt->GetLineGroupIncludesLineData( pFinalDestLineData );
//
//			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
//			float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
//
//			switch( m_MoveState )
//			{
//			case MS_JUMP:
//			case MS_DOWN:
//				{
//					D3DXVECTOR3 vDestDir = m_vDestPos - vCurrPos;
//					float fDestDist = D3DXVec3Length( &vDestDir );				
//					if( fDestDist < MAGIC_COLLISION_BOUND ) // 목적지에 도착
//					{
//						m_MoveState = MS_WAIT;
//						vCurrPos = m_vDestPos;
//						iCurrLineDataIndex = m_iDestLineDataIndex;
//
//						m_pPrevLineGroup = m_pCurrLineGroup;
//						m_pCurrLineGroup = pLineMapExt->GetLineGroupIncludesLineData( pLineMap->GetLineData(m_iDestLineDataIndex) );
//						return;
//					}
//					else
//					{
//						D3DXVec3Normalize( &vDestDir, &vDestDir );
//						vCurrPos += vDestDir * MAGIC_MOVE_SPEED;
//						return;
//					}
//				} break;
//
//			case MS_WAIT:
//			case MS_LEFT:
//			case MS_RIGHT:
//			case MS_FORCE_LEFT:
//			case MS_FORCE_RIGHT:
//				{
//					CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );
//					m_pCurrLineGroup = pLineMapExt->GetLineGroupIncludesLineData( pCurrLineData );
//
//					D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
//					float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
//					D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
//					const D3DXVECTOR3 vUpVec(0,1,0);
//					D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
//					D3DXVec3Normalize( &vRightVec, &vRightVec );
//
//					float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
//					float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
//
//					if( pFinalDestLineGroup == m_pCurrLineGroup ) // 같은 line group
//					{
//						if( fFinalDestDist < MAGIC_COLLISION_BOUND ) // 목적지에 도착
//						{
//							m_MoveState = MS_WAIT;
//							return;
//						}
//						else
//						{
//							// move left or right
//							if( fIsRight > 0.f )
//							{
//								m_MoveState = MS_RIGHT;
//								vCurrPos += vRightVec * MAGIC_MOVE_SPEED;							
//							}
//							else 
//							{
//								m_MoveState = MS_LEFT;
//								vCurrPos -= vRightVec * MAGIC_MOVE_SPEED;
//							}
//							goto COMPENSATION;
//						}		
//					} 
//					else // 같은 line group이 아니면
//					{
//						if( fIsUp > 0.f ) // 목적지가 위에 있는 경우
//						{
//							D3DXVECTOR3 vJumpPos = vCurrPos + vUpVec * fMaxJumpUp;
//							m_iDestLineDataIndex = iCurrLineDataIndex;
//							vJumpPos = pLineMap->GetLandPosition( vJumpPos, MAGIC_LINE_RADIUS, &m_iDestLineDataIndex );
//							if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
//							{
//								CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
//								CLineMapExt::LineGroup* pDestLineGroup = pLineMapExt->GetLineGroupIncludesLineData( pDestLineData );
//
//								if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
//								{
//									// 수직 위로 jump
//									m_MoveState = MS_JUMP;
//									m_vDestPos = vJumpPos;
//									return;
//								}
//							}
//						}
//						else // 목적지가 아래에 있는 경우
//						{
//							D3DXVECTOR3 vJumpPos = vCurrPos - vUpVec * MAGIC_LINE_RADIUS * 1.5f;
//							m_iDestLineDataIndex = iCurrLineDataIndex;
//							vJumpPos = pLineMap->GetLandPosition( vJumpPos, MAGIC_LINE_RADIUS, &m_iDestLineDataIndex );
//							if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
//							{
//								CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
//								CLineMapExt::LineGroup* pDestLineGroup = pLineMapExt->GetLineGroupIncludesLineData( pDestLineData );
//
//								if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
//								{
//									// 목적지 보다 높이가 낮으면 왠만하면 내려가지 않는다
//									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState || vJumpPos.y >= vFinalDestPos.y )
//									{
//										// 수직 아래로 down
//										m_MoveState = MS_DOWN;
//										m_vDestPos = vJumpPos;
//										return;
//									}
//								}
//							}
//						}
//
//						// 수직 위 아래로 jump 하거나 down 할 수 없으면 그냥 이동
//						if( MS_FORCE_LEFT != m_MoveState && (MS_FORCE_RIGHT == m_MoveState || fIsRight > 0.1f) ) // 목적지가 오른쪽 
//						{
//							if( pCurrLineData->nextLine < 0 ) 
//							{
//								D3DXVECTOR3 vLineEnd = pCurrLineData->endPos - vCurrPos;
//								float fLineEndDist = D3DXVec3Length( &vLineEnd );
//								if( fLineEndDist < MAGIC_COLLISION_BOUND ) // 오른쪽으로 갈 수 없으면
//								{
//									m_iDestLineDataIndex = iCurrLineDataIndex;
//									D3DXVECTOR3 vJumpPos;
//									for( int i=0; i<3; i++ )
//									{
//										vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
//										vJumpPos = pLineMap->GetLandPosition( vJumpPos, MAGIC_LINE_RADIUS, &m_iDestLineDataIndex );
//										if( m_iDestLineDataIndex != iCurrLineDataIndex )
//											break;
//									}								
//
//									if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
//									{
//										CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
//										CLineMapExt::LineGroup* pDestLineGroup = pLineMapExt->GetLineGroupIncludesLineData( pDestLineData );
//
//										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
//											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
//										{
//											// 대각선 오른쪽 jump
//											m_MoveState = MS_JUMP;
//											m_vDestPos = vJumpPos;
//											return;
//										}
//									}
//
//									// 오른쪽으로 갈 수도 없고 jump도 할 수 없으면 왼쪽으로 이동
//									{
//										switch( m_MoveState )
//										{									
//										case MS_LEFT:
//										case MS_FORCE_LEFT:
//											m_MoveState = MS_FORCE_RIGHT;
//											break;
//
//										case MS_WAIT:
//										case MS_RIGHT:
//										case MS_FORCE_RIGHT:
//											m_MoveState = MS_FORCE_LEFT;
//											break;
//										}
//									}
//								}							
//							}
//
//							// can go right
//							if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
//							{					
//								m_MoveState = MS_RIGHT;
//							}
//							vCurrPos += vRightVec * MAGIC_MOVE_SPEED;
//							goto COMPENSATION;
//						}
//						else if( MS_FORCE_RIGHT != m_MoveState && (MS_FORCE_LEFT == m_MoveState || fIsRight < -0.1f) ) // 목적지가 왼쪽
//						{
//							if( pCurrLineData->beforeLine < 0 ) 
//							{
//								D3DXVECTOR3 vLineStart = pCurrLineData->startPos - vCurrPos;
//								float fLineStartDist = D3DXVec3Length( &vLineStart );
//								if( fLineStartDist < MAGIC_COLLISION_BOUND ) // 왼쪽으로 갈 수 없으면
//								{
//									m_iDestLineDataIndex = iCurrLineDataIndex;
//									D3DXVECTOR3 vJumpPos;
//									for( int i=0; i<3; i++ )
//									{
//										vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);
//										vJumpPos = pLineMap->GetLandPosition( vJumpPos, MAGIC_LINE_RADIUS, &m_iDestLineDataIndex );
//										if( m_iDestLineDataIndex != iCurrLineDataIndex )
//											break;
//									}
//
//									if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
//									{
//										CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
//										CLineMapExt::LineGroup* pDestLineGroup = pLineMapExt->GetLineGroupIncludesLineData( pDestLineData );
//
//										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
//											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
//										{
//											// 대각선 왼쪽 jump
//											m_MoveState = MS_JUMP;
//											m_vDestPos = vJumpPos;
//											return;
//										}
//									}
//									// 왼쪽으로 갈 수도 없고 jump도 할 수 없으면 오른쪽으로 이동
//									{
//										switch( m_MoveState )
//										{
//										case MS_WAIT:
//										case MS_LEFT:
//										case MS_FORCE_LEFT:
//											m_MoveState = MS_FORCE_RIGHT;
//											break;
//
//										case MS_RIGHT:
//										case MS_FORCE_RIGHT:
//											m_MoveState = MS_FORCE_LEFT;
//											break;
//										}
//									}
//								}
//							}
//
//							// can go left
//							if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
//							{					
//								m_MoveState = MS_LEFT;
//							}
//
//							vCurrPos -= vRightVec * MAGIC_MOVE_SPEED;
//
//							goto COMPENSATION;
//						}
//						else // 목적지가 왼쪽도 오른쪽도 아닌 경우
//						{
//							// force move
//							if( MS_LEFT == m_MoveState )
//							{
//								m_MoveState = MS_FORCE_LEFT;
//							}
//							else if( MS_RIGHT == m_MoveState )
//							{
//								m_MoveState = MS_FORCE_RIGHT;
//							}
//							else if( MS_WAIT == m_MoveState )
//							{
//								m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
//							}
//							return;
//						}				
//					} 
//				}break;
//
//			} // end of switch( m_MoveState )
//
//
//COMPENSATION:
//			pLineMap->IsOnLineConnect( vCurrPos, &iCurrLineDataIndex, MAGIC_LINE_RADIUS, &vCurrPos );
//		} // end of OnFrameMove()
//
//	private:
//		CLineMapExt::LineGroup* m_pPrevLineGroup;
//		CLineMapExt::LineGroup* m_pCurrLineGroup;
//
//		MOVESTATE m_MoveState;
//
//		D3DXVECTOR3 m_vDestPos;
//		int m_iDestLineDataIndex;
//	}; // end of class CTestAI
//
//
//	//class CMonster : public CX2GUElsword_SwordMan
//	//{
//	//public:
//	//	virtual ~CMonster()
//	//	{			
//	//	}
//
//	//	CMonster( char inGameID, CX2Room::SlotData* pSlotData, int slotIndex, CX2Game* pGame, int frameBufferNum )
//	//		: CX2GUElsword_SwordMan( inGameID, pSlotData, slotIndex, pGame, frameBufferNum )
//	//	{			
//	//		m_bIsLineMapExtReady = false;
//	//	}
//
//	//	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime )
//	//	{
//	//		KTDXPROFILE();
//
//	//		//return true;
//
//
//	//		//m_InputData.Init();
//	//		//m_InputData.pureRight = rand()%2==0 ? true : false;
//	//		D3DXVECTOR3 vCurrPos = GetPos();
//	//		int iCurrLineDataIndex = m_FrameDataNow.syncData.lastTouchLineIndex;
//	//		D3DXVECTOR3 vFinalDestPos = g_pX2Game->GetMyUnit()->GetPos();
//	//		int iFinalDestLineDataIndex = g_pX2Game->GetMyUnit()->GetFrameData()->syncData.lastTouchLineIndex;
//
//	//		if( m_bIsLineMapExtReady == false )
//	//		{
//	//			m_LineMapExt.ReBuild( g_pX2Game->GetWorld()->GetLineMap() );
//	//			m_bIsLineMapExtReady = true;
//	//			CX2GUElsword_SwordMan::OnFrameMove( fTime, fElapsedTime );
//	//		}
//
//	//		m_TestAI.OnFrameMove( vCurrPos, iCurrLineDataIndex, vFinalDestPos, iFinalDestLineDataIndex, 270.f, 350.f, 
//	//			g_pX2Game->GetWorld()->GetLineMap(), &m_LineMapExt, fElapsedTime );
//
//	//		m_FrameDataNow.syncData.position = vCurrPos;	
//	//		m_FrameDataNow.syncData.lastTouchLineIndex = iCurrLineDataIndex;			
//	//		m_FrameDataFuture.syncData.position = vCurrPos;	
//	//		m_FrameDataFuture.syncData.lastTouchLineIndex = iCurrLineDataIndex;
//	//		
//	//		//return CX2GUElsword_SwordMan::OnFrameMove( fTime, fElapsedTime );
//	//		return true;
//	//	}
//
//	//private:
//	//	CTestAI m_TestAI;
//	//	CLineMapExt m_LineMapExt;
//	//	bool m_bIsLineMapExtReady;
//	//};	
//
//	// 현재 CX2Dungeon은 그냥 하는 일 없이 DungeonData만 하나 가지고 있는다. 
//	// CX2World와 모양을 같이 하려고 이렇게 했음.
//	
//	class CSoloGame : public CX2PVPGame
//	{
//	public:
//		virtual ~CSoloGame(void) 
//		{
//		}
//
//		CSoloGame(void)
//		{
//		}
//
//		virtual HRESULT OnLostDevice()
//		{
//			HRESULT hr = S_OK;
//			hr = CX2PVPGame::OnLostDevice();
//			return hr;
//		}
//
//		virtual HRESULT OnResetDevice()
//		{
//			HRESULT hr = S_OK;
//			hr = CX2PVPGame::OnResetDevice();
//			return hr;
//		}
//
//		virtual HRESULT OnFrameMove( double fTime, float fElapsedTime ) 
//		{
//			HRESULT hr = S_OK;
//			hr = CX2PVPGame::OnFrameMove(fTime, fElapsedTime);
//			return hr;		
//		}
//
//		virtual HRESULT OnFrameRender() 
//		{
//			HRESULT hr = S_OK;
//			hr = CX2PVPGame::OnFrameRender();
//			return hr;
//		}
//
//		virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//		{
//			bool bNoFurtherProcessing = false;
//
//			switch( wParam )
//			{
//			case CX2Packet::XPT_UNIT_FULL_SYNC:
//				return P2PUnitFullSync( hWnd, uMsg, wParam, lParam );
//				break;
//			}
//
//			return bNoFurtherProcessing;
//		}
//
//		void SetWorld( CX2World* pWorld ) { m_pWorld = pWorld; }
//
//		int HostReady( int myUID )
//		{
//			//네트워크 만들기
//			m_pDP		= new CKTDNDP();
//			m_pTransfer = new CKTDNTransfer();
//
//			m_pDP->InitDirectPlay( m_pTransfer );
//			m_pTransfer->SetNetwork( m_pDP );
//
//			//m_pTransfer->SetNetwork( CKTDNTransfer::NT_TCP_RELAY );
//			int port = g_pMain->GetGameP2PPort();
//			m_pDP->OpenHost( L"X2GAME", myUID, g_pMain->GetGameP2PPort(), 8 );
//
//			return port;
//		}
//
//	protected:
//		bool P2PUnitFullSync( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//		{
//			if( (long)lParam < 0 ) 
//				return false;
//
//			CKTDNPacket::PacketData* pPacketData = (CKTDNPacket::PacketData*)lParam;
//			if( NULL == pPacketData->pPacket || pPacketData->pPacket <= 0 )
//				return false;
//
//			CX2Packet::SXPT_UNIT_FULL_SYNC* pSXPT_UNIT_FULL_SYNC = (CX2Packet::SXPT_UNIT_FULL_SYNC*)pPacketData->pPacket;
//			if( NULL == pSXPT_UNIT_FULL_SYNC )
//				return false;
//
//			//CX2GameUnit* pCX2GameUnit = GetUnitByUID( pSXPT_UNIT_FULL_SYNC->unitUID );
//			//if ( pCX2GameUnit != NULL )
//			//	pCX2GameUnit->ReceiveFrameData( pPacketData );
//
//			CX2GameUnit* pCX2GameUnit = m_UserUnitList[pSXPT_UNIT_FULL_SYNC->unitIndex];
//			if ( pCX2GameUnit != NULL )
//				pCX2GameUnit->ReceiveFrameData( pPacketData );
//
//			SAFE_DELETE( pPacketData );
//			return true;
//		}
//
//	private:
//
//	}; // end of CSoloGame
//
//
//
//
//	////////////////////////////////////////////////////////////////////////////
//	//// CX2DungeonRoom
//	////////////////////////////////////////////////////////////////////////////
//	//class CX2DungeonRoom : public CX2Room
//	//{
//	//public:
//	//	enum DUNGEON_GAME_TYPE
//	//	{
//	//		DGT_TEAM = 0,
//	//		DGT_SURVIVAL,
//	//		DGT_TEAM_DEATH_MATCH,
//	//		DGT_SURVIVAL_DEATH_MATCH,
//	//	};
//
//	//	struct DungeonRoomSimpleData : public RoomSimpleData
//	//	{
//	//		DUNGEON_GAME_TYPE			gameType;
//
//	//		int							winGameNum;
//	//		int							winKillNum;
//
//	//		bool						bPossibleIntrude;
//	//		bool						bIsOpen;
//
//	//		DungeonRoomSimpleData()
//	//		{
//	//			gameType = DGT_TEAM;
//
//	//			winGameNum = 0;
//	//			winKillNum = 0;
//
//	//			bPossibleIntrude = false;
//	//			bIsOpen = true;
//	//		}
//
//	//		virtual void CopyData( const KEVENT_X2_SERVER_ROOM_SIMPLE_INFO& data )
//	//		{
//	//			RoomSimpleData::CopyData( data );
//
//	//			winGameNum = data.winGameNum;
//	//			winKillNum = data.winKillNum;
//
//	//			bPossibleIntrude = data.bPossibleIntrude;
//	//			bIsOpen			 = data.bIsOpen;
//
//	//			gameType = (DUNGEON_GAME_TYPE)data.gameType;
//	//		}
//	//	};
//
//	//	struct DungeonRoomData : public RoomData
//	//	{
//	//		DUNGEON_GAME_TYPE			gameType;
//
//	//		int							winGameNum;
//	//		int							winKillNum;
//
//	//		bool						bPossibleIntrude;
//	//		bool						bIsOpen;
//
//	//		DungeonRoomData()
//	//		{
//	//			gameType	= DGT_TEAM;
//
//	//			winGameNum	= 0;
//	//			winKillNum	= 0;
//
//	//			bPossibleIntrude = false;
//	//			bIsOpen = true;
//	//		}
//	//		void CopyData( const KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK& data )
//	//		{
//	//			roomNum			= data.roomInfo.roomNum;
//	//			roomName		= data.roomInfo.roomName;
//
//	//			roomState		= (ROOM_STATE)data.roomInfo.roomState;
//	//			gameType		= (DUNGEON_GAME_TYPE)data.roomInfo.gameType;
//
//	//			winGameNum		= data.roomInfo.winGameNum;
//	//			winKillNum		= data.roomInfo.winKillNum;
//
//	//			bPossibleIntrude= data.roomInfo.bPossibleIntrude;
//	//			bIsOpen			= data.roomInfo.bIsOpen;
//
//	//			worldID			= (CX2World::WORLD_ID)data.roomInfo.worldID;
//
//	//			//삭제
//	//			for( int i = 0; i < (int)slotDataList.size(); i++ )
//	//			{
//	//				SlotData* pSlotData = slotDataList[i];
//	//				SAFE_DELETE( pSlotData );
//	//			}
//	//			slotDataList.clear();
//
//	//			for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
//	//			{
//	//				KEVENT_X2_SERVER_SLOT_INFO slotInfo = data.slotInfoList[i];
//	//				SlotData* pSlotData = new SlotData();
//	//				pSlotData->CopyData( slotInfo );
//
//	//				slotDataList.push_back( pSlotData );
//	//			}
//	//		}
//	//		void CopyData( const KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK& data )
//	//		{
//	//			roomNum			= data.roomInfo.roomNum;
//	//			roomName		= data.roomInfo.roomName;
//
//	//			roomState		= (ROOM_STATE)data.roomInfo.roomState;
//	//			gameType		= (DUNGEON_GAME_TYPE)data.roomInfo.gameType;
//
//	//			winGameNum		= data.roomInfo.winGameNum;
//	//			winKillNum		= data.roomInfo.winKillNum;
//
//	//			bPossibleIntrude= data.roomInfo.bPossibleIntrude;
//	//			bIsOpen			= data.roomInfo.bIsOpen;
//
//	//			worldID			= (CX2World::WORLD_ID)data.roomInfo.worldID;
//
//	//			//삭제
//	//			for( int i = 0; i < (int)slotDataList.size(); i++ )
//	//			{
//	//				SlotData* pSlotData = slotDataList[i];
//	//				SAFE_DELETE( pSlotData );
//	//			}
//	//			slotDataList.clear();
//
//	//			for( int i = 0; i < (int)data.slotInfoList.size(); i++ )
//	//			{
//	//				KEVENT_X2_SERVER_SLOT_INFO slotInfo = data.slotInfoList[i];
//	//				SlotData* pSlotData = new SlotData();
//	//				pSlotData->CopyData( slotInfo );
//
//	//				slotDataList.push_back( pSlotData );
//	//			}
//	//		}
//	//		void CopyData( const KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_INFO_NOT& data )
//	//		{
//	//			roomNum			= data.roomInfo.roomNum;
//	//			roomName		= data.roomInfo.roomName;
//
//	//			roomState		= (ROOM_STATE)data.roomInfo.roomState;
//	//			gameType		= (DUNGEON_GAME_TYPE)data.roomInfo.gameType;
//
//	//			winGameNum		= data.roomInfo.winGameNum;
//	//			winKillNum		= data.roomInfo.winKillNum;
//
//	//			bPossibleIntrude= data.roomInfo.bPossibleIntrude;
//	//			bIsOpen			= data.roomInfo.bIsOpen;
//
//	//			worldID			= (CX2World::WORLD_ID)data.roomInfo.worldID;
//	//		}
//	//		void CopyData( const KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_SLOT_INFO_LIST_NOT& data )
//	//		{
//	//			for( int i = 0; i < (int)data.slotIndex.size(); i++ )
//	//			{
//	//				int index = data.slotIndex[i];
//	//				KEVENT_X2_SERVER_SLOT_INFO slotInfo = data.slotInfoList[i];
//	//				if( SE_NO_CHANGE == (SLOT_EVENT)slotInfo.slotEvent )
//	//					continue;
//
//	//				SlotData* pSlotData = slotDataList[index];
//
//	//				if ( slotInfo.slotEvent == (SLOT_EVENT)SE_EXIT_ROOM )
//	//				{
//	//					SlotData* pNewSlotData = new SlotData();						
//	//					SAFE_DELETE( pSlotData );
//	//					slotDataList[index] = pNewSlotData;
//	//				}
//	//				else if ( slotInfo.slotEvent == (SLOT_EVENT)SE_CHANGE )
//	//				{
//
//	//					if ( pSlotData->pUnit != NULL && pSlotData->pUnit->GetUID() == slotInfo.unitInfo.unitUID )
//	//					{
//	//						pSlotData->slotEvent = (SLOT_EVENT)slotInfo.slotEvent;
//	//						pSlotData->slotState = (SLOT_STATE)slotInfo.slotState;
//	//						pSlotData->teamNum = (TEAM_NUM)slotInfo.teamNum;
//	//						pSlotData->slotUnitSpecialMotion = (SLOT_UNIT_SPECIAL_MOTION)slotInfo.slotUnitSpecialMotion;
//	//					}
//	//					else
//	//					{
//	//						SlotData* pNewSlotData = new SlotData();
//	//						pNewSlotData->CopyData( slotInfo );
//
//	//						SAFE_DELETE( pSlotData );
//	//						slotDataList[index] = pNewSlotData;
//	//					}
//	//				}
//	//				else
//	//				{
//	//					SlotData* pNewSlotData = new SlotData();
//	//					pNewSlotData->CopyData( slotInfo );
//
//	//					SAFE_DELETE( pSlotData );
//	//					slotDataList[index] = pNewSlotData;
//	//				}
//	//			}
//	//		}
//	//	};
//
//	//public:
//	//	virtual CX2DungeonRoom::~CX2DungeonRoom()
//	//	{
//	//	}
//
//	//	CX2DungeonRoom::CX2DungeonRoom( DungeonRoomData* pDungeonRoomData )
//	//		: CX2Room( pDungeonRoomData )
//	//	{
//	//		m_pDungeonRoomData = pDungeonRoomData;
//
//	//		for( int i = 0; i < (int)m_pDungeonRoomData->slotDataList.size(); i++ )
//	//		{
//	//			SlotData* pSlotData = m_pDungeonRoomData->slotDataList[i];
//
//	//			if( NULL != pSlotData->pUnit )
//	//			{
//	//				if( pSlotData->pUnit->GetOwnerUserUID() == g_pData->GetMyUser()->GetUID() )
//	//				{
//	//					m_pMySlot = pSlotData;
//	//				}
//
//	//				if( SS_JOIN_HOST == pSlotData->slotState )
//	//				{
//	//					m_pHostSlot = pSlotData;
//	//				}
//	//			}
//	//		}
//	//	}
//
//	//	int					GetWinGameNum(){ return m_pDungeonRoomData->winGameNum; }
//	//	int					GetWinKillNum(){ return m_pDungeonRoomData->winKillNum; }
//
//	//	bool				GetIsOpenRoom() { return m_pDungeonRoomData->bIsOpen; }
//	//	bool				GetIsPossibleIntrude() { return m_pDungeonRoomData->bPossibleIntrude; }		
//
//	//	DUNGEON_GAME_TYPE	GetGameType() { return m_pDungeonRoomData->gameType; }
//
//	//	//void				SetRoomInfo( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_INFO_NOT& kEvent );
//	//	//void				SetSlotInfo( KEVENT_X2_SERVER_BROADCAST_DUNGEON_ROOM_SLOT_INFO_LIST_NOT& kEvent );
//	//	//void				SetSlotInfo( KEVENT_X2_SERVER_BROADCAST_STATE_CHANGE_DUNGEON_GAME_RESULT_NOT& kEvent );
//
//	//private:
//	//	DungeonRoomData*	m_pDungeonRoomData;
//	//};
//
//	//////////////////////////////////////////////////////////////////////////
//	// CX2DungeonGame
//	//////////////////////////////////////////////////////////////////////////
//	//class CX2DungeonGame : public CX2Game
//	//{
//	//public:
//	//	//CX2DungeonGame(void);
//	//	//virtual ~CX2DungeonGame(void);
//
//	//	//virtual void			GameLoading( CX2Room* pRoom );
//
//	//	//virtual HRESULT			OnResetDevice();
//	//	//virtual HRESULT			OnLostDevice();
//
//	//	//virtual bool			MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	virtual bool			DetermineGameResult( CX2Room::TEAM_NUM* pWinTeam = NULL ) { return false; };
//	//	virtual bool			DetermineLastKill() { return false; };
//
//	//	virtual CX2DungeonGame::~CX2DungeonGame(void)
//	//	{
//	//	}
//
//	//	CX2DungeonGame::CX2DungeonGame(void)
//	//	{
//	//		m_pDungeonRoom = NULL;
//	//	}
//
//	//	virtual void CX2DungeonGame::GameLoading( CX2Room* pRoom )
//	//	{
//	//		m_GameState = GS_LOADING;
//	//		CX2Game::GameLoading( pRoom );
//	//		m_GameState = GS_LOADING;
//
//	//		m_pDungeonRoom = (CX2DungeonRoom*)pRoom;
//	//		AddNPCUnit();
//
//	//		m_GameState = GS_READY;	
//	//	}
//
//	//	virtual HRESULT CX2DungeonGame::OnResetDevice()
//	//	{
//	//		CX2Game::OnResetDevice();
//	//		return S_OK;
//	//	}
//
//	//	virtual HRESULT CX2DungeonGame::OnLostDevice()
//	//	{
//	//		CX2Game::OnLostDevice();
//	//		return S_OK;
//	//	}
//
//	//	virtual bool CX2DungeonGame::MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//	{
//	//		switch( wParam )
//	//		{
//	//		case CX2Packet::XPT_UNIT_FULL_SYNC:
//	//			return P2PUnitFullSync( hWnd, uMsg, wParam, lParam );
//	//			break;
//	//		}
//
//	//		return false;
//	//	}
//
//	//	virtual int HostReady( int myUID )
//	//	{
//	//		//네트워크 만들기
//	//		m_pDP		= new CKTDNDP();
//	//		m_pTransfer = new CKTDNTransfer();
//
//	//		m_pDP->InitDirectPlay( m_pTransfer );
//	//		m_pTransfer->SetNetwork( m_pDP );
//
//	//		//m_pTransfer->SetNetwork( CKTDNTransfer::NT_TCP_RELAY );
//	//		int port = g_pMain->GetGameP2PPort();
//	//		m_pDP->OpenHost( L"X2GAME", myUID, g_pMain->GetGameP2PPort(), 8 );
//
//	//		return port;
//	//	}
//
//	//	//CX2GUNPC* CreateNPC( CX2UnitManager::NPCUnitID npcID, CX2UnitManager::NPC_CLASS_TYPE classType );
//
//	//protected:
//	//	bool CX2DungeonGame::P2PUnitFullSync( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//	{
//	//		if( (long)lParam < 0 ) 
//	//			return false;
//
//	//		CKTDNPacket::PacketData* pPacketData = (CKTDNPacket::PacketData*)lParam;
//	//		if( NULL == pPacketData->pPacket || pPacketData->pPacket <= 0 )
//	//			return false;
//
//	//		CX2Packet::SXPT_UNIT_FULL_SYNC* pSXPT_UNIT_FULL_SYNC = (CX2Packet::SXPT_UNIT_FULL_SYNC*)pPacketData->pPacket;
//	//		if( NULL == pSXPT_UNIT_FULL_SYNC )
//	//			return false;
//
//	//		//CX2GameUnit* pCX2GameUnit = GetUnitByUID( pSXPT_UNIT_FULL_SYNC->unitUID );
//	//		//if ( pCX2GameUnit != NULL )
//	//		//	pCX2GameUnit->ReceiveFrameData( pPacketData );
//
//	//		CX2GameUnit* pCX2GameUnit = m_UserUnitList[pSXPT_UNIT_FULL_SYNC->unitIndex];
//	//		if ( pCX2GameUnit != NULL )
//	//			pCX2GameUnit->ReceiveFrameData( pPacketData );
//
//	//		SAFE_DELETE( pPacketData );
//
//	//		return true;
//	//	}
//
//	//	void CX2DungeonGame::AddNPCUnit()
//	//	{
//	//		if( NULL == m_pWorld )
//	//			return;
//	//	
//	//		CX2GameUnit* pCX2GameUnit = NULL;
//	//		vector<CX2World::NPCData*>& NPCDataList = m_pWorld->GetNPCDataList();
//	//		for( unsigned i=0; i<NPCDataList.size(); i++ )
//	//		{
//	//			
//	//			const int MAGIC_USER_UID = 777;
//	//			const int MAGIC_UNIT_UID = 777;
//
//	//			CX2Unit::UnitData* pUnitData = new CX2Unit::UnitData();
//	//			{		
//	//				pUnitData->userUID		= MAGIC_USER_UID;
//	//				pUnitData->unitUID		= m_NextInGameID;
//	//				pUnitData->unitClass	= CX2Unit::UC_ELSWORD_SWORDMAN;
//	//				pUnitData->IP			= L"0.0.0.0"; // L"127.0.0.1";
//	//				pUnitData->nickName		= L"*^^*♡";
//	//				pUnitData->GP			= 0;
//	//				pUnitData->level		= 1;
//	//				pUnitData->EXP			= 0;
//	//			}
//	//			CX2Unit* pUnit	= new CX2Unit( pUnitData );
//	//			
//	//			CX2Room::SlotData* pSlotData	= new CX2Room::SlotData();
//	//			pSlotData->slotState			= CX2Room::SS_JOIN_READY;
//	//			pSlotData->pUnit				= pUnit;
//
//	//			//pCX2GameUnit = new CX2GUElsword_SwordMan( m_NextInGameID, pSlotData, i, this, g_pMain->GetFrameBufferNum() );
//
//
//	//			//pCX2GameUnit = new CMonster( m_NextInGameID, pSlotData, i, this, g_pMain->GetFrameBufferNum() );
//	//			//m_UnitList.push_back( pCX2GameUnit );
//	//			//m_NPCUnitList.push_back( pCX2GameUnit );
//	//			//g_pKTDXApp->GetDGManager()->AddObjectChain( pCX2GameUnit );		
//	//			
//	//			m_NextInGameID++;
//	//		}	
//	//	}
//
//	//private:
//	//	CX2DungeonRoom*			m_pDungeonRoom;
//	//};
//
//
//
//	//class CX2StateDungeonLobby : public CX2State
//	//{
//	//public:
//	//	enum DUNGEON_LOBBY_UI_CUSTOM_MSG
//	//	{
//	//		DLUCM_OPEN_CREATE_ROOM_DLG = 0,
//	//		DLUCM_JOIN_ROOM,
//	//		DLUCM_ROOM_BACK_PAGE,
//	//		DLUCM_ROOM_NEXT_PAGE,
//	//		DLUCM_BACK_TO_MAIN_LOBBY,
//
//	//		DLUCM_CREATE_ROOM_OK,
//	//		DLUCM_CREATE_ROOM_CANCEL,
//
//	//		DLUCM_ROOM_OPEN_OPTION_CHANGED,
//
//	//		DLUCM_ROOM_PASSWORD_CHECK_OK,
//	//		DLUCM_ROOM_PASSWORD_CHECK_CANCEL,
//	//	};
//
//	//public:
//	//	CX2StateDungeonLobby(void);
//	//	virtual ~CX2StateDungeonLobby(void);
//
//	//	HRESULT OnFrameMove(double fTime, float fElapsedTime );
//
//	//protected:
//	//	virtual bool UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	virtual bool UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	virtual bool UIServerTimeOutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	//	bool DungeonRoomSimpleInfoListReq( int pageNum );
//	//	bool DungeonRoomSimpleInfoListAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	bool DungeonRoomSimpleInfoListTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	bool DungeonRoomSimpleInfoListNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	//	bool CreateDungeonRoomReq();
//	//	bool CreateDungeonRoomAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	bool CreateDungeonRoomTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	//	bool JoinDungeonRoomReq( int roomNum );
//	//	bool JoinDungeonRoomAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	bool JoinDungeonRoomTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	//	bool StateChangeMainLobbyReq();
//	//	bool StateChangeMainLobbyAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//	//	bool StateChangeMainLobbyTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	//	void ResetRoomListUI();
//	//	void OpenCreateRoomDLG();
//	//	void BuildRoomData( KEVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK& kEvent );
//	//	void BuildRoomData( KEVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK& kEvent );
//
//	//private:
//	//	void RoomOpenOptionChanged();
//	//	void OpenDLGJoinPasswordRoom();
//
//	//private:
//	//	CKTDGUIDialogType		m_pDLGDungeonLobbyBack;
//	//	CKTDGUIDialogType		m_pDLGDungeonLobbyFront;
//	//	CKTDGUIDialogType		m_pDLGMsgBox;
//
//	//	CKTDGUIDialogType		m_pDLGCreateDungeonRoom;
//
//	//	CKTDGUIDialogType		m_pDLGJoinPasswordRoom;
//
//	//	vector<CKTDGUIDialogType> m_DLGDungeonLobbyRoomList;
//
//	//	int					m_NowPage;
//	//	int					m_EndPage;
//
//	//	wstring				m_Password;
//	//	int					m_JoinRoomNum;
//
//	//	vector<CX2DungeonRoom::DungeonRoomSimpleData*> m_RoomList;
//	//};
//
//	//CX2StateDungeonLobby::~CX2StateDungeonLobby()
//	//{
//	//	SAFE_DELETE_DIALOG( m_pDLGDungeonLobbyBack );
//	//	SAFE_DELETE_DIALOG( m_pDLGDungeonLobbyFront );
//	//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	//	SAFE_DELETE_DIALOG( m_pDLGCreateDungeonRoom );
//	//	SAFE_DELETE_DIALOG( m_pDLGJoinPasswordRoom );
//
//	//	for( int i = 0; i < (int)m_RoomList.size(); i++ )
//	//	{			
//	//		//CX2PVPRoom::PVPRoomSimpleData* pRoomData = m_RoomList[i];
//	//		CX2DungeonRoom::DungeonRoomSimpleData* pRoomData = m_RoomList[i];
//	//		SAFE_DELETE( pRoomData );
//	//	}
//	//	m_RoomList.clear();
//
//	//	for ( int i = 0; i < (int)m_DLGDungeonLobbyRoomList.size(); i++ )
//	//	{
//	//		CKTDGUIDialogType pDialog = NULL;
//	//		pDialog = m_DLGDungeonLobbyRoomList[i];
//	//		SAFE_DELETE_DIALOG( pDialog )
//	//	}
//	//	m_DLGDungeonLobbyRoomList.clear();
//	//}
//
//	//CX2StateDungeonLobby::CX2StateDungeonLobby()
//	//{
//	//	m_pDLGDungeonLobbyBack = NULL;
//	//	m_pDLGDungeonLobbyFront = NULL;
//
//	//	m_pDLGDungeonLobbyBack = new CKTDGUIDialog( this, L"DLG_PVP_Lobby_Back.lua" );
//	//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDungeonLobbyBack );
//	//	
//	//	m_pDLGDungeonLobbyFront = new CKTDGUIDialog( this, L"DLG_PVP_Lobby_Front.lua" );
//	//	g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( m_pDLGDungeonLobbyFront );
//
//	//	for ( int i = 0; i < 6; i++ )
//	//	{
//	//		CKTDGUIDialogType pDLGDungeonLobbyRoom = new CKTDGUIDialog( this, L"DLG_PVP_Lobby_Room_Button.lua" );
//	//		g_pKTDXApp->GetDGManager()->GetDialogManager()->AddDlg( pDLGDungeonLobbyRoom );
//	//		m_DLGDungeonLobbyRoomList.push_back( pDLGDungeonLobbyRoom );
//	//	}
//
//	//	m_pDLGMsgBox			= NULL;
//	//	m_pDLGCreateDungeonRoom	= NULL;
//
//	//	m_NowPage		= 1;
//	//	m_EndPage		= 1;
//
//	//	m_Password		= L"";
//
//	//	m_pDLGJoinPasswordRoom = NULL;
//
//	//	TextureReady( L"DLG_Common3.tga" );
//	//	TextureReady( L"DLG_CreateRoom0.tga" );
//	//	DungeonRoomSimpleInfoListReq( m_NowPage );
//
//	//	m_JoinRoomNum = -1;
//	//}
//
//	//HRESULT CX2StateDungeonLobby::OnFrameMove(double fTime, float fElapsedTime )
//	//{
//	//	CX2State::OnFrameMove( fTime, fElapsedTime );
//
//	//	if ( m_pDLGCreateDungeonRoom != NULL )
//	//	{
//	//		CKTDGUIRadioButton* pTNMRadioButton = (CKTDGUIRadioButton*)m_pDLGCreateDungeonRoom->GetControl(L"CreateRoomTeamNormalMatch");
//	//		CKTDGUIRadioButton* pSNMRadioButton = (CKTDGUIRadioButton*)m_pDLGCreateDungeonRoom->GetControl(L"CreateRoomSurvivalNormalMatch");
//	//		CKTDGUIRadioButton* pSDMRadioButton = (CKTDGUIRadioButton*)m_pDLGCreateDungeonRoom->GetControl(L"CreateRoomSurvivalDeathMatch");
//	//		CKTDGUIRadioButton* pTDMRadioButton = (CKTDGUIRadioButton*)m_pDLGCreateDungeonRoom->GetControl(L"CreateRoomTeamDeathMatch");
//
//	//		CKTDGUIStatic*	pStaticRoundOrKillPicture = (CKTDGUIStatic*)m_pDLGCreateDungeonRoom->GetControl(L"StaticCreateRoomGameModeText");
//	//		CKTDGUIStatic*	pStaticArrowPicture = (CKTDGUIStatic*)m_pDLGCreateDungeonRoom->GetControl(L"StaticCreateRoomArrow");
//
//	//		if ( pTNMRadioButton->GetChecked() == true || pSNMRadioButton->GetChecked() == true )
//	//		{
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomKillNum")->SetEnable( false );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomKillNum")->SetShow( false );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomMatchNum")->SetEnable( true );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomMatchNum")->SetShow( true );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"RadioButtonCreateRoomintrudeOK")->SetShow( false );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"RadioButtonCreateRoomintrudeNO")->SetShow( false );
//
//	//			pStaticRoundOrKillPicture->GetPicture(0)->SetShow(true);
//	//			pStaticRoundOrKillPicture->GetPicture(1)->SetShow(true);
//	//			pStaticRoundOrKillPicture->GetPicture(2)->SetShow(false);
//	//			pStaticRoundOrKillPicture->GetPicture(3)->SetShow(false);
//	//			pStaticRoundOrKillPicture->GetPicture(4)->SetShow(false);
//	//			pStaticRoundOrKillPicture->GetPicture(5)->SetShow(false);
//
//	//			pStaticArrowPicture->GetPicture(0)->SetShow(true);
//	//			pStaticArrowPicture->GetPicture(1)->SetShow(false);
//	//			pStaticArrowPicture->GetPicture(2)->SetShow(false);
//	//		}
//	//		else
//	//		{
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomKillNum")->SetEnable( true );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomKillNum")->SetShow( true );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomMatchNum")->SetEnable( false );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"ComboBoxCreateRoomMatchNum")->SetShow( false );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"RadioButtonCreateRoomintrudeOK")->SetShow( true );
//	//			m_pDLGCreateDungeonRoom->GetControl(L"RadioButtonCreateRoomintrudeNO")->SetShow( true );
//
//	//			pStaticRoundOrKillPicture->GetPicture(0)->SetShow(false);
//	//			pStaticRoundOrKillPicture->GetPicture(1)->SetShow(false);
//	//			pStaticRoundOrKillPicture->GetPicture(2)->SetShow(true);
//	//			pStaticRoundOrKillPicture->GetPicture(3)->SetShow(true);
//	//			pStaticRoundOrKillPicture->GetPicture(4)->SetShow(true);
//	//			pStaticRoundOrKillPicture->GetPicture(5)->SetShow(true);
//
//	//			pStaticArrowPicture->GetPicture(0)->SetShow(false);
//	//			pStaticArrowPicture->GetPicture(1)->SetShow(true);
//	//			pStaticArrowPicture->GetPicture(2)->SetShow(true);
//	//		}
//	//	}
//
//	//	return S_OK;
//	//}
//
//	//bool CX2StateDungeonLobby::UICustomEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//{
//	//	switch(wParam)
//	//	{
//	//	case DLUCM_OPEN_CREATE_ROOM_DLG:
//	//		OpenCreateRoomDLG();
//	//		break;
//
//	//	case DLUCM_JOIN_ROOM:
//	//		{
//	//			CKTDGUIButton* pJoinButton = (CKTDGUIButton*)lParam;
//	//			CKTDGUIDialogType pDialog = pJoinButton->GetDialog();
//
//	//			int roomNum = -1;
//
//	//			WCHAR tempSTR[10] = {0};
//	//			CKTDGUIStatic* pStaticRoomNum = (CKTDGUIStatic*)pDialog->GetControl( L"StaticRoomNumber" );
//
//	//			roomNum = _wtoi( pStaticRoomNum->GetString(0)->msg.c_str() );
//	//			m_JoinRoomNum = roomNum;
//
//	//			if ( roomNum != -1 )
//	//			{
//	//				for ( int i = 0; i < (int)m_RoomList.size(); i++ )
//	//				{
//	//					//CX2PVPRoom::PVPRoomSimpleData* pTempData = m_RoomList[i];
//	//					CX2DungeonRoom::DungeonRoomSimpleData* pTempData = m_RoomList[i];
//	//					if ( pTempData->roomNum == roomNum )
//	//					{
//	//						if ( pTempData->bIsOpen == false )
//	//							OpenDLGJoinPasswordRoom();
//	//						else
//	//							JoinDungeonRoomReq( roomNum );
//	//					}
//	//				}	
//	//			}
//	//			else
//	//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"입장할 수 없습니다.", this );
//
//	//			break;
//	//		}
//
//	//	case DLUCM_ROOM_BACK_PAGE:
//	//		{
//	//			if( m_NowPage <= 1 )
//	//				return true;
//
//	//			m_NowPage--;
//	//			DungeonRoomSimpleInfoListReq( m_NowPage );
//	//		}
//	//		break;
//
//	//	case DLUCM_ROOM_NEXT_PAGE:
//	//		{
//	//			if( m_NowPage >= m_EndPage )
//	//				return true;
//
//	//			m_NowPage++;
//	//			DungeonRoomSimpleInfoListReq( m_NowPage );
//	//		}
//	//		break;
//
//	//	case DLUCM_BACK_TO_MAIN_LOBBY:
//	//		return StateChangeMainLobbyReq();
//	//		break;
//
//	//	case DLUCM_CREATE_ROOM_OK:
//	//		{
//	//			return CreateDungeonRoomReq();
//	//		}
//	//		break;
//
//	//	case DLUCM_CREATE_ROOM_CANCEL:
//	//		{
//	//			//g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGCreatePVPRoom, NULL, false );
//	//			m_pDLGCreateDungeonRoom->Move( D3DXVECTOR2(0,0), D3DXCOLOR(1,1,1,0), 0.1f, false, true );
//	//			m_pDLGCreateDungeonRoom = NULL;
//	//		}
//	//		break;
//
//	//	case DLUCM_ROOM_OPEN_OPTION_CHANGED:
//	//		{
//	//			RoomOpenOptionChanged();
//	//		}
//	//		break;
//
//	//	case DLUCM_ROOM_PASSWORD_CHECK_OK:
//	//		{
//	//			CKTDGUIEditBox* pTempPasswordEditBox = (CKTDGUIEditBox*)m_pDLGJoinPasswordRoom->GetControl(L"EditBoxPassword");
//	//			m_Password = pTempPasswordEditBox->GetText();
//	//			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGJoinPasswordRoom, NULL, false );
//	//			m_pDLGJoinPasswordRoom = NULL;
//	//			JoinDungeonRoomReq( m_JoinRoomNum );
//	//		}
//	//		break;
//
//	//	case DLUCM_ROOM_PASSWORD_CHECK_CANCEL:
//	//		{
//	//			g_pKTDXApp->SendGameDlgMessage( XGM_DELETE_DIALOG, m_pDLGJoinPasswordRoom, NULL, false );
//	//			m_pDLGJoinPasswordRoom = NULL;
//	//		}
//	//		break;
//	//	}
//
//	//	return true;
//	//}
//
//	//bool CX2StateDungeonLobby::UIServerEventProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//{
//	//	switch(wParam)
//	//	{
//	//	case EVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK:
//	//		return DungeonRoomSimpleInfoListAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//
//	//	case EVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT:
//	//		return DungeonRoomSimpleInfoListNot( hWnd, uMsg, wParam, lParam );
//	//		break;
//
//	//	case EVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK:
//	//		return CreateDungeonRoomAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//
//	//	case EVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK:
//	//		return JoinDungeonRoomAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//
//	//	case EVENT_X2_SERVER_STATE_CHANGE_MAIN_LOBBY_ACK:
//	//		return StateChangeMainLobbyAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//	//	}
//
//	//	return false;
//	//}
//
//	//bool CX2StateDungeonLobby::UIServerTimeOutProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//{
//	//	switch(wParam)
//	//	{
//	//	case EVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK:
//	//		return DungeonRoomSimpleInfoListTimeOutAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//
//	//	case EVENT_X2_SERVER_CREATE_DUNGEON_ROOM_ACK:
//	//		return CreateDungeonRoomTimeOutAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//
//	//	case EVENT_X2_SERVER_JOIN_DUNGEON_ROOM_ACK:
//	//		return JoinDungeonRoomTimeOutAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//
//	//	case EVENT_X2_SERVER_STATE_CHANGE_MAIN_LOBBY_ACK:
//	//		return StateChangeMainLobbyTimeOutAck( hWnd, uMsg, wParam, lParam );
//	//		break;
//	//	}
//
//	//	return false;
//	//}
//
//	//bool CX2StateDungeonLobby::DungeonRoomSimpleInfoListReq( int pageNum )
//	//{
//	//	KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ	kEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ;
//	//	kEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ.pageNum = pageNum;
//
//	//	g_pData->GetServerProtocol()->SendPacket( EVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ, kEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_REQ );
//	//	g_pMain->AddServerPacket( EVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK, 5.0f );
//	//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	//	m_pDLGMsgBox = g_pMain->KTDGUIMsgBox( D3DXVECTOR2(250,300), L"방 정보를 받아오는 중입니다", this );
//	//	return true;
//	//}
//
//	//bool CX2StateDungeonLobby::DungeonRoomSimpleInfoListAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//{
//	//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	//	KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK kEvent;
//	//	DeSerialize( pBuff, &kEvent );
//	//	SAFE_DELETE( pBuff );
//
//	//	SAFE_DELETE_DIALOG( m_pDLGMsgBox );
//	//	if( g_pMain->DeleteServerPacket( EVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK ) == true )
//	//	{
//	//		if( kEvent.bResult == true )
//	//		{
//	//			m_EndPage = kEvent.maxPageNum;
//
//	//			for( int i = 0; i < (int)m_RoomList.size(); i++ )
//	//			{
//	//				CX2DungeonRoom::DungeonRoomSimpleData* pRoomData = m_RoomList[i];
//	//				SAFE_DELETE( pRoomData );
//	//			}
//	//			m_RoomList.clear();
//
//	//			for( int i = 0; i < (int)kEvent.roomSimpleInfoList.size(); i++ )
//	//			{
//	//				KEVENT_X2_SERVER_ROOM_SIMPLE_INFO roomInfo = kEvent.roomSimpleInfoList[i];
//	//				CX2DungeonRoom::DungeonRoomSimpleData* pRoomData = new CX2DungeonRoom::DungeonRoomSimpleData;
//	//				pRoomData->CopyData( roomInfo );
//
//	//				m_RoomList.push_back( pRoomData );
//	//			}
//
//	//			ResetRoomListUI();
//	//		}
//	//		else
//	//		{
//	//			switch( kEvent.errCode )
//	//			{
//	//			case KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK::ERROR_CODE::EC_NO_ERROR:
//	//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"에러가 없습니다.", this );
//	//				break;
//
//	//			case KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK::ERROR_CODE::EC_STATE_FAIL:
//	//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"스테이트(FSM) 에러입니다.", this );
//	//				break;
//
//	//			case KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_ACK::ERROR_CODE::EC_ROOMLIST_FAIL:
//	//				g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"방정보가 없습니다.(있어서는 안될오류)", this );
//	//				break;
//	//			}
//
//	//			//g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"방 정보를 받아올 수 수 없습니다", this );
//	//		}
//	//	}
//	//	return true;
//	//}
//
//	//bool CX2StateDungeonLobby::DungeonRoomSimpleInfoListTimeOutAck( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//{
//	//	g_pKTDXApp->GetDGManager()->GetDialogManager()->DeleteDlg( m_pDLGMsgBox );
//	//	m_pDLGMsgBox = NULL;
//
//	//	g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"서버에 응답이 없습니다\n방 정보를 받아올 수 수 없습니다", this );
//	//	return true;
//	//}
//
//	//bool CX2StateDungeonLobby::DungeonRoomSimpleInfoListNot( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
//	//{
//	//	KSerBuffer* pBuff = (KSerBuffer*)lParam;
//	//	KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT kEvent;
//	//	DeSerialize( pBuff, &kEvent );
//	//	SAFE_DELETE( pBuff );
//
//	//	if( kEvent.bResult == true )
//	//	{
//	//		m_EndPage = kEvent.maxPageNum;
//
//	//		for( int i = 0; i < (int)m_RoomList.size(); i++ )
//	//		{
//	//			CX2DungeonRoom::DungeonRoomSimpleData* pRoomData = m_RoomList[i];
//	//			SAFE_DELETE( pRoomData );
//	//		}
//	//		m_RoomList.clear();
//
//	//		for( int i = 0; i < (int)kEvent.roomSimpleInfoList.size(); i++ )
//	//		{
//	//			KEVENT_X2_SERVER_ROOM_SIMPLE_INFO roomInfo = kEvent.roomSimpleInfoList[i];
//	//			CX2DungeonRoom::DungeonRoomSimpleData* pRoomData = new CX2DungeonRoom::DungeonRoomSimpleData;
//	//			pRoomData->CopyData( roomInfo );
//
//	//			m_RoomList.push_back( pRoomData );
//	//		}
//
//	//		ResetRoomListUI();
//	//	}
//	//	else
//	//	{
//	//		switch( kEvent.errCode )
//	//		{
//	//		case KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT::ERROR_CODE::EC_NO_ERROR:
//	//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"에러가 없습니다.", this );
//	//			break;
//
//	//		case KEVENT_X2_SERVER_GET_DUNGEON_ROOM_SIMPLE_INFO_LIST_NOT::ERROR_CODE::EC_ROOM_LIST_FAIL:
//	//			g_pMain->KTDGUIOKMsgBox( D3DXVECTOR2(250,300), L"페이지 정보 얻어오기 실패.(이오류가 있어서는 안된다.)", this );
//	//			break;
//	//		}
//	//	}
//
//	//	return true;
//	//}
//
//
//
//
//	////////////////////////////////////////////////////////////////////////////
//	//// CX2StateDungeonGame
//	////////////////////////////////////////////////////////////////////////////	
//	//class CX2StateDungeonGame : public CX2State
//	//{
//	//};
//
//
//
//
//	class KGCTrigger
//	{
//	public:
//		KGCTrigger(void);
//		virtual ~KGCTrigger(void);
//
//	public:
//		bool FrameMove();
//
//		std::string& GetReaction(){ return m_strReaction; }
//
//		void SetAllTrue( bool bAll ){ m_bAllTrue = bAll; }
//		void SetCount( int iCount ){ m_iTotalCount = iCount; }
//		void SetCheckDelay( float fDelay ){ m_fCheckDelay = fDelay; }
//
//		void AddCondition( const std::string& strCondition ){ m_vecCondition.push_back( strCondition ); }
//		void SetReaction( const std::string& strReaction ){ m_strReaction = strReaction; }
//		void SetRate( int iRate ){ m_iRate = iRate; }
//
//	private:
//		bool CheckTrigger();
//
//	private:
//		bool m_bAllTrue; // 모든 condition을 만족해야 하는지, 아니면 condition중 하나만 만족하면 되는지
//
//		int m_iRate;
//		float m_fCheckDelay;
//		float m_fCurrDelay;
//
//		std::vector<std::string> m_vecCondition;
//		std::string m_strReaction;
//
//		int m_iCurrCount;
//		int m_iTotalCount;
//	};
//
//
//
//	KGCTrigger::KGCTrigger(void)
//	{
//		m_strReaction = "";
//		m_iTotalCount = 0;
//		m_iCurrCount = 0;
//		m_fCheckDelay = 0.0f;
//		m_fCurrDelay = 0.0f;
//		m_iRate = 100;
//		m_bAllTrue = false;
//	}
//
//	KGCTrigger::~KGCTrigger(void)
//	{
//		m_vecCondition.size();
//	}
//
//	bool KGCTrigger::FrameMove()
//	{
//		if( CheckTrigger() == true )
//		{
//			/*if( g_kLuabinder.DoString( m_strReaction.c_str() ) == E_FAIL )
//			assert( "Trigger Reaction Failed!!" );*/
//
//			++m_iCurrCount;
//			m_fCurrDelay = m_fCheckDelay;
//
//			return true;
//		}
//
//		return false;
//	}
//
//	bool KGCTrigger::CheckTrigger()
//	{
//
//		float fElapsedTime = 0.1f;
//
//		// 수행횟수를 체크
//		if( m_iTotalCount != 0 && m_iCurrCount >= m_iTotalCount )
//			return false;
//
//		// 딜레이 체크
//		if( m_fCurrDelay > 0.0f )
//		{
//			m_fCurrDelay -= fElapsedTime;
//			return false;
//		}
//
//		// 확률체크
//		int iRand = rand() % 5500;
//		if( iRand > m_iRate )
//			return false;
//
//		for( int i=0 ; i<(int)m_vecCondition.size() ; ++i )
//		{
//			if( g_pKTDXApp->GetLuaBinder()->DoString( m_vecCondition[i].c_str() ) == S_OK )
//			{
//				if( m_bAllTrue == false )
//					return true;
//			}
//			else
//			{
//				return false;
//			}
//		}
//
//		return true;
//	}
//
//
//
//	struct SMonsterAction
//	{
//		int m_iMotionID;
//		std::string m_strFunc;
//		std::vector<KGCTrigger> m_vecTrigger;
//	};
//
//	////class CNPC
//	////{
//	////public:
//	////	~CNPC();
//	////	CNPC();
//
//	////	void SetNPCAI_LUA()
//	////	{
//	////		KLuaManager luaManager( g_pKTDXApp->GetLuaBinder()->GetLuaState() );
//	////		//TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
//	////		bool bTableOpen = false;
//
//	////		bTableOpen = luaManager.BeginTable( "ActionList" );
//	////		if( true == bTableOpen )
//	////		{
//	////			for( int i=1;; i++ )
//	////			{
//	////				SMonsterAction sAction;
//
//	////				bTableOpen = m_LuaManager.BeginTable(i);
//	////				if( true == bTableOpen  )
//	////				{
//	////					LUA_GET_VALUE( luaManager, L"ID", strID, L"" );
//	////					//LUA_GET_VALUE( luaManager, L"Motion", strMotionFile, L"" );
//	////					//LUA_GET_VALUE( luaManager, L"AttackMotion", bAttackMotion, false );
//	////					//LUA_GET_VALUE( luaManager, L"Func", sAction.m_strFunc, L"" );
//	////					
//	////					bTableOpen = m_LuaManager.BeginTable( "Trigger" );
//	////					if( true == bTableOpen )
//	////					{
//	////						for( int j=1;; j++ )
//	////						{
//	////							KGCTrigger kTrigger;
//	////							bool bAll;
//	////							float fDelay;
//	////							int iRate;
//	////							int iCount;
//	////							std::string strReaction;
//	////							std::string strCondition;
//
//	////							bTableOpen = m_LuaManager.BeginTable(j);
//	////							if( true == bTableOpen )
//	////							{
//	////								LUA_GET_VALUE( luaManager, L"ConditionOp", bAll, true )
//	////								LUA_GET_VALUE( luaManager, L"Rate", iRate, 100 );
//	////								LUA_GET_VALUE( luaManager, L"Delay", fDelay, 0.f );
//	////								LUA_GET_VALUE( luaManager, L"Count", iCount, 0 );
//	////								LUA_GET_VALUE( luaManager, L"Reaction", strReaction, L"" );
//
//	////								bTableOpen = m_LuaManager.BeginTable( "Condition" );
//	////								if( true == bTableOpen )
//	////								{
//	////									for( int k=1;; k++ )
//	////									{
//	////										bool bGotValue = luaManager.GetValue(k, strCondition);
//	////										if( true = bGotValue )
//	////										{
//	////											kTrigger.AddCondition( strCondition );												
//	////										}
//	////										else
//	////										{
//	////											break; // for(k)
//	////										}
//	////									}										
//	////									m_LuaManager.EndTable(); // Condition
//	////								}
//
//	////								kTrigger.SetAllTrue( bAll );
//	////								kTrigger.SetReaction( strReaction );
//	////								kTrigger.SetCheckDelay( fDelay );
//	////								kTrigger.SetRate( iRate );
//	////								kTrigger.SetCount( iCount );
//
//	////								sAction.m_vecTrigger.push_back( kTrigger );
//	////							}
//	////							else
//	////							{
//	////								break; // for(j)
//	////							}
//	////						}
//	////						m_LuaManager.EndTable(); // Trigger
//	////					}					
//
//	////					m_LuaManager.EndTable();
//	////				}
//	////				else 
//	////				{
//	////					break; // for(i)
//	////				}
//	////			}
//	////			m_LuaManager.EndTable(); // ActionList
//	////		}
//	////	}
//
//	////private:
//	////	//KLuaManager m_LuaManager;
//	////	std::map<std::string, SMonsterAction> m_mapAction;
//	////};
//
//
//
//
//
//	// AI FSM test
//	//template <class entity_type>
//	//class CX2AIFSM
//	//{
//	//public:
//	//	CX2AIFSM( entity_type* pOwner )
//	//	{
//	//		m_pOwner = pOwner;
//	//	}
//
//	//	void SetCurrentState( const luabind::object& new_state )
//	//	{
//	//		m_CurrentState = new_state;
//	//	}
//
//	//	const luabind::object& GetCurrentState() const 
//	//	{ 
//	//		return m_CurrentState; 
//	//	}
//
//	//	void UpdateStateMachine()
//	//	{
//	//		if( m_CurrentState.is_valid() )
//	//		{
//	//			m_CurrentState.at("Execute")(m_pOwner);			
//	//		}
//	//	}
//
//	//	void ChangeState( const luabind::object& new_state )
//	//	{
//	//		m_CurrentState.at("Exit")(m_pOwner);
//	//		m_CurrentState = new_state;
//	//		m_CurrentState.at("Enter")(m_pOwner);
//	//	}
//
//	//private:
//	//	entity_type* m_pOwner;		
//	//	luabind::object m_CurrentState;
//	//};
//	
//	//
//	//class CNPC;
//	//class CX2AIState
//	//{
//	//public:
//	//	virtual ~CX2AIState();
//
//	//	virtual void Enter(CNPC* pUnit) = NULL;
//	//	virtual void Execute(CNPC* pUnit) = NULL;
//	//	virtual void Exit(CNPC* pUnit) = NULL;
//	//};
//
//	//class CX2ASWander : public CX2AIState
//	//{
//	//public:
//	//	virtual ~CX2ASWander() 
//	//	{			
//	//	}
//	//	
//	//	virtual void Enter(CNPC* pUnit)
//	//	{
//	//	}
//	//	virtual void Execute(CNPC* pUnit)
//	//	{
//	//	}
//	//	virtual void Exit(CNPC* pUnit)
//	//	{
//	//	}
//	//	
//	//};
//
//	//
//
//	//class CNPC //: public CX2GUNPC
//	//{
//	//public:
//	//	~CNPC()
//	//	{
//	//		if( NULL != m_pCurrAIState )
//	//			m_pCurrAIState->Exit(this);
//
//	//		SAFE_DELETE(m_pCurrAIState);
//	//	}
//	//	CNPC()
//	//	{
//	//	}
//	//	void ChangeState(CX2AIState* pNewAIState)
//	//	{
//	//		assert( NULL != m_pCurrAIState && NULL != pNewAIState );
//	//		m_pCurrAIState->Exit(this);
//	//		m_pCurrAIState = pNewAIState;
//	//		m_pCurrAIState->Enter(this);
//	//	}
//
//	//private:
//	//	CX2AIState* m_pCurrAIState;
//
//	//};
//
//	
//} // end of namespace SANDBOX
//
//
////////////////////////////////////////////////////////////////////////////
//// CMyTest
////////////////////////////////////////////////////////////////////////////
//class CMyTest : public CX2Main //public CKTDXStage
//{
//public:
//
//public:
//	virtual ~CMyTest(void);
//	CMyTest(void);
//
//	virtual HRESULT OnLostDevice();
//	virtual HRESULT OnResetDevice();
//	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
//	virtual HRESULT OnFrameRender();
//	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
//
//	void FinalDialog();
//	void InitDialog();
//
//	void CreateNewGame();
//
//private:
//	vector<CDXUTDialog*> m_vecDialog;
/////	SANDBOX::CX2DungeonGame* m_pDungeonGame;
//
//	KLuaManager m_LuaManager;
//};
//
