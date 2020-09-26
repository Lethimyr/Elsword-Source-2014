#pragma once
#include <KNCSingleton.h>
#include <d3dx9math.h>
#include <vector>
#include <map>
#include <string>
#include <KncUidType.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#endif

class CKTDGLineMap
{
	DeclareSingleton( CKTDGLineMap );

public:

	struct LINE_MAP_VERTEX
	{
		D3DXVECTOR3	pos;
		DWORD		color;
	};

	enum LINE_TYPE
	{
		LT_NORMAL = 0,
		LT_WALL,
		LT_POTAL,
		LT_HOLE,
		LT_JUMP_UP_REL,
		LT_JUMP_UP_ABS,
		LT_BUNGEE,
        LT_MONSTER_ROAD,
        LT_UNIT_ROAD,
		LT_OTHER_ROAD,
	};

	enum LINE_DUST_TYPE
	{
		LDT_DEFALT_DUST,
		LDT_WATER_SPLASH,
	};

	struct LineData
	{
		LINE_TYPE			lineType;		//선의 종류
		bool				bEnable;		//선이 존재하는가
		LINE_DUST_TYPE		eDustType;		//뛰어다닐 때 생기는 먼지 파티클 종류
		int					potalGroup;		//이 그룹의 포탈끼리 이동 가능
		float				fStopFactor;	//마찰계수
		bool				bCanDown;		//이 선에서 내려갈 수 있는가

		int					beforeLine;
		int					nextLine;

		D3DXVECTOR3			startPos;
		D3DXVECTOR3			endPos;
		D3DXVECTOR3			dirVector;
		D3DXVECTOR3			dirDegree;
		D3DXVECTOR3			dirRadian;
		float				fLength;

		int					lineSet;
		float				m_fSpeed;
		D3DXVECTOR3			m_vJumpSpeed;

		int					m_iTeleportLineIndex;

		bool				m_bMonsterAllowed;		// 몬스터가 이 라인맵을 인식하고 밟을 수 있는지
		bool				m_bCanPass;				// jump up, jump down 해서 이 라인맵을 통과 할 수 있는지

		int					m_iCameraIndex;

		void Init()
		{
			lineType		= LT_NORMAL;
			bEnable			= true;		
			potalGroup		= -1;		
			fStopFactor		= 1.0f;		
			bCanDown		= true;
			eDustType		= LDT_DEFALT_DUST;

			startPos		= D3DXVECTOR3(0,0,0);
			endPos			= D3DXVECTOR3(0,0,0);
			dirVector		= D3DXVECTOR3(0,0,0);
			dirDegree		= D3DXVECTOR3(0,0,0);
			dirRadian		= D3DXVECTOR3(0,0,0);
			fLength			= 0.0f;

			beforeLine		= -1;
			nextLine		= -1;

			lineSet			= -1;
			m_fSpeed		= 0.0f;
			m_vJumpSpeed	= D3DXVECTOR3(0,0,0);

			m_iTeleportLineIndex = -1; 

			m_bMonsterAllowed	= true;
			m_bCanPass			= true;

			m_iCameraIndex		= -1;
		}
		LineData()
		{
			Init();
		}
	};

	struct LineGroup
	{
		std::vector<LineData*> vecpLineData;			
		D3DXVECTOR3 vStartPos;
		D3DXVECTOR3 vEndPos;
		
		D3DXVECTOR3 GetStartPos() const { return vStartPos; }
		D3DXVECTOR3 GetEndPos() const { return vEndPos; }
		
		LineGroup()
		{
			vStartPos = D3DXVECTOR3(0, 0, 0);
			vEndPos = D3DXVECTOR3(0, 0, 0);
		}
	};

public:
	CKTDGLineMap();
	virtual ~CKTDGLineMap(void);

	bool LoadLineMap( const char* pFileName );

	inline float GetDistance( const D3DXVECTOR3& vPos1, const D3DXVECTOR3& vPos2 )
	{
		return sqrtf( powf( vPos2.x - vPos1.x, 2.0f ) + powf( vPos2.y - vPos1.y, 2.0f ) + powf( vPos2.z - vPos1.z, 2.0f ) );
	}

