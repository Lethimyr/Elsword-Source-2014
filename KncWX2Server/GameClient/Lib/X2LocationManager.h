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

class CX2LocationManager
{
	DeclareSingleton( CX2LocationManager );

public:		
	enum MAP_ID
	{
		MI_NONE = 0,

		MI_RUBEN			= 20000,
		MI_VELDER_NORTH		= 10000,
		MI_ELDER			= 20001,
		MI_VELDER_EAST		= 10001,
		MI_BESMA			= 20002,
		MI_VELDER_SOUTH		= 10002,
		MI_ALTERA			= 20003,
		MI_ALTERA_ISLAND	= 10003,
	};

	struct NpcFieldTalk
	{
		bool m_bUserName;
		std::wstring m_msgNpcTalk;
	};

	struct CommonNpcPos
	{
		int			m_iNpcId;
		D3DXVECTOR3 m_vNpcPos;			
		D3DXVECTOR3 m_vNpcRot;
	};

	struct VillageTemplet
	{
		int					m_MapID;
		std::wstring		m_FrameMoveFuncName;

		float				m_fCameraDistance;
		float				m_fLookAtHeight;
		float				m_fEyeHeight;
		std::vector<int>	m_vecStartPos;
		std::vector<int>	m_vecSummonPos;
	};

	struct VillageStartPos
	{
		D3DXVECTOR3			m_StartPos;
		int					m_MapID;
		bool				m_bIsNPC;
		bool				m_bIsMarket;
		bool				m_bIsSummon;
		int					m_iDungeonId;
		int					m_iDungeonPos;
		std::vector<int>	m_vecLinkStartPos;
	};

public:
	CX2LocationManager(void);
	virtual ~CX2LocationManager(void);

	bool	OpenScriptFile( const char* pFileName );
	void	VillageParcing();
	
	VillageTemplet*		GetVillageMapTemplet( int iMapID );	
	CX2LocationManager::VillageStartPos* GetVillageStartPos( int startPos );

	int GetMapID( int startPos );
	void GetVecLinkedPos(std::vector<int>& vStart);
	void GetVecMarketPos(std::vector<int>& vStart);
	D3DXVECTOR3 GetLastLinkPos(int startPos);
	D3DXVECTOR3 GetStartPosLoc(int startPos);
	int	GetStartPosLink(int startPos);		
	int GetLoginPos(UidType mapId);
	int GetLinkedDungeon(int startPos);
	D3DXVECTOR3 GetNearLoginPos(D3DXVECTOR3 vPos);

	void SetVillage(MAP_ID val, D3DXVECTOR3 pos);
	void GetVillage(MAP_ID &VillageId, D3DXVECTOR3 &vPos);
	void RestoreVillage();

	CX2LocationManager::MAP_ID GetCurrentVillageID() const { return m_eCurrentVillage; }
	bool IsDungeonGate( int iMapID )
	{
		if( iMapID >= MI_RUBEN )
			return false;

		return true;
	}

	int GetWorldID(MAP_ID val);
	void SetLastPos(D3DXVECTOR3 vPos) { m_vLastPos = vPos; }
	D3DXVECTOR3 GetLastPos() { return m_vLastPos; }

	inline float GetDistance( D3DXVECTOR3& vPos1, D3DXVECTOR3& vPos2 )
	{
		return sqrtf( powf( vPos2.x - vPos1.x, 2.0f ) + powf( vPos2.y - vPos1.y, 2.0f ) + powf( vPos2.z - vPos1.z, 2.0f ) );
	}

public:
	//{{ 2008. 12. 8  √÷¿∞ªÁ
	bool GetRandomPositionInField( int iMapID, D3DXVECTOR3& kPos );
	//}}

protected:
	std::map< int, VillageTemplet* >	m_mapVillageTemplet;

	// <StartPos Id, StartPos List>
	std::map< int, VillageStartPos* >	m_mapVillageMap;

	MAP_ID			m_eCurrentVillage;		
	MAP_ID			m_eOldVillage;
	D3DXVECTOR3		m_vCurrentPos;
	D3DXVECTOR3		m_vOldPos;
	D3DXVECTOR3     m_vLastPos;
};

DefSingletonInline( CX2LocationManager );


