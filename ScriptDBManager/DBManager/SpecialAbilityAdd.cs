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
    public partial class SpecialAbilityAdd : Form
    {
        DataGridView m_dgvData;
        DataTable m_tblItem;
        DataTable m_tblAdd;



        ArrayList m_ModifyIDList;

        int m_nRowIndex;

        object objTemp;

        //09. 06. 06 김정협 자동업데이트를 위하여 추가
        DataTable tblSpecialAbilityType;

        Stack m_UndoStack;
        Stack m_RedoStack;

        //09. 07. 02 김정협 붙여넣기시 링크 에러 해결
        bool m_bPaste = false;
        DataGridViewCell m_dgvcTemp;

        public SpecialAbilityAdd()
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
            m_tblItem = tblItem;
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

        //09. 06. 06 김정협 자동업데이트를 위하여 추가
        public void SetSpecialAbility(DataTable tblData)
        {
            tblSpecialAbilityType = tblData;
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            for (int nMultiADD = 0; nMultiADD < dataGridView1.RowCount; nMultiADD++)
            {
                for (int j = 0; j < m_dgvData.RowCount; j++)
                {
                    if (dataGridView1["ID", nMultiADD].Value.Equals(m_dgvData["ID", j].Value))
                    {
                        MessageBox.Show("동일한 ID가 존재합니다. 다시 입력해주십시요.");
                        return;
                    }
                }
            }

            for (int nMultiADD = 0; nMultiADD < dataGridView1.RowCount; nMultiADD++)
            {             

                //09. 06. 01 김정협 수정 : 인덱스 카운터를 로우 카운터에서 얻어옴
                int nIndexKey = Convert.ToInt32(m_tblItem.Rows[m_tblItem.Rows.Count - 1]["IndexKey"].ToString());//Converty.ToInt32(m_dgvData[0, m_dgvData.RowCount - 1].Value.ToString());
                nIndexKey++;

                m_tblItem.Rows.Add();


                //09. 06. 01 김정협 수정 : 0부터 시작 m_dgvData[0, m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                m_dgvData[0, 0].Value = nIndexKey;
                // m_dgvData[0, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                nIndexKey++;


                //09. 06. 01 김정협 수정 : 1부터 시작 ID는 제일 나중에 대입해야함. 넣는즉시 정렬
                for (int j = 1; j < m_tblAdd.Columns.Count; j++)
                {
                    m_dgvData[j + 1, 0].Value = dataGridView1[j, nMultiADD].Value;
                    // m_dgvData[j + 1, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                }


                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                m_dgvData["Regdate", 0].Value = DateTime.Now;
                //m_dgvData["Regdate", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                sTableValue sModiftIDList = new sTableValue();
                //sModiftIDList.nRowIndex = m_tblItem.Rows.Count - 1; //tbl은 datagridview와 상관없이 IndexKet순으로정렬되어있으므로 이값으로 넘김
                sModiftIDList.nTabIndex = ((ItemTable)this.Owner).GetCurrentTab();
                sModiftIDList.nIndexKey = Convert.ToInt32(m_dgvData["IndexKey", 0].Value.ToString());
                sModiftIDList.bCell = false;


                //09. 06. 01 김정협 수정 : ID는 따로 제일 마지막에 대입 바로 정렬됨
                m_dgvData["ID", 0].Value = dataGridView1["ID", nMultiADD].Value;


                sUndoRedo sUndoData = new sUndoRedo();
                sUndoData.nTabIndex = 1;
                sUndoData.nType = 2;//2. Add 수정
                sUndoData.sModyfyList = sModiftIDList;
                //DataTable tblTemp = m_tblItem.Clone();
                //tblTemp = m_tblItem.Copy();
                //sUndoData.drAdd = tblTemp.Rows[tblTemp.Rows.Count - 1];
                //09. 07. 02 김정협 Undo그룹짓기
                if (m_UndoStack.Count > 0)
                {
                    sUndoData.nGroupID = ((sUndoRedo)m_UndoStack.Peek()).nGroupID + 1;
                }
                else
                {
                    sUndoData.nGroupID = 0;
                }

                m_UndoStack.Push(sUndoData);
                m_RedoStack.Clear();

                m_ModifyIDList.Add(sModiftIDList);


                ((ItemTable)this.Owner).m_bSaveEnd = false;
                for (int i = 0; i < m_dgvData.RowCount; i++)
                {
                    if (sModiftIDList.nIndexKey == Convert.ToInt32(m_dgvData["IndexKey", i].Value.ToString()))
                    {
                        m_dgvData.CurrentCell = m_dgvData["IndexKey", i];
                        break;
                    }
                }
            }

            //ColorRefresh실행해야함
            ((ItemTable)this.Owner).ColorRefresh(true);

            for (int nRowHeader = 0; nRowHeader < m_dgvData.RowCount; nRowHeader++)
            {
                m_dgvData.Rows[nRowHeader].HeaderCell.Value = m_dgvData["ID", nRowHeader].Value.ToString() + "     " + m_dgvData["TYPE_", nRowHeader].Value.ToString();
            }

            //09. 07. 08 김정협 아이템 추가시 컬러 밀리는 현상 수정을 위한 것
            //현재 아이템이 추가된 datagridview만 수정하면 됨            
            for (int i = 0; i < m_dgvData.RowCount; i++)
            {
                for (int k = 0; k < m_dgvData.ColumnCount; k++)
                {
                    if (m_dgvData[k, i].HasStyle)
                        m_dgvData[k, i].Style.BackColor = Color.White;
                }
            }

            ((ItemTable)this.Owner).SetCellBackgroundColorForDB(1);

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

        private void btnEsc_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        //09. 07. 01 김정협 ADD 창 떠 있을 시 계속 추가되게하는 부분
        public void ADDItem()
        {
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

                    for (int nCount = 0; nCount < m_tblItem.Rows.Count; nCount++)
                    {
                        if (m_tblItem.Rows[nCount].RowState == DataRowState.Deleted)
                        {
                            continue;
                        }
                        OriginalRow = m_tblItem.Rows[nCount];
                        if (m_dgvData["IndexKey", m_dgvData.SelectedRows[i].Index].Value.ToString() == OriginalRow["IndexKey"].ToString())
                        {
                            break;
                        }
                    }
                    /*     while (m_dgvData["IndexKey", m_nRowIndex].Value.ToString() != OriginalRow["IndexKey"].ToString())
                         {
                             OriginalRow = m_tblItem.Rows[nCount];
                             nCount++;
                         }*/


                    for (int j = 0; j < m_tblAdd.Columns.Count; j++)
                    {
                        AddRow[j] = OriginalRow[j + 1];//m_dgvData[j, m_nRowIndex + i];
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

                for (int nCount = 0; nCount < m_tblItem.Rows.Count; nCount++)
                {
                    if (m_tblItem.Rows[nCount].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    OriginalRow = m_tblItem.Rows[nCount];
                    if (m_dgvData["IndexKey", m_nRowIndex].Value.ToString() == OriginalRow["IndexKey"].ToString())
                    {
                        break;
                    }
                }

                for (int j = 0; j < m_tblAdd.Columns.Count; j++)
                {
                    AddRow[j] = OriginalRow[j + 1];//m_dgvData[j, m_nRowIndex + i];
                }
            }

            //09. 07. 14 김정협 엑셀의 고정셀 기능을 위하여
            if (dataGridView1.DataSource != null)
            {
                for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
                {
                    dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["TYPE_", nRowHeader].Value.ToString();
                }
            }    
        }

        private void SpecialAbilityAdd_Load(object sender, EventArgs e)
        {
            m_tblAdd = new DataTable("tblAdd");

            dataGridView1.AllowUserToAddRows = false;

            DataColumn Col;

            for (int i = 1; i < m_tblItem.Columns.Count - 1; i++)
            {
                Col = new DataColumn(m_tblItem.Columns[i].ColumnName, m_tblItem.Columns[i].DataType);
                Col.MaxLength = m_tblItem.Columns[i].MaxLength;
                Col.AllowDBNull = m_tblItem.Columns[i].AllowDBNull;
                Col.Unique = m_tblItem.Columns[i].Unique;

                m_tblAdd.Columns.Add(Col);
            }


            ADDItem();


            dataGridView1.DataSource = m_tblAdd;
            dataGridView1.RowHeadersWidth = 130;
            dataGridView1.RowHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;

            //09. 06. 06 김정협 자동업데이트를 위하여 추가
            dataGridView1.Columns["TYPE_"].ReadOnly = true; //수정방지
            dataGridView1.Columns["TYPE_C"].ReadOnly = true; //수정방지  

            //09. 07. 14 김정협 엑셀의 고정셀 기능을 위하여
            for (int nRowHeader = 0; nRowHeader < dataGridView1.RowCount; nRowHeader++)
            {
                dataGridView1.Rows[nRowHeader].HeaderCell.Value = dataGridView1["ID", nRowHeader].Value.ToString() + "     " + dataGridView1["TYPE_", nRowHeader].Value.ToString();
            }

        }

        private void SpecialAbilityAdd_Resize(object sender, EventArgs e)
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

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            //09. 07. 02 김정협 붙여넣기시 링크 에러 해결
            //dataGridView1.CurrentCell -> m_dgvcTemp 로 리플레이스함

            DataGridViewCell dgvCurrentCell;
            if (!m_bPaste)
            {
                dgvCurrentCell = dataGridView1.CurrentCell;
            }
            else
            {
                dgvCurrentCell = m_dgvcTemp;
            }

            if (m_tblAdd.Columns[dgvCurrentCell.ColumnIndex].DataType != Type.GetType("System.String") &&
               m_tblAdd.Columns[dgvCurrentCell.ColumnIndex].DataType != Type.GetType("System.Boolean"))
            {
                if (dgvCurrentCell.Value.ToString() == "")
                    dgvCurrentCell.Value = 0;
            }

            int nIndexRow = dgvCurrentCell.RowIndex;


            //09. 06. 06 김정협 자동업데이트를 위하여 추가
            if (dgvCurrentCell.OwningColumn.Name == "TYPE")
            {
                DataRow dr = tblSpecialAbilityType.Rows.Find(dgvCurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["TYPE_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["TYPE_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dgvCurrentCell.Value = objTemp;
                }
            }

            //09. 07. 14 김정협 엑셀의 고정셀 기능을 위하여
            if (dgvCurrentCell.ColumnIndex == 2)
            {
                dataGridView1.Rows[dgvCurrentCell.RowIndex].HeaderCell.Value = dataGridView1["ID", dgvCurrentCell.RowIndex].Value.ToString() + "     " + dataGridView1["TYPE_", dgvCurrentCell.RowIndex].Value.ToString();
            }
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            //09. 07. 02 김정협 붙여넣기시 링크 에러 해결            
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

        }

   
    }
}