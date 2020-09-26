//09. 06. 24 김정협 나라별 define
// BillingTable.cs, QuestTable.cs, TitleTable.cs SkillTable.cs 네군대 정의를
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

using System.Diagnostics;
using DBManager.BillingDataSetTableAdapters;





namespace DBManager
{


    public partial class BillingTable : Form
    {   
        object objTemp;
        object objTemp2;



        DataTable tblProductCategory;
        DataTable tblProductCategory_History;                
        DataTable tblProductCategory_DeleteHistory;
        
        DataTable tblProductInfo;
        DataTable tblProductInfo_History;
        DataTable tblProductInfo_DeleteHistory;
        
        DataTable tblProductAttribute;
        DataTable tblProductAttribute_History;
        DataTable tblProductAttribute_DeleteHistory;


        Bill_ProductCategoryTableAdapter Adpt_ProductCategory = new Bill_ProductCategoryTableAdapter();
        Bill_ProductCategory_HistoryTableAdapter Adpt_ProductCategory_History = new Bill_ProductCategory_HistoryTableAdapter();
        Bill_ProductCategory_DeleteHistoryTableAdapter Adpt_ProductCategory_DeleteHistory = new Bill_ProductCategory_DeleteHistoryTableAdapter();

        Bill_ProductAttributeTableAdapter Adpt_ProductAttribute = new Bill_ProductAttributeTableAdapter();
        Bill_ProductAttribute_HistoryTableAdapter Adpt_ProductAttribute_History = new Bill_ProductAttribute_HistoryTableAdapter();
        Bill_ProductAttribute_DeleteHistoryTableAdapter Adpt_ProductAttribute_DeleteHistory = new Bill_ProductAttribute_DeleteHistoryTableAdapter();

        Bill_ProductInfoTableAdapter Adpt_ProductInfo = new Bill_ProductInfoTableAdapter();
        Bill_ProductInfo_HistoryTableAdapter Adpt_ProductInfo_History = new Bill_ProductInfo_HistoryTableAdapter();
        Bill_ProductInfo_DeleteHistoryTableAdapter Adpt_ProductInfo_DeleteHistory = new Bill_ProductInfo_DeleteHistoryTableAdapter();
        

        string m_strID;
        ArrayList m_ModifyIDList = new ArrayList();

        ArrayList m_DeleteRowList = new ArrayList();

        Stack m_UndoStack = new Stack();
        Stack m_RedoStack = new Stack();

        public bool m_bSaveEnd = true; //수정했을 시 저장한 후 끝내는지 확인하는 변수



        int nSelectFocus = -1;//0, 1, 2
        


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




        
        
        public BillingTable()
        {
            InitializeComponent();
                      
        }

        public void SetID(string strID)
        {
            m_strID = strID;
        }
        

        public void ColorRefresh(bool bRed)
        {
        
        }

