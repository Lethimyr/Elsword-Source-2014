#include "dxstdafx.h"
#include ".\x2viewerparticle.h"

CX2ViewerParticle::CX2ViewerParticle( CX2ViewerSkinMesh* pSkinMesh )
{
	CX2ViewerObject::SetObjectStyle( CX2ViewerObject::OS_PARTICLE );

	m_iLastPlayCount		= 0;
	m_LastAnimName		= L"";
	m_pSkinMeshObj = pSkinMesh;
	m_pParticleSystem	= new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        );
	m_pCustomParticleSystem = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 1
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK        
        );

	m_mode = PM_NORMAL;
	m_bPreviewRepeat = true;
	m_fPreviewPlaySpeed = 1.0f;
	m_fElapsedPlayTime = 0;

	m_ParticleEffData.clear();
	m_pParticleSystem->OpenScriptFile( L"Particle.txt" );
	m_pCustomParticleSystem->OpenScriptFile( L"ParticleTemp.txt" );
}
CX2ViewerParticle::~CX2ViewerParticle()
{
	for( int i =0; i < (int)m_ParticleEffData.size(); i++ )
	{
		ParticleEffectData* pParticleEffectData = m_ParticleEffData[i];
		if( pParticleEffectData->m_bIsTemplet )
		{
			m_pParticleSystem->DestroyInstanceHandle( pParticleEffectData->m_hSeq );
		}
		else
		{
			m_pCustomParticleSystem->DestroyInstanceHandle( pParticleEffectData->m_hSeq );
		}
		
		SAFE_DELETE( pParticleEffectData );
	}
	m_ParticleEffData.clear();

	SAFE_DELETE(m_pParticleSystem);
	SAFE_DELETE(m_pCustomParticleSystem);
}

