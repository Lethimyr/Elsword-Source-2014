#pragma once

class CXSLSquareUnit
{
public:
	enum PERSONAL_SHOP_STATE
	{
		PSS_NONE	= 0,
		PSS_WAIT,
		PSS_SHOP,
	};

#ifdef SERV_UPGRADE_TRADE_SYSTEM

#else //SERV_UPGRADE_TRADE_SYSTEM
	enum PERSONAL_SHOP_TYPE
	{
		PST_NONE = 0,

		PST_NORMAL,
		PST_PREMIUM,
		PST_END,
	};
#endif //SERV_UPGRADE_TRADE_SYSTEM

public:
	CXSLSquareUnit();
	~CXSLSquareUnit();
};