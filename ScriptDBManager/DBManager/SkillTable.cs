//09. 06. 24 김정협 나라별 define
// ItemTable.cs, QuestTable.cs, TitleTable.cs SkillTable.cs 네군대 정의를
// 모두 바꿔줘야 함.
#define __COUNTRY_KR__
//#define __COUNTRY_TW__
//#define __COUNTRY_HK__
//#define __COUNTRY_JP__



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
using DBManager.LocalizationDataSetTableAdapters;
using DBManager.ScriptData_TWDataSetTableAdapters;
using DBManager.ScriptData_HKDataSetTableAdapters;
using System.Diagnostics;

namespace DBManager
{
    public partial class SkillTable : Form
    {       

        private SqlConnection Con;
        
        object objTemp;
        object objTemp2;
        
        
        //09. 06. 03 김정협 추가 : 셀배경색 DB
        //ColorTable용        
        private SqlDataAdapter Adpt_ColorTable;
        DataTable tblColor = null;
        
        //09. 07. 07         
        //BorderTable용        
        private SqlDataAdapter Adpt_BorderTable;
        DataTable tblBorder;

        //SkillTable용        
        private SqlDataAdapter Adpt_SkillTable;
        DataTable tblSkill;

        //SkillTableHistory용        
        private SqlDataAdapter Adpt_SkillTable_History;
        DataTable tblSkill_History;
        //SkillTableDelete용        
        private SqlDataAdapter Adpt_SkillTable_DeleteHistory;
        DataTable tblSkill_DeleteHistory;

        //SkillTable용        
        private SqlDataAdapter Adpt_SkillEnum;
        DataTable tblSkillEnum;

        //SkillTableHistory용        
        private SqlDataAdapter Adpt_SkillEnum_History;
        DataTable tblSkillEnum_History;
        //SkillTableDelete용        
        private SqlDataAdapter Adpt_SkillEnum_DeleteHistory;
        DataTable tblSkillEnum_DeleteHistory;

        //String 용        
        //private SqlDataAdapter Adpt_String;
        //09. 06. 19 김정협 Trans 연결
        DataTable tblTrans;
        TransTableAdapter Adpt_Trans = new TransTableAdapter();
        DataTable tblTrans_History;
        Trans_HistoryTableAdapter Adpt_Trans_History = new Trans_HistoryTableAdapter();

        //09. 06. 19 김정협 Release 연결
        DataTable tblRelease;
        ReleaseTableAdapter Adpt_Release = new ReleaseTableAdapter();
        DataTable tblRelease_History;
        Release_HistoryTableAdapter Adpt_Release_History = new Release_HistoryTableAdapter();

        //09. 06. 02 김정협 추가 : 색상을 변경 기억을 위한 배열
        ArrayList m_ChangeCellList = new ArrayList();

        string m_strID;
        ArrayList m_ModifyIDList = new ArrayList();

        ArrayList m_DeleteRowList = new ArrayList();

        Stack m_UndoStack = new Stack();
        Stack m_RedoStack = new Stack();

        public bool m_bSaveEnd = true; //수정했을 시 저장한 후 끝내는지 확인하는 변수

        

        //Elsword용        
        private SqlDataAdapter Adpt_Elsword;
        DataTable tblElsword;
        /*//ElswordHistory용        
        private SqlDataAdapter Adpt_Elsword_History;
        DataTable tblElsword_History;
        //ElswordDelete용        
        private SqlDataAdapter Adpt_Elsword_DeleteHistory;
        DataTable tblElsword_DeleteHistory;
        */
        //Elsword_SK용        
        private SqlDataAdapter Adpt_Elsword_SK;
        DataTable tblElsword_SK;
        /*//Elsword_SKHistory용        
        private SqlDataAdapter Adpt_Elsword_SK_History;
        DataTable tblElsword_SK_History;
        //Elsword_SKDelete용        
        private SqlDataAdapter Adpt_Elsword_SK_DeleteHistory;
        DataTable tblElsword_SK_DeleteHistory;*/

        //Elsword_MK용        
        private SqlDataAdapter Adpt_Elsword_MK;
        DataTable tblElsword_MK;
        /*//Elsword_MKHistory용        
        private SqlDataAdapter Adpt_Elsword_MK_History;
        DataTable tblElsword_MK_History;
        //Elsword_MKDelete용        
        private SqlDataAdapter Adpt_Elsword_MK_DeleteHistory;
        DataTable tblElsword_MK_DeleteHistory;*/

        //Aisha용        
        private SqlDataAdapter Adpt_Aisha;
        DataTable tblAisha;
        /*//AishaHistory용        
        private SqlDataAdapter Adpt_Aisha_History;
        DataTable tblAisha_History;
        //AishaDelete용        
        private SqlDataAdapter Adpt_Aisha_DeleteHistory;
        DataTable tblAisha_DeleteHistory;*/

        //Aisha_HM용        
        private SqlDataAdapter Adpt_Aisha_HM;
        DataTable tblAisha_HM;
        //Aisha_HMHistory용        
        /*private SqlDataAdapter Adpt_Aisha_HM_History;
        DataTable tblAisha_HM_History;
        //Aisha_HMDelete용        
        private SqlDataAdapter Adpt_Aisha_HM_DeleteHistory;
        DataTable tblAisha_HM_DeleteHistory;*/

        //Aisha_DM용        
        private SqlDataAdapter Adpt_Aisha_DM;
        DataTable tblAisha_DM;
        //Aisha_DMHistory용        
        /*private SqlDataAdapter Adpt_Aisha_DM_History;
        DataTable tblAisha_DM_History;
        //Aisha_DMDelete용        
        private SqlDataAdapter Adpt_Aisha_DM_DeleteHistory;
        DataTable tblAisha_DM_DeleteHistory;*/


        //Rena용        
        private SqlDataAdapter Adpt_Rena;
        DataTable tblRena;
        //RenaHistory용        
        /*private SqlDataAdapter Adpt_Rena_History;
        DataTable tblRena_History;
        //RenaDelete용        
        private SqlDataAdapter Adpt_Rena_DeleteHistory;
        DataTable tblRena_DeleteHistory;*/

        //Rena_CR용        
        private SqlDataAdapter Adpt_Rena_CR;
        DataTable tblRena_CR;
        /*//Rena_CRHistory용        
        private SqlDataAdapter Adpt_Rena_CR_History;
        DataTable tblRena_CR_History;
        //Rena_CRDelete용        
        private SqlDataAdapter Adpt_Rena_CR_DeleteHistory;
        DataTable tblRena_CR_DeleteHistory;*/

        //Rena_SR용        
        private SqlDataAdapter Adpt_Rena_SR;
        DataTable tblRena_SR;
        /*//Rena_SRHistory용        
        private SqlDataAdapter Adpt_Rena_SR_History;
        DataTable tblRena_SR_History;
        //Rena_SRDelete용        
        private SqlDataAdapter Adpt_Rena_SR_DeleteHistory;
        DataTable tblRena_SR_DeleteHistory;*/


        //Raven용        
        private SqlDataAdapter Adpt_Raven;
        DataTable tblRaven;
        /*//RavenHistory용        
        private SqlDataAdapter Adpt_Raven_History;
        DataTable tblRaven_History;
        //RavenDelete용        
        private SqlDataAdapter Adpt_Raven_DeleteHistory;
        DataTable tblRaven_DeleteHistory;*/

        //Raven_OT용        
        private SqlDataAdapter Adpt_Raven_OT;
        DataTable tblRaven_OT;
        /*//Raven_OTHistory용        
        private SqlDataAdapter Adpt_Raven_OT_History;
        DataTable tblRaven_OT_History;
        //Raven_OTDelete용        
        private SqlDataAdapter Adpt_Raven_OT_DeleteHistory;
        DataTable tblRaven_OT_DeleteHistory;*/

        //Raven_ST용        
        private SqlDataAdapter Adpt_Raven_ST;
        DataTable tblRaven_ST;
        /*//Raven_STHistory용        
        private SqlDataAdapter Adpt_Raven_ST_History;
        DataTable tblRaven_ST_History;
        //Raven_STDelete용        
        private SqlDataAdapter Adpt_Raven_ST_DeleteHistory;
        DataTable tblRaven_ST_DeleteHistory;*/


        //Eve용        
        private SqlDataAdapter Adpt_Eve;
        DataTable tblEve;
        /*//EveHistory용        
        private SqlDataAdapter Adpt_Eve_History;
        DataTable tblEve_History;
        //EveDelete용        
        private SqlDataAdapter Adpt_Eve_DeleteHistory;
        DataTable tblEve_DeleteHistory;*/

        //Eve_CA용        
        private SqlDataAdapter Adpt_Eve_CA;
        DataTable tblEve_CA;
        /*//Eve_CAHistory용        
        private SqlDataAdapter Adpt_Eve_CA_History;
        DataTable tblEve_CA_History;
        //Eve_CADelete용        
        private SqlDataAdapter Adpt_Eve_CA_DeleteHistory;
        DataTable tblEve_CA_DeleteHistory;*/

        //Eve_CE용        
        private SqlDataAdapter Adpt_Eve_CE;
        DataTable tblEve_CE;
        /*//Eve_CEHistory용        
        private SqlDataAdapter Adpt_Eve_CE_History;
        DataTable tblEve_CE_History;
        //Eve_CEDelete용        
        private SqlDataAdapter Adpt_Eve_CE_DeleteHistory;
        DataTable tblEve_CE_DeleteHistory;
        */
        //SKILL_TYPE용        
        private SqlDataAdapter Adpt_SkillType;
        DataTable tblSkillType;
        
        
        //ACTIVE_SKILL_USE_CONDITION용        
        private SqlDataAdapter Adpt_ActiveSkillUseCondition;
        DataTable tblActiveSkillUseCondition;

        /*//ACTIVE_SKILL_USE_CONDITION용        
        private SqlDataAdapter Adpt_ActiveSkillUseCondition;
        DataTable tblActiveSkillUseCondition;

        //UNIT_CLASS용        
        private SqlDataAdapter Adpt_UnitClass;
        DataTable tblUnitClass;

        //SKILL_ABILITY용        
        private SqlDataAdapter Adpt_SkillAbility;
        DataTable tblSkillAbility;
        */
        public int m_nFilteringColumnIndex = -1;

        private int m_nStartPoint = 0;

        private int m_nLastRow = 0;

        //다중 붙여넣기에서 CellEditBegin 과 End를 사용하기 위해서 만듬
        private DataGridViewCell m_dgvcTemp;
        private bool m_bPaste = false;

        //09. 07. 02 김정협 여러 값을 그룹화하여 언두리두시 한번에 바꿀때 사용하기 위해 만든 변수
        int m_nPasteGroup = -1;

        //09. 06. 10 필터링 결과내 재검색 기능
        public bool m_IsFiltering = false;
        
               


        public SkillTable()
        {
            InitializeComponent();
        }

        public void SetID(string strID)
        {
            m_strID = strID;
        }

        public int GetCurrentTab() { return tabControl1.SelectedIndex; }

        public int CurrentMaxStringIndex()
        {
            //09. 06. 22 김정협 현재 맥스스트링을 Rows.Count가 아닌 현재 인덱스 아이디를 얻어와서 계산
            DataTable dtTemp = Adpt_Trans.GetDataByDescTransIDX();
            int nMaxStringIndex = Convert.ToInt32(dtTemp.Rows[0]["TransIDX"].ToString());
            return nMaxStringIndex;//tblTrans.Rows.Count - 1; 
        }

        public void ColorRefresh(bool bRed)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        for (int i = 0; i < dataGridView1.RowCount; i++)
                        {
                            for (int k = 0; k < dataGridView1.ColumnCount; k++)
                            {
                                if (dataGridView1[k, i].HasStyle)
                                    dataGridView1[k, i].Style.ForeColor = Color.Black;
                            }
                        }

