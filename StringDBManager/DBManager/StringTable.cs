//09. 06. 24 김정협 나라별 define
// ItemTable.cs, QuestTable.cs, TitleTable.cs 세군대 정의를
// DBManager 추가
// 모두 바꿔줘야 함.
#define __COUNTRY_KR__
//#define __COUNTRY_TW__
//#define __COUNTRY_HK__
//#define __COUNTRY_JP__

//한국과 일본은 복호화 작업 제대로 안되어있음 해줘야함.

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
using DBManager.LocalizationDataSetTableAdapters;
using DBManager.ScriptData_TWDataSetTableAdapters;
using DBManager.ScriptData_HKDataSetTableAdapters;
using System.Diagnostics;

namespace DBManager
{    
    public partial class StringTable : Form
    {
        private SqlConnection Con;
        private SqlDataAdapter Adpt;
        DataTable tblString;
        object objTemp;
        

        //History용        
        private SqlDataAdapter Adpt_History;
        DataTable tblString_History;

        string m_strID;        
        ArrayList m_ModifyIDList = new ArrayList();
        public bool m_bSaveEnd = true; //수정했을 시 저장한 후 끝내는지 확인하는 변수
        
        
        
               
        public StringTable()
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
                    dataGridView1[i, j].Style.SelectionForeColor = Color.White;
                }
            }

            //History 추가
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            string strMyIP = host.AddressList[0].ToString();
            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                tblString_History.Rows.Add();
                DataRow Row_History = tblString_History.Rows[tblString_History.Rows.Count - 1];
                DataRow Row = tblString.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];
                Row_History["ID"] = Row["ID"];
                Row_History["ENum"] = Row["ENum"];
                Row_History["Type"] = Row["Type"];
                Row_History["String"] = Row["String"];
                Row_History["UserID"] = m_strID;
                Row_History["UserIP"] = strMyIP;
                Row_History["Regdate"] = Row["Regdate"];

                try
                {
                    Adpt.Update(new DataRow[] { Row });
                    Adpt_History.Update(new DataRow[] { Row_History });
                }
                catch (SqlException ex)
                {
                    DataTable tblTemp;
                    tblTemp = tblString.Clone();
                    tblTemp = tblString.Copy();

                    tblString.Clear();
                    Adpt.Fill(tblString);

                    int nSamePoint = -1;
                    for (int j = tblTemp.Rows.Count - 1; j >= 0; j--)
                    {
                        if (j >= tblString.Rows.Count)
                        {
                            continue;
                        }
                        if (tblTemp.Rows[j]["ID"].ToString() == tblString.Rows[j]["ID"].ToString() &&
                            tblTemp.Rows[j]["String"].ToString() == tblString.Rows[j]["String"].ToString())
                        {
                            nSamePoint = j;
                            break;
                        }
                    }

                    for (int j = nSamePoint + 1; j < tblTemp.Rows.Count; j++)
                    {
                        /*tblString.Rows.Add();
                        Adpt.Update(new DataRow[] { tblString.Rows[tblString.Rows.Count - 1] });
                        tblString.Clear();
                        Adpt.Fill(tblString);*/
                        tblString.Rows.Add();
                        tblString_History.Rows.Add();
                        for (int k = 2; k < tblString.Columns.Count; k++)
                        {
                            tblString.Rows[tblString.Rows.Count - 1][k] = tblTemp.Rows[j][k];
                        }
                        tblString.Rows[tblString.Rows.Count - 1]["ID"] = Convert.ToInt32(tblString.Rows[tblString.Rows.Count - 2]["ID"].ToString()) + 1;
                        tblString.Rows[tblString.Rows.Count - 1]["ENum"] = "STR_ID_" + tblString.Rows[tblString.Rows.Count - 1]["ID"].ToString();

                        tblString_History.Rows[tblString_History.Rows.Count - 1]["ID"] = tblString.Rows[tblString.Rows.Count - 1]["ID"];
                        tblString_History.Rows[tblString_History.Rows.Count - 1]["ENum"] = tblString.Rows[tblString.Rows.Count - 1]["ENum"];
                        tblString_History.Rows[tblString_History.Rows.Count - 1]["Type"] = tblString.Rows[tblString.Rows.Count - 1]["Type"];
                        tblString_History.Rows[tblString_History.Rows.Count - 1]["String"] = tblString.Rows[tblString.Rows.Count - 1]["String"];
                        tblString_History.Rows[tblString_History.Rows.Count - 1]["UserID"] = m_strID;
                        tblString_History.Rows[tblString_History.Rows.Count - 1]["UserIP"] = strMyIP;
                        tblString_History.Rows[tblString_History.Rows.Count - 1]["Regdate"] = tblString.Rows[tblString.Rows.Count - 1]["Regdate"];


                        Adpt.Update(new DataRow[] { tblString.Rows[tblString.Rows.Count - 1] });
                        Adpt_History.Update(new DataRow[] { tblString_History.Rows[tblString_History.Rows.Count - 1] });
                    }
                    //tblString.Rows.Add();

                }

                int nRowIndex = -1;
                ScriptData_TWDataSetTableAdapters.StringTableTableAdapter Adpt_StringTW = new ScriptData_TWDataSetTableAdapters.StringTableTableAdapter();
                ScriptData_HKDataSetTableAdapters.StringTableTableAdapter Adpt_StringHK = new ScriptData_HKDataSetTableAdapters.StringTableTableAdapter();
                ScriptData_JPDataSetTableAdapters.StringTableTableAdapter Adpt_StringJP = new ScriptData_JPDataSetTableAdapters.StringTableTableAdapter();
                DataTable tblStringTW = Adpt_StringTW.GetData();
                TransTableAdapter Adpt_Trans = new TransTableAdapter();
                for (int twRow = 0; twRow < tblStringTW.Rows.Count; twRow++ )
                {
                    if (Row["ID"].ToString() == tblStringTW.Rows[twRow]["ID"].ToString())
                    {
                        nRowIndex = twRow;
                        break;
                    }
                }

                if(nRowIndex != -1)
                {                    
                    Adpt_Trans.UpdateStringTable(Row["String"].ToString(), Convert.ToInt32(Row["ID"].ToString()));                    
                }
                else
                {
                    Adpt_Trans.InsertTrans("KR", "ScriptData", "StringTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("TW", "ScriptData_TW", "StringTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("HK", "ScriptData_HK", "StringTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("JP", "ScriptData_JP", "StringTable", "String", Convert.ToInt32(Row["ID"].ToString()), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_StringTW.InsertString(Convert.ToInt16(Row["ID"].ToString()), Row["ENum"].ToString(), Row["TYPE"].ToString(), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_StringHK.InsertString(Convert.ToInt16(Row["ID"].ToString()), Row["ENum"].ToString(), Row["TYPE"].ToString(), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_StringJP.InsertString(Convert.ToInt16(Row["ID"].ToString()), Row["ENum"].ToString(), Row["TYPE"].ToString(), Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()));
                }   

            }

            tblString.Clear();
            Adpt.Fill(tblString);

            tblString_History.Clear();
            Adpt_History.Fill(tblString_History);

            dataGridView1.CurrentCell = dataGridView1[0, dataGridView1.RowCount - 1];
            //tblString_History.Rows[tblString_History.Rows.Count - 1]
            //DataRow Row = tblString.Rows[Convert.ToInt32(m_ModifyIDList[0].ToString())];
            m_ModifyIDList.Clear();
            m_bSaveEnd = true;
        }

        private void StringTable_Load(object sender, EventArgs e)
        {
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

            Adpt = new SqlDataAdapter("SELECT * FROM StringTable", Con);
            tblString = new DataTable("tblString");

            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt);
            Adpt.Fill(tblString);
            dataGridView1.DataSource = tblString;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView1.Columns[4].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["ID"].ReadOnly = true;
            dataGridView1.Columns["ENum"].ReadOnly = true;
            dataGridView1.CurrentCell = dataGridView1[0, dataGridView1.RowCount - 1];

            //History
            Adpt_History = new SqlDataAdapter("SELECT * FROM StringTable_History", Con);
            tblString_History = new DataTable("tblString_History");

            SqlCommandBuilder Builder_History = new SqlCommandBuilder(Adpt_History);
            Adpt_History.Fill(tblString_History);

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
                
                
                m_ModifyIDList.Add(dataGridView1.CurrentCell.RowIndex);
                m_bSaveEnd = false;
            }
        }

        private void Add_Click(object sender, EventArgs e)
        {
            //삭제 기능 막음 
            //dataGridView1.Rows.RemoveAt(dataGridView1.CurrentCell.RowIndex);            

            Add frm = new Add();
            frm.SetDataGridView(dataGridView1);
            frm.SetDataTable(tblString);
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
                        tblString_History.Rows.Add();
                        DataRow Row_History = tblString_History.Rows[tblString_History.Rows.Count - 1];
                        DataRow Row = tblString.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];
                        Row_History["ID"] = Row["ID"];
                        Row_History["ENum"] = Row["ENum"];
                        Row_History["Type"] = Row["Type"];
                        Row_History["String"] = Row["String"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                    }
                    m_ModifyIDList.Clear();
                    m_bSaveEnd = true;
                    Adpt.Update(tblString);
                    Adpt_History.Update(tblString_History);    */
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
            string strGeneral = "";
            string strDefineLua = "";
            string[] strDefineHeader = null;


            
