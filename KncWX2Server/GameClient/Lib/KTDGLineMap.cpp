#include "..\KncRobotManager.h"
#include ".\ktdglinemap.h"

ImplementSingleton( CKTDGLineMap );

CKTDGLineMap::CKTDGLineMap()
{
	m_pLineMapVB	= NULL;

	m_LineList.reserve( 64 );
	m_RedTeamStartPos.reserve( 8 );
	m_BlueTeamStartPos.reserve( 8 );
	m_RedTeamStartRight.reserve( 8 );
	m_BlueTeamStartRight.reserve( 8 );
	m_RedTeamStartLineIndex.reserve( 8 );
	m_BlueTeamStartLineIndex.reserve( 8 );

	m_vecLineGroupList.reserve( 32 );
}

CKTDGLineMap::~CKTDGLineMap(void)
{
	SAFE_RELEASE( m_pLineMapVB );

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( pLineData != NULL )
		{
			delete pLineData;
			pLineData = NULL;
		}
	}
	m_LineList.clear();

	for( int i = 0; i < (int)m_WallList.size(); i++ )
	{
		LineData* pLineData = m_WallList[i];
		if( pLineData != NULL )
		{
			delete pLineData;
			pLineData = NULL;
		}
	}
	m_WallList.clear();


	m_RedTeamStartPos.clear();
	m_BlueTeamStartPos.clear();
	m_mapStartPos.clear();

	ReleaseLineGroup();
}

bool CKTDGLineMap::LoadLineMap( const char* pFileName )
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();
	
	//lua_tinker::class_add<D3DXVECTOR3>( spLua.get(), "D3DXVECTOR3" );
	//lua_tinker::class_con<D3DXVECTOR3>( spLua.get(), lua_tinker::constructor<float, float, float>() );
	//lua_tinker::class_mem<D3DXVECTOR3>( spLua.get(), "x", &D3DXVECTOR3::x );
	//lua_tinker::class_mem<D3DXVECTOR3>( spLua.get(), "y", &D3DXVECTOR3::y );
	//lua_tinker::class_mem<D3DXVECTOR3>( spLua.get(), "z", &D3DXVECTOR3::z );

	//lua_tinker::class_add<D3DXVECTOR3>( spLua.get(), "D3DXVECTOR3" );
	//lua_tinker::class_con<D3DXVECTOR3>( spLua.get(), lua_tinker::constructor<float, float, float>() );
	//lua_tinker::class_mem<D3DXVECTOR3>( spLua.get(), "x", &D3DXVECTOR3::x );
	//lua_tinker::class_mem<D3DXVECTOR3>( spLua.get(), "y", &D3DXVECTOR3::y );
	//lua_tinker::class_mem<D3DXVECTOR3>( spLua.get(), "z", &D3DXVECTOR3::z );

	lua_tinker::class_add<CKTDGLineMap>( spLua.get(), "CKTDGLineMap" );
	lua_tinker::class_def<CKTDGLineMap>( spLua.get(), "SetLandHeight",		&CKTDGLineMap::SetLandHeight );
	lua_tinker::class_def<CKTDGLineMap>( spLua.get(), "SetTeamStartPos",	&CKTDGLineMap::SetTeamStartPos );
	lua_tinker::class_def<CKTDGLineMap>( spLua.get(), "AddLine_LUA",		&CKTDGLineMap::AddLine_LUA );
	lua_tinker::decl( spLua.get(), "g_pLineMap", this );

	if( 0 != luaL_dofile( spLua.get(), pFileName ) ) return false;

	ReBuildLineGroup();

	return true;
}

bool CKTDGLineMap::AddLine_LUA()
{
	boost::shared_ptr<lua_State> spLua = SiKncRobotManager()->GetLuaState();

	KLuaManager luaManager( spLua.get() );
	//TableBind( &luaManager, g_pKTDXApp->GetLuaBinder() );
	
	LineData*	pLineData = new LineData();

	LUA_GET_VALUE_ENUM( luaManager, "LINE_TYPE",		pLineData->lineType,			LINE_TYPE, LT_NORMAL	);
	LUA_GET_VALUE( luaManager,		"ENABLE",			pLineData->bEnable,				true					);
	LUA_GET_VALUE( luaManager,		"POTAL_GROUP",		pLineData->potalGroup,			-1						);
	LUA_GET_VALUE( luaManager,		"STOP_FACTOR",		pLineData->fStopFactor,			1.0f					);
	LUA_GET_VALUE_ENUM( luaManager, "DUST_TYPE",		pLineData->eDustType,			LINE_DUST_TYPE, LDT_DEFALT_DUST	);

	
	LUA_GET_VALUE( luaManager,		"BEFORE_LINE",		pLineData->beforeLine,			-1						);
	LUA_GET_VALUE( luaManager,		"NEXT_LINE",		pLineData->nextLine,			-1						);
	LUA_GET_VALUE( luaManager,		"CAN_DOWN",			pLineData->bCanDown,			true					);
	LUA_GET_VALUE( luaManager,		"LINE_SET",			pLineData->lineSet,				-1						);
	LUA_GET_VALUE( luaManager,		"SPEED",			pLineData->m_fSpeed,			0.0f					);

	LUA_GET_VALUE( luaManager,		"JUMP_SPEED_X",		pLineData->m_vJumpSpeed.x,		0.f						);
	LUA_GET_VALUE( luaManager,		"JUMP_SPEED_Y",		pLineData->m_vJumpSpeed.y,		0.f						);

	LUA_GET_VALUE( luaManager,		"TELEPORT_LINE",	pLineData->m_iTeleportLineIndex,			-1	);


	LUA_GET_VALUE( luaManager,		"MONSTER_ALLOWED",		pLineData->m_bMonsterAllowed,				true			);
	LUA_GET_VALUE( luaManager,		"CAN_PASS",				pLineData->m_bCanPass,						true			);


	// lua script에 값이 없는 경우가 있으면 lua_tinker::get() 사용하면 안됨
#ifdef  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "START_POS", pLineData->startPos, D3DXVECTOR3(0,0,0) );
    LUA_GET_USER_DEFINED_TYPE_VALUE( luaManager, "END_POS", pLineData->endPos, D3DXVECTOR3(0,0,0) );
#else   X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	pLineData->startPos		= lua_tinker::get<D3DXVECTOR3>( spLua.get(),  "START_POS" );
	pLineData->endPos		= lua_tinker::get<D3DXVECTOR3>( spLua.get(),  "END_POS" );
#endif  X2OPTIMIZE_AVOID_LUA_RUNTIME_INTERPRETING
	//pLineData->m_vJumpSpeed	= lua_tinker::get<D3DXVECTOR3>( g_pKTDXApp->GetLuaBinder()->GetLuaState(),  "JUMP_SPEED" ); 

	MakeLine( pLineData );

	if( pLineData->lineType == LT_WALL )
	{
		m_WallList.push_back( pLineData );
	}
	else
	{
		m_LineList.push_back( pLineData );


		if( LT_BUNGEE != pLineData->lineType )
		{
			if( 1 == m_LineList.size() )
			{
				m_fXMin = pLineData->startPos.x;
				m_fXMax = pLineData->endPos.x;
				m_fYMin = pLineData->startPos.y;
				m_fYMax = pLineData->endPos.y;
			}

			m_fXMin = min( pLineData->startPos.x, m_fXMin );
			m_fXMin = min( pLineData->endPos.x, m_fXMin );
			m_fYMin = min( pLineData->startPos.y, m_fYMin );
			m_fYMin = min( pLineData->endPos.y, m_fYMin );

			m_fXMax = max( pLineData->startPos.x, m_fXMax );
			m_fXMax = max( pLineData->endPos.x, m_fXMax );
			m_fYMax = max( pLineData->startPos.y, m_fYMax );
			m_fYMax = max( pLineData->endPos.y, m_fYMax );
		}
	}

	return true;
}