                        //09. 06. 01 김정협 수정 : IndexKey와 대조하여 색을 새로 입힘
                        for (int j = 0; j < m_ModifyIDList.Count; j++)
                        {
                            if (((sTableValue)m_ModifyIDList[j]).nTabIndex == 0)
                            {
                                for (int i = 0; i < dataGridView1.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString()) == ((sTableValue)m_ModifyIDList[j]).nIndexKey)
                                    {
                                        if (!((sTableValue)m_ModifyIDList[j]).bCell)
                                        {
                                            //m_dgvData.Rows[i].DefaultCellStyle.ForeColor = Color.Red;
                                            for (int k = 0; k < dataGridView1.ColumnCount; k++)
                                            {
                                                if (bRed)
                                                    dataGridView1[k, i].Style.ForeColor = Color.Red;
                                                else
                                                    dataGridView1[k, i].Style.ForeColor = Color.Black;
                                            }
                                            break;
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        //09. 06. 02 김정협 수정 : 배열정보에 따라 글씨 색 붉게 함.
                        for (int j = 0; j < m_ChangeCellList.Count; j++)
                        {
                            if (((sChangeCell)m_ChangeCellList[j]).nTabIndex == 0)
                            {
                                for (int i = 0; i < dataGridView1.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString()) == ((sChangeCell)m_ChangeCellList[j]).nIndexKey)
                                    {
                                        if (bRed)
                                        {
                                            dataGridView1[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Red;
                                            dataGridView1["Regdate", i].Style.ForeColor = Color.Red;
                                        }
                                        else
                                        {
                                            dataGridView1[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Black;
                                            dataGridView1["Regdate", i].Style.ForeColor = Color.Black;
                                        }
                                        break;
                                    }
                                }
                            }
                        }

                    }
                    break;
                case 16:
                    {
                        for (int i = 0; i < dataGridView17.RowCount; i++)
                        {
                            for (int k = 0; k < dataGridView17.ColumnCount; k++)
                            {
                                if (dataGridView17[k, i].HasStyle)
                                    dataGridView17[k, i].Style.ForeColor = Color.Black;
                            }
                        }

                        //09. 06. 01 김정협 수정 : IndexKey와 대조하여 색을 새로 입힘
                        for (int j = 0; j < m_ModifyIDList.Count; j++)
                        {
                            if (((sTableValue)m_ModifyIDList[j]).nTabIndex == 16)
                            {
                                for (int i = 0; i < dataGridView17.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView17["IndexKey", i].Value.ToString()) == ((sTableValue)m_ModifyIDList[j]).nIndexKey)
                                    {
                                        if (!((sTableValue)m_ModifyIDList[j]).bCell)
                                        {
                                            //m_dgvData.Rows[i].DefaultCellStyle.ForeColor = Color.Red;
                                            for (int k = 0; k < dataGridView17.ColumnCount; k++)
                                            {
                                                if (bRed)
                                                    dataGridView17[k, i].Style.ForeColor = Color.Red;
                                                else
                                                    dataGridView17[k, i].Style.ForeColor = Color.Black;
                                            }
                                            break;
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        //09. 06. 02 김정협 수정 : 배열정보에 따라 글씨 색 붉게 함.
                        for (int j = 0; j < m_ChangeCellList.Count; j++)
                        {
                            if (((sChangeCell)m_ChangeCellList[j]).nTabIndex == 1)
                            {
                                for (int i = 0; i < dataGridView17.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView17["IndexKey", i].Value.ToString()) == ((sChangeCell)m_ChangeCellList[j]).nIndexKey)
                                    {
                                        if (bRed)
                                        {
                                            dataGridView17[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Red;
                                            dataGridView17["Regdate", i].Style.ForeColor = Color.Red;
                                        }
                                        else
                                        {
                                            dataGridView17[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Black;
                                            dataGridView17["Regdate", i].Style.ForeColor = Color.Black;
                                        }
                                        break;
                                    }
                                }
                            }
                        }

                    }
                    break;
            }
        }

        public void SetCellBackgroundColorForDB(int nSelect)
        {
            //09. 06. 03 김정협 추가 : 셀배경색 DB에서 로딩함
            for (int i = 0; i < tblColor.Rows.Count; i++)
            {
                if (tblColor.Rows[i].RowState == DataRowState.Deleted)
                {
                    continue;
                }

                DataRow drTemp = tblColor.Rows[i];
                int nColumn = Convert.ToInt32(drTemp["Column"].ToString());
                int nARGB = Convert.ToInt32(drTemp["Color"].ToString());

                if (nARGB == 0)
                {
                    tblColor.Rows[i].Delete();
                    continue;
                }

                int nRow = -1;

                if (drTemp["TableName"].ToString() == "SkillTable" && (nSelect == 0 || nSelect == -1))
                {
                    for (int j = 0; j < dataGridView1.RowCount; j++)
                    {
                        if (dataGridView1["IndexKey", j].Value.ToString() == drTemp["IndexNum"].ToString())
                        {
                            nRow = dataGridView1["IndexKey", j].RowIndex;
                            break;
                        }
                    }


                    if (nRow != -1)
                    {
                        tabControl1.SelectedIndex = 0;
                        dataGridView1[nColumn, nRow].Style.BackColor = Color.FromArgb(nARGB);
                    }
                    else
                    {
                        tblColor.Rows[i].Delete();
                        //Adpt_ColorTable.Update(tblColor);
                    }

                    //dataGridView1.RefreshEdit();
                    //dataGridView1.Refresh();

                }                
                else if (drTemp["TableName"].ToString() == "SkillEnum" && (nSelect == 16 || nSelect == -1))
                {
                    for (int j = 0; j < dataGridView17.RowCount; j++)
                    {
                        if (dataGridView17["IndexKey", j].Value.ToString() == drTemp["IndexNum"].ToString())
                        {
                            nRow = dataGridView17["IndexKey", j].RowIndex;
                            break;
                        }
                    }
                    if (nRow != -1)
                    {
                        tabControl1.SelectedIndex = 16;
                        dataGridView17[nColumn, nRow].Style.BackColor = Color.FromArgb(nARGB);
                    }
                    else
                    {
                        tblColor.Rows[i].Delete();
                        //Adpt_ColorTable.Update(tblColor);
                    }
                    
                }
            }
            if (nSelect == -1)
            {
                tabControl1.SelectedIndex = 0;
            }


        }

        void SetCharacterSkillView(DataGridView dgvSkill, DataTable tblCurrent)
        {
            dgvSkill.DefaultCellStyle.BackColor = Color.Gray;
            dgvSkill.RowsDefaultCellStyle.WrapMode = DataGridViewTriState.True;

            DataGridViewColumn Column = null;
            int nColumnSize = 40;
            int nValueColumnSize = 70;

            for (int i = 0; i < 5; i++)
            {
                Column = new DataGridViewColumn(new DataGridViewTextBoxCell());
                Column.ReadOnly = true;
                Column.Width = nColumnSize;
                dgvSkill.Columns.Add(Column);
                Column = new DataGridViewColumn(new DataGridViewTextBoxCell());
                Column.ReadOnly = true;
                Column.Width = nValueColumnSize;
                dgvSkill.Columns.Add(Column);
                Column = new DataGridViewColumn(new DataGridViewTextBoxCell());
                Column.ReadOnly = true;
                Column.Width = nValueColumnSize;
                dgvSkill.Columns.Add(Column);
                Column = new DataGridViewColumn(new DataGridViewTextBoxCell());
                Column.ReadOnly = true;
                Column.Width = nValueColumnSize;
                dgvSkill.Columns.Add(Column);
            }
            Column = new DataGridViewColumn(new DataGridViewTextBoxCell());
            Column.ReadOnly = true;
            Column.Width = nColumnSize;
            dgvSkill.Columns.Add(Column);

            

            dgvSkill.Rows.Add();
            for (int i = 1; i <= tblCurrent.Rows.Count * 3; i++)
            {
                dgvSkill.Rows.Add();
            }

            int nCount = 1;
            dgvSkill.Rows[0].Height = 60;
            for (int i = 0; i < 5; i++)
            {
                dgvSkill[1 + (i * 4), 0].Value = "COLUMN_" + (i + 1) + "\n_NAME";
                dgvSkill[1 + (i * 4), 0].Style.BackColor = Color.Yellow;
                dgvSkill[2 + (i * 4), 0].Value = "COLUMN_" + (i + 1) + "\n_M_LV";
                dgvSkill[2 + (i * 4), 0].Style.BackColor = Color.Yellow;
                dgvSkill[3 + (i * 4), 0].Value = "COLUMN_" + (i + 1) + "\n_CONDITION";
                dgvSkill[3 + (i * 4), 0].Style.BackColor = Color.Yellow;
            }
            for (int i = 1; i <= tblCurrent.Rows.Count * 3; i += 3)
            {

                dgvSkill[0, i].Value = "LINE_" + nCount;
                dgvSkill[0, i].Style.BackColor = Color.Yellow;
                dgvSkill.Rows[i].Height = 60;

                for (int k = 0; k < 5; k++)
                {
                    if (tblCurrent.Rows[(i - 1) / 3][((k * 3) + 1)].ToString() != "0")
                    {
                        dgvSkill[1 + (k * 4), i].Value = tblCurrent.Rows[(i - 1) / 3][((k * 3) + 1)];
                    }
                    else
                    {
                        dgvSkill[1 + (k * 4), i].Value = "";
                    }
                    dgvSkill[1 + (k * 4), i].Style.BackColor = Color.White;
                    dgvSkill[1 + (k * 4), i].ReadOnly = false;

                    if (tblCurrent.Rows[(i - 1) / 3][((k * 3) + 2)].ToString() != "0")
                    {
                        dgvSkill[2 + (k * 4), i].Value = tblCurrent.Rows[(i - 1) / 3][((k * 3) + 2)];
                    }
                    else
                    {
                        dgvSkill[2 + (k * 4), i].Value = "";
                    }
                    dgvSkill[2 + (k * 4), i].Style.BackColor = Color.White;
                    dgvSkill[2 + (k * 4), i].ReadOnly = false;

                    if (tblCurrent.Rows[(i - 1) / 3][((k * 3) + 3)].ToString() != "0")
                    {
                        dgvSkill[3 + (k * 4), i].Value = tblCurrent.Rows[(i - 1) / 3][((k * 3) + 3)];
                    }
                    else
                    {
                        dgvSkill[3 + (k * 4), i].Value = "";
                    }
                    dgvSkill[3 + (k * 4), i].Style.BackColor = Color.White;
                    dgvSkill[3 + (k * 4), i].ReadOnly = false;
                }
                
                dgvSkill.Rows[i + 1].Height = 100;
                
                dgvSkill.Rows[i + 2].Height = 60;
                nCount++;
            }
            SkillDataLink(dgvSkill, tblCurrent);
        }

        void SkillDataLink(DataGridView dgvSkill, DataTable tblCurrent)
        {
            bool bFind = false;
            for (int i = 1; i <= tblCurrent.Rows.Count * 3; i += 3)
            {
                for (int k = 0; k < 5; k++)
                {
                    int nLeft = 0;
                    int nRight = dataGridView1.RowCount - 1;
                    while (nLeft <= nRight)
                    {
                        if (dgvSkill[1 + (k * 4), i].Value.ToString() == "")
                        {
                            dgvSkill[1 + (k * 4), i + 1].Value = "";
                            bFind = true;
                            break;
                        }
                        int nMid = (nLeft + nRight) / 2;

                        int nValue = Convert.ToInt32(dgvSkill[1 + (k * 4), i].Value.ToString());
                        int nCurrentID = Convert.ToInt32(dataGridView1[1, nMid].Value.ToString());

                        if (nValue == nCurrentID)
                        {
                            dgvSkill[1 + (k * 4), i + 1].Value = dataGridView1["NAME", nMid].Value;
                            bFind = true;
                            break;
                        }
                        if (nValue > nCurrentID)
                        {
                            nLeft = nMid + 1;
                        }
                        if (nValue < nCurrentID)
                        {
                            nRight = nMid - 1;
                        }
                    }
                    if(!bFind)
                    {
                        MessageBox.Show("존재하지 않는 값입니다.");
                        dgvSkill[1 + (k * 4), i].Value = "";
                        continue;
                    }

                    //dgvSkill[1 + (k * 4), i + 1].Value = 
                    dgvSkill[1 + (k * 4), i + 1].Style.BackColor = Color.LightGray;
                    dgvSkill[2 + (k * 4), i + 1].Value = "MAX_LV " + dgvSkill[2 + (k * 4), i].Value.ToString();
                    dgvSkill[2 + (k * 4), i + 1].Style.BackColor = Color.LightGray;

                    nLeft = 0;
                    nRight = dataGridView1.RowCount - 1;
                    while (nLeft <= nRight)
                    {
                        if (dgvSkill[3 + (k * 4), i].Value.ToString() == "")
                        {
                            break;
                        }
                        int nMid = (nLeft + nRight) / 2;

                        int nValue = Convert.ToInt32(dgvSkill[3 + (k * 4), i].Value.ToString());
                        int nCurrentID = Convert.ToInt32(dataGridView1[1, nMid].Value.ToString());

                        if (nValue == nCurrentID)
                        {
                            dgvSkill[3 + (k * 4), i + 1].Value = dataGridView1["NAME", nMid].Value;
                            break;
                        }
                        if (nValue > nCurrentID)
                        {
                            nLeft = nMid + 1;
                        }
                        if (nValue < nCurrentID)
                        {
                            nRight = nMid - 1;
                        }
                    }


                    dgvSkill[3 + (k * 4), i + 1].Style.BackColor = Color.LightGray;
                }
            }

        }

        void Save()
        {
            //09. 07. 01 김정협 현재 위치 기억
            int nCurrentPosTabIndex = tabControl1.SelectedIndex;
            int nCurrentPosColumnIndex = -1;
            int nCurrentPosRowIndex = -1;
            switch (nCurrentPosTabIndex)
            {
                case 0:
                    nCurrentPosColumnIndex = dataGridView1.CurrentCell.ColumnIndex;
                    nCurrentPosRowIndex = dataGridView1.CurrentCell.RowIndex;
                    break;
                case 16:
                    nCurrentPosColumnIndex = dataGridView17.CurrentCell.ColumnIndex;
                    nCurrentPosRowIndex = dataGridView17.CurrentCell.RowIndex;
                    break;
            }

#if __COUNTRY_KR__

            //09. 07. 08 김정협 저장 속도 증가를 위하여 한번만 실행하게 함.
            ArrayList arModifyTabIndex = new ArrayList();
            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                if (!arModifyTabIndex.Contains(((sTableValue)m_ModifyIDList[i]).nTabIndex))
                {
                    arModifyTabIndex.Add(((sTableValue)m_ModifyIDList[i]).nTabIndex);
                }
            }
            ArrayList arDeleteTabIndex = new ArrayList();
            for (int i = 0; i < m_DeleteRowList.Count; i++)
            {
                if (!arDeleteTabIndex.Contains(((sDeleteRow)m_DeleteRowList[i]).nTabIndex))
                {
                    arDeleteTabIndex.Add(((sDeleteRow)m_DeleteRowList[i]).nTabIndex);
                }
            }

            ///////////////////////////////////////////////////////////////////////////
            //대만버전
            ///////////////////////////////////////////////////////////////////////////
            ScriptData_TWDataSetTableAdapters.SkillTableTableAdapter Adt_SkillTableTW = null;            
            DataTable tblSkillTW = null;
            ScriptData_TWDataSetTableAdapters.SkillTable_HistoryTableAdapter Adt_SkillTableTW_History = null;
            DataTable tblSkillTW_History = null;
            ScriptData_TWDataSetTableAdapters.SkillTable_DeleteHistoryTableAdapter Adpt_SkillTableTW_DeleteHistory = null;
            DataTable tblSkillTW_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_SkillTableTW = new ScriptData_TWDataSetTableAdapters.SkillTableTableAdapter();

                tblSkillTW = Adt_SkillTableTW.GetData();

                tblSkillTW.Columns["IndexKey"].AllowDBNull = true;
                tblSkillTW.Columns["ID"].AllowDBNull = true;
                tblSkillTW.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(0))
                {
                    Adt_SkillTableTW_History = new ScriptData_TWDataSetTableAdapters.SkillTable_HistoryTableAdapter();

                    tblSkillTW_History = Adt_SkillTableTW_History.GetData();

                    tblSkillTW_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSkillTW_History.Columns["ID"].AllowDBNull = true;
                    tblSkillTW_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_SkillTableTW_DeleteHistory = new ScriptData_TWDataSetTableAdapters.SkillTable_DeleteHistoryTableAdapter();

                    tblSkillTW_DeleteHistory = Adpt_SkillTableTW_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblSkillTW_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSkillTW_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_TWDataSetTableAdapters.SkillEnumTableAdapter Adt_SkillEnumTW = null;
            DataTable tblSkillEnumTW = null;
            ScriptData_TWDataSetTableAdapters.SkillEnum_HistoryTableAdapter Adt_SkillEnumTW_History = null;
            DataTable tblSkillEnumTW_History = null;
            ScriptData_TWDataSetTableAdapters.SkillEnum_DeleteHistoryTableAdapter Adpt_SkillEnumTW_DeleteHistory = null;
            DataTable tblSkillEnumTW_DeleteHistory = null;
            if (arModifyTabIndex.Contains(16) || arDeleteTabIndex.Contains(16))
            {
                Adt_SkillEnumTW = new ScriptData_TWDataSetTableAdapters.SkillEnumTableAdapter();

                tblSkillEnumTW = Adt_SkillEnumTW.GetData();

                tblSkillEnumTW.Columns["IndexKey"].AllowDBNull = true;
                tblSkillEnumTW.Columns["ID Num"].AllowDBNull = true;
                tblSkillEnumTW.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(16))
                {
                    Adt_SkillEnumTW_History = new ScriptData_TWDataSetTableAdapters.SkillEnum_HistoryTableAdapter();

                    tblSkillEnumTW_History = Adt_SkillEnumTW_History.GetData();

                    tblSkillEnumTW_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSkillEnumTW_History.Columns["ID Num"].AllowDBNull = true;
                    tblSkillEnumTW_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(16))
                {
                    Adpt_SkillEnumTW_DeleteHistory = new ScriptData_TWDataSetTableAdapters.SkillEnum_DeleteHistoryTableAdapter();

                    tblSkillEnumTW_DeleteHistory = Adpt_SkillEnumTW_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblSkillEnumTW_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSkillEnumTW_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }

            ///////////////////////////////////////////////////////////////////////////
            //홍콩버전
            ///////////////////////////////////////////////////////////////////////////
            ScriptData_HKDataSetTableAdapters.SkillTableTableAdapter Adt_SkillTableHK = null;
            DataTable tblSkillHK = null;
            ScriptData_HKDataSetTableAdapters.SkillTable_HistoryTableAdapter Adt_SkillTableHK_History = null;
            DataTable tblSkillHK_History = null;
            ScriptData_HKDataSetTableAdapters.SkillTable_DeleteHistoryTableAdapter Adpt_SkillTableHK_DeleteHistory = null;
            DataTable tblSkillHK_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_SkillTableHK = new ScriptData_HKDataSetTableAdapters.SkillTableTableAdapter();

                tblSkillHK = Adt_SkillTableHK.GetData();

                tblSkillHK.Columns["IndexKey"].AllowDBNull = true;
                tblSkillHK.Columns["ID"].AllowDBNull = true;
                tblSkillHK.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(0))
                {
                    Adt_SkillTableHK_History = new ScriptData_HKDataSetTableAdapters.SkillTable_HistoryTableAdapter();

                    tblSkillHK_History = Adt_SkillTableHK_History.GetData();

                    tblSkillHK_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSkillHK_History.Columns["ID"].AllowDBNull = true;
                    tblSkillHK_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_SkillTableHK_DeleteHistory = new ScriptData_HKDataSetTableAdapters.SkillTable_DeleteHistoryTableAdapter();

                    tblSkillHK_DeleteHistory = Adpt_SkillTableHK_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblSkillHK_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSkillHK_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_HKDataSetTableAdapters.SkillEnumTableAdapter Adt_SkillEnumHK = null;
            DataTable tblSkillEnumHK = null;
            ScriptData_HKDataSetTableAdapters.SkillEnum_HistoryTableAdapter Adt_SkillEnumHK_History = null;
            DataTable tblSkillEnumHK_History = null;
            ScriptData_HKDataSetTableAdapters.SkillEnum_DeleteHistoryTableAdapter Adpt_SkillEnumHK_DeleteHistory = null;
            DataTable tblSkillEnumHK_DeleteHistory = null;
            if (arModifyTabIndex.Contains(16) || arDeleteTabIndex.Contains(16))
            {
                Adt_SkillEnumHK = new ScriptData_HKDataSetTableAdapters.SkillEnumTableAdapter();

                tblSkillEnumHK = Adt_SkillEnumHK.GetData();

                tblSkillEnumHK.Columns["IndexKey"].AllowDBNull = true;
                tblSkillEnumHK.Columns["ID Num"].AllowDBNull = true;
                tblSkillEnumHK.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(16))
                {
                    Adt_SkillEnumHK_History = new ScriptData_HKDataSetTableAdapters.SkillEnum_HistoryTableAdapter();

                    tblSkillEnumHK_History = Adt_SkillEnumHK_History.GetData();

                    tblSkillEnumHK_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSkillEnumHK_History.Columns["ID Num"].AllowDBNull = true;
                    tblSkillEnumHK_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(16))
                {
                    Adpt_SkillEnumHK_DeleteHistory = new ScriptData_HKDataSetTableAdapters.SkillEnum_DeleteHistoryTableAdapter();

                    tblSkillEnumHK_DeleteHistory = Adpt_SkillEnumHK_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblSkillEnumHK_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSkillEnumHK_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }

            ///////////////////////////////////////////////////////////////////////////
            //일본버전
            ///////////////////////////////////////////////////////////////////////////
            ScriptData_JPDataSetTableAdapters.SkillTableTableAdapter Adt_SkillTableJP = null;
            DataTable tblSkillJP = null;
            ScriptData_JPDataSetTableAdapters.SkillTable_HistoryTableAdapter Adt_SkillTableJP_History = null;
            DataTable tblSkillJP_History = null;
            ScriptData_JPDataSetTableAdapters.SkillTable_DeleteHistoryTableAdapter Adpt_SkillTableJP_DeleteHistory = null;
            DataTable tblSkillJP_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_SkillTableJP = new ScriptData_JPDataSetTableAdapters.SkillTableTableAdapter();

                tblSkillJP = Adt_SkillTableJP.GetData();

                tblSkillJP.Columns["IndexKey"].AllowDBNull = true;
                tblSkillJP.Columns["ID"].AllowDBNull = true;
                tblSkillJP.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(0))
                {
                    Adt_SkillTableJP_History = new ScriptData_JPDataSetTableAdapters.SkillTable_HistoryTableAdapter();

                    tblSkillJP_History = Adt_SkillTableJP_History.GetData();

                    tblSkillJP_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSkillJP_History.Columns["ID"].AllowDBNull = true;
                    tblSkillJP_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_SkillTableJP_DeleteHistory = new ScriptData_JPDataSetTableAdapters.SkillTable_DeleteHistoryTableAdapter();

                    tblSkillJP_DeleteHistory = Adpt_SkillTableJP_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblSkillJP_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSkillJP_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_JPDataSetTableAdapters.SkillEnumTableAdapter Adt_SkillEnumJP = null;
            DataTable tblSkillEnumJP = null;
            ScriptData_JPDataSetTableAdapters.SkillEnum_HistoryTableAdapter Adt_SkillEnumJP_History = null;
            DataTable tblSkillEnumJP_History = null;
            ScriptData_JPDataSetTableAdapters.SkillEnum_DeleteHistoryTableAdapter Adpt_SkillEnumJP_DeleteHistory = null;
            DataTable tblSkillEnumJP_DeleteHistory = null;
            if (arModifyTabIndex.Contains(16) || arDeleteTabIndex.Contains(16))
            {
                Adt_SkillEnumJP = new ScriptData_JPDataSetTableAdapters.SkillEnumTableAdapter();

                tblSkillEnumJP = Adt_SkillEnumJP.GetData();

                tblSkillEnumJP.Columns["IndexKey"].AllowDBNull = true;
                tblSkillEnumJP.Columns["ID Num"].AllowDBNull = true;
                tblSkillEnumJP.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(16))
                {
                    Adt_SkillEnumJP_History = new ScriptData_JPDataSetTableAdapters.SkillEnum_HistoryTableAdapter();

                    tblSkillEnumJP_History = Adt_SkillEnumJP_History.GetData();

                    tblSkillEnumJP_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSkillEnumJP_History.Columns["ID Num"].AllowDBNull = true;
                    tblSkillEnumJP_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(16))
                {
                    Adpt_SkillEnumJP_DeleteHistory = new ScriptData_JPDataSetTableAdapters.SkillEnum_DeleteHistoryTableAdapter();

                    tblSkillEnumJP_DeleteHistory = Adpt_SkillEnumJP_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblSkillEnumJP_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSkillEnumJP_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }

#endif //__COUNTRY_KR__

            //History 추가
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            string strMyIP = host.AddressList[0].ToString();
            DataRow Row_History = null;
            DataRow Row = null;
            //09. 06. 01 김정협 수정 붉은색 모두 검은색 글씨로
            ColorRefresh(false);

            //09. 07. 08 김정협 중국일 경우를 대비해 전처리문에서 빼고 단 한번만 
            //실행하면 되므로 포문에서 뺌(수정할경우임. 추가된 경우는 한국에서만 돌아감)
            {
                //Adpt_String.Update(tblScriptString);
                //09. 06. 19 김정협 Trans
                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                tblTrans.Rows.CopyTo(drArray, 0);
                Adpt_Trans.Update(drArray);


                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                tblTrans_History.Rows.CopyTo(drArray, 0);
                Adpt_Trans_History.Update(drArray);*/


                //09. 06. 19 김정협 Release
                drArray = new DataRow[tblRelease.Rows.Count];
                tblRelease.Rows.CopyTo(drArray, 0);
                Adpt_Release.Update(drArray);

                /*drArray = new DataRow[tblRelease_History.Rows.Count];
                tblRelease_History.Rows.CopyTo(drArray, 0);
                Adpt_Release_History.Update(drArray);*/
            }

            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                switch (((sTableValue)m_ModifyIDList[i]).nTabIndex)
                {
                    case 0:
                        {

                            tblSkill_History.Rows.Add();

                            Row_History = tblSkill_History.Rows[tblSkill_History.Rows.Count - 1];
                            //09. 06. 01 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey전부 검색 중간 삭제시 인덱스 어긋나기 때문
                            for (int nRow = 0; nRow < tblSkill.Rows.Count; nRow++)
                            {
                                if (tblSkill.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblSkill.Rows[nRow]["IndexKey"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblSkill.Rows[nRow];
                                    break;
                                }
                            }

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSkill.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];

                            Adpt_SkillTable_History.Update(new DataRow[] { Row_History });
                            Adpt_SkillTable.Update(new DataRow[] { Row });


                            //09. 06. 19 김정협
                            //국가별 #define처리 해 줄 것
                            // 이부분은 한국일경우만 실행하게 함. 한국에서 추가하면 모두 추가됨
#if __COUNTRY_KR__

                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblQuestTW.Rows.Add();
                            tblSkillTW_History.Rows.Add();
                            DataRow RowTW_History = tblSkillTW_History.Rows[tblSkillTW_History.Rows.Count - 1];

                            int nRowIndex = -1;
                            for (int twRow = 0; twRow < tblSkillTW.Rows.Count; twRow++)
                            {
                                if (tblSkillTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    nRowIndex = twRow;
                                    break;
                                }
                            }

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            int nBefore = Convert.ToInt32(Row["IndexKey"].ToString());
                            if (nRowIndex == -1)
                            {
                                //tblQuest.Clear();
                                //Adpt_QuestTable.Fill(tblQuest);
                                //Row = tblQuest.Rows[tblQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SkillTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSkillTW.Columns.Count; nTemp++)
                                {
                                    tblSkillTW.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowTW = null;// = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1];
                                tblSkillTW.Rows.Add();
                                RowTW = tblSkillTW.Rows[tblSkillTW.Rows.Count - 1];

                                for (int twColumn = 0; twColumn < tblSkillTW.Columns.Count; twColumn++)
                                {
                                    /*if (tblSkillTW.Columns[twColumn].ColumnName == "IndexKey")
                                    {
                                        continue;
                                    }*/
                                    RowTW[twColumn] = Row[twColumn];
                                }

                                ///////////////////////////////////////////////////////////////////
                                //스트링 시작
                                ///////////////////////////////////////////////////////////////////


                                //int nIndexCount = CurrentMaxStringIndex();
                                //nIndexCount++;

                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임                                
                                // m_dgvData["Index_Name", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                                //RowTW["Index_Name"] = nIndexCount;
                                //nIndexCount++;

                                
                                
                                tblTrans.Rows.Add();
                                DataRow RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "TW";
                                RowTrans["DB"] = "ScriptData_TW";
                                RowTrans["TableName"] = "SkillTable";
                                RowTrans["Column"] = "DESC";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSkillTW.Rows[tblSkillTW.Rows.Count - 1]["DESC"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                               
                                


                                for (int twColumn_His = 0; twColumn_His < tblSkillTW_History.Columns.Count; twColumn_His++)
                                {
                                    RowTW_History[twColumn_His] = Row_History[twColumn_His];
                                }

                                Adt_SkillTableTW.Update(RowTW);
                                Adt_SkillTableTW_History.Update(RowTW_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/


                                //09. 06. 19 김정협 Release
                                drArray = new DataRow[tblRelease.Rows.Count];
                                tblRelease.Rows.CopyTo(drArray, 0);
                                Adpt_Release.Update(drArray);

                                /*drArray = new DataRow[tblRelease_History.Rows.Count];
                                tblRelease_History.Rows.CopyTo(drArray, 0);
                                Adpt_Release_History.Update(drArray);*/

                                int nCurrent = (int)Adt_SkillTableTW.GetMaxIndexRow();
                                if (nBefore != nCurrent)
                                {
                                    Adpt_Trans.UpdateIndexKey(nCurrent, nBefore, "SkillTable");
                                    Adpt_Release.UpdateIndexKey(nCurrent, nBefore, "SkillTable");
                                }

                            }
                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////

                            //tblQuestHK.Rows.Add();
                            tblSkillHK_History.Rows.Add();
                            DataRow RowHK_History = tblSkillHK_History.Rows[tblSkillHK_History.Rows.Count - 1];
                                                       

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            
                            if (nRowIndex == -1)
                            {
                                //tblQuest.Clear();
                                //Adpt_QuestTable.Fill(tblQuest);
                                //Row = tblQuest.Rows[tblQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SkillTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSkillHK.Columns.Count; nTemp++)
                                {
                                    tblSkillHK.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowHK = null;// = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1];
                                tblSkillHK.Rows.Add();
                                RowHK = tblSkillHK.Rows[tblSkillHK.Rows.Count - 1];

                                for (int HKColumn = 0; HKColumn < tblSkillHK.Columns.Count; HKColumn++)
                                {
                                    /*if (tblSkillHK.Columns[HKColumn].ColumnName == "IndexKey")
                                    {
                                        continue;
                                    }*/
                                    RowHK[HKColumn] = Row[HKColumn];
                                }

                                ///////////////////////////////////////////////////////////////////
                                //스트링 시작
                                ///////////////////////////////////////////////////////////////////


                                //int nIndexCount = CurrentMaxStringIndex();
                                //nIndexCount++;

                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임                                
                                // m_dgvData["Index_Name", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                                //RowHK["Index_Name"] = nIndexCount;
                                //nIndexCount++;



                                tblTrans.Rows.Add();
                                DataRow RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "HK";
                                RowTrans["DB"] = "ScriptData_HK";
                                RowTrans["TableName"] = "SkillTable";
                                RowTrans["Column"] = "DESC";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSkillHK.Rows[tblSkillHK.Rows.Count - 1]["DESC"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());






                                for (int HKColumn_His = 0; HKColumn_His < tblSkillHK_History.Columns.Count; HKColumn_His++)
                                {
                                    RowHK_History[HKColumn_His] = Row_History[HKColumn_His];
                                }

                                Adt_SkillTableHK.Update(RowHK);
                                Adt_SkillTableHK_History.Update(RowHK_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/


                                //09. 06. 19 김정협 Release
                                drArray = new DataRow[tblRelease.Rows.Count];
                                tblRelease.Rows.CopyTo(drArray, 0);
                                Adpt_Release.Update(drArray);

                                /*drArray = new DataRow[tblRelease_History.Rows.Count];
                                tblRelease_History.Rows.CopyTo(drArray, 0);
                                Adpt_Release_History.Update(drArray);*/


                            }

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////

                            //tblQuestJP.Rows.Add();
                            tblSkillJP_History.Rows.Add();
                            DataRow RowJP_History = tblSkillJP_History.Rows[tblSkillJP_History.Rows.Count - 1];


                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도

                            if (nRowIndex == -1)
                            {
                                //tblQuest.Clear();
                                //Adpt_QuestTable.Fill(tblQuest);
                                //Row = tblQuest.Rows[tblQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SkillTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSkillJP.Columns.Count; nTemp++)
                                {
                                    tblSkillJP.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowJP = null;// = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1];
                                tblSkillJP.Rows.Add();
                                RowJP = tblSkillJP.Rows[tblSkillJP.Rows.Count - 1];

                                for (int JPColumn = 0; JPColumn < tblSkillJP.Columns.Count; JPColumn++)
                                {
                                    /*if (tblSkillJP.Columns[JPColumn].ColumnName == "IndexKey")
                                    {
                                        continue;
                                    }*/
                                    RowJP[JPColumn] = Row[JPColumn];
                                }

                                ///////////////////////////////////////////////////////////////////
                                //스트링 시작
                                ///////////////////////////////////////////////////////////////////


                                //int nIndexCount = CurrentMaxStringIndex();
                                //nIndexCount++;

                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임                                
                                // m_dgvData["Index_Name", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                                //RowJP["Index_Name"] = nIndexCount;
                                //nIndexCount++;



                                tblTrans.Rows.Add();
                                DataRow RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "JP";
                                RowTrans["DB"] = "ScriptData_JP";
                                RowTrans["TableName"] = "SkillTable";
                                RowTrans["Column"] = "DESC";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSkillJP.Rows[tblSkillJP.Rows.Count - 1]["DESC"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());






                                for (int JPColumn_His = 0; JPColumn_His < tblSkillJP_History.Columns.Count; JPColumn_His++)
                                {
                                    RowJP_History[JPColumn_His] = Row_History[JPColumn_His];
                                }

                                Adt_SkillTableJP.Update(RowJP);
                                Adt_SkillTableJP_History.Update(RowJP_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/


                                //09. 06. 19 김정협 Release
                                drArray = new DataRow[tblRelease.Rows.Count];
                                tblRelease.Rows.CopyTo(drArray, 0);
                                Adpt_Release.Update(drArray);

                                /*drArray = new DataRow[tblRelease_History.Rows.Count];
                                tblRelease_History.Rows.CopyTo(drArray, 0);
                                Adpt_Release_History.Update(drArray);*/


                            }
#endif //__COUNTRY_KR__

                        }
                        break;
                    case 16:
                        {
                            tblSkillEnum_History.Rows.Add();

                            Row_History = tblSkillEnum_History.Rows[tblSkillEnum_History.Rows.Count - 1];

                            //09. 06. 01 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey값을 이용. tblItem은 IndexKey값이 로우와 같으므로 
                            // IndexKey전부 검색 중간 삭제시 인덱스 어긋나기 때문
                            for (int nRow = 0; nRow < tblSkillEnum.Rows.Count; nRow++)
                            {
                                if (tblSkillEnum.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblSkillEnum.Rows[nRow]["IndexKey"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblSkillEnum.Rows[nRow];
                                    break;
                                }
                            }

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSkillEnum.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];

                            //SpecialAbility
                            Adpt_SkillEnum.Update(new DataRow[] { Row });
                            Adpt_SkillEnum_History.Update(new DataRow[] { Row_History });

                                                        

                            //09. 06. 19 김정협
                            //국가별 #define처리 해 줄 것
                            // 이부분은 한국일경우만 실행하게 함. 한국에서 추가하면 모두 추가됨
#if __COUNTRY_KR__      
                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                           
                            // tblSpecialAbilityTW.Rows.Add();
                            tblSkillEnumTW_History.Rows.Add();
                            DataRow RowTW_History = tblSkillEnumTW_History.Rows[tblSkillEnumTW_History.Rows.Count - 1];

                            int nRowIndex = -1;
                            for (int twRow = 0; twRow <  tblSkillEnumTW.Rows.Count; twRow++)
                            {
                                if (tblSkillEnumTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    nRowIndex = twRow;
                                    break;
                                }
                            }
                             //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            int nBefore = Convert.ToInt32(Row["IndexKey"].ToString());
                            if (nRowIndex == -1)
                            {
                                DataTable dtTemp = new DataTable();
                                Adpt_SkillEnum.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSkillEnumTW.Columns.Count; nTemp++)
                                {
                                    tblSkillEnumTW.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowTW = null;// =  tblSpecialAbilityTW.Rows[ tblSpecialAbilityTW.Rows.Count - 1];
                                tblSkillEnumTW.Rows.Add();
                                RowTW = tblSkillEnumTW.Rows[tblSkillEnumTW.Rows.Count - 1];

                                for (int twColumn = 0; twColumn < tblSkillEnumTW.Columns.Count; twColumn++)
                                {
                                    /*if ( tblSpecialAbilityTW.Columns[twColumn].ColumnName == "IndexKey")
                                    {
                                        continue;
                                    }*/
                                    RowTW[twColumn] = Row[twColumn];
                                }

                                ///////////////////////////////////////////////////////////////////
                                //스트링 시작
                                ///////////////////////////////////////////////////////////////////


                                //int nIndexCount = CurrentMaxStringIndex();
                                //nIndexCount++;

                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임                                
                                // m_dgvData["Index_Name", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                                tblTrans.Rows.Add();
                                DataRow RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "TW";
                                RowTrans["DB"] = "ScriptData_TW";
                                RowTrans["TableName"] = "SkillEnum";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSkillEnumTW.Rows[tblSkillEnumTW.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());




                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////

                                //09. 06. 19 김정협 Release 추가
                                tblRelease.Rows.Add();
                                DataRow RowRelease = tblRelease.Rows[tblRelease.Rows.Count - 1];

                                RowRelease = tblRelease.Rows[tblRelease.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowRelease["CountryCode"] = "TW";
                                RowRelease["DB"] = "ScriptData_TW";
                                RowRelease["TableName"] = "SkillEnum";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["SubKey"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["ID Num"].ToString());
                                RowRelease["Name"] = tblSkillEnumTW.Rows[tblSkillEnumTW.Rows.Count - 1]["Description"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = DateTime.Now;
                                RowRelease["Last Update Date"] = DateTime.Now;


                                for (int twColumn_His = 0; twColumn_His <  tblSkillEnumTW_History.Columns.Count; twColumn_His++)
                                {
                                    RowTW_History[twColumn_His] = Row_History[twColumn_His];
                                }

                                Adt_SkillEnumTW.Update(RowTW);
                                Adt_SkillEnumTW_History.Update(RowTW_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/


                                //09. 06. 19 김정협 Release
                                drArray = new DataRow[tblRelease.Rows.Count];
                                tblRelease.Rows.CopyTo(drArray, 0);
                                Adpt_Release.Update(drArray);

                                /*drArray = new DataRow[tblRelease_History.Rows.Count];
                                tblRelease_History.Rows.CopyTo(drArray, 0);
                                Adpt_Release_History.Update(drArray);*/

                                int nCurrent = (int)Adt_SkillEnumTW.GetMaxIndexRow();
                                if (nBefore != nCurrent)
                                {
                                    Adpt_Trans.UpdateIndexKey(nCurrent, nBefore, "SkillEnum");
                                    Adpt_Release.UpdateIndexKey(nCurrent, nBefore, "SkillEnum");
                                }

                            }
                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////

                            // tblSpecialAbilityHK.Rows.Add();
                            tblSkillEnumHK_History.Rows.Add();
                            DataRow RowHK_History = tblSkillEnumHK_History.Rows[tblSkillEnumHK_History.Rows.Count - 1];
                                                       
                            
                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            
                            if (nRowIndex == -1)
                            {
                                DataTable dtTemp = new DataTable();
                                Adpt_SkillEnum.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSkillEnumHK.Columns.Count; nTemp++)
                                {
                                    tblSkillEnumHK.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowHK = null;// =  tblSpecialAbilityHK.Rows[ tblSpecialAbilityHK.Rows.Count - 1];
                                tblSkillEnumHK.Rows.Add();
                                RowHK = tblSkillEnumHK.Rows[tblSkillEnumHK.Rows.Count - 1];

                                for (int HKColumn = 0; HKColumn < tblSkillEnumHK.Columns.Count; HKColumn++)
                                {
                                    /*if ( tblSpecialAbilityHK.Columns[HKColumn].ColumnName == "IndexKey")
                                    {
                                        continue;
                                    }*/
                                    RowHK[HKColumn] = Row[HKColumn];
                                }

                                ///////////////////////////////////////////////////////////////////
                                //스트링 시작
                                ///////////////////////////////////////////////////////////////////


                                //int nIndexCount = CurrentMaxStringIndex();
                                //nIndexCount++;

                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임                                
                                // m_dgvData["Index_Name", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                                tblTrans.Rows.Add();
                                DataRow RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "HK";
                                RowTrans["DB"] = "ScriptData_HK";
                                RowTrans["TableName"] = "SkillEnum";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSkillEnumHK.Rows[tblSkillEnumHK.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());




                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////

                                //09. 06. 19 김정협 Release 추가
                                tblRelease.Rows.Add();
                                DataRow RowRelease = tblRelease.Rows[tblRelease.Rows.Count - 1];

                                RowRelease = tblRelease.Rows[tblRelease.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowRelease["CountryCode"] = "HK";
                                RowRelease["DB"] = "ScriptData_HK";
                                RowRelease["TableName"] = "SkillEnum";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["SubKey"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["ID Num"].ToString());
                                RowRelease["Name"] = tblSkillEnumHK.Rows[tblSkillEnumHK.Rows.Count - 1]["Description"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = DateTime.Now;
                                RowRelease["Last Update Date"] = DateTime.Now;


                                for (int HKColumn_His = 0; HKColumn_His < tblSkillEnumHK_History.Columns.Count; HKColumn_His++)
                                {
                                    RowHK_History[HKColumn_His] = Row_History[HKColumn_His];
                                }

                                Adt_SkillEnumHK.Update(RowHK);
                                Adt_SkillEnumHK_History.Update(RowHK_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/


                                //09. 06. 19 김정협 Release
                                drArray = new DataRow[tblRelease.Rows.Count];
                                tblRelease.Rows.CopyTo(drArray, 0);
                                Adpt_Release.Update(drArray);

                                /*drArray = new DataRow[tblRelease_History.Rows.Count];
                                tblRelease_History.Rows.CopyTo(drArray, 0);
                                Adpt_Release_History.Update(drArray);*/
                                                                
                            }


                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////

                            // tblSpecialAbilityJP.Rows.Add();
                            tblSkillEnumJP_History.Rows.Add();
                            DataRow RowJP_History = tblSkillEnumJP_History.Rows[tblSkillEnumJP_History.Rows.Count - 1];


                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도

                            if (nRowIndex == -1)
                            {
                                DataTable dtTemp = new DataTable();
                                Adpt_SkillEnum.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSkillEnumJP.Columns.Count; nTemp++)
                                {
                                    tblSkillEnumJP.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowJP = null;// =  tblSpecialAbilityJP.Rows[ tblSpecialAbilityJP.Rows.Count - 1];
                                tblSkillEnumJP.Rows.Add();
                                RowJP = tblSkillEnumJP.Rows[tblSkillEnumJP.Rows.Count - 1];

                                for (int JPColumn = 0; JPColumn < tblSkillEnumJP.Columns.Count; JPColumn++)
                                {
                                    /*if ( tblSpecialAbilityJP.Columns[JPColumn].ColumnName == "IndexKey")
                                    {
                                        continue;
                                    }*/
                                    RowJP[JPColumn] = Row[JPColumn];
                                }

                                ///////////////////////////////////////////////////////////////////
                                //스트링 시작
                                ///////////////////////////////////////////////////////////////////


                                //int nIndexCount = CurrentMaxStringIndex();
                                //nIndexCount++;

                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임                                
                                // m_dgvData["Index_Name", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                                tblTrans.Rows.Add();
                                DataRow RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "JP";
                                RowTrans["DB"] = "ScriptData_JP";
                                RowTrans["TableName"] = "SkillEnum";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSkillEnumJP.Rows[tblSkillEnumJP.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());




                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////

                                //09. 06. 19 김정협 Release 추가
                                tblRelease.Rows.Add();
                                DataRow RowRelease = tblRelease.Rows[tblRelease.Rows.Count - 1];

                                RowRelease = tblRelease.Rows[tblRelease.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowRelease["CountryCode"] = "JP";
                                RowRelease["DB"] = "ScriptData_JP";
                                RowRelease["TableName"] = "SkillEnum";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["SubKey"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["ID Num"].ToString());
                                RowRelease["Name"] = tblSkillEnumJP.Rows[tblSkillEnumJP.Rows.Count - 1]["Description"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = DateTime.Now;
                                RowRelease["Last Update Date"] = DateTime.Now;


                                for (int JPColumn_His = 0; JPColumn_His < tblSkillEnumJP_History.Columns.Count; JPColumn_His++)
                                {
                                    RowJP_History[JPColumn_His] = Row_History[JPColumn_His];
                                }

                                Adt_SkillEnumJP.Update(RowJP);
                                Adt_SkillEnumJP_History.Update(RowJP_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/


                                //09. 06. 19 김정협 Release
                                drArray = new DataRow[tblRelease.Rows.Count];
                                tblRelease.Rows.CopyTo(drArray, 0);
                                Adpt_Release.Update(drArray);

                                /*drArray = new DataRow[tblRelease_History.Rows.Count];
                                tblRelease_History.Rows.CopyTo(drArray, 0);
                                Adpt_Release_History.Update(drArray);*/

                            }
#endif //__COUNTRY_KR__
                        }
                        break;
                }
            }

#if __COUNTRY_KR__
            for (int i = 0; i < m_DeleteRowList.Count; i++)
            {
                switch (((sDeleteRow)m_DeleteRowList[i]).nTabIndex)
                {
                    case 0:
                        {
                            tblSkill_DeleteHistory.Rows.Add();

                            Row_History = tblSkill_DeleteHistory.Rows[tblSkill_DeleteHistory.Rows.Count - 1];

                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSkill.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            Row_History["Regdate"] = DateTime.Now;

                            Adpt_SkillTable.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                            Adpt_SkillTable_DeleteHistory.Update(new DataRow[] { Row_History });


                            //09. 06. 19 김정협
                            //외국쪽 지우는 것
                            //
                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////

                            DataRow drTW = null;

                            DataRow drTW_DeleteHistory = null;

                            tblSkillTW_DeleteHistory.Rows.Add();
                            drTW_DeleteHistory = tblSkillTW_DeleteHistory.Rows[tblSkillTW_DeleteHistory.Rows.Count - 1];

                            for (int twRow = 0; twRow < tblSkillTW.Rows.Count; twRow++)
                            {
                                if (tblSkillTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drTW = tblSkillTW.Rows[twRow];
                                    break;
                                }
                            }

                            for (int twColumn = 0; twColumn < tblSkillTW.Columns.Count - 1; twColumn++)
                            {
                                drTW_DeleteHistory[twColumn] = drTW[twColumn];
                            }
                            drTW_DeleteHistory["UserID"] = m_strID;
                            drTW_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drTW_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SkillTableTW_DeleteHistory.Update(drTW_DeleteHistory);

                            int nDeleteIndex = Convert.ToInt32(drTW["IndexKey"].ToString());

                            drTW.Delete();
                            Adt_SkillTableTW.Update(drTW);
                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drHK = null;

                            DataRow drHK_DeleteHistory = null;

                            tblSkillHK_DeleteHistory.Rows.Add();
                            drHK_DeleteHistory = tblSkillHK_DeleteHistory.Rows[tblSkillHK_DeleteHistory.Rows.Count - 1];

                            for (int HKRow = 0; HKRow < tblSkillHK.Rows.Count; HKRow++)
                            {
                                if (tblSkillHK.Rows[HKRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drHK = tblSkillHK.Rows[HKRow];
                                    break;
                                }
                            }

                            for (int HKColumn = 0; HKColumn < tblSkillHK.Columns.Count - 1; HKColumn++)
                            {
                                drHK_DeleteHistory[HKColumn] = drHK[HKColumn];
                            }
                            drHK_DeleteHistory["UserID"] = m_strID;
                            drHK_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drHK_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SkillTableHK_DeleteHistory.Update(drHK_DeleteHistory);

                            

                            drHK.Delete();
                            Adt_SkillTableHK.Update(drHK);

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drJP = null;

                            DataRow drJP_DeleteHistory = null;

                            tblSkillJP_DeleteHistory.Rows.Add();
                            drJP_DeleteHistory = tblSkillJP_DeleteHistory.Rows[tblSkillJP_DeleteHistory.Rows.Count - 1];

                            for (int JPRow = 0; JPRow < tblSkillJP.Rows.Count; JPRow++)
                            {
                                if (tblSkillJP.Rows[JPRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drJP = tblSkillJP.Rows[JPRow];
                                    break;
                                }
                            }

                            for (int JPColumn = 0; JPColumn < tblSkillJP.Columns.Count - 1; JPColumn++)
                            {
                                drJP_DeleteHistory[JPColumn] = drJP[JPColumn];
                            }
                            drJP_DeleteHistory["UserID"] = m_strID;
                            drJP_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drJP_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SkillTableJP_DeleteHistory.Update(drJP_DeleteHistory);



                            drJP.Delete();
                            Adt_SkillTableJP.Update(drJP);

                            Adpt_Trans.DeleteIndexRow(nDeleteIndex, "SkillTable");

                            //Adpt_Release.DeleteIndexRow(nDeleteIndex, "SkillTable");
                        }
                        break;

                    case 16:
                        {
                            tblSkillEnum_DeleteHistory.Rows.Add();

                            Row_History = tblSkillEnum_DeleteHistory.Rows[tblSkillEnum_DeleteHistory.Rows.Count - 1];

                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSkillEnum.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];


                            Adpt_SkillEnum.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                            Adpt_SkillEnum_DeleteHistory.Update(new DataRow[] { Row_History });

                            //09. 06. 19 김정협
                            //외국쪽 지우는 것
                            //
                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////

                            DataRow drTW = null;

                            DataRow drTW_DeleteHistory = null;

                            tblSkillEnumTW_DeleteHistory.Rows.Add();
                            drTW_DeleteHistory = tblSkillEnumTW_DeleteHistory.Rows[tblSkillEnumTW_DeleteHistory.Rows.Count - 1];

                            for (int twRow = 0; twRow < tblSkillEnumTW.Rows.Count; twRow++)
                            {
                                if (tblSkillEnumTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drTW = tblSkillEnumTW.Rows[twRow];
                                    break;
                                }
                            }

                            for (int twColumn = 0; twColumn < tblSkillEnumTW.Columns.Count - 1; twColumn++)
                            {
                                drTW_DeleteHistory[twColumn] = drTW[twColumn];
                            }
                            drTW_DeleteHistory["UserID"] = m_strID;
                            drTW_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drTW_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SkillEnumTW_DeleteHistory.Update(drTW_DeleteHistory);

                            int nDeleteIndex = Convert.ToInt32(drTW["IndexKey"].ToString());

                            drTW.Delete();
                            Adt_SkillEnumTW.Update(drTW);
                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drHK = null;

                            DataRow drHK_DeleteHistory = null;

                            tblSkillEnumHK_DeleteHistory.Rows.Add();
                            drHK_DeleteHistory = tblSkillEnumHK_DeleteHistory.Rows[tblSkillEnumHK_DeleteHistory.Rows.Count - 1];

                            for (int HKRow = 0; HKRow < tblSkillEnumHK.Rows.Count; HKRow++)
                            {
                                if (tblSkillEnumHK.Rows[HKRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drHK = tblSkillEnumHK.Rows[HKRow];
                                    break;
                                }
                            }

                            for (int HKColumn = 0; HKColumn < tblSkillEnumHK.Columns.Count - 1; HKColumn++)
                            {
                                drHK_DeleteHistory[HKColumn] = drHK[HKColumn];
                            }
                            drHK_DeleteHistory["UserID"] = m_strID;
                            drHK_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drHK_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SkillEnumHK_DeleteHistory.Update(drHK_DeleteHistory);

                            

                            drHK.Delete();
                            Adt_SkillEnumHK.Update(drHK);

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drJP = null;

                            DataRow drJP_DeleteHistory = null;

                            tblSkillEnumJP_DeleteHistory.Rows.Add();
                            drJP_DeleteHistory = tblSkillEnumJP_DeleteHistory.Rows[tblSkillEnumJP_DeleteHistory.Rows.Count - 1];

                            for (int JPRow = 0; JPRow < tblSkillEnumJP.Rows.Count; JPRow++)
                            {
                                if (tblSkillEnumJP.Rows[JPRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drJP = tblSkillEnumJP.Rows[JPRow];
                                    break;
                                }
                            }

                            for (int JPColumn = 0; JPColumn < tblSkillEnumJP.Columns.Count - 1; JPColumn++)
                            {
                                drJP_DeleteHistory[JPColumn] = drJP[JPColumn];
                            }
                            drJP_DeleteHistory["UserID"] = m_strID;
                            drJP_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drJP_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SkillEnumJP_DeleteHistory.Update(drJP_DeleteHistory);



                            drJP.Delete();
                            Adt_SkillEnumJP.Update(drJP);

//                            Adpt_Trans.DeleteIndexRow(nDeleteIndex, "SkillEnum");                            

                            Adpt_Release.DeleteIndexRow(nDeleteIndex, "SkillEnum");
                        }
                        break;

                }
            }
#endif //__COUNTRY_KR__

            Adpt_ColorTable.Update(tblColor);
            Adpt_BorderTable.Update(tblBorder);

            tblColor.Clear();
            Adpt_ColorTable.Fill(tblColor);
            tblBorder.Clear();
            Adpt_BorderTable.Fill(tblBorder);

            Adpt_Elsword.Update(tblElsword);
            Adpt_Elsword_MK.Update(tblElsword_MK);
            Adpt_Elsword_SK.Update(tblElsword_SK);
            Adpt_Aisha.Update(tblAisha);
            Adpt_Aisha_HM.Update(tblAisha_HM);
            Adpt_Aisha_DM.Update(tblAisha_DM);
            Adpt_Rena.Update(tblRena);
            Adpt_Rena_SR.Update(tblRena_SR);
            Adpt_Rena_CR.Update(tblRena_CR);
            Adpt_Raven.Update(tblRaven);
            Adpt_Raven_OT.Update(tblRaven_OT);
            Adpt_Raven_ST.Update(tblRaven_ST);
            Adpt_Eve.Update(tblEve);
            Adpt_Eve_CA.Update(tblEve_CA);
            Adpt_Eve_CE.Update(tblEve_CE);


            tblSkill.Clear();
            Adpt_SkillTable.Fill(tblSkill);

            tblSkillEnum.Clear();
            Adpt_SkillEnum.Fill(tblSkillEnum);

            SetCellBackgroundColorForDB(-1);

            m_DeleteRowList.Clear();
            m_ModifyIDList.Clear();
            m_UndoStack.Clear();
            m_RedoStack.Clear();
            m_bSaveEnd = true;


            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["NAME", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 1;
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView17.RowCount; nRowHeader++)
            {
                dataGridView17.Rows[nRowHeader].HeaderCell.Value = dataGridView17["ID Num", nRowHeader].Value.ToString() + "     " + dataGridView17["Description", nRowHeader].Value.ToString();
            }

            //09. 07. 01 김정협 현재 위치 기억
            tabControl1.SelectedIndex = nCurrentPosTabIndex;
            switch (nCurrentPosTabIndex)
            {
                case 0:
                    dataGridView1.CurrentCell = dataGridView1[nCurrentPosColumnIndex, nCurrentPosRowIndex];
                    break;
                case 1:
                    dataGridView17.CurrentCell = dataGridView17[nCurrentPosColumnIndex, nCurrentPosRowIndex];
                    break;
            }

        }

        private void SkillTable_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();
            //국가별 define 정의
#if __COUNTRY_KR__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";
            btnAdd.Enabled = true;            
#elif __COUNTRY_TW__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_TW; uid=Script_User; pwd=x2_tmzmflqxm";
            btnAdd.Enabled = false;            
#elif __COUNTRY_HK__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_HK; uid=Script_User; pwd=x2_tmzmflqxm";
            btnAdd.Enabled = false;            
#elif __COUNTRY_JP__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_JP; uid=Script_User; pwd=x2_tmzmflqxm";
            btnAdd.Enabled = false;            
#endif
            Con.Open();

            dataGridView1.ContextMenuStrip = contextMenuStrip1;

            Adpt_SkillTable = new SqlDataAdapter("SELECT * FROM SkillTable", Con);
            tblSkill = new DataTable("tblSkill");

            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt_SkillTable);
            Adpt_SkillTable.Fill(tblSkill);
            dataGridView1.DataSource = tblSkill;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView1.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView1.Columns["Regdate"].ReadOnly = true; //Reg_data수정방지

            dataGridView1.Columns["ID_ENUM"].ReadOnly = true; //수정방지
            dataGridView1.Columns["TYPE_ENUM"].ReadOnly = true; //수정방지
            dataGridView1.Columns["NAME"].ReadOnly = true; //수정방지
            dataGridView1.Columns["ICON_TEXTURE_PIECE_NAME"].ReadOnly = true; //수정방지

            dataGridView1.Columns["ACTIVE_SKILL_USE_CONDITION_ENUM"].ReadOnly = true; //수정방지
            dataGridView1.Columns["Description"].ReadOnly = true; //수정방지

            dataGridView1.Columns["ID_ENUM"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["TYPE_ENUM"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["NAME"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["ICON_TEXTURE_PIECE_NAME"].DefaultCellStyle.BackColor = Color.LightGray;

            dataGridView1.Columns["ACTIVE_SKILL_USE_CONDITION_ENUM"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["Description"].DefaultCellStyle.BackColor = Color.LightGray;

            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }
            //09. 06. 01 김정협 수정 : 아이디로 오름차순 정렬
            DataGridViewColumn Column_1 = dataGridView1.Columns[1];//dataGridView1.Columns["ID"];
            ListSortDirection direction_1 = ListSortDirection.Ascending;
            dataGridView1.Sort(Column_1, direction_1);


            //History
            Adpt_SkillTable_History = new SqlDataAdapter("SELECT * FROM SkillTable_History", Con);
            tblSkill_History = new DataTable("tblSkill_History");
            SqlCommandBuilder Builder_History = new SqlCommandBuilder(Adpt_SkillTable_History);
            Adpt_SkillTable_History.Fill(tblSkill_History);

            //Delete
            Adpt_SkillTable_DeleteHistory = new SqlDataAdapter("SELECT * FROM SkillTable_DeleteHistory", Con);
            tblSkill_DeleteHistory = new DataTable("tblSkill_DeleteHistory");
            SqlCommandBuilder Builder_SkillTable_Delete = new SqlCommandBuilder(Adpt_SkillTable_DeleteHistory);
            Adpt_SkillTable_DeleteHistory.Fill(tblSkill_DeleteHistory);

            //09. 06. 19 김정협 Trans/Release 로드
            //Trans
            //Adpt_String = new SqlDataAdapter("SELECT * FROM ScriptStringTable", Con);
            tblTrans = Adpt_Trans.GetData();
            tblTrans_History = Adpt_Trans_History.GetData();
            //Release
            tblRelease = Adpt_Release.GetData();
            tblRelease_History = Adpt_Release_History.GetData();
            //SqlCommandBuilder Builder_String = new SqlCommandBuilder(Adpt_String);



            //Skill_Elsword
            Adpt_Elsword = new SqlDataAdapter("SELECT * FROM Skill_Elsword", Con);
            tblElsword = new DataTable("tblElsword");
            Builder = new SqlCommandBuilder(Adpt_Elsword);
            Adpt_Elsword.Fill(tblElsword);

            //Skill_Elsword_SK
            Adpt_Elsword_SK = new SqlDataAdapter("SELECT * FROM Skill_Elsword_SK", Con);
            tblElsword_SK = new DataTable("tblElsword_SK");
            Builder = new SqlCommandBuilder(Adpt_Elsword_SK);
            Adpt_Elsword_SK.Fill(tblElsword_SK);

            //Skill_Elsword_MK
            Adpt_Elsword_MK = new SqlDataAdapter("SELECT * FROM Skill_Elsword_MK", Con);
            tblElsword_MK = new DataTable("tblElsword_MK");
            Builder = new SqlCommandBuilder(Adpt_Elsword_MK);
            Adpt_Elsword_MK.Fill(tblElsword_MK);

            //Skill_Aisha
            Adpt_Aisha = new SqlDataAdapter("SELECT * FROM Skill_Aisha", Con);
            tblAisha = new DataTable("tblAisha");
            Builder = new SqlCommandBuilder(Adpt_Aisha);
            Adpt_Aisha.Fill(tblAisha);

            //Skill_Aisha_SK
            Adpt_Aisha_HM = new SqlDataAdapter("SELECT * FROM Skill_Aisha_HM", Con);
            tblAisha_HM = new DataTable("tblAisha_HM");
            Builder = new SqlCommandBuilder(Adpt_Aisha_HM);
            Adpt_Aisha_HM.Fill(tblAisha_HM);

            //Skill_Aisha_DM
            Adpt_Aisha_DM = new SqlDataAdapter("SELECT * FROM Skill_Aisha_DM", Con);
            tblAisha_DM = new DataTable("tblAisha_DM");
            Builder = new SqlCommandBuilder(Adpt_Aisha_DM);
            Adpt_Aisha_DM.Fill(tblAisha_DM);

            //Skill_Rena
            Adpt_Rena = new SqlDataAdapter("SELECT * FROM Skill_Rena", Con);
            tblRena = new DataTable("tblRena");
            Builder = new SqlCommandBuilder(Adpt_Rena);
            Adpt_Rena.Fill(tblRena);

            //Skill_Rena_CR
            Adpt_Rena_CR = new SqlDataAdapter("SELECT * FROM Skill_Rena_CR", Con);
            tblRena_CR = new DataTable("tblRena_CR");
            Builder = new SqlCommandBuilder(Adpt_Rena_CR);
            Adpt_Rena_CR.Fill(tblRena_CR);

            //Skill_Rena_SR
            Adpt_Rena_SR = new SqlDataAdapter("SELECT * FROM Skill_Rena_SR", Con);
            tblRena_SR = new DataTable("tblRena_SR");
            Builder = new SqlCommandBuilder(Adpt_Rena_SR);
            Adpt_Rena_SR.Fill(tblRena_SR);

            //Skill_Raven
            Adpt_Raven = new SqlDataAdapter("SELECT * FROM Skill_Raven", Con);
            tblRaven = new DataTable("tblRaven");
            Builder = new SqlCommandBuilder(Adpt_Raven);
            Adpt_Raven.Fill(tblRaven);

            //Skill_Raven_SK
            Adpt_Raven_OT = new SqlDataAdapter("SELECT * FROM Skill_Raven_OT", Con);
            tblRaven_OT = new DataTable("tblRaven_OT");
            Builder = new SqlCommandBuilder(Adpt_Raven_OT);
            Adpt_Raven_OT.Fill(tblRaven_OT);

            //Skill_Raven_ST
            Adpt_Raven_ST = new SqlDataAdapter("SELECT * FROM Skill_Raven_ST", Con);
            tblRaven_ST = new DataTable("tblRaven_ST");
            Builder = new SqlCommandBuilder(Adpt_Raven_ST);
            Adpt_Raven_ST.Fill(tblRaven_ST);

            //Skill_Eve
            Adpt_Eve = new SqlDataAdapter("SELECT * FROM Skill_Eve", Con);
            tblEve = new DataTable("tblEve");
            Builder = new SqlCommandBuilder(Adpt_Eve);
            Adpt_Eve.Fill(tblEve);

            //Skill_Eve_SK
            Adpt_Eve_CE = new SqlDataAdapter("SELECT * FROM Skill_Eve_CE", Con);
            tblEve_CE = new DataTable("tblEve_CE");
            Builder = new SqlCommandBuilder(Adpt_Eve_CE);
            Adpt_Eve_CE.Fill(tblEve_CE);

            //Skill_Eve_MK
            Adpt_Eve_CA = new SqlDataAdapter("SELECT * FROM Skill_Eve_CA", Con);
            tblEve_CA = new DataTable("tblEve_CA");
            Builder = new SqlCommandBuilder(Adpt_Eve_CA);
            Adpt_Eve_CA.Fill(tblEve_CA);


            Adpt_SkillEnum = new SqlDataAdapter("SELECT * FROM SkillEnum", Con);
            tblSkillEnum = new DataTable("tblSkillEnum");

            Builder = new SqlCommandBuilder(Adpt_SkillEnum);
            Adpt_SkillEnum.Fill(tblSkillEnum);

            tblSkillEnum.PrimaryKey = new DataColumn[] { tblSkillEnum.Columns["ID Num"] };

            dataGridView17.DataSource = tblSkillEnum;
            dataGridView17.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView17.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView17.Columns["Regdate"].ReadOnly = true; //Reg_data수정방지


            for (int i = 0; i < dataGridView17.ColumnCount; i++)
            {
                dataGridView17.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }
            //09. 06. 01 김정협 수정 : 아이디로 오름차순 정렬
            DataGridViewColumn Column_2 = dataGridView17.Columns[1];//dataGridView1.Columns["ID"];
            ListSortDirection direction_2 = ListSortDirection.Ascending;
            dataGridView17.Sort(Column_2, direction_2);

            //History
            Adpt_SkillEnum_History = new SqlDataAdapter("SELECT * FROM SkillEnum_History", Con);
            tblSkillEnum_History = new DataTable("tblSkillEnum_History");
            Builder_History = new SqlCommandBuilder(Adpt_SkillEnum_History);
            Adpt_SkillEnum_History.Fill(tblSkillEnum_History);

            //Delete
            Adpt_SkillEnum_DeleteHistory = new SqlDataAdapter("SELECT * FROM SkillEnum_DeleteHistory", Con);
            tblSkillEnum_DeleteHistory = new DataTable("tblSkillEnum_DeleteHistory");
            Builder_SkillTable_Delete = new SqlCommandBuilder(Adpt_SkillEnum_DeleteHistory);
            Adpt_SkillEnum_DeleteHistory.Fill(tblSkillEnum_DeleteHistory);
            

            SetCharacterSkillView(dataGridView2, tblElsword);
            SetCharacterSkillView(dataGridView3, tblElsword_SK);
            SetCharacterSkillView(dataGridView4, tblElsword_MK);

            SetCharacterSkillView(dataGridView5, tblAisha);
            SetCharacterSkillView(dataGridView6, tblAisha_HM);
            SetCharacterSkillView(dataGridView7, tblAisha_DM);

            SetCharacterSkillView(dataGridView8, tblRena);
            SetCharacterSkillView(dataGridView9, tblRena_CR);
            SetCharacterSkillView(dataGridView10, tblRena_SR);

            SetCharacterSkillView(dataGridView11, tblRaven);
            SetCharacterSkillView(dataGridView12, tblRaven_OT);
            SetCharacterSkillView(dataGridView13, tblRaven_ST);

            SetCharacterSkillView(dataGridView14, tblEve);
            SetCharacterSkillView(dataGridView15, tblEve_CA);
            SetCharacterSkillView(dataGridView16, tblEve_CE);
            //dataGridView2.GridColor = Color.Black;

            //09. 06. 03 김정협 추가 : 셀배경색 DB
            ////ColorTable용        
            Adpt_ColorTable = new SqlDataAdapter("SELECT * FROM ColorTable", Con);
            tblColor = new DataTable("tblColor");
            SqlCommandBuilder Builder_Color = new SqlCommandBuilder(Adpt_ColorTable);
            Adpt_ColorTable.Fill(tblColor);


            //09. 07. 07 김정협 추가 : Border추가
            ////BorderTable용        
            Adpt_BorderTable = new SqlDataAdapter("SELECT * FROM BorderTable", Con);
            tblBorder = new DataTable("tblBorder");
            SqlCommandBuilder Builder_Border = new SqlCommandBuilder(Adpt_BorderTable);
            Adpt_BorderTable.Fill(tblBorder);


            //SkillType용        
            Adpt_SkillType = new SqlDataAdapter("SELECT * FROM SkillType", Con);
            tblSkillType = new DataTable("tblSkillType");
            Adpt_SkillType.Fill(tblSkillType);
            tblSkillType.PrimaryKey = new DataColumn[] { tblSkillType.Columns["Skill Type Num"] };
            
            //ActiveSkillUseCondition
            Adpt_ActiveSkillUseCondition = new SqlDataAdapter("SELECT * FROM ACTIVE_SKILL_USE_CONDITION", Con);
            tblActiveSkillUseCondition = new DataTable("tblActiveSkillUseCondition");
            Adpt_ActiveSkillUseCondition.Fill(tblActiveSkillUseCondition);
            tblActiveSkillUseCondition.PrimaryKey = new DataColumn[] {tblActiveSkillUseCondition.Columns["ACTIVE_SKILL_USE_CONDITION_NUM"]};


            SetCellBackgroundColorForDB(-1);

            dataGridView1.RowHeadersWidth = 130;
            dataGridView1.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;

            dataGridView17.RowHeadersWidth = 130;
            dataGridView17.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;

            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["NAME", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 16;
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView17.RowCount; nRowHeader++)
            {
                dataGridView17.Rows[nRowHeader].HeaderCell.Value = dataGridView17["ID Num", nRowHeader].Value.ToString() + "     " + dataGridView17["Description", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 0;
            
        }


        //09. 07. 01 김정협 ADD 창 떠 있을 시 계속 추가되게하는 부분
        SkillTableAdd frmSkillTable = new SkillTableAdd();
        SkillEnumAdd frmSkillEnum = new SkillEnumAdd();
        private void btnAdd_Click(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        if (!frmSkillTable.Visible)
                        {
                            frmSkillTable = new SkillTableAdd();
                        }
                        frmSkillTable.SetDataGridView(dataGridView1);
                        frmSkillTable.SetRowIndex(dataGridView1.CurrentCell.RowIndex);
                        frmSkillTable.SetDataTable(tblSkill);
                        frmSkillTable.SetModifyArray(m_ModifyIDList);
                        //09. 06. 19 김정협 Trans 추가
                        frmSkillTable.SetTrans(tblTrans);
                        //frmSkillTable.SetRelease(tblRelease);

                        frmSkillTable.SetUndoStack(m_UndoStack);
                        frmSkillTable.SetRedoStack(m_RedoStack);

                        //09. 06. 06 김정협 자동변환으로 인해 추가
                        frmSkillTable.SetSkilEnum(tblSkillEnum);
                        frmSkillTable.SetSkilType(tblSkillType);
                        frmSkillTable.SetActiveSkillUseCondition(tblActiveSkillUseCondition);


                        frmSkillTable.Owner = this;

                        if (!frmSkillTable.Visible)
                        {
                            //frm.SetTableNew();
                            frmSkillTable.Show(this);
                        }
                        else
                        {
                            frmSkillTable.ADDItem();// ItemAdd_Load(null, null);
                        }
                    }
                    break;
                case 16:
                    {
                        if (!frmSkillEnum.Visible)
                        {
                            frmSkillEnum = new SkillEnumAdd();
                        }
                        frmSkillEnum.SetDataGridView(dataGridView17);
                        frmSkillEnum.SetRowIndex(dataGridView17.CurrentCell.RowIndex);
                        frmSkillEnum.SetDataTable(tblSkillEnum);
                        frmSkillEnum.SetModifyArray(m_ModifyIDList);
                        //09. 06. 19 김정협 Trans 추가
                        frmSkillEnum.SetTrans(tblTrans);
                        frmSkillEnum.SetRelease(tblRelease);

                        frmSkillEnum.SetUndoStack(m_UndoStack);
                        frmSkillEnum.SetRedoStack(m_RedoStack);


                        frmSkillEnum.Owner = this;

                        if (!frmSkillEnum.Visible)
                        {
                            //frm.SetTableNew();
                            frmSkillEnum.Show(this);
                        }
                        else
                        {
                            frmSkillEnum.ADDItem();// ItemAdd_Load(null, null);
                        }
                    }
                    break;
                case 1:
                    {
                        tblElsword.Rows.Add();
                        for(int k = 1; k < tblElsword.Columns.Count - 1; k++)
                        {
                            tblElsword.Rows[tblElsword.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView2, tblElsword);
                    }
                    break;
                case 2:
                    {
                        tblElsword_SK.Rows.Add();
                        for (int k = 1; k < tblElsword_SK.Columns.Count - 1; k++)
                        {
                            tblElsword_SK.Rows[tblElsword_SK.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView3, tblElsword_SK);
                    }
                    break;
                case 3:
                    {
                        tblElsword_MK.Rows.Add();
                        for (int k = 1; k < tblElsword_MK.Columns.Count - 1; k++)
                        {
                            tblElsword_MK.Rows[tblElsword_MK.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView4, tblElsword_MK);
                    }
                    break;
                case 4:
                    {
                        tblAisha.Rows.Add();
                        for (int k = 1; k < tblAisha.Columns.Count - 1; k++)
                        {
                            tblAisha.Rows[tblAisha.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView5, tblAisha);
                    }
                    break;
                case 5:
                    {
                        tblAisha_HM.Rows.Add();
                        for (int k = 1; k < tblAisha_HM.Columns.Count - 1; k++)
                        {
                            tblAisha_HM.Rows[tblAisha_HM.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView6, tblAisha_HM);
                    }
                    break;
                case 6:
                    {
                        tblAisha_DM.Rows.Add();
                        for (int k = 1; k < tblAisha_DM.Columns.Count - 1; k++)
                        {
                            tblAisha_DM.Rows[tblAisha_DM.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView7, tblAisha_DM);
                    }
                    break;
                case 7:
                    {
                        tblRena.Rows.Add();
                        for (int k = 1; k < tblRena.Columns.Count - 1; k++)
                        {
                            tblRena.Rows[tblRena.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView8, tblRena);
                    }
                    break;
                case 8:
                    {
                        tblRena_CR.Rows.Add();
                        for (int k = 1; k < tblRena_CR.Columns.Count - 1; k++)
                        {
                            tblRena_CR.Rows[tblRena_CR.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView9, tblRena_CR);
                    }
                    break;
                case 9:
                    {
                        tblRena_SR.Rows.Add();
                        for (int k = 1; k < tblRena_SR.Columns.Count - 1; k++)
                        {
                            tblRena_SR.Rows[tblRena_SR.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView10, tblRena_SR);
                    }
                    break;
                case 10:
                    {
                        tblRaven.Rows.Add();
                        for (int k = 1; k < tblRaven.Columns.Count - 1; k++)
                        {
                            tblRaven.Rows[tblRaven.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView11, tblRaven);
                    }
                    break;
                case 11:
                    {
                        tblRaven_OT.Rows.Add();
                        for (int k = 1; k < tblRaven_OT.Columns.Count - 1; k++)
                        {
                            tblRaven_OT.Rows[tblRaven_OT.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView12, tblRaven_OT);
                    }
                    break;
                case 12:
                    {
                        tblRaven_ST.Rows.Add();
                        for (int k = 1; k < tblRaven_ST.Columns.Count - 1; k++)
                        {
                            tblRaven_ST.Rows[tblRaven_ST.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView13, tblRaven_ST);
                    }
                    break;
                case 13:
                    {
                        tblEve.Rows.Add();
                        for (int k = 1; k < tblEve.Columns.Count - 1; k++)
                        {
                            tblEve.Rows[tblEve.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView14, tblEve);
                    }
                    break;
                case 14:
                    {
                        tblEve_CA.Rows.Add();
                        for (int k = 1; k < tblEve_CA.Columns.Count - 1; k++)
                        {
                            tblEve_CA.Rows[tblEve_CA.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView15, tblEve_CA);
                    }
                    break;
                case 15:
                    {
                        tblEve_CE.Rows.Add();
                        for (int k = 1; k < tblEve_CE.Columns.Count - 1; k++)
                        {
                            tblEve_CE.Rows[tblEve_CE.Rows.Count - 1][k] = 0;
                        }
                        SetCharacterSkillView(dataGridView16, tblEve_CE);
                    }
                    break;
            }
        }

        private void SkillTable_Resize(object sender, EventArgs e)
        {
            tabControl1.Width = this.Width - 11;
            tabControl1.Height = this.Height - 136;
            dataGridView1.Width = tabControl1.Width - 13;
            dataGridView1.Height = tabControl1.Height - 29;

            dataGridView2.Width = tabControl1.Width - 13;
            dataGridView2.Height = tabControl1.Height - 29;

            dataGridView3.Width = tabControl1.Width - 13;
            dataGridView3.Height = tabControl1.Height - 29;

            dataGridView4.Width = tabControl1.Width - 13;
            dataGridView4.Height = tabControl1.Height - 29;

            dataGridView5.Width = tabControl1.Width - 13;
            dataGridView5.Height = tabControl1.Height - 29;

            dataGridView6.Width = tabControl1.Width - 13;
            dataGridView6.Height = tabControl1.Height - 29;

            dataGridView7.Width = tabControl1.Width - 13;
            dataGridView7.Height = tabControl1.Height - 29;

            dataGridView8.Width = tabControl1.Width - 13;
            dataGridView8.Height = tabControl1.Height - 29;

            dataGridView9.Width = tabControl1.Width - 13;
            dataGridView9.Height = tabControl1.Height - 29;

            dataGridView10.Width = tabControl1.Width - 13;
            dataGridView10.Height = tabControl1.Height - 29;

            dataGridView11.Width = tabControl1.Width - 13;
            dataGridView11.Height = tabControl1.Height - 29;

            dataGridView12.Width = tabControl1.Width - 13;
            dataGridView12.Height = tabControl1.Height - 29;

            dataGridView13.Width = tabControl1.Width - 13;
            dataGridView13.Height = tabControl1.Height - 29;

            dataGridView14.Width = tabControl1.Width - 13;
            dataGridView14.Height = tabControl1.Height - 29;

            dataGridView15.Width = tabControl1.Width - 13;
            dataGridView15.Height = tabControl1.Height - 29;

            dataGridView16.Width = tabControl1.Width - 13;
            dataGridView16.Height = tabControl1.Height - 29;

            dataGridView17.Width = tabControl1.Width - 13;
            dataGridView17.Height = tabControl1.Height - 29;

            btnAdd.Left = tabControl1.Location.X + 7;
            btnAdd.Top = tabControl1.Location.Y + tabControl1.Height + 5;

            btnSave.Left = btnAdd.Location.X + btnAdd.Width + 25;
            btnSave.Top = btnAdd.Location.Y;
            btnFileGenerate.Left = btnSave.Location.X + btnSave.Width + 25;
            btnFileGenerate.Top = btnAdd.Location.Y;
            btnHistory.Left = btnFileGenerate.Location.X + btnFileGenerate.Width + 25;
            btnHistory.Top = btnAdd.Location.Y;
            btnString.Left = btnHistory.Location.X + btnHistory.Width + 25;
            btnString.Top = btnAdd.Location.Y;
        }


        private void btnHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.Text = "History";
            if (tabControl1.SelectedIndex == 0)
            {
                frm.SetHistoryTable(tblSkill_History);
            }
            else if (tabControl1.SelectedIndex == 16)
            {
                frm.SetHistoryTable(tblSkillEnum_History);
            }
            frm.ShowDialog(this);

        }

        private void btnString_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.Text = "String";
            frm.SetHistoryTable(tblTrans);
            frm.ShowDialog(this);
        }


        private void dataGridView2_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            //for (int i = 1; i <= tblElsword.Rows.Count * 3; i += 3)
            {                

              //  for (int k = 0; k < 5; k++)
                {
                    if (e.RowIndex == 1 && e.ColumnIndex == 1)
                    {
                        //e.AdvancedBorderStyle.Top = DataGridViewAdvancedCellBorderStyle.OutsetDouble;
                        //e.AdvancedBorderStyle.Left = DataGridViewAdvancedCellBorderStyle.OutsetDouble;
                    }
                    
                    
               }
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
                if (dataGridView2.CurrentCell.Value == null)
                {
                    dataGridView2.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView2, tblElsword);

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
                if (dataGridView3.CurrentCell.Value == null)
                {
                    dataGridView3.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView3, tblElsword_SK);
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
                if (dataGridView4.CurrentCell.Value == null)
                {
                    dataGridView4.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView4, tblElsword_MK);

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
                if (dataGridView5.CurrentCell.Value == null)
                {
                    dataGridView5.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView5, tblAisha);

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
                if (dataGridView6.CurrentCell.Value == null)
                {
                    dataGridView6.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView6, tblAisha_HM);

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
                if (dataGridView7.CurrentCell.Value == null)
                {
                    dataGridView7.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView7, tblAisha_DM);

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
                if (dataGridView8.CurrentCell.Value == null)
                {
                    dataGridView8.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView8, tblRena);

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
                if (dataGridView9.CurrentCell.Value == null)
                {
                    dataGridView9.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView9, tblRena_CR);

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
                if (dataGridView10.CurrentCell.Value == null)
                {
                    dataGridView10.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView10, tblRena_SR);

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
                if (dataGridView11.CurrentCell.Value == null)
                {
                    dataGridView11.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView11, tblRaven);

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
                if (dataGridView12.CurrentCell.Value == null)
                {
                    dataGridView12.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView12, tblRaven_OT);

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
                if (dataGridView13.CurrentCell.Value == null)
                {
                    dataGridView13.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView13, tblRaven_ST);

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
                if (dataGridView14.CurrentCell.Value == null)
                {
                    dataGridView14.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView14, tblEve);

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
                if (dataGridView15.CurrentCell.Value == null)
                {
                    dataGridView15.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView15, tblEve_CA);

                m_bSaveEnd = false;
            }
        }

        private void dataGridView16_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView16.CurrentCell.Value;
        }

        private void dataGridView16_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {

            if (!objTemp.Equals((object)dataGridView16.CurrentCell.Value))
            {
                if (dataGridView16.CurrentCell.Value == null)
                {
                    dataGridView16.CurrentCell.Value = "";
                }
                SkillDataLink(dataGridView16, tblEve_CE);

                m_bSaveEnd = false;
            }
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("저장하시겠습니까?", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                return;
            }
            Save();
            MessageBox.Show("저장되었습니다.");
        }

        private void btnFileGenerate_Click(object sender, EventArgs e)
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
                    Save();
                }
                else if (result == DialogResult.Cancel)
                {
                    MessageBox.Show("저장하지 않으시면 파일을 생성할 수 없습니다.");
                    return;
                }
            }


            string strCountryCode = "";
            StreamWriter sw;
            //국가별 define정의할 것
#if __COUNTRY_KR__
            string strFileName = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Major\\NewSkillTemplet.lua";
            strCountryCode = "KR";
            sw = new StreamWriter(strFileName, false, Encoding.Default);
#elif __COUNTRY_TW__
            string strFileName = "..\\..\\..\\resource\\TW\\Trunk\\dat\\Script\\Major\\NewSkillTemplet.lua";
            strCountryCode = "TW";
            sw = new StreamWriter(strFileName, false, Encoding.UTF8);
            sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_HK__
            string strFileName = "..\\..\\..\\resource\\HK\\Trunk\\dat\\Script\\Major\\NewSkillTemplet.lua";
            strCountryCode = "HK";
            sw = new StreamWriter(strFileName, false, Encoding.UTF8);
            sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_JP__
            string strFileName = "..\\..\\..\\resource\\JP\\Trunk\\dat\\Script\\Major\\NewSkillTemplet.lua";
            strCountryCode = "JP";
            sw = new StreamWriter(strFileName, false, Encoding.UTF8);
            sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#endif
            //sw = new StreamWriter(strFileName, false, Encoding.Default);
            

            const int MAX_UNIT_CLASS = 15;
            string[] strUnitClass = new string[MAX_UNIT_CLASS];
            strUnitClass[0] = "UNIT_CLASS[\"UC_ELSWORD_SWORDMAN\"]";
            strUnitClass[1] = "UNIT_CLASS[\"UC_ARME_VIOLET_MAGE\"]";
            strUnitClass[2] = "UNIT_CLASS[\"UC_LIRE_ELVEN_RANGER\"]";
            strUnitClass[3] = "UNIT_CLASS[\"UC_RAVEN_FIGHTER\"]";
            strUnitClass[4] = "UNIT_CLASS[\"UC_EVE_NASOD\"]";
            strUnitClass[5] = "UNIT_CLASS[\"UC_ELSWORD_KNIGHT\"]";
            strUnitClass[6] = "UNIT_CLASS[\"UC_ELSWORD_MAGIC_KNIGHT\"]";
            strUnitClass[7] = "UNIT_CLASS[\"UC_LIRE_COMBAT_RANGER\"]";
            strUnitClass[8] = "UNIT_CLASS[\"UC_LIRE_SNIPING_RANGER\"]";
            strUnitClass[9] = "UNIT_CLASS[\"UC_ARME_HIGH_MAGICIAN\"]";
            strUnitClass[10] = "UNIT_CLASS[\"UC_ARME_DARK_MAGICIAN\"]";
            strUnitClass[11] = "UNIT_CLASS[\"UC_RAVEN_SOUL_TAKER\"]";
            strUnitClass[12] = "UNIT_CLASS[\"UC_RAVEN_OVER_TAKER\"]";
            strUnitClass[13] = "UNIT_CLASS[\"UC_EVE_EXOTIC_GEAR\"]";
            strUnitClass[14] = "UNIT_CLASS[\"UC_EVE_ARCHITECTURE\"]";


            //sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");

            for (int i = 0; i < dataGridView1.RowCount; i++)
            {
#if __COUNTRY_KR__
                DataRow dr = Adpt_Release.GetDataBySubKey("KR", "SkillEnum", Convert.ToInt32(dataGridView1["ID", i].Value.ToString())).Rows[0];
#elif __COUNTRY_TW__
                DataRow dr = Adpt_Release.GetDataBySubKey("TW", "SkillEnum", Convert.ToInt32(dataGridView1["ID", i].Value.ToString())).Rows[0];
#elif __COUNTRY_HK__
                DataRow dr = Adpt_Release.GetDataBySubKey("HK", "SkillEnum", Convert.ToInt32(dataGridView1["ID", i].Value.ToString())).Rows[0];
#elif __COUNTRY_JP__
                DataRow dr = Adpt_Release.GetDataBySubKey("JP", "SkillEnum", Convert.ToInt32(dataGridView1["ID", i].Value.ToString())).Rows[0];
#endif
                if (dr["State"].ToString() == "False" )
                {
                    continue;
                }

                sw.Write("g_pSkillTree:AddSkillTemplet_LUA\r\n");
                sw.Write("{\r\n");
                if (dataGridView1["IN_HOUSE_TEST_ONLY", i].Value.ToString() == "True")
                {
                    sw.Write("    ");
                    sw.Write("IN_HOUSE_TEST_ONLY = TRUE,\r\n");
                }

                sw.Write("    ");
                sw.Write("m_eID = ");
                sw.Write(dataGridView1["ID", i].Value.ToString());
                sw.Write(",\r\n");
                sw.Write("    ");
                sw.Write("m_eType = ");
                sw.Write(dataGridView1["TYPE", i].Value.ToString());
                sw.Write(",\r\n");

                sw.Write("    ");
                sw.Write("m_Name = \"");
                sw.Write(dataGridView1["NAME", i].Value.ToString());
                sw.Write("\",\r\n");
                sw.Write("    ");
                sw.Write("m_Desc = \"");
                sw.Write(dataGridView1["DESC", i].Value.ToString());
                sw.Write("\",\r\n");

                sw.Write("    ");
                sw.Write("m_iSkillLevel = ");
                sw.Write(dataGridView1["SKILL_LEVEL", i].Value.ToString());
                sw.Write(",\r\n");

                sw.Write("    ");
                sw.Write("m_fSkillCoolTime = ");
                sw.Write(dataGridView1["COOL_TIME", i].Value.ToString());
                sw.Write(",\r\n");

                sw.Write("    ");
                sw.Write("m_fMPConsumption = ");
                sw.Write(dataGridView1["MP_CONSUMPTION", i].Value.ToString());
                sw.Write(",\r\n");


                sw.Write("    ");
                sw.Write("m_bBornSealed = ");
                sw.Write(dataGridView1["BORN_SEALED", i].Value.ToString());
                sw.Write(",\r\n");


                sw.Write("    ");
                sw.Write("m_bShowSkillLevel = ");
                sw.Write(dataGridView1["SHOW_LEVEL", i].Value.ToString());
                sw.Write(",\r\n");


                sw.Write("    ");
                sw.Write("m_wstrTextureName = \"");
                sw.Write(dataGridView1["ICON_TEXTURE_NAME", i].Value.ToString());
                sw.Write("\",\r\n");

                sw.Write("    ");
                sw.Write("m_wstrTexturePieceName = \"");
                sw.Write(dataGridView1["ICON_TEXTURE_PIECE_NAME", i].Value.ToString());
                sw.Write("\",\r\n");

                sw.Write("    ");
                sw.Write("m_fPowerRate = ");
                sw.Write(dataGridView1["POWER_RATE", i].Value.ToString());
                sw.Write(",\r\n");

                sw.Write("    ");
                sw.Write("m_eActiveSkillUseCondtion = ");
                sw.Write(dataGridView1["ACTIVE_SKILL_USE_CONDITION", i].Value.ToString());
                sw.Write(",\r\n");



                sw.Write("    ");
                sw.Write("m_Stat = \r\n");
                sw.Write("    {\r\n");
                sw.Write("        ");
                sw.Write("m_fBaseHP = ");
                sw.Write(dataGridView1["STAT_BASE_HP", i].Value.ToString());
                sw.Write(",\r\n");
                sw.Write("        ");
                sw.Write("m_fAtkPhysic = ");
                sw.Write(dataGridView1["STAT_ATK_PHYSIC", i].Value.ToString());
                sw.Write(",\r\n");
                sw.Write("        ");
                sw.Write("m_fAtkMagic = ");
                sw.Write(dataGridView1["STAT_ATK_MAGIC", i].Value.ToString());
                sw.Write(",\r\n");
                sw.Write("        ");
                sw.Write("m_fDefPhysic = ");
                sw.Write(dataGridView1["STAT_DEF_PHYSIC", i].Value.ToString());
                sw.Write(",\r\n");
                sw.Write("        ");
                sw.Write("m_fDefMagic = ");
                sw.Write(dataGridView1["STAT_DEF_MAGIC", i].Value.ToString());
                sw.Write(",\r\n");
                sw.Write("    },\r\n");

                string strAbilityKeyword = "MISC_";
                int nNumber = 0;
                while(dataGridView1[strAbilityKeyword + nNumber, i].Value.ToString() != "")
                {
                    if(dataGridView1[strAbilityKeyword + nNumber, i].Value.ToString() == "SA_HP_REL_TO_MP_ABS")
                    {
                        sw.Write("    ");
                        sw.Write(dataGridView1[strAbilityKeyword + nNumber, i].Value.ToString() + "_1 = ");
                        nNumber++;
                        sw.Write(dataGridView1[strAbilityKeyword + nNumber, i].Value.ToString());
                        sw.Write(",\r\n");

                        
                        sw.Write("    ");
                        sw.Write(dataGridView1[strAbilityKeyword + (nNumber - 1), i].Value.ToString() + "_2 = ");
                        nNumber++;
                        sw.Write(dataGridView1[strAbilityKeyword + nNumber, i].Value.ToString());
                        sw.Write(",\r\n");
                        break;
                    }
                    else
                    {
                        sw.Write("    ");
                        sw.Write(dataGridView1[strAbilityKeyword + nNumber, i].Value.ToString() + " = ");
                        nNumber++;
                        sw.Write(dataGridView1[strAbilityKeyword + nNumber, i].Value.ToString());
                        sw.Write(",\r\n");
                    }
                    nNumber++;
                    if (nNumber > 7)
                    {
                        break;
                    }
                }

                sw.Write("}\r\n\r\n\r\n\r\n");
            }


            DataGridView dgvSkill = null;
            DataTable tblCurrent = null;
            for (int j = 0; j < MAX_UNIT_CLASS; j++)
            {
                switch (j)
                {
                    case 0:
                        {
                            dgvSkill = dataGridView2;
                            tblCurrent = tblElsword;
                        }
                        break;
                    case 1:
                        {
                            dgvSkill = dataGridView5;
                            tblCurrent = tblAisha;
                        }
                        break;
                    case 2:
                        {
                            dgvSkill = dataGridView8;
                            tblCurrent = tblRena;
                        }
                        break;
                    case 3:
                        {
                            dgvSkill = dataGridView11;
                            tblCurrent = tblRaven;
                        }
                        break;
                    case 4:
                        {
                            dgvSkill = dataGridView14;
                            tblCurrent = tblEve;
                        }
                        break;
                    case 5:
                        {
                            dgvSkill = dataGridView3;
                            tblCurrent = tblElsword_SK;
                        }
                        break;
                    case 6:
                        {
                            dgvSkill = dataGridView4;
                            tblCurrent = tblElsword_MK;
                        }
                        break;
                    case 7:
                        {
                            dgvSkill = dataGridView9;
                            tblCurrent = tblRena_CR;
                        }
                        break;
                    case 8:
                        {
                            dgvSkill = dataGridView10;
                            tblCurrent = tblRena_SR;
                        }
                        break;
                    case 9:
                        {
                            dgvSkill = dataGridView6;
                            tblCurrent = tblAisha_HM;
                        }
                        break;
                    case 10:
                        {
                            dgvSkill = dataGridView7;
                            tblCurrent = tblAisha_DM;
                        }
                        break;
                    case 11:
                        {
                            dgvSkill = dataGridView13;
                            tblCurrent = tblRaven_ST;
                        }
                        break;
                    case 12:
                        {
                            dgvSkill = dataGridView12;
                            tblCurrent = tblRaven_OT;
                        }
                        break;
                    case 13:
                        {
                            dgvSkill = dataGridView16;
                            tblCurrent = tblEve_CE;
                        }
                        break;
                    case 14:
                        {
                            dgvSkill = dataGridView15;
                            tblCurrent = tblEve_CA;
                        }
                        break;

                }

                int nCount = 0;
                for (int nRow = 1; nRow <= tblCurrent.Rows.Count * 3; nRow += 3)
                {
                    for (int k = 0; k < 5; k++)
                    {
                        if (dgvSkill[1 + (k * 4), nRow].Value.ToString() != "")
                        {
#if __COUNTRY_KR__
                            DataRow dr = Adpt_Release.GetDataBySubKey("KR", "SkillEnum", Convert.ToInt32(dgvSkill[1 + (k * 4), nRow].Value.ToString())).Rows[0];
#elif __COUNTRY_TW__
                            DataRow dr = Adpt_Release.GetDataBySubKey("TW", "SkillEnum", Convert.ToInt32(dgvSkill[1 + (k * 4), nRow].Value.ToString())).Rows[0];
#elif __COUNTRY_HK__
                            DataRow dr = Adpt_Release.GetDataBySubKey("HK", "SkillEnum", Convert.ToInt32(dgvSkill[1 + (k * 4), nRow].Value.ToString())).Rows[0];
#elif __COUNTRY_JP__
                            DataRow dr = Adpt_Release.GetDataBySubKey("JP", "SkillEnum", Convert.ToInt32(dgvSkill[1 + (k * 4), nRow].Value.ToString())).Rows[0];
#endif
                            if (dr == null)
                            {
                                continue;
                            }
                            sw.Write("g_pSkillTree:AddSkillTreeTemplet_LUA\r\n");
                            sw.Write("{\r\n");

                            sw.Write("    ");
                            sw.Write("SKILL_ID = ");
                            sw.Write(dgvSkill[1 + (k * 4), nRow].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("UNIT_CLASS = ");
                            sw.Write(strUnitClass[j]);
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("MAX_SKILL_LEVEL = ");
                            sw.Write(dgvSkill[2 + (k * 4), nRow].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("TIER_INDEX = ");
                            sw.Write(nCount);
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("INDEX_IN_TIER = ");
                            sw.Write(k);
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_vecPrecedingSkill = \"");
                            sw.Write(dgvSkill[3 + (k * 4), nRow].Value.ToString());
                            sw.Write("\",\r\n");



                            sw.Write("}\r\n\r\n\r\n\r\n");
                        }

                    }
                    nCount++;
                }
            }
        
                    

            

            sw.Close();

            Process Dos = new Process();
            Dos.StartInfo.FileName = "tortoiseProc";
            Dos.StartInfo.Arguments = "/Command:commit /path:\"..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\NewSkillTemplet.lua\" /notempfile /closeonend:1";
            Dos.Start();
            
            MessageBox.Show("파일이 생성되었습니다.");

        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;
            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView1.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            objTemp = dgvCurrentCell.Value;
            //09. 06. 02 김정협 수정 : 인덱스 저장용 임시 저장소
            objTemp2 = dataGridView1["IndexKey", dgvCurrentCell.RowIndex].Value;
            //09. 06. 02 김정협 수정 : 배경색때문에 전의 셀로우를 임시저장
            m_nLastRow = dgvCurrentCell.RowIndex;

            //09. 07. 01 김정협 텍스트 수정창 열기
            if (dgvCurrentCell.Value.ToString().Length > 15 && !m_bPaste)
            {
                AssistBox.Show();
                AssistBox.Left = dataGridView1.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Left + tabControl1.Location.X + tabControl1.TabPages[0].Left;
                AssistBox.Top = dataGridView1.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Bottom + tabControl1.Location.Y + tabControl1.TabPages[0].Top;
                AssistBox.Text = dgvCurrentCell.Value.ToString();
            }
        }

        
        public void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;
            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView1.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            int nIndexRow = 0;
            if (dgvCurrentCell.ColumnIndex == 1)
            {
                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    if (dataGridView1["IndexKey", i].Value.Equals(objTemp2))
                    {
                        nIndexRow = i;
                        break;
                    }
                }
            }
            else
            {
                nIndexRow = dgvCurrentCell.RowIndex;
            }

            //09. 06. 06 김정협
            if (dgvCurrentCell.OwningColumn.Name == "ID")
            {
                DataRow dr = tblSkillEnum.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["ID_ENUM", nIndexRow].Value = dr["ID Name"].ToString();
                    dataGridView1["NAME", nIndexRow].Value = dr["Description"].ToString();
                    dataGridView1["ICON_TEXTURE_PIECE_NAME", nIndexRow].Value = dataGridView1["ID_ENUM", nIndexRow].Value;
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            if (dgvCurrentCell.OwningColumn.Name == "TYPE")
            {
                DataRow dr = tblSkillType.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["TYPE_ENUM", nIndexRow].Value = dr["Skill Type Name"].ToString();                    
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            if (dgvCurrentCell.OwningColumn.Name == "ACTIVE_SKILL_USE_CONDITION")
            {
                DataRow dr = tblActiveSkillUseCondition.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["ACTIVE_SKILL_USE_CONDITION_ENUM", nIndexRow].Value = dr["ACTIVE_SKILL_USE_CONDITION"].ToString();
                    dataGridView1["Description", nIndexRow].Value = dr["Description"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }
                        

            //09. 06. 02 김정협 수정 : 자동정렬 후 Index 재 검색 후 값비교
            if (!objTemp.Equals((object)dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Value))
            {
                //09. 06. 02 김정협 수정 : dgvCurrentCell.RowIndex 를 IndexRow로 교체
                string strOldTime = dataGridView1["Regdate", nIndexRow].Value.ToString();

                dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Red;
                dataGridView1["Regdate", nIndexRow].Value = DateTime.Now;
                dataGridView1["Regdate", nIndexRow].Style.ForeColor = Color.Red;



                //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여
                if (dgvCurrentCell.OwningColumn.Name == "ID")
                {
                    dataGridView1.Rows[dgvCurrentCell.RowIndex].HeaderCell.Value = dataGridView1["ID", dgvCurrentCell.RowIndex].Value.ToString() + "     " + dataGridView1["NAME", dgvCurrentCell.RowIndex].Value.ToString();
                }


                if (dgvCurrentCell.OwningColumn.Name == "DESC")
                {
                    //int nIndex_Name = Convert.ToInt32(dataGridView1["Index_Title", dgvCurrentCell.RowIndex].Value.ToString());
                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //Row["String"] = dgvCurrentCell.Value.ToString();



                    //09. 06. 23 김정협 국가별 디파일 정의 할 것
#if __COUNTRY_KR__
                    int nIndex_Name = -1;
                    string strCountry = "KR";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SkillTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SkillTable");
                    }

#elif __COUNTRY_TW__
                    int nIndex_Name = -1;
                    string strCountry = "TW";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "SkillTable") != null)
                    {
                         nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "SkillTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_HK__
                    int nIndex_Name = -1;
                    string strCountry = "HK";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "SkillTable") != null)
                    {
                         nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "SkillTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_JP__
                    int nIndex_Name = -1;
                    string strCountry = "JP";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "SkillTable") != null)
                    {
                         nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "SkillTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#endif

                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //09. 06. 19 김정협 Trans 수정부분 추가 
                    DataRow Row = null;// tblTrans.Rows[nIndex_Name];

                    if (nIndex_Name != -1)
                    {
                        for (int i = 0; i < tblTrans.Rows.Count; i++)
                        {
                            if (tblTrans.Rows[i]["TransIDX"].ToString() == nIndex_Name.ToString())
                            {
                                Row = tblTrans.Rows[i];
                                break;
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < tblTrans.Rows.Count; i++)
                        {
                            if (tblTrans.Rows[i]["Index"].ToString() == dataGridView1["IndexKey", nIndexRow].Value.ToString() &&
                                tblTrans.Rows[i]["TableName"].ToString() == "SkillTable" &&
                                tblTrans.Rows[i]["CountryCode"].ToString() == strCountry &&
                                tblTrans.Rows[i]["Column"].ToString() == dgvCurrentCell.OwningColumn.Name)
                            {
                                Row = tblTrans.Rows[i];
                                break;
                            }
                        }

                    }

                    Row["String"] = dgvCurrentCell.Value.ToString();
                    Row["Last Update Date"] = DateTime.Now;
                    //09. 06. 19 김정협 Trans 연결 히스토리 추가
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    tblTrans_History.Rows.Add();
                    DataRow Row_History = tblTrans_History.Rows[tblTrans_History.Rows.Count - 1];
                    Row_History["TransIDX"] = Row["TransIDX"];
                    Row_History["String"] = Row["String"];
                    Row_History["UserID"] = m_strID;
                    Row_History["UserIP"] = strMyIP;
                    Row_History["Last Update Date"] = Row["Last Update Date"];

                }


                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = nIndexRow;
                sModiftIDList.nTabIndex = 0;

                //09. 06. 01 김정협 수정 : Cell정보 추가
                sModiftIDList.bCell = true;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString());


                bool m_bSame = false;
                for (int i = 0; i < m_ModifyIDList.Count; i++)
                {
                    if (((sTableValue)m_ModifyIDList[i]).nTabIndex == sModiftIDList.nTabIndex &&
                        ((sTableValue)m_ModifyIDList[i]).nIndexKey == sModiftIDList.nIndexKey)
                    {
                        m_bSame = true;
                        // break;
                    }
                }

                sUndoRedo sUndoData = new sUndoRedo();
                sUndoData.nTabIndex = 0;
                sUndoData.nType = 1;//1. Cell타입 수정
                sUndoData.nColumnIndex = dgvCurrentCell.ColumnIndex;
                sUndoData.nRowIndex = nIndexRow;
                sUndoData.objUndoRedo = objTemp;

                //09. 07. 02 김정협 Undo그룹짓기
                if (!m_bPaste)
                {
                    if (m_UndoStack.Count > 0)
                    {
                        sUndoData.nGroupID = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                    }
                    else
                    {
                        sUndoData.nGroupID = 0;
                    }
                }
                else
                {
                    sUndoData.nGroupID = m_nPasteGroup;
                }


                //09. 06. 01 김정협 수정 : 열 중복저장 방지
                if (!m_bSame)
                {
                    m_ModifyIDList.Add(sModiftIDList);
                    sUndoData.sModyfyList = sModiftIDList;
                }
                else
                {
                    sUndoData.sModyfyList.nTabIndex = -1;
                }

                m_UndoStack.Push(sUndoData);
                m_RedoStack.Clear();


                m_bSaveEnd = false;

                //09. 06. 02 김정협 수정 : 셀 변환정보 배열에 추가

                sChangeCell sChangeCellList = new sChangeCell();
                sChangeCellList.nTabIndex = 0;
                sChangeCellList.nColumnIndex = dgvCurrentCell.ColumnIndex;
                sChangeCellList.nIndexKey = Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString());
                if (!m_ChangeCellList.Contains(sChangeCellList))
                {
                    m_ChangeCellList.Add(sChangeCellList);
                }

                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int c = 0; c < dataGridView1.ColumnCount; c++)
                    {
                        if (dataGridView1[c, m_nLastRow].HasStyle)
                            dataGridView1[c, m_nLastRow].Style.BackColor = Color.White;
                    }
                    SetCellBackgroundColorForDB(0);
                    ColorRefresh(true);
                }
            }
        }

        private void dataGridView17_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;
            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView17.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            objTemp = dgvCurrentCell.Value;
            //09. 06. 02 김정협 수정 : 인덱스 저장용 임시 저장소
            objTemp2 = dataGridView1["IndexKey", dgvCurrentCell.RowIndex].Value;
            //09. 06. 02 김정협 수정 : 배경색때문에 전의 셀로우를 임시저장
            m_nLastRow = dgvCurrentCell.RowIndex;

            //09. 07. 01 김정협 텍스트 수정창 열기
            if (dgvCurrentCell.Value.ToString().Length > 15 && !m_bPaste)
            {
                AssistBox.Show();
                AssistBox.Left = dataGridView17.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Left + tabControl1.Location.X + tabControl1.TabPages[0].Left;
                AssistBox.Top = dataGridView17.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Bottom + tabControl1.Location.Y + tabControl1.TabPages[0].Top;
                AssistBox.Text = dgvCurrentCell.Value.ToString();
            }
        }

        private void dataGridView17_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;
            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView17.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            int nIndexRow = 0;
            if (dgvCurrentCell.ColumnIndex == 1)
            {
                for (int i = 0; i < dataGridView17.RowCount; i++)
                {
                    if (dataGridView17["IndexKey", i].Value.Equals(objTemp2))
                    {
                        nIndexRow = i;
                        break;
                    }
                }
            }
            else
            {
                nIndexRow = dgvCurrentCell.RowIndex;
            }


            //09. 06. 02 김정협 수정 : 자동정렬 후 Index 재 검색 후 값비교
            if (!objTemp.Equals((object)dataGridView17[dgvCurrentCell.ColumnIndex, nIndexRow].Value))
            {
                //09. 06. 02 김정협 수정 : dgvCurrentCell.RowIndex 를 IndexRow로 교체
                string strOldTime = dataGridView17["Regdate", nIndexRow].Value.ToString();

                dataGridView17[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Red;
                dataGridView17["Regdate", nIndexRow].Value = DateTime.Now;
                dataGridView17["Regdate", nIndexRow].Style.ForeColor = Color.Red;

                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView17.RowCount; i++)
                    {
                        if (dataGridView17[1, i].Value.ToString() == dataGridView17[dgvCurrentCell.ColumnIndex, nIndexRow].Value.ToString())
                        {
                            if (i == nIndexRow)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID값이 존재합니다.");

                            dataGridView17[dgvCurrentCell.ColumnIndex, nIndexRow].Value = objTemp;
                            dataGridView17[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Black;
                            dataGridView17["Regdate", nIndexRow].Value = strOldTime;
                            dataGridView17["Regdate", nIndexRow].Style.ForeColor = Color.Black;
                            return;

                        }
                    }

                }


                if (dgvCurrentCell.OwningColumn.Name == "Description")
                {
                    //int nIndex_Name = Convert.ToInt32(dataGridView1["Index_Title", dgvCurrentCell.RowIndex].Value.ToString());
                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //Row["String"] = dgvCurrentCell.Value.ToString();



                    //09. 06. 23 김정협 국가별 디파일 정의 할 것
#if __COUNTRY_KR__
                    int nIndex_Name = -1;
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SkillEnum") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SkillEnum");
                    }
                    string strCountry = "KR";
#elif __COUNTRY_TW__
                    int nIndex_Name = -1;//Convert.ToInt32(dataGridView17["Index_Name", nIndexRow].Value.ToString());
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()),"TW", objTemp.ToString(), "SkillEnum") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "SkillEnum"); 
                    }
                    string strCountry = "TW";
#elif __COUNTRY_HK__
                    int nIndex_Name = -1;//Convert.ToInt32(dataGridView17["Index_Name", nIndexRow].Value.ToString());
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()),"HK", objTemp.ToString(), "SkillEnum") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "SkillEnum"); 
                    }
                    string strCountry = "HK";
#elif __COUNTRY_JP__
                    int nIndex_Name = -1;//Convert.ToInt32(dataGridView17["Index_Name", nIndexRow].Value.ToString());
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()),"JP", objTemp.ToString(), "SkillEnum") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "SkillEnum"); 
                    }
                    string strCountry = "JP";
#endif

                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //09. 06. 19 김정협 Trans 수정부분 추가 
                    DataRow Row = null;// tblTrans.Rows[nIndex_Name];

                    if (nIndex_Name != -1)
                    {
                        for (int i = 0; i < tblTrans.Rows.Count; i++)
                        {
                            if (tblTrans.Rows[i]["TransIDX"].ToString() == nIndex_Name.ToString())
                            {
                                Row = tblTrans.Rows[i];
                                break;
                            }
                        }
                    }
                    else
                    {
                        for (int i = 0; i < tblTrans.Rows.Count; i++)
                        {
                            if (tblTrans.Rows[i]["Index"].ToString() == dataGridView17["IndexKey", nIndexRow].Value.ToString() &&
                                tblTrans.Rows[i]["TableName"].ToString() == "SkillEnum" &&
                                tblTrans.Rows[i]["CountryCode"].ToString() == strCountry &&
                                tblTrans.Rows[i]["Column"].ToString() == dgvCurrentCell.OwningColumn.Name)
                            {
                                Row = tblTrans.Rows[i];
                                break;
                            }
                        }

                    }

                    Row["String"] = dgvCurrentCell.Value.ToString();
                    Row["Last Update Date"] = DateTime.Now;
                    //09. 06. 19 김정협 Trans 연결 히스토리 추가
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    tblTrans_History.Rows.Add();
                    DataRow Row_History = tblTrans_History.Rows[tblTrans_History.Rows.Count - 1];
                    Row_History["TransIDX"] = Row["TransIDX"];
                    Row_History["String"] = Row["String"];
                    Row_History["UserID"] = m_strID;
                    Row_History["UserIP"] = strMyIP;
                    Row_History["Last Update Date"] = Row["Last Update Date"];

                }


                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = nIndexRow;
                sModiftIDList.nTabIndex = 16;

                //09. 06. 01 김정협 수정 : Cell정보 추가
                sModiftIDList.bCell = true;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString());


                bool m_bSame = false;
                for (int i = 0; i < m_ModifyIDList.Count; i++)
                {
                    if (((sTableValue)m_ModifyIDList[i]).nTabIndex == sModiftIDList.nTabIndex &&
                        ((sTableValue)m_ModifyIDList[i]).nIndexKey == sModiftIDList.nIndexKey)
                    {
                        m_bSame = true;
                        // break;
                    }
                }

                sUndoRedo sUndoData = new sUndoRedo();
                sUndoData.nTabIndex = 16;
                sUndoData.nType = 1;//1. Cell타입 수정
                sUndoData.nColumnIndex = dgvCurrentCell.ColumnIndex;
                sUndoData.nRowIndex = nIndexRow;
                sUndoData.objUndoRedo = objTemp;

                //09. 07. 02 김정협 Undo그룹짓기
                if (!m_bPaste)
                {
                    if (m_UndoStack.Count > 0)
                    {
                        sUndoData.nGroupID = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                    }
                    else
                    {
                        sUndoData.nGroupID = 0;
                    }
                }
                else
                {
                    sUndoData.nGroupID = m_nPasteGroup;
                }


                //09. 06. 01 김정협 수정 : 열 중복저장 방지
                if (!m_bSame)
                {
                    m_ModifyIDList.Add(sModiftIDList);
                    sUndoData.sModyfyList = sModiftIDList;
                }
                else
                {
                    sUndoData.sModyfyList.nTabIndex = -1;
                }

                m_UndoStack.Push(sUndoData);
                m_RedoStack.Clear();


                m_bSaveEnd = false;

                //09. 06. 02 김정협 수정 : 셀 변환정보 배열에 추가

                sChangeCell sChangeCellList = new sChangeCell();
                sChangeCellList.nTabIndex = 16;
                sChangeCellList.nColumnIndex = dgvCurrentCell.ColumnIndex;
                sChangeCellList.nIndexKey = Convert.ToInt32(dataGridView17["IndexKey", nIndexRow].Value.ToString());
                if (!m_ChangeCellList.Contains(sChangeCellList))
                {
                    m_ChangeCellList.Add(sChangeCellList);
                }

                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int c = 0; c < dataGridView17.ColumnCount; c++)
                    {
                        if (dataGridView17[c, m_nLastRow].HasStyle)
                            dataGridView17[c, m_nLastRow].Style.BackColor = Color.White;
                    }
                    SetCellBackgroundColorForDB(16);
                    ColorRefresh(true);
                }
            }
        }

        private void SkillTable_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.OKCancel);
                if (result == DialogResult.OK)
                {
                    Save();
                    MessageBox.Show("저장되었습니다.");
                }
                m_ModifyIDList.Clear();
                m_bSaveEnd = true;
            }
        }

        private void dataGridView1_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                Search frm = new Search();
                frm.SetDataGridView(dataGridView1);
                frm.ShowDialog(this);
            }

