using System;
using System.Collections.Generic;
using System.Text;

namespace TestConsole
{
    class Program
    {
        static void Main(string[] args)
        {
            ESBuildTask.ExportPatchTask t = new ESBuildTask.ExportPatchTask();
            t.SrcURL="http://116.120.238.51:8080/svn/patch/KR/Client/OpenTest/patch";
            t.DestDir = "d:\\TEST";
            t.OldRev = "0";
            t.NewRev = "402";
            t.Test();

            //ESBuildTask.ExportPatchTask t = new ESBuildTask.ExportPatchTask();
            //t.ConvertLowerCase(@"D:\elsword\test");
        }
    }
}
