#pragma once

class CX2PageManager
{
	public:

		class CX2PageSlot
		{
			public:

				CX2PageSlot();
				CX2PageSlot( int index, const WCHAR* pFileName );
				virtual ~CX2PageSlot();

				virtual void SetShow( bool bShow );
				virtual void SetPos( D3DXVECTOR2 pos );
				virtual void UpdateUI() {}

				virtual CKTDGUIDialogType GetDialog() { return m_pDialog; }

				virtual CX2PageSlot* Clone( int index, const WCHAR* pFileName ) { return new CX2PageSlot( index, pFileName ); }

			protected:

				int				m_Index;
				CKTDGUIDialogType  m_pDialog;
		};

	public:

		CX2PageManager( const WCHAR* pFileName = NULL );
		virtual ~CX2PageManager(void);

		virtual void Init( CX2PageSlot* pCX2PageSlot, const WCHAR* pSlotDLGName, int maxSlotNumPerPage );
		
		virtual void SetOpen( bool bOpen );

		virtual void SetReverseDLGLayer( bool bCheck ) { m_bReverseDLGLayer = bCheck; }

		virtual int	GetNowPage() { return m_NowPage; }
		virtual int GetMaxPage() { return m_MaxPage; }

		virtual bool NextPage();
		virtual bool PrevPage();

#ifdef COUPON_SYSTEM
		virtual bool LastPage();
		virtual bool FirstPage();
#endif // COUPON_SYSTEM

	protected:

		virtual void InitbyOpen() {}
		virtual CX2PageSlot* CreateSlot( CX2PageSlot* pPageSlot, int slotIndex, const WCHAR* pSlotDLGName );

		virtual void ResetNowPage();
		virtual CX2PageSlot* GetPageSlot( CKTDGUIDialogType pDialog );

		bool m_bOpen;

		bool m_bReverseDLGLayer;

		int m_NowPage;
		int m_MaxPage;

		int	m_MaxSlotNumPerPage;

		CKTDGUIDialogType m_pDialog;

		vector< CX2PageSlot* > m_vecPageSlot;
		
};
