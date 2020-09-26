namespace DBManager
{
    partial class StringTable
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
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnHistory = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.checkTW = new System.Windows.Forms.CheckBox();
            this.checkHK = new System.Windows.Forms.CheckBox();
            this.checkJP = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(12, 72);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(638, 281);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dataGridView1_KeyDown);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(12, 359);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(145, 51);
            this.btnSave.TabIndex = 2;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.Save_Click);
            // 
            // btnHistory
            // 
            this.btnHistory.Location = new System.Drawing.Point(163, 359);
            this.btnHistory.Name = "btnHistory";
            this.btnHistory.Size = new System.Drawing.Size(145, 51);
            this.btnHistory.TabIndex = 3;
            this.btnHistory.Text = "History";
            this.btnHistory.UseVisualStyleBackColor = true;
            this.btnHistory.Click += new System.EventHandler(this.btnHistory_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(52, 40);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(95, 21);
            this.textBox1.TabIndex = 5;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "All",
            "Item",
            "QuestTable",
            "TitleTable"});
            this.comboBox1.Location = new System.Drawing.Point(311, 10);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(101, 20);
            this.comboBox1.TabIndex = 6;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // checkTW
            // 
            this.checkTW.AutoSize = true;
            this.checkTW.Location = new System.Drawing.Point(421, 12);
            this.checkTW.Name = "checkTW";
            this.checkTW.Size = new System.Drawing.Size(42, 16);
            this.checkTW.TabIndex = 7;
            this.checkTW.Text = "TW";
            this.checkTW.UseVisualStyleBackColor = true;
            this.checkTW.CheckedChanged += new System.EventHandler(this.checkTW_CheckedChanged);
            // 
            // checkHK
            // 
            this.checkHK.AutoSize = true;
            this.checkHK.Location = new System.Drawing.Point(498, 12);
            this.checkHK.Name = "checkHK";
            this.checkHK.Size = new System.Drawing.Size(40, 16);
            this.checkHK.TabIndex = 8;
            this.checkHK.Text = "HK";
            this.checkHK.UseVisualStyleBackColor = true;
            this.checkHK.CheckedChanged += new System.EventHandler(this.checkHK_CheckedChanged);
            // 
            // checkJP
            // 
            this.checkJP.AutoSize = true;
            this.checkJP.Location = new System.Drawing.Point(573, 12);
            this.checkJP.Name = "checkJP";
            this.checkJP.Size = new System.Drawing.Size(38, 16);
            this.checkJP.TabIndex = 9;
            this.checkJP.Text = "JP";
            this.checkJP.UseVisualStyleBackColor = true;
            this.checkJP.CheckedChanged += new System.EventHandler(this.checkJP_CheckedChanged);
            // 
            // StringTable
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(662, 424);
            this.Controls.Add(this.checkJP);
            this.Controls.Add(this.checkHK);
            this.Controls.Add(this.checkTW);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.btnHistory);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.dataGridView1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "StringTable";
            this.Text = "Release";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.StringTable_FormClosed);
            this.Resize += new System.EventHandler(this.StringTable_Resize);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.StringTable_FormClosing);
            this.Load += new System.EventHandler(this.StringTable_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnHistory;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.CheckBox checkTW;
        private System.Windows.Forms.CheckBox checkHK;
        private System.Windows.Forms.CheckBox checkJP;
    }
}