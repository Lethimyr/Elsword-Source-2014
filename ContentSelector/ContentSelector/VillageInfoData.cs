using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using Microsoft.DirectX;
using LuaInterface;

namespace ContentSelector
{
    public class VillageInfoData
    {
        //string m_Name;
        public int m_Name_ID;
        public int m_VillageID;
        public List<int> m_HouseList;
        public string m_FrameMoveFuncName;

        public float m_fCameraDistance;
        public float m_fLookAtHeight;
        public float m_fEyeHeight;

        public bool m_bIsDungeonResultField; // 던전결과방 필드인지
        public Dictionary<int, DungeonLoungeUserStartPos> m_mapDungeonLoungeUserStartPos;

        public int m_BaseLocalStateID;
        public int m_WorldID;

        public List<VillageStartPos> m_vecStartPos;

        public struct DungeonLoungeUserStartPos
        {
            public List<Vector3> m_vecStartPos;
        };

        public struct VillageStartPos
        {
            public Vector3 m_StartPos;
            public int m_StartPosID;
            public int m_VillageID;
            public bool m_bIsNPC;
            public bool m_bIsMarket;
            public bool m_bIsSummon;
            public bool m_bIsWarp;
            public int m_iDungeonId;
            public int m_iDungeonPos;
            public List<int> m_vecLinkStartPos;
        };

