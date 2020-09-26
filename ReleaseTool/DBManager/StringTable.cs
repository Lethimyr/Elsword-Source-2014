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
using DBManager.ScriptDataDataSetTableAdapters;


namespace DBManager
{    
    struct sCell
    {
        public string strCountryCode;
        public int nIndex;
        public string strTableName;
        public bool bChange;
    }
    public partial class StringTable : Form
    {
        string m_strID;
        DataTable m_tblKR;
        DataTable m_tblTW;
        DataTable m_tblHK;
        DataTable m_tblJP;
        DataTable m_tblCurrent;


        DataTable m_tblQuest;

        DataTable m_tblRelease_History;

        ReleaseTableAdapter m_Adpt = new ReleaseTableAdapter();
        Release_HistoryTableAdapter m_Adpt_History = new Release_HistoryTableAdapter();

        QuestTableTableAdapter m_Adpt_Quest = new QuestTableTableAdapter();

        ArrayList m_NewCell = new ArrayList();
        ArrayList m_StateChangeCell = new ArrayList();
        object objTemp;

        private int m_nStartPoint = 0;

        bool m_bSaveEnd = true;
       
                       
        public StringTable()
        {
            InitializeComponent();            
        }

        public void SetID(string strID)
        {
            m_strID = strID;
        }


