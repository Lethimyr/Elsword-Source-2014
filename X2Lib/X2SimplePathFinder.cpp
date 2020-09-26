#include "StdAfx.h"
#include ".\x2simplepathfinder.h"


CX2SimplePathFinder::CX2SimplePathFinder(void)
{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
	m_pPrevLineGroup.Init();
	m_pCurrLineGroup.Init();
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	m_pPrevLineGroup = NULL;
	m_pCurrLineGroup = NULL;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	m_MoveState = MS_WAIT;

#ifdef AI_FLY
	m_bFlyTurn = false;		
#endif

	m_iDestLineDataIndex = -1;
}


CX2SimplePathFinder::~CX2SimplePathFinder(void)
{
}


//////////////////////////////////////////////////////////////////////////
// CSimplePathFinder::FollowTargetPath() -- 코드랑 완전히 똑같지는 않지만 대략 이런식의 rule
// if( 점프상태 ) 
//   목적지까지 정해진 속도로 움직인다
// else
//   if( 최종목적지가 연결된 line위에 있다 ) 달려~
//   else
//     if( 최종목적지가 위쪽 && 수직 위로 점프가능 ) 수직 위로 점프
//     else if( 최종목적지가 아래쪽 && 수직 아래로 점프가능 ) 수직 아래로 점프
//     else      
//       if( 라인 끝에 도착 )
//         if( 대각선 점프 가능 ) 대각선 점프
//         else 오던 반대방향으로 걷기
//       else 계속 걷기
//
// todo: randomness 추가
//       못찾는 길 찾도록 고치기.
//       
//////////////////////////////////////////////////////////////////////////

void CX2SimplePathFinder::FollowTargetPath( /*float fElapsedTime,*/ const D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
											const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex, 
											const float fMaxJumpUp, const float fMaxJumpRight,
											IN CKTDGLineMap* pLineMap, const float fDestArriveGap, const float fDestLeaveGap, const bool bFootOnLine,
											const bool bStayOnCurrLineGroup, OUT bool& bTargetOnRight,
											const float fLineEndDetectRange 
#ifdef ADD_NPC_CONDITION_TABLE
											, IN bool bIfCannotFindMoveStateDoWait /*= false*/
#endif // ADD_NPC_CONDITION_TABLE
											)
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 
	// 위에 이미 const로 되어 있는데 왜 또 const로 다시 받나?ㅡㅡ;
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
	
#ifdef UNIT_ROAD
	if( pFinalDestLineData != NULL )
	{
		switch ( pFinalDestLineData->lineType )
		{
		case CKTDGLineMap::LT_UNIT_ROAD:
		case CKTDGLineMap::LT_JUMP_UP_ABS:
		case CKTDGLineMap::LT_JUMP_UP_REL:
			m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
			return;
			break;

		default:
			break;
		}							
	}
