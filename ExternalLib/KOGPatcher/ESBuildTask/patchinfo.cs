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
    [TaskName("DeleteFileinfo")]
    public class DeleteFileinfoTask : NAnt.Core.Task
    {
        [TaskAttribute("filename", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string FileName { get { return filename; } set { filename = value; } }

        [TaskAttribute("name", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string Name { get { return name; } set { name = value; } }

        string filename;
        string name;
  
        protected override void ExecuteTask()
        {
            if (System.IO.File.Exists(filename) == false)
            {
                throw new BuildException("ES.Build.DeleteNode Task Exception: " + filename + " file not found");
            }
            try
            {
                System.Xml.XmlDocument doc = new XmlDocument();
                doc.Load(filename);

                System.Xml.XmlNode node = doc.SelectSingleNode(string.Format("Patchinfo/Files/File[@Name=\"{0}\"]",name));

                if(node != null)
                {
                    node.ParentNode.RemoveChild(node);
                    doc.Save(filename);
                }
            }
            catch (System.Exception e)
            {
                throw new BuildException("GC.Build.SetRevision Task Exception", e);
            }
        }


        public void Test() { ExecuteTask(); }
    }

    [TaskName("SetRevision")]
    public class SetRevisionTask : NAnt.Core.Task
    {
        [TaskAttribute("filename", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string FileName { get { return filename; } set { filename = value; } }

        [TaskAttribute("type", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string TypeName { get { return type; } set { type = value; } }

        [TaskAttribute("revision", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string Revision { get { return revision; } set { revision = value; } }

        string filename;
        string type;
        string revision;

        protected override void ExecuteTask()
        {
            if (System.IO.File.Exists(filename) == false)
            {
                throw new BuildException("ES.Build.SetRevision Task Exception: " + filename + " file not found");
            }            
            try
            {
                System.Xml.XmlDocument doc = new XmlDocument();
                doc.Load(filename);

                System.Xml.XmlNode node = doc.SelectSingleNode("Patchinfo/Revisions/" + type);
                node.Attributes["Revision"].Value = revision;
                doc.Save(filename);
            }
            catch (System.Exception e)
            {
                throw new BuildException("GC.Build.SetRevision Task Exception", e);
            }
        }


        public void Test() { ExecuteTask(); }
    }

    [TaskName("GetRevision")]
    public class GetRevisionTask : NAnt.Core.Task
    {
        [TaskAttribute("filename", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string FileName { get { return filename; } set { filename = value; } }

        [TaskAttribute("type", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string TypeName { get { return type; } set { type = value; } }

        [TaskAttribute("property", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string Property { get { return property; } set { property = value; } }

        string filename;
        string type;
        string property;

        protected override void ExecuteTask()
        {
            System.Xml.XmlDocument doc = new XmlDocument();
            if (System.Text.RegularExpressions.Regex.IsMatch(filename, "http://.*") || System.Text.RegularExpressions.Regex.IsMatch(filename, "ftp://.*")) // 주소이면
            {
                System.Net.WebClient client = new System.Net.WebClient();
                string data = client.Encoding.GetString(client.DownloadData(filename)).Trim();
                if (System.Text.RegularExpressions.Regex.IsMatch(data, "<http://.*>") || System.Text.RegularExpressions.Regex.IsMatch(filename, "ftp://.*")) // 패치패스이면 
                {

                    string patchpath = data.Replace("<", "").Replace(">", "");
                    if (patchpath.EndsWith("/") == false)
                        patchpath += "/";

                    doc.Load(patchpath + "patchinfo.xml");
                }
                else
                {
                    doc.Load(filename);
                }
            }
            else
            {
                if (System.IO.File.Exists(filename) == false)
                {
                    throw new BuildException("ES.Build.GetRevision Task Exception: " + filename + " file not found");
                }      
                doc.Load(filename);
            }

            System.Xml.XmlNode node = doc.SelectSingleNode("Patchinfo/Revisions/" + type);

            if (Project.Properties.Contains(property))
            {
                Project.Properties[property] = node.Attributes["Revision"].Value;
            }
            else
            {
                Project.Properties.Add(property, node.Attributes["Revision"].Value);
            }
            
        }
        public void Test() { ExecuteTask(); }
    }



    [TaskName("Patchinfo")]
    public class PatchinfoTask : NAnt.Core.Task
    {        

        [TaskAttribute("workingdir", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string WorkDir
        {
            get { return workdir; }
            set { workdir = value; }
        }

        [TaskAttribute("filename", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string FileName
        {
            get { return filename; }
            set { filename = value; }
        }
                
        string filename = string.Empty;
        string workdir = ".";

        protected override void ExecuteTask()
        {
            if (System.IO.Directory.Exists(workdir) == false)
            {
                System.IO.Directory.CreateDirectory(workdir);
            }

            if (workdir.EndsWith("\\") == false)
                workdir += "\\";
            workdir = workdir.ToLower();
            // 마스터 디스크립터를 기록하자 
            // 알아먹기 쉽게 일단 xml로

            System.Collections.ArrayList files = new System.Collections.ArrayList();

            // 리스트 
            foreach (string path in System.IO.Directory.GetFiles(workdir, "*", System.IO.SearchOption.AllDirectories))
            {

                if (path.Contains("\\.svn\\") == false && System.IO.Path.GetFileName(path).ToLower() != System.IO.Path.GetFileName(filename).ToLower())
                    files.Add(path.ToLower().Replace(workdir, ""));
            }

            System.Xml.XmlDocument desc = new System.Xml.XmlDocument();                

            if (System.IO.File.Exists(filename) == true)
                desc.Load(filename);
            else                
                desc.AppendChild(desc.CreateNode(System.Xml.XmlNodeType.XmlDeclaration, "", ""));

            System.Xml.XmlElement patchinfo = (System.Xml.XmlElement)desc.SelectSingleNode("Patchinfo"); 
            
            if(patchinfo == null)
            {
                patchinfo = desc.CreateElement("Patchinfo");
                desc.AppendChild(patchinfo);
            }

            System.Xml.XmlElement revisions = (System.Xml.XmlElement)patchinfo.SelectSingleNode("Revisions");
            if (revisions == null)
            {
                revisions = desc.CreateElement("Revisions");
                patchinfo.AppendChild(revisions);
            }

            System.Xml.XmlElement packing = (System.Xml.XmlElement)revisions.SelectSingleNode("Packing");
            if(packing==null)
            {
                packing = desc.CreateElement("Packing");
                packing.SetAttribute("Revision", "0");
                revisions.AppendChild(packing);
            }

            System.Xml.XmlElement client = (System.Xml.XmlElement)revisions.SelectSingleNode("Client");
            if (client == null)
            {
                client = desc.CreateElement("Client");
                client.SetAttribute("Revision", "0");
                revisions.AppendChild(client);
            }

            System.Xml.XmlElement patcher = (System.Xml.XmlElement)revisions.SelectSingleNode("Patcher");
            if (patcher == null)
            {
                patcher = desc.CreateElement("Patcher");
                patcher.SetAttribute("Revision", "0");
                revisions.AppendChild(patcher);
            }

            System.Xml.XmlElement patch = (System.Xml.XmlElement)revisions.SelectSingleNode("Patch");
            if (patch == null)
            {
                patch = desc.CreateElement("Patch");
                patch.SetAttribute("Revision", "0");
                revisions.AppendChild(patch);
            }

            System.Xml.XmlElement filesnode = (System.Xml.XmlElement)patchinfo.SelectSingleNode("Files");
            if (filesnode != null)
                patchinfo.RemoveChild(filesnode);                
            
            filesnode = desc.CreateElement("Files");
            patchinfo.AppendChild(filesnode);                

            
            foreach (string path in files)
            {
                System.Xml.XmlElement file = desc.CreateElement("File");

                System.IO.FileInfo fi = new System.IO.FileInfo(System.IO.Path.Combine(workdir, path));

                file.SetAttribute("Name", path);
                file.SetAttribute("Size", fi.Length.ToString());

                Kom2.NET.Komfile kom = new Kom2.NET.Komfile();

                if (kom.Open(System.IO.Path.Combine(workdir, path)) == true)
                {
                    file.SetAttribute("Checksum", string.Format("{0:x8}", kom.Adler32));
                    file.SetAttribute("FileTime", string.Format("{0:x8}", kom.FileTime));
                    kom.Close();
                }
                else
                {
                    file.SetAttribute("Checksum", string.Format("{0:x8}", Kom2.NET.AdlerCheckSum.GetAdler32(System.IO.Path.Combine(workdir, path))));
                    file.SetAttribute("FileTime", "0");
                }

                filesnode.AppendChild(file);
            }

            if(System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(filename))==false)
            {
                System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(filename));
            }
            desc.Save( filename);
            
    
        }

        public void Test() { ExecuteTask(); }
    }
}