        private void Setting()
        {
            m_tblCurrent = new DataTable("tblCurrent");

            DataColumn Col;
            Col = new DataColumn("ReleaseIDX", typeof(int));
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("Index", typeof(int));
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("TableName", m_tblKR.Columns["TableName"].DataType);
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("TYPE", m_tblKR.Columns["TableName"].DataType);
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("Name", m_tblKR.Columns["Name"].DataType);
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("KR_State", m_tblKR.Columns["State"].DataType);
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("TW_State", m_tblTW.Columns["State"].DataType);
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("HK_State", m_tblTW.Columns["State"].DataType);
            m_tblCurrent.Columns.Add(Col);

            Col = new DataColumn("JP_State", m_tblTW.Columns["State"].DataType);
            m_tblCurrent.Columns.Add(Col);


            
            for (int i = 0; i < m_tblKR.Rows.Count; i++)
            {
                m_tblCurrent.Rows.Add();

                DataRow Release_Current = m_tblCurrent.Rows[i];
                DataRow Release_KR = m_tblKR.Rows[i];
                DataRow Release_TW = m_tblTW.Rows[i];
                DataRow Release_HK = m_tblHK.Rows[i];
                DataRow Release_JP = m_tblJP.Rows[i];
                //DataRow String_Taiwan = tblString_Tw.Rows[i];

                Release_Current["ReleaseIDX"] = Release_KR["ReleaseIDX"];
                Release_Current["Index"] = Release_KR["Index"];                
                Release_Current["TableName"] = Release_KR["TableName"];
                Release_Current["Name"] = Release_KR["Name"];
                Release_Current["KR_State"] = Release_KR["State"];
                Release_Current["TW_State"] = Release_TW["State"];
                Release_Current["HK_State"] = Release_HK["State"];
                Release_Current["JP_State"] = Release_JP["State"];


                if(Release_Current["TableName"].ToString() == "QuestTable")
                {
                    DataRow Result = m_tblQuest.Rows.Find(Release_Current["Index"].ToString());

                    Release_Current["TYPE"] =  Result["Quest Type"];
                }
            }

            dataGridView1.Width = this.Width - 33;
            dataGridView1.Height = this.Height - 140;

            dataGridView1.DataSource = m_tblCurrent;
            dataGridView1.AllowUserToAddRows = false;

            dataGridView1.Columns["ReleaseIDX"].Width = 100;
            dataGridView1.Columns["ReleaseIDX"].ReadOnly = true;
            dataGridView1.Columns["Index"].Width = 100;
            dataGridView1.Columns["Index"].ReadOnly = true;
            dataGridView1.Columns["TYPE"].Width = 100;
            dataGridView1.Columns["TYPE"].ReadOnly = true;
            dataGridView1.Columns["TableName"].Width = 100;
            dataGridView1.Columns["TableName"].ReadOnly = true;
            dataGridView1.Columns["Name"].Width = this.Width / 3;
            dataGridView1.Columns["Name"].ReadOnly = true;
            dataGridView1.Columns["KR_State"].Width = 100;
            dataGridView1.Columns["TW_State"].Width = 100;
            dataGridView1.Columns["HK_State"].Width = 100;
            dataGridView1.Columns["JP_State"].Width = 100;

            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
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
                    for (int j = 0; j < m_tblKR.Rows.Count; j++)
                    {
                        if (m_tblTW.Rows[j]["Index"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).nIndex) &&
                                m_tblKR.Rows[j]["TableName"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).strTableName))
                        {
                            DataRow dr = m_tblTW.Rows[j];

                            dr["State"] = ((sCell)m_StateChangeCell[i]).bChange;

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
                        if (m_tblKR.Rows[j]["Index"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).nIndex) &&
                            m_tblKR.Rows[j]["TableName"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).strTableName))
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
                else if (((sCell)m_StateChangeCell[i]).strCountryCode == "HK")
                {
                    for (int j = 0; j < m_tblHK.Rows.Count; j++)
                    {
                        if (m_tblHK.Rows[j]["Index"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).nIndex) &&
                            m_tblHK.Rows[j]["TableName"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).strTableName))
                        {
                            DataRow dr = m_tblHK.Rows[j];

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
                else if (((sCell)m_StateChangeCell[i]).strCountryCode == "JP")
                {
                    for (int j = 0; j < m_tblJP.Rows.Count; j++)
                    {
                        if (m_tblJP.Rows[j]["Index"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).nIndex) &&
                            m_tblJP.Rows[j]["TableName"].ToString() == Convert.ToString(((sCell)m_StateChangeCell[i]).strTableName))
                        {
                            DataRow dr = m_tblJP.Rows[j];

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
            m_bSaveEnd = true;

        }

        private void StringTable_Load(object sender, EventArgs e)
        {
            m_tblKR = m_Adpt.GetDataBy("KR");

            m_tblTW = m_Adpt.GetDataBy("TW");

            m_tblHK = m_Adpt.GetDataBy("HK");

            m_tblJP = m_Adpt.GetDataBy("JP");

            m_tblQuest = m_Adpt_Quest.GetData();

            m_tblQuest.PrimaryKey = new DataColumn[] { m_tblQuest.Columns["IndexKey"] };

            //Setting();
            comboBox1.SelectedIndex = 0;
            checkTW.Checked = true;
            checkHK.Checked = true;
            checkJP.Checked = true;

            m_tblRelease_History = m_Adpt_History.GetData();
        }
        
        private void StringTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();            
        }

        private void Save_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("저장하시겠습니까?", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                return;
            }
            Save();           
            MessageBox.Show("저장되었습니다.");
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {            
            objTemp = dataGridView1.CurrentCell.Value;            
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
           /* 
            if(objTemp.Equals(dataGridView1.CurrentCell.Value))
            {
                return;
            }
            
            sCell sChange = new sCell();
            if(dataGridView1.CurrentCell.OwningColumn.Name == "TW_State")
            {                
                sChange.strCountryCode = "TW";
                sChange.bChange = Convert.ToBoolean(dataGridView1["TW_State", dataGridView1.CurrentCell.RowIndex].Value.ToString());
            }
            else if (dataGridView1.CurrentCell.OwningColumn.Name == "KR_State")
            {
                sChange.strCountryCode = "KR";
                sChange.bChange = Convert.ToBoolean(dataGridView1["KR_State", dataGridView1.CurrentCell.RowIndex].Value.ToString());
            }
            else if (dataGridView1.CurrentCell.OwningColumn.Name == "HK_State")
            {
                sChange.strCountryCode = "HK";
                sChange.bChange = Convert.ToBoolean(dataGridView1["HK_State", dataGridView1.CurrentCell.RowIndex].Value.ToString());
            }
            sChange.strTableName = dataGridView1["TableName", dataGridView1.CurrentCell.RowIndex].Value.ToString();
            sChange.nIndex = Convert.ToInt32(dataGridView1["Index", dataGridView1.CurrentCell.RowIndex].Value.ToString());
            
            m_StateChangeCell.Add(sChange);
            m_bSaveEnd = false;*/
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
            if (!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {
                    Save();
                    MessageBox.Show("저장되었습니다.");
                }
                m_StateChangeCell.Clear();
                m_bSaveEnd = true;
            }
        }

       
        private void StringTable_Resize(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - 33;
            dataGridView1.Height = this.Height - 140;


            dataGridView1.Columns["ReleaseIDX"].Width = 100;
            dataGridView1.Columns["Index"].Width = 100;
            dataGridView1.Columns["TYPE"].Width = 100;
            dataGridView1.Columns["TableName"].Width = 100;
            dataGridView1.Columns["Name"].Width = this.Width / 3;
            dataGridView1.Columns["KR_State"].Width = 100;
            dataGridView1.Columns["TW_State"].Width = 100;
            dataGridView1.Columns["HK_State"].Width = 100;
            dataGridView1.Columns["JP_State"].Width = 100;
            //dataGridView1.Columns[1].Width = this.Width / 3;
            //dataGridView1.Columns[2].Width = this.Width / 3;

            btnSave.Left = dataGridView1.Location.X;
            btnSave.Top = dataGridView1.Location.Y + dataGridView1.Height + 5;

            

            btnHistory.Left = btnSave.Location.X + btnSave.Width + 5;
            btnHistory.Top = btnSave.Location.Y;

            
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox1.SelectedItem.ToString() == "All")
            {
                m_tblKR = m_Adpt.GetDataBy("KR");
                m_tblTW = m_Adpt.GetDataBy("TW");
                m_tblHK = m_Adpt.GetDataBy("HK");
                m_tblJP = m_Adpt.GetDataBy("JP");
                if(m_tblCurrent != null)
                    m_tblCurrent.Clear();
                Setting();

            }
            else if (comboBox1.SelectedItem.ToString() == "Item")
            {
                m_tblKR = m_Adpt.GetDataBy_Item("KR");
                m_tblTW = m_Adpt.GetDataBy_Item("TW");
                m_tblHK = m_Adpt.GetDataBy_Item("HK");
                m_tblJP = m_Adpt.GetDataBy_Item("JP");
                
                m_tblCurrent.Clear();
                
                Setting();

            }
            else if (comboBox1.SelectedItem.ToString() == "QuestTable")
            {
                m_tblKR = m_Adpt.GetDataBy_QuestTable("KR");
                m_tblTW = m_Adpt.GetDataBy_QuestTable("TW");
                m_tblHK = m_Adpt.GetDataBy_QuestTable("HK");
                m_tblJP = m_Adpt.GetDataBy_QuestTable("JP");
                m_tblCurrent.Clear();
                Setting();

            }
            else if (comboBox1.SelectedItem.ToString() == "TitleTable")
            {
                m_tblKR = m_Adpt.GetDataBy_TitleTable("KR");
                m_tblTW = m_Adpt.GetDataBy_TitleTable("TW");
                m_tblHK = m_Adpt.GetDataBy_TitleTable("HK");
                m_tblJP = m_Adpt.GetDataBy_TitleTable("JP");
                m_tblCurrent.Clear();
                Setting();
            }
            else if (comboBox1.SelectedItem.ToString() == "SkillEnum")
            {
                m_tblKR = m_Adpt.GetDataBy_SkillEnum("KR");
                m_tblTW = m_Adpt.GetDataBy_SkillEnum("TW");
                m_tblHK = m_Adpt.GetDataBy_SkillEnum("HK");
                m_tblJP = m_Adpt.GetDataBy_SkillEnum("JP");
                m_tblCurrent.Clear();
                Setting();
            }
        }

