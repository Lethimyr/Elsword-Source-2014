using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;
using LuaInterface;

namespace ContentSelector
{    
    public class TreeData
    {
        public enum NodeDataType
        {
            DT_WORLD,
            DT_LOCAL,
            DT_DUNGEON,
            DT_VILLAGE,
        }

        public int m_iID;
        public string m_strEnumName;
        public bool m_bOn;
        public NodeDataType m_eType;
        
        public virtual void AddtoTreeView(TreeNodeCollection treeNodeCollection, bool bParentChecked )
        {
            TreeNode treeNode = new TreeNode(m_strEnumName);
            treeNode.Tag = this;
            treeNode.ToolTipText = "ID : " + m_iID.ToString();
            treeNode.Checked = m_bOn && bParentChecked;
            treeNodeCollection.Add(treeNode);
        }

        public virtual void CreateCheckList(ref Dictionary<int, bool> dicCheckList)
        {
            dicCheckList.Add(m_iID, m_bOn);
        }
                
        public virtual void LoadMetaData(Dictionary<int, bool> CheckList)
        {
            try
            {
                m_bOn = CheckList[m_iID];
            }
            catch
            {
                m_bOn = false;
            }
        }

    }

    public class WorldInfo : TreeData
    {        
        public string m_strFileName;
        public Dictionary<int, LocalMapInfo> m_LocalMapList;

        public WorldInfo()
        {
            m_eType = NodeDataType.DT_WORLD;
        }

        public override void AddtoTreeView(TreeNodeCollection treeNodeCollection, bool bParentChecked )
        {
            TreeNode treeNode = new TreeNode(m_strEnumName);

            treeNode.ToolTipText = "ID : " + m_iID.ToString() + "\nFilename : " + m_strFileName;
            treeNode.Checked = m_bOn && bParentChecked;
            treeNode.Tag = this;
            treeNodeCollection.Add(treeNode);

            // Create the ContextMenuStrip.
            ContextMenuStrip WorldMenu = new ContextMenuStrip();

            //Create some menu items.
            ToolStripMenuItem openLabel = new ToolStripMenuItem();
            openLabel.Text = "Open Script(Not Yet)";
            
            //Add the menu items to the menu.
            WorldMenu.Items.AddRange(new ToolStripMenuItem[]{openLabel});

            // Set the ContextMenuStrip property to the ContextMenuStrip.
            treeNode.ContextMenuStrip = WorldMenu;


            foreach( LocalMapInfo LMInfo in m_LocalMapList.Values )
            {
                LMInfo.AddtoTreeView(treeNode.Nodes, treeNode.Checked);
            }
        }

        public override void CreateCheckList(ref Dictionary<int, bool> dicCheckList)
        {
            dicCheckList.Add(m_iID, m_bOn);

            foreach (LocalMapInfo LMInfo in m_LocalMapList.Values)
            {
                LMInfo.CreateCheckList(ref dicCheckList);
            }
        }
        
        public override void LoadMetaData(Dictionary<int, bool> CheckList)
        {
            try
            {
                m_bOn = CheckList[m_iID];
            }
            catch
            {
                m_bOn = false;
            }

            foreach (LocalMapInfo LMInfo in m_LocalMapList.Values)
            {
                LMInfo.LoadMetaData(CheckList);
            }
        }

    };

    public class LocalMapInfo : TreeData
    {

        public LocalMapInfo()
        {
            m_eType = NodeDataType.DT_LOCAL;
        }

        public string m_strFileName;
        public Dictionary<int, DungeonInfo> m_DungeonList;
        public int BaseWorldStateID;

