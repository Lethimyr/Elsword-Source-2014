#pragma once

class CXSLDamageManager
{
public:
	enum DAMAGE_TYPE
	{
		DT_PHYSIC = 0,			//물리 타격
		DT_MAGIC,				//마법 타격
	};

public:
	CXSLDamageManager(void);
	~CXSLDamageManager(void);
};
