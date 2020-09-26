using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DBManager
{
    public partial class Filtering : Form
    {
        DataGridView dgvTemp;
        int m_nFilterColumnIndex;

        bool m_bFiltering;

        public Filtering()
        {
            InitializeComponent();
        }

        public void SetDataGridView(DataGridView dataGridView)
        {
            dgvTemp = dataGridView;
        }

        public void SetFilterColumnIndex(int nFilterColumnIndex)
        {
            m_nFilterColumnIndex = nFilterColumnIndex;
        }

        public void SetIsFiltering(bool bFiltering)
        {
            m_bFiltering = bFiltering;
        }

        private void button1_Click(object sender, EventArgs e)
        {            
            for (int j = 0; j < dgvTemp.RowCount; j++)
            {
                if (m_bFiltering && !dgvTemp.Rows[j].Visible)
                {
                    continue;
                }

                if(checkedListBox1.CheckedItems.Contains(dgvTemp[m_nFilterColumnIndex, j].Value.ToString()))
                {
                    dgvTemp.Rows[j].Visible = true;
                }
                else
                {
                    dgvTemp.CurrentCell = null;
                    dgvTemp.Rows[j].Visible = false;
                }                
            }
            this.Hide();
        }

        private void Filtering_Load(object sender, EventArgs e)
        {
            for(int i = 0; i < dgvTemp.RowCount; i++)
            {
                if (m_bFiltering && !dgvTemp.Rows[i].Visible)
                {
                    continue;
                }
                if(checkedListBox1.Items.IndexOf(dgvTemp[m_nFilterColumnIndex, i].Value.ToString()) == -1)
                {
                    checkedListBox1.Items.Add(dgvTemp[m_nFilterColumnIndex, i].Value.ToString());
                }
            }
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void Filtering_Resize(object sender, EventArgs e)
        {
            checkedListBox1.Width = this.Width - 15;
            checkedListBox1.Height = this.Height - 61;

            button2.Left = checkedListBox1.Location.X + checkedListBox1.Width - button1.Width;
            button2.Top = checkedListBox1.Location.Y + checkedListBox1.Height + 5;

            button1.Left = button2.Location.X - button1.Width - 5;
            button1.Top = button2.Top;
        }
        
    }
}