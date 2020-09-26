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
    public partial class TitleTable : Form
    {
        

        private SqlConnection Con;
        private SqlDataAdapter Adpt_TitleTable;
        DataTable tblTitle;
        object objTemp;
        object objTemp2;


        //09. 06. 03 김정협 추가 : 셀배경색 DB
        //ColorTable용        
        private SqlDataAdapter Adpt_ColorTable;
        DataTable tblColor;

        //09. 07. 07         
        //BorderTable용        
        private SqlDataAdapter Adpt_BorderTable;
        DataTable tblBorder;

        //TitleTableHistory용        
        private SqlDataAdapter Adpt_TitleTable_History;
        DataTable tblTitle_History;

        //TitleTableDelete용        
        private SqlDataAdapter Adpt_TitleTable_DeleteHistory;
        DataTable tblTitle_DeleteHistory;

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

        //TitleMission 용
        private SqlDataAdapter Adpt_TitleMission;
        DataTable tblTitleMission;
        //TitleMission History용        
        private SqlDataAdapter Adpt_TitleMission_History;
        DataTable tblTitleMission_History;

        //TitleTableDelete용        
        private SqlDataAdapter Adpt_TitleMission_DeleteHistory;
        DataTable tblTitleMission_DeleteHistory;

        //SubTitleMission 용
        private SqlDataAdapter Adpt_SubTitleMission;
        DataTable tblSubTitleMission;
        //SubTitle History용        
        private SqlDataAdapter Adpt_SubTitleMission_History;
        DataTable tblSubTitleMission_History;

        //TitleTableDelete용        
        private SqlDataAdapter Adpt_SubTitleMission_DeleteHistory;
        DataTable tblSubTitleMission_DeleteHistory;

        //TitleType용        
        private SqlDataAdapter Adpt_TitleType;
        DataTable tblTitleType;

        //MissionClearType용        
        private SqlDataAdapter Adpt_MissionClearType;
        DataTable tblMissionClearType;

        //UnitType_Title용        
        private SqlDataAdapter Adpt_UnitType_Title;
        DataTable tblUnitType_Title;

        //UnitClass_Title용        
        private SqlDataAdapter Adpt_UnitClass_Title;
        DataTable tblUnitClass_Title;

        //NpcID용        
        private SqlDataAdapter Adpt_NpcID;
        DataTable tblNpcID;

        //Item용
        private SqlDataAdapter Adpt_Item;
        DataTable tblItem;

        
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

        
        public TitleTable()
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
                case 1:
                    {
                        for (int i = 0; i < dataGridView2.RowCount; i++)
                        {
                            for (int k = 0; k < dataGridView2.ColumnCount; k++)
                            {
                                if (dataGridView2[k, i].HasStyle)
                                    dataGridView2[k, i].Style.ForeColor = Color.Black;
                            }
                        }

                        //09. 06. 01 김정협 수정 : IndexKey와 대조하여 색을 새로 입힘
                        for (int j = 0; j < m_ModifyIDList.Count; j++)
                        {
                            if (((sTableValue)m_ModifyIDList[j]).nTabIndex == 1)
                            {
                                for (int i = 0; i < dataGridView2.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView2["IndexKey", i].Value.ToString()) == ((sTableValue)m_ModifyIDList[j]).nIndexKey)
                                    {
                                        if (!((sTableValue)m_ModifyIDList[j]).bCell)
                                        {
                                            //m_dgvData.Rows[i].DefaultCellStyle.ForeColor = Color.Red;
                                            for (int k = 0; k < dataGridView2.ColumnCount; k++)
                                            {
                                                if (bRed)
                                                    dataGridView2[k, i].Style.ForeColor = Color.Red;
                                                else
                                                    dataGridView2[k, i].Style.ForeColor = Color.Black;
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
                                for (int i = 0; i < dataGridView2.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView2["IndexKey", i].Value.ToString()) == ((sChangeCell)m_ChangeCellList[j]).nIndexKey)
                                    {
                                        if (bRed)
                                        {
                                            dataGridView2[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Red;
                                            dataGridView2["Regdate", i].Style.ForeColor = Color.Red;
                                        }
                                        else
                                        {
                                            dataGridView2[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Black;
                                            dataGridView2["Regdate", i].Style.ForeColor = Color.Black;
                                        }
                                        break;
                                    }
                                }
                            }
                        }

                    }
                    break;

                case 2:
                    {
                        for (int i = 0; i < dataGridView3.RowCount; i++)
                        {
                            for (int k = 0; k < dataGridView3.ColumnCount; k++)
                            {
                                if (dataGridView3[k, i].HasStyle)
                                    dataGridView3[k, i].Style.ForeColor = Color.Black;
                            }
                        }

                        //09. 06. 01 김정협 수정 : IndexKey와 대조하여 색을 새로 입힘
                        for (int j = 0; j < m_ModifyIDList.Count; j++)
                        {
                            if (((sTableValue)m_ModifyIDList[j]).nTabIndex == 2)
                            {
                                for (int i = 0; i < dataGridView3.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView3["IndexKey", i].Value.ToString()) == ((sTableValue)m_ModifyIDList[j]).nIndexKey)
                                    {
                                        if (!((sTableValue)m_ModifyIDList[j]).bCell)
                                        {
                                            //m_dgvData.Rows[i].DefaultCellStyle.ForeColor = Color.Red;
                                            for (int k = 0; k < dataGridView3.ColumnCount; k++)
                                            {
                                                if (bRed)
                                                    dataGridView3[k, i].Style.ForeColor = Color.Red;
                                                else
                                                    dataGridView3[k, i].Style.ForeColor = Color.Black;
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
                            if (((sChangeCell)m_ChangeCellList[j]).nTabIndex == 2)
                            {
                                for (int i = 0; i < dataGridView3.RowCount; i++)
                                {
                                    if (Convert.ToInt32(dataGridView3["IndexKey", i].Value.ToString()) == ((sChangeCell)m_ChangeCellList[j]).nIndexKey)
                                    {
                                        if (bRed)
                                        {
                                            dataGridView3[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Red;
                                            dataGridView3["Regdate", i].Style.ForeColor = Color.Red;
                                        }
                                        else
                                        {
                                            dataGridView3[((sChangeCell)m_ChangeCellList[j]).nColumnIndex, i].Style.ForeColor = Color.Black;
                                            dataGridView3["Regdate", i].Style.ForeColor = Color.Black;
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

        private void Save()
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
                case 1:
                    nCurrentPosColumnIndex = dataGridView2.CurrentCell.ColumnIndex;
                    nCurrentPosRowIndex = dataGridView2.CurrentCell.RowIndex;
                    break;
                case 2:
                    nCurrentPosColumnIndex = dataGridView3.CurrentCell.ColumnIndex;
                    nCurrentPosRowIndex = dataGridView3.CurrentCell.RowIndex;
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
            ScriptData_TWDataSetTableAdapters.TitleTableTableAdapter Adt_TitleTableTW = null;
            DataTable tblTitleTW = null;
            ScriptData_TWDataSetTableAdapters.TitleTable_HistoryTableAdapter Adt_TitleTableTW_History = null;
            DataTable tblTitleTW_History = null;
            ScriptData_TWDataSetTableAdapters.TitleTable_DeleteHistoryTableAdapter Adpt_TitleTableTW_DeleteHistory = null;
            DataTable tblTitleTW_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_TitleTableTW = new ScriptData_TWDataSetTableAdapters.TitleTableTableAdapter();

                tblTitleTW = Adt_TitleTableTW.GetData();
                tblTitleTW.Columns["IndexKey"].AllowDBNull = true;
                tblTitleTW.Columns["TitleID"].AllowDBNull = true;
                tblTitleTW.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(0))
                {
                    Adt_TitleTableTW_History = new ScriptData_TWDataSetTableAdapters.TitleTable_HistoryTableAdapter();

                    tblTitleTW_History = Adt_TitleTableTW_History.GetData();

                    tblTitleTW_History.Columns["IndexKey"].AllowDBNull = true;
                    tblTitleTW_History.Columns["TitleID"].AllowDBNull = true;
                    tblTitleTW_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_TitleTableTW_DeleteHistory = new ScriptData_TWDataSetTableAdapters.TitleTable_DeleteHistoryTableAdapter();

                    tblTitleTW_DeleteHistory = Adpt_TitleTableTW_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblTitleTW_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblTitleTW_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }


            ScriptData_TWDataSetTableAdapters.TitleMissionTableAdapter Adt_TitleMissionTW = null;
            DataTable tblTitleMissionTW = null;
            ScriptData_TWDataSetTableAdapters.TitleMission_HistoryTableAdapter Adt_TitleMissionTW_History = null;
            DataTable tblTitleMissionTW_History = null;
            ScriptData_TWDataSetTableAdapters.TitleMission_DeleteHistoryTableAdapter Adpt_TitleMissionTW_DeleteHistory = null;
            DataTable tblTitleMissionTW_DeleteHistory = null;
            if (arModifyTabIndex.Contains(1) || arDeleteTabIndex.Contains(1))
            {
                Adt_TitleMissionTW = new ScriptData_TWDataSetTableAdapters.TitleMissionTableAdapter();

                tblTitleMissionTW = Adt_TitleMissionTW.GetData();
                tblTitleMissionTW.Columns["IndexKey"].AllowDBNull = true;
                tblTitleMissionTW.Columns["ID"].AllowDBNull = true;
                tblTitleMissionTW.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(1))
                {
                    Adt_TitleMissionTW_History = new ScriptData_TWDataSetTableAdapters.TitleMission_HistoryTableAdapter();

                    tblTitleMissionTW_History = Adt_TitleMissionTW_History.GetData();

                    tblTitleMissionTW_History.Columns["IndexKey"].AllowDBNull = true;
                    tblTitleMissionTW_History.Columns["ID"].AllowDBNull = true;
                    tblTitleMissionTW_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(1))
                {
                    Adpt_TitleMissionTW_DeleteHistory = new ScriptData_TWDataSetTableAdapters.TitleMission_DeleteHistoryTableAdapter();

                    tblTitleMissionTW_DeleteHistory = Adpt_TitleMissionTW_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblTitleMissionTW_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblTitleMissionTW_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_TWDataSetTableAdapters.SubTitleMissionTableAdapter Adt_SubTitleMissionTW = null;
            DataTable tblSubTitleMissionTW = null;
            ScriptData_TWDataSetTableAdapters.SubTitleMission_HistoryTableAdapter Adt_SubTitleMissionTW_History = null;
            DataTable tblSubTitleMissionTW_History = null;
            ScriptData_TWDataSetTableAdapters.SubTitleMission_DeleteHistoryTableAdapter Adpt_SubTitleMissionTW_DeleteHistory = null;
            DataTable tblSubTitleMissionTW_DeleteHistory = null;
            if (arModifyTabIndex.Contains(2) || arDeleteTabIndex.Contains(2))
            {
                Adt_SubTitleMissionTW = new ScriptData_TWDataSetTableAdapters.SubTitleMissionTableAdapter();

                tblSubTitleMissionTW = Adt_SubTitleMissionTW.GetData();

                tblSubTitleMissionTW.Columns["IndexKey"].AllowDBNull = true;
                tblSubTitleMissionTW.Columns["ID"].AllowDBNull = true;
                tblSubTitleMissionTW.Columns["Regdate"].AllowDBNull = true;
                                

                if (arModifyTabIndex.Contains(2))
                {
                    Adt_SubTitleMissionTW_History = new ScriptData_TWDataSetTableAdapters.SubTitleMission_HistoryTableAdapter();

                    tblSubTitleMissionTW_History = Adt_SubTitleMissionTW_History.GetData();

                    tblSubTitleMissionTW_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSubTitleMissionTW_History.Columns["ID"].AllowDBNull = true;
                    tblSubTitleMissionTW_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(2))
                {
                    Adpt_SubTitleMissionTW_DeleteHistory = new ScriptData_TWDataSetTableAdapters.SubTitleMission_DeleteHistoryTableAdapter();

                    tblSubTitleMissionTW_DeleteHistory = Adpt_SubTitleMissionTW_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblSubTitleMissionTW_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSubTitleMissionTW_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }

            ///////////////////////////////////////////////////////////////////////////
            //홍콩버전
            ///////////////////////////////////////////////////////////////////////////

            ScriptData_HKDataSetTableAdapters.TitleTableTableAdapter Adt_TitleTableHK = null;
            DataTable tblTitleHK = null;
            ScriptData_HKDataSetTableAdapters.TitleTable_HistoryTableAdapter Adt_TitleTableHK_History = null;
            DataTable tblTitleHK_History = null;
            ScriptData_HKDataSetTableAdapters.TitleTable_DeleteHistoryTableAdapter Adpt_TitleTableHK_DeleteHistory = null;
            DataTable tblTitleHK_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_TitleTableHK = new ScriptData_HKDataSetTableAdapters.TitleTableTableAdapter();

                tblTitleHK = Adt_TitleTableHK.GetData();
                tblTitleHK.Columns["IndexKey"].AllowDBNull = true;
                tblTitleHK.Columns["TitleID"].AllowDBNull = true;
                tblTitleHK.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(0))
                {
                    Adt_TitleTableHK_History = new ScriptData_HKDataSetTableAdapters.TitleTable_HistoryTableAdapter();

                    tblTitleHK_History = Adt_TitleTableHK_History.GetData();

                    tblTitleHK_History.Columns["IndexKey"].AllowDBNull = true;
                    tblTitleHK_History.Columns["TitleID"].AllowDBNull = true;
                    tblTitleHK_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_TitleTableHK_DeleteHistory = new ScriptData_HKDataSetTableAdapters.TitleTable_DeleteHistoryTableAdapter();

                    tblTitleHK_DeleteHistory = Adpt_TitleTableHK_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblTitleHK_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblTitleHK_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }


            ScriptData_HKDataSetTableAdapters.TitleMissionTableAdapter Adt_TitleMissionHK = null;
            DataTable tblTitleMissionHK = null;
            ScriptData_HKDataSetTableAdapters.TitleMission_HistoryTableAdapter Adt_TitleMissionHK_History = null;
            DataTable tblTitleMissionHK_History = null;
            ScriptData_HKDataSetTableAdapters.TitleMission_DeleteHistoryTableAdapter Adpt_TitleMissionHK_DeleteHistory = null;
            DataTable tblTitleMissionHK_DeleteHistory = null;
            if (arModifyTabIndex.Contains(1) || arDeleteTabIndex.Contains(1))
            {
                Adt_TitleMissionHK = new ScriptData_HKDataSetTableAdapters.TitleMissionTableAdapter();

                tblTitleMissionHK = Adt_TitleMissionHK.GetData();
                tblTitleMissionHK.Columns["IndexKey"].AllowDBNull = true;
                tblTitleMissionHK.Columns["ID"].AllowDBNull = true;
                tblTitleMissionHK.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(1))
                {
                    Adt_TitleMissionHK_History = new ScriptData_HKDataSetTableAdapters.TitleMission_HistoryTableAdapter();

                    tblTitleMissionHK_History = Adt_TitleMissionHK_History.GetData();

                    tblTitleMissionHK_History.Columns["IndexKey"].AllowDBNull = true;
                    tblTitleMissionHK_History.Columns["ID"].AllowDBNull = true;
                    tblTitleMissionHK_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(1))
                {
                    Adpt_TitleMissionHK_DeleteHistory = new ScriptData_HKDataSetTableAdapters.TitleMission_DeleteHistoryTableAdapter();

                    tblTitleMissionHK_DeleteHistory = Adpt_TitleMissionHK_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblTitleMissionHK_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblTitleMissionHK_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_HKDataSetTableAdapters.SubTitleMissionTableAdapter Adt_SubTitleMissionHK = null;
            DataTable tblSubTitleMissionHK = null;
            ScriptData_HKDataSetTableAdapters.SubTitleMission_HistoryTableAdapter Adt_SubTitleMissionHK_History = null;
            DataTable tblSubTitleMissionHK_History = null;
            ScriptData_HKDataSetTableAdapters.SubTitleMission_DeleteHistoryTableAdapter Adpt_SubTitleMissionHK_DeleteHistory = null;
            DataTable tblSubTitleMissionHK_DeleteHistory = null;
            if (arModifyTabIndex.Contains(2) || arDeleteTabIndex.Contains(2))
            {
                Adt_SubTitleMissionHK = new ScriptData_HKDataSetTableAdapters.SubTitleMissionTableAdapter();

                tblSubTitleMissionHK = Adt_SubTitleMissionHK.GetData();

                tblSubTitleMissionHK.Columns["IndexKey"].AllowDBNull = true;
                tblSubTitleMissionHK.Columns["ID"].AllowDBNull = true;
                tblSubTitleMissionHK.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(2))
                {
                    Adt_SubTitleMissionHK_History = new ScriptData_HKDataSetTableAdapters.SubTitleMission_HistoryTableAdapter();

                    tblSubTitleMissionHK_History = Adt_SubTitleMissionHK_History.GetData();

                    tblSubTitleMissionHK_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSubTitleMissionHK_History.Columns["ID"].AllowDBNull = true;
                    tblSubTitleMissionHK_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(2))
                {
                    Adpt_SubTitleMissionHK_DeleteHistory = new ScriptData_HKDataSetTableAdapters.SubTitleMission_DeleteHistoryTableAdapter();

                    tblSubTitleMissionHK_DeleteHistory = Adpt_SubTitleMissionHK_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblSubTitleMissionHK_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSubTitleMissionHK_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }


            ///////////////////////////////////////////////////////////////////////////
            //일본버전
            ///////////////////////////////////////////////////////////////////////////

            ScriptData_JPDataSetTableAdapters.TitleTableTableAdapter Adt_TitleTableJP = null;
            DataTable tblTitleJP = null;
            ScriptData_JPDataSetTableAdapters.TitleTable_HistoryTableAdapter Adt_TitleTableJP_History = null;
            DataTable tblTitleJP_History = null;
            ScriptData_JPDataSetTableAdapters.TitleTable_DeleteHistoryTableAdapter Adpt_TitleTableJP_DeleteHistory = null;
            DataTable tblTitleJP_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_TitleTableJP = new ScriptData_JPDataSetTableAdapters.TitleTableTableAdapter();

                tblTitleJP = Adt_TitleTableJP.GetData();
                tblTitleJP.Columns["IndexKey"].AllowDBNull = true;
                tblTitleJP.Columns["TitleID"].AllowDBNull = true;
                tblTitleJP.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(0))
                {
                    Adt_TitleTableJP_History = new ScriptData_JPDataSetTableAdapters.TitleTable_HistoryTableAdapter();

                    tblTitleJP_History = Adt_TitleTableJP_History.GetData();

                    tblTitleJP_History.Columns["IndexKey"].AllowDBNull = true;
                    tblTitleJP_History.Columns["TitleID"].AllowDBNull = true;
                    tblTitleJP_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_TitleTableJP_DeleteHistory = new ScriptData_JPDataSetTableAdapters.TitleTable_DeleteHistoryTableAdapter();

                    tblTitleJP_DeleteHistory = Adpt_TitleTableJP_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblTitleJP_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblTitleJP_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }


            ScriptData_JPDataSetTableAdapters.TitleMissionTableAdapter Adt_TitleMissionJP = null;
            DataTable tblTitleMissionJP = null;
            ScriptData_JPDataSetTableAdapters.TitleMission_HistoryTableAdapter Adt_TitleMissionJP_History = null;
            DataTable tblTitleMissionJP_History = null;
            ScriptData_JPDataSetTableAdapters.TitleMission_DeleteHistoryTableAdapter Adpt_TitleMissionJP_DeleteHistory = null;
            DataTable tblTitleMissionJP_DeleteHistory = null;
            if (arModifyTabIndex.Contains(1) || arDeleteTabIndex.Contains(1))
            {
                Adt_TitleMissionJP = new ScriptData_JPDataSetTableAdapters.TitleMissionTableAdapter();

                tblTitleMissionJP = Adt_TitleMissionJP.GetData();
                tblTitleMissionJP.Columns["IndexKey"].AllowDBNull = true;
                tblTitleMissionJP.Columns["ID"].AllowDBNull = true;
                tblTitleMissionJP.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(1))
                {
                    Adt_TitleMissionJP_History = new ScriptData_JPDataSetTableAdapters.TitleMission_HistoryTableAdapter();

                    tblTitleMissionJP_History = Adt_TitleMissionJP_History.GetData();

                    tblTitleMissionJP_History.Columns["IndexKey"].AllowDBNull = true;
                    tblTitleMissionJP_History.Columns["ID"].AllowDBNull = true;
                    tblTitleMissionJP_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(1))
                {
                    Adpt_TitleMissionJP_DeleteHistory = new ScriptData_JPDataSetTableAdapters.TitleMission_DeleteHistoryTableAdapter();

                    tblTitleMissionJP_DeleteHistory = Adpt_TitleMissionJP_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblTitleMissionJP_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblTitleMissionJP_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_JPDataSetTableAdapters.SubTitleMissionTableAdapter Adt_SubTitleMissionJP = null;
            DataTable tblSubTitleMissionJP = null;
            ScriptData_JPDataSetTableAdapters.SubTitleMission_HistoryTableAdapter Adt_SubTitleMissionJP_History = null;
            DataTable tblSubTitleMissionJP_History = null;
            ScriptData_JPDataSetTableAdapters.SubTitleMission_DeleteHistoryTableAdapter Adpt_SubTitleMissionJP_DeleteHistory = null;
            DataTable tblSubTitleMissionJP_DeleteHistory = null;
            if (arModifyTabIndex.Contains(2) || arDeleteTabIndex.Contains(2))
            {
                Adt_SubTitleMissionJP = new ScriptData_JPDataSetTableAdapters.SubTitleMissionTableAdapter();

                tblSubTitleMissionJP = Adt_SubTitleMissionJP.GetData();

                tblSubTitleMissionJP.Columns["IndexKey"].AllowDBNull = true;
                tblSubTitleMissionJP.Columns["ID"].AllowDBNull = true;
                tblSubTitleMissionJP.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(2))
                {
                    Adt_SubTitleMissionJP_History = new ScriptData_JPDataSetTableAdapters.SubTitleMission_HistoryTableAdapter();

                    tblSubTitleMissionJP_History = Adt_SubTitleMissionJP_History.GetData();

                    tblSubTitleMissionJP_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSubTitleMissionJP_History.Columns["ID"].AllowDBNull = true;
                    tblSubTitleMissionJP_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(2))
                {
                    Adpt_SubTitleMissionJP_DeleteHistory = new ScriptData_JPDataSetTableAdapters.SubTitleMission_DeleteHistoryTableAdapter();

                    tblSubTitleMissionJP_DeleteHistory = Adpt_SubTitleMissionJP_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblSubTitleMissionJP_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSubTitleMissionJP_DeleteHistory.Columns[nTemp].AllowDBNull = true;
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

                            tblTitle_History.Rows.Add();

                            Row_History = tblTitle_History.Rows[tblTitle_History.Rows.Count - 1];
                            //09. 06. 01 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey전부 검색 중간 삭제시 인덱스 어긋나기 때문
                            for (int nRow = 0; nRow < tblTitle.Rows.Count; nRow++)
                            {
                                if (tblTitle.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblTitle.Rows[nRow]["IndexKey"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblTitle.Rows[nRow];
                                    break;
                                }
                            }

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblTitle.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];


                            Adpt_TitleTable_History.Update(new DataRow[] { Row_History });
                            Adpt_TitleTable.Update(new DataRow[] { Row });
                            

                            //09. 06. 19 김정협
                            //국가별 #define처리 해 줄 것
                            // 이부분은 한국일경우만 실행하게 함. 한국에서 추가하면 모두 추가됨
#if __COUNTRY_KR__

                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblTitleTW.Rows.Add();
                            tblTitleTW_History.Rows.Add();
                            DataRow RowTW_History = tblTitleTW_History.Rows[tblTitleTW_History.Rows.Count - 1];

                            int nRowIndex = -1;
                            for (int twRow = 0; twRow < tblTitleTW.Rows.Count; twRow++)
                            {
                                if (tblTitleTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    nRowIndex = twRow;
                                    break;
                                }
                            }

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            int nBefore = Convert.ToInt32(Row["IndexKey"].ToString());
                            if (nRowIndex == -1)
                            {
                                //tblTitle.Clear();
                                //Adpt_TitleTable.Fill(tblTitle);
                                //Row = tblTitle.Rows[tblTitle.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_TitleTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblTitleTW.Columns.Count; nTemp++)
                                {
                                    tblTitleTW.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowTW = null;// = tblTitleTW.Rows[tblTitleTW.Rows.Count - 1];
                                tblTitleTW.Rows.Add();
                                RowTW = tblTitleTW.Rows[tblTitleTW.Rows.Count - 1];

                                for (int twColumn = 0; twColumn < tblTitleTW.Columns.Count; twColumn++)
                                {
                                    /*if (tblTitleTW.Columns[twColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "TitleTable";
                                RowTrans["Column"] = "Title_Name";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString()); 
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleTW.Rows[tblTitleTW.Rows.Count - 1]["Title_Name"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowTW["Index_Name"] = nIndexCount;
                                //nIndexCount++;

                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "TW";
                                RowTrans["DB"] = "ScriptData_TW";
                                RowTrans["TableName"] = "TitleTable";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString()); 
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleTW.Rows[tblTitleTW.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                //RowTW["Index_Description_In_Shop"] = nIndexCount;
                                //nIndexCount++;

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
                                RowRelease["TableName"] = "TitleTable";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString()); 
                                RowRelease["Name"] = tblTitleTW.Rows[tblTitleTW.Rows.Count - 1]["Title_Name"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = DateTime.Now;
                                RowRelease["Last Update Date"] = DateTime.Now;


                                for (int twColumn_His = 0; twColumn_His < tblTitleTW_History.Columns.Count; twColumn_His++)
                                {
                                    RowTW_History[twColumn_His] = Row_History[twColumn_His];
                                }

                                Adt_TitleTableTW.Update(RowTW);
                                Adt_TitleTableTW_History.Update(RowTW_History);

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

                                int nCurrent = (int)Adt_TitleTableTW.GetMaxIndexRow();
                                if (nBefore != nCurrent)
                                {
                                    Adpt_Trans.UpdateIndexKey(nCurrent, nBefore, "TitleTable");
                                    Adpt_Release.UpdateIndexKey(nCurrent, nBefore, "TitleTable");
                                }
                            }

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblTitleHK.Rows.Add();
                            tblTitleHK_History.Rows.Add();
                            DataRow RowHK_History = tblTitleHK_History.Rows[tblTitleHK_History.Rows.Count - 1];

                            

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            
                            if (nRowIndex == -1)
                            {
                                //tblTitle.Clear();
                                //Adpt_TitleTable.Fill(tblTitle);
                                //Row = tblTitle.Rows[tblTitle.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_TitleTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblTitleHK.Columns.Count; nTemp++)
                                {
                                    tblTitleHK.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowHK = null;// = tblTitleHK.Rows[tblTitleHK.Rows.Count - 1];
                                tblTitleHK.Rows.Add();
                                RowHK = tblTitleHK.Rows[tblTitleHK.Rows.Count - 1];

                                for (int HKColumn = 0; HKColumn < tblTitleHK.Columns.Count; HKColumn++)
                                {
                                    /*if (tblTitleHK.Columns[HKColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "TitleTable";
                                RowTrans["Column"] = "Title_Name";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleHK.Rows[tblTitleHK.Rows.Count - 1]["Title_Name"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowHK["Index_Name"] = nIndexCount;
                                //nIndexCount++;

                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "HK";
                                RowTrans["DB"] = "ScriptData_HK";
                                RowTrans["TableName"] = "TitleTable";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleHK.Rows[tblTitleHK.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                //RowHK["Index_Description_In_Shop"] = nIndexCount;
                                //nIndexCount++;

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
                                RowRelease["TableName"] = "TitleTable";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["Name"] = tblTitleHK.Rows[tblTitleHK.Rows.Count - 1]["Title_Name"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = DateTime.Now;
                                RowRelease["Last Update Date"] = DateTime.Now;


                                for (int HKColumn_His = 0; HKColumn_His < tblTitleHK_History.Columns.Count; HKColumn_His++)
                                {
                                    RowHK_History[HKColumn_His] = Row_History[HKColumn_His];
                                }

                                Adt_TitleTableHK.Update(RowHK);
                                Adt_TitleTableHK_History.Update(RowHK_History);

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
                            //tblTitleJP.Rows.Add();
                            tblTitleJP_History.Rows.Add();
                            DataRow RowJP_History = tblTitleJP_History.Rows[tblTitleJP_History.Rows.Count - 1];



                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도

                            if (nRowIndex == -1)
                            {
                                //tblTitle.Clear();
                                //Adpt_TitleTable.Fill(tblTitle);
                                //Row = tblTitle.Rows[tblTitle.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_TitleTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblTitleJP.Columns.Count; nTemp++)
                                {
                                    tblTitleJP.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowJP = null;// = tblTitleJP.Rows[tblTitleJP.Rows.Count - 1];
                                tblTitleJP.Rows.Add();
                                RowJP = tblTitleJP.Rows[tblTitleJP.Rows.Count - 1];

                                for (int JPColumn = 0; JPColumn < tblTitleJP.Columns.Count; JPColumn++)
                                {
                                    /*if (tblTitleJP.Columns[JPColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "TitleTable";
                                RowTrans["Column"] = "Title_Name";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleJP.Rows[tblTitleJP.Rows.Count - 1]["Title_Name"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowJP["Index_Name"] = nIndexCount;
                                //nIndexCount++;

                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "JP";
                                RowTrans["DB"] = "ScriptData_JP";
                                RowTrans["TableName"] = "TitleTable";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleJP.Rows[tblTitleJP.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                //RowJP["Index_Description_In_Shop"] = nIndexCount;
                                //nIndexCount++;

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
                                RowRelease["TableName"] = "TitleTable";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["Name"] = tblTitleJP.Rows[tblTitleJP.Rows.Count - 1]["Title_Name"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = DateTime.Now;
                                RowRelease["Last Update Date"] = DateTime.Now;


                                for (int JPColumn_His = 0; JPColumn_His < tblTitleJP_History.Columns.Count; JPColumn_His++)
                                {
                                    RowJP_History[JPColumn_His] = Row_History[JPColumn_His];
                                }

                                Adt_TitleTableJP.Update(RowJP);
                                Adt_TitleTableJP_History.Update(RowJP_History);

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
                    case 1:
                        {
                            
                            tblTitleMission_History.Rows.Add();

                            Row_History = tblTitleMission_History.Rows[tblTitleMission_History.Rows.Count - 1];
                            //09. 06. 01 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey값을 이용. tblItem은 IndexKey값이 로우와 같으므로 
                            for (int nRow = 0; nRow < tblTitleMission.Rows.Count; nRow++)
                            {
                                if (tblTitleMission.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblTitleMission.Rows[nRow]["IndexKey"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblTitleMission.Rows[nRow];
                                    break;
                                }
                            }

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblTitleMission.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];


                            Adpt_TitleMission_History.Update(new DataRow[] { Row_History });
                            Adpt_TitleMission.Update(new DataRow[] { Row });

                            //09. 06. 19 김정협
                            //국가별 #define처리 해 줄 것
                            // 이부분은 한국일경우만 실행하게 함. 한국에서 추가하면 모두 추가됨
#if __COUNTRY_KR__

                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblTitleMissionTW.Rows.Add();
                            tblTitleMissionTW_History.Rows.Add();
                            DataRow RowTW_History = tblTitleMissionTW_History.Rows[tblTitleMissionTW_History.Rows.Count - 1];

                            int nRowIndex = -1;
                            for (int twRow = 0; twRow < tblTitleMissionTW.Rows.Count; twRow++)
                            {
                                if (tblTitleMissionTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    nRowIndex = twRow;
                                    break;
                                }
                            }

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            int nBefore = Convert.ToInt32(Row["IndexKey"].ToString());
                            if (nRowIndex == -1)
                            {
                                //tblTitleMission.Clear();
                                //Adpt_TitleMission.Fill(tblTitleMission);
                                //Row = tblTitleMission.Rows[tblTitleMission.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_TitleMission.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblTitleMissionTW.Columns.Count; nTemp++)
                                {
                                    tblTitleMissionTW.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowTW = null;// = tblTitleMissionTW.Rows[tblTitleMissionTW.Rows.Count - 1];
                                tblTitleMissionTW.Rows.Add();
                                RowTW = tblTitleMissionTW.Rows[tblTitleMissionTW.Rows.Count - 1];

                                for (int twColumn = 0; twColumn < tblTitleMissionTW.Columns.Count; twColumn++)
                                {
                                    /*if (tblTitleMissionTW.Columns[twColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "TitleMission";
                                RowTrans["Column"] = "Mission_Name";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString()); 
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleMissionTW.Rows[tblTitleMissionTW.Rows.Count - 1]["Mission_Name"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int twColumn_His = 0; twColumn_His < tblTitleMissionTW_History.Columns.Count; twColumn_His++)
                                {
                                    RowTW_History[twColumn_His] = Row_History[twColumn_His];
                                }

                                Adt_TitleMissionTW.Update(RowTW);
                                Adt_TitleMissionTW_History.Update(RowTW_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/



                                int nCurrent = (int)Adt_TitleMissionTW.GetMaxIndexRow();
                                if (nBefore != nCurrent)
                                {
                                    Adpt_Trans.UpdateIndexKey(nCurrent, nBefore, "TitleMission");

                                }
                            }

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblTitleMissionHK.Rows.Add();
                            tblTitleMissionHK_History.Rows.Add();
                            DataRow RowHK_History = tblTitleMissionHK_History.Rows[tblTitleMissionHK_History.Rows.Count - 1];
                                                        

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            
                            if (nRowIndex == -1)
                            {
                                //tblTitleMission.Clear();
                                //Adpt_TitleMission.Fill(tblTitleMission);
                                //Row = tblTitleMission.Rows[tblTitleMission.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_TitleMission.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblTitleMissionHK.Columns.Count; nTemp++)
                                {
                                    tblTitleMissionHK.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowHK = null;// = tblTitleMissionHK.Rows[tblTitleMissionHK.Rows.Count - 1];
                                tblTitleMissionHK.Rows.Add();
                                RowHK = tblTitleMissionHK.Rows[tblTitleMissionHK.Rows.Count - 1];

                                for (int HKColumn = 0; HKColumn < tblTitleMissionHK.Columns.Count; HKColumn++)
                                {
                                    /*if (tblTitleMissionHK.Columns[HKColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "TitleMission";
                                RowTrans["Column"] = "Mission_Name";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleMissionHK.Rows[tblTitleMissionHK.Rows.Count - 1]["Mission_Name"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int HKColumn_His = 0; HKColumn_His < tblTitleMissionHK_History.Columns.Count; HKColumn_His++)
                                {
                                    RowHK_History[HKColumn_His] = Row_History[HKColumn_His];
                                }

                                Adt_TitleMissionHK.Update(RowHK);
                                Adt_TitleMissionHK_History.Update(RowHK_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/

                            }

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblTitleMissionJP.Rows.Add();
                            tblTitleMissionJP_History.Rows.Add();
                            DataRow RowJP_History = tblTitleMissionJP_History.Rows[tblTitleMissionJP_History.Rows.Count - 1];


                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도

                            if (nRowIndex == -1)
                            {
                                //tblTitleMission.Clear();
                                //Adpt_TitleMission.Fill(tblTitleMission);
                                //Row = tblTitleMission.Rows[tblTitleMission.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_TitleMission.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblTitleMissionJP.Columns.Count; nTemp++)
                                {
                                    tblTitleMissionJP.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowJP = null;// = tblTitleMissionJP.Rows[tblTitleMissionJP.Rows.Count - 1];
                                tblTitleMissionJP.Rows.Add();
                                RowJP = tblTitleMissionJP.Rows[tblTitleMissionJP.Rows.Count - 1];

                                for (int JPColumn = 0; JPColumn < tblTitleMissionJP.Columns.Count; JPColumn++)
                                {
                                    /*if (tblTitleMissionJP.Columns[JPColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "TitleMission";
                                RowTrans["Column"] = "Mission_Name";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblTitleMissionJP.Rows[tblTitleMissionJP.Rows.Count - 1]["Mission_Name"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int JPColumn_His = 0; JPColumn_His < tblTitleMissionJP_History.Columns.Count; JPColumn_His++)
                                {
                                    RowJP_History[JPColumn_His] = Row_History[JPColumn_His];
                                }

                                Adt_TitleMissionJP.Update(RowJP);
                                Adt_TitleMissionJP_History.Update(RowJP_History);


                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/

                            } 
#endif //__COUNTRY_KR__

                        }
                        break;
                    case 2:
                        {
                            
                            tblSubTitleMission_History.Rows.Add();

                            Row_History = tblSubTitleMission_History.Rows[tblSubTitleMission_History.Rows.Count - 1];
                            //09. 06. 01 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey값을 이용. tblItem은 IndexKey값이 로우와 같으므로 
                            for (int nRow = 0; nRow < tblSubTitleMission.Rows.Count; nRow++)
                            {
                                if (tblSubTitleMission.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblSubTitleMission.Rows[nRow]["IndexKey"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblSubTitleMission.Rows[nRow];
                                    break;
                                }
                            }

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSubTitleMission.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];

                            
                            Adpt_SubTitleMission_History.Update(new DataRow[] { Row_History });
                            Adpt_SubTitleMission.Update(new DataRow[] { Row });


                            //09. 06. 19 김정협
                            //국가별 #define처리 해 줄 것
                            // 이부분은 한국일경우만 실행하게 함. 한국에서 추가하면 모두 추가됨
#if __COUNTRY_KR__

                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            
                            //tblSubTitleMissionTW.Rows.Add();
                            tblSubTitleMissionTW_History.Rows.Add();
                            DataRow RowTW_History = tblSubTitleMissionTW_History.Rows[tblSubTitleMissionTW_History.Rows.Count - 1];

                            int nRowIndex = -1;
                            for (int twRow = 0; twRow < tblSubTitleMissionTW.Rows.Count; twRow++)
                            {
                                if (tblSubTitleMissionTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    nRowIndex = twRow;
                                    break;
                                }
                            }

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            int nBefore = Convert.ToInt32(Row["IndexKey"].ToString());
                            if (nRowIndex == -1)
                            {
                                //tblSubTitleMission.Clear();
                                //Adpt_SubTitleMission.Fill(tblTitleMission);
                                //Row = tblSubTitleMission.Rows[tblSubTitleMission.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SubTitleMission.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSubTitleMissionTW.Columns.Count; nTemp++)
                                {
                                    tblSubTitleMissionTW.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowTW = null;// = tblSubTitleMissionTW.Rows[tblSubTitleMissionTW.Rows.Count - 1];
                                tblSubTitleMissionTW.Rows.Add();
                                RowTW = tblSubTitleMissionTW.Rows[tblSubTitleMissionTW.Rows.Count - 1];

                                for (int twColumn = 0; twColumn < tblSubTitleMissionTW.Columns.Count; twColumn++)
                                {
                                    /*if (tblSubTitleMissionTW.Columns[twColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "SubTitleMission";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString()); 
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSubTitleMissionTW.Rows[tblSubTitleMissionTW.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int twColumn_His = 0; twColumn_His < tblSubTitleMissionTW_History.Columns.Count; twColumn_His++)
                                {
                                    RowTW_History[twColumn_His] = Row_History[twColumn_His];
                                }

                                Adt_SubTitleMissionTW.Update(RowTW);
                                Adt_SubTitleMissionTW_History.Update(RowTW_History);

                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/



                                int nCurrent = (int)Adt_SubTitleMissionTW.GetMaxIndexRow();
                                if (nBefore != nCurrent)
                                {
                                    Adpt_Trans.UpdateIndexKey(nCurrent, nBefore, "SubTitleMission");

                                }
                                
                            }

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblSubTitleMissionHK.Rows.Add();
                            tblSubTitleMissionHK_History.Rows.Add();
                            DataRow RowHK_History = tblSubTitleMissionHK_History.Rows[tblSubTitleMissionHK_History.Rows.Count - 1];

                            

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            
                            if (nRowIndex == -1)
                            {
                                //tblSubTitleMission.Clear();
                                //Adpt_SubTitleMission.Fill(tblTitleMission);
                                //Row = tblSubTitleMission.Rows[tblSubTitleMission.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SubTitleMission.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSubTitleMissionHK.Columns.Count; nTemp++)
                                {
                                    tblSubTitleMissionHK.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowHK = null;// = tblSubTitleMissionHK.Rows[tblSubTitleMissionHK.Rows.Count - 1];
                                tblSubTitleMissionHK.Rows.Add();
                                RowHK = tblSubTitleMissionHK.Rows[tblSubTitleMissionHK.Rows.Count - 1];

                                for (int HKColumn = 0; HKColumn < tblSubTitleMissionHK.Columns.Count; HKColumn++)
                                {
                                    /*if (tblSubTitleMissionHK.Columns[HKColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "SubTitleMission";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSubTitleMissionHK.Rows[tblSubTitleMissionHK.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int HKColumn_His = 0; HKColumn_His < tblSubTitleMissionHK_History.Columns.Count; HKColumn_His++)
                                {
                                    RowHK_History[HKColumn_His] = Row_History[HKColumn_His];
                                }

                                Adt_SubTitleMissionHK.Update(RowHK);
                                Adt_SubTitleMissionHK_History.Update(RowHK_History);

                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/

                            }


                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblSubTitleMissionJP.Rows.Add();
                            tblSubTitleMissionJP_History.Rows.Add();
                            DataRow RowJP_History = tblSubTitleMissionJP_History.Rows[tblSubTitleMissionJP_History.Rows.Count - 1];



                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도

                            if (nRowIndex == -1)
                            {
                                //tblSubTitleMission.Clear();
                                //Adpt_SubTitleMission.Fill(tblTitleMission);
                                //Row = tblSubTitleMission.Rows[tblSubTitleMission.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SubTitleMission.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSubTitleMissionJP.Columns.Count; nTemp++)
                                {
                                    tblSubTitleMissionJP.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowJP = null;// = tblSubTitleMissionJP.Rows[tblSubTitleMissionJP.Rows.Count - 1];
                                tblSubTitleMissionJP.Rows.Add();
                                RowJP = tblSubTitleMissionJP.Rows[tblSubTitleMissionJP.Rows.Count - 1];

                                for (int JPColumn = 0; JPColumn < tblSubTitleMissionJP.Columns.Count; JPColumn++)
                                {
                                    /*if (tblSubTitleMissionJP.Columns[JPColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "SubTitleMission";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSubTitleMissionJP.Rows[tblSubTitleMissionJP.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int JPColumn_His = 0; JPColumn_His < tblSubTitleMissionJP_History.Columns.Count; JPColumn_His++)
                                {
                                    RowJP_History[JPColumn_His] = Row_History[JPColumn_His];
                                }

                                Adt_SubTitleMissionJP.Update(RowJP);
                                Adt_SubTitleMissionJP_History.Update(RowJP_History);

                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/

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

                            tblTitle_DeleteHistory.Rows.Add();

                            Row_History = tblTitle_DeleteHistory.Rows[tblTitle_DeleteHistory.Rows.Count - 1];

                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblTitle.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            Row_History["Regdate"] = DateTime.Now;


                            Adpt_TitleTable.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                            Adpt_TitleTable_DeleteHistory.Update(new DataRow[] { Row_History });


                            //09. 06. 19 김정협
                            //외국쪽 지우는 것
                            //
                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drTW = null;

                            DataRow drTW_DeleteHistory = null;

                            tblTitleTW_DeleteHistory.Rows.Add();
                            drTW_DeleteHistory = tblTitleTW_DeleteHistory.Rows[tblTitleTW_DeleteHistory.Rows.Count - 1];

                            for (int twRow = 0; twRow < tblTitleTW.Rows.Count; twRow++)
                            {
                                if (tblTitleTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drTW = tblTitleTW.Rows[twRow];
                                    break;
                                }
                            }

                            for (int twColumn = 0; twColumn < tblTitleTW.Columns.Count - 1; twColumn++)
                            {
                                drTW_DeleteHistory[twColumn] = drTW[twColumn];
                            }
                            drTW_DeleteHistory["UserID"] = m_strID;
                            drTW_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drTW_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_TitleTableTW_DeleteHistory.Update(drTW_DeleteHistory);

                            int nDeleteIndex = Convert.ToInt32(drTW["IndexKey"].ToString());

                            drTW.Delete();
                            Adt_TitleTableTW.Update(drTW);

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drHK = null;

                            DataRow drHK_DeleteHistory = null;

                            tblTitleHK_DeleteHistory.Rows.Add();
                            drHK_DeleteHistory = tblTitleHK_DeleteHistory.Rows[tblTitleHK_DeleteHistory.Rows.Count - 1];

                            for (int HKRow = 0; HKRow < tblTitleHK.Rows.Count; HKRow++)
                            {
                                if (tblTitleHK.Rows[HKRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drHK = tblTitleHK.Rows[HKRow];
                                    break;
                                }
                            }

                            for (int HKColumn = 0; HKColumn < tblTitleHK.Columns.Count - 1; HKColumn++)
                            {
                                drHK_DeleteHistory[HKColumn] = drHK[HKColumn];
                            }
                            drHK_DeleteHistory["UserID"] = m_strID;
                            drHK_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drHK_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_TitleTableHK_DeleteHistory.Update(drHK_DeleteHistory);

                            
                            drHK.Delete();
                            Adt_TitleTableHK.Update(drHK);

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drJP = null;

                            DataRow drJP_DeleteHistory = null;

                            tblTitleJP_DeleteHistory.Rows.Add();
                            drJP_DeleteHistory = tblTitleJP_DeleteHistory.Rows[tblTitleJP_DeleteHistory.Rows.Count - 1];

                            for (int JPRow = 0; JPRow < tblTitleJP.Rows.Count; JPRow++)
                            {
                                if (tblTitleJP.Rows[JPRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drJP = tblTitleJP.Rows[JPRow];
                                    break;
                                }
                            }

                            for (int JPColumn = 0; JPColumn < tblTitleJP.Columns.Count - 1; JPColumn++)
                            {
                                drJP_DeleteHistory[JPColumn] = drJP[JPColumn];
                            }
                            drJP_DeleteHistory["UserID"] = m_strID;
                            drJP_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drJP_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_TitleTableJP_DeleteHistory.Update(drJP_DeleteHistory);



                            drJP.Delete();
                            Adt_TitleTableJP.Update(drJP);


                            Adpt_Trans.DeleteIndexRow(nDeleteIndex, "TitleTable");

                            Adpt_Release.DeleteIndexRow(nDeleteIndex, "TitleTable");
                        }
                        break;
                    case 1:
                        {

                            tblTitleMission_DeleteHistory.Rows.Add();

                            Row_History = tblTitleMission_DeleteHistory.Rows[tblTitleMission_DeleteHistory.Rows.Count - 1];

                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblTitleMission.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];

                            Adpt_TitleMission.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                            Adpt_TitleMission_DeleteHistory.Update(new DataRow[] { Row_History });


                            //09. 06. 19 김정협
                            //외국쪽 지우는 것
                            //
                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            
                            DataRow drTW = null;

                            DataRow drTW_DeleteHistory = null;

                            tblTitleMissionTW_DeleteHistory.Rows.Add();
                            drTW_DeleteHistory = tblTitleMissionTW_DeleteHistory.Rows[tblTitleMissionTW_DeleteHistory.Rows.Count - 1];

                            for (int twRow = 0; twRow < tblTitleMissionTW.Rows.Count; twRow++)
                            {
                                if (tblTitleMissionTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drTW = tblTitleMissionTW.Rows[twRow];
                                    break;
                                }
                            }

                            for (int twColumn = 0; twColumn < tblTitleMissionTW.Columns.Count - 1; twColumn++)
                            {
                                drTW_DeleteHistory[twColumn] = drTW[twColumn];
                            }
                            drTW_DeleteHistory["UserID"] = m_strID;
                            drTW_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drTW_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_TitleMissionTW_DeleteHistory.Update(drTW_DeleteHistory);

                            int nDeleteIndex = Convert.ToInt32(drTW["IndexKey"].ToString());

                            drTW.Delete();
                            Adt_TitleMissionTW.Update(drTW);

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drHK = null;

                            DataRow drHK_DeleteHistory = null;

                            tblTitleMissionHK_DeleteHistory.Rows.Add();
                            drHK_DeleteHistory = tblTitleMissionHK_DeleteHistory.Rows[tblTitleMissionHK_DeleteHistory.Rows.Count - 1];

                            for (int HKRow = 0; HKRow < tblTitleMissionHK.Rows.Count; HKRow++)
                            {
                                if (tblTitleMissionHK.Rows[HKRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drHK = tblTitleMissionHK.Rows[HKRow];
                                    break;
                                }
                            }

                            for (int HKColumn = 0; HKColumn < tblTitleMissionHK.Columns.Count - 1; HKColumn++)
                            {
                                drHK_DeleteHistory[HKColumn] = drHK[HKColumn];
                            }
                            drHK_DeleteHistory["UserID"] = m_strID;
                            drHK_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drHK_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_TitleMissionHK_DeleteHistory.Update(drHK_DeleteHistory);

                            

                            drHK.Delete();
                            Adt_TitleMissionHK.Update(drHK);

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drJP = null;

                            DataRow drJP_DeleteHistory = null;

                            tblTitleMissionJP_DeleteHistory.Rows.Add();
                            drJP_DeleteHistory = tblTitleMissionJP_DeleteHistory.Rows[tblTitleMissionJP_DeleteHistory.Rows.Count - 1];

                            for (int JPRow = 0; JPRow < tblTitleMissionJP.Rows.Count; JPRow++)
                            {
                                if (tblTitleMissionJP.Rows[JPRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drJP = tblTitleMissionJP.Rows[JPRow];
                                    break;
                                }
                            }

                            for (int JPColumn = 0; JPColumn < tblTitleMissionJP.Columns.Count - 1; JPColumn++)
                            {
                                drJP_DeleteHistory[JPColumn] = drJP[JPColumn];
                            }
                            drJP_DeleteHistory["UserID"] = m_strID;
                            drJP_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drJP_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_TitleMissionJP_DeleteHistory.Update(drJP_DeleteHistory);



                            drJP.Delete();
                            Adt_TitleMissionJP.Update(drJP);

                            Adpt_Trans.DeleteIndexRow(nDeleteIndex, "TitleMission");

                        }
                        break;

                    case 2:
                        {

                            tblSubTitleMission_DeleteHistory.Rows.Add();

                            Row_History = tblSubTitleMission_DeleteHistory.Rows[tblSubTitleMission_DeleteHistory.Rows.Count - 1];

                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSubTitleMission.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];

                            Adpt_SubTitleMission.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                            Adpt_SubTitleMission_DeleteHistory.Update(new DataRow[] { Row_History });


                            //09. 06. 19 김정협
                            //외국쪽 지우는 것
                            //
                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drTW = null;

                            DataRow drTW_DeleteHistory = null;
                            

                            tblSubTitleMissionTW_DeleteHistory.Rows.Add();
                            drTW_DeleteHistory = tblSubTitleMissionTW_DeleteHistory.Rows[tblSubTitleMissionTW_DeleteHistory.Rows.Count - 1];

                            for (int twRow = 0; twRow < tblSubTitleMissionTW.Rows.Count; twRow++)
                            {
                                if (tblSubTitleMissionTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drTW = tblSubTitleMissionTW.Rows[twRow];
                                    break;
                                }
                            }

                            for (int twColumn = 0; twColumn < tblSubTitleMissionTW.Columns.Count - 1; twColumn++)
                            {
                                drTW_DeleteHistory[twColumn] = drTW[twColumn];
                            }
                            drTW_DeleteHistory["UserID"] = m_strID;
                            drTW_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drTW_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SubTitleMissionTW_DeleteHistory.Update(drTW_DeleteHistory);

                            int nDeleteIndex = Convert.ToInt32(drTW["IndexKey"].ToString());

                            drTW.Delete();
                            Adt_SubTitleMissionTW.Update(drTW);

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drHK = null;

                            DataRow drHK_DeleteHistory = null;


                            tblSubTitleMissionHK_DeleteHistory.Rows.Add();
                            drHK_DeleteHistory = tblSubTitleMissionHK_DeleteHistory.Rows[tblSubTitleMissionHK_DeleteHistory.Rows.Count - 1];

                            for (int HKRow = 0; HKRow < tblSubTitleMissionHK.Rows.Count; HKRow++)
                            {
                                if (tblSubTitleMissionHK.Rows[HKRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drHK = tblSubTitleMissionHK.Rows[HKRow];
                                    break;
                                }
                            }

                            for (int HKColumn = 0; HKColumn < tblSubTitleMissionHK.Columns.Count - 1; HKColumn++)
                            {
                                drHK_DeleteHistory[HKColumn] = drHK[HKColumn];
                            }
                            drHK_DeleteHistory["UserID"] = m_strID;
                            drHK_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drHK_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SubTitleMissionHK_DeleteHistory.Update(drHK_DeleteHistory);

                            

                            drHK.Delete();
                            Adt_SubTitleMissionHK.Update(drHK);

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drJP = null;

                            DataRow drJP_DeleteHistory = null;


                            tblSubTitleMissionJP_DeleteHistory.Rows.Add();
                            drJP_DeleteHistory = tblSubTitleMissionJP_DeleteHistory.Rows[tblSubTitleMissionJP_DeleteHistory.Rows.Count - 1];

                            for (int JPRow = 0; JPRow < tblSubTitleMissionJP.Rows.Count; JPRow++)
                            {
                                if (tblSubTitleMissionJP.Rows[JPRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drJP = tblSubTitleMissionJP.Rows[JPRow];
                                    break;
                                }
                            }

                            for (int JPColumn = 0; JPColumn < tblSubTitleMissionJP.Columns.Count - 1; JPColumn++)
                            {
                                drJP_DeleteHistory[JPColumn] = drJP[JPColumn];
                            }
                            drJP_DeleteHistory["UserID"] = m_strID;
                            drJP_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drJP_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SubTitleMissionJP_DeleteHistory.Update(drJP_DeleteHistory);



                            drJP.Delete();
                            Adt_SubTitleMissionJP.Update(drJP);

                            Adpt_Trans.DeleteIndexRow(nDeleteIndex, "SubTitleMission");
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

            tblTitle.Clear();
            Adpt_TitleTable.Fill(tblTitle);
            tblTitleMission.Clear();
            Adpt_TitleMission.Fill(tblTitleMission);
            tblSubTitleMission.Clear();
            Adpt_SubTitleMission.Fill(tblSubTitleMission);

            SetCellBackgroundColorForDB(-1);

            m_ChangeCellList.Clear();
            m_DeleteRowList.Clear();
            m_ModifyIDList.Clear();
            m_UndoStack.Clear();
            m_RedoStack.Clear();
            m_bSaveEnd = true;

            

            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["TitleID", nRowHeader].Value.ToString() + "     " + dataGridView1["Title_Name", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 1;

            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
            {
                dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["Mission_Name", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 2;
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
            for (int nRowHeader = 0; nRowHeader < dataGridView3.RowCount; nRowHeader++)
            {
                dataGridView3.Rows[nRowHeader].HeaderCell.Value = dataGridView3["ID", nRowHeader].Value.ToString() + "     " + dataGridView3["Description", nRowHeader].Value.ToString();
            }

            //09. 07. 01 김정협 현재 위치 기억
            tabControl1.SelectedIndex = nCurrentPosTabIndex;
            switch (nCurrentPosTabIndex)
            {
                case 0:
                    dataGridView1.CurrentCell = dataGridView1[nCurrentPosColumnIndex, nCurrentPosRowIndex];
                    break;
                case 1:
                    dataGridView2.CurrentCell = dataGridView2[nCurrentPosColumnIndex, nCurrentPosRowIndex];
                    break;
                case 2:
                    dataGridView3.CurrentCell = dataGridView3[nCurrentPosColumnIndex, nCurrentPosRowIndex];
                    break;
            }
        }

        TitleTableAdd frmTitleTable = new TitleTableAdd();
        TitleMissionAdd frmTitleMission = new TitleMissionAdd();
        SubTitleMissionAdd frmSubTitleMission = new SubTitleMissionAdd();

        private void btnAdd_Click(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        if (!frmTitleTable.Visible)
                        {
                            frmTitleTable = new TitleTableAdd();
                        }
                        frmTitleTable.SetDataGridView(dataGridView1);
                        frmTitleTable.SetRowIndex(dataGridView1.CurrentCell.RowIndex);
                        frmTitleTable.SetDataTable(tblTitle);
                        frmTitleTable.SetModifyArray(m_ModifyIDList);
                        frmTitleTable.SetTrans(tblTrans);
                        frmTitleTable.SetRelease(tblRelease);
                        frmTitleTable.SetUndoStack(m_UndoStack);
                        frmTitleTable.SetRedoStack(m_RedoStack);

                        frmTitleTable.SetTitleType(tblTitleType);
                        frmTitleTable.SetUnitType_Title(tblUnitType_Title);
                        frmTitleTable.SetUnitClass_Title(tblUnitClass_Title);

                        frmTitleTable.Owner = this;
                        if (!frmTitleTable.Visible)
                        {
                            //frm.SetTableNew();
                            frmTitleTable.Show(this);
                        }
                        else
                        {
                            frmTitleTable.ADDItem();// ItemAdd_Load(null, null);
                        }
                    }
                    break;
                   
                case 1:
                    {
                        if (!frmTitleMission.Visible)
                        {
                            frmTitleMission = new TitleMissionAdd();
                        }
                        frmTitleMission.SetDataGridView(dataGridView2);
                        frmTitleMission.SetRowIndex(dataGridView2.CurrentCell.RowIndex);
                        frmTitleMission.SetDataTable(tblTitleMission);
                        frmTitleMission.SetModifyArray(m_ModifyIDList);
                        frmTitleMission.SetTrans(tblTrans);
                        frmTitleMission.SetUndoStack(m_UndoStack);
                        frmTitleMission.SetRedoStack(m_RedoStack);

                        frmTitleMission.SetMissionClearType(tblMissionClearType);
                        frmTitleMission.SetTitleTable(tblTitle);
                        frmTitleMission.SetUnitType_Title(tblUnitType_Title);
                        frmTitleMission.SetUnitClass_Title(tblUnitClass_Title);

                        frmTitleMission.Owner = this;
                        if (!frmTitleMission.Visible)
                        {
                            //frm.SetTableNew();
                            frmTitleMission.Show(this);
                        }
                        else
                        {
                            frmTitleMission.ADDItem();// ItemAdd_Load(null, null);
                        }

                    }
                    break;
                case 2:
                    {
                        if (!frmSubTitleMission.Visible)
                        {
                            frmSubTitleMission = new SubTitleMissionAdd();
                        }
                        frmSubTitleMission.SetDataGridView(dataGridView3);
                        frmSubTitleMission.SetRowIndex(dataGridView3.CurrentCell.RowIndex);
                        frmSubTitleMission.SetDataTable(tblSubTitleMission);
                        frmSubTitleMission.SetModifyArray(m_ModifyIDList);
                        frmSubTitleMission.SetTrans(tblTrans);
                        frmSubTitleMission.SetUndoStack(m_UndoStack);
                        frmSubTitleMission.SetRedoStack(m_RedoStack);

                        frmSubTitleMission.SetMissionClearType(tblMissionClearType);
                        frmSubTitleMission.SetNPCID(tblNpcID);
                        frmSubTitleMission.SetItem(tblItem);
                        frmSubTitleMission.Owner = this;
                        if (!frmSubTitleMission.Visible)
                        {
                            //frm.SetTableNew();
                            frmSubTitleMission.Show(this);
                        }
                        else
                        {
                            frmSubTitleMission.ADDItem();// ItemAdd_Load(null, null);
                        }

                    }
                    break;
            }
        }

        public void SetCellBackgroundColorForDB(int nSelect)
        {
            //09. 06. 03 김정협 추가 : 셀배경색 DB로딩함
            for (int i = 0; i < tblColor.Rows.Count; i++)
            {
                if(tblColor.Rows[i].RowState == DataRowState.Deleted)
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

                if (drTemp["TableName"].ToString() == "TitleTable" && (nSelect == 0 || nSelect == -1))
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
                    
                }
                else if (drTemp["TableName"].ToString() == "TitleMission" && (nSelect == 1 || nSelect == -1))
                {
                    for (int j = 0; j < dataGridView2.RowCount; j++)
                    {
                        if (dataGridView2["IndexKey", j].Value.ToString() == drTemp["IndexNum"].ToString())
                        {
                            nRow = dataGridView2["IndexKey", j].RowIndex;
                            break;
                        }
                    }
                    if (nRow != -1)
                    {
                        tabControl1.SelectedIndex = 1;
                        dataGridView2[nColumn, nRow].Style.BackColor = Color.FromArgb(nARGB);
                    }
                    else
                    {
                        tblColor.Rows[i].Delete();
                        //Adpt_ColorTable.Update(tblColor);
                    }
                    
                }
                else if (drTemp["TableName"].ToString() == "SubTitleMission" && (nSelect == 2 || nSelect == -1))
                {
                    for (int j = 0; j < dataGridView3.RowCount; j++)
                    {
                        if (dataGridView3["IndexKey", j].Value.ToString() == drTemp["IndexNum"].ToString())
                        {
                            nRow = dataGridView3["IndexKey", j].RowIndex;
                            break;
                        }
                    }
                    if (nRow != -1)
                    {
                        tabControl1.SelectedIndex = 2;
                        dataGridView3[nColumn, nRow].Style.BackColor = Color.FromArgb(nARGB);
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

        private void TitleTable_Load(object sender, EventArgs e)
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
            dataGridView2.ContextMenuStrip = contextMenuStrip1;
            dataGridView3.ContextMenuStrip = contextMenuStrip1;

            //TitleTable
            Adpt_TitleTable = new SqlDataAdapter("SELECT * FROM TitleTable", Con);
            tblTitle = new DataTable("tblTitle");

            SqlCommandBuilder Builder_Title = new SqlCommandBuilder(Adpt_TitleTable);
            Adpt_TitleTable.Fill(tblTitle);
            dataGridView1.DataSource = tblTitle;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView1.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView1.Columns["Regdate"].ReadOnly = true; //Reg_data수정방지

            dataGridView1.Columns["Title_Type_"].ReadOnly = true; //수정방지            
            dataGridView1.Columns["Condition_Unit_Type_"].ReadOnly = true; //수정방지
            dataGridView1.Columns["Condition_Unit_Class_"].ReadOnly = true; //수정방지
            dataGridView1.Columns["Title_Type_"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["Condition_Unit_Type_"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["Condition_Unit_Class_"].DefaultCellStyle.BackColor = Color.LightGray;

            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }
            DataGridViewColumn Column_1 = dataGridView1.Columns[1];//dataGridView1.Columns["ID"];
            ListSortDirection direction_1 = ListSortDirection.Ascending;
            dataGridView1.Sort(Column_1, direction_1);

            Adpt_TitleTable_History = new SqlDataAdapter("SELECT * FROM TitleTable_History", Con);
            tblTitle_History = new DataTable("tblTitle_History");
            SqlCommandBuilder Builder_Title_History = new SqlCommandBuilder(Adpt_TitleTable_History);
            Adpt_TitleTable_History.Fill(tblTitle_History);

            Adpt_TitleTable_DeleteHistory = new SqlDataAdapter("SELECT * FROM TitleTable_DeleteHistory", Con);
            tblTitle_DeleteHistory = new DataTable("tblTitle_DeleteHistory");
            SqlCommandBuilder Builder_TitleTable_Delete = new SqlCommandBuilder(Adpt_TitleTable_DeleteHistory);
            Adpt_TitleTable_DeleteHistory.Fill(tblTitle_DeleteHistory);

            //09. 06. 19 김정협 Trans/Release 로드
            //Trans
            //Adpt_String = new SqlDataAdapter("SELECT * FROM ScriptStringTable", Con);
            tblTrans = Adpt_Trans.GetData();
            tblTrans_History = Adpt_Trans_History.GetData();
            //Release
            tblRelease = Adpt_Release.GetData();
            tblRelease_History = Adpt_Release_History.GetData();


            //TitleMission
            Adpt_TitleMission = new SqlDataAdapter("SELECT * FROM TitleMission", Con);
            tblTitleMission = new DataTable("tblTitleMission");

            SqlCommandBuilder Builder_TitleMission = new SqlCommandBuilder(Adpt_TitleMission);
            Adpt_TitleMission.Fill(tblTitleMission);
            dataGridView2.DataSource = tblTitleMission;
            dataGridView2.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView2.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView2.Columns["Regdate"].ReadOnly = true; //Reg_data수정방지

            dataGridView2.Columns["Mission_Clear_Type_"].ReadOnly = true; //수정방지
            dataGridView2.Columns["Mission_Name"].ReadOnly = true; //수정방지
            dataGridView2.Columns["Condition_Unit_Type_"].ReadOnly = true; //수정방지
            dataGridView2.Columns["Condition_Unit_Class_"].ReadOnly = true; //수정방지
            dataGridView2.Columns["Title Name"].ReadOnly = true; //수정방지

            dataGridView2.Columns["Mission_Clear_Type_"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView2.Columns["Mission_Name"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView2.Columns["Condition_Unit_Type_"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView2.Columns["Condition_Unit_Class_"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView2.Columns["Title Name"].DefaultCellStyle.BackColor = Color.LightGray;

            for (int i = 0; i < dataGridView2.ColumnCount; i++)
            {
                dataGridView2.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }

            //09. 06. 01 김정협 수정 : 아이디로 오름차순 정렬
            DataGridViewColumn Column_2 = dataGridView2.Columns[1];//dataGridView1.Columns["ID"];
            ListSortDirection direction_2 = ListSortDirection.Ascending;
            dataGridView2.Sort(Column_2, direction_2);

            Adpt_TitleMission_History = new SqlDataAdapter("SELECT * FROM TitleMission_History", Con);
            tblTitleMission_History = new DataTable("tblTitleMission_History");
            SqlCommandBuilder Builder_TitleMission_History = new SqlCommandBuilder(Adpt_TitleMission_History);
            Adpt_TitleMission_History.Fill(tblTitleMission_History);

            Adpt_TitleMission_DeleteHistory = new SqlDataAdapter("SELECT * FROM TitleMission_DeleteHistory", Con);
            tblTitleMission_DeleteHistory = new DataTable("tblTitleMission_DeleteHistory");
            SqlCommandBuilder Builder_TitleMission_Delete = new SqlCommandBuilder(Adpt_TitleMission_DeleteHistory);
            Adpt_TitleMission_DeleteHistory.Fill(tblTitleMission_DeleteHistory);


            //SubTitleMission
            Adpt_SubTitleMission = new SqlDataAdapter("SELECT * FROM SubTitleMission", Con);
            tblSubTitleMission = new DataTable("tblSubTitleMission");
            SqlCommandBuilder Builder_SubTitleMission = new SqlCommandBuilder(Adpt_SubTitleMission);
            Adpt_SubTitleMission.Fill(tblSubTitleMission);
            dataGridView3.DataSource = tblSubTitleMission;
            dataGridView3.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView3.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView3.Columns["Regdate"].ReadOnly = true; //대리키수정방지

            dataGridView3.Columns["Clear_Type_"].ReadOnly = true; //대리키수정방지
            dataGridView3.Columns["Clear Condition Kill NPC ID"].ReadOnly = true; //대리키수정방지
            dataGridView3.Columns["Name"].ReadOnly = true; //대리키수정방지

            dataGridView3.Columns["Clear_Type_"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView3.Columns["Clear Condition Kill NPC ID"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView3.Columns["Name"].DefaultCellStyle.BackColor = Color.LightGray;

            for (int i = 0; i < dataGridView3.ColumnCount; i++)
            {
                dataGridView3.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }
            //09. 06. 01 김정협 수정 : 아이디로 오름차순 정렬
            DataGridViewColumn Column_3 = dataGridView3.Columns[1];//dataGridView1.Columns["ID"];
            ListSortDirection direction_3 = ListSortDirection.Ascending;
            dataGridView3.Sort(Column_3, direction_3);

            //SubTitleMission_History
            Adpt_SubTitleMission_History = new SqlDataAdapter("SELECT * FROM SubTitleMission_History", Con);
            tblSubTitleMission_History = new DataTable("tblSubTitleMission_History");
            SqlCommandBuilder Builder_SubTitleMission_History = new SqlCommandBuilder(Adpt_SubTitleMission_History);
            Adpt_SubTitleMission_History.Fill(tblSubTitleMission_History);

            Adpt_SubTitleMission_DeleteHistory = new SqlDataAdapter("SELECT * FROM SubTitleMission_DeleteHistory", Con);
            tblSubTitleMission_DeleteHistory = new DataTable("tblSubTitleMission_DeleteHistory");
            SqlCommandBuilder Builder_SubTitleMission_Delete = new SqlCommandBuilder(Adpt_SubTitleMission_DeleteHistory);
            Adpt_SubTitleMission_DeleteHistory.Fill(tblSubTitleMission_DeleteHistory);

            //TitleType용        
            Adpt_TitleType = new SqlDataAdapter("SELECT * FROM TITLE_TYPE", Con);
            tblTitleType = new DataTable("tblTitleType");
            Adpt_TitleType.Fill(tblTitleType);
            tblTitleType.PrimaryKey = new DataColumn[] { tblTitleType.Columns["ID"] };

            //MissionClearType용        
            Adpt_MissionClearType = new SqlDataAdapter("SELECT * FROM TITLE_MISSION_CLEAR_TYPE", Con);
            tblMissionClearType = new DataTable("tblTitleType");
            Adpt_MissionClearType.Fill(tblMissionClearType);
            tblMissionClearType.PrimaryKey = new DataColumn[] { tblMissionClearType.Columns["ID"] };

            //UnitClass_Title용        
            Adpt_UnitClass_Title = new SqlDataAdapter("SELECT * FROM UNIT_CLASS_Title", Con);
            tblUnitClass_Title = new DataTable("tblTitleType");
            Adpt_UnitClass_Title.Fill(tblUnitClass_Title);
            tblUnitClass_Title.PrimaryKey = new DataColumn[] { tblUnitClass_Title.Columns["ID"] };

            //UnitType_Title용        
            Adpt_UnitType_Title = new SqlDataAdapter("SELECT * FROM UNIT_TYPE_Title", Con);
            tblUnitType_Title = new DataTable("tblTitleType");
            Adpt_UnitType_Title.Fill(tblUnitType_Title);
            tblUnitType_Title.PrimaryKey = new DataColumn[] { tblUnitType_Title.Columns["ID"] };

            //NpcID용        
            Adpt_NpcID = new SqlDataAdapter("SELECT * FROM NPCID", Con);
            tblNpcID = new DataTable("tblTitleType");
            Adpt_NpcID.Fill(tblNpcID);
            tblNpcID.PrimaryKey = new DataColumn[] { tblNpcID.Columns["ID"] };

            //NpcID용        
            Adpt_Item = new SqlDataAdapter("SELECT * FROM Item", Con);
            tblItem = new DataTable("tblItem");
            Adpt_Item.Fill(tblItem);
            tblItem.PrimaryKey = new DataColumn[] { tblItem.Columns["ID"] };


            tblTitle.PrimaryKey = new DataColumn[] { tblTitle.Columns["TitleID"] };



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

            SetCellBackgroundColorForDB(-1);

            dataGridView1.RowHeadersWidth = 130;
            dataGridView1.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;

            dataGridView2.RowHeadersWidth = 130;
            dataGridView2.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;

            dataGridView3.RowHeadersWidth = 130;
            dataGridView3.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;


            

            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {                
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["TitleID", nRowHeader].Value.ToString() + "     " + dataGridView1["Title_Name", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 1;

            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여
            
            for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
            {                
                dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["Mission_Name", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 2;
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
            for (int nRowHeader = 0; nRowHeader < dataGridView3.RowCount; nRowHeader++)
            {                
                dataGridView3.Rows[nRowHeader].HeaderCell.Value = dataGridView3["ID", nRowHeader].Value.ToString() + "     " + dataGridView3["Description", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 0;
        }

        private void TitleTable_Resize(object sender, EventArgs e)
        {
            tabControl1.Width = this.Width - 11;
            tabControl1.Height = this.Height - 136;
            dataGridView1.Width = tabControl1.Width - 13;
            dataGridView1.Height = tabControl1.Height - 29;

            dataGridView2.Width = tabControl1.Width - 13;
            dataGridView2.Height = tabControl1.Height - 29;

            dataGridView3.Width = tabControl1.Width - 13;
            dataGridView3.Height = tabControl1.Height - 29;

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
                frm.SetHistoryTable(tblTitle);
            }
            else if (tabControl1.SelectedIndex == 1)
            {
                frm.SetHistoryTable(tblTitleMission_History);
            }
            else if (tabControl1.SelectedIndex == 2)
            {
                frm.SetHistoryTable(tblSubTitleMission_History);
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
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {                        
                        StreamWriter sw;
                        
                        //국가별 define정의할 것
#if __COUNTRY_KR__
                        string strFileName = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Major\\TitleTable.lua";
                        strCountryCode = "KR";
                        sw = new StreamWriter(strFileName, false, Encoding.Default);
#elif __COUNTRY_TW__
                        string strFileName = "..\\..\\..\\resource\\TW\\Trunk\\dat\\Script\\Major\\TitleTable.lua";
                        strCountryCode = "TW";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
                        
#elif __COUNTRY_HK__
                        string strFileName = "..\\..\\..\\resource\\HK\\Trunk\\dat\\Script\\Major\\TitleTable.lua";
                        strCountryCode = "HK";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
                        
#elif __COUNTRY_JP__
                        string strFileName = "..\\..\\..\\resource\\JP\\Trunk\\dat\\Script\\Major\\TitleTable.lua";
                        strCountryCode = "JP";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
                        
#endif



                        sw.Write("\r\n\r\n\r\n");

                        for (int i = 0; i < dataGridView1.RowCount; i++)
                        {                            
                            //국가별 define정의 해줄 것
#if __COUNTRY_KR__
                            DataRow dr = Adpt_Release.GetDataByRelease("KR", "TitleTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#elif __COUNTRY_TW__
                            DataRow dr = Adpt_Release.GetDataByRelease("TW", "TitleTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#elif __COUNTRY_HK__
                            DataRow dr = Adpt_Release.GetDataByRelease("HK", "TitleTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#elif __COUNTRY_JP__
                            DataRow dr = Adpt_Release.GetDataByRelease("JP", "TitleTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#endif
                            if (dr["State"].ToString() == "False")
                            {
                                continue;
                            }

                            sw.Write("TitleManager:AddTitleInfo_LUA\r\n");
                            sw.Write("{\r\n");

                            sw.Write("    ");
                            sw.Write("m_iTitleID = ");
                            sw.Write(dataGridView1["TitleID", i].Value.ToString());
                            sw.Write(",\r\n");
                            sw.Write("    ");
                            sw.Write("m_iSortNum = ");
                            sw.Write(dataGridView1["Sort_Num", i].Value.ToString());
                            sw.Write(",\r\n"); 
                            sw.Write("    ");
                            sw.Write("m_eTitleType = ");
                            sw.Write(dataGridView1["Title_Type_", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_TitleName = \"");
                            sw.Write(dataGridView1["Title_Name", i].Value.ToString());
                            sw.Write("\",\r\n");
                            sw.Write("    ");
                            sw.Write("m_Description = \"");
                            sw.Write(dataGridView1["Description", i].Value.ToString());
                            sw.Write("\",\r\n");
                            
                            sw.Write("\r\n");
                            
                            sw.Write("    ");
                            sw.Write("m_ParticleName = \"");
                            sw.Write(dataGridView1["Particle_Name", i].Value.ToString());
                            sw.Write("\",\r\n");

                            sw.Write("\r\n");

                            sw.Write("    ");
                            sw.Write("m_eUnitType = ");
                            sw.Write(dataGridView1["Condition_Unit_Type_", i].Value.ToString());
                            sw.Write(",\r\n");
                            sw.Write("    ");
                            sw.Write("m_eUnitClass = ");
                            sw.Write(dataGridView1["Condition_Unit_Class_", i].Value.ToString());
                            sw.Write(",\r\n");
                            sw.Write("    ");
                            sw.Write("m_iOpenLevel = ");
                            sw.Write(dataGridView1["Open_Level", i].Value.ToString());
                            sw.Write(",\r\n");
                            sw.Write("    ");
                            sw.Write("m_bIsSecretTitle = ");
                            sw.Write(dataGridView1["Secret_Title", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("\r\n");

                            sw.Write("    ");
                            sw.Write("m_bVisible = ");
                            sw.Write(dataGridView1["Visible", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("\r\n");

                            if (Convert.ToInt32(dataGridView1["Base_HP", i].Value.ToString()) != 0)
                            {
                                sw.Write("    ");
                                sw.Write("m_iBaseHP = ");
                                sw.Write(dataGridView1["Base_HP", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            if (Convert.ToInt32(dataGridView1["Atk_Physic", i].Value.ToString()) != 0)
                            {
                                sw.Write("    ");
                                sw.Write("m_iAtkPhysic = ");
                                sw.Write(dataGridView1["Atk_Physic", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            if (Convert.ToInt32(dataGridView1["Atk_Magic", i].Value.ToString()) != 0)
                            {
                                sw.Write("    ");
                                sw.Write("m_iAtkMagic = ");
                                sw.Write(dataGridView1["Atk_Magic", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            if (Convert.ToInt32(dataGridView1["Def_Physic", i].Value.ToString()) != 0)
                            {
                                sw.Write("    ");
                                sw.Write("m_iDefPhysic = ");
                                sw.Write(dataGridView1["Def_Physic", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            if (Convert.ToInt32(dataGridView1["Def_Magic", i].Value.ToString()) != 0)
                            {
                                sw.Write("    ");
                                sw.Write("m_iDefMagic = ");
                                sw.Write(dataGridView1["Def_Magic", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            sw.Write("\r\n");

                            sw.Write("    ");
                            sw.Write("SOCKET_OPTION = \r\n");
                            sw.Write("    {\r\n");
                            if (Convert.ToInt32(dataGridView1["Socket01", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket01", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            if (Convert.ToInt32(dataGridView1["Socket02", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket02", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["Socket03", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket03", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["Socket04", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket04", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["Socket05", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket05", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["Socket06", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket06", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["Socket07", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket07", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["Socket08", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket08", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            if (Convert.ToInt32(dataGridView1["Socket09", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket09", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["Socket10", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Socket10", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            sw.Write("    },\r\n");
                            sw.Write("}\r\n\r\n");
                        }

                        sw.Close();
                        
                        Process Dos = new Process();
                        Dos.StartInfo.FileName = "tortoiseProc";
                        Dos.StartInfo.Arguments = "/Command:commit /path:\"..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\TitleTable.lua\" /notempfile /closeonend:1";
                        Dos.Start();
                    }
                    break;
                case 1:
                    {
                        StreamWriter sw;
                        //국가별 define정의할 것
#if __COUNTRY_KR__
                        string strFileName = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Major\\TitleMission.lua";
                        strCountryCode = "KR";
                        sw = new StreamWriter(strFileName, false, Encoding.Default);
#elif __COUNTRY_TW__
                        string strFileName = "..\\..\\..\\resource\\TW\\Trunk\\dat\\Script\\Major\\TitleMission.lua";
                        strCountryCode = "TW";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_HK__
                        string strFileName = "..\\..\\..\\resource\\HK\\Trunk\\dat\\Script\\Major\\TitleMission.lua";
                        strCountryCode = "HK";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_JP__
                        string strFileName = "..\\..\\..\\resource\\JP\\Trunk\\dat\\Script\\Major\\TitleMission.lua";
                        strCountryCode = "JP";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#endif



                        sw.Write("\r\n\r\n\r\n");
                        for (int i = 0; i < dataGridView2.RowCount; i++)
                        {
                            sw.Write("TitleManager:AddTitleMissionInfo_LUA\r\n");
                            sw.Write("{\r\n");

                            sw.Write("    ");
                            sw.Write("m_iMissionID = ");
                            sw.Write(dataGridView2["ID", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_eClearType = ");
                            sw.Write(dataGridView2["Mission_Clear_Type_", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_MissionName = \"");
                            sw.Write(dataGridView2["Mission_Name", i].Value.ToString());
                            sw.Write("\",\r\n");

                            sw.Write("    ");
                            sw.Write("m_iConditionLv = ");
                            sw.Write(dataGridView2["Condition_Lv", i].Value.ToString());
                            sw.Write(",\r\n");
                            sw.Write("    ");
                            sw.Write("m_eConditionUnitType = ");
                            sw.Write(dataGridView2["Condition_Unit_Type_", i].Value.ToString());
                            sw.Write(",\r\n");
                            sw.Write("    ");
                            sw.Write("m_eConditionUnitClass = ");
                            sw.Write(dataGridView2["Condition_Unit_Class_", i].Value.ToString());
                            sw.Write(",\r\n");
                            sw.Write("    ");
                            sw.Write("m_iConditionClearMissionID = ");
                            sw.Write(dataGridView2["Condition_ClearMission_ID", i].Value.ToString());
                            sw.Write(",\r\n\r\n");

                            sw.Write("    ");
                            sw.Write("m_SubMission = \r\n");
                            sw.Write("    {\r\n");

                            if (Convert.ToInt32(dataGridView2["SubMission_1", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView2["SubMission_1", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView2["SubMission_2", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView2["SubMission_2", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView2["SubMission_3", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView2["SubMission_3", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView2["SubMission_4", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView2["SubMission_4", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView2["SubMission_5", i].Value.ToString()) != 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView2["SubMission_5", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            sw.Write("    },\r\n");

                            sw.Write("\r\n");

                            sw.Write("    ");
                            sw.Write("m_sPeriod = ");
                            sw.Write(dataGridView2["Period", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_iTitleID = ");
                            sw.Write(dataGridView2["Reward_Title_ID", i].Value.ToString());
                            sw.Write(",\r\n");


                            sw.Write("}\r\n\r\n");

                        }
                        sw.Close();
                        
                        Process Dos = new Process();
                        Dos.StartInfo.FileName = "tortoiseProc";
                        Dos.StartInfo.Arguments = "/Command:commit /path:\"..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\TitleMission.lua\" /notempfile /closeonend:1";
                        Dos.Start();
                        
                    }
                    break;
                case 2:
                    {
                        StreamWriter sw;
                        //국가별 define정의할 것
#if __COUNTRY_KR__
                        string strFileName = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Major\\SubTitleMission.lua";
                        strCountryCode = "KR";
                        sw = new StreamWriter(strFileName, false, Encoding.Default);
#elif __COUNTRY_TW__
                        string strFileName = "..\\..\\..\\resource\\TW\\Trunk\\dat\\Script\\Major\\SubTitleMission.lua";
                        strCountryCode = "TW";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");

#elif __COUNTRY_HK__
                        string strFileName = "..\\..\\..\\resource\\HK\\Trunk\\dat\\Script\\Major\\SubTitleMission.lua";
                        strCountryCode = "HK";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_JP__
                        string strFileName = "..\\..\\..\\resource\\JP\\Trunk\\dat\\Script\\Major\\SubTitleMission.lua";
                        strCountryCode = "JP";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#endif



                        for (int i = 0; i < dataGridView3.RowCount; i++)
                        {

                            sw.Write("TitleManager:AddSubTitleMissionInfo_LUA\r\n");
                            sw.Write("{\r\n");

                            sw.Write("    ");
                            sw.Write("m_iID = ");
                            sw.Write(dataGridView3["ID", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_wstrDescription = \"");
                            sw.Write(dataGridView3["Description", i].Value.ToString());
                            sw.Write("\",\r\n");


                            sw.Write("    ");
                            sw.Write("m_eClearType = ");
                            sw.Write(dataGridView3["Clear_Type_", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_bAutomaticDescription = ");
                            sw.Write(dataGridView3["AutomaticDescription", i].Value.ToString());
                            sw.Write(",\r\n");


                            sw.Write("    ");
                            sw.Write("m_ClearCondition = \r\n");
                            sw.Write("    {\r\n");

                            if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 1)
                            {
                                sw.Write("        ");
                                sw.Write("m_eTalkNPCID = NPC_UNIT_ID[\"");
                                sw.Write(dataGridView3["Clear_Condition_Talk_NPC_ID", i].Value.ToString());
                                sw.Write("\"],\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 2)
                            {
                                if (dataGridView3["Clear Condition Kill NPC ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eKillNPCID = NPC_UNIT_ID[\"");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eKillNPCID = NPC_UNIT_ID[\"");
                                    sw.Write(dataGridView3["Clear Condition Kill NPC ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }
                                sw.Write("        ");
                                sw.Write("m_iKillNum = ");
                                sw.Write(dataGridView3["Clear Condition Kill NPC Quantity", i].Value.ToString());
                                sw.Write(",\r\n");

                                if (dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write(dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }
                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 3)
                            {
                                sw.Write("        ");
                                sw.Write("m_iCollectionItemID = ");
                                sw.Write(dataGridView3["Clear_Condition_Collection_Item_ID", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_iCollectionItemNum = ");
                                sw.Write(dataGridView3["Clear_Condition_Collection_Item_Quantity", i].Value.ToString());
                                sw.Write(",\r\n");

                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 4)
                            {
                                if (dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write(dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }
                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                if (dataGridView3["Clear Condition Kill NPC ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eKillNPCID = NPC_UNIT_ID[\"");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eKillNPCID = NPC_UNIT_ID[\"");
                                    sw.Write(dataGridView3["Clear Condition Kill NPC ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }

                                sw.Write("        ");
                                sw.Write("m_iCollectionItemID = ");
                                sw.Write(dataGridView3["Clear_Condition_Collection_Item_ID", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_iCollectionItemNum = ");
                                sw.Write(dataGridView3["Clear_Condition_Collection_Item_Quantity", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_fQuestItemDropRate = ");
                                sw.Write(dataGridView3["Quest_Item_Drop_Rate", i].Value.ToString());
                                sw.Write(",\r\n");


                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 5)
                            {
                                if (dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write(dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }
                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");


                                sw.Write("        ");
                                sw.Write("m_iDungeonClearTime = ");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Clear_Time", i].Value.ToString());
                                sw.Write(",\r\n");

                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 6)
                            {
                                if (dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write(dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }
                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");


                                sw.Write("        ");
                                sw.Write("m_eDungeonRank = ");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Rank", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 7)
                            {
                                if (dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write(dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }
                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");


                                sw.Write("        ");
                                sw.Write("m_iDungeonDamage = ");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Damage", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 8)
                            {
                                if (dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write("NONE");
                                    sw.Write("\"],\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eDungeonID = DUNGEON_ID[\"DI_");
                                    sw.Write(dataGridView3["Clear_Condition_Dungeon_ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }
                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");


                                sw.Write("        ");
                                sw.Write("m_iDungeonClearCount = ");
                                sw.Write(dataGridView3["Clear_Condition_Dungeon_clear_count", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 9)
                            {
                                sw.Write("        ");
                                sw.Write("m_ePVPType = PVP_GAME_TYPE[\"PGT_");
                                sw.Write(dataGridView3["Clear_Condition_PVP_TYPE", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_iPVPPlay = ");
                                sw.Write(dataGridView3["Clear_Condition_PVP_PLAY", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 10)
                            {
                                sw.Write("        ");
                                sw.Write("m_ePVPType = PVP_GAME_TYPE[\"PGT_");
                                sw.Write(dataGridView3["Clear_Condition_PVP_TYPE", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_iPVPWin = ");
                                sw.Write(dataGridView3["Clear_Condition_PVP_WIN", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 11)
                            {
                                sw.Write("        ");
                                sw.Write("m_ePVPType = PVP_GAME_TYPE[\"PGT_");
                                sw.Write(dataGridView3["Clear_Condition_PVP_TYPE", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_iPVPKill = ");
                                sw.Write(dataGridView3["Clear_Condition_PVP_KILL", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 12)
                            {
                                sw.Write("        ");
                                sw.Write("m_iQuestID = ");
                                sw.Write(dataGridView3["Clear_Condition_QUEST", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 13)
                            {
                                sw.Write("        ");
                                sw.Write("m_iTitleID = ");
                                sw.Write(dataGridView3["Clear_Condition_COLLECT_TITLE", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (Convert.ToInt32(dataGridView3["Clear_Type_Idx", i].Value.ToString()) == 14)
                            {
                                sw.Write("        ");
                                sw.Write("m_iItemID = ");
                                sw.Write(dataGridView3["Clear_Condition_USE_ITEM", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            sw.Write("    },\r\n");



                            sw.Write("}\r\n\r\n");
                        }
                        sw.Close();
                                               

                        Process Dos = new Process();
                        Dos.StartInfo.FileName = "tortoiseProc";
                        Dos.StartInfo.Arguments = "/Command:commit /path:\"..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\SubTitleMission.lua\" /notempfile /closeonend:1";
                        Dos.Start();

                        
                    }
                    break;
            }


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

            if (dgvCurrentCell.OwningColumn.Name == "Title_Type")
            {
                DataRow dr = tblTitleType.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Title_Type_", nIndexRow].Value = dr["CODE"].ToString();                    
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }
            if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Type")
            {
                DataRow dr = tblUnitType_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Condition_Unit_Type_", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }
            if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Class")
            {
                DataRow dr = tblUnitClass_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Condition_Unit_Class_", nIndexRow].Value = dr["CODE"].ToString();
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

                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        if (dataGridView1[1, i].Value.ToString() == dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Value.ToString())
                        {
                            if (i == nIndexRow)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID값이 존재합니다.");

                            dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Value = objTemp;
                            dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Black;
                            dataGridView1["Regdate", nIndexRow].Value = strOldTime;
                            dataGridView1["Regdate", nIndexRow].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }


                if (dgvCurrentCell.OwningColumn.Name == "Title_Name" ||
                    dgvCurrentCell.OwningColumn.Name == "Description")
                {
                    //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여
                    if (dgvCurrentCell.OwningColumn.Name == "Title_Name")
                    {
                        dataGridView1.Rows[dgvCurrentCell.RowIndex].HeaderCell.Value = dataGridView1["TitleID", dgvCurrentCell.RowIndex].Value.ToString() + "     " + dataGridView1["Title_Name", dgvCurrentCell.RowIndex].Value.ToString();
                    }


                    //09. 06. 23 김정협 국가별 디파일 정의 할 것
#if __COUNTRY_KR__
                    int nIndex_Name = -1;
                    string strCountry = "KR";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "TitleTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "TitleTable");
                    }
#elif __COUNTRY_TW__
                    int nIndex_Name = -1;
                    string strCountry = "TW";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "TitleTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()),"TW", objTemp.ToString(), "TitleTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_HK__
                    int nIndex_Name = -1;
                    string strCountry = "HK";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "TitleTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()),"HK", objTemp.ToString(), "TitleTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_JP__
                    int nIndex_Name = -1;
                    string strCountry = "JP";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "TitleTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()),"JP", objTemp.ToString(), "TitleTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#endif
                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //09. 06. 19 김정협 Trans 수정부분 추가 
                    DataRow Row = null;

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
                                tblTrans.Rows[i]["TableName"].ToString() == "TitleTable" &&
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

        private void dataGridView2_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;
            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView2.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            objTemp = dgvCurrentCell.Value;
            //09. 06. 02 김정협 수정 : 인덱스 저장용 임시 저장소
            objTemp2 = dataGridView2["IndexKey", dgvCurrentCell.RowIndex].Value;

            m_nLastRow = dgvCurrentCell.RowIndex;

            //09. 07. 01 김정협 텍스트 수정창 열기
            if (dgvCurrentCell.Value.ToString().Length > 15 && !m_bPaste)
            {
                AssistBox.Show();
                AssistBox.Left = dataGridView2.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Left + tabControl1.Location.X + tabControl1.TabPages[0].Left;
                AssistBox.Top = dataGridView2.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Bottom + tabControl1.Location.Y + tabControl1.TabPages[0].Top;
                AssistBox.Text = dgvCurrentCell.Value.ToString();
            }
        }

        public void dataGridView2_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;

            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView2.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            int nIndexRow = 0;
            if (dgvCurrentCell.ColumnIndex == 1)
            {
                for (int i = 0; i < dataGridView2.RowCount; i++)
                {
                    if (dataGridView2["IndexKey", i].Value.Equals(objTemp2))
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

            if (dgvCurrentCell.OwningColumn.Name == "Mission Clear Type")
            {
                DataRow dr = tblMissionClearType.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView2["Mission_Clear_Type_", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }
            
            if (dgvCurrentCell.OwningColumn.Name == "ID")
            {
                DataRow dr = tblTitle.Rows.Find(dataGridView2["ID", nIndexRow].Value.ToString());
                if (dr != null)
                {
                    dataGridView2["Mission_Name", nIndexRow].Value = dr["Title_Name"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Type")
            {
                DataRow dr = tblUnitType_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView2["Condition_Unit_Type_", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Class")
            {
                DataRow dr = tblUnitClass_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView2["Condition_Unit_Class_", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            if (dgvCurrentCell.OwningColumn.Name == "Reward_Title_ID")
            {
                if (dgvCurrentCell.Value.ToString() == "0")
                {
                    dataGridView2["Title Name", nIndexRow].Value = "";
                }
                else
                {
                    DataRow dr = tblTitle.Rows.Find(dgvCurrentCell.Value.ToString());
                    if (dr != null)
                    {
                        dataGridView2["Title Name", nIndexRow].Value = dr["Title_Name"].ToString();
                    }
                    else
                    {
                        MessageBox.Show("존재하지 않는 값입니다.");
                        dgvCurrentCell.Value = objTemp;
                    }
                }
            }
            


            //09. 06. 02 김정협 수정 : 자동정렬 후 Index 재 검색 후 값비교
            if (!objTemp.Equals((object)dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Value))
            {
                //09. 06. 02 김정협 수정 : dgvCurrentCell.RowIndex 를 IndexRow로 교체
                string strOldTime = dataGridView2["Regdate", nIndexRow].Value.ToString();

                dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Red;
                dataGridView2["Regdate", nIndexRow].Value = DateTime.Now;
                dataGridView2["Regdate", nIndexRow].Style.ForeColor = Color.Red;

                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView2.RowCount; i++)
                    {
                        if (dataGridView2[1, i].Value.ToString() == dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Value.ToString())
                        {
                            if (i == nIndexRow)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID값이 존재합니다.");

                            dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Value = objTemp;
                            dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Black;
                            dataGridView2["Regdate", nIndexRow].Value = strOldTime;
                            dataGridView2["Regdate", nIndexRow].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                if (dgvCurrentCell.OwningColumn.Name == "Mission_Name")
                {
                    dataGridView2.Rows[dgvCurrentCell.RowIndex].HeaderCell.Value = dataGridView2["ID", dgvCurrentCell.RowIndex].Value.ToString() + "     " + dataGridView2["Mission_Name", dgvCurrentCell.RowIndex].Value.ToString();

                    //09. 06. 23 김정협 국가별 디파일 정의 할 것
#if __COUNTRY_KR__
                    int nIndex_Name = -1;
                    string strCountry = "KR";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "TitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "TitleMission");
                    }
#elif __COUNTRY_TW__
                    int nIndex_Name = -1;
                    string strCountry = "TW";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "TitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()),"TW", objTemp.ToString(), "TitleMission");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_HK__
                    int nIndex_Name = -1;
                    string strCountry = "HK";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "TitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()),"HK", objTemp.ToString(), "TitleMission");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_JP__
                    int nIndex_Name = -1;
                    string strCountry = "JP";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "TitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()),"JP", objTemp.ToString(), "TitleMission");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#endif
                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //09. 06. 19 김정협 Trans 수정부분 추가 
                    DataRow Row = null;

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
                            if (tblTrans.Rows[i]["Index"].ToString() == dataGridView2["IndexKey", nIndexRow].Value.ToString() &&
                                tblTrans.Rows[i]["TableName"].ToString() == "TitleMission" &&
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
                sModiftIDList.nTabIndex = 1;
                //09. 06. 01 김정협 수정 : Cell정보 추가
                sModiftIDList.bCell = true;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString());


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
                sUndoData.nTabIndex = 1;
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
                sChangeCellList.nTabIndex = 1;
                sChangeCellList.nColumnIndex = dgvCurrentCell.ColumnIndex;
                sChangeCellList.nIndexKey = Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString());
                if (!m_ChangeCellList.Contains(sChangeCellList))
                {
                    m_ChangeCellList.Add(sChangeCellList);
                }
                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int c = 0; c < dataGridView2.ColumnCount; c++)
                    {
                        if (dataGridView2[c, m_nLastRow].HasStyle)
                            dataGridView2[c, m_nLastRow].Style.BackColor = Color.White;
                    }
                    SetCellBackgroundColorForDB(1);
                    ColorRefresh(true);
                }
            }

        }

        private void dataGridView3_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;
            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView3.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            objTemp = dgvCurrentCell.Value;
            //09. 06. 02 김정협 수정 : 인덱스 저장용 임시 저장소
            objTemp2 = dataGridView3["IndexKey", dgvCurrentCell.RowIndex].Value;
            m_nLastRow = dgvCurrentCell.RowIndex;

            //09. 07. 01 김정협 텍스트 수정창 열기
            if (dgvCurrentCell.Value.ToString().Length > 15 && !m_bPaste)
            {
                AssistBox.Show();
                AssistBox.Left = dataGridView3.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Left + tabControl1.Location.X + tabControl1.TabPages[0].Left;
                AssistBox.Top = dataGridView3.GetCellDisplayRectangle(dgvCurrentCell.ColumnIndex, dgvCurrentCell.RowIndex, false).Bottom + tabControl1.Location.Y + tabControl1.TabPages[0].Top;
                AssistBox.Text = dgvCurrentCell.Value.ToString();
            }

        }

        public void dataGridView3_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            DataGridViewCell dgvCurrentCell;

            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView3.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            int nIndexRow = 0;
            if (dgvCurrentCell.ColumnIndex == 1)
            {
                for (int i = 0; i < dataGridView3.RowCount; i++)
                {
                    if (dataGridView3["IndexKey", i].Value.Equals(objTemp2))
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

            if (dgvCurrentCell.OwningColumn.Name == "Clear_Condition_Collection_Item_ID")
            {
                DataRow dr = tblItem.Rows.Find(dgvCurrentCell.Value.ToString());
                if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                {
                    dataGridView3["Name", nIndexRow].Value = "";
                }
                else
                {
                    if (dr != null)
                    {
                        dataGridView3["Name", nIndexRow].Value = dr["Name"].ToString();
                    }
                    else
                    {
                        MessageBox.Show("존재하지 않는 값입니다.");
                        dgvCurrentCell.Value = objTemp;
                    }
                }
            }
            if (dgvCurrentCell.OwningColumn.Name == "Clear_Type_Idx")
            {
                DataRow dr = tblMissionClearType.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView3["Clear_Type_", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            if (dgvCurrentCell.OwningColumn.Name == "NPC ID")
            {
                DataRow dr = tblNpcID.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                    {
                        dataGridView3["Clear Condition Kill NPC ID", nIndexRow].Value = "NULL";
                    }
                    else
                    {

                        dataGridView3["Clear Condition Kill NPC ID", nIndexRow].Value = dr["CODE"].ToString();
                    }

                }
                else
                {

                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            //09. 06. 02 김정협 수정 : 자동정렬 후 Index 재 검색 후 값비교
            if (!objTemp.Equals((object)dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Value))
            {
                //09. 06. 02 김정협 수정 : dgvCurrentCell.RowIndex 를 IndexRow로 교체
                string strOldTime = dataGridView3["Regdate", nIndexRow].Value.ToString();

                dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Red;
                dataGridView3["Regdate", nIndexRow].Value = DateTime.Now;
                dataGridView3["Regdate", nIndexRow].Style.ForeColor = Color.Red;

                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int i = 0; i < dataGridView3.RowCount; i++)
                    {
                        if (dataGridView3[1, i].Value.ToString() == dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Value.ToString())
                        {
                            if (i == nIndexRow)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID값이 존재합니다.");

                            dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Value = objTemp;
                            dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Black;
                            dataGridView3["Regdate", nIndexRow].Value = strOldTime;
                            dataGridView3["Regdate", nIndexRow].Style.ForeColor = Color.Black;
                            return;

                        }
                    }
                }

                if (dgvCurrentCell.OwningColumn.Name == "Description")
                {
                    dataGridView3.Rows[dgvCurrentCell.RowIndex].HeaderCell.Value = dataGridView3["ID", dgvCurrentCell.RowIndex].Value.ToString() + "     " + dataGridView3["Description", dgvCurrentCell.RowIndex].Value.ToString();

                    //09. 06. 23 김정협 국가별 디파일 정의 할 것
#if __COUNTRY_KR__
                    int nIndex_Name = -1;
                    string strCountry = "KR";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SubTitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SubTitleMission");
                    }
#elif __COUNTRY_TW__
                    int nIndex_Name = -1;
                    string strCountry = "TW";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "SubTitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "SubTitleMission");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_HK__
                    int nIndex_Name = -1;
                    string strCountry = "HK";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "SubTitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "SubTitleMission");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_JP__
                    int nIndex_Name = -1;
                    string strCountry = "JP";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "SubTitleMission") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "SubTitleMission");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#endif
                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //09. 06. 19 김정협 Trans 수정부분 추가 
                    DataRow Row = null;

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
                            if (tblTrans.Rows[i]["Index"].ToString() == dataGridView3["IndexKey", nIndexRow].Value.ToString() &&
                                tblTrans.Rows[i]["TableName"].ToString() == "SubTitleMission" &&
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
                sModiftIDList.nTabIndex = 2;
                //09. 06. 01 김정협 수정 : Cell정보 추가
                sModiftIDList.bCell = true;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString());


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
                sUndoData.nTabIndex = 2;
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
                sChangeCellList.nTabIndex = 2;
                sChangeCellList.nColumnIndex = dgvCurrentCell.ColumnIndex;
                sChangeCellList.nIndexKey = Convert.ToInt32(dataGridView3["IndexKey", nIndexRow].Value.ToString());
                if (!m_ChangeCellList.Contains(sChangeCellList))
                {
                    m_ChangeCellList.Add(sChangeCellList);
                }
                if (dgvCurrentCell.ColumnIndex == 1)
                {
                    for (int c = 0; c < dataGridView3.ColumnCount; c++)
                    {
                        if (dataGridView3[c, m_nLastRow].HasStyle)
                            dataGridView3[c, m_nLastRow].Style.BackColor = Color.White;
                    }
                    SetCellBackgroundColorForDB(2);
                    ColorRefresh(true);
                }
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

            //dataGridView1.ClipboardCopyMode = DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
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
                            dataGridView1.RefreshEdit();
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

        private void dataGridView2_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                Search frm = new Search();
                frm.SetDataGridView(dataGridView2);
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
                Clipboard.SetDataObject(dataGridView2.GetClipboardContent().GetText());
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
                            m_bPaste = true;
                            m_dgvcTemp = dataGridView2[nColumnIndex, nRowIndex];
                            dataGridView2_CellBeginEdit(null, null);
                            dataGridView2[nColumnIndex, nRowIndex].Value = strData[j];

                            dataGridView2_CellEndEdit(null, null);
                            dataGridView2.RefreshEdit();
                            m_bPaste = false;
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
                    //09. 07. 02 김정협 언두리두시 그룹지정
                    if (m_UndoStack.Count > 0)
                    {
                        m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                    }
                    else
                    {
                        m_nPasteGroup = 0;
                    }

                    for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                    {
                        m_bPaste = true;
                        m_dgvcTemp = dataGridView2.SelectedCells[i];
                        dataGridView2_CellBeginEdit(null, null);

                        if (dataGridView2.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                        {
                            dataGridView2.SelectedCells[i].Value = false;
                        }
                        else if (dataGridView2.SelectedCells[i].ValueType == Type.GetType("System.String"))
                        {
                            dataGridView2.SelectedCells[i].Value = "";
                        }
                        else
                        {
                            dataGridView2.SelectedCells[i].Value = 0;
                        }

                        dataGridView2_CellEndEdit(null, null);
                        dataGridView2.RefreshEdit();
                        m_bPaste = false;
                    }
                }
            }
        }

        private void dataGridView3_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                Search frm = new Search();
                frm.SetDataGridView(dataGridView3);
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
                Clipboard.SetDataObject(dataGridView3.GetClipboardContent().GetText());
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
                            m_bPaste = true;
                            m_dgvcTemp = dataGridView3[nColumnIndex, nRowIndex];
                            dataGridView3_CellBeginEdit(null, null);
                            dataGridView3[nColumnIndex, nRowIndex].Value = strData[j];

                            dataGridView3_CellEndEdit(null, null);
                            dataGridView3.RefreshEdit();
                            m_bPaste = false;
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
                    //09. 07. 02 김정협 언두리두시 그룹지정
                    if (m_UndoStack.Count > 0)
                    {
                        m_nPasteGroup = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                    }
                    else
                    {
                        m_nPasteGroup = 0;
                    }

                    for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                    {
                        m_bPaste = true;
                        m_dgvcTemp = dataGridView3.SelectedCells[i];
                        dataGridView3_CellBeginEdit(null, null);

                        if (dataGridView3.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                        {
                            dataGridView3.SelectedCells[i].Value = false;
                        }
                        else if (dataGridView3.SelectedCells[i].ValueType == Type.GetType("System.String"))
                        {
                            dataGridView3.SelectedCells[i].Value = "";
                        }
                        else
                        {
                            dataGridView3.SelectedCells[i].Value = 0;
                        }

                        dataGridView3_CellEndEdit(null, null);
                        dataGridView3.RefreshEdit();
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
                    case 1:
                        {
                            if (dataGridView2.CurrentCell.ColumnIndex != dataGridView2[textBox1.Text, 0].ColumnIndex)
                            {
                                m_nStartPoint = 0;
                            }

                            for (int i = m_nStartPoint; i < dataGridView2.RowCount; i++)
                            {
                                if (!dataGridView2[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView2[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView2.CurrentCell = dataGridView2[textBox1.Text, i];
                                    m_nStartPoint = i + 1;
                                    return;
                                }
                            }

                            for (int i = 0; i < m_nStartPoint; i++)
                            {
                                if (!dataGridView2[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView2[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView2.CurrentCell = dataGridView2[textBox1.Text, i];
                                    m_nStartPoint = i + 1;
                                    return;
                                }
                            }

                            MessageBox.Show("결과값이 존재하지 않습니다.");
                        }
                        break;
                    case 2:
                        {
                            if (dataGridView3.CurrentCell.ColumnIndex != dataGridView3[textBox1.Text, 0].ColumnIndex)
                            {
                                m_nStartPoint = 0;
                            }

                            for (int i = m_nStartPoint; i < dataGridView3.RowCount; i++)
                            {
                                if (!dataGridView3[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView3[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView3.CurrentCell = dataGridView3[textBox1.Text, i];
                                    m_nStartPoint = i + 1;
                                    return;
                                }
                            }

                            for (int i = 0; i < m_nStartPoint; i++)
                            {
                                if (!dataGridView3[textBox1.Text, i].Visible)
                                {
                                    continue;
                                }
                                if (dataGridView3[textBox1.Text, i].Value.ToString().IndexOf(textBox2.Text) > -1)
                                {
                                    dataGridView3.CurrentCell = dataGridView3[textBox1.Text, i];
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

        private void dataGridView1_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView1[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void dataGridView2_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView2[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void dataGridView3_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView3[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void tabControl1_SelectedIndexChanged(object sender, EventArgs e)
        {
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

            if(tblColor.Rows.Count == 0)
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
                                    dataGridView1.SelectedCells[i].OwningColumn.Name != "Regdate" )
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
                                    if (drTemp["TableName"].ToString() == "TitleTable" &&
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
                                drTemp["TableName"] = "TitleTable";
                                drTemp["Column"] = dataGridView1.SelectedCells[i].ColumnIndex;
                                drTemp["IndexNum"] = dataGridView1["IndexKey", dataGridView1.SelectedCells[i].RowIndex].Value;

                                drTemp["Color"] = cColor.ToArgb();

                                drTemp["Regdate"] = DateTime.Now;
                            }
                        }
                        RowDelete.Clear();
                        //Adpt_ColorTable.Update(tblColor);
                    }
                    break;
                case 1:
                    {
                        if (cColor == Color.White)
                        {
                            for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView2.SelectedCells[i].ReadOnly &&
                                    dataGridView2.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView2.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }

                                if (dataGridView2.SelectedCells[i].Style.BackColor == Color.White)
                                {
                                    continue;
                                }
                                dataGridView2.SelectedCells[i].Style.BackColor = cColor;
                                for (int j = 0; j < tblColor.Rows.Count; j++)
                                {
                                    if (RowDelete.Contains(j) || tblColor.Rows[j].RowState == DataRowState.Deleted)
                                    {
                                        continue;
                                    }
                                    DataRow drTemp = tblColor.Rows[j];
                                    if (drTemp["TableName"].ToString() == "TitleMission" &&
                                    drTemp["Column"].ToString() == Convert.ToString(dataGridView2.SelectedCells[i].ColumnIndex) &&
                                    drTemp["IndexNum"].ToString() == dataGridView2["IndexKey", dataGridView2.SelectedCells[i].RowIndex].Value.ToString())
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
                            for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView2.SelectedCells[i].ReadOnly &&
                                    dataGridView2.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView2.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }

                                dataGridView2.SelectedCells[i].Style.BackColor = cColor;
                                if (tblColor.Rows.Count > 0)
                                {
                                    //nIndex = 0;
                                    DataRow dr = tblColor.Rows[tblColor.Rows.Count - 1];
                                    nIndex = Convert.ToInt32(dr["IndexKey"].ToString()) + 1;
                                }
                                tblColor.Rows.Add();
                                DataRow drTemp = tblColor.Rows[tblColor.Rows.Count - 1];
                                drTemp["IndexKey"] = nIndex;
                                drTemp["TableName"] = "TitleMission";
                                drTemp["Column"] = dataGridView2.SelectedCells[i].ColumnIndex;
                                drTemp["IndexNum"] = dataGridView2["IndexKey", dataGridView2.SelectedCells[i].RowIndex].Value;

                                drTemp["Color"] = cColor.ToArgb();

                                drTemp["Regdate"] = DateTime.Now;
                            }
                        }
                        RowDelete.Clear();
                        //Adpt_ColorTable.Update(tblColor);
                    }
                    break;
                case 2:
                    {
                        if (cColor == Color.White)
                        {
                            for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView3.SelectedCells[i].ReadOnly &&
                                    dataGridView3.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView3.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }

                                if (dataGridView3.SelectedCells[i].Style.BackColor == Color.White)
                                {
                                    continue;
                                }
                                dataGridView3.SelectedCells[i].Style.BackColor = cColor;
                                for (int j = 0; j < tblColor.Rows.Count; j++)
                                {
                                    if (RowDelete.Contains(j) || tblColor.Rows[j].RowState == DataRowState.Deleted)
                                    {
                                        continue;
                                    }
                                    DataRow drTemp = tblColor.Rows[j];
                                    if (drTemp["TableName"].ToString() == "SubTitleMission" &&
                                    drTemp["Column"].ToString() == Convert.ToString(dataGridView3.SelectedCells[i].ColumnIndex) &&
                                    drTemp["IndexNum"].ToString() == dataGridView3["IndexKey", dataGridView3.SelectedCells[i].RowIndex].Value.ToString())
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
                            for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                            {
                                //09. 06. 10 링크데이터 셀색 변경 금지
                                if (dataGridView3.SelectedCells[i].ReadOnly &&
                                    dataGridView3.SelectedCells[i].OwningColumn.Name != "IndexKey" &&
                                    dataGridView3.SelectedCells[i].OwningColumn.Name != "Regdate")
                                {
                                    continue;
                                }

                                dataGridView3.SelectedCells[i].Style.BackColor = cColor;
                                if (tblColor.Rows.Count > 0)
                                {
                                    //nIndex = 0;
                                    DataRow dr = tblColor.Rows[tblColor.Rows.Count - 1];
                                    nIndex = Convert.ToInt32(dr["IndexKey"].ToString()) + 1;
                                }
                                tblColor.Rows.Add();
                                DataRow drTemp = tblColor.Rows[tblColor.Rows.Count - 1];
                                drTemp["IndexKey"] = nIndex;
                                drTemp["TableName"] = "SubTitleMission";
                                drTemp["Column"] = dataGridView3.SelectedCells[i].ColumnIndex;
                                drTemp["IndexNum"] = dataGridView3["IndexKey", dataGridView3.SelectedCells[i].RowIndex].Value;

                                drTemp["Color"] = cColor.ToArgb();

                                drTemp["Regdate"] = DateTime.Now;
                            }
                        }
                        RowDelete.Clear();
                        //Adpt_ColorTable.Update(tblColor);
                    }
                    break;
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                for (int i = 0; i < dataGridView1.ColumnCount; i++)
                {
                    dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Automatic;
                }
                for (int i = 0; i < dataGridView2.ColumnCount; i++)
                {
                    dataGridView2.Columns[i].SortMode = DataGridViewColumnSortMode.Automatic;
                }
                for (int i = 0; i < dataGridView3.ColumnCount; i++)
                {
                    dataGridView3.Columns[i].SortMode = DataGridViewColumnSortMode.Automatic;
                }
            }
            else
            {
                for (int i = 0; i < dataGridView1.ColumnCount; i++)
                {
                    dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                }
                for (int i = 0; i < dataGridView2.ColumnCount; i++)
                {
                    dataGridView2.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                }
                for (int i = 0; i < dataGridView3.ColumnCount; i++)
                {
                    dataGridView3.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                }
            }
        }

        private void TitleTable_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.YesNoCancel);
                if (result == DialogResult.Yes)
                {
                    Save();
                    MessageBox.Show("저장되었습니다.");
                }
                else if (result == DialogResult.Cancel)
                {
                    e.Cancel = true;
                    return;
                }

                m_ModifyIDList.Clear();
                m_ChangeCellList.Clear();
                m_bSaveEnd = true;
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

                            if (dataGridView1.CurrentCell.OwningColumn.Name == "Title_Type")
                            {
                                frm.SetTable(tblTitleType);
                            }
                            else if (dataGridView1.CurrentCell.OwningColumn.Name == "Condition Unit Type")
                            {
                                frm.SetTable(tblUnitType_Title);
                            }
                            else if (dataGridView1.CurrentCell.OwningColumn.Name == "Condition Unit Class")
                            {
                                frm.SetTable(tblUnitClass_Title);
                            }

                            frm.SetPoint(MousePosition.X, MousePosition.Y);
                            frm.SetDataGridView(dataGridView1, tabControl1.SelectedIndex, "TitleTable");
                            frm.Owner = this;
                            frm.ShowDialog(this);
                        }

                        dataGridView1.RefreshEdit();
                    }
                    break;
                case 1:
                    {
                        if (e.ClickedItem.Text == "전체 선택")
                        {
                            for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                            {
                                if (dataGridView2.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                                {
                                    m_bPaste = true;
                                    m_dgvcTemp = dataGridView2.SelectedCells[i];
                                    dataGridView2_CellBeginEdit(null, null);
                                    dataGridView2.SelectedCells[i].Value = true;

                                    dataGridView2_CellEndEdit(null, null);
                                    dataGridView2.RefreshEdit();
                                    //전체선택 후 셀에서 빠져나갈대 dataGridView1_CellEndEdit이벤트 발생시키지않기위하여
                                    objTemp = dataGridView2.CurrentCell.Value;
                                    m_bPaste = false;
                                }

                            }
                        }
                        if (e.ClickedItem.Text == "전체 해제")
                        {
                            for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                            {
                                if (dataGridView2.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                                {
                                    m_bPaste = true;
                                    m_dgvcTemp = dataGridView2.SelectedCells[i];
                                    dataGridView2_CellBeginEdit(null, null);
                                    dataGridView2.SelectedCells[i].Value = false;

                                    dataGridView2_CellEndEdit(null, null);
                                    dataGridView2.RefreshEdit();
                                    //전체선택 후 셀에서 빠져나갈대 dataGridView1_CellEndEdit이벤트 발생시키지않기위하여
                                    objTemp = dataGridView2.CurrentCell.Value;
                                    m_bPaste = false;
                                }
                            }
                        }

                        //09. 07. 02 김정협 링크리스트 제작
                        if (e.ClickedItem.Text == "List열기")
                        {
                            dataGridView2_CellBeginEdit(null, null);
                            LinkListView frm = new LinkListView();

                            if (dataGridView2.CurrentCell.OwningColumn.Name == "Mission Clear Type")
                            {
                                frm.SetTable(tblTitleType);
                            }
                            else if (dataGridView2.CurrentCell.OwningColumn.Name == "Condition Unit Type")
                            {
                                frm.SetTable(tblUnitType_Title);
                            }
                            else if (dataGridView2.CurrentCell.OwningColumn.Name == "Condition Unit Class")
                            {
                                frm.SetTable(tblUnitClass_Title);
                            }

                            frm.SetPoint(MousePosition.X, MousePosition.Y);
                            frm.SetDataGridView(dataGridView2, tabControl1.SelectedIndex, "TitleTable");
                            frm.Owner = this;
                            frm.ShowDialog(this);
                        }

                        dataGridView2.RefreshEdit();
                    }
                    break;
                case 2:
                    {
                        if (e.ClickedItem.Text == "전체 선택")
                        {
                            for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                            {
                                if (dataGridView3.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                                {
                                    m_bPaste = true;
                                    m_dgvcTemp = dataGridView3.SelectedCells[i];
                                    dataGridView3_CellBeginEdit(null, null);
                                    dataGridView3.SelectedCells[i].Value = true;

                                    dataGridView3_CellEndEdit(null, null);
                                    dataGridView3.RefreshEdit();
                                    //전체선택 후 셀에서 빠져나갈대 dataGridView1_CellEndEdit이벤트 발생시키지않기위하여
                                    objTemp = dataGridView3.CurrentCell.Value;
                                    m_bPaste = false;
                                }

                            }
                        }
                        if (e.ClickedItem.Text == "전체 해제")
                        {
                            for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                            {
                                if (dataGridView3.SelectedCells[i].ValueType == Type.GetType("System.Boolean"))
                                {
                                    m_bPaste = true;
                                    m_dgvcTemp = dataGridView3.SelectedCells[i];
                                    dataGridView3_CellBeginEdit(null, null);
                                    dataGridView3.SelectedCells[i].Value = false;

                                    dataGridView3_CellEndEdit(null, null);
                                    dataGridView3.RefreshEdit();
                                    //전체선택 후 셀에서 빠져나갈대 dataGridView1_CellEndEdit이벤트 발생시키지않기위하여
                                    objTemp = dataGridView3.CurrentCell.Value;
                                    m_bPaste = false;
                                }
                            }
                        }

                        //09. 07. 02 김정협 링크리스트 제작
                        if (e.ClickedItem.Text == "List열기")
                        {
                            dataGridView3_CellBeginEdit(null, null);
                            LinkListView frm = new LinkListView();

                            if (dataGridView3.CurrentCell.OwningColumn.Name == "Clear_Type_Idx")
                            {
                                frm.SetTable(tblMissionClearType);
                            }
                            else if (dataGridView3.CurrentCell.OwningColumn.Name == "NPC ID")
                            {
                                frm.SetTable(tblNpcID);
                            }
                            

                            frm.SetPoint(MousePosition.X, MousePosition.Y);
                            frm.SetDataGridView(dataGridView3, tabControl1.SelectedIndex, "TitleTable");
                            frm.Owner = this;
                            frm.ShowDialog(this);
                        }

                        dataGridView3.RefreshEdit();
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
                 case 1:
                     frm.SetDataGridView(dataGridView2);
                     break;
                 case 2:
                     frm.SetDataGridView(dataGridView3);
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
                    if (dataGridView1.CurrentCell.OwningColumn.Name == "Title_Type" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Condition Unit Type" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Condition Unit Class")
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

        private void dataGridView2_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
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
                    for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                    {
                        if (e.ColumnIndex == dataGridView2.SelectedCells[i].ColumnIndex && e.RowIndex == dataGridView2.SelectedCells[i].RowIndex)
                        {
                            bSelected = true;
                            break;
                        }
                    }
                    if (!bSelected)
                    {
                        dataGridView2.CurrentCell = dataGridView2[e.ColumnIndex, e.RowIndex];
                    }

                    if (dataGridView2.CurrentCell.ValueType == Type.GetType("System.Boolean") && bSelected && dataGridView2.SelectedCells.Count > 1)
                    {
                        contextMenuStrip1.Items.Add("전체 선택");
                        contextMenuStrip1.Items.Add("전체 해제");
                        contextMenuStrip1.Show();
                    }

                    //09. 07. 02 김정협 링크리스트 열기
                    if (dataGridView2.CurrentCell.OwningColumn.Name == "Mission Clear Type" ||
                        dataGridView2.CurrentCell.OwningColumn.Name == "Condition Unit Type" ||
                        dataGridView2.CurrentCell.OwningColumn.Name == "Condition Unit Class")
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

        private void dataGridView3_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
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
                    for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                    {
                        if (e.ColumnIndex == dataGridView3.SelectedCells[i].ColumnIndex && e.RowIndex == dataGridView3.SelectedCells[i].RowIndex)
                        {
                            bSelected = true;
                            break;
                        }
                    }
                    if (!bSelected)
                    {
                        dataGridView3.CurrentCell = dataGridView3[e.ColumnIndex, e.RowIndex];
                    }

                    if (dataGridView3.CurrentCell.ValueType == Type.GetType("System.Boolean") && bSelected && dataGridView3.SelectedCells.Count > 1)
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

                //09. 07. 02 김정협 링크리스트 열기
                if (dataGridView3.CurrentCell.OwningColumn.Name == "Clear_Type_Idx" ||
                    dataGridView3.CurrentCell.OwningColumn.Name == "NPC ID")
                {
                    contextMenuStrip1.Items.Add("List열기");
                    contextMenuStrip1.Show();
                }
            }
        }

        private void contextMenuStrip2_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            contextMenuStrip2.Items.Clear();
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
                    case 1:
                        {
                            frm.SetDataGridView(dataGridView2);
                        }
                        break;
                    case 2:
                        {
                            frm.SetDataGridView(dataGridView3);
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
                    case 1:
                        {
                            for (int i = 0; i < dataGridView2.RowCount; i++)
                            {
                                if (dataGridView2.Rows[i].Visible == false)
                                    dataGridView2.Rows[i].Visible = true;
                            }
                        }
                        break;
                    case 2:
                        {
                            for (int i = 0; i < dataGridView3.RowCount; i++)
                            {
                                if (dataGridView3.Rows[i].Visible == false)
                                    dataGridView3.Rows[i].Visible = true;
                            }
                        }
                        break;
                }

                m_IsFiltering = false;
            }
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
                case 1:
                    {
                        textBox3.Text = dataGridView2.CurrentCell.Value.ToString();
                    }
                    break;
                case 2:
                    {
                        textBox3.Text = dataGridView3.CurrentCell.Value.ToString();
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
                case 1:
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

                        for (int i = 0; i < dataGridView2.SelectedCells.Count; i++)
                        {
                            if (dataGridView2.SelectedCells[i].ReadOnly)
                            {
                                continue;
                            }
                            int nIndex = dataGridView2.SelectedCells[i].Value.ToString().IndexOf(textBox3.Text);
                            if (nIndex != -1)
                            {
                                m_bPaste = true;
                                m_dgvcTemp = dataGridView2.SelectedCells[i];
                                dataGridView2_CellBeginEdit(null, null);

                                string strTemp = dataGridView2.SelectedCells[i].Value.ToString();
                                dataGridView2.SelectedCells[i].Value = strTemp.Replace(textBox3.Text, textBox4.Text);

                                dataGridView2_CellEndEdit(null, null);
                                dataGridView2.RefreshEdit();
                                m_bPaste = false; 
                            }
                        }
                    }
                    break;
                case 2:
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

                        for (int i = 0; i < dataGridView3.SelectedCells.Count; i++)
                        {
                            if (dataGridView3.SelectedCells[i].ReadOnly)
                            {
                                continue;
                            }
                            int nIndex = dataGridView3.SelectedCells[i].Value.ToString().IndexOf(textBox3.Text);
                            if (nIndex != -1)
                            {
                                m_bPaste = true;
                                m_dgvcTemp = dataGridView3.SelectedCells[i];
                                dataGridView3_CellBeginEdit(null, null);

                                string strTemp = dataGridView3.SelectedCells[i].Value.ToString();
                                dataGridView3.SelectedCells[i].Value = strTemp.Replace(textBox3.Text, textBox4.Text);

                                dataGridView3_CellEndEdit(null, null);
                                dataGridView3.RefreshEdit();
                                m_bPaste = false; 
                            }
                        }
                    }
                    break;
            }
            panel1.Visible = false;
            textBox3.Text = "";
            textBox4.Text = "";
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            panel1.Visible = false;
            textBox3.Text = "";
            textBox4.Text = "";
            
        }

        //09. 07. 02 김정협 Undo그룹짓기 nGroup매개변수 추가
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
                if(dataGridView1.SelectedRows.Count > 1)
                {
                    int nCount = dataGridView1.SelectedRows.Count;
                    int nMin = dataGridView1.SelectedRows[0].Index;
                    for (int i = 0; i < dataGridView1.SelectedRows.Count ; i++)
                    {
                        if(nMin > dataGridView1.SelectedRows[i].Index)
                        {
                            nMin = dataGridView1.SelectedRows[i].Index;
                        }
                    }
                    for (int i = 0; i < nCount; i++)
                    {
                        UserDeletingRow(nMin, dataGridView1, tblTitle, false, 0);
                    }
                    
                }
                else
                {
                    UserDeletingRow(e.Row.Index, dataGridView1, tblTitle, false, 0);
                }                
                e.Cancel = true;
            }
#else
            e.Cancel = true;
#endif
        }

        private void dataGridView2_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
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
                if (dataGridView2.SelectedRows.Count > 1)
                {
                    int nCount = dataGridView2.SelectedRows.Count;
                    int nMin = dataGridView2.SelectedRows[0].Index;
                    for (int i = 0; i < dataGridView2.SelectedRows.Count; i++)
                    {
                        if (nMin > dataGridView2.SelectedRows[i].Index)
                        {
                            nMin = dataGridView2.SelectedRows[i].Index;
                        }
                    }
                    for (int i = 0; i < nCount; i++)
                    {
                        
                        UserDeletingRow(nMin, dataGridView2, tblTitleMission, false, 1);
                    }

                }
                else
                {
                    UserDeletingRow(e.Row.Index, dataGridView2, tblTitleMission, false, 1);
                }         
                
                e.Cancel = true;
            }
#else
            e.Cancel = true;
#endif
        }

        private void dataGridView3_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
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
                if (dataGridView2.SelectedRows.Count > 1)
                {
                    int nCount = dataGridView2.SelectedRows.Count;
                    int nMin = dataGridView2.SelectedRows[0].Index;
                    for (int i = 0; i < dataGridView2.SelectedRows.Count; i++)
                    {
                        if (nMin > dataGridView2.SelectedRows[i].Index)
                        {
                            nMin = dataGridView2.SelectedRows[i].Index;
                        }
                    }
                    for (int i = 0; i < nCount; i++)
                    {                                                
                        UserDeletingRow(nMin, dataGridView3, tblSubTitleMission, false, 2);
                    }

                }
                else
                {                    
                    UserDeletingRow(e.Row.Index, dataGridView3, tblSubTitleMission, false, 2);
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
                sUndoData.nGroupID = sRedoTemp.nGroupID;

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
                        if (dgvCurrentCell.OwningColumn.Name == "Title_Type")
                        {
                            DataRow dr = tblTitleType.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView1["Title_Type_", nIndexRow].Value = dr["CODE"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }
                        if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Type")
                        {
                            DataRow dr = tblUnitType_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView1["Condition_Unit_Type_", nIndexRow].Value = dr["CODE"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }
                        if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Class")
                        {
                            DataRow dr = tblUnitClass_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView1["Condition_Unit_Class_", nIndexRow].Value = dr["CODE"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }
                    }
                    break;
                case 1:
                    {
                        if (dgvCurrentCell.OwningColumn.Name == "Mission Clear Type")
                        {
                            DataRow dr = tblMissionClearType.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView2["Mission_Clear_Type_", nIndexRow].Value = dr["CODE"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }

                        if (dgvCurrentCell.OwningColumn.Name == "ID")
                        {
                            DataRow dr = tblTitle.Rows.Find(dataGridView2["ID", nIndexRow].Value.ToString());
                            if (dr != null)
                            {
                                dataGridView2["Mission_Name", nIndexRow].Value = dr["Title_Name"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }

                        if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Type")
                        {
                            DataRow dr = tblUnitType_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView2["Condition_Unit_Type_", nIndexRow].Value = dr["CODE"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }

                        if (dgvCurrentCell.OwningColumn.Name == "Condition Unit Class")
                        {
                            DataRow dr = tblUnitClass_Title.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView2["Condition_Unit_Class_", nIndexRow].Value = dr["CODE"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }

                        if (dgvCurrentCell.OwningColumn.Name == "Reward_Title_ID")
                        {
                            DataRow dr = tblTitle.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView2["Title Name", nIndexRow].Value = dr["Title_Name"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }
                    }
                    break;
                case 2:
                    {
                        if (dgvCurrentCell.OwningColumn.Name == "Clear_Type_Idx")
                        {
                            DataRow dr = tblMissionClearType.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView3["Clear_Type_", nIndexRow].Value = dr["CODE"].ToString();
                            }
                            else
                            {
                                MessageBox.Show("존재하지 않는 값입니다.");
                                dgvCurrentCell.Value = objTemp;
                            }
                        }

                        if (dgvCurrentCell.OwningColumn.Name == "NPC ID")
                        {
                            DataRow dr = tblNpcID.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                                {
                                    dataGridView3["Clear Condition Kill NPC ID", nIndexRow].Value = "NULL";
                                }
                                else
                                {

                                    dataGridView3["Clear Condition Kill NPC ID", nIndexRow].Value = dr["CODE"].ToString();
                                }

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
                                    UndoKeyboardControl(dataGridView1, tblTitle, sUndoTemp, sRedoTemp, 0);
                                }
                                break;
                            case 1:
                                {
                                    UndoKeyboardControl(dataGridView2, tblTitleMission, sUndoTemp, sRedoTemp, 1);
                                }
                                break;
                            case 2:
                                {
                                    UndoKeyboardControl(dataGridView3, tblSubTitleMission, sUndoTemp, sRedoTemp, 2);
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
                                    RedoKeyboardControl(dataGridView1, tblTitle, sUndoTemp, sRedoTemp, 0);
                                }
                                break;
                            case 1:
                                {
                                    RedoKeyboardControl(dataGridView2, tblTitleMission, sUndoTemp, sRedoTemp, 1);
                                }
                                break;
                            case 2:
                                {
                                    RedoKeyboardControl(dataGridView3, tblSubTitleMission, sUndoTemp, sRedoTemp, 2);
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
                            tblTitle.Clear();
                            Adpt_TitleTable.Fill(tblTitle);

                            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
                            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
                            {
                                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["TitleID", nRowHeader].Value.ToString() + "     " + dataGridView1["Title_Name", nRowHeader].Value.ToString();
                            }

                        }
                        break;
                    case 1:
                        {
                            tblTitleMission.Clear();
                            Adpt_TitleMission.Fill(tblTitleMission);
                                                        
                            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

                            for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
                            {
                                dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["Mission_Name", nRowHeader].Value.ToString();
                            }                            
                        }
                        break;
                    case 3:
                        {
                            tblSubTitleMission.Clear();
                            Adpt_SubTitleMission.Fill(tblSubTitleMission);
                                                        
                            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
                            for (int nRowHeader = 0; nRowHeader < dataGridView3.RowCount; nRowHeader++)
                            {
                                dataGridView3.Rows[nRowHeader].HeaderCell.Value = dataGridView3["ID", nRowHeader].Value.ToString() + "     " + dataGridView3["Description", nRowHeader].Value.ToString();
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

        //09. 07. 01 김정협 텍스트 수정창 열기
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
                case 1:
                    dataGridView2.CurrentCell.Value = AssistBox.Text;
                    break;
                case 2:
                    dataGridView3.CurrentCell.Value = AssistBox.Text;
                    break;
            }
        }

        //09. 07. 01 김정협 텍스트 수정창 열기
        private void AssistBox_Leave(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    dataGridView1_CellEndEdit(null, null);
                    break;
                case 1:
                    dataGridView2_CellEndEdit(null, null);
                    break;
                case 2:
                    dataGridView3_CellEndEdit(null, null);
                    break;
            }
            AssistBox.Hide();
        }

        private void btnLink_Click(object sender, EventArgs e)
        {
            int nCount = 0;
            for (int i = 0; i < tblTitle.Columns.Count; i++)
            {
                if (tblTitle.Columns[i].ColumnName == "Title_Type")
                {
                    for (int j = 0; j < tblTitle.Rows.Count; j++)
                    {
                        DataRow dr = tblTitleType.Rows.Find(tblTitle.Rows[j]["Title_Type"].ToString());
                        if (tblTitle.Rows[j]["Title_Type_"].ToString() != dr["CODE"].ToString())
                        {
                            tblTitle.Rows[j]["Title_Type_"] = dr["CODE"];
                            Adpt_TitleTable.Update(new DataRow[] { tblTitle.Rows[j] });
                            nCount++;
                        }
                    }
                }


                if (tblTitle.Columns[i].ColumnName == "Condition Unit Type")
                {
                    for (int j = 0; j < tblTitle.Rows.Count; j++)
                    {
                        DataRow dr = tblUnitType_Title.Rows.Find(tblTitle.Rows[j]["Condition Unit Type"].ToString());
                        if (tblTitle.Rows[j]["Condition_Unit_Type_"].ToString() != dr["CODE"].ToString())
                        {
                            tblTitle.Rows[j]["Condition_Unit_Type_"] = dr["CODE"];
                            Adpt_TitleTable.Update(new DataRow[] { tblTitle.Rows[j] });
                            nCount++;
                        }
                    }
                }


                if (tblTitle.Columns[i].ColumnName == "Condition Unit Class")
                {
                    for (int j = 0; j < tblTitle.Rows.Count; j++)
                    {
                        DataRow dr = tblUnitClass_Title.Rows.Find(tblTitle.Rows[j]["Condition Unit Class"].ToString());
                        if (tblTitle.Rows[j]["Condition_Unit_Class_"].ToString() != dr["CODE"].ToString())
                        {
                            tblTitle.Rows[j]["Condition_Unit_Class_"] = dr["CODE"];
                            Adpt_TitleTable.Update(new DataRow[] { tblTitle.Rows[j] });
                            nCount++;
                        }
                    }
                }
            }

            MessageBox.Show("TitleTable 틀린 개수는 " + nCount + "개의 잘못된 링크를 수정하였습니다.");
            nCount = 0;



            for (int i = 0; i < tblTitleMission.Columns.Count; i++)
            {
                if (tblTitleMission.Columns[i].ColumnName == "Mission Clear Type")
                {
                    for (int j = 0; j < tblTitleMission.Rows.Count; j++)
                    {
                        DataRow dr = tblMissionClearType.Rows.Find(tblTitleMission.Rows[j]["Mission Clear Type"].ToString());
                        if (tblTitleMission.Rows[j]["Mission_Clear_Type_"].ToString() != dr["CODE"].ToString())
                        {
                            tblTitleMission.Rows[j]["Mission_Clear_Type_"] = dr["CODE"];
                            Adpt_TitleMission.Update(new DataRow[] { tblTitleMission.Rows[j] });
                            nCount++;
                        }
                    }
                }


                if (tblTitleMission.Columns[i].ColumnName == "ID")
                {
                    for (int j = 0; j < tblTitleMission.Rows.Count; j++)
                    {
                        DataRow dr = tblTitle.Rows.Find(tblTitleMission.Rows[j]["ID"].ToString());
                        if (tblTitleMission.Rows[j]["Mission_Name"].ToString() != dr["Title_Name"].ToString())
                        {
                            tblTitleMission.Rows[j]["Mission_Name"] = dr["Title_Name"];
                            Adpt_TitleMission.Update(new DataRow[] { tblTitleMission.Rows[j] });
                            nCount++;
                        }
                    }
                }

                if (tblTitleMission.Columns[i].ColumnName == "Condition Unit Type")
                {
                    for (int j = 0; j < tblTitleMission.Rows.Count; j++)
                    {
                        DataRow dr = tblUnitType_Title.Rows.Find(tblTitleMission.Rows[j]["Condition Unit Type"].ToString());
                        if (tblTitleMission.Rows[j]["Condition_Unit_Type_"].ToString() != dr["CODE"].ToString())
                        {
                            tblTitleMission.Rows[j]["Condition_Unit_Type_"] = dr["CODE"];
                            Adpt_TitleMission.Update(new DataRow[] { tblTitleMission.Rows[j] });
                            nCount++;
                        }
                    }
                }

                if (tblTitleMission.Columns[i].ColumnName == "Condition Unit Class")
                {
                    for (int j = 0; j < tblTitleMission.Rows.Count; j++)
                    {
                        DataRow dr = tblUnitClass_Title.Rows.Find(tblTitleMission.Rows[j]["Condition Unit Class"].ToString());
                        if (tblTitleMission.Rows[j]["Condition_Unit_Class_"].ToString() != dr["CODE"].ToString())
                        {
                            tblTitleMission.Rows[j]["Condition_Unit_Class_"] = dr["CODE"];
                            Adpt_TitleMission.Update(new DataRow[] { tblTitleMission.Rows[j] });
                            nCount++;
                        }
                    }
                }

                if (tblTitleMission.Columns[i].ColumnName == "Reward_Title_ID")
                {
                    for (int j = 0; j < tblTitleMission.Rows.Count; j++)
                    {
                        if(tblTitleMission.Rows[j]["Reward_Title_ID"].ToString() == "0")
                        {
                            continue;
                        }
                        DataRow dr = tblTitle.Rows.Find(tblTitleMission.Rows[j]["Reward_Title_ID"].ToString());
                        if (tblTitleMission.Rows[j]["Title Name"].ToString() != dr["Title_Name"].ToString())
                        {
                            tblTitleMission.Rows[j]["Title Name"] = dr["Title_Name"];
                            Adpt_TitleMission.Update(new DataRow[] { tblTitleMission.Rows[j] });
                            nCount++;
                        }
                    }
                }
            }

            MessageBox.Show("TitleMission 틀린 개수는 " + nCount + "개의 잘못된 링크를 수정하였습니다.");
            nCount = 0;

            for (int i = 0; i < tblSubTitleMission.Columns.Count; i++)
            {
                if (tblSubTitleMission.Columns[i].ColumnName == "Clear_Type_Idx")
                {
                    for (int j = 0; j < tblSubTitleMission.Rows.Count; j++)
                    {
                        DataRow dr = tblMissionClearType.Rows.Find(tblSubTitleMission.Rows[j]["Clear_Type_Idx"].ToString());
                        if (tblSubTitleMission.Rows[j]["Clear_Type_"].ToString() != dr["CODE"].ToString())
                        {
                            tblSubTitleMission.Rows[j]["Clear_Type_"] = dr["CODE"];
                            Adpt_SubTitleMission.Update(new DataRow[] { tblSubTitleMission.Rows[j] });
                            nCount++;
                        }
                    }
                }


                if (tblSubTitleMission.Columns[i].ColumnName == "NPC ID")
                {
                    for (int j = 0; j < tblSubTitleMission.Rows.Count; j++)
                    {
                        if(tblSubTitleMission.Rows[j]["NPC ID"].ToString() == "0")
                        {
                            continue;
                        }
                        DataRow dr = tblNpcID.Rows.Find(tblSubTitleMission.Rows[j]["NPC ID"].ToString());
                        if (tblSubTitleMission.Rows[j]["Clear Condition Kill NPC ID"].ToString() != dr["CODE"].ToString())
                        {
                            tblSubTitleMission.Rows[j]["Clear Condition Kill NPC ID"] = dr["CODE"];
                            Adpt_SubTitleMission.Update(new DataRow[] { tblSubTitleMission.Rows[j] });
                            nCount++;
                        }
                    }
                }
            }

            MessageBox.Show("SubTitleMission 틀린 개수는 " + nCount + "개의 잘못된 링크를 수정하였습니다.");
            nCount = 0;

        }

        private void dataGridView1_RowStateChanged(object sender, DataGridViewRowStateChangedEventArgs e)
        {
            if (dataGridView1.SelectedRows.Count > 0)
            {
                toolTip1.SetToolTip(dataGridView1, dataGridView1.SelectedRows.Count.ToString() + "R");
            }
        }

        private void dataGridView2_RowStateChanged(object sender, DataGridViewRowStateChangedEventArgs e)
        {
            if (dataGridView2.SelectedRows.Count > 0)
            {
                toolTip1.SetToolTip(dataGridView2, dataGridView2.SelectedRows.Count.ToString() + "R");
            }
        }

        private void dataGridView3_RowStateChanged(object sender, DataGridViewRowStateChangedEventArgs e)
        {
            if (dataGridView3.SelectedRows.Count > 0)
            {
                toolTip1.SetToolTip(dataGridView3, dataGridView3.SelectedRows.Count.ToString() + "R");
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
                            if (tblBorder.Rows[i]["IDNum"].ToString() == dataGridView1["TitleID", dataGridView1.CurrentCell.RowIndex].Value.ToString())
                            {
                                nRow = i;
                                break;
                            }
                        }

                        if (nRow == -1)
                        {
                            tblBorder.Rows.Add();
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["TableName"] = "TitleTable";
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["IDNum"] = dataGridView1["TitleID", dataGridView1.CurrentCell.RowIndex].Value;
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["Regdate"] = DateTime.Now;
                        }
                        else
                        {
                            tblBorder.Rows[nRow].Delete();
                        }

                        dataGridView1.Refresh();
                    }
                    break;
                case 1:
                    {
                        for (int i = 0; i < tblBorder.Rows.Count; i++)
                        {
                            if (tblBorder.Rows[i].RowState == DataRowState.Deleted)
                            {
                                continue;
                            }
                            if (tblBorder.Rows[i]["IDNum"].ToString() == dataGridView2["ID", dataGridView2.CurrentCell.RowIndex].Value.ToString())
                            {
                                nRow = i;
                                break;
                            }
                        }

                        if (nRow == -1)
                        {
                            tblBorder.Rows.Add();
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["TableName"] = "TitleMission";
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["IDNum"] = dataGridView2["ID", dataGridView2.CurrentCell.RowIndex].Value;
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["Regdate"] = DateTime.Now;
                        }
                        else
                        {
                            tblBorder.Rows[nRow].Delete();
                        }

                        dataGridView2.Refresh();
                    }
                    break;
                case 2:
                    {
                        for (int i = 0; i < tblBorder.Rows.Count; i++)
                        {
                            if (tblBorder.Rows[i].RowState == DataRowState.Deleted)
                            {
                                continue;
                            }
                            if (tblBorder.Rows[i]["IDNum"].ToString() == dataGridView3["ID", dataGridView3.CurrentCell.RowIndex].Value.ToString())
                            {
                                nRow = i;
                                break;
                            }
                        }

                        if (nRow == -1)
                        {
                            tblBorder.Rows.Add();
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["TableName"] = "SubTitleMission";
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["IDNum"] = dataGridView3["ID", dataGridView3.CurrentCell.RowIndex].Value;
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["Regdate"] = DateTime.Now;
                        }
                        else
                        {
                            tblBorder.Rows[nRow].Delete();
                        }

                        dataGridView3.Refresh();

                    }
                    break;
            }
        }

        private void dataGridView1_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            for (int i = 0; i < tblBorder.Rows.Count; i++)
            {
                if (tblBorder.Rows[i].RowState == DataRowState.Deleted || tblBorder.Rows[i]["TableName"].ToString() != "TitleTable")
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

                    int nDgv = Convert.ToInt32(dataGridView1["TitleID", nMid].Value.ToString());
                    int nColorID = Convert.ToInt32(drTemp["IDNum"].ToString());
                    if (nDgv == nColorID)
                    {
                        nRow = dataGridView1["TitleID", nMid].RowIndex;
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

        private void dataGridView2_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            for (int i = 0; i < tblBorder.Rows.Count; i++)
            {
                if (tblBorder.Rows[i].RowState == DataRowState.Deleted || tblBorder.Rows[i]["TableName"].ToString() != "TitleMission")
                {
                    continue;
                }
                int nRow = -1;

                DataRow drTemp = tblBorder.Rows[i];

                int nLeft = 0;
                int nRight = dataGridView2.RowCount - 1;
                while (nLeft <= nRight)
                {
                    int nMid = (nLeft + nRight) / 2;

                    int nDgv = Convert.ToInt32(dataGridView2["ID", nMid].Value.ToString());
                    int nColorID = Convert.ToInt32(drTemp["IDNum"].ToString());
                    if (nDgv == nColorID)
                    {
                        nRow = dataGridView2["ID", nMid].RowIndex;
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

        private void dataGridView3_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            for (int i = 0; i < tblBorder.Rows.Count; i++)
            {
                if (tblBorder.Rows[i].RowState == DataRowState.Deleted || tblBorder.Rows[i]["TableName"].ToString() != "SubTitleMission")
                {
                    continue;
                }
                int nRow = -1;

                DataRow drTemp = tblBorder.Rows[i];

                int nLeft = 0;
                int nRight = dataGridView3.RowCount - 1;
                while (nLeft <= nRight)
                {
                    int nMid = (nLeft + nRight) / 2;

                    int nDgv = Convert.ToInt32(dataGridView3["ID", nMid].Value.ToString());
                    int nColorID = Convert.ToInt32(drTemp["IDNum"].ToString());
                    if (nDgv == nColorID)
                    {
                        nRow = dataGridView3["ID", nMid].RowIndex;
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
            if(e.KeyCode == Keys.Escape || e.KeyCode == Keys.Enter)
            {                
                AssistBox.Hide();                
            }
        }
        
    }
}