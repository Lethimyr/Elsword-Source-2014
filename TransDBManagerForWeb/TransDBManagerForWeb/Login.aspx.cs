using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;
using System.Net;

namespace TransDBManagerForWeb
{
    
    public partial class Login : System.Web.UI.Page
    {
        private SqlConnection Con;
        private SqlDataAdapter Adpt_Login;
        DataTable tblLogin;

        protected void Page_Load(object sender, EventArgs e)
        {
            Con = new SqlConnection();
            Con.ConnectionString = "server=116.120.238.52,3130; database=ScriptData; uid=Script_User; pwd=x2_tmzmflqxm";
            Con.Open();
            
            Adpt_Login = new SqlDataAdapter("SELECT * FROM TUser", Con);
            tblLogin = new DataTable("tblLogin");

            SqlCommandBuilder Builder = new SqlCommandBuilder(Adpt_Login);
            Adpt_Login.Fill(tblLogin);
            tblLogin.PrimaryKey = new DataColumn[] { tblLogin.Columns["ID"] };
            TextBox2.TextMode = TextBoxMode.Password;            
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            if (TextBox1.Text == "")
            {
                Label3.Text = "아이디를 입력하여 주십시요.";
                return;
            }
            if (TextBox2.Text == "")
            {
                Label3.Text = "비밀번호를 입력하여 주십시요.";                
                return;
            }

            DataRow Result = tblLogin.Rows.Find(TextBox1.Text);
            if (Result == null)
            {
                Label3.Text = "존재하지 않는 아이디입니다.";                
            }
            else
            {
                if (TextBox2.Text != Result["PW"].ToString())
                {
                    Label3.Text = "비밀번호가 틀렸습니다.";
                }
                else
                {
                    string strMyIP = Request.UserHostAddress;
                    if (strMyIP != Result["IPAdress"].ToString())
                    {
                       Label3.Text = "해당아이디에 할당된 IP가 아닙니다.";                        
                    }
                    else
                    {
                        FormsAuthentication.RedirectFromLoginPage(TextBox1.Text, false);
                        Response.Redirect("Trans.aspx?COUNTRY=" + Result["Country"].ToString() + "&ID=" + TextBox1.Text);
                        
                    }
                    
                }
                
            }
        }
    }
}
