namespace DBManager
{
    partial class SelectTable
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
            this.btnExpTable = new System.Windows.Forms.Button();
            this.btnStatTable = new System.Windows.Forms.Button();
            this.btnItemTable = new System.Windows.Forms.Button();
            this.btnQuestTable = new System.Windows.Forms.Button();
            this.btnTitleTable = new System.Windows.Forms.Button();
            this.btnSkillTable = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnExpTable
            // 
            this.btnExpTable.Location = new System.Drawing.Point(29, 26);
            this.btnExpTable.Name = "btnExpTable";
            this.btnExpTable.Size = new System.Drawing.Size(115, 50);
            this.btnExpTable.TabIndex = 0;
            this.btnExpTable.Text = "ExpTable";
            this.btnExpTable.UseVisualStyleBackColor = true;
            this.btnExpTable.Click += new System.EventHandler(this.btnExpTable_Click);
            // 
            // btnStatTable
            // 
            this.btnStatTable.Location = new System.Drawing.Point(168, 26);
            this.btnStatTable.Name = "btnStatTable";
            this.btnStatTable.Size = new System.Drawing.Size(115, 50);
            this.btnStatTable.TabIndex = 1;
            this.btnStatTable.Text = "StatTable";
            this.btnStatTable.UseVisualStyleBackColor = true;
            this.btnStatTable.Click += new System.EventHandler(this.btnStatTable_Click);
            // 
            // btnItemTable
            // 
            this.btnItemTable.Location = new System.Drawing.Point(29, 107);
            this.btnItemTable.Name = "btnItemTable";
            this.btnItemTable.Size = new System.Drawing.Size(115, 50);
            this.btnItemTable.TabIndex = 2;
            this.btnItemTable.Text = "ItemTable";
            this.btnItemTable.UseVisualStyleBackColor = true;
            this.btnItemTable.Click += new System.EventHandler(this.btnItemTable_Click);
            // 
            // btnQuestTable
            // 
            this.btnQuestTable.Location = new System.Drawing.Point(168, 107);
            this.btnQuestTable.Name = "btnQuestTable";
            this.btnQuestTable.Size = new System.Drawing.Size(115, 50);
            this.btnQuestTable.TabIndex = 3;
            this.btnQuestTable.Text = "QuestTable";
            this.btnQuestTable.UseVisualStyleBackColor = true;
            this.btnQuestTable.Click += new System.EventHandler(this.btnQuestTable_Click);
            // 
            // btnTitleTable
            // 
            this.btnTitleTable.Location = new System.Drawing.Point(29, 188);
            this.btnTitleTable.Name = "btnTitleTable";
            this.btnTitleTable.Size = new System.Drawing.Size(115, 50);
            this.btnTitleTable.TabIndex = 3;
            this.btnTitleTable.Text = "TitleTable";
            this.btnTitleTable.UseVisualStyleBackColor = true;
            this.btnTitleTable.Click += new System.EventHandler(this.btnTitleTable_Click);
            // 
            // btnSkillTable
            // 
            this.btnSkillTable.Location = new System.Drawing.Point(168, 188);
            this.btnSkillTable.Name = "btnSkillTable";
            this.btnSkillTable.Size = new System.Drawing.Size(115, 50);
            this.btnSkillTable.TabIndex = 4;
            this.btnSkillTable.Text = "SkillTable";
            this.btnSkillTable.UseVisualStyleBackColor = true;
            this.btnSkillTable.Click += new System.EventHandler(this.btnSkillTable_Click);
            // 
            // SelectTable
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(484, 316);
            this.Controls.Add(this.btnSkillTable);
            this.Controls.Add(this.btnTitleTable);
            this.Controls.Add(this.btnQuestTable);
            this.Controls.Add(this.btnItemTable);
            this.Controls.Add(this.btnStatTable);
            this.Controls.Add(this.btnExpTable);
            this.Name = "SelectTable";
            this.Text = "SelectTable";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.SelectTable_FormClosed);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnExpTable;
        private System.Windows.Forms.Button btnStatTable;
        private System.Windows.Forms.Button btnItemTable;
        private System.Windows.Forms.Button btnQuestTable;
        private System.Windows.Forms.Button btnTitleTable;
        private System.Windows.Forms.Button btnSkillTable;
    }
}