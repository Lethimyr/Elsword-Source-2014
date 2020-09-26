#pragma once

class CX2InformerManager
{
	public:

		CX2InformerManager(void);
		virtual ~CX2InformerManager(void);

		CX2InformerSkill* GetSkillInformer();
		CX2InformerQuest* GetQuestInformer();
		CX2InformerInven* GetInvenInformer();
		CX2InformerCashShop* GetCashShopInformer();

		//{{ kimhc // 2009-11-02 // 길드 추가
#ifdef	GUILD_MANAGEMENT
		CX2InformerGuildExp* GetGuildInformer();
#endif	GUILD_MANAGEMENT
		//}} kimhc // 2009-11-02 // 길드 추가

		void Reset();
		void CheckInfo( bool bStateChange );

		void InvalidInfo();
		void ClearAllInfo();

		bool MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		HRESULT OnFrameMove( double fTime, float fElapsedTime );
				
	protected:

		bool IsCreatedInformerType( CX2Informer::X2_INFORMER_TYPE informerType );
		CX2Informer* GetInformerByType( CX2Informer::X2_INFORMER_TYPE informerType );

		vector< CX2Informer* > m_vecInformer;
};
