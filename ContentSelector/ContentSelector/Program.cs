using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace ContentSelector
{
    

    static class Program
    {
        /// <summary>
        /// 해당 응용 프로그램의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string wstrGeneralStringFileName = ScriptTreeManager.strKRStringDir + "General.ess";
            string wstrScriptStringFileName = ScriptTreeManager.strKRStringDir + "Script.ess";
            StringManager.InitializeStringTable(wstrGeneralStringFileName, wstrScriptStringFileName);

            DungeonDataDivider FD = new DungeonDataDivider();
            FD.DivideDungeonData(ScriptTreeManager.strDungeonDir + "DungeonData.lua");
                                                
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            m_Form = new Form1();
            Application.Run(m_Form);
                        
            FD.CleanUpDungeonData();
            
        }

        static public Form1 m_Form;
    }
    
}