using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace DBManager
{
    public partial class ProductInfoAdd : Form
    {
        DataGridView m_dgvData;
        DataTable m_tblInfo;
        DataTable m_tblAdd;
        
        
        ArrayList m_ModifyIDList;

        int m_nRowIndex = -1;

        
                

        Stack m_UndoStack;
        Stack m_RedoStack;

        
        
        DataGridViewCell m_dgvcTemp;

        public ProductInfoAdd()
        {
            InitializeComponent();
        }

        public void SetUndoStack(Stack UndoStack)
        {
            m_UndoStack = UndoStack;
        }
        public void SetRedoStack(Stack RedoStack)
        {
            m_RedoStack = RedoStack;
        }

        public void SetDataTable(DataTable tblItem)
        {
            m_tblInfo = tblItem;
        }

        public void SetRowIndex(int nCurrentRowIndex)
        {
            m_nRowIndex = nCurrentRowIndex;
        }

        public void SetDataGridView(DataGridView dgvData)
        {
            m_dgvData = dgvData;
        }
        public void SetModifyArray(ArrayList ModifyIDList)
        {
            m_ModifyIDList = ModifyIDList;
        }

        

        private void btnAdd_Click(object sender, EventArgs e)
        {
            for (int nMultiADD = 0; nMultiADD < dataGridView1.RowCount; nMultiADD++)
            {
                for (int j = 0; j < m_dgvData.RowCount; j++)
                {
                    if (dataGridView1["ProductNo", nMultiADD].Value.Equals(m_dgvData["ProductNo", j].Value))
                    {
                        MessageBox.Show("동일한 ProductNo가 존재합니다. 다시 입력해주십시요.");
                        return;
                    }
                }
            }

            for (int nMultiADD = 0; nMultiADD < dataGridView1.RowCount; nMultiADD++)
            {
             
                m_tblInfo.Columns["ProductNo"].AllowDBNull = true;
                m_tblInfo.Rows.Add();

                //09. 06. 01 김정협 수정 : 1부터 시작 ID는 제일 나중에 대입해야함. 넣는즉시 정렬
                for (int j = 0; j < m_tblAdd.Columns.Count; j++)
                {
                    m_dgvData[j, m_dgvData.RowCount - 1].Value = dataGridView1[j, nMultiADD].Value;
                    m_dgvData[j, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                }               

                sTableValue sModiftIDList = new sTableValue();
                //sModiftIDList.nRowIndex = m_dgvData.RowCount - 1;
                sModiftIDList.nTabIndex = 1;
                sModiftIDList.nIndexKey = Convert.ToInt32(m_dgvData["ProductNo", m_dgvData.RowCount - 1].Value.ToString());
                sModiftIDList.bCell = false;
                //09. 06. 01 김정협 수정 : ID는 따로 제일 마지막에 대입 바로 정렬됨
                

                sUndoRedo sUndoData = new sUndoRedo();
                sUndoData.nTabIndex = 1;
                sUndoData.nType = 2;//2. Add 수정
                sUndoData.sModyfyList = sModiftIDList;

                //09. 07. 02 김정협 Undo그룹짓기
                if (m_UndoStack.Count > 0)
                {
                    sUndoData.nGroupID = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                }
                else
                {
                    sUndoData.nGroupID = 0;
                }
                //DataTable tblTemp = m_tblItem.Clone();
                //tblTemp = m_tblItem.Copy();
                //sUndoData.drAdd = tblTemp.Rows[tblTemp.Rows.Count - 1];


                m_UndoStack.Push(sUndoData);
                m_RedoStack.Clear();

                m_ModifyIDList.Add(sModiftIDList);


                ((BillingTable)this.Owner).m_bSaveEnd = false;
                for (int i = 0; i < m_dgvData.RowCount; i++)
                {
                    if (sModiftIDList.nIndexKey == Convert.ToInt32(m_dgvData["ProductNo", i].Value.ToString()))
                    {
                        m_dgvData.CurrentCell = m_dgvData["ProductNo", i];
                        break;
                    }
                }
            }

            //ColorRefresh실행해야함
            ((BillingTable)this.Owner).ColorRefresh(true);
                        
            this.Hide();

        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {

                if (m_tblAdd.Columns[i].DataType == Type.GetType("System.String"))
                {
                    dataGridView1[i, 0].Value = "";
                }
                else if (m_tblAdd.Columns[i].DataType == Type.GetType("System.Boolean"))
                {
                    dataGridView1[i, 0].Value = false;
                }
                else
                {
                    dataGridView1[i, 0].Value = 0;
                }

            }     
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        //09. 07. 01 김정협 ADD 창 떠 있을 시 계속 추가되게하는 부분
        public void ADDItem()
        {
            m_tblAdd.Columns["ProductNo"].AllowDBNull = true;
            m_tblAdd.Columns["ProductNo"].Unique = false;

            if (m_dgvData.SelectedRows.Count > 0)
            {
                for (int i = 0; i < m_dgvData.SelectedRows.Count; i++)
                {
                    m_tblAdd.Rows.Add();
                    //09. 07. 01 김정협 ADD 창 떠 있을 시 계속 추가되게하는 부분
                    DataRow AddRow = m_tblAdd.Rows[m_tblAdd.Rows.Count - 1];



                    //09. 06. 01 김정협 수정 : 인덱스키를 전부 검색
                    //int nCount = 0;
                    DataRow OriginalRow = null;// = m_tblItem.Rows[nCount];

                    for (int nCount = 0; nCount < m_tblInfo.Rows.Count; nCount++)
                    {
                        if (m_tblInfo.Rows[nCount].RowState == DataRowState.Deleted)
                        {
                            continue;
                        }
                        OriginalRow = m_tblInfo.Rows[nCount];
                        if (m_dgvData["ProductNo", m_dgvData.SelectedRows[i].Index].Value.ToString() == OriginalRow["ProductNo"].ToString())
                        {
                            break;
                        }
                    }
             

                    for (int j = 0; j < m_tblAdd.Columns.Count; j++)
                    {
                        AddRow[j] = OriginalRow[j];//m_dgvData[j, m_nRowIndex + i];
                    }
                }
            }
            else
            {
                m_tblAdd.Rows.Add();
                //09. 07. 01 김정협 ADD 창 떠 있을 시 계속 추가되게하는 부분
                DataRow AddRow = m_tblAdd.Rows[m_tblAdd.Rows.Count - 1];



                //09. 06. 01 김정협 수정 : 인덱스키를 전부 검색
                //int nCount = 0;
                DataRow OriginalRow = null;// = m_tblItem.Rows[nCount];

                for (int nCount = 0; nCount < m_tblInfo.Rows.Count; nCount++)
                {
                    if (m_tblInfo.Rows[nCount].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    OriginalRow = m_tblInfo.Rows[nCount];
                    if (m_dgvData["ProductNo", m_nRowIndex].Value.ToString() == OriginalRow["ProductNo"].ToString())
                    {
                        break;
                    }
                }

                for (int j = 0; j < m_tblAdd.Columns.Count; j++)
                {
                    AddRow[j] = OriginalRow[j];//m_dgvData[j, m_nRowIndex + i];
                }
            }
            
        }
        

       

        private void dataGridView1_KeyDown(object sender, KeyEventArgs e)
        {
            //dataGridView1.ClipboardCopyMode = DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.X)
            {
                Clipboard.SetDataObject(dataGridView1.GetClipboardContent().GetText());
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
                        //09. 07. 02 김정협 붙여넣기시 링크 에러 해결 및 붙여넣기 영역 벗어
                        //났을경우 에러나는부분 해결
                        if (dataGridView1.ColumnCount <= nColumnIndex ||
                            dataGridView1.RowCount <= nRowIndex)
                        {
                            continue;
                        }
                        if (!dataGridView1[nColumnIndex, nRowIndex].ReadOnly)
                        {                            
                            m_dgvcTemp = dataGridView1[nColumnIndex, nRowIndex];                            
                            dataGridView1[nColumnIndex, nRowIndex].Value = strData[j];                            
                            dataGridView1.RefreshEdit();
                        }
                        nColumnIndex++;
                    }
                    nColumnIndex = dataGridView1.CurrentCell.ColumnIndex;
                    nRowIndex++;
                }
            }
        }

        private void ProductInfoAdd_Load(object sender, EventArgs e)
        {

            m_tblAdd = new DataTable("tblAdd");

            dataGridView1.AllowUserToAddRows = false;

            DataColumn Col;

            for (int i = 0; i < m_tblInfo.Columns.Count; i++)
            {
                Col = new DataColumn(m_tblInfo.Columns[i].ColumnName, m_tblInfo.Columns[i].DataType);
                Col.MaxLength = m_tblInfo.Columns[i].MaxLength;
                Col.AllowDBNull = m_tblInfo.Columns[i].AllowDBNull;
                Col.Unique = m_tblInfo.Columns[i].Unique;

                m_tblAdd.Columns.Add(Col);
            }

            if (m_nRowIndex != -1)
            {
                ADDItem();
            }
            else
            {
                m_tblAdd.Columns["ProductNo"].AllowDBNull = true;
                m_tblAdd.Columns["ProductNo"].Unique = false;
                m_tblAdd.Rows.Add();
            }

            dataGridView1.DataSource = m_tblAdd;

        }

        private void ProductInfoAdd_Resize(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - 30;
            dataGridView1.Height = this.Height - 112;

            btnCancel.Left = dataGridView1.Location.X + dataGridView1.Width - btnCancel.Width;
            btnCancel.Top = dataGridView1.Location.Y + dataGridView1.Height + 5;

            btnClear.Left = btnCancel.Left - 15 - btnClear.Width;
            btnClear.Top = btnCancel.Top;

            btnAdd.Left = btnClear.Left - 15 - btnAdd.Width;
            btnAdd.Top = btnCancel.Top;
        }
    }
}