//09. 06. 24 김정협 나라별 define
// ItemTable.cs, QuestTable.cs, TitleTable.cs 세군대 정의를
// 모두 바꿔줘야 함.
#define __COUNTRY_KR__
//#define __COUNTRY_TW__
//#define __COUNTRY_HK__
//#define __COUNTRY_JP__


using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.SqlClient;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.IO;
using System.Net;
using System.Collections;
using DBManager.ScriptData_TWDataSetTableAdapters;
using DBManager.ScriptData_HKDataSetTableAdapters;
using DBManager.LocalizationDataSetTableAdapters;


namespace DBManager
{    

    public partial class TextureStringManager : Form
    {
        private SqlConnection Con;
        private SqlDataAdapter Adpt;
        DataTable tblTextureStringManager;
        object objTemp;
        

        //History용        
        private SqlDataAdapter Adpt_History;
        DataTable tblTextureStringManager_History;

        //DeleteHistory용        
        private SqlDataAdapter Adpt_DeleteHistory;
        DataTable tblTextureStringManager_DeleteHistory;

        string m_strID;        
        ArrayList m_ModifyIDList = new ArrayList();

        ArrayList m_DeleteRowList = new ArrayList();

        public bool m_bSaveEnd = true; //수정했을 시 저장한 후 끝내는지 확인하는 변수

        private int m_nStartPoint = 0;

        ScriptData_TWDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManagerTW = new ScriptData_TWDataSetTableAdapters.TextureStringManagerTableAdapter();
        ScriptData_HKDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManagerHK = new ScriptData_HKDataSetTableAdapters.TextureStringManagerTableAdapter();
        ScriptData_JPDataSetTableAdapters.TextureStringManagerTableAdapter Adpt_TextureStringManagerJP = new ScriptData_JPDataSetTableAdapters.TextureStringManagerTableAdapter();

        TransTableAdapter Adpt_Trans = new TransTableAdapter();

        string strCountryCode = string.Empty;

        //다중 붙여넣기에서 CellEditBegin 과 End를 사용하기 위해서 만듬
        private DataGridViewCell m_dgvcTemp;
        private bool m_bPaste = false;
       
        public TextureStringManager()
        {
            InitializeComponent();            
            
        }

        public void SetID(string strID)
        {
            m_strID = strID;
            
        }

        void StringSave()
        {
            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                for (int j = 0; j < dataGridView1.RowCount; j++)
                {
                    dataGridView1[i, j].Style.ForeColor = Color.Black;                    
                }
            }

            //History 추가
            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());