        // dicStartPos : <StartPosID, LocalMapID>
        public void ParseVillage( Lua lua, ref Dictionary<int, int> dicStartPos )
        {
            m_fCameraDistance = 1200.0f;
            m_fLookAtHeight = 200.0f;
            m_fEyeHeight = 300.0f;

            LuaTool.DoFile(lua, ScriptTreeManager.strDialogDir + "DLG_Map_Village.lua");
            
            LuaTable VillageTable;
            
            try
            {
                VillageTable = lua.GetTable("Village");
                
            }
            catch(Exception e)
            {
                System.Windows.Forms.MessageBox.Show(e.Message);
                return;
            }

            foreach(DictionaryEntry Entry in VillageTable)
            {
                int villageID = (int)(double)Entry.Key;
                LuaTable Villageinfo = (LuaTable)Entry.Value;

                LuaTool.GetValue(Villageinfo, "NAME", out m_Name_ID, 0);
                LuaTool.GetValue(Villageinfo, "BaseLocalStateID", out m_BaseLocalStateID, 0);

                LuaTable HouseIDList;
                if (true == LuaTool.GetTable(Villageinfo, "HouseIDList", out HouseIDList))
                {
                    m_HouseList = new List<int>();
                    foreach (DictionaryEntry entry in HouseIDList)
                    {
                        m_HouseList.Add((int)(double)entry.Value);
                    }
                }

                LuaTool.GetValue(Villageinfo, "WORLD_ID", out m_WorldID, 0);
                // m_mapWorldMap.insert(std::make_pair((int)villageID, worldId));			

                LuaTable Camera;
                if (true == LuaTool.GetTable(Villageinfo, "CAMERA", out Camera))
                {
                    m_fCameraDistance = (float)(double)Camera[1];
                    m_fLookAtHeight = (float)(double)Camera[2];
                    m_fEyeHeight = (float)(double)Camera[3];
                }

                LuaTable StartPos;
                if (true == LuaTool.GetTable(Villageinfo, "START_POS", out StartPos))
                {
                    m_vecStartPos = new List<VillageStartPos>();
                    foreach (DictionaryEntry ent in StartPos)
                    {
                        LuaTable startposentry = (LuaTable)ent.Value;
                        VillageStartPos VSP = new VillageStartPos();
                        VSP.m_VillageID = m_VillageID;
                        LuaTool.GetValue(startposentry, "StartPosId", out VSP.m_StartPosID, 0);
                        float x, y, z;
                        LuaTool.GetValue(startposentry, "StartPosX", out x, 0.0f);
                        VSP.m_StartPos.X = x;
                        LuaTool.GetValue(startposentry, "StartPosY", out y, 0.0f);
                        VSP.m_StartPos.Y = y;
                        LuaTool.GetValue(startposentry, "StartPosZ", out z, 0.0f);
                        VSP.m_StartPos.Z = z;

                        LuaTool.GetValue(startposentry, "IsMarket", out VSP.m_bIsMarket, false);
                        LuaTool.GetValue(startposentry, "IsNPC", out VSP.m_bIsNPC, false);
                        LuaTool.GetValue(startposentry, "IsSummon", out VSP.m_bIsSummon, false);
                        LuaTool.GetValue(startposentry, "IsWarp", out VSP.m_bIsWarp, false);

                        LuaTool.GetValue(startposentry, "LinkDungeon", out VSP.m_iDungeonId, 0);
                        LuaTool.GetValue(startposentry, "LinkDungeonPos", out VSP.m_iDungeonPos, 0);

                        LuaTable LinkPos;
                        if (true == LuaTool.GetTable(startposentry, "LinkPos", out LinkPos))
                        {
                            VSP.m_vecLinkStartPos = new List<int>();
                            foreach (DictionaryEntry LinkPosEntry in LinkPos)
                            {
                                if ((int)(double)LinkPosEntry.Value >= 0)
                                    VSP.m_vecLinkStartPos.Add((int)(double)LinkPosEntry.Value);

                            }
                        }
                        dicStartPos.Add(VSP.m_StartPosID, m_BaseLocalStateID);
                        m_vecStartPos.Add(VSP);
                    }

                } // StartPos
                
            }
                        
        }
        #region CopyedCode
#if false
        void CX2LocationManager::VillageParcing( KLuaManager& kLuamanager, const int villageID )
{
	//빌리지
	
	vector< HOUSE_ID > vecHouseID;

	if ( kLuamanager.BeginTable( "Village" ) == true )
	{
		if( kLuamanager.BeginTable( villageID ) == true )
		{			
					
		    
        	// 던전 라운지에서 유저 유닛의 시작 위치를 parsing 
			std::map<CX2World::WORLD_ID, DungeonLoungeUserStartPos > mapDungeonLoungeUserStartPos;
			if( kLuamanager.BeginTable( "DUNGEON_LOUNGE_START_POS" ) == true )
			{
				int index = 1;
				while( kLuamanager.BeginTable( index ) == true )
				{
					CX2World::WORLD_ID eWorldID;
					LUA_GET_VALUE_ENUM( kLuamanager, L"START_POS_WORLD_ID", eWorldID, CX2World::WORLD_ID, CX2World::WI_NONE );
					
					if( true == kLuamanager.BeginTable( "USER_START_POS" ) )
					{
						DungeonLoungeUserStartPos dungeonLoungeUserStartPos;

						int iStartPosIndex = 1;
						while( kLuamanager.BeginTable( iStartPosIndex ) == true )
						{
							D3DXVECTOR3 vUserPos;
							
							LUA_GET_VALUE( kLuamanager, L"StartPosX", vUserPos.x , 0 );
							LUA_GET_VALUE( kLuamanager, L"StartPosY", vUserPos.y , 0 );
							LUA_GET_VALUE( kLuamanager, L"StartPosZ", vUserPos.z , 0 );

							dungeonLoungeUserStartPos.m_vecStartPos.push_back( vUserPos );

							kLuamanager.EndTable(); // iStartPosIndex 
							iStartPosIndex++;
						}

						if( false == dungeonLoungeUserStartPos.m_vecStartPos.empty() )
						{
							mapDungeonLoungeUserStartPos[ eWorldID ] = dungeonLoungeUserStartPos;
						}

						kLuamanager.EndTable(); // USER_START_POS
					}

					kLuamanager.EndTable(); // index
					index++;
				}

				kLuamanager.EndTable(); // DUNGEON_LOUNGE_START_POS
			}



			wstring frameMoveFuncName;
			LUA_GET_VALUE( kLuamanager, L"FrameMoveFunc", frameMoveFuncName, L"" );

			CX2LocationManager::VillageTemplet* pVillageTemplet = new CX2LocationManager::VillageTemplet();
			pVillageTemplet->m_VillageID = (CX2LocationManager::VILLAGE_MAP_ID)villageID;
			pVillageTemplet->m_HouseList = vecHouseID;
			pVillageTemplet->m_FrameMoveFuncName = frameMoveFuncName;

			pVillageTemplet->m_fCameraDistance	= fCameraDist;
			pVillageTemplet->m_fLookAtHeight	= fLookatHeight;
			pVillageTemplet->m_fEyeHeight		= fEyeHeight;

			pVillageTemplet->m_mapDungeonLoungeUserStartPos = mapDungeonLoungeUserStartPos;

            int iIndex;
			LUA_GET_VALUE( kLuamanager, L"NAME", iIndex,	STR_ID_EMPTY );
            pVillageTemplet->m_Name = GET_STRING( iIndex );

			LUA_GET_VALUE( kLuamanager, "IS_DUNGEON_RESULT_FIELD", pVillageTemplet->m_bIsDungeonResultField, false );






			map< VILLAGE_MAP_ID, VillageTemplet* >::iterator it_temp =
				m_mapVillageTemplet.find( (CX2LocationManager::VILLAGE_MAP_ID)villageID );
			ASSERT( it_temp == m_mapVillageTemplet.end() );

			m_mapVillageTemplet.insert( std::make_pair( (CX2LocationManager::VILLAGE_MAP_ID)villageID, pVillageTemplet ) );

			kLuamanager.EndTable();
		}
		else
		{
			MessageBox( g_pKTDXApp->GetHWND(), L"CX2LocationManager Village OpenScript Error", L"Error", MB_OK );
			return;
		}

		kLuamanager.EndTable();
	}

	HouseParcing( kLuamanager, vecHouseID );

}
#endif
#endregion CopyedCode

    }
}