void CKTDGLineMap::MakeLine( LineData* pLineData )
{
	float temp;
	D3DXMATRIX matRotationOrth;
	D3DXMATRIX matRotation;

	//D3DXVec3Normalize( &pLineData->dirVector, &(pLineData->endPos - pLineData->startPos) );
	//D3DXMatrixLookAtLH( &matRotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &pLineData->dirVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
	//D3DXMatrixInverse( &matRotationOrth, NULL, &matRotation );

	temp = matRotationOrth._11;
	matRotationOrth._11 = matRotationOrth._13;
	matRotationOrth._13 = temp;

	temp = matRotationOrth._21;
	matRotationOrth._21 = matRotationOrth._23;
	matRotationOrth._23 = temp;

	temp = matRotationOrth._31;
	matRotationOrth._31 = matRotationOrth._33;
	matRotationOrth._33 = temp;

	if( matRotationOrth._12 == 0 && matRotationOrth._22 == 0 )
	{
		pLineData->dirRadian.z	= 0.0f;
		pLineData->dirRadian.y	= atan2f( -matRotationOrth._31, matRotationOrth._33 );
		pLineData->dirRadian.x	= atan2f( -matRotationOrth._21, matRotationOrth._11 );

		pLineData->dirDegree.z	= 0.0f;
		pLineData->dirDegree.y	= D3DXToDegree(pLineData->dirRadian.y);
		pLineData->dirDegree.x	= D3DXToDegree(pLineData->dirRadian.x);
	}
	else
	{
		pLineData->dirRadian.z	= asinf( matRotationOrth._32 );
		pLineData->dirRadian.y	= atan2f( -matRotationOrth._31, matRotationOrth._33 );
		pLineData->dirRadian.x	= 0.0f;//D3DXToDegree(atan2f( -matRotationOrth._12, matRotationOrth._22 ));

		pLineData->dirDegree.z	= D3DXToDegree(pLineData->dirRadian.z);
		pLineData->dirDegree.y	= D3DXToDegree(pLineData->dirRadian.y);
		pLineData->dirDegree.x	= 0.0f;
	}

	pLineData->fLength		= GetDistance( pLineData->startPos, pLineData->endPos );
}

