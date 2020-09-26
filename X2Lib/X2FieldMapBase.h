#pragma once


class CX2FieldMapBase : public CKTDGObject
{
public:
	enum MINIMAP_ICON
	{
		MI_MYSELF,
		MI_PARTY,
		MI_PARTY_ARROW,

		MI_MARKET_PLACE,
		MI_PVP_ARENA,
		MI_BANK,
		MI_POSTBOX,
		MI_GATE,		

		MI_NPC_COMMON,
		MI_NPC_WEAPON_SHOP,
		MI_NPC_ACCESSARY_SHOP,
		MI_NPC_ALCHEMIST,

		// 퀘스트 관련
		MI_NPC_COMPLETE_QUEST,
		MI_NPC_COMPLETE_REPEAT_QUEST,
		MI_NPC_INCOMPLETE_QUEST,
		MI_NPC_AVAIL_QUEST,
		MI_NPC_AVAIL_REPEAT_QUEST,
		MI_NPC_AVAIL_EVENT_QUEST,

		MI_BOARD,

		MI_END,
	};

protected:
	CX2FieldMapBase(void);
	virtual ~CX2FieldMapBase(void);

public:
	virtual void SetVillageID( int iMapID ) = NULL;

	bool IsInWindowTexture( const D3DXVECTOR2& vProjectedPos );
	void CalcWindowTexturePos( const D3DXVECTOR2& vProjectedMyPos );


	D3DXVECTOR2 ProjectToScreenShot( const D3DXVECTOR3& vPos );		// 3D 공간상의 좌표를 2D 스크린샷 상의 좌표로
	D3DXVECTOR2 CalcPosInWindowTexture( const D3DXVECTOR2& vPos );	// 2D 스크린샷 상의 좌표를 2D Minimap window texture 안의 좌표로
	D3DXVECTOR2 CalcPosOnScreen( const D3DXVECTOR2& vPos );			// 2D Minimap window 안의 좌표를 실제 렌더링될 screen 상의 좌표로
	

	void ResetMapPosAndSize( int iMapID );
	void ResetCameraMatrix( int iMapID );
	void CalcCameraMatrix();


protected:
	CKTDXDeviceTexture* m_pTextureWholeMap;
	CKTDXDeviceTexture*	m_pTextureIcon[ MI_END ];

	D3DXVECTOR3 m_vEyePos;
	D3DXVECTOR3 m_vLookAt;
	float	m_fFov;		/// 시야각 값, 라디안 값임(아트에서 Degree 값을 주면 라디안으로 계산해서 넣어야됨)
	float	m_fAspect;	/// 종횡비
	
	CKTDGUIButton*	m_pMyChar;
	CKTDGUIButton*	m_pPartyMember[4];

	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_matView;
	D3DXMATRIX		m_matProj;
	D3DVIEWPORT9	m_ViewPort;

	D3DXVECTOR2 m_vScreenShotSize;			// (0,0) ~ (x, y)

	D3DXVECTOR2 m_vPosInWholeTexture;		// screen shot 상에서 상대좌표. left_top
	D3DXVECTOR2 m_vWholeTextureSize;		

	D3DXVECTOR2 m_vWindowTexturePos;		// screen shot 상에서 window의 위치, 상대좌표. left_top
	D3DXVECTOR2 m_vWindowTextureSize;

	D3DXVECTOR2 m_vMiniMapWindowPos;		// 화면에 실제 미니맵이 렌더링 될 위치, left_top
	D3DXVECTOR2 m_vMiniMapWindowSize;		// 화면에 실제 미니맵이 렌더링 될 크기

	
};
