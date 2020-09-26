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
    public partial class MultiAdd : Form
    {
        DataGridView m_dgvData;
        DataTable m_tblItem;
        DataTable m_tblMultiAdd;
        DataTable m_tblTrans;

        ArrayList m_ModifyIDList;

        int m_nRowIndex;

        object objTemp;

        //09. 06. 06 김정협 자동업데이트를 위하여 추가
        DataTable tblItemType;
        DataTable tblUseType;
        DataTable tblPeriodType;
        DataTable tblPriceType;
        DataTable tblUseCondition;
        DataTable tblUnitType;
        DataTable tblUnitClass;
        DataTable tblEqipPosition;
        DataTable tblItemGrade;


        Stack m_UndoStack;
        Stack m_RedoStack;

        public void SetDataTable(DataTable tblItem)
        {
            m_tblItem = tblItem;
            
            //m_tblItem.
                
        }

        public void SetRowIndex(int nCurrentRowIndex)
        {
            m_nRowIndex = nCurrentRowIndex;
        }

        public void SetUndoStack(Stack UndoStack)
        {
            m_UndoStack = UndoStack;
        }
        public void SetRedoStack(Stack RedoStack)
        {
            m_RedoStack = RedoStack;
        }

        public void SetTrans(DataTable tblTrans)
        {
            m_tblTrans = tblTrans;
        }

        public void SetDataGridView(DataGridView dgvData)
        {
            m_dgvData = dgvData;
        }
        public void SetModifyArray(ArrayList ModifyIDList)
        {
            m_ModifyIDList = ModifyIDList;
        }

        public void SetItemType(DataTable tblData)
        {
            tblItemType = tblData;
        }

        public void SetUseType(DataTable tblData)
        {
            tblUseType = tblData;
        }

        public void SePeriodType(DataTable tblData)
        {
            tblPeriodType = tblData;
        }

        public void SetPriceType(DataTable tblData)
        {
            tblPriceType = tblData;
        }

        public void SetUseCondition(DataTable tblData)
        {
            tblUseCondition = tblData;
        }

        public void SetUnitType(DataTable tblData)
        {
            tblUnitType = tblData;
        }

        public void SetUnitClass(DataTable tblData)
        {
            tblUnitClass = tblData;
        }

        public void SetEqipPosition(DataTable tblData)
        {
            tblEqipPosition = tblData;
        }

        public void SetItemGrade(DataTable tblData)
        {
            tblItemGrade = tblData;
        }

        
        public MultiAdd()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < dataGridView1.RowCount - 1; i++)
            {
                for(int j = 0; j < m_dgvData.RowCount; j++)
                {
                    if(dataGridView1["ID", i].Value.Equals(m_dgvData["ID", j].Value))
                    {
                        MessageBox.Show("동일한 ID가 존재합니다. 다시 입력해주십시요.");
                        return;
                    }
                }                
            }

            //09. 06. 01 김정협 수정 : 인덱스 카운터를 로우 카운터에서 얻어옴
            int nIndexKey = m_dgvData.RowCount - 1;//Converty.ToInt32(m_dgvData[0, m_dgvData.RowCount - 1].Value.ToString());
            nIndexKey++;

            sTableValue sModiftIDList = new sTableValue();
            m_tblItem.Columns["ID"].AllowDBNull = true; 

            for (int i = 0; i < m_tblMultiAdd.Rows.Count; i ++ )
            {                               
                m_tblItem.Rows.Add();

                //09. 06. 01 김정협 수정 : 0부터 시작 m_dgvData[0, m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                m_dgvData[0, 0].Value = nIndexKey;
                // m_dgvData[0, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                nIndexKey++;


                //09. 06. 01 김정협 수정 : 1부터 시작 ID는 제일 나중에 대입해야함. 넣는즉시 정렬
                for (int j = 1; j < m_tblMultiAdd.Columns.Count; j++)
                {
                    m_dgvData[j + 1, 0].Value = dataGridView1[j, 0].Value;
                    // m_dgvData[j + 1, m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;
                }

                ///////////////////////////////////////////////////////////////////
                //스트링 시작
                ///////////////////////////////////////////////////////////////////


                int nIndexCount = ((ItemTable)this.Owner).CurrentMaxStringIndex();
                nIndexCount++;

                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                m_dgvData["Index_Name", 0].Value = nIndexCount;
                // m_dgvData["Index_Name", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;

                m_tblTrans.Rows.Add();
                DataRow Row = m_tblTrans.Rows[m_tblTrans.Rows.Count - 1];
                Row["StringIndex"] = nIndexCount;
                Row["TableName"] = "Item";
                Row["Column"] = "Name";
                Row["Row"] = m_dgvData.RowCount - 1;
                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                Row["String"] = m_dgvData["Name", 0].Value;
                Row["Regdate"] = DateTime.Now;
                nIndexCount++;

                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                m_dgvData["Index_Description", 0].Value = nIndexCount;
                //m_dgvData["Index_Description", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;

                m_tblTrans.Rows.Add();
                Row = m_tblTrans.Rows[m_tblTrans.Rows.Count - 1];
                Row["StringIndex"] = nIndexCount;
                Row["TableName"] = "Item";
                Row["Column"] = "Description";
                Row["Row"] = m_dgvData.RowCount - 1;
                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                Row["String"] = m_dgvData["Description", 0].Value;
                Row["Regdate"] = DateTime.Now;
                nIndexCount++;

                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                m_dgvData["Index_Description_In_Shop", 0].Value = nIndexCount;
                //m_dgvData["Index_Description_In_Shop", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;

                m_tblTrans.Rows.Add();
                Row = m_tblTrans.Rows[m_tblTrans.Rows.Count - 1];
                Row["StringIndex"] = nIndexCount;
                Row["TableName"] = "Item";
                Row["Column"] = "Description In Shop";
                Row["Row"] = m_dgvData.RowCount - 1;
                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                Row["String"] = m_dgvData["Description In Shop", 0].Value;
                Row["Regdate"] = DateTime.Now;
                nIndexCount++;

                ///////////////////////////////////////////////////////////////////
                //스트링 끝
                ///////////////////////////////////////////////////////////////////

                //09. 06. 01 김정협 수정 : 0으로함  m_dgvData.RowCount - 1]이였지만 자동정렬이므로 추가하면 바로 첫행임
                m_dgvData["Regdate", 0].Value = DateTime.Now;
                //m_dgvData["Regdate", m_dgvData.RowCount - 1].Style.ForeColor = Color.Red;


                
                //sModiftIDList.nRowIndex = m_dgvData.RowCount - 1;
                sModiftIDList.nTabIndex = ((ItemTable)this.Owner).GetCurrentTab();
                sModiftIDList.nIndexKey = Convert.ToInt32(m_dgvData["IndexKey", 0].Value.ToString());
                sModiftIDList.bCell = false;

                //09. 06. 01 김정협 수정 : ID는 따로 제일 마지막에 대입 바로 정렬됨
                m_dgvData["ID", 0].Value = dataGridView1["ID", i].Value;

                sUndoRedo sUndoData = new sUndoRedo();
                sUndoData.nTabIndex = 0;
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
                
                
                m_ModifyIDList.Add(sModiftIDList);
                //m_ModifyIDList.Add(m_dgvData.RowCount - 1);

            }

            m_RedoStack.Clear();

            ((ItemTable)this.Owner).m_bSaveEnd = false;
            for (int i = 0; i < m_dgvData.RowCount; i++)
            {
                if (sModiftIDList.nIndexKey == Convert.ToInt32(m_dgvData["IndexKey", i].Value.ToString()))
                {
                    m_dgvData.CurrentCell = m_dgvData["IndexKey", i];
                    break;
                }
            }

            //ColorRefresh실행해야함
            ((ItemTable)this.Owner).ColorRefresh(true);

            this.Hide();            

        }

        private void MultiAdd_Load(object sender, EventArgs e)
        {
           // dataGridView1.DataSource = m_tblItem;
            //m_dgvData.AllowUserToAddRows = false;
            m_tblMultiAdd =  new DataTable("tblMultiAdd");

            DataRow Row = m_tblItem.Rows[m_nRowIndex];

            if(Convert.ToInt32(Row["Item Type"].ToString()) == 1)
            {
                string[] strList = new string[9];
                strList[0] = "쓸만한";
                strList[1] = "균형잡힌";
                strList[2] = "연마된";
                strList[3] = "마력의";
                strList[4] = "예리한";
                strList[5] = "가벼운";
                strList[6] = "정밀한";
                strList[7] = "축복받은";
                strList[8] = "궁극의";
                               

                string strItemName = Row["Name"].ToString();
                for(int i = 0; i < 9; i++)
                {
                    if (strItemName.IndexOf(strList[i]) > -1)
                    {
                        //Row = m_tblItem.Rows[m_nRowIndex - (i + 1)];                        
                        m_nRowIndex = m_nRowIndex - (i + 1);
                        break;
                    }
                }
                //m_tblMultiAdd = m_tblItem;

                DataColumn Col;

                for (int i = 1; i < m_tblItem.Columns.Count - 4; i++)                
                {
                    Col = new DataColumn(m_tblItem.Columns[i].ColumnName, m_tblItem.Columns[i].DataType);
                    Col.MaxLength = m_tblItem.Columns[i].MaxLength;
                    Col.AllowDBNull = m_tblItem.Columns[i].AllowDBNull;
                    Col.Unique = m_tblItem.Columns[i].Unique;

                    m_tblMultiAdd.Columns.Add(Col);
                }

                //9개 중 지워진 것이 없나 검사
                for (int i = 0; i <= 9; i++)
                {
                    if (m_nRowIndex + i >= m_tblItem.Rows.Count)
                    {
                        MessageBox.Show("Multi ADD할 조건이 충족되지 않은 아이템입니다.");
                        this.Hide();                        
                        return;
                    }
                    if (m_tblItem.Rows[m_nRowIndex + i].RowState == DataRowState.Deleted)                        
                    {
                        MessageBox.Show("Multi ADD할 조건이 충족되지 않은 아이템입니다.");
                        this.Hide();
                        return;
                    }
                }

                                
                for(int i = 0; i <=9; i++)
                {
                    m_tblMultiAdd.Columns["ID"].AllowDBNull = true;                    
                    m_tblMultiAdd.Rows.Add();
                    DataRow MultiRow = m_tblMultiAdd.Rows[i];                    
                    DataRow OriginalRow = m_tblItem.Rows[m_nRowIndex + i];
                    for (int j = 0; j < m_tblMultiAdd.Columns.Count; j++)
                    {
                        MultiRow[j] = OriginalRow[j + 1];//m_dgvData[j, m_nRowIndex + i];
                    }
                }
                dataGridView1.DataSource = m_tblMultiAdd;
                dataGridView1.AllowUserToAddRows = false;
            }
            else if(Convert.ToInt32(Row["Item Type"].ToString()) == 2)
            {
                string[] strList = new string[17];
                strList[0] = "쓸만한";
                strList[1] = "양질의";
                strList[2] = "두꺼운";
                strList[3] = "세련된";
                strList[4] = "가벼운";
                strList[5] = "체력의";
                strList[6] = "편한";
                strList[7] = "반짝이는";
                strList[8] = "빛나는";
                strList[9] = "불타는";
                strList[10] = "냉기의";
                strList[11] = "녹음의";
                strList[12] = "바람의";
                strList[13] = "성스러운";
                strList[14] = "타락한";
                strList[15] = "축복받은";
                strList[16] = "궁극의";

                string strItemName = Row["Name"].ToString();
                for (int i = 0; i < 17; i++)
                {
                    if (strItemName.IndexOf(strList[i]) > -1)
                    {
                        //Row = m_tblItem.Rows[m_nRowIndex - (i + 1)];                        
                        m_nRowIndex = m_nRowIndex - (i + 1);
                        break;
                    }
                }
                //m_tblMultiAdd = m_tblItem;

                DataColumn Col;

                for (int i = 1; i < m_tblItem.Columns.Count - 4; i++)
                {
                    Col = new DataColumn(m_tblItem.Columns[i].ColumnName, m_tblItem.Columns[i].DataType);
                    Col.MaxLength = m_tblItem.Columns[i].MaxLength;
                    Col.AllowDBNull = m_tblItem.Columns[i].AllowDBNull;
                    Col.Unique = m_tblItem.Columns[i].Unique;

                    m_tblMultiAdd.Columns.Add(Col);
                }
                //17개 중 지워진 것이 없나 검사
                for (int i = 0; i <= 17; i++ )
                {
                    if(m_nRowIndex + i >= m_tblItem.Rows.Count)
                    {
                        MessageBox.Show("Multi ADD할 조건이 충족되지 않은 아이템입니다.");
                        this.Hide();
                        return;
                    }
                    if(m_tblItem.Rows[m_nRowIndex + i].RowState == DataRowState.Deleted)
                        
                    {
                        MessageBox.Show("Multi ADD할 조건이 충족되지 않은 아이템입니다.");
                        this.Hide();
                        return;
                    }
                }

                for (int i = 0; i <= 17; i++)
                {
                    m_tblMultiAdd.Columns["ID"].AllowDBNull = true;
                    m_tblMultiAdd.Rows.Add();
                    DataRow MultiRow = m_tblMultiAdd.Rows[i];
                    DataRow OriginalRow = m_tblItem.Rows[m_nRowIndex + i];
                    for (int j = 0; j < m_tblMultiAdd.Columns.Count; j++)
                    {
                        MultiRow[j] = OriginalRow[j + 1];//m_dgvData[j, m_nRowIndex + i];
                    }
                }
                dataGridView1.DataSource = m_tblMultiAdd;
                dataGridView1.AllowUserToAddRows = false;

                dataGridView1.Columns["Item Type_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["ITEMTYPE_C"].ReadOnly = true; //수정방지
                dataGridView1.Columns["Use Type_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["USETYPE_C"].ReadOnly = true; //수정방지
                dataGridView1.Columns["Period Type_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["PERIODTYPE_C"].ReadOnly = true; //수정방지

                dataGridView1.Columns["Price Type_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["PRICETYPE_C"].ReadOnly = true; //수정방지
                dataGridView1.Columns["Use Condition_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["USECONDITION_C"].ReadOnly = true; //수정방지
                dataGridView1.Columns["Unit Type_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["UNITTYPE_C"].ReadOnly = true; //수정방지
                dataGridView1.Columns["Unit Class_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["UNITCLASS_C"].ReadOnly = true; //수정방지
                dataGridView1.Columns["Eqip Position_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["EQIPPOSITION_C"].ReadOnly = true; //수정방지
                dataGridView1.Columns["Item Grade_"].ReadOnly = true; //수정방지
                dataGridView1.Columns["ITEMGRADE_C"].ReadOnly = true; //수정방지

            }

            
            
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void dataGridView1_CellBeginEdit(object sender, DataGridViewCellCancelEventArgs e)
        {
            objTemp = dataGridView1.CurrentCell.Value;           
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if (!objTemp.Equals((object)dataGridView1.CurrentCell.Value))
            {
                dataGridView1.CurrentCell.Style.ForeColor = Color.Red;
                
                if (dataGridView1.CurrentCell.ColumnIndex == 0)
                {
                    for (int i = 0; i < dataGridView1.RowCount -1; i++)
                    {
                        if (dataGridView1["ID", i].Value.ToString() == dataGridView1.CurrentCell.Value.ToString())
                        {
                            if (i == dataGridView1.CurrentCell.RowIndex)
                            {
                                continue;
                            }

                            MessageBox.Show("동일한 ID가 존재합니다.");

                            dataGridView1.CurrentCell.Value = objTemp;
                            dataGridView1.CurrentCell.Style.ForeColor = Color.Black;                            
                            return;

                        }
                    }
                }

                //m_ModifyIDList.Add(dataGridView1.CurrentCell.RowIndex);
                //m_bSaveEnd = false;
            }

            if (m_tblMultiAdd.Columns[dataGridView1.CurrentCell.ColumnIndex].DataType != Type.GetType("System.String") &&
                m_tblMultiAdd.Columns[dataGridView1.CurrentCell.ColumnIndex].DataType != Type.GetType("System.Boolean"))
            {
                if (dataGridView1.CurrentCell.Value.ToString() == "")
                    dataGridView1.CurrentCell.Value = 0;
            }

            int nIndexRow = dataGridView1.CurrentCell.RowIndex;


            //09. 06. 06 김정협 자동업데이트를 위하여 추가
            if (dataGridView1.CurrentCell.OwningColumn.Name == "Item Type")
            {
                DataRow dr = tblItemType.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Item Type_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["ITEMTYPE_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Use Type")
            {
                DataRow dr = tblUseType.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Use Type_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["USETYPE_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Period Type")
            {
                DataRow dr = tblPeriodType.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Period Type_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["PERIODTYPE_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Price Type")
            {
                DataRow dr = tblPriceType.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Price Type_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["PRICETYPE_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Use Condition")
            {
                DataRow dr = tblUseCondition.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Use Condition_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["USECONDITION_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Unit Type")
            {
                DataRow dr = tblUnitType.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Unit Type_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["UNITTYPE_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Unit Class")
            {
                DataRow dr = tblUnitClass.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Unit Class_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["UNITCLASS_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Eqip Position")
            {
                DataRow dr = tblEqipPosition.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Eqip Position_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["EQIPPOSITION_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }

            if (dataGridView1.CurrentCell.OwningColumn.Name == "Item Grade")
            {
                DataRow dr = tblItemGrade.Rows.Find(dataGridView1.CurrentCell.Value.ToString());
                if (dr != null)
                {
                    dataGridView1["Item Grade_", nIndexRow].Value = dr["Des"].ToString();
                    dataGridView1["ITEMGRADE_C", nIndexRow].Value = dr["CODE"].ToString();
                }
                else
                {
                    MessageBox.Show("존재하지 않는 값입니다.");
                    dataGridView1.CurrentCell.Value = objTemp;
                }
            }



        }
    }
}