bool CKTDGLineMap::UpdateLineVB()
{
	if( NULL == m_pLineMapVB )
	{
		//MakeLineVB();
	}

	if( NULL == m_pLineMapVB )
	{
		return false;
	}

	HRESULT				hr;
	LINE_MAP_VERTEX*	pVertices = NULL;
	int					vbNum = (int)m_LineList.size() * 2;

    // D3DUSAGE_DYNAMIC으로 생성되지 않은 vertex buffer에 대해 D3DLOCK_DISCARD 사용 불가능
    // - jintaeks on 2008-10-15, 11:28
	if(FAILED(hr = m_pLineMapVB->Lock( 0, vbNum * sizeof(LINE_MAP_VERTEX), (void**) &pVertices, 0L/*D3DLOCK_DISCARD*/ )))
	{
		//ErrorLog( KEM_ERROR25 );
		return false;
	}
	{
		for( int i = 0; i < (int)m_LineList.size(); i++ )
		{
			LineData* pLineData = m_LineList[i];
			pVertices[(i*2)].pos		= pLineData->startPos;
			pVertices[(i*2)+1].pos		= pLineData->endPos;

			if( pLineData->bEnable == false )
			{
				pVertices[(i*2)].color		= 0xff000000;
				pVertices[(i*2)+1].color	= 0xff000000;
			}
			else
			{
				switch( pLineData->lineType )
				{
				default:
				case LT_NORMAL:
					{                        
#if 1
                        pVertices[(i*2)].color		= 0xffff0000;
                        pVertices[(i*2)+1].color	= 0xffff0000;
#else
                        // 월드툴용
                        if(pLineData->lineSet % 2 == 0)
                        {
                            pVertices[(i*2)].color		= 0xffff0000;
                            pVertices[(i*2)+1].color	= 0xffff0000;
                        }
                        else
                        {
                            pVertices[(i*2)].color		= 0xff00ff00;
                            pVertices[(i*2)+1].color	= 0xff00ff00;
                        }
#endif						
					}
					break;

				case LT_WALL:
					{
						pVertices[(i*2)].color		= 0xff00ff00;
						pVertices[(i*2)+1].color	= 0xff00ff00;
					}
					break;

				case LT_POTAL:
					{
						pVertices[(i*2)].color		= 0xff00ffff;
						pVertices[(i*2)+1].color	= 0xff00ffff;
					}
					break;

				case LT_HOLE:
					{
						pVertices[(i*2)].color		= 0xff0000ff;
						pVertices[(i*2)+1].color	= 0xff0000ff;
					}
					break;

				case LT_JUMP_UP_REL:
					{
						pVertices[(i*2)].color		= 0xffAAAA00;
						pVertices[(i*2)+1].color	= 0xffAAAA00;
					} break;

				case LT_JUMP_UP_ABS:
					{
						pVertices[(i*2)].color		= 0xffAA00AA;
						pVertices[(i*2)+1].color	= 0xffAA00AA;
					} break;

				case LT_BUNGEE:
					{
						pVertices[(i*2)].color		= 0xffAAAAAA;
						pVertices[(i*2)+1].color	= 0xffAAAAAA;
					} break;

#if 0   // 실제 라인맵상에서는 라인을 보여주지말자.
#ifdef MONSTER_ROAD
                case LT_MONSTER_ROAD:
                    {
                        pVertices[(i*2)].color		= 0xff00ff00;
                        pVertices[(i*2)+1].color	= 0xff00ff00;
                    } break;
#endif
#endif

#ifdef UNIT_ROAD
                case LT_UNIT_ROAD:
                    {
                        pVertices[(i*2)].color		= 0xffff0000;
                        pVertices[(i*2)+1].color	= 0xffff0000;
                    } break;
#endif
				}				
			}			
		}


		int nLineVertexCount = (int)m_LineList.size() * 2;
		for( int i = 0; i < (int)m_WallList.size(); i++ )
		{
			LineData* pLineData = m_WallList[i];
			pVertices[nLineVertexCount+(i*2)].pos		= pLineData->startPos;
			pVertices[nLineVertexCount+(i*2)+1].pos		= pLineData->endPos;

			if( pLineData->bEnable == false )
			{
				pVertices[nLineVertexCount+(i*2)].color		= 0xff000000;
				pVertices[nLineVertexCount+(i*2)+1].color	= 0xff000000;
			}
			else
			{
				switch( pLineData->lineType )
				{
				default:
				case LT_WALL:
					{
						pVertices[nLineVertexCount+(i*2)].color		= 0xff00ff00;
						pVertices[nLineVertexCount+(i*2)+1].color	= 0xff00ff00;
					}
					break;
				}				
			}			
		}

	}
	if(FAILED(hr = m_pLineMapVB->Unlock()))
	{
		//ErrorLog( KEM_ERROR26 );
		return false;
	}
	return true;
}


D3DXVECTOR3 CKTDGLineMap::GetLandPosition( D3DXVECTOR3 pos, float fRadius, int* pLineIndex )
{
	D3DXVECTOR3 zeroPos, pointL1, pointL2, retVal;

	zeroPos		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL1		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL2		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	retVal		= D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
	int lineIndex = -1;
	LineData* pLineData = NULL;

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		pLineData = m_LineList[i];
		if( false == pLineData->bEnable )
			continue;

		if( pos.y + fRadius < pLineData->startPos.y && pos.y + fRadius < pLineData->endPos.y )
			continue;

		if( (pos.x < pLineData->startPos.x && pos.x < pLineData->endPos.x)
			|| (pos.x > pLineData->startPos.x && pos.x > pLineData->endPos.x) )
		{
			if( (pos.y < pLineData->startPos.y && pos.y < pLineData->endPos.y)
				|| (pos.y > pLineData->startPos.y && pos.y > pLineData->endPos.y) )
			{
				if( (pos.z < pLineData->startPos.z && pos.z < pLineData->endPos.z)
					|| (pos.z > pLineData->startPos.z && pos.z > pLineData->endPos.z) )
				{
					continue;
				}
			}
		}

		zeroPos		= pos;
		zeroPos.y	= 0.0f;
		//if( g_pKTDXApp->GetCollision()->LineToLine( pos, zeroPos, pLineData->startPos, pLineData->endPos, fRadius, pointL1, pointL2 ) == true )
		//{
		//	if( pointL2.y > retVal.y )
		//	{
		//		lineIndex	= i;
		//		retVal		= pointL2;
		//	}
		//}
	}

	if( lineIndex == -1 )
	{
		if( pLineIndex != NULL )
			lineIndex = *pLineIndex;

		pLineData = GetLineData(lineIndex);
		if ( pLineData != NULL )
		{
			float fDistToStart = GetDistance( pos, pLineData->startPos );
			float fDistToEnd = GetDistance( pos, pLineData->endPos );

			if( fDistToStart > fDistToEnd )
			{
				retVal = pLineData->endPos;
			}
			else
			{
				retVal = pLineData->startPos;
			}
		}
	}
	if( lineIndex == -1 )
	{
		float nearPoint = 999999.0f;
		for( int i = 0; i < (int)m_LineList.size(); i++ )
		{
			pLineData = m_LineList[i];
			if( pLineData->bEnable == true )
			{
				float fDistToStart = GetDistance( pos, pLineData->startPos );
				float fDistToEnd = GetDistance( pos, pLineData->endPos );
				if( fDistToStart < nearPoint )
				{
					nearPoint = fDistToStart;
					lineIndex = i;
				}
				if( fDistToEnd < nearPoint )
				{
					nearPoint = fDistToEnd;
					lineIndex = i;
				}
			}			
		}
	}

	if( pLineIndex != NULL )
		*pLineIndex = lineIndex;

	return retVal;
}





