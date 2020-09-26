using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace DBManager
{
    public partial class Add : Form
    {
        DataGridView m_dgvData;
        DataTable m_tblString;

        ArrayList m_ModifyIDList;

        public Add()
        {
            InitializeComponent();
        }

        public void SetDataTable(DataTable tblString)
        {
            m_tblString = tblString;
        }
        public void SetDataGridView(DataGridView dgvData)
        {
            m_dgvData = dgvData;
        }
        public void SetModifyArray(ArrayList ModifyIDList)
        {
            m_ModifyIDList = ModifyIDList;
        }

        private void Add_Load(object sender, EventArgs e)
        {
            comboBox1.SelectedIndex = 0;
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if(textBox1.Text == "")
            {
                MessageBox.Show("스트링을 입력하여 주십시요.");
                return;
            }
            if(m_dgvData.Columns[3].Name == "String")
            {
                
                for(int i = 0; i < m_dgvData.RowCount; i++)
                {
                    if(m_dgvData[3, i].Value.ToString() == textBox1.Text)
                    {
                        m_dgvData.CurrentCell = m_dgvData[3, i];
                        DialogResult result = MessageBox.Show("입력한 스트링과 동일한 스트링이 발견 되었습니다. 그래도 추가하시겠습니까?", "", MessageBoxButtons.OKCancel);
                        if(result == DialogResult.OK)
                        {
                            break;
                        }
                        else
                        {
                            return;
                        }
                    }                    
                }

                int nID = Convert.ToInt32(m_dgvData[0, m_dgvData.RowCount - 1].Value.ToString());
                nID++;
                //string strEnumTemp = m_dgvData[1, m_dgvData.RowCount - 1].Value.ToString();
                //int nIndex = strEnumTemp.IndexOf("ID_") + 3;
                //string strENUMSub = strEnumTemp.Substring(nIndex, strEnumTemp.Length - nIndex);
                //int nEnum = Convert.ToInt32(strENUMSub);
                //nEnum++;

                string strENUM = "STR_ID_" + nID;

                string strType = "";
                if(comboBox1.SelectedIndex == 0)
                {
                    strType = "CODE";
                }
                else if(comboBox1.SelectedIndex == 1) 
                {
                    strType = "NPC";
                }
                else if(comboBox1.SelectedIndex == 2)
                {
                    strType = "DIALOG";
                }
                                
                m_tblString.Rows.Add();                
                               
                m_dgvData[0, m_dgvData.RowCount - 1].Value = nID;
                m_dgvData[0, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                m_dgvData[1, m_dgvData.RowCount - 1].Value = strENUM;
                m_dgvData[1, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                m_dgvData[2, m_dgvData.RowCount - 1].Value = strType;
                m_dgvData[2, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                m_dgvData[3, m_dgvData.RowCount - 1].Value = textBox1.Text;
                m_dgvData[3, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                m_dgvData[4, m_dgvData.RowCount - 1].Value = DateTime.Now;
                m_dgvData[4, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;

                
                m_dgvData.CurrentCell = m_dgvData[0, m_dgvData.RowCount - 1];

                m_ModifyIDList.Add(m_dgvData.RowCount - 1);
                this.Hide();
            }            
        }

        private void btnEsc_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Enter == e.KeyChar)
                this.btnAdd_Click(null, null);
        }
    }
}