        public override void AddtoTreeView(TreeNodeCollection treeNodeCollection, bool bParentChecked)
        {
            System.Windows.Forms.TreeNode treeNode = new System.Windows.Forms.TreeNode(m_strEnumName);
            treeNode.Checked = m_bOn && bParentChecked;
            treeNode.Tag = this;
            treeNode.ToolTipText = "ID : " + m_iID.ToString() + "\nFilename : " + m_strFileName;

            // Create the ContextMenuStrip.
            ContextMenuStrip LocalMenu = new ContextMenuStrip();

            //Create some menu items.
            ToolStripMenuItem openLabel = new ToolStripMenuItem();
            openLabel.Text = "Open Script(Not Yet)";

            //Add the menu items to the menu.
            LocalMenu.Items.AddRange(new ToolStripMenuItem[] { openLabel });

            // Set the ContextMenuStrip property to the ContextMenuStrip.
            treeNode.ContextMenuStrip = LocalMenu;


            treeNodeCollection.Add(treeNode);

            foreach (DungeonInfo DgInfo in m_DungeonList.Values)
            {
                DgInfo.AddtoTreeView(treeNode.Nodes, treeNode.Checked);
            }
        }

        public override void CreateCheckList(ref Dictionary<int, bool> dicCheckList)
        {
            dicCheckList.Add(m_iID, m_bOn);

            foreach (DungeonInfo DgInfo in m_DungeonList.Values)
            {
                DgInfo.CreateCheckList(ref dicCheckList);
            }
        }

        public override void LoadMetaData(Dictionary<int, bool> CheckList)
        {
            try
            {
                m_bOn = CheckList[m_iID];
            }
            catch
            {
                m_bOn = false;
            }

            foreach (DungeonInfo DgInfo in m_DungeonList.Values)
            {
                DgInfo.LoadMetaData(CheckList);
            }
        }
        
    };

    public class DungeonInfo : TreeData
    {
        public string m_strFileName;
        public DungeonInfoData m_DInfoData = null;

        public DungeonInfo()
        {
            m_eType = NodeDataType.DT_DUNGEON;
        }
        
        public override void AddtoTreeView(TreeNodeCollection treeNodeCollection, bool bParentChecked)
        {
            TreeNode treeNode = new TreeNode(m_strEnumName);
            treeNode.Checked = m_bOn && bParentChecked;
            treeNode.Tag = this;
            if(m_DInfoData == null)
            {
                treeNode.ToolTipText = "DID : " + m_iID.ToString() + "\nDungeonInfoFileName : " + m_strFileName;
            }
            else
            {
                treeNode.ToolTipText = "DID : " + m_iID.ToString() +
                                        "\nDungeonName : " + StringManager.GetGeneralString(m_DInfoData.m_DungeonName_ID) +
                                        "\nDungeonInfoFileName : " + m_strFileName +
                                        "\nDungeonDataFileName : " + m_DInfoData.m_DataFileName;
            }  
            

            // Create the ContextMenuStrip.
            ContextMenuStrip DungeonMenu = new ContextMenuStrip();

            //Create some menu items.
            ToolStripMenuItem openLabel = new ToolStripMenuItem();
            openLabel.Text = "Open DungeonData Script(Not Yet)";

            //Create some menu items.
            ToolStripMenuItem openLabel2 = new ToolStripMenuItem();
            openLabel2.Text = "Open Data Script(Not Yet)";

            //Add the menu items to the menu.
            DungeonMenu.Items.AddRange(new ToolStripMenuItem[] { openLabel, openLabel2 });

            // Set the ContextMenuStrip property to the ContextMenuStrip.
            treeNode.ContextMenuStrip = DungeonMenu;


            treeNodeCollection.Add(treeNode);
        }

        public void SetDungeonData()
        {
            int beginIndex = m_strEnumName.IndexOf("[\"");
            int endIndex = m_strEnumName.IndexOf("\"]");
            string IDPart = m_strEnumName.Substring(beginIndex + 2, endIndex - beginIndex - 2);
            m_strFileName = "DungeonData_" + IDPart + ".lua";
            string FullPathFileName = ".\\TempDungeonLua\\" + m_strFileName;

            m_DInfoData = new DungeonInfoData();
            m_DInfoData.ParseDungeonInfoData(FullPathFileName);
        }

        
    };
        
    static class ScriptTreeManager
    {
        static ScriptTreeManager()
        {
            m_WorldList = new Dictionary<int, WorldInfo>();
            m_LocalMapList = new Dictionary<int, LocalMapInfo>();
            m_StartPosList = new Dictionary<int, int>();
            m_CheckList = new Dictionary<int, bool>();
        }
        
