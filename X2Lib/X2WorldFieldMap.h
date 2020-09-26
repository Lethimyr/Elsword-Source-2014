// dmlee, 2009-01-30, 월드맵에서 나오는 필드맵

#pragma once

#include "X2FieldMapBase.h"



class CX2WorldFieldMap : public CX2FieldMapBase
{
private:
	CX2WorldFieldMap(void);
	virtual ~CX2WorldFieldMap(void);


	struct FieldNPCData
	{
		CX2UnitManager::NPC_UNIT_ID m_eNPCID;

		D3DXVECTOR3	m_vPos;

		int m_nNewQuestRepeat;
		int m_nNewQuestNormal;
		int m_nEventQuest;
		int m_nDoQuest;
		int m_nCompleteQuestRepeat;
		int m_nCompleteQuestNormal;
		int m_nFairLvQuest;
		

		FieldNPCData()
		{
			m_eNPCID = CX2UnitManager::NUI_NONE;

			m_nNewQuestRepeat = 0;
			m_nNewQuestNormal = 0;
			m_nEventQuest = 0;
			m_nDoQuest = 0;
			m_nCompleteQuestRepeat = 0;
			m_nCompleteQuestNormal = 0;
			m_nFairLvQuest = 0;
		}
	};

	struct FieldData
	{
		int m_iMapID;
		std::map< CX2UnitManager::NPC_UNIT_ID, FieldNPCData > m_mapFieldNPCData;
	};

public:
	static CX2WorldFieldMap* CreateWorldFieldMap() { return new CX2WorldFieldMap; }


	virtual void NotifyShowObjectChanged();
	virtual void OnFrameRender_Draw();


	void SetVillageID( int iMapID );

	CX2WorldFieldMap::FieldData* GetFieldInfo( int iMapID );
	void MakeQuestNoticeEffect( int NPCID );
	void DestoryQuestNoticeEffect();

	void SetFieldDLG( IN CKTDGUIDialogType pFieldDLG_ )
	{
		m_pFieldDLG = pFieldDLG_;
	}
	D3DXVECTOR2 CalcPosForScroll( const D3DXVECTOR2& vPos_ );

	void InitCharacterMaker( void );

private:
	std::map< int, FieldData > m_mapFieldData;
	FieldData* m_pCurrFieldData;
	CKTDGParticleSystem::CParticleEventSequenceHandle 	m_hSeqNewQuestRing;

	CKTDGFontManager::CUKFont* m_pFont;
	CKTDGUIDialogType m_pFieldDLG;
	
	int m_iMapID;		/// 배틀 필드 경우 처리

#ifdef FIX_FIELD_MAP_SCROLLING
	D3DXVECTOR2 m_vProjectedPos;
#endif FIX_FIELD_MAP_SCROLLING
};
