//09. 06. 24 김정협 나라별 define
// ItemTable.cs, QuestTable.cs, TitleTable.cs 세군대 정의를
// 모두 바꿔줘야 함.
#define __COUNTRY_KR__
//#define __COUNTRY_TW__
//#define __COUNTRY_HK__
//#define __COUNTRY_JP__

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.IO;
using System.Net;
using System.Collections;
using DBManager.ScriptData_TWDataSetTableAdapters;
using DBManager.ScriptData_HKDataSetTableAdapters;
using DBManager.LocalizationDataSetTableAdapters;
using System.Diagnostics;

namespace DBManager
{    
    public partial class NetErrorTable : Form
    {
        private SqlConnection Con_KR;
        private SqlDataAdapter Adpt_KR;
        DataTable tblNetErrorKR;

        private SqlConnection Con;
        private SqlDataAdapter Adpt;
        DataTable tblNetError;
        object objTemp;
        

        //History용        
        private SqlDataAdapter Adpt_History;
        DataTable tblNetError_History;

        //DeleteHistory용        
        private SqlDataAdapter Adpt_DeleteHistory;
        DataTable tblNetError_DeleteHistory;

        string m_strID;        
        ArrayList m_ModifyIDList = new ArrayList();

        ArrayList m_DeleteRowList = new ArrayList();

        public bool m_bSaveEnd = true; //수정했을 시 저장한 후 끝내는지 확인하는 변수


        ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorTW = new ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter();
        ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorHK = new ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter();
        ScriptData_JPDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorJP = new ScriptData_JPDataSetTableAdapters.NetErrorTableTableAdapter();
        TransTableAdapter Adpt_Trans = new TransTableAdapter();

                               
        public NetErrorTable()
        {
            InitializeComponent();            
        }

        public void SetID(string strID)
        {
            m_strID = strID;
        }

        void StringSave()
        {
            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                for (int j = 0; j < dataGridView1.RowCount; j++)
                {
                    dataGridView1[i, j].Style.ForeColor = Color.Black;                    
                }
            }

            //History 추가
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            string strMyIP = host.AddressList[0].ToString();
            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                tblNetError_History.Rows.Add();
                DataRow Row_History = tblNetError_History.Rows[tblNetError_History.Rows.Count - 1];
                DataRow Row = null;

