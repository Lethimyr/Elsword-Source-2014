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
    public partial class ExpTableAdd : Form
    {
        DataGridView m_dgvData;
        DataTable m_tblExp;

        ArrayList m_ModifyIDList;

        public ExpTableAdd()
        {
            InitializeComponent();
        }

        public void SetDataTable(DataTable tblExp)
        {
            m_tblExp = tblExp;
        }
        public void SetDataGridView(DataGridView dgvData)
        {
            m_dgvData = dgvData;
        }
        public void SetModifyArray(ArrayList ModifyIDList)
        {
            m_ModifyIDList = ModifyIDList;
        }

        public void SetData(string strLevel, string strNeedExp, string strTotalExp )
        {
            textBox1.Text = strLevel;
            textBox2.Text = strNeedExp;
            textBox3.Text = strTotalExp;
        }

        private void ExpTableAdd_Load(object sender, EventArgs e)
        {

        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Equals("") || textBox2.Text.Equals("")|| textBox3.Text.Equals(""))
            {
                MessageBox.Show("입력하지 않은 데이터가 있습니다.");
                return;
            }


            for (int i = 0; i < m_dgvData.RowCount; i++)
            {
                if (m_dgvData[1, i].Value.ToString() == textBox1.Text)
                {
                    m_dgvData.CurrentCell = m_dgvData[1, i];
                    MessageBox.Show("동일한 Level은 추가할 수 없습니다. 다시 입력해 주십시요.");
                    return;                    
                }
            }

            int nIndex = Convert.ToInt32(m_dgvData[0, m_dgvData.RowCount - 1].Value.ToString());
            nIndex++;
            
            m_tblExp.Rows.Add();

            m_dgvData[0, m_dgvData.RowCount - 1].Value = nIndex;
            m_dgvData[0, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[1, m_dgvData.RowCount - 1].Value = textBox1.Text;
            m_dgvData[1, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[2, m_dgvData.RowCount - 1].Value = textBox2.Text;
            m_dgvData[2, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[3, m_dgvData.RowCount - 1].Value = textBox3.Text;
            m_dgvData[3, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[4, m_dgvData.RowCount - 1].Value = DateTime.Now;
            m_dgvData[4, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;

            ((ExpTable)this.Owner).m_bSaveEnd = false;
            m_dgvData.CurrentCell = m_dgvData[0, m_dgvData.RowCount - 1];

            m_ModifyIDList.Add(m_dgvData.RowCount - 1);
            this.Hide();
                        
        }

        private void btnEsc_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            textBox3.Text = "";
        }

        private void textBox3_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Enter == e.KeyChar)
                this.btnAdd_Click(null, null);
        }
    }
}