#pragma once



class CX2PVPResultTable
{

public:
	enum RESULT_TYPE
	{
		RT_TEAM_MATCH_WIN		= 0,
		RT_TEAM_MATCH_LOSE,
		RT_DEATH_MATCH_KILL,
		RT_DEATH_MATCH_DEATH,
		RT_TIME_BONUS,
		RT_OUT_PENALTY,	
		RT_END,
	};

	struct ResultData
	{
		int						m_VP;
		int						m_EXP;

		ResultData()
		{
			m_VP			= 0;
			m_EXP			= 0;
		}
	};

public:		
	CX2PVPResultTable(void);
	~CX2PVPResultTable(void);

	bool OpenScriptFile( const WCHAR* wstrFileName );

	void SetTeamMatchWin_LUA( int iPVPChannelClass, int VP, int EXP );
	void SetTeamMatchLose_LUA( int iPVPChannelClass, int VP, int EXP );
	void SetDeathMatchKill_LUA( int iUnitEmblem, int VP, int EXP );
	void SetDeathMatchDeath_LUA( int iUnitEmblem, int VP, int EXP );
	void SetGameOutPenalty_LUA( int iUnitEmblem, int VP, int EXP );
	void SetTimeBonus_LUA( int iUnitEmblem, int VP, int EXP );

	const ResultData* GetTeamMatchWin( int iPVPChannelClass );
	const ResultData* GetTeamMatchLose( int iPVPChannelClass );
	const ResultData* GetDeathMatchKill( CX2PVPEmblem::PVP_EMBLEM eEmblem );
	const ResultData* GetDeathMatchDeath( CX2PVPEmblem::PVP_EMBLEM eEmblem );
	const ResultData* GetGameOutPenalty( CX2PVPEmblem::PVP_EMBLEM eEmblem );
	const ResultData* GetTimeBonus( CX2PVPEmblem::PVP_EMBLEM eEmblem );

protected:
	void				RegisterLuaBind();
	void				SetTable( RESULT_TYPE eResultType, int iKey, int iVP, int iEXP );
	const ResultData*	GetTable( RESULT_TYPE eResultType, int iKey );

	std::map<int, ResultData>		m_mapResultData[RT_END];
};