        static public bool ReadWorldInfo()
        {
            string TableName;
            Dictionary<int, string> dicWorld;
            Dictionary<int, string> dicWorldIDString;

            m_WorldList.Clear();
            m_LocalMapList.Clear();
            m_StartPosList.Clear();

            ScriptParser.m_DicEnumString.Clear();

            if (ScriptParser.ParseIDString( ScriptTreeManager.strDialogDir + "DLG_Map_World.lua", out dicWorldIDString, out TableName) &&
                ScriptParser.ParseIDandValue<string>(ScriptTreeManager.strDialogDir + "DLG_Map_World.lua", TableName, out dicWorld))
            {
                //{{ 각 World Info에 대해
                foreach (KeyValuePair<int, string> entry in dicWorldIDString)
                {
                    WorldInfo WI = new WorldInfo();
                    WI.m_iID = (int)entry.Key;
                    WI.m_strEnumName = (string)entry.Value;
                    
                    try
                    {
                        WI.m_strFileName = dicWorld[WI.m_iID];
                        WI.m_bOn = true;    
                    }
                    catch(Exception)
                    {
                        WI.m_strFileName = "";
                        WI.m_bOn = false;
                    }
                    WI.m_LocalMapList = new Dictionary<int, LocalMapInfo>();

                    m_WorldList.Add(WI.m_iID, WI);
                } //}} World

                // Local Info 안에 자기가 어떤 World에 속하는지가 있으므로 따로 뽑자. 스크립트 상하관계가 좀 이상한데 뭐 한두날 이런가..
                ReadLocalInfo(ref m_WorldList);

                VillageInfoData VD = new VillageInfoData();
                Lua lua = new Lua();

                LuaTool.DoFile(lua, ScriptTreeManager.strMajorDir + "Enum.lua");
                LuaTool.DoFile(lua, ScriptTreeManager.strMajorDir + "StringID_def.lua");
                                
                m_StartPosList.Clear();
                VD.ParseVillage(lua, ref m_StartPosList);
            }
            else
            {
                return false;
            }

            

            return true;
        }

        static private void ReadLocalInfo(ref Dictionary<int, WorldInfo> WorldMapList)
        {            
            //{{ World Info에 속하는 Local Info들에 대해
            Dictionary<int, string> dicLocal;
            Dictionary<int, string> dicLocalIDString;
            string LocalTableName;
            if (ScriptParser.ParseIDString(ScriptTreeManager.strDialogDir + "DLG_Map_Local.lua", out dicLocalIDString, out LocalTableName) &&
                ScriptParser.ParseIDandValue<string>(ScriptTreeManager.strDialogDir + "Dlg_Map_Local.lua", LocalTableName, out dicLocal))
            {
                //LocalmapList = new Dictionary<int, LocalMapInfo>();
                foreach (KeyValuePair<int, string> localentry in dicLocalIDString)
                {
                    LocalMapInfo LI = new LocalMapInfo();
                    LI.m_iID = localentry.Key;
                    LI.m_strEnumName = localentry.Value;
                    
                    int BaseWorldmapID = -1;
                    try
                    {                        
                        LI.m_strFileName = dicLocal[LI.m_iID];
                        LI.m_bOn = true;

                        Lua Locallua = new Lua();

                        LuaTool.DoFile(Locallua, ScriptTreeManager.strMajorDir + "Enum.lua");
                        LuaTool.DoFile(Locallua, ScriptTreeManager.strDialogDir + LI.m_strFileName);
                        
                        LuaTool.GetValue(Locallua, "BaseWorldStateID", out BaseWorldmapID, -1);

                        ReadDungeonInfo(ScriptTreeManager.strDialogDir + LI.m_strFileName, Locallua, out LI.m_DungeonList);
                        
                    }
                    catch (Exception)
                    {
                        LI.m_strFileName = "";
                        LI.m_bOn = false;
                    }

                    if (BaseWorldmapID != -1)
                    {
                        try
                        {
                            m_LocalMapList.Add(LI.m_iID, LI);
                            WorldMapList[BaseWorldmapID].m_LocalMapList.Add(LI.m_iID, LI);
                        }
                        catch (KeyNotFoundException)
                        {
                            MessageBox.Show("World ID " + BaseWorldmapID.ToString() + " : No such World ID");
                        }
                        catch( Exception e )
                        {
                            MessageBox.Show(e.Message);
                        }
                    }                   
                }
            }
        }

