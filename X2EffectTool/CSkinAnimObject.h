#pragma once
class CSkinAnimObject
{
public:
	CSkinAnimObject(void);
	virtual ~CSkinAnimObject(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

public:
	void	Clear();
	bool	SetAnimXSkinMesh( std::wstring fileName );

	void	ChangeAnim( std::wstring animName );
	void	ChangeAnim( int index );

	void	Play( CKTDGXSkinAnim::XSKIN_ANIM_PLAYTYPE playType );
	void	SetPos( const D3DXVECTOR3& vPos );
	D3DXVECTOR3 GetPos();

	void SetRotationDegree( const D3DXVECTOR3& vRotation );
	D3DXVECTOR3 GetRotationDegree();


	D3DXVECTOR3 GetMotionOffset() { return m_pXSkinAnim->GetMotionOffset(); }
	bool	IsPlaying() { return !m_pXSkinAnim->IsAnimationEnd(); }

	CKTDGXRenderer::RenderParam* GetRenderParam();

	void	SetShow( bool bShow	) { m_bShow = bShow; } 

	CKTDGXSkinAnim* GetXSkinAnim() const { return m_pXSkinAnim; }

	bool	GetAnimNameList( std::vector<std::wstring>& vecAnimNameList );
	bool	GetFrameNameList( OUT std::vector<std::wstring>& vecFrameNameList );

#ifdef SHOW_ATTACK_BOX_DUMMY
	void ToggleShowAttackBox() { m_bShowAttackBox = !m_bShowAttackBox; }
#endif // SHOW_ATTACK_BOX_DUMMY



public:
	CKTDGXSkinAnim*									m_pXSkinAnim;
	std::map<std::wstring, CKTDXDeviceXSkinMesh*>	m_mapSkinMesh;

	bool											m_bShow;
	int												m_iNowAniNameIndex;

private:
#ifdef SHOW_ATTACK_BOX_DUMMY
	CKTDXDeviceXMesh*								m_pXMeshSphere;
	CKTDGMatrix*									m_pMatrixSphere;
	bool											m_bShowAttackBox;
#endif // SHOW_ATTACK_BOX_DUMMY


};