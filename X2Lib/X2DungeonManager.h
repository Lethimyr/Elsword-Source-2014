#pragma once

class CX2DungeonManager
{
	public:
		~CX2DungeonManager();
		CX2DungeonManager();

		CX2Dungeon* CreateDungeon( SEnum::DUNGEON_ID dungeonID );
		//{{ 2007. 9. 13  최육사  DungeonTool
		CX2Dungeon* CreateDungeon( SEnum::DUNGEON_ID dungeonID, bool bIsNpcLoad );
		//}}
		bool OpenScriptFile( const WCHAR* pFileName );
		bool AddDungeonData_LUA();
		bool AddDefaultRoomTitle_LUA( int iStringIndex );

		int GetDungeonCount();
		CX2Dungeon::DungeonData* GetDungeonDataAt( int iIdx );
		const CX2Dungeon::DungeonData* GetDungeonData( SEnum::DUNGEON_ID dungeonID );

#ifdef SERV_NEW_EVENT_TYPES
		void SetDungeonStatus( IN std::map< int, bool >& mapDungeonStatus ) { m_mapDungeonStatus = mapDungeonStatus; }
		bool IsDungeonEnable( int iDungeonID )
		{
			const CX2Dungeon::DungeonData* pkDungeonData = GetDungeonData( (SEnum::DUNGEON_ID)iDungeonID );
			if ( pkDungeonData == NULL )
				return false;

			std::map< int, bool >::iterator mitDungeonStatus = m_mapDungeonStatus.find( iDungeonID );
			if( mitDungeonStatus != m_mapDungeonStatus.end() && mitDungeonStatus->second == false )
				return false;

			return true;
		}
#endif SERV_NEW_EVENT_TYPES

		bool IsActiveDungeon( int iDungeonID, int iDiffLevel );
		//{{ 허상형 : [2011/3/18/] //	월드 미션
#ifdef SERV_INSERT_GLOBAL_SERVER
		bool IsDefenceDungeon( IN const int iDungeonID ) const;
#endif SERV_INSERT_GLOBAL_SERVER
		//}} 허상형 : [2011/3/18/] //	월드 미션

#ifdef SERV_DUNGEON_OPTION_IN_LUA
		bool IsEventDungeon( IN const int iDungeonID );
#else SERV_DUNGEON_OPTION_IN_LUA
		bool IsEventDungeon( IN const int iDungeonID ) const;
#endif SERV_DUNGEON_OPTION_IN_LUA
		
		//{{ kimhc // 2010.3.19 //	비밀던전 개편 작업
#ifdef	SERV_SECRET_HELL
		int GetDungeonType( int iDungeonID );
		// 한개라도 갈수있는 비밀던전이 있는가?
		bool CanGoOneSecretDungeonAtLeast();
#endif	SERV_SECRET_HELL
	//}} kimhc // 2010.3.19 //	비밀던전 개편 작업

#ifdef SERV_LIMITED_DUNGEON_PLAY_TIMES
		bool	GetLimitedPlayTimes( IN const int nDungeonID, OUT int& iPlayTimes );
		bool	GetLimitedClearTimes( IN const int nDungeonID, OUT int& iClearTimes );
#endif SERV_LIMITED_DUNGEON_PLAY_TIMES

		vector<int> GetNextDungeon( int iDungeonID );

		//{{ 2007. 9. 5  최육사  DungeonTool
		void GetDungeonList( map< std::wstring, SEnum::DUNGEON_ID >& mapDungeonList );
		//}}

		D3DXCOLOR GetDifficultyColor( int dungeonID, int difficulty, int checkLevel );
		D3DXCOLOR GetLimitLevelColor( int dungeonID, int difficulty, int checkLevel );

		wstring MakeDungeonNameString( SEnum::DUNGEON_ID dungeonID, CX2Dungeon::DIFFICULTY_LEVEL eDifficulty, CX2Dungeon::DUNGEON_MODE eDungeonMode );
		wstring MakeDungeonNameString( SEnum::DUNGEON_ID dungeonID );



		const wstring& GetDefaultRoomTitle()
		{
			if( m_vecDefaultRoomTitle.empty() )
			{
				m_vecDefaultRoomTitle.push_back( L"Action Together Elsword" );
				return m_vecDefaultRoomTitle[0];
			}

			int iRand = rand() % (int) m_vecDefaultRoomTitle.size(); 
			return m_vecDefaultRoomTitle[ iRand ];
		}
		
private:
	void SetEndingSpeech( IN KLuaManager& luaManager_, IN int iStrIndexDungeon_, OUT CX2Dungeon::DungeonData::EndingSpeechSet& pEndingSpeechSet_ );
		
	private:
		map<SEnum::DUNGEON_ID, CX2Dungeon::DungeonData*> m_mapDungeonData;
		typedef map<SEnum::DUNGEON_ID, CX2Dungeon::DungeonData*>::iterator DungeonDataIterator;

		vector<wstring> m_vecDefaultRoomTitle;			// 던전방 생성시 자동으로 만들어지는 방제목

		//{{ kimhc // 2010.7.08 //	비밀던전 개편 작업
#ifdef	SERV_SECRET_HELL
		// 비밀던전의 ID를 모아놓음
		vector<SEnum::DUNGEON_ID> m_vecSecretDungeonID;
#endif	SERV_SECRET_HELL
		//}} kimhc // 2010.708 //	비밀던전 개편 작업

#ifdef SERV_NEW_EVENT_TYPES
		std::map< int, bool >	m_mapDungeonStatus;
#endif SERV_NEW_EVENT_TYPES
};