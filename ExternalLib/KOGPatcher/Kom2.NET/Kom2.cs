using System;
using System.Collections.Generic;
using System.Text;
using ComponentAce.Compression.Libs;

namespace Kom2.NET
{
    public class XorEncrypt
    {
        public void AddKey(UInt32 key)
        {
            keylist.Add(key);
        }
        
        public bool Encrypt(string readfile)
        {
            string writefile = System.IO.Path.GetTempFileName();
            Encrypt(readfile,writefile);
            System.IO.File.Copy(writefile,readfile, true);
            System.IO.File.Delete(writefile);
            return true;
        }
        public bool Encrypt(string readfile,string writefile)
        {
            if(System.IO.Path.GetFullPath(readfile).ToLower() == System.IO.Path.GetFullPath(writefile).ToLower())
                return Encrypt(readfile);

            System.IO.FileStream read= new System.IO.FileStream(readfile, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            System.IO.FileStream write= new System.IO.FileStream(writefile, System.IO.FileMode.Create, System.IO.FileAccess.Write);
            
            bool re = Encrypt(read,write);

            read.Close();
            write.Close();
            return true;
        }

        public bool Encrypt(System.IO.Stream readstream, System.IO.Stream writestream)
        {
            if (keylist.Count == 0)
                return false;

            int read = 1;
            while(read>0)
            {                
                foreach( UInt32 val in keylist)
                {
                    byte [] key = System.BitConverter.GetBytes(val);
                    byte [] buffer = new byte[key.Length];

                    read = readstream.Read(buffer,0,key.Length);
                    if(read <=0)
                        break;

                    for(int i=0;i<key.Length;i++)
                    {
                        buffer[i] = (byte)(buffer[i] ^ key[i]);
                    }

                    writestream.Write(buffer,0,read);
                }                
            }
            return true;
        }

        System.Collections.Generic.List<UInt32> keylist = new List<uint>();
    }
  

    public class AdlerCheckSum
    {
        public const UInt32 AdlerBase = 0xFFF1;
        public const UInt32 AdlerStart = 0x0001;

        static public UInt32 GetAdler32(string filename)
        {
            System.IO.FileStream stream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            UInt32 adler32 = GetAdler32(stream,0,(int)stream.Length);
            stream.Close();
            return adler32;
        }

        static public UInt32 GetAdler32(string filename,int offset,int length)
        {
            System.IO.FileStream stream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            UInt32 adler32 = GetAdler32(stream,offset,length);
            stream.Close();
            return adler32;
        }

        static public UInt32 GetAdler32(System.IO.Stream stream,int offset,int len)
        {
            if (stream.Length == 0)
                return 0;

            long pos = stream.Position;

            UInt32 adler32 = AdlerStart;
            UInt32 unSum1 = adler32 & 0xFFFF;
            UInt32 unSum2 = (adler32 >> 16) & 0xFFFF;

            stream.Position = offset;
            
            while (len >0)
            {
                int i = stream.ReadByte();
                if (i < 0)
                    break;

                unSum1 = (unSum1 + (uint)i) % AdlerBase;
                unSum2 = (unSum1 + unSum2) % AdlerBase;
                len--;
            }

            stream.Position = pos;
            return ((unSum2 << 16) + unSum1);
        }

        static public UInt32 GetAdler32(byte [] data,uint offset,uint length )
        {
            if (length == 0)
                return 0;

            UInt32 adler32 = AdlerStart;
            UInt32 unSum1 = adler32 & 0xFFFF;
            UInt32 unSum2 = (adler32 >> 16) & 0xFFFF;                       

            for(uint i=offset;i<offset+length;i++)
            {

                unSum1 = (unSum1 + (uint)data[i]) % AdlerBase;
                unSum2 = (unSum1 + unSum2) % AdlerBase;
            }
            return ((unSum2 << 16) + unSum1);
        }
    }
        
    public abstract class Kom2SubFile
    {        
        public enum AlgorithmType 
        {
            infilate = 0,
            infilateandblowfish = 1
        }

        public enum NeedCypherType
        {
            none = 0,
            encrypt = 1,
            decrypt = 1
        }

        static public bool Equals(Kom2SubFile x,Kom2SubFile y)
        {
            if (x.Adler32 != y.Adler32 || x.Size != y.Size || x.FileTime != y.FileTime || x.CompressedSize != y.CompressedSize)
                return false;
            return true;
        }

