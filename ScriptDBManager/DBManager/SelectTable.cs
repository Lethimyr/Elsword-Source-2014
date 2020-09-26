using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DBManager
{    
    public partial class SelectTable : Form
    {
        string m_strID;
        public SelectTable()
        {
            InitializeComponent();
        }
        public void SetID(string strID)
        {
            m_strID = strID;
        }


        private void btnExpTable_Click(object sender, EventArgs e)
        {
            ExpTable frm = new ExpTable();                        
            frm.SetID(m_strID);
            frm.Show(this);

        }

        private void SelectTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void btnStatTable_Click(object sender, EventArgs e)
        {
            StatTable frm = new StatTable();
            frm.SetID(m_strID);
            frm.Show(this);
        }

        private void btnItemTable_Click(object sender, EventArgs e)
        {
            ItemTable frm = new ItemTable();
            frm.SetID(m_strID);
            frm.Show(this);

        }

        private void btnQuestTable_Click(object sender, EventArgs e)
        {
            QuestTable frm = new QuestTable();
            frm.SetID(m_strID);
            frm.Show(this);
        }

        private void btnTitleTable_Click(object sender, EventArgs e)
        {
            TitleTable frm = new TitleTable();
            frm.SetID(m_strID);
            frm.Show(this);
        }

        private void btnSkillTable_Click(object sender, EventArgs e)
        {
            SkillTable frm = new SkillTable();
            //frm.SetID(m_strID);
            frm.Show(this);
        }

        
    }
}