#endif

	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == bFootOnLine )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData가 NULL일경우 처리는....????
			if(pCurrLineData == NULL)
				return;

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDistSq = D3DXVec3LengthSq( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // 같은 line group
			{						
				if( fFinalDestDistSq < fDestArriveGap * fDestArriveGap ) // 목적지에 도착
				{
					m_MoveState = MS_WAIT;
					return;
				}
				else
				{
					if( MS_WAIT == m_MoveState && fFinalDestDistSq < fDestLeaveGap * fDestLeaveGap )
					{
						return;
					}

					if( fIsRight > 0.f )
					{
						m_MoveState = MS_RIGHT;
					}
					else 
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}		
			} 
			else // 같은 line group이 아니면
			{
				if( false == bStayOnCurrLineGroup )
				{
					if( fIsUp > 0.f ) // 목적지가 위에 있는 경우
					{
						D3DXVECTOR3 vJumpPos = vCurrPos + vUpVec * fMaxJumpUp;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

						// FieldFix: 이게 뭐하는 짓이래...
						// 라인의 끝에서 점프해서 못 올라오는 경우를 막기 위한 보정 코드
						const BOOL MAGIC_USE_COMPENSATION = true;
						if( true == MAGIC_USE_COMPENSATION )
						{
							const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( m_iDestLineDataIndex );

							const float MAGIC_LINE_RADIUS_EXT = LINE_RADIUS * 1.5f;
							if( pLineData->beforeLine < 0 )
							{
								if( GetDistance3Sq( pLineData->startPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}

							if( pLineData->nextLine < 0 )
							{
								if( GetDistance3Sq( pLineData->endPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}
						}

						if( -1 != m_iDestLineDataIndex && m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData	= pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( pDestLineGroup != m_pCurrLineGroup &&
											(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										{
											// 수직 위로 jump
											m_MoveState = MS_JUMP;
											m_vDestPos = vJumpPos;
											return;
										}
									} break;
								}							
							}

						}
					}
					else // 목적지가 아래에 있는 경우
					{
						D3DXVECTOR3 vJumpPos = vCurrPos - vUpVec * LINE_RADIUS * 1.5f;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{   
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState )
										{
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										    if( pDestLineGroup != m_pCurrLineGroup &&
											    (pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										    {
											    // 목적지 보다 높이가 낮으면 왠만하면 내려가지 않는다
											    //if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState || vJumpPos.y >= vFinalDestPos.y )

												    // 수직 아래로 down
												m_MoveState = MS_DOWN;
												m_vDestPos = vJumpPos;
												return;
										    }
                                        }
									} break;
								}										
							}

						}
					}
				}

				// 수직 위 아래로 jump 하거나 down 할 수 없으면 그냥 이동
				if( MS_FORCE_LEFT != m_MoveState && (MS_FORCE_RIGHT == m_MoveState || fIsRight > 0.1f) ) // 목적지가 오른쪽 
				{
					if( pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineEnd = pCurrLineData->endPos - vCurrPos;
						const float fLineEndDist = D3DXVec3LengthSq( &vLineEnd );
						if( fLineEndDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // 오른쪽으로 갈 수 없으면
						{	
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{									
									// 그냥 걸어가면 내려갈 수 있는 발판이 있는지 확인
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos( pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS );
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

										if( 
                                            pDestLineData == NULL || 
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{													
											m_MoveState = MS_FORCE_RIGHT;
											return;
										}	
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}								

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                        pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// 대각선 오른쪽 jump
										m_MoveState = MS_JUMP_RIGHT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// 오른쪽으로 갈 수도 없고 jump도 할 수 없으면 왼쪽으로 이동
							{
#ifdef ADD_NPC_CONDITION_TABLE
								if( true == bIfCannotFindMoveStateDoWait )
								{
									m_MoveState = MS_WAIT;
									return;
								}
								else
#endif // ADD_NPC_CONDITION_TABLE
								{
									switch( m_MoveState )
									{									
									case MS_LEFT:
									case MS_FORCE_LEFT:
										{
											m_MoveState = MS_FORCE_RIGHT;
										} break;

									case MS_WAIT:
									case MS_RIGHT:
									case MS_FORCE_RIGHT:
										{
											m_MoveState = MS_FORCE_LEFT;
										} break;
									}
								}
							}
						}							
					}

					// can go right
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_RIGHT;
					}
					return;
				}
				else if( MS_FORCE_RIGHT != m_MoveState && (MS_FORCE_LEFT == m_MoveState || fIsRight < -0.1f) ) // 목적지가 왼쪽
				{
					if( pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineStart = pCurrLineData->startPos - vCurrPos;
						float fLineStartDist = D3DXVec3Length( &vLineStart );
						if( fLineStartDist < MAGIC_COLLISION_BOUND ) // 왼쪽으로 갈 수 없으면
						{
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{	
									// 그냥 걸어가면 내려갈 수 있는 발판이 있는지 확인
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos;
									vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

										if( 
                                            pDestLineData == NULL ||
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{
											m_MoveState = MS_FORCE_LEFT;
											return;
										}										
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                        pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// 대각선 왼쪽 jump
										m_MoveState = MS_JUMP_LEFT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// 왼쪽으로 갈 수도 없고 jump도 할 수 없으면 오른쪽으로 이동
							{
#ifdef ADD_NPC_CONDITION_TABLE
								if( true == bIfCannotFindMoveStateDoWait )
								{
									m_MoveState = MS_WAIT;
									return;
								}
								else
#endif // ADD_NPC_CONDITION_TABLE
								{
									switch( m_MoveState )
									{
									case MS_WAIT:
									case MS_LEFT:
									case MS_FORCE_LEFT:
										m_MoveState = MS_FORCE_RIGHT;
										break;

									case MS_RIGHT:
									case MS_FORCE_RIGHT:
										m_MoveState = MS_FORCE_LEFT;
										break;
									}
								}
							}
						}
					}

					// can go left
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_LEFT;
					}
					return;
				}
				else // 목적지가 왼쪽도 오른쪽도 아닌 경우
				{
					// force move
					if( MS_LEFT == m_MoveState )
					{
						m_MoveState = MS_FORCE_LEFT;
					}
					else if( MS_RIGHT == m_MoveState )
					{
						m_MoveState = MS_FORCE_RIGHT;
					}
					else if( MS_WAIT == m_MoveState )
					{
						m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
					}
					return;
				}				
			} 
		}break;
		default:
			{
				if( true == bFootOnLine )
					m_MoveState = MS_WAIT;
			}
		break;
	} // end of switch( m_MoveState )
} // end of OnFrameMove()


void CX2SimplePathFinder::EscapeTargetPath( /*float fElapsedTime,*/ const D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
											const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex, 
											const float fMaxJumpUp, const float fMaxJumpRight,
											IN CKTDGLineMap* pLineMap, const float fDestArriveGap, /*const float fDestLeaveGap,*/ const bool bFootOnLine,
											/*bool bStayOnCurrLineGroup,*/ OUT bool& bTargetOnRight,
											const bool bIsRight,
											const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 

	const CKTDGLineMap::LineData*			pFinalDestLineData	= pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID	    pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup*	pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir	= vFinalDestPos - vCurrPos;
	float fFinalDestDist		= D3DXVec3Length( &vFinalDestDir );

	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == bFootOnLine )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            if ( pCurrLineData == NULL )
                m_pCurrLineGroup.Init();
            else
                m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // 같은 line group
			{
				if( bTargetOnRight == bIsRight && fFinalDestDist < fDestArriveGap ) // 타겟쪽으로 이동하면서 타겟 가까이 있으면
				{
					// 점프해서 넘어가자
					if( true == bIsRight )
					{
						m_MoveState = MS_JUMP_RIGHT;
					}
					else
					{
						m_MoveState = MS_JUMP_LEFT;
					}
					return;					
				}		
			}

			// 같은 line group이 아니거나, 같은 line group이라도 가까이 있지 않으면 
			if( fIsRight > 0.f ) // 타겟이 오른쪽에 있고
			{
				if( pCurrLineData->beforeLine < 0 || pLineMap->GetLineData(pCurrLineData->beforeLine)->bEnable == false )
				{
					D3DXVECTOR3 vLineStart	= pCurrLineData->startPos - vCurrPos;
					float fLineStartDist	= D3DXVec3Length( &vLineStart );
					if( fLineStartDist < MAGIC_COLLISION_BOUND ) // 왼쪽으로 갈 수 없으면
					{
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

							//대각선 왼쪽 jump
							m_MoveState = MS_JUMP_LEFT;
							m_vDestPos = vJumpPos;
							return;
						}
						else // jump 해서 밟을 땅이 없으면 반대 방향으로 돌아가자
						{
							m_MoveState = MS_FORCE_RIGHT;
							return;
						}
					}
				}

				// 왼쪽으로 갈 수 있으면 
				m_MoveState = MS_LEFT;
				return;
			}
			else // 타겟이 왼쪽에 있고 
			{
				if( pCurrLineData->nextLine < 0 || pLineMap->GetLineData(pCurrLineData->nextLine)->bEnable == false ) 
				{
					D3DXVECTOR3 vLineEnd	= pCurrLineData->endPos - vCurrPos;
					float fLineEndDist		= D3DXVec3Length( &vLineEnd );
					if( fLineEndDist < MAGIC_COLLISION_BOUND ) // 오른쪽으로 갈 수 없으면
					{	
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}								

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

							// 대각선 오른쪽 jump
							m_MoveState = MS_JUMP_RIGHT;
							m_vDestPos = vJumpPos;
							return;
						}
						else // jump 해서 밟을 땅이 없으면 반대 방향으로 돌아가자
						{
							m_MoveState = MS_FORCE_LEFT;
							return;
						}
					}
				}

				// 오른쪽으로 갈 수 있으면 
				m_MoveState = MS_RIGHT;
				return;
			}		
		} break;
#ifdef AI_FLY
	default:
		m_MoveState = MS_WAIT;
		break;
#endif
	} // end of switch( m_MoveState )
} // end of OnFrameMove()


#ifdef AI_FLY
// *FlyPos : 실제로 움직일 위치
// *TargetPos : 유저의 위치
// *NpcPos : 현재 날틀 위치//
// *Fly 이동시 높이는 자동으로 조절되도록 한다. (gunpc::PhysicProcess 에서 조절하도록한다.)
//
//	1. 일정범위안에 도착했다면 
//		1. 고도를 높여야 한다면 Fly Up 시킨다.
//		2. 고도를 낮춰야 한다면 Fly Down 시킨다.
//		3. 고도를 조절할 필요가 없다면 Wait
//	2. 더이상 갈수 없는 지역이라면(라인맵의 끝) 방향으로 바꾼다.
//	3. TargetPos로 Fly 시킨다. (높이조절포함)

