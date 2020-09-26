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

namespace ESBuildTask
{
    [TaskName("ExportPatch")]
    public class ExportPatchTask : NAnt.Core.Task
    {
        [TaskAttribute("srcurl", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string SrcURL { get { return srcurl; } set { srcurl = value; } }

        [TaskAttribute("destdir", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string DestDir { get { return destdir; } set { destdir = value; } }

        [TaskAttribute("oldrev", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string OldRev { get { return oldrev; } set { oldrev = value; } }

        [TaskAttribute("newrev", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string NewRev { get { return newrev; } set { newrev = value; } }

        [TaskAttribute("patcherfiles", Required = false)]
        [StringValidator(AllowEmpty = true)]
        public string PatcherFiles { get { return patcherfiles; } set { patcherfiles = value; } }

        [TaskAttribute("verifyfiles", Required = false)]
        [StringValidator(AllowEmpty = true)]
        public string VerifyFiles { get { return verifyfiles; } set { verifyfiles = value; } }
        
        string srcurl;
        string destdir;
        string oldrev;
        string newrev;
        string verifyfiles = null;
        string patcherfiles = null;
     

        protected override void ExecuteTask()
        {
            if (srcurl.EndsWith("/") == false)
                srcurl += "/";

            if (destdir.EndsWith("\\") == false)
                destdir += "\\";


            if (oldrev == "0")
            {
                NoshellProcess.Execute("svn", string.Format("export --force -r {0} {1} {2}", newrev, srcurl, destdir));
            }
            else
            {
                System.Xml.XmlDocument xml = NoshellProcess.ExcuteXML("svn",
                    string.Format("diff -r {0}:{1} --summarize --xml {2}", oldrev, newrev, srcurl)); 
                
                System.Xml.XmlNodeList nodes = xml.SelectNodes("/diff/paths/path");

                for (int i = 0; i < nodes.Count; i++)
                {
                    XmlNode node = nodes.Item(i);

                    string kind = node.Attributes["kind"].Value;
                    string props = node.Attributes["props"].Value;
                    string item = node.Attributes["item"].Value;
                    string path = node.InnerXml.Replace(srcurl, "").Replace("/", "\\");

                    if (item == "none" && props != "none")
                        item = props;

                     if(kind == "file")
                     {
                         string destfilename = System.IO.Path.Combine(destdir, path);

                         if (System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(destfilename)) == false)
                             System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(destfilename));

                         System.Console.WriteLine(string.Format("Export {0}", path));
                         NoshellProcess.Execute("svn", string.Format("export --force -r {0} {1} {2}", newrev, node.InnerXml, destfilename));
                     }
                }
            }

            // 마지막으로 익스포트 된 파일을 전부 소문자로 변환한다.
            // 안하면 문제의 여지가 있다
            ConvertLowerCase(destdir);

            AddPatchInfo(System.IO.Path.Combine(destdir,"patchinfo.xml"), "PatcherFiles", patcherfiles);
            AddPatchInfo(System.IO.Path.Combine(destdir, "patchinfo.xml"), "VerifyFiles", verifyfiles);
            
        }

        void AddPatchInfo(string patchinfofilename, string nodename,string filestring)
        {
            if (filestring == null)
                return;

            string [] filenames = filestring.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);

            if (System.IO.File.Exists(patchinfofilename))
            {
                System.Xml.XmlDocument xml = new XmlDocument();
                xml.Load(patchinfofilename);

                System.Xml.XmlNode patchinfo = xml.SelectSingleNode("Patchinfo");

                System.Xml.XmlElement node = (System.Xml.XmlElement)patchinfo.SelectSingleNode(nodename);
                if (node == null)
                {
                    node = xml.CreateElement(nodename);
                    patchinfo.AppendChild(node);                
                }

                foreach(string name in filenames)
                {
                    System.Xml.XmlElement file= xml.CreateElement("File");
                    file.SetAttribute("Name", name);
                    node.AppendChild(file);
                }

                xml.Save(patchinfofilename);
            }
        }


        void AliasEXE(string parentdir)
        {

            // 일단 이름을 바꾼 후 
            foreach (string file in System.IO.Directory.GetFiles(parentdir,"*.exe", SearchOption.AllDirectories))
            {
                System.IO.File.Move(file, file.Replace(".exe", ".ex_"));
            }

            foreach (string file in System.IO.Directory.GetFiles(parentdir, "*.dll", SearchOption.AllDirectories))
            {
                System.IO.File.Move(file, file.Replace(".dll", ".dl_"));
            }

            // Patchinfo에 있는 데이터도 바꾸자 

            string xmlfilename = System.IO.Path.Combine(parentdir, "patchinfo.xml");
            if(System.IO.File.Exists((xmlfilename)))
            {
                System.Xml.XmlDocument xml = new XmlDocument();
                xml.Load(xmlfilename);

                System.Xml.XmlNodeList nodes = xml.SelectNodes("Patchinfo/Files/File");

                for(int i=0;i<nodes.Count;i++)
                {
                    System.Xml.XmlNode node = nodes.Item(i);

                    string name = node.Attributes["Name"].Value;
                    if(name.EndsWith(".exe") || name.EndsWith(".dll"))
                    {
                        ((XmlElement)node).SetAttribute("Alias",name.Replace(".exe",".ex_").Replace(".dll",".dl_"));
                    }
                }
                xml.Save(xmlfilename);
            }
        }

        void ConvertLowerCase(string parentdir)
        {
            foreach (string dir in System.IO.Directory.GetDirectories(parentdir))
            {
                string tmpdir = System.IO.Path.GetFileName(dir);

                while (System.IO.Directory.Exists(System.IO.Path.Combine(parentdir, tmpdir)) == true)
                {
                    tmpdir = "." + tmpdir;
                }
                tmpdir = System.IO.Path.Combine(parentdir, tmpdir);

                System.IO.Directory.Move(dir, tmpdir);
                System.IO.Directory.Move(tmpdir, dir.ToLower());
                
                ConvertLowerCase(dir);
            }

            foreach (string filename in System.IO.Directory.GetFiles(parentdir))
            {
                System.IO.File.Move(filename, filename.ToLower());
            }
        }

        public void Test() { ExecuteTask(); }
    }
}