using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;

using System.Collections;
using DBManager.LocalizationDataSetTableAdapters;


namespace DBManager
{    
    struct sCell
    {
        public string strCountryCode;
        public int nCount;
        public string strTableName;
    }
    public partial class StringTable : Form
    {
        string m_strID;
        DataTable m_tblKR;
        DataTable m_tblTW;
        DataTable m_tblCurrent;

        DataTable m_tblRelease_History;

        ReleaseTableAdapter m_Adpt = new ReleaseTableAdapter();
        Release_HistoryTableAdapter m_Adpt_History = new Release_HistoryTableAdapter();

        ArrayList m_NewCell = new ArrayList();
        ArrayList m_StateChangeCell = new ArrayList();
        object objTemp;
       
                       
        public StringTable()
        {
            InitializeComponent();            
        }

        public void SetID(string strID)
        {
            m_strID = strID;
        }

        /*private void ColumnStateChange()
        {
            int nCount;
            int ntblCurrentIndex = 0;
            if (m_tblKR.Rows.Count > m_tblTW.Rows.Count)
            {
                nCount = m_tblKR.Rows.Count;
            }
            else
            {
                nCount = m_tblTW.Rows.Count;
            }

            for (int i = 0; i < nCount; i++)
            {
                DataRow Release_Current = m_tblCurrent.Rows[ntblCurrentIndex];
                DataRow Release_KR = m_tblKR.Rows[i];
                DataRow Release_TW = m_tblTW.Rows[i];
                if (m_tblCurrent.Columns.IndexOf("TW_State") != -1)
                {
                    Release_TW = m_tblTW.Rows[i];
                    if (Release_KR["TableName"].ToString() == Release_TW["TableName"].ToString() &&
                        Release_KR["Index"].ToString() == Release_TW["Index"].ToString())
                    {
                        Release_Current["TW_State"] = Release_TW["State"];
                    }
                    else
                    {
                        if (Release_KR["TableName"].ToString() == "" &&
                            Release_KR["Index"].ToString() == "")
                        {
                            //우리나라 해당부분이 공백일떄

                            Release_Current["ID"] = Release_TW["Index"];
                            Release_Current["Name"] = Release_TW["Name"];
                            Release_Current["TW_State"] = Release_TW["State"];
                            Release_Current["KR_State"] = false;

                            sCell sNew = new sCell();
                            sNew.strCountryCode = "KR";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);
                        }
                        else if (Release_TW["TableName"].ToString() == "" &&
                             Release_TW["Index"].ToString() == "")
                        {
                            //대만 해당부분이 공백일떄
                            Release_Current["TW_State"] = false;

                            sCell sNew = new sCell();
                            sNew.strCountryCode = "TW";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);
                        }
                        else
                        {
                            Release_Current["TW_State"] = false;

                            sCell sNew = new sCell();
                            sNew.strCountryCode = "TW";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);

                            ntblCurrentIndex++;
                            m_tblCurrent.Rows.Add();

                            Release_Current = m_tblCurrent.Rows[ntblCurrentIndex];
                            Release_Current["ID"] = Release_TW["Index"];
                            Release_Current["Name"] = Release_TW["Name"];
                            Release_Current["KR_State"] = false;
                            Release_Current["TW_State"] = Release_TW["State"];


                            sNew.strCountryCode = "KR";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);
                        }

                    }
                }
                ntblCurrentIndex++;
            }
            dataGridView1.DataSource = m_tblCurrent;
            dataGridView1.AllowUserToAddRows = false;

            for (int i = 0; i < m_NewCell.Count; i++)
            {
                if (((sCell)m_NewCell[i]).strCountryCode == "TW")
                {
                    dataGridView1["TW_STate", ((sCell)m_NewCell[i]).nCount].Style.BackColor = Color.Red;
                    dataGridView1["TW_STate", ((sCell)m_NewCell[i]).nCount].ReadOnly = true;
                }
                else if (((sCell)m_NewCell[i]).strCountryCode == "KR")
                {
                    dataGridView1["KR_STate", ((sCell)m_NewCell[i]).nCount].Style.BackColor = Color.Red;
                    dataGridView1["KR_STate", ((sCell)m_NewCell[i]).nCount].ReadOnly = true;
                }
            }

        }*/