HRESULT CX2ViewerParticle::OnFrameMove( double fTime, float fElapsedTime )
{
	switch( m_mode )
	{
	case PM_NORMAL:
		{
			if(m_pParticleSystem != NULL)
				m_pParticleSystem->OnFrameMove( fTime, fElapsedTime );
			if(m_pCustomParticleSystem != NULL)
				m_pCustomParticleSystem->OnFrameMove( fTime, fElapsedTime );

			CKTDGXSkinAnim*	pXSkinAnim;
			pXSkinAnim = m_pSkinMeshObj->GetXSkinAnim();
			if( pXSkinAnim == NULL )
				return S_OK;

			if( pXSkinAnim != NULL && (pXSkinAnim->GetNowPlayCount() != m_iLastPlayCount || pXSkinAnim->GetNowAnimName() != m_LastAnimName) )
			{		
				m_iLastPlayCount = pXSkinAnim->GetNowPlayCount();
				m_LastAnimName = pXSkinAnim->GetNowAnimName();
				ResetAllParticle();
			}
			for( vector<ParticleEffectData*>::iterator it = m_ParticleEffData.begin(); it < m_ParticleEffData.end(); ++it )
			{
				ParticleEffectData* pParticleData = *it;
				CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
				if( pParticleData->m_bIsTemplet )
				{
					pSeq = m_pParticleSystem->GetInstanceSequence( pParticleData->m_hSeq );
				}
				else
				{
					pSeq = m_pCustomParticleSystem->GetInstanceSequence( pParticleData->m_hSeq );
				}

				if( pParticleData->m_bUse == false && pParticleData->m_hSeq == INVALID_PARTICLE_HANDLE )
				{
					if( pXSkinAnim->EventTimer( pParticleData->m_fTime ) == true )
					{
						pParticleData->m_bUse = true;
						D3DXVECTOR3 pos(0,0,0);
						if( pParticleData->m_bWeapon == true )
						{
							pos = m_pSkinMeshObj->GetWeaponXSkinAnim()->GetCloneFramePosition( pParticleData->m_Pos.c_str() );
						}
						else
						{
							if( pParticleData->m_Pos.length() != 0 )
							{
								pos = pXSkinAnim->GetCloneFramePosition( pParticleData->m_Pos.c_str() );
							}
						}

						if( pParticleData->m_bLandPosition )
							pos.y = 0;
						// offset
						pos += pParticleData->m_vOffset;

						if( pParticleData->m_bIsTemplet )
						{
							pParticleData->m_hSeq = m_pParticleSystem->CreateSequenceHandle( NULL, pParticleData->m_SequenceName.c_str(), pos );
							pSeq = m_pParticleSystem->GetInstanceSequence( pParticleData->m_hSeq );
						}
						else
						{
							pParticleData->m_hSeq = m_pCustomParticleSystem->CreateSequenceHandle( NULL, pParticleData->m_SequenceName.c_str(), pos );
							pSeq = m_pCustomParticleSystem->GetInstanceSequence( pParticleData->m_hSeq );
						}

						// rot
						D3DXVECTOR3 vRot(0,0,0);
						vRot += pParticleData->m_vRotation;
						
						if( NULL != pSeq )
						{
							pSeq->SetAddRotate( vRot );
							pSeq->SetAxisAngle( vRot );
							pSeq->SetScaleFactor( pParticleData->m_vScale );
						}

						// 				if(pParticleEffectData->m_bUnitScale == true)
						// 				{
						// 					CKTDXDeviceXSkinMesh::MultiAnimFrame* pFrame = m_pXSkinAnim->GetCloneFrame( pParticleEffectData->m_Pos.c_str() );
						// 					float sx = pFrame->baseMatrix._11;
						// 					float sy = pFrame->baseMatrix._22;
						// 					float sz = pFrame->baseMatrix._33;
						// 
						// 					pParticleEffectData->m_vScale = D3DXVECTOR3(sx, sy, sz);


					}
				}

				// 메인터넌스
				if( pParticleData->m_hSeq != INVALID_PARTICLE_HANDLE )
				{
					if( pParticleData->m_bIsTemplet && m_pParticleSystem->IsLiveInstanceHandle( pParticleData->m_hSeq ) == false )
					{
						pParticleData->m_hSeq = INVALID_PARTICLE_HANDLE;
					}
					else if( !pParticleData->m_bIsTemplet && m_pCustomParticleSystem->IsLiveInstanceHandle( pParticleData->m_hSeq ) == false )
					{
						pParticleData->m_hSeq = INVALID_PARTICLE_HANDLE;
					}
					else
					{						
						if( pSeq != NULL )
						{
							if( pParticleData->m_bTrace )
							{
								D3DXVECTOR3 pos(0,0,0);
								if( pParticleData->m_bWeapon == true )
								{
									pos = m_pSkinMeshObj->GetWeaponXSkinAnim()->GetCloneFramePosition( pParticleData->m_Pos.c_str() );
								}
								else
								{
									if( pParticleData->m_Pos.length() != 0 )
									{
										pos = pXSkinAnim->GetCloneFramePosition( pParticleData->m_Pos.c_str() );
									}
								}

								if( pParticleData->m_bLandPosition )
									pos.y = 0;
								// offset
								pos += pParticleData->m_vOffset;


								if( NULL != pSeq )
								{
									pSeq->SetPosition( pos );
								}

							}

							// rotation
							D3DXVECTOR3 rot(0,0,0);
							rot += pParticleData->m_vRotation;
							if( NULL != pSeq )
							{
								pSeq->SetAddRotate( rot );
								pSeq->SetAxisAngle( rot );
							}
						}

					}

				}
			}

		} break;
	case PM_EDITOR:
		{
			fElapsedTime *= m_fPreviewPlaySpeed;
			m_fElapsedPlayTime += fElapsedTime;
			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
			if( m_PreviewParticleEffData.m_bIsTemplet )
			{
				m_pParticleSystem->OnFrameMove( fTime, fElapsedTime );
				if( m_pParticleSystem->IsLiveInstanceHandle( m_PreviewParticleEffData.m_hSeq ) == false && m_bPreviewRepeat == true )
				{
                    m_PreviewParticleEffData.m_hSeq = m_pParticleSystem->CreateSequenceHandle( NULL, m_PreviewParticleEffData.m_SequenceName.c_str(), 0, 0, 0 );
					m_fElapsedPlayTime = 0;
				}
				pSeq = m_pParticleSystem->GetInstanceSequence( m_PreviewParticleEffData.m_hSeq );
			}
			else
			{
				m_pCustomParticleSystem->OnFrameMove( fTime, fElapsedTime );
				if( m_pCustomParticleSystem->IsLiveInstanceHandle( m_PreviewParticleEffData.m_hSeq ) == false && m_bPreviewRepeat == true )
				{
					m_PreviewParticleEffData.m_hSeq = m_pCustomParticleSystem->CreateSequenceHandle( NULL,m_PreviewParticleEffData.m_SequenceName.c_str(), 0, 0, 0 );
					m_fElapsedPlayTime = 0;
				}
				pSeq = m_pCustomParticleSystem->GetInstanceSequence( m_PreviewParticleEffData.m_hSeq );
			}
			if( pSeq != NULL )
			{
				// rot
				D3DXVECTOR3 vRot(0,0,0);
				vRot += m_PreviewParticleEffData.m_vRotation;

				if( NULL != pSeq )
				{
					pSeq->SetAddRotate( vRot );
					pSeq->SetAxisAngle( vRot );
					pSeq->SetScaleFactor( m_PreviewParticleEffData.m_vScale );
				}
				//pSeq->OnFrameMove( fTime, fElapsedTime );
			}

		} break;
	}
	

	return S_OK;

}
HRESULT CX2ViewerParticle::OnFrameRender()
{
	switch( m_mode )
	{
	case PM_NORMAL:
		{
			for( vector<ParticleEffectData*>::iterator it = m_ParticleEffData.begin(); it<m_ParticleEffData.end(); ++it)
			{
				ParticleEffectData* pData = *it;
				CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
				if( pData->m_bIsTemplet )
				{
					pSeq = m_pParticleSystem->GetInstanceSequence( pData->m_hSeq );
				}
				else
				{
					pSeq = m_pCustomParticleSystem->GetInstanceSequence( pData->m_hSeq );
				}

				if( pSeq != NULL )
				{
					int     iStackTop = CKTDGStateManager::GetStackTop();
					CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
					pSeq->OnFrameRender_Draw();
					CKTDGStateManager::PopStates( iStackTop );			
				}
			}
		} break;
	case PM_EDITOR:
		{
			CKTDGParticleSystem::CParticleEventSequence* pSeq = NULL;
			if( m_PreviewParticleEffData.m_bIsTemplet )
			{
				pSeq = m_pParticleSystem->GetInstanceSequence( m_PreviewParticleEffData.m_hSeq );
			}
			else
			{
				pSeq = m_pCustomParticleSystem->GetInstanceSequence( m_PreviewParticleEffData.m_hSeq );
			}

			if( pSeq != NULL )
			{
				int     iStackTop = CKTDGStateManager::GetStackTop();
				CKTDGStateManager::PushStates( pSeq->GetRenderStateID() );
				pSeq->OnFrameRender_Draw();
				CKTDGStateManager::PopStates( iStackTop );			
			}
		}
	}
	
	
	return S_OK;
}
HRESULT CX2ViewerParticle::OnResetDevice()
{
	m_pParticleSystem->OnResetDevice();
	m_pCustomParticleSystem->OnResetDevice();

	return S_OK;
}
HRESULT CX2ViewerParticle::OnLostDevice()
{
	m_pParticleSystem->OnLostDevice();
	m_pCustomParticleSystem->OnLostDevice();

	return S_OK;
}

