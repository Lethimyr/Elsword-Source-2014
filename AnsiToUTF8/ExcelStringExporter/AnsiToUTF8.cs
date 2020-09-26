using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Data.OleDb;
using Excel = Microsoft.Office.Interop.Excel;
using System.Data.SqlClient;
using System.IO;


//using Excel = Microsoft.off

namespace WizardForm
{
    public partial class AnsiToUTF8 : Form
    {
        public AnsiToUTF8()
        {
            InitializeComponent();
        }

        private void btnConverter_Click(object sender, EventArgs e)
        {
            if(FilePathName.Text == string.Empty)
            {
                MessageBox.Show("파일이나 폴더가 지정되지 않았습니다.");
                return;
            }

            

            if (radioButton1.Checked)
            {
                OneFileConvert(FilePathName.Text, false);
                
                MessageBox.Show("변환완료했습니다.");
            }
            else if(radioButton2.Checked)
            {
                if(comboBox1.SelectedIndex == -1)
                {
                    MessageBox.Show("확장자를 선택하여야 합니다.");
                    return;
                }

                string strExtention = string.Empty;
                
                
                

                if (comboBox1.SelectedItem.ToString() != "All")
                {
                    strExtention = comboBox1.SelectedItem.ToString().Remove(0, 1);
                }

                FileCopyToPath(FilePathName.Text, strExtention, checkBox1.Checked, false);
                /*strFiles = Directory.GetFiles(FilePathName.Text);
                for(int i = 0; i < strFiles.Length; i++)
                {
                    if (strExtention != string.Empty)
                    {
                        if (strFiles[i].IndexOf(strExtention) == -1)
                        {
                            continue;
                        }
                    }                    
                    
                    FileStream fs = new FileStream(strFiles[i], FileMode.Open);
                    byte[] bom = new byte[4];
                    fs.Read(bom, 0, 4);
                    if (bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf) // UTF-8
                    {
                        
                        fs.Close();
                        continue;
                    }
                    fs.Close();

                    StreamReader sr = new StreamReader(strFiles[i], Encoding.Default);
                    string strFile = sr.ReadToEnd();
                    sr.Close();
                    StreamWriter sw = new StreamWriter(strFiles[i], false, Encoding.UTF8);
                    sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
                    sw.Write(strFile);
                    sw.Close();

                    
                    
                }*/

                MessageBox.Show("변환완료했습니다.");
                
            }
            
            
        }

        public void OneFileConvert(string strFilePath, bool m_bCommand)
        {
            FileStream fs = new FileStream(strFilePath, FileMode.Open);
            byte[] bom = new byte[4];
            fs.Read(bom, 0, 4);
            if (bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf) // UTF-8
            {
                //MessageBox.Show("해당파일은 이미 UTF-8형식입니다.");
                MessageBox.Show("해당파일은 이미 UTF-8형식입니다.");
                fs.Close();
                return;
            }
            fs.Close();

            StreamReader sr = new StreamReader(strFilePath, Encoding.Default);
            string strFile = sr.ReadToEnd();
            sr.Close();
            StreamWriter sw = new StreamWriter(strFilePath, false, Encoding.UTF8);
            if (!m_bCommand)
            {
                sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
            }
            sw.Write(strFile);
            sw.Close();
        }

        public void FileCopyToPath(string strPath, string strExtention, bool bSubSearch, bool m_bCommand)
        {
            

            string sSFile;
            if (Directory.GetFiles(strPath).Length > 0)
            {
                for (int j = 0; j < Directory.GetFiles(strPath).Length; j++)
                {
                    sSFile = Directory.GetFiles(strPath)[j];
                    if (strExtention != string.Empty)
                    {

                        string strExtentionTemp = sSFile.Substring(sSFile.Length - strExtention.Length, strExtention.Length);
                        string strTemp = strExtentionTemp.ToUpper();
                        string strTemp2 = strExtention.ToUpper();
                        if (strTemp != strTemp2)
                        {
                            continue;
                        }
                    }

                    FileStream fs = new FileStream(sSFile, FileMode.Open);
                    byte[] bom = new byte[4];
                    fs.Read(bom, 0, 4);
                    if (bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf) // UTF-8
                    {

                        fs.Close();
                        continue;
                    }
                    fs.Close();

                    StreamReader sr = new StreamReader(sSFile, Encoding.Default);
                    string strFile = sr.ReadToEnd();
                    sr.Close();
                    StreamWriter sw = new StreamWriter(sSFile, false, Encoding.UTF8);
                    if (!m_bCommand)
                    {
                        sw.Write("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.\r\n\r\n");
                    }                    
                    sw.Write(strFile);
                    sw.Close();
                }
            }



            if (bSubSearch)
            {
                string[] saDir1 = Directory.GetDirectories(strPath);
                for (int i = 0; i < saDir1.Length; i++)
                {
                    //if (Directory.GetDirectories(saDir1[i]).Length > 0)
                    {
                        FileCopyToPath(saDir1[i], strExtention, bSubSearch, m_bCommand);
                    }
                    
                }
            }


        }

        private void AnsiToUTF8_Load(object sender, EventArgs e)
        {
            radioButton1.Checked = true;
            comboBox1.Enabled = false;
            checkBox1.Enabled = false;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            comboBox1.Enabled = true;
            FilePathName.Text = string.Empty;
            checkBox1.Enabled = true;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            comboBox1.Enabled = false;
            FilePathName.Text = string.Empty;
            checkBox1.Enabled = false;
        }

        private void Search_Click(object sender, EventArgs e)
        {
            if(radioButton1.Checked)
            {
                FileDialog dlg = new OpenFileDialog();
                dlg.InitialDirectory = @"c:\";

                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    FilePathName.Text = dlg.FileName;
                }   
            }
            else if (radioButton2.Checked)
            {                
                FolderBrowserDialog dlg = new FolderBrowserDialog();

                //이런식으로 지정 합니다.
                dlg.SelectedPath = @"c:\";

                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    FilePathName.Text = dlg.SelectedPath;
                }
            }
        }

    }       

}