        public static Kom2SubFileFromKom ReadSubFileFromKom(string komfilename,System.Xml.XmlElement node, UInt32 offset)
        {
            Kom2SubFileFromKom subfile = new Kom2SubFileFromKom();
            subfile.filename = komfilename;
            subfile.ReadHeader(node);
            subfile.offset = offset;
            return subfile;
        }

        public static Kom2SubFileFromKom ReadSubFileFromOldKom(string komfilename, System.IO.BinaryReader reader, uint headersize)
        {
            Kom2SubFileFromKom subfile = new Kom2SubFileFromKom();            
            subfile.size = reader.ReadUInt32();
            subfile.compressedsize = reader.ReadUInt32();
            subfile.offset = headersize + reader.ReadUInt32();
            subfile.adler32 = AdlerCheckSum.GetAdler32(komfilename, (int)subfile.offset, (int)subfile.compressedsize);

            subfile.filename = komfilename;
            return subfile;
        }

        public static Kom2SubFileFromFile ReadSubFileFromFile(string filename)
        {
            Kom2SubFileFromFile subfile = new Kom2SubFileFromFile();
            subfile.filename = filename;
            return subfile;
        }


        public static Kom2SubFileFromKom ReadSubFileFromKom(BlowfishNET.BlowfishECB CBC,string komfilename, System.Xml.XmlElement node, UInt32 offset)
        {
            Kom2SubFileFromKom subfile = new Kom2SubFileFromKom();
            subfile.ecb = CBC;
            subfile.filename = komfilename;
            subfile.ReadHeader(node);
            subfile.offset = offset;            
            return subfile;
        }        

        static protected byte[] buffer = new byte[4096];

        public abstract void Convert(BlowfishNET.BlowfishECB ECB, AlgorithmType newalgorithm);
        public abstract void WriteCompressed(System.IO.Stream stream);
        public abstract void WriteDecompressed(System.IO.Stream stream);
        public abstract void Close();

        protected string filename;        
        protected UInt32 adler32 = 0;
        protected uint size = 0;
        protected uint compressedsize = 0;
        protected uint offset = 0;
        protected uint filetime = 0;
        protected AlgorithmType algorithm = AlgorithmType.infilate;
        protected AlgorithmType oldalgorithm = AlgorithmType.infilate;        

        abstract public UInt32 Size { get; }
        abstract public UInt32 CompressedSize { get; }
        abstract public UInt32 Adler32 { get;}
        abstract public UInt32 FileTime { get;}
                
        public UInt32 Algorithm
        {
            get { return System.Convert.ToUInt32(algorithm); }
        }

        public void WriteHeader(System.Xml.XmlElement node)
        {
            node.SetAttribute("Size", Size.ToString());
            node.SetAttribute("CompressedSize", CompressedSize.ToString());
            node.SetAttribute("Checksum", String.Format("{0:x8}", Adler32));
            node.SetAttribute("FileTime", String.Format("{0:x8}", FileTime));
            node.SetAttribute("Algorithm", Algorithm.ToString());
        }

        void ReadHeader(System.Xml.XmlElement node)
        {
            size = System.Convert.ToUInt32(node.GetAttribute("Size"));
            compressedsize = System.Convert.ToUInt32(node.GetAttribute("CompressedSize"));
            adler32 = System.Convert.ToUInt32(node.GetAttribute("Checksum"),16);
            filetime = System.Convert.ToUInt32(node.GetAttribute("FileTime"),16);
            algorithm = (AlgorithmType)System.Convert.ToUInt32(node.GetAttribute("Algorithm"));
            oldalgorithm = algorithm;
        }
        protected BlowfishNET.BlowfishECB ecb = null;
    }

  
    public class Kom2SubFileFromKom : Kom2SubFile
    {   
        public override void Convert(BlowfishNET.BlowfishECB ECB, AlgorithmType newalgorithm)
        {
            ecb = ECB;                                   
            algorithm = newalgorithm;            

            if (oldalgorithm == algorithm || compressedsize < BlowfishNET.BlowfishECB.BLOCK_SIZE)
            {
                compressedmem = null;
                return;
            }

            int encryptlen = (int)compressedsize;

            compressedmem = new System.IO.MemoryStream();
            System.IO.FileStream filestream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            filestream.Position = offset;
            byte[] data = new byte[encryptlen];
            filestream.Read(data, 0, encryptlen);
            byte[] tmp = (byte[])data.Clone();

            if (encryptlen % BlowfishNET.BlowfishECB.BLOCK_SIZE != 0)
                encryptlen -= encryptlen % BlowfishNET.BlowfishECB.BLOCK_SIZE;

            if (algorithm == AlgorithmType.infilateandblowfish)
            {
                ecb.Encrypt(tmp, 0, data, 0, encryptlen);
            }
            else
            {
                ecb.Decrypt(tmp, 0, data, 0, encryptlen);
            }
            compressedmem.Write(data, 0, (int)compressedsize);
            adler32 = AdlerCheckSum.GetAdler32(compressedmem, 0, (int)compressedmem.Length);            
            filestream.Close();
        }

