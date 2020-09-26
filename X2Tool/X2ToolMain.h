#pragma once

#include "ToolEnumDefine.h"
#include "X2EnumManager.h"

/** @struct	: tLineVertexInfo
	@brief	: 라인 정점 정보
	@date	: 2013/02/20
*/
#pragma region tLineVertexInfok
struct tLineVertexInfo
{
public:
	bool m_bStartPos;	/// 시작 위치 여부
	int m_iLineIndex;	/// 라인 인덱스
	
	/// 생성자
	tLineVertexInfo()
	{
		m_bStartPos = false;
		m_iLineIndex = -1;	
	}
};
#pragma endregion 

/** @struct	: tLineVertex
	@brief	: 라인 정점 속성
	@date	: 2013/02/20
*/
#pragma region 
struct tLineVertex
{
public:
	D3DXVECTOR3 vVertex;	/// 위치 값
	bool m_bOver;			/// 마우스 오버 상태 여부
	bool m_bSelected;		/// 선택 여부

	std::vector<tLineVertexInfo> m_vecLineInfo;		/// Vertex vetor

	/// 생성자
	tLineVertex()
	{
		vVertex = D3DXVECTOR3(0.f, 0.f, 0.f);		
		m_bOver = false;
		m_bSelected = false;
		m_vecLineInfo.clear();
	}
};
#pragma endregion 

/** @struct	: tLineData
	@brief	: 라인 데이터
	@date	: 2013/02/20
*/
#pragma region 
struct tLineData
{
public:
	vector<CKTDGLineMap::LineData *>	m_vecUnDoLineData;
};
#pragma endregion 

/** @struct	: tWorldElem
	@brief	: 
	@date	: 2013/02/20
*/
#pragma region 
struct tWorldElem
{
	enum UNDOFLAG
	{
		UF_NONE = 0,
		UF_INT,
		UF_FLOAT,
		UF_BOOL,
		UF_WSTRING,
	};

public:
	int		m_iEid;
	CX2WorldObject *m_pWorldObject;
	UNDOFLAG m_eFlag;
	float	m_fValue;
	int		m_iValue;
	bool	m_bValue;
	wstring	m_wstrValue;

	tWorldElem()
	{
		m_iEid = 0;
		m_pWorldObject = NULL;
		m_eFlag = UF_NONE;
		m_fValue = 0.f;
		m_iValue = 0;
		m_bValue = false;
		m_wstrValue = L"";
	}
};
#pragma endregion 

/** @struct	: tWorldElem
	@brief	: 
	@date	: 2013/02/20
*/
#pragma region 
struct tWorldElemGroup
{
	std::vector<tWorldElem> m_vecUndoGroup;
};
#pragma endregion 

/** @struct	: tAddRmoveWorldElem
	@brief	: 
	@date	: 2013/02/20
*/
#pragma region 
struct tAddRmoveWorldElem
{
public:
	bool	m_bAdded;
	std::vector<CX2WorldObjectPtr> m_vecWorldObject;

	tAddRmoveWorldElem()
	{
		m_bAdded = false;
		m_vecWorldObject.clear();
	}
};
#pragma endregion 

/** @struct	: tUNDOWORLD
	@brief	: 
	@date	: 2013/02/20
*/
#pragma region 
struct tUNDOWORLD
{
	std::vector<tWorldElemGroup> m_vecUndoElem;
	std::vector<tAddRmoveWorldElem> m_vecUndoARElem;

	tUNDOWORLD()
	{
		m_vecUndoElem.clear();
		m_vecUndoARElem.clear();
	}
};
#pragma endregion 