        static private void ReadDungeonInfo(string filename, Lua Locallua, out Dictionary<int, DungeonInfo> DungeonList)
        {
            // 각 Local Map에 속하는 Dungeon ID에 대해
            //{{
            DungeonList = null;
            Dictionary<int, string> dicDungeonIDString;
            if (ScriptParser.ParseIDStringFromTable(filename, "DungeonIDList", out dicDungeonIDString))
            {
                LuaTable DIDTable = Locallua.GetTable("DungeonIDList");

                DungeonList = new Dictionary<int, DungeonInfo>();
                foreach (KeyValuePair<int, string> dungeonEntry in dicDungeonIDString)
                {
                    DungeonInfo DI = new DungeonInfo();
                    DI.m_iID = dungeonEntry.Key;
                    DI.m_strEnumName = dungeonEntry.Value;
                    
                    DI.m_bOn = false;
                    foreach (DictionaryEntry ent in DIDTable)
                    {
                        if (DI.m_iID == (int)(double)ent.Value)
                        {
                            DI.m_bOn = true;
                        }
                    }
                    DI.SetDungeonData();
                    
                    DungeonList.Add(DI.m_iID, DI);
                }

            }

            //}} Dungeon
        }

        static public void AddtoTreeView(TreeNodeCollection treeNodeCollection)
        {
            foreach( WorldInfo WI in m_WorldList.Values )
            {
                WI.AddtoTreeView(treeNodeCollection, true);
            }
        }
        
        static public void CreateCheckList()
        {
            m_CheckList = new Dictionary<int,bool>();

            foreach (WorldInfo w in m_WorldList.Values)
            {
                w.CreateCheckList(ref m_CheckList);
            }

        }

        static public void SaveMetadata(string filename, string code)
        {
            CreateCheckList();
            StreamWriter sw = null;
            
            try
            {
                sw = new StreamWriter(filename, false, Encoding.UTF8);
                sw.WriteLine("##ScriptData##");
                sw.WriteLine("##Code##\t" + code);
                
                foreach (KeyValuePair<int,bool> check in m_CheckList)
                {
                    int outputvalue;
                    if (check.Value)
                        outputvalue = 1;
                    else
                        outputvalue = 0;
                    sw.WriteLine(check.Key.ToString()+"\t"+outputvalue.ToString());
                }
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }
            finally
            {
                if(sw != null)
                    sw.Close();
            }
        }

        static public void LoadMetadata(string filename)
        {
            StreamReader sr = null;
            try
            {
                sr = new StreamReader(filename, Encoding.UTF8);
                Dictionary<int, bool> CheckList = new Dictionary<int, bool>();

                string wstrStringRead;

                wstrStringRead = sr.ReadLine();
                if(wstrStringRead.IndexOf("##ScriptData##") == -1)
                {
                    MessageBox.Show("Not a Valid data file");
                    return;
                }

                wstrStringRead = sr.ReadLine();
                if(wstrStringRead.IndexOf("##Code##") == -1)
                {
                    MessageBox.Show("Not a Valid data file");
                    return;
                }
                else
                {
                    int iTabPos = wstrStringRead.IndexOf("\t");
                    string Code = wstrStringRead.Substring(iTabPos+1);
                    Program.m_Form.textBoxCode.Text = Code;

                }

                do
                {
                    wstrStringRead = sr.ReadLine();

                    int iTabPos = wstrStringRead.IndexOf("\t");
                    string wstrIndex = wstrStringRead.Substring(0, iTabPos);
                    int iIndexRead = int.Parse(wstrIndex);

                    string wstrContent = "";
                    bool bVal = false;
                                        
                    // tab 을 찾은 경우에만 그 뒤의 내용을 읽어야 한다.
                    if (iTabPos != -1)
                    {                            
                        // tab 뒤에 있는 것들만 읽는다.
                        if ((iTabPos + 1) < wstrStringRead.Length)
                        {
                            wstrContent = wstrStringRead.Substring(iTabPos + 1);
                        }
                        int value = int.Parse(wstrContent);
                        if (value == 1)
                            bVal = true;
                        else
                            bVal = false;
                    }

                    CheckList.Add(iIndexRead, bVal);

                    
                } while (sr.EndOfStream == false);

                foreach( WorldInfo WI in m_WorldList.Values )
                {
                    WI.LoadMetaData(CheckList);                    
                }

            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }
            finally
            {
                if (sr != null)
                    sr.Close();
                
            }
            
        }

