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

namespace DBManager
{    
    public partial class ExpTable : Form
    {
        //private TableType m_enumTableSelecter; //어느 테이블을 사용할 것인지 선택

        private SqlConnection Con;
        private SqlDataAdapter Adpt;
        DataTable tblExp;
        object objTemp;
        

        //History용        
        private SqlDataAdapter Adpt_History;
        DataTable tblExp_History;

        string m_strID;        
        ArrayList m_ModifyIDList = new ArrayList();
        public bool m_bSaveEnd = true; //수정했을 시 저장한 후 끝내는지 확인하는 변수
        
        
        
               
        public ExpTable()
        {
            InitializeComponent();            
        }

        public void SetID(string strID)
        {
            m_strID = strID;
        }

  /*      public void SetTableSelecter(TableType enumTableSelecter)
        {
            m_enumTableSelecter = enumTableSelecter;
        }*/

        private void ExpTable_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";
            Con.Open();

            Adpt = new SqlDataAdapter("SELECT * FROM ExpTable", Con);
            tblExp = new DataTable("tblExp");

            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt);
            Adpt.Fill(tblExp);
            dataGridView1.DataSource = tblExp;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView1.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView1.Columns[4].ReadOnly = true; //Reg_data수정방지

            //History
            Adpt_History = new SqlDataAdapter("SELECT * FROM ExpTable_History", Con);
            tblExp_History = new DataTable("tblExp_History");

            SqlCommandBuilder Builder_History = new SqlCommandBuilder(Adpt_History);
            Adpt_History.Fill(tblExp_History);       
               
        }

        private void ExpTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Con.Close();            
        }

        private void Save_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                for(int j = 0; j< dataGridView1.RowCount; j++)
                {
                    dataGridView1[i, j].Style.ForeColor = Color.Black;
                }
            }

            //History 추가
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            string strMyIP = host.AddressList[0].ToString();
            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                tblExp_History.Rows.Add();
                DataRow Row_History = tblExp_History.Rows[tblExp_History.Rows.Count - 1];
                DataRow Row = tblExp.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];

                Row_History["IndexKey"] = Row["IndexKey"];
                Row_History["Level"] = Row["Level"];
                Row_History["NeedExp"] = Row["NeedExp"];
                Row_History["TotalExp"] = Row["TotalExp"];
                Row_History["UserID"] = m_strID;
                Row_History["UserIP"] = strMyIP;
                Row_History["Regdate"] = Row["Regdate"];
                
            }

            //tblExp_History.Rows[tblExp_History.Rows.Count - 1]
            //DataRow Row = tblExp.Rows[Convert.ToInt32(m_ModifyIDList[0].ToString())];
            m_ModifyIDList.Clear();
            m_bSaveEnd = true;
            Adpt_History.Update(tblExp_History);
            Adpt.Update(tblExp);
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

                if (dataGridView1.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        if (dataGridView1[1, i].Value.ToString() == dataGridView1.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView1.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

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

            ExpTableAdd frm = new ExpTableAdd();
            frm.SetDataGridView(dataGridView1);
            frm.SetDataTable(tblExp);
            frm.SetModifyArray(m_ModifyIDList);
            int nRowIndex = dataGridView1.CurrentCell.RowIndex;
            frm.SetData(dataGridView1[1, nRowIndex].Value.ToString(), dataGridView1[2, nRowIndex].Value.ToString(), dataGridView1[3, nRowIndex].Value.ToString());
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
            if (m_bSaveEnd)
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
                {
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    for (int i = 0; i < m_ModifyIDList.Count; i++)
                    {
                        tblExp_History.Rows.Add();
                        DataRow Row_History = tblExp_History.Rows[tblExp_History.Rows.Count - 1];
                        DataRow Row = tblExp.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];

                        Row_History["IndexKey"] = Row["IndexKey"];
                        Row_History["Level"] = Row["Level"];
                        Row_History["NeedExp"] = Row["NeedExp"];
                        Row_History["TotalExp"] = Row["TotalExp"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                        
                    }
                    m_ModifyIDList.Clear();
                    m_bSaveEnd = true;
                    Adpt.Update(tblExp);
                    Adpt_History.Update(tblExp_History);
                }
                else if (result == DialogResult.Cancel)
                {
                    MessageBox.Show("저장하지 않으시면 파일을 생성할 수 없습니다.");
                    return;
                }
            }


            StreamWriter sw = new StreamWriter("..\\..\\ElswordNew\\X2\\dat\\Script\\Major\\ExpTable.lua", false, Encoding.Default);

            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
                sw.Write("g_pkExpTable:AddExpTable_LUA\r\n");
                sw.Write("{\r\n");
                sw.Write("       ");

                sw.Write("LEVEL = ");
                sw.Write(dataGridView1[1, i].Value.ToString());
                sw.Write(",\r\n");

                sw.Write("       ");
                sw.Write("NEED_EXP = ");
                sw.Write(dataGridView1[2, i].Value.ToString());
                sw.Write(",\r\n");

                sw.Write("       ");
                sw.Write("TOTAL_EXP = ");
                sw.Write(dataGridView1[3, i].Value.ToString());
                sw.Write(",\r\n");

                sw.Write("}\r\n");
                sw.Write("\r\n\r\n");
            }

            sw.Close();
            MessageBox.Show("파일이 생성되었습니다.");
            
        }

        private void btnHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.SetHistoryTable(tblExp_History);
            frm.ShowDialog(this);
        }

        private void ExpTable_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {
                    //History 추가
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    for (int i = 0; i < m_ModifyIDList.Count; i++)
                    {
                        tblExp_History.Rows.Add();
                        DataRow Row_History = tblExp_History.Rows[tblExp_History.Rows.Count - 1];
                        DataRow Row = tblExp.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];

                        Row_History["IndexKey"] = Row["IndexKey"];
                        Row_History["Level"] = Row["Level"];
                        Row_History["NeedExp"] = Row["NeedExp"];
                        Row_History["TotalExp"] = Row["TotalExp"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                               
                    }
                    
                    Adpt.Update(tblExp);
                    Adpt_History.Update(tblExp_History);
                    MessageBox.Show("저장되었습니다.");
                }
                m_ModifyIDList.Clear();
                m_bSaveEnd = true;
            }            
        }        
       
    }
}