void CX2SimplePathFinder::FollowTargetPathFly( /*float fElapsedTime,*/ const D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
										   const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex,
										   const float fMaxJumpUp, const float fMaxJumpRight,
										   const float fDestArriveHeight, const float fDestArriveArea,
										   IN CKTDGLineMap* pLineMap, const float fDestArriveGap, /*const float fDestLeaveGap,*/ /*bool bFootOnLine,*/
										   /*bool bStayOnCurrLineGroup,*/ bool& bTargetOnRight,
										   const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID   pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
	if( pFinalDestLineData != NULL )
	{
		switch ( pFinalDestLineData->lineType )
		{
		case CKTDGLineMap::LT_UNIT_ROAD:
		case CKTDGLineMap::LT_JUMP_UP_ABS:
		case CKTDGLineMap::LT_JUMP_UP_REL:
			m_MoveState = rand()%2 ? MS_FLY_LEFT_FRONT : MS_FLY_RIGHT_FRONT;
			return;
			break;

		default:
			break;
		}							
	}
#endif

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;	
	float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );

	D3DXVECTOR3 vFinalDestLandDir = vFinalDestDir;
	vFinalDestLandDir.y = 0.f;
	float fFinalDestLandDist = D3DXVec3Length( &vFinalDestLandDir );

	switch( m_MoveState )
	{
	case MS_WAIT:
	case MS_FLY_UP:
	case MS_FLY_DOWN:
	case MS_FLY_LEFT_FRONT:
	case MS_FLY_LEFT_FRONT_UP:
	case MS_FLY_LEFT_FRONT_DOWN:
	case MS_FLY_LEFT_BACK:
	case MS_FLY_LEFT_BACK_UP:
	case MS_FLY_LEFT_BACK_DOWN:
	case MS_FLY_RIGHT_FRONT:
	case MS_FLY_RIGHT_FRONT_UP:
	case MS_FLY_RIGHT_FRONT_DOWN:
	case MS_FLY_RIGHT_BACK:
	case MS_FLY_RIGHT_BACK_UP:
	case MS_FLY_RIGHT_BACK_DOWN:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData가 NULL일경우 처리는....????
			if(pCurrLineData == NULL)
			{				
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;
				return;
			}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			int iLandLineIndex = -1;
			D3DXVECTOR3 vLandPos = pLineMap->GetLandPosition(vCurrPos, 2000.f, &iLandLineIndex);

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDist = D3DXVec3Length( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			//if( m_MoveState == MS_WAIT && fFinalDestDist < fDestLeaveGap )
			//{
			//	return;
			//}

			bool bFlyFront = true;
			if( fFinalDestLandDist < fDestArriveGap - fDestArriveArea )
			{
				bFlyFront = false;
			}
			if( fFinalDestLandDist > fDestArriveGap + fDestArriveArea )
			{					
				bFlyFront = true;
			}

			bool bFlyUp = false;
			bool bFlyDown = false;
			if( vCurrPos.y < vFinalDestPos.y + fDestArriveHeight - 10.f )
			{
				// 목적지가 위에 있는 경우
				bFlyUp = true;
				bFlyDown = false;
			}
			else if( vCurrPos.y > vFinalDestPos.y + fDestArriveHeight + 10.f )
			{
				// 목적지가 아래에 있는 경우
				bFlyUp = false;
				bFlyDown = true;
			}

			// 목적지의 일정범위내에 도착했는지 검사한다.
			if( fFinalDestLandDist >= fDestArriveGap - fDestArriveArea && fFinalDestLandDist <= fDestArriveGap + fDestArriveArea ) // 목적지에 도착
			{
				// 정지상태에서 고도를 높일지 낮출지 결정한다.
				if(bFlyUp == true)
				{
					m_MoveState = MS_FLY_UP;
					m_bFlyTurn = false;		
					return;
				}
				else if(bFlyDown == true)
				{
					m_MoveState = MS_FLY_DOWN;
					m_bFlyTurn = false;		
					return;
				}

				// 고도 조절할 필요가 없다면 상태를 Wait로 바꾼다.
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;				
				return;
			}
			else
			{
				// 아직 목적지에 도달하지 못했으므로 이동시킨다.
				// 현재 라인맵의 끝이라서 더이상 이동불가능한 상태인지 검사한다.
				// 이동불가하다면 방향을 바꿔서 이동한다.
				
				// Fly Front로 이동할지 Back으로 이동할지 결정
				
				
				if(m_bFlyTurn == true)
				{
					if( ((m_MoveState == MS_FLY_LEFT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT_UP || m_MoveState == MS_FLY_LEFT_FRONT_DOWN) &&
						 fIsRight > 0.3f) ||
						((m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_RIGHT_FRONT_UP || m_MoveState == MS_FLY_RIGHT_FRONT_DOWN) && 
						 fIsRight < -0.3f) )
					{
						m_bFlyTurn = false;
						m_MoveState = MS_WAIT;						
					}					
					return;
				}

				if( m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT ||
					m_MoveState == MS_FLY_RIGHT_BACK || m_MoveState == MS_FLY_LEFT_BACK ||
					m_MoveState == MS_FLY_RIGHT_BACK_UP || m_MoveState == MS_FLY_LEFT_BACK_UP ||
					m_MoveState == MS_FLY_RIGHT_BACK_DOWN || m_MoveState == MS_FLY_LEFT_BACK_DOWN ) //|| m_MoveState == MS_WAIT)
				{
					//const float fMaxJumpUp = 282.f;
					//const float fMaxJumpRight = 94.f;

					if( fIsRight < 0.f )
					{
						// 목적지는 왼쪽에 있고, 진행방향은 오른쪽 뒤로 이동중이다.
						bool bLeftLand, bLeftUpLand, bLeftDownLand;
						bLeftLand = true;
						bLeftUpLand = bLeftDownLand = false;

						// 현재 라인
						if(pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false)
                           bLeftLand = false;

						// 아래로 내려갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {													
										    bLeftDownLand = true;
									    }	
								    } break;
							    }		
                            }
						}

						// 위로 올라갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}								

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
    #ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
    #endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // 대각선 오른쪽 jump
										    bLeftUpLand = true;
									    }
								    } break;
							    }
                            }
						}


						if( bLeftLand == false && bLeftUpLand == false && bLeftDownLand == false ) //
						{
							m_MoveState = MS_FLY_LEFT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
					else
					{
						// 목적지는 오른쪽에 있고, 진행방향은 왼쪽 뒤로 이동중이다.
						bool bRightLand, bRightUpLand, bRightDownLand;
						bRightLand = true;
						bRightUpLand = bRightDownLand = false;

						// 현재 라인
						if(pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false)
							bRightLand = false;

						// 아래로 내려갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
							
                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    bRightDownLand = true;
									    }										
								    } break;
							    }
                            }
						}

						// 위로 올라갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );							
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;	
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // 대각선 왼쪽 jump
										    bRightUpLand = true;
									    }
								    } break;
							    }
                            }
						}

						if( bRightLand == false && bRightUpLand == false && bRightDownLand == false ) //
						{
							m_MoveState = MS_FLY_RIGHT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
				}


				// 이동가능하다면 쭉~ 이동시킨다.
				if( fIsRight > 0.f )
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_BACK;
					}
				}
				else
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_BACK;
					}
				}				
				return;
			}
		}
		break;
	default:
		m_MoveState = MS_WAIT;
		break;

	} // end of switch( m_MoveState )
	
} // end of OnFrameMove()
#endif


