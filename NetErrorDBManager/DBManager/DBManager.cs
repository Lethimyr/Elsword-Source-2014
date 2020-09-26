//09. 06. 24 김정협 나라별 define
// ItemTable.cs, QuestTable.cs, TitleTable.cs 세군대 정의를
// DBManager 추가
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

namespace DBManager
{
    //삭제된 로우를 위한것
    public struct sDeleteRow
    {        
        public DataRow drRow;
        public DataRow drOriginalRow;
    }

    public partial class DBManager : Form
    {
        private SqlConnection Con;
        private SqlDataAdapter Adpt;
        DataTable tblUser;
                
        public DBManager()
        {
            InitializeComponent();
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            if(textID.Text == "")
            {
                MessageBox.Show("아이디를 입력하여 주십시요.");
                return;
            }
            if (textPWD.Text == "")
            {
                MessageBox.Show("비밀번호를 입력하여 주십시요.");
                return;
            }

            DataRow Result = tblUser.Rows.Find(textID.Text);
            if(Result == null)
            {
                MessageBox.Show("존재하지 않는 아이디입니다.");
            }
            else
            {
                if(textPWD.Text != Result["PW"].ToString())
                {
                    MessageBox.Show("비밀번호가 틀렸습니다.");
                }
                else
                {
                    StreamWriter sw = new StreamWriter("Setting.ini", false, Encoding.Default);
                    if (checkSave.Checked)
                    {
                        sw.Write(1);
                        sw.Write("\r\n");
                        sw.Write(textID.Text);
                        sw.Write("\r\n");
                        sw.Write(textPWD.Text);
                        sw.Write("\r\n");
                    }
                    else
                    {
                        sw.Write(0);
                    }
                    sw.Close();

                    this.Hide();
                    NetErrorTable frm = new NetErrorTable();
                    frm.SetID(textID.Text);
                    frm.ShowDialog(this);                    
                }
            }
        }

        private void DBManager_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();
#if __COUNTRY_KR__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";
#elif __COUNTRY_TW__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_TW; uid=Script_User; pwd=x2_tmzmflqxm";
#elif __COUNTRY_HK__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_HK; uid=Script_User; pwd=x2_tmzmflqxm";
#elif __COUNTRY_JP__
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_JP; uid=Script_User; pwd=x2_tmzmflqxm";
#endif
            Con.Open();

            Adpt = new SqlDataAdapter("SELECT * FROM TUser", Con);
            tblUser = new DataTable("tblUser");

            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt);
            Adpt.Fill(tblUser);            

            tblUser.PrimaryKey = new DataColumn[] { tblUser.Columns["ID"] };
            
            Con.Close();

            FileInfo finfo = new FileInfo("Setting.ini"); 
            if (finfo.Exists)
            {
                StreamReader sr = new StreamReader("Setting.ini", Encoding.Default);
                string strCheck = sr.ReadLine();
                int nCheck = Convert.ToInt32(strCheck);
                if (nCheck == 1)
                {
                    textID.Text = sr.ReadLine();
                    textPWD.Text = sr.ReadLine();
                    checkSave.Checked = true;
                }
                sr.Close();
            }
            else
            {
                StreamWriter sw = new StreamWriter("Setting.ini", false, Encoding.Default);
                sw.Write(0);
                sw.Close();
            }
        }

        private void textPWD_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((char)Keys.Enter == e.KeyChar)
                this.btnLogin_Click(null, null);
        }       

    }

}