            string strMyIP = host.AddressList[0].ToString();
            for (int i = 0; i < host.AddressList.Length; i++)
            {
                if (host.AddressList[i].AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                {
                    strMyIP = host.AddressList[i].ToString();
                    break;
                }
            }
            

            for (int i = 0; i < m_ModifyIDList.Count; i++)
            {
                tblTextureStringManager_History.Rows.Add();
                DataRow Row_History = tblTextureStringManager_History.Rows[tblTextureStringManager_History.Rows.Count - 1];
                DataRow Row = null;

                for (int nRow = 0; nRow < tblTextureStringManager.Rows.Count; nRow++)
                {
                    if (tblTextureStringManager.Rows[nRow].RowState == DataRowState.Deleted)
                    {
                        continue;
                    }
                    if (tblTextureStringManager.Rows[nRow]["TextureIDX"].ToString() == m_ModifyIDList[i].ToString())
                    {
                        Row = tblTextureStringManager.Rows[nRow];
                    }
                }


                Row_History["TextureIDX"] = Row["TextureIDX"];
                Row_History["FilePath"] = Row["FilePath"];
                Row_History["String"] = Row["String"];
                Row_History["IsChange"] = Row["IsChange"];
                Row_History["bUse"] = Row["bUse"];
                Row_History["UserID"] = m_strID;
                Row_History["UserIP"] = strMyIP;
                Row_History["Regdate"] = Row["Regdate"];


                Adpt.Update(new DataRow[] { Row });
                Adpt_History.Update(new DataRow[] { Row_History });



#if __COUNTRY_KR__


                int nRowIndex = -1;
                //TextureStringManagerTableTableAdapter Adpt_TextureStringManagerTW = new TextureStringManagerTableTableAdapter();
                //DataTable tblErrorTW = Adpt_TextureStringManagerTW.GetData();
                
                DataTable tblTextureTW = Adpt_TextureStringManagerTW.GetData();
                
                for (int twRow = 0; twRow < tblTextureTW.Rows.Count; twRow++)
                {
                    if (Row["TextureIDX"].ToString() == tblTextureTW.Rows[twRow]["TextureIDX"].ToString())
                    {
                        nRowIndex = twRow;
                        break;
                    }
                }

                if (nRowIndex != -1)
                {
                    Adpt_Trans.UpdateTextureStringManagerTable(Row["String"].ToString(), Convert.ToInt32(Row["TextureIDX"].ToString()), "KR");
                }
                else
                {
                   // tblTextureStringManager.Clear();
                    //Adpt.Fill(tblTextureStringManager);
                    //추가된 것이므로 무조건 제일 밑

                    

                    Adpt_TextureStringManagerTW.InsertTextureStringManagerTable(Row["FilePath"].ToString(), Row["String"].ToString(), false, true, Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_TextureStringManagerHK.InsertTextureStringManagerTable(Row["FilePath"].ToString(), Row["String"].ToString(), false, true, Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_TextureStringManagerJP.InsertTextureStringManagerTable(Row["FilePath"].ToString(), Row["String"].ToString(), false, true, Convert.ToDateTime(Row["Regdate"].ToString()));

                    int nMaxIDX = Convert.ToInt32(Adpt_TextureStringManagerTW.GetMaxIDX().ToString());

                    Adpt_Trans.InsertTrans("KR", "ScriptData", "TextureStringManagerTable", "String", nMaxIDX, Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("TW", "ScriptData_TW", "TextureStringManagerTable", "String", nMaxIDX, Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("HK", "ScriptData_HK", "TextureStringManagerTable", "String", nMaxIDX, Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));
                    Adpt_Trans.InsertTrans("JP", "ScriptData_JP", "TextureStringManagerTable", "String", nMaxIDX, Row["String"].ToString(), Convert.ToDateTime(Row["Regdate"].ToString()), Convert.ToDateTime(Row["Regdate"].ToString()));                    
                }   



            }

            for (int i = 0; i < m_DeleteRowList.Count; i++)
            {
                tblTextureStringManager_DeleteHistory.Rows.Add();

                DataRow Row_History = tblTextureStringManager_DeleteHistory.Rows[tblTextureStringManager_DeleteHistory.Rows.Count - 1];

                DataRow Row = ((sDeleteRow)m_DeleteRowList[i]).drOriginalRow;

                //09. 06. 22 히스토리 입력부분 간소화
                for (int Column = 0; Column < tblTextureStringManager.Columns.Count - 1; Column++)
                {
                    Row_History[Column] = Row[Column];
                }
                Row_History["UserID"] = m_strID;
                Row_History["UserIP"] = strMyIP;
                //09. 06. 22 김정협 DeleteBHistory에서 시간을 현재 지우는 시간으로
                Row_History["Regdate"] = DateTime.Now;

                Adpt.Update(new DataRow[] { ((sDeleteRow)m_DeleteRowList[i]).drRow });
                Adpt_DeleteHistory.Update(new DataRow[] { Row_History });

                //TextureStringManagerTableTableAdapter Adpt_TextureStringManagerTW = new TextureStringManagerTableTableAdapter();
                TransTableAdapter Adpt_Trans = new TransTableAdapter();

                Adpt_TextureStringManagerTW.DeleteTextureStringManagerTable(Convert.ToInt32(Row["TextureIDX"].ToString()));
                Adpt_TextureStringManagerHK.DeleteTextureStringManagerTable(Convert.ToInt32(Row["TextureIDX"].ToString()));
                Adpt_TextureStringManagerJP.DeleteTextureStringManagerTable(Convert.ToInt32(Row["TextureIDX"].ToString()));
                Adpt_Trans.DeleteTrans("TextureStringManagerTable", Convert.ToInt32(Row["TextureIDX"].ToString()));
            }


            tblTextureStringManager.Clear();
            Adpt.Fill(tblTextureStringManager);

            tblTextureStringManager_History.Clear();
            Adpt_History.Fill(tblTextureStringManager_History);

            tblTextureStringManager_DeleteHistory.Clear();
            Adpt_DeleteHistory.Fill(tblTextureStringManager_DeleteHistory);

#elif __COUNTRY_TW__
                Adpt_Trans.UpdateTextureStringManagerTable(Row["String"].ToString(), Convert.ToInt32(Row["TextureIDX"].ToString()), "TW");
            }
#elif __COUNTRY_HK__
                Adpt_Trans.UpdateTextureStringManagerTable(Row["String"].ToString(), Convert.ToInt32(Row["TextureIDX"].ToString()), "HK");
            }
#elif __COUNTRY_JP__
                Adpt_Trans.UpdateTextureStringManagerTable(Row["String"].ToString(), Convert.ToInt32(Row["TextureIDX"].ToString()), "JP");
            }

#endif


            dataGridView1.CurrentCell = dataGridView1[0, dataGridView1.RowCount - 1];
            //tblTextureStringManager_History.Rows[tblTextureStringManager_History.Rows.Count - 1]
            //DataRow Row = tblTextureStringManager.Rows[Convert.ToInt32(m_ModifyIDList[0].ToString())];
            m_ModifyIDList.Clear();
            m_bSaveEnd = true;
        }

