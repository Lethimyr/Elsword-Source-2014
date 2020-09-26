
using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using TransDBManagerForWeb.LocalizationDataSetTableAdapters;
using TransDBManagerForWeb.ScriptData_TWDataSetTableAdapters;
using TransDBManagerForWeb.ScriptData_HKDataSetTableAdapters;
using TransDBManagerForWeb.ScriptData_JPDataSetTableAdapters;
using System.Data.SqlClient;


namespace TransDBManagerForWeb
{
    public partial class _Default : System.Web.UI.Page
    {
        private SqlConnection Con;
        private SqlDataAdapter Adpt_Login;
        DataTable tblLogin;


        TransTableAdapter Adpt_Trans = new TransTableAdapter();
        Trans_HistoryTableAdapter Adpt_Trans_History = new Trans_HistoryTableAdapter();
        DataTable m_tblTransKR;
        DataTable m_tblTransForeign;
        DataTable m_tblTransCurrent;
        string strCountryCode = string.Empty;
        string strIP = string.Empty;

        static bool m_bSearch = false;
        protected void Page_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";
            Con.Open();

            Adpt_Login = new SqlDataAdapter("SELECT * FROM TUser", Con);
            tblLogin = new DataTable("tblLogin");
                        
            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt_Login);
            Adpt_Login.Fill(tblLogin);
            tblLogin.PrimaryKey = new DataColumn[] { tblLogin.Columns["ID"] };

            DataRow Result = tblLogin.Rows.Find(Request.QueryString["ID"].ToString());
            if (Result == null)
            {
                Response.Redirect("Error.aspx");
            }            

            strCountryCode = Request.QueryString["COUNTRY"].ToString();
            strIP = Result["IPAdress"].ToString();

            if (Result["Country"].ToString() != strCountryCode)
            {
                Response.Redirect("Error.aspx");
            }