        static public void ExportScript(string TargetFolder, bool AddCode, string code, Encoding encoding)
        {
            CreateCheckList();
            ExportWorldScript(TargetFolder, AddCode, code, encoding);
            ExportLocalMapScript(TargetFolder, AddCode, code, encoding);
            ExportVillageScript(TargetFolder, AddCode, code, encoding);
            ExportDungeonData(TargetFolder, AddCode, code, encoding);
                        
            
        }

        static void ExportWorldScript(string TargetFolder, bool AddCode, string code, Encoding encoding)
        {
            // 1. DLG_Map_World Export
            string swfilename;
            if (AddCode && code.Length > 0)
            {
                swfilename = TargetFolder + "\\DLG_Map_World@" + code + ".lua";
            }
            else
            {
                swfilename = TargetFolder + "\\DLG_Map_World.lua";

            }
            StreamWriter swWorldMap = null;
            StreamReader srWorldMap = null;
            try
            {
                srWorldMap = new StreamReader(ScriptTreeManager.strDialogDir + "DLG_Map_World.lua", encoding);
                string tmp = srWorldMap.ReadLine();
                swWorldMap = new StreamWriter(swfilename,false, Encoding.UTF8);
                swWorldMap.WriteLine(tmp);

                swWorldMap.WriteLine();
                swWorldMap.WriteLine("World = {}");
                foreach (KeyValuePair<int, WorldInfo> worldentry in m_WorldList)
                {
                    if (worldentry.Value.m_bOn == false)
                        swWorldMap.Write("--");
                    
                    string EnumString = ScriptParser.m_DicEnumString[worldentry.Key];
                    swWorldMap.WriteLine("World[ " + EnumString + " ] = \"" + worldentry.Value.m_strFileName + "\"");

                    if (worldentry.Value.m_bOn == true)
                    {
                        ExportDetailedWorldMapScript(worldentry.Value.m_strFileName, TargetFolder, AddCode, code, encoding);
                    }
                }
            }
            finally
            {
                if (swWorldMap != null)
                    swWorldMap.Close();
                if (srWorldMap != null)
                    srWorldMap.Close();
            }
        }
        static void ExportDetailedWorldMapScript(string filename, string TargetFolder, bool AddCode, string code, Encoding encoding)
        {
            // 2-1. Export Files in DLG_Map_World
            string swfilename;
            if (AddCode && code.Length > 0)
            {
                int extensionpos = filename.IndexOf(".lua");
                string namepart = filename.Substring(0, extensionpos);

                swfilename = TargetFolder + "\\" + namepart + "@" + code + ".lua";
            }
            else
            {
                swfilename = TargetFolder + "\\" + filename;
            }


            StreamReader srWorld = null;
            StreamWriter swWorld = null;
            try
            {
                srWorld = new StreamReader(ScriptTreeManager.strDialogDir + filename, Encoding.UTF8, false);
                swWorld = new StreamWriter(swfilename, false, encoding);

                // LocalIdList만 잘 확인하고, 나머지는 전부 그대로 Output.
                bool bInLocalIDList = false;
                int ContextDepth = 0;
                Lua lua = new Lua();
                LuaTool.DoFile(lua, ScriptTreeManager.strMajorDir + "Enum.lua");
                while (false == srWorld.EndOfStream)
                {
                    string strReadBuffer = srWorld.ReadLine();

                    if (bInLocalIDList)
                    {
                        int searchindex = 0;
                        while (strReadBuffer.IndexOf('{', searchindex) != -1)
                        {
                            searchindex = strReadBuffer.IndexOf('{', searchindex) + 1;
                            ContextDepth++;
                        }

                        searchindex = 0;
                        while (strReadBuffer.IndexOf('}', searchindex) != -1)
                        {
                            searchindex = strReadBuffer.IndexOf('}', searchindex) + 1;
                            ContextDepth--;
                        }

                        if (ContextDepth < 1)
                            bInLocalIDList = false;

                        if (strReadBuffer.Contains("LOCAL_MAP_ID"))
                        {
                            strReadBuffer = strReadBuffer.TrimStart('-');
                            strReadBuffer = strReadBuffer.TrimStart();
                            strReadBuffer = strReadBuffer.TrimStart('-');

                            int indexofcolon = strReadBuffer.IndexOf(',');
                            string DIDEnumString = strReadBuffer.Substring(0, indexofcolon);
                            int IDValue = (int)(double)lua[LuaTool.ConvertStringForm(DIDEnumString)];

                            if (m_CheckList[IDValue])
                            {
                                strReadBuffer = "\t" + strReadBuffer;
                            }
                            else
                            {
                                strReadBuffer = "\t--" + strReadBuffer;
                            }

                        }

                    }

                    if (strReadBuffer.Contains("LocalIDList ="))
                    {
                        bInLocalIDList = true;
                    }

                    swWorld.WriteLine(strReadBuffer);
                }

            }
            finally
            {
                if (srWorld != null)
                    srWorld.Close();
                if (swWorld != null)
                    swWorld.Close();
            }


        }

