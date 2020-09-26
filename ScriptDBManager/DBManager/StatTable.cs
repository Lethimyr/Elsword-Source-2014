using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Data.SqlClient;
using System.IO;
using System.Net;
using System.Collections;

namespace DBManager
{
    public partial class StatTable : Form
    {
        private SqlConnection Con;
        
        object objTemp;
        

        //StatTable_Elsword && History
        private SqlDataAdapter Adpt_Elsword;
        DataTable tblStat_Elsword;               
        private SqlDataAdapter Adpt_Elsword_History;
        DataTable tblStat_Elsword_History;

        //StatTable_Lena && History
        private SqlDataAdapter Adpt_Lena;
        DataTable tblStat_Lena;
        private SqlDataAdapter Adpt_Lena_History;
        DataTable tblStat_Lena_History;

        //StatTable_Aisha && History
        private SqlDataAdapter Adpt_Aisha;
        DataTable tblStat_Aisha;
        private SqlDataAdapter Adpt_Aisha_History;
        DataTable tblStat_Aisha_History;

        //StatTable_Raven && History
        private SqlDataAdapter Adpt_Raven;
        DataTable tblStat_Raven;
        private SqlDataAdapter Adpt_Raven_History;
        DataTable tblStat_Raven_History;

        //StatTable_Eve && History
        private SqlDataAdapter Adpt_Eve;
        DataTable tblStat_Eve;
        private SqlDataAdapter Adpt_Eve_History;
        DataTable tblStat_Eve_History;

        //StatTable_Elsword_Magic && History
        private SqlDataAdapter Adpt_Elsword_Magic;
        DataTable tblStat_Elsword_Magic;
        private SqlDataAdapter Adpt_Elsword_Magic_History;
        DataTable tblStat_Elsword_Magic_History;

        //StatTable_Elsword_Knight && History
        private SqlDataAdapter Adpt_Elsword_Knight;
        DataTable tblStat_Elsword_Knight;
        private SqlDataAdapter Adpt_Elsword_Knight_History;
        DataTable tblStat_Elsword_Knight_History;

        //StatTable_Lena_Combat && History
        private SqlDataAdapter Adpt_Lena_Combat;
        DataTable tblStat_Lena_Combat;
        private SqlDataAdapter Adpt_Lena_Combat_History;
        DataTable tblStat_Lena_Combat_History;

        //StatTable_Lena_Sniper && History
        private SqlDataAdapter Adpt_Lena_Sniper;
        DataTable tblStat_Lena_Sniper;
        private SqlDataAdapter Adpt_Lena_Sniper_History;
        DataTable tblStat_Lena_Sniper_History;

        //StatTable_Aisha_Dark && History
        private SqlDataAdapter Adpt_Aisha_Dark;
        DataTable tblStat_Aisha_Dark;
        private SqlDataAdapter Adpt_Aisha_Dark_History;
        DataTable tblStat_Aisha_Dark_History;

        //StatTable_Aisha_High && History
        private SqlDataAdapter Adpt_Aisha_High;
        DataTable tblStat_Aisha_High;
        private SqlDataAdapter Adpt_Aisha_High_History;
        DataTable tblStat_Aisha_High_History;

        //StatTable_Raven_Soul && History
        private SqlDataAdapter Adpt_Raven_Soul;
        DataTable tblStat_Raven_Soul;
        private SqlDataAdapter Adpt_Raven_Soul_History;
        DataTable tblStat_Raven_Soul_History;

        //StatTable_Raven_Over && History
        private SqlDataAdapter Adpt_Raven_Over;
        DataTable tblStat_Raven_Over;
        private SqlDataAdapter Adpt_Raven_Over_History;
        DataTable tblStat_Raven_Over_History;

        //StatTable_Eve_Exotic && History
        private SqlDataAdapter Adpt_Eve_Exotic;
        DataTable tblStat_Eve_Exotic;
        private SqlDataAdapter Adpt_Eve_Exotic_History;
        DataTable tblStat_Eve_Exotic_History;

        //StatTable_Eve_Archi && History
        private SqlDataAdapter Adpt_Eve_Archi;
        DataTable tblStat_Eve_Archi;
        private SqlDataAdapter Adpt_Eve_Archi_History;
        DataTable tblStat_Eve_Archi_History;

        

        string m_strID;
        ArrayList m_ModifyIDList = new ArrayList();
        public bool m_bSaveEnd = true; //수정했을 시 저장한 후 끝내는지 확인하는 변수


        
        public StatTable()
        {
            InitializeComponent();
        }

        public void SetID(string strID)
        {
            m_strID = strID;
        }
        public int GetCurrentTab() { return tabStatTable.SelectedIndex; }

