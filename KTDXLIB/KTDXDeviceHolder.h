#pragma once



class	CKTDXDevice;
struct	CKTDXDeviceData;
typedef std::list<CKTDXDeviceData>   CKTDXDeviceDataList;

class CKTDXDeviceHolder
{
public:
	CKTDXDeviceHolder(void);
	virtual ~CKTDXDeviceHolder(void);

	bool	TextureReady( const WCHAR* pFileName );
	bool	XSkinMeshReady( const WCHAR* pFileName );
	bool	XMeshReady( const WCHAR* pFileName );
	bool	SoundReady( const WCHAR* pFileName );

	// 2 ´Â CKTDXDeviceManager::PRIORITY_LOW¸¦ ³ªÅ¸³¿
	bool	TextureReadyInBackground( const WCHAR* pFileName, int iPriority = 2 );
	bool	XSkinMeshReadyInBackground( const WCHAR* pFileName, int iPriority = 2 );
	bool	XMeshReadyInBackground( const WCHAR* pFileName, int iPriority = 2 );
	bool	SoundReadyInBackground( const WCHAR* pFileName
#ifdef	X2OPTIMIZE_SOUND_BACKGROUND_LOAD		
		, int iPriority = 2
#endif	X2OPTIMIZE_SOUND_BACKGROUND_LOAD		
		);

	bool 	DeviceReady( CKTDXDevice* pDevice );
	bool 	DeviceUnready( CKTDXDevice* pDevice );

	void	ClearAllDevice();
#ifdef	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	CKTDXDeviceDataList&	AccessReadyDeviceList()	{ return m_listReadyDevice; }
#endif	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST

protected:

	void            CustomInitialize_();
	void            CustomFinalize_();

private:

#ifdef	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	CKTDXDeviceDataList		m_listReadyDevice;
#else	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST
	vector<CKTDXDevice*>	m_vecpReadyDevice;
#endif	X2OPTIMIZE_DEVICE_HOLDER_VECTOR_TO_LIST

};