bool CKTDGLineMap::CanDown( D3DXVECTOR3 pos, float fRadius, D3DXVECTOR3* pOutPos, int* pLineIndex, bool bDownRadius, bool bIsMonster, bool bOnLine )
{	
	D3DXVECTOR3 zeroPos, pointL1, pointL2, retVal;
	if( bDownRadius == true )
		pos.y -= fRadius + 1.0f;
	zeroPos		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL1		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL2		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	retVal		= D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
	int line	= -1;
	bool touch	= false;

	if( bOnLine == true )	// 현재 line을 밟고 있고, 그 line이 밑으로 못 내려가는 line이라면 
	{
		if( pLineIndex != NULL )
		{
			LineData* pLine = m_LineList[*pLineIndex];
			if( pLine->bCanDown == false )
			{
				*pOutPos = pos;
				return false;
			}
		}
	}

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		LineData* pLine = m_LineList[i];
		if( false == pLine->bEnable )
			continue;

		if( pos.y < pLine->startPos.y && pos.y < pLine->endPos.y )
			continue;

		if( (pos.x < pLine->startPos.x && pos.x < pLine->endPos.x)
			|| (pos.x > pLine->startPos.x && pos.x > pLine->endPos.x) )
		{
			if( (pos.y < pLine->startPos.y && pos.y < pLine->endPos.y)
				|| (pos.y > pLine->startPos.y && pos.y > pLine->endPos.y) )
			{
				if( (pos.z < pLine->startPos.z && pos.z < pLine->endPos.z)
					|| (pos.z > pLine->startPos.z && pos.z > pLine->endPos.z) )
				{
					continue;
				}
			}
		}

		zeroPos		= pos;
		zeroPos.y	= 0.0f;
		//if( g_pKTDXApp->GetCollision()->LineToLine( pos, zeroPos, pLine->startPos, pLine->endPos, fRadius, pointL1, pointL2 ) == true )
		//{
		//	if( pointL2.y >= retVal.y ) //충돌된 라인의 높이가 더 높으면
		//	{
		//		if( GetDistance( pointL2, pLine->startPos ) < 0.001f )
		//		{
		//			if( pLine->beforeLine != -1
		//				&& pLine->beforeLine != -2 &&
		//				m_LineList[ pLine->beforeLine ]->bEnable == true ) //시작 지점에 연결라인이 있으면
		//			{
		//				//연결라인으로 보정한다
		//				//line	= m_LineList[i]->beforeLine;
		//				//g_pKTDXApp->GetCollision()->LineToLine( pos, zeroPos, m_LineList[line]->startPos, m_LineList[line]->endPos, fRadius, pointL1, pointL2 );

		//				//retVal	= pointL2;
		//			}
		//			else
		//			{
		//				//아직 터치되지 않았으면 보정한다
		//				if( touch == false )
		//				{
		//					retVal	= pointL2;
		//					line	= i;
		//				}
		//			}
		//		}
		//		else if( GetDistance( pointL2, pLine->endPos ) < 0.001f )
		//		{
		//			if( pLine->nextLine != -1
		//				&& pLine->nextLine != -2 && 
		//				m_LineList[ pLine->nextLine ]->bEnable == true ) //AI¾iAø ¼±AI AO´A°¡?
		//			{
		//				//AO´U¸e AI¾iAø ¶oAIA¸·I ¿A±a´U
		//				line	= m_LineList[i]->nextLine;
		//				g_pKTDXApp->GetCollision()->LineToLine( pos, zeroPos, m_LineList[line]->startPos, m_LineList[line]->endPos, fRadius, pointL1, pointL2 );

		//				retVal	= pointL2;
		//			}
		//			else
		//			{
		//				//¾ø´U¸e A¹¹øA° AIA¡AI °æ¿i Au¿eCN´U.
		//				if( touch == false )
		//				{
		//					retVal	= pointL2;
		//					line	= i;
		//				}
		//			}
		//		}
		//		else
		//		{
		//			retVal	= pointL2;
		//			line	= i;
		//		}

		//	}
		//	else ////충돌된 라인의 높이가 더 낮으면
		//	{
		//		if( GetDistance( m_LineList[line]->startPos, retVal ) < 0.001f )
		//		{
		//			if( m_LineList[line]->beforeLine == -1
		//				&& m_LineList[line]->beforeLine != -2 )
		//			{
		//				line	= i;
		//				retVal	= pointL2;						
		//			}
		//			else if( m_LineList[line]->beforeLine != -1
		//				&& m_LineList[line]->beforeLine != -2 )
		//			{
		//				CKTDGLineMap::LineData* pBeforeLine = m_LineList[ m_LineList[line]->beforeLine ];
		//				if( NULL != pBeforeLine && true == pBeforeLine->bEnable )
		//				{
		//					line	= m_LineList[line]->beforeLine;
		//					retVal	= m_LineList[line]->endPos;
		//				}
		//				else
		//				{
		//					line = i;
		//					retVal = pointL2;
		//				}
		//			}
		//		}
		//		else if( GetDistance( m_LineList[line]->endPos, retVal ) < 0.001f )
		//		{
		//			if( m_LineList[line]->nextLine == -1
		//				&& m_LineList[line]->nextLine != -2 )
		//			{
		//				line	= i;
		//				retVal	= pointL2;
		//			}
		//			else if( m_LineList[line]->nextLine != -1
		//				&& m_LineList[line]->nextLine != -2 )
		//			{
		//				CKTDGLineMap::LineData* pNextLine = m_LineList[ m_LineList[line]->nextLine ];
		//				if( NULL != pNextLine && true == pNextLine->bEnable )
		//				{
		//					line	= m_LineList[line]->nextLine;
		//					retVal	= m_LineList[line]->startPos;
		//				}
		//				else
		//				{
		//					line = i;
		//					retVal = pointL2;
		//				}
		//			}					
		//		}			
		//	}
		//	touch = true;
		//}
	}

	if( touch == false && pLineIndex != NULL )
	{
		float dist1 = GetDistance( m_LineList[*pLineIndex]->startPos, pos );
		float dist2 = GetDistance( m_LineList[*pLineIndex]->endPos, pos );

		if( dist1 < dist2 )
		{
			if( m_LineList[*pLineIndex]->beforeLine != -1
				&& m_LineList[*pLineIndex]->beforeLine != -2
				&& dist1 < fRadius )
			{
				retVal	= m_LineList[*pLineIndex]->startPos;
				line	= m_LineList[*pLineIndex]->beforeLine;
				touch	= true;
			}
		}
		else
		{
			if( m_LineList[*pLineIndex]->nextLine != -1
				&& m_LineList[*pLineIndex]->nextLine != -2
				&& dist2 < fRadius )
			{
				retVal	= m_LineList[*pLineIndex]->endPos;
				line	= m_LineList[*pLineIndex]->nextLine;
				touch	= true;
			}
		}
	}

	if( touch == true )
	{
		if( pOutPos != NULL )
			*pOutPos = retVal;
		if( pLineIndex != NULL )
			*pLineIndex = line;
	}

	if( pos.y < 0.0f )
		return false;

	return touch;
}