#ifdef SERV_PET_SYSTEM
void CX2SimplePathFinder::SimpleFollowTargetPath( float fElapsedTime, D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
										   const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex, 
										   const float fMaxJumpUp, const float fMaxJumpRight,
										   CKTDGLineMap* pLineMap, const float fDestArriveGap, const float fDestLeaveGap, bool bFootOnLine,
										   bool bStayOnCurrLineGroup, bool& bTargetOnRight,
										   const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
	float fFinalDestDist = D3DXVec3LengthSq( &vFinalDestDir );


	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == bFootOnLine )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData가 NULL일경우 처리는....????
			if(pCurrLineData == NULL)
				return;

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			float fFinalDestDist = D3DXVec3LengthSq( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // 같은 line group
			{						
				if( MS_WAIT == m_MoveState && fFinalDestDist < pow( fDestLeaveGap, 2 ) )
				{
					return;
				}

				if( fFinalDestDist < pow( fDestArriveGap, 2 ) ) // 목적지에 도착
				{
					m_MoveState = MS_WAIT;
					return;
				}
				else
				{
					if( fIsRight > 0.f )
					{
						m_MoveState = MS_RIGHT;
					}
					else 
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}		
			} 
			else // 같은 line group이 아니면
			{
				if( false == bStayOnCurrLineGroup )
				{
					if( fIsUp > 0.f ) // 목적지가 위에 있는 경우
					{
						D3DXVECTOR3 vJumpPos = vCurrPos + vUpVec * fMaxJumpUp;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

						// 라인의 끝에서 점프해서 못 올라오는 경우를 막기 위한 보정 코드
						const BOOL MAGIC_USE_COMPENSATION = true;
						if( true == MAGIC_USE_COMPENSATION )
						{
							const CKTDGLineMap::LineData* pLineData = pLineMap->GetLineData( m_iDestLineDataIndex );

							const float MAGIC_LINE_RADIUS_EXT = LINE_RADIUS * 1.5f;
							if( pLineData->beforeLine < 0 )
							{
								D3DXVECTOR3 vDist = vJumpPos - pLineData->startPos;
								if( D3DXVec3LengthSq(&vDist) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}

							if( pLineData->nextLine < 0 )
							{
								D3DXVECTOR3 vDist = vJumpPos - pLineData->endPos;
								if( D3DXVec3LengthSq(&vDist) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
								{
									m_iDestLineDataIndex = -1;
								}
							}
						}

						if( -1 != m_iDestLineDataIndex && m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData	= pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( pDestLineGroup != m_pCurrLineGroup &&
											(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										{
											// 수직 위로 jump
											m_MoveState = MS_JUMP;
											m_vDestPos = vJumpPos;
											return;
										}
									} break;
								}
							}

						}
					}
					else // 목적지가 아래에 있는 경우
					{
						D3DXVECTOR3 vJumpPos = vCurrPos - vUpVec * LINE_RADIUS * 1.5f;
						m_iDestLineDataIndex = iCurrLineDataIndex;
						vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
							if( pDestLineData != NULL )
#endif
							{   
								switch ( pDestLineData->lineType )
								{
								case CKTDGLineMap::LT_UNIT_ROAD:
								case CKTDGLineMap::LT_JUMP_UP_ABS:
								case CKTDGLineMap::LT_JUMP_UP_REL:
									break;

								default:
									{
										if( false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( pDestLineGroup != m_pCurrLineGroup &&
											(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
										{
											// 목적지 보다 높이가 낮으면 왠만하면 내려가지 않는다
											//if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState || vJumpPos.y >= vFinalDestPos.y )
											if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState )
											{
												// 수직 아래로 down
												m_MoveState = MS_DOWN;
												m_vDestPos = vJumpPos;
												return;
											}
										}
									} break;
								}
							}

						}
					}
				}

				// 수직 위 아래로 jump 하거나 down 할 수 없으면 그냥 이동
				if( MS_FORCE_LEFT != m_MoveState && (MS_FORCE_RIGHT == m_MoveState || fIsRight > 0.1f) ) // 목적지가 오른쪽 
				{
					if( pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineEnd = pCurrLineData->endPos - vCurrPos;
						float fLineEndDist = D3DXVec3LengthSq( &vLineEnd );
						if( fLineEndDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // 오른쪽으로 갈 수 없으면
						{	
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{									
									// 그냥 걸어가면 내려갈 수 있는 발판이 있는지 확인
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos;
									vDownPos = pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

										if( 
                                            pDestLineData == NULL ||
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{													
											m_MoveState = MS_FORCE_RIGHT;
											return;
										}										
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}								

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                            pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// 대각선 오른쪽 jump
										m_MoveState = MS_JUMP_RIGHT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// 오른쪽으로 갈 수도 없고 jump도 할 수 없으면 왼쪽으로 이동
							{
								switch( m_MoveState )
								{									
								case MS_LEFT:
								case MS_FORCE_LEFT:
									{
										m_MoveState = MS_FORCE_RIGHT;
									} break;

								case MS_WAIT:
								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									{
										m_MoveState = MS_FORCE_LEFT;
									} break;
								}
							}
						}							
					}

					// can go right
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_RIGHT;
					}
					return;
				}
				else if( MS_FORCE_RIGHT != m_MoveState && (MS_FORCE_LEFT == m_MoveState || fIsRight < -0.1f) ) // 목적지가 왼쪽
				{
					if( pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineStart = pCurrLineData->startPos - vCurrPos;
						float fLineStartDist = D3DXVec3LengthSq( &vLineStart );
						if( fLineStartDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // 왼쪽으로 갈 수 없으면
						{
							if( false == bStayOnCurrLineGroup )
							{
								if( vFinalDestPos.y < vCurrPos.y )
								{	
									// 그냥 걸어가면 내려갈 수 있는 발판이 있는지 확인
									m_iDestLineDataIndex = iCurrLineDataIndex;
									D3DXVECTOR3 vDownPos;
									vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
									vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
									if( m_iDestLineDataIndex != iCurrLineDataIndex )
									{
										const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
										CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( 
                                            pDestLineData == NULL ||
                                            false == pDestLineData->m_bMonsterAllowed )
										{
											m_MoveState = MS_WAIT;
											return;
										}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                        CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
											pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
										{
											m_MoveState = MS_FORCE_LEFT;
											return;
										}										
									}
								}

								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vJumpPos;
								for( int i=0; i<3; i++ )
								{
									vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
									vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

									if( m_iDestLineDataIndex != iCurrLineDataIndex )
										break;
								}

								if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( 
                                        pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										// 대각선 왼쪽 jump
										m_MoveState = MS_JUMP_LEFT;
										m_vDestPos = vJumpPos;
										return;
									}
								}
							}

							// 왼쪽으로 갈 수도 없고 jump도 할 수 없으면 오른쪽으로 이동
							{
								switch( m_MoveState )
								{
								case MS_WAIT:
								case MS_LEFT:
								case MS_FORCE_LEFT:
									m_MoveState = MS_FORCE_RIGHT;
									break;

								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									m_MoveState = MS_FORCE_LEFT;
									break;
								}
							}
						}
					}

					// can go left
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_LEFT;
					}
					return;
				}
				else // 목적지가 왼쪽도 오른쪽도 아닌 경우
				{
					// force move
					if( MS_LEFT == m_MoveState )
					{
						m_MoveState = MS_FORCE_LEFT;
					}
					else if( MS_RIGHT == m_MoveState )
					{
						m_MoveState = MS_FORCE_RIGHT;
					}
					else if( MS_WAIT == m_MoveState )
					{
						m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
					}
					return;
				}				
			} 
		}break;

	} // end of switch( m_MoveState )
} // end of OnFrameMove()

