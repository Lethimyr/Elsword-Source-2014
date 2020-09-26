#pragma once

class KX2DTDungeonMapUI
{
public:
	struct SubStageNode
	{
		vector< std::pair<int, int> >		m_vecNextSubStageIndex;
	};

public:
	KX2DTDungeonMapUI( CX2Dungeon* pDungeon );
	virtual ~KX2DTDungeonMapUI(void);

	void SetSelectedMiniMap( int iStage, int iSubStage ) { m_pairPickMiniMap.first = iStage; m_pairPickMiniMap.second = iSubStage; }
	void SetMapDungeonMapList( std::map< std::pair<int,int>, CX2Dungeon::DungeonMapData >* mapDungeonMapList )
	{
		m_mapDungeonMapList = mapDungeonMapList;
	}
	bool GetShow() const { return m_bShow; }
	void SetShow( bool bShow ) { m_bShow = bShow; }

	HRESULT OnResetDevice();
	HRESULT OnLostDevice();
	void OnFrameRender();
	void MakeLine( CX2Dungeon* pDungeon );

	CKTDXDeviceTexture* GetStageTexture() { return m_pStageTexture; }
	CKTDXDeviceTexture* GetBossStageTexture() { return m_pBossStageTexture; }

private:
	bool					m_bShow;

	D3DXVECTOR2				m_vPos;
	float					m_fCurrStageAlpha;

	CX2Dungeon*				m_pDungeon;

	CKTDXDeviceTexture*		m_pBGTexture;
	CKTDXDeviceTexture*		m_pStageTexture;
	CKTDXDeviceTexture*		m_pBossStageTexture;
	CKTDXDeviceTexture*		m_pCurrStageTexture; 

	CDXUTLineManager		m_LineManager;

	//map<DetailStageIndex, StagePosData>	m_mapStagePosData; 

	map< std::pair<int, int>, SubStageNode> m_mapStageGraph;

	std::map< std::pair<int,int>, CX2Dungeon::DungeonMapData >* m_mapDungeonMapList;

	std::pair< int, int >	m_pairPickMiniMap;
};