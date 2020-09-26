namespace DBManager
{
    partial class BillingTable
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
            this.components = new System.ComponentModel.Container();
            this.btnCategoryAdd = new System.Windows.Forms.Button();
            this.btnCategorySave = new System.Windows.Forms.Button();
            this.btnCategoryHistory = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSearch = new System.Windows.Forms.Button();
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.contextMenuStrip2 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.btnReplace = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOK = new System.Windows.Forms.Button();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.dataGridView3 = new System.Windows.Forms.DataGridView();
            this.btnInfoAdd = new System.Windows.Forms.Button();
            this.btnInfoHistory = new System.Windows.Forms.Button();
            this.btnInfoSave = new System.Windows.Forms.Button();
            this.btnAttributeHistory = new System.Windows.Forms.Button();
            this.btnAttributeAdd = new System.Windows.Forms.Button();
            this.btnAttributeSave = new System.Windows.Forms.Button();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView3)).BeginInit();
            this.SuspendLayout();
            // 
            // btnCategoryAdd
            // 
            this.btnCategoryAdd.Location = new System.Drawing.Point(864, 241);
            this.btnCategoryAdd.Name = "btnCategoryAdd";
            this.btnCategoryAdd.Size = new System.Drawing.Size(125, 52);
            this.btnCategoryAdd.TabIndex = 1;
            this.btnCategoryAdd.Text = "Add";
            this.btnCategoryAdd.UseVisualStyleBackColor = true;
            this.btnCategoryAdd.Click += new System.EventHandler(this.btnCategoryAdd_Click);
            // 
            // btnCategorySave
            // 
            this.btnCategorySave.Location = new System.Drawing.Point(995, 240);
            this.btnCategorySave.Name = "btnCategorySave";
            this.btnCategorySave.Size = new System.Drawing.Size(125, 52);
            this.btnCategorySave.TabIndex = 1;
            this.btnCategorySave.Text = "Save";
            this.btnCategorySave.UseVisualStyleBackColor = true;
            this.btnCategorySave.Click += new System.EventHandler(this.btnCategorySave_Click);
            // 
            // btnCategoryHistory
            // 
            this.btnCategoryHistory.Location = new System.Drawing.Point(1123, 240);
            this.btnCategoryHistory.Name = "btnCategoryHistory";
            this.btnCategoryHistory.Size = new System.Drawing.Size(125, 52);
            this.btnCategoryHistory.TabIndex = 1;
            this.btnCategoryHistory.Text = "History";
            this.btnCategoryHistory.UseVisualStyleBackColor = true;
            this.btnCategoryHistory.Click += new System.EventHandler(this.btnCategoryHistory_Click);
            // 
            // textBox1
            // 
            this.textBox1.Enabled = false;
            this.textBox1.Location = new System.Drawing.Point(50, 10);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(138, 21);
            this.textBox1.TabIndex = 3;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(195, 10);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(138, 21);
            this.textBox2.TabIndex = 3;
            this.textBox2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBox2_KeyDown);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "검색";
            // 
            // btnSearch
            // 
            this.btnSearch.Location = new System.Drawing.Point(343, 8);
            this.btnSearch.Name = "btnSearch";
            this.btnSearch.Size = new System.Drawing.Size(63, 24);
            this.btnSearch.TabIndex = 5;
            this.btnSearch.Text = "검색";
            this.btnSearch.UseVisualStyleBackColor = true;
            this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.ShowImageMargin = false;
            this.contextMenuStrip1.Size = new System.Drawing.Size(36, 4);
            // 
            // contextMenuStrip2
            // 
            this.contextMenuStrip2.Name = "contextMenuStrip2";
            this.contextMenuStrip2.ShowImageMargin = false;
            this.contextMenuStrip2.Size = new System.Drawing.Size(36, 4);
            // 
            // btnReplace
            // 
            this.btnReplace.Location = new System.Drawing.Point(489, 10);
            this.btnReplace.Name = "btnReplace";
            this.btnReplace.Size = new System.Drawing.Size(60, 23);
            this.btnReplace.TabIndex = 7;
            this.btnReplace.Text = "Replace";
            this.btnReplace.UseVisualStyleBackColor = true;
            this.btnReplace.Click += new System.EventHandler(this.btnReplace_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 12);
            this.label2.TabIndex = 0;
            this.label2.Text = "찾을 내용";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 59);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 12);
            this.label3.TabIndex = 0;
            this.label3.Text = "바꿀 내용";
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.btnCancel);
            this.panel1.Controls.Add(this.btnOK);
            this.panel1.Controls.Add(this.textBox4);
            this.panel1.Controls.Add(this.textBox3);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Location = new System.Drawing.Point(489, 30);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(392, 130);
            this.panel1.TabIndex = 8;
            this.panel1.Visible = false;
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(295, 91);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 4;
            this.btnCancel.Text = "취소";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(200, 91);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 3;
            this.btnOK.Text = "실행";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(87, 55);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(283, 21);
            this.textBox4.TabIndex = 2;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(87, 16);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(283, 21);
            this.textBox3.TabIndex = 1;
            // 
            // toolTip1
            // 
            this.toolTip1.AutomaticDelay = 1000;
            this.toolTip1.IsBalloon = true;
            // 
            // dataGridView1
            // 
            this.dataGridView1.ClipboardCopyMode = System.Windows.Forms.DataGridViewClipboardCopyMode.EnableWithoutHeaderText;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.dataGridView1.Location = new System.Drawing.Point(17, 47);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.RowTemplate.Height = 23;
            this.dataGridView1.Size = new System.Drawing.Size(1231, 187);
            this.dataGridView1.TabIndex = 10;
            this.dataGridView1.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView1_CellBeginEdit);
            this.dataGridView1.Enter += new System.EventHandler(this.dataGridView1_Enter);
            this.dataGridView1.ColumnHeaderMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView1_ColumnHeaderMouseClick);
            this.dataGridView1.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellEndEdit);
            // 
            // dataGridView2
            // 
            this.dataGridView2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView2.Location = new System.Drawing.Point(17, 307);
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.RowTemplate.Height = 23;
            this.dataGridView2.Size = new System.Drawing.Size(888, 475);
            this.dataGridView2.TabIndex = 11;
            this.dataGridView2.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView2_CellBeginEdit);
            this.dataGridView2.Enter += new System.EventHandler(this.dataGridView2_Enter);
            this.dataGridView2.ColumnHeaderMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView2_ColumnHeaderMouseClick);
            this.dataGridView2.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellEndEdit);
            // 
            // dataGridView3
            // 
            this.dataGridView3.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView3.Location = new System.Drawing.Point(946, 307);
            this.dataGridView3.Name = "dataGridView3";
            this.dataGridView3.RowTemplate.Height = 23;
            this.dataGridView3.Size = new System.Drawing.Size(302, 475);
            this.dataGridView3.TabIndex = 12;
            this.dataGridView3.CellBeginEdit += new System.Windows.Forms.DataGridViewCellCancelEventHandler(this.dataGridView3_CellBeginEdit);
            this.dataGridView3.Enter += new System.EventHandler(this.dataGridView3_Enter);
            this.dataGridView3.ColumnHeaderMouseClick += new System.Windows.Forms.DataGridViewCellMouseEventHandler(this.dataGridView3_ColumnHeaderMouseClick);
            this.dataGridView3.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView3_CellEndEdit);
            // 
            // btnInfoAdd
            // 
            this.btnInfoAdd.Location = new System.Drawing.Point(518, 788);
            this.btnInfoAdd.Name = "btnInfoAdd";
            this.btnInfoAdd.Size = new System.Drawing.Size(125, 52);
            this.btnInfoAdd.TabIndex = 1;
            this.btnInfoAdd.Text = "Add";
            this.btnInfoAdd.UseVisualStyleBackColor = true;
            this.btnInfoAdd.Click += new System.EventHandler(this.btnInfoAdd_Click);
            // 
            // btnInfoHistory
            // 
            this.btnInfoHistory.Location = new System.Drawing.Point(780, 788);
            this.btnInfoHistory.Name = "btnInfoHistory";
            this.btnInfoHistory.Size = new System.Drawing.Size(125, 52);
            this.btnInfoHistory.TabIndex = 1;
            this.btnInfoHistory.Text = "History";
            this.btnInfoHistory.UseVisualStyleBackColor = true;
            this.btnInfoHistory.Click += new System.EventHandler(this.btnInfoHistory_Click);
            // 
            // btnInfoSave
            // 
            this.btnInfoSave.Location = new System.Drawing.Point(649, 788);
            this.btnInfoSave.Name = "btnInfoSave";
            this.btnInfoSave.Size = new System.Drawing.Size(125, 52);
            this.btnInfoSave.TabIndex = 1;
            this.btnInfoSave.Text = "Save";
            this.btnInfoSave.UseVisualStyleBackColor = true;
            this.btnInfoSave.Click += new System.EventHandler(this.btnInfoSave_Click);
            // 
            // btnAttributeHistory
            // 
            this.btnAttributeHistory.Location = new System.Drawing.Point(1178, 788);
            this.btnAttributeHistory.Name = "btnAttributeHistory";
            this.btnAttributeHistory.Size = new System.Drawing.Size(70, 52);
            this.btnAttributeHistory.TabIndex = 1;
            this.btnAttributeHistory.Text = "History";
            this.btnAttributeHistory.UseVisualStyleBackColor = true;
            this.btnAttributeHistory.Click += new System.EventHandler(this.btnAttributeHistory_Click);
            // 
            // btnAttributeAdd
            // 
            this.btnAttributeAdd.Location = new System.Drawing.Point(1026, 788);
            this.btnAttributeAdd.Name = "btnAttributeAdd";
            this.btnAttributeAdd.Size = new System.Drawing.Size(70, 52);
            this.btnAttributeAdd.TabIndex = 1;
            this.btnAttributeAdd.Text = "Add";
            this.btnAttributeAdd.UseVisualStyleBackColor = true;
            this.btnAttributeAdd.Click += new System.EventHandler(this.btnAttributeAdd_Click);
            // 
            // btnAttributeSave
            // 
            this.btnAttributeSave.Location = new System.Drawing.Point(1102, 788);
            this.btnAttributeSave.Name = "btnAttributeSave";
            this.btnAttributeSave.Size = new System.Drawing.Size(70, 52);
            this.btnAttributeSave.TabIndex = 1;
            this.btnAttributeSave.Text = "Save";
            this.btnAttributeSave.UseVisualStyleBackColor = true;
            this.btnAttributeSave.Click += new System.EventHandler(this.btnAttributeSave_Click);
            // 
            // checkBox1
            // 
            this.checkBox1.AutoSize = true;
            this.checkBox1.Location = new System.Drawing.Point(418, 12);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(46, 16);
            this.checkBox1.TabIndex = 6;
            this.checkBox1.Text = "Sort";
            this.checkBox1.UseVisualStyleBackColor = true;
            this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // BillingTable
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1272, 866);
            this.Controls.Add(this.dataGridView2);
            this.Controls.Add(this.checkBox1);
            this.Controls.Add(this.dataGridView3);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.btnSearch);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.btnReplace);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.btnCategorySave);
            this.Controls.Add(this.btnCategoryAdd);
            this.Controls.Add(this.btnCategoryHistory);
            this.Controls.Add(this.btnAttributeHistory);
            this.Controls.Add(this.btnInfoSave);
            this.Controls.Add(this.btnAttributeSave);
            this.Controls.Add(this.btnInfoAdd);
            this.Controls.Add(this.btnAttributeAdd);
            this.Controls.Add(this.btnInfoHistory);
            this.KeyPreview = true;
            this.Name = "BillingTable";
            this.Text = "BillingTable";
            this.Load += new System.EventHandler(this.BillingTable_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.BillingTable_FormClosed);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.BillingTable_FormClosing);
            this.Resize += new System.EventHandler(this.BillingTable_Resize);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.BillingTable_KeyDown);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView3)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnCategoryAdd;
        private System.Windows.Forms.Button btnCategorySave;
        private System.Windows.Forms.Button btnCategoryHistory;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnSearch;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip2;
        private System.Windows.Forms.Button btnReplace;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox3;
        public System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridView dataGridView2;
        private System.Windows.Forms.DataGridView dataGridView3;
        private System.Windows.Forms.Button btnInfoAdd;
        private System.Windows.Forms.Button btnInfoHistory;
        private System.Windows.Forms.Button btnInfoSave;
        private System.Windows.Forms.Button btnAttributeHistory;
        private System.Windows.Forms.Button btnAttributeAdd;
        private System.Windows.Forms.Button btnAttributeSave;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}