/** @class : CX2ToolMain
	@brief : 
	@date  : 2013/02/20
*/
#pragma region 
class CX2ToolMain : public CX2Main
{
	/// 바운딩 박스 형식
	enum BOUNDINGBOX_TYPE
	{
		BT_NONE = 0,	/// 없음	
		BT_START,		/// 시작
		BT_MIDDLE,		/// 중간
		BT_END,			/// 끝
	};

public:
	/** @class : CCollision
		@brief : 충돌 확인 클래스
		@date  : 2013/02/20
	*/
	#pragma region CCollision
	class CCollision
	{
	public:
		/** @function 	: FindNearestPointOnLineSegment
			@brief 		: 라인의 근거리 정점 찾기
		*/
		static void FindNearestPointOnLineSegment( const float& A1x, const float& A1y, const float& A1z,
												   const float& Lx, const float& Ly, const float& Lz,
												   const float& Bx, const float& By, const float& Bz,
												   bool infinite_line, float epsilon_squared, float &NearestPointX,
												   float &NearestPointY, float &NearestPointZ, float &parameter )
		{
			KTDXPROFILE();
			// Line/Segment is degenerate --- special case #1
			float D = Lx * Lx + Ly * Ly + Lz * Lz;
			if (D < epsilon_squared)
			{
				NearestPointX = A1x;
				NearestPointY = A1y;
				NearestPointZ = A1z;
				return;
			}

			float ABx = Bx - A1x;
			float ABy = By - A1y;
			float ABz = Bz - A1z;

			// parameter is computed from Equation (20).
			parameter = (Lx * ABx + Ly * ABy + Lz * ABz) / D;

			if (false == infinite_line) 
				parameter = max(0.0f, min(1.0f, parameter));

			NearestPointX = A1x + parameter * Lx;
			NearestPointY = A1y + parameter * Ly;
			NearestPointZ = A1z + parameter * Lz;
			return;
		}

		/** @function 	: FindNearestPointOnLineSegment
			@brief 		: 라인의 근거리 정점 찾기
		*/
		static D3DXVECTOR3 FindNearestPointOnLineSegment( const D3DXVECTOR3& vPoint, const D3DXVECTOR3& vLineStart, const D3DXVECTOR3& vLineEnd )
		{
			D3DXVECTOR3 vLineDir = vLineEnd - vLineStart;
			float fLineVectorLength = D3DXVec3Length( &vLineDir );
			ASSERT( fLineVectorLength > 0.f );
			vLineDir /= fLineVectorLength;
			D3DXVECTOR3 vDiagonal = vPoint - vLineStart;
			float fProjectedLength = D3DXVec3Dot( &vLineDir, &vDiagonal );

			float fInterpolationValue = fProjectedLength / fLineVectorLength;	// lineend에서 line상에 projection 된 위치까지 거리 / line segment length
			if( fInterpolationValue > 1.f )
				fInterpolationValue = 1.f;
			else if( fInterpolationValue < 0.f )
				fInterpolationValue = 0.f;

			return vLineStart + vLineDir * fInterpolationValue * fLineVectorLength;
		}

		/** @function 	: AdjustNearestPoints
			@brief 		: 근거의 점 조정
			@return		: D3DXVECTOR3
		*/
		static void AdjustNearestPoints( float A1x, float A1y, float A1z, float Lax, float Lay, float Laz,
										 float B1x, float B1y, float B1z, float Lbx, float Lby, float Lbz,
										 float epsilon_squared, float s, float t,
										 float &PointOnSegAx, float &PointOnSegAy, float &PointOnSegAz,
										 float &PointOnSegBx, float &PointOnSegBy, float &PointOnSegBz )
		{
			KTDXPROFILE();
			// handle the case where both parameter s and t are out of range
			if (OUTOF_RANGE(s,0,1) && OUTOF_RANGE(t,0,1))
			{
				s = max(0.0f, min(1.0f, s));
				PointOnSegAx = (A1x + s * Lax);
				PointOnSegAy = (A1y + s * Lay);
				PointOnSegAz = (A1z + s * Laz);
				FindNearestPointOnLineSegment(B1x, B1y, B1z, Lbx, Lby, Lbz, PointOnSegAx,
					PointOnSegAy, PointOnSegAz, true, epsilon_squared,
					PointOnSegBx, PointOnSegBy, PointOnSegBz, t);
				if (OUTOF_RANGE(t,0,1))
				{
					t = max(0.0f, min(1.0f, t));
					PointOnSegBx = (B1x + t * Lbx);
					PointOnSegBy = (B1y + t * Lby);
					PointOnSegBz = (B1z + t * Lbz);
					FindNearestPointOnLineSegment(A1x, A1y, A1z, Lax, Lay, Laz, PointOnSegBx,
						PointOnSegBy, PointOnSegBz, false, epsilon_squared,
						PointOnSegAx, PointOnSegAy, PointOnSegAz, s);
					FindNearestPointOnLineSegment(B1x, B1y, B1z, Lbx, Lby, Lbz, PointOnSegAx,
						PointOnSegAy, PointOnSegAz, false, epsilon_squared,
						PointOnSegBx, PointOnSegBy, PointOnSegBz, t);
				}
			}
			// otherwise, handle the case where the parameter for only one segment is
			// out of range
			else if (OUTOF_RANGE(s,0,1))
			{
				s = max(0.0f, min(1.0f, s));
				PointOnSegAx = (A1x + s * Lax);
				PointOnSegAy = (A1y + s * Lay);
				PointOnSegAz = (A1z + s * Laz);
				FindNearestPointOnLineSegment(B1x, B1y, B1z, Lbx, Lby, Lbz, PointOnSegAx,
					PointOnSegAy, PointOnSegAz, false, epsilon_squared,
					PointOnSegBx, PointOnSegBy, PointOnSegBz, t);
			}
			else if (OUTOF_RANGE(t,0,1))
			{
				t = max(0.0f, min(1.0f, t));
				PointOnSegBx = (B1x + t * Lbx);
				PointOnSegBy = (B1y + t * Lby);
				PointOnSegBz = (B1z + t * Lbz);
				FindNearestPointOnLineSegment(A1x, A1y, A1z, Lax, Lay, Laz, PointOnSegBx,
					PointOnSegBy, PointOnSegBz, false, epsilon_squared,
					PointOnSegAx, PointOnSegAy, PointOnSegAz, s);
			}
			else
			{
				ASSERT(0);
			}
		}