        static void ExportLocalMapScript(string TargetFolder, bool AddCode, string code, Encoding encoding)
        {
            // 2. DLG_Map_Local Export
            string swfilename;
            if (AddCode && code.Length > 0)
            {
                swfilename = TargetFolder + "\\DLG_Map_Local@" + code + ".lua";
            }
            else
            {
                swfilename = TargetFolder + "\\DLG_Map_Local.lua";
            }
            StreamWriter swLocalMap = null;
            StreamReader srLocalMap = null;
            try
            {
                srLocalMap = new StreamReader(ScriptTreeManager.strDialogDir + "DLG_Map_Local.lua", Encoding.UTF8);
                string tmp = srLocalMap.ReadLine();

                swLocalMap = new StreamWriter(swfilename, false, encoding);
                swLocalMap.WriteLine(tmp);
                swLocalMap.WriteLine();
                swLocalMap.WriteLine("Local = {}");
                swLocalMap.WriteLine();
                foreach (KeyValuePair<int, LocalMapInfo> localentry in m_LocalMapList)
                {
                    if (localentry.Value.m_bOn == false)
                        swLocalMap.Write("--");
                    
                    string EnumString = ScriptParser.m_DicEnumString[localentry.Key];
                    swLocalMap.WriteLine("Local[ " + EnumString + " ]\t\t = \"" + localentry.Value.m_strFileName + "\"");

                    if(localentry.Value.m_bOn == true)
                    {
                        ExportDetailedLocalMapScript(localentry.Value.m_strFileName, TargetFolder, AddCode, code, encoding);
                    }
                }
            }
            finally
            {
                if (swLocalMap != null)
                    swLocalMap.Close();
                if (srLocalMap != null)
                    srLocalMap.Close();
            }

        }
        static void ExportDetailedLocalMapScript(string filename, string TargetFolder, bool AddCode, string code, Encoding encoding)
        {
            // 2-1. Export Files in DLG_Map_Local
            string swfilename;
            if (AddCode && code.Length > 0)
            {
                int extensionpos = filename.IndexOf(".lua");
                string namepart = filename.Substring(0, extensionpos);

                swfilename = TargetFolder + "\\" + namepart + "@" + code + ".lua";
            }
            else
            {
                swfilename = TargetFolder + "\\" + filename;
            }


            StreamReader srLocal = null;
            StreamWriter swLocal = null;
            try
            {
                srLocal = new StreamReader(ScriptTreeManager.strDialogDir + filename, Encoding.UTF8, false);
                swLocal = new StreamWriter(swfilename, false, encoding);

                // DungeonIDList만 잘 확인하고, 나머지는 전부 그대로 Output.
                bool bInDungeonIDList = false;
                int ContextDepth = 0;
                Lua lua = new Lua();
                LuaTool.DoFile(lua, ScriptTreeManager.strMajorDir + "Enum.lua");
                while (false == srLocal.EndOfStream)
                {
                    string strReadBuffer = srLocal.ReadLine();
                                        
                    if(bInDungeonIDList)
                    {
                        int searchindex = 0;
                        while (strReadBuffer.IndexOf('{', searchindex) != -1)
                        {
                            searchindex = strReadBuffer.IndexOf('{', searchindex) + 1;
                            ContextDepth++;
                        }

                        searchindex = 0;
                        while (strReadBuffer.IndexOf('}', searchindex) != -1)
                        {
                            searchindex = strReadBuffer.IndexOf('}', searchindex) + 1;
                            ContextDepth--;
                        }

                        if (ContextDepth < 1)
                            bInDungeonIDList = false;

                        if(strReadBuffer.Contains("DUNGEON_ID"))
                        {
                            strReadBuffer = strReadBuffer.TrimStart('-');
                            strReadBuffer = strReadBuffer.TrimStart();
                            strReadBuffer = strReadBuffer.TrimStart('-');                            

                            int indexofcolon = strReadBuffer.IndexOf(',');
                            string DIDEnumString = strReadBuffer.Substring(0, indexofcolon);
                            int IDValue = (int)(double)lua[LuaTool.ConvertStringForm(DIDEnumString)];

                            if(m_CheckList[IDValue])
                            {
                                strReadBuffer = "\t" + strReadBuffer;
                            }
                            else
                            {
                                strReadBuffer = "\t--" + strReadBuffer;
                            }

                        }

                    }

                    if (strReadBuffer.Contains("DungeonIDList ="))
                    {
                        bInDungeonIDList = true;
                    }

                    swLocal.WriteLine(strReadBuffer);
                }

            }
            finally
            {
                if (srLocal != null)
                    srLocal.Close();
                if (swLocal != null)
                    swLocal.Close();
            }


        }
        static void ExportVillageScript(string TargetFolder, bool AddCode, string code, Encoding encoding)
        {
            // 3. DLG_Map_Village Export
            string swfilename;
            if (AddCode && code.Length > 0)
            {
                swfilename = TargetFolder + "\\DLG_Map_Village@" + code + ".lua";
            }
            else
            {
                swfilename = TargetFolder + "\\DLG_Map_Village.lua";
            }
            StreamReader srVillageMap = null;
            StreamWriter swVillageMap = null;
            try
            {
                srVillageMap = new StreamReader(ScriptTreeManager.strDialogDir + "DLG_Map_Village.lua", Encoding.UTF8, false);
                swVillageMap = new StreamWriter(swfilename, false, encoding);

                // LinkPos만 잘 확인하고, 나머지는 전부 그대로 Output.
                while (false == srVillageMap.EndOfStream)
                {
                    string strReadBuffer = srVillageMap.ReadLine();
                    if(strReadBuffer.IndexOf("LinkPos") != -1)
                    {
                        int indexbegin = strReadBuffer.IndexOf('=');
                        string SubStr = strReadBuffer.Substring(indexbegin + 1);
                        char[] split = new char[3];
                        split[0] = '{';
                        split[1] = '}';
                        split[2] = ',';
                        string[] LinkPos = SubStr.Split(split, System.StringSplitOptions.RemoveEmptyEntries);
                        bool bLinkOK = true;
                        foreach(string s in LinkPos)
                        {
                            string str = s.Trim();
                            if (str.IndexOf("--") != -1)
                                continue;
                            if (str.Length < 1)
                                continue;
                            int TargetLinkPos = int.Parse(str);
                            if (m_CheckList[m_StartPosList[TargetLinkPos]] == false)
                            {
                                bLinkOK = false;
                            }
                        }

                        if(bLinkOK)
                        {
                            strReadBuffer = strReadBuffer.Replace("--LinkPos", "LinkPos");
                        }
                        else
                        {
                            strReadBuffer = strReadBuffer.Replace("LinkPos", "--LinkPos");
                        }
                    }
                    swVillageMap.WriteLine(strReadBuffer);
                }
                
            }
            finally
            {
                if (srVillageMap != null)
                    srVillageMap.Close();
                if (swVillageMap != null)
                    swVillageMap.Close();
            }

        }
        static void ExportDungeonData(string TargetFolder, bool AddCode, string code, Encoding encoding)
        {
            // 4. DungeonData.lua Export
            string swfilename;
            if (AddCode && code.Length > 0)
            {
                swfilename = TargetFolder + "\\DungeonData@" + code + ".lua";
            }
            else
            {
                swfilename = TargetFolder + "\\DungeonData.lua";

            }

            StreamReader srDungeonData = null;
            StreamWriter swDungeonData = null;
            try
            {
                srDungeonData = new StreamReader(ScriptTreeManager.strDungeonDir + "DungeonData.lua", Encoding.UTF8, false);
                swDungeonData = new StreamWriter(swfilename, false, encoding);

                // DungeonIDList만 잘 확인하고, 나머지는 전부 그대로 Output.
                Lua lua = new Lua();
                LuaTool.DoFile(lua, ScriptTreeManager.strMajorDir + "Enum.lua");
                while (false == srDungeonData.EndOfStream)
                {
                    string strReadBuffer = srDungeonData.ReadLine();

                    if (strReadBuffer.Contains("dungeonID"))
                    {
                        int indexequal = strReadBuffer.IndexOf('=');
                        int indexofcolon = strReadBuffer.IndexOf(',');
                        string DIDEnumString = strReadBuffer.Substring(indexequal + 1, indexofcolon - indexequal - 1);
                        DIDEnumString = DIDEnumString.Trim();
                        DIDEnumString = DIDEnumString.Replace("HARD", "NORMAL");
                        DIDEnumString = DIDEnumString.Replace("EXPERT", "NORMAL");
                        
                        try
                        {
                            int IDValue = (int)(double)lua[LuaTool.ConvertStringForm(DIDEnumString)];
                            if (m_CheckList[IDValue])
                            {
                                strReadBuffer = strReadBuffer.Replace("--dungeonID", "dungeonID");
                            }
                            else
                            {
                                strReadBuffer = strReadBuffer.Replace("dungeonID", "--dungeonID");
                            }
                        }
                        catch(Exception)
                        {
                            // 딱히 List에 없었던 던전의 경우 그냥 넘어간다.
                        }

                        

                    }

                    swDungeonData.WriteLine(strReadBuffer);
                }

            }
            finally
            {
                if (srDungeonData != null)
                    srDungeonData.Close();
                if (swDungeonData != null)
                    swDungeonData.Close();
            }
        }