#if __COUNTRY_KR__
            string strFileName = "..\\..\\..\\resource\\KR\\Trunk\\General.ess";
            strCountryCode = "KR";
#elif __COUNTRY_TW__
            string strFileName = "..\\..\\..\\resource\\TW\\Trunk\\General.ess";                                                
            strCountryCode = "TW";
#elif __COUNTRY_HK__
            string strFileName = "..\\..\\..\\resource\\HK\\Trunk\\General.ess";                                                
            strCountryCode = "HK";
#elif __COUNTRY_JP__
            string strFileName = "..\\..\\..\\resource\\JP\\Trunk\\General.ess";                                                
            strCountryCode = "JP";
#endif
            sw = new StreamWriter(strFileName, false, Encoding.Unicode);

            if(dataGridView1.Columns[3].Name == "String" )
            {
                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    //09. 07. 03 김정협 암호화하기위해 루틴 추가
                    string strXOR = dataGridView1[0, i].Value.ToString() + "\t" + dataGridView1[3, i].Value.ToString() ;                    
                    char[] cXOR = new char[strXOR.Length];

                    short iKey = 16;

                    int nCountOfString = strXOR.Length;

                    for (int k = 0; k < nCountOfString; k++)
                    {
                        cXOR[k] = (char)(strXOR[k] ^ iKey);
                        sw.Write(cXOR[k]);
                    }
                    sw.Write("\r\n");
                    /*
                    sw.Write("\t");                    
                    sw.Write(dataGridView1[3, i].Value.ToString());
                    sw.Write("\r\n");*/
                }
            }
            sw.Close();
            sw = null;

            strGeneral = Environment.CurrentDirectory + "\\..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\General.ess";