		/** @function 	: FindNearestPointOfParallelLineSegments
			@brief 		: 평행한 라인의 근거리 정점 찾기
		*/
		static void FindNearestPointOfParallelLineSegments( 
			float A1x, float A1y, float A1z, float A2x, float A2y, float A2z, float Lax, float Lay, float Laz,
			float B1x, float B1y, float B1z, float B2x, float B2y, float B2z, float Lbx, float Lby, float Lbz,
			bool infinite_lines, float epsilon_squared, float &PointOnSegAx, float &PointOnSegAy, float &PointOnSegAz,
			float &PointOnSegBx, float &PointOnSegBy, float &PointOnSegBz )
		{
			KTDXPROFILE();
			float s[2], temp;
			FindNearestPointOnLineSegment(A1x, A1y, A1z, Lax, Lay, Laz, B1x, B1y, B1z,
				true, epsilon_squared, PointOnSegAx, PointOnSegAy, PointOnSegAz, s[0]);
			if (true == infinite_lines)
			{
				PointOnSegBx = B1x;
				PointOnSegBy = B1y;
				PointOnSegBz = B1z;
			}
			else
			{
				float tp[3];
				FindNearestPointOnLineSegment(A1x, A1y, A1z, Lax, Lay, Laz, B2x, B2y, B2z,
					true, epsilon_squared, tp[0], tp[1], tp[2], s[1]);
				if (s[0] < 0.f && s[1] < 0.f)
				{
					PointOnSegAx = A1x;
					PointOnSegAy = A1y;
					PointOnSegAz = A1z;
					if (s[0] < s[1])
					{
						PointOnSegBx = B2x;
						PointOnSegBy = B2y;
						PointOnSegBz = B2z;
					}
					else
					{
						PointOnSegBx = B1x;
						PointOnSegBy = B1y;
						PointOnSegBz = B1z;
					}
				}
				else if (s[0] > 1.f && s[1] > 1.f)
				{
					PointOnSegAx = A2x;
					PointOnSegAy = A2y;
					PointOnSegAz = A2z;
					if (s[0] < s[1])
					{
						PointOnSegBx = B1x;
						PointOnSegBy = B1y;
						PointOnSegBz = B1z;
					}
					else
					{
						PointOnSegBx = B2x;
						PointOnSegBy = B2y;
						PointOnSegBz = B2z;
					}
				}
				else
				{
					temp = 0.5f*(max(0.0f, min(1.0f, s[0])) + max(0.0f, min(1.0f, s[1])));
					PointOnSegAx = (A1x + temp * Lax);
					PointOnSegAy = (A1y + temp * Lay);
					PointOnSegAz = (A1z + temp * Laz);
					FindNearestPointOnLineSegment(B1x, B1y, B1z, Lbx, Lby, Lbz,
						PointOnSegAx, PointOnSegAy, PointOnSegAz, true,
						epsilon_squared, PointOnSegBx, PointOnSegBy, PointOnSegBz, temp);
				}
			}
		}

