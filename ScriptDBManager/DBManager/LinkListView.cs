using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DBManager
{
    public partial class LinkListView : Form
    {

        DataTable m_tblTable;
        int m_nX = 0, m_nY = 0;
        DataGridView m_DataGridView;
        int m_nTab = -1;
        string m_strToolName;

        public LinkListView()
        {
            InitializeComponent();
        }

        public void SetTable(DataTable tblTable)
        {
            m_tblTable = tblTable;
        }

        public void SetDataGridView(DataGridView dataGridView, int nTab, string strToolName)
        {
            m_DataGridView = dataGridView;
            m_nTab = nTab;
            m_strToolName = strToolName;
        }
        public void SetPoint(int nX, int nY)
        {
            m_nX = nX;
            m_nY = nY;
        }

        private void LinkListView_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Escape)
            {
                this.Hide();
            }
        }

        private void dataGridView1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                this.Hide();
            }
        }

        private void LinkListView_Load(object sender, EventArgs e)
        {
            dataGridView1.DataSource = m_tblTable;
            dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            
            dataGridView1.Left = 0;
            dataGridView1.Top = 0;
            dataGridView1.Height = dataGridView1.Rows.GetRowsHeight(DataGridViewElementStates.Visible);
            dataGridView1.Width = dataGridView1.Columns.GetColumnsWidth(DataGridViewElementStates.Visible);

            this.Width = dataGridView1.Width;
            this.Height = dataGridView1.Height;

            this.Left = m_nX;
            this.Top = m_nY;
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if(e.Button == MouseButtons.Left)
            {
                m_DataGridView.CurrentCell.Value = dataGridView1[0, e.RowIndex].Value;//ID를 넣어야 하기때문

                if (m_strToolName == "Item")
                {
                    switch (m_nTab)
                    {
                        case 0:
                            {
                                ((ItemTable)this.Owner).dataGridView1_CellEndEdit(null, null);
                            }
                            break;
                        case 1:
                            {
                                ((ItemTable)this.Owner).dataGridView2_CellEndEdit(null, null);
                            }
                            break;
                        case 2:
                            {
                                ((ItemTable)this.Owner).dataGridView3_CellEndEdit(null, null);
                            }
                            break;
                        case 3:
                            {
                                ((ItemTable)this.Owner).dataGridView4_CellEndEdit(null, null);
                            }
                            break;
                        case 4:
                            {
                                ((ItemTable)this.Owner).dataGridView5_CellEndEdit(null, null);
                            }
                            break;
                        case 5:
                            {
                                ((ItemTable)this.Owner).dataGridView6_CellEndEdit(null, null);
                            }
                            break;
                        case 6:
                            {
                                ((ItemTable)this.Owner).dataGridView7_CellEndEdit(null, null);
                            }
                            break;
                        case 7:
                            {
                                ((ItemTable)this.Owner).dataGridView8_CellEndEdit(null, null);
                            }
                            break;
                        case 8:
                            {
                                ((ItemTable)this.Owner).dataGridView9_CellEndEdit(null, null);
                            }
                            break;
                        case 9:
                            {
                                ((ItemTable)this.Owner).dataGridView10_CellEndEdit(null, null);
                            }
                            break;
                        case 10:
                            {
                                ((ItemTable)this.Owner).dataGridView11_CellEndEdit(null, null);
                            }
                            break;
                        case 11:
                            {
                                ((ItemTable)this.Owner).dataGridView12_CellEndEdit(null, null);
                            }
                            break;
                        case 12:
                            {
                                ((ItemTable)this.Owner).dataGridView13_CellEndEdit(null, null);
                            }
                            break;
                        case 13:
                            {
                                ((ItemTable)this.Owner).dataGridView14_CellEndEdit(null, null);
                            }
                            break;
                        case 14:
                            {
                                ((ItemTable)this.Owner).dataGridView15_CellEndEdit(null, null);
                            }
                            break;

                    }
                }
                else if (m_strToolName == "QuestTable")
                {
                    switch (m_nTab)
                    {
                        case 0:
                            {
                                
                            }
                            break;
                        case 1:
                            {

                            }
                            break;
                    }
                }
                else if (m_strToolName == "TitleTable")
                {
                    switch (m_nTab)
                    {
                        case 0:
                            {
                                ((TitleTable)this.Owner).dataGridView1_CellEndEdit(null, null);
                            }
                            break;
                        case 1:
                            {
                                ((TitleTable)this.Owner).dataGridView2_CellEndEdit(null, null);
                            }
                            break;
                        case 2:
                            {
                                ((TitleTable)this.Owner).dataGridView3_CellEndEdit(null, null);
                            }
                            break;
                    }

                }
                else if (m_strToolName == "SkillTable")
                {
                    switch (m_nTab)
                    {
                        case 0:
                            {
                                ((SkillTable)this.Owner).dataGridView1_CellEndEdit(null, null);
                            }
                            break;                        
                    }

                }
                
                this.Hide();

            }
        }

        
    }
}