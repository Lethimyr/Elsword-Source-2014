#pragma once

class CEventScriptParsing
{
public:
	CEventScriptParsing(void);
	~CEventScriptParsing(void);

	void	OpenScriptFile( IN const WCHAR * pFileName );

public:
	int		GetEventSize(){ return static_cast<int>(m_mapEventSceneData.size()); }
	void	GetEventIDList( OUT _EST_OBJECT_::EventIDList & eventIDList );
	bool	GetUTF8byEventScriptData( IN std::wstring wstrEventID, OUT std::wstring &wstrScript );

	bool	ScriptRefresh( IN const WCHAR *pScrpt, IN const long iLength );

	void	Clear();

	bool	SaveScript();


private:
	bool	Parsing( IN const char * pScript, IN const long iLength );

private:
	_EST_OBJECT_::EventSceneData m_mapEventSceneData;
};