	__forceinline bool IsOnLineConnect( D3DXVECTOR3 pos, int* pLastLineIndex, float fRadius = 1.0f, D3DXVECTOR3* pOutPos = NULL, bool bIsMonster = false )
	{
		D3DXVECTOR3	temp;
		float		length;
		LineData*	pLineData;

        pLineData = m_LineList[*pLastLineIndex];

		length = GetNearest( pos, &temp, *pLastLineIndex );

		if( length < fRadius )
		{
			if( pOutPos != NULL )
				*pOutPos = temp;
			return true;
		}
		else
		{
			float fDistanceToStart	= 999999;
			float fDistanceToEnd	= 999999;
			//pLineData = m_LineList[*pLastLineIndex];

			fDistanceToStart	= GetDistance( pos, pLineData->startPos );
			fDistanceToEnd		= GetDistance( pos, pLineData->endPos );

			LineData*	pBeforeLineData = m_LineList[pLineData->beforeLine];
			LineData*	pNextLineData   = m_LineList[pLineData->nextLine];

			if( fDistanceToStart < fDistanceToEnd )
			{
				if( pLineData->beforeLine != -1
					&& pLineData->beforeLine != -2 
					&& true == pBeforeLineData->bEnable )
				{
					if( pOutPos != NULL )
						*pOutPos = m_LineList[pLineData->beforeLine]->endPos;
					*pLastLineIndex = pLineData->beforeLine;
					return true;
				}
				else
				{
					//if( outPos != NULL )
					//	*outPos = lineData.startPos;
					return false;
				}
			}
			else
			{
				if( pLineData->nextLine != -1
					&& pLineData->nextLine != -2 
					&& true == pNextLineData->bEnable )
				{
					if( pOutPos != NULL )
						*pOutPos = m_LineList[pLineData->nextLine]->startPos;
					*pLastLineIndex = pLineData->nextLine;
					return true;
				}
				else
				{
					//if( outPos != NULL )
					//	*outPos = lineData.endPos;
					return false;
				}
			}
		}
	}

	__forceinline bool IsOnLine( D3DXVECTOR3 pos, float fRadius = 1.0f, D3DXVECTOR3* pOutPos = NULL, int* pLineIndex = NULL, bool bIsMonster = false )
	{
		D3DXVECTOR3	temp;
		float		length;
		int			index;

		length = GetNearest( pos, &temp, &index, bIsMonster );

		if( length < fRadius )
		{
			if( pOutPos != NULL )
				*pOutPos = temp;
			if( pLineIndex != NULL )
				*pLineIndex = index;
			return true;
		}
		else
		{
			return false;
		}
	}

	__forceinline D3DXVECTOR3 GetLandPosition_LUA( D3DXVECTOR3 pos )
	{
		return GetLandPosition( pos, 30.f, (int*) NULL );
	}

	__forceinline D3DXVECTOR3 GetLandPosition( D3DXVECTOR3 pos, float fRadius = 1.0f, char* pLineIndex = NULL )
	{
		if( pLineIndex != NULL )
		{
			int tempLineIndex = *pLineIndex;
			D3DXVECTOR3 bRetVal = GetLandPosition( pos, fRadius, &tempLineIndex );
			*pLineIndex = (char) tempLineIndex;
			return bRetVal;
		}
		else
		{
			int* pTemp = NULL;
			return GetLandPosition( pos, fRadius, pTemp );
		}
	}

	__forceinline D3DXVECTOR3 GetLandPosition( D3DXVECTOR3 pos, float fRadius = 1.0f, int* pLineIndex = NULL );

	bool CanPassUp( D3DXVECTOR3 pos );

	__forceinline bool CanDown( D3DXVECTOR3 pos, float fRadius = 1.0f, D3DXVECTOR3* pOutPos = NULL, char* pLineIndex = NULL )
	{
		if( pLineIndex != NULL )
		{
			int tempLineIndex = *pLineIndex;
			bool bRetVal = CanDown( pos, fRadius, pOutPos, &tempLineIndex );
			*pLineIndex = (char) tempLineIndex;
			return bRetVal;
		}
		else
		{
			int* pTemp = NULL;
			return CanDown( pos, fRadius, pOutPos, pTemp );
		}
	}
	
	// @ bOnLine : 캐릭터가 라인위에 있는지 공중에 떠 있는지
	bool CanDown( D3DXVECTOR3 pos, float fRadius = 1.0f, D3DXVECTOR3* pOutPos = NULL, int* pLineIndex = NULL, 
			bool bDownRadius = true, bool bIsMonster = false, bool bOnLine = true );
	

	__forceinline LineData* GetLineData( int index );
	__forceinline const int GetNumLineData() { return (int)m_LineList.size(); }
	__forceinline const float GetLandHeight() { return m_fLandHeight; }		

	int GetRedTeamStartPosNum(){ return m_RedTeamStartPos.size(); }
	int GetBlueTeamStartPosNum(){ return m_BlueTeamStartPos.size(); }
	int GetStartPosNum(){ return m_mapStartPos.size(); }
	std::map<int, D3DXVECTOR3>& GetStartPosMap() { return m_mapStartPos; }
	