        System.IO.MemoryStream compressedmem;


        
        public override void WriteCompressed(System.IO.Stream writestream)
        {
            System.IO.FileStream filestream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);            
            System.IO.Stream readstream = filestream;
            readstream.Position = offset;

            if(compressedmem != null && compressedmem.Length== compressedsize)
            {
                readstream = compressedmem;
                readstream.Position = 0;
            }
            
            byte[] data = new byte[(int)compressedsize];
            readstream.Read(data, 0, (int)compressedsize);
            writestream.Write(data, 0, (int)compressedsize);

            writestream.Flush();
            filestream.Close();
        }

        public override void WriteDecompressed(System.IO.Stream writestream)
        {
            System.IO.FileStream filestream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            System.IO.Stream readstream = filestream;
            readstream.Position = offset;

            if (compressedmem != null && compressedmem.Length == compressedsize)
            {
                readstream = compressedmem;
                readstream.Position = 0;
            }

            ComponentAce.Compression.Libs.zlib.ZOutputStream zstream = new ComponentAce.Compression.Libs.zlib.ZOutputStream(writestream);
                        
            int encryptlen = (int)compressedsize;

            byte[] data = new byte[(int)compressedsize];
            readstream.Read(data, 0, (int)compressedsize);

            if (algorithm == AlgorithmType.infilateandblowfish && encryptlen >= BlowfishNET.BlowfishECB.BLOCK_SIZE)             
            {
                if (ecb == null)
                    throw new Exception("Encrypted File");
                
                if (encryptlen % BlowfishNET.BlowfishECB.BLOCK_SIZE != 0)                
                    encryptlen -= encryptlen % BlowfishNET.BlowfishECB.BLOCK_SIZE;                
                
                byte[] tmp = (byte [])data.Clone();
                ecb.Decrypt(tmp, 0, data, 0, encryptlen);                
            }

            zstream.Write(data, 0, (int)compressedsize);
            zstream.finish();
            
            filestream.Close();
            writestream.Flush();
                    
        }        

        public override uint Size{ get { return size; }}
        public override uint CompressedSize{ get { return compressedsize; }}
        public override uint Adler32{get { return adler32; }}
        public override uint FileTime{get { return filetime; }}
        public override void Close() { }
    }

    public class Kom2SubFileFromFile : Kom2SubFile	
    {

        public override void Convert(BlowfishNET.BlowfishECB ECB,AlgorithmType newalgorithm)
        {            
            ecb = ECB;
            oldalgorithm = algorithm;
            algorithm = newalgorithm;

            if(compressedmem == null)
            {
                LoadAndCompress();
            }

            if (oldalgorithm == algorithm || compressedsize < BlowfishNET.BlowfishECB.BLOCK_SIZE)
                return;            

            int encryptlen = (int)compressedsize;
            if (encryptlen % BlowfishNET.BlowfishECB.BLOCK_SIZE != 0)
                encryptlen -= encryptlen % BlowfishNET.BlowfishECB.BLOCK_SIZE;

            byte [] tmp = (byte []) compressedmem.GetBuffer().Clone();

            if (algorithm == AlgorithmType.infilateandblowfish)
            {
                ecb.Encrypt(tmp, 0, compressedmem.GetBuffer(), 0, encryptlen);
            }
            else
            {
                ecb.Decrypt(tmp, 0, compressedmem.GetBuffer(), 0, encryptlen);
            }
            adler32 = AdlerCheckSum.GetAdler32(compressedmem, 0, (int)compressedmem.Length);            
        }

