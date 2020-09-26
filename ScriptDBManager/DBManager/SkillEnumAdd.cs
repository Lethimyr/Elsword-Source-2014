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
    public partial class SkillEnumAdd : Form
    {
        DataGridView m_dgvData;
        DataTable m_tblSkillEnum;
        DataTable m_tblAdd;
        //DataTable m_tblString;
        DataTable m_tblTrans;
        DataTable m_tblRelease;

        ArrayList m_ModifyIDList;

        int m_nRowIndex;
        object objTemp;
                
        Stack m_UndoStack;
        Stack m_RedoStack;

        //09. 07. 02 김정협 붙여넣기시 링크 에러 해결
        bool m_bPaste = false;
        DataGridViewCell m_dgvcTemp;

        public void SetUndoStack(Stack UndoStack)
        {
            m_UndoStack = UndoStack;
        }
        public void SetRedoStack(Stack RedoStack)
        {
            m_RedoStack = RedoStack;
        }

        //09. 06. 19 Trans / Release연결 추가
        public void SetTrans(DataTable tblTrans)
        {
            m_tblTrans = tblTrans;
        }
        public void SetRelease(DataTable tblRelease)
        {
            m_tblRelease = tblRelease;
        }

        public void SetDataTable(DataTable tblData)
        {
            m_tblSkillEnum = tblData;
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

        public SkillEnumAdd()
        {
            InitializeComponent();
        }

        private void SkillEnumAdd_Load(object sender, EventArgs e)
        {

            m_tblAdd = new DataTable("tblAdd");

            dataGridView1.AllowUserToAddRows = false;

            DataColumn Col;
            m_tblSkillEnum.Columns["ID Num"].AllowDBNull = true;
            for (int i = 1; i < m_tblSkillEnum.Columns.Count - 1; i++)
            {
                Col = new DataColumn(m_tblSkillEnum.Columns[i].ColumnName, m_tblSkillEnum.Columns[i].DataType);
                Col.MaxLength = m_tblSkillEnum.Columns[i].MaxLength;
                Col.AllowDBNull = m_tblSkillEnum.Columns[i].AllowDBNull;
                Col.Unique = m_tblSkillEnum.Columns[i].Unique;

                m_tblAdd.Columns.Add(Col);
            }

            ADDItem();

            dataGridView1.DataSource = m_tblAdd;
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

        private void btnAdd_Click(object sender, EventArgs e)
        {
            for (int nMultiADD = 0; nMultiADD < dataGridView1.RowCount; nMultiADD++)
            {
                for (int j = 0; j < m_dgvData.RowCount; j++)
                {
                    if (dataGridView1["ID Num", nMultiADD].Value.Equals(m_dgvData["ID Num", j].Value))
                    {
                        MessageBox.Show("동일한 ID가 존재합니다. 다시 입력해주십시요.");
                        return;
                    }
                }
            }

            for (int nMultiADD = 0; nMultiADD < dataGridView1.RowCount; nMultiADD++)
            {            
                //09. 06. 01 김정협 수정 : 인덱스 카운터를 로우 카운터에서 얻어옴
                int nIndexKey = Convert.ToInt32(m_tblSkillEnum.Rows[m_tblSkillEnum.Rows.Count - 1]["IndexKey"].ToString());//Converty.ToInt32(m_dgvData[0, m_dgvData.RowCount - 1].Value.ToString());
                nIndexKey++;


                m_tblSkillEnum.Rows.Add();

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

                ///////////////////////////////////////////////////////////////////
                //스트링 시작
                ///////////////////////////////////////////////////////////////////

                //int nIndexCount = ((QuestTable)this.Owner).CurrentMaxStringIndex();
                //nIndexCount++;

                m_tblTrans.Rows.Add();
                DataRow Row = m_tblTrans.Rows[m_tblTrans.Rows.Count - 1];
                //Row["TransIDX"] = nIndexCount;
                Row["CountryCode"] = "KR";
                Row["DB"] = "ScriptData";
                Row["TableName"] = "SkillEnum";
                Row["Column"] = "Description";
                Row["Index"] = Convert.ToInt32(m_tblSkillEnum.Rows[m_tblSkillEnum.Rows.Count - 1]["IndexKey"].ToString());//m_dgvData.RowCount - 1;
                Row["SubKey"] = Convert.ToInt32(m_tblSkillEnum.Rows[m_tblSkillEnum.Rows.Count - 1]["ID Num"].ToString());
                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                Row["String"] = m_dgvData["Description", 0].Value;
                Row["Create Date"] = DateTime.Now;
                Row["Last Update Date"] = DateTime.Now;

                ///////////////////////////////////////////////////////////////////
                //스트링 끝
                ///////////////////////////////////////////////////////////////////


                m_dgvData["Regdate", 0].Value = DateTime.Now;
                // m_dgvData["Regdate", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                //09. 06. 19 김정협 Release 추가

                m_tblRelease.Rows.Add();
                Row = m_tblRelease.Rows[m_tblRelease.Rows.Count - 1];
                //Row["TransIDX"] = nIndexCount;
                Row["CountryCode"] = "KR";
                Row["DB"] = "ScriptData";
                Row["TableName"] = "SkillEnum";
                Row["Index"] = Convert.ToInt32(m_tblSkillEnum.Rows[m_tblSkillEnum.Rows.Count - 1]["IndexKey"].ToString());//m_dgvData.RowCount - 1;
                Row["Name"] = m_dgvData["Description", 0].Value;
                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                Row["State"] = true;
                Row["Create Date"] = DateTime.Now;
                Row["Last Update Date"] = DateTime.Now;

               
                sTableValue sModiftIDList = new sTableValue();
                //sModiftIDList.nRowIndex = m_dgvData.RowCount - 1;
                sModiftIDList.nTabIndex = ((SkillTable)this.Owner).GetCurrentTab();
                sModiftIDList.nIndexKey = Convert.ToInt32(m_dgvData["IndexKey", 0].Value.ToString());
                sModiftIDList.bCell = false;
                //09. 06. 01 김정협 수정 : ID는 따로 제일 마지막에 대입 바로 정렬됨
                m_dgvData["ID Num", 0].Value = dataGridView1["ID Num", nMultiADD].Value;

                sUndoRedo sUndoData = new sUndoRedo();
                sUndoData.nTabIndex = 16;
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


                ((SkillTable)this.Owner).m_bSaveEnd = false;
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
            ((SkillTable)this.Owner).ColorRefresh(true);

            for (int nRowHeader = 0; nRowHeader < m_dgvData.RowCount; nRowHeader++)
            {
                m_dgvData.Rows[nRowHeader].HeaderCell.Value = m_dgvData["ID Num", nRowHeader].Value.ToString() + "     " + m_dgvData["Description", nRowHeader].Value.ToString();
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

            ((SkillTable)this.Owner).SetCellBackgroundColorForDB(16);

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

                    for (int nCount = 0; nCount < m_tblSkillEnum.Rows.Count; nCount++)
                    {
                        if (m_tblSkillEnum.Rows[nCount].RowState == DataRowState.Deleted)
                        {
                            continue;
                        }
                        OriginalRow = m_tblSkillEnum.Rows[nCount];
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

                for (int nCount = 0; nCount < m_tblSkillEnum.Rows.Count; nCount++)
                {
                    if (m_tblSkillEnum.Rows[nCount].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    OriginalRow = m_tblSkillEnum.Rows[nCount];
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

            
        }

        private void SkillEnumAdd_Resize(object sender, EventArgs e)
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