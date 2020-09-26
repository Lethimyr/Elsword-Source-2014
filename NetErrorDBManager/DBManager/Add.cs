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
        DataTable m_tblNetError;

        ArrayList m_ModifyIDList;

        public Add()
        {
            InitializeComponent();
        }

        public void SetDataTable(DataTable tblString)
        {
            m_tblNetError = tblString;
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
            //comboBox1.SelectedIndex = 0;

            textBox2.Text = m_dgvData["Enum", m_dgvData.SelectedCells[0].RowIndex].Value.ToString();
            textBox3.Text = m_dgvData["PreProcessor", m_dgvData.SelectedCells[0].RowIndex].Value.ToString();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "")
            {
                MessageBox.Show("스트링을 입력하여 주십시요.");
                return;
            }


            for (int i = 0; i < m_dgvData.RowCount; i++)
            {
                if (m_dgvData["String", i].Value.ToString() == textBox1.Text)
                {
                    m_dgvData.CurrentCell = m_dgvData["String", i];
                    DialogResult result = MessageBox.Show("입력한 스트링과 동일한 스트링이 발견 되었습니다. 그래도 추가하시겠습니까?", "", MessageBoxButtons.OKCancel);
                    if (result == DialogResult.OK)
                    {
                        break;
                    }
                    else
                    {
                        return;
                    }
                }
            }

            //int nID = Convert.ToInt32(m_dgvData[0, m_dgvData.RowCount - 1].Value.ToString());
            int nID = Convert.ToInt32(m_tblNetError.Rows[m_tblNetError.Rows.Count - 1]["ID"].ToString());
            nID++;
            //string strEnumTemp = m_dgvData[1, m_dgvData.RowCount - 1].Value.ToString();
            //int nIndex = strEnumTemp.IndexOf("ID_") + 3;
            //string strENUMSub = strEnumTemp.Substring(nIndex, strEnumTemp.Length - nIndex);
            //int nEnum = Convert.ToInt32(strENUMSub);
            //nEnum++;



            m_tblNetError.Rows.Add();

            m_dgvData[0, m_dgvData.RowCount - 1].Value = nID;
            m_dgvData[0, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;            
            m_dgvData[1, m_dgvData.RowCount - 1].Value = textBox2.Text;
            m_dgvData[1, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;            
            m_dgvData[2, m_dgvData.RowCount - 1].Value = textBox1.Text;
            m_dgvData[2, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[3, m_dgvData.RowCount - 1].Value = textBox3.Text;
            m_dgvData[3, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
            m_dgvData[4, m_dgvData.RowCount - 1].Value = DateTime.Now;
            m_dgvData[4, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;

            ((NetErrorTable)this.Owner).m_bSaveEnd = false;
            m_dgvData.CurrentCell = m_dgvData[0, m_dgvData.RowCount - 1];

            int nIDRow = Convert.ToInt32(m_dgvData["ID", m_dgvData.RowCount - 1].Value.ToString());

            m_ModifyIDList.Add(nIDRow);
            this.Hide();
                       
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