bool CKTDGLineMap::CanPassUp( D3DXVECTOR3 pos )
{
	D3DXVECTOR3 zeroPos, pointL1, pointL2, retVal;
	zeroPos		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL1		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pointL2		= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	retVal		= D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
	bool bFound = false;

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		LineData* pLine = m_LineList[i];
		if( false == pLine->bEnable )
			continue;

		if( true == pLine->m_bCanPass )
			continue;

		if( (pos.x < pLine->startPos.x && pos.x < pLine->endPos.x)
			|| (pos.x > pLine->startPos.x && pos.x > pLine->endPos.x) )
		{
			if( (pos.y < pLine->startPos.y && pos.y < pLine->endPos.y)
				|| (pos.y > pLine->startPos.y && pos.y > pLine->endPos.y) )
			{
				if( (pos.z < pLine->startPos.z && pos.z < pLine->endPos.z)
					|| (pos.z > pLine->startPos.z && pos.z > pLine->endPos.z) )
				{
					continue;
				}
			}
		}

		zeroPos		= pos;
		zeroPos.y	+= 50.0f;
		//if( g_pKTDXApp->GetCollision()->LineToLine( zeroPos, pos, pLine->startPos, pLine->endPos, 50.f, pointL1, pointL2 ) == true )
		//{
		//	if( retVal.y == -1.f || pointL2.y < retVal.y ) 
		//	{
		//		if( pointL2.y > pos.y )
		//		{
		//			bFound = true;
		//			retVal = pointL2;
		//		}
		//	}
		//}
	}

	if( true == bFound && GetDistance( pos, retVal ) < 50.f )
	{
		return false;
	}

	return true;
}

CKTDGLineMap::LineData* CKTDGLineMap::GetLineData( int index )
{ 
	if ( index < 0 || index >= (int)m_LineList.size() )
	{
		return NULL;
	}

	return m_LineList[index]; 
}

float CKTDGLineMap::GetDownNearest( D3DXVECTOR3 inPos, D3DXVECTOR3* pOutPos, int* pLlineIndex )
{	
	D3DXVECTOR3 target;
	float		fProjectionLength;
	float		outLength = 999999;
	float		tempLength;
	D3DXVECTOR3	tempPos;

	//*lineIndex = -1;

	for( int i = 0; i < (int)m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( false == pLineData->bEnable )
			continue;

		if( inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y )
			continue;

		if( (inPos.x < pLineData->startPos.x && inPos.x < pLineData->endPos.x)
			|| (inPos.x > pLineData->startPos.x && inPos.x > pLineData->endPos.x) )
		{
			if( (inPos.y < pLineData->startPos.y && inPos.y < pLineData->endPos.y)
				|| (inPos.y > pLineData->startPos.y && inPos.y > pLineData->endPos.y) )
			{
				if( (inPos.z < pLineData->startPos.z && inPos.z < pLineData->endPos.z)
					|| (inPos.z > pLineData->startPos.z && inPos.z > pLineData->endPos.z) )
				{
					continue;
				}
			}
		}

		target				= inPos - pLineData->startPos;
		fProjectionLength	= D3DXVec3Dot( &pLineData->dirVector, &target );
		if( fProjectionLength >= 0.0f && fProjectionLength < pLineData->fLength )
		{
			tempPos		= fProjectionLength * pLineData->dirVector + pLineData->startPos;
			tempLength	= GetDistance( tempPos, inPos );
			if( tempLength < outLength )
			{
				outLength		= tempLength;
				*pOutPos		= tempPos;
				*pLlineIndex	= i;
			}
		}
	}
	return outLength;
}




void CKTDGLineMap::ReleaseLineGroup()
{	
	for( unsigned i=0; i<m_vecLineGroupList.size(); i++ )
	{
		SAFE_DELETE( m_vecLineGroupList[i] );
	}

	m_vecLineGroupList.resize( 0 );
	m_mapLineData2LineGroup.clear();
}

CKTDGLineMap::LineGroup* CKTDGLineMap::GetLineGroupIncludesLineData( CKTDGLineMap::LineData* pLineData )
{	
	if( NULL == pLineData )
		return NULL;

	std::map<LineData*, LineGroup*>::iterator it;
	it = m_mapLineData2LineGroup.find( pLineData );
	if( it != m_mapLineData2LineGroup.end() )
		return it->second;
	else
		return NULL;
}