                for (int nRow = 0; nRow < tblNetError.Rows.Count; nRow++)
                {
                    if (tblNetError.Rows[nRow].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    if (tblNetError.Rows[nRow]["ID"].ToString() == m_ModifyIDList[i].ToString())
                    {
                        Row = tblNetError.Rows[nRow];
                    }
                }


                Row_History["ID"] = Row["ID"];
                Row_History["Enum"] = Row["Enum"];
                Row_History["String"] = Row["String"];
                Row_History["PreProcessor"] = Row["PreProcessor"];
                Row_History["UserID"] = m_strID;
                Row_History["UserIP"] = strMyIP;
                Row_History["Regdate"] = Row["Regdate"];


                Adpt.Update(new DataRow[] { Row });
                Adpt_History.Update(new DataRow[] { Row_History });


#if __COUNTRY_KR__


                int nRowIndex = -1;
                
                DataTable tblErrorTW = Adpt_NetErrorTW.GetData();
                
                for (int twRow = 0; twRow < tblErrorTW.Rows.Count; twRow++)
                {
                    if (Row["ID"].ToString() == tblErrorTW.Rows[twRow]["ID"].ToString())
                    {
                        nRowIndex = twRow;
                        break;
                    }
                }

                if (nRowIndex != -1)
                {
                    Adpt_Trans.UpdateNetErrorTable(Row["String"].ToString(), Convert.ToInt32(Row["ID"].ToString()), "KR");
                }
                else
                {
                    //tblNetError.Clear();
                    //Adpt.Fill(tblNetError);
                    //추가된 것이므로 무조건 제일 밑


                    Adpt_Trans.InsertTrans("KR", "ScriptData", "NetErrorTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("TW", "ScriptData_TW", "NetErrorTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("HK", "ScriptData_HK", "NetErrorTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("JP", "ScriptData_JP", "NetErrorTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_NetErrorTW.InsertNetErrorTable(Row["Enum"].ToString(), Row["String"].ToString(), Row["PreProcessor"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_NetErrorHK.InsertNetErrorTable(Row["Enum"].ToString(), Row["String"].ToString(), Row["PreProcessor"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_NetErrorJP.InsertNetErrorTable(Row["Enum"].ToString(), Row["String"].ToString(), Row["PreProcessor"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()));

                }   



            }

            for (int i = 0; i < m_DeleteRowList.Count; i++)
            {
                tblNetError_DeleteHistory.Rows.Add();

                DataRow Row_History = tblNetError_DeleteHistory.Rows[tblNetError_DeleteHistory.Rows.Count - 1];

                DataRow Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                //09. 06. 22 히스토리 입력부분 간소화
                for (int Column = 0; Column < tblNetError.Columns.Count - 1; Column++)
                {
                    Row_History[Column] = Row[Column];
                }
                Row_History["UserID"] = m_strID;
                Row_History["UserIP"] = strMyIP;
                //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                Row_History["Regdate"] = DateTime.Now;

                Adpt.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                Adpt_DeleteHistory.Update(new DataRow[] { Row_History });

                ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorTW = new ScriptData_TWDataSetTableAdapters.NetErrorTableTableAdapter();
                ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorHK = new ScriptData_HKDataSetTableAdapters.NetErrorTableTableAdapter();
                ScriptData_JPDataSetTableAdapters.NetErrorTableTableAdapter Adpt_NetErrorJP = new ScriptData_JPDataSetTableAdapters.NetErrorTableTableAdapter();
                TransTableAdapter Adpt_Trans = new TransTableAdapter();

                Adpt_NetErrorTW.DeleteNetErrorTable(Convert.ToInt32(Row["ID"].ToString()));
                Adpt_NetErrorHK.DeleteNetErrorTable(Convert.ToInt32(Row["ID"].ToString()));
                Adpt_NetErrorJP.DeleteNetErrorTable(Convert.ToInt32(Row["ID"].ToString()));
                Adpt_Trans.DeleteTrans("NetErrorTable", Convert.ToInt32(Row["ID"].ToString()));
            }


            tblNetError.Clear();
            Adpt.Fill(tblNetError);

            tblNetError_History.Clear();
            Adpt_History.Fill(tblNetError_History);

            tblNetError_DeleteHistory.Clear();
            Adpt_DeleteHistory.Fill(tblNetError_DeleteHistory);

#elif __COUNTRY_TW__
                Adpt_Trans.UpdateNetErrorTable(Row["String"].ToString(), Convert.ToInt32(Row["ID"].ToString()), "TW");
            }
#elif __COUNTRY_HK__
                Adpt_Trans.UpdateNetErrorTable(Row["String"].ToString(), Convert.ToInt32(Row["ID"].ToString()), "HK");
            }
#elif __COUNTRY_JP__
                Adpt_Trans.UpdateNetErrorTable(Row["String"].ToString(), Convert.ToInt32(Row["ID"].ToString()), "JP");
            }

#endif


            dataGridView1.CurrentCell = dataGridView1[0, dataGridView1.RowCount - 1];
            //tblNetError_History.Rows[tblNetError_History.Rows.Count - 1]
            //DataRow Row = tblNetError.Rows[Convert.ToInt32(m_ModifyIDList[0].ToString())];
            m_ModifyIDList.Clear();
            m_bSaveEnd = true;
        }

        private void StringTable_Load(object sender, EventArgs e)
        {
            Con_KR = new SqlConnection();
            Con_KR.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";

            Adpt_KR = new SqlDataAdapter("SELECT * FROM NetErrorTable", Con_KR);
            tblNetErrorKR = new DataTable("tblNetErrorKR");
            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt_KR);
            Adpt_KR.Fill(tblNetErrorKR);


            Con = new SqlConnection();            
#if __COUNTRY_KR__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";
            Add.Enabled = true;
#elif __COUNTRY_TW__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_TW; uid=Script_User; pwd=x2_tmzmflqxm";
            Add.Enabled = false;
#elif __COUNTRY_HK__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_HK; uid=Script_User; pwd=x2_tmzmflqxm";
            Add.Enabled = false;
#elif __COUNTRY_JP__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_JP; uid=Script_User; pwd=x2_tmzmflqxm";
            Add.Enabled = false;
#endif
            Con.Open();

            Adpt = new SqlDataAdapter("SELECT * FROM NetErrorTable", Con);
            tblNetError = new DataTable("tblNetError");

            Builder = new SqlCommandBuilder(Adpt);
            Adpt.Fill(tblNetError);
            dataGridView1.DataSource = tblNetError;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView1.Columns[4].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["ID"].ReadOnly = true;            
            dataGridView1.CurrentCell = dataGridView1[0, dataGridView1.RowCount - 1];

            //History
            Adpt_History = new SqlDataAdapter("SELECT * FROM NetErrorTable_History", Con);
            tblNetError_History = new DataTable("tblNetError_History");

            SqlCommandBuilder Builder_History = new SqlCommandBuilder(Adpt_History);
            Adpt_History.Fill(tblNetError_History);


            //DeleteHistory용        
            Adpt_DeleteHistory = new SqlDataAdapter("SELECT * FROM NetErrorTable_DeleteHistory", Con);
            tblNetError_DeleteHistory = new DataTable("tblNetError_DeleteHistory");
            SqlCommandBuilder Builder_DeleteHistory = new SqlCommandBuilder(Adpt_DeleteHistory);
            Adpt_DeleteHistory.Fill(tblNetError_DeleteHistory);



            this.Width = 1000;
            this.Height = 600;
            StringTable_Resize(null, null);
        }
        
        private void StringTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Con.Close();            
            Application.Exit();
            
        }

