using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;

namespace WizardForm
{
    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            if(args.Length > 0 )
            {
                AnsiToUTF8 Utf8 =  new AnsiToUTF8();
                if(args[0] == "-d")
                {
                    if (args[1].IndexOf("*.") != -1 && Directory.Exists(args[2]))
                    {
                        string strExtension = args[1].Remove(0,1);
                        Utf8.FileCopyToPath(args[2], strExtension, true, true);
                    }
                }
                else if(args[0] == "-f")
                {
                    if(File.Exists(args[1]))
                    {
                        Utf8.OneFileConvert(args[1], true);
                    }
                    else
                    {
                        MessageBox.Show(args[1] + " 파일이 존재하지 않습니다.");
                    }
                }

            }
            else
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new AnsiToUTF8());
            }            
        }
    }
}