// fix!! 이 코드 왠지 계산시간 많이 걸릴것 같은데...
// linegroup로 vector가 아닌 map에 저장해야? 
void CKTDGLineMap::EnableLineData( CKTDGLineMap::LineData* pLineData, bool bEnable )
{	
	if( pLineData->bEnable == bEnable )
		return;
	pLineData->bEnable = bEnable;

	LineData* pBeforeLine = GetLineData( pLineData->beforeLine );
	LineData* pNextLine = GetLineData( pLineData->nextLine );

	std::vector<LineGroup*>::iterator it;
	if( true == bEnable ) // disable --> enable 
	{
		if( NULL != pBeforeLine && NULL != pNextLine )
		{
			LineGroup* pBeforeLineGroup = m_mapLineData2LineGroup[ pBeforeLine ];
			LineGroup* pNextLineGroup = m_mapLineData2LineGroup[ pNextLine ];

			if( pBeforeLineGroup->vecpLineData.size() < pNextLineGroup->vecpLineData.size() )
			{
				LineGroup* pTemp = pBeforeLineGroup;
				pBeforeLineGroup = pNextLineGroup;
				pNextLineGroup = pTemp;				
			}

			// 두 line group을 합한다
			pBeforeLineGroup->vecpLineData.push_back( pLineData );
			m_mapLineData2LineGroup[ pLineData ] = pBeforeLineGroup;

			pBeforeLineGroup->vecpLineData.insert( pBeforeLineGroup->vecpLineData.begin(), 
				pNextLineGroup->vecpLineData.begin(), pNextLineGroup->vecpLineData.end() );
			for( unsigned i=0; i<pNextLineGroup->vecpLineData.size(); i++ )
			{
				m_mapLineData2LineGroup[ pNextLineGroup->vecpLineData[i] ] = pBeforeLineGroup;
			}

			for( it = m_vecLineGroupList.begin(); it != m_vecLineGroupList.end() ; it++ )
			{
				if( *it == pNextLineGroup )
				{
					it = m_vecLineGroupList.erase(it);
					break;						
				}
			}
			SAFE_DELETE( pNextLineGroup );
		}
		else if( NULL != pBeforeLine )
		{
			LineGroup* pBeforeLineGroup = m_mapLineData2LineGroup[ pBeforeLine ];

			pBeforeLineGroup->vecpLineData.push_back( pLineData );
			m_mapLineData2LineGroup[ pLineData ] = pBeforeLineGroup;
		}
		else if( NULL != pNextLine )
		{
			LineGroup* pNextLineGroup = m_mapLineData2LineGroup[ pBeforeLine ];

			pNextLineGroup->vecpLineData.push_back( pLineData );
			m_mapLineData2LineGroup[ pLineData ] = pNextLineGroup;			
		}
		else
		{
			LineGroup* pNewLineGroup = new LineGroup;
			pNewLineGroup->vecpLineData.push_back( pLineData );
			m_vecLineGroupList.push_back( pNewLineGroup );
			m_mapLineData2LineGroup[ pLineData ] = pNewLineGroup;
		}
	}
	else // enable --> disable 
	{
		if( NULL != pBeforeLine && NULL != pNextLine )
		{
			LineGroup* pCurrLineGroup = m_mapLineData2LineGroup[ pLineData ];
			m_mapLineData2LineGroup[ pLineData ] = NULL;

			// 2개의 linegroup으로 분리
			LineData* pTempLine = NULL;
			
			LineGroup* pBeforeLineGroup = new LineGroup;
			LineGroup* pNextLineGroup = new LineGroup;
			while( NULL != pBeforeLine )
			{
				pBeforeLineGroup->vecpLineData.push_back( pBeforeLine );
				m_mapLineData2LineGroup[ pBeforeLine ] = pBeforeLineGroup;
				pBeforeLine = GetLineData( pBeforeLine->beforeLine );
			}
			
			while( NULL != pNextLine )
			{
				pNextLineGroup->vecpLineData.push_back( pNextLine );
				m_mapLineData2LineGroup[ pNextLine ] = pNextLineGroup;
				pNextLine = GetLineData( pNextLine->nextLine );
			}			

			for( it = m_vecLineGroupList.begin(); it != m_vecLineGroupList.end() ; it++ )
			{
				if( *it == pCurrLineGroup )
				{
					it = m_vecLineGroupList.erase(it);
					break;						
				}
			}
			SAFE_DELETE( pCurrLineGroup );

			m_vecLineGroupList.push_back( pBeforeLineGroup );
			m_vecLineGroupList.push_back( pNextLineGroup );
		}
		else if( NULL != pBeforeLine )
		{
			m_mapLineData2LineGroup[ pLineData ] = NULL;
		}
		else if( NULL != pNextLine )
		{
			m_mapLineData2LineGroup[ pLineData ] = NULL;
		}
		else 
		{
			LineGroup* pCurrLineGroup = m_mapLineData2LineGroup[ pLineData ];
			for( it = m_vecLineGroupList.begin(); it != m_vecLineGroupList.end() ; it++ )
			{
				if( *it == pCurrLineGroup )
				{
					it = m_vecLineGroupList.erase(it);
					break;						
				}
			}
			SAFE_DELETE( pCurrLineGroup );
			m_mapLineData2LineGroup[ pLineData ] = NULL;
		}
	}
}


void CKTDGLineMap::EnableLineSet( int iLineSetIndex, bool bEnable )	// fix!! 이 함수 렉 걸리는지 확인해야됨
{	
	if( iLineSetIndex < 0 )
		return;

	for( unsigned int i = 0; i < m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( iLineSetIndex == pLineData->lineSet )
		{
			pLineData->bEnable = bEnable;
		}
	}
	ReBuildLineGroup();


	// fix!! 일단 wall list는 아무런 다른 작업을 하지 않고 두자
	//for( UINT i=0; i<m_WallList.size(); i++ )
	//{
	//	LineData* pLineData = m_WallList[i];
	//	if( iLineSetIndex == pLineData->lineSet )
	//	{
	//		pLineData->bEnable = bEnable;
	//	}
	//}

	//for( unsigned int i = 0; i < m_LineList.size(); i++ )
	//{
	//	LineData* pLineData = m_LineList[i];
	//	if( iLineSetIndex == pLineData->lineSet )
	//	{
	//		this->EnableLineData( pLineData, bEnable );
	//		//pLineData.bEnable = bEnable;
	//	}
	//}
}

