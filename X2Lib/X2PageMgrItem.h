#pragma once

class CX2PageMgrItem : public CX2PageManager
{
	public:

		class CX2PageSlotItem : public CX2PageManager::CX2PageSlot
		{
			public:

				CX2PageSlotItem() 
				{
					m_ItemID = 0;
				}
				CX2PageSlotItem( int index, const WCHAR* pFileName );
				virtual ~CX2PageSlotItem();

				virtual void SetItemID( int itemID ) { m_ItemID = itemID; }
				virtual int GetItemID() { return m_ItemID; }

				virtual CX2PageSlot* Clone( int index, const WCHAR* pFileName ) { return new CX2PageSlotItem( index, pFileName ); }

			protected:

				int m_ItemID;
		};

	public:

		CX2PageMgrItem( const WCHAR* pFileName = NULL );
		virtual ~CX2PageMgrItem(void);

		void Reset( vector<int>& vecItemID );

	protected:

		void ResetNowPage();

		vector< int > m_vecItemID;
};
