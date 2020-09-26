using System;
using System.Collections.Generic;
using System.Text;
using NAnt.Core.Attributes;
using NAnt.Core;
using System.IO;
using NAnt.Core.Types;
using System.Xml;
using System.Globalization;
using System.Collections.Specialized;

namespace CrashAlayzeTask
{
    class CCallStack 
    {
        public string FinalCallstack;
        public string CallStack;
    }

    class Frame
    {
        public string module = null;
        public UInt32 address = 0;
    }


    [TaskName("CrashAnalyze")]
    public class CrashAnalyze : NAnt.Core.Task
    {
        [TaskAttribute("crashfile", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string CrashFile { get { return crashfile; } set { crashfile = value; } }

        [TaskAttribute("debuginfourl", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string DebugInfoURL { get { return debuginfourl; } set { debuginfourl = value; } }

        [TaskAttribute("id", Required = false)]
        [StringValidator(AllowEmpty = true)]
        public string ID { get { return id; } set { id = value; } }

        [TaskAttribute("pw", Required = false)]
        [StringValidator(AllowEmpty = true)]
        public string PW { get { return pw; } set { pw = value; } }

               
        [TaskAttribute("connectstring", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string ConnectString { get { return connectstring; } set { connectstring = value; } }

        [TaskAttribute("usemap", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string UseMap { get { return usemap; } set { usemap = value; } }


        [TaskAttribute("archivedir", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string ArchiveDir { get { return archivedir; } set { archivedir = value; } }

        [TaskAttribute("projectname", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string ProjectName { get { return projectname; } set { projectname = value; } }
        
        string crashfile;
        string debuginfourl;
        string id = null;
        string pw = null;
        string connectstring;        
        string usemap = "true";
        string archivedir;
        string projectname;

        string GetOSName(string bugtrapfile)
        {
            System.IO.StreamReader reader = new System.IO.StreamReader(bugtrapfile);
            string xmlstring = reader.ReadToEnd();
            reader.Close();

            System.Xml.XmlDocument document = new System.Xml.XmlDocument();
            document.LoadXml(xmlstring);

            System.Xml.XPath.XPathNavigator nav = document.CreateNavigator();
            
            string os = string.Format("{0} {1} {2}", System.Convert.ToString(nav.SelectSingleNode("/report/os/version/text()")),
               System.Convert.ToString(nav.SelectSingleNode("/report/os/spack/text()")),
               System.Convert.ToString(nav.SelectSingleNode("/report/os/build/text()")));

            return os;
        }


        CCallStack AnalyzeMap(string bugtrapfile,string mapfile)
        {
            CCallStack callstack = new CCallStack();

            System.Collections.SortedList address_list = new System.Collections.SortedList();
            System.Collections.SortedList find_address_list = new System.Collections.SortedList();

            System.IO.StreamReader reader = new System.IO.StreamReader(bugtrapfile);
            string xmlstring = reader.ReadToEnd();
            reader.Close();

            System.Xml.XmlDocument document = new System.Xml.XmlDocument();
            document.LoadXml(xmlstring);

            System.Xml.XPath.XPathNavigator nav = document.CreateNavigator();
            System.Xml.XPath.XPathNodeIterator i;

            string what = System.Convert.ToString(nav.SelectSingleNode("/report/error/what/text()"));
            string module = System.IO.Path.GetFileName(System.Convert.ToString(nav.SelectSingleNode("/report/error/module/text()")));
            string tmp = System.Convert.ToString(nav.SelectSingleNode("/report/error/address/text()"));          

            if (tmp == "" || tmp.Trim().Length < 1)
                return null;

            UInt32 address = System.Convert.ToUInt32(tmp.Substring(tmp.Length - 8), 16);

            i = nav.Select("/report/threads/thread/status[text()='interrupted']/../stack/frame");

            System.Collections.ArrayList frame_list = new System.Collections.ArrayList();
            while (i.MoveNext() == true)
            {
                Frame frame = new Frame();

                frame.module = System.IO.Path.GetFileName(System.Convert.ToString(i.Current.SelectSingleNode("module/text()")));
                string frame_address = System.Convert.ToString(i.Current.SelectSingleNode("address/text()"));

                string function_name = null;
                string function_offset = null;

                if (i.Current.SelectSingleNode("function/name/text()") != null)
                    function_name = System.Convert.ToString(i.Current.SelectSingleNode("function/name/text()"));

                if (i.Current.SelectSingleNode("function/offset/text()") != null)
                    function_offset = System.Convert.ToString(i.Current.SelectSingleNode("function/offset/text()"));

                frame.address = System.Convert.ToUInt32(frame_address.Substring(frame_address.Length - 8), 16);

                if (address_list.Contains(frame.address) == false &&
                    find_address_list.Contains(frame.address) == false)
                {
                    if (function_name == null)
                    {
                        address_list.Add(frame.address, null);
                    }
                    else
                    {
                        if (function_offset == null)
                            find_address_list.Add(frame.address, function_name);
                        else
                            find_address_list.Add(frame.address, string.Format("{0}+0x{1:x}", function_name, function_offset));

                    }
                }

                frame_list.Add(frame);
            }

            if (address_list.Contains(address) == false &&
                find_address_list.Contains(address) == false)
                address_list.Add(address, null);

            UInt32 baseaddress = 0;
            reader = new System.IO.StreamReader(mapfile);

            while (reader.EndOfStream == false)
            {
                string line = reader.ReadLine();
                string[] e = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                if (e.Length >= 5 && e[0] == "Preferred")
                {
                    baseaddress = System.Convert.ToUInt32(e[4], 16);
                    break;
                }
            }

            while (reader.EndOfStream == false)
            {
                string line = reader.ReadLine();
                string[] e = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                if (e.Length >= 5 && e[0] == "Address")
                    break;
            }

            string old_name = "";
            UInt32 old_address = 0;
            while (reader.EndOfStream == false && address_list.Count > 0)
            {
                string line = reader.ReadLine();
                string[] e = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                
                UInt32 map_address = System.Convert.ToUInt32(e[2], 16);
                UInt32 list_address = (UInt32)address_list.GetKey(0);
                if (list_address <= map_address)
                {
                    address_list.RemoveAt(0);
                    e[1] = ConvertUname(old_name);                        
                    find_address_list.Add(list_address, string.Format("{0}+0x{1:x}", e[1], list_address - old_address));
                }

                old_name = e[1];
                old_address = map_address;
                
            }
            reader.Close();

            if (find_address_list.Contains(address) == true)
            {
                string fcs = find_address_list[address].ToString();
                int pi = fcs.IndexOf("+");
                if (pi > 0)
                    callstack.FinalCallstack = String.Format("{0}!{1}", module.ToLower(), fcs.Substring(0, pi));
                else
                    callstack.FinalCallstack = String.Format("{0}!{1}", module.ToLower(), fcs);
            }
            else
                callstack.FinalCallstack = "Unknown Address";

          
            foreach (Frame frame in frame_list)
            {
                if (find_address_list.Contains(frame.address) == true)
                    callstack.CallStack += string.Format("{0:x8} {1} {2}\n", frame.address, frame.module.ToLower(), find_address_list[frame.address]);
                else
                    callstack.CallStack += string.Format("{0:x8} {1}\n", frame.address, frame.module.ToLower());
            }         

            return callstack;
        }


        bool GetDebuginfo(string url, string workingdir, DateTime patchtime)
        {
            string svnlog = "";
            System.Diagnostics.ProcessStartInfo psi = new System.Diagnostics.ProcessStartInfo();
            System.Diagnostics.Process process;

            psi.FileName = "svn";
            psi.Arguments = string.Format("log {0} --xml -r {1}:{2} --username {3} --password {4}",
                url,
                patchtime.AddDays(-1).ToString("{yyyy-MM-dd}"),
                patchtime.AddDays(1).ToString("{yyyy-MM-dd}"),
                "hudson", "hudson");

            psi.CreateNoWindow = true;
            psi.UseShellExecute = false;
            psi.RedirectStandardOutput = true;
            psi.StandardOutputEncoding = System.Text.Encoding.UTF8;

            process = System.Diagnostics.Process.Start(psi);

            while (!process.StandardOutput.EndOfStream)
                svnlog = process.StandardOutput.ReadToEnd();

            process.WaitForExit();
            if (process.ExitCode != 0)
                throw new Exception();

            System.Xml.XmlDocument svnlogxml = new XmlDocument();
            svnlogxml.LoadXml(svnlog);
            System.Xml.XmlNode node = svnlogxml.SelectSingleNode(string.Format("/log/logentry/msg[contains(.,'{0}')]/../@revision", patchtime.ToString("yyyy-MM-dd_HH-mm-ss")));
            if (node == null)
                return false;

            string revision = node.Value;

            psi = new System.Diagnostics.ProcessStartInfo();
            psi.FileName = "svn";
            psi.WorkingDirectory = workingdir;
            psi.Arguments = string.Format("export -r {0} {1}  --username {3} --password {4}",
                revision,
                url,
                workingdir, "hudson", "hudson");

            psi.CreateNoWindow = true;
            psi.UseShellExecute = false;
            process = System.Diagnostics.Process.Start(psi);
            process.WaitForExit();


            ExtractCompress(System.IO.Path.Combine(workingdir, System.IO.Path.GetFileName(url)));            

            return true;

        }

        string ConvertUname(string name)
        {
            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.StartInfo.Arguments = name;            
            p.StartInfo.FileName = "undname.exe";
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.UseShellExecute = false;

            p.Start();
            while (p.StandardOutput.EndOfStream == false)
            {
                string l = p.StandardOutput.ReadLine();
                if (l.StartsWith("is") == true)
                    name = l.Substring(7, l.Length - 8);
            }
            p.WaitForExit();

            return name;
        }

        bool ExtractCompress(string filename)
        {
            System.Diagnostics.ProcessStartInfo psi = new System.Diagnostics.ProcessStartInfo();
            System.Diagnostics.Process process;

            psi = new System.Diagnostics.ProcessStartInfo();
            psi.FileName = "7za";
            psi.WorkingDirectory = System.IO.Path.GetDirectoryName(filename);
            psi.Arguments = string.Format("x -y {0}", System.IO.Path.GetFileName(filename));
            psi.CreateNoWindow = true;
            psi.UseShellExecute = false;
            process = System.Diagnostics.Process.Start(psi);
            process.WaitForExit();
            if (process.ExitCode != 0)
                throw new Exception();
            
            return true;
        }


        CCallStack AnalyzeDump(string dumppath, string imagepath,string workingdir)
        {
            System.Diagnostics.ProcessStartInfo info = new System.Diagnostics.ProcessStartInfo();
            info.WorkingDirectory = workingdir;
            info.FileName = "cdb.exe";
            info.Arguments = string.Format("-logo analyze.log -y \"{0}\" -i \"{0}\" -z {1} -lines -c \"!analyze -v;q\"", imagepath, dumppath);
            info.CreateNoWindow = true;
            info.UseShellExecute = false;
            System.Diagnostics.Process process = System.Diagnostics.Process.Start(info);
            process.WaitForExit(20000);

            if (process.HasExited == false)
            {
                process.Kill();
                return null;
            }
            
            return FormattingFile(workingdir + "\\analyze.log");
        }

        CCallStack FormattingFile(string filename)
        {
            if (System.IO.File.Exists(filename) == false)
                return null;

            CCallStack callstack = new CCallStack();
            callstack.FinalCallstack = "";
            callstack.CallStack = "";            

            System.IO.StreamReader reader = new System.IO.StreamReader(filename);

            while (reader.EndOfStream == false)
            {
                string line = reader.ReadLine();
                string[] e = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                if (e.Length > 2 && e[0] == "ExceptionAddress:")
                {
                    int a = line.IndexOf(' ');
                    if (a > 0)
                    {
                        int b = line.IndexOf(' ', a + 1);
                        if (b > a)
                        {
                            callstack.FinalCallstack = line.Substring(b + 1);
                        }
                    }
                }
                if (e.Length > 0 && e[0] == "STACK_TEXT:")
                {
                    break;
                }
            }
            while (reader.EndOfStream == false)
            {
                string line = reader.ReadLine();
                string[] e = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                if (e.Length < 2)
                    break;

                callstack.CallStack += line + "\n";
            }
            reader.Close();   
         
            if(callstack.FinalCallstack.Trim().Length==0)
                callstack.FinalCallstack = "Unknown Address";
            return callstack;
        }

        protected override void ExecuteTask()
        {           
            
            DateTime patchtime;
            DateTime crashtime;

            string[] mapfile = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.map");
            string[] pdbfile = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.pdb");

            // 받은 시간, 패치 시간 구하기
            try
            {
                string crashtimestring = System.IO.Path.GetFileName(CrashFile).Substring(0, 19);
                string patchtimestring = System.IO.Path.GetFileName(System.IO.Path.GetDirectoryName(CrashFile));
                crashtime = DateTime.ParseExact(crashtimestring, "yyyy-MM-dd_HH-mm-ss", System.Globalization.CultureInfo.CurrentCulture);
                patchtime = DateTime.ParseExact(patchtimestring, "yyyy-MM-dd_HH-mm-ss", System.Globalization.CultureInfo.CurrentCulture);
            }
            catch (System.Exception ex)
            {
                throw new BuildException(string.Format("{0} 패치 시간을 확인 할 수 없음", CrashFile),ex);
            }

            // pdb 랑 맵 파일을 얻자
            if(mapfile.Length==0 && pdbfile.Length==0)
            {
                try
                {
                    GetDebuginfo(DebugInfoURL, System.IO.Path.GetDirectoryName(CrashFile), patchtime);
                }
                catch (System.Exception ex)
                {
                    throw new BuildException(string.Format("{0} {1} 디버깅 정보를 확인 할 수 없음", DebugInfoURL, CrashFile), ex);
                }     

            }

            mapfile = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.map");
            pdbfile = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.pdb");

            // 크래시파일 압축 풀기
            try
            {
                ExtractCompress(CrashFile);
            }
            catch (System.Exception ex)
            {
                throw new BuildException(string.Format("{0} 압축을 풀 수 없음", CrashFile),ex);
            }


            string[] dumpfiles = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.dmp");
            string[] errorlogfiles = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.xml");            
            string[] screenshotfiles = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.jpg");
            string[] logfiles = System.IO.Directory.GetFiles(System.IO.Path.GetDirectoryName(CrashFile), "*.txt");
            
            // 파일 분석 
            string os = "Unknown";
            CCallStack callstack = null;
            if(UseMap!="true" && pdbfile.Length>0 && dumpfiles.Length>0)
            {
                try
                {
                    callstack = AnalyzeDump(dumpfiles[0], System.IO.Path.GetDirectoryName(CrashFile), System.IO.Path.GetDirectoryName(CrashFile));                    
                }
                catch (System.Exception ex)
                {
                    throw new BuildException(string.Format("{0} 덤프파일 분석 실패", CrashFile), ex);
                }                
            }
            else if(errorlogfiles.Length>0)
            {
                try
                {
                    callstack = AnalyzeMap(errorlogfiles[0], mapfile[0]);
                }
                catch (System.Exception ex)
                {
                    throw new BuildException(string.Format("{0} 맵파일 분석 실패", CrashFile), ex);
                }                
            }

            try
            {
                os = GetOSName(errorlogfiles[0]);
            }
            catch (System.Exception ex)
            {
            	
            }
            

            // 디비 등록

            int UID = 0;

            try
            {
                System.Data.SqlClient.SqlConnection con = new System.Data.SqlClient.SqlConnection(ConnectString);
                con.Open();

                System.Data.SqlClient.SqlCommand command = con.CreateCommand();
                command.CommandType = System.Data.CommandType.StoredProcedure;
                command.CommandText = "crash_insert";
                command.Parameters.AddWithValue("strCountryName_", ProjectName);
                command.Parameters.AddWithValue("dtPatchDate_", patchtime);
                command.Parameters.AddWithValue("strFinalCallStack_", callstack.FinalCallstack);
                command.Parameters.AddWithValue("strOSName_", os);
                command.Parameters.AddWithValue("dtCrashDate_", crashtime);
                command.Parameters.AddWithValue("strCallStack_", callstack.CallStack);
                UID = System.Convert.ToInt32(command.ExecuteScalar());

                con.Close();
            }
            catch (System.Exception ex)
            {
                throw new Exception(string.Format("{0} DB 접속 실패 {1}", ConnectString,CrashFile), ex);
            }


            // 파일 이동          
            try
            {
                foreach(string filename in dumpfiles)                
                    MoveFile(filename, ArchiveDir, UID);

                foreach (string filename in errorlogfiles)
                    MoveFile(filename, ArchiveDir, UID);

                foreach (string filename in screenshotfiles)
                    MoveFile(filename, ArchiveDir, UID);

                foreach (string filename in logfiles)
                    MoveFile(filename, ArchiveDir, UID);
     
            }
            catch
            {

            }


        }

        void MoveFile(string path,string destdir,int UID)
        {

            string destpath = System.IO.Path.Combine(destdir, string.Format("{0}{1}", UID, System.IO.Path.GetExtension(path)));
            if (System.IO.File.Exists(path))
            {
                System.IO.File.Move(path,destpath);
            }
        }

        public void Test() { ExecuteTask(); }
       
    }




}
