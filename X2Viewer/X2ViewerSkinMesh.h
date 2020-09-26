#pragma once
/**	@file	: X2ViewerSkinMesh.h
	@desc	: 메쉬 파일 관련
	@author	: Anomymouse

	@date	JHKang on 2013-09-13,
		- 필드 이후 빌드 성공
	@date	JHKang on 2013-09-17,
		- 파일 정리 및 블록화
*/

#pragma region CX2ViewerSkinMesh
/** @class : CX2ViewerSkinMesh
	@brief : 메쉬 관련 클래스
*/
class CX2ViewerSkinMesh : public CX2ViewerObject
{
public:
	struct SBoneInfo;
	struct SUnitBoneModify;
	
	typedef std::map<std::wstring, CKTDXDeviceXSkinMesh*> XSkinMeshMap;
	typedef std::vector<SBoneInfo*> ModifyBoneInfoVector;
	typedef std::vector<SUnitBoneModify*> UnitBoneModifyVector;
	typedef std::map<std::wstring, SWeaponInfo*> WeaponMap;
	typedef std::map<std::wstring, SAccessoryInfo*> AccessoryMap;

	enum SKINMESH_OPEN_TYPE
	{
		SOT_NONE = 0,		/// 없음
		SOT_SKINMESH,		/// 모션이 있는 메쉬
		SOT_MESH,			/// 모션이 없는 메쉬
		SOT_NOT_ADDMESH,	/// 추가되지 않는 메쉬
		SOT_END,			/// 열거체의 끝
	};

	/** @struct : SModifyBoneInfo
		@brief : 뼈대 변환 정보
		@date  : 2013/09/28
	*/
	struct SBoneInfo
	{
		wstring				m_strBoneName;		/// 뼈대 이름
		MODIFY_BONE_STYLE	m_eModifyBoneStyle;	/// 뼈대 변환 방법
		D3DXVECTOR3			m_vValue;			/// 위치 또는 크기 변화 값
	};
		