#ifdef AI_FLY
void CX2SimplePathFinder::SimpleFollowTargetPathFly( float fElapsedTime, D3DXVECTOR3& vCurrPos, const int iCurrLineDataIndex,
											  const D3DXVECTOR3& vFinalDestPos, const int iFinalDestLineDataIndex,
											  const float fMaxJumpUp, const float fMaxJumpRight,
											  const float fDestArriveHeight, const float fDestArriveArea,
											  CKTDGLineMap* pLineMap, const float fDestArriveGap, const float fDestLeaveGap, bool bFootOnLine,
											  bool bStayOnCurrLineGroup, bool& bTargetOnRight,
											  const float fLineEndDetectRange )
{
	KTDXPROFILE();

	//const float MAGIC_COLLISION_BOUND = LINE_RADIUS * 1.5f;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 
	const float MAGIC_COLLISION_BOUND = fLineEndDetectRange;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 

	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap->GetLineData(iFinalDestLineDataIndex);
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
    CKTDGLineMap::LineGroupID   pFinalDestLineGroup;
    if ( pFinalDestLineData != NULL )
        pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
	const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

	D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;	
	
	D3DXVECTOR3 vFinalDestLandDir = vFinalDestDir;
	vFinalDestLandDir.y = 0.f;
	float fFinalDestLandDist = D3DXVec3Length( &vFinalDestLandDir );

	switch( m_MoveState )
	{
	case MS_WAIT:
	case MS_FLY_UP:
	case MS_FLY_DOWN:
	case MS_FLY_LEFT_FRONT:
	case MS_FLY_LEFT_FRONT_UP:
	case MS_FLY_LEFT_FRONT_DOWN:
	case MS_FLY_LEFT_BACK:
	case MS_FLY_LEFT_BACK_UP:
	case MS_FLY_LEFT_BACK_DOWN:
	case MS_FLY_RIGHT_FRONT:
	case MS_FLY_RIGHT_FRONT_UP:
	case MS_FLY_RIGHT_FRONT_DOWN:
	case MS_FLY_RIGHT_BACK:
	case MS_FLY_RIGHT_BACK_UP:
	case MS_FLY_RIGHT_BACK_DOWN:
		{
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap->GetLineData( iCurrLineDataIndex );

			// pCurrLineData가 NULL일경우 처리는....????
			if(pCurrLineData == NULL)
			{				
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;
				return;
			}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap->GetLineGroupIncludesLineData( iCurrLineDataIndex );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			int iLandLineIndex = -1;
			D3DXVECTOR3 vLandPos = pLineMap->GetLandPosition(vCurrPos, 2000.f, &iLandLineIndex);

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;			
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );
			const D3DXVECTOR3 vUpVec(0,1,0);
			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );
			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight = true;
			else
				bTargetOnRight = false;

			
			bool bFlyFront = true;
			if( fFinalDestLandDist < fDestArriveGap - fDestArriveArea )
			{
				bFlyFront = false;
			}
			if( fFinalDestLandDist > fDestArriveGap + fDestArriveArea )
			{					
				bFlyFront = true;
			}

			bool bFlyUp = false;
			bool bFlyDown = false;
			if( vCurrPos.y < vFinalDestPos.y + fDestArriveHeight - 10.f )
			{
				// 목적지가 위에 있는 경우
				bFlyUp = true;
				bFlyDown = false;
			}
			else if( vCurrPos.y > vFinalDestPos.y + fDestArriveHeight + 10.f )
			{
				// 목적지가 아래에 있는 경우
				bFlyUp = false;
				bFlyDown = true;
			}

			// 목적지의 일정범위내에 도착했는지 검사한다.
			if( fFinalDestLandDist >= fDestArriveGap - fDestArriveArea && fFinalDestLandDist <= fDestArriveGap + fDestArriveArea ) // 목적지에 도착
			{
				// 정지상태에서 고도를 높일지 낮출지 결정한다.
				if(bFlyUp == true)
				{
					m_MoveState = MS_FLY_UP;
					m_bFlyTurn = false;		
					return;
				}
				else if(bFlyDown == true)
				{
					m_MoveState = MS_FLY_DOWN;
					m_bFlyTurn = false;		
					return;
				}

				// 고도 조절할 필요가 없다면 상태를 Wait로 바꾼다.
				m_MoveState = MS_WAIT;
				m_bFlyTurn = false;				
				return;
			}
			else
			{
				// 아직 목적지에 도달하지 못했으므로 이동시킨다.
				// 현재 라인맵의 끝이라서 더이상 이동불가능한 상태인지 검사한다.
				// 이동불가하다면 방향을 바꿔서 이동한다.

				// Fly Front로 이동할지 Back으로 이동할지 결정


				if(m_bFlyTurn == true)
				{
					if( ((m_MoveState == MS_FLY_LEFT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT_UP || m_MoveState == MS_FLY_LEFT_FRONT_DOWN) &&
						fIsRight > 0.3f) ||
						((m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_RIGHT_FRONT_UP || m_MoveState == MS_FLY_RIGHT_FRONT_DOWN) && 
						fIsRight < -0.3f) )
					{
						m_bFlyTurn = false;
						m_MoveState = MS_WAIT;						
					}					
					return;
				}

				if( m_MoveState == MS_FLY_RIGHT_FRONT || m_MoveState == MS_FLY_LEFT_FRONT ||
					m_MoveState == MS_FLY_RIGHT_BACK || m_MoveState == MS_FLY_LEFT_BACK ||
					m_MoveState == MS_FLY_RIGHT_BACK_UP || m_MoveState == MS_FLY_LEFT_BACK_UP ||
					m_MoveState == MS_FLY_RIGHT_BACK_DOWN || m_MoveState == MS_FLY_LEFT_BACK_DOWN ) //|| m_MoveState == MS_WAIT)
				{
					//const float fMaxJumpUp = 282.f;
					//const float fMaxJumpRight = 94.f;

					if( fIsRight < 0.f )
					{
						// 목적지는 왼쪽에 있고, 진행방향은 오른쪽 뒤로 이동중이다.
						bool bLeftLand, bLeftUpLand, bLeftDownLand;
						bLeftLand = true;
						bLeftUpLand = bLeftDownLand = false;

						// 현재 라인
						if(pCurrLineData->nextLine < 0 || pLineMap->GetLineData( pCurrLineData->nextLine)->bEnable == false)
							bLeftLand = false;

						// 아래로 내려갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->endPos + vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {													
										    bLeftDownLand = true;
									    }	
								    } break;
							    }	
                            }
						}

						// 위로 올라갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp + vRightVec * fMaxJumpRight / pow(2.f, (float)i);
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}								

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bLeftLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // 대각선 오른쪽 jump
										    bLeftUpLand = true;
									    }
								    } break;
							    }
                            }
						}


						if( bLeftLand == false && bLeftUpLand == false && bLeftDownLand == false ) //
						{
							m_MoveState = MS_FLY_LEFT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
					else
					{
						// 목적지는 오른쪽에 있고, 진행방향은 왼쪽 뒤로 이동중이다.
						bool bRightLand, bRightUpLand, bRightDownLand;
						bRightLand = true;
						bRightUpLand = bRightDownLand = false;

						// 현재 라인
						if(pCurrLineData->beforeLine < 0 || pLineMap->GetLineData( pCurrLineData->beforeLine)->bEnable == false)
							bRightLand = false;

						// 아래로 내려갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vDownPos;
						vDownPos = pCurrLineData->startPos - vRightVec * LINE_RADIUS - vUpVec * LINE_RADIUS;
						vDownPos = pLineMap->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );
						if( m_iDestLineDataIndex != iCurrLineDataIndex )
						{
							const CKTDGLineMap::LineData*   pDestLineData = pLineMap->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    bRightDownLand = true;
									    }										
								    } break;
							    }
                            }
						}

						// 위로 올라갈수 있는 라인
						m_iDestLineDataIndex = iCurrLineDataIndex;
						D3DXVECTOR3 vJumpPos;
						for( int i=0; i<3; i++ )
						{
							vJumpPos = vCurrPos + vUpVec * fMaxJumpUp - vRightVec * fMaxJumpRight / pow(2.f, (float)i);									
							vJumpPos = pLineMap->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

							if( m_iDestLineDataIndex != iCurrLineDataIndex )
								break;
						}

						if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
						{
							const CKTDGLineMap::LineData* pDestLineData = pLineMap->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
							CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP					

                            if ( pDestLineData != NULL )
                            {
							    switch ( pDestLineData->lineType )
							    {
							    case CKTDGLineMap::LT_UNIT_ROAD:
							    case CKTDGLineMap::LT_JUMP_UP_ABS:
							    case CKTDGLineMap::LT_JUMP_UP_REL:
								    break;

							    default:
								    {
									    if( false == pDestLineData->m_bMonsterAllowed )
									    {
										    bRightLand = false;
										    return;
									    }

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
							            CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									    if( pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									    {
										    // 대각선 왼쪽 jump
										    bRightUpLand = true;
									    }
								    } break;
							    }
                            }
						}

						if( bRightLand == false && bRightUpLand == false && bRightDownLand == false ) //
						{
							m_MoveState = MS_FLY_RIGHT_FRONT;
							m_bFlyTurn = true;							
							return;
						}
					}
				}


				// 이동가능하다면 쭉~ 이동시킨다.
				if( fIsRight > 0.f )
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_RIGHT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_RIGHT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_RIGHT_BACK;
					}
				}
				else
				{
					if(bFlyFront == true)
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_FRONT_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_FRONT_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_FRONT;
					}
					else
					{
						if(bFlyUp == true)
							m_MoveState = MS_FLY_LEFT_BACK_UP;
						else if(bFlyDown == true)
							m_MoveState = MS_FLY_LEFT_BACK_DOWN;
						else
							m_MoveState = MS_FLY_LEFT_BACK;
					}
				}				
				return;
			}
		}
		break;
	default:
		m_MoveState = MS_WAIT;
		break;

	} // end of switch( m_MoveState )

} // end of OnFrameMove()
#endif
#endif // PET


