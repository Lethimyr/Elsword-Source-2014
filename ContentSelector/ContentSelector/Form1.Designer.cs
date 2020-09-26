namespace ContentSelector
{
    partial class Form1
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

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.ScriptTreeView = new System.Windows.Forms.TreeView();
            this.LoadButton = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.파일ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SaveMetadataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.LoadMetaDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.정보ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ButtonExport = new System.Windows.Forms.Button();
            this.textBoxCode = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.CheckBoxAddCode = new System.Windows.Forms.CheckBox();
            this.ButtonReload = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.ContentsTab = new System.Windows.Forms.TabPage();
            this.CharacterTab = new System.Windows.Forms.TabPage();
            this.checkedListBoxNewMark = new System.Windows.Forms.CheckedListBox();
            this.checkedListBoxCharacter = new System.Windows.Forms.CheckedListBox();
            this.ButtonCharacterExport = new System.Windows.Forms.Button();
            this.comboBoxEncoding = new System.Windows.Forms.ComboBox();
            this.menuStrip1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.ContentsTab.SuspendLayout();
            this.CharacterTab.SuspendLayout();
            this.SuspendLayout();
            // 
            // ScriptTreeView
            // 
            this.ScriptTreeView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.ScriptTreeView.CheckBoxes = true;
            this.ScriptTreeView.HotTracking = true;
            this.ScriptTreeView.Location = new System.Drawing.Point(0, 0);
            this.ScriptTreeView.Name = "ScriptTreeView";
            this.ScriptTreeView.ShowNodeToolTips = true;
            this.ScriptTreeView.Size = new System.Drawing.Size(1072, 507);
            this.ScriptTreeView.TabIndex = 0;
            this.ScriptTreeView.AfterCheck += new System.Windows.Forms.TreeViewEventHandler(this.treeView1_AfterCheck);
            this.ScriptTreeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.ScriptTreeView_AfterSelect);
            // 
            // LoadButton
            // 
            this.LoadButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.LoadButton.Location = new System.Drawing.Point(12, 565);
            this.LoadButton.Name = "LoadButton";
            this.LoadButton.Size = new System.Drawing.Size(131, 34);
            this.LoadButton.TabIndex = 1;
            this.LoadButton.Text = "SelectResourceDir";
            this.LoadButton.UseVisualStyleBackColor = true;
            this.LoadButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.파일ToolStripMenuItem,
            this.정보ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1078, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.menuStrip1_ItemClicked);
            // 
            // 파일ToolStripMenuItem
            // 
            this.파일ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.SaveMetadataToolStripMenuItem,
            this.LoadMetaDataToolStripMenuItem});
            this.파일ToolStripMenuItem.Name = "파일ToolStripMenuItem";
            this.파일ToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.파일ToolStripMenuItem.Text = "File";
            // 
            // SaveMetadataToolStripMenuItem
            // 
            this.SaveMetadataToolStripMenuItem.Name = "SaveMetadataToolStripMenuItem";
            this.SaveMetadataToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.SaveMetadataToolStripMenuItem.Text = "Save Metadata";
            this.SaveMetadataToolStripMenuItem.Click += new System.EventHandler(this.SaveMetaDataToolStripMenuItem_Click);
            // 
            // LoadMetaDataToolStripMenuItem
            // 
            this.LoadMetaDataToolStripMenuItem.Name = "LoadMetaDataToolStripMenuItem";
            this.LoadMetaDataToolStripMenuItem.Size = new System.Drawing.Size(160, 22);
            this.LoadMetaDataToolStripMenuItem.Text = "Load Metadata";
            this.LoadMetaDataToolStripMenuItem.Click += new System.EventHandler(this.LoadMetaDataToolStripMenuItem_Click);
            // 
            // 정보ToolStripMenuItem
            // 
            this.정보ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.정보ToolStripMenuItem.Name = "정보ToolStripMenuItem";
            this.정보ToolStripMenuItem.Size = new System.Drawing.Size(49, 20);
            this.정보ToolStripMenuItem.Text = "About";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
            this.aboutToolStripMenuItem.Text = "About..";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // ButtonExport
            // 
            this.ButtonExport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ButtonExport.Location = new System.Drawing.Point(728, 565);
            this.ButtonExport.Name = "ButtonExport";
            this.ButtonExport.Size = new System.Drawing.Size(166, 31);
            this.ButtonExport.TabIndex = 3;
            this.ButtonExport.Text = "Export ContentsScriptSet";
            this.ButtonExport.UseVisualStyleBackColor = true;
            this.ButtonExport.Click += new System.EventHandler(this.ButtonExport_Click);
            // 
            // textBoxCode
            // 
            this.textBoxCode.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.textBoxCode.Location = new System.Drawing.Point(450, 573);
            this.textBoxCode.Name = "textBoxCode";
            this.textBoxCode.Size = new System.Drawing.Size(104, 21);
            this.textBoxCode.TabIndex = 4;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(409, 576);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "Code";
            // 
            // CheckBoxAddCode
            // 
            this.CheckBoxAddCode.AutoSize = true;
            this.CheckBoxAddCode.Location = new System.Drawing.Point(560, 572);
            this.CheckBoxAddCode.Name = "CheckBoxAddCode";
            this.CheckBoxAddCode.Size = new System.Drawing.Size(162, 16);
            this.CheckBoxAddCode.TabIndex = 6;
            this.CheckBoxAddCode.Text = "Add Code@ in Filename";
            this.CheckBoxAddCode.UseMnemonic = false;
            this.CheckBoxAddCode.UseVisualStyleBackColor = true;
            // 
            // ButtonReload
            // 
            this.ButtonReload.Location = new System.Drawing.Point(149, 567);
            this.ButtonReload.Name = "ButtonReload";
            this.ButtonReload.Size = new System.Drawing.Size(63, 31);
            this.ButtonReload.TabIndex = 7;
            this.ButtonReload.Text = "Reload";
            this.ButtonReload.UseVisualStyleBackColor = true;
            this.ButtonReload.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.ContentsTab);
            this.tabControl1.Controls.Add(this.CharacterTab);
            this.tabControl1.Location = new System.Drawing.Point(0, 27);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1080, 532);
            this.tabControl1.TabIndex = 8;
            // 
            // ContentsTab
            // 
            this.ContentsTab.Controls.Add(this.ScriptTreeView);
            this.ContentsTab.Location = new System.Drawing.Point(4, 21);
            this.ContentsTab.Name = "ContentsTab";
            this.ContentsTab.Padding = new System.Windows.Forms.Padding(3);
            this.ContentsTab.Size = new System.Drawing.Size(1072, 507);
            this.ContentsTab.TabIndex = 0;
            this.ContentsTab.Text = "Contents";
            this.ContentsTab.UseVisualStyleBackColor = true;
            // 
            // CharacterTab
            // 
            this.CharacterTab.Controls.Add(this.checkedListBoxNewMark);
            this.CharacterTab.Controls.Add(this.checkedListBoxCharacter);
            this.CharacterTab.Location = new System.Drawing.Point(4, 21);
            this.CharacterTab.Name = "CharacterTab";
            this.CharacterTab.Padding = new System.Windows.Forms.Padding(3);
            this.CharacterTab.Size = new System.Drawing.Size(1072, 507);
            this.CharacterTab.TabIndex = 1;
            this.CharacterTab.Text = "Characters";
            this.CharacterTab.UseVisualStyleBackColor = true;
            // 
            // checkedListBoxNewMark
            // 
            this.checkedListBoxNewMark.CheckOnClick = true;
            this.checkedListBoxNewMark.FormattingEnabled = true;
            this.checkedListBoxNewMark.Items.AddRange(new object[] {
            "NewMark_Elsword",
            "NewMark_Aisha",
            "NewMark_Rena",
            "NewMark_Raven",
            "NewMark_Eve"});
            this.checkedListBoxNewMark.Location = new System.Drawing.Point(514, 4);
            this.checkedListBoxNewMark.Name = "checkedListBoxNewMark";
            this.checkedListBoxNewMark.Size = new System.Drawing.Size(557, 500);
            this.checkedListBoxNewMark.TabIndex = 1;
            this.checkedListBoxNewMark.SelectedIndexChanged += new System.EventHandler(this.checkedListBox2_SelectedIndexChanged);
            // 
            // checkedListBoxCharacter
            // 
            this.checkedListBoxCharacter.CheckOnClick = true;
            this.checkedListBoxCharacter.FormattingEnabled = true;
            this.checkedListBoxCharacter.Items.AddRange(new object[] {
            "Elsword",
            "Aisha",
            "Rena",
            "Raven",
            "Eve"});
            this.checkedListBoxCharacter.Location = new System.Drawing.Point(3, 4);
            this.checkedListBoxCharacter.Name = "checkedListBoxCharacter";
            this.checkedListBoxCharacter.Size = new System.Drawing.Size(506, 500);
            this.checkedListBoxCharacter.TabIndex = 0;
            // 
            // ButtonCharacterExport
            // 
            this.ButtonCharacterExport.Location = new System.Drawing.Point(900, 565);
            this.ButtonCharacterExport.Name = "ButtonCharacterExport";
            this.ButtonCharacterExport.Size = new System.Drawing.Size(166, 32);
            this.ButtonCharacterExport.TabIndex = 9;
            this.ButtonCharacterExport.Text = "Export CharacterScript";
            this.ButtonCharacterExport.UseVisualStyleBackColor = true;
            this.ButtonCharacterExport.Click += new System.EventHandler(this.ButtonCharacterExport_Click);
            // 
            // comboBoxEncoding
            // 
            this.comboBoxEncoding.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxEncoding.FormattingEnabled = true;
            this.comboBoxEncoding.ImeMode = System.Windows.Forms.ImeMode.Disable;
            this.comboBoxEncoding.Items.AddRange(new object[] {
            "Default",
            "UTF-8"});
            this.comboBoxEncoding.Location = new System.Drawing.Point(283, 574);
            this.comboBoxEncoding.Name = "comboBoxEncoding";
            this.comboBoxEncoding.Size = new System.Drawing.Size(117, 20);
            this.comboBoxEncoding.TabIndex = 10;
            this.comboBoxEncoding.Text = "Encoding";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1078, 606);
            this.Controls.Add(this.comboBoxEncoding);
            this.Controls.Add(this.ButtonCharacterExport);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.ButtonReload);
            this.Controls.Add(this.CheckBoxAddCode);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBoxCode);
            this.Controls.Add(this.ButtonExport);
            this.Controls.Add(this.LoadButton);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "ScriptManager v0.001";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.ContentsTab.ResumeLayout(false);
            this.CharacterTab.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button LoadButton;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 파일ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SaveMetadataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem LoadMetaDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 정보ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        public System.Windows.Forms.TreeView ScriptTreeView;
        private System.Windows.Forms.Button ButtonExport;
        public System.Windows.Forms.TextBox textBoxCode;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox CheckBoxAddCode;
        private System.Windows.Forms.Button ButtonReload;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage ContentsTab;
        private System.Windows.Forms.TabPage CharacterTab;
        private System.Windows.Forms.Button ButtonCharacterExport;
        public System.Windows.Forms.CheckedListBox checkedListBoxNewMark;
        public System.Windows.Forms.CheckedListBox checkedListBoxCharacter;
        private System.Windows.Forms.ComboBox comboBoxEncoding;
                
    }
}

