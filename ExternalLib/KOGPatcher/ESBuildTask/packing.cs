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
    [TaskName("PackingKom")]
    public class PackingKomTask : NAnt.Core.Task
    {
        [TaskAttribute("packinginfo", Required = false)]
        [StringValidator(AllowEmpty = true)]
        public string KomList { get { return komlist; } set { komlist = value; } }

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


        [TaskAttribute("resdir", Required = true)]
        [StringValidator(AllowEmpty = false)]
        public string ResDir { get { return resdir; } set { resdir = value; } }

        [TaskAttribute("xor", Required = false)]
        [StringValidator(AllowEmpty = true)]
        public string Xor { get { return xor; } set { xor = value; } }

        [TaskAttribute("luac", Required = false)]
        [StringValidator(AllowEmpty = true)]
        public string Luac { get { return luac; } set { luac = value; } }
        

        string komlist=null;
        string srcurl;        
        string destdir;
        string oldrev;
        string newrev;
        string resdir;
        string xor = "false";
        string luac = "false";

        string CheckDir(System.Collections.Hashtable table,string path)
        {
            while (path != null && path.Length >0)
            {
                if(table.Contains(path))
                    return path;
                
                path = System.IO.Path.GetDirectoryName(path);                
            }
            return "\\";
        }


        void ExcuteWithKomlist()
        {
            if (System.IO.File.Exists(komlist) == false)
            {
                throw new BuildException("ES.Build.PackingKom Task Exception: " + komlist + " file not found");
            }

            System.Collections.Hashtable komtable = new System.Collections.Hashtable();
            System.Collections.Hashtable dirtable = new System.Collections.Hashtable();
            System.Collections.SortedList updatetable = new System.Collections.SortedList();

            Kom2.NET.XorEncrypt encrypt = new Kom2.NET.XorEncrypt();

            System.Xml.XmlDocument xml = new XmlDocument();
            xml.Load(komlist);

            XmlNodeList nodes;

            nodes = xml.SelectNodes("Packinginfo/Xors/Xor");
            for (int i = 0; i < nodes.Count; i++)
            {
                XmlNode node = nodes.Item(i);
                encrypt.AddKey(System.Convert.ToUInt32(node.Attributes["Value"].Value));
            }

            nodes = xml.SelectNodes("Packinginfo/Koms/kom");
            for (int i = 0; i < nodes.Count; i++)
            {
                XmlNode node = nodes.Item(i);
                komtable.Add(node.Attributes["Source"].Value, node.Attributes["Dest"].Value);
            }

            nodes = xml.SelectNodes("Packinginfo/Dirs/Dir");
            for (int i = 0; i < nodes.Count; i++)
            {
                XmlNode node = nodes.Item(i);
                dirtable.Add(node.Attributes["Source"].Value, node.Attributes["Dest"].Value);
            }

            if (oldrev == "0")
            {
                xml = NoshellProcess.ExcuteXML("svn", string.Format("ls --xml -r {0} {1}", newrev, srcurl));
                nodes = xml.SelectNodes("lists/list/entry");

                foreach (string dirname in dirtable.Keys)
                {
                    xml = NoshellProcess.ExcuteXML("svn", string.Format("ls --xml -r {0} {1}", newrev, srcurl + dirname.Replace("\\", "/")));
                    nodes = xml.SelectNodes("lists/list/entry");
                    for (int i = 0; i < nodes.Count; i++)
                    {
                        XmlNode node = nodes.Item(i);
                        if (node.Attributes["kind"].Value == "file")
                        {
                            updatetable.Add(System.IO.Path.Combine(dirname, node.SelectSingleNode("name").InnerText), "added");
                        }
                    }
                }

                foreach (string komname in komtable.Keys)
                {
                    updatetable.Add(komname, "added");
                }
            }
            else
            {
                xml = NoshellProcess.ExcuteXML("svn",
                    string.Format("diff -r {0}:{1} --summarize --xml {2}", oldrev, newrev, srcurl));

                nodes = xml.SelectNodes("/diff/paths/path");

                for (int i = 0; i < nodes.Count; i++)
                {
                    XmlNode node = nodes.Item(i);

                    string kind = node.Attributes["kind"].Value;
                    string props = node.Attributes["props"].Value;
                    string item = node.Attributes["item"].Value;
                    string path = node.InnerXml.Replace(srcurl, "").Replace("/", "\\");

                    if (item == "none" && props != "none")
                        item = props;

                    string komname = CheckDir(komtable, path);
                    string dirname = CheckDir(dirtable, path);

                    if (komtable.Contains(komname))
                    {
                        if (kind == "dir")
                        {
                            if (updatetable.Contains(komname) == false)
                                updatetable.Add(komname, item);
                            else
                                updatetable[komname] = item;
                        }
                        else if (updatetable.Contains(komname) == false)
                        {
                            updatetable.Add(komname, "modified");
                        }
                    }
                    else if (dirtable.Contains(dirname) && kind == "file")
                    {
                        if (updatetable.Contains(path) == false)
                        {
                            updatetable.Add(path, item);
                        }
                    }
                }
            }


            foreach (System.Collections.DictionaryEntry entry in updatetable)
            {
                string path = entry.Key.ToString();
                string kind = entry.Value.ToString();
                string srcfileurl = srcurl + path.Replace("\\", "/");

                string komname = CheckDir(komtable, path);
                string dirname = CheckDir(dirtable, path);

                if (komtable.Contains(komname))
                {
                    string destfilename = System.IO.Path.Combine(destdir, komtable[komname].ToString() + ".kom");
                    Console.WriteLine(String.Format("{0} {1}", kind, destfilename.Replace(destdir, "")));

                    if (kind == "deleted")
                    {
                        if (0 != NoshellProcess.Execute("svn", string.Format("delete {0}", destfilename)))
                            throw new Exception("Delete Error");
                    }
                    else
                    {
                        if (System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(destfilename)) == false)
                            System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(destfilename));

                        string temppath = System.IO.Path.Combine(resdir, System.IO.Path.GetFileName(path));
                        if (System.IO.Directory.Exists(temppath))
                            System.IO.Directory.Delete(temppath, true);

                        if (0 != NoshellProcess.Execute("svn", string.Format("export -r {0} \"{1}\" \"{2}\"", newrev, srcfileurl, temppath)))
                            throw new Exception("Export Error");

                        foreach (string luafile in System.IO.Directory.GetFiles(temppath, "*.lua", SearchOption.AllDirectories))
                        {

                            if (luac == "true")
                            {
                                System.IO.StreamReader reader = new StreamReader(luafile);
                                System.Text.Encoding encode = reader.CurrentEncoding;
                                reader.Close();

                                if (encode == System.Text.Encoding.UTF8)
                                    NoshellProcess.Execute("luac_utf8", string.Format("-o {0} {0}", luafile));
                                else
                                    NoshellProcess.Execute("luac", string.Format("-o {0} {0}", luafile));
                            }
                            if (xor == "true")
                                encrypt.Encrypt(luafile);
                        }
                        if (xor == "true")
                        {
                            foreach (string txtfile in System.IO.Directory.GetFiles(temppath, "*.txt", SearchOption.AllDirectories))
                            {
                                encrypt.Encrypt(txtfile);
                            }
                        }

                        Kom2.NET.Komfile.CompressKom(temppath, destfilename);
                        if (kind == "added")
                        {
                            if (0 != NoshellProcess.Execute("svn", string.Format("add --parents \"{0}\"", destfilename)))
                                throw new Exception("Add Error");
                        }
                    }

                }
                else if (dirtable.Contains(dirname))
                {
                    string destfilename = System.IO.Path.Combine(destdir, System.IO.Path.Combine(dirtable[dirname].ToString(), System.IO.Path.GetFileName(path)));
                    Console.WriteLine(String.Format("{0} {1}", kind, destfilename.Replace(destdir, "")));

                    if (kind == "deleted")
                    {
                        if (0 != NoshellProcess.Execute("svn", string.Format("delete \"{0}\"", destfilename)))
                            throw new Exception("Delete Error");
                    }
                    else
                    {
                        if (System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(destfilename)) == false)
                            System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(destfilename));

                        if (0 != NoshellProcess.Execute("svn", string.Format("export --force -r {0} \"{1}\" \"{2}\"", newrev, srcfileurl, destfilename)))
                            throw new Exception("Export Error");

                        if (kind == "added")
                        {
                            if (0 != NoshellProcess.Execute("svn", string.Format("add --parents \"{0}\"", destfilename)))
                                throw new Exception("Add Error");
                        }
                    }
                }
            }

        }

        string CombinPath(string a,string b)
        {
            if (a.EndsWith("/"))
                return a + b;
            else
                return a + "/" + b;
        }

        void SearchKom(string url, string rev, System.Collections.Hashtable table)
        {
            System.Xml.XmlDocument xml = new XmlDocument();
            XmlNodeList nodes;

            xml = NoshellProcess.ExcuteXML("svn", string.Format("ls --xml -r {0} {1}", rev, url));
            nodes = xml.SelectNodes("lists/list/entry");
            for (int i = 0; i < nodes.Count; i++)
            {
                XmlNode node = nodes.Item(i);

                string kind = node.Attributes["kind"].Value;
                string path = node.SelectSingleNode("./name/text()").Value;
                if (kind == "file")
                {
                    table.Add(CombinPath(url, path).Replace(srcurl, "").Replace("/", "\\"), "added");
                }
                else if (kind == "dir")
                {
                    if(path.EndsWith(".kom")==true)
                    {
                        table.Add(CombinPath(url, path).Replace(srcurl, "").Replace("/", "\\"), "added");
                    }                     
                    else
                    {
                        SearchKom(CombinPath(url, path), rev, table);
                    }
                }
            }
        }

        void SearchExternal(string url, string rev, System.Collections.Hashtable externals, System.Collections.Hashtable table)
        {
            System.Xml.XmlDocument xml = new XmlDocument();
            XmlNodeList nodes;


            xml = NoshellProcess.ExcuteXML("svn", string.Format("pg svn:externals --xml -r {0} {1}", rev, url));
            nodes = xml.SelectNodes("properties/target/property/text()");
            for (int i = 0; i < nodes.Count; i++)
            {
                XmlNode node = nodes.Item(i);
                string [] props = node.Value.Split(new string[] { " ", "\r\n", "\n", "\r" }, StringSplitOptions.RemoveEmptyEntries);

                if(props.Length%2 !=0)
                {
                    throw new BuildException( url + " svn:external error");
                }

                for(int j=0;j<props.Length/2;j++)
                {
                    table.Add(CombinPath(url, props[j * 2 + 1]).Replace(srcurl, "").Replace("/", "\\"),"added");
                    externals.Add(CombinPath(url, props[j * 2 + 1]).Replace(srcurl, "").Replace("/", "\\"), props[j * 2]);
                }
            }

            xml = NoshellProcess.ExcuteXML("svn", string.Format("ls --xml -r {0} {1}", rev, url));
            nodes = xml.SelectNodes("lists/list/entry");
            for (int i = 0; i < nodes.Count; i++)
            {
                XmlNode node = nodes.Item(i);

                string kind = node.Attributes["kind"].Value;
                string path = node.SelectSingleNode("./name/text()").Value;               
               
                if(kind=="dir" && path.EndsWith(".kom")==false)
                {
                    SearchExternal(CombinPath(url, path), rev, externals,table);                
                }                
            }
        }

        
        void ExcuteWithoutKomlist()
        {           

            System.Console.WriteLine(string.Format("<packing resdir={0} destdir={1} srcurl={2} oldrev={3} newrev={4}>", resdir, destdir, srcurl, oldrev, newrev));



            System.Collections.Hashtable table = new System.Collections.Hashtable();
            System.Collections.Hashtable externals = new System.Collections.Hashtable();


            SearchExternal(srcurl, newrev, externals,table);

            if (oldrev == "0")
            {
                SearchKom(srcurl, newrev, table);
            }
            else       
            {
                System.Xml.XmlDocument xml = NoshellProcess.ExcuteXML("svn",
                    string.Format("diff -r {0}:{1} --summarize --xml {2}", oldrev, newrev, srcurl));

                XmlNodeList nodes = xml.SelectNodes("/diff/paths/path");

                for (int i = 0; i < nodes.Count; i++)
                {
                    XmlNode node = nodes.Item(i);

                    string kind = node.Attributes["kind"].Value;
                    string props = node.Attributes["props"].Value;
                    string item = node.Attributes["item"].Value;
                    string path = node.InnerXml.Replace(srcurl, "").Replace("/", "\\");

                    if (item == "none" && props != "none")
                        item = props;
                
                    int k = path.IndexOf(".kom");
                    if (k > 0)
                    {
                        string komfilename = path.Substring(0, k + 4);

                        if (kind == "dir")
                        {
                            if (table.Contains(path) == false)
                                table.Add(path, item);
                            else
                                table[path] = item;
                        }
                        else if (table.Contains(komfilename) == false)
                        {
                            table.Add(komfilename, "modified");
                        }
                    }
                    else if (kind == "file")
                    {
                        table.Add(path, item);
                    }
                }
            }


            foreach (System.Collections.DictionaryEntry entry in table)
            {
                string urlfile = CombinPath(srcurl,entry.Key.ToString().Replace('\\', '/'));
                string srcfile = System.IO.Path.Combine(resdir,entry.Key.ToString());
                string destfile = System.IO.Path.Combine(destdir,entry.Key.ToString());
                string srcdir = System.IO.Path.GetDirectoryName(srcfile);
                string destdir2 = System.IO.Path.GetDirectoryName(destfile);

                Console.WriteLine(String.Format("process {0}", urlfile));

                if (entry.Value.ToString() == "deleted")
                {
                    try
                    {
                        if (NoshellProcess.Execute("svn", string.Format("delete \"{0}\"", destfile)) != 0)
                            throw new Exception();
                        Console.WriteLine("\tdelete");
                    }
                    catch
                    {
                        throw new BuildException("GC.Build.Packing Task Exception: " + urlfile + " delete failed");
                    }
                }
                else
                {
                    if (System.IO.Directory.Exists(srcdir) == false)
                        System.IO.Directory.CreateDirectory(srcdir);

                    if (System.IO.Directory.Exists(destdir2) == false)
                        System.IO.Directory.CreateDirectory(destdir2);

                    try
                    {
                        if (externals.Contains(entry.Key) == true)
                        {
                            Console.WriteLine(String.Format("External from {0}", externals[entry.Key].ToString()));
                            if (NoshellProcess.Execute("svn", string.Format("export  \"{0}\" \"{1}\"", externals[entry.Key].ToString(), srcfile)) != 0)
                                throw new Exception();
                        }
                        else
                        {
                            if (NoshellProcess.Execute("svn", string.Format("export -r {0} \"{1}\" \"{2}\"", newrev, urlfile, srcfile)) != 0)
                                throw new Exception();
                        }
                    }
                    catch
                    {
                        throw new BuildException("GC.Build.Packing Task Exception: " + urlfile + " export failed");
                    }

                    if (System.IO.Directory.Exists(srcfile) == true) //디렉토리면 
                    {
                        if (srcfile.ToLower().EndsWith(".kom") == true) // 콤파일이면 
                        {
                            

                            try
                            {
                                Kom2.NET.Komfile.CompressKom(srcfile, destfile);                                
                            }
                            catch
                            {
                                throw new BuildException("GC.Build.Packing Task Exception: " + urlfile + " kom archive failed");
                            }
                        }
                        else
                        {
                            if (System.IO.Directory.Exists(destfile) == false)
                                System.IO.Directory.CreateDirectory(destfile);
                        }
                    }
                    else
                    {
                        System.IO.File.Copy(srcfile, destfile, true);
                    }

                    if (entry.Value.ToString() == "added")
                    {
                        try
                        {

                            if (NoshellProcess.Execute("svn", string.Format("add --parents \"{0}\"", destfile)) != 0)
                                throw new Exception();
                            Console.WriteLine("\tadd");
                        }
                        catch
                        {
                            throw new BuildException("GC.Build.Packing Task Exception: " + urlfile + " svn add failed");
                        }
                    }
                    else
                    {
                        Console.WriteLine("\tmodify");
                    }
                }
            }
            
        }

        protected override void ExecuteTask()
        {
            if (srcurl.EndsWith("/") == false)
                srcurl += "/";

            if (destdir.EndsWith("\\") == false)
                destdir += "\\";



            if (System.IO.Directory.Exists(resdir) == true)
                System.IO.Directory.Delete(resdir,true);

            if (System.IO.Directory.Exists(resdir) == false)
                System.IO.Directory.CreateDirectory(resdir);

            if (KomList == null)
                ExcuteWithoutKomlist();
            else
                ExcuteWithKomlist();

            if (System.IO.Directory.Exists(resdir) == true)
                System.IO.Directory.Delete(resdir, true);
        }


        public void Test() { ExecuteTask(); }
    }
}