        private void StringTable_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();
#if __COUNTRY_KR__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=sa; pwd=x2akstp";
            strCountryCode = "KR";
            Add.Enabled = true;
#elif __COUNTRY_TW__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_TW; uid=sa; pwd=x2akstp";
            strCountryCode = "TW";
            Add.Enabled = false;
#elif __COUNTRY_HK__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_HK; uid=sa; pwd=x2akstp";
            strCountryCode = "HK";
            Add.Enabled = false;
#elif __COUNTRY_JP__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_JP; uid=sa; pwd=x2akstp";
            strCountryCode = "JP";
            Add.Enabled = false;
#endif
            Con.Open();


            


            Adpt = new SqlDataAdapter("SELECT * FROM TextureStringManager", Con);
            tblTextureStringManager = new DataTable("tblTextureStringManager");

            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt);
            Adpt.Fill(tblTextureStringManager);
            dataGridView1.DataSource = tblTextureStringManager;
            dataGridView1.AllowUserToAddRows = false; //제일 밑부분 직접 추가할 수 있는 Row 없앰
            dataGridView1.Columns["Regdate"].ReadOnly = true; //Reg_data수정방지
            dataGridView1.Columns["TextureIDX"].ReadOnly = true;            
            dataGridView1.CurrentCell = dataGridView1[0, dataGridView1.RowCount - 1];

            //History
            Adpt_History = new SqlDataAdapter("SELECT * FROM TextureStringManager_History", Con);
            tblTextureStringManager_History = new DataTable("tblTextureStringManager_History");

            SqlCommandBuilder Builder_History = new SqlCommandBuilder(Adpt_History);
            Adpt_History.Fill(tblTextureStringManager_History);


            //DeleteHistory용        
            Adpt_DeleteHistory = new SqlDataAdapter("SELECT * FROM TextureStringManager_DeleteHistory", Con);
            tblTextureStringManager_DeleteHistory = new DataTable("tblTextureStringManager_DeleteHistory");
            SqlCommandBuilder Builder_DeleteHistory = new SqlCommandBuilder(Adpt_DeleteHistory);
            Adpt_DeleteHistory.Fill(tblTextureStringManager_DeleteHistory);



            this.Width = 1280;
            this.Height = 900;
            StringTable_Resize(null, null);           

            pictureBox1.BackColor = Color.White;

            for (int i = 0; i < dataGridView1.ColumnCount; i++)
            {
                dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
            }
            
        }
        
        private void StringTable_FormClosed(object sender, FormClosedEventArgs e)
        {
            Con.Close();            
            Application.Exit();
            
        }

        private void Save_Click(object sender, EventArgs e)
        {
            StringSave();            
            MessageBox.Show("저장되었습니다.");
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

            if (!objTemp.Equals((object)dgvCurrentCell.Value))
            {
                string strOldTime = dataGridView1["Regdate", dgvCurrentCell.RowIndex].Value.ToString();

                dgvCurrentCell.Style.ForeColor = Color.Red;
                dataGridView1["Regdate", dgvCurrentCell.RowIndex].Value = DateTime.Now;
                dataGridView1["Regdate", dgvCurrentCell.RowIndex].Style.ForeColor = Color.Red;

                for (int i = 0; i < dataGridView1.RowCount; i++)
                {
                    if (dataGridView1["String", i].Value.ToString() == dgvCurrentCell.Value.ToString())
                    {
                        if (i == dgvCurrentCell.RowIndex)
                        {
                            continue;
                        }
                        
                        DialogResult result = MessageBox.Show("수정한 스트링과 동일한 스트링이 발견 되었습니다. 그래도 이 값으로 수정하시겠습니까?", "", MessageBoxButtons.OKCancel);
                        if (result == DialogResult.OK)
                        {
                            break;
                        }
                        else
                        {
                            dgvCurrentCell.Value = objTemp;
                            dgvCurrentCell.Style.ForeColor = Color.Black;
                            dataGridView1["Regdate", dgvCurrentCell.RowIndex].Value = strOldTime;
                            dataGridView1["Regdate", dgvCurrentCell.RowIndex].Style.ForeColor = Color.Black;
                            return;
                        }
                    }
                }
                int nID = Convert.ToInt32(dataGridView1["TextureIDX", dgvCurrentCell.RowIndex].Value.ToString());
                m_ModifyIDList.Add(nID);
                m_bSaveEnd = false;
            }
        }

        private void Add_Click(object sender, EventArgs e)
        {
            //삭제 기능 막음 
            //dataGridView1.Rows.RemoveAt(dataGridView1.CurrentCell.RowIndex);            

            Add frm = new Add();
            frm.SetDataGridView(dataGridView1);
            frm.SetDataTable(tblTextureStringManager);
            frm.SetModifyArray(m_ModifyIDList);            
            frm.Owner = this;
            frm.ShowDialog(this);
        }

        private void dataGridView1_KeyDown(object sender, KeyEventArgs e)
        {
            if (Control.ModifierKeys == Keys.Control && e.KeyCode == Keys.F)
            {
                Search frm = new Search();
                frm.SetDataGridView(dataGridView1);
                frm.ShowDialog(this);
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


        }


        private void btnHistory_Click(object sender, EventArgs e)
        {
            History frm = new History();
            frm.SetHistoryTable(tblTextureStringManager_History);
            frm.ShowDialog(this);
        }

        private void StringTable_FormClosing(object sender, FormClosingEventArgs e)
        {
            if(!m_bSaveEnd)
            {
                DialogResult result = MessageBox.Show("내용을 수정 후 저장하지 않으셨습니다. 저장하고 끝내시겠습니까?", "", MessageBoxButtons.YesNoCancel);
                if (result == DialogResult.Yes)
                {
                    /*//History 추가
                    IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
                    string strMyIP = host.AddressList[0].ToString();
                    for (int i = 0; i < m_ModifyIDList.Count; i++)
                    {
                        tblNetError_History.Rows.Add();
                        DataRow Row_History = tblNetError_History.Rows[tblNetError_History.Rows.Count - 1];
                        DataRow Row = tblNetError.Rows[Convert.ToInt32(m_ModifyIDList[i].ToString())];
                        Row_History["ID"] = Row["ID"];
                        Row_History["Enum"] = Row["Enum"];
                        Row_History["Type"] = Row["Type"];
                        Row_History["String"] = Row["String"];
                        Row_History["UserID"] = m_strID;
                        Row_History["UserIP"] = strMyIP;
                        Row_History["Regdate"] = Row["Regdate"];
                    }
                    m_ModifyIDList.Clear();
                    Adpt.Update(tblNetError);
                    Adpt_History.Update(tblNetError_History);*/
                    StringSave();
                    MessageBox.Show("저장되었습니다.");
                }
                else if (result == DialogResult.Cancel)
                {
                    e.Cancel = true;
                    return;
                }
                m_bSaveEnd = true;
                m_ModifyIDList.Clear();
            }            
        }
               
      

        private void StringTable_Resize(object sender, EventArgs e)
        {
            dataGridView1.Width = this.Width - 430;
            dataGridView1.Height = this.Height - 140;

            Add.Left = dataGridView1.Left;
            Add.Top = dataGridView1.Top + dataGridView1.Height + 5;

            Save.Left = Add.Left + Add.Width + 20;
            Save.Top = Add.Top;

            btnHistory.Left = Save.Left + Save.Width + 20;
            btnHistory.Top = Save.Top;
                        

            dataGridView1.Columns["TextureIDX"].Width = 100;
            dataGridView1.Columns["FilePath"].Width = 150;
            dataGridView1.Columns["IsChange"].Width = 100;
            dataGridView1.Columns["bUse"].Width = 100;
            dataGridView1.Columns["Regdate"].Width = 150;

            dataGridView1.Columns["String"].Width = dataGridView1.Width -600;

            pictureBox1.Left = dataGridView1.Left +  dataGridView1.Width + 5;
            pictureBox1.Width = 400;
            pictureBox1.Height = 400;
            pictureBox1.Top = dataGridView1.Top;
        }

        private void dataGridView1_UserDeletingRow(object sender, DataGridViewRowCancelEventArgs e)
        {
            DialogResult result = MessageBox.Show("정말 삭제하시겠습니까? 삭제 후 문제가 발생할 수도 있습니다.", "", MessageBoxButtons.OKCancel);
            if (result == DialogResult.Cancel)
            {
                e.Cancel = true;
                return;
            }
            else if (result == DialogResult.OK)
            {
                UserDeletingRow(e.Row.Index, dataGridView1, tblTextureStringManager);
                e.Cancel = true;
            }
        }

        

        void UserDeletingRow(int nDeletingRowIndex, DataGridView dgvView, DataTable tblDelete)
        {
            DataRow dr = null;// = tblDelete.Rows[e.Row.Index];
            int nRowIndex = 0;

            for (int i = 0; i < tblDelete.Rows.Count; i++)
            {
                if (tblDelete.Rows[i].RowState == DataRowState.Deleted)
                {
                    continue;
                }

                if (dgvView["TextureIDX", nDeletingRowIndex].Value.ToString() == tblDelete.Rows[i]["TextureIDX"].ToString())
                {
                    dr = tblDelete.Rows[i];
                    nRowIndex = i;
                    break;
                }
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
            
            if (!m_DeleteRowList.Contains(sRow))
            {
                m_DeleteRowList.Add(sRow);                
            }


            //Adpt_NpcExtraAbility.Update(new DataRow[] { dr });
            m_bSaveEnd = false;
        }

        private void dataGridView1_RowEnter(object sender, DataGridViewCellEventArgs e)
        {
            string strFilePath = dataGridView1["FilePath", e.RowIndex].Value.ToString();

            strFilePath = "..\\..\\..\\Resource\\" + strCountryCode + "\\Trunk\\" + strFilePath;
            string strFile;
            
//             if(File.Exists(strFilePath + ".tga"))
//             {
//                 strFile = strFilePath + ".tga";
//             
// 
//             }
//             else if(File.Exists(strFilePath + ".dds"))
//             {
//                 strFile = strFilePath + ".dds";
//             
//             }
//             else
            if (File.Exists(strFilePath))
            {
                strFile = strFilePath;
            }
            else
            {
                System.Drawing.Bitmap None = DevIL.DevIL.LoadBitmap("None.jpg");
                pictureBox1.Image = None;

                pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
                return;
            }



            System.Drawing.Bitmap bmp = DevIL.DevIL.LoadBitmap(strFile);
            pictureBox1.Image = bmp;

            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;

        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            if (textBox1.Text != "")
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
        }

        private void dataGridView1_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            if (dataGridView1.RowCount > 0)
                textBox1.Text = dataGridView1[e.ColumnIndex, 0].OwningColumn.Name;
        }

        private void btnReplace_Click(object sender, EventArgs e)
        {
            panel1.Visible = true;
                       
            textBox3.Text = dataGridView1.CurrentCell.Value.ToString();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {           

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

        private void textBox2_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                btnSearch_Click(null, null);
            }
            
        }

        private void checkSort_CheckedChanged(object sender, EventArgs e)
        {
            if(checkSort.Checked)
            {
                for (int i = 0; i < dataGridView1.ColumnCount; i++)
                {
                    dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Automatic;
                }
            }
            else
            {
                for (int i = 0; i < dataGridView1.ColumnCount; i++)
                {
                    dataGridView1.Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                }
            }
        }       


       
    }
}