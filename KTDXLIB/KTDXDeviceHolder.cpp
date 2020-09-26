#include "StdAfx.h"
#include ".\ktdxdeviceholder.h"

CKTDXDeviceHolder::CKTDXDeviceHolder(void)
{
	CustomInitialize_();
}

/*virtual*/ CKTDXDeviceHolder::~CKTDXDeviceHolder(void)
{
	CustomFinalize_();
}

void CKTDXDeviceHolder::CustomInitialize_()
{
#ifdef	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	m_listReadyDevice.clear();
#else	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	m_vecpReadyDevice.resize( 0 );

	const size_t INITIAL_CAPACITY = 32; 
	if( m_vecpReadyDevice.capacity() < INITIAL_CAPACITY )
		m_vecpReadyDevice.reserve( INITIAL_CAPACITY );
#endif	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
}

/*virtual*/ void CKTDXDeviceHolder::CustomFinalize_()
{
	ClearAllDevice();
}

bool CKTDXDeviceHolder::TextureReady( const WCHAR* pFileName )
{
	if( NULL == pFileName || pFileName[0] == NULL )
		return false;


	CKTDXDeviceTexture* pDevice = g_pKTDXApp->GetDeviceManager()->OpenTexture( pFileName );
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );
	return true;
}

bool CKTDXDeviceHolder::XSkinMeshReady( const WCHAR* pFileName )
{
	if( NULL == pFileName || pFileName[0] == NULL )
		return false;

	CKTDXDeviceXSkinMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXSkinMesh( pFileName );
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );
	return true;
}

bool CKTDXDeviceHolder::XMeshReady( const WCHAR* pFileName )
{
	if( NULL == pFileName || pFileName[0] == NULL )
		return false;

	CKTDXDeviceXMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXMesh( pFileName );
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );
	return true;
}

bool CKTDXDeviceHolder::SoundReady( const WCHAR* pFileName )
{
	if( NULL == pFileName || pFileName[0] == NULL )
		return false;

	CKTDXDeviceSound* pDevice = g_pKTDXApp->GetDeviceManager()->OpenSound( pFileName );
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );
	return true;
}

bool CKTDXDeviceHolder::TextureReadyInBackground( const WCHAR* pFileName, int iPriority )
{

	if( NULL == pFileName || pFileName[0] == NULL )
		return false;
	
	CKTDXDeviceTexture* pDevice = g_pKTDXApp->GetDeviceManager()->OpenTextureInBackground( pFileName, (CKTDXDeviceManager::EPriority) iPriority );
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );
	return true;

}

bool CKTDXDeviceHolder::XSkinMeshReadyInBackground( const WCHAR* pFileName, int iPriority )
{
	if( NULL == pFileName || pFileName[0] == NULL )
		return false;


	CKTDXDeviceXSkinMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXSkinMeshInBackground( pFileName, (CKTDXDeviceManager::EPriority) iPriority );
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );
	return true;

}

bool CKTDXDeviceHolder::XMeshReadyInBackground( const WCHAR* pFileName, int iPriority )
{

	if( NULL == pFileName || pFileName[0] == NULL )
		return false;

	CKTDXDeviceXMesh* pDevice = g_pKTDXApp->GetDeviceManager()->OpenXMeshInBackground( pFileName,  (CKTDXDeviceManager::EPriority) iPriority );
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );
	return true;
}

bool CKTDXDeviceHolder::SoundReadyInBackground( const WCHAR* pFileName
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
	, int iPriority 
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
	)
{

	if( NULL == pFileName || pFileName[0] == NULL )
		return false;

	CKTDXDeviceSound* pDevice = g_pKTDXApp->GetDeviceManager()->OpenSoundInBackground( pFileName
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD	
			, (CKTDXDeviceManager::EPriority) iPriority 
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD			
		);
	if( pDevice == NULL )
		return false;

	DeviceReady( pDevice );

	return true;

}

bool CKTDXDeviceHolder::DeviceReady( CKTDXDevice* pDevice )
{
	KTDXPROFILE();

	if( NULL == pDevice )
		return false;
#ifdef	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	m_listReadyDevice.resize( m_listReadyDevice.size() + 1 );
	m_listReadyDevice.back().m_pDevice = pDevice;
#else	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	m_vecpReadyDevice.push_back( pDevice );
#endif	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	return true;
}


// vector에서 찾아야 하니까 필요한 경우에만 사용
bool CKTDXDeviceHolder::DeviceUnready( CKTDXDevice* pDevice )
{
	KTDXPROFILE();

	if( NULL == pDevice )
		return false;

#ifdef	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	for( CKTDXDeviceDataList::iterator iter = m_listReadyDevice.begin();
		iter != m_listReadyDevice.end(); ++iter )
	{
		if( pDevice == iter->m_pDevice )
		{
			m_listReadyDevice.erase( iter );
			return true;
		}
	}
#else	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	for( UINT i=0; i<m_vecpReadyDevice.size(); i++ )
	{
		if( pDevice == m_vecpReadyDevice[i] )
		{
			m_vecpReadyDevice.erase( m_vecpReadyDevice.begin() + i );
			SAFE_CLOSE( pDevice );
			return true;
		}
	}
#endif	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST

	return false;
}

void CKTDXDeviceHolder::ClearAllDevice()
{
#ifdef	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	m_listReadyDevice.clear();
#else	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	for( int i = 0; i < (int)m_vecpReadyDevice.size(); i++ )
	{
		CKTDXDevice* pDevice = m_vecpReadyDevice[i];
		SAFE_CLOSE( pDevice );
	}
	m_vecpReadyDevice.resize(0);
#endif	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
}
