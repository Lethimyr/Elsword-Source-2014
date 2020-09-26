namespace Kom2GUI
{
    partial class KomView
    {
        /// <summary> 
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 구성 요소 디자이너에서 생성한 코드

        /// <summary> 
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.listView1 = new System.Windows.Forms.ListView();
            this.colHeader_Name = new System.Windows.Forms.ColumnHeader();
            this.colHeader_CompressedSize = new System.Windows.Forms.ColumnHeader();
            this.colHeader_Size = new System.Windows.Forms.ColumnHeader();
            this.colHeader_CompressedRate = new System.Windows.Forms.ColumnHeader();
            this.colHeader_Adler32 = new System.Windows.Forms.ColumnHeader();
            this.colHeader_Filetime = new System.Windows.Forms.ColumnHeader();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.모두ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.압축풀기ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.삭제ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.압축풀기ToolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.삭제ToolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.파일추가ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.contextMenuStrip1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.panel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // listView1
            // 
            this.listView1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colHeader_Name,
            this.colHeader_CompressedSize,
            this.colHeader_Size,
            this.colHeader_CompressedRate,
            this.colHeader_Adler32,
            this.colHeader_Filetime});
            this.listView1.ContextMenuStrip = this.contextMenuStrip1;
            this.listView1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listView1.FullRowSelect = true;
            this.listView1.GridLines = true;
            this.listView1.Location = new System.Drawing.Point(0, 0);
            this.listView1.Name = "listView1";
            this.listView1.RightToLeftLayout = true;
            this.listView1.Size = new System.Drawing.Size(755, 453);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // colHeader_Name
            // 
            this.colHeader_Name.Text = "이름";
            this.colHeader_Name.Width = 276;
            // 
            // colHeader_CompressedSize
            // 
            this.colHeader_CompressedSize.Text = "압축 크기";
            this.colHeader_CompressedSize.Width = 83;
            // 
            // colHeader_Size
            // 
            this.colHeader_Size.Text = "실제 크기";
            this.colHeader_Size.Width = 71;
            // 
            // colHeader_CompressedRate
            // 
            this.colHeader_CompressedRate.Text = "압축률";
            this.colHeader_CompressedRate.Width = 66;
            // 
            // colHeader_Adler32
            // 
            this.colHeader_Adler32.Text = "체크섬";
            this.colHeader_Adler32.Width = 68;
            // 
            // colHeader_Filetime
            // 
            this.colHeader_Filetime.Text = "파일타임";
            this.colHeader_Filetime.Width = 74;
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.모두ToolStripMenuItem,
            this.toolStripMenuItem1,
            this.압축풀기ToolStripMenuItem2,
            this.삭제ToolStripMenuItem2,
            this.파일추가ToolStripMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(153, 120);
            this.contextMenuStrip1.Opening += new System.ComponentModel.CancelEventHandler(this.contextMenuStrip1_Opening);
            // 
            // 모두ToolStripMenuItem
            // 
            this.모두ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.압축풀기ToolStripMenuItem,
            this.삭제ToolStripMenuItem});
            this.모두ToolStripMenuItem.Name = "모두ToolStripMenuItem";
            this.모두ToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.모두ToolStripMenuItem.Text = "모두";
            // 
            // 압축풀기ToolStripMenuItem
            // 
            this.압축풀기ToolStripMenuItem.Name = "압축풀기ToolStripMenuItem";
            this.압축풀기ToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.압축풀기ToolStripMenuItem.Text = "압축 풀기";
            this.압축풀기ToolStripMenuItem.Click += new System.EventHandler(this.압축풀기ToolStripMenuItem_Click);
            // 
            // 삭제ToolStripMenuItem
            // 
            this.삭제ToolStripMenuItem.Name = "삭제ToolStripMenuItem";
            this.삭제ToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.삭제ToolStripMenuItem.Text = "삭제";
            this.삭제ToolStripMenuItem.Click += new System.EventHandler(this.삭제ToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(123, 6);
            // 
            // 압축풀기ToolStripMenuItem2
            // 
            this.압축풀기ToolStripMenuItem2.Name = "압축풀기ToolStripMenuItem2";
            this.압축풀기ToolStripMenuItem2.Size = new System.Drawing.Size(126, 22);
            this.압축풀기ToolStripMenuItem2.Text = "압축 풀기";
            this.압축풀기ToolStripMenuItem2.Click += new System.EventHandler(this.압축풀기ToolStripMenuItem2_Click);
            // 
            // 삭제ToolStripMenuItem2
            // 
            this.삭제ToolStripMenuItem2.Name = "삭제ToolStripMenuItem2";
            this.삭제ToolStripMenuItem2.Size = new System.Drawing.Size(126, 22);
            this.삭제ToolStripMenuItem2.Text = "삭제";
            this.삭제ToolStripMenuItem2.Click += new System.EventHandler(this.삭제ToolStripMenuItem2_Click);
            // 
            // 파일추가ToolStripMenuItem
            // 
            this.파일추가ToolStripMenuItem.Name = "파일추가ToolStripMenuItem";
            this.파일추가ToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
            this.파일추가ToolStripMenuItem.Text = "파일 추가";
            // 
            // textBox1
            // 
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox1.Location = new System.Drawing.Point(0, 0);
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(685, 14);
            this.textBox1.TabIndex = 3;
            this.textBox1.TabStop = false;
            // 
            // checkBox1
            // 
            this.checkBox1.Dock = System.Windows.Forms.DockStyle.Left;
            this.checkBox1.Location = new System.Drawing.Point(0, 0);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(70, 22);
            this.checkBox1.TabIndex = 1;
            this.checkBox1.Text = "암호화";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.panel3);
            this.panel2.Controls.Add(this.checkBox1);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(755, 22);
            this.panel2.TabIndex = 5;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.listView1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 22);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(755, 453);
            this.panel1.TabIndex = 6;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.textBox1);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel3.Location = new System.Drawing.Point(70, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(685, 22);
            this.panel3.TabIndex = 4;
            // 
            // KomView
            // 
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.panel2);
            this.Name = "KomView";
            this.Size = new System.Drawing.Size(755, 475);
            this.contextMenuStrip1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.ColumnHeader colHeader_Name;
        private System.Windows.Forms.ColumnHeader colHeader_CompressedSize;
        private System.Windows.Forms.ColumnHeader colHeader_Size;
        private System.Windows.Forms.ColumnHeader colHeader_Adler32;
        private System.Windows.Forms.ColumnHeader colHeader_Filetime;
        private System.Windows.Forms.ColumnHeader colHeader_CompressedRate;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 모두ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 압축풀기ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 삭제ToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem 파일추가ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 압축풀기ToolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem 삭제ToolStripMenuItem2;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel3;
    }
}
