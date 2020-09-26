#pragma once

class CX2TrainingCenterTable
{
public:
	struct TC_TABLE_INFO
	{
		int		m_iID;
		int		m_iDungeonID;

		int		m_iBeforeID;
		int		m_iRewardED;
		int		m_iRewardEXP;
		int		m_iMinLevel;
		float	m_fPlayTime;

		int		m_iDifficulty;

		int			m_iTrainingNumber; // 이동훈련 1 에서, '1'
		wstring		m_wstrTitle;
		wstring		m_wstrDesc;

		wstring		m_wstrTextureName;
		wstring		m_wstrTexturePieceName;

		wstring		m_wstrDescTextureName;
		wstring		m_wstrDescTexturePieceName;

		std::vector<char>	m_vecUnitClass;

		TC_TABLE_INFO()
		{
			m_iID = 0;
			m_iDungeonID = 0;
			m_iBeforeID = 0;
			m_iRewardED = 0;
			m_iRewardEXP = 0;
			m_iMinLevel = 0;
			m_fPlayTime = 0.f;
			m_iDifficulty = 0;
			m_iTrainingNumber = 0;
		};
		TC_TABLE_INFO( const TC_TABLE_INFO& t ) { *this = t; }    // copy constructor
		TC_TABLE_INFO& operator=( const TC_TABLE_INFO& t )       // assign operator
		{
			m_iID = t.m_iID;
			m_iDungeonID = t.m_iDungeonID;
			m_iBeforeID = t.m_iBeforeID;
			m_iRewardED = t.m_iRewardED;
			m_iRewardEXP = t.m_iRewardEXP;
			m_iMinLevel = t.m_iMinLevel;
			m_fPlayTime = t.m_fPlayTime;
			m_iDifficulty = t.m_iDifficulty;
			m_iTrainingNumber = t.m_iTrainingNumber;
			m_wstrTitle = t.m_wstrTitle;
			m_wstrDesc = t.m_wstrDesc;
			m_wstrTextureName = t.m_wstrTextureName;
			m_wstrTexturePieceName = t.m_wstrTexturePieceName;
			m_wstrDescTextureName = t.m_wstrDescTextureName;
			m_wstrDescTexturePieceName = t.m_wstrDescTexturePieceName;
			m_vecUnitClass = t.m_vecUnitClass;

			return *this;
		}
	};

	
public:
	CX2TrainingCenterTable(void);
	virtual ~CX2TrainingCenterTable(void);

	bool	OpenScriptFile( const WCHAR* pFileName );
	bool	AddTCTemplet_LUA();
	bool	CheckIfEnter( std::map< int, KTCClearInfo >& mapTCClear, int iTCID );
	int		GetDungeonID( int iTrainingID );	//return -1이면 에러

	bool GetTrainingInfo( TC_TABLE_INFO& trainingInfo, int iTrainingID );
	vector<int>& GetTrainingListByUnitClass( CX2Unit::UNIT_CLASS eUnitClass );
	bool SetTrainingListByUnitClass_LUA();
	int GetNextTrainingID( CX2Unit::UNIT_CLASS eUnitClass, int iNowTrainingID );
		
private:
	//std::map<int, KTrainingCenterTableInfo>		m_mapTCInfo;
	std::map<int, TC_TABLE_INFO>		m_mapTCInfo;

	std::map< CX2Unit::UNIT_CLASS, vector<int> >	m_mapTrainingListByUnitClass;
};
