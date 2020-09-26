namespace DBManager
{
    partial class Search
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
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.SearchText = new System.Windows.Forms.Label();
            this.btnNextSearch = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(95, 21);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(353, 21);
            this.textBox1.TabIndex = 0;
            this.textBox1.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBox1_KeyPress);
            // 
            // SearchText
            // 
            this.SearchText.AutoSize = true;
            this.SearchText.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.SearchText.Location = new System.Drawing.Point(16, 26);
            this.SearchText.Name = "SearchText";
            this.SearchText.Size = new System.Drawing.Size(79, 13);
            this.SearchText.TabIndex = 1;
            this.SearchText.Text = "찾을 내용 :  ";
            // 
            // btnNextSearch
            // 
            this.btnNextSearch.Location = new System.Drawing.Point(202, 112);
            this.btnNextSearch.Name = "btnNextSearch";
            this.btnNextSearch.Size = new System.Drawing.Size(120, 27);
            this.btnNextSearch.TabIndex = 2;
            this.btnNextSearch.Text = "다음 찾기";
            this.btnNextSearch.UseVisualStyleBackColor = true;
            this.btnNextSearch.Click += new System.EventHandler(this.btnNextSearch_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(328, 112);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(120, 27);
            this.button2.TabIndex = 3;
            this.button2.Text = "취소";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Search
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(468, 151);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.btnNextSearch);
            this.Controls.Add(this.SearchText);
            this.Controls.Add(this.textBox1);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Search";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "찾기";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Label SearchText;
        private System.Windows.Forms.Button btnNextSearch;
        private System.Windows.Forms.Button button2;
    }
}