CX2ViewerParticle::ParticleEffectData* CX2ViewerParticle::GetParticleEffectDataByName( const wstring name )
{

	for( vector<ParticleEffectData*>::iterator it = m_ParticleEffData.begin(); it < m_ParticleEffData.end(); ++it )
	{
		ParticleEffectData* pData = *it;
		if( name == pData->m_Name )
		{
			return pData;
		}
	}
	return NULL;
}

bool CX2ViewerParticle::DeleteParticleEffectData( const wstring name )
{
	for( vector<ParticleEffectData*>::iterator it = m_ParticleEffData.begin(); it < m_ParticleEffData.end(); ++it )
	{
		ParticleEffectData* pData = *it;
		if( name == pData->m_Name )
		{
			if(pData->m_bIsTemplet)
			{
				m_pParticleSystem->DestroyInstanceHandle( pData->m_hSeq );
			}
			else
			{
				m_pCustomParticleSystem->DestroyInstanceHandle( pData->m_hSeq );
			}
			SAFE_DELETE( pData );
			m_ParticleEffData.erase( it );
			return true;
		}
	}
	return false;
}

void CX2ViewerParticle::SaveParticleEffectData( const WCHAR* pwszFileFullPath )
{	
	// file path와 name을 분리
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	WCHAR ext[_MAX_EXT];
	_wsplitpath( pwszFileFullPath, drive, dir, fname, ext);

	string strFileName;
	ConvertWCHARToChar( strFileName, fname );
	strFileName += ".lua";

	std::wfstream fstrm;
	fstrm.open( strFileName.c_str(), std::fstream::out | std::fstream::trunc );

	fstrm << L"Particle Setup" << std::endl << std::endl;
	fstrm << L"!! NOTE !!" << std::endl;
	fstrm << L"Major and StateEndDelete bool values are Set to Default : true." << std::endl;
	fstrm << L"Please Check and Edit." << std::endl << std::endl;
	if( m_pSkinMeshObj != NULL && m_pSkinMeshObj->GetXSkinAnim() != NULL )
	{
		fstrm << L"Anim Name : " << m_pSkinMeshObj->GetXSkinAnim()->GetNowAnimName() << std::endl << std::endl;
	}
	
	fstrm << L"PARTICLE_SEQ = " << std::endl;
	fstrm << L"{" << std::endl;
	fstrm << L"\t--Major, time, Name, weaponBonePos, Pos, StateEndDelete, bTrace, posx, posy, posz, bApplyUnitRotation, rotx, roty, rotz," << std::endl;


	for( vector<ParticleEffectData*>::iterator it = m_ParticleEffData.begin(); it < m_ParticleEffData.end(); ++it )
	{
		ParticleEffectData* pData = *it;

		fstrm << L"\t{ TRUE, ";		// Major
		fstrm << pData->m_fTime << ", ";
		fstrm << L"\"" << pData->m_SequenceName << L"\", ";
		if(pData->m_bWeapon)
		{
			fstrm << L"TRUE, ";
		}
		else
		{
			fstrm << L"FALSE, ";
		}
		fstrm << L"\"" << pData->m_Pos << L"\", ";
		fstrm << L"TRUE, ";			// StateEndDelete
		if(pData->m_bTrace)
		{
			fstrm << L"TRUE, ";
		}
		else
		{
			fstrm << L"FALSE, ";
		}
		fstrm << pData->m_vOffset.x << L", " << pData->m_vOffset.y << L", " << pData->m_vOffset.z << L", ";

		if(pData->m_bApplyUnitRotation)
		{
			fstrm << L"TRUE, ";
			if( pData->m_vRotation != D3DXVECTOR3(0,0,0) )
			{
				fstrm << pData->m_vRotation.x << L", " << pData->m_vRotation.y << L", " << pData->m_vRotation.z << L", ";
			}

		}
		fstrm << L"}," << std::endl;
	}

	fstrm << "}," << std::endl;
	fstrm.close();
	ConvertFileAnsiToUTF8( strFileName, strFileName );

	return;

}

