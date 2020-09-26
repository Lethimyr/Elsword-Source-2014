using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using Microsoft.DirectX;
using LuaInterface;

namespace ContentSelector
{
    public class DungeonInfoData
    {
        public enum DIFFICULTY_LEVEL
        {
            DL_NORMAL = 0,
            DL_HARD,
            DL_EXPERT,
        };
        
        public enum DUNGEON_TYPE
        {
            DT_INVALID,
            DT_NORMAL,
            DT_HENIR,
        };

        public enum HENIR_DUNGEON_MODE
        {
            HDM_INVALID,
            HDM_PRACTICE,
            HDM_CHALLENGE,
        };
        
        //클라이언트만 필요한 UI Data들 .. 
		public struct DungeonDataUI
		{
			public bool		m_bArcadeMode;

			public string		m_TextureName;
			public string		m_PieceName;

			public string		m_Explanation;
			public Vector2     m_LocalStarPos;					//지역 지도에 던전 별표시 위치 좌표
			public Vector2     m_PopUpOffsetPos;				//지역 지도에 던전 팝업창 오프셋 위치 좌표

			public List<string>		m_vecAutoRoomTitle;		// 던전방 생성시 자동으로 지정되는 방 제목

			public string		m_LoadingScreenFileName;
			public string		m_LoadingScreenFileName2;

		};
        	
            
		public struct EndingSpeech
		{
			public List< string > m_vecSpeech;
			
		};

//         public struct EndingSpeechSet
// 		{
// 			List< EndingSpeech > m_vecEndingSpeech;
// 		};
// 
//         public struct EndingSpeechSetMap
// 		{
// 			Dictionary< int, EndingSpeechSet > m_mapEndingSpeechSet;
//             				
// 		};
//
//
//      public List<EndingSpeechSetMap> m_vecEndingSpeechSetMap;

        public int m_DungeonID;
        public int m_DungeonName_ID;
        public string m_DataFileName;
		//int			m_RequireLevel;
        public int m_RequireDungeonID;
        public List<int> m_vecExtraRequireDungeonID;		// 비밀던전과 같이 특별히 require dungeon id가 여러개인 경우에 여기에 추가
        
        public bool m_bHellMode;		// hell mode 던전은 입장조건이 되기 전까지 던전 아이콘도 보이지 않아야 한다

        public int m_RequireItemID;
        public int m_RequireItemCount;

        public int m_RequireItemID2;
        public int m_RequireItemCount2;
		

		//{{ 2007. 10. 4  최육사  근성도
        public int m_RequireSpirit;
		//}}

        public int m_MinLevel;
        public int m_MaxLevel;

        public int m_eDefaultDungeonLoungeWorldID;

        public DIFFICULTY_LEVEL m_eDifficulty;

        public bool m_bNormalOnly;			// 난이도가 "보통"만 있는 던전이다
        public float m_fTimeLimit;			// 던전 플레이 시간 제한

        public DungeonDataUI m_UIData;
        
        public DUNGEON_TYPE m_eDungeonType;
        public bool m_bRelativeMonsterLevel;