        public override void WriteCompressed(System.IO.Stream stream)
        {
            if (compressedmem == null)
                LoadAndCompress();           
            stream.Write(compressedmem.GetBuffer(), 0, (int)compressedmem.Length); // 이미 메모리에 있으니 그냥 다 써버리자            
            stream.Flush();
        }

        public override void WriteDecompressed(System.IO.Stream stream)
        {
            System.IO.FileStream file = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);

            int totallen = (int)file.Length;
            int len = buffer.Length;
            while (totallen > 0)
            {
                if (totallen < buffer.Length)
                    len = totallen;

                file.Read(buffer, 0, len);
                stream.Write(buffer, 0, len);
                totallen -= len;
            }
            file.Close();
            stream.Flush();
        }

        void LoadAndCompress()
        {
            compressedmem = new System.IO.MemoryStream();

            System.IO.FileInfo fi = new System.IO.FileInfo(filename);
            System.IO.FileStream filestream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            ComponentAce.Compression.Libs.zlib.ZOutputStream zstream = new ComponentAce.Compression.Libs.zlib.ZOutputStream(compressedmem, ComponentAce.Compression.Libs.zlib.zlibConst.Z_DEFAULT_COMPRESSION);

            int totallen = (int)Size;
            int len = buffer.Length;
 
            while (totallen>0)
            {
                if (len > totallen)
                    len = totallen;
                filestream.Read(buffer, 0, len);
                zstream.Write(buffer, 0, len);
                totallen-=len;
            }
            zstream.finish();            
            filestream.Close();
            
            compressedsize = (uint)compressedmem.Length;
            adler32 = AdlerCheckSum.GetAdler32(compressedmem, 0, (int)compressedmem.Length);            
        }

        System.IO.MemoryStream compressedmem = null;

        public override UInt32 Size
        {
            get 
            {
                if (size == 0)
                {
                    System.IO.FileInfo info = new System.IO.FileInfo(filename);
                    size = (UInt32)info.Length;
                }
                return size;
            }
        }

        public override UInt32 CompressedSize
        {
            get 
            {
                if (compressedmem == null)
                    LoadAndCompress();

                return compressedsize;
            }
        }

        public override UInt32 Adler32
        {
            get 
            {
                if (compressedmem == null)
                    LoadAndCompress();

                return adler32; 
            }
        }

        public override uint FileTime
        {
            get 
            {
                if (filetime == 0)
                {
                    System.IO.FileInfo info = new System.IO.FileInfo(filename);
                    filetime = (UInt32)info.LastWriteTimeUtc.ToFileTimeUtc();
                }
                return filetime;
            }
        }

