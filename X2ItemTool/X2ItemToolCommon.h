
#pragma once


class CX2ItemToolCommon
{
public:
	CX2ItemToolCommon(void);
	virtual ~CX2ItemToolCommon(void);

	void RegisterLuabind();

	KGCMassFileManager*		GetMassFileManager()	{ return &m_MassFileManager; }
	KLuabinder*				GetLuaBinder()			{ return &m_LuaBinder; }
	CX2ItemManager*			GetItemManager()		{ return &m_ItemManager; }


protected:
	KGCMassFileManager		m_MassFileManager;
	KLuabinder				m_LuaBinder;
	CX2ItemManager			m_ItemManager;
};