void CKTDGLineMap::DisableAllLineData()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		pLineData->bEnable = false;
	}

	ReBuildLineGroup();
}

void CKTDGLineMap::ReBuildLineGroup()
{	
	CKTDGLineMap* pLineMap = this;

	if( NULL == pLineMap )
		return;

	int nNodes = pLineMap->GetNumLineData();
	if( nNodes <= 0 )
		return;

	bool* bDoneLineData = new bool[nNodes];
	for( int i=0; i<nNodes; i++ )
		bDoneLineData[i] = false;

	ReleaseLineGroup();

	// line group 생성
	for( int i=0; i<nNodes; i++ )
	{
		if( bDoneLineData[i] == true )
			continue;

		LineData* pLineData = pLineMap->GetLineData(i);
        if( false == pLineData->bEnable )		
		{
			continue;
		}

		bDoneLineData[i] = true;
		LineGroup* pGroup = new LineGroup();		
		pGroup->vecpLineData.push_back( pLineData );
		pGroup->vStartPos = pLineData->startPos;
		pGroup->vEndPos = pLineData->endPos;

		LineData* pTempLineData = pLineData;
		while( true )
		{			
			if( pTempLineData->nextLine >= 0 && false == bDoneLineData[pTempLineData->nextLine] &&
				pTempLineData->bEnable == true )
			{	
				bDoneLineData[pTempLineData->nextLine] = true;
				pTempLineData = pLineMap->GetLineData( pTempLineData->nextLine );
				pGroup->vecpLineData.push_back( pTempLineData );
				pGroup->vEndPos = pTempLineData->endPos;
			}
			else
			{
				break;
			}				
		}

		pTempLineData = pLineData;
		while( true )
		{			
			if( pTempLineData->beforeLine >= 0 && false == bDoneLineData[pTempLineData->beforeLine] &&
				pTempLineData->bEnable == true )
			{						
				bDoneLineData[pTempLineData->beforeLine] = true;
				pTempLineData = pLineMap->GetLineData( pTempLineData->beforeLine );
				pGroup->vecpLineData.push_back( pTempLineData );
				pGroup->vStartPos = pTempLineData->startPos;
			}
			else
			{
				break;
			}				
		}

		m_vecLineGroupList.push_back( pGroup );
	}

	m_mapLineData2LineGroup.clear();
	int nLineGroups = (int)m_vecLineGroupList.size(); 
	for( int i=0; i<nLineGroups; i++ )
	{
		LineGroup* pGroup = m_vecLineGroupList.at(i);
		int nLineDataInANode = (int)pGroup->vecpLineData.size();

		for(int j=0; j<nLineDataInANode; j++ )
		{					
			LineData* pLineData = pGroup->vecpLineData.at(j);
			m_mapLineData2LineGroup.insert( std::make_pair(pLineData, pGroup) );
		}
	}

	SAFE_DELETE_ARRAY(bDoneLineData);			
}




// fix!! getlandposition이 실패하는 경우가 많을 수 있다
D3DXVECTOR3 CKTDGLineMap::GetRandomPosition( D3DXVECTOR3* pvPos, float fRange, bool bOnThisLineGroup )
{
	int iLineIndex = -1;

	const float MAGIC_LINE_RADIUS = 30.f;

	if( NULL == pvPos )
	{
		int iRandLineIndex = rand() % GetNumLineData();
		LineData* pLineData = GetLineData( iRandLineIndex );
		
		
		if( pLineData == NULL || pLineData->bEnable == false || pLineData->lineType != LT_NORMAL)
		{
			int iTempRandIndex = iRandLineIndex+1;
			for( ; iTempRandIndex != iRandLineIndex; iTempRandIndex++ )
			{
				if( iTempRandIndex > GetNumLineData()-1 )
				{
					iTempRandIndex = 0;
				}

				pLineData = GetLineData( iTempRandIndex );
                
                if(pLineData != NULL && pLineData->bEnable == true && pLineData->lineType == LT_NORMAL)
                    break;
			}
		}

		float fLerpCoeff = (float)(rand()%10001) / 10000.f; 		
		D3DXVECTOR3 vResultPos = (1.f-fLerpCoeff) * pLineData->startPos + fLerpCoeff * pLineData->endPos;		

		return vResultPos;	
	}
	else
	{
		D3DXVECTOR3 vPos = GetLandPosition( *pvPos, MAGIC_LINE_RADIUS, &iLineIndex );
		if( -1 == iLineIndex )
			return *pvPos;	// 아마 error

		LineData* pLineData = GetLineData( iLineIndex );
		LineGroup* pLineGroup = this->GetLineGroupIncludesLineData( pLineData );

		if( false == bOnThisLineGroup )
		{
			int iResultLineIndex = -1;
			D3DXVECTOR3 vResultPos;
			bool bDone = false;
			while( false == bDone )
			{
				float fRandX = (float)(rand()%10001 - 5000) / 5000.f; 
				float fRandY = (float)(rand()%10001 - 5000) / 5000.f; 
				float fRandZ = (float)(rand()%10001 - 5000) / 5000.f;
				D3DXVECTOR3 vDir = D3DXVECTOR3( fRandX, fRandY, fRandZ );
				//D3DXVec3Normalize( &vDir, &vDir );
				vResultPos = vPos + vDir * fRange;
				vResultPos = GetLandPosition( vResultPos, MAGIC_LINE_RADIUS, &iResultLineIndex );
				if( -1 != iResultLineIndex )
				{
					bDone = true;
				}
			}
			return vResultPos;
		}
		else
		{
			float fRandDist = (float)(rand()%10001) / 10000.f * fRange;
			int IsRight = rand()%2;
			if( 0 == IsRight ) // 왼쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->startPos );
				LineData* pTempLineData = pLineData;
				while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
			}
			else // 오른쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->endPos );
				LineData* pTempLineData = pLineData;
				while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->nextLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->nextLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}		
			}
		}		
	}



	// 여기까지 오면 에러 
	return D3DXVECTOR3(0,0,0);
}