            if (RadioButton1.Checked)
            {
                
                m_tblTransKR = Adpt_Trans.GetDataByCountry("KR");
                m_tblTransForeign = Adpt_Trans.GetDataByCountry(strCountryCode);

            }
            else if(RadioButton2.Checked)         
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "Item");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "Item");
            }
            else if(RadioButton3.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "SetItem");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "SetItem");
            }
            else if (RadioButton4.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "QuestTable");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "QuestTable");
            }
            else if (RadioButton5.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "SubQuestTable");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "SubQuestTable");
            }
            else if (RadioButton6.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "TitleTable");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "TitleTable");
            }
            else if (RadioButton7.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "TitleMission");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "TitleMission");
            }
            else if (RadioButton8.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "SubTitleMission");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "SubTitleMission");
            }
            else if (RadioButton9.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "StringTable");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "StringTable");
            }
            else if (RadioButton10.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "NetErrorTable");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "NetErrorTable");
            }
            else if (RadioButton11.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "TextureStringManager");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "TextureStringManager");
            }
            else if (RadioButton13.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "SkillEnum");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "SkillEnum");
            }
            else if (RadioButton14.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountryTableName("KR", "SkillTable");
                m_tblTransForeign = Adpt_Trans.GetDataByCountryTableName(strCountryCode, "SkillTable");
            }
            else if (RadioButton12.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountry("KR");
                m_tblTransForeign = Adpt_Trans.GetDataByCountry(strCountryCode);

                for(int i = 0; i < m_tblTransKR.Rows.Count; i++)
                {
                    int nCompare = Convert.ToDateTime(m_tblTransKR.Rows[i]["Last Update Date"].ToString()).CompareTo(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()));
                    if(nCompare < 0)
                    {
                        m_tblTransKR.Rows[i].Delete();
                        m_tblTransForeign.Rows[i].Delete();
                    }
                }

            }

            if(m_bSearch)
            {
                if (RadioButton15.Checked)
                {
                    for (int i = 0; i < m_tblTransKR.Rows.Count; i++)
                    {
                        if (m_tblTransKR.Rows[i].RowState == DataRowState.Deleted)
                        {
                            continue;
                        }
                        if (m_tblTransKR.Rows[i]["String"].ToString().IndexOf(TextSearch.Text) == -1)
                        {
                            m_tblTransKR.Rows[i].Delete();
                            m_tblTransForeign.Rows[i].Delete();
                        }
                    }
                }
                else if (RadioButton16.Checked)
                {
                    for (int i = 0; i < m_tblTransForeign.Rows.Count; i++)
                    {
                        if (m_tblTransForeign.Rows[i].RowState == DataRowState.Deleted)
                        {
                            continue;
                        }
                        if (m_tblTransForeign.Rows[i]["String"].ToString().IndexOf(TextSearch.Text) == -1)
                        {
                            m_tblTransKR.Rows[i].Delete();
                            m_tblTransForeign.Rows[i].Delete();
                        }
                    }
                }
            }

            CurrentTableCreate();

            
        }

        void CurrentTableCreate()
        {
            m_tblTransCurrent = new DataTable();
            DataColumn Col;
            Col = new DataColumn("Index", m_tblTransKR.Columns["Index"].DataType);
            Col.MaxLength = m_tblTransKR.Columns["Index"].MaxLength;
            //Col.AllowDBNull = m_tblTransKR.Columns["TransIDX"].AllowDBNull;
            //Col.Unique = m_tblTransKR.Columns["TransIDX"].Unique;
            m_tblTransCurrent.Columns.Add(Col);

            Col = new DataColumn(m_tblTransKR.Columns["TableName"].ColumnName, m_tblTransKR.Columns["TableName"].DataType);
            Col.MaxLength = m_tblTransKR.Columns["TableName"].MaxLength;
            Col.AllowDBNull = m_tblTransKR.Columns["TableName"].AllowDBNull;
            Col.Unique = m_tblTransKR.Columns["TableName"].Unique;
            m_tblTransCurrent.Columns.Add(Col);

            Col = new DataColumn(m_tblTransKR.Columns["String"].ColumnName, m_tblTransKR.Columns["String"].DataType);
            Col.MaxLength = m_tblTransKR.Columns["String"].MaxLength;
            Col.AllowDBNull = m_tblTransKR.Columns["String"].AllowDBNull;
            Col.Unique = m_tblTransKR.Columns["String"].Unique;
            m_tblTransCurrent.Columns.Add(Col);


            Col = new DataColumn("String_Foreign", m_tblTransForeign.Columns["String"].DataType);

            Col.MaxLength = m_tblTransForeign.Columns["String"].MaxLength;
            Col.AllowDBNull = m_tblTransForeign.Columns["String"].AllowDBNull;
            Col.Unique = m_tblTransForeign.Columns["String"].Unique;
            m_tblTransCurrent.Columns.Add(Col);

            //09. 06. 29 김정협 메모기능을 위한 행하나 추가 스트링과 기본기능이 같으므로 스트링의 기본설정가져다 씀
            Col = new DataColumn("Memo", m_tblTransForeign.Columns["String"].DataType);

            Col.MaxLength = m_tblTransForeign.Columns["String"].MaxLength;
            Col.AllowDBNull = m_tblTransForeign.Columns["String"].AllowDBNull;
            Col.Unique = m_tblTransForeign.Columns["String"].Unique;
            m_tblTransCurrent.Columns.Add(Col);

            Col = new DataColumn("Last Update Date", m_tblTransForeign.Columns["Last Update Date"].DataType);

            Col.MaxLength = m_tblTransForeign.Columns["Last Update Date"].MaxLength;
            Col.AllowDBNull = m_tblTransForeign.Columns["Last Update Date"].AllowDBNull;
            Col.Unique = m_tblTransForeign.Columns["Last Update Date"].Unique;
            m_tblTransCurrent.Columns.Add(Col);


            int nCount = 0;
            for (int i = 0; i < m_tblTransKR.Rows.Count; i++)
            {
                if (m_tblTransKR.Rows[i].RowState == DataRowState.Deleted)
                {
                    continue;
                }
                m_tblTransCurrent.Rows.Add();

                DataRow String_Current = m_tblTransCurrent.Rows[nCount];
                DataRow String_Original = m_tblTransKR.Rows[i];
                DataRow String_Foreign = m_tblTransForeign.Rows[i];


                String_Current["Index"] = String_Original["Index"];
                String_Current["TableName"] = String_Original["TableName"];
                String_Current["String"] = String_Original["String"];
                String_Current["String_Foreign"] = String_Foreign["String"];
                String_Current["Memo"] = String_Original["Memo"];
                String_Current["Last Update Date"] = String_Foreign["Last Update Date"];
                nCount++;
            }

            if (!IsPostBack)
            {
                GridView1.DataSource = m_tblTransCurrent;
                GridView1.DataBind();
                TextPage.Text = Convert.ToString(GridView1.PageIndex + 1);
                Label1.Text = "/" + GridView1.PageCount;                
            }
            
        }

        protected void GridView1_PageIndexChanging(object sender, GridViewPageEventArgs e)
        {
            GridView1.PageIndex = e.NewPageIndex;
            TextPage.Text = Convert.ToString(e.NewPageIndex + 1);
            CurrentTableCreate();
            GridView1.DataSource = m_tblTransCurrent;
            GridView1.DataBind();
        }

        protected void GridView1_RowUpdating(object sender, GridViewUpdateEventArgs e)
        {

                //GridView1.EditIndex = e.RowIndex;
                //DataKey dk = (GridView1.DataKeys[e.RowIndex]);

                //string strTemp = ((TextBox)GridView1.Rows[e.RowIndex].FindControl("String_TW")).Text;
                //GridView1.Rows[e.RowIndex].Cells[2].Text = "임시확인";
                //GridView1.DataBind();

                //GridView1.DataBind();
                GridView1.SelectedIndex = e.RowIndex;

                int nID = -1;
                for (int i = 0; i < m_tblTransKR.Rows.Count; i++)
                {
                    if (m_tblTransKR.Rows[i].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    if (m_tblTransKR.Rows[i]["Index"].ToString() == GridView1.Rows[e.RowIndex].Cells[0].Text)
                    {
                        if (m_tblTransKR.Rows[i]["String"].ToString() == GridView1.Rows[e.RowIndex].Cells[2].Text)
                        {
                            nID = i;
                            break;
                        }
                        else if (m_tblTransKR.Rows[i]["String"].ToString() == "" && GridView1.Rows[e.RowIndex].Cells[2].Text == "&nbsp;")
                        {
                            nID = i;
                            break;
                        }

                    }                    
                }

                for (int i = 0; i < m_tblTransForeign.Rows.Count; i++)
                {
                    if (m_tblTransForeign.Rows[i].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }

                    if (m_tblTransForeign.Rows[i]["Index"].ToString() == m_tblTransKR.Rows[nID]["Index"].ToString() &&
                        m_tblTransForeign.Rows[i]["TableName"].ToString() == m_tblTransKR.Rows[nID]["TableName"].ToString() &&
                        m_tblTransForeign.Rows[i]["Column"].ToString() == m_tblTransKR.Rows[nID]["Column"].ToString())
                    {
                        if (CheckBox1.Checked)
                        {
                            m_tblTransKR.Rows[nID]["Memo"] = ((TextBox)GridView1.Rows[e.RowIndex].FindControl("Memo")).Text;
                            Adpt_Trans.Update(m_tblTransKR.Rows[i]);
                            continue;
                        }

                        m_tblTransForeign.Rows[i]["String"] = ((TextBox)GridView1.Rows[e.RowIndex].FindControl("String_Foreign")).Text;
                        //m_tblTransForeign.Rows[i]["Memo"] = ((TextBox)GridView1.Rows[e.RowIndex].FindControl("Memo")).Text;
                        m_tblTransForeign.Rows[i]["Last Update Date"] = DateTime.Now;
                        Adpt_Trans.Update(m_tblTransForeign.Rows[i]);
                        Adpt_Trans_History.InsertTransHistory(Convert.ToInt32(m_tblTransForeign.Rows[i]["TransIDX"].ToString()),
                            m_tblTransForeign.Rows[i]["CountryCode"].ToString(), m_tblTransForeign.Rows[i]["TableName"].ToString(),
                            m_tblTransForeign.Rows[i]["Column"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()),
                            m_tblTransForeign.Rows[i]["String"].ToString(), Request.QueryString["ID"].ToString(), strIP, 
                            Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()));

                        if (strCountryCode == "TW")
                        {
                            switch (m_tblTransForeign.Rows[i]["TableName"].ToString())
                            {
                                case "Item":

                                    ScriptData_TWDataSetTableAdapters.ItemTableAdapter Adpt_Item = new ScriptData_TWDataSetTableAdapters.ItemTableAdapter();


                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Name")
                                    {
                                        Adpt_Item.UpdateItem_Name(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_Item.UpdateItem_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description In Shop")
                                    {
                                        Adpt_Item.UpdateItem_DescriptionInShop(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SetItem":
                                    ScriptData_TWDataSetTableAdapters.SetItemTableAdapter Adpt_SetItem = new ScriptData_TWDataSetTableAdapters.SetItemTableAdapter();
                                    Adpt_SetItem.UpdateSetItem_SetName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SetItem.UpdateSetItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "QuestTable":
                                    ScriptData_TWDataSetTableAdapters.QuestTableTableAdapter Adpt_QuestTable = new ScriptData_TWDataSetTableAdapters.QuestTableTableAdapter();

                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_Title(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Main Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_MainText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Thanks Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_ThanksText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Diss Clear Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_DissClearText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SubQuestTable":
                                    ScriptData_TWDataSetTableAdapters.SubQuestTableTableAdapter Adpt_SubQuestTable = new ScriptData_TWDataSetTableAdapters.SubQuestTableTableAdapter();
                                    Adpt_SubQuestTable.UpdateSubQuestTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SubQuestTable.UpdateSubQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "TitleTable":
                                    ScriptData_TWDataSetTableAdapters.TitleTableTableAdapter Adpt_TitleTable = new ScriptData_TWDataSetTableAdapters.TitleTableTableAdapter();

                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title_Name")
                                    {
                                        Adpt_TitleTable.UpdateTitleTable_TitleName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_TitleTable.UpdateTitleTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_TitleTable.UpdateTitleTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_TitleTable.UpdateTitleTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "TitleMission":
                                    ScriptData_TWDataSetTableAdapters.TitleMissionTableAdapter Adpt_TitleMission = new ScriptData_TWDataSetTableAdapters.TitleMissionTableAdapter();

                                    Adpt_TitleMission.UpdateTitleMission_MissionName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_TitleMission.UpdateTitleMission_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "SubTitleMission":
                                    ScriptData_TWDataSetTableAdapters.SubTitleMissionTableAdapter Adpt_SubTitleMission = new ScriptData_TWDataSetTableAdapters.SubTitleMissionTableAdapter();

                                    Adpt_SubTitleMission.UpdateSubTitleMission_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SubTitleMission.UpdateSubTitleMission_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "StringTable":
                                    ScriptData_TWDataSetTableAdapters.StringTableTableAdapter Adpt_StringTable = new ScriptData_TWDataSetTableAdapters.StringTableTableAdapter();

                                    Adpt_StringTable.UpdateStringTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_StringTable.UpdateStringTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "NetErrorTable":
                                    ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorTable = new ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter();

                                    Adpt_NetErrorTable.UpdateNetErrorTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_NetErrorTable.UpdateNetErrorTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "TextureStringManager":
                                    ScriptData_TWDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManager = new ScriptData_TWDataSetTableAdapters.TextureStringManagerTableAdapter();

                                    Adpt_TextureStringManager.UpdateTextureStringManager(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_TextureStringManager.UpdateTextureStringManager_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;
                                case "SkillTable":
                                    ScriptData_TWDataSetTableAdapters.SkillTableTableAdapter Adpt_SkillTable = new ScriptData_TWDataSetTableAdapters.SkillTableTableAdapter();
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "DESC")
                                    {
                                        Adpt_SkillTable.UpdateSkillTable_DESC(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_SkillTable.UpdateSkillTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SkillEnum":
                                    ScriptData_TWDataSetTableAdapters.SkillEnumTableAdapter Adpt_SkillEnum = new ScriptData_TWDataSetTableAdapters.SkillEnumTableAdapter();
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_SkillEnum.UpdateSkillEnum_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_SkillEnum.UpdateSkillEnum_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                            }
                        }
                        else if (strCountryCode == "HK")
                        {
                            switch (m_tblTransForeign.Rows[i]["TableName"].ToString())
                            {
                                case "Item":

                                    ScriptData_HKDataSetTableAdapters.ItemTableAdapter Adpt_Item = new ScriptData_HKDataSetTableAdapters.ItemTableAdapter();


                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Name")
                                    {
                                        Adpt_Item.UpdateItem_Name(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_Item.UpdateItem_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description In Shop")
                                    {
                                        Adpt_Item.UpdateItem_DescriptionInShop(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SetItem":
                                    ScriptData_HKDataSetTableAdapters.SetItemTableAdapter Adpt_SetItem = new ScriptData_HKDataSetTableAdapters.SetItemTableAdapter();
                                    Adpt_SetItem.UpdateSetItem_SetName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SetItem.UpdateSetItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "QuestTable":
                                    ScriptData_HKDataSetTableAdapters.QuestTableTableAdapter Adpt_QuestTable = new ScriptData_HKDataSetTableAdapters.QuestTableTableAdapter();

                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_Title(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Main Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_MainText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Thanks Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_ThanksText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Diss Clear Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_DissClearText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SubQuestTable":
                                    ScriptData_HKDataSetTableAdapters.SubQuestTableTableAdapter Adpt_SubQuestTable = new ScriptData_HKDataSetTableAdapters.SubQuestTableTableAdapter();
                                    Adpt_SubQuestTable.UpdateSubQuestTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SubQuestTable.UpdateSubQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "TitleTable":
                                    ScriptData_HKDataSetTableAdapters.TitleTableTableAdapter Adpt_TitleTable = new ScriptData_HKDataSetTableAdapters.TitleTableTableAdapter();

                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title_Name")
                                    {
                                        Adpt_TitleTable.UpdateTitleTable_TitleName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_TitleTable.UpdateTitleTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_TitleTable.UpdateTitleTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_TitleTable.UpdateTitleTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "TitleMission":
                                    ScriptData_HKDataSetTableAdapters.TitleMissionTableAdapter Adpt_TitleMission = new ScriptData_HKDataSetTableAdapters.TitleMissionTableAdapter();

                                    Adpt_TitleMission.UpdateTitleMission_MissionName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_TitleMission.UpdateTitleMission_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "SubTitleMission":
                                    ScriptData_HKDataSetTableAdapters.SubTitleMissionTableAdapter Adpt_SubTitleMission = new ScriptData_HKDataSetTableAdapters.SubTitleMissionTableAdapter();

                                    Adpt_SubTitleMission.UpdateSubTitleMission_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SubTitleMission.UpdateSubTitleMission_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "StringTable":
                                    ScriptData_HKDataSetTableAdapters.StringTableTableAdapter Adpt_StringTable = new ScriptData_HKDataSetTableAdapters.StringTableTableAdapter();

                                    Adpt_StringTable.UpdateStringTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_StringTable.UpdateStringTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "NetErrorTable":
                                    ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorTable = new ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter();

                                    Adpt_NetErrorTable.UpdateNetErrorTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_NetErrorTable.UpdateNetErrorTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "TextureStringManager":
                                    ScriptData_HKDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManager = new ScriptData_HKDataSetTableAdapters.TextureStringManagerTableAdapter();

                                    Adpt_TextureStringManager.UpdateTextureStringManager(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_TextureStringManager.UpdateTextureStringManager_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;
                                case "SkillTable":
                                    ScriptData_HKDataSetTableAdapters.SkillTableTableAdapter Adpt_SkillTable = new ScriptData_HKDataSetTableAdapters.SkillTableTableAdapter();
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "DESC")
                                    {
                                        Adpt_SkillTable.UpdateSkillTable_DESC(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_SkillTable.UpdateSkillTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SkillEnum":
                                    ScriptData_HKDataSetTableAdapters.SkillEnumTableAdapter Adpt_SkillEnum = new ScriptData_HKDataSetTableAdapters.SkillEnumTableAdapter();
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_SkillEnum.UpdateSkillEnum_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_SkillEnum.UpdateSkillEnum_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                            }
                        }
                        else if (strCountryCode == "JP")
                        {
                            switch (m_tblTransForeign.Rows[i]["TableName"].ToString())
                            {
                                case "Item":

                                    ScriptData_JPDataSetTableAdapters.ItemTableAdapter Adpt_Item = new ScriptData_JPDataSetTableAdapters.ItemTableAdapter();


                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Name")
                                    {
                                        Adpt_Item.UpdateItem_Name(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_Item.UpdateItem_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description In Shop")
                                    {
                                        Adpt_Item.UpdateItem_DescriptionInShop(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_Item.UpdateItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SetItem":
                                    ScriptData_JPDataSetTableAdapters.SetItemTableAdapter Adpt_SetItem = new ScriptData_JPDataSetTableAdapters.SetItemTableAdapter();
                                    Adpt_SetItem.UpdateSetItem_SetName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SetItem.UpdateSetItem_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "QuestTable":
                                    ScriptData_JPDataSetTableAdapters.QuestTableTableAdapter Adpt_QuestTable = new ScriptData_JPDataSetTableAdapters.QuestTableTableAdapter();

                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_Title(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Main Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_MainText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Thanks Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_ThanksText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Diss Clear Text")
                                    {
                                        Adpt_QuestTable.UpdateQuestTable_DissClearText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_QuestTable.UpdateQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SubQuestTable":
                                    ScriptData_JPDataSetTableAdapters.SubQuestTableTableAdapter Adpt_SubQuestTable = new ScriptData_JPDataSetTableAdapters.SubQuestTableTableAdapter();
                                    Adpt_SubQuestTable.UpdateSubQuestTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SubQuestTable.UpdateSubQuestTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "TitleTable":
                                    ScriptData_JPDataSetTableAdapters.TitleTableTableAdapter Adpt_TitleTable = new ScriptData_JPDataSetTableAdapters.TitleTableTableAdapter();

                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title_Name")
                                    {
                                        Adpt_TitleTable.UpdateTitleTable_TitleName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_TitleTable.UpdateTitleTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_TitleTable.UpdateTitleTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_TitleTable.UpdateTitleTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "TitleMission":
                                    ScriptData_JPDataSetTableAdapters.TitleMissionTableAdapter Adpt_TitleMission = new ScriptData_JPDataSetTableAdapters.TitleMissionTableAdapter();

                                    Adpt_TitleMission.UpdateTitleMission_MissionName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_TitleMission.UpdateTitleMission_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                    break;
                                case "SubTitleMission":
                                    ScriptData_JPDataSetTableAdapters.SubTitleMissionTableAdapter Adpt_SubTitleMission = new ScriptData_JPDataSetTableAdapters.SubTitleMissionTableAdapter();

                                    Adpt_SubTitleMission.UpdateSubTitleMission_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_SubTitleMission.UpdateSubTitleMission_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "StringTable":
                                    ScriptData_JPDataSetTableAdapters.StringTableTableAdapter Adpt_StringTable = new ScriptData_JPDataSetTableAdapters.StringTableTableAdapter();

                                    Adpt_StringTable.UpdateStringTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_StringTable.UpdateStringTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "NetErrorTable":
                                    ScriptData_JPDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorTable = new ScriptData_JPDataSetTableAdapters.NetErrorTableTableAdapter();

                                    Adpt_NetErrorTable.UpdateNetErrorTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_NetErrorTable.UpdateNetErrorTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;

                                case "TextureStringManager":
                                    ScriptData_JPDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManager = new ScriptData_JPDataSetTableAdapters.TextureStringManagerTableAdapter();

                                    Adpt_TextureStringManager.UpdateTextureStringManager(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    Adpt_TextureStringManager.UpdateTextureStringManager_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    break;
                                case "SkillTable":
                                    ScriptData_JPDataSetTableAdapters.SkillTableTableAdapter Adpt_SkillTable = new ScriptData_JPDataSetTableAdapters.SkillTableTableAdapter();
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "DESC")
                                    {
                                        Adpt_SkillTable.UpdateSkillTable_DESC(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_SkillTable.UpdateSkillTable_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                                case "SkillEnum":
                                    ScriptData_JPDataSetTableAdapters.SkillEnumTableAdapter Adpt_SkillEnum = new ScriptData_JPDataSetTableAdapters.SkillEnumTableAdapter();
                                    if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                    {
                                        Adpt_SkillEnum.UpdateSkillEnum_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                        Adpt_SkillEnum.UpdateSkillEnum_Regdate(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                    }
                                    break;
                            }
                        }

                        break;
                    }
                }

            //수정안한목록만 보는 것에서 수정하면 즉시 목록에서 빼주기 위한것
            //Page_Load를 불러봤지만 반응하지 않아 이렇게 직접 복사해서 씀
            if(RadioButton12.Checked)
            {
                m_tblTransKR = Adpt_Trans.GetDataByCountry("KR");
                m_tblTransForeign = Adpt_Trans.GetDataByCountry(strCountryCode);

                for(int i = 0; i < m_tblTransKR.Rows.Count; i++)
                {
                    int nCompare = Convert.ToDateTime(m_tblTransKR.Rows[i]["Last Update Date"].ToString()).CompareTo(Convert.ToDateTime(m_tblTransForeign.Rows[i]["Last Update Date"].ToString()));
                    if(nCompare < 0)
                    {
                        m_tblTransKR.Rows[i].Delete();
                        m_tblTransForeign.Rows[i].Delete();
                    }
                }

                int nPageTemp = GridView1.PageIndex;
                
                CurrentTableCreate();
                
                GridView1.DataSource = m_tblTransCurrent;
                GridView1.DataBind();

                GridView1.PageIndex = nPageTemp;
                Label1.Text = "/" + GridView1.PageCount;
                TextPage.Text = Convert.ToString(GridView1.PageIndex + 1);                
            }
        }

        protected void GridView1_RowEditing(object sender, GridViewEditEventArgs e)
        {
            //GridView1.DataBind();
            //string strTemp = ((TextBox)GridView1.Rows[e.NewEditIndex].Cells[3].Controls[1]).Text;
            
        }

        protected void GridView1_RowCommand(object sender, GridViewCommandEventArgs e)
        {
            //string strTemp = e.CommandName;
        }

        protected void GridView1_RowUpdated(object sender, GridViewUpdatedEventArgs e)
        {           
            
        }

        protected void btnAllUpdate_Click(object sender, EventArgs e)
        {            
            //고친거 있는지 없는지 확인한 후 없을경우 업데이트 안하게 수정할 것
        
/*            int nID = -1;
            for (int i = 0; i < m_tblTransKR.Rows.Count; i++)
            {
                if (m_tblTransKR.Rows[i]["TransIDX"].ToString() == GridView1.Rows[0].Cells[0].Text)
                {
                    nID = i;
                    break;
                }
            }

            int nCount = 0;
            bool bStart = false;

            DataRow[] drTemp = new DataRow[m_tblTransForeign.Rows.Count];
            for (int i = 0; i < m_tblTransForeign.Rows.Count; i++)
            {
                if (m_tblTransForeign.Rows[i]["Index"].ToString() == m_tblTransKR.Rows[nID]["Index"].ToString())
                {
                    bStart = true;
                }
                if(bStart)
                {
                    if (CheckBox1.Checked)
                    {
                        m_tblTransKR.Rows[i]["Memo"] = ((TextBox)GridView1.Rows[nCount].FindControl("Memo")).Text;
                        Adpt_Trans.Update(m_tblTransKR.Rows[i]);

                        nCount++;
                        if (nCount >= 50)
                            break;
                        continue;
                    }

                    m_tblTransForeign.Rows[i]["String"] = ((TextBox)GridView1.Rows[nCount].FindControl("String_Foreign" )).Text;
                    m_tblTransForeign.Rows[i]["Last Update Date"] = DateTime.Now;
                    drTemp[i] = m_tblTransForeign.Rows[i];

                    if (strCountryCode == "TW")
                    {
                        switch (m_tblTransForeign.Rows[i]["TableName"].ToString())
                        {
                            case "Item":

                                ScriptData_TWDataSetTableAdapters.ItemTableAdapter Adpt_Item = new ScriptData_TWDataSetTableAdapters.ItemTableAdapter();


                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Name")
                                {
                                    Adpt_Item.UpdateItem_Name(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                {
                                    Adpt_Item.UpdateItem_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description In Shop")
                                {
                                    Adpt_Item.UpdateItem_DescriptionInShop(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "SetItem":
                                ScriptData_TWDataSetTableAdapters.SetItemTableAdapter Adpt_SetItem = new ScriptData_TWDataSetTableAdapters.SetItemTableAdapter();
                                Adpt_SetItem.UpdateSetItem_SetName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                break;
                            case "QuestTable":
                                ScriptData_TWDataSetTableAdapters.QuestTableTableAdapter Adpt_QuestTable = new ScriptData_TWDataSetTableAdapters.QuestTableTableAdapter();

                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_Title(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Main Text")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_MainText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Thanks Text")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_ThanksText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Diss Clear Text")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_DissClearText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "SubQuestTable":
                                ScriptData_TWDataSetTableAdapters.SubQuestTableTableAdapter Adpt_SubQuestTable = new ScriptData_TWDataSetTableAdapters.SubQuestTableTableAdapter();
                                Adpt_SubQuestTable.UpdateSubQuestTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                break;
                            case "TitleTable":
                                ScriptData_TWDataSetTableAdapters.TitleTableTableAdapter Adpt_TitleTable = new ScriptData_TWDataSetTableAdapters.TitleTableTableAdapter();

                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title_Name")
                                {
                                    Adpt_TitleTable.UpdateTitleTable_TitleName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                {
                                    Adpt_TitleTable.UpdateTitleTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "TitleMission":
                                ScriptData_TWDataSetTableAdapters.TitleMissionTableAdapter Adpt_TitleMission = new ScriptData_TWDataSetTableAdapters.TitleMissionTableAdapter();

                                Adpt_TitleMission.UpdateTitleMission_MissionName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                break;
                            case "SubTitleMission":
                                ScriptData_TWDataSetTableAdapters.SubTitleMissionTableAdapter Adpt_SubTitleMission = new ScriptData_TWDataSetTableAdapters.SubTitleMissionTableAdapter();

                                Adpt_SubTitleMission.UpdateSubTitleMission_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;

                            case "StringTable":
                                ScriptData_TWDataSetTableAdapters.StringTableTableAdapter Adpt_StringTable = new ScriptData_TWDataSetTableAdapters.StringTableTableAdapter();

                                Adpt_StringTable.UpdateStringTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;

                            case "NetErrorTable":
                                ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorTable = new ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter();

                                Adpt_NetErrorTable.UpdateNetErrorTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;

                            case "TextureStringManager":
                                ScriptData_TWDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManager = new ScriptData_TWDataSetTableAdapters.TextureStringManagerTableAdapter();

                                Adpt_TextureStringManager.UpdateTextureStringManager(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;
                            case "SkillTable":
                                ScriptData_TWDataSetTableAdapters.SkillTableTableAdapter Adpt_SkillTable = new ScriptData_TWDataSetTableAdapters.SkillTableTableAdapter();
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "DESC")
                                {
                                    Adpt_SkillTable.UpdateSkillTable_DESC(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "SkillEnum":
                                ScriptData_TWDataSetTableAdapters.SkillEnumTableAdapter Adpt_SkillEnum = new ScriptData_TWDataSetTableAdapters.SkillEnumTableAdapter();
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                {
                                    Adpt_SkillEnum.UpdateSkillEnum_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                        }
                    }
                    else if (strCountryCode == "HK")
                    {
                        switch (m_tblTransForeign.Rows[i]["TableName"].ToString())
                        {
                            case "Item":

                                ScriptData_HKDataSetTableAdapters.ItemTableAdapter Adpt_Item = new ScriptData_HKDataSetTableAdapters.ItemTableAdapter();


                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Name")
                                {
                                    Adpt_Item.UpdateItem_Name(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                {
                                    Adpt_Item.UpdateItem_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description In Shop")
                                {
                                    Adpt_Item.UpdateItem_DescriptionInShop(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "SetItem":
                                ScriptData_HKDataSetTableAdapters.SetItemTableAdapter Adpt_SetItem = new ScriptData_HKDataSetTableAdapters.SetItemTableAdapter();
                                Adpt_SetItem.UpdateSetItem_SetName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                break;
                            case "QuestTable":
                                ScriptData_HKDataSetTableAdapters.QuestTableTableAdapter Adpt_QuestTable = new ScriptData_HKDataSetTableAdapters.QuestTableTableAdapter();

                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_Title(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Main Text")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_MainText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Thanks Text")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_ThanksText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Diss Clear Text")
                                {
                                    Adpt_QuestTable.UpdateQuestTable_DissClearText(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "SubQuestTable":
                                ScriptData_HKDataSetTableAdapters.SubQuestTableTableAdapter Adpt_SubQuestTable = new ScriptData_HKDataSetTableAdapters.SubQuestTableTableAdapter();
                                Adpt_SubQuestTable.UpdateSubQuestTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                break;
                            case "TitleTable":
                                ScriptData_HKDataSetTableAdapters.TitleTableTableAdapter Adpt_TitleTable = new ScriptData_HKDataSetTableAdapters.TitleTableTableAdapter();

                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Title_Name")
                                {
                                    Adpt_TitleTable.UpdateTitleTable_TitleName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                {
                                    Adpt_TitleTable.UpdateTitleTable_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "TitleMission":
                                ScriptData_HKDataSetTableAdapters.TitleMissionTableAdapter Adpt_TitleMission = new ScriptData_HKDataSetTableAdapters.TitleMissionTableAdapter();

                                Adpt_TitleMission.UpdateTitleMission_MissionName(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));

                                break;
                            case "SubTitleMission":
                                ScriptData_HKDataSetTableAdapters.SubTitleMissionTableAdapter Adpt_SubTitleMission = new ScriptData_HKDataSetTableAdapters.SubTitleMissionTableAdapter();

                                Adpt_SubTitleMission.UpdateSubTitleMission_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;

                            case "StringTable":
                                ScriptData_HKDataSetTableAdapters.StringTableTableAdapter Adpt_StringTable = new ScriptData_HKDataSetTableAdapters.StringTableTableAdapter();

                                Adpt_StringTable.UpdateStringTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt16(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;

                            case "NetErrorTable":
                                ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorTable = new ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter();

                                Adpt_NetErrorTable.UpdateNetErrorTable(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;

                            case "TextureStringManager":
                                ScriptData_HKDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManager = new ScriptData_HKDataSetTableAdapters.TextureStringManagerTableAdapter();

                                Adpt_TextureStringManager.UpdateTextureStringManager(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                break;
                            case "SkillTable":
                                ScriptData_HKDataSetTableAdapters.SkillTableTableAdapter Adpt_SkillTable = new ScriptData_HKDataSetTableAdapters.SkillTableTableAdapter();
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "DESC")
                                {
                                    Adpt_SkillTable.UpdateSkillTable_DESC(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                            case "SkillEnum":
                                ScriptData_HKDataSetTableAdapters.SkillEnumTableAdapter Adpt_SkillEnum = new ScriptData_HKDataSetTableAdapters.SkillEnumTableAdapter();
                                if (m_tblTransForeign.Rows[i]["Column"].ToString() == "Description")
                                {
                                    Adpt_SkillEnum.UpdateSkillEnum_Description(m_tblTransForeign.Rows[i]["String"].ToString(), Convert.ToInt32(m_tblTransForeign.Rows[i]["Index"].ToString()));
                                }
                                break;
                        }
                    }

                    nCount++;
                    if (nCount >= 50)
                        break;
                }
            }
            //LocalizationDataSet.TransDataTable tblCurrent;
            //m_tblTransForeign;
            if (!CheckBox1.Checked)
            {
                Adpt_Trans.Update(drTemp);

                GridView1.DataSource = m_tblTransCurrent;
                GridView1.DataBind();
                Label1.Text = "/" + GridView1.PageCount;
                TextPage.Text = Convert.ToString(GridView1.PageIndex + 1);
            }*/
        }


        protected void Button1_Click(object sender, EventArgs e)
        {
            GridView1.PageIndex = 0;
            CurrentTableCreate();

            GridView1.DataSource = m_tblTransCurrent;
            GridView1.DataBind();
            Label1.Text = "/" + GridView1.PageCount;
            TextPage.Text = Convert.ToString(GridView1.PageIndex + 1);
            m_bSearch = false;
        }

        protected void btnMove_Click(object sender, EventArgs e)
        {
            if (Convert.ToInt32(TextPage.Text) <= GridView1.PageCount && 0 < Convert.ToInt32(TextPage.Text))
            {
                GridView1.PageIndex = Convert.ToInt32(TextPage.Text) - 1;

                GridView1.DataSource = m_tblTransCurrent;
                GridView1.DataBind();
                Label1.Text = "/" + GridView1.PageCount;
                TextPage.Text = Convert.ToString(GridView1.PageIndex + 1);
            }

        }

        protected void btnSearch_Click(object sender, EventArgs e)
        {

            if (RadioButton15.Checked)
            {
                for (int i = 0; i < m_tblTransKR.Rows.Count; i++)
                {
                    if (m_tblTransKR.Rows[i].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    if (m_tblTransKR.Rows[i]["String"].ToString().IndexOf(TextSearch.Text) == -1)
                    {
                        m_tblTransKR.Rows[i].Delete();
                        m_tblTransForeign.Rows[i].Delete();
                    }
                }
            }
            else if (RadioButton16.Checked)
            {
                for (int i = 0; i < m_tblTransForeign.Rows.Count; i++)
                {
                    if (m_tblTransForeign.Rows[i].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    if (m_tblTransForeign.Rows[i]["String"].ToString().IndexOf(TextSearch.Text) == -1)
                    {
                        m_tblTransKR.Rows[i].Delete();
                        m_tblTransForeign.Rows[i].Delete();
                    }
                }
            }

            CurrentTableCreate();
            GridView1.DataSource = m_tblTransCurrent;
            GridView1.DataBind();
            Label1.Text = "/" + GridView1.PageCount;
            //  m_bSearch = false;
            m_bSearch = true;
            TextPage.Text = Convert.ToString(GridView1.PageIndex + 1);
            
        }



    }
}