        private void Setting()
        {
            int nCount;
            int ntblCurrentIndex = 0;
            if (m_tblKR.Rows.Count > m_tblTW.Rows.Count)
            {
                nCount = m_tblKR.Rows.Count;
            }
            else
            {
                nCount = m_tblTW.Rows.Count;
            }

            for (int i = 0; i < nCount; i++)
            {
                m_tblCurrent.Rows.Add();

                if (m_tblKR.Rows.Count <= i)
                {
                    m_tblKR.Rows.Add();
                }
                DataRow Release_Current = m_tblCurrent.Rows[ntblCurrentIndex];
                DataRow Release_KR = m_tblKR.Rows[i];
                //DataRow String_Taiwan = tblString_Tw.Rows[i];

                Release_Current["ID"] = Release_KR["Index"];
                Release_Current["Name"] = Release_KR["Name"];
                Release_Current["KR_State"] = Release_KR["State"];

                if (m_tblTW.Rows.Count <= i)
                {
                    m_tblTW.Rows.Add();
                }
                if (m_tblCurrent.Columns.IndexOf("TW_State") != -1)
                {
                    DataRow Release_TW = m_tblTW.Rows[i];
                    if (Release_KR["TableName"].ToString() == Release_TW["TableName"].ToString() &&
                        Release_KR["Index"].ToString() == Release_TW["Index"].ToString())
                    {
                        Release_Current["TW_State"] = Release_TW["State"];
                    }
                    else
                    {
                        if (Release_KR["TableName"].ToString() == "" &&
                            Release_KR["Index"].ToString() == "")
                        {
                            //우리나라 해당부분이 공백일떄

                            Release_Current["ID"] = Release_TW["Index"];
                            Release_Current["Name"] = Release_TW["Name"];
                            Release_Current["TW_State"] = Release_TW["State"];
                            Release_Current["KR_State"] = false;

                            sCell sNew = new sCell();
                            sNew.strCountryCode = "KR";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);
                        }
                        else if (Release_TW["TableName"].ToString() == "" &&
                             Release_TW["Index"].ToString() == "")
                        {
                            //대만 해당부분이 공백일떄
                            Release_Current["TW_State"] = false;

                            sCell sNew = new sCell();
                            sNew.strCountryCode = "TW";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);
                        }
                        else
                        {
                            Release_Current["TW_State"] = false;

                            sCell sNew = new sCell();
                            sNew.strCountryCode = "TW";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);

                            ntblCurrentIndex++;
                            m_tblCurrent.Rows.Add();

                            Release_Current = m_tblCurrent.Rows[ntblCurrentIndex];
                            Release_Current["ID"] = Release_TW["Index"];
                            Release_Current["Name"] = Release_TW["Name"];
                            Release_Current["KR_State"] = false;
                            Release_Current["TW_State"] = Release_TW["State"];


                            sNew.strCountryCode = "KR";
                            sNew.nCount = ntblCurrentIndex;
                            m_NewCell.Add(sNew);
                        }
                        
                        
                    }
                }
                ntblCurrentIndex++;
            }
            dataGridView1.DataSource = m_tblCurrent;
            dataGridView1.AllowUserToAddRows = false;

            if (m_tblCurrent.Columns.IndexOf("TW_State") != -1)
            {
                for (int i = 0; i < m_NewCell.Count; i++)
                {
                    if (((sCell)m_NewCell[i]).strCountryCode == "TW")
                    {
                        dataGridView1["TW_STate", ((sCell)m_NewCell[i]).nCount].Style.BackColor = Color.Red;
                        dataGridView1["TW_STate", ((sCell)m_NewCell[i]).nCount].ReadOnly = true;
                    }
                    else if (((sCell)m_NewCell[i]).strCountryCode == "KR")
                    {
                        dataGridView1["KR_STate", ((sCell)m_NewCell[i]).nCount].Style.BackColor = Color.Red;
                        dataGridView1["KR_STate", ((sCell)m_NewCell[i]).nCount].ReadOnly = true;
                    }
                }
            }
        }

        private void Save()
        {
            
            for(int i = 0; i < m_StateChangeCell.Count; i++)
            {
                IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                string strMyIP = host.AddressList[0].ToString();
                if(((sCell)m_StateChangeCell[i]).strCountryCode == "TW")
                {
                    for(int j = 0; j< m_tblTW.Rows.Count; j++)
                    {
                        if (m_tblTW.Rows[j]["Index"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).nCount) &&
                            m_tblTW.Rows[j]["Name"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).strTableName))
                        {
                            DataRow dr = m_tblTW.Rows[j];
                            //m_tblTW["State"] = m_tblCurrent.Rows

                            m_tblRelease_History.Rows.Add();
                            DataRow drHistory = m_tblRelease_History.Rows[m_tblRelease_History.Rows.Count - 1];
                            drHistory["ReleaseIDX"] = dr["ReleaseIDX"];
                            drHistory["Name"] = dr["Name"];
                            drHistory["State"] = dr["State"];
                            drHistory["UserID"] = m_strID;
                            drHistory["UserIP"] = strMyIP;
                            drHistory["Last Update Date"] = dr["Last Update Date"];
                            
                            m_Adpt.Update(dr);
                            m_Adpt_History.Update(drHistory);
                        }
                    }                    
                }
                else if (((sCell)m_StateChangeCell[i]).strCountryCode == "KR")
                {
                    for (int j = 0; j < m_tblKR.Rows.Count; j++)
                    {
                        if (m_tblKR.Rows[j]["Index"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).nCount) &&
                            m_tblKR.Rows[j]["Name"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).strTableName))
                        {
                            DataRow dr = m_tblKR.Rows[j];

                            m_tblRelease_History.Rows.Add();
                            DataRow drHistory = m_tblRelease_History.Rows[m_tblRelease_History.Rows.Count - 1];
                            drHistory["ReleaseIDX"] = dr["ReleaseIDX"];
                            drHistory["Name"] = dr["Name"];
                            drHistory["State"] = dr["State"];
                            drHistory["UserID"] = m_strID;
                            drHistory["UserIP"] = strMyIP;
                            drHistory["Last Update Date"] = dr["Last Update Date"];

                            m_Adpt.Update(dr);
                            m_Adpt_History.Update(drHistory);
                        }
                    }                    

                }                
            }            

        }

        private void StringTable_Load(object sender, EventArgs e)
        {
            m_tblKR = m_Adpt.GetDataByKR();
                        
            m_tblTW = m_Adpt.GetDataByTW();

            m_tblCurrent = new DataTable("tblCurrent");

            DataColumn Col;
            Col = new DataColumn("ID", typeof(int));            
            m_tblCurrent.Columns.Add(Col);

            
            Col = new DataColumn(m_tblKR.Columns["Name"].ColumnName, m_tblKR.Columns["Name"].DataType);
            Col.MaxLength = m_tblKR.Columns["Name"].MaxLength;
            Col.AllowDBNull = m_tblKR.Columns["Name"].AllowDBNull;
            Col.Unique = m_tblKR.Columns["Name"].Unique;
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("KR_State", m_tblKR.Columns["State"].DataType);
            Col.MaxLength = m_tblKR.Columns["State"].MaxLength;
            Col.AllowDBNull = m_tblKR.Columns["State"].AllowDBNull;
            Col.Unique = m_tblKR.Columns["State"].Unique;
            m_tblCurrent.Columns.Add(Col);


            Col = new DataColumn("TW_State", m_tblTW.Columns["State"].DataType);
            Col.MaxLength = m_tblTW.Columns["State"].MaxLength;
            Col.AllowDBNull = m_tblTW.Columns["State"].AllowDBNull;
            Col.Unique = m_tblTW.Columns["State"].Unique;
            m_tblCurrent.Columns.Add(Col);
            
            comboBox1.SelectedIndex = 0;
            checkTW.Checked = true;

            m_tblRelease_History = m_Adpt_History.GetData();

           
            dataGridView1.Width = this.Width - 33;
            dataGridView1.Height = this.Height - 175;


            dataGridView1.Columns[0].Width = 100;
            dataGridView1.Columns[1].Width = this.Width / 3;
            dataGridView1.Columns[2].Width = 100;
            dataGridView1.Columns[3].Width = 100;
        }
        
        private void StringTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();            
        }

        private void Save_Click(object sender, EventArgs e)
        {
            Save();           
            MessageBox.Show("저장되었습니다.");
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            if (dataGridView1.CurrentCell.ColumnIndex < 2)
                return;
            objTemp = dataGridView1.CurrentCell.Value;
            //m_bChange =    
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (dataGridView1.CurrentCell.ColumnIndex < 2)
                return;
            if(objTemp.Equals(dataGridView1.CurrentCell.Value))
            {
                return;
            }
            if(dataGridView1.CurrentCell.Style.BackColor == Color.Red)
            {
                return;
            }
            if(dataGridView1.CurrentCell.OwningColumn.Name == "TW_State")
            {
                sCell sChange = new sCell();
                sChange.strCountryCode = "TW";
                sChange.nCount = Convert.ToInt32(dataGridView1["ID", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                

                for(int i = 0; i < m_tblTW.Rows.Count; i++)
                {
                    if(m_tblTW.Rows[i]["Name"].ToString() == dataGridView1["Name", dataGridView1.CurrentCell.RowIndex].Value.ToString())
                    {
                        m_tblTW.Rows[i]["State"] = dataGridView1["TW_State", dataGridView1.CurrentCell.RowIndex].Value;
                        sChange.strTableName = m_tblTW.Rows[i]["Name"].ToString();
                        break;
                    }
                }

                m_StateChangeCell.Add(sChange);
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "KR_State")
            {
                sCell sChange = new sCell();
                sChange.strCountryCode = "KR";
                sChange.nCount = Convert.ToInt32(dataGridView1["ID", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                

                for (int i = 0; i < m_tblKR.Rows.Count; i++)
                {
                    if (m_tblKR.Rows[i]["Name"].ToString() == dataGridView1["Name", dataGridView1.CurrentCell.RowIndex].Value.ToString())
                    {
                        m_tblKR.Rows[i]["State"] = dataGridView1["KR_State", dataGridView1.CurrentCell.RowIndex].Value;
                        sChange.strTableName = m_tblKR.Rows[i]["Name"].ToString();
                        break;
                    }
                }
                m_StateChangeCell.Add(sChange);
            }            
          
        }
        

        private void dataGridView1_KeyDown(object sender, KeyEventArgs e)
        {
           /* if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                Search frm = new Search();
                frm.SetDataGridView(dataGridView1);
                frm.ShowDialog(this);
            }*/       
        }

        private void FileGenerate_Click(object sender, EventArgs e)
        {
  
        }

        private void btnHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            m_tblTW.DefaultView.Sort = "Last Update Date";
            frm.SetHistoryTable(m_tblRelease_History);
            
            frm.ShowDialog(this);
        }

        private void StringTable_FormClosing(object sender, FormClosingEventArgs e)
        {
          
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

        private void StringTable_Resize(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - 33;
            dataGridView1.Height = this.Height - 175;


            dataGridView1.Columns[0].Width = 100;
            dataGridView1.Columns[1].Width = this.Width / 3;
            dataGridView1.Columns[2].Width = 100;
            dataGridView1.Columns[3].Width = 100;
            //dataGridView1.Columns[1].Width = this.Width / 3;
            //dataGridView1.Columns[2].Width = this.Width / 3;

            btnSave.Left = dataGridView1.Location.X;
            btnSave.Top = dataGridView1.Location.Y + dataGridView1.Height + 5;

            

            btnHistory.Left = btnSave.Location.X + btnSave.Width + 5;
            btnHistory.Top = btnSave.Location.Y;

            
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_NewCell.Clear();
            if (comboBox1.SelectedIndex == 0)
            {
                m_tblKR = m_Adpt.GetDataByKR();
                m_tblTW = m_Adpt.GetDataByTW();
                while (m_tblCurrent.Rows.Count > 0)
                {
                    m_tblCurrent.Rows.RemoveAt(0);
                }
                Setting();

            }
            else if(comboBox1.SelectedIndex == 1)
            {
                m_tblKR = m_Adpt.GetDataByKR_Item();
                m_tblTW = m_Adpt.GetDataByTW_Item();
                while (m_tblCurrent.Rows.Count > 0)
                {
                    m_tblCurrent.Rows.RemoveAt(0);
                }
                Setting();

            }
            else if (comboBox1.SelectedIndex == 2)
            {
                m_tblKR = m_Adpt.GetDataByKr_QuestTable();
                m_tblTW = m_Adpt.GetDataByTW_QuestTable();
                while (m_tblCurrent.Rows.Count > 0)
                {
                    m_tblCurrent.Rows.RemoveAt(0);
                }
                Setting();

            }
            else if (comboBox1.SelectedIndex == 3)
            {
                m_tblKR = m_Adpt.GetDataByKR_TitleTable();
                m_tblTW = m_Adpt.GetDataByTW_TitleTable();
                while (m_tblCurrent.Rows.Count > 0)
                {
                    m_tblCurrent.Rows.RemoveAt(0);
                }
                Setting();
            }
        }

        private void checkTW_CheckedChanged(object sender, EventArgs e)
        {
            if (checkTW.Checked)
            {     
                //if(m_tblCurrent.Columns.IndexOf("TW_State") == -1)
                {
                    /*DataColumn Col;
                    Col = new DataColumn("TW_State", m_tblTW.Columns["State"].DataType);
                    Col.MaxLength = m_tblTW.Columns["State"].MaxLength;
                    Col.AllowDBNull = m_tblTW.Columns["State"].AllowDBNull;
                    Col.Unique = m_tblTW.Columns["State"].Unique;
                    m_tblCurrent.Columns.Add(Col);*/

                    //ColumnStateChange();
                }
                dataGridView1.Columns["TW_State"].Visible = true;
            }
            else
            {
                /*m_tblCurrent.Columns.Remove(m_tblCurrent.Columns["TW_State"]);
                for (int i = 0; i < m_NewCell.Count; i++)
                {
                    if (((sCell)m_NewCell[i]).strCountryCode == "KR")
                        dataGridView1["KR_STate", ((sCell)m_NewCell[i]).nCount].Style.BackColor = Color.Red;
                }*/
                dataGridView1.Columns["TW_State"].Visible = false;
            }
        }

        private void checkHK_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkJP_CheckedChanged(object sender, EventArgs e)
        {

        }

      
    }
}