void CX2SimplePathFinder::FollowTargetPathInBattleField( const CX2BattleFieldNpcAi* pBattleFieldNpcAi_, IN const CKTDGLineMap* pLineMap_, OUT bool& bTargetOnRight_ )
{
	KTDXPROFILE();

    int iFinalDestLineIndex = pBattleFieldNpcAi_->GetFinalDestLineIndex();
	const CKTDGLineMap::LineData* pFinalDestLineData = pLineMap_->GetLineData( iFinalDestLineIndex );
	
#ifdef UNIT_ROAD
	if( pFinalDestLineData != NULL )
	{
		switch ( pFinalDestLineData->lineType )
		{
		case CKTDGLineMap::LT_UNIT_ROAD:
		case CKTDGLineMap::LT_JUMP_UP_ABS:
		case CKTDGLineMap::LT_JUMP_UP_REL:
			m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
			return;
			break;

		default:
			break;
		}
	}
#endif

	switch( m_MoveState )
	{
	case MS_JUMP:
	case MS_DOWN:
	case MS_JUMP_LEFT:
	case MS_JUMP_RIGHT:
	case MS_DOWN_LEFT:
	case MS_DOWN_RIGHT:
		{
			if( true == pBattleFieldNpcAi_->GetFootOnLine() )
				m_MoveState = MS_WAIT;
		} break;

	case MS_WAIT:
	case MS_LEFT:
	case MS_RIGHT:
	case MS_FORCE_LEFT:
	case MS_FORCE_RIGHT:
		{
			const int iCurrLineDataIndex = pBattleFieldNpcAi_->GetLastTouchLineIndex();
			const CKTDGLineMap::LineData* pCurrLineData = pLineMap_->GetLineData( iCurrLineDataIndex );

			if(pCurrLineData == NULL)
				return;
			
#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            m_pCurrLineGroup = pCurrLineData->m_lineGroupID;
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			m_pCurrLineGroup = pLineMap_->GetLineGroupIncludesLineData( pCurrLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			const D3DXVECTOR3& vFinalDestPos = pBattleFieldNpcAi_->GetFinalDest();
			const D3DXVECTOR3& vCurrPos = pBattleFieldNpcAi_->GetNowPosition();

			D3DXVECTOR3 vFinalDestDir = vFinalDestPos - vCurrPos;
			const float fFinalDestDistSq = D3DXVec3LengthSq( &vFinalDestDir );
			D3DXVec3Normalize( &vFinalDestDir, &vFinalDestDir );

			D3DXVECTOR3 vRightVec = pCurrLineData->dirVector;
			D3DXVec3Normalize( &vRightVec, &vRightVec );

			float fIsRight = D3DXVec3Dot( &vRightVec, &vFinalDestDir );
			if( fIsRight > 0.f ) 
				bTargetOnRight_ = true;
			else
				bTargetOnRight_ = false;

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
            CKTDGLineMap::LineGroupID pFinalDestLineGroup;
            if ( pFinalDestLineData != NULL )
                pFinalDestLineGroup = pFinalDestLineData->m_lineGroupID; 
#else   X2OPTIMIZE_LINEMAP_LINEGROUP
			const CKTDGLineMap::LineGroup* pFinalDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pFinalDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

			if( pFinalDestLineGroup == m_pCurrLineGroup ) // 같은 line group
			{					
				const float fDestArriveGap = static_cast<float>( pBattleFieldNpcAi_->GetAIData()->chaseMoveData.destGap );

				if( fFinalDestDistSq < fDestArriveGap * fDestArriveGap ) // 목적지에 도착
				{
					m_MoveState = MS_WAIT;
					return;
				}
				else
				{
					const float fDestLeaveGap = static_cast<float>( pBattleFieldNpcAi_->GetAIData()->chaseMoveData.moveGap );

					if( MS_WAIT == m_MoveState && fFinalDestDistSq < fDestLeaveGap * fDestLeaveGap )
					{
						return;
					}

					if( fIsRight > 0.f )
					{
						m_MoveState = MS_RIGHT;
					}
					else 
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}		
			} 
			else // 같은 line group이 아니면
			{
				const D3DXVECTOR3 vUpVec(0,1,0);
				const float fIsUp    = D3DXVec3Dot( &vUpVec,    &vFinalDestDir );

				if( fIsUp > 0.f ) // 목적지가 위에 있는 경우
				{
					m_iDestLineDataIndex = iCurrLineDataIndex;
					
					D3DXVECTOR3 vJumpPos = vCurrPos + vUpVec * pBattleFieldNpcAi_->GetMaxJumpUp();
					vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

					const CKTDGLineMap::LineData* pLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );

					const float MAGIC_LINE_RADIUS_EXT = LINE_RADIUS * 1.5f;
					if( pLineData->beforeLine < 0 )
					{
						if( GetDistance3Sq( pLineData->startPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
						{
							m_iDestLineDataIndex = -1;
						}
					}

					if( pLineData->nextLine < 0 )
					{
						if( GetDistance3Sq( pLineData->endPos, vJumpPos ) < MAGIC_LINE_RADIUS_EXT * MAGIC_LINE_RADIUS_EXT )
						{
							m_iDestLineDataIndex = -1;
						}
					}

					if( -1 != m_iDestLineDataIndex && m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
					{
						const CKTDGLineMap::LineData* pDestLineData	= pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef  X2OPTIMIZE_LINEMAP_LINEGROUP
						CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
						if( pDestLineData != NULL )
#endif
						{
							switch ( pDestLineData->lineType )
							{
							case CKTDGLineMap::LT_UNIT_ROAD:
							case CKTDGLineMap::LT_JUMP_UP_ABS:
							case CKTDGLineMap::LT_JUMP_UP_REL:
								break;

							default:
								{
									if( false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( pDestLineGroup != m_pCurrLineGroup &&
										(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
									{
										// 수직 위로 jump
										m_MoveState = MS_JUMP;
										m_vDestPos = vJumpPos;
										return;
									}
								} break;
							}
						}

					}
				}
				else // 목적지가 아래에 있는 경우
				{
					m_iDestLineDataIndex = iCurrLineDataIndex;
					D3DXVECTOR3 vJumpPos = vCurrPos;
					vJumpPos -= vUpVec * LINE_RADIUS * 1.5f;
					vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

					if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
					{
						const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
						CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

#ifdef UNIT_ROAD
						if( pDestLineData != NULL )
#endif
						{   
							switch ( pDestLineData->lineType )
							{
							case CKTDGLineMap::LT_UNIT_ROAD:
							case CKTDGLineMap::LT_JUMP_UP_ABS:
							case CKTDGLineMap::LT_JUMP_UP_REL:
								break;

							default:
								{
									if( false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( pDestLineGroup != m_pCurrLineGroup &&
										(pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup) )
									{
										// 목적지 보다 높이가 낮으면 왠만하면 내려가지 않는다
										//if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState || vJumpPos.y >= vFinalDestPos.y )
										if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState )
										{
											// 수직 아래로 down
											m_MoveState = MS_DOWN;
											m_vDestPos = vJumpPos;
											return;
										}
									}
								} break;
							}
						}

					}
				}

				const float& MAGIC_COLLISION_BOUND = pBattleFieldNpcAi_->GetAIData()->chaseMoveData.m_fLineEndDetectRange;	// 라인 끝에 도착했는지 체크할 때 사용되는 범위 

				// 수직 위 아래로 jump 하거나 down 할 수 없으면 그냥 이동
				if( MS_FORCE_LEFT != m_MoveState && (MS_FORCE_RIGHT == m_MoveState || fIsRight > 0.1f) ) // 목적지가 오른쪽 
				{
					// 현재 라인맵에서 오른쪽으로 연결 된 것이 없는데
					if( pCurrLineData->nextLine < 0 || pLineMap_->GetLineData( pCurrLineData->nextLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineEnd = pCurrLineData->endPos - vCurrPos;
						const float fLineEndDist = D3DXVec3LengthSq( &vLineEnd );
						// 라인 끝에 도착 했으면
						if( fLineEndDist < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // 오른쪽으로 갈 수 없으면
						{	
							// 목적지가 현재 위치 보다 아래면
							if( vFinalDestPos.y < vCurrPos.y )
							{									
								// 그냥 걸어가면 내려갈 수 있는 발판이 있는지 확인
								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vDownPos = pCurrLineData->endPos;
								vDownPos += vRightVec * LINE_RADIUS;
								vDownPos -= vUpVec * LINE_RADIUS;
								vDownPos = pLineMap_->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );

								// 걸어 내려가도 되면
								if( m_iDestLineDataIndex != iCurrLineDataIndex )
								{
									const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
									CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

									if( pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{													
										m_MoveState = MS_FORCE_RIGHT;
										return;
									}										
								}
							}

							m_iDestLineDataIndex = iCurrLineDataIndex;
							D3DXVECTOR3 vJumpPos;
							for( int i = 0; i < 3; i++ )
							{
								vJumpPos = vCurrPos;
								vJumpPos += vUpVec * pBattleFieldNpcAi_->GetMaxJumpUp();
								vJumpPos += vRightVec * pBattleFieldNpcAi_->GetMaxJumpRight() / pow(2.f, (float)i);
								vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );
								
								if( m_iDestLineDataIndex != iCurrLineDataIndex )
									break;
							}								

							if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
							{
								const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
								CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

								if( pDestLineData == NULL ||
                                    false == pDestLineData->m_bMonsterAllowed )
								{
									m_MoveState = MS_WAIT;
									return;
								}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
								if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
									pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
								{
									// 대각선 오른쪽 jump
									m_MoveState = MS_JUMP_RIGHT;
									m_vDestPos = vJumpPos;
									return;
								}
							}

							// 오른쪽으로 갈 수도 없고 jump도 할 수 없으면 왼쪽으로 이동
							{
								switch( m_MoveState )
								{									
								case MS_LEFT:
								case MS_FORCE_LEFT:
									{
										m_MoveState = MS_FORCE_RIGHT;
									} break;

								case MS_WAIT:
								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									{
										m_MoveState = MS_FORCE_LEFT;
									} break;
								}
							}
						}							
					}

					// can go right
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_RIGHT;
					}
					else
					{
						m_MoveState = MS_LEFT;
					}
					return;
				}
				else if( MS_FORCE_RIGHT != m_MoveState && (MS_FORCE_LEFT == m_MoveState || fIsRight < -0.1f) ) // 목적지가 왼쪽
				{
					if( pCurrLineData->beforeLine < 0 || pLineMap_->GetLineData( pCurrLineData->beforeLine)->bEnable == false ) 
					{
						D3DXVECTOR3 vLineStart = pCurrLineData->startPos - vCurrPos;
						float fLineStartDist3Sq = D3DXVec3LengthSq( &vLineStart );
						if( fLineStartDist3Sq < MAGIC_COLLISION_BOUND * MAGIC_COLLISION_BOUND ) // 왼쪽으로 갈 수 없으면
						{
							if( vFinalDestPos.y < vCurrPos.y )
							{	
								// 그냥 걸어가면 내려갈 수 있는 발판이 있는지 확인
								m_iDestLineDataIndex = iCurrLineDataIndex;
								D3DXVECTOR3 vDownPos = pCurrLineData->startPos;
								vDownPos -= vRightVec * LINE_RADIUS;
								vDownPos -= vUpVec * LINE_RADIUS;
								vDownPos = pLineMap_->GetLandPosition( vDownPos, LINE_RADIUS, &m_iDestLineDataIndex );

								if( m_iDestLineDataIndex != iCurrLineDataIndex )
								{
									const CKTDGLineMap::LineData*   pDestLineData = pLineMap_->GetLineData( m_iDestLineDataIndex );
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
								    CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

								    if( pDestLineData == NULL ||
                                        false == pDestLineData->m_bMonsterAllowed )
									{
										m_MoveState = MS_WAIT;
										return;
									}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                    CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
									if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
										pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
									{
										m_MoveState = MS_FORCE_LEFT;
										return;
									}										
								}
							}

							m_iDestLineDataIndex = iCurrLineDataIndex;

							D3DXVECTOR3 vJumpPos;
							for( int i = 0; i < 3; i++ )
							{
								vJumpPos = vCurrPos;
								vJumpPos += vUpVec * pBattleFieldNpcAi_->GetMaxJumpUp();
								vJumpPos -= vRightVec * pBattleFieldNpcAi_->GetMaxJumpRight() / pow(2.f, (float)i);									
								vJumpPos = pLineMap_->GetLandPosition( vJumpPos, LINE_RADIUS, &m_iDestLineDataIndex );

								if( m_iDestLineDataIndex != iCurrLineDataIndex )
									break;
							}

							if( m_iDestLineDataIndex != iCurrLineDataIndex ) // jump해서 밟을 땅이 있으면
							{
								const CKTDGLineMap::LineData* pDestLineData = pLineMap_->GetLineData(m_iDestLineDataIndex);
#ifndef X2OPTIMIZE_LINEMAP_LINEGROUP
								CKTDGLineMap::LineGroup* pDestLineGroup = pLineMap_->GetLineGroupIncludesLineData( pDestLineData );
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP

								if( pDestLineData == NULL ||
                                    false == pDestLineData->m_bMonsterAllowed )
								{
									m_MoveState = MS_WAIT;
									return;
								}

#ifdef  X2OPTIMIZE_LINEMAP_LINEGROUP
                                CKTDGLineMap::LineGroupID pDestLineGroup = pDestLineData->m_lineGroupID;
#endif  X2OPTIMIZE_LINEMAP_LINEGROUP
								if( MS_FORCE_LEFT == m_MoveState || MS_FORCE_RIGHT == m_MoveState ||
									pDestLineGroup == pFinalDestLineGroup || pDestLineGroup != m_pPrevLineGroup )
								{
									// 대각선 왼쪽 jump
									m_MoveState = MS_JUMP_LEFT;
									m_vDestPos = vJumpPos;
									return;
								}
							}

							// 왼쪽으로 갈 수도 없고 jump도 할 수 없으면 오른쪽으로 이동
							{
								switch( m_MoveState )
								{
								case MS_WAIT:
								case MS_LEFT:
								case MS_FORCE_LEFT:
									m_MoveState = MS_FORCE_RIGHT;
									break;

								case MS_RIGHT:
								case MS_FORCE_RIGHT:
									m_MoveState = MS_FORCE_LEFT;
									break;
								}
							}
						}
					}

					// can go left
					if( MS_FORCE_RIGHT != m_MoveState && MS_FORCE_LEFT != m_MoveState )
					{					
						m_MoveState = MS_LEFT;
					}
					else
					{
						m_MoveState = MS_RIGHT;
					}
					return;
				}
				else // 목적지가 왼쪽도 오른쪽도 아닌 경우
				{
					// force move
					if( MS_LEFT == m_MoveState )
					{
						m_MoveState = MS_FORCE_LEFT;
					}
					else if( MS_RIGHT == m_MoveState )
					{
						m_MoveState = MS_FORCE_RIGHT;
					}
					else if( MS_WAIT == m_MoveState )
					{
						m_MoveState = rand()%2 ? MS_FORCE_LEFT : MS_FORCE_RIGHT;
					}
					return;
				}				
			} 
		}break;

	} // end of switch( m_MoveState )
}