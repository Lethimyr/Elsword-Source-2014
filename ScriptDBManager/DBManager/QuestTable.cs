//09. 06. 24 김정협 나라별 define
// ItemTable.cs, QuestTable.cs, TitleTable.cs SkillTable.cs 네군대 정의를
// DBManager 부분 추가 접속할때 
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
    public partial class QuestTable : Form
    {

        

        private SqlConnection Con;
        private SqlDataAdapter Adpt_QuestTable;
        DataTable tblQuest;
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

        //QuestTableHistory용        
        private SqlDataAdapter Adpt_QuestTable_History;
        DataTable tblQuest_History;

        //QuestTableDelete용        
        private SqlDataAdapter Adpt_QuestTable_DeleteHistory;
        DataTable tblQuest_DeleteHistory;

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

        //SubQuest 용
        private SqlDataAdapter Adpt_SubQuestTable;
        DataTable tblSubQuest;
        //SubQuest History용        
        private SqlDataAdapter Adpt_SubQuestTable_History;
        DataTable tblSubQuest_History;
        //SubQuesDelete용        
        private SqlDataAdapter Adpt_SubQuestTable_DeleteHistory;
        DataTable tblSubQuest_DeleteHistory;

        //09. 06. 06 김정협
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


        public QuestTable()
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
            ScriptData_TWDataSetTableAdapters.QuestTableTableAdapter Adt_QuestTableTW = null;
            DataTable tblQuestTW = null;
            ScriptData_TWDataSetTableAdapters.QuestTable_HistoryTableAdapter Adt_QuestTableTW_History = null;
            DataTable tblQuestTW_History = null;
            ScriptData_TWDataSetTableAdapters.QuestTable_DeleteHistoryTableAdapter Adpt_QuestTableTW_DeleteHistory = null;
            DataTable tblQuestTW_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_QuestTableTW = new ScriptData_TWDataSetTableAdapters.QuestTableTableAdapter();

                tblQuestTW = Adt_QuestTableTW.GetData();

                tblQuestTW.Columns["IndexKey"].AllowDBNull = true;
                tblQuestTW.Columns["ID"].AllowDBNull = true;
                tblQuestTW.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(0))
                {
                    Adt_QuestTableTW_History = new ScriptData_TWDataSetTableAdapters.QuestTable_HistoryTableAdapter();

                    tblQuestTW_History = Adt_QuestTableTW_History.GetData();

                    tblQuestTW_History.Columns["IndexKey"].AllowDBNull = true;
                    tblQuestTW_History.Columns["ID"].AllowDBNull = true;
                    tblQuestTW_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_QuestTableTW_DeleteHistory = new ScriptData_TWDataSetTableAdapters.QuestTable_DeleteHistoryTableAdapter();

                    tblQuestTW_DeleteHistory = Adpt_QuestTableTW_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblQuestTW_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblQuestTW_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_TWDataSetTableAdapters.SubQuestTableTableAdapter Adt_SubQuestTableTW = null;
            DataTable tblSubQuestTW = null;
            ScriptData_TWDataSetTableAdapters.SubQuestTable_HistoryTableAdapter Adt_SubQuestTableTW_History = null;
            DataTable tblSubQuestTW_History = null;
            ScriptData_TWDataSetTableAdapters.SubQuestTable_DeleteHistoryTableAdapter Adpt_SubQuestTableTW_DeleteHistory = null;
            DataTable tblSubQuestTW_DeleteHistory = null;
            if (arModifyTabIndex.Contains(1) || arDeleteTabIndex.Contains(1))
            {
                Adt_SubQuestTableTW = new ScriptData_TWDataSetTableAdapters.SubQuestTableTableAdapter();

                tblSubQuestTW = Adt_SubQuestTableTW.GetData();

                tblSubQuestTW.Columns["IndexKey"].AllowDBNull = true;
                tblSubQuestTW.Columns["ID"].AllowDBNull = true;
                tblSubQuestTW.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(1))
                {
                    Adt_SubQuestTableTW_History = new ScriptData_TWDataSetTableAdapters.SubQuestTable_HistoryTableAdapter();

                    tblSubQuestTW_History = Adt_SubQuestTableTW_History.GetData();

                    tblSubQuestTW_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSubQuestTW_History.Columns["ID"].AllowDBNull = true;
                    tblSubQuestTW_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(1))
                {
                    Adpt_SubQuestTableTW_DeleteHistory = new ScriptData_TWDataSetTableAdapters.SubQuestTable_DeleteHistoryTableAdapter();

                    tblSubQuestTW_DeleteHistory = Adpt_SubQuestTableTW_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblSubQuestTW_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSubQuestTW_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }

            ///////////////////////////////////////////////////////////////////////////
            //홍콩버전
            ///////////////////////////////////////////////////////////////////////////
            ScriptData_HKDataSetTableAdapters.QuestTableTableAdapter Adt_QuestTableHK = null;
            DataTable tblQuestHK = null;
            ScriptData_HKDataSetTableAdapters.QuestTable_HistoryTableAdapter Adt_QuestTableHK_History = null;
            DataTable tblQuestHK_History = null;
            ScriptData_HKDataSetTableAdapters.QuestTable_DeleteHistoryTableAdapter Adpt_QuestTableHK_DeleteHistory = null;
            DataTable tblQuestHK_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_QuestTableHK = new ScriptData_HKDataSetTableAdapters.QuestTableTableAdapter();

                tblQuestHK = Adt_QuestTableHK.GetData();

                tblQuestHK.Columns["IndexKey"].AllowDBNull = true;
                tblQuestHK.Columns["ID"].AllowDBNull = true;
                tblQuestHK.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(0))
                {
                    Adt_QuestTableHK_History = new ScriptData_HKDataSetTableAdapters.QuestTable_HistoryTableAdapter();

                    tblQuestHK_History = Adt_QuestTableHK_History.GetData();

                    tblQuestHK_History.Columns["IndexKey"].AllowDBNull = true;
                    tblQuestHK_History.Columns["ID"].AllowDBNull = true;
                    tblQuestHK_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_QuestTableHK_DeleteHistory = new ScriptData_HKDataSetTableAdapters.QuestTable_DeleteHistoryTableAdapter();

                    tblQuestHK_DeleteHistory = Adpt_QuestTableHK_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblQuestHK_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblQuestHK_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_HKDataSetTableAdapters.SubQuestTableTableAdapter Adt_SubQuestTableHK = null;
            DataTable tblSubQuestHK = null;
            ScriptData_HKDataSetTableAdapters.SubQuestTable_HistoryTableAdapter Adt_SubQuestTableHK_History = null;
            DataTable tblSubQuestHK_History = null;
            ScriptData_HKDataSetTableAdapters.SubQuestTable_DeleteHistoryTableAdapter Adpt_SubQuestTableHK_DeleteHistory = null;
            DataTable tblSubQuestHK_DeleteHistory = null;
            if (arModifyTabIndex.Contains(1) || arDeleteTabIndex.Contains(1))
            {
                Adt_SubQuestTableHK = new ScriptData_HKDataSetTableAdapters.SubQuestTableTableAdapter();

                tblSubQuestHK = Adt_SubQuestTableHK.GetData();

                tblSubQuestHK.Columns["IndexKey"].AllowDBNull = true;
                tblSubQuestHK.Columns["ID"].AllowDBNull = true;
                tblSubQuestHK.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(1))
                {
                    Adt_SubQuestTableHK_History = new ScriptData_HKDataSetTableAdapters.SubQuestTable_HistoryTableAdapter();

                    tblSubQuestHK_History = Adt_SubQuestTableHK_History.GetData();

                    tblSubQuestHK_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSubQuestHK_History.Columns["ID"].AllowDBNull = true;
                    tblSubQuestHK_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(1))
                {
                    Adpt_SubQuestTableHK_DeleteHistory = new ScriptData_HKDataSetTableAdapters.SubQuestTable_DeleteHistoryTableAdapter();

                    tblSubQuestHK_DeleteHistory = Adpt_SubQuestTableHK_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblSubQuestHK_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSubQuestHK_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }

                }
            }


            ///////////////////////////////////////////////////////////////////////////
            //일본버전
            ///////////////////////////////////////////////////////////////////////////
            ScriptData_JPDataSetTableAdapters.QuestTableTableAdapter Adt_QuestTableJP = null;
            DataTable tblQuestJP = null;
            ScriptData_JPDataSetTableAdapters.QuestTable_HistoryTableAdapter Adt_QuestTableJP_History = null;
            DataTable tblQuestJP_History = null;
            ScriptData_JPDataSetTableAdapters.QuestTable_DeleteHistoryTableAdapter Adpt_QuestTableJP_DeleteHistory = null;
            DataTable tblQuestJP_DeleteHistory = null;
            if (arModifyTabIndex.Contains(0) || arDeleteTabIndex.Contains(0))
            {
                Adt_QuestTableJP = new ScriptData_JPDataSetTableAdapters.QuestTableTableAdapter();

                tblQuestJP = Adt_QuestTableJP.GetData();

                tblQuestJP.Columns["IndexKey"].AllowDBNull = true;
                tblQuestJP.Columns["ID"].AllowDBNull = true;
                tblQuestJP.Columns["Regdate"].AllowDBNull = true;


                if (arModifyTabIndex.Contains(0))
                {
                    Adt_QuestTableJP_History = new ScriptData_JPDataSetTableAdapters.QuestTable_HistoryTableAdapter();

                    tblQuestJP_History = Adt_QuestTableJP_History.GetData();

                    tblQuestJP_History.Columns["IndexKey"].AllowDBNull = true;
                    tblQuestJP_History.Columns["ID"].AllowDBNull = true;
                    tblQuestJP_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(0))
                {
                    Adpt_QuestTableJP_DeleteHistory = new ScriptData_JPDataSetTableAdapters.QuestTable_DeleteHistoryTableAdapter();

                    tblQuestJP_DeleteHistory = Adpt_QuestTableJP_DeleteHistory.GetData();
                    for (int nTemp = 0; nTemp < tblQuestJP_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblQuestJP_DeleteHistory.Columns[nTemp].AllowDBNull = true;
                    }
                }
            }

            ScriptData_JPDataSetTableAdapters.SubQuestTableTableAdapter Adt_SubQuestTableJP = null;
            DataTable tblSubQuestJP = null;
            ScriptData_JPDataSetTableAdapters.SubQuestTable_HistoryTableAdapter Adt_SubQuestTableJP_History = null;
            DataTable tblSubQuestJP_History = null;
            ScriptData_JPDataSetTableAdapters.SubQuestTable_DeleteHistoryTableAdapter Adpt_SubQuestTableJP_DeleteHistory = null;
            DataTable tblSubQuestJP_DeleteHistory = null;
            if (arModifyTabIndex.Contains(1) || arDeleteTabIndex.Contains(1))
            {
                Adt_SubQuestTableJP = new ScriptData_JPDataSetTableAdapters.SubQuestTableTableAdapter();

                tblSubQuestJP = Adt_SubQuestTableJP.GetData();

                tblSubQuestJP.Columns["IndexKey"].AllowDBNull = true;
                tblSubQuestJP.Columns["ID"].AllowDBNull = true;
                tblSubQuestJP.Columns["Regdate"].AllowDBNull = true;

                if (arModifyTabIndex.Contains(1))
                {
                    Adt_SubQuestTableJP_History = new ScriptData_JPDataSetTableAdapters.SubQuestTable_HistoryTableAdapter();

                    tblSubQuestJP_History = Adt_SubQuestTableJP_History.GetData();

                    tblSubQuestJP_History.Columns["IndexKey"].AllowDBNull = true;
                    tblSubQuestJP_History.Columns["ID"].AllowDBNull = true;
                    tblSubQuestJP_History.Columns["Regdate"].AllowDBNull = true;
                }
                if (arDeleteTabIndex.Contains(1))
                {
                    Adpt_SubQuestTableJP_DeleteHistory = new ScriptData_JPDataSetTableAdapters.SubQuestTable_DeleteHistoryTableAdapter();

                    tblSubQuestJP_DeleteHistory = Adpt_SubQuestTableJP_DeleteHistory.GetData();

                    for (int nTemp = 0; nTemp < tblSubQuestJP_DeleteHistory.Columns.Count; nTemp++)
                    {
                        tblSubQuestJP_DeleteHistory.Columns[nTemp].AllowDBNull = true;
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

                            tblQuest_History.Rows.Add();

                            Row_History = tblQuest_History.Rows[tblQuest_History.Rows.Count - 1];
                            //09. 06. 01 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey전부 검색 중간 삭제시 인덱스 어긋나기 때문
                            for (int nRow = 0; nRow < tblQuest.Rows.Count; nRow++)
                            {
                                if (tblQuest.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblQuest.Rows[nRow]["IndexKey"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblQuest.Rows[nRow];
                                    break;
                                }
                            }

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblQuest.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];

                            Adpt_QuestTable_History.Update(new DataRow[] { Row_History });
                            Adpt_QuestTable.Update(new DataRow[] { Row });


                            //09. 06. 19 김정협
                            //국가별 #define처리 해 줄 것
                            // 이부분은 한국일경우만 실행하게 함. 한국에서 추가하면 모두 추가됨
#if __COUNTRY_KR__
                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblQuestTW.Rows.Add();
                            tblQuestTW_History.Rows.Add();
                            DataRow RowTW_History = tblQuestTW_History.Rows[tblQuestTW_History.Rows.Count - 1];

                            int nRowIndex = -1;
                            for (int twRow = 0; twRow < tblQuestTW.Rows.Count; twRow++)
                            {
                                if (tblQuestTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
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
                                Adpt_QuestTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblQuestTW.Columns.Count; nTemp++)
                                {
                                    tblQuestTW.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowTW = null;// = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1];
                                tblQuestTW.Rows.Add();
                                RowTW = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1];

                                for (int twColumn = 0; twColumn < tblQuestTW.Columns.Count; twColumn++)
                                {
                                    /*if (tblQuestTW.Columns[twColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Title";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1]["Title"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowTW["Index_Name"] = nIndexCount;
                                //nIndexCount++;

                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "TW";
                                RowTrans["DB"] = "ScriptData_TW";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Main Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1]["Main Text"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowTW["Index_Description"] = nIndexCount;
                                // nIndexCount++;


                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "TW";
                                RowTrans["DB"] = "ScriptData_TW";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Thanks Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1]["Thanks Text"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "TW";
                                RowTrans["DB"] = "ScriptData_TW";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Diss Clear Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1]["Diss Clear Text"];
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
                                RowRelease["TableName"] = "QuestTable";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["Name"] = tblQuestTW.Rows[tblQuestTW.Rows.Count - 1]["Title"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = DateTime.Now;
                                RowRelease["Last Update Date"] = DateTime.Now;


                                for (int twColumn_His = 0; twColumn_His < tblQuestTW_History.Columns.Count; twColumn_His++)
                                {
                                    RowTW_History[twColumn_His] = Row_History[twColumn_His];
                                }

                                Adt_QuestTableTW.Update(RowTW);
                                Adt_QuestTableTW_History.Update(RowTW_History);


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

                                int nCurrent = (int)Adt_QuestTableTW.GetMaxIndexRow();
                                if (nBefore != nCurrent)
                                {
                                    Adpt_Trans.UpdateIndexKey(nCurrent, nBefore,"QuestTable");
                                    Adpt_Release.UpdateIndexKey(nCurrent, nBefore, "QuestTable");
                                }

                            }
                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            //tblQuestHK.Rows.Add();
                            tblQuestHK_History.Rows.Add();
                            DataRow RowHK_History = tblQuestHK_History.Rows[tblQuestHK_History.Rows.Count - 1];

                            

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            
                            if (nRowIndex == -1)
                            {
                                //tblQuest.Clear();
                                //Adpt_QuestTable.Fill(tblQuest);
                                //Row = tblQuest.Rows[tblQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_QuestTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblQuestHK.Columns.Count; nTemp++)
                                {
                                    tblQuestHK.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowHK = null;// = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1];
                                tblQuestHK.Rows.Add();
                                RowHK = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1];

                                for (int HKColumn = 0; HKColumn < tblQuestHK.Columns.Count; HKColumn++)
                                {
                                    /*if (tblQuestHK.Columns[HKColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Title";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1]["Title"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowHK["Index_Name"] = nIndexCount;
                                //nIndexCount++;

                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "HK";
                                RowTrans["DB"] = "ScriptData_HK";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Main Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1]["Main Text"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowHK["Index_Description"] = nIndexCount;
                                // nIndexCount++;


                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "HK";
                                RowTrans["DB"] = "ScriptData_HK";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Thanks Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1]["Thanks Text"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "HK";
                                RowTrans["DB"] = "ScriptData_HK";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Diss Clear Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1]["Diss Clear Text"];
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
                                RowRelease["TableName"] = "QuestTable";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["Name"] = tblQuestHK.Rows[tblQuestHK.Rows.Count - 1]["Title"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowRelease["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                for (int HKColumn_His = 0; HKColumn_His < tblQuestHK_History.Columns.Count; HKColumn_His++)
                                {
                                    RowHK_History[HKColumn_His] = Row_History[HKColumn_His];
                                }

                                Adt_QuestTableHK.Update(RowHK);
                                Adt_QuestTableHK_History.Update(RowHK_History);


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
                            tblQuestJP_History.Rows.Add();
                            DataRow RowJP_History = tblQuestJP_History.Rows[tblQuestJP_History.Rows.Count - 1];



                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도

                            if (nRowIndex == -1)
                            {
                                //tblQuest.Clear();
                                //Adpt_QuestTable.Fill(tblQuest);
                                //Row = tblQuest.Rows[tblQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_QuestTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblQuestJP.Columns.Count; nTemp++)
                                {
                                    tblQuestJP.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowJP = null;// = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1];
                                tblQuestJP.Rows.Add();
                                RowJP = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1];

                                for (int JPColumn = 0; JPColumn < tblQuestJP.Columns.Count; JPColumn++)
                                {
                                    /*if (tblQuestJP.Columns[JPColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Title";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1]["Title"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowJP["Index_Name"] = nIndexCount;
                                //nIndexCount++;

                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "JP";
                                RowTrans["DB"] = "ScriptData_JP";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Main Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1]["Main Text"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                //RowJP["Index_Description"] = nIndexCount;
                                // nIndexCount++;


                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "JP";
                                RowTrans["DB"] = "ScriptData_JP";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Thanks Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1]["Thanks Text"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                tblTrans.Rows.Add();
                                RowTrans = tblTrans.Rows[tblTrans.Rows.Count - 1];
                                //Row["TransIDX"] = nIndexCount;
                                RowTrans["CountryCode"] = "JP";
                                RowTrans["DB"] = "ScriptData_JP";
                                RowTrans["TableName"] = "QuestTable";
                                RowTrans["Column"] = "Diss Clear Text";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1]["Diss Clear Text"];
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
                                RowRelease["TableName"] = "QuestTable";
                                RowRelease["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                RowRelease["Name"] = tblQuestJP.Rows[tblQuestJP.Rows.Count - 1]["Title"];
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowRelease["State"] = true;
                                RowRelease["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowRelease["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());


                                for (int JPColumn_His = 0; JPColumn_His < tblQuestJP_History.Columns.Count; JPColumn_His++)
                                {
                                    RowJP_History[JPColumn_His] = Row_History[JPColumn_His];
                                }

                                Adt_QuestTableJP.Update(RowJP);
                                Adt_QuestTableJP_History.Update(RowJP_History);


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

                            tblSubQuest_History.Rows.Add();

                            Row_History = tblSubQuest_History.Rows[tblSubQuest_History.Rows.Count - 1];
                            //09. 06. 01 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey값을 이용. tblItem은 IndexKey값이 로우와 같으므로 
                            for (int nRow = 0; nRow < tblSubQuest.Rows.Count; nRow++)
                            {
                                if (tblSubQuest.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblSubQuest.Rows[nRow]["IndexKey"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblSubQuest.Rows[nRow];
                                    break;
                                }
                            }

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSubQuest.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];



                            Adpt_SubQuestTable_History.Update(new DataRow[] { Row_History });
                            Adpt_SubQuestTable.Update(new DataRow[] { Row });



                            //09. 06. 19 김정협
                            //국가별 #define처리 해 줄 것
                            // 이부분은 한국일경우만 실행하게 함. 한국에서 추가하면 모두 추가됨
#if __COUNTRY_KR__

                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            //////////////////////////////////////////////////////////////////////////
                            //tblSubQuestTW.Rows.Add();
                            tblSubQuestTW_History.Rows.Add();
                            DataRow RowTW_History = tblSubQuestTW_History.Rows[tblSubQuestTW_History.Rows.Count - 1];

                            int nRowIndex = -1;
                            for (int twRow = 0; twRow < tblSubQuestTW.Rows.Count; twRow++)
                            {
                                if (tblSubQuestTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    nRowIndex = twRow;
                                    break;
                                }
                            }

                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            int nBefore = Convert.ToInt32(Row["IndexKey"].ToString());
                            if (nRowIndex == -1)
                            {
                                //tblSubQuest.Clear();
                                //Adpt_SubQuestTable.Fill(tblSubQuest);
                                //Row = tblSubQuest.Rows[tblSubQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SubQuestTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSubQuestTW.Columns.Count; nTemp++)
                                {
                                    tblSubQuestTW.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowTW = null;// = tblSubQuestTW.Rows[tblSubQuestTW.Rows.Count - 1];
                                tblSubQuestTW.Rows.Add();
                                RowTW = tblSubQuestTW.Rows[tblSubQuestTW.Rows.Count - 1];

                                for (int twColumn = 0; twColumn < tblSubQuestTW.Columns.Count; twColumn++)
                                {
                                    /*if (tblSubQuestTW.Columns[twColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "SubQuestTable";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSubQuestTW.Rows[tblSubQuestTW.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int twColumn_His = 0; twColumn_His < tblSubQuestTW_History.Columns.Count; twColumn_His++)
                                {
                                    RowTW_History[twColumn_His] = Row_History[twColumn_His];
                                }

                                Adt_SubQuestTableTW.Update(RowTW);
                                Adt_SubQuestTableTW_History.Update(RowTW_History);

                                //Adpt_String.Update(tblScriptString);
                                //09. 06. 19 김정협 Trans
                                DataRow[] drArray = new DataRow[tblTrans.Rows.Count];
                                tblTrans.Rows.CopyTo(drArray, 0);
                                Adpt_Trans.Update(drArray);


                                /*drArray = new DataRow[tblTrans_History.Rows.Count];
                                tblTrans_History.Rows.CopyTo(drArray, 0);
                                Adpt_Trans_History.Update(drArray);*/



                                int nCurrent = (int)Adt_SubQuestTableTW.GetMaxIndexRow();
                                if (nBefore != nCurrent)
                                {
                                    Adpt_Trans.UpdateIndexKey(nCurrent, nBefore, "SubQuestTable");

                                }
                            }
                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////

                            //tblSubQuestHK.Rows.Add();
                            tblSubQuestHK_History.Rows.Add();
                            DataRow RowHK_History = tblSubQuestHK_History.Rows[tblSubQuestHK_History.Rows.Count - 1];


                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도
                            
                            if (nRowIndex == -1)
                            {
                                //tblSubQuest.Clear();
                                //Adpt_SubQuestTable.Fill(tblSubQuest);
                                //Row = tblSubQuest.Rows[tblSubQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SubQuestTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSubQuestHK.Columns.Count; nTemp++)
                                {
                                    tblSubQuestHK.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowHK = null;// = tblSubQuestHK.Rows[tblSubQuestHK.Rows.Count - 1];
                                tblSubQuestHK.Rows.Add();
                                RowHK = tblSubQuestHK.Rows[tblSubQuestHK.Rows.Count - 1];

                                for (int HKColumn = 0; HKColumn < tblSubQuestHK.Columns.Count; HKColumn++)
                                {
                                    /*if (tblSubQuestHK.Columns[HKColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "SubQuestTable";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSubQuestHK.Rows[tblSubQuestHK.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int HKColumn_His = 0; HKColumn_His < tblSubQuestHK_History.Columns.Count; HKColumn_His++)
                                {
                                    RowHK_History[HKColumn_His] = Row_History[HKColumn_His];
                                }

                                Adt_SubQuestTableHK.Update(RowHK);
                                Adt_SubQuestTableHK_History.Update(RowHK_History);

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

                            //tblSubQuestJP.Rows.Add();
                            tblSubQuestJP_History.Rows.Add();
                            DataRow RowJP_History = tblSubQuestJP_History.Rows[tblSubQuestJP_History.Rows.Count - 1];


                            //새로 항목이 추가된 경우에만 다른 나라 테이블에도

                            if (nRowIndex == -1)
                            {
                                //tblSubQuest.Clear();
                                //Adpt_SubQuestTable.Fill(tblSubQuest);
                                //Row = tblSubQuest.Rows[tblSubQuest.Rows.Count - 1];
                                DataTable dtTemp = new DataTable();
                                Adpt_SubQuestTable.Fill(dtTemp);
                                Row = dtTemp.Rows[dtTemp.Rows.Count - 1];

                                for (int nTemp = 0; nTemp < tblSubQuestJP.Columns.Count; nTemp++)
                                {
                                    tblSubQuestJP.Columns[nTemp].AllowDBNull = true;
                                }
                                DataRow RowJP = null;// = tblSubQuestJP.Rows[tblSubQuestJP.Rows.Count - 1];
                                tblSubQuestJP.Rows.Add();
                                RowJP = tblSubQuestJP.Rows[tblSubQuestJP.Rows.Count - 1];

                                for (int JPColumn = 0; JPColumn < tblSubQuestJP.Columns.Count; JPColumn++)
                                {
                                    /*if (tblSubQuestJP.Columns[JPColumn].ColumnName == "IndexKey")
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
                                RowTrans["TableName"] = "SubQuestTable";
                                RowTrans["Column"] = "Description";
                                RowTrans["Index"] = Convert.ToInt32(dtTemp.Rows[dtTemp.Rows.Count - 1]["IndexKey"].ToString());
                                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                                RowTrans["String"] = tblSubQuestJP.Rows[tblSubQuestJP.Rows.Count - 1]["Description"];
                                RowTrans["Create Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());
                                RowTrans["Last Update Date"] = Convert.ToDateTime(dtTemp.Rows[dtTemp.Rows.Count - 1]["Regdate"].ToString());



                                ///////////////////////////////////////////////////////////////////
                                //스트링 끝
                                ///////////////////////////////////////////////////////////////////


                                for (int JPColumn_His = 0; JPColumn_His < tblSubQuestJP_History.Columns.Count; JPColumn_His++)
                                {
                                    RowJP_History[JPColumn_His] = Row_History[JPColumn_His];
                                }

                                Adt_SubQuestTableJP.Update(RowJP);
                                Adt_SubQuestTableJP_History.Update(RowJP_History);

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
                            tblQuest_DeleteHistory.Rows.Add();

                            Row_History = tblQuest_DeleteHistory.Rows[tblQuest_DeleteHistory.Rows.Count - 1];

                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblQuest.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            Row_History["Regdate"] = DateTime.Now;

                            Adpt_QuestTable.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                            Adpt_QuestTable_DeleteHistory.Update(new DataRow[] { Row_History });


                            //09. 06. 19 김정협
                            //외국쪽 지우는 것
                            //

                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////

                            DataRow drTW = null;

                            DataRow drTW_DeleteHistory = null;

                            tblQuestTW_DeleteHistory.Rows.Add();
                            drTW_DeleteHistory = tblQuestTW_DeleteHistory.Rows[tblQuestTW_DeleteHistory.Rows.Count - 1];

                            for (int twRow = 0; twRow < tblQuestTW.Rows.Count; twRow++)
                            {
                                if (tblQuestTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drTW = tblQuestTW.Rows[twRow];
                                    break;
                                }
                            }

                            for (int twColumn = 0; twColumn < tblQuestTW.Columns.Count - 1; twColumn++)
                            {
                                drTW_DeleteHistory[twColumn] = drTW[twColumn];
                            }
                            drTW_DeleteHistory["UserID"] = m_strID;
                            drTW_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drTW_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_QuestTableTW_DeleteHistory.Update(drTW_DeleteHistory);

                            int nDeleteIndex = Convert.ToInt32(drTW["IndexKey"].ToString());

                            drTW.Delete();
                            Adt_QuestTableTW.Update(drTW);

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drHK = null;

                            DataRow drHK_DeleteHistory = null;

                            tblQuestHK_DeleteHistory.Rows.Add();
                            drHK_DeleteHistory = tblQuestHK_DeleteHistory.Rows[tblQuestHK_DeleteHistory.Rows.Count - 1];

                            for (int HKRow = 0; HKRow < tblQuestHK.Rows.Count; HKRow++)
                            {
                                if (tblQuestHK.Rows[HKRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drHK = tblQuestHK.Rows[HKRow];
                                    break;
                                }
                            }

                            for (int HKColumn = 0; HKColumn < tblQuestHK.Columns.Count - 1; HKColumn++)
                            {
                                drHK_DeleteHistory[HKColumn] = drHK[HKColumn];
                            }
                            drHK_DeleteHistory["UserID"] = m_strID;
                            drHK_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drHK_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_QuestTableHK_DeleteHistory.Update(drHK_DeleteHistory);

                            

                            drHK.Delete();
                            Adt_QuestTableHK.Update(drHK);

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drJP = null;

                            DataRow drJP_DeleteHistory = null;

                            tblQuestJP_DeleteHistory.Rows.Add();
                            drJP_DeleteHistory = tblQuestJP_DeleteHistory.Rows[tblQuestJP_DeleteHistory.Rows.Count - 1];

                            for (int JPRow = 0; JPRow < tblQuestJP.Rows.Count; JPRow++)
                            {
                                if (tblQuestJP.Rows[JPRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drJP = tblQuestJP.Rows[JPRow];
                                    break;
                                }
                            }

                            for (int JPColumn = 0; JPColumn < tblQuestJP.Columns.Count - 1; JPColumn++)
                            {
                                drJP_DeleteHistory[JPColumn] = drJP[JPColumn];
                            }
                            drJP_DeleteHistory["UserID"] = m_strID;
                            drJP_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drJP_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_QuestTableJP_DeleteHistory.Update(drJP_DeleteHistory);



                            drJP.Delete();
                            Adt_QuestTableJP.Update(drJP);


                            Adpt_Trans.DeleteIndexRow(nDeleteIndex, "QuestTable");

                            Adpt_Release.DeleteIndexRow(nDeleteIndex, "QuestTable");
                        }
                        break;

                    case 1:
                        {
                            tblSubQuest_DeleteHistory.Rows.Add();

                            Row_History = tblSubQuest_DeleteHistory.Rows[tblSubQuest_DeleteHistory.Rows.Count - 1];

                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblSubQuest.Columns.Count - 1; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = Row["Regdate"];


                            Adpt_SubQuestTable.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                            Adpt_SubQuestTable_DeleteHistory.Update(new DataRow[] { Row_History });

                            //09. 06. 19 김정협
                            //외국쪽 지우는 것
                            //

                            ///////////////////////////////////////////////////////////////////////////
                            //대만버전
                            ///////////////////////////////////////////////////////////////////////////

                            DataRow drTW = null;

                            DataRow drTW_DeleteHistory = null;

                            tblSubQuestTW_DeleteHistory.Rows.Add();
                            drTW_DeleteHistory = tblSubQuestTW_DeleteHistory.Rows[tblSubQuestTW_DeleteHistory.Rows.Count - 1];

                            for (int twRow = 0; twRow < tblSubQuestTW.Rows.Count; twRow++)
                            {
                                if (tblSubQuestTW.Rows[twRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drTW = tblSubQuestTW.Rows[twRow];
                                    break;
                                }
                            }

                            for (int twColumn = 0; twColumn < tblSubQuestTW.Columns.Count - 1; twColumn++)
                            {
                                drTW_DeleteHistory[twColumn] = drTW[twColumn];
                            }
                            drTW_DeleteHistory["UserID"] = m_strID;
                            drTW_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drTW_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SubQuestTableTW_DeleteHistory.Update(drTW_DeleteHistory);

                            int nDeleteIndex = Convert.ToInt32(drTW["IndexKey"].ToString());

                            drTW.Delete();
                            Adt_SubQuestTableTW.Update(drTW);

                            ///////////////////////////////////////////////////////////////////////////
                            //홍콩버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drHK = null;

                            DataRow drHK_DeleteHistory = null;

                            tblSubQuestHK_DeleteHistory.Rows.Add();
                            drHK_DeleteHistory = tblSubQuestHK_DeleteHistory.Rows[tblSubQuestHK_DeleteHistory.Rows.Count - 1];

                            for (int HKRow = 0; HKRow < tblSubQuestHK.Rows.Count; HKRow++)
                            {
                                if (tblSubQuestHK.Rows[HKRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drHK = tblSubQuestHK.Rows[HKRow];
                                    break;
                                }
                            }

                            for (int HKColumn = 0; HKColumn < tblSubQuestHK.Columns.Count - 1; HKColumn++)
                            {
                                drHK_DeleteHistory[HKColumn] = drHK[HKColumn];
                            }
                            drHK_DeleteHistory["UserID"] = m_strID;
                            drHK_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drHK_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SubQuestTableHK_DeleteHistory.Update(drHK_DeleteHistory);

                            

                            drHK.Delete();
                            Adt_SubQuestTableHK.Update(drHK);

                            ///////////////////////////////////////////////////////////////////////////
                            //일본버전
                            ///////////////////////////////////////////////////////////////////////////
                            DataRow drJP = null;

                            DataRow drJP_DeleteHistory = null;

                            tblSubQuestJP_DeleteHistory.Rows.Add();
                            drJP_DeleteHistory = tblSubQuestJP_DeleteHistory.Rows[tblSubQuestJP_DeleteHistory.Rows.Count - 1];

                            for (int JPRow = 0; JPRow < tblSubQuestJP.Rows.Count; JPRow++)
                            {
                                if (tblSubQuestJP.Rows[JPRow]["IndexKey"].ToString() == Row["IndexKey"].ToString())
                                {
                                    drJP = tblSubQuestJP.Rows[JPRow];
                                    break;
                                }
                            }

                            for (int JPColumn = 0; JPColumn < tblSubQuestJP.Columns.Count - 1; JPColumn++)
                            {
                                drJP_DeleteHistory[JPColumn] = drJP[JPColumn];
                            }
                            drJP_DeleteHistory["UserID"] = m_strID;
                            drJP_DeleteHistory["UserIP"] = strMyIP;
                            //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                            drJP_DeleteHistory["Regdate"] = DateTime.Now;

                            Adpt_SubQuestTableJP_DeleteHistory.Update(drJP_DeleteHistory);



                            drJP.Delete();
                            Adt_SubQuestTableJP.Update(drJP);


                            Adpt_Trans.DeleteIndexRow(nDeleteIndex, "SubQuestTable");

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

            tblQuest.Clear();
            Adpt_QuestTable.Fill(tblQuest);

            tblSubQuest.Clear();
            Adpt_SubQuestTable.Fill(tblSubQuest);

            SetCellBackgroundColorForDB(-1);

            m_DeleteRowList.Clear();
            m_ModifyIDList.Clear();
            m_UndoStack.Clear();
            m_RedoStack.Clear();
            m_bSaveEnd = true;


            
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["Title", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 1;
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
            {
                dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["Clear Type", nRowHeader].Value.ToString();
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
            }
        }

        QuestTableAdd frmQuestTable = new QuestTableAdd();
        SubQuestTableAdd frmSubQuestTable = new SubQuestTableAdd();

        private void btnAdd_Click(object sender, EventArgs e)
        {
            switch (tabControl1.SelectedIndex)
            {
                case 0:
                    {
                        if (!frmQuestTable.Visible)
                        {
                            frmQuestTable = new QuestTableAdd();
                        }
                        frmQuestTable.SetDataGridView(dataGridView1);
                        frmQuestTable.SetRowIndex(dataGridView1.CurrentCell.RowIndex);
                        frmQuestTable.SetDataTable(tblQuest);
                        frmQuestTable.SetModifyArray(m_ModifyIDList);
                        frmQuestTable.SetTrans(tblTrans);
                        frmQuestTable.SetRelease(tblRelease);
                        frmQuestTable.SetUndoStack(m_UndoStack);
                        frmQuestTable.SetRedoStack(m_RedoStack);

                        //09. 06. 06 김정협 자동변환으로 인해 추가
                        frmQuestTable.SetItem(tblItem);

                        frmQuestTable.Owner = this;
                        if (!frmQuestTable.Visible)
                        {
                            //frm.SetTableNew();
                            frmQuestTable.Show(this);
                        }
                        else
                        {
                            frmQuestTable.ADDItem();// ItemAdd_Load(null, null);
                        }
                    }
                    break;
                case 1:
                    {
                        if (!frmSubQuestTable.Visible)
                        {
                            frmSubQuestTable = new SubQuestTableAdd();
                        }
                        frmSubQuestTable.SetDataGridView(dataGridView2);
                        frmSubQuestTable.SetRowIndex(dataGridView2.CurrentCell.RowIndex);
                        frmSubQuestTable.SetDataTable(tblSubQuest);
                        frmSubQuestTable.SetModifyArray(m_ModifyIDList);
                        frmSubQuestTable.SetTrans(tblTrans);
                        frmSubQuestTable.SetUndoStack(m_UndoStack);
                        frmSubQuestTable.SetRedoStack(m_RedoStack);

                        //09. 06. 06 김정협 자동변환으로 인해 추가
                        frmSubQuestTable.SetItem(tblItem);

                        frmSubQuestTable.Owner = this;
                        if (!frmSubQuestTable.Visible)
                        {
                            //frm.SetTableNew();
                            frmSubQuestTable.Show(this);
                        }
                        else
                        {
                            frmSubQuestTable.ADDItem();// ItemAdd_Load(null, null);
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
                if (tblColor.Rows[i].RowState == DataRowState.Deleted)
                {
                    continue;
                }

                DataRow drTemp = tblColor.Rows[i];
                int nColumn = Convert.ToInt32(drTemp["Column"].ToString());
                int nARGB = Convert.ToInt32(drTemp["Color"].ToString());

                if(nARGB == 0)
                {
                    tblColor.Rows[i].Delete();
                    continue;
                }

                int nRow = -1;

                if (drTemp["TableName"].ToString() == "QuestTable" && (nSelect == 0 || nSelect == -1))
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
                else if (drTemp["TableName"].ToString() == "SubQuestTable" && (nSelect == 1 || nSelect == -1))
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
            }
            if (nSelect == -1)
            {
                tabControl1.SelectedIndex = 0;
            }
        }


        private void QuestTable_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();


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

            //QuestTable
            Adpt_QuestTable = new SqlDataAdapter("SELECT * FROM QuestTable", Con);
            tblQuest = new DataTable("tblQuest");

            SqlCommandBuilder Builder_Quest = new SqlCommandBuilder(Adpt_QuestTable);
            Adpt_QuestTable.Fill(tblQuest);
            dataGridView1.DataSource = tblQuest;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView1.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView1.Columns["Regdate"].ReadOnly = true; //Reg_data수정방지

            dataGridView1.Columns["name1"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["name2"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["name3"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["name1_1"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["name2_1"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["name3_1"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["name4_1"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["name5_1"].ReadOnly = true; //Reg_data수정방지

            dataGridView1.Columns["name1"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["name2"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["name3"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["name1_1"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["name2_1"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["name3_1"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["name4_1"].DefaultCellStyle.BackColor = Color.LightGray;
            dataGridView1.Columns["name5_1"].DefaultCellStyle.BackColor = Color.LightGray;

            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }
            DataGridViewColumn Column_1 = dataGridView1.Columns[1];//dataGridView1.Columns["ID"];
            ListSortDirection direction_1 = ListSortDirection.Ascending;
            dataGridView1.Sort(Column_1, direction_1);

            Adpt_QuestTable_History = new SqlDataAdapter("SELECT * FROM QuestTable_History", Con);
            tblQuest_History = new DataTable("tblQuest_History");
            SqlCommandBuilder Builder_Quest_History = new SqlCommandBuilder(Adpt_QuestTable_History);
            Adpt_QuestTable_History.Fill(tblQuest_History);

            Adpt_QuestTable_DeleteHistory = new SqlDataAdapter("SELECT * FROM QuestTable_DeleteHistory", Con);
            tblQuest_DeleteHistory = new DataTable("tblQuest_DeleteHistory");
            SqlCommandBuilder Builder_Quest_DeleteHistory = new SqlCommandBuilder(Adpt_QuestTable_DeleteHistory);
            Adpt_QuestTable_DeleteHistory.Fill(tblQuest_DeleteHistory);

            //09. 06. 19 김정협 Trans/Release 로드
            //Trans
            //Adpt_String = new SqlDataAdapter("SELECT * FROM ScriptStringTable", Con);
            tblTrans = Adpt_Trans.GetData();
            tblTrans_History = Adpt_Trans_History.GetData();
            //Release
            tblRelease = Adpt_Release.GetData();
            tblRelease_History = Adpt_Release_History.GetData();


            //SubQuestTable
            Adpt_SubQuestTable = new SqlDataAdapter("SELECT * FROM SubQuestTable", Con);
            tblSubQuest = new DataTable("tblSubQuest");

            SqlCommandBuilder Builder_SubQuest = new SqlCommandBuilder(Adpt_SubQuestTable);
            Adpt_SubQuestTable.Fill(tblSubQuest);
            dataGridView2.DataSource = tblSubQuest;
            dataGridView2.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView2.Columns[0].ReadOnly = true; //대리키수정방지
            dataGridView2.Columns["Regdate"].ReadOnly = true; //Reg_data수정방지

            dataGridView2.Columns["Name"].ReadOnly = true; //Reg_data수정방지

            dataGridView2.Columns["Name"].DefaultCellStyle.BackColor = Color.LightGray;
            for (int i = 0; i < dataGridView2.ColumnCount; i++)
            {
                dataGridView2.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }

            //09. 06. 01 김정협 수정 : 아이디로 오름차순 정렬
            DataGridViewColumn Column_2 = dataGridView2.Columns[1];//dataGridView1.Columns["ID"];
            ListSortDirection direction_2 = ListSortDirection.Ascending;
            dataGridView2.Sort(Column_2, direction_2);

            Adpt_SubQuestTable_History = new SqlDataAdapter("SELECT * FROM SubQuestTable_History", Con);
            tblSubQuest_History = new DataTable("tblSubQuest_History");
            SqlCommandBuilder Builder_SubQuest_History = new SqlCommandBuilder(Adpt_SubQuestTable_History);
            Adpt_SubQuestTable_History.Fill(tblSubQuest_History);

            Adpt_SubQuestTable_DeleteHistory = new SqlDataAdapter("SELECT * FROM SubQuestTable_DeleteHistory", Con);
            tblSubQuest_DeleteHistory = new DataTable("tblSubQuest_DeleteHistory");
            SqlCommandBuilder Builder_SubQuest_DeleteHistory = new SqlCommandBuilder(Adpt_SubQuestTable_DeleteHistory);
            Adpt_SubQuestTable_DeleteHistory.Fill(tblSubQuest_DeleteHistory);

            //09. 06. 06 김정협
            //Item용        
            Adpt_Item = new SqlDataAdapter("SELECT * FROM Item", Con);
            tblItem = new DataTable("tblItem");
            Adpt_Item.Fill(tblItem);
            tblItem.PrimaryKey = new DataColumn[] { tblItem.Columns["ID"] };


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


            

            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["Title", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 1;
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

            for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
            {
                dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["Clear Type", nRowHeader].Value.ToString();
            }

            tabControl1.SelectedIndex = 0;

        }

        private void QuestTable_Resize(object sender, EventArgs e)
        {
            tabControl1.Width = this.Width - 11;
            tabControl1.Height = this.Height - 136;
            dataGridView1.Width = tabControl1.Width - 13;
            dataGridView1.Height = tabControl1.Height - 29;

            dataGridView2.Width = tabControl1.Width - 13;
            dataGridView2.Height = tabControl1.Height - 29;

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
                frm.SetHistoryTable(tblQuest_History);
            }
            else if (tabControl1.SelectedIndex == 1)
            {
                frm.SetHistoryTable(tblSubQuest_History);
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
                        string strFileName = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Major\\Quest.lua";
                        strCountryCode = "KR";
                        sw = new StreamWriter(strFileName, false, Encoding.Default);
                        
#elif __COUNTRY_TW__
                        string strFileName = "..\\..\\..\\resource\\TW\\Trunk\\dat\\Script\\Major\\Quest.lua";
                        strCountryCode = "TW";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_HK__
                        string strFileName = "..\\..\\..\\resource\\HK\\Trunk\\dat\\Script\\Major\\Quest.lua";
                        strCountryCode = "HK";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_JP__
                        string strFileName = "..\\..\\..\\resource\\JP\\Trunk\\dat\\Script\\Major\\Quest.lua";
                        strCountryCode = "JP";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#endif

                        for (int i = 0; i < dataGridView1.RowCount; i++)
                        {
#if __COUNTRY_KR__
                            DataRow dr = Adpt_Release.GetDataByRelease("KR", "QuestTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#elif __COUNTRY_TW__
                            DataRow dr = Adpt_Release.GetDataByRelease("TW", "QuestTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#elif __COUNTRY_HK__
                            DataRow dr = Adpt_Release.GetDataByRelease("HK", "QuestTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#elif __COUNTRY_JP__
                            DataRow dr = Adpt_Release.GetDataByRelease("JP", "QuestTable", Convert.ToInt32(dataGridView1["IndexKey", i].Value.ToString())).Rows[0];
#endif
                            if (dr["State"].ToString() == "False" || dataGridView1["Enable Quest", i].Value.ToString() == "False")
                            {
                                continue;
                            }
                            //sw.Write("\r\n\r\n\r\n");
                            sw.Write("g_pQuestManager:AddQuestTemplet_LUA\r\n");
                            sw.Write("{\r\n");

                            sw.Write("    ");
                            sw.Write("m_iID = ");
                            sw.Write(dataGridView1["ID", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_eQuestType = QUEST_TYPE[\"QT_");
                            sw.Write(dataGridView1["Quest Type", i].Value.ToString());
                            sw.Write("\"],\r\n");

                            sw.Write("    ");
                            sw.Write("CHANGE_JOB_TYPE = CHANGE_JOB_TYPE[\"CJT_JOB_");
                            sw.Write(dataGridView1["Change Job Type", i].Value.ToString());
                            sw.Write("\"],\r\n");

                            sw.Write("    ");
                            sw.Write("m_eStartNPCID = NPC_UNIT_ID[\"NUI_");
                            sw.Write(dataGridView1["Start NPC Name", i].Value.ToString());
                            sw.Write("\"],\r\n");

                            sw.Write("    ");
                            sw.Write("m_eEndNPCID = NPC_UNIT_ID[\"NUI_");
                            sw.Write(dataGridView1["End NPC Name", i].Value.ToString());
                            sw.Write("\"],\r\n");

                            sw.Write("    ");
                            sw.Write("m_wstrTitle = \"");
                            sw.Write(dataGridView1["Title", i].Value.ToString());
                            sw.Write("\",\r\n");
                            sw.Write("    ");
                            sw.Write("m_iFairLevel = ");
                            sw.Write(dataGridView1["Fair Level", i].Value.ToString());
                            sw.Write(",\r\n");

                            if (dataGridView1["Fair Dungeon ID", i].Value.ToString() == "NULL")
                            {
                                sw.Write("    ");
                                sw.Write("m_iFairDungeonID = DUNGEON_ID[\"DI_NONE\"]");
                                sw.Write(",\r\n");
                            }
                            else
                            {
                                sw.Write("    ");
                                sw.Write("m_iFairDungeonID = DUNGEON_ID[\"DI_");
                                sw.Write(dataGridView1["Fair Dungeon ID", i].Value.ToString());
                                sw.Write("\"],\r\n");
                            }

                            sw.Write("    ");
                            sw.Write("m_bRepeat = ");
                            sw.Write(dataGridView1["Quest Repeat", i].Value.ToString());
                            sw.Write(",\r\n");


                            sw.Write("    ");
                            sw.Write("m_wstrMainText = \"");
                            sw.Write(dataGridView1["Main Text", i].Value.ToString());
                            sw.Write("\",\r\n");

                            sw.Write("    ");
                            sw.Write("m_wstrThanksText = \"");
                            sw.Write(dataGridView1["Thanks Text", i].Value.ToString());
                            sw.Write("\",\r\n");

                            sw.Write("    ");
                            sw.Write("m_wstrDissClearText = \"");
                            sw.Write(dataGridView1["Diss Clear Text", i].Value.ToString());
                            sw.Write("\",\r\n");


                            sw.Write("    ");
                            sw.Write("QuestCondition = \r\n");
                            sw.Write("    {\r\n");

                            sw.Write("        ");
                            sw.Write("m_iLevel = ");
                            sw.Write(dataGridView1["Condition Lv", i].Value.ToString());
                            sw.Write(",\r\n");


                            sw.Write("        ");
                            sw.Write("m_eUnitType = UNIT_TYPE[\"UT_");
                            sw.Write(dataGridView1["Condition Unit Type", i].Value.ToString());
                            sw.Write("\"],\r\n");

                            sw.Write("        ");
                            sw.Write("m_eUnitClass = UNIT_CLASS[\"UC_");
                            sw.Write(dataGridView1["Condition Unit Class", i].Value.ToString());
                            sw.Write("\"],\r\n");


                            sw.Write("        ");
                            sw.Write("m_iConditionItemID = ");
                            sw.Write(dataGridView1["Condition Item ID", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("        ");
                            sw.Write("m_iConditionItemNum = ");
                            sw.Write(dataGridView1["Condition Item Quantity", i].Value.ToString());
                            sw.Write(",\r\n");

                            if (Convert.ToInt32(dataGridView1["Condition DisClearQuest ID", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write("m_iNotBeforeQuestID = ");
                                sw.Write(dataGridView1["Condition DisClearQuest ID", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            sw.Write("        ");
                            sw.Write("m_vecBeforeQuestID = \r\n");
                            sw.Write("        {\r\n");
                            sw.Write("            ");

                            int nCountID = 1;
                            string strTableID = "Condition ClearQuest ID " + nCountID;

                            while (nCountID < 8)
                            {

                                if (Convert.ToInt32(dataGridView1[strTableID, i].Value.ToString()) > 0)
                                {
                                    sw.Write(dataGridView1[strTableID, i].Value.ToString());
                                    sw.Write(",");
                                }

                                nCountID = nCountID + 1;
                                strTableID = "Condition ClearQuest ID " + nCountID;

                            }
                            sw.Write("\r\n");
                            sw.Write("        },\r\n");
                            sw.Write("    },\r\n");


                            sw.Write("    ");
                            sw.Write("m_vecSubQuest = \r\n");
                            sw.Write("    {\r\n");

                            if (Convert.ToInt32(dataGridView1["SubQuest1", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["SubQuest1", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["SubQuest2", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["SubQuest2", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["SubQuest3", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["SubQuest3", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["SubQuest4", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["SubQuest4", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            if (Convert.ToInt32(dataGridView1["SubQuest5", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["SubQuest5", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            sw.Write("    },\r\n");

                            sw.Write("    ");
                            sw.Write("Reward = \r\n");
                            sw.Write("    {\r\n");

                            sw.Write("        ");
                            sw.Write("m_bEnable = ");
                            sw.Write(dataGridView1["Reward Enable", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("        ");
                            sw.Write("m_iEXP = ");
                            sw.Write(dataGridView1["Reward EXP", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("        ");
                            sw.Write("m_iED = ");
                            sw.Write(dataGridView1["Reward ED", i].Value.ToString());
                            sw.Write(",\r\n");

                            if (dataGridView1["Reward Grade", i].Value.ToString() != "NULL")
                            {
                                sw.Write("        ");
                                sw.Write(dataGridView1["Reward Grade", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            sw.Write("        ");
                            sw.Write("m_iBuff = ");
                            sw.Write(dataGridView1["Reward Buff", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("        ");
                            sw.Write("m_iSP = ");
                            sw.Write(dataGridView1["Reward SP", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("        ");
                            sw.Write("m_eChangeUnitClass = UNIT_CLASS[\"UC_");
                            sw.Write(dataGridView1["Reward Unit Class", i].Value.ToString());
                            sw.Write("\"],\r\n");

                            if (Convert.ToInt32(dataGridView1["Reward Item1", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write("m_vecItem = \r\n");
                                sw.Write("        {\r\n");
                                int nCount = 1;
                                string strTable = "Reward Item" + nCount;
                                string strTableQ = "Reward ItemQuantity" + nCount;
                                string strPeriod = "Period " + nCount;
                                string strSocketOpt = "Socket Option" + nCount;

                                while (nCount < 4)
                                {
                                    if (Convert.ToInt32(dataGridView1[strTable, i].Value.ToString()) > 0)
                                    {
                                        sw.Write("            {\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_iItemID = ");
                                        sw.Write(dataGridView1[strTable, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_iQuantity = ");
                                        sw.Write(dataGridView1[strTableQ, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_iPeriod = ");
                                        sw.Write(dataGridView1[strPeriod, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_sSocketOption = ");
                                        sw.Write(dataGridView1[strSocketOpt, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("            },\r\n");
                                    }
                                    nCount++;
                                    strTable = "Reward Item" + nCount;
                                    strTableQ = "Reward ItemQuantity" + nCount;
                                    strPeriod = "Period " + nCount;
                                    strSocketOpt = "Socket Option" + nCount;
                                }
                                sw.Write("        },\r\n");
                            }
                            sw.Write("    },\r\n");


                            sw.Write("    ");
                            sw.Write("SelectReward = \r\n");
                            sw.Write("    {\r\n");

                            sw.Write("        ");
                            sw.Write("m_iSelectionCount = ");
                            sw.Write(dataGridView1["Select Reward ItemCount", i].Value.ToString());
                            sw.Write(",\r\n");

                            if (Convert.ToInt32(dataGridView1["Select Reward Item1", i].Value.ToString()) > 0)
                            {
                                sw.Write("        ");
                                sw.Write("m_vecSelectItem = \r\n");
                                sw.Write("        {\r\n");

                                //////////////////////////////

                                int nCount = 1;
                                string strTable = "Select Reward Item" + nCount;
                                string strTableQ = "Select Reward ItemQuantity" + nCount;
                                string strPeriod = "Select Period" + nCount;
                                string strSocketOpt = "Select Socket Option" + nCount;

                                while (nCount < 6)
                                {
                                    if (Convert.ToInt32(dataGridView1[strTable, i].Value.ToString()) > 0)
                                    {
                                        sw.Write("            {\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_iItemID = ");
                                        sw.Write(dataGridView1[strTable, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_iQuantity = ");
                                        sw.Write(dataGridView1[strTableQ, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_iPeriod = ");
                                        sw.Write(dataGridView1[strPeriod, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("                ");
                                        sw.Write("m_sSocketOption = ");
                                        sw.Write(dataGridView1[strSocketOpt, i].Value.ToString());
                                        sw.Write(",\r\n");

                                        sw.Write("            },\r\n");
                                    }
                                    nCount++;
                                    strTable = "Select Reward Item" + nCount;
                                    strTableQ = "Select Reward ItemQuantity" + nCount;
                                    strPeriod = "Select Period" + nCount;
                                    strSocketOpt = "Select Socket Option" + nCount;
                                }


                                /////////////////////////////
                                sw.Write("        },\r\n");
                            }

                            sw.Write("    },\r\n");



                            sw.Write("}\r\n\r\n");
                        }

                        sw.Close();
                        
                        Process Dos = new Process();
                        Dos.StartInfo.FileName = "tortoiseProc";
                        Dos.StartInfo.Arguments = "/Command:commit /path:\"..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\Quest.lua\" /notempfile /closeonend:1";
                        Dos.Start();

                    }
                    break;
                case 1:
                    {
                        StreamWriter sw;
                        //국가별 define정의할 것
#if __COUNTRY_KR__
                        string strFileName = "..\\..\\..\\resource\\KR\\Trunk\\dat\\Script\\Major\\SubQuest.lua";
                        strCountryCode = "KR";
                        sw = new StreamWriter(strFileName, false, Encoding.Default);
#elif __COUNTRY_TW__
                        string strFileName = "..\\..\\..\\resource\\TW\\Trunk\\dat\\Script\\Major\\SubQuest.lua";
                        strCountryCode = "TW";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_HK__
                        string strFileName = "..\\..\\..\\resource\\HK\\Trunk\\dat\\Script\\Major\\SubQuest.lua";
                        strCountryCode = "HK";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#elif __COUNTRY_JP__
                        string strFileName = "..\\..\\..\\resource\\JP\\Trunk\\dat\\Script\\Major\\SubQuest.lua";
                        strCountryCode = "JP";
                        sw = new StreamWriter(strFileName, false, Encoding.UTF8);
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
#endif



                        for (int i = 0; i < dataGridView2.RowCount; i++)
                        {
                            //sw.Write("\r\n\r\n\r\n");
                            sw.Write("g_pQuestManager:AddSubQuestTemplet_LUA\r\n");
                            sw.Write("{\r\n");

                            sw.Write("    ");
                            sw.Write("m_iID = ");
                            sw.Write(dataGridView2["ID", i].Value.ToString());
                            sw.Write(",\r\n");

                            sw.Write("    ");
                            sw.Write("m_wstrDescription = \"");
                            sw.Write(dataGridView2["Description", i].Value.ToString());
                            sw.Write("\",\r\n");

                            sw.Write("    ");
                            sw.Write("m_eClearType = SUB_QUEST_TYPE[\"SQT_");
                            sw.Write(dataGridView2["Clear Type", i].Value.ToString());
                            sw.Write("\"],\r\n");


                            sw.Write("    ");
                            sw.Write("m_bAutomaticDescription = ");
                            sw.Write(dataGridView2["AutomaticDescription", i].Value.ToString());
                            sw.Write(",\r\n");


                            sw.Write("    ");
                            sw.Write("m_ClearCondition = \r\n");
                            sw.Write("    {\r\n");

                            if (dataGridView2["Clear Type", i].Value.ToString() == "NPC_TALK")
                            {
                                sw.Write("        ");
                                sw.Write("m_eTalkNPCID = NPC_UNIT_ID[\"NUI_");
                                sw.Write(dataGridView2["Clear Condition Talk NPC ID", i].Value.ToString());
                                sw.Write("\"],\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "NPC_HUNT")
                            {
                                if (dataGridView2["Clear Condition Kill NPC ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eKillNPCID = ");
#if __COUNTRY_KR__
                                    sw.Write("{ NONE },");
#else
                                    sw.Write("NPC_UNIT_ID[\"NUI_NONE\"],");
#endif
                                    sw.Write("\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");
#if __COUNTRY_KR__
                                    sw.Write("m_eKillNPCID = { ");
                                    sw.Write(dataGridView2["Clear Condition Kill NPC ID", i].Value.ToString());
                                    sw.Write(" },\r\n");
#else
                                    sw.Write("m_eKillNPCID = NPC_UNIT_ID[\"NUI_");
                                    sw.Write(dataGridView2["Clear Condition Kill NPC ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
#endif
                                    
                                }

                                sw.Write("        ");
                                sw.Write("m_iKillNum = ");
                                sw.Write(dataGridView2["Clear Condition Kill NPC Quantity", i].Value.ToString());
                                sw.Write(",\r\n");


                                if (dataGridView2["Clear Condition Dungeon ID", i].Value.ToString() == "NULL")
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
                                    sw.Write(dataGridView2["Clear Condition Dungeon ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }

                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView2["Clear Condition Dungeon Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_bUpperDifficulty = ");
                                sw.Write(dataGridView2["Clear Condition Upper Difficulty", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "QUEST_ITEM_COLLECTION")
                            {
                                if (dataGridView2["Clear Condition Dungeon ID", i].Value.ToString() == "NULL")
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
                                    sw.Write(dataGridView2["Clear Condition Dungeon ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }

                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView2["Clear Condition Dungeon Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_bUpperDifficulty = ");
                                sw.Write(dataGridView2["Clear Condition Upper Difficulty", i].Value.ToString());
                                sw.Write(",\r\n");

                                if (dataGridView2["Clear Condition Kill NPC ID", i].Value.ToString() == "NULL")
                                {
                                    sw.Write("        ");
                                    sw.Write("m_eKillNPCID = ");
#if __COUNTRY_KR__
                                    sw.Write("{ NONE },");
#else
                                    sw.Write("NPC_UNIT_ID[\"NUI_NONE\"],");
#endif
                                    sw.Write("\r\n");
                                }
                                else
                                {
                                    sw.Write("        ");

#if __COUNTRY_KR__
                                    sw.Write("m_eKillNPCID = { ");
                                    sw.Write(dataGridView2["Clear Condition Kill NPC ID", i].Value.ToString());
                                    sw.Write(" },\r\n");
#else
                                    sw.Write("m_eKillNPCID = NPC_UNIT_ID[\"NUI_");
                                    sw.Write(dataGridView2["Clear Condition Kill NPC ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
#endif

                                }

                                sw.Write("        ");
                                sw.Write("m_iCollectionItemID = ");
                                sw.Write(dataGridView2["Clear Condition Collection Item ID", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_iCollectionItemNum = ");
                                sw.Write(dataGridView2["Clear Condition Collection Item Quantity", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_fQuestItemDropRate = ");
                                sw.Write(dataGridView2["Quest Item Drop Rate", i].Value.ToString());
                                sw.Write(",\r\n");

                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "ITEM_COLLECTION")
                            {
                                sw.Write("        ");
                                sw.Write("m_iCollectionItemID = ");
                                sw.Write(dataGridView2["Clear Condition Collection Item ID", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_iCollectionItemNum = ");
                                sw.Write(dataGridView2["Clear Condition Collection Item Quantity", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "DUNGEON_TIME")
                            {
                                if (dataGridView2["Clear Condition Dungeon ID", i].Value.ToString() == "NULL")
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
                                    sw.Write(dataGridView2["Clear Condition Dungeon ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }

                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView2["Clear Condition Dungeon Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_bUpperDifficulty = ");
                                sw.Write(dataGridView2["Clear Condition Upper Difficulty", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_iDungeonClearTime = ");
                                sw.Write(dataGridView2["Clear Condition Dungeon Clear Time", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "DUNGEON_RANK")
                            {
                                if (dataGridView2["Clear Condition Dungeon ID", i].Value.ToString() == "NULL")
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
                                    sw.Write(dataGridView2["Clear Condition Dungeon ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }

                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView2["Clear Condition Dungeon Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_bUpperDifficulty = ");
                                sw.Write(dataGridView2["Clear Condition Upper Difficulty", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_eDungeonRank = ");
                                sw.Write(dataGridView2["Clear Condition Dungeon Rank", i].Value.ToString());
                                sw.Write(",\r\n");

                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "DUNGEON_DAMAGE")
                            {
                                if (dataGridView2["Clear Condition Dungeon ID", i].Value.ToString() == "NULL")
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
                                    sw.Write(dataGridView2["Clear Condition Dungeon ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }

                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView2["Clear Condition Dungeon Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_bUpperDifficulty = ");
                                sw.Write(dataGridView2["Clear Condition Upper Difficulty", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_iDungeonDamage = ");
                                sw.Write(dataGridView2["Clear Condition Dungeon Damage", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "DUNGEON_CLEAR_COUNT")
                            {
                                if (dataGridView2["Clear Condition Dungeon ID", i].Value.ToString() == "NULL")
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
                                    sw.Write(dataGridView2["Clear Condition Dungeon ID", i].Value.ToString());
                                    sw.Write("\"],\r\n");
                                }

                                sw.Write("        ");
                                sw.Write("m_cDifficulty = DIFFICULTY_LEVEL[\"DL_");
                                sw.Write(dataGridView2["Clear Condition Dungeon Difficulty", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_bUpperDifficulty = ");
                                sw.Write(dataGridView2["Clear Condition Upper Difficulty", i].Value.ToString());
                                sw.Write(",\r\n");

                                sw.Write("        ");
                                sw.Write("m_iDungeonClearCount = ");
                                sw.Write(dataGridView2["Clear Condition Dungeon clear count", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "PVP_PLAY")
                            {
                                sw.Write("        ");
                                sw.Write("m_ePVPType = PVP_GAME_TYPE[\"PGT_");
                                sw.Write(dataGridView2["Clear Condition PVP_TYPE", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_iPVPPlay = ");
                                sw.Write(dataGridView2["Clear Condition PVP_PLAY", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "PVP_WIN")
                            {
                                sw.Write("        ");
                                sw.Write("m_ePVPType = PVP_GAME_TYPE[\"PGT_");
                                sw.Write(dataGridView2["Clear Condition PVP_TYPE", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_iPVPWin = ");
                                sw.Write(dataGridView2["Clear Condition PVP_WIN", i].Value.ToString());
                                sw.Write(",\r\n");
                            }
                            else if (dataGridView2["Clear Type", i].Value.ToString() == "PVP_KILL")
                            {
                                sw.Write("        ");
                                sw.Write("m_ePVPType = PVP_GAME_TYPE[\"PGT_");
                                sw.Write(dataGridView2["Clear Condition PVP_TYPE", i].Value.ToString());
                                sw.Write("\"],\r\n");

                                sw.Write("        ");
                                sw.Write("m_iPVPKill = ");
                                sw.Write(dataGridView2["Clear Condition PVP_KILL", i].Value.ToString());
                                sw.Write(",\r\n");
                            }

                            sw.Write("    },\r\n");

                            sw.Write("}\r\n\r\n");
                        }

                        sw.Close();
                        
                        Process Dos = new Process();
                        Dos.StartInfo.FileName = "tortoiseProc";
                        Dos.StartInfo.Arguments = "/Command:commit /path:\"..\\..\\..\\resource\\" + strCountryCode + "\\Trunk\\dat\\Script\\Major\\SubQuest.lua\" /notempfile /closeonend:1";
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

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
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

            for (int i = 1; i <= 3; i++)
            {
                string strRItem = "Reward Item" + i;
                string strItem = "name" + i;

                if (dgvCurrentCell.OwningColumn.Name == strRItem)
                {
                    if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                    {
                        dataGridView1[strItem, nIndexRow].Value = "";
                    }
                    else
                    {
                        DataRow dr = tblItem.Rows.Find(dgvCurrentCell.Value.ToString());
                        if (dr != null)
                        {
                            dataGridView1[strItem, nIndexRow].Value = dr["Name"].ToString();

                        }
                        else
                        {
                            MessageBox.Show("존재하지 않는 값입니다.");
                            dgvCurrentCell.Value = objTemp;
                        }
                    }
                }
            }

            for (int i = 1; i <= 5; i++)
            {
                string strRItem = "Select Reward Item" + i;
                string strItem = "name" + i + "_1";

                if (dgvCurrentCell.OwningColumn.Name == strRItem)
                {
                    if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                    {
                        dataGridView1[strItem, nIndexRow].Value = "";
                    }
                    else
                    {
                        DataRow dr = tblItem.Rows.Find(dgvCurrentCell.Value.ToString());
                        if (dr != null)
                        {
                            dataGridView1[strItem, nIndexRow].Value = dr["Name"].ToString();

                        }
                        else
                        {
                            MessageBox.Show("존재하지 않는 값입니다.");
                            dgvCurrentCell.Value = objTemp;
                        }
                    }
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


                if (dgvCurrentCell.OwningColumn.Name == "Title" || dgvCurrentCell.OwningColumn.Name == "Main Text" ||
                    dgvCurrentCell.OwningColumn.Name == "Thanks Text" || dgvCurrentCell.OwningColumn.Name == "Diss Clear Text")
                {
                    //int nIndex_Name = Convert.ToInt32(dataGridView1["Index_Title", dgvCurrentCell.RowIndex].Value.ToString());
                    //DataRow Row = tblScriptString.Rows[nIndex_Name];
                    //Row["String"] = dgvCurrentCell.Value.ToString();

                    //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여
                    if (dgvCurrentCell.OwningColumn.Name == "Title")
                    {
                        dataGridView1.Rows[dgvCurrentCell.RowIndex].HeaderCell.Value = dataGridView1["ID", dgvCurrentCell.RowIndex].Value.ToString() + "     " + dataGridView1["Title", dgvCurrentCell.RowIndex].Value.ToString();
                    }


                    //09. 06. 23 김정협 국가별 디파일 정의 할 것
#if __COUNTRY_KR__
                    int nIndex_Name = -1;
                    string strCountry = "KR";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "QuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "QuestTable");
                    }

#elif __COUNTRY_TW__
                    int nIndex_Name = -1;
                    string strCountry = "TW";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "QuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()),"TW", objTemp.ToString(), "QuestTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_HK__
                    int nIndex_Name = -1;
                    string strCountry = "HK";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "QuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()),"HK", objTemp.ToString(), "QuestTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_JP__
                    int nIndex_Name = -1;
                    string strCountry = "JP";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "QuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView1["IndexKey", nIndexRow].Value.ToString()),"JP", objTemp.ToString(), "QuestTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
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
                                tblTrans.Rows[i]["TableName"].ToString() == "QuestTable" &&
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

        private void QuestTable_FormClosing(object sender, FormClosingEventArgs e)
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

        private void dataGridView2_CellEndEdit(object sender, DataGridViewCellEventArgs e)
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

            if (dgvCurrentCell.OwningColumn.Name == "Clear Condition Collection Item ID")
            {
                if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                {
                    dataGridView2["Name", nIndexRow].Value = "";
                }
                else
                {
                    DataRow dr = tblItem.Rows.Find(dgvCurrentCell.Value.ToString());
                    if (dr != null)
                    {
                        dataGridView2["Name", nIndexRow].Value = dr["Name"].ToString();

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

                    //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여    
                    for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
                    {
                        dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["Clear Type", nRowHeader].Value.ToString();
                    }
                }

                if (dgvCurrentCell.OwningColumn.Name == "Description")
                {
                    //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여                    
                    dataGridView2.Rows[dgvCurrentCell.RowIndex].HeaderCell.Value = dataGridView2["ID", dgvCurrentCell.RowIndex].Value.ToString() + "     " + dataGridView2["Clear Type", dgvCurrentCell.RowIndex].Value.ToString();


                    //09. 06. 23 김정협 국가별 디파일 정의 할 것
#if __COUNTRY_KR__
                    int nIndex_Name = -1;
                    string strCountry = "KR";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SubQuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "KR", objTemp.ToString(), "SubQuestTable");
                    }
#elif __COUNTRY_TW__
                    int nIndex_Name = -1;
                    string strCountry = "TW";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "TW", objTemp.ToString(), "SubQuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()),"TW", objTemp.ToString(), "SubQuestTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_HK__
                    int nIndex_Name = -1;
                    string strCountry = "HK";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "HK", objTemp.ToString(), "SubQuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()),"HK", objTemp.ToString(), "SubQuestTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
                    }
#elif __COUNTRY_JP__
                    int nIndex_Name = -1;
                    string strCountry = "JP";
                    if (Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()), "JP", objTemp.ToString(), "SubQuestTable") != null)
                    {
                        nIndex_Name = (int)Adpt_Trans.GetTransIDX(Convert.ToInt32(dataGridView2["IndexKey", nIndexRow].Value.ToString()),"JP", objTemp.ToString(), "SubQuestTable");//Convert.ToInt32(dataGridView1["Index_Name", nIndexRow].Value.ToString());
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
                                tblTrans.Rows[i]["TableName"].ToString() == "SubQuestTable" &&
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
                for (int i = 0; i < dataGridView2.ColumnCount; i++)
                {
                    dataGridView2.Columns[i].SortMode = DataGridViewColumnSortMode.Automatic;
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
            }

        }

        private void dataGridView1_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView1[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void dataGridView2_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            textBox1.Text = dataGridView2[e.ColumnIndex, 0].OwningColumn.Name;
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

        //09. 06. 03 김정협 추가 : 셀배경색
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
                                    if (drTemp["TableName"].ToString() == "QuestTable" &&
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
                                    if (drTemp["TableName"].ToString() == "SubQuestTable" &&
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
                                drTemp["TableName"] = "SubQuestTable";
                                drTemp["Column"] = dataGridView2.SelectedCells[i].ColumnIndex;
                                drTemp["IndexNum"] = dataGridView2["IndexKey", dataGridView2.SelectedCells[i].RowIndex].Value;

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
                        dataGridView2.RefreshEdit();
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
            }
            frm.SetCurrentTab(GetCurrentTab());
            frm.Owner = this;
            frm.ShowDialog(this);
        }

        private void dataGridView1_CellMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            //dataGridView1.Rows[0].Visible = false;
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
                    case 1:
                        {
                            frm.SetDataGridView(dataGridView2);
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
                case 1:
                    {
                        textBox3.Text = dataGridView2.CurrentCell.Value.ToString();
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
                        UserDeletingRow(nMin, dataGridView1, tblQuest, false, 0);
                    }

                }
                else
                {
                    UserDeletingRow(e.Row.Index, dataGridView1, tblQuest, false, 0);
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
                        UserDeletingRow(nMin, dataGridView2, tblSubQuest, false, 1);
                    }

                }
                else
                {
                    UserDeletingRow(e.Row.Index, dataGridView2, tblSubQuest, false, 1);
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

                        for (int i = 1; i <= 3; i++)
                        {
                            string strRItem = "Reward Item" + i;
                            string strItem = "name" + i;

                            if (dgvCurrentCell.OwningColumn.Name == strRItem)
                            {
                                if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                                {
                                    dataGridView1[strItem, nIndexRow].Value = "";
                                }
                                else
                                {
                                    DataRow dr = tblItem.Rows.Find(dgvCurrentCell.Value.ToString());
                                    if (dr != null)
                                    {
                                        dataGridView1[strItem, nIndexRow].Value = dr["Name"].ToString();

                                    }
                                    else
                                    {
                                        MessageBox.Show("존재하지 않는 값입니다.");
                                        dgvCurrentCell.Value = objTemp;
                                    }
                                }
                            }
                        }

                        for (int i = 1; i <= 5; i++)
                        {
                            string strRItem = "Select Reward Item" + i;
                            string strItem = "name" + i + "_1";

                            if (dgvCurrentCell.OwningColumn.Name == strRItem)
                            {
                                if (Convert.ToInt32(dgvCurrentCell.Value.ToString()) == 0)
                                {
                                    dataGridView1[strItem, nIndexRow].Value = "";
                                }
                                else
                                {
                                    DataRow dr = tblItem.Rows.Find(dgvCurrentCell.Value.ToString());
                                    if (dr != null)
                                    {
                                        dataGridView1[strItem, nIndexRow].Value = dr["Name"].ToString();

                                    }
                                    else
                                    {
                                        MessageBox.Show("존재하지 않는 값입니다.");
                                        dgvCurrentCell.Value = objTemp;
                                    }
                                }
                            }
                        }
                    }
                    break;
                case 1:
                    {
                        if (dgvCurrentCell.OwningColumn.Name == "Clear Condition Collection Item ID")
                        {
                            DataRow dr = tblItem.Rows.Find(dgvCurrentCell.Value.ToString());
                            if (dr != null)
                            {
                                dataGridView2["Name", nIndexRow].Value = dr["Name"].ToString();

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
                                    UndoKeyboardControl(dataGridView1, tblQuest, sUndoTemp, sRedoTemp, 0);
                                }
                                break;
                            case 1:
                                {
                                    UndoKeyboardControl(dataGridView2, tblSubQuest, sUndoTemp, sRedoTemp, 1);
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
                                    RedoKeyboardControl(dataGridView1, tblQuest, sUndoTemp, sRedoTemp, 0);
                                }
                                break;
                            case 1:
                                {
                                    RedoKeyboardControl(dataGridView2, tblSubQuest, sUndoTemp, sRedoTemp, 1);
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
                            tblQuest.Clear();
                            Adpt_QuestTable.Fill(tblQuest);


                            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

                            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
                            {
                                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["Title", nRowHeader].Value.ToString();
                            }
                        }
                        break;
                    case 1:
                        {
                            tblSubQuest.Clear();
                            Adpt_SubQuestTable.Fill(tblSubQuest);
                            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여

                            for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
                            {
                                dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["Clear Type", nRowHeader].Value.ToString();
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
            }

            AssistBox.Hide();
        }

        private void btnLink_Click(object sender, EventArgs e)
        {
            //09. 06. 06 김정협
            int nCount = 0;
            for (int i = 0; i < tblQuest.Columns.Count; i++)
            {

                for (int k = 1; k <= 3; k++)
                {
                    string strRItem = "Reward Item" + k;
                    string strItem = "name" + k;

                    if (tblQuest.Columns[i].ColumnName == strRItem)
                    {
                        for (int j = 0; j < tblQuest.Rows.Count; j++)
                        {
                            if (tblQuest.Rows[j][strRItem].ToString() == "0")
                            {
                                continue;
                            }
                            else
                            {
                                DataRow dr = tblItem.Rows.Find(tblQuest.Rows[j][strRItem].ToString());
                                if (tblQuest.Rows[j][strItem].ToString() != dr["Name"].ToString())
                                {
                                    tblQuest.Rows[j][strItem] = dr["Name"];
                                    Adpt_QuestTable.Update(new DataRow[] { tblQuest.Rows[j] });
                                    nCount++;
                                }
                            }
                        }
                    }
                }

                for (int k = 1; k <= 5; k++)
                {
                    string strRItem = "Select Reward Item" + k;
                    string strItem = "name" + k + "_1";

                    if (tblQuest.Columns[i].ColumnName == strRItem)
                    {
                        for (int j = 0; j < tblQuest.Rows.Count; j++)
                        {
                            if (tblQuest.Rows[j][strRItem].ToString() == "0")
                            {
                                continue;
                            }
                            else
                            {
                                DataRow dr = tblItem.Rows.Find(tblQuest.Rows[j][strRItem].ToString());
                                if (tblQuest.Rows[j][strItem].ToString() != dr["Name"].ToString())
                                {
                                    tblQuest.Rows[j][strItem] = dr["Name"];
                                    Adpt_QuestTable.Update(new DataRow[] { tblQuest.Rows[j] });
                                    nCount++;
                                }
                            }
                        }
                    }
                }
            }
            MessageBox.Show("Quest 틀린 개수는 " + nCount + "개의 잘못된 링크를 수정하였습니다.");
            nCount = 0;

            for (int i = 0; i < tblSubQuest.Columns.Count; i++)
            {
                if (tblSubQuest.Columns[i].ColumnName == "Clear Condition Collection Item ID")
                {
                    for (int j = 0; j < tblSubQuest.Rows.Count; j++)
                    {
                        if (tblSubQuest.Rows[j]["Clear Condition Collection Item ID"].ToString() == "0")
                        {
                            continue;
                        }
                        else
                        {
                            DataRow dr = tblItem.Rows.Find(tblSubQuest.Rows[j]["Clear Condition Collection Item ID"]);
                            if (tblSubQuest.Rows[j]["Name"].ToString() != dr["Name"].ToString())
                            {
                                tblSubQuest.Rows[j]["Name"] = dr["Name"];
                                Adpt_SubQuestTable.Update(new DataRow[] { tblSubQuest.Rows[j] });
                                nCount++;
                            }
                        }

                    }
                }
            }

            MessageBox.Show("SubQuest 틀린 개수는 " + nCount + "개의 잘못된 링크를 수정하였습니다.");
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
                            if (tblBorder.Rows[i]["IDNum"].ToString() == dataGridView1["ID", dataGridView1.CurrentCell.RowIndex].Value.ToString())
                            {
                                nRow = i;
                                break;
                            }
                        }

                        if (nRow == -1)
                        {
                            tblBorder.Rows.Add();
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["TableName"] = "QuestTable";
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
                            tblBorder.Rows[tblBorder.Rows.Count - 1]["TableName"] = "SubQuestTable";
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
            }
        }

        private void dataGridView1_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            for (int i = 0; i < tblBorder.Rows.Count; i++)
            {
                if (tblBorder.Rows[i].RowState == DataRowState.Deleted || tblBorder.Rows[i]["TableName"].ToString() != "QuestTable")
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
                        nRow = dataGridView1["ID", nMid].RowIndex;
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
                if (tblBorder.Rows[i].RowState == DataRowState.Deleted || tblBorder.Rows[i]["TableName"].ToString() != "SubQuestTable")
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

        private void AssistBox_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Escape || e.KeyCode == Keys.Enter)
            {                
                AssistBox.Hide();                
            }
        }
        
    }
}