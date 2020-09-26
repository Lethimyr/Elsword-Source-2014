#pragma once

class CKTDXCollision
{
	public:
		enum CollisionType
		{
			CT_LINE = 0,
			CT_SPHERE,
            CT_GUARD,
		};

		struct CollisionData
		{
			wstring					m_Name;
			bool					m_bEnable;
			CollisionType			m_CollisionType;

			wstring					m_FrameName;
			wstring					m_FrameNameLineEnd; //타입이 라인일 경우 라인 끝 점 프레임 네임
			
			D3DXMATRIX*				m_pCombineMatrix;
			D3DXMATRIX*				m_pCombineMatrixLineEnd;

			D3DXMATRIX*				m_pTransformMatrix;
			D3DXMATRIX*				m_pTransformMatrixLineEnd;

			D3DXMATRIX*				m_pBaseMatrix;
			D3DXMATRIX*				m_pBaseMatrixLineEnd;

			D3DXVECTOR3				m_Offset;
			D3DXVECTOR3				m_OffsetLineEnd;
			
			float					m_fRadiusScale;			// 일정기간 동안 CT_SPHERE 타입의 collision box를 키워주기 위해서
			float					m_fRadius;
#ifdef EVE_ELECTRA
			bool					m_bUseRadiusScaleY;
#endif EVE_ELECTRA
			CKTDXDevice*			m_pDeviceMesh;

			CollisionData()
			{
				m_bEnable					= true;
				m_CollisionType				= CT_SPHERE;

				m_pCombineMatrix			= NULL;
				m_pCombineMatrixLineEnd		= NULL;

				m_pTransformMatrix			= NULL;
				m_pTransformMatrixLineEnd	= NULL;

				m_pBaseMatrix				= NULL;
				m_pBaseMatrixLineEnd		= NULL;

				INIT_VECTOR3( m_Offset, 0.0f, 0.0f, 0.0f );
				INIT_VECTOR3( m_OffsetLineEnd, 0.0f, 0.0f, 0.0f );

				m_fRadiusScale				= 1.f;
				m_fRadius					= 0.0f;
#ifdef EVE_ELECTRA
				m_bUseRadiusScaleY			= false;
#endif EVE_ELECTRA
				m_pDeviceMesh				= NULL;
			}

			CollisionData& operator=( const CollisionData& data )
			{
				m_Name						= data.m_Name;
				m_bEnable					= data.m_bEnable;
				m_CollisionType				= data.m_CollisionType;

				m_FrameName					= data.m_FrameName;
				m_FrameNameLineEnd			= data.m_FrameNameLineEnd; //타입이 라인일 경우 라인 끝 점 프레임 네임

				m_pCombineMatrix			= data.m_pCombineMatrix;
				m_pCombineMatrixLineEnd		= data.m_pCombineMatrixLineEnd;

				m_pTransformMatrix			= data.m_pTransformMatrix;
				m_pTransformMatrixLineEnd	= data.m_pTransformMatrixLineEnd;

				m_pBaseMatrix				= data.m_pBaseMatrix;
				m_pBaseMatrixLineEnd		= data.m_pBaseMatrixLineEnd;

				m_Offset					= data.m_Offset;
				m_OffsetLineEnd				= data.m_OffsetLineEnd;

				m_fRadiusScale				= data.m_fRadiusScale;
				m_fRadius					= data.m_fRadius;
				m_pDeviceMesh				= data.m_pDeviceMesh;

				return *this;
			}



			void SetPoint1_LUA( const char* pFrameName, D3DXVECTOR3 offset );
			void SetPoint2_LUA( const char* pFrameName, D3DXVECTOR3 offset );

			__forceinline float GetScaleRadius( D3DXMATRIX* pWorldMat = NULL )
			{
				KTDXPROFILE();
				D3DXVECTOR3 scale;
				if( m_pCombineMatrix != NULL )
				{
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
					scale = GetDecomposeScaleXY( m_pCombineMatrix );
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
					scale = GetDecomposeScale( m_pCombineMatrix );
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
				}
				else if( pWorldMat != NULL )
				{
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
					scale = GetDecomposeScaleXY( pWorldMat );
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
					scale = GetDecomposeScale( pWorldMat );
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
				}

				ASSERT( m_fRadiusScale > 0.f );
#ifdef EVE_ELECTRA
				float fRetRadius = 0.f;
				if( m_bUseRadiusScaleY == true )
					fRetRadius = m_fRadius * m_fRadiusScale * scale.y;
				else
					fRetRadius = m_fRadius * m_fRadiusScale * scale.x;
#else
				float fRetRadius = m_fRadius * m_fRadiusScale * scale.x;
#endif EVE_ELECTRA
				return fRetRadius;
			}
			__forceinline D3DXVECTOR3 GetPointStart( D3DXMATRIX* pWorldMat = NULL )
			{
				KTDXPROFILE();
				D3DXVECTOR3 point( 0.0f, 0.0f, 0.0f );

				if( m_Offset == point )
				{
					if( m_pCombineMatrix != NULL )
					{
						INIT_VECTOR3( point, m_pCombineMatrix->_41, m_pCombineMatrix->_42, m_pCombineMatrix->_43 );
					}
					else if( pWorldMat != NULL )
					{
						INIT_VECTOR3( point, pWorldMat->_41, pWorldMat->_42, pWorldMat->_43 );
					}
				}
				else
				{
					//오프셋이 있으면 로컬 좌표계에서 오프셋 만큼 이동시킨후 좌표를 구한다
					D3DXMATRIX offsetTransform;
					D3DXMATRIX offsetMatrix;
					D3DXMatrixTranslation( &offsetTransform, m_Offset.x, m_Offset.y, m_Offset.z );
					if( m_pTransformMatrix != NULL && m_pBaseMatrix != NULL )
					{
						offsetMatrix = offsetTransform * (*m_pTransformMatrix) * (*m_pBaseMatrix);
					}
					else if( pWorldMat != NULL )
					{
						offsetMatrix = offsetTransform * (*pWorldMat);
					}
					INIT_VECTOR3( point, offsetMatrix._41, offsetMatrix._42, offsetMatrix._43 );
				}				

				return point;
			}
			__forceinline  D3DXVECTOR3 GetPointEnd( D3DXMATRIX* pWorldMat = NULL )
			{
				KTDXPROFILE();
				D3DXVECTOR3 point( 0.0f, 0.0f, 0.0f );

				if( m_OffsetLineEnd == point )
				{
					if( m_pCombineMatrixLineEnd != NULL )
					{
						INIT_VECTOR3( point, m_pCombineMatrixLineEnd->_41, m_pCombineMatrixLineEnd->_42, m_pCombineMatrixLineEnd->_43 );
					}
					else if( pWorldMat != NULL )
					{
						INIT_VECTOR3( point, pWorldMat->_41, pWorldMat->_42, pWorldMat->_43 );
					}
				}
				else
				{
					D3DXMATRIX offsetTransform;
					D3DXMATRIX offsetMatrix;
					D3DXMatrixTranslation( &offsetTransform, m_OffsetLineEnd.x, m_OffsetLineEnd.y, m_OffsetLineEnd.z );

					if( m_pTransformMatrixLineEnd != NULL && m_pBaseMatrixLineEnd != NULL )
					{
						offsetMatrix = offsetTransform * (*m_pTransformMatrixLineEnd) * (*m_pBaseMatrixLineEnd);
					}
					else if( pWorldMat != NULL )
					{
						offsetMatrix = offsetTransform * (*pWorldMat);
					}
					INIT_VECTOR3( point, offsetMatrix._41, offsetMatrix._42, offsetMatrix._43 );
				}

				return point;
			}
		};

//{{ robobeg : 2008-10-28
        typedef std::list<CollisionData*>               CollisionDataList;
        typedef std::set<const CollisionDataList*>      CollisionDataListSet;
//}} robobeg : 2008-10-28