D3DXVECTOR3 CKTDGLineMap::GetRandomPositionDir( D3DXVECTOR3* pvPos, float fRange, bool bOnThisLineGroup, bool bIsRight )
{
	int iLineIndex = -1;

	const float MAGIC_LINE_RADIUS = 30.f;

	if( NULL == pvPos )
	{
		// 여기는 아직 bIsRight 구현 안됨

		int iRandLineIndex = rand() % GetNumLineData();
		LineData* pLineData = GetLineData( iRandLineIndex );


		if( pLineData == NULL || pLineData->bEnable == false || pLineData->lineType != LT_NORMAL)
		{
			int iTempRandIndex = iRandLineIndex+1;
			for( ; iTempRandIndex != iRandLineIndex; iTempRandIndex++ )
			{
				if( iTempRandIndex > GetNumLineData()-1 )
				{
					iTempRandIndex = 0;
				}

				pLineData = GetLineData( iTempRandIndex );
                if(pLineData != NULL && pLineData->bEnable == true && pLineData->lineType == LT_NORMAL) 
                    break;
			}
		}


		float fLerpCoeff = (float)(rand()%10001) / 10000.f; 		
		D3DXVECTOR3 vResultPos = (1.f-fLerpCoeff) * pLineData->startPos + fLerpCoeff * pLineData->endPos;		

		return vResultPos;	
	}
	else
	{
		D3DXVECTOR3 vPos = GetLandPosition( *pvPos, MAGIC_LINE_RADIUS, &iLineIndex );
		if( -1 == iLineIndex )
			return *pvPos;	// 아마 error

		LineData* pLineData = GetLineData( iLineIndex );
		LineGroup* pLineGroup = this->GetLineGroupIncludesLineData( pLineData );
		

		if( false == bOnThisLineGroup )
		{
			int iResultLineIndex = -1;
			D3DXVECTOR3 vResultPos;
			bool bDone = false;
			while( false == bDone )
			{
				float fRandX = (float)(rand()%10001) / 10000.f;
				float fRandY = (float)(rand()%10001 - 5000) / 5000.f;
				float fRandZ = (float)(rand()%10001) / 10000.f;
				D3DXVECTOR3 vDir = pLineData->dirVector;
				if( false == bIsRight )
				{
					vDir.x = -vDir.x;
					vDir.z = -vDir.z;
				}

				vDir.x *= fRandX;
				vDir.z *= fRandZ;
				vDir.y = fRandY;
				//D3DXVec3Normalize( &vDir, &vDir );				
				vResultPos = vPos + vDir * fRange;
				vResultPos = GetLandPosition( vResultPos, MAGIC_LINE_RADIUS, &iResultLineIndex );
				if( -1 != iResultLineIndex )
				{
					bDone = true;
				}
			}
			return vResultPos;
		}
		else
		{
			float fRandDist = (float)(rand()%10001) / 10000.f * fRange;
			if( false == bIsRight ) // 왼쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->startPos );

				if( fWalkDist > fRandDist )
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pLineData->startPos, *pvPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pLineData->startPos + fLerp * (*pvPos);
					return vResultPos;
				}

				
				LineData* pTempLineData = pLineData;
				while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->beforeLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->beforeLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = fLerp * pTempLineData->startPos + (1.f-fLerp) * pTempLineData->endPos;
					return vResultPos;
				}
			}
			else // 오른쪽으로
			{
				float fWalkDist = GetDistance( *pvPos, pLineData->endPos );

				if( fWalkDist > fRandDist )
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pLineData->endPos, *pvPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pLineData->endPos + fLerp * (*pvPos);

					return vResultPos;
				}

				LineData* pTempLineData = pLineData;
				while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
				{
					pTempLineData = this->GetLineData( pTempLineData->nextLine );					
					fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
				}

				if( fWalkDist < fRandDist )
				{
					fRandDist = (float)(rand()%10001) / 10000.f * fWalkDist;
					fWalkDist = GetDistance( *pvPos, pLineData->startPos );
					pTempLineData = pLineData;
					while( pTempLineData->nextLine >= 0 && fWalkDist < fRandDist )
					{
						pTempLineData = this->GetLineData( pTempLineData->nextLine );					
						fWalkDist += GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					}

					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}
				else
				{
					float fLerp = (fWalkDist - fRandDist) / GetDistance( pTempLineData->startPos, pTempLineData->endPos );
					D3DXVECTOR3 vResultPos = (1.f-fLerp) * pTempLineData->startPos + fLerp * pTempLineData->endPos;
					return vResultPos;
				}		
			}
		}		
	}



	// 여기까지 오면 에러 
	return D3DXVECTOR3(0,0,0);
}

D3DXVECTOR3 CKTDGLineMap::GetNearest_LUA(D3DXVECTOR3 inPos)
{
    D3DXVECTOR3 outPos;

    GetNearest(inPos, &outPos);

    return outPos;
}

int CKTDGLineMap::GetLineSetIndex( int lineIndex )
{
	LineData* pLineData = GetLineData( lineIndex );
	if( pLineData == NULL )
		return -1;

	return pLineData->lineSet;
}



int CKTDGLineMap::GetRandomStartPosIndex()
{
	int iRand = rand() % (int)m_mapStartPos.size();	
	std::map<int, D3DXVECTOR3>::iterator it = m_mapStartPos.begin();
	for( int i=0; i<iRand; i++ )
	{
		it++;
	}

	return (int) it->first;
}



CKTDGLineMap::LineData* CKTDGLineMap::GetAnyEnabledNormalLine()
{
	for( int i = 0 ; i < (int) m_LineList.size(); i++ )
	{
		LineData* pLineData = m_LineList[i];
		if( NULL == pLineData )
			continue;

		if( false == pLineData->bEnable )
			continue;

		if( LT_NORMAL == pLineData->lineType )
			return pLineData;
	}

	return NULL;
}