	D3DXVECTOR3 GetRedTeamStartPosition( int index ){ return m_RedTeamStartPos[index]; }
	D3DXVECTOR3 GetBlueTeamStartPosition( int index ){ return m_BlueTeamStartPos[index]; }
	D3DXVECTOR3 GetStartPosition( int key ){ return m_mapStartPos[key]; }

	bool GetRedTeamStartRight( int index ){ return m_RedTeamStartRight[index]; }
	bool GetBlueTeamStartRight( int index ){ return m_BlueTeamStartRight[index]; }
	bool GetStartRight( int key ){ return m_mapStartRight[key]; }

	int GetRedTeamStartLineIndex( int index ){ return m_RedTeamStartLineIndex[index]; }
	int GetBlueTeamStartLineIndex( int index ){ return m_BlueTeamStartLineIndex[index]; }
	int GetStartLineIndex( int key ){ return m_mapStartLineIndex[key]; }

	int GetRandomStartPosIndex();

	D3DXVECTOR3 CKTDGLineMap::GetRandomPosition_LUA()
	{
		return GetRandomPosition( NULL, 0.f, false );
	}

	D3DXVECTOR3 GetRandomPosition( D3DXVECTOR3* pvPos, float fRange, bool bOnThisLineGroup );
	D3DXVECTOR3 GetRandomPositionDir( D3DXVECTOR3* pvPos, float fRange, bool bOnThisLineGroup, bool bIsRight );
    D3DXVECTOR3 GetNearest_LUA(D3DXVECTOR3 inPos);

	bool Step( int lineIndex, D3DXVECTOR3 pos );

	void ReleaseLineGroup();
	void ReBuildLineGroup();
	int GetLineSetIndex( int lineIndex );
	LineGroup* GetLineGroupIncludesLineData( CKTDGLineMap::LineData* pLineData );
	LineGroup* GetLineGroup(int index) { return m_vecLineGroupList[index]; }
	void EnableLineData( CKTDGLineMap::LineData* pLineData, bool bEnable );
	void EnableLineSet( int iLineSetIndex, bool bEnable );
	void DisableAllLineData();
	

	//LUA
	void SetLandHeight( float fLandHeight ){ m_fLandHeight = fLandHeight; }
	void SetTeamStartPos( bool bRed, D3DXVECTOR3 pos, bool bRight, int iLineIndex )
	{
		if( bRed == true )
		{
			m_RedTeamStartPos.push_back( pos );
			m_RedTeamStartRight.push_back( bRight );
			m_RedTeamStartLineIndex.push_back( iLineIndex );
		}
		else
		{
			m_BlueTeamStartPos.push_back( pos );
			m_BlueTeamStartRight.push_back( bRight );
			m_BlueTeamStartLineIndex.push_back( iLineIndex );
		}
	}
	void AddStartPos( int key, D3DXVECTOR3 pos, bool bRight, int iLineIndex )
	{
		m_mapStartPos[key]			= pos;
		m_mapStartRight[key]		= bRight;
		m_mapStartLineIndex[key]	= iLineIndex;
	}
	
	bool AddLine_LUA();

	__forceinline int		GetNearestLine(	const D3DXVECTOR3& pos, D3DXVECTOR3* pOutPos = NULL, bool bIsMonster = false )
    {
        int			touchLine		= -1;
        float		touchDistance	= 9999999.0f;
        D3DXVECTOR3	touchPos;

        //디딜 곳이 없다면 모든 선을 검색한다
        for( int i = 0; i < (int)m_LineList.size(); i++ )
        {
            LineData* pLineData = m_LineList[i];

            //정상적인 발판만 확인한다
            //점과 선사이의 최단 거리를 구한다.
            D3DXVECTOR3 projectionPoint;
            float		fFinalLength;
            fFinalLength = GetLengthToLine( pos, i, &projectionPoint );

            if( fFinalLength <= touchDistance )
            {
                touchLine		= i;
                touchDistance	= fFinalLength;
                touchPos		= projectionPoint;
            }
        }

        if( pOutPos != NULL )
            *pOutPos = touchPos;

        return touchLine;
    }
	bool	UpdateLineVB();

	LineData* GetAnyEnabledNormalLine();

	float GetXMin() const { return m_fXMin; }
	float GetXMax() const { return m_fXMax; }
	float GetYMin() const { return m_fYMin; }
	float GetYMax() const { return m_fYMax; }

private:
	void	MakeLine( LineData* pLineData );

	__forceinline bool	VerifyLine( int lineIndex, bool bIsMonster = false )
	{
		//라인이 정상이면 TRUE
		if( lineIndex < 0 || (int)m_LineList.size() <= lineIndex )
			return false;

		LineData* pLineData = m_LineList[lineIndex];

		if( pLineData->bEnable == false
			|| pLineData->lineType == LT_WALL )
		{
			return false;
		}

		return true;
	}       
		
