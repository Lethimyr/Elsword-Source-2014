using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace ContentSelector
{
    class DungeonDataDivider
    {
        enum State
        {
            NOT_DUNGEONDATA,
            DUNGEONDATA_READY,
            DUNGEONDATA,
        }

        public bool DivideDungeonData(string filename)
        {
            if(false == Directory.Exists(".\\TempDungeonLua"))
            {
                Directory.CreateDirectory(".\\TempDungeonLua");
            }

            m_eState = State.NOT_DUNGEONDATA;

            StreamReader sr;
            sr = new StreamReader(filename, Encoding.UTF8, false);
                       
            bool bOnComment = false;
            StreamWriter swDungeonData = null;

            int iBlock = 1;

            do
            {
                string wstrStringRead;
                wstrStringRead = sr.ReadLine();

                if (bOnComment == true)
                {
                    if (wstrStringRead.Contains("--]]"))
                    {
                        bOnComment = false;
                    }
                }
                else
                {
                    if (wstrStringRead.Contains("--[["))
                    {
                        bOnComment = true;
                    }
                }


                switch (m_eState)
                {
                    case State.NOT_DUNGEONDATA:
                        {
                            if (wstrStringRead.Contains("g_pDungeonManager:AddDungeonData"))
                            {
                                m_eState = State.DUNGEONDATA_READY;
                                continue;

                            }
                        } break;
                    case State.DUNGEONDATA_READY:
                        {
                            if (wstrStringRead.Contains("DUNGEON_ID"))
                            {
                                int indexstart = wstrStringRead.IndexOf('"');
                                int indexend = wstrStringRead.IndexOf('"', indexstart + 1);
                                string DID = wstrStringRead.Substring(indexstart + 1, indexend - indexstart - 1);
                                string OutFileName = "DungeonData_" + DID + ".lua";
                                string OutFileNameFullPath = ".\\TempDungeonLua\\" + OutFileName;

                                swDungeonData = new StreamWriter(OutFileNameFullPath, false, Encoding.UTF8);
                                swDungeonData.WriteLine("g_pDungeonManager:AddDungeonData");
                                swDungeonData.WriteLine("{");
                                swDungeonData.WriteLine(wstrStringRead);
                                
                                m_eState = State.DUNGEONDATA;

                                iBlock = 1;

                                continue;
                            }

                        } break;
                    case State.DUNGEONDATA:
                        {
                            swDungeonData.WriteLine(wstrStringRead);

                            if (wstrStringRead.Contains("{"))
                            {
                                iBlock++;
                            }

                            if (wstrStringRead.Contains("}"))
                            {
                                iBlock--;
                            }

                            if (iBlock == 0)
                            {
                                swDungeonData.Close();
                                m_eState = State.NOT_DUNGEONDATA;
                            }
                            
                        } break;
                }

            } while (sr.EndOfStream == false);

            return true;
        }
        public void CleanUpDungeonData()
        {
            DirectoryInfo directoryinfo = new DirectoryInfo(".\\TempDungeonLua");
            directoryinfo.Delete(true);
        }
        State m_eState;
    }
}