        public override void Close()
        {
            compressedmem.Close();
        }
    }

    public class EncryptedKomException: Exception
    {
        public EncryptedKomException()
        {
            
        }
    }
    
    public class Komfile
    {        
        public enum EKomType
        {
            notkom = 0x00,
            oldkom = 0x01,
            newkom = 0x02,
            encrypt = 0x04

        }
        
        public Komfile()
        {

        }

        BlowfishNET.BlowfishECB ecb = null;
        public BlowfishNET.BlowfishECB ECB 
        {
            get { return ecb; }
        }


        public void SetECB(string pw)
        {
            byte[] key = System.Text.Encoding.ASCII.GetBytes(pw);
            ecb = new BlowfishNET.BlowfishECB(key, 0, key.Length);
            komtype = EKomType.encrypt;
        }

        static bool ContainExt(string [] exts,string filename)
        {
            string ext = System.IO.Path.GetExtension(filename).ToLower();
            foreach(string s in exts)
            {
                if(s.ToLower() == ext)                
                    return true;                
            }
            return false;
        }

        static public bool CompressKom(string dir, string filename,string keystr)
        {
            if (System.IO.Directory.Exists(dir) == false)
                return false;

            Komfile kom = new Komfile();

            string[] files = System.IO.Directory.GetFiles(dir, "*", System.IO.SearchOption.AllDirectories);

            foreach (string file in files)
            {
                if (file.Contains(".svn\\") == false)
                {
                    string key = System.IO.Path.GetFileName(file);
                    if (kom.subfiles.ContainsKey(key) == false)
                        kom.Add(key, Kom2SubFile.ReadSubFileFromFile(file), true);
                }
            }
            if (System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(filename)) == false)
                System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(filename));

            kom.SetECB(keystr);
            kom.Encrypt();
            kom.Save(filename);
            kom.Close();
            return true;
        }

        static public bool CompressKom(string dir,string filename)
        {
            if (System.IO.Directory.Exists(dir) == false)
                return false;

            Komfile kom = new Komfile();

            string[] files = System.IO.Directory.GetFiles(dir, "*", System.IO.SearchOption.AllDirectories);

            foreach (string file in files)
            {
                if (file.Contains(".svn\\") == false)
                {
                    string key = System.IO.Path.GetFileName(file);
                    if(kom.subfiles.ContainsKey(key)==false)
                        kom.Add(key, Kom2SubFile.ReadSubFileFromFile(file),true);
                }
            }
            if (System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(filename)) == false)
                System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(filename));
            kom.Save(filename);
            kom.Close();            
            return true;
        }

        static public bool DecompressKom(string filename,string dir)
        {
            if (System.IO.Directory.Exists(dir) == false)
                if (System.IO.Directory.CreateDirectory(dir).Exists == false)
                    return false;

            Komfile kom = new Komfile();
            if (kom.Open(filename) == false)
                return false;

            foreach(System.Collections.Generic.KeyValuePair<string,Kom2SubFile> KeyValue in kom.Subfiles)
            {
                System.IO.FileStream file = new System.IO.FileStream(System.IO.Path.Combine(dir, KeyValue.Key), System.IO.FileMode.Create, System.IO.FileAccess.Write);
                KeyValue.Value.WriteDecompressed(file);
                file.Close();
            }
            kom.Close();
            return true;
        }

        static public EKomType IsKom(string filename)
        {
            System.IO.FileStream stream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            System.IO.BinaryReader reader = new System.IO.BinaryReader(stream);
            byte[] magicbyte = reader.ReadBytes(52);
            string magicstring = System.Text.ASCIIEncoding.ASCII.GetString(magicbyte).Trim('\0');

            EKomType re = EKomType.notkom;
            uint size = reader.ReadUInt32();
            bool compressed = true;
            if (reader.ReadUInt32() == 0)
            {
                compressed = false;
            }

            if(magicstring == "KOG GC TEAM MASSFILE V.0.3.")
            {
                reader.ReadUInt32();
                reader.ReadUInt32();
                UInt32 header_size = reader.ReadUInt32();

                System.Xml.XmlDocument headerxml = new System.Xml.XmlDocument();

                byte[] header_raw = reader.ReadBytes((int)header_size);

                if (header_raw[0] != '<' ||
                    header_raw[1] != '?' ||
                    header_raw[2] != 'x' ||
                    header_raw[3] != 'm' ||
                    header_raw[4] != 'l')
                {
                    re = EKomType.encrypt;
                }
                else
                {
                    re = EKomType.newkom;
                }
            }
            else if(magicstring == "KOG GC TEAM MASSFILE V.0.2." || 
                magicstring == "KOG GC TEAM MASSFILE V.0.1.")
            {
                re = EKomType.oldkom;
            }
            
            stream.Close();
            return re;
        }


        static public bool DecompressKom(string filename, string dir, string keystr)
        {
            if (System.IO.Directory.Exists(dir) == false)
                if (System.IO.Directory.CreateDirectory(dir).Exists == false)
                    return false;

            Komfile kom = new Komfile();
            kom.SetECB(keystr);
            if (kom.Open(filename) == false)
                return false;

            foreach (System.Collections.Generic.KeyValuePair<string, Kom2SubFile> KeyValue in kom.Subfiles)
            {
                System.IO.FileStream file = new System.IO.FileStream(System.IO.Path.Combine(dir, KeyValue.Key), System.IO.FileMode.Create, System.IO.FileAccess.Write);
                KeyValue.Value.WriteDecompressed(file);
                file.Close();
            }
            kom.Close();
            return true;
        }

        EKomType komtype = EKomType.notkom;
        public EKomType KomType
        {
            get { return komtype; }
        }

        const UInt32 version = 3;                
        const UInt32 OffsetStart = 52 + 4 + 4 +4 +4 +4;        

        UInt32 adler32 = 0;
        public UInt32 Adler32 
        {
            get { return adler32; }
        }

        UInt32 header_size = 0;
        public UInt32 HeaderSize
        {
            get { return header_size; }
        }

        UInt32 filetime = 0;
        public UInt32 FileTime
        {
            get { return filetime; }
        }

        System.Collections.Generic.SortedList<string, Kom2SubFile> subfiles = new SortedList<string, Kom2SubFile>(StringComparer.Ordinal);


        public void Close()
        {
            adler32 = 0;
            header_size = 0;
            filetime = 0;

            foreach (System.Collections.Generic.KeyValuePair<string, Kom2SubFile> KeyValue in subfiles)
            {
                KeyValue.Value.Close();
            }

            subfiles.Clear();
            ecb = null;
            komtype = EKomType.notkom;
        }
        
        public bool Open(string filename)
        {
            System.IO.FileStream stream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read);
            System.IO.BinaryReader reader = new System.IO.BinaryReader(stream);
            bool re = true;

            try
            {
                byte[] magicbyte = reader.ReadBytes(52);

                string magicstring = System.Text.ASCIIEncoding.ASCII.GetString(magicbyte).Trim('\0');

                uint size = reader.ReadUInt32();
                bool compressed = true;
                if (reader.ReadUInt32() == 0)
                {
                    compressed = false;
                }

                switch (magicstring)
                {
                    case "KOG GC TEAM MASSFILE V.0.3.":

                        komtype = EKomType.newkom;

                        filetime = reader.ReadUInt32();
                        adler32 = reader.ReadUInt32();
                        header_size = reader.ReadUInt32();

                        System.Xml.XmlDocument headerxml = new System.Xml.XmlDocument();

                        byte[] header_raw = reader.ReadBytes((int)header_size);

                        if (header_raw[0] != '<' ||
                            header_raw[1] != '?' ||
                            header_raw[2] != 'x' ||
                            header_raw[3] != 'm' ||
                            header_raw[4] != 'l')
                        {
                            if (ecb == null)
                                throw new EncryptedKomException();

                            byte[] data = (byte[])header_raw.Clone();
                            ecb.Decrypt(data, 0, header_raw, 0, (int)header_size);
                            komtype = EKomType.encrypt;
                        }
                        string xmlstring = System.Text.ASCIIEncoding.ASCII.GetString(header_raw);
                        headerxml.LoadXml(xmlstring);
                        System.Xml.XmlNodeList files = headerxml.SelectNodes("Files/File");

                        UInt32 offset = OffsetStart + header_size;

                        foreach (System.Xml.XmlElement file in files)
                        {
                            string key = file.GetAttribute("Name");
                            Kom2SubFile subfile = Kom2SubFile.ReadSubFileFromKom(ecb, filename, file, offset);
                            offset += subfile.CompressedSize;
                            subfiles.Add(key, subfile);
                        }
                        break;
                    case "KOG GC TEAM MASSFILE V.0.1.":
                    case "KOG GC TEAM MASSFILE V.0.2.":

                        komtype = EKomType.oldkom;
                        for (int i = 0; i < size; i++)
                        {
                            string key = System.Text.ASCIIEncoding.ASCII.GetString(reader.ReadBytes(60)).Trim('\0');
                            Kom2SubFile subfile = Kom2SubFile.ReadSubFileFromOldKom(filename, reader, 60 + size * 72);
                            if (key != "crc.xml")
                                subfiles.Add(key, subfile);
                        }
                        break;
                    default:
                        re = false;
                        break;
                }
                
            }
            catch (System.Exception ex)
            {
                komtype = EKomType.notkom;
                re = false;
            }	
                    
            stream.Close();
            return re;
        }       
        
        public bool Decrypt()
        {
            if (ecb == null)
                return false;

            foreach (System.Collections.Generic.KeyValuePair<string, Kom2SubFile> KeyValue in subfiles)
            {
               if((Kom2SubFile.AlgorithmType)KeyValue.Value.Algorithm == Kom2SubFile.AlgorithmType.infilateandblowfish) // 인크립트 되있으면 풀자 
               {
                   KeyValue.Value.Convert(ecb, Kom2SubFile.AlgorithmType.infilate);
               }
            }

            ecb = null;
            komtype = EKomType.newkom;
            return true;
        }

        public bool Encrypt()
        {            
            if (ecb == null)
                return false;

            foreach (System.Collections.Generic.KeyValuePair<string, Kom2SubFile> KeyValue in subfiles)
            {
                if ((Kom2SubFile.AlgorithmType)KeyValue.Value.Algorithm == Kom2SubFile.AlgorithmType.infilate) // 인크립트 되있으면 풀자 
                {
                    KeyValue.Value.Convert(ecb, Kom2SubFile.AlgorithmType.infilateandblowfish);
                }
            }            
            komtype = EKomType.encrypt;
            return true;
        }   

        public void Save(string filename)
        {
            string tmpfilename = filename + ".tmp";
            System.IO.FileStream stream = new System.IO.FileStream(tmpfilename, System.IO.FileMode.Create, System.IO.FileAccess.Write);
            System.IO.BinaryWriter writer = new System.IO.BinaryWriter(stream);

            // 형식 정보 쓰자 
            byte[] magicbyte = new byte[52];
            string magicstring = string.Format("KOG GC TEAM MASSFILE V.{0}.{1}.", version / 10, version % 10);


            System.Text.ASCIIEncoding.ASCII.GetBytes(magicstring, 0, magicstring.Length, magicbyte, 0);
            writer.Write(magicbyte);

            filetime = 0;
            foreach (System.Collections.Generic.KeyValuePair<string, Kom2SubFile> KeyValue in subfiles)
            {
                filetime += KeyValue.Value.FileTime;
            }

            // 헤더는 xml 형식

            System.Xml.XmlDocument headerxml = new System.Xml.XmlDocument();
            headerxml.AppendChild(headerxml.CreateNode(System.Xml.XmlNodeType.XmlDeclaration, "", ""));

            System.Xml.XmlElement files = headerxml.CreateElement("Files");
            headerxml.AppendChild(files);

            foreach (System.Collections.Generic.KeyValuePair<string, Kom2SubFile> KeyValue in subfiles)
            {
                System.Xml.XmlElement file = headerxml.CreateElement("File");

                file.SetAttribute("Name", KeyValue.Key);
                KeyValue.Value.WriteHeader(file);
                files.AppendChild(file);
            }

            int header_raw_size = headerxml.InnerXml.Length;
            if (header_raw_size % BlowfishNET.BlowfishECB.BLOCK_SIZE != 0)
                header_raw_size += BlowfishNET.BlowfishECB.BLOCK_SIZE - (header_raw_size % BlowfishNET.BlowfishECB.BLOCK_SIZE);


            byte[] header_raw = new byte[header_raw_size];            

            System.Text.ASCIIEncoding.ASCII.GetBytes(headerxml.InnerXml, 0, headerxml.InnerXml.Length, header_raw,0);       
     
            if(ecb != null)
            {
                byte[] header_encrypt = (byte [])header_raw.Clone();
                int a = ecb.Encrypt(header_encrypt, 0, header_raw, 0, header_raw.Length);
            }
            
            writer.Write((UInt32)subfiles.Count);
            writer.Write((UInt32)1);
            writer.Write(FileTime);// 파일타임 
            writer.Write(AdlerCheckSum.GetAdler32(header_raw, 0, (uint)header_raw.Length)); // 체크섬
            writer.Write((UInt32)header_raw.Length);// 압축 안한 사이즈
            writer.Write(header_raw, 0, (int)header_raw.Length);// 헤더


            foreach (System.Collections.Generic.KeyValuePair<string, Kom2SubFile> KeyValue in subfiles)
            {
                KeyValue.Value.WriteCompressed(writer.BaseStream);
            }

            stream.Close();            
            System.IO.File.Delete(filename);
            System.IO.File.Move(tmpfilename, filename);            
        }


        public bool Add(string filename,Kom2SubFile subfile,bool overwrite)
        {
            if (subfiles.ContainsKey(filename))
            {
                if(overwrite == false)
                    return false;

                subfiles.Remove(filename);
            }
            subfiles.Add(filename, subfile);
            return true;
        }
        
        public System.Collections.Generic.SortedList<string,Kom2SubFile> Subfiles
        {
            get { return subfiles; }
        }

        
        static public bool Equals(Komfile x, Komfile y)
        {
            if (x.Adler32 != y.Adler32 || x.HeaderSize != y.HeaderSize || x.FileTime != y.FileTime || x.Subfiles.Count != y.Subfiles.Count) 
                return false;
            
            foreach(System.Collections.Generic.KeyValuePair<string,Kom2SubFile> KeyValue in x.Subfiles)
            {
                if (y.Subfiles.ContainsKey(KeyValue.Key) == false)
                    return false;

                if (Kom2SubFile.Equals(KeyValue.Value, y.Subfiles[KeyValue.Key]) == false)
                    return false;
            }

            return true;
        }

        
    }
}
