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
    public partial class StatTableAdd : Form
    {
        DataGridView m_dgvData;
        DataTable m_tblStat;

        ArrayList m_ModifyIDList;

        public StatTableAdd()
        {
            InitializeComponent();
        }

        public void SetData(string strLV, string strPhysAtk, string strMagicAtk, string strPhysDef, string strMagicDef, string strHP)
        {
            textBox1.Text = strLV;
            textBox2.Text = strPhysAtk;
            textBox3.Text = strMagicAtk;
            textBox4.Text = strPhysDef;
            textBox5.Text = strMagicDef;
            textBox6.Text = strHP;            

        }

        public void SetDataTable(DataTable tblStat)
        {
            m_tblStat = tblStat;
        }
        public void SetDataGridView(DataGridView dgvData)
        {
            m_dgvData = dgvData;
        }
        public void SetModifyArray(ArrayList ModifyIDList)
        {
            m_ModifyIDList = ModifyIDList;
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Equals("") || textBox2.Text.Equals("") || textBox3.Text.Equals("") ||
                textBox4.Text.Equals("") || textBox5.Text.Equals("") || textBox6.Text.Equals(""))
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

            m_tblStat.Rows.Add();

            m_dgvData[0, m_dgvData.RowCount - 1].Value = nIndex;
            m_dgvData[0, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[1, m_dgvData.RowCount - 1].Value = textBox1.Text;
            m_dgvData[1, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[2, m_dgvData.RowCount - 1].Value = textBox2.Text;
            m_dgvData[2, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[3, m_dgvData.RowCount - 1].Value = textBox3.Text;
            m_dgvData[3, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[4, m_dgvData.RowCount - 1].Value = textBox4.Text;
            m_dgvData[4, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[5, m_dgvData.RowCount - 1].Value = textBox5.Text;
            m_dgvData[5, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[6, m_dgvData.RowCount - 1].Value = textBox6.Text;
            m_dgvData[6, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[7, m_dgvData.RowCount - 1].Value = DateTime.Now;
            m_dgvData[7, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;

            ((StatTable)this.Owner).m_bSaveEnd = false;
            m_dgvData.CurrentCell = m_dgvData[0, m_dgvData.RowCount - 1];

            sTableValue sModiftIDList = new sTableValue();
            sModiftIDList.nRowIndex = m_dgvData.RowCount - 1;
            sModiftIDList.nTabIndex = ((StatTable)this.Owner).GetCurrentTab();
            sModiftIDList.nIndexKey = Convert.ToInt32(m_dgvData["IndexKey", m_dgvData.RowCount - 1].Value.ToString());

            m_ModifyIDList.Add(sModiftIDList);
            //m_ModifyIDList.Add(m_dgvData.RowCount - 1);
            this.Hide();

        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
            textBox2.Text = "";
            textBox3.Text = "";
            textBox4.Text = "";
            textBox5.Text = "";
            textBox6.Text = "";   

        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void textBox6_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Enter == e.KeyChar)
                this.btnAdd_Click(null, null);
        }
    }
}