        private void checkTW_CheckedChanged(object sender, EventArgs e)
        {
            if (checkTW.Checked)
            {     
           
                dataGridView1.Columns["TW_State"].Visible = true;
            }
            else
            {           
                dataGridView1.Columns["TW_State"].Visible = false;
            }
        }

        private void checkHK_CheckedChanged(object sender, EventArgs e)
        {
            if (checkHK.Checked)
            {

                dataGridView1.Columns["HK_State"].Visible = true;
            }
            else
            {
                dataGridView1.Columns["HK_State"].Visible = false;
            }
        }

        private void checkJP_CheckedChanged(object sender, EventArgs e)
        {
            if (checkJP.Checked)
            {

                dataGridView1.Columns["JP_State"].Visible = true;
            }
            else
            {
                dataGridView1.Columns["JP_State"].Visible = false;
            }
        }

        private void dataGridView1_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView1[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                if (dataGridView1.CurrentCell.ColumnIndex != dataGridView1[textBox1.Text, 0].ColumnIndex)
                {
                    m_nStartPoint = 0;
                }

                for (int i = m_nStartPoint; i < dataGridView1.RowCount; i++)
                {
                    if (!dataGridView1[textBox1.Text, i].Visible)
                    {
                        continue;
                    }
                    if (dataGridView1[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                    {
                        dataGridView1.CurrentCell = dataGridView1[textBox1.Text, i];
                        m_nStartPoint = i + 1;
                        return;
                    }
                }

                for (int i = 0; i < m_nStartPoint; i++)
                {
                    if (!dataGridView1[textBox1.Text, i].Visible)
                    {
                        continue;
                    }
                    if (dataGridView1[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                    {
                        dataGridView1.CurrentCell = dataGridView1[textBox1.Text, i];
                        m_nStartPoint = i + 1;
                        return;
                    }
                }

                MessageBox.Show("결과값이 존재하지 않습니다.");
            }
        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                button1_Click(null, null);
            }
        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            if (dataGridView1.CurrentCell.ValueType == Type.GetType("System.Boolean"))
            {
                sCell sChange = new sCell();
                if (dataGridView1.CurrentCell.OwningColumn.Name == "TW_State")
                {
                    sChange.strCountryCode = "TW";
                    sChange.bChange = Convert.ToBoolean(dataGridView1["TW_State", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                }
                else if (dataGridView1.CurrentCell.OwningColumn.Name == "KR_State")
                {
                    sChange.strCountryCode = "KR";
                    sChange.bChange = Convert.ToBoolean(dataGridView1["KR_State", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                }
                else if (dataGridView1.CurrentCell.OwningColumn.Name == "HK_State")
                {
                    sChange.strCountryCode = "HK";
                    sChange.bChange = Convert.ToBoolean(dataGridView1["HK_State", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                }
                else if (dataGridView1.CurrentCell.OwningColumn.Name == "JP_State")
                {
                    sChange.strCountryCode = "JP";
                    sChange.bChange = Convert.ToBoolean(dataGridView1["JP_State", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                }
                sChange.strTableName = dataGridView1["TableName", dataGridView1.CurrentCell.RowIndex].Value.ToString();
                sChange.nIndex = Convert.ToInt32(dataGridView1["Index", dataGridView1.CurrentCell.RowIndex].Value.ToString());

                m_StateChangeCell.Add(sChange);
                m_bSaveEnd = false;
            }
        }      

    }
}