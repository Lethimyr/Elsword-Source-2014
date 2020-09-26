#pragma once

class CKTDXEnumToLua
{
	public:
		CKTDXEnumToLua(void);
		~CKTDXEnumToLua(void);

		bool LoadEnumToLua( KLuaManager* pLuaManager, const WCHAR* pEnumFileName );

	private:
};
