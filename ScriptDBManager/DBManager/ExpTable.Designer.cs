namespace DBManager
{
    partial class ExpTable
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
            this.Save = new System.Windows.Forms.Button();
            this.FileGenerate = new System.Windows.Forms.Button();
            this.Add = new System.Windows.Forms.Button();
            this.btnHistory = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();
            // 
            // dataGridView1
            // 
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Location = new System.Drawing.Point(12, 19);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(638, 299);
            this.dataGridView1.TabIndex = 0;
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dataGridView1_KeyDown);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(180, 324);
            this.Save.Name = "Save";
            this.Save.Size = new System.Drawing.Size(145, 51);
            this.Save.TabIndex = 2;
            this.Save.Text = "Save";
            this.Save.UseVisualStyleBackColor = true;
            this.Save.Click += new System.EventHandler(this.Save_Click);
            // 
            // FileGenerate
            // 
            this.FileGenerate.Location = new System.Drawing.Point(343, 324);
            this.FileGenerate.Name = "FileGenerate";
            this.FileGenerate.Size = new System.Drawing.Size(145, 51);
            this.FileGenerate.TabIndex = 3;
            this.FileGenerate.Text = "파일 생성";
            this.FileGenerate.UseVisualStyleBackColor = true;
            this.FileGenerate.Click += new System.EventHandler(this.FileGenerate_Click);
            // 
            // Add
            // 
            this.Add.Location = new System.Drawing.Point(12, 324);
            this.Add.Name = "Add";
            this.Add.Size = new System.Drawing.Size(145, 51);
            this.Add.TabIndex = 4;
            this.Add.Text = "Add";
            this.Add.UseVisualStyleBackColor = true;
            this.Add.Click += new System.EventHandler(this.Add_Click);
            // 
            // btnHistory
            // 
            this.btnHistory.Location = new System.Drawing.Point(505, 324);
            this.btnHistory.Name = "btnHistory";
            this.btnHistory.Size = new System.Drawing.Size(145, 51);
            this.btnHistory.TabIndex = 3;
            this.btnHistory.Text = "History";
            this.btnHistory.UseVisualStyleBackColor = true;
            this.btnHistory.Click += new System.EventHandler(this.btnHistory_Click);
            // 
            // ExpTable
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(662, 387);
            this.Controls.Add(this.Add);
            this.Controls.Add(this.btnHistory);
            this.Controls.Add(this.FileGenerate);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.dataGridView1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ExpTable";
            this.Text = "ExpTable";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ExpTable_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ExpTable_FormClosing);
            this.Load += new System.EventHandler(this.ExpTable_Load);
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.Button FileGenerate;
        private System.Windows.Forms.Button Add;
        private System.Windows.Forms.Button btnHistory;
    }
}