bool CX2ViewerParticle::AddParticleEffectData( CX2ViewerParticle::ParticleEffectData* pData )
{
	if(pData == NULL)
	{
		WARNINGMSG( L"내부 에러" );
		return false;
	}

	if( GetParticleEffectDataByName( pData->m_Name ) != NULL )
	{
		WARNINGMSG( L"같은 이름의 파티클이 이미 존재" );
		return false;
	}

	m_ParticleEffData.push_back( pData );
	return true;
}

void CX2ViewerParticle::ResetAllParticle()
{
	for( vector<ParticleEffectData*>::iterator it = m_ParticleEffData.begin(); it < m_ParticleEffData.end(); ++it )
	{
		ParticleEffectData* pData = *it;
		if(pData->m_bIsTemplet)
		{
			m_pParticleSystem->DestroyInstanceHandle( pData->m_hSeq );
		}
		else
		{
			m_pCustomParticleSystem->DestroyInstanceHandle( pData->m_hSeq );
		}
		
		pData->m_hSeq = INVALID_PARTICLE_HANDLE;
		pData->m_bUse = false;
	}
}

void CX2ViewerParticle::ReloadParticleFile()
{
	if( m_pParticleSystem != NULL )
		SAFE_DELETE( m_pParticleSystem );
	
	m_pParticleSystem = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 0
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK        
        );
	m_pParticleSystem->OpenScriptFile( L"Particle.txt" );
}


