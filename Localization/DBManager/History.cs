using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DBManager
{
    public partial class History : Form
    {

        DataTable m_dtHistory;

        public History()
        {
            InitializeComponent();
        }

        public void SetHistoryTable(DataTable dtHistory)
        {
            m_dtHistory = dtHistory;
        }

        private void History_Load(object sender, EventArgs e)
        {
            dataGridView1.DataSource = m_dtHistory;
            dataGridView1.AllowUserToAddRows = false;
            dataGridView1.ReadOnly = true;
        }

        private void History_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Escape == e.KeyChar)
                this.Hide();
        }

        private void dataGridView1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Escape == e.KeyChar)
                this.Hide();
        }

        private void History_Resize(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - 30;
            dataGridView1.Height = this.Height - 60;
        }
    }
}