#if __COUNTRY_KR__
            sw = new StreamWriter("..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\StringID_def.lua", false, Encoding.Default);
#elif __COUNTRY_TW__
            sw = new StreamWriter("..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\StringID_def.lua", false, Encoding.UTF8);
#elif __COUNTRY_HK__
            sw = new StreamWriter("..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\StringID_def.lua", false, Encoding.UTF8);
#elif __COUNTRY_JP__
            sw = new StreamWriter("..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\StringID_def.lua", false, Encoding.UTF8);
#endif

            //sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
            if (dataGridView1.Columns[1].Name == "ENum")
            {
                sw.Write("STR_ID_EMPTY = -1\r\n");

                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    sw.Write(dataGridView1[1, i].Value.ToString());
                    sw.Write("\t\t = ");
                    sw.Write(dataGridView1[0, i].Value.ToString());
                    sw.Write("\r\n");
                }
            }

            sw.Close();
            sw = null;
            //09. 07. 14 김정협 상대경로로 인해 경로 조정함
            strDefineLua = Environment.CurrentDirectory + "\\..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\StringID_def.lua";

            const int nDivide = 2500;

            if (dataGridView1.RowCount > nDivide)
            {
               // string strFileName = "..\\..\\ElswordNew\\KTDXLIB\\StringID_def.h";
                
                int nFileCount = dataGridView1.RowCount / nDivide;
                if (dataGridView1.RowCount % nDivide != 0)
                {
                    nFileCount += 1;
                }

                strDefineHeader = new string[nFileCount];

                for (int i = 1; i <= nFileCount; i++ )
                {
                    strFileName = "..\\..\\..\\source\\" + strCountryCode + "\\Trunk\\KTDXLIB\\StringID_def" + i + ".h";

                    //차후 소스에서 UTF8지원하는지 안하는지에 따라 국가별 디파인으로 나눌지 안나눌지 결정
                    sw = new StreamWriter(strFileName, false, Encoding.Default);

                    sw.Write("#ifdef __NEVER_DEFINED_MACRO__\r\n");
                    sw.Write("#define _ENUM(VAL) VAL,\r\n");
                    sw.Write("enum\r\n{\r\n#endif\r\n\r\n");

                    if(i == 1)
                    {
                        //첫번째 파일에만 생성
                        sw.Write("_ENUM( STR_ID_EMPTY = -1 )\r\n");
                    }
                    

                    int nBegin = (i - 1) * nDivide;
                    int nEnd = i * nDivide;

                    if (nEnd > dataGridView1.RowCount)
                    {
                        nEnd = dataGridView1.RowCount;
                    }

                    for (int k = nBegin; k < nEnd; k++)
                    {
                        sw.Write("_ENUM( ");
                        sw.Write(dataGridView1[1, k].Value.ToString());
                        if (i != 1 && k == nBegin)
                        {                            
                            sw.Write(" = ");
                            sw.Write(dataGridView1[1, nBegin - 1].Value.ToString());
                            sw.Write(" + 1");
                        }
                        sw.Write(" )");

                        sw.Write("\t\t// ");
                        sw.Write(dataGridView1[3, k].Value.ToString());
                        sw.Write("\r\n");
                    }


                    sw.Write("\r\n#ifdef __NEVER_DEFINED_MACRO__\r\n};\r\n");
                    sw.Write("#undef _ENUM\r\n");
                    sw.Write("#endif");

                    sw.Close();

                    strDefineHeader[i - 1] = Environment.CurrentDirectory + "\\..\\..\\..\\source\\" + strCountryCode + "\\Trunk\\KTDXLIB\\StringID_def" + i + ".h";
                    
                    
                }

                
                
            }
            else
            {
                strDefineHeader = new string[1];

                sw = new StreamWriter("..\\..\\..\\source\\" + strCountryCode + "\\Trunk\\KTDXLIB\\StringID_def1.h", false, Encoding.Default);
                
                sw.Write("#ifdef __NEVER_DEFINED_MACRO__\r\n");
                sw.Write("#define _ENUM(VAL) VAL,\r\n");
                sw.Write("enum\r\n{\r\n#endif\r\n\r\n");
                sw.Write("_ENUM( STR_ID_EMPTY = -1 )\r\n");

                //if (dataGridView1.Columns[1].Name == "ENum" && dataGridView1.Columns[3].Name == "String")

                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    sw.Write("_ENUM( ");
                    sw.Write(dataGridView1[1, i].Value.ToString());
                    sw.Write(" )");
                    sw.Write("\t\t// ");
                    sw.Write(dataGridView1[3, i].Value.ToString());
                    sw.Write("\r\n");
                }


                sw.Write("\r\n#ifdef __NEVER_DEFINED_MACRO__\r\n};\r\n");
                sw.Write("#undef _ENUM\r\n");
                sw.Write("#endif");

                sw.Close();

                strDefineHeader[0] = "..\\..\\..\\..\\source\\" + strCountryCode + "\\Trunk\\KTDXLIB\\StringID_def1.h";
            }

            string strCommitFile = "";
            

            for(int j = 0; j < strDefineHeader.Length; j++)
            {
                if (j != 0)
                {
                    strCommitFile += "*";
                }
                strCommitFile += strDefineHeader[j];
            }
            

            Process Dos = new Process();
            Dos.StartInfo.FileName = "tortoiseProc";
            Dos.StartInfo.Arguments = "/Command:commit /path:\"" + strGeneral + "*" + strDefineLua + "\" /notempfile /closeonend:1";
            Dos.Start();

            Dos = new Process();
            Dos.StartInfo.FileName = "tortoiseProc";
            Dos.StartInfo.Arguments = "/Command:commit /path:\"" + strCommitFile + "\" /notempfile /closeonend:1";
            Dos.Start();

            MessageBox.Show("파일이 생성되었습니다.");
            
        }

        private void btnHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.SetHistoryTable(tblString_History);
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
                        tblString_History.Rows.Add();
                        DataRow Row_History = tblString_History.Rows[tblString_History.Rows.Count - 1];
                        DataRow Row = tblString.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];
                        Row_History["ID"] = Row["ID"];
                        Row_History["ENum"] = Row["ENum"];
                        Row_History["Type"] = Row["Type"];
                        Row_History["String"] = Row["String"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                    }
                    m_ModifyIDList.Clear();
                    Adpt.Update(tblString);
                    Adpt_History.Update(tblString_History);*/
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
                int nIndex = dataGridView1["ENum", i].Value.ToString().IndexOf(textBox2.Text);
                //if (textBox1.Text == m_dgvData[nColumn, nRow].Value.ToString().IndexOf())
                //int nIndex = m_dgvData[nColumn, nRow].Value.ToString().IndexOf(textBox1.Text.ToString());
                if (nIndex > -1)
                {
                    dataGridView1.CurrentCell = dataGridView1["ENum", i];
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
            dataGridView1.Columns["ENum"].Width = 150;
            dataGridView1.Columns["Type"].Width = 100;
            dataGridView1.Columns["Regdate"].Width = 150;

            dataGridView1.Columns["String"].Width = dataGridView1.Width -600;
        }        
       
    }
}