		/** @function 	: LineToSphere
			@brief 		: 선과 구 충돌
		*/
		static bool LineToSphere( const D3DXVECTOR3& lineStart, const D3DXVECTOR3& lineEnd, const float& lineRadius,
								  const D3DXVECTOR3& center, const float& radius, D3DXVECTOR3* point = NULL )
		{
			KTDXPROFILE();

			float rayLength = GetDistance( lineStart, lineEnd );
			float sphereLength = GetDistance( lineStart, center ) - radius - lineRadius;

			if( rayLength <= 0.f )
				return false;

			if( rayLength < sphereLength )
				return false;

			D3DXVECTOR3 vPos = CCollision::FindNearestPointOnLineSegment( center, lineStart, lineEnd );
			float fDist = GetDistance(vPos, center);
			if( fDist - lineRadius <= radius )
			{
				if( point != NULL )
				{
					*point = vPos;
				}
				return true;
			}

			return false;
		}

		/** @function 	: LineToLine
			@brief 		: 선과 선 충돌
		*/
		static bool LineToLine( 
			const float& A1x, const float& A1y, const float& A1z, const float& A2x, const float& A2y, const float& A2z, 
			const float& B1x, const float& B1y, const float& B1z, const float& B2x, const float& B2y, const float& B2z,
			bool infinite_lines, float epsilon, float &PointOnSegAx, float &PointOnSegAy, float &PointOnSegAz,
			float &PointOnSegBx, float &PointOnSegBy, float &PointOnSegBz )
		{
			KTDXPROFILE();
			float temp = 0.f;
			float epsilon_squared = epsilon * epsilon;

			// Compute parameters from Equations (1) and (2) in the text
			float Lax = A2x - A1x;
			float Lay = A2y - A1y;
			float Laz = A2z - A1z;
			float Lbx = B2x - B1x;
			float Lby = B2y - B1y;
			float Lbz = B2z - B1z;
			// From Equation (15)
			float L11 =  (Lax * Lax) + (Lay * Lay) + (Laz * Laz);
			float L22 =  (Lbx * Lbx) + (Lby * Lby) + (Lbz * Lbz);

			// Line/Segment A is degenerate ---- Special Case #1
			if (L11 < epsilon_squared)
			{
				PointOnSegAx = A1x;
				PointOnSegAy = A1y;
				PointOnSegAz = A1z;
				FindNearestPointOnLineSegment(B1x, B1y, B1z, Lbx, Lby, Lbz, A1x, A1y, A1z,
					infinite_lines, epsilon, PointOnSegBx, PointOnSegBy,
					PointOnSegBz, temp);
			}
			// Line/Segment B is degenerate ---- Special Case #1
			else if (L22 < epsilon_squared)
			{
				PointOnSegBx = B1x;
				PointOnSegBy = B1y;
				PointOnSegBz = B1z;
				FindNearestPointOnLineSegment(A1x, A1y, A1z, Lax, Lay, Laz, B1x, B1y, B1z,
					infinite_lines, epsilon, PointOnSegAx, PointOnSegAy,
					PointOnSegAz, temp);
			}
			// Neither line/segment is degenerate
			else
			{
				// Compute more parameters from Equation (3) in the text.
				float ABx = B1x - A1x;
				float ABy = B1y - A1y;
				float ABz = B1z - A1z;

				// and from Equation (15).
				float L12 = -(Lax * Lbx) - (Lay * Lby) - (Laz * Lbz);

				float DetL = L11 * L22 - L12 * L12;

				// Lines/Segments A and B are parallel ---- special case #2.
				if (fabs(DetL) < epsilon)
				{
					FindNearestPointOfParallelLineSegments( A1x, A1y, A1z, A2x, A2y, A2z,
						Lax, Lay, Laz,
						B1x, B1y, B1z, B2x, B2y, B2z,
						Lbx, Lby, Lbz,
						infinite_lines, epsilon,
						PointOnSegAx, PointOnSegAy, PointOnSegAz,
						PointOnSegBx, PointOnSegBy, PointOnSegBz );
				}
				// The general case
				else
				{
					// from Equation (15)
					float ra = Lax * ABx + Lay * ABy + Laz * ABz;
					float rb = -Lbx * ABx - Lby * ABy - Lbz * ABz;

					float t = (L11 * rb - ra * L12)/DetL; // Equation (12)
					float s = (ra-L12*t)/L11;             // Equation (13)

					// if we are dealing with infinite lines or if parameters s and t both
					// lie in the range [0,1] then just compute the points using Equations
					// (1) and (2) from the text.
					PointOnSegAx = (A1x + s * Lax);
					PointOnSegAy = (A1y + s * Lay);
					PointOnSegAz = (A1z + s * Laz);
					PointOnSegBx = (B1x + t * Lbx);
					PointOnSegBy = (B1y + t * Lby);
					PointOnSegBz = (B1z + t * Lbz);
					// otherwise, at least one of s and t is outside of [0,1] and we have to
					// handle this case.
					if (false == infinite_lines && (OUTOF_RANGE(s,0,1) || OUTOF_RANGE(t,0,1)))
					{
						AdjustNearestPoints(A1x, A1y, A1z, Lax, Lay, Laz,
							B1x, B1y, B1z, Lbx, Lby, Lbz,
							epsilon, s, t,
							PointOnSegAx, PointOnSegAy, PointOnSegAz,
							PointOnSegBx, PointOnSegBy, PointOnSegBz);
					}//if
				}//if.. else..
			}//if.. else..

			float NearestVectorX = PointOnSegBx - PointOnSegAx;
			float NearestVectorY = PointOnSegBy - PointOnSegAy;
			float NearestVectorZ = PointOnSegBz - PointOnSegAz;

			// optional check to indicate if the lines truly intersect
			bool true_intersection = (fabs(NearestVectorX) +
				fabs(NearestVectorY) +
				fabs(NearestVectorZ)) < epsilon ? true : false;

			return true_intersection;
		}