        private void btnSave_Click(object sender, EventArgs e)
        {
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            string strMyIP = host.AddressList[0].ToString();
            DataRow Row_History = null;
            DataRow Row = null;
            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                
                switch(((sTableValue)m_ModifyIDList[i]).nTabIndex)
                {
                    case 0:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView1[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Elsword_History.Rows.Add();
                            Row_History = tblStat_Elsword_History.Rows[tblStat_Elsword_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 1:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView2[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Lena_History.Rows.Add();
                            Row_History = tblStat_Lena_History.Rows[tblStat_Lena_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 2:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView3[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Aisha_History.Rows.Add();
                            Row_History = tblStat_Aisha_History.Rows[tblStat_Aisha_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 3:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView4[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Elsword_Knight_History.Rows.Add();
                            Row_History = tblStat_Elsword_Knight_History.Rows[tblStat_Elsword_Knight_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 4:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView5[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Elsword_Magic_History.Rows.Add();
                            Row_History = tblStat_Elsword_Magic_History.Rows[tblStat_Elsword_Magic_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 5:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView6[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Lena_Combat_History.Rows.Add();
                            Row_History = tblStat_Lena_Combat_History.Rows[tblStat_Lena_Combat_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 6:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView7[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Lena_Sniper_History.Rows.Add();
                            Row_History = tblStat_Lena_Sniper_History.Rows[tblStat_Lena_Sniper_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 7:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView8[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Aisha_High_History.Rows.Add();
                            Row_History = tblStat_Aisha_High_History.Rows[tblStat_Aisha_High_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 8:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView9[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Aisha_Dark_History.Rows.Add();
                            Row_History = tblStat_Aisha_Dark_History.Rows[tblStat_Aisha_Dark_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 9:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView10[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Raven_History.Rows.Add();
                            Row_History = tblStat_Raven_History.Rows[tblStat_Raven_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 10:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView11[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Raven_Soul_History.Rows.Add();
                            Row_History = tblStat_Raven_Soul_History.Rows[tblStat_Raven_Soul_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 11:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView12[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Raven_Over_History.Rows.Add();
                            Row_History = tblStat_Raven_Over_History.Rows[tblStat_Raven_Over_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 12:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView13[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Eve_History.Rows.Add();
                            Row_History = tblStat_Eve_History.Rows[tblStat_Eve_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 13:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView14[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Eve_Exotic_History.Rows.Add();
                            Row_History = tblStat_Eve_Exotic_History.Rows[tblStat_Eve_Exotic_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                    case 14:
                        {
                            for (int j = 0; j < 8; j++)
                            {
                                dataGridView15[j, ((sTableValue)m_ModifyIDList[i]).nRowIndex].Style.ForeColor = Color.Black;
                            }
                            tblStat_Eve_Archi_History.Rows.Add();
                            Row_History = tblStat_Eve_Archi_History.Rows[tblStat_Eve_Archi_History.Rows.Count - 1];
                            Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                        }
                        break;
                }

                Row_History["IndexKey"] = Row["IndexKey"];
                Row_History["LV"] = Row["LV"];
                Row_History["PhysAtk"] = Row["PhysAtk"];
                Row_History["MagicAtk"] = Row["MagicAtk"];
                Row_History["PhysDef"] = Row["PhysDef"];
                Row_History["MagicDef"] = Row["MagicDef"];
                Row_History["HP"] = Row["HP"];
                Row_History["UserID"] = m_strID;
                Row_History["UserIP"] = strMyIP;
                Row_History["Regdate"] = Row["Regdate"];
            }

            m_ModifyIDList.Clear();
            m_bSaveEnd = true;
            Adpt_Elsword_History.Update(tblStat_Elsword_History);
            Adpt_Elsword.Update(tblStat_Elsword);

            Adpt_Elsword_Knight_History.Update(tblStat_Elsword_Knight_History);
            Adpt_Elsword_Knight.Update(tblStat_Elsword_Knight);

            Adpt_Elsword_Magic_History.Update(tblStat_Elsword_Magic_History);
            Adpt_Elsword_Magic.Update(tblStat_Elsword_Magic);

            Adpt_Lena_History.Update(tblStat_Lena_History);
            Adpt_Lena.Update(tblStat_Lena);

            Adpt_Lena_Combat_History.Update(tblStat_Lena_Combat_History);
            Adpt_Lena_Combat.Update(tblStat_Lena_Combat);

            Adpt_Lena_Sniper_History.Update(tblStat_Lena_Sniper_History);
            Adpt_Lena_Sniper.Update(tblStat_Lena_Sniper);

            Adpt_Aisha_History.Update(tblStat_Aisha_History);
            Adpt_Aisha.Update(tblStat_Aisha);

            Adpt_Aisha_High_History.Update(tblStat_Aisha_High_History);
            Adpt_Aisha_High.Update(tblStat_Aisha_High);

            Adpt_Aisha_Dark_History.Update(tblStat_Aisha_Dark_History);
            Adpt_Aisha_Dark.Update(tblStat_Aisha_Dark);

            Adpt_Raven_History.Update(tblStat_Raven_History);
            Adpt_Raven.Update(tblStat_Raven);

            Adpt_Raven_Soul_History.Update(tblStat_Raven_Soul_History);
            Adpt_Raven_Soul.Update(tblStat_Raven_Soul);

            Adpt_Raven_Over_History.Update(tblStat_Raven_Over_History);
            Adpt_Raven_Over.Update(tblStat_Raven_Over);

            Adpt_Eve_History.Update(tblStat_Eve_History);
            Adpt_Eve.Update(tblStat_Eve);

            Adpt_Eve_Exotic_History.Update(tblStat_Eve_Exotic_History);
            Adpt_Eve_Exotic.Update(tblStat_Eve_Exotic);

            Adpt_Eve_Archi_History.Update(tblStat_Eve_Archi_History);
            Adpt_Eve_Archi.Update(tblStat_Eve_Archi);
            MessageBox.Show("저장되었습니다.");


        }

        private void StatTable_Load(object sender, EventArgs e)
        {
            tabStatTable.SelectedIndex = 0;

            Con = new SqlConnection();
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";
            Con.Open();

            //Elsword_Stat
            Adpt_Elsword = new SqlDataAdapter("SELECT * FROM StatTable_Elsword", Con);
            tblStat_Elsword = new DataTable("tblStat_Elsword");

            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt_Elsword);
            Adpt_Elsword.Fill(tblStat_Elsword);
            dataGridView1.DataSource = tblStat_Elsword;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            
            dataGridView1.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView1.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Elsword_Stat History
            Adpt_Elsword_History = new SqlDataAdapter("SELECT * FROM StatTable_Elsword_History", Con);
            tblStat_Elsword_History = new DataTable("tblStat_Elsword_History");

            SqlCommandBuilder Builder_History = new SqlCommandBuilder(Adpt_Elsword_History);
            Adpt_Elsword_History.Fill(tblStat_Elsword_History);



            //Elsword_Knight_Stat
            Adpt_Elsword_Knight = new SqlDataAdapter("SELECT * FROM StatTable_Elsword_Knight", Con);
            tblStat_Elsword_Knight = new DataTable("tblStat_Elsword_Knight");

            SqlCommandBuilder Elsword_Builder = new SqlCommandBuilder(Adpt_Elsword_Knight);
            Adpt_Elsword_Knight.Fill(tblStat_Elsword_Knight);
            dataGridView4.DataSource = tblStat_Elsword_Knight;
            dataGridView4.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView4.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView4.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Elsword_Knight_Stat History
            Adpt_Elsword_Knight_History = new SqlDataAdapter("SELECT * FROM StatTable_Elsword_Knight_History", Con);
            tblStat_Elsword_Knight_History = new DataTable("tblStat_Elsword_Knight_History");

            SqlCommandBuilder Elsword_Builder_History = new SqlCommandBuilder(Adpt_Elsword_Knight_History);
            Adpt_Elsword_Knight_History.Fill(tblStat_Elsword_Knight_History);



            //Elsword_Magic_Stat
            Adpt_Elsword_Magic = new SqlDataAdapter("SELECT * FROM StatTable_Elsword_Magic", Con);
            tblStat_Elsword_Magic = new DataTable("tblStat_Elsword_Magic");

            SqlCommandBuilder Elsword_Magic_Builder = new SqlCommandBuilder(Adpt_Elsword_Magic);
            Adpt_Elsword_Magic.Fill(tblStat_Elsword_Magic);
            dataGridView5.DataSource = tblStat_Elsword_Magic;
            dataGridView5.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView5.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView5.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Elsword_Magic_Stat History
            Adpt_Elsword_Magic_History = new SqlDataAdapter("SELECT * FROM StatTable_Elsword_Magic_History", Con);
            tblStat_Elsword_Magic_History = new DataTable("tblStat_Elsword_Magic_History");

            SqlCommandBuilder Elsword_Magic_Builder_History = new SqlCommandBuilder(Adpt_Elsword_Magic_History);
            Adpt_Elsword_Magic_History.Fill(tblStat_Elsword_Magic_History);



            //Lena_Stat
            Adpt_Lena = new SqlDataAdapter("SELECT * FROM StatTable_Lena", Con);
            tblStat_Lena = new DataTable("tblStat_Lena");

            SqlCommandBuilder Lena_Builder = new SqlCommandBuilder(Adpt_Lena);
            Adpt_Lena.Fill(tblStat_Lena);
            dataGridView2.DataSource = tblStat_Lena;
            dataGridView2.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView2.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView2.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Lena_Stat History
            Adpt_Lena_History = new SqlDataAdapter("SELECT * FROM StatTable_Lena_History", Con);
            tblStat_Lena_History = new DataTable("tblStat_Lena_History");

            SqlCommandBuilder Lena_Builder_History = new SqlCommandBuilder(Adpt_Lena_History);
            Adpt_Lena_History.Fill(tblStat_Lena_History);



            //Lena_Combat_Stat
            Adpt_Lena_Combat = new SqlDataAdapter("SELECT * FROM StatTable_Lena_Combat", Con);
            tblStat_Lena_Combat = new DataTable("tblStat_Lena_Combat");

            SqlCommandBuilder Lena_Combat_Builder = new SqlCommandBuilder(Adpt_Lena_Combat);
            Adpt_Lena_Combat.Fill(tblStat_Lena_Combat);
            dataGridView6.DataSource = tblStat_Lena_Combat;
            dataGridView6.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView6.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView6.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Lena_Combat_Stat History
            Adpt_Lena_Combat_History = new SqlDataAdapter("SELECT * FROM StatTable_Lena_Combat_History", Con);
            tblStat_Lena_Combat_History = new DataTable("tblStat_Lena_Combat_History");

            SqlCommandBuilder Lena_Combat_Builder_History = new SqlCommandBuilder(Adpt_Lena_Combat_History);
            Adpt_Lena_Combat_History.Fill(tblStat_Lena_Combat_History);



            //Lena_Sniper_Stat
            Adpt_Lena_Sniper = new SqlDataAdapter("SELECT * FROM StatTable_Lena_Sniper", Con);
            tblStat_Lena_Sniper = new DataTable("tblStat_Lena_Sniper");

            SqlCommandBuilder Lena_Sniper_Builder = new SqlCommandBuilder(Adpt_Lena_Sniper);
            Adpt_Lena_Sniper.Fill(tblStat_Lena_Sniper);
            dataGridView7.DataSource = tblStat_Lena_Sniper;
            dataGridView7.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView7.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView7.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Lena_Sniper_Stat History
            Adpt_Lena_Sniper_History = new SqlDataAdapter("SELECT * FROM StatTable_Lena_Sniper_History", Con);
            tblStat_Lena_Sniper_History = new DataTable("tblStat_Lena_Sniper_History");

            SqlCommandBuilder Lena_Sniper_Builder_History = new SqlCommandBuilder(Adpt_Lena_Sniper_History);
            Adpt_Lena_Sniper_History.Fill(tblStat_Lena_Sniper_History);



            //Aisha_Stat
            Adpt_Aisha = new SqlDataAdapter("SELECT * FROM StatTable_Aisha", Con);
            tblStat_Aisha = new DataTable("tblStat_Aisha");

            SqlCommandBuilder Aisha_Builder = new SqlCommandBuilder(Adpt_Aisha);
            Adpt_Aisha.Fill(tblStat_Aisha);
            dataGridView3.DataSource = tblStat_Aisha;
            dataGridView3.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView3.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView3.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Aisha_Stat History
            Adpt_Aisha_History = new SqlDataAdapter("SELECT * FROM StatTable_Aisha_History", Con);
            tblStat_Aisha_History = new DataTable("tblStat_Aisha_History");

            SqlCommandBuilder Aisha_Builder_History = new SqlCommandBuilder(Adpt_Aisha_History);
            Adpt_Aisha_History.Fill(tblStat_Aisha_History);



            //Aisha_High_Stat
            Adpt_Aisha_High = new SqlDataAdapter("SELECT * FROM StatTable_Aisha_High", Con);
            tblStat_Aisha_High = new DataTable("tblStat_Aisha_High");

            SqlCommandBuilder Aisha_High_Builder = new SqlCommandBuilder(Adpt_Aisha_High);
            Adpt_Aisha_High.Fill(tblStat_Aisha_High);
            dataGridView8.DataSource = tblStat_Aisha_High;
            dataGridView8.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView8.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView8.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Aisha_High_Stat History
            Adpt_Aisha_High_History = new SqlDataAdapter("SELECT * FROM StatTable_Aisha_High_History", Con);
            tblStat_Aisha_High_History = new DataTable("tblStat_Aisha_High_History");

            SqlCommandBuilder Aisha_High_Builder_History = new SqlCommandBuilder(Adpt_Aisha_High_History);
            Adpt_Aisha_High_History.Fill(tblStat_Aisha_High_History);



            //Aisha_Dark_Stat
            Adpt_Aisha_Dark = new SqlDataAdapter("SELECT * FROM StatTable_Aisha_Dark", Con);
            tblStat_Aisha_Dark = new DataTable("tblStat_Aisha_Dark");

            SqlCommandBuilder Aisha_Dark_Builder = new SqlCommandBuilder(Adpt_Aisha_Dark);
            Adpt_Aisha_Dark.Fill(tblStat_Aisha_Dark);
            dataGridView9.DataSource = tblStat_Aisha_Dark;
            dataGridView9.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView9.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView9.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Aisha_Dark_Stat History
            Adpt_Aisha_Dark_History = new SqlDataAdapter("SELECT * FROM StatTable_Aisha_Dark_History", Con);
            tblStat_Aisha_Dark_History = new DataTable("tblStat_Aisha_Dark_History");

            SqlCommandBuilder Aisha_Dark_Builder_History = new SqlCommandBuilder(Adpt_Aisha_Dark_History);
            Adpt_Aisha_Dark_History.Fill(tblStat_Aisha_Dark_History);



            //Raven_Stat
            Adpt_Raven = new SqlDataAdapter("SELECT * FROM StatTable_Raven", Con);
            tblStat_Raven = new DataTable("tblStat_Raven");

            SqlCommandBuilder Raven_Builder = new SqlCommandBuilder(Adpt_Raven);
            Adpt_Raven.Fill(tblStat_Raven);
            dataGridView10.DataSource = tblStat_Raven;
            dataGridView10.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView10.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView10.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Raven_Stat History
            Adpt_Raven_History = new SqlDataAdapter("SELECT * FROM StatTable_Raven_History", Con);
            tblStat_Raven_History = new DataTable("tblStat_Raven_History");

            SqlCommandBuilder Raven_Builder_History = new SqlCommandBuilder(Adpt_Raven_History);
            Adpt_Raven_History.Fill(tblStat_Raven_History);



            //Raven_Soul_Stat
            Adpt_Raven_Soul = new SqlDataAdapter("SELECT * FROM StatTable_Raven_Soul", Con);
            tblStat_Raven_Soul = new DataTable("tblStat_Raven_Soul");

            SqlCommandBuilder Raven_Soul_Builder = new SqlCommandBuilder(Adpt_Raven_Soul);
            Adpt_Raven_Soul.Fill(tblStat_Raven_Soul);
            dataGridView11.DataSource = tblStat_Raven_Soul;
            dataGridView11.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView11.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView11.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Raven_Soul_Stat History
            Adpt_Raven_Soul_History = new SqlDataAdapter("SELECT * FROM StatTable_Raven_Soul_History", Con);
            tblStat_Raven_Soul_History = new DataTable("tblStat_Raven_Soul_History");

            SqlCommandBuilder Raven_Soul_Builder_History = new SqlCommandBuilder(Adpt_Raven_Soul_History);
            Adpt_Raven_Soul_History.Fill(tblStat_Raven_Soul_History);



            //Raven_Over_Stat
            Adpt_Raven_Over = new SqlDataAdapter("SELECT * FROM StatTable_Raven_Over", Con);
            tblStat_Raven_Over = new DataTable("tblStat_Raven_Over");

            SqlCommandBuilder Raven_Over_Builder = new SqlCommandBuilder(Adpt_Raven_Over);
            Adpt_Raven_Over.Fill(tblStat_Raven_Over);
            dataGridView12.DataSource = tblStat_Raven_Over;
            dataGridView12.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView12.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView12.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Raven_Over_Stat History
            Adpt_Raven_Over_History = new SqlDataAdapter("SELECT * FROM StatTable_Raven_Over_History", Con);
            tblStat_Raven_Over_History = new DataTable("tblStat_Raven_Over_History");

            SqlCommandBuilder Raven_Over_Builder_History = new SqlCommandBuilder(Adpt_Raven_Over_History);
            Adpt_Raven_Over_History.Fill(tblStat_Raven_Over_History);



            //Eve_Stat
            Adpt_Eve = new SqlDataAdapter("SELECT * FROM StatTable_Eve", Con);
            tblStat_Eve = new DataTable("tblStat_Eve");

            SqlCommandBuilder Eve_Builder = new SqlCommandBuilder(Adpt_Eve);
            Adpt_Eve.Fill(tblStat_Eve);
            dataGridView13.DataSource = tblStat_Eve;
            dataGridView13.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView13.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView13.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Eve_Stat History
            Adpt_Eve_History = new SqlDataAdapter("SELECT * FROM StatTable_Eve_History", Con);
            tblStat_Eve_History = new DataTable("tblStat_Eve_History");

            SqlCommandBuilder Eve_Builder_History = new SqlCommandBuilder(Adpt_Eve_History);
            Adpt_Eve_History.Fill(tblStat_Eve_History);



            //Eve_Archi_Stat
            Adpt_Eve_Archi = new SqlDataAdapter("SELECT * FROM StatTable_Eve_Archi", Con);
            tblStat_Eve_Archi = new DataTable("tblStat_Eve_Archi");

            SqlCommandBuilder Eve_Archi_Builder = new SqlCommandBuilder(Adpt_Eve_Archi);
            Adpt_Eve_Archi.Fill(tblStat_Eve_Archi);
            dataGridView15.DataSource = tblStat_Eve_Archi;
            dataGridView15.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView15.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView15.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Eve_Archi_Stat History
            Adpt_Eve_Archi_History = new SqlDataAdapter("SELECT * FROM StatTable_Eve_Archi_History", Con);
            tblStat_Eve_Archi_History = new DataTable("tblStat_Eve_Archi_History");

            SqlCommandBuilder Eve_Archi_Builder_History = new SqlCommandBuilder(Adpt_Eve_Archi_History);
            Adpt_Eve_Archi_History.Fill(tblStat_Eve_Archi_History);



            //Eve_Exotic_Stat
            Adpt_Eve_Exotic = new SqlDataAdapter("SELECT * FROM StatTable_Eve_Exotic", Con);
            tblStat_Eve_Exotic = new DataTable("tblStat_Eve_Exotic");

            SqlCommandBuilder Eve_Exotic_Builder = new SqlCommandBuilder(Adpt_Eve_Exotic);
            Adpt_Eve_Exotic.Fill(tblStat_Eve_Exotic);
            dataGridView14.DataSource = tblStat_Eve_Exotic;
            dataGridView14.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            dataGridView14.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView14.Columns[7].ReadOnly = true; //Reg_data수정방지


            //Eve_Exotic_Stat History
            Adpt_Eve_Exotic_History = new SqlDataAdapter("SELECT * FROM StatTable_Eve_Exotic_History", Con);
            tblStat_Eve_Exotic_History = new DataTable("tblStat_Eve_Exotic_History");

            SqlCommandBuilder Eve_Exotic_Builder_History = new SqlCommandBuilder(Adpt_Eve_Exotic_History);
            Adpt_Eve_Exotic_History.Fill(tblStat_Eve_Exotic_History);       

        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            switch (tabStatTable.SelectedIndex)
            {
                case 0:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView1);
                        frm.SetDataTable(tblStat_Elsword);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView1.CurrentCell.RowIndex;
                        frm.SetData(dataGridView1[1, nRowIndex].Value.ToString(), dataGridView1[2, nRowIndex].Value.ToString(), dataGridView1[3, nRowIndex].Value.ToString(), dataGridView1[4, nRowIndex].Value.ToString(), dataGridView1[5, nRowIndex].Value.ToString(), dataGridView1[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 1:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView2);
                        frm.SetDataTable(tblStat_Lena);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView2.CurrentCell.RowIndex;
                        frm.SetData(dataGridView2[1, nRowIndex].Value.ToString(), dataGridView2[2, nRowIndex].Value.ToString(), dataGridView2[3, nRowIndex].Value.ToString(), dataGridView2[4, nRowIndex].Value.ToString(), dataGridView2[5, nRowIndex].Value.ToString(), dataGridView2[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 2:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView3);
                        frm.SetDataTable(tblStat_Aisha);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView3.CurrentCell.RowIndex;
                        frm.SetData(dataGridView3[1, nRowIndex].Value.ToString(), dataGridView3[2, nRowIndex].Value.ToString(), dataGridView3[3, nRowIndex].Value.ToString(), dataGridView3[4, nRowIndex].Value.ToString(), dataGridView3[5, nRowIndex].Value.ToString(), dataGridView3[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 3:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView4);
                        frm.SetDataTable(tblStat_Elsword_Knight);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView4.CurrentCell.RowIndex;
                        frm.SetData(dataGridView4[1, nRowIndex].Value.ToString(), dataGridView4[2, nRowIndex].Value.ToString(), dataGridView4[3, nRowIndex].Value.ToString(), dataGridView4[4, nRowIndex].Value.ToString(), dataGridView4[5, nRowIndex].Value.ToString(), dataGridView4[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 4:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView5);
                        frm.SetDataTable(tblStat_Elsword_Magic);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView5.CurrentCell.RowIndex;
                        frm.SetData(dataGridView5[1, nRowIndex].Value.ToString(), dataGridView5[2, nRowIndex].Value.ToString(), dataGridView5[3, nRowIndex].Value.ToString(), dataGridView5[4, nRowIndex].Value.ToString(), dataGridView5[5, nRowIndex].Value.ToString(), dataGridView5[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 5:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView6);
                        frm.SetDataTable(tblStat_Lena_Combat);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView6.CurrentCell.RowIndex;
                        frm.SetData(dataGridView6[1, nRowIndex].Value.ToString(), dataGridView6[2, nRowIndex].Value.ToString(), dataGridView6[3, nRowIndex].Value.ToString(), dataGridView6[4, nRowIndex].Value.ToString(), dataGridView6[5, nRowIndex].Value.ToString(), dataGridView6[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 6:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView7);
                        frm.SetDataTable(tblStat_Lena_Sniper);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView7.CurrentCell.RowIndex;
                        frm.SetData(dataGridView7[1, nRowIndex].Value.ToString(), dataGridView7[2, nRowIndex].Value.ToString(), dataGridView7[3, nRowIndex].Value.ToString(), dataGridView7[4, nRowIndex].Value.ToString(), dataGridView7[5, nRowIndex].Value.ToString(), dataGridView7[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 7:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView8);
                        frm.SetDataTable(tblStat_Aisha_High);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView8.CurrentCell.RowIndex;
                        frm.SetData(dataGridView8[1, nRowIndex].Value.ToString(), dataGridView8[2, nRowIndex].Value.ToString(), dataGridView8[3, nRowIndex].Value.ToString(), dataGridView8[4, nRowIndex].Value.ToString(), dataGridView8[5, nRowIndex].Value.ToString(), dataGridView8[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 8:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView9);
                        frm.SetDataTable(tblStat_Aisha_Dark);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView9.CurrentCell.RowIndex;
                        frm.SetData(dataGridView9[1, nRowIndex].Value.ToString(), dataGridView9[2, nRowIndex].Value.ToString(), dataGridView9[3, nRowIndex].Value.ToString(), dataGridView9[4, nRowIndex].Value.ToString(), dataGridView9[5, nRowIndex].Value.ToString(), dataGridView9[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 9:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView10);
                        frm.SetDataTable(tblStat_Raven);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView10.CurrentCell.RowIndex;
                        frm.SetData(dataGridView10[1, nRowIndex].Value.ToString(), dataGridView10[2, nRowIndex].Value.ToString(), dataGridView10[3, nRowIndex].Value.ToString(), dataGridView10[4, nRowIndex].Value.ToString(), dataGridView10[5, nRowIndex].Value.ToString(), dataGridView10[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 10:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView11);
                        frm.SetDataTable(tblStat_Raven_Soul);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView11.CurrentCell.RowIndex;
                        frm.SetData(dataGridView11[1, nRowIndex].Value.ToString(), dataGridView11[2, nRowIndex].Value.ToString(), dataGridView11[3, nRowIndex].Value.ToString(), dataGridView11[4, nRowIndex].Value.ToString(), dataGridView11[5, nRowIndex].Value.ToString(), dataGridView11[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 11:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView12);
                        frm.SetDataTable(tblStat_Raven_Over);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView12.CurrentCell.RowIndex;
                        frm.SetData(dataGridView12[1, nRowIndex].Value.ToString(), dataGridView12[2, nRowIndex].Value.ToString(), dataGridView12[3, nRowIndex].Value.ToString(), dataGridView12[4, nRowIndex].Value.ToString(), dataGridView12[5, nRowIndex].Value.ToString(), dataGridView12[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 12:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView13);
                        frm.SetDataTable(tblStat_Eve);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView13.CurrentCell.RowIndex;
                        frm.SetData(dataGridView13[1, nRowIndex].Value.ToString(), dataGridView13[2, nRowIndex].Value.ToString(), dataGridView13[3, nRowIndex].Value.ToString(), dataGridView13[4, nRowIndex].Value.ToString(), dataGridView13[5, nRowIndex].Value.ToString(), dataGridView13[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 13:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView14);
                        frm.SetDataTable(tblStat_Eve_Exotic);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView14.CurrentCell.RowIndex;
                        frm.SetData(dataGridView14[1, nRowIndex].Value.ToString(), dataGridView14[2, nRowIndex].Value.ToString(), dataGridView14[3, nRowIndex].Value.ToString(), dataGridView14[4, nRowIndex].Value.ToString(), dataGridView14[5, nRowIndex].Value.ToString(), dataGridView14[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;
                case 14:
                    {
                        StatTableAdd frm = new StatTableAdd();
                        frm.SetDataGridView(dataGridView15);
                        frm.SetDataTable(tblStat_Eve_Archi);
                        frm.SetModifyArray(m_ModifyIDList);
                        int nRowIndex = dataGridView15.CurrentCell.RowIndex;
                        frm.SetData(dataGridView15[1, nRowIndex].Value.ToString(), dataGridView15[2, nRowIndex].Value.ToString(), dataGridView15[3, nRowIndex].Value.ToString(), dataGridView15[4, nRowIndex].Value.ToString(), dataGridView15[5, nRowIndex].Value.ToString(), dataGridView15[6, nRowIndex].Value.ToString());
                        frm.Owner = this;
                        frm.ShowDialog(this);
                    }
                    break;

            }

        }

        private void btnHistory_Click(object sender, EventArgs e)
        {
            switch(tabStatTable.SelectedIndex)
            {
                case 0 : 
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Elsword_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 1:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Lena_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 2:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Aisha_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 3:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Elsword_Knight_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 4:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Elsword_Magic_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 5:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Lena_Combat_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 6:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Lena_Sniper_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 7:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Aisha_High_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 8:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Aisha_Dark_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 9:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Raven_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 10:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Raven_Soul_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 11:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Raven_Over_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 12:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Eve_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 13:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Eve_Exotic_History);
                        frm.ShowDialog(this);
                    }
                    break;
                case 14:
                    {
                        History frm = new History();
                        frm.SetHistoryTable(tblStat_Eve_Archi_History);
                        frm.ShowDialog(this);
                    }
                    break;

            }
        }

        private void tabStatTable_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                switch (tabStatTable.SelectedIndex)
                {
                    case 0:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView1);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 1:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView2);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 2:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView3);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 3:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView4);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 4:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView5);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 5:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView6);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 6:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView7);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 7:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView8);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 8:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView9);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 9:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView10);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 10:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView11);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 11:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView12);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 12:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView13);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 13:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView14);
                            frm.ShowDialog(this);
                        }
                        break;
                    case 14:
                        {
                            Search frm = new Search();
                            frm.SetDataGridView(dataGridView15);
                            frm.ShowDialog(this);
                        }
                        break;
                }                
            }  
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView1.CurrentCell.Value;      
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView1.CurrentCell.Value))
            {
                string strOldTime = dataGridView1[7, dataGridView1.CurrentCell.RowIndex].Value.ToString();

                dataGridView1.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView1[7, dataGridView1.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView1[7, dataGridView1.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView1.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        if (dataGridView1[1, i].Value.ToString() == dataGridView1.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView1.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView1.CurrentCell.Value = objTemp;
                            dataGridView1.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView1[7, dataGridView1.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView1[7, dataGridView1.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }
                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView1.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 0;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView1["IndexKey", dataGridView1.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }
        }

        private void dataGridView2_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView2.CurrentCell.Value;      
        }

        private void dataGridView2_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView2.CurrentCell.Value))
            {
                string strOldTime = dataGridView2[7, dataGridView2.CurrentCell.RowIndex].Value.ToString();

                dataGridView2.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView2[7, dataGridView2.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView2[7, dataGridView2.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView2.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView2.RowCount; i++)
                    {
                        if (dataGridView2[1, i].Value.ToString() == dataGridView2.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView2.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView2.CurrentCell.Value = objTemp;
                            dataGridView2.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView2[7, dataGridView2.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView2[7, dataGridView2.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView2.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 1;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView2["IndexKey", dataGridView2.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }
            
        }

        private void dataGridView3_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView3.CurrentCell.Value;      
        }

        private void dataGridView3_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView3.CurrentCell.Value))
            {
                string strOldTime = dataGridView3[7, dataGridView3.CurrentCell.RowIndex].Value.ToString();

                dataGridView3.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView3[7, dataGridView3.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView3[7, dataGridView3.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView3.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView3.RowCount; i++)
                    {
                        if (dataGridView3[1, i].Value.ToString() == dataGridView3.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView3.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView3.CurrentCell.Value = objTemp;
                            dataGridView3.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView3[7, dataGridView3.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView3[7, dataGridView3.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView3.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 2;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView3["IndexKey", dataGridView3.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void dataGridView4_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView4.CurrentCell.Value;
        }

        private void dataGridView4_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView4.CurrentCell.Value))
            {
                string strOldTime = dataGridView4[7, dataGridView4.CurrentCell.RowIndex].Value.ToString();

                dataGridView4.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView4[7, dataGridView4.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView4[7, dataGridView4.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView4.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView4.RowCount; i++)
                    {
                        if (dataGridView4[1, i].Value.ToString() == dataGridView4.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView4.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView4.CurrentCell.Value = objTemp;
                            dataGridView4.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView4[7, dataGridView4.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView4[7, dataGridView4.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView4.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 3;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView4["IndexKey", dataGridView4.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void dataGridView5_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView5.CurrentCell.Value;
        }

        private void dataGridView5_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView5.CurrentCell.Value))
            {
                string strOldTime = dataGridView5[7, dataGridView5.CurrentCell.RowIndex].Value.ToString();

                dataGridView5.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView5[7, dataGridView5.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView5[7, dataGridView5.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView5.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView5.RowCount; i++)
                    {
                        if (dataGridView5[1, i].Value.ToString() == dataGridView5.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView5.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView5.CurrentCell.Value = objTemp;
                            dataGridView5.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView5[7, dataGridView5.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView5[7, dataGridView5.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView5.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 4;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView5["IndexKey", dataGridView5.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void dataGridView6_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView6.CurrentCell.Value;
        }

        private void dataGridView6_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView6.CurrentCell.Value))
            {
                string strOldTime = dataGridView6[7, dataGridView6.CurrentCell.RowIndex].Value.ToString();

                dataGridView6.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView6[7, dataGridView6.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView6[7, dataGridView6.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView6.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView6.RowCount; i++)
                    {
                        if (dataGridView6[1, i].Value.ToString() == dataGridView6.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView6.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView6.CurrentCell.Value = objTemp;
                            dataGridView6.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView6[7, dataGridView6.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView6[7, dataGridView6.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView6.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 5;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView6["IndexKey", dataGridView6.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void dataGridView7_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView7.CurrentCell.Value;
        }

        private void dataGridView7_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView7.CurrentCell.Value))
            {
                string strOldTime = dataGridView7[7, dataGridView7.CurrentCell.RowIndex].Value.ToString();

                dataGridView7.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView7[7, dataGridView7.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView7[7, dataGridView7.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView7.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView7.RowCount; i++)
                    {
                        if (dataGridView7[1, i].Value.ToString() == dataGridView7.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView7.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView7.CurrentCell.Value = objTemp;
                            dataGridView7.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView7[7, dataGridView7.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView7[7, dataGridView7.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView7.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 6;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView7["IndexKey", dataGridView7.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void dataGridView8_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView8.CurrentCell.Value;
        }

        private void dataGridView8_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView8.CurrentCell.Value))
            {
                string strOldTime = dataGridView8[7, dataGridView8.CurrentCell.RowIndex].Value.ToString();

                dataGridView8.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView8[7, dataGridView8.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView8[7, dataGridView8.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView8.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView8.RowCount; i++)
                    {
                        if (dataGridView8[1, i].Value.ToString() == dataGridView8.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView8.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView8.CurrentCell.Value = objTemp;
                            dataGridView8.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView8[7, dataGridView8.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView8[7, dataGridView8.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView8.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 7;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView8["IndexKey", dataGridView8.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void dataGridView9_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView9.CurrentCell.Value;
        }

        private void dataGridView9_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView9.CurrentCell.Value))
            {
                string strOldTime = dataGridView9[7, dataGridView9.CurrentCell.RowIndex].Value.ToString();

                dataGridView9.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView9[7, dataGridView9.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView9[7, dataGridView9.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView9.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView9.RowCount; i++)
                    {
                        if (dataGridView9[1, i].Value.ToString() == dataGridView9.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView9.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView9.CurrentCell.Value = objTemp;
                            dataGridView9.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView9[7, dataGridView9.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView9[7, dataGridView9.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView9.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 8;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView9["IndexKey", dataGridView9.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void dataGridView10_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView10.CurrentCell.Value;
        }

        private void dataGridView10_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView10.CurrentCell.Value))
            {
                string strOldTime = dataGridView10[7, dataGridView10.CurrentCell.RowIndex].Value.ToString();

                dataGridView10.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView10[7, dataGridView10.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView10[7, dataGridView10.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView10.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView10.RowCount; i++)
                    {
                        if (dataGridView10[1, i].Value.ToString() == dataGridView10.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView10.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView10.CurrentCell.Value = objTemp;
                            dataGridView10.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView10[7, dataGridView10.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView10[7, dataGridView10.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView10.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 9;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView10["IndexKey", dataGridView10.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }
        }

        private void dataGridView11_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView11.CurrentCell.Value;
        }

        private void dataGridView11_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView11.CurrentCell.Value))
            {
                string strOldTime = dataGridView11[7, dataGridView11.CurrentCell.RowIndex].Value.ToString();

                dataGridView11.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView11[7, dataGridView11.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView11[7, dataGridView11.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView11.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView11.RowCount; i++)
                    {
                        if (dataGridView11[1, i].Value.ToString() == dataGridView11.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView11.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView11.CurrentCell.Value = objTemp;
                            dataGridView11.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView11[7, dataGridView11.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView11[7, dataGridView11.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView11.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 10;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView11["IndexKey", dataGridView11.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }
        }

        private void dataGridView12_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView12.CurrentCell.Value;
        }

        private void dataGridView12_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView12.CurrentCell.Value))
            {
                string strOldTime = dataGridView12[7, dataGridView12.CurrentCell.RowIndex].Value.ToString();

                dataGridView12.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView12[7, dataGridView12.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView12[7, dataGridView12.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView12.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView12.RowCount; i++)
                    {
                        if (dataGridView12[1, i].Value.ToString() == dataGridView12.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView12.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView12.CurrentCell.Value = objTemp;
                            dataGridView12.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView12[7, dataGridView12.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView12[7, dataGridView12.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView12.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 11;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView12["IndexKey", dataGridView12.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }
        }

        private void dataGridView13_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView13.CurrentCell.Value;
        }

        private void dataGridView13_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView13.CurrentCell.Value))
            {
                string strOldTime = dataGridView13[7, dataGridView13.CurrentCell.RowIndex].Value.ToString();

                dataGridView13.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView13[7, dataGridView13.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView13[7, dataGridView13.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView13.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView13.RowCount; i++)
                    {
                        if (dataGridView13[1, i].Value.ToString() == dataGridView13.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView13.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView13.CurrentCell.Value = objTemp;
                            dataGridView13.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView13[7, dataGridView13.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView13[7, dataGridView13.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView13.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 12;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView13["IndexKey", dataGridView13.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }
        }

        private void dataGridView14_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView14.CurrentCell.Value;
        }

        private void dataGridView14_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView14.CurrentCell.Value))
            {
                string strOldTime = dataGridView14[7, dataGridView14.CurrentCell.RowIndex].Value.ToString();

                dataGridView14.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView14[7, dataGridView14.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView14[7, dataGridView14.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView14.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView14.RowCount; i++)
                    {
                        if (dataGridView14[1, i].Value.ToString() == dataGridView14.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView14.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView14.CurrentCell.Value = objTemp;
                            dataGridView14.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView14[7, dataGridView14.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView14[7, dataGridView14.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView14.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 13;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView14["IndexKey", dataGridView14.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }
        }

        private void dataGridView15_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView15.CurrentCell.Value;
        }

        private void dataGridView15_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView15.CurrentCell.Value))
            {
                string strOldTime = dataGridView15[7, dataGridView15.CurrentCell.RowIndex].Value.ToString();

                dataGridView15.CurrentCell.Style.ForeColor = Color.Red;
                dataGridView15[7, dataGridView15.CurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView15[7, dataGridView15.CurrentCell.RowIndex].Style.ForeColor = Color.Red;

                if (dataGridView15.CurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView15.RowCount; i++)
                    {
                        if (dataGridView15[1, i].Value.ToString() == dataGridView15.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView15.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 LV값이 존재합니다.");

                            dataGridView15.CurrentCell.Value = objTemp;
                            dataGridView15.CurrentCell.Style.ForeColor = Color.Black;
                            dataGridView15[7, dataGridView15.CurrentCell.RowIndex].Value = strOldTime;
                            dataGridView15[7, dataGridView15.CurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = dataGridView15.CurrentCell.RowIndex;
                sModiftIDList.nTabIndex = 14;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView15["IndexKey", dataGridView15.CurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(sModiftIDList);
                m_bSaveEnd = false;
            }

        }

        private void StatTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Con.Close();
        }

        private void StatTable_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    DataRow Row_History = null;
                    DataRow Row = null;
                    for (int i = 0; i < m_ModifyIDList.Count; i++)
                    {

                        switch (((sTableValue)m_ModifyIDList[i]).nTabIndex)
                        {
                            case 0:
                                {
                                    tblStat_Elsword_History.Rows.Add();
                                    Row_History = tblStat_Elsword_History.Rows[tblStat_Elsword_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 1:
                                {
                                    tblStat_Lena_History.Rows.Add();
                                    Row_History = tblStat_Lena_History.Rows[tblStat_Lena_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 2:
                                {
                                    tblStat_Aisha_History.Rows.Add();
                                    Row_History = tblStat_Aisha_History.Rows[tblStat_Aisha_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 3:
                                {
                                    tblStat_Elsword_Knight_History.Rows.Add();
                                    Row_History = tblStat_Elsword_Knight_History.Rows[tblStat_Elsword_Knight_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 4:
                                {
                                    tblStat_Elsword_Magic_History.Rows.Add();
                                    Row_History = tblStat_Elsword_Magic_History.Rows[tblStat_Elsword_Magic_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 5:
                                {
                                    tblStat_Lena_Combat_History.Rows.Add();
                                    Row_History = tblStat_Lena_Combat_History.Rows[tblStat_Lena_Combat_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 6:
                                {
                                    tblStat_Lena_Sniper_History.Rows.Add();
                                    Row_History = tblStat_Lena_Sniper_History.Rows[tblStat_Lena_Sniper_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 7:
                                {
                                    tblStat_Aisha_High_History.Rows.Add();
                                    Row_History = tblStat_Aisha_High_History.Rows[tblStat_Aisha_High_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 8:
                                {
                                    tblStat_Aisha_Dark_History.Rows.Add();
                                    Row_History = tblStat_Aisha_Dark_History.Rows[tblStat_Aisha_Dark_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 9:
                                {
                                    tblStat_Raven_History.Rows.Add();
                                    Row_History = tblStat_Raven_History.Rows[tblStat_Raven_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 10:
                                {                                    
                                    tblStat_Raven_Soul_History.Rows.Add();
                                    Row_History = tblStat_Raven_Soul_History.Rows[tblStat_Raven_Soul_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 11:
                                {                                 
                                    tblStat_Raven_Over_History.Rows.Add();
                                    Row_History = tblStat_Raven_Over_History.Rows[tblStat_Raven_Over_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 12:
                                {                                    
                                    tblStat_Eve_History.Rows.Add();
                                    Row_History = tblStat_Eve_History.Rows[tblStat_Eve_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 13:
                                {                                    
                                    tblStat_Eve_Exotic_History.Rows.Add();
                                    Row_History = tblStat_Eve_Exotic_History.Rows[tblStat_Eve_Exotic_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 14:
                                {                                   
                                    tblStat_Eve_Archi_History.Rows.Add();
                                    Row_History = tblStat_Eve_Archi_History.Rows[tblStat_Eve_Archi_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                        }

                        Row_History["IndexKey"] = Row["IndexKey"];
                        Row_History["LV"] = Row["LV"];
                        Row_History["PhysAtk"] = Row["PhysAtk"];
                        Row_History["MagicAtk"] = Row["MagicAtk"];
                        Row_History["PhysDef"] = Row["PhysDef"];
                        Row_History["MagicDef"] = Row["MagicDef"];
                        Row_History["HP"] = Row["HP"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                    }
                                       
             
                    Adpt_Elsword_History.Update(tblStat_Elsword_History);
                    Adpt_Elsword.Update(tblStat_Elsword);

                    Adpt_Elsword_Knight_History.Update(tblStat_Elsword_Knight_History);
                    Adpt_Elsword_Knight.Update(tblStat_Elsword_Knight);

                    Adpt_Elsword_Magic_History.Update(tblStat_Elsword_Magic_History);
                    Adpt_Elsword_Magic.Update(tblStat_Elsword_Magic);

                    Adpt_Lena_History.Update(tblStat_Lena_History);
                    Adpt_Lena.Update(tblStat_Lena);

                    Adpt_Lena_Combat_History.Update(tblStat_Lena_Combat_History);
                    Adpt_Lena_Combat.Update(tblStat_Lena_Combat);

                    Adpt_Lena_Sniper_History.Update(tblStat_Lena_Sniper_History);
                    Adpt_Lena_Sniper.Update(tblStat_Lena_Sniper);

                    Adpt_Aisha_History.Update(tblStat_Aisha_History);
                    Adpt_Aisha.Update(tblStat_Aisha);

                    Adpt_Aisha_High_History.Update(tblStat_Aisha_High_History);
                    Adpt_Aisha_High.Update(tblStat_Aisha_High);

                    Adpt_Aisha_Dark_History.Update(tblStat_Aisha_Dark_History);
                    Adpt_Aisha_Dark.Update(tblStat_Aisha_Dark);

                    Adpt_Raven_History.Update(tblStat_Raven_History);
                    Adpt_Raven.Update(tblStat_Raven);

                    Adpt_Raven_Soul_History.Update(tblStat_Raven_Soul_History);
                    Adpt_Raven_Soul.Update(tblStat_Raven_Soul);

                    Adpt_Raven_Over_History.Update(tblStat_Raven_Over_History);
                    Adpt_Raven_Over.Update(tblStat_Raven_Over);

                    Adpt_Eve_History.Update(tblStat_Eve_History);
                    Adpt_Eve.Update(tblStat_Eve);

                    Adpt_Eve_Exotic_History.Update(tblStat_Eve_Exotic_History);
                    Adpt_Eve_Exotic.Update(tblStat_Eve_Exotic);

                    Adpt_Eve_Archi_History.Update(tblStat_Eve_Archi_History);
                    Adpt_Eve_Archi.Update(tblStat_Eve_Archi);
                    MessageBox.Show("저장되었습니다.");
                    
                }
                m_ModifyIDList.Clear();
                m_bSaveEnd = true;                    
            }

        }

        private void FileGenerate_Click(object sender, EventArgs e)
        {
            if (m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("파일을 생성하시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.Cancel)
                {
                    return;
                }
            }
            else
            {
                DialogResult result = MessageBox.Show("저장되지 않았습니다. 저장한 후 파일을 생성하시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    DataRow Row_History = null;
                    DataRow Row = null;
                    for (int i = 0; i < m_ModifyIDList.Count; i++)
                    {

                        switch (((sTableValue)m_ModifyIDList[i]).nTabIndex)
                        {
                            case 0:
                                {
                                    tblStat_Elsword_History.Rows.Add();
                                    Row_History = tblStat_Elsword_History.Rows[tblStat_Elsword_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 1:
                                {
                                    tblStat_Lena_History.Rows.Add();
                                    Row_History = tblStat_Lena_History.Rows[tblStat_Lena_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 2:
                                {
                                    tblStat_Aisha_History.Rows.Add();
                                    Row_History = tblStat_Aisha_History.Rows[tblStat_Aisha_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 3:
                                {
                                    tblStat_Elsword_Knight_History.Rows.Add();
                                    Row_History = tblStat_Elsword_Knight_History.Rows[tblStat_Elsword_Knight_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 4:
                                {
                                    tblStat_Elsword_Magic_History.Rows.Add();
                                    Row_History = tblStat_Elsword_Magic_History.Rows[tblStat_Elsword_Magic_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 5:
                                {
                                    tblStat_Lena_Combat_History.Rows.Add();
                                    Row_History = tblStat_Lena_Combat_History.Rows[tblStat_Lena_Combat_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 6:
                                {
                                    tblStat_Lena_Sniper_History.Rows.Add();
                                    Row_History = tblStat_Lena_Sniper_History.Rows[tblStat_Lena_Sniper_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 7:
                                {
                                    tblStat_Aisha_High_History.Rows.Add();
                                    Row_History = tblStat_Aisha_High_History.Rows[tblStat_Aisha_High_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 8:
                                {
                                    tblStat_Aisha_Dark_History.Rows.Add();
                                    Row_History = tblStat_Aisha_Dark_History.Rows[tblStat_Aisha_Dark_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 9:
                                {
                                    tblStat_Raven_History.Rows.Add();
                                    Row_History = tblStat_Raven_History.Rows[tblStat_Raven_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 10:
                                {                                    
                                    tblStat_Raven_Soul_History.Rows.Add();
                                    Row_History = tblStat_Raven_Soul_History.Rows[tblStat_Raven_Soul_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 11:
                                {                                 
                                    tblStat_Raven_Over_History.Rows.Add();
                                    Row_History = tblStat_Raven_Over_History.Rows[tblStat_Raven_Over_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 12:
                                {                                    
                                    tblStat_Eve_History.Rows.Add();
                                    Row_History = tblStat_Eve_History.Rows[tblStat_Eve_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 13:
                                {                                    
                                    tblStat_Eve_Exotic_History.Rows.Add();
                                    Row_History = tblStat_Eve_Exotic_History.Rows[tblStat_Eve_Exotic_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                            case 14:
                                {                                   
                                    tblStat_Eve_Archi_History.Rows.Add();
                                    Row_History = tblStat_Eve_Archi_History.Rows[tblStat_Eve_Archi_History.Rows.Count - 1];
                                    Row = tblStat_Elsword.Rows[((sTableValue)m_ModifyIDList[i]).nRowIndex];
                                }
                                break;
                        }

                        Row_History["IndexKey"] = Row["IndexKey"];
                        Row_History["LV"] = Row["LV"];
                        Row_History["PhysAtk"] = Row["PhysAtk"];
                        Row_History["MagicAtk"] = Row["MagicAtk"];
                        Row_History["PhysDef"] = Row["PhysDef"];
                        Row_History["MagicDef"] = Row["MagicDef"];
                        Row_History["HP"] = Row["HP"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                    }
                                       
             
                    Adpt_Elsword_History.Update(tblStat_Elsword_History);
                    Adpt_Elsword.Update(tblStat_Elsword);

                    Adpt_Elsword_Knight_History.Update(tblStat_Elsword_Knight_History);
                    Adpt_Elsword_Knight.Update(tblStat_Elsword_Knight);

                    Adpt_Elsword_Magic_History.Update(tblStat_Elsword_Magic_History);
                    Adpt_Elsword_Magic.Update(tblStat_Elsword_Magic);

                    Adpt_Lena_History.Update(tblStat_Lena_History);
                    Adpt_Lena.Update(tblStat_Lena);

                    Adpt_Lena_Combat_History.Update(tblStat_Lena_Combat_History);
                    Adpt_Lena_Combat.Update(tblStat_Lena_Combat);

                    Adpt_Lena_Sniper_History.Update(tblStat_Lena_Sniper_History);
                    Adpt_Lena_Sniper.Update(tblStat_Lena_Sniper);

                    Adpt_Aisha_History.Update(tblStat_Aisha_History);
                    Adpt_Aisha.Update(tblStat_Aisha);

                    Adpt_Aisha_High_History.Update(tblStat_Aisha_High_History);
                    Adpt_Aisha_High.Update(tblStat_Aisha_High);

                    Adpt_Aisha_Dark_History.Update(tblStat_Aisha_Dark_History);
                    Adpt_Aisha_Dark.Update(tblStat_Aisha_Dark);

                    Adpt_Raven_History.Update(tblStat_Raven_History);
                    Adpt_Raven.Update(tblStat_Raven);

                    Adpt_Raven_Soul_History.Update(tblStat_Raven_Soul_History);
                    Adpt_Raven_Soul.Update(tblStat_Raven_Soul);

                    Adpt_Raven_Over_History.Update(tblStat_Raven_Over_History);
                    Adpt_Raven_Over.Update(tblStat_Raven_Over);

                    Adpt_Eve_History.Update(tblStat_Eve_History);
                    Adpt_Eve.Update(tblStat_Eve);

                    Adpt_Eve_Exotic_History.Update(tblStat_Eve_Exotic_History);
                    Adpt_Eve_Exotic.Update(tblStat_Eve_Exotic);

                    Adpt_Eve_Archi_History.Update(tblStat_Eve_Archi_History);
                    Adpt_Eve_Archi.Update(tblStat_Eve_Archi);
                    MessageBox.Show("저장되었습니다.");
                    m_ModifyIDList.Clear();
                    m_bSaveEnd = true;                    
            
                    
                }
                else if (result == DialogResult.Cancel)
                {
                    MessageBox.Show("저장하지 않으시면 파일을 생성할 수 없습니다.");
                    return;
                }
            }



            StreamWriter sw = new StreamWriter("..\\..\\ElswordNew\\KncWX2Server\\GameServer\\StatTable.lua", false, Encoding.Default);

            sw.Write("StatTable:ReserveMemory( 1, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 2, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 3, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 4, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 5, 80 )\r\n");
            
            sw.Write("StatTable:ReserveMemory( 10, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 11, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 12, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 13, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 14, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 15, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 16, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 17, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 18, 80 )\r\n");
            sw.Write("StatTable:ReserveMemory( 19, 80 )\r\n");
            
            for (int i = 0; i < dataGridView1.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 1, ");
                sw.Write(dataGridView1[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView1[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView1[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView1[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView1[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView1[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView3.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 2, ");
                sw.Write(dataGridView3[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView3[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView3[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView3[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView3[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView3[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView2.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 3, ");
                sw.Write(dataGridView2[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView2[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView2[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView2[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView2[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView2[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView4.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 10, ");
                sw.Write(dataGridView4[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView4[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView4[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView4[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView4[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView4[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView5.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 11, ");
                sw.Write(dataGridView5[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView5[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView5[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView5[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView5[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView5[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView6.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 12, ");
                sw.Write(dataGridView6[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView6[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView6[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView6[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView6[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView6[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView7.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 13, ");
                sw.Write(dataGridView7[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView7[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView7[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView7[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView7[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView7[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView8.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 14, ");
                sw.Write(dataGridView8[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView8[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView8[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView8[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView8[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView8[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView9.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 15, ");
                sw.Write(dataGridView9[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView9[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView9[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView9[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView9[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView9[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView10.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 4, ");
                sw.Write(dataGridView10[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView10[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView10[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView10[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView10[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView10[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView11.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 16, ");
                sw.Write(dataGridView11[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView11[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView11[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView11[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView11[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView11[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView12.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 17, ");
                sw.Write(dataGridView12[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView12[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView12[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView12[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView12[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView12[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView13.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 5, ");
                sw.Write(dataGridView13[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView13[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView13[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView13[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView13[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView13[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView15.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 19, ");
                sw.Write(dataGridView15[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView15[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView15[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView15[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView15[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView15[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }

            sw.Write("\r\n\r\n");

            for (int i = 0; i < dataGridView14.RowCount; i++)
            {                
                sw.Write("StatTable:SetUnitStat( 18, ");
                sw.Write(dataGridView14[1, i].Value.ToString());
                sw.Write(", { AtkPhysic = ");
                sw.Write(dataGridView14[2, i].Value.ToString());
                sw.Write(", AtkMagic = ");
                sw.Write(dataGridView14[3, i].Value.ToString());
                sw.Write(", DefPhysic = ");
                sw.Write(dataGridView14[4, i].Value.ToString());
                sw.Write(", DefMagic = ");
                sw.Write(dataGridView14[5, i].Value.ToString());
                sw.Write(", HP = ");
                sw.Write(dataGridView14[6, i].Value.ToString());
                sw.Write(" } )\r\n");               
            }






            sw.Close();
            MessageBox.Show("파일이 생성되었습니다.");



        }
    }
}