        private void Save_Click(object sender, EventArgs e)
        {
            StringSave();            
            MessageBox.Show("저장되었습니다.");
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView1.CurrentCell.Value;            
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView1.CurrentCell.Value))
            {
                string strOldTime = dataGridView1[4, dataGridView1.CurrentCell.RowIndex].Value.ToString();

                dataGridView1.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView1[4, dataGridView1.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView1[4, dataGridView1.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    if (dataGridView1[3, i].Value.ToString() == dataGridView1.CurrentCell.Value.ToString())
                    {
                        if (i == dataGridView1.CurrentCell.RowIndex)
                        {
                            continue;
                        }
                        
                        DialogResult result = MessageBox.Show("수정한 스트링과 동일한 스트링이 발견 되었습니다. 그래도 이 값으로 수정하시겠습니까?", "", MessageBoxButtons.OKCancel);
                        if (result == DialogResult.OK)
                        {
                            break;
                        }
                        else
                        {
                            dataGridView1.CurrentCell.Value = objTemp;
                            dataGridView1.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView1[4, dataGridView1.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView1[4, dataGridView1.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;
                        }
                    }
                }
                int nID = Convert.ToInt32(dataGridView1["ID", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(nID);
                m_bSaveEnd = false;
            }
        }

        private void Add_Click(object sender, EventArgs e)
        {
            //삭제 기능 막음 
            //dataGridView1.Rows.RemoveAt(dataGridView1.CurrentCell.RowIndex);            

            Add frm = new Add();
            frm.SetDataGridView(dataGridView1);
            frm.SetDataTable(tblNetError);
            frm.SetModifyArray(m_ModifyIDList);            
            frm.Owner = this;
            frm.ShowDialog(this);
        }

        private void dataGridView1_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                Search frm = new Search();
                frm.SetDataGridView(dataGridView1);
                frm.ShowDialog(this);
            }       
        }

        private void FileGenerate_Click(object sender, EventArgs e)
        {
            if(m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("파일을 생성하시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.Cancel)
                {
                    return;
                }
            }
            else
            {
                DialogResult result = MessageBox.Show("저장되지 않았습니다. 저장한 후 파일을 생성하시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {/*
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    for (int i = 0; i < m_ModifyIDList.Count; i++)
                    {
                        tblNetError_History.Rows.Add();
                        DataRow Row_History = tblNetError_History.Rows[tblNetError_History.Rows.Count - 1];
                        DataRow Row = tblNetError.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];
                        Row_History["ID"] = Row["ID"];
                        Row_History["Enum"] = Row["Enum"];
                        Row_History["Type"] = Row["Type"];
                        Row_History["String"] = Row["String"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                    }
                    m_ModifyIDList.Clear();
                    m_bSaveEnd = true;
                    Adpt.Update(tblNetError);
                    Adpt_History.Update(tblNetError_History);    */
                    StringSave();
                }
                else if (result == DialogResult.Cancel)
                {
                    MessageBox.Show("저장하지 않으시면 파일을 생성할 수 없습니다.");
                    return;                                      
                }
            }

            string strCountryCode = "";
            StreamWriter sw;            
#if __COUNTRY_KR__
            string strFileName = "..\\..\\..\\source\\KR\\Trunk\\KncWX2Server\\Common\\NetError_def.h";
            strCountryCode = "KR";
            sw = new StreamWriter(strFileName, false, Encoding.Default);
#elif __COUNTRY_TW__
            string strFileName = "..\\..\\..\\source\\TW\\Trunk\\KncWX2Server\\Common\\NetError_def.h";
            strCountryCode = "TW";
            sw = new StreamWriter(strFileName, false, Encoding.UTF8);
#elif __COUNTRY_HK__
            string strFileName = "..\\..\\..\\source\\HK\\Trunk\\KncWX2Server\\Common\\NetError_def.h";
            strCountryCode = "HK";
            sw = new StreamWriter(strFileName, false, Encoding.UTF8);
#elif __COUNTRY_JP__
            string strFileName = "..\\..\\..\\source\\JP\\Trunk\\KncWX2Server\\Common\\NetError_def.h";
            strCountryCode = "JP";
            sw = new StreamWriter(strFileName, false, Encoding.UTF8);
#endif

//#if __COUNTRY_KR__

//            sw.Write("#ifndef NEW_SKILL_TREE\r\n");
//            sw.Write("#define NEW_SKILL_TREE\r\n");
//            sw.Write("#endif\r\n\r\n\r\n\r\n\r\n");

//            sw.Write("#ifdef __NEVER_DEFINED_MACRO__\r\n");
//            sw.Write("#define NET_ERROR(VAL, VAL2) VAL,\r\n");
//            sw.Write("enum\r\n");
//            sw.Write("{\r\n");
//            sw.Write("#endif\r\n\r\n\r\n");

//            bool bPreProcesser = false;

//            for (int i = 0; i < tblNetError.Rows.Count; i++)
//            {
//                if (bPreProcesser && tblNetError.Rows[i]["PreProcessor"].ToString() != tblNetError.Rows[i - 1]["PreProcessor"].ToString())
//                {
//                    sw.Write("\r\n");
//                    sw.Write(tblNetError.Rows[i]["PreProcessor"].ToString());
//                    sw.Write("\r\n\r\n");
//                }

//                if (tblNetError.Rows[i]["PreProcessor"].ToString().Length > 0 && !bPreProcesser)
//                {
//                    sw.Write("\r\n");
//                    sw.Write(tblNetError.Rows[i]["PreProcessor"].ToString());
//                    sw.Write("\r\n");
//                    bPreProcesser = true;
//                }

//                sw.Write("NET_ERROR( ");
//                sw.Write(tblNetError.Rows[i]["Enum"].ToString());
//                sw.Write(", ");
//                sw.Write(tblNetError.Rows[i]["String"].ToString());
//                sw.Write(" )\r\n");
//                if (bPreProcesser && tblNetError.Rows[i + 1]["PreProcessor"].ToString().Length == 0)
//                {
//                    string[] strArrayTemp = tblNetError.Rows[i]["PreProcessor"].ToString().Split(' ');
//                    string strTemp = "#endif " + strArrayTemp[1];
//                    sw.Write(strTemp);
//                    sw.Write("\r\n\r\n");
//                    bPreProcesser = false;
//                }
//            }
//            sw.Write("\r\n");

//            sw.Write("NET_ERROR( ERR_SENTINEL,    ERR_SENTINEL )\r\n\r\n");


//            sw.Write("#ifdef __NEVER_DEFINED_MACRO__\r\n");
//            sw.Write("};\r\n");
//            sw.Write("#undef NET_ERROR\r\n");
//            sw.Write("#endif");

//            sw.Close();
//#else



            sw.Write("#pragma once\r\n");

            sw.Write("#ifndef NEW_SKILL_TREE\r\n");
            sw.Write("#define NEW_SKILL_TREE\r\n");
            sw.Write("#endif\r\n\r\n\r\n\r\n\r\n");

            sw.Write("enum NetErrorEnum\r\n");
            sw.Write("{\r\n");


            bool bPreProcesser = false;

            for (int i = 0; i < tblNetError.Rows.Count; i++)
            {
                if (bPreProcesser && tblNetError.Rows[i]["PreProcessor"].ToString() != tblNetError.Rows[i - 1]["PreProcessor"].ToString())
                {
                    sw.Write("\r\n");
                    sw.Write(tblNetError.Rows[i]["PreProcessor"].ToString());
                    sw.Write("\r\n\r\n");
                }

                if (tblNetError.Rows[i]["PreProcessor"].ToString().Length > 0 && !bPreProcesser)
                {
                    sw.Write("\r\n");
                    sw.Write(tblNetError.Rows[i]["PreProcessor"].ToString());
                    sw.Write("\r\n");
                    bPreProcesser = true;
                }                

                sw.Write("\t");
                sw.Write(tblNetError.Rows[i]["Enum"].ToString());
                sw.Write(",\r\n");
                
                
                if (bPreProcesser && tblNetError.Rows[i + 1]["PreProcessor"].ToString().Length == 0)
                {
                    string[] strArrayTemp = tblNetError.Rows[i]["PreProcessor"].ToString().Split(' ');
                    string strTemp = "#endif " + strArrayTemp[1];
                    sw.Write(strTemp);
                    sw.Write("\r\n\r\n");
                    bPreProcesser = false;
                }
            }
            sw.Write("\r\n");

            sw.Write("\tERR_SENTINEL\r\n};\r\n\r\n");

           

            sw.Write("static wchar_t* szErrorStr[] = {\r\n");

            for (int i = 0; i < tblNetErrorKR.Rows.Count; i++)
            {

                if (bPreProcesser && tblNetErrorKR.Rows[i]["PreProcessor"].ToString() != tblNetErrorKR.Rows[i - 1]["PreProcessor"].ToString())
                {
                    sw.Write("\r\n");
                    sw.Write(tblNetErrorKR.Rows[i]["PreProcessor"].ToString());
                    sw.Write("\r\n\r\n");
                }

                if (tblNetErrorKR.Rows[i]["PreProcessor"].ToString().Length > 0 && !bPreProcesser)
                {
                    sw.Write("\r\n");
                    sw.Write(tblNetErrorKR.Rows[i]["PreProcessor"].ToString());
                    sw.Write("\r\n");
                    bPreProcesser = true;
                }

                sw.Write("\tL\"");
                sw.Write(tblNetErrorKR.Rows[i]["String"].ToString());
                
                sw.Write("\",\r\n");
                
                if (bPreProcesser && tblNetErrorKR.Rows[i + 1]["PreProcessor"].ToString().Length == 0)
                {
                    string[] strArrayTemp = tblNetErrorKR.Rows[i]["PreProcessor"].ToString().Split(' ');
                    string strTemp = "#endif " + strArrayTemp[1];
                    sw.Write(strTemp);
                    sw.Write("\r\n\r\n");
                    bPreProcesser = false;
                }

            }
            sw.Write("\r\n");

            sw.Write("\tL\"\"\r\n");

            sw.Write("};\r\n");


            sw.Write("//" + strCountryCode + "버전 출력\r\n");
            sw.Write("static wchar_t* szErrorStrF[] = {\r\n");

            for (int i = 0; i < tblNetError.Rows.Count; i++)
            {

                if (bPreProcesser && tblNetError.Rows[i]["PreProcessor"].ToString() != tblNetError.Rows[i - 1]["PreProcessor"].ToString())
                {
                    sw.Write("\r\n");
                    sw.Write(tblNetError.Rows[i]["PreProcessor"].ToString());
                    sw.Write("\r\n\r\n");
                }

                if (tblNetError.Rows[i]["PreProcessor"].ToString().Length > 0 && !bPreProcesser)
                {
                    sw.Write("\r\n");
                    sw.Write(tblNetError.Rows[i]["PreProcessor"].ToString());
                    sw.Write("\r\n");
                    bPreProcesser = true;
                }

                sw.Write("\tL\"");
                sw.Write(tblNetError.Rows[i]["String"].ToString());
                                
                sw.Write("\",\r\n");
                
                if (bPreProcesser && tblNetError.Rows[i + 1]["PreProcessor"].ToString().Length == 0)
                {
                    string[] strArrayTemp = tblNetError.Rows[i]["PreProcessor"].ToString().Split(' ');
                    string strTemp = "#endif " + strArrayTemp[1];
                    sw.Write(strTemp);
                    sw.Write("\r\n\r\n");
                    bPreProcesser = false;
                }

            }

            sw.Write("\r\n");

            sw.Write("\tL\"\"\r\n");

            sw.Write("};\r\n");
                        

            sw.Close();
//#endif

            Process Dos = new Process();
            Dos.StartInfo.FileName = "tortoiseProc";
            Dos.StartInfo.Arguments = "/Command:commit /path:\"..\\..\\..\\source\\" + strCountryCode + "\\Trunk\\KncWX2Server\\Common\\NetError_def.h\" /notempfile /closeonend:1";
            Dos.Start();
            
            MessageBox.Show("파일이 생성되었습니다.");
            
        }

        private void btnHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.SetHistoryTable(tblNetError_History);
            frm.ShowDialog(this);
        }

        private void StringTable_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.YesNoCancel);
                if (result == DialogResult.Yes)
                {
                    /*//History 추가
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    for (int i = 0; i < m_ModifyIDList.Count; i++)
                    {
                        tblNetError_History.Rows.Add();
                        DataRow Row_History = tblNetError_History.Rows[tblNetError_History.Rows.Count - 1];
                        DataRow Row = tblNetError.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];
                        Row_History["ID"] = Row["ID"];
                        Row_History["Enum"] = Row["Enum"];
                        Row_History["Type"] = Row["Type"];
                        Row_History["String"] = Row["String"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                    }
                    m_ModifyIDList.Clear();
                    Adpt.Update(tblNetError);
                    Adpt_History.Update(tblNetError_History);*/
                    StringSave();
                    MessageBox.Show("저장되었습니다.");
                }
                else if (result == DialogResult.Cancel)
                {
                    e.Cancel = true;
                    return;
                }
                m_bSaveEnd = true;
                m_ModifyIDList.Clear();
            }            
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                int nIndex = dataGridView1["ID", i].Value.ToString().IndexOf(textBox1.Text);
                //if (textBox1.Text == m_dgvData[nColumn, nRow].Value.ToString().IndexOf())
                //int nIndex = m_dgvData[nColumn, nRow].Value.ToString().IndexOf(textBox1.Text.ToString());
                if (nIndex > -1)
                {
                    dataGridView1.CurrentCell = dataGridView1["ID", i];                    
                    break;
                }
            }
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                int nIndex = dataGridView1["Enum", i].Value.ToString().IndexOf(textBox2.Text);
                //if (textBox1.Text == m_dgvData[nColumn, nRow].Value.ToString().IndexOf())
                //int nIndex = m_dgvData[nColumn, nRow].Value.ToString().IndexOf(textBox1.Text.ToString());
                if (nIndex > -1)
                {
                    dataGridView1.CurrentCell = dataGridView1["Enum", i];
                    break;
                }
            }

        }

        private void StringTable_Resize(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - 30;
            dataGridView1.Height = this.Height - 140;

            Add.Left = dataGridView1.Left;
            Add.Top = dataGridView1.Top + dataGridView1.Height + 5;

            Save.Left = Add.Left + Add.Width + 20;
            Save.Top = Add.Top;

            FileGenerate.Left = Save.Left + Save.Width + 20;
            FileGenerate.Top = Save.Top;

            btnHistory.Left = FileGenerate.Left + FileGenerate.Width + 20;
            btnHistory.Top = FileGenerate.Top;

            dataGridView1.Columns["ID"].Width = 100;
            dataGridView1.Columns["Enum"].Width = 150;
            dataGridView1.Columns["PreProcessor"].Width = 100;
            dataGridView1.Columns["Regdate"].Width = 150;

            dataGridView1.Columns["String"].Width = dataGridView1.Width -600;
        }

        private void dataGridView1_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            DialogResult result = MessageBox.Show("정말 삭제하시겠습니까? 삭제 후 문제가 발생할 수도 있습니다.", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                e.Cancel = true;
                return;
            }
            else if (result == DialogResult.OK)
            {
                UserDeletingRow(e.Row.Index, dataGridView1, tblNetError);
                e.Cancel = true;
            }
        }

        

        void UserDeletingRow(int nDeletingRowIndex, DataGridView dgvView, DataTable tblDelete)
        {
            DataRow dr = null;// = tblDelete.Rows[e.Row.Index];
            int nRowIndex = 0;

            for (int i = 0; i < tblDelete.Rows.Count; i++)
            {
                if (tblDelete.Rows[i].RowState == DataRowState.Deleted)
                {
                    continue;
                }

                if (dgvView["ID", nDeletingRowIndex].Value.ToString() == tblDelete.Rows[i]["ID"].ToString())
                {
                    dr = tblDelete.Rows[i];
                    nRowIndex = i;
                    break;
                }
            }
            

            //DeleteHistory에 넣어주기 위하여 값을 보존
            DataTable tblTemp = tblDelete.Clone();
            tblTemp = tblDelete.Copy();
            DataRow drOriginal = tblTemp.Rows[nRowIndex];

            tblDelete.Rows[nRowIndex].Delete();
            dgvView.DataSource = tblDelete;

            sDeleteRow sRow = new sDeleteRow();
            sRow.drRow = dr;
            sRow.drOriginalRow = drOriginal;
            
            if (!m_DeleteRowList.Contains(sRow))
            {
                m_DeleteRowList.Add(sRow);                
            }


            //Adpt_NpcExtraAbility.Update(new DataRow[] { dr });
            m_bSaveEnd = false;
        }
       
    }
}