void CX2ViewerParticle::SetPreviewParticle( wstring name, bool bIsTemplet )
{
	// 우선 이전 Preview를 지우고
	if(m_PreviewParticleEffData.m_bIsTemplet)
	{
		m_pParticleSystem->DestroyInstanceHandle( m_PreviewParticleEffData.m_hSeq );
	}
	else
	{
		m_pCustomParticleSystem->DestroyInstanceHandle( m_PreviewParticleEffData.m_hSeq );
	}

	m_PreviewParticleEffData.m_bIsTemplet = bIsTemplet;
	if( bIsTemplet )
	{
		m_PreviewParticleEffData.m_hSeq = m_pParticleSystem->CreateSequenceHandle( NULL, name.c_str(), 0, 0, 0 );
	}
	else
	{
		m_PreviewParticleEffData.m_hSeq = m_pCustomParticleSystem->CreateSequenceHandle( NULL, name.c_str(), 0, 0, 0 );
	}
	
	m_PreviewParticleEffData.m_SequenceName = name;
	m_fElapsedPlayTime = 0;
}

bool CX2ViewerParticle::CopyTempletParticleToCustom( wstring name )
{	

	map<wstring, CKTDGParticleSystem::CParticleEventSequence*>::const_iterator it;
	it = m_pParticleSystem->GetTempletSequences().find( name );

	if( it == m_pParticleSystem->GetTempletSequences().end() )
	{
		WARNINGMSG( L"particle not found" );
		return false;
	}

	CKTDGParticleSystem::CParticleEventSequence* pTemplet = it->second;
	CKTDGParticleSystem::CParticleEventSequence* pNewSeq = pTemplet->GetCloneSequence();
	if( pNewSeq == NULL )
	{
		//** SHOW ERROR MESSAGE
		WARNINGMSG( L"내부 오류 : 파티클 생성 실패" );
		return false;
	}
	
	pNewSeq->SetParticleEventSequence( pTemplet );
	pNewSeq->SetParticleSystem( m_pCustomParticleSystem );
	
	

	WCHAR buf[256];
	int i = 0;
	wstring newname;
	do 
	{
		wsprintf(buf, L"%s_copy%02d", name.c_str(), i);
		i++;
		pNewSeq->SetName( buf );
		newname = buf;
	} while(m_pCustomParticleSystem->GetTempletSequencesByName( newname ));
	
	m_pCustomParticleSystem->AddTempletSequence(newname, pNewSeq);	
	m_X2ParticleEditor.ExportParticleScript( m_pCustomParticleSystem, L"ParticleTemp.txt" );
	SAFE_DELETE( m_pCustomParticleSystem );
	m_pCustomParticleSystem = new CKTDGParticleSystem( g_pKTDXApp->GetDevice()
#ifdef  X2OPTIMIZE_HANDLE_VALIDITY_CHECK
        , 1
#endif  X2OPTIMIZE_HANDLE_VALIDITY_CHECK        
        );
	m_pCustomParticleSystem->OpenScriptFile( L"ParticleTemp.txt" );

	
	return true;	
}

void CX2ViewerParticle::ReplayPreviewParticle()
{	
	m_fElapsedPlayTime = 0;
	if( m_PreviewParticleEffData.m_bIsTemplet )
	{	
		m_pParticleSystem->DestroyInstanceHandle( m_PreviewParticleEffData.m_hSeq );
		m_PreviewParticleEffData.m_hSeq = m_pParticleSystem->CreateSequenceHandle( NULL, m_PreviewParticleEffData.m_SequenceName.c_str(), 0, 0, 0 );
	}
	else
	{	
		m_pCustomParticleSystem->DestroyInstanceHandle( m_PreviewParticleEffData.m_hSeq );
		m_PreviewParticleEffData.m_hSeq = m_pCustomParticleSystem->CreateSequenceHandle( NULL,m_PreviewParticleEffData.m_SequenceName.c_str(), 0, 0, 0 );
	}
}