            if (e.KeyCode == Keys.Escape)
            {
                if (AssistBox.Visible)
                {
                    AssistBox.Hide();
                }
            }


            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView1.GetClipboardContent().GetText());
            }
            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {
                //09. 07. 02 김정협 언두리두시 그룹지정
                if (m_UndoStack.Count > 0)
                {
                    m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                }
                else
                {
                    m_nPasteGroup = 0;
                }

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView1.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView1.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView1.ColumnCount <= nColumnIndex ||
                            dataGridView1.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView1[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            m_bPaste = true;
                            m_dgvcTemp = dataGridView1[nColumnIndex, nRowIndex];
                            dataGridView1_CellBeginEdit(null, null);
                            dataGridView1[nColumnIndex, nRowIndex].Value = strData[j];

                            dataGridView1_CellEndEdit(null, null);
                            //dataGridView1.RefreshEdit();
                            m_bPaste = false;
                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView1.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }


            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView1.CurrentCell.ReadOnly)
                {
                    //09. 07. 02 김정협 언두리두시 그룹지정
                    if (m_UndoStack.Count > 0)
                    {
                        m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                    }
                    else
                    {
                        m_nPasteGroup = 0;
                    }

                    for (int i = 0; i < dataGridView1.SelectedCells.Count; i++)
                    {
                        m_bPaste = true;
                        m_dgvcTemp = dataGridView1.SelectedCells[i];
                        dataGridView1_CellBeginEdit(null, null);

                        if (dataGridView1.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                        {
                            dataGridView1.SelectedCells[i].Value = false;
                        }
                        else if (dataGridView1.SelectedCells[i].ValueType == Type.GetType("System.String"))
                        {
                            dataGridView1.SelectedCells[i].Value = "";
                        }
                        else
                        {
                            dataGridView1.SelectedCells[i].Value = 0;
                        }

                        dataGridView1_CellEndEdit(null, null);
                        dataGridView1.RefreshEdit();
                        m_bPaste = false;
                    }
                }
            }
        }

        private void dataGridView17_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                Search frm = new Search();
                frm.SetDataGridView(dataGridView17);
                frm.ShowDialog(this);
            }

            if (e.KeyCode == Keys.Escape)
            {
                if (AssistBox.Visible)
                {
                    AssistBox.Hide();
                }
            }


            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView17.GetClipboardContent().GetText());
            }
            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {
                //09. 07. 02 김정협 언두리두시 그룹지정
                if (m_UndoStack.Count > 0)
                {
                    m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                }
                else
                {
                    m_nPasteGroup = 0;
                }

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView17.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView17.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView17.ColumnCount <= nColumnIndex ||
                            dataGridView17.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView17[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            m_bPaste = true;
                            m_dgvcTemp = dataGridView17[nColumnIndex, nRowIndex];
                            dataGridView17_CellBeginEdit(null, null);
                            dataGridView17[nColumnIndex, nRowIndex].Value = strData[j];

                            dataGridView17_CellEndEdit(null, null);
                            //dataGridView17.RefreshEdit();
                            m_bPaste = false;
                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView17.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }


            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView17.CurrentCell.ReadOnly)
                {
                    //09. 07. 02 김정협 언두리두시 그룹지정
                    if (m_UndoStack.Count > 0)
                    {
                        m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                    }
                    else
                    {
                        m_nPasteGroup = 0;
                    }

                    for (int i = 0; i < dataGridView17.SelectedCells.Count; i++)
                    {
                        m_bPaste = true;
                        m_dgvcTemp = dataGridView17.SelectedCells[i];
                        dataGridView17_CellBeginEdit(null, null);

                        if (dataGridView17.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                        {
                            dataGridView17.SelectedCells[i].Value = false;
                        }
                        else if (dataGridView17.SelectedCells[i].ValueType == Type.GetType("System.String"))
                        {
                            dataGridView17.SelectedCells[i].Value = "";
                        }
                        else
                        {
                            dataGridView17.SelectedCells[i].Value = 0;
                        }

                        dataGridView17_CellEndEdit(null, null);
                        dataGridView17.RefreshEdit();
                        m_bPaste = false;
                    }
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                switch (tabControl1.SelectedIndex)
                {
                    case 0:
                        {
                            if (dataGridView1.CurrentCell.ColumnIndex != dataGridView1[textBox1.Text, 0].ColumnIndex)
                            {
                                m_nStartPoint = 0;
                            }

                            for (int i = m_nStartPoint; i < dataGridView1.RowCount; i++)
                            {
                                if (!dataGridView1[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView1[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView1.CurrentCell = dataGridView1[textBox1.Text, i];
                                    m_nStartPoint = i + 1;
                                    return;
                                }
                            }

                            for (int i = 0; i < m_nStartPoint; i++)
                            {
                                if (!dataGridView1[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView1[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView1.CurrentCell = dataGridView1[textBox1.Text, i];
                                    m_nStartPoint = i + 1;
                                    return;
                                }
                            }

                            MessageBox.Show("결과값이 존재하지 않습니다.");
                        }
                        break;
                    case 16:
                        {
                            if (dataGridView17.CurrentCell.ColumnIndex != dataGridView17[textBox1.Text, 0].ColumnIndex)
                            {
                                m_nStartPoint = 0;
                            }

                            for (int i = m_nStartPoint; i < dataGridView17.RowCount; i++)
                            {
                                if (!dataGridView17[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView17[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView17.CurrentCell = dataGridView17[textBox1.Text, i];
                                    m_nStartPoint = i + 1;
                                    return;
                                }
                            }

                            for (int i = 0; i < m_nStartPoint; i++)
                            {
                                if (!dataGridView17[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView17[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView17.CurrentCell = dataGridView17[textBox1.Text, i];
                                    m_nStartPoint = i + 1;
                                    return;
                                }
                            }

                            MessageBox.Show("결과값이 존재하지 않습니다.");
                        }
                        break;
                }
            }
        }

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Enter == e.KeyChar)
                button1_Click(null, null);
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                for (int i = 0; i < dataGridView1.ColumnCount; i++)
                {
                    dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Automatic;
                }
                for (int i = 0; i < dataGridView17.ColumnCount; i++)
                {
                    dataGridView17.Columns[i].SortMode = DataGridViewColumnSortMode.Automatic;
                }
            }
            else
            {
                for (int i = 0; i < dataGridView1.ColumnCount; i++)
                {
                    dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                }
                for (int i = 0; i < dataGridView17.ColumnCount; i++)
                {
                    dataGridView17.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                }
            }
        }

        private void dataGridView1_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView1[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void dataGridView17_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView17[e.ColumnIndex, 0].OwningColumn.Name;
        }


        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (tabControl1.SelectedIndex == 0)
            {
                btnFileGenerate.Enabled = true;
                btnHistory.Enabled = true;
                btnString.Enabled = true;
            }
            else
            {
                btnFileGenerate.Enabled = false;
                btnHistory.Enabled = false;
                btnString.Enabled = false;
            }

            m_nStartPoint = 0;
            textBox1.Text = "";

            //09. 06. 10 김정협 리플레이스 기능 추가
            panel1.Visible = false;
            textBox3.Text = "";
            textBox4.Text = "";

            //09. 07. 01 김정협 텍스트 수정창 열기
            if (AssistBox.Visible)
            {
                AssistBox.Hide();
            }
        }

        private void btnColor_Click(object sender, EventArgs e)
        {
            Color cColor = new Color();
            ColorDialog dlg = new ColorDialog();

            if (dlg.ShowDialog() == DialogResult.OK)
            {

                cColor = dlg.Color;
            }
            ArrayList RowDelete = new ArrayList();
            int nIndex = 0;

            if (tblColor.Rows.Count == 0)
            {
                nIndex = 0;
            }

            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        if (cColor == Color.White)
                        {
                            for (int i = 0; i < dataGridView1.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView1.SelectedCells[i].ReadOnly &&
                                    dataGridView1.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView1.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }

                                if (dataGridView1.SelectedCells[i].Style.BackColor == Color.White)
                                {
                                    continue;
                                }
                                dataGridView1.SelectedCells[i].Style.BackColor = cColor;
                                for (int j = 0; j < tblColor.Rows.Count; j++)
                                {
                                    if (RowDelete.Contains(j) || tblColor.Rows[j].RowState == DataRowState.Deleted)
                                    {
                                        continue;
                                    }
                                    DataRow drTemp = tblColor.Rows[j];
                                    if (drTemp["TableName"].ToString() == "SkillTable" &&
                                    drTemp["Column"].ToString() == Convert.ToString(dataGridView1.SelectedCells[i].ColumnIndex) &&
                                    drTemp["IndexNum"].ToString() == dataGridView1["IndexKey", dataGridView1.SelectedCells[i].RowIndex].Value.ToString())
                                    {
                                        //tblColor.Rows.RemoveAt(j);
                                        tblColor.Rows[j].Delete();
                                        RowDelete.Add(j);
                                        //break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < dataGridView1.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView1.SelectedCells[i].ReadOnly &&
                                    dataGridView1.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView1.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }

                                dataGridView1.SelectedCells[i].Style.BackColor = cColor;
                                if (tblColor.Rows.Count > 0)
                                {
                                    //nIndex = 0;
                                    DataRow dr = tblColor.Rows[tblColor.Rows.Count - 1];
                                    nIndex = Convert.ToInt32(dr["IndexKey"].ToString()) + 1;
                                }

                                tblColor.Rows.Add();
                                DataRow drTemp = tblColor.Rows[tblColor.Rows.Count - 1];
                                drTemp["IndexKey"] = nIndex;
                                drTemp["TableName"] = "QuestTable";
                                drTemp["Column"] = dataGridView1.SelectedCells[i].ColumnIndex;
                                drTemp["IndexNum"] = dataGridView1["IndexKey", dataGridView1.SelectedCells[i].RowIndex].Value;

                                drTemp["Color"] = cColor.ToArgb();

                                drTemp["Regdate"] = DateTime.Now;
                            }
                        }
                        RowDelete.Clear();
                        // Adpt_ColorTable.Update(tblColor);
                    }
                    break;

                case 16:
                    {
                        if (cColor == Color.White)
                        {
                            for (int i = 0; i < dataGridView17.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView17.SelectedCells[i].ReadOnly &&
                                    dataGridView17.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView17.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }


                                if (dataGridView17.SelectedCells[i].Style.BackColor == Color.White)
                                {
                                    continue;
                                }
                                dataGridView17.SelectedCells[i].Style.BackColor = cColor;
                                for (int j = 0; j < tblColor.Rows.Count; j++)
                                {
                                    if (RowDelete.Contains(j) || tblColor.Rows[j].RowState == DataRowState.Deleted)
                                    {
                                        continue;
                                    }
                                    DataRow drTemp = tblColor.Rows[j];
                                    if (drTemp["TableName"].ToString() == "SubQuestTable" &&
                                    drTemp["Column"].ToString() == Convert.ToString(dataGridView17.SelectedCells[i].ColumnIndex) &&
                                    drTemp["IndexNum"].ToString() == dataGridView2["IndexKey", dataGridView17.SelectedCells[i].RowIndex].Value.ToString())
                                    {
                                        //tblColor.Rows.RemoveAt(j);
                                        tblColor.Rows[j].Delete();
                                        RowDelete.Add(j);
                                        //break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for (int i = 0; i < dataGridView17.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView17.SelectedCells[i].ReadOnly &&
                                    dataGridView17.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView17.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }

                                dataGridView17.SelectedCells[i].Style.BackColor = cColor;
                                if (tblColor.Rows.Count > 0)
                                {
                                    //nIndex = 0;
                                    DataRow dr = tblColor.Rows[tblColor.Rows.Count - 1];
                                    nIndex = Convert.ToInt32(dr["IndexKey"].ToString()) + 1;
                                }
                                tblColor.Rows.Add();
                                DataRow drTemp = tblColor.Rows[tblColor.Rows.Count - 1];
                                drTemp["IndexKey"] = nIndex;
                                drTemp["TableName"] = "SubQuestTable";
                                drTemp["Column"] = dataGridView17.SelectedCells[i].ColumnIndex;
                                drTemp["IndexNum"] = dataGridView17["IndexKey", dataGridView17.SelectedCells[i].RowIndex].Value;

                                drTemp["Color"] = cColor.ToArgb();

                                drTemp["Regdate"] = DateTime.Now;
                            }
                        }
                        RowDelete.Clear();
                        // Adpt_ColorTable.Update(tblColor);
                    }
                    break;
            }
        }

        private void contextMenuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        if (e.ClickedItem.Text == "전체 선택")
                        {
                            for (int i = 0; i < dataGridView1.SelectedCells.Count; i++)
                            {
                                if (dataGridView1.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                                {
                                    m_bPaste = true;
                                    m_dgvcTemp = dataGridView1.SelectedCells[i];
                                    dataGridView1_CellBeginEdit(null, null);
                                    dataGridView1.SelectedCells[i].Value = true;

                                    dataGridView1_CellEndEdit(null, null);
                                    dataGridView1.RefreshEdit();
                                    //전체선택 후 셀에서 빠져나갈대 dataGridView1_CellEndEdit이벤트 발생시키지않기위하여
                                    objTemp = dataGridView1.CurrentCell.Value;
                                    m_bPaste = false;
                                }
                            }

                        }
                        if (e.ClickedItem.Text == "전체 해제")
                        {
                            for (int i = 0; i < dataGridView1.SelectedCells.Count; i++)
                            {
                                if (dataGridView1.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                                {
                                    m_bPaste = true;
                                    m_dgvcTemp = dataGridView1.SelectedCells[i];
                                    dataGridView1_CellBeginEdit(null, null);
                                    dataGridView1.SelectedCells[i].Value = false;

                                    dataGridView1_CellEndEdit(null, null);
                                    dataGridView1.RefreshEdit();
                                    //전체선택 후 셀에서 빠져나갈대 dataGridView1_CellEndEdit이벤트 발생시키지않기위하여
                                    objTemp = dataGridView1.CurrentCell.Value;
                                    m_bPaste = false;
                                }
                            }
                        }

                        //09. 07. 02 김정협 링크리스트 제작
                        if (e.ClickedItem.Text == "List열기")
                        {
                            dataGridView1_CellBeginEdit(null, null);
                            LinkListView frm = new LinkListView();

                            if (dataGridView1.CurrentCell.OwningColumn.Name == "TYPE")
                            {
                                frm.SetTable(tblSkillType);
                            }
                            else if (dataGridView1.CurrentCell.OwningColumn.Name == "ACTIVE_SKILL_USE_CONDITION")
                            {
                                frm.SetTable(tblActiveSkillUseCondition);
                            }
                            
                            frm.SetPoint(MousePosition.X, MousePosition.Y);
                            frm.SetDataGridView(dataGridView1, tabControl1.SelectedIndex, "SkillTable");
                            frm.Owner = this;
                            frm.ShowDialog(this);
                        }

                        
                        dataGridView1.RefreshEdit();
                    }
                    break;
            }
        }

        private void contextMenuStrip1_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            contextMenuStrip1.Items.Clear();
        }

        private void btnColumn_Click(object sender, EventArgs e)
        {
            ColumnManager frm = new ColumnManager();
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    frm.SetDataGridView(dataGridView1);
                    break;
                case 16:
                    frm.SetDataGridView(dataGridView17);
                    break;
            }
            frm.SetCurrentTab(GetCurrentTab());
            frm.Owner = this;
            frm.ShowDialog(this);
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (e.ColumnIndex == -1)
                {
                    return;
                }
                if (e.RowIndex != -1)
                {
                    bool bSelected = false;
                    for (int i = 0; i < dataGridView1.SelectedCells.Count; i++)
                    {
                        if (e.ColumnIndex == dataGridView1.SelectedCells[i].ColumnIndex && e.RowIndex == dataGridView1.SelectedCells[i].RowIndex)
                        {
                            bSelected = true;
                            break;
                        }
                    }
                    if (!bSelected)
                    {
                        dataGridView1.CurrentCell = dataGridView1[e.ColumnIndex, e.RowIndex];
                    }

                    if (dataGridView1.CurrentCell.ValueType == Type.GetType("System.Boolean") && bSelected && dataGridView1.SelectedCells.Count > 1)
                    {
                        contextMenuStrip1.Items.Add("전체 선택");
                        contextMenuStrip1.Items.Add("전체 해제");
                        contextMenuStrip1.Show();
                    }

                    //09. 07. 02 김정협 링크리스트 열기
                    if (dataGridView1.CurrentCell.OwningColumn.Name == "TYPE" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "ACTIVE_SKILL_USE_CONDITION")
                        
                    {
                        contextMenuStrip1.Items.Add("List열기");
                        contextMenuStrip1.Show();
                    }
                }
                else
                {
                    contextMenuStrip2.Items.Add("필터링 설정");
                    contextMenuStrip2.Items.Add("필터링 해제");
                    contextMenuStrip2.Show(MousePosition.X, MousePosition.Y);
                    m_nFilteringColumnIndex = e.ColumnIndex;
                }
            }
        }

        private void dataGridView17_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (e.ColumnIndex == -1)
                {
                    return;
                }
                if (e.RowIndex != -1)
                {
                    bool bSelected = false;
                    for (int i = 0; i < dataGridView17.SelectedCells.Count; i++)
                    {
                        if (e.ColumnIndex == dataGridView17.SelectedCells[i].ColumnIndex && e.RowIndex == dataGridView17.SelectedCells[i].RowIndex)
                        {
                            bSelected = true;
                            break;
                        }
                    }
                    if (!bSelected)
                    {
                        dataGridView17.CurrentCell = dataGridView17[e.ColumnIndex, e.RowIndex];
                    }

                    if (dataGridView17.CurrentCell.ValueType == Type.GetType("System.Boolean") && bSelected && dataGridView17.SelectedCells.Count > 1)
                    {
                        contextMenuStrip1.Items.Add("전체 선택");
                        contextMenuStrip1.Items.Add("전체 해제");
                        contextMenuStrip1.Show();
                    }
                }
                else
                {
                    contextMenuStrip2.Items.Add("필터링 설정");
                    contextMenuStrip2.Items.Add("필터링 해제");
                    contextMenuStrip2.Show(MousePosition.X, MousePosition.Y);
                    m_nFilteringColumnIndex = e.ColumnIndex;
                }
            }
        }

        private void contextMenuStrip2_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            if (e.ClickedItem.Text == "필터링 설정")
            {

                Filtering frm = new Filtering();
                switch (tabControl1.SelectedIndex)
                {
                    case 0:
                        {
                            frm.SetDataGridView(dataGridView1);
                        }
                        break;
                    case 16:
                        {
                            frm.SetDataGridView(dataGridView17);
                        }
                        break;
                }

                //09. 06. 10 필터링 결과내 재검색 기능
                frm.SetIsFiltering(m_IsFiltering);

                frm.SetFilterColumnIndex(m_nFilteringColumnIndex);
                frm.Owner = this;
                frm.ShowDialog(this);

                //09. 06. 10 필터링 결과내 재검색 기능
                m_IsFiltering = true;
            }

            if (e.ClickedItem.Text == "필터링 해제")
            {
                switch (tabControl1.SelectedIndex)
                {
                    case 0:
                        {
                            for (int i = 0; i < dataGridView1.RowCount; i++)
                            {
                                if (dataGridView1.Rows[i].Visible == false)
                                    dataGridView1.Rows[i].Visible = true;
                            }
                        }
                        break;
                    case 16:
                        {
                            for (int i = 0; i < dataGridView17.RowCount; i++)
                            {
                                if (dataGridView17.Rows[i].Visible == false)
                                    dataGridView17.Rows[i].Visible = true;
                            }
                        }
                        break;
                }
            }
        }

       
        private void contextMenuStrip2_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            contextMenuStrip2.Items.Clear();
        }


        private void btnReplace_Click(object sender, EventArgs e)
        {
            panel1.Visible = true;
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        textBox3.Text = dataGridView1.CurrentCell.Value.ToString();
                    }
                    break;
                case 16:
                    {
                        textBox3.Text = dataGridView17.CurrentCell.Value.ToString();
                    }
                    break;
            }
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        //09. 07. 02 김정협 언두리두시 그룹지정
                        if (m_UndoStack.Count > 0)
                        {
                            m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                        }
                        else
                        {
                            m_nPasteGroup = 0;
                        }

                        for (int i = 0; i < dataGridView1.SelectedCells.Count; i++)
                        {
                            if (dataGridView1.SelectedCells[i].ReadOnly)
                            {
                                continue;
                            }
                            int nIndex = dataGridView1.SelectedCells[i].Value.ToString().IndexOf(textBox3.Text);
                            if (nIndex != -1)
                            {
                                m_bPaste = true;
                                m_dgvcTemp = dataGridView1.SelectedCells[i];
                                dataGridView1_CellBeginEdit(null, null);

                                string strTemp = dataGridView1.SelectedCells[i].Value.ToString();
                                dataGridView1.SelectedCells[i].Value = strTemp.Replace(textBox3.Text, textBox4.Text);

                                dataGridView1_CellEndEdit(null, null);
                                dataGridView1.RefreshEdit();
                                m_bPaste = false;
                            }
                        }
                    }
                    break;

                case 16:
                    {
                        //09. 07. 02 김정협 언두리두시 그룹지정
                        if (m_UndoStack.Count > 0)
                        {
                            m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                        }
                        else
                        {
                            m_nPasteGroup = 0;
                        }

                        for (int i = 0; i < dataGridView17.SelectedCells.Count; i++)
                        {
                            if (dataGridView17.SelectedCells[i].ReadOnly)
                            {
                                continue;
                            }
                            int nIndex = dataGridView17.SelectedCells[i].Value.ToString().IndexOf(textBox3.Text);
                            if (nIndex != -1)
                            {
                                m_bPaste = true;
                                m_dgvcTemp = dataGridView17.SelectedCells[i];
                                dataGridView17_CellBeginEdit(null, null);

                                string strTemp = dataGridView17.SelectedCells[i].Value.ToString();
                                dataGridView17.SelectedCells[i].Value = strTemp.Replace(textBox3.Text, textBox4.Text);

                                dataGridView17_CellEndEdit(null, null);
                                dataGridView17.RefreshEdit();
                                m_bPaste = false;
                            }
                        }
                    }
                    break;
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            panel1.Visible = false;
            textBox3.Text = "";
            textBox4.Text = "";
        }

        void UserAddUndoRow(int nDeletingRowIndex, DataGridView dgvView, DataTable tblDelete, sTableValue Modify, int nGroup)
        {
            DataRow dr = null;// = tblDelete.Rows[e.Row.Index];
            int nRowIndex = 0;

            for (int i = 0; i < tblDelete.Rows.Count; i++)
            {
                if (tblDelete.Rows[i].RowState == DataRowState.Deleted)
                {
                    continue;
                }

                if (tblDelete.Rows[i]["IndexKey"].ToString() == Convert.ToString(nDeletingRowIndex))
                {
                    dr = tblDelete.Rows[i];
                    nRowIndex = i;
                    break;
                }
            }

            //Redo를 위하여 값을 보존
            DataTable tblTemp = tblDelete.Clone();
            tblTemp = tblDelete.Copy();
            DataRow drOriginal = tblTemp.Rows[nRowIndex];

            sUndoRedo sRedo = new sUndoRedo();
            sRedo.nType = 2;
            sRedo.nTabIndex = 0;
            sRedo.drAdd = drOriginal;
            sRedo.sModyfyList = Modify;

            //09. 07. 02 김정협 Undo그룹짓기
            sRedo.nGroupID = nGroup;
            m_RedoStack.Push(sRedo);

            tblDelete.Rows[nRowIndex].Delete();
            dgvView.DataSource = tblDelete;



        }

        void UserDeletingRow(int nDeletingRowIndex, DataGridView dgvView, DataTable tblDelete, bool bTable, int nTabPage)
        {
            DataRow dr = null;// = tblDelete.Rows[e.Row.Index];
            int nRowIndex = 0;
            if (!bTable)
            {
                for (int i = 0; i < tblDelete.Rows.Count; i++)
                {
                    if (tblDelete.Rows[i].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }

                    if (dgvView["IndexKey", nDeletingRowIndex].Value.ToString() == tblDelete.Rows[i]["IndexKey"].ToString())
                    {
                        dr = tblDelete.Rows[i];
                        nRowIndex = i;
                        break;
                    }
                }
            }
            else
            {
                dr = tblDelete.Rows[nDeletingRowIndex];
                nRowIndex = nDeletingRowIndex;
            }

            //DeleteHistory에 넣어주기 위하여 값을 보존
            DataTable tblTemp = tblDelete.Clone();
            tblTemp = tblDelete.Copy();
            DataRow drOriginal = tblTemp.Rows[nRowIndex];

            tblDelete.Rows[nRowIndex].Delete();
            dgvView.DataSource = tblDelete;

            sDeleteRow sRow = new sDeleteRow();
            sRow.drRow = dr;
            sRow.drOriginalRow = drOriginal;
            sRow.nTabIndex = nTabPage;

            sUndoRedo sUndoData = new sUndoRedo();
            sUndoData.nTabIndex = nTabPage;
            sUndoData.nType = 3;//3. Delete타입 수정                
            sUndoData.nRowIndex = nRowIndex;

            //09. 07. 02 김정협 Undo그룹짓기
            if (m_UndoStack.Count > 0)
            {
                sUndoData.nGroupID = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
            }
            else
            {
                sUndoData.nGroupID = 0;
            }

            if (!m_DeleteRowList.Contains(sRow))
            {
                m_DeleteRowList.Add(sRow);
                sUndoData.sDeleteList = sRow;
            }
            else
            {
                sUndoData.sDeleteList.nTabIndex = -1;
            }

            m_UndoStack.Push(sUndoData);
            if (!bTable)
            {
                m_RedoStack.Clear();
            }

            //Adpt_NpcExtraAbility.Update(new DataRow[] { dr });
            m_bSaveEnd = false;
        }

        private void dataGridView1_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            //국가별 define정의할 것