        private void Save()
        {
            //09. 07. 01 김정협 현재 위치 기억
            

            //History 추가
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            string strMyIP = host.AddressList[0].ToString();
            DataRow Row_History = null;
            DataRow Row = null;
            //09. 06. 01 김정협 수정 붉은색 모두 검은색 글씨로
            //ColorRefresh(false);



            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                switch (((sTableValue)m_ModifyIDList[i]).nTabIndex)
                {
                    case 0:
                        {

                            tblProductCategory_History.Columns["CategoryNo"].AllowDBNull = true;
                            tblProductCategory_History.Columns["Regdate"].AllowDBNull = true;
                            tblProductCategory_History.Rows.Add();

                            Row_History = tblProductCategory_History.Rows[tblProductCategory_History.Rows.Count - 1];

                            //09. 06. 23 김정협 수정 : ((sTableValue)m_ModifyIDList[i]).nRowIndex 대신
                            // IndexKey전부 검색 중간 삭제시 인덱스 어긋나기 때문
                            for (int nRow = 0; nRow < tblProductCategory.Rows.Count; nRow++)
                            {
                                if(tblProductCategory.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblProductCategory.Rows[nRow]["CategoryNo"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblProductCategory.Rows[nRow];
                                    break;
                                }                                
                            }
                                                        
                           

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblProductCategory.Columns.Count ; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = DateTime.Now;


                            Adpt_ProductCategory_History.Update( Row_History );
                            Adpt_ProductCategory.Update(Row);

                        }
                        break;
                    case 1:
                        {

                            tblProductInfo_History.Columns["ProductNo"].AllowDBNull = true;
                            tblProductInfo_History.Columns["Regdate"].AllowDBNull = true;

                            tblProductInfo_History.Rows.Add();

                            Row_History = tblProductInfo_History.Rows[tblProductInfo_History.Rows.Count - 1];

                            
                            for (int nRow = 0; nRow < tblProductInfo.Rows.Count; nRow++)
                            {
                                if (tblProductInfo.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblProductInfo.Rows[nRow]["ProductNo"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblProductInfo.Rows[nRow];
                                    break;
                                }
                            }
                                                        
                            for (int Column = 0; Column < tblProductInfo.Columns.Count; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = DateTime.Now;

                            
                            Adpt_ProductInfo.Update( Row );
                            Adpt_ProductInfo_History.Update( Row_History );

                        }
                        break;
                    case 2:
                        {
                            tblProductInfo_History.Columns["ProductNo"].AllowDBNull = true;
                            tblProductInfo_History.Columns["Regdate"].AllowDBNull = true;
                            
                            tblProductAttribute_History.Rows.Add();

                            Row_History = tblProductAttribute_History.Rows[tblProductAttribute_History.Rows.Count - 1];
                            
                            for (int nRow = 0; nRow < tblProductAttribute.Rows.Count; nRow++)
                            {
                                if (tblProductAttribute.Rows[nRow].RowState == DataRowState.Deleted)
                                {
                                    continue;
                                }
                                if (tblProductAttribute.Rows[nRow]["ProductNo"].ToString() == ((sTableValue)m_ModifyIDList[i]).nIndexKey.ToString())
                                {
                                    Row = tblProductAttribute.Rows[nRow];
                                    break;
                                }
                            }

                            
                            for (int Column = 0; Column < tblProductAttribute.Columns.Count ; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            Row_History["Regdate"] = DateTime.Now;


                            Adpt_ProductAttribute.Update(Row);
                            Adpt_ProductAttribute_History.Update(Row_History);

                        }
                        break;
                    
                }

            }


            for (int i = 0; i < m_DeleteRowList.Count; i++)
            {
                switch (((sDeleteRow)m_DeleteRowList[i]).nTabIndex)
                {
                    case 0:
                        {
                            tblProductCategory_DeleteHistory.Columns["CategoryNo"].AllowDBNull = true;
                            tblProductCategory_DeleteHistory.Columns["Regdate"].AllowDBNull = true;

                            tblProductCategory_DeleteHistory.Rows.Add();

                            Row_History = tblProductCategory_DeleteHistory.Rows[tblProductCategory_DeleteHistory.Rows.Count - 1];
                                                        
                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            
                            for (int Column = 0; Column < tblProductCategory.Columns.Count ; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            
                            Row_History["Regdate"] = DateTime.Now;

                            Adpt_ProductCategory.Update( ((sDeleteRow)m_DeleteRowList[i]).drRow );
                            Adpt_ProductCategory_DeleteHistory.Update( Row_History );

                            

                            

                        }
                        break;
                    case 1:
                        {
                            tblProductInfo_DeleteHistory.Columns["ProductNo"].AllowDBNull = true;
                            tblProductInfo_DeleteHistory.Columns["Regdate"].AllowDBNull = true;

                            tblProductInfo_DeleteHistory.Rows.Add();
                            Row_History = tblProductInfo_DeleteHistory.Rows[tblProductInfo_DeleteHistory.Rows.Count - 1];
                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;
                                                        
                            for (int Column = 0; Column < tblProductInfo.Columns.Count ; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;                            
                            Row_History["Regdate"] = DateTime.Now;

                            Adpt_ProductInfo.Update( ((sDeleteRow)m_DeleteRowList[i]).drRow );
                            Adpt_ProductInfo_DeleteHistory.Update( Row_History );

                            
                        }
                        break;
                    case 2:
                        {
                            tblProductAttribute_DeleteHistory.Columns["ProductNo"].AllowDBNull = true;
                            tblProductAttribute_DeleteHistory.Columns["Regdate"].AllowDBNull = true;

                            tblProductAttribute_DeleteHistory.Rows.Add();
                            Row_History = tblProductAttribute_DeleteHistory.Rows[tblProductAttribute_DeleteHistory.Rows.Count - 1];
                            Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                            //09. 06. 22 히스토리 입력부분 간소화
                            for (int Column = 0; Column < tblProductAttribute.Columns.Count ; Column++)
                            {
                                Row_History[Column] = Row[Column];
                            }
                            Row_History["UserID"] = m_strID;
                            Row_History["UserIP"] = strMyIP;
                            
                            Row_History["Regdate"] = DateTime.Now;


                            Adpt_ProductAttribute.Update( ((sDeleteRow)m_DeleteRowList[i]).drRow );
                            Adpt_ProductAttribute_DeleteHistory.Update( Row_History );
                            
                        }
                        break;
                    
                }
            }

            
//            
// 
//             tblProductCategory.Clear();
//             Adpt.Fill(tblProductCategory);
// 
//             tblProductInfo.Clear();
//             Adpt_SpecialAbility.Fill(tblProductInfo);
// 
//             tblProductAttribute.Clear();
//             Adpt_SocketItem.Fill(tblProductAttribute);

                                    
            m_ModifyIDList.Clear();
           
            m_DeleteRowList.Clear();
            m_UndoStack.Clear();
            m_RedoStack.Clear();
            m_bSaveEnd = true;

        }


        
        


        private void BillingTable_Load(object sender, EventArgs e)
        {   
            
            
            btnCategoryAdd.Enabled = true;
            

            

            dataGridView1.ContextMenuStrip = contextMenuStrip1;
            dataGridView2.ContextMenuStrip = contextMenuStrip1;
            dataGridView3.ContextMenuStrip = contextMenuStrip1;
            
            
            
            tblProductCategory = Adpt_ProductCategory.GetData();
            tblProductCategory_History = Adpt_ProductCategory_History.GetData();
            tblProductCategory_DeleteHistory = Adpt_ProductCategory_DeleteHistory.GetData();
            tblProductInfo = Adpt_ProductInfo.GetData();
            tblProductInfo_History = Adpt_ProductInfo_History.GetData();
            tblProductInfo_DeleteHistory = Adpt_ProductInfo_DeleteHistory.GetData();
            tblProductAttribute = Adpt_ProductAttribute.GetData();
            tblProductAttribute_History = Adpt_ProductAttribute_History.GetData();
            tblProductAttribute_DeleteHistory = Adpt_ProductAttribute_DeleteHistory.GetData();


    
            dataGridView1.DataSource = tblProductCategory;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }

            dataGridView2.DataSource = tblProductInfo;
            dataGridView2.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰

            for (int i = 0; i < dataGridView2.ColumnCount; i++)
            {
                dataGridView2.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }

            dataGridView3.DataSource = tblProductAttribute;
            dataGridView3.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            for (int i = 0; i < dataGridView3.ColumnCount; i++)
            {
                dataGridView3.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }


            /*
            dataGridView1.RowHeadersWidth = 130;
            dataGridView1.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;
            dataGridView2.RowHeadersWidth = 130;
            dataGridView2.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;
            dataGridView3.RowHeadersWidth = 130;
            dataGridView3.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;
            

            

            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {                
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["Name", nRowHeader].Value.ToString();
            }

            
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
            for (int nRowHeader = 0; nRowHeader < dataGridView2.RowCount; nRowHeader++)
            {                
                dataGridView2.Rows[nRowHeader].HeaderCell.Value = dataGridView2["ID", nRowHeader].Value.ToString() + "     " + dataGridView2["TYPE_", nRowHeader].Value.ToString();
            }

            
            //09. 06. 30 김정협 엑셀의 고정셀 기능을 위하여            
            for (int nRowHeader = 0; nRowHeader < dataGridView3.RowCount; nRowHeader++)
            {                
                dataGridView3.Rows[nRowHeader].HeaderCell.Value = dataGridView3["ID", nRowHeader].Value.ToString() + "     " + dataGridView3["Description", nRowHeader].Value.ToString();
            }*/

            
            
        }



        //09. 07. 01 김정협 ADD 창 떠 있을 시 계속 추가되게하는 부분
        ProductCategoryAdd frmCategory = new ProductCategoryAdd();
        ProductInfoAdd frmInfo = new ProductInfoAdd();
        ProductAttributeAdd frmAttribute = new ProductAttributeAdd();

        private void btnCategoryAdd_Click(object sender, EventArgs e)
        {
            if (!frmCategory.Visible)
            {
                frmCategory = new ProductCategoryAdd();
            }
            frmCategory.SetDataGridView(dataGridView1);
            if(dataGridView1.RowCount > 0)
                frmCategory.SetRowIndex(dataGridView1.CurrentCell.RowIndex);
            frmCategory.SetDataTable(tblProductCategory);
            frmCategory.SetModifyArray(m_ModifyIDList);


            frmCategory.SetUndoStack(m_UndoStack);
            frmCategory.SetRedoStack(m_RedoStack);



            frmCategory.Owner = this;


            if (!frmCategory.Visible)
            {
                //frm.SetTableNew();
                frmCategory.Show(this);
            }
            else
            {
                if (dataGridView1.RowCount > 0)
                    frmCategory.ADDItem();// ItemAdd_Load(null, null);
            }
        
        }

        private void btnInfoAdd_Click(object sender, EventArgs e)
        {
            if (!frmInfo.Visible)
            {
                frmInfo = new ProductInfoAdd();
            }
            frmInfo.SetDataGridView(dataGridView2);

            if (dataGridView2.RowCount > 0)
                frmInfo.SetRowIndex(dataGridView2.CurrentCell.RowIndex);
            frmInfo.SetDataTable(tblProductInfo);
            frmInfo.SetModifyArray(m_ModifyIDList);
            
            frmInfo.SetUndoStack(m_UndoStack);
            frmInfo.SetRedoStack(m_RedoStack);

            frmInfo.Owner = this;

            if (!frmInfo.Visible)
            {
                //frm.SetTableNew();
                frmInfo.Show(this);
            }
            else
            {
                if (dataGridView2.RowCount > 0)
                    frmInfo.ADDItem();// ItemAdd_Load(null, null);
            }

        }

        private void btnAttributeAdd_Click(object sender, EventArgs e)
        {
            if (!frmAttribute.Visible)
            {
                frmAttribute = new ProductAttributeAdd();
            }
            frmAttribute.SetDataGridView(dataGridView3);
            if (dataGridView3.RowCount > 0)
                frmAttribute.SetRowIndex(dataGridView3.CurrentCell.RowIndex);
            frmAttribute.SetDataTable(tblProductAttribute);
            frmAttribute.SetModifyArray(m_ModifyIDList);

            frmAttribute.SetUndoStack(m_UndoStack);
            frmAttribute.SetRedoStack(m_RedoStack);

            frmAttribute.Owner = this;

            if (!frmAttribute.Visible)
            {
                //frm.SetTableNew();
                frmAttribute.Show(this);
            }
            else
            {
                if (dataGridView3.RowCount > 0)
                    frmAttribute.ADDItem();// ItemAdd_Load(null, null);
            }
        }

       
        private void BillingTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void btnCategoryHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.Text = "History";
            frm.SetHistoryTable(tblProductCategory_History);
            frm.ShowDialog(this);
        }

        private void btnInfoHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.Text = "History";
            frm.SetHistoryTable(tblProductInfo_History);
            frm.ShowDialog(this);
        }

        private void btnAttributeHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.Text = "History";
            frm.SetHistoryTable(tblProductAttribute_History);
            frm.ShowDialog(this);
        }

        


        private void btnCategorySave_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("저장하시겠습니까?", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                return;
            }

            Save();
            MessageBox.Show("저장되었습니다.");

        }