	/** @struct : SUnitBoneModify
		@brief : 캐릭터의 뼈대들 변환 정보
		@date  : 2013/09/28
	*/
	struct SUnitBoneModify
	{
		CHARACTER_ID			m_eUnitID;				/// UNIT 번호
		ModifyBoneInfoVector	m_vecModifyBoneInfo;	/// 현재 모션의 뼈대 변환 정보
	};


public:
	CX2ViewerSkinMesh(void);
	virtual ~CX2ViewerSkinMesh(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();
	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();
    virtual bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


	bool	SetAnimXSkinMesh( std::wstring fileName );
	bool	AddWeapon( const WCHAR* pFullFileName, const WCHAR* attachFrameName, D3DXVECTOR3 rot );
    bool    AddAccessory( const WCHAR* pFullFileName, const WCHAR* attachFrameName, D3DXVECTOR3 rot );
	bool	DelModelXSkinMesh( std::wstring fileName );
	bool	DelModelXSkinAnim( std::wstring fileName );
	bool	DelModelXMesh( std::wstring fileName );
	SKINMESH_OPEN_TYPE	InsertSkinMesh( std::wstring& fileName, std::wstring& dir );
	
	void	Reset();
	void	ChangeAnim( std::wstring animName );
	void	ChangeAnim( int index );

	void	SetScale( D3DXVECTOR3& vScale )	{ m_vScale = vScale; }
	void	SetScale( float fX, float fY, float fZ );
	void	SetScaleX( float fX )	{ m_vScale.x = fX; }
	void	SetScaleY( float fY )	{ m_vScale.y = fY; }
	void	SetScaleZ( float fZ )	{ m_vScale.z = fZ; }

	void	SetPosition( D3DXVECTOR3& vPosition )	{ m_vPosition = vPosition; }
	void	SetPosition( float fX, float fY, float fZ );
	void	SetPositionX( float fX )	{ m_vPosition.x = fX; }
	void	SetPositionY( float fY )	{ m_vPosition.y = fY; }
	void	SetPositionZ( float fZ )	{ m_vPosition.z = fZ; }

    void    SetTransX( float fX )    { m_TransAccessory.x = fX; }
    void    SetTransY( float fY )    { m_TransAccessory.y = fY; }
    void    SetTransZ( float fZ )    { m_TransAccessory.z = fZ; }
    void    SetAccRotX( float fX )    { m_RotAccessory.x = fX; }
    void    SetAccRotY( float fY )    { m_RotAccessory.y = fY; }
    void    SetAccRotZ( float fZ )    { m_RotAccessory.z = fZ; }
    void    SetAccScaleX( float fX )  { m_ScaleAccessory.x = fX; }
    void    SetAccScaleY( float fY )  { m_ScaleAccessory.y = fY; }
    void    SetAccScaleZ( float fZ )  { m_ScaleAccessory.z = fZ; }

    void    SetWeaponRotX( float fX )       { m_RotWeapon.x = fX; }
    void    SetWeaponRotY( float fY )       { m_RotWeapon.y = fY; }
    void    SetWeaponRotZ( float fZ )       { m_RotWeapon.z = fZ; }

	float    GetWeaponRotX()	{ return m_RotWeapon.x; }
    float    GetWeaponRotY()	{ return m_RotWeapon.y; }
    float    GetWeaponRotZ()	{ return m_RotWeapon.z; }

	void	SetLightPos( D3DXVECTOR3& vLightPos )	{ m_RenderParam.lightPos = vLightPos; }
	void	SetLightPos( float fX, float fY, float fZ );
	void	SetLightPosX( float fX )	{ m_RenderParam.lightPos.x = fX; }
	void	SetLightPosY( float fY )	{ m_RenderParam.lightPos.y = fY; }
	void	SetLightPosZ( float fZ )	{ m_RenderParam.lightPos.z = fZ; }

	void	SetWireFrameMode( bool bWireFrame )	{ m_bWireframeMode = bWireFrame; }

	D3DXVECTOR3&	GetScale()	{ return m_vScale; }
	D3DXVECTOR3&	GetPosition()	{ return m_vPosition; }
	D3DXVECTOR3&	GetLightPos()	{ return m_RenderParam.lightPos; }
	void	SetLightOnOff( bool bIsLight )	{ m_bIsLight = bIsLight; }

    bool	GetFrameNameList( VecMultiAnimFrame& vecFrameNameList );
	bool	GetAnimNameList( std::vector<std::wstring>& vecAnimNameList );

	bool	SetAnimPlayPause();
	void	SetAnimStop();
	void	SetMotionOnOff( bool bIsMotion );
	bool	GetMotionOnOff()	{ return m_bIsMotion; }
	void	SetPlaySpeed( float fPlaySpeed );
	void	GetAnimTime( float& fNowTime, float& fMaxTime );
	void	SetAnimTime( float fTime );
	void	SetPlayType( WCHAR* wszPlayType );
	void	SetBounding( bool bIsBounding )	{ m_bIsBounding = bIsBounding; }
	bool	GetBounding()	{ return m_bIsBounding; }
	void	SetShowAttackBox( bool bShowAttackBox ) { m_bShowAttackBox = bShowAttackBox; }
	bool	GetShowAttackBox() { return m_bShowAttackBox; }

	CKTDGXRenderer::RenderParam*	GetRenderParam()	{ return &m_RenderParam; }
	IMPACT_DATA*					GetImpactData()		{ return &m_ImpactData; }
	std::vector<TEX_STAGE_DATA>*	GetTexStageData()	{ return &m_vecTexStageData; }
	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	GetPlayType()	{ return m_AnimPlaytype; }

	CKTDGXSkinAnim*	GetXSkinAnim()			{ return m_pXSkinAnim; }
	//CKTDGXSkinAnim* GetWeaponXSkinAnim( Value_ )	{ return m_mapXSkinWeapon[iValue_]; }
	WeaponMap& GetWeaponXSkinAnim() { return m_mapXSkinWeapon; }
	CKTDGXSkinAnim* GetWeaponXSkinAnim( wstring strName_ );
	SWeaponInfo* GetWeaponInfo( wstring strName_ );

	AccessoryMap& GetAccessoryXMesh() { return m_mapXMeshAccessory; }
	CKTDXDeviceXMesh* GetAccessoryXMesh( wstring strName_ );
	SAccessoryInfo* GetAccessoryInfo( wstring strName_ );
		
    void SetAttachPoint(WCHAR *szName);
    void SelectionChange( DWORD dwX,  DWORD dwY);
    BOOL BIntersectMeshContainer ( CKTDXDeviceXSkinMesh::MultiAnimMC *pmcMesh, DWORD dwX, DWORD dwY, D3DVIEWPORT9 *pViewport,
		D3DXMATRIX *pmatProjection, D3DXMATRIX *pmatView, float *pfDistMin, /*SMeshContainer **ppmcHit,*/ DWORD *pdwFaceHit,
        DWORD *pdwVertHit );

	bool OpenScriptFile( IN const WCHAR* pFileName_, KLuabinder* pKLuabinder );
	void RefreshBoneInfoScript( KLuabinder* pKLuabinder );
	bool AddBoneInfo();
	void SetUnitReForm( CHARACTER_ID eValue_ = CI_NONE );
	void GetModifyBoneList( CHARACTER_ID eValue_, VecMultiAnimFrame &vecBoneList_ );

private:
	CKTDGXSkinAnim*		m_pXSkinAnim;			/// 메인 모션
	CKTDXDeviceXET*		m_pXET;					/// 모션 XET
	
	CKTDGXRenderer*		m_pRendererAccessory;	/// 악세서리에 렌더링 파라미터 적용
    
	CKTDXDeviceXMesh*   m_pXMeshAccessory;		/// 악세서리 메쉬
	AccessoryMap		m_mapXMeshAccessory;	/// 악세서리 메쉬 맵
	
	WeaponMap			m_mapXSkinWeapon;		/// 무기 스킨 메쉬 맵
	CKTDXDeviceXET*		m_pXETWeapon;			/// 무기 XET 벡터

	XSkinMeshMap		m_mapSkinMesh;			/// SkinMesh Map Data

	CKTDXDeviceXMesh*	m_pXMeshLight;			/// 태양 모양 메쉬
	CKTDGMatrix*		m_pMatrixLight;			/// 빛 행렬

	CKTDXDeviceXMesh*	m_pXMeshSphere;			/// 충돌용 메쉬
	CKTDXDeviceXMesh*	m_pXAttackMeshSphere;	/// 어택용 메쉬
	CKTDGMatrix*		m_pMatrixSphere;		/// 충돌, 어택박스용 행렬

	D3DXVECTOR3			m_vScale;				/// 비율 벡터
	D3DXVECTOR3			m_vPosition;			/// 위치 벡터

	bool	m_bWireframeMode;	/// 와이어 프레임 모드
	bool	m_bIsLight;			/// 빛 사용
	bool	m_bIsAnimPlay;		/// 재생
	bool	m_bIsMotion;		/// 모션의 모델 메쉬를 표시할 것인가?
	bool	m_bIsBounding;		/// 바운딩 박스 표시
	bool	m_bShowAttackBox;	/// 어택 박스 표시

	CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE	m_AnimPlaytype;			/// 재생 방식
	CKTDGXRenderer::RenderParam			m_RenderParam;			/// 렌더링 파리미터
	IMPACT_DATA							m_ImpactData;			/// 플로우 이펙트 정보
	std::vector<TEX_STAGE_DATA>			m_vecTexStageData;		/// 텍스처 스테이지 데이터
	float								m_ImpactNowAnimTime;	/// 플로우 이펙트 갑 계산을 위한 시간 값

	D3DXMATRIX*			m_pWeaponMatrix;	/// 행렬
	D3DXVECTOR3			m_RotWeapon;		/// 무기 회전

    D3DXMATRIX*			m_pMatrixAccessory;	/// 악세서리 행렬
    D3DXVECTOR3			m_TransAccessory;	/// 이동 벡터
    D3DXVECTOR3			m_RotAccessory;		/// 회전 벡터
    D3DXVECTOR3			m_ScaleAccessory;	/// 비율 벡터

    bool                m_bAttachPoint;		/// 붙이는 위치
    CKTDGXRenderer*		m_pRendererPoint;	/// 렌더링 포인터
    CKTDXDeviceXMesh*	m_pXMeshPoint;		/// 메쉬의 포인터
    D3DXMATRIX*			m_pMatrixPoint;		/// 행렬 포인터
	D3DXVECTOR3			m_vBoneScale;		/// 뼈대 크기 벡터

	UnitBoneModifyVector	m_vUnitBoneModify;		/// 기본 전직 뼈대들 변환 정보를 가지는 벡터

	//LPD3DXMESH	m_pSMesh;
	//CKTDGMatrix* m_pSMatrix;
};
#pragma endregion class
