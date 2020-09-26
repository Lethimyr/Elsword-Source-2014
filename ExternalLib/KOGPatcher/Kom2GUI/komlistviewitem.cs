using System;
using System.Collections.Generic;
using System.Text;

namespace Kom2GUI
{
    class KonmListViewItem:System.Windows.Forms.ListViewItem
    {
        Kom2.NET.Kom2SubFile subfile = null;

        public KonmListViewItem(string text,Kom2.NET.Kom2SubFile sub)
        {           
            Subfile = sub;
            Text = text;
        }

        Kom2.NET.Kom2SubFile Subfile
        {
            get { return subfile; }
            set
            {
                if(value != null)
                {
                    SubItems.Clear();
                    SubItems.Add(value.CompressedSize.ToString());
                    SubItems.Add(value.Size.ToString());
                    SubItems.Add(string.Format("{0}%", (value.Size-value.CompressedSize) * 100 / value.Size));
                    SubItems.Add(string.Format("{0:x8}", value.Adler32));
                    SubItems.Add(string.Format("{0:x8}", value.FileTime));
                }
                subfile = value;
            }
        }

    }
}
