#pragma once


//{{ 2010. 02. 16  최육사	해킹툴 리스트
#ifdef SERV_HACKING_TOOL_LIST


class KUserHackingManager
{
	enum HACKING_ENUM
	{
		HE_MIN_TIME = 10,
		HE_MAX_TIME = 20,
	};

public:
	KUserHackingManager(void);
	~KUserHackingManager(void);

	void SetRandomCheckTime();
	double GetHackingToolCheckTime() { return m_fHackingToolCheckTime; }

	void GetHackingToolList( OUT int& iRandomKey, OUT std::vector< KHackingToolInfo >& vecHackingToolList );

private:
	double				m_fHackingToolCheckTime;	
};


#endif SERV_HACKING_TOOL_LIST
//}}