	__forceinline float	GetLengthToLine( const D3DXVECTOR3& pos, int lineIndex, D3DXVECTOR3* pOutPos = NULL )
	{
		LineData* pLineData = m_LineList[lineIndex];

		//정상적인 발판만 확인한다
		if( VerifyLine( lineIndex ) == false )
			return -1.0f;

		//점과 선사이의 최단 거리를 구한다.
		float fLineLength				= GetDistance( pLineData->startPos, pLineData->endPos );
		float fLineStartToPointLength	= GetDistance( pLineData->startPos, pos );

		D3DXVECTOR3 lineDir = pLineData->endPos - pLineData->startPos;
		D3DXVec3Normalize( &lineDir, &lineDir );
		D3DXVECTOR3 pointDir = pos - pLineData->startPos;

		D3DXVECTOR3 projectionPoint;
		float		fProjectionLength	= D3DXVec3Dot( &lineDir, &pointDir );

		//만약 최단거리가 선 내부가 아니라면 선 가장자리로 보정한다.
		if( fProjectionLength <= 0.0f )
			projectionPoint = pLineData->startPos;
		else if( fProjectionLength >= fLineLength )
			projectionPoint = pLineData->endPos;
		else
			projectionPoint		= lineDir * fProjectionLength + pLineData->startPos;

		float fFinalLength		= GetDistance( pos, projectionPoint );

		if( pOutPos != NULL )
			*pOutPos = projectionPoint;

		return fFinalLength;
	}


	__forceinline float GetNearest( D3DXVECTOR3 inPos, D3DXVECTOR3* pOutPos, bool bIsMonster = false )
	{
		D3DXVECTOR3 target;
		float		fProjectionLength;
		float		outLength = 999999;
		float		tempLength;
		D3DXVECTOR3	tempPos;

		for( int i = 0; i < (int)m_LineList.size(); i++ )
		{
			LineData* pLineData = m_LineList[i];
			if( false == pLineData->bEnable )
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
					outLength	= tempLength;
					*pOutPos	= tempPos;
				}
			}
		}
		return outLength;
	}

	__forceinline float GetNearest( D3DXVECTOR3 inPos, D3DXVECTOR3* pOutPos, int* pLlineIndex, bool bIsMonster = false )
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
					outLength	= tempLength;
					*pOutPos		= tempPos;
					*pLlineIndex	= i;
				}
			}
		}
		return outLength;
	}

	__forceinline float GetNearest( D3DXVECTOR3 inPos, D3DXVECTOR3* pOutPos, int lineIndex )
	{
		D3DXVECTOR3 target;
		float		fProjectionLength;
		float		outLength = 999999;
		float		tempLength;
		D3DXVECTOR3	tempPos;

		target				= inPos - m_LineList[lineIndex]->startPos;
		fProjectionLength	= D3DXVec3Dot( &m_LineList[lineIndex]->dirVector, &target );
		if( fProjectionLength >= 0.0f && fProjectionLength < m_LineList[lineIndex]->fLength )
		{
			tempPos		= fProjectionLength * m_LineList[lineIndex]->dirVector + m_LineList[lineIndex]->startPos;
			tempLength	= GetDistance( tempPos, inPos );
			if( tempLength < outLength )
			{
				outLength	= tempLength;
				*pOutPos		= tempPos;
			}
		}

		return outLength;
	}

	__forceinline float GetDownNearest( D3DXVECTOR3 inPos, D3DXVECTOR3* pOutPos, int* pLlineIndex );

	std::vector<LineData*>			m_LineList;
	std::vector<LineData*>			m_WallList;

	LPDIRECT3DVERTEXBUFFER9			m_pLineMapVB;

	float							m_fLandHeight;
	std::vector<D3DXVECTOR3>		m_RedTeamStartPos;
	std::vector<D3DXVECTOR3>		m_BlueTeamStartPos;
	std::vector<bool>				m_RedTeamStartRight;
	std::vector<bool>				m_BlueTeamStartRight;
	std::vector<int>				m_RedTeamStartLineIndex;
	std::vector<int>				m_BlueTeamStartLineIndex;		

	std::map<int, D3DXVECTOR3>		m_mapStartPos;
	std::map<int, bool>				m_mapStartRight;
	std::map<int, int>				m_mapStartLineIndex;
	

	std::vector<LineGroup*>							m_vecLineGroupList;
	std::map<CKTDGLineMap::LineData*, LineGroup*>	m_mapLineData2LineGroup;

	float						m_fXMin;
	float						m_fXMax;
	float						m_fYMin;
	float						m_fYMax;
};

DefSingletonInline( CKTDGLineMap );