        static public bool SetResourceDir(string path)
        {
            DirectoryInfo dirinfo1 = new DirectoryInfo(path + "\\dat\\Script\\Major\\");
            DirectoryInfo dirinfo2 = new DirectoryInfo(path + "\\dat\\Script\\Dungeon\\");
            DirectoryInfo dirinfo3 = new DirectoryInfo(path + "\\dat\\Script\\Dialog\\");
            DirectoryInfo dirinfo4 = new DirectoryInfo(path);
            if (dirinfo1.Exists && dirinfo2.Exists && dirinfo3.Exists && dirinfo4.Exists)
            {
                strMajorDir = path + "\\dat\\Script\\Major\\";
                strDungeonDir = path + "\\dat\\Script\\Dungeon\\";
                strDialogDir = path + "\\dat\\Script\\Dialog\\";
                strKRStringDir = path;
                return true;
            }
            else
            {
                MessageBox.Show("Not a vaild Elsword resource folder");
                return false;
            }
        }
        

        static Dictionary<int, WorldInfo> m_WorldList;
        static Dictionary<int, LocalMapInfo> m_LocalMapList;
        static Dictionary<int, int> m_StartPosList;     // <StartPos, LocalMapID>
        static Dictionary<int, bool> m_CheckList;

        static public string strMajorDir = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Major\\";
        static public string strDungeonDir = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Dungeon\\";
        static public string strDialogDir = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Dialog\\";
        static public string strKRStringDir = "..\\..\\..\\resource\\KR\\Trunk\\";
        

    }
}
