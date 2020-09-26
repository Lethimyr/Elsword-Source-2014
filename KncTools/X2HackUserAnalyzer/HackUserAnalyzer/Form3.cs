using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace HackUserReport
{
	/// <summary>
	/// Form3에 대한 요약 설명입니다.
	/// </summary>
	public class Form3 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.MenuItem menuItem1;

		// 윈폼 객체
		Form1 form1 = new Form1();
		Form2 form2 = new Form2();
		private System.Windows.Forms.TabPage tabPage1;
		Form4 form4 = new Form4();

		[STAThread]
		static void Main()
		{
			Application.Run( new Form3() );
		}

		public Form3()
		{			
			// Windows Form 디자이너 지원에 필요합니다.			
			InitializeComponent();

			// HackUser 폴더 생성
			if( System.IO.Directory.Exists( "HackUser" ) == false )
				System.IO.Directory.CreateDirectory( "HackUser" );

			// report 폴더 생성
			if( System.IO.Directory.Exists( "report" ) == false )
				System.IO.Directory.CreateDirectory( "report" );
		}

		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( components != null )
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form 디자이너에서 생성한 코드
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		private void InitializeComponent()
		{
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.tabControl1.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.tabPage1);
			this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl1.Location = new System.Drawing.Point(0, 0);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(1112, 585);
			this.tabControl1.TabIndex = 0;
			// 
			// tabPage1
			// 
			this.tabPage1.Location = new System.Drawing.Point(4, 21);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Size = new System.Drawing.Size(1104, 560);
			this.tabPage1.TabIndex = 0;
			this.tabPage1.Text = "tabPage1";
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.menuItem1});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.Text = "업데이트";
			this.menuItem1.Click += new System.EventHandler(this.menuItem1_Click);
			// 
			// Form3
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(1112, 585);
			this.Controls.Add(this.tabControl1);
			this.Menu = this.mainMenu1;
			this.Name = "Form3";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "핵유저 분석기";
			this.Load += new System.EventHandler(this.Form3_Load);
			this.tabControl1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion


		//bool updating=false;




		void UpdateParing()
		{
			lock( form4 )
			{
				form4.DataParsing();
				form4.ShowDialog();
			}
		}

		void UpdateTab()
		{
			tabControl1.TabPages.Clear();
			string [] dirs = System.IO.Directory.GetDirectories( "HackUser\\", "*" );
			foreach ( string dir in dirs )
			{
                //string []str = folder.FolderPath.Split('\\');
				string []str = dir.Split('\\');
				string version = str[str.Length-1];

				System.Windows.Forms.TabPage tab = new TabPage( version );

				AxSHDocVw.AxWebBrowser web = new AxSHDocVw.AxWebBrowser();

				System.Object nullObject = 0;
								
				tab.Controls.Add( web );
				tabControl1.TabPages.Add( tab );
				
				
				web.Dock = DockStyle.Fill;

				try
				{
					web.Navigate( "file://"+Application.StartupPath+"\\report\\"+version+"_dmp.html", ref nullObject, ref nullObject, ref nullObject, ref nullObject );
				}
				catch ( System.Exception e )
				{
					e.ToString();
				}
			}
		}

	
		private void Form3_Load( object sender, System.EventArgs e )
		{
			//UpdateParing();
			UpdateTab();
		}

		private void menuItem1_Click( object sender, System.EventArgs e )
		{
			// e메일 트리 오픈
			form1.UpdateOulookFolder();
			if( DialogResult.OK == form1.ShowDialog() )
			{
				// e메일 업데이트 - 크래쉬 리포트 검색 및 정리
				form2.UpdateMailList( form1.GetSelectFolder() );
				form2.ShowDialog();

				// 정리된 데이터 분석
				UpdateParing();
				UpdateTab();
			}
		}
	}
}