        public void ParseDungeonInfoData(string filename)
        {
            bool bNormalScript = true;
            string normalFilename = null;
            if (filename.IndexOf("HARD") != -1)
            {
                normalFilename = filename.Replace("HARD", "NORMAL");
                bNormalScript = false;
            }
            else if (filename.IndexOf("EXPERT") != -1)
            {
                normalFilename = filename.Replace("EXPERT", "NORMAL");
                bNormalScript = false;
            }

            if (bNormalScript == false)
            {
                ParseDungeonInfoData(normalFilename);
            }

            StreamReader sr = null;
            try
            {
                sr = new StreamReader(filename, Encoding.UTF8);
                string strFile = sr.ReadToEnd();
                if (strFile.IndexOf("g_pDungeonManager:AddDungeonData") != -1)
                {
                    strFile = strFile.Replace("g_pDungeonManager:AddDungeonData", "TABLE = ");
                    //strFile = strFile.Substring(0, strFile.Length - 3);

                    // Valid file
                    
                    Lua newlua = new Lua();
                    LuaTool.DoFile(newlua, ScriptTreeManager.strMajorDir + "Enum.lua");
                    LuaTool.DoFile(newlua, ScriptTreeManager.strMajorDir + "StringID_def.lua");
                    
                    newlua.DoString(strFile);
                    
                    LuaTool.GetValue(newlua, "TABLE.dungeonID", out m_DungeonID, 0);

                    LuaTool.GetValue(newlua, "TABLE.dungeonName", out m_DungeonName_ID, -1);

                    LuaTool.GetValue(newlua, "TABLE.dataFileName", out m_DataFileName, "");

                    #region CopyedCode
#if false
              
    
	
	//LUA_GET_VALUE(				luaManager, L"requireLevel",		pDungeonData->m_RequireLevel, 0 );
	LUA_GET_VALUE_ENUM(			luaManager, L"requireDungeonID",	pDungeonData->m_RequireDungeonID, 
		CX2Dungeon::DUNGEON_ID,	CX2Dungeon::DI_NONE );


	if( true == luaManager.BeginTable( "EXTRA_REQUIRE_DUNGEON_ID" ) )
	{
		for( int iValueIndex=1; true; iValueIndex++ )
		{
			int iExtraDungeonID = -1;
			if( true == luaManager.GetValue( iValueIndex, iExtraDungeonID ) )
			{
				if( -1 != iExtraDungeonID )
				{
					pDungeonData->m_vecExtraRequireDungeonID.push_back( (CX2Dungeon::DUNGEON_ID) iExtraDungeonID );
				}
			}
			else
			{
				break;
			}
		}

		luaManager.EndTable(); // EXTRA_REQUIRE_DUNGEON_ID
	}


	LUA_GET_VALUE(				luaManager, L"IS_HELL_MODE",		pDungeonData->m_bHellMode,	false );


	//{{ 2007. 8. 29  최육사  
	LUA_GET_VALUE(				luaManager, L"requireItemID",		pDungeonData->m_RequireItemID, 0 );
	LUA_GET_VALUE(				luaManager, L"requireItemCount",	pDungeonData->m_RequireItemCount, 0 );
	//}}


	LUA_GET_VALUE(				luaManager, L"requireItemID2",		pDungeonData->m_RequireItemID2, 0 );
	LUA_GET_VALUE(				luaManager, L"requireItemCount2",	pDungeonData->m_RequireItemCount2, 0 );



	//{{ 2007. 10. 4  최육사  근성도
	LUA_GET_VALUE(				luaManager, L"requireSpirit",		pDungeonData->m_RequireSpirit,		0 );
	//}}
    
	LUA_GET_VALUE_ENUM(			luaManager, L"difficulty",			pDungeonData->m_eDifficulty,	CX2Dungeon::DIFFICULTY_LEVEL, CX2Dungeon::DL_NORMAL );

	LUA_GET_VALUE(				luaManager, L"normalOnly",			pDungeonData->m_bNormalOnly,	false );
	LUA_GET_VALUE(				luaManager, L"m_fTimeLimit",		pDungeonData->m_fTimeLimit,		-1.f );

	
	LUA_GET_VALUE(				luaManager, L"m_MinLevel",			pDungeonData->m_MinLevel,		0 );
	LUA_GET_VALUE(				luaManager, L"m_MaxLevel",			pDungeonData->m_MaxLevel,		0 );


	LUA_GET_VALUE_ENUM(			luaManager, L"m_eDefaultDungeonLoungeWorldID",		pDungeonData->m_eDefaultDungeonLoungeWorldID,		CX2World::WORLD_ID, CX2World::WI_NONE );



	LUA_GET_VALUE_ENUM(			luaManager, L"m_eDungeonType",				pDungeonData->m_eDungeonType,				CX2Dungeon::DUNGEON_TYPE,	CX2Dungeon::DT_NORMAL );
	LUA_GET_VALUE(				luaManager, L"m_bRelativeMonsterLevel",		pDungeonData->m_bRelativeMonsterLevel,		false );



	// UI Data About..
	LUA_GET_VALUE(				luaManager, L"m_bArcadeMode",		pDungeonData->m_UIData.m_bArcadeMode,	false );

	LUA_GET_VALUE(				luaManager, L"m_TextureName",		pDungeonData->m_UIData.m_TextureName,	L"" );
	LUA_GET_VALUE(				luaManager, L"m_PieceName",			pDungeonData->m_UIData.m_PieceName,		L"" );
	LUA_GET_VALUE(				luaManager, L"m_Explanation",		iStringIndex,	STR_ID_EMPTY );
    pDungeonData->m_UIData.m_Explanation = GET_STRING( iStringIndex );

	LUA_GET_VALUE(				luaManager, L"m_LocalStarPosX",		pDungeonData->m_UIData.m_LocalStarPos.x, -999.0f );
	LUA_GET_VALUE(				luaManager, L"m_LocalStarPosY",		pDungeonData->m_UIData.m_LocalStarPos.y, -999.0f );

	LUA_GET_VALUE(				luaManager, L"m_PopUpOffsetPosX",	pDungeonData->m_UIData.m_PopUpOffsetPos.x, 0.0f );
	LUA_GET_VALUE(				luaManager, L"m_PopUpOffsetPosY",	pDungeonData->m_UIData.m_PopUpOffsetPos.y, 0.0f );

	//{{ 2009.1.22 김태완 던전별 로딩화면

	LUA_GET_VALUE(				luaManager, L"m_LoadingScreenFileName",		pDungeonData->m_UIData.m_LoadingScreenFileName,	L"" );
	LUA_GET_VALUE(				luaManager, L"m_LoadingScreenFileName2",		pDungeonData->m_UIData.m_LoadingScreenFileName2,	L"" );

	//}}





	wstring wstrBuf = L"";
	if( true == luaManager.BeginTable( "AUTO_DUNGEON_ROOM_TITLE" ) )
	{
		for( int iValueIndex=1; true; iValueIndex++ )
		{
			LUA_GET_VALUE_RETURN( luaManager, iValueIndex, iStringIndex, STR_ID_EMPTY, break; );

			pDungeonData->m_UIData.m_vecAutoRoomTitle.push_back( GET_STRING( iStringIndex ) );
		}
		luaManager.EndTable(); // AUTO_DUNGEON_ROOM_TITLE
	}




	int iEndSpeechTableIndex = 0;
	while( true == luaManager.BeginTable( "ENDING_SPEECH", iEndSpeechTableIndex ) )
	{
		CX2Dungeon::DungeonData::EndingSpeechSetMap endingSpeechSetMap;

		if( true == luaManager.BeginTable( "ELSWORD" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{
				CX2Dungeon::DungeonData::EndingSpeech endingSpeech;

				int iValueIndex = 1;				
				while( true )
				{
					LUA_GET_VALUE_RETURN(		luaManager,	 iValueIndex,	iStringIndex, STR_ID_EMPTY, break; );
					endingSpeech.m_vecSpeech.push_back( GET_STRING( iStringIndex ) );
					iValueIndex++;
				}
				endingSpeechSet.m_vecEndingSpeech.push_back( endingSpeech );
				
				luaManager.EndTable(); // iTableIndex
				iTableIndex++;
			}

			endingSpeechSetMap.m_mapEndingSpeechSet[ CX2Unit::UT_ELSWORD ] = endingSpeechSet;
			luaManager.EndTable(); // ELSWORD
		}

		if( true == luaManager.BeginTable( "AISHA" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{
				CX2Dungeon::DungeonData::EndingSpeech endingSpeech;

				int iValueIndex = 1;				
				while( true )
				{
                    LUA_GET_VALUE_RETURN(		luaManager,	 iValueIndex,	iStringIndex, STR_ID_EMPTY, break; );
                    endingSpeech.m_vecSpeech.push_back( GET_STRING( iStringIndex ) );
					iValueIndex++;
				}
				endingSpeechSet.m_vecEndingSpeech.push_back( endingSpeech );

				luaManager.EndTable(); // iTableIndex
				iTableIndex++;
			}

			endingSpeechSetMap.m_mapEndingSpeechSet[ CX2Unit::UT_ARME ] = endingSpeechSet;
			luaManager.EndTable(); // AISHA
		}



		if( true == luaManager.BeginTable( "RENA" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{
				CX2Dungeon::DungeonData::EndingSpeech endingSpeech;

				int iValueIndex = 1;				
				while( true )
				{
					LUA_GET_VALUE_RETURN(		luaManager,	 iValueIndex,	iStringIndex, STR_ID_EMPTY, break; );
					endingSpeech.m_vecSpeech.push_back( GET_STRING( iStringIndex ) );
					iValueIndex++;
				}
				endingSpeechSet.m_vecEndingSpeech.push_back( endingSpeech );

				luaManager.EndTable(); // iTableIndex
				iTableIndex++;
			}

			endingSpeechSetMap.m_mapEndingSpeechSet[ CX2Unit::UT_LIRE ] = endingSpeechSet;
			luaManager.EndTable(); // RENA
		}



		if( true == luaManager.BeginTable( "RAVEN" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{
				CX2Dungeon::DungeonData::EndingSpeech endingSpeech;

				int iValueIndex = 1;				
				while( true )
				{
                    LUA_GET_VALUE_RETURN(		luaManager,	 iValueIndex,	iStringIndex, STR_ID_EMPTY, break; );
                    endingSpeech.m_vecSpeech.push_back( GET_STRING( iStringIndex ) );
					iValueIndex++;
				}
				endingSpeechSet.m_vecEndingSpeech.push_back( endingSpeech );

				luaManager.EndTable(); // iTableIndex
				iTableIndex++;
			}

			endingSpeechSetMap.m_mapEndingSpeechSet[ CX2Unit::UT_RAVEN ] = endingSpeechSet;
			luaManager.EndTable(); // RAVEN
		}



		if( true == luaManager.BeginTable( "EVE" ) )
		{
			CX2Dungeon::DungeonData::EndingSpeechSet endingSpeechSet;

			int iTableIndex = 1;
			while( true == luaManager.BeginTable( iTableIndex ) )
			{
				CX2Dungeon::DungeonData::EndingSpeech endingSpeech;

				int iValueIndex = 1;				
				while( true )
				{
                    LUA_GET_VALUE_RETURN(		luaManager,	 iValueIndex,	iStringIndex, STR_ID_EMPTY, break; );
                    endingSpeech.m_vecSpeech.push_back( GET_STRING( iStringIndex ) );
					iValueIndex++;
				}
				endingSpeechSet.m_vecEndingSpeech.push_back( endingSpeech );

				luaManager.EndTable(); // iTableIndex
				iTableIndex++;
			}

			endingSpeechSetMap.m_mapEndingSpeechSet[ CX2Unit::UT_EVE ] = endingSpeechSet;
			luaManager.EndTable(); // EVE
		}





		pDungeonData->m_vecEndingSpeechSetMap.push_back( endingSpeechSetMap );

		luaManager.EndTable(); // ENDING_SPEECH (iEndSpeechTableIndex)
		iEndSpeechTableIndex++;
	}
#endif
                    #endregion

                }
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                if (sr != null)
                    sr.Close();
            }


            return;

        }

    };
        
}
