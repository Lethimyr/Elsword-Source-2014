#pragma once

class CX2ItemManager
{
	public:
		CX2ItemManager(void);
		~CX2ItemManager(void);

		bool	OpenScriptFile( const WCHAR* pFileName );
		
		CX2Item::ItemTemplet* GetItemTemplet( const WCHAR* pItemName );
		CX2Item::ItemTemplet* GetItemTemplet( const int itemID );

		map<int,CX2Item::ItemTemplet*>& GetItemTempletIDMap() { return m_ItemTempletIDMap; }

		bool	AddItemTemplet_LUA();

	private:
		map<wstring,CX2Item::ItemTemplet*>	m_ItemTempletNameMap;
		map<int,CX2Item::ItemTemplet*>		m_ItemTempletIDMap;
};