#if __COUNTRY_KR__
            DialogResult result = MessageBox.Show("정말 삭제하시겠습니까? 삭제 후 문제가 발생할 수도 있습니다.", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                e.Cancel = true;
                return;
            }
            else if (result == DialogResult.OK)
            {
                if (dataGridView1.SelectedRows.Count > 1)
                {
                    int nCount = dataGridView1.SelectedRows.Count;
                    int nMin = dataGridView1.SelectedRows[0].Index;
                    for (int i = 0; i < dataGridView1.SelectedRows.Count; i++)
                    {
                        if (nMin > dataGridView1.SelectedRows[i].Index)
                        {
                            nMin = dataGridView1.SelectedRows[i].Index;
                        }
                    }
                    for (int i = 0; i < nCount; i++)
                    {
                        UserDeletingRow(nMin, dataGridView1, tblSkill, false, 0);                        
                    }

                }
                else
                {                    
                    UserDeletingRow(e.Row.Index, dataGridView1, tblSkill, false, 0);
                }

                e.Cancel = true;
            }
#else
            e.Cancel = true;
#endif
        }

        private void dataGridView17_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            //국가별 define정의할 것
#if __COUNTRY_KR__
            DialogResult result = MessageBox.Show("정말 삭제하시겠습니까? 삭제 후 문제가 발생할 수도 있습니다.", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                e.Cancel = true;
                return;
            }
            else if (result == DialogResult.OK)
            {               
                
                if (dataGridView17.SelectedRows.Count > 1)
                {
                    int nCount = dataGridView17.SelectedRows.Count;
                    int nMin = dataGridView17.SelectedRows[0].Index;
                    for (int i = 0; i < dataGridView17.SelectedRows.Count; i++)
                    {
                        if (nMin > dataGridView17.SelectedRows[i].Index)
                        {
                            nMin = dataGridView17.SelectedRows[i].Index;
                        }
                    }
                    for (int i = 0; i < nCount; i++)
                    {
                        UserDeletingRow(nMin, dataGridView17, tblSkillEnum, false, 16);
                    }

                }
                else
                {
                    UserDeletingRow(e.Row.Index, dataGridView17, tblSkillEnum, false, 16);
                }

                e.Cancel = true;
            }