        private void btnInfoSave_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("저장하시겠습니까?", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                return;
            }

            Save();
            MessageBox.Show("저장되었습니다.");

        }

        private void btnAttributeSave_Click(object sender, EventArgs e)
        {
            DialogResult result = MessageBox.Show("저장하시겠습니까?", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                return;
            }

            Save();
            MessageBox.Show("저장되었습니다.");
        }




        private void BillingTable_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.YesNoCancel);
                if (result == DialogResult.Yes)
                {
                    Save();
                    MessageBox.Show("저장되었습니다.");
                }
                else if(result == DialogResult.Cancel)
                {
                    e.Cancel = true;
                    return;
                }

                m_ModifyIDList.Clear();
                
                m_bSaveEnd = true;

                
            }

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
            objTemp2 = dataGridView1["CategoryNo", dgvCurrentCell.RowIndex].Value;
            //09. 06. 02 김정협 수정 : 배경색때문에 전의 셀로우를 임시저장
            m_nLastRow = dgvCurrentCell.RowIndex;
                        

        }

        public void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            //            contextMenuStrip1.Items.Clear();

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
                    if (dataGridView1["CategoryNo", i].Value.Equals(objTemp2))
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
            if (!objTemp.Equals((object)dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Value))
            {
                //09. 06. 02 김정협 수정 : dgvCurrentCell.RowIndex 를 IndexRow로 교체
                
                dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Red;


                if (dgvCurrentCell.OwningColumn.Name == "CategoryNo")
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        if (dataGridView1["CategoryNo", i].Value.ToString() == dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Value.ToString())
                        {
                            if (i == nIndexRow)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID값이 존재합니다.");

                            dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Value = objTemp;
                            dataGridView1[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Black;
                            
                            return;

                        }
                    }
                }

               

                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = nIndexRow;
                sModiftIDList.nTabIndex = 0;

                //09. 06. 01 김정협 수정 : Cell정보 추가
                sModiftIDList.bCell = true;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView1["CategoryNo", nIndexRow].Value.ToString());


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
                if(!m_bPaste)
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
            objTemp2 = dataGridView2["ProductNo", dgvCurrentCell.RowIndex].Value;

            m_nLastRow = dgvCurrentCell.RowIndex;

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
            if (dgvCurrentCell.OwningColumn.Name == "ProductNo")
            {
                for (int i = 0; i < dataGridView2.RowCount; i++)
                {
                    if (dataGridView2["ProductNo", i].Value.Equals(objTemp2))
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
            if (!objTemp.Equals((object)dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Value))
            {
                //09. 06. 02 김정협 수정 : dgvCurrentCell.RowIndex 를 IndexRow로 교체
                
                dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Red;

                if (dgvCurrentCell.OwningColumn.Name == "ProductNo")
                {
                    for (int i = 0; i < dataGridView2.RowCount; i++)
                    {
                        if (dataGridView2["ProductNo", i].Value.ToString() == dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Value.ToString())
                        {
                            if (i == nIndexRow)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID값이 존재합니다.");

                            dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Value = objTemp;
                            dataGridView2[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Black;
                            
                            return;

                        }
                    }
                }


                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = nIndexRow;
                sModiftIDList.nTabIndex = 1;
                //09. 06. 01 김정협 수정 : Cell정보 추가
                sModiftIDList.bCell = true;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView2["ProductNo", nIndexRow].Value.ToString());


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
            objTemp2 = dataGridView3["ProductNo", dgvCurrentCell.RowIndex].Value;
            m_nLastRow = dgvCurrentCell.RowIndex;

            
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
            if (dgvCurrentCell.OwningColumn.Name == "ProductNo")
            {
                for (int i = 0; i < dataGridView3.RowCount; i++)
                {
                    if (dataGridView3["ProductNo", i].Value.Equals(objTemp2))
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
            if (!objTemp.Equals((object)dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Value))
            {
                //09. 06. 02 김정협 수정 : dgvCurrentCell.RowIndex 를 IndexRow로 교체
                

                dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Red;

                if (dgvCurrentCell.OwningColumn.Name == "ProductNo")
                {
                    for (int i = 0; i < dataGridView3.RowCount; i++)
                    {
                        if (dataGridView3["ProductNo", i].Value.ToString() == dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Value.ToString())
                        {
                            if (i == nIndexRow)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID값이 존재합니다.");

                            dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Value = objTemp;
                            dataGridView3[dgvCurrentCell.ColumnIndex, nIndexRow].Style.ForeColor = Color.Black;
                            
                            return;

                        }
                    }
                }


                sTableValue sModiftIDList = new sTableValue();
                sModiftIDList.nRowIndex = nIndexRow;
                sModiftIDList.nTabIndex = 2;
                //09. 06. 01 김정협 수정 : Cell정보 추가
                sModiftIDList.bCell = true;
                sModiftIDList.nIndexKey = Convert.ToInt32(dataGridView3["ProductNo", nIndexRow].Value.ToString());


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
            //tblProductCategory

            

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
                for(int i = 0; i < arrData.Length; i ++)
                {                    
                    string strTemp = "\t";
                    char[] cDe = strTemp.ToCharArray();
                    //string[] arrData = strClp.Split(delimiter, strClp.Length);
                    string[] strData = arrData[i].Split(cDe, arrData[i].Length);
                    for(int j = 0; j < strData.Length; j++)
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
                            dataGridView1_CellBeginEdit(null,null);
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
                if (!dataGridView1.CurrentCell.ReadOnly && dataGridView1.SelectedRows.Count <= 0)
                {
                    //09. 07. 02 김정협 언두리두시 그룹지정
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

            

            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView2.GetClipboardContent().GetText());
            }

            //09. 06. 10 김정협 셀선택상태에서 붙여넣기 및 삭제가능
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.V)
            {
                //09. 07. 02 김정협 언두리두시 그룹지정
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

        

        private void BillingTable_Resize(object sender, EventArgs e)
        {            
            dataGridView1.Width = this.Width - 40;
            //dataGridView1.Height = this.Height - 29;

            dataGridView2.Width = this.Width / 2 + this.Width / 5;
            //dataGridView2.Height = tabControl1.Height - 29;

            dataGridView3.Width = this.Width / 4;
            dataGridView3.Left = dataGridView1.Left + dataGridView1.Width - dataGridView3.Width;


            btnCategoryHistory.Left = dataGridView1.Left + dataGridView1.Width - btnCategoryHistory.Width;
            btnCategorySave.Left = btnCategoryHistory.Left - btnCategorySave.Width - 10;
            btnCategoryAdd.Left = btnCategorySave.Left - btnCategoryAdd.Width - 10;

            btnInfoHistory.Left = dataGridView2.Left + dataGridView2.Width - btnInfoHistory.Width;
            btnInfoSave.Left = btnInfoHistory.Left - btnInfoSave.Width - 10;
            btnInfoAdd.Left = btnInfoSave.Left - btnInfoAdd.Width - 10;


            btnAttributeHistory.Left = dataGridView3.Left + dataGridView3.Width - btnAttributeHistory.Width;
            btnAttributeSave.Left = btnAttributeHistory.Left - btnAttributeSave.Width - 10;
            btnAttributeAdd.Left = btnAttributeSave.Left - btnAttributeAdd.Width - 10;

            //dataGridView3.Height = tabControl1.Height - 29;
            
        }


        

        private void textBox2_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Enter == e.KeyChar)
                btnSearch_Click(null, null);
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



        private void dataGridView1_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if(dataGridView1.RowCount > 0)
                textBox1.Text = dataGridView1[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void dataGridView2_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (dataGridView2.RowCount > 0)
                textBox1.Text = dataGridView2[e.ColumnIndex, 0].OwningColumn.Name;

        }

        private void dataGridView3_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (dataGridView3.RowCount > 0)
                textBox1.Text = dataGridView3[e.ColumnIndex, 0].OwningColumn.Name;
        }



        private void contextMenuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            if (nSelectFocus == 0)
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
                else if (e.ClickedItem.Text == "전체 해제")
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
            else if (nSelectFocus == 1)
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
            else if (nSelectFocus == 2)
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
                dataGridView3.RefreshEdit();
            }
        }

        private void contextMenuStrip1_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            contextMenuStrip1.Items.Clear();           
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

                    //09. 07. 02 김정협 링크리스트 열기
                    if (dataGridView1.CurrentCell.OwningColumn.Name == "Item Type" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Use Type" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Period Type" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Price Type" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Use Condition" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Unit Type" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Unit Class" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Eqip Position" ||
                        dataGridView1.CurrentCell.OwningColumn.Name == "Item Grade")
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
                    if (dataGridView2.CurrentCell.OwningColumn.Name == "TYPE")
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
                    //09. 07. 02 김정협 링크리스트 열기
                    if (dataGridView3.CurrentCell.OwningColumn.Name == "Socket Type" ||
                        dataGridView3.CurrentCell.OwningColumn.Name == "ExtraDamageType" ||
                        dataGridView3.CurrentCell.OwningColumn.Name == "ExtraDamageResistType" ||
                        dataGridView3.CurrentCell.OwningColumn.Name == "ImmuneExtraDamageType")
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

        

        private void contextMenuStrip2_Closed(object sender, ToolStripDropDownClosedEventArgs e)
        {
            contextMenuStrip2.Items.Clear();
        }

        private void contextMenuStrip2_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            if (e.ClickedItem.Text == "필터링 설정")
            {

                Filtering frm = new Filtering();
                if (nSelectFocus == 0)
                {
                    frm.SetDataGridView(dataGridView1);
                }
                else if (nSelectFocus == 1)
                {
                    frm.SetDataGridView(dataGridView2);
                }
                else if (nSelectFocus == 2)
                {
                    frm.SetDataGridView(dataGridView3);
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
                if (nSelectFocus == 0)
                {
                    for (int i = 0; i < dataGridView1.RowCount; i++)
                    {
                        if (dataGridView1.Rows[i].Visible == false)
                            dataGridView1.Rows[i].Visible = true;
                    }
                }
                else if (nSelectFocus == 1)
                {
                    for (int i = 0; i < dataGridView2.RowCount; i++)
                    {
                        if (dataGridView2.Rows[i].Visible == false)
                            dataGridView2.Rows[i].Visible = true;
                    }
                }
                else if (nSelectFocus == 2)
                {
                    for (int i = 0; i < dataGridView3.RowCount; i++)
                    {
                        if (dataGridView3.Rows[i].Visible == false)
                            dataGridView3.Rows[i].Visible = true;
                    }
                }
            }

        }        

        void UndoKeyboardControl(DataGridView dgvCurrent,DataTable tblCurrent, sUndoRedo sUndoTemp, sUndoRedo sRedoTemp, int nTabPage)
        {            
            if (sUndoTemp.nType == 1) ////1. 수정(CellEditEnd를거침) 2. ADD 3. Delete
            {
                if (sUndoTemp.nColumnIndex != 1)//ID등 정렬기준값
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
                    
                }
                else
                {
                    for (int i = 0; i < dgvCurrent.Rows.Count; i++)
                    {
                        if (dgvCurrent[0, i].Value.ToString() == Convert.ToString(sUndoTemp.nRowIndex))
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
                                if (dgvCurrent[0, k].Value.ToString() == Convert.ToString(sUndoTemp.nRowIndex))
                                {
                                    dgvCurrent.CurrentCell = dgvCurrent[sUndoTemp.nColumnIndex, k];                                    
                                    break;
                                }
                            }
                                                        
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
                //if(sUndoTemp.sModyfyList.nIndexKey == tblProductCategory.Rows[i]
                //09. 07. 02 김정협 언두리두그룹화위해 매개변수 추가
                UserAddUndoRow(sUndoTemp.sModyfyList.nIndexKey, dgvCurrent, tblCurrent, sUndoTemp.sModyfyList, sUndoTemp.nGroupID, nTabPage);
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
                    sRedoTemp.nRowIndex = Convert.ToInt32(tblCurrent.Rows[tblCurrent.Rows.Count - 1][0].ToString());
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

                }
                else
                {
                    for (int i = 0; i < dgvCurrent.Rows.Count; i++)
                    {
                        if (dgvCurrent["0", i].Value.ToString() == Convert.ToString(sRedoTemp.nRowIndex))
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
                                if (dgvCurrent[0, k].Value.ToString() == Convert.ToString(sUndoTemp.nRowIndex))
                                {
                                    dgvCurrent.CurrentCell = dgvCurrent.CurrentCell = dgvCurrent[sRedoTemp.nColumnIndex, k]; 
                                    break;
                                }
                            }
                            
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
                sUndoData.nTabIndex = nTabPage;
                sUndoData.nType = 2;//2. Add 수정
                sUndoData.sModyfyList = sRedoTemp.sModyfyList;

                //09. 07. 02 김정협 Undo그룹짓기
                sUndoData.nGroupID = sRedoTemp.nGroupID;

                m_UndoStack.Push(sUndoData);

                m_ModifyIDList.Add(sUndoData.sModyfyList);
            }
            else if (sRedoTemp.nType == 3)
            {
                int nValue = -1;
                for (int i = 0; i < tblCurrent.Rows.Count; i++)
                {
                    if (tblCurrent.Rows[i].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    if (tblCurrent.Rows[i][0].ToString() == sRedoTemp.nRowIndex.ToString())
                    {
                        nValue = i;
                        break;
                    }
                }
                UserDeletingRow(nValue, dgvCurrent, tblCurrent, true, nTabPage);
            }
            dgvCurrent.RefreshEdit();
        }
        

        //09. 06. 10 김정협 리플레이스 기능 추가
        private void btnReplace_Click(object sender, EventArgs e)
        {
            panel1.Visible = true;

            if (nSelectFocus == 0)
            {
                textBox3.Text = dataGridView1.CurrentCell.Value.ToString();
            }
            else if (nSelectFocus == 1)
            {
                textBox3.Text = dataGridView2.CurrentCell.Value.ToString();
            }
            else if (nSelectFocus == 2)
            {
                textBox3.Text = dataGridView3.CurrentCell.Value.ToString();
            }
        }
        //09. 06. 10 김정협 리플레이스 기능 추가
        private void btnOK_Click(object sender, EventArgs e)
        {
            if (nSelectFocus == 0)
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
            else if (nSelectFocus == 1)
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
            else if (nSelectFocus == 2)
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
            panel1.Visible = false;
            textBox3.Text = "";
            textBox4.Text = "";
        }
        //09. 06. 10 김정협 리플레이스 기능 추가
        private void btnCancel_Click(object sender, EventArgs e)
        {
            panel1.Visible = false;
            textBox3.Text = "";
            textBox4.Text = "";
        }

        //09. 07. 02 김정협 Undo그룹짓기 nGroup매개변수 추가
        void UserAddUndoRow(int nDeletingRowIndex, DataGridView dgvView, DataTable tblDelete, sTableValue Modify, int nGroup, int nTabPage)
        {
            DataRow dr = null;// = tblDelete.Rows[e.Row.Index];
            int nRowIndex = 0;

            for (int i = 0; i < tblDelete.Rows.Count; i++)
            {
                if (tblDelete.Rows[i].RowState == DataRowState.Deleted)
                {
                    continue;
                }

                if (tblDelete.Rows[i][0].ToString() == Convert.ToString(nDeletingRowIndex))
                {
                    dr = tblDelete.Rows[i];
                    nRowIndex = i;
                    break;
                }
            }

            //Redo를 위하여 값을 보존
            DataTable tblTemp = tblDelete.Clone();
            tblTemp.Rows.Add();
            for(int i = 0; i < tblTemp.Columns.Count; i++)
            {
                tblTemp.Rows[0][i] = tblDelete.Rows[nRowIndex][i];
            }
            
            DataRow drOriginal = tblTemp.Rows[0];

            sUndoRedo sRedo = new sUndoRedo();
            sRedo.nType = 2;
            sRedo.nTabIndex = nTabPage;
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

                    if (dgvView[0, nDeletingRowIndex].Value.ToString() == tblDelete.Rows[i][0].ToString())
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
            if(!bTable)
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
                UserDeletingRow(e.Row.Index, dataGridView1, tblProductCategory, false, 0);
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
                UserDeletingRow(e.Row.Index, dataGridView2, tblProductInfo, false, 1);
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
                UserDeletingRow(e.Row.Index, dataGridView3, tblProductAttribute, false, 2);
                e.Cancel = true;
            }
#else
            e.Cancel = true;
#endif

        }


        private void dataGridView1_RowStateChanged(object sender, DataGridViewRowStateChangedEventArgs e)
        {
            if(dataGridView1.SelectedRows.Count > 0)
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

        private void btnSearch_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
            {
                if (nSelectFocus == 0)
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
                else if (nSelectFocus == 1)
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
                else if (nSelectFocus == 2)
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
                        
            }
        }

        private void BillingTable_KeyDown(object sender, KeyEventArgs e)
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
                                    UndoKeyboardControl(dataGridView1, tblProductCategory, sUndoTemp, sRedoTemp, 0);

                                }
                                break;
                            case 1:
                                {
                                    UndoKeyboardControl(dataGridView2, tblProductInfo, sUndoTemp, sRedoTemp, 1);
                                }
                                break;
                            case 2:
                                {
                                    UndoKeyboardControl(dataGridView3, tblProductAttribute, sUndoTemp, sRedoTemp, 2);
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
                                    RedoKeyboardControl(dataGridView1, tblProductCategory, sUndoTemp, sRedoTemp, 0);
                                }
                                break;
                            case 1:
                                {
                                    RedoKeyboardControl(dataGridView2, tblProductInfo, sUndoTemp, sRedoTemp, 1);
                                }
                                break;
                            case 2:
                                {
                                    RedoKeyboardControl(dataGridView3, tblProductAttribute, sUndoTemp, sRedoTemp, 2);
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
        }

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                btnSearch_Click(null, null);
            }
        }

        private void dataGridView1_Enter(object sender, EventArgs e)
        {
            nSelectFocus = 0;
        }

        private void dataGridView2_Enter(object sender, EventArgs e)
        {
            nSelectFocus = 1;
        }

        private void dataGridView3_Enter(object sender, EventArgs e)
        {
            nSelectFocus = 2;
        }
            
       
    }
}


        