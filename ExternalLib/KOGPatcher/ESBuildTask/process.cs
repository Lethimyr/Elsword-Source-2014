using System;
using System.Collections.Generic;
using System.Text;
using NAnt.Core.Attributes;
using NAnt.Core;
using System.Diagnostics;

namespace ESBuildTask
{

    public class NoshellProcess
    {
        public static int Execute(string program, string argument)
        {
            System.Diagnostics.ProcessStartInfo psi = new System.Diagnostics.ProcessStartInfo();
            psi.UseShellExecute = false;
            psi.CreateNoWindow = true;
            psi.FileName = program;
            psi.Arguments = argument;
            System.Diagnostics.Process process = System.Diagnostics.Process.Start(psi);
            process.WaitForExit();
            return process.ExitCode;
        }

        public static System.Xml.XmlDocument ExcuteXML(string program, string argument)
        {
            System.Diagnostics.ProcessStartInfo psi = new System.Diagnostics.ProcessStartInfo();
            psi.FileName = program;
            psi.Arguments = argument;
            psi.CreateNoWindow = true;
            psi.UseShellExecute = false;
            psi.RedirectStandardOutput = true;
            psi.StandardOutputEncoding = System.Text.Encoding.UTF8;
            string data = "";
            System.Diagnostics.Process process = System.Diagnostics.Process.Start(psi);
            while (!process.StandardOutput.EndOfStream)
            {
                data = process.StandardOutput.ReadToEnd();
            }
            process.WaitForExit();
            if (process.ExitCode != 0)
                throw new Exception();
            System.Xml.XmlDocument doc = new System.Xml.XmlDocument();
            doc.LoadXml(data);
            return doc;
        }
    }

    [TaskName("kill")]
    public class KillTask : Task
    {
        private string processName;
        [TaskAttribute("processName", Required = true), StringValidator(AllowEmpty = false)]
        public string ProcessName
        {
            get { return processName; }
            set { processName = value; }
        }

        protected override void ExecuteTask()
        {
            try
            {
                Log(Level.Info, "Looking for processes named {0}...", processName);
                foreach (Process process in Process.GetProcessesByName(processName))
                {
                    Log(Level.Info, "Found process named {0}, killing...", processName);
                    process.Kill();
                }
            }
            catch (System.Exception ex)
            {
                Log(Level.Info, "Cant Kill {0}...", processName);
            }

        }
    }

}