#else
            e.Cancel = true;
#endif
        }

        void UndoKeyboardControl(DataGridView dgvCurrent, DataTable tblCurrent, sUndoRedo sUndoTemp, sUndoRedo sRedoTemp, int nTabPage)
        {
            tabControl1.SelectedIndex = nTabPage;
            if (sUndoTemp.nType == 1) ////1. 수정(CellEditEnd를거침) 2. ADD 3. Delete
            {
                if (sUndoTemp.nColumnIndex != 1)
                {
                    sRedoTemp.nType = sUndoTemp.nType;
                    sRedoTemp.nTabIndex = sUndoTemp.nTabIndex;
                    sRedoTemp.nColumnIndex = sUndoTemp.nColumnIndex;
                    sRedoTemp.nRowIndex = sUndoTemp.nRowIndex;
                    sRedoTemp.objUndoRedo = dgvCurrent[sUndoTemp.nColumnIndex, sUndoTemp.nRowIndex].Value;

                    //09. 07. 02 김정협 Undo그룹짓기
                    sRedoTemp.nGroupID = sUndoTemp.nGroupID;

                    sRedoTemp.sModyfyList = sUndoTemp.sModyfyList;
                    if (sRedoTemp.sModyfyList.nTabIndex != -1)
                    {
                        m_ModifyIDList.Remove(sRedoTemp.sModyfyList);
                    }

                    dgvCurrent[sUndoTemp.nColumnIndex, sUndoTemp.nRowIndex].Value = sUndoTemp.objUndoRedo;

                    //Undo/Redo시 Cell이동에 따른 CellEditEnd이벤트가 발생하는 것을 막기위하여
                    objTemp = dgvCurrent.CurrentCell.Value;
                    dgvCurrent.CurrentCell = dgvCurrent[sUndoTemp.nColumnIndex, sUndoTemp.nRowIndex];
                    UndoRedoLinkDataChange(nTabPage, dgvCurrent);
                }
                else
                {
                    for (int i = 0; i < dgvCurrent.Rows.Count; i++)
                    {
                        if (dgvCurrent["IndexKey", i].Value.ToString() == Convert.ToString(sUndoTemp.nRowIndex))
                        {
                            sRedoTemp.nType = sUndoTemp.nType;
                            sRedoTemp.nTabIndex = sUndoTemp.nTabIndex;
                            sRedoTemp.nColumnIndex = sUndoTemp.nColumnIndex;
                            sRedoTemp.nRowIndex = sUndoTemp.nRowIndex;
                            sRedoTemp.objUndoRedo = dgvCurrent[sUndoTemp.nColumnIndex, i].Value;

                            //09. 07. 02 김정협 Undo그룹짓기
                            sRedoTemp.nGroupID = sUndoTemp.nGroupID;

                            sRedoTemp.sModyfyList = sUndoTemp.sModyfyList;
                            if (sRedoTemp.sModyfyList.nTabIndex != -1)
                            {
                                m_ModifyIDList.Remove(sRedoTemp.sModyfyList);
                            }

                            dgvCurrent[sUndoTemp.nColumnIndex, i].Value = sUndoTemp.objUndoRedo;

                            //Undo/Redo시 Cell이동에 따른 CellEditEnd이벤트가 발생하는 것을 막기위하여
                            objTemp = dgvCurrent.CurrentCell.Value;
                            for (int k = 0; k < dgvCurrent.Rows.Count; k++)
                            {
                                if (dgvCurrent["IndexKey", k].Value.ToString() == Convert.ToString(sUndoTemp.nRowIndex))
                                {
                                    dgvCurrent.CurrentCell = dgvCurrent[sUndoTemp.nColumnIndex, k];
                                    break;
                                }
                            }
                            UndoRedoLinkDataChange(nTabPage, dgvCurrent);

                            SetCellBackgroundColorForDB(nTabPage);
                            ColorRefresh(true);
                            break;
                        }
                    }

                }
                m_RedoStack.Push(sRedoTemp);
                textBox2.Text = Convert.ToString(m_ModifyIDList.Count);
            }
            else if (sUndoTemp.nType == 2)
            {
                //if(sUndoTemp.sModyfyList.nIndexKey == tblItem.Rows[i]
                //09. 07. 02 김정협 언두리두그룹화위해 매개변수 추가
                UserAddUndoRow(sUndoTemp.sModyfyList.nIndexKey, dgvCurrent, tblCurrent, sUndoTemp.sModyfyList, sUndoTemp.nGroupID);
                m_ModifyIDList.Remove(sUndoTemp.sModyfyList);

            }
            else if (sUndoTemp.nType == 3)
            {
                tblCurrent.Columns[1].AllowDBNull = true;
                tblCurrent.Rows.Add();

                DataRow drUndoRow = tblCurrent.Rows[tblCurrent.Rows.Count - 1];

                sRedoTemp.nTabIndex = sUndoTemp.nTabIndex;
                sRedoTemp.nType = sUndoTemp.nType;
                //sRedoTemp.sDeleteList
                for (int i = 0; i < tblCurrent.Columns.Count; i++)
                {
                    drUndoRow[i] = sUndoTemp.sDeleteList.drOriginalRow[i];
                }
                //drUndoRow[1] = sUndoTemp.sDeleteList.drOriginalRow[1];
                if (sUndoTemp.sDeleteList.nTabIndex != -1)
                {
                    sRedoTemp.nRowIndex = Convert.ToInt32(tblCurrent.Rows[tblCurrent.Rows.Count - 1]["IndexKey"].ToString());
                    m_DeleteRowList.Remove(sUndoTemp.sDeleteList);
                }
                //09. 07. 02 김정협 Undo그룹짓기
                sRedoTemp.nGroupID = sUndoTemp.nGroupID;
                m_RedoStack.Push(sRedoTemp);

            }
            dgvCurrent.RefreshEdit();
        }

        void RedoKeyboardControl(DataGridView dgvCurrent, DataTable tblCurrent, sUndoRedo sUndoTemp, sUndoRedo sRedoTemp, int nTabPage)
        {
            tabControl1.SelectedIndex = nTabPage;
            if (sRedoTemp.nType == 1)
            {
                if (sRedoTemp.nColumnIndex != 1)
                {
                    sUndoTemp.nType = sRedoTemp.nType;
                    sUndoTemp.nTabIndex = sRedoTemp.nTabIndex;
                    sUndoTemp.nColumnIndex = sRedoTemp.nColumnIndex;
                    sUndoTemp.nRowIndex = sRedoTemp.nRowIndex;
                    sUndoTemp.objUndoRedo = dgvCurrent[sRedoTemp.nColumnIndex, sRedoTemp.nRowIndex].Value;

                    //09. 07. 02 김정협 Undo그룹짓기
                    sUndoTemp.nGroupID = sRedoTemp.nGroupID;

                    sUndoTemp.sModyfyList = sRedoTemp.sModyfyList;
                    if (sUndoTemp.sModyfyList.nTabIndex != -1)
                    {
                        m_ModifyIDList.Add(sUndoTemp.sModyfyList);
                    }

                    dgvCurrent[sRedoTemp.nColumnIndex, sRedoTemp.nRowIndex].Value = sRedoTemp.objUndoRedo;

                    //Undo/Redo시 Cell이동에 따른 CellEditEnd이벤트가 발생하는 것을 막기위하여
                    objTemp = dgvCurrent.CurrentCell.Value;
                    dgvCurrent.CurrentCell = dgvCurrent[sRedoTemp.nColumnIndex, sRedoTemp.nRowIndex];

                    UndoRedoLinkDataChange(nTabPage, dgvCurrent);
                }
                else
                {
                    for (int i = 0; i < dgvCurrent.Rows.Count; i++)
                    {
                        if (dgvCurrent["IndexKey", i].Value.ToString() == Convert.ToString(sRedoTemp.nRowIndex))
                        {
                            sUndoTemp.nType = sRedoTemp.nType;
                            sUndoTemp.nTabIndex = sRedoTemp.nTabIndex;
                            sUndoTemp.nColumnIndex = sRedoTemp.nColumnIndex;
                            sUndoTemp.nRowIndex = sRedoTemp.nRowIndex;
                            sUndoTemp.objUndoRedo = dgvCurrent[sRedoTemp.nColumnIndex, i].Value;

                            //09. 07. 02 김정협 Undo그룹짓기
                            sUndoTemp.nGroupID = sRedoTemp.nGroupID;

                            sUndoTemp.sModyfyList = sRedoTemp.sModyfyList;
                            if (sUndoTemp.sModyfyList.nTabIndex != -1)
                            {
                                m_ModifyIDList.Add(sUndoTemp.sModyfyList);
                            }

                            dgvCurrent[sRedoTemp.nColumnIndex, i].Value = sRedoTemp.objUndoRedo;

                            //Undo/Redo시 Cell이동에 따른 CellEditEnd이벤트가 발생하는 것을 막기위하여
                            objTemp = dgvCurrent.CurrentCell.Value;
                            for (int k = 0; k < dgvCurrent.Rows.Count; k++)
                            {
                                if (dgvCurrent["IndexKey", k].Value.ToString() == Convert.ToString(sUndoTemp.nRowIndex))
                                {
                                    dgvCurrent.CurrentCell = dgvCurrent.CurrentCell = dgvCurrent[sRedoTemp.nColumnIndex, k];
                                    break;
                                }
                            }

                            UndoRedoLinkDataChange(nTabPage, dgvCurrent);
                            SetCellBackgroundColorForDB(nTabPage);
                            ColorRefresh(true);
                            break;
                        }
                    }

                }
                m_UndoStack.Push(sUndoTemp);
                textBox2.Text = Convert.ToString(m_ModifyIDList.Count);
            }
            else if (sRedoTemp.nType == 2)
            {
                tblCurrent.Rows.Add();
                DataRow drRow = tblCurrent.Rows[tblCurrent.Rows.Count - 1];
                for (int i = 0; i < tblCurrent.Columns.Count; i++)
                {
                    drRow[i] = sRedoTemp.drAdd[i];
                }

                sUndoRedo sUndoData = new sUndoRedo();
                sUndoData.nTabIndex = 0;
                sUndoData.nType = 2;//2. Add 수정
                sUndoData.sModyfyList = sRedoTemp.sModyfyList;

                //09. 07. 02 김정협 Undo그룹짓기
                sUndoTemp.nGroupID = sRedoTemp.nGroupID;

                m_UndoStack.Push(sUndoData);

                m_ModifyIDList.Add(sUndoData.sModyfyList);
            }
            else if (sRedoTemp.nType == 3)
            {
                int nValue = 0;
                for (int i = 0; i < tblCurrent.Rows.Count; i++)
                {
                    if (tblCurrent.Rows[i].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    if (tblCurrent.Rows[i]["IndexKey"].ToString() == sRedoTemp.nRowIndex.ToString())
                    {
                        nValue = i;
                        break;
                    }
                }
                UserDeletingRow(nValue, dgvCurrent, tblCurrent, true, nTabPage);
            }
            dgvCurrent.RefreshEdit();
        }

        void UndoRedoLinkDataChange(int nTabPage, DataGridView dgvCurrent)
        {
            DataGridViewCell dgvCurrentCell;
            dgvCurrentCell = dgvCurrent.CurrentCell;
            int nIndexRow = dgvCurrentCell.RowIndex;
            switch (nTabPage)
            {
                case 0:
                    {
                        //09. 06. 06 김정협
                        if (dgvCurrentCell.OwningColumn.Name == "ID")
                        {
                            DataRow dr = tblSkillEnum.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView1["ID_ENUM", nIndexRow].Value = dr["ID Name"].ToString();
                                dataGridView1["NAME", nIndexRow].Value = dr["Description"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }

                        if (dgvCurrentCell.OwningColumn.Name == "TYPE")
                        {
                            DataRow dr = tblSkillType.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView1["TYPE_ENUM", nIndexRow].Value = dr["Skill Type Name"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }

                    }
                    break;                
            }
        }

        private void tabControl1_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.Z)
            {
                if (m_UndoStack.Count > 0)
                {
                    sUndoRedo sUndoTemp;
                    //09. 07. 02 김정협 그룹같은놈 모두 같이 언두 및 리두
                    do
                    {
                        sUndoTemp = ((sUndoRedo)m_UndoStack.Pop());

                        sUndoRedo sRedoTemp = new sUndoRedo();
                        switch (sUndoTemp.nTabIndex)
                        {
                            case 0:
                                {
                                    UndoKeyboardControl(dataGridView1, tblSkill, sUndoTemp, sRedoTemp, 0);
                                }
                                break;
                            case 16:
                                {
                                    UndoKeyboardControl(dataGridView17, tblSkillEnum, sUndoTemp, sRedoTemp, 16);
                                }
                                break;
                        }
                        if (m_UndoStack.Count <= 0)
                        {
                            break;
                        }

                    } while (((sUndoRedo)m_UndoStack.Peek()).nGroupID == sUndoTemp.nGroupID);

                }
            }


            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.Y)
            {
                if (m_RedoStack.Count > 0)
                {
                    sUndoRedo sRedoTemp;
                    //09. 07. 02 김정협 그룹같은놈 모두 같이 언두 및 리두
                    do
                    {
                        sRedoTemp = ((sUndoRedo)m_RedoStack.Pop());

                        sUndoRedo sUndoTemp = new sUndoRedo();

                        switch (sRedoTemp.nTabIndex)
                        {
                            case 0:
                                {
                                    RedoKeyboardControl(dataGridView1, tblSkill, sUndoTemp, sRedoTemp, 0);
                                }
                                break;
                            case 16:
                                {
                                    RedoKeyboardControl(dataGridView17, tblSkillEnum, sUndoTemp, sRedoTemp, 1);
                                }
                                break;
                        }
                        if (m_RedoStack.Count <= 0)
                        {
                            break;
                        }

                    } while (((sUndoRedo)m_RedoStack.Peek()).nGroupID == sRedoTemp.nGroupID);
                }
            }

            if (e.KeyCode == Keys.Escape)
            {
                if (AssistBox.Visible)
                {
                    AssistBox.Hide();
                }
            }

            if (e.KeyCode == Keys.F5)
            {
                int nCurrentIndex = tabControl1.SelectedIndex;
                switch (tabControl1.SelectedIndex)
                {
                    case 0:
                        {
                            tblSkill.Clear();
                            Adpt_SkillTable.Fill(tblSkill);


                            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여
                                                        
                            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
                            {
                                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["NAME", nRowHeader].Value.ToString();
                            }
                        }
                        break;
                    case 16:
                        {
                            tblSkillEnum.Clear();
                            Adpt_SkillEnum.Fill(tblSkillEnum);
                            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여                            

                            for (int nRowHeader = 0; nRowHeader < dataGridView17.RowCount; nRowHeader++)
                            {
                                dataGridView17.Rows[nRowHeader].HeaderCell.Value = dataGridView17["ID Num", nRowHeader].Value.ToString() + "     " + dataGridView17["Description", nRowHeader].Value.ToString();
                            }
                        }
                        break;
                }

                SetCellBackgroundColorForDB(tabControl1.SelectedIndex);


                m_DeleteRowList.Clear();
                m_ModifyIDList.Clear();
                m_UndoStack.Clear();
                m_RedoStack.Clear();
                m_bSaveEnd = true;


            }
        }

        private void AssistBox_TextChanged(object sender, EventArgs e)
        {
            if (AssistBox.Text.IndexOf("\n") != -1 || AssistBox.Text.IndexOf("\r\n") != -1)
            {
                return;
            }
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    dataGridView1.CurrentCell.Value = AssistBox.Text;
                    break;
                case 16:
                    dataGridView17.CurrentCell.Value = AssistBox.Text;
                    break;
            }
        }

        private void AssistBox_Leave(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    dataGridView1_CellEndEdit(null, null);
                    break;
                case 16:
                    dataGridView17_CellEndEdit(null, null);
                    break;
            }

            AssistBox.Hide();
        }

        private void btnLink_Click(object sender, EventArgs e)
        {
            //09. 06. 06 김정협
            int nCount = 0;

            for (int i = 0; i < tblSkill.Columns.Count; i++)
            {
                if (tblSkill.Columns[i].ColumnName == "ID")
                {
                    for (int j = 0; j < tblSkill.Rows.Count; j++)
                    {
                        if (tblSkill.Rows[j]["ID"].ToString() == "0")
                        {
                            continue;
                        }
                        else
                        {
                            DataRow dr = tblSkillEnum.Rows.Find(tblSkill.Rows[j]["ID"]);
                            if (tblSkill.Rows[j]["NAME"].ToString() != dr["Description"].ToString())
                            {
                                tblSkill.Rows[j]["ID_ENUM"] = dr["ID Name"];
                                tblSkill.Rows[j]["NAME"] = dr["Description"];

                                tblSkill.Rows[j]["ICON_TEXTURE_PIECE_NAME"] = tblSkill.Rows[j]["ID_ENUM"];
                                Adpt_SkillTable.Update(new DataRow[] { tblSkill.Rows[j] });
                                nCount++;
                            }
                        }

                    }
                }

                if (tblSkill.Columns[i].ColumnName == "TYPE")
                {
                    for (int j = 0; j < tblSkill.Rows.Count; j++)
                    {
                        if (tblSkill.Rows[j]["TYPE"].ToString() == "0")
                        {
                            continue;
                        }
                        else
                        {
                            DataRow dr = tblSkillType.Rows.Find(tblSkill.Rows[j]["TYPE"]);
                            if (tblSkill.Rows[j]["TYPE_ENUM"].ToString() != dr["Skill Type Name"].ToString())
                            {
                                tblSkill.Rows[j]["TYPE_ENUM"] = dr["Skill Type Name"];
                                Adpt_SkillTable.Update(new DataRow[] { tblSkill.Rows[j] });
                                nCount++;
                            }
                        }

                    }
                }


                if (tblSkill.Columns[i].ColumnName == "ACTIVE_SKILL_USE_CONDITION")
                {
                    for (int j = 0; j < tblSkill.Rows.Count; j++)
                    {
                        DataRow dr = tblActiveSkillUseCondition.Rows.Find(tblSkill.Rows[j]["ACTIVE_SKILL_USE_CONDITION"]);
                        if (tblSkill.Rows[j]["ACTIVE_SKILL_USE_CONDITION_ENUM"].ToString() != dr["ACTIVE_SKILL_USE_CONDITION"].ToString())
                        {
                            tblSkill.Rows[j]["ACTIVE_SKILL_USE_CONDITION_ENUM"] = dr["ACTIVE_SKILL_USE_CONDITION"].ToString();
                            tblSkill.Rows[j]["Description"] = dr["Description"].ToString();
                            Adpt_SkillTable.Update(new DataRow[] { tblSkill.Rows[j] });
                            nCount++;
                        }                        
                    }
                }
            }

            MessageBox.Show("SkillTable 틀린 개수는 " + nCount + "개의 잘못된 링크를 수정하였습니다.");
            nCount = 0;
        }

        private void dataGridView1_RowStateChanged(object sender, DataGridViewRowStateChangedEventArgs e)
        {
            if (dataGridView1.SelectedRows.Count > 0)
            {
                toolTip1.SetToolTip(dataGridView1, dataGridView1.SelectedRows.Count.ToString() + "R");
            }
        }

        private void dataGridView17_RowStateChanged(object sender, DataGridViewRowStateChangedEventArgs e)
        {
            if (dataGridView17.SelectedRows.Count > 0)
            {
                toolTip1.SetToolTip(dataGridView17, dataGridView17.SelectedRows.Count.ToString() + "R");
            }
        }

        private void btnBorder_Click(object sender, EventArgs e)
        {
            int nRow = -1;
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        for (int i = 0; i < tblBorder.Rows.Count; i++)
                        {
                            if (tblBorder.Rows[i].RowState == DataRowState.Deleted)
                            {
                                continue;
                            }
                            if (tblBorder.Rows[i]["IndexNum"].ToString() == dataGridView1["IndexKey", dataGridView1.CurrentCell.RowIndex].Value.ToString())
                            {
                                nRow = i;
                                break;
                            }
                        }

                        if (nRow == -1)
                        {
                            tblBorder.Rows.Add();
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["TableName"] = "SkillTable";
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["IndexNum"] = dataGridView4["IndexKey", dataGridView4.CurrentCell.RowIndex].Value;
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["IDNum"] = dataGridView1["ID", dataGridView1.CurrentCell.RowIndex].Value;
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["Regdate"] = DateTime.Now;
                        }
                        else
                        {
                            tblBorder.Rows[nRow].Delete();
                        }

                        dataGridView1.Refresh();
                    }
                    break;
                case 16:
                    {
                        for (int i = 0; i < tblBorder.Rows.Count; i++)
                        {
                            if (tblBorder.Rows[i].RowState == DataRowState.Deleted)
                            {
                                continue;
                            }
                            if (tblBorder.Rows[i]["IDNum"].ToString() == dataGridView17["ID Num", dataGridView17.CurrentCell.RowIndex].Value.ToString())
                            {
                                nRow = i;
                                break;
                            }
                        }

                        if (nRow == -1)
                        {
                            tblBorder.Rows.Add();
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["TableName"] = "SkillEnum";                            
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["IDNum"] = dataGridView17["ID Num", dataGridView17.CurrentCell.RowIndex].Value;
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["Regdate"] = DateTime.Now;
                        }
                        else
                        {
                            tblBorder.Rows[nRow].Delete();
                        }

                        dataGridView17.Refresh();
                    }
                    break;
            }
        }

        private void dataGridView1_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            for (int i = 0; i < tblBorder.Rows.Count; i++)
            {
                if (tblBorder.Rows[i].RowState == DataRowState.Deleted || tblBorder.Rows[i]["TableName"].ToString() != "SkillTable")
                {
                    continue;
                }
                int nRow = -1;

                DataRow drTemp = tblBorder.Rows[i];

                int nLeft = 0;
                int nRight = dataGridView1.RowCount - 1;
                while (nLeft <= nRight)
                {
                    int nMid = (nLeft + nRight) / 2;

                    int nDgv = Convert.ToInt32(dataGridView1["ID", nMid].Value.ToString());
                    int nColorID = Convert.ToInt32(drTemp["IDNum"].ToString());
                    if (nDgv == nColorID)
                    {
                        break;
                    }
                    if (nDgv < nColorID)
                    {
                        nLeft = nMid + 1;
                    }
                    if (nDgv > nColorID)
                    {
                        nRight = nMid - 1;
                    }
                }

                for (int k = nLeft; k <= nRight; k++)
                {
                    if (Convert.ToInt32(dataGridView1["IndexKey", k].Value.ToString()) == Convert.ToInt32(drTemp["IndexNum"].ToString()))
                    {
                        nRow = dataGridView1["IndexKey", k].RowIndex;
                        break;
                    }
                }

                if (e.RowIndex == nRow)
                {
                    e.AdvancedBorderStyle.Top = DataGridViewAdvancedCellBorderStyle.OutsetDouble;
                }

            }
        }

        private void dataGridView17_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            for (int i = 0; i < tblBorder.Rows.Count; i++)
            {
                if (tblBorder.Rows[i].RowState == DataRowState.Deleted || tblBorder.Rows[i]["TableName"].ToString() != "SkillEnum")
                {
                    continue;
                }
                int nRow = -1;

                DataRow drTemp = tblBorder.Rows[i];

                int nLeft = 0;
                int nRight = dataGridView17.RowCount - 1;
                while (nLeft <= nRight)
                {
                    int nMid = (nLeft + nRight) / 2;

                    int nDgv = Convert.ToInt32(dataGridView17["ID Num", nMid].Value.ToString());
                    int nColorID = Convert.ToInt32(drTemp["IDNum"].ToString());
                    if (nDgv == nColorID)
                    {
                        nRow = dataGridView17["ID Num", nMid].RowIndex;
                        break;
                    }
                    if (nDgv < nColorID)
                    {
                        nLeft = nMid + 1;
                    }
                    if (nDgv > nColorID)
                    {
                        nRight = nMid - 1;
                    }
                }

                if (e.RowIndex == nRow)
                {
                    e.AdvancedBorderStyle.Top = DataGridViewAdvancedCellBorderStyle.OutsetDouble;
                }

            }
        }

        private void AssistBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape || e.KeyCode == Keys.Enter)
            {
                AssistBox.Hide();
            }
        }

        private void dataGridView2_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView2.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {
                
                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView2.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView2.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView2.ColumnCount <= nColumnIndex ||
                            dataGridView2.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView2[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView2.CurrentCell = dataGridView2[nColumnIndex, nRowIndex];
                            dataGridView2_CellBeginEdit(null, null);                           
                            dataGridView2[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView2_CellEndEdit(null, null);
                           
                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView2.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView2.CurrentCell.ReadOnly)
                {
                    
                    for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                    {
                        dataGridView2.CurrentCell = dataGridView2.SelectedCells[i];
                        dataGridView2_CellBeginEdit(null, null);
                        dataGridView2.SelectedCells[i].Value = "";
                        dataGridView2_CellEndEdit(null, null);
                        
                    }
                }
            }
        }

        private void dataGridView3_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView3.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView3.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView3.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView3.ColumnCount <= nColumnIndex ||
                            dataGridView3.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView3[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView3.CurrentCell = dataGridView3[nColumnIndex, nRowIndex];
                            dataGridView3_CellBeginEdit(null, null);
                            dataGridView3[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView3_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView3.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView3.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                    {
                        dataGridView3.CurrentCell = dataGridView3.SelectedCells[i];
                        dataGridView3_CellBeginEdit(null, null);
                        dataGridView3.SelectedCells[i].Value = "";
                        dataGridView3_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView4_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView4.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView4.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView4.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView4.ColumnCount <= nColumnIndex ||
                            dataGridView4.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView4[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView4.CurrentCell = dataGridView4[nColumnIndex, nRowIndex];
                            dataGridView4_CellBeginEdit(null, null);
                            dataGridView4[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView4_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView4.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView4.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView4.SelectedCells.Count; i++)
                    {
                        dataGridView4.CurrentCell = dataGridView4.SelectedCells[i];
                        dataGridView4_CellBeginEdit(null, null);
                        dataGridView4.SelectedCells[i].Value = "";
                        dataGridView4_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView5_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView5.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView5.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView5.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView5.ColumnCount <= nColumnIndex ||
                            dataGridView5.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView5[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView5.CurrentCell = dataGridView5[nColumnIndex, nRowIndex];
                            dataGridView5_CellBeginEdit(null, null);
                            dataGridView5[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView5_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView5.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView5.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView5.SelectedCells.Count; i++)
                    {
                        dataGridView5.CurrentCell = dataGridView5.SelectedCells[i];
                        dataGridView5_CellBeginEdit(null, null);
                        dataGridView5.SelectedCells[i].Value = "";
                        dataGridView5_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView6_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView6.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView6.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView6.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView6.ColumnCount <= nColumnIndex ||
                            dataGridView6.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView6[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView6.CurrentCell = dataGridView6[nColumnIndex, nRowIndex];
                            dataGridView6_CellBeginEdit(null, null);
                            dataGridView6[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView6_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView6.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView6.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView6.SelectedCells.Count; i++)
                    {
                        dataGridView6.CurrentCell = dataGridView6.SelectedCells[i];
                        dataGridView6_CellBeginEdit(null, null);
                        dataGridView6.SelectedCells[i].Value = "";
                        dataGridView6_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView7_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView7.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView7.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView7.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView7.ColumnCount <= nColumnIndex ||
                            dataGridView7.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView7[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView7.CurrentCell = dataGridView7[nColumnIndex, nRowIndex];
                            dataGridView7_CellBeginEdit(null, null);
                            dataGridView7[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView7_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView7.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView7.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView7.SelectedCells.Count; i++)
                    {
                        dataGridView7.CurrentCell = dataGridView7.SelectedCells[i];
                        dataGridView7_CellBeginEdit(null, null);
                        dataGridView7.SelectedCells[i].Value = "";
                        dataGridView7_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView8_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView8.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView8.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView8.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView8.ColumnCount <= nColumnIndex ||
                            dataGridView8.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView8[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView8.CurrentCell = dataGridView8[nColumnIndex, nRowIndex];
                            dataGridView8_CellBeginEdit(null, null);
                            dataGridView8[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView8_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView8.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView8.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView8.SelectedCells.Count; i++)
                    {
                        dataGridView8.CurrentCell = dataGridView8.SelectedCells[i];
                        dataGridView8_CellBeginEdit(null, null);
                        dataGridView8.SelectedCells[i].Value = "";
                        dataGridView8_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView9_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView9.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView9.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView9.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView9.ColumnCount <= nColumnIndex ||
                            dataGridView9.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView9[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView9.CurrentCell = dataGridView9[nColumnIndex, nRowIndex];
                            dataGridView9_CellBeginEdit(null, null);
                            dataGridView9[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView9_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView9.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView9.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView9.SelectedCells.Count; i++)
                    {
                        dataGridView9.CurrentCell = dataGridView9.SelectedCells[i];
                        dataGridView9_CellBeginEdit(null, null);
                        dataGridView9.SelectedCells[i].Value = "";
                        dataGridView9_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView10_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView10.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView10.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView10.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView10.ColumnCount <= nColumnIndex ||
                            dataGridView10.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView10[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView10.CurrentCell = dataGridView10[nColumnIndex, nRowIndex];
                            dataGridView10_CellBeginEdit(null, null);
                            dataGridView10[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView10_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView10.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView10.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView10.SelectedCells.Count; i++)
                    {
                        dataGridView10.CurrentCell = dataGridView10.SelectedCells[i];
                        dataGridView10_CellBeginEdit(null, null);
                        dataGridView10.SelectedCells[i].Value = "";
                        dataGridView10_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView11_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView11.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView11.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView11.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView11.ColumnCount <= nColumnIndex ||
                            dataGridView11.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView11[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView11.CurrentCell = dataGridView11[nColumnIndex, nRowIndex];
                            dataGridView11_CellBeginEdit(null, null);
                            dataGridView11[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView11_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView11.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView11.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView11.SelectedCells.Count; i++)
                    {
                        dataGridView11.CurrentCell = dataGridView11.SelectedCells[i];
                        dataGridView11_CellBeginEdit(null, null);
                        dataGridView11.SelectedCells[i].Value = "";
                        dataGridView11_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView12_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView12.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView12.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView12.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView12.ColumnCount <= nColumnIndex ||
                            dataGridView12.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView12[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView12.CurrentCell = dataGridView12[nColumnIndex, nRowIndex];
                            dataGridView12_CellBeginEdit(null, null);
                            dataGridView12[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView12_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView12.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView12.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView12.SelectedCells.Count; i++)
                    {
                        dataGridView12.CurrentCell = dataGridView12.SelectedCells[i];
                        dataGridView12_CellBeginEdit(null, null);
                        dataGridView12.SelectedCells[i].Value = "";
                        dataGridView12_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView13_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView13.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView13.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView13.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView13.ColumnCount <= nColumnIndex ||
                            dataGridView13.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView13[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView13.CurrentCell = dataGridView13[nColumnIndex, nRowIndex];
                            dataGridView13_CellBeginEdit(null, null);
                            dataGridView13[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView13_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView13.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView13.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView13.SelectedCells.Count; i++)
                    {
                        dataGridView13.CurrentCell = dataGridView13.SelectedCells[i];
                        dataGridView13_CellBeginEdit(null, null);
                        dataGridView13.SelectedCells[i].Value = "";
                        dataGridView13_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView14_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView14.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView14.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView14.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView14.ColumnCount <= nColumnIndex ||
                            dataGridView14.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView14[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView14.CurrentCell = dataGridView14[nColumnIndex, nRowIndex];
                            dataGridView14_CellBeginEdit(null, null);
                            dataGridView14[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView14_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView14.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView14.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView14.SelectedCells.Count; i++)
                    {
                        dataGridView14.CurrentCell = dataGridView14.SelectedCells[i];
                        dataGridView14_CellBeginEdit(null, null);
                        dataGridView14.SelectedCells[i].Value = "";
                        dataGridView14_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView15_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView15.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView15.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView15.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView15.ColumnCount <= nColumnIndex ||
                            dataGridView15.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView15[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView15.CurrentCell = dataGridView15[nColumnIndex, nRowIndex];
                            dataGridView15_CellBeginEdit(null, null);
                            dataGridView15[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView15_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView15.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView15.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView15.SelectedCells.Count; i++)
                    {
                        dataGridView15.CurrentCell = dataGridView15.SelectedCells[i];
                        dataGridView15_CellBeginEdit(null, null);
                        dataGridView15.SelectedCells[i].Value = "";
                        dataGridView15_CellEndEdit(null, null);

                    }
                }
            }
        }

        private void dataGridView16_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView16.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {

                IDataObject iData = Clipboard.GetDataObject();
                string strClp = (string)iData.GetData(DataFormats.Text);
                string delimstr = "\n";
                char[] delimiter = delimstr.ToCharArray();
                string[] arrData = strClp.Split(delimiter, strClp.Length);
                for (int i = 0; i < arrData.Length; i++)
                {
                    arrData[i] = arrData[i].Replace("\r", "");
                }

                int nColumnIndex = dataGridView16.CurrentCell.ColumnIndex;
                int nRowIndex = dataGridView16.CurrentCell.RowIndex;
                for (int i = 0; i < arrData.Length; i++)
                {
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for (int j = 0; j < strData.Length; j++)
                    {
                        //09. 07. 02 김정협 붙여넣기 영역벗어났을경우 에러나는부분해결
                        if (dataGridView16.ColumnCount <= nColumnIndex ||
                            dataGridView16.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView16[nColumnIndex, nRowIndex].ReadOnly)
                        {
                            dataGridView16.CurrentCell = dataGridView16[nColumnIndex, nRowIndex];
                            dataGridView16_CellBeginEdit(null, null);
                            dataGridView16[nColumnIndex, nRowIndex].Value = strData[j];
                            dataGridView16_CellEndEdit(null, null);

                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView16.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }

            if (e.KeyCode == Keys.Delete)
            {
                if (!dataGridView16.CurrentCell.ReadOnly)
                {

                    for (int i = 0; i < dataGridView16.SelectedCells.Count; i++)
                    {
                        dataGridView16.CurrentCell = dataGridView16.SelectedCells[i];
                        dataGridView16_CellBeginEdit(null, null);
                        dataGridView16.SelectedCells[i].Value = "";
                        dataGridView16_CellEndEdit(null, null);

                    }
                }
            }
        }

        /*
        private void btnLink_Click(object sender, EventArgs e)
        {
            int nCount = 0;
            for (int i = 0; i < tblSkill.Columns.Count; i++)
            {
                if (tblSkill.Columns[i].ColumnName == "ID")
                {
                    for (int j = 0; j < tblSkill.Rows.Count; j++)
                    {
                        DataRow dr = tblSkillEnum.Rows.Find(tblSkill.Rows[j]["ID"].ToString());
                        if (tblSkill.Rows[j]["NAME"].ToString() != dr["Description"].ToString())
                        {
                            tblSkill.Rows[j]["NAME"] = dr["Description"];                            

                            Adpt_SkillTable.Update(new DataRow[] { tblSkill.Rows[j] });
                            nCount++;
                        }
                    }
                }
            }

            MessageBox.Show("총 " + nCount + "개의 링크 오류를 수정했습니다.");
        }*/
        


    }
}