		/** @function 	: LineToLine
			@brief 		: 선과 선 충돌
		*/
		static bool LineToLine( const D3DXVECTOR3& L1Start, const D3DXVECTOR3& L1End, const D3DXVECTOR3& L2Start,
								const D3DXVECTOR3& L2End, const float& radius, D3DXVECTOR3& pointL1, D3DXVECTOR3& pointL2 )
		{
			return LineToLine( L1Start.x, L1Start.y, L1Start.z, L1End.x, L1End.y, L1End.z,
				L2Start.x, L2Start.y, L2Start.z, L2End.x, L2End.y, L2End.z,
				false, radius, 
				pointL1.x, pointL1.y, pointL1.z, pointL2.x, pointL2.y, pointL2.z );
		}

		/** @function 	: LineToLine
			@brief 		: 선과 선 충돌
		*/
		static bool LineToLine( const D3DXVECTOR2 A1, const D3DXVECTOR2 A2, const D3DXVECTOR2 B1, const D3DXVECTOR2 B2 )
		{			
			float fUnder = (B2.y - B1.y)*(A2.x-A1.x)-(B2.x-B1.x)*(A2.y-A1.y);
			if( IsSamef(fUnder) == true )
				return false;

			float _t = (B2.x-B1.x)*(A1.y-B1.y) - (B2.y-B1.y)*(A1.x-B1.x);
			float _s = (A2.x-A1.x)*(A1.y-B1.y) - (A2.y-A1.y)*(A1.x-B1.x);

			if( IsSamef(_t) == true && IsSamef(_s) == true )
				return false;

			float t = _t/fUnder;
			float s = _s/fUnder;

			if( t<0.f || t>1.f || s<0.f || s>1.f )
				return false;
			
			return true;
		}

		/** @function 	: DXRayCheck
			@brief 		: DirectX의 반직선 검사
		*/
		static bool DXRayCheck( LPD3DXBASEMESH pDestMesh, D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, 
								const D3DXMATRIX& transform, float* distance )
		{
			KTDXPROFILE();
			BOOL retval;

			DWORD pFaceIndex;
			FLOAT pU;
			FLOAT pV;
			//LPD3DXBUFFER ppAllHits;
			DWORD pCountOfHits;

			D3DXMATRIX InverseMatTransform = transform;
			D3DXMatrixInverse(&InverseMatTransform, NULL, &InverseMatTransform);

			D3DXVec3TransformCoord(&rayPos, &rayPos, &InverseMatTransform);
			D3DXVec3TransformNormal(&rayDir, &rayDir, &InverseMatTransform);
			//rayPos = rayPos - transform;

			D3DXIntersect( pDestMesh, &rayPos, &rayDir, &retval, &pFaceIndex,
				&pU, &pV, distance, NULL, &pCountOfHits );

			if( retval == TRUE )
				return true;
			else
				return false;
		}
	};
	#pragma endregion 클래스

public:
	CX2ToolMain(void);
	virtual ~CX2ToolMain(void);
	
	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();
	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void InitX2Data();
	bool LoadConfig();
	void RenderAxisView();
	void UpdateMonsterList();
	void CreateGame( SEnum::DUNGEON_ID eDungeonId = SEnum::DI_TEMP0, bool bReset = false );
	void ResetGame( SEnum::DUNGEON_ID eDungeonId );	
	void RenderStartPos();
	
	// frame간 인터페이스	 
	DWORD_PTR GetSelectedDungeonItem();	
	void ResetTool( bool bEditLine, bool bEditWorld, bool bViewLine, bool bViewWorld, bool bEditTrigger );
	void SetUserCamera(bool bVal);
	void SetWindowSize(int iWidth, int iHeight)
	{
		g_pMain->GetGameOption().SetResolution( iWidth, iHeight );
	}
	void SetCheckNpc(bool bVal) { m_bCheckNpc = bVal; }
	void RefreshNpcData(int iKey);

	// 공용 인터페이스
	void SetFreeCamera()
	{
		g_pX2Game->SetFreeCamera( m_bEditLine || m_bEditWorld );
	}
	void SetCameraView(int iView, D3DXVECTOR3 vFocus);

	void RenderBoundingSphere( D3DXVECTOR3& vCenter, float fRadius, bool bOver, bool bSelected, BOUNDINGBOX_TYPE eBT = BT_NONE );
	void RenderRectangle( D3DXVECTOR2 vLeftTop, D3DXVECTOR2 vRightBottom, D3DXCOLOR vColor );
	void RenderMoveAxis( D3DXVECTOR3 vPos, bool bUseWorld = false );
	void RenderLine( D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXCOLOR vColor );
	void RenderGrid();
	void RenderMoveWorld( D3DXVECTOR3& vCenter );
	
	bool PickMoveAxis();
	bool PickMoveWorldObject();

	// 카메라 인터페이스
	void DeleteCameraData(int iItem);

	// 라인맵 인터페이스
	void CalcUnderWaterUnitColor(int iFlag, float fColor);
	int GetSelectedLineVertCount()
	{
		int nCount = 0;
		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
		{
			if( m_vecLineVertex[i].m_bSelected == true )
				++nCount;
		}

		return nCount;
	}
	void GetVertIndexByLine(vector<int> &vecVertInx, int index)
	{
		int nCount = 0;
		for(UINT i=0; i<m_vecLineVertex.size(); ++i)
		{
			for(UINT j=0; j<m_vecLineVertex[i].m_vecLineInfo.size(); ++j)
			{
				if( index == m_vecLineVertex[i].m_vecLineInfo[j].m_iLineIndex )
				{
					++nCount;
					vecVertInx.push_back(i);
				}
			}

			if( nCount >= 2 )
				return;
		}

		return;
	}
	void SelectLine(int iIndex, bool bSelect);
	float DistanceBetweenPointAndSegment(float& fLerpCoef, D3DXVECTOR3* pvPoint, D3DXVECTOR3* pvSegmentStart, D3DXVECTOR3* pvSegmentEnd);
	bool GetMousePosOnLine( D3DXVECTOR3* pvPosOnLine, const D3DXVECTOR2& mousepos );
	bool IsValidLine();
	void SetVertexPos(D3DXVECTOR3 vPos, D3DXVECTOR3 vApply = D3DXVECTOR3(1.f, 1.f, 1.f), bool bMulti = false);
	void ReMakeLineData();
	void UpdateLineMap();
	void NewLine(wstring wstrLineName = L"new_create_LineMap.lua");
	void LoadLineMap( const WCHAR* pwszFileName );
	tLineVertex* GetSameVertexLine( D3DXVECTOR3 &vPos ); 	
	void ParsingLine();
	bool PickLineVertex(bool bDrag = false);
	bool PickLine(bool bDrag = false);	
	bool PickStartPosition();
	void DeleteVertex();
	void DeleteLine();
	void DoDeleteEditLine();
	int	 AddVertex(D3DXVECTOR3 vAddPos, int lineIndex = -1);	// 라인대상
	void SetMakeFirst();
	void SetWeld();
	void InitFillet();
	void SetFillet(int iVal);
	void CopyLine(int iVal = 0);
	void DivideLine(int iCount);
	bool SwapLine( int iOldLine, int iNewLine );
	void JoinVertex();
	void JoinVertex( int iBeforeIndex, int iNextIndex );
	void DeleteLineVertByLineIndex( int iIndex, int iReplaceLine = -1 );
	void AddUndoList();
	void UnDoLine();
	void ClearUnDoList();
	void UpdateVertexProperty();
	void UpdateLineProperty();
	void SetLineProperty(PROPERTY_ID ePI, int iVal, float fVal, wstring wstrVal, bool bVal = false );
	void SelectStartPos(int iIndex);
	int AddStartPosition(D3DXVECTOR3 vPos, int iIndex);
	void MoveStartPosition(D3DXVECTOR3 vPos);
	void RebuildStartPosition();
	void DeleteObjectStartPosition(int iIndex)
	{
		if( iIndex < (int)vecpXSkinAnim.size() )
		{
			SAFE_DELETE_KTDGOBJECT( (CKTDGXSkinAnim*)vecpXSkinAnim[iIndex] );
			vecpXSkinAnim.erase( vecpXSkinAnim.begin() + iIndex );
		}
		m_iPickedStartPosition = -1;
	}
	bool SaveLineMapToFile( const WCHAR* wszFileName, bool bUpdateFileName );
	void ExportLineMap(wstring wstrExportName);
	void ReArrangeLine();

	// 월드 인터페이스
	void LoadWorld( const WCHAR* pwszFileName );
	int SelectWorldlayer();
	void SetWorldlayerProperty(PROPERTY_ID ePI, int iVal, float fVal, wstring wstrVal, bool bVal = false );
	void SetEnableWorldFog(bool bVal);
	void SetWorldFogProperty(PROPERTY_ID ePI, float fVal);
	void SetWorldBaseProperty(PROPERTY_ID ePI, float fVal);
	int SelectSkydome();
	void SetSkyDomeProperty(PROPERTY_ID ePI, int iVal);
	void SetSkyDomeProperty(PROPERTY_ID ePI, bool bVal);
	void SetSkyDomeProperty(PROPERTY_ID ePI, float fVal);
	int SelectWorldObject(int iSelectedInx, bool bCheck);
	void SetWorldObjectProperty(PROPERTY_ID ePI, int iVal, CX2WorldObject *pChangeObject = NULL);
	void SetWorldObjectProperty(PROPERTY_ID ePI, bool bVal, CX2WorldObject *pChangeObject = NULL);
	void SetWorldObjectProperty(PROPERTY_ID ePI, float fVal, CX2WorldObject *pChangeObject = NULL);
	void SetWorldObjectProperty(PROPERTY_ID ePI, wstring wstrVal, CX2WorldObject *pChangeObject = NULL);
	void SetShowWorldObject();
	void SetFreezingWorldObject();

	void SetMoveWorldObject(bool bVal);
	void SetRotationWorldObject(bool bVal);
	void SetScaleWorldObject(bool bVal);
	void MoveWorldObject(D3DXVECTOR3 vAfterGap);
	void RotationWorldObject(D3DXVECTOR3 vAfterGap, bool bGroup = false);
	void ScaleWorldObject(D3DXVECTOR3 vAfterGap);
	void UpdateMoveWorldObject();
	void UpdateRotationWorldObject();
	void UpdateScaleWorldObject();
	void CopyWorldObject();
	void SaveWorldToFile( const WCHAR* pwszFileFullPath, bool bUpdateFileName );
	void ClearUnDoWorld();
	void AddUndoWorld(tWorldElemGroup tElem);
	void AddUndoWorld(tAddRmoveWorldElem tElem);
	void UndoWorld();

	void CreateHeightMap(long x, long y, BYTE h, long iWidht, long iHeight);
	
	void CreateThumbnail();
	void AddWorldObject( D3DXVECTOR3 vPos );

	bool CreateSector();
	void OnRenderTile();
	wstring GetSectorInfo();
	void InitReadyDungeonNpc();	
	void AddReadyDungeonNpc(CX2UnitManager::NPC_UNIT_ID npcId);
	void RemoveReadyDungeonNpc(int iIndex);

	void ChangeUnit();

private:
	void OnFrameRenderLineInfo();


public:
	CX2EnumManager				m_enumManager;

	CKTDGFontManager::CUKFont*	m_pFont_A;
	CKTDGFontManager::CUKFont*	m_pFont_B;

	wstring m_wstrSoundName;
	wstring m_wstrLineMapFullName;
	wstring m_wstrWorldFullName;

	bool m_bCanProcess;
	bool m_bEditLine;
	bool m_bEditLineVertex;
	bool m_bEditStartPosition;
	bool m_bEditWorld;
	bool m_bViewLine;
	bool m_bViewWorld;
	bool m_bEditTrigger;

	bool m_bMoveWorldObject;
	bool m_bRotationWorldObject;
	bool m_bScaleWorldObject;

	CKTDXDeviceXMesh*			m_pXMeshBoundingSphere;
	CKTDXDeviceXMesh*			m_pStartLineMesh;
	CKTDXDeviceXMesh*			m_pEndLineMesh;
	CKTDXDeviceXMesh*			m_pControlXMesh;
	CKTDXDeviceXMesh*			m_pControlYMesh;
	CKTDXDeviceXMesh*			m_pControlZMesh;
	CKTDXDeviceXMesh*			m_pControlAxis;

	D3DXVECTOR3 m_vRayLine[2];
	D3DXVECTOR3 m_vRectRayLine;
	D3DXVECTOR3 m_vPivot;

	vector<tLineVertex> m_vecLineVertex;

	bool m_bDrag;
	bool m_bLCtrl;
	bool m_bLShift;
	bool m_bLAlt;
	D3DXVECTOR2 m_vDragLineView[2];
	D3DXVECTOR2	m_vDragStartPos;
	D3DXVECTOR2 m_vMousePos;
	POINTS		mousepos;
	bool m_bMoveAxisX;
	bool m_bMoveAxisY;
	bool m_bMoveAxisZ;
	bool m_bMoveAxis;
	bool m_bShowMoveAxis;
	D3DXVECTOR3 m_vPosMoveAxis;
	bool m_bFreeMoveLine;
	D3DXVECTOR3 m_vFreeMoveLinePos;

	vector<int> m_vecLineInfo;
	vector<CKTDGLineMap::LineData*>			m_vecLineList;

	bool m_bUserCamera;
	float m_fPerspective;

	float m_fScaleAxis;
	int m_iVertPivot;
	bool m_bSnap;	
	bool m_bCopyLine;

	bool m_bFillet;
	int m_vFilletLine[2];
	D3DXVECTOR3 m_vFilletStartPos;
	D3DXVECTOR3 m_vDirFillet[2];

	vector<tLineData> m_vecUnDoLine;
	bool m_bShowLineInfo;

	bool m_bShowLineNumber;			/// 라인 번호
	bool m_bShowLineSetNumber;		/// 라인셋 번호
	bool m_bShowLineCameraNumber;	/// 라인 카메라 번호
	bool m_bShowLineTriggerNumber;	/// 라인 트리거 번호
	bool m_bShowInnerWaterInfo;		/// 물속 정보
		
	bool m_bHideUser;
	bool m_bHideNpc;
	CKTDGMatrix*				m_pMatrix;
	CKTDXDeviceXSkinMesh*		m_pXSkinMesh;
	CKTDXDeviceXSkinMesh*		m_pXSkinMeshNpc;
	vector<CKTDGXSkinAnim*>		vecpXSkinAnim;

	vector<CKTDXDeviceXSkinMesh*> m_vecDungeonSkinMeshNpc;
	vector<CKTDGXSkinAnim*>		m_vecDungeonSkinAnimNpc;

	int							m_iPickedStartPosition;
	int							m_iOverStartPosition;
	bool						m_bMoveStartPosition;

	wstring						m_wstrPickWorldObjectName;
	bool						m_bViewSelectedWorldobject;

	bool						m_bSelectedWorldObject;	
	bool						m_bPickMoveWorldObjectPivot;
	bool						m_bMoveWorld;
	D3DXVECTOR3					m_vMoveWorldObjectPivot;
	D3DXVECTOR3					m_vMoveWorldObject;
	D3DXVECTOR3					m_vRotationWorldObject;
	D3DXVECTOR3					m_vScaleWorldObject;
	D3DXMATRIX					m_matPivot;

	vector<tUNDOWORLD>			m_vecUnDoWorld;

	int							m_iWidth;
	int							m_iHeight;
	bool						m_bHeightMap;

	bool						m_bViewSelectObject;

	bool						m_bAddObjectByLib;
	wstring						m_wstrSelectedLib;
	wstring						m_wstrLibPath;
	vector<wstring>				m_vecThumbnail;
	CKTDXDeviceRenderTargetTexture *m_pRenderTargetTex;
	D3DXVECTOR3					m_vPickPos;

	bool						m_bRenderTopView;
	CKTDXDeviceRenderTargetTexture *m_pRenderTargetTopView;
	CKTDXDeviceRenderTargetTexture *m_pRenderTargetFrontView;

	ID3DXMesh*                      m_pBox;
	int                             m_iSectorWidth;
	int                             m_iSectorHeight;
	int                             m_iSectorDepth;
	bool                            m_bTileRender;
		
	std::vector<D3DXVECTOR3>        m_vecCenter;
	bool							m_bCanPlay;

	bool							m_bCheckNpc;
	bool							m_bChangeStartPos;



	KUnitSkillData					m_tSkillData;
};
#pragma endregion 