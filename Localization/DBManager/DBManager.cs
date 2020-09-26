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

namespace DBManager
{
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

            IPHostEntry host = Dns.GetHostEntry(Dns.GetHostName());
            string strMyIP = host.AddressList[0].ToString();

            

            if(Result == null)
            {
                MessageBox.Show("존재하지 않는 아이디입니다.");
            }
            else
            {
               /* if (strMyIP != Result["UserIP"].ToString())
                {
                    MessageBox.Show("해당 아이디가 접속할 수 있는 아이피 주소가 아닙니다.");
                    return;
                }*/

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
                    StringTable frm = new StringTable();
                    frm.SetID(textID.Text);
                    frm.ShowDialog(this);                    
                }
            }
        }

        private void DBManager_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData_Tw; uid=sa; pwd=x2akstp";
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