	public:
		CKTDXCollision(void);
		~CKTDXCollision(void);

//{{ robobeg : 2008-11-04
		//__forceinline bool CollisionDataCheck( vector<CollisionData*>& dataListA, vector<CollisionData*>& dataListB, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
        __forceinline bool CollisionDataCheck( const CollisionDataList& dataListA, const CollisionDataList& dataListB, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
		{
			KTDXPROFILE();
            bool        bCollObj1, bCollObj2;
            D3DXVECTOR3 vCollObj1, vCollObj2;

            bCollObj1 = bCollObj2 = false;

            BOOST_TEST_FOREACH( CollisionData*, pColA, dataListA )
			{
                ASSERT( pColA != NULL );
				if( pColA->m_bEnable == false )
					continue;

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
				D3DXVECTOR3 kPointStartColA = D3DXVECTOR3( 0, 0, 0 );
				D3DXVECTOR3 kPointEndColA = D3DXVECTOR3( 0, 0, 0 );
				float fLengthColA = 0.0f;
				float fScaleRadiusColA = 0.0f;

				//ColA의 타입에 따라 사용되는 변수들이 다르다.
				if( pColA->m_CollisionType == CT_LINE )
				{
					kPointStartColA = pColA->GetPointStart();
					kPointEndColA = pColA->GetPointEnd();
					fLengthColA = GetDistance( kPointStartColA, kPointEndColA );
					fScaleRadiusColA = pColA->GetScaleRadius();
				}
				else
				{
					kPointStartColA = pColA->GetPointStart();
					fScaleRadiusColA = pColA->GetScaleRadius();
				}
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION

                BOOST_TEST_FOREACH( CollisionData*, pColB, dataListB )
				{
                    ASSERT( pColB != NULL );
					if( pColB->m_bEnable == false )
						continue;

					if( pColA->m_CollisionType == CT_LINE )
					{
						if( pColB->m_CollisionType == CT_LINE )
						{
							D3DXVECTOR3 pointA, pointB;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToLine( kPointStartColA, kPointEndColA,
								pColB->GetPointStart(), pColB->GetPointEnd(),
								fScaleRadiusColA + pColB->GetScaleRadius(),
								pointA, pointB ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToLine( pColA->GetPointStart(), pColA->GetPointEnd(),
								pColB->GetPointStart(), pColB->GetPointEnd(),
								pColA->GetScaleRadius() + pColB->GetScaleRadius(),
								pointA, pointB ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							{
								if( impactPoint != NULL)
								{               
                                    if(bCollObj2 != true)
                                    {
                                        bCollObj2 = true;
                                        vCollObj2 = pointA;
                                    }
                         
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                    bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                    if(cType == true)
                                        collType = CT_GUARD;
                                    else
                                        collType = pColB->m_CollisionType;

									//*impactPoint = pointA;                                    
                                    //collType = pColB->m_CollisionType;
                                    if(bCollObj2 == true)
									    return true;
								}
							}
						}
						else
						{
							D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( kPointStartColA, kPointEndColA, fLengthColA,
								fScaleRadiusColA, pColB->GetPointStart(),
								pColB->GetScaleRadius(), &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( pColA->GetPointStart(), pColA->GetPointEnd(),
								pColA->GetScaleRadius(), pColB->GetPointStart(),
								pColB->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							{
								if( impactPoint != NULL)
								{
                                    if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                    {
                                        bCollObj1 = true;
                                        vCollObj1 = pointA;
                                        continue;
                                    }

                                    if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                    {
                                        continue;
                                    }

                                    if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                    {
                                        bCollObj2 = true;
                                        vCollObj2 = pointA;
                                    }                                                             
                                    
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                    bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                    if(cType == true)
                                        collType = CT_GUARD;
                                    else
                                        collType = pColB->m_CollisionType;                                      

									//*impactPoint = pointA;
                                    //collType = pColB->m_CollisionType; 
                                    if(bCollObj2 == true)
									    return true;
								}
							}
						}
					}
					else
					{
						if( pColB->m_CollisionType == CT_LINE )
						{
							D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
								pColB->GetScaleRadius(), kPointStartColA,
								fScaleRadiusColA, &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
								pColB->GetScaleRadius(), pColA->GetPointStart(),
								pColA->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							{
								if( impactPoint != NULL)
								{
                                    if(bCollObj2 != true)
                                    {
                                        bCollObj2 = true;
                                        vCollObj2 = pointA;
                                    }

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                    if(cType == true)
                                        collType = CT_GUARD;
                                    else
                                        collType = pColB->m_CollisionType;

									//*impactPoint = pointA;
                                    //collType = pColB->m_CollisionType;

                                    if(bCollObj2 == true)
									    return true;
								}
							}
						}
						else
						{
							D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( SphereToSphere( kPointStartColA, fScaleRadiusColA,
								pColB->GetPointStart(), pColB->GetScaleRadius(), 
								&pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( SphereToSphere( pColA->GetPointStart(), pColA->GetScaleRadius(),
								pColB->GetPointStart(), pColB->GetScaleRadius(), 
								&pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							{
								if( impactPoint != NULL)
								{
                                    if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                    {
                                        bCollObj1 = true;
                                        vCollObj1 = pointA;
                                        continue;
                                    }

                                    if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                    {
                                        continue;
                                    }

                                    if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                    {
                                        bCollObj2 = true;
                                        vCollObj2 = pointA;
                                    }
                            
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                    if(cType == true)
                                        collType = CT_GUARD;
                                    else
                                        collType = pColB->m_CollisionType;

									//*impactPoint = pointA;
                                    //collType = pColB->m_CollisionType;

                                    if(bCollObj2 == true)
									    return true;
								}
							}
						}
					}
				}
			}

            if(bCollObj1 == true)
            {
                *impactPoint = vCollObj1;
                collType = CT_GUARD;
                return true;
            }

			return false;
		}

        __forceinline bool CollisionDataCheck( const CollisionDataListSet& dataListSetA, const CollisionDataList& dataListB, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
		{
			KTDXPROFILE();
            bool        bCollObj1, bCollObj2;
            D3DXVECTOR3 vCollObj1, vCollObj2;

            bCollObj1 = bCollObj2 = false;

            BOOST_TEST_FOREACH( const CollisionDataList*, pDataListA, dataListSetA )
            {
                ASSERT( pDataListA != NULL );
                BOOST_TEST_FOREACH( CollisionData*, pColA, *pDataListA )
			    {
                    ASSERT( pColA != NULL );
				    if( pColA->m_bEnable == false )
					    continue;

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
					D3DXVECTOR3 kPointStartColA = D3DXVECTOR3( 0, 0, 0 );
					D3DXVECTOR3 kPointEndColA = D3DXVECTOR3( 0, 0, 0 );
					float fLengthColA = 0.0f;
					float fScaleRadiusColA = 0.0f;

					//ColA의 타입에 따라 사용되는 변수들이 다르다.
					if( pColA->m_CollisionType == CT_LINE )
					{
						kPointStartColA = pColA->GetPointStart();
						kPointEndColA = pColA->GetPointEnd();
						fLengthColA = GetDistance( kPointStartColA, kPointEndColA );
						fScaleRadiusColA = pColA->GetScaleRadius();
					}
					else
					{
						kPointStartColA = pColA->GetPointStart();
						fScaleRadiusColA = pColA->GetScaleRadius();
					}
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION

                    BOOST_TEST_FOREACH( CollisionData*, pColB, dataListB )
				    {
                        ASSERT( pColB != NULL );
					    if( pColB->m_bEnable == false )
						    continue;

					    if( pColA->m_CollisionType == CT_LINE )
					    {
						    if( pColB->m_CollisionType == CT_LINE )
						    {
							    D3DXVECTOR3 pointA, pointB;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToLine( kPointStartColA, kPointEndColA,
									pColB->GetPointStart(), pColB->GetPointEnd(),
									fScaleRadiusColA + pColB->GetScaleRadius(),
									pointA, pointB ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToLine( pColA->GetPointStart(), pColA->GetPointEnd(),
									pColB->GetPointStart(), pColB->GetPointEnd(),
									pColA->GetScaleRadius() + pColB->GetScaleRadius(),
									pointA, pointB ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {               
                                        if(bCollObj2 != true)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }
                             
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;

									    //*impactPoint = pointA;                                    
                                        //collType = pColB->m_CollisionType;
                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
						    else
						    {
							    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( kPointStartColA, kPointEndColA, fLengthColA,
									fScaleRadiusColA, pColB->GetPointStart(),
									pColB->GetScaleRadius(), &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( pColA->GetPointStart(), pColA->GetPointEnd(),
									pColA->GetScaleRadius(), pColB->GetPointStart(),
									pColB->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {
                                        if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            bCollObj1 = true;
                                            vCollObj1 = pointA;
                                            continue;
                                        }

                                        if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            continue;
                                        }

                                        if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }                                                             
                                        
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;                                      

									    //*impactPoint = pointA;
                                        //collType = pColB->m_CollisionType; 
                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
					    }
					    else
					    {
						    if( pColB->m_CollisionType == CT_LINE )
						    {
							    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
									pColB->GetScaleRadius(), kPointStartColA,
									fScaleRadiusColA, &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
									pColB->GetScaleRadius(), pColA->GetPointStart(),
									pColA->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {
                                        if(bCollObj2 != true)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }
                                                                     
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;

									    //*impactPoint = pointA;
                                        //collType = pColB->m_CollisionType;

                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
						    else
						    {
							    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( SphereToSphere( kPointStartColA, fScaleRadiusColA,
									pColB->GetPointStart(), pColB->GetScaleRadius(), 
									&pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( SphereToSphere( pColA->GetPointStart(), pColA->GetScaleRadius(),
									pColB->GetPointStart(), pColB->GetScaleRadius(), 
									&pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {
                                        if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            bCollObj1 = true;
                                            vCollObj1 = pointA;
                                            continue;
                                        }

                                        if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            continue;
                                        }

                                        if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }
                                
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;

									    //*impactPoint = pointA;
                                        //collType = pColB->m_CollisionType;

                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
					    }
				    }
			    }
            }

            if(bCollObj1 == true)
            {
                *impactPoint = vCollObj1;
                collType = CT_GUARD;
                return true;
            }

			return false;
		}

        __forceinline bool CollisionDataCheck( const CollisionDataList& dataListA, const CollisionDataListSet& dataListSetB, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
		{
			KTDXPROFILE();
            bool        bCollObj1, bCollObj2;
            D3DXVECTOR3 vCollObj1, vCollObj2;

            bCollObj1 = bCollObj2 = false;

            BOOST_TEST_FOREACH( CollisionData*, pColA, dataListA )
			{
                ASSERT( pColA != NULL );
				if( pColA->m_bEnable == false )
					continue;

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
				D3DXVECTOR3 kPointStartColA = D3DXVECTOR3( 0, 0, 0 );
				D3DXVECTOR3 kPointEndColA = D3DXVECTOR3( 0, 0, 0 );
				float fLengthColA = 0.0f;
				float fScaleRadiusColA = 0.0f;

				//ColA의 타입에 따라 사용되는 변수들이 다르다.
				if( pColA->m_CollisionType == CT_LINE )
				{
					kPointStartColA = pColA->GetPointStart();
					kPointEndColA = pColA->GetPointEnd();
					fLengthColA = GetDistance( kPointStartColA, kPointEndColA );
					fScaleRadiusColA = pColA->GetScaleRadius();
				}
				else
				{
					kPointStartColA = pColA->GetPointStart();
					fScaleRadiusColA = pColA->GetScaleRadius();
				}
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION

                BOOST_TEST_FOREACH( const CollisionDataList*, pDataListB, dataListSetB )
                {
                    ASSERT( pDataListB != NULL );
                    BOOST_TEST_FOREACH( CollisionData*, pColB, *pDataListB )
				    {
                        ASSERT( pColB != NULL );
					    if( pColB->m_bEnable == false )
						    continue;

					    if( pColA->m_CollisionType == CT_LINE )
					    {
						    if( pColB->m_CollisionType == CT_LINE )
						    {
							    D3DXVECTOR3 pointA, pointB;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToLine( kPointStartColA, kPointEndColA,
									pColB->GetPointStart(), pColB->GetPointEnd(),
									fScaleRadiusColA + pColB->GetScaleRadius(),
									pointA, pointB ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToLine( pColA->GetPointStart(), pColA->GetPointEnd(),
									pColB->GetPointStart(), pColB->GetPointEnd(),
									pColA->GetScaleRadius() + pColB->GetScaleRadius(),
									pointA, pointB ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {               
                                        if(bCollObj2 != true)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }
                             
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;

									    //*impactPoint = pointA;                                    
                                        //collType = pColB->m_CollisionType;
                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
						    else
						    {
							    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( kPointStartColA, kPointEndColA, fLengthColA,
									fScaleRadiusColA, pColB->GetPointStart(),
									pColB->GetScaleRadius(), &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( pColA->GetPointStart(), pColA->GetPointEnd(),
									pColA->GetScaleRadius(), pColB->GetPointStart(),
									pColB->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {
                                        if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            bCollObj1 = true;
                                            vCollObj1 = pointA;
                                            continue;
                                        }

                                        if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            continue;
                                        }

                                        if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }                                                             
                                        
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;                                      

									    //*impactPoint = pointA;
                                        //collType = pColB->m_CollisionType; 
                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
					    }
					    else
					    {
						    if( pColB->m_CollisionType == CT_LINE )
						    {
							    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
									pColB->GetScaleRadius(), kPointStartColA,
									fScaleRadiusColA, &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
									pColB->GetScaleRadius(), pColA->GetPointStart(),
									pColA->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {
                                        if(bCollObj2 != true)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }
                                                                     
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;

									    //*impactPoint = pointA;
                                        //collType = pColB->m_CollisionType;

                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
						    else
						    {
							    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( SphereToSphere( kPointStartColA, fScaleRadiusColA,
									pColB->GetPointStart(), pColB->GetScaleRadius(), 
									&pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
								if( SphereToSphere( pColA->GetPointStart(), pColA->GetScaleRadius(),
									pColB->GetPointStart(), pColB->GetScaleRadius(), 
									&pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							    {
								    if( impactPoint != NULL)
								    {
                                        if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            bCollObj1 = true;
                                            vCollObj1 = pointA;
                                            continue;
                                        }

                                        if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                        {
                                            continue;
                                        }

                                        if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                        {
                                            bCollObj2 = true;
                                            vCollObj2 = pointA;
                                        }
                                
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
										bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                        if(cType == true)
                                            collType = CT_GUARD;
                                        else
                                            collType = pColB->m_CollisionType;

									    //*impactPoint = pointA;
                                        //collType = pColB->m_CollisionType;

                                        if(bCollObj2 == true)
									        return true;
								    }
							    }
						    }
					    }
                    }
				}
			}

            if(bCollObj1 == true)
            {
                *impactPoint = vCollObj1;
                collType = CT_GUARD;
                return true;
            }

			return false;
		}

        __forceinline bool CollisionDataCheck( const CollisionDataListSet& dataListSetA, const CollisionDataListSet& dataListSetB, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
		{
			KTDXPROFILE();
            bool        bCollObj1, bCollObj2;
            D3DXVECTOR3 vCollObj1, vCollObj2;

            bCollObj1 = bCollObj2 = false;

            BOOST_TEST_FOREACH( const CollisionDataList*, pDataListA, dataListSetA )
            {
                ASSERT( pDataListA != NULL );
                BOOST_TEST_FOREACH( CollisionData*, pColA, *pDataListA )
			    {
                    ASSERT( pColA != NULL );
				    if( pColA->m_bEnable == false )
					    continue;

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
					D3DXVECTOR3 kPointStartColA = D3DXVECTOR3( 0, 0, 0 );
					D3DXVECTOR3 kPointEndColA = D3DXVECTOR3( 0, 0, 0 );
					float fLengthColA = 0.0f;
					float fScaleRadiusColA = 0.0f;

					//ColA의 타입에 따라 사용되는 변수들이 다르다.
					if( pColA->m_CollisionType == CT_LINE )
					{
						kPointStartColA = pColA->GetPointStart();
						kPointEndColA = pColA->GetPointEnd();
						fLengthColA = GetDistance( kPointStartColA, kPointEndColA );
						fScaleRadiusColA = pColA->GetScaleRadius();
					}
					else
					{
						kPointStartColA = pColA->GetPointStart();
						fScaleRadiusColA = pColA->GetScaleRadius();
					}
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION

                    BOOST_TEST_FOREACH( const CollisionDataList*, pDataListB, dataListSetB )
                    {
                        ASSERT( pDataListB != NULL );
                        BOOST_TEST_FOREACH( CollisionData*, pColB, *pDataListB )
				        {
                            ASSERT( pColB != NULL );
					        if( pColB->m_bEnable == false )
						        continue;

					        if( pColA->m_CollisionType == CT_LINE )
					        {
						        if( pColB->m_CollisionType == CT_LINE )
						        {
							        D3DXVECTOR3 pointA, pointB;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( LineToLine( kPointStartColA, kPointEndColA,
										pColB->GetPointStart(), pColB->GetPointEnd(),
										fScaleRadiusColA + pColB->GetScaleRadius(),
										pointA, pointB ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( LineToLine( pColA->GetPointStart(), pColA->GetPointEnd(),
										pColB->GetPointStart(), pColB->GetPointEnd(),
										pColA->GetScaleRadius() + pColB->GetScaleRadius(),
										pointA, pointB ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							        {
								        if( impactPoint != NULL)
								        {               
                                            if(bCollObj2 != true)
                                            {
                                                bCollObj2 = true;
                                                vCollObj2 = pointA;
                                            }

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                            bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                            if(cType == true)
                                                collType = CT_GUARD;
                                            else
                                                collType = pColB->m_CollisionType;

									        //*impactPoint = pointA;                                    
                                            //collType = pColB->m_CollisionType;
                                            if(bCollObj2 == true)
									            return true;
								        }
							        }
						        }
						        else
						        {
							        D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( LineToSphere( kPointStartColA, kPointEndColA, fLengthColA,
										fScaleRadiusColA, pColB->GetPointStart(),
										pColB->GetScaleRadius(), &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( LineToSphere( pColA->GetPointStart(), pColA->GetPointEnd(),
										pColA->GetScaleRadius(), pColB->GetPointStart(),
										pColB->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							        {
								        if( impactPoint != NULL)
								        {
                                            if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                            {
                                                bCollObj1 = true;
                                                vCollObj1 = pointA;
                                                continue;
                                            }

                                            if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                            {
                                                continue;
                                            }

                                            if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                            {
                                                bCollObj2 = true;
                                                vCollObj2 = pointA;
                                            }                                                             
                                            
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											D3DXVECTOR3 vCenter = kPointEndColA + (kPointEndColA - kPointStartColA) * 3.0f;
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											D3DXVECTOR3 vCenter = pColA->GetPointEnd() + (pColA->GetPointEnd() - pColA->GetPointStart()) * 3.0f;
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                            bool cType = CheckCollisionGuard(vCenter, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
                                            if(cType == true)
                                                collType = CT_GUARD;
                                            else
                                                collType = pColB->m_CollisionType;                                      

									        //*impactPoint = pointA;
                                            //collType = pColB->m_CollisionType; 
                                            if(bCollObj2 == true)
									            return true;
								        }
							        }
						        }
					        }
					        else
					        {
						        if( pColB->m_CollisionType == CT_LINE )
						        {
							        D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
										pColB->GetScaleRadius(), kPointStartColA,
										fScaleRadiusColA, &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
										pColB->GetScaleRadius(), pColA->GetPointStart(),
										pColA->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							        {
								        if( impactPoint != NULL)
								        {
                                            if(bCollObj2 != true)
                                            {
                                                bCollObj2 = true;
                                                vCollObj2 = pointA;
                                            }
                                                                         
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                            if(cType == true)
                                                collType = CT_GUARD;
                                            else
                                                collType = pColB->m_CollisionType;

									        //*impactPoint = pointA;
                                            //collType = pColB->m_CollisionType;

                                            if(bCollObj2 == true)
									            return true;
								        }
							        }
						        }
						        else
						        {
							        D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( SphereToSphere( kPointStartColA, fScaleRadiusColA,
										pColB->GetPointStart(), pColB->GetScaleRadius(), 
										&pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
									if( SphereToSphere( pColA->GetPointStart(), pColA->GetScaleRadius(),
										pColB->GetPointStart(), pColB->GetScaleRadius(), 
										&pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							        {
								        if( impactPoint != NULL)
								        {
                                            if(bCollObj1 != true && pColB->m_CollisionType == CT_GUARD)
                                            {
                                                bCollObj1 = true;
                                                vCollObj1 = pointA;
                                                continue;
                                            }

                                            if(bCollObj1 == true && pColB->m_CollisionType == CT_GUARD)
                                            {
                                                continue;
                                            }

                                            if(bCollObj2 != true && pColB->m_CollisionType != CT_GUARD)
                                            {
                                                bCollObj2 = true;
                                                vCollObj2 = pointA;
                                            }
                                    
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											bool cType = CheckCollisionGuard(kPointStartColA, bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
											bool cType = CheckCollisionGuard(pColA->GetPointStart(), bCollObj1, vCollObj1, bCollObj2, vCollObj2, impactPoint);
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
                                            if(cType == true)
                                                collType = CT_GUARD;
                                            else
                                                collType = pColB->m_CollisionType;

									        //*impactPoint = pointA;
                                            //collType = pColB->m_CollisionType;

                                            if(bCollObj2 == true)
									            return true;
								        }
							        }
						        }
					        }
				        }
                    }
			    }
            }

            if(bCollObj1 == true)
            {
                *impactPoint = vCollObj1;
                collType = CT_GUARD;
                return true;
            }

			return false;
		}

		//__forceinline bool CollisionDataCheck( vector<CollisionData*>& dataListA, CollisionData* pCollisionData, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
		__forceinline bool CollisionDataCheck( const CollisionDataList& dataListA, CollisionData* pColB, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
        {
			KTDXPROFILE();

            ASSERT( pColB != NULL );
            if ( pColB->m_bEnable == false )
                return false;

            BOOST_TEST_FOREACH( CollisionData*, pColA, dataListA )
			{
                ASSERT( pColA != NULL );
                if( pColA->m_bEnable == false )
					continue;

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
				D3DXVECTOR3 kPointStartColA = D3DXVECTOR3( 0, 0, 0 );
				D3DXVECTOR3 kPointEndColA = D3DXVECTOR3( 0, 0, 0 );
				float fLengthColA = 0.0f;
				float fScaleRadiusColA = 0.0f;

				//ColA의 타입에 따라 사용되는 변수들이 다르다.
				if( pColA->m_CollisionType == CT_LINE )
				{
					kPointStartColA = pColA->GetPointStart();
					kPointEndColA = pColA->GetPointEnd();
					fLengthColA = GetDistance( kPointStartColA, kPointEndColA );
					fScaleRadiusColA = pColA->GetScaleRadius();
				}
				else
				{
					kPointStartColA = pColA->GetPointStart();
					fScaleRadiusColA = pColA->GetScaleRadius();
				}
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION

				if( pColA->m_CollisionType == CT_LINE )
				{
					if( pColB->m_CollisionType == CT_LINE )
					{
						D3DXVECTOR3 pointA, pointB;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( LineToLine( kPointStartColA, kPointEndColA,
							pColB->GetPointStart(), pColB->GetPointEnd(),
							fScaleRadiusColA + pColB->GetScaleRadius(),
							pointA, pointB ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( LineToLine( pColA->GetPointStart(), pColA->GetPointEnd(),
							pColB->GetPointStart(), pColB->GetPointEnd(),
							pColA->GetScaleRadius() + pColB->GetScaleRadius(),
							pointA, pointB ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						{
							if( impactPoint != NULL)
							{
								*impactPoint = pointA;
                                collType = pColB->m_CollisionType;                                
								return true;
							}
						}
					}
					else
					{
						D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( LineToSphere( kPointStartColA, kPointEndColA, fLengthColA,
							fScaleRadiusColA, pColB->GetPointStart(),
							pColB->GetScaleRadius(), &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( LineToSphere( pColA->GetPointStart(), pColA->GetPointEnd(),
							pColA->GetScaleRadius(), pColB->GetPointStart(),
							pColB->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						{
							if( impactPoint != NULL )
							{
								*impactPoint = pointA;
                                collType = pColB->m_CollisionType;                             
								return true;
							}
						}
					}
				}
				else
				{
					if( pColB->m_CollisionType == CT_LINE )
					{
						D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
							pColB->GetScaleRadius(), kPointStartColA,
							fScaleRadiusColA, &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
							pColB->GetScaleRadius(), pColA->GetPointStart(),
							pColA->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						{
							if( impactPoint != NULL)
							{
								*impactPoint = pointA;
                                collType = pColB->m_CollisionType;
								return true;
							}
						}
					}
					else
					{
						D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( SphereToSphere( kPointStartColA, fScaleRadiusColA,
							pColB->GetPointStart(), pColB->GetScaleRadius(), 
							&pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						if( SphereToSphere( pColA->GetPointStart(), pColA->GetScaleRadius(),
							pColB->GetPointStart(), pColB->GetScaleRadius(), 
							&pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						{
							if( impactPoint != NULL)
							{
								*impactPoint = pointA;
                                collType = pColB->m_CollisionType;
								return true;
							}
						}
					}
				}
			}            
			return false;
		}


		__forceinline bool CollisionDataCheck( const CollisionDataListSet& dataListSetA, CollisionData* pColB, CKTDXCollision::CollisionType &collType, D3DXVECTOR3* impactPoint = NULL )
        {
			KTDXPROFILE();

            ASSERT( pColB != NULL );
            if ( pColB->m_bEnable == false )
                return false;

            BOOST_TEST_FOREACH( const CollisionDataList*, pDataListA, dataListSetA )
            {
                ASSERT( pDataListA != NULL );
                BOOST_TEST_FOREACH( CollisionData*, pColA, *pDataListA )
			    {
                    ASSERT( pColA != NULL );
                    if( pColA->m_bEnable == false )
					    continue;

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
					D3DXVECTOR3 kPointStartColA = D3DXVECTOR3( 0, 0, 0 );
					D3DXVECTOR3 kPointEndColA = D3DXVECTOR3( 0, 0, 0 );
					float fLengthColA = 0.0f;
					float fScaleRadiusColA = 0.0f;

					//ColA의 타입에 따라 사용되는 변수들이 다르다.
					if( pColA->m_CollisionType == CT_LINE )
					{
						kPointStartColA = pColA->GetPointStart();
						kPointEndColA = pColA->GetPointEnd();
						fLengthColA = GetDistance( kPointStartColA, kPointEndColA );
						fScaleRadiusColA = pColA->GetScaleRadius();
					}
					else
					{
						kPointStartColA = pColA->GetPointStart();
						fScaleRadiusColA = pColA->GetScaleRadius();
					}
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION

				    if( pColA->m_CollisionType == CT_LINE )
				    {
					    if( pColB->m_CollisionType == CT_LINE )
					    {
						    D3DXVECTOR3 pointA, pointB;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToLine( kPointStartColA, kPointEndColA,
								pColB->GetPointStart(), pColB->GetPointEnd(),
								fScaleRadiusColA + pColB->GetScaleRadius(),
								pointA, pointB ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToLine( pColA->GetPointStart(), pColA->GetPointEnd(),
								pColB->GetPointStart(), pColB->GetPointEnd(),
								pColA->GetScaleRadius() + pColB->GetScaleRadius(),
								pointA, pointB ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						    {
							    if( impactPoint != NULL)
							    {
								    *impactPoint = pointA;
                                    collType = pColB->m_CollisionType;                                
								    return true;
							    }
						    }
					    }
					    else
					    {
						    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( kPointStartColA, kPointEndColA, fLengthColA,
								fScaleRadiusColA, pColB->GetPointStart(),
								pColB->GetScaleRadius(), &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( pColA->GetPointStart(), pColA->GetPointEnd(),
								pColA->GetScaleRadius(), pColB->GetPointStart(),
								pColB->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						    {
							    if( impactPoint != NULL )
							    {
								    *impactPoint = pointA;
                                    collType = pColB->m_CollisionType;                             
								    return true;
							    }
						    }
					    }
				    }
				    else
				    {
					    if( pColB->m_CollisionType == CT_LINE )
					    {
						    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
								pColB->GetScaleRadius(), kPointStartColA,
								fScaleRadiusColA, &pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( LineToSphere( pColB->GetPointStart(), pColB->GetPointEnd(),
								pColB->GetScaleRadius(), pColA->GetPointStart(),
								pColA->GetScaleRadius(), &pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						    {
							    if( impactPoint != NULL)
							    {
								    *impactPoint = pointA;
                                    collType = pColB->m_CollisionType;
								    return true;
							    }
						    }
					    }
					    else
					    {
						    D3DXVECTOR3 pointA;
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( SphereToSphere( kPointStartColA, fScaleRadiusColA,
								pColB->GetPointStart(), pColB->GetScaleRadius(), 
								&pointA ) == true )
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
							if( SphereToSphere( pColA->GetPointStart(), pColA->GetScaleRadius(),
								pColB->GetPointStart(), pColB->GetScaleRadius(), 
								&pointA ) == true )
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
						    {
							    if( impactPoint != NULL)
							    {
								    *impactPoint = pointA;
                                    collType = pColB->m_CollisionType;
								    return true;
							    }
						    }
					    }
				    }
			    }            
            }
			return false;
		}
//}} robobeg : 2008-11-04


		__forceinline bool SphereToSphere( const float& center1X, const float& center1Y, const float& center1Z, const float& radius1, 
								const float& center2X, const float& center2Y, const float& center2Z, const float& radius2, 
								D3DXVECTOR3* point = NULL )
		{
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
			//자주사용됨으로 속도를 위해...
			return SphereToSphere( D3DXVECTOR3( center1X, center1Y, center1Z ), radius1, D3DXVECTOR3( center2X, center2Y, center2Z ), radius2, point );
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
			D3DXVECTOR3 center1;
			D3DXVECTOR3 center2;

			center1.x = center1X;
			center1.y = center1Y;
			center1.z = center1Z;

			center2.x = center2X;
			center2.y = center2Y;
			center2.z = center2Z;

			return SphereToSphere( center1, radius1, center2, radius2, point );
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
		}
		__forceinline bool SphereToSphere( const D3DXVECTOR3& center1, const float& radius1, 
								const D3DXVECTOR3& center2, const float& radius2, 
								D3DXVECTOR3* point = NULL )
		{
			KTDXPROFILE();
			float fDistance = GetDistance( center1, center2 );

			if( fDistance < (radius1 + radius2) )
			{
				if( point != NULL )
				{
					D3DXVECTOR3 dir = center2 - center1;
					D3DXVec3Normalize( &dir, &dir );
					*point = center1 + (dir * (fDistance / 2.0f));
				}
				return true;
			}

			return false;
		}

		__forceinline bool LineToSphere( const float& lineStartX, const float& lineStartY, const float& lineStartZ, 
							const float& lineEndX,	const float& lineEndY,	const float& lineEndZ, const float& lineRadius,
							const float& centerX,	const float& centerY,	const float& centerZ, 
							const float& radius,	D3DXVECTOR3* point = NULL )
		{
#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
			//자주사용됨으로 속도를 위해...
			return LineToSphere( D3DXVECTOR3( lineStartX, lineStartY, lineStartZ), 
				D3DXVECTOR3( lineEndX, lineEndY, lineEndZ), 
				lineRadius, 
				D3DXVECTOR3( centerX, centerY, centerZ), 
				radius, 
				point );
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
			D3DXVECTOR3 lineStart = D3DXVECTOR3( lineStartX, lineStartY, lineStartZ);
			D3DXVECTOR3 lineEnd = D3DXVECTOR3( lineEndX, lineEndY, lineEndZ);
			D3DXVECTOR3 center = D3DXVECTOR3( centerX, centerY, centerZ);

			return LineToSphere( lineStart, lineEnd, lineRadius, center, radius, point );
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
		}

#ifdef X2OPTIMIZE_DAMAGEEFFECT_COLLISION
		__forceinline bool LineToSphere( const D3DXVECTOR3& lineStart, const D3DXVECTOR3& lineEnd, const float& lineRadius,
			const D3DXVECTOR3& center, const float& radius,
			D3DXVECTOR3* point = NULL )
		{
			return LineToSphere( lineStart, lineEnd, GetDistance( lineStart, lineEnd ), lineRadius, center, radius, point );
		}

		__forceinline bool LineToSphere( const D3DXVECTOR3& lineStart, const D3DXVECTOR3& lineEnd, const float& lineDistance, const float& lineRadius,
			const D3DXVECTOR3& center, const float& radius,
			D3DXVECTOR3* point = NULL )
		{
			KTDXPROFILE();

			float sphereLength = GetDistance( lineStart, center ) - radius - lineRadius;

			if( lineDistance < sphereLength )
				return false;

			D3DXVECTOR3 lineDir = lineEnd - lineStart;
			D3DXVec3Normalize( &lineDir, &lineDir );

			D3DXVECTOR3 sphereDir = center - lineStart;

			float fProjectionLength	= D3DXVec3Dot( &lineDir, &sphereDir );
			if( fProjectionLength < 0.0f )
				return false;
			D3DXVECTOR3 projectionPoint = lineDir * fProjectionLength + lineStart;
			float finalLength = GetDistance( center, projectionPoint );

			if( finalLength >= 0.0f && finalLength > (radius + lineRadius) )
				return false;

			//if( fProjectionLength > rayLength || fProjectionLength < 0.0f )
			//	return false;

			if( point != NULL )
			{
				*point = projectionPoint;
			}
			return true;
		}
#else//X2OPTIMIZE_DAMAGEEFFECT_COLLISION
		__forceinline bool LineToSphere( const D3DXVECTOR3& lineStart, const D3DXVECTOR3& lineEnd, const float& lineRadius,
							const D3DXVECTOR3& center, const float& radius,
							D3DXVECTOR3* point = NULL )
		{
			KTDXPROFILE();

			float rayLength = GetDistance( lineStart, lineEnd );
			float sphereLength = GetDistance( lineStart, center ) - radius - lineRadius;

			if( rayLength < sphereLength )
				return false;

			D3DXVECTOR3 lineDir = lineEnd - lineStart;
			D3DXVec3Normalize( &lineDir, &lineDir );

			D3DXVECTOR3 sphereDir = center - lineStart;

			float fProjectionLength	= D3DXVec3Dot( &lineDir, &sphereDir );
			if( fProjectionLength < 0.0f )
				return false;
			D3DXVECTOR3 projectionPoint = lineDir * fProjectionLength + lineStart;
			float finalLength = GetDistance( center, projectionPoint );

			if( finalLength >= 0.0f && finalLength > (radius + lineRadius) )
				return false;

			//if( fProjectionLength > rayLength || fProjectionLength < 0.0f )
			//	return false;

			if( point != NULL )
			{
				*point = projectionPoint;
			}
			return true;
		}
#endif//X2OPTIMIZE_DAMAGEEFFECT_COLLISION

		__forceinline bool DXRayCheck( LPD3DXBASEMESH pDestMesh, D3DXVECTOR3 rayPos, const D3DXVECTOR3& rayDir, const D3DXVECTOR3& transform, float* distance )
		{
			KTDXPROFILE();
			BOOL retval;

			DWORD pFaceIndex;
			FLOAT pU;
			FLOAT pV;
			//LPD3DXBUFFER ppAllHits;
			DWORD pCountOfHits;

			rayPos = rayPos - transform;

			D3DXIntersect( pDestMesh, &rayPos, &rayDir, &retval, &pFaceIndex,
				&pU, &pV, distance, NULL, &pCountOfHits );

			if( retval == TRUE )
				return true;
			else
				return false;
		}
		__forceinline bool DXRayCheck( LPD3DXBASEMESH pDestMesh, D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, const D3DXMATRIX& transform, float* distance )
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

		__forceinline bool LineToLine( const float& A1x, const float& A1y, const float& A1z,
						const float& A2x, const float& A2y, const float& A2z, 
						const float& B1x, const float& B1y, const float& B1z,
						const float& B2x, const float& B2y, const float& B2z,
						bool infinite_lines, float epsilon,
						float &PointOnSegAx, float &PointOnSegAy, float &PointOnSegAz,
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
		
		__forceinline bool LineToLine( const D3DXVECTOR3& L1Start, const D3DXVECTOR3& L1End, const D3DXVECTOR3& L2Start, const D3DXVECTOR3& L2End, const float& radius, D3DXVECTOR3& pointL1, D3DXVECTOR3& pointL2 )
		{
			return LineToLine( L1Start.x, L1Start.y, L1Start.z, L1End.x, L1End.y, L1End.z,
								L2Start.x, L2Start.y, L2Start.z, L2End.x, L2End.y, L2End.z,
								false, radius, 
								pointL1.x, pointL1.y, pointL1.z, pointL2.x, pointL2.y, pointL2.z );
		}


#ifdef LINEMAP_RECT_TEST
		// @Epsilon : 오차 범위
		// @pNearestOutPoint : OUT param, 만약 점이 사각형 내부에 있다면 가장 가까운 사각형 바깥의 위치 값을 리턴
		__forceinline bool IsPointInRect( const D3DXVECTOR3& vPoint, const D3DXVECTOR3& vRectLeftTop, const D3DXVECTOR3& vRectRightBottom, const D3DXVECTOR3& vRectLeftBottom, const float fEpsilon, 
			D3DXVECTOR3* pNearestOutPoint )
		{
			// 평면의 normal vector와 원점에서 평면까지의 거리를 계산한다
			D3DXVECTOR3 vNormalToPlane(0, 0, 0);
			D3DXVec3Cross( &vNormalToPlane, &(vRectLeftTop-vRectLeftBottom), &(vRectRightBottom-vRectLeftBottom) );
			float fDistToPlane = D3DXVec3Dot( &vRectLeftTop, &vNormalToPlane );


			const float fNormalLength = D3DXVec3Length( &vNormalToPlane );
			if( 0.f == fNormalLength )
				return false;

			if( fDistToPlane < 0.f )
			{
				vNormalToPlane = -vNormalToPlane;
				fDistToPlane = -fDistToPlane;
			}



			// 점과 평면사이의 거리가 오차범위내인지 확인
			float fDotPointNormal = D3DXVec3Dot( &vNormalToPlane, &vPoint );
			float fPointToPlaneDist = fabs( fDotPointNormal - fDistToPlane ) / fNormalLength;

			if( fPointToPlaneDist > fEpsilon )
				return false;


			// 점을 평면위로 투사
			D3DXVECTOR3 vPointProjectedOnPlane = vPoint + ( fDistToPlane - fDotPointNormal ) / fNormalLength * vNormalToPlane;

			
			// 투사된 점이 평면에 포함되는지 
			D3DXVECTOR3 vOA = vRectLeftTop - vRectLeftBottom;
			D3DXVECTOR3 vOB = vRectRightBottom - vRectLeftBottom;
			D3DXVECTOR3 vOPoint = vPointProjectedOnPlane - vRectLeftBottom;

			D3DXVECTOR3 vCrossOAOB(0, 0, 0);
			D3DXVec3Cross( &vCrossOAOB, &vOA, &vOB );
			D3DXVec3Normalize( &vCrossOAOB, &vCrossOAOB );
			D3DXVECTOR3 vOA_(0, 0, 0);
			D3DXVec3Cross( &vOA_, &vOB, &vCrossOAOB );
			D3DXVec3Normalize( &vOA_, &vOA_ );
			D3DXVECTOR3 vOB_(0, 0, 0);
			D3DXVec3Cross( &vOB_, &vCrossOAOB, &vOA );
			D3DXVec3Normalize( &vOB_, &vOB_ );
			

			float fU = D3DXVec3Dot( &vOPoint, &vOA_ ) / D3DXVec3Length( &vOA );
			if( fU < 0.f || fU > 1.f )
				return false;

			float fV = D3DXVec3Dot( &vOPoint, &vOB_ ) / D3DXVec3Length( &vOB );
			if( fV < 0.f || fV > 1.f )
				return false;



			if( NULL != pNearestOutPoint )
			{
				if( fU > 0.5f )
				{
					fU = 1.f - fU;
					vOA = -vOA;
				}

				if( fV > 0.5f ) 
				{
					fV = 1.f - fV;
					vOB = -vOB;
				}

				if( fU < fV )
				{
					*pNearestOutPoint = vPointProjectedOnPlane - vOA * fU;
				}
				else
				{
					*pNearestOutPoint = vPointProjectedOnPlane - vOB * fV;
				}
			}
			return true;
		}
#endif LINEMAP_RECT_TEST
		





		__forceinline bool RayToPlane( OUT float& fDistance, OUT D3DXVECTOR3& vIntersection, 
			const D3DXVECTOR3& vRayOrigin, const D3DXVECTOR3& vRayDirection, 
			const D3DXVECTOR3& vPlaneVertex, const D3DXVECTOR3& vPlaneNormal )
		{
			// ray: P = P0 + t*V
			// plane: P.N + d = 0
			float VdotN = D3DXVec3Dot( &vRayDirection, &vPlaneNormal );
			float P0dotN = D3DXVec3Dot( &vRayOrigin, &vPlaneNormal );
			float d = -D3DXVec3Dot( &vPlaneVertex, &vPlaneNormal );
			if( VdotN == 0.f )
				return false;

			float t = -(P0dotN + d) / VdotN;
			fDistance = t;

			if( t < 0.f ) // no intersection
			{				
				return false;
			}
			else
			{				
				vIntersection = vRayOrigin + vRayDirection * t;
				return true;
			}
		}
		


		// 피타고라스 정리를 이용해서 점과 선 사이의 최단 거리를 구한다
		__forceinline float LinePointDistance( const D3DXVECTOR3& vPoint, const D3DXVECTOR3& vPointOnLine, const D3DXVECTOR3& vLineDir )
		{
			//ASSERT( D3DXVec3LengthSq( &vLineDir ) == 1.f );
			D3DXVECTOR3 vDiagonal = vPoint - vPointOnLine;
			float fDiagonalLengthSq = D3DXVec3LengthSq( &vDiagonal );
			float fProjectedLength = D3DXVec3Dot( &vLineDir, &vDiagonal );
			float fLineVectorLengthSq = D3DXVec3LengthSq( &vLineDir );

			return sqrt( fDiagonalLengthSq - fProjectedLength*fProjectedLength / fLineVectorLengthSq );
		}


		__forceinline D3DXVECTOR3 FindNearestPointOnLineSegment( const D3DXVECTOR3& vPoint, const D3DXVECTOR3& vLineStart, const D3DXVECTOR3& vLineEnd )
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


	private:

		__forceinline void FindNearestPointOnLineSegment( const float& A1x, const float& A1y, const float& A1z,
											const float& Lx, const float& Ly, const float& Lz,
											const float& Bx, const float& By, const float& Bz,
											bool infinite_line, float epsilon_squared, float &NearestPointX,
											float &NearestPointY, float &NearestPointZ,
											float &parameter )
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

		__forceinline void FindNearestPointOfParallelLineSegments( float A1x, float A1y, float A1z,
														float A2x, float A2y, float A2z,
														float Lax, float Lay, float Laz,
														float B1x, float B1y, float B1z,
														float B2x, float B2y, float B2z,
														float Lbx, float Lby, float Lbz,
														bool infinite_lines, float epsilon_squared,
														float &PointOnSegAx, float &PointOnSegAy, float &PointOnSegAz,
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

		__forceinline void AdjustNearestPoints( float A1x, float A1y, float A1z,
									float Lax, float Lay, float Laz,
									float B1x, float B1y, float B1z,
									float Lbx, float Lby, float Lbz,
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

        bool CheckCollisionGuard(D3DXVECTOR3 vCenter, bool bGuard, D3DXVECTOR3 vGuard, bool bColl, D3DXVECTOR3 vColl, D3DXVECTOR3* impactPoint = NULL)
        {
#if 0
            // 라인과 충돌시 라인의 시작점(End?)과 충돌점의 거리를 비교 가까운 박스를 충돌박스로 판정한다.
            if(bGuard == true && bColl == true)
            {                
                float fLen1 = D3DXVec3Length(&(vGuard - vCenter));    
                float fLen2 = D3DXVec3Length(&(vColl - vCenter));

                if(fLen1 <= fLen2)
                {
                    *impactPoint = vGuard;
                    return true;
                }
                else
                {
                    *impactPoint = vColl;
                    return false;
                }
            }
            
            if(bGuard != true && bColl == true)
            {
                *impactPoint = vColl;
                return false;
            }

            if(bGuard == true && bColl != true)
            {
                *impactPoint = vGuard;
                return true;
            }
#else
            // 가드박스와 일반박스와 충돌시 무조건 가드박스에 충돌했다고 판정한다.
            if(bGuard == true)
            {
                *impactPoint = vGuard;
                return true;
            }

            *impactPoint = vColl;
            return false;
#endif

            return false;
        }
};

