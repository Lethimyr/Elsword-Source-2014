using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DBManager
{
    public partial class Search : Form
    {
        DataGridView  m_dgvData;
       
        int m_nSearchRow = 0;
        int m_nSearchColumn = 0;

        int m_nLoopRow = 0;
        int m_nLoopColumn = 0;

        public Search()
        {
            InitializeComponent();
        }

        private void btnNextSearch_Click(object sender, EventArgs e)
        {
            m_nSearchRow = m_dgvData.CurrentCell.RowIndex;
            m_nSearchColumn = m_dgvData.CurrentCell.ColumnIndex;
            
            if (m_nSearchColumn < m_dgvData.ColumnCount-1)
            {
                m_nSearchColumn = m_nSearchColumn + 1;
            }
            else
            {
                m_nSearchColumn = 0;
                m_nSearchRow = m_nSearchRow + 1;
            }
            m_nLoopRow = m_nSearchRow;
            m_nLoopColumn = m_nSearchColumn;

            

            //textBox1.Text
            for (int nRow = m_nSearchRow; nRow < m_dgvData.RowCount; nRow++) //맨 마지막 줄은 비어있기 때문
            {
                for (int nColumn = m_nSearchColumn; nColumn < m_dgvData.ColumnCount; nColumn++)
                {
                    //[Column, Row]
                    //if (textBox1.Text == m_dgvData[nColumn, nRow].Value.ToString().IndexOf())
                    if(!m_dgvData[nColumn, nRow].Visible)
                    {
                        continue;
                    }
                    int nIndex = m_dgvData[nColumn, nRow].Value.ToString().IndexOf(textBox1.Text.ToString());
                    if(nIndex > -1)
                    {
                        m_dgvData.CurrentCell = m_dgvData[nColumn, nRow];
                        m_nLoopRow = nRow;
                        m_nLoopColumn = nColumn;
                        return;
                    }
                    m_nSearchColumn = 0;
                }
            }

            for (int nRow = 0; nRow <= m_nLoopRow; nRow++) //맨 마지막 줄은 비어있기 때문
            {
                for (int nColumn = 0; nColumn < m_dgvData.ColumnCount; nColumn++)
                {
                    //[Column, Row]                    
                    //if (textBox1.Text == m_dgvData[nColumn, nRow].Value.ToString())
                    if (!m_dgvData[nColumn, nRow].Visible)
                    {
                        continue;
                    }
                    int nIndex = m_dgvData[nColumn, nRow].Value.ToString().IndexOf(textBox1.Text.ToString());

                    if (nIndex > -1)
                    {
                        m_dgvData.CurrentCell = m_dgvData[nColumn, nRow];
                        return;
                    }
                }
            }

            MessageBox.Show("해당하는 검색결과가 없습니다.");
            
            
        }

        public void SetDataGridView(DataGridView dgvData)
        {
            m_dgvData = dgvData;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if((char)Keys.Enter == e.KeyChar)
                this.btnNextSearch_Click(null, null);

            if ((char)Keys.Escape == e.KeyChar)
                this.Hide();
        }

    }
}