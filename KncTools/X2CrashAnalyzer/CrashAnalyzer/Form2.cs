using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Outlook = Microsoft.Office.Interop.Outlook;


namespace CrashReport
{
	/// <summary>
	/// Form2에 대한 요약 설명입니다.
	/// </summary>
	public class Form2 : System.Windows.Forms.Form
	{
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form2()
		{			
			// Windows Form 디자이너 지원에 필요합니다.		
			InitializeComponent();
			
			// TODO: InitializeComponent를 호출한 다음 생성자 코드를 추가합니다.
		}

		static public Hashtable parselist  = new Hashtable();

		private System.Windows.Forms.ProgressBar progressBar1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ProgressBar progressBar2;

		Outlook.MAPIFolder folder;
		private System.Windows.Forms.Label label3;

		public string versionfilter=null;

		// 로그 서식
		void FormattingLog( string filename )
		{
			if( System.IO.File.Exists( filename ) == false )
				return;

			System.IO.StreamReader reader = new System.IO.StreamReader( filename, System.Text.Encoding.Default );
			string data = reader.ReadToEnd();
			reader.Close();

			//{{ dmlee 2008.05.25 크래시 파일들이 있는 폴더 볼 수 있게
			string newdata = "";
			newdata += "<body link=darkgreen vlink=lightgreen><font size=4>";
			int path_end = filename.IndexOf( "dmp.txt" );
			string path = filename.Substring( 0, path_end );
			newdata += path;
			newdata += "</font><br><br><font size=2>";
			newdata += "<a href='X2.dmp'>Dmp File</a>  ";
			newdata += "<a href='_StateLog.txt'>stateLog</a>  ";
			newdata += "<a href='_ErrorLog.txt'>errorLog</a>  ";
			newdata += "<a href='Crash_ScreenShot.jpg'>screenshot</a>  ";
			newdata += "<a href='" + path +"' target='_blank'>folder</a>  ";
			newdata += "<a href='HardwareInfo.txt'>HardwareInfo</a><br></font><font size=3><pre>  ";
			//}} dmlee 2008.05.25 크래시 파일들이 있는 폴더 볼 수 있게


			int s, e;

			s = data.IndexOf( "ExceptionAddress" );
			e = data.IndexOf( "NumberParameters" );

			if(e>0 && s>0)
			{	
				newdata += data.Substring( s, e - s );;
			}
			else
			{
				return;
			}


			s = data.IndexOf( "STACK_TEXT" );
			e = data.IndexOf( "STACK_COMMAND" );

			if( e > 0  &&  s > 0 )
			{
				if( e < s )
					e=data.IndexOf("FAULTING_THREAD");

				string tmp = data.Substring( s, e - s );

				newdata += tmp;
			}


			s = data.IndexOf( "FAULTING_SOURCE_CODE" );
			e = data.IndexOf( "SYMBOL_STACK_INDEX" );

			if( e > 0  &&  s > 0 )
				newdata += "\n"+data.Substring( s, e - s );




			
			//{{ dmlee 2008.05.25 메일 제목과 본문 출력
			newdata += "\n-----------------------------------------------\n";
			s = data.IndexOf( "__MAIL_SUBJECT_START" );
			e = data.IndexOf( "__MAIL_SUBJECT_END" );
			if( e > 0  &&  s > 0 )
			{
				string tmp = data.Substring( s, e - s );


				newdata += "\n";				
				newdata += tmp;
				newdata += "\n";				
			}

			s = data.IndexOf( "__MAIL_BODY_START" );
			e = data.IndexOf( "__MAIL_BODY_END" );
			if( e > 0  &&  s > 0 )
			{
				string tmp = data.Substring( s, e - s );

				newdata += "\n";
				newdata += tmp;
				newdata += "\n\n\n";
			}
			//}} dmlee 2008.05.25 메일 제목과 본문 출력 

			
		
			newdata+="</pre>";
			System.IO.StreamWriter writer = new System.IO.StreamWriter( System.IO.Path.GetDirectoryName( filename )+"\\"+
				System.IO.Path.GetFileNameWithoutExtension( filename )+".html" );

			writer.Write( newdata );
			writer.Close();
		}
		
		// e메일 업데이트 - 크래쉬 덤프 첨부 파일 찾아서 파싱
		void UpdateMail()
		{			
			Outlook.Items items = folder.Items;
			parselist.Clear();
			int iEmailItemSize = items.Count;
			System.Diagnostics.Process process;
			System.Diagnostics.ProcessStartInfo info = new System.Diagnostics.ProcessStartInfo();

			System.Collections.ArrayList updatelist = new ArrayList();
			System.Collections.ArrayList deletelist = new ArrayList();

			
			this.progressBar1.Maximum = iEmailItemSize;
			this.label3.Text = "";
			this.label2.Text = "다운로드 && 분석";
			this.progressBar2.Value = 0;
			
			for( int iIdx = 1 ; iIdx <= iEmailItemSize; iIdx++ )
			{
				try
				{
					Outlook.MailItem msg = (Outlook.MailItem)items[iIdx];

					
					string time = string.Format( "{0:yyyy/MM/dd/HH/mm/ss}", msg.CreationTime );
					time += string.Format( "_email_NO_{0}", iIdx );					
					
					// 첨부 파일 검사
					if( msg.Attachments.Count == 1 &&
						msg.Attachments[1].FileName == "CrashReport.zip" &&
						msg.Attachments[1].Size > 0 )
					{
						int iStringPos = msg.Subject.IndexOf( "(" );
						string version = msg.Subject.Substring( iStringPos + 1, msg.Subject.IndexOf( ")", iStringPos ) - iStringPos - 1 ); // 버전을 표시해줄 기호가 있어야 할듯..						

						// 크래쉬 리포트 백업 폴더에 이미 파일이 있는지 검사하고 하나라도 없으면 업데이트 리스트에 포함한다.
						if( System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\X2.dmp" ) == false ||
							System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\X2.xml" ) == false ||
							System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\DialogLog.txt" ) == false ||
							System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\ErrorLog.txt" ) == false ||
							System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\HardwareInfo.txt" ) == false ||
							System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\P2PLog.txt" ) == false ||
							System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\State.log" ) == false ||
							System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\log.htm") == false )
						{
							// 업데이트할 크래쉬 데이터의 e메일 item index를 넣는다.
							updatelist.Add( iIdx );
							if( parselist.Contains( version ) == false )
								parselist.Add( version, 0 );
						}
					}
				}
				catch ( System.Exception e )
				{
					e.ToString();
				}

				this.progressBar1.Value = iIdx;
				this.label1.Text = string.Format( "리스트 업데이트 ({0}/{1})", this.progressBar1.Value, this.progressBar1.Maximum );

			}			

			this.progressBar2.Maximum = updatelist.Count;
			this.progressBar2.Value	  = 0;
			
			foreach( int i in updatelist )
			{
				lock( thread )
				{						
					Outlook.MailItem msg = (Outlook.MailItem)items[i];
					if( msg != null )
					{
						try
						{
							// 크래쉬 리포트 백업폴더 이름을 위한 시간, 버전 스트링 얻기
							string time = string.Format( "{0:yyyy/MM/dd/HH/mm/ss}", msg.CreationTime );
							time += string.Format( "_email_NO_{0}", i );

							int s = msg.Subject.IndexOf( "(" );
							string version = msg.Subject.Substring( s + 1, msg.Subject.IndexOf( ")", s ) - s - 1 ); // 버전을 표시해줄 기호가 있어야 할듯..							

							// 버전 폴더 생성
							if( System.IO.Directory.Exists( "crash\\"+version ) == false )
								System.IO.Directory.CreateDirectory( "crash\\"+version );
					
							// 시간 폴더 생성
							if( System.IO.Directory.Exists( "crash\\"+version+"\\"+time ) == false )
								System.IO.Directory.CreateDirectory( "crash\\"+version+"\\"+time );

							this.label3.Text = version+" " +time;

							// 첨부파일을 백업 폴더에 저장
							msg.Attachments[1].SaveAsFile( Application.StartupPath+"\\crash\\"+version+"\\"+time+"\\CrashReport.zip");

							info.FileName		  = "7za.exe";
							info.Arguments		  = "e -y "+"CrashReport.zip"+" *.dmp *.txt *.log *.htm *.xml *.jpg";
							info.WorkingDirectory = Application.StartupPath+"\\crash\\"+version+"\\"+time;
							info.CreateNoWindow	  = true;
							info.UseShellExecute  = false;
							process = System.Diagnostics.Process.Start(info);
							process.WaitForExit();
							System.IO.File.Delete( "crash\\"+version+"\\"+time+"\\CrashReport.zip" );

							if( System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\X2.dmp" ) == false ||
								System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\X2.xml" ) == false ||
								System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\DialogLog.txt" ) == false ||
								System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\ErrorLog.txt" ) == false ||
								System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\HardwareInfo.txt" ) == false ||
								System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\P2PLog.txt" ) == false ||
								System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\State.log" ) == false ||
								System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\log.htm") == false )
							{
								if( System.IO.Directory.Exists( "crash\\"+version+"\\"+time ) == true )
									System.IO.Directory.Delete( "crash\\"+version+"\\"+time ,true );
							}
							else
							{								
								if( System.IO.File.Exists( "crash\\"+version+"\\"+time+"\\dmp.html" ) == false )
								{
									// PDB파일 검사
									while( System.IO.File.Exists( "crash\\"+version+"\\X2.exe" ) == false ||
										   System.IO.File.Exists( "crash\\"+version+"\\X2.pdb" ) == false )
									{
										MessageBox.Show( version+" 의 X2.pdb 파일과 X2.exe 파일을 "+ 
											System.Windows.Forms.Application.StartupPath+"\\crash\\"+version+
											"폴더에 넣어 주세요" );
									}

									// 크래쉬 덤프 내용을 텍스트로..
									info = new System.Diagnostics.ProcessStartInfo();
									info.WorkingDirectory = System.Windows.Forms.Application.StartupPath+"\\crash\\"+version+"\\"+time;			
									info.FileName = System.Windows.Forms.Application.StartupPath+"\\Debugging Tools for Windows\\cdb.exe";
									info.Arguments = "-logo dmp.txt -y "+
										System.Windows.Forms.Application.StartupPath+"\\crash\\"+version+
										" -i "+System.Windows.Forms.Application.StartupPath+"\\crash\\"+version+
										" -z X2.dmp -lines -c \"!analyze -v;q\"";
									info.CreateNoWindow	 = true;
									info.UseShellExecute = false;
									process = System.Diagnostics.Process.Start(info);
									process.WaitForExit();



									//{{ state.log, error.log를 decrypt
									info = new System.Diagnostics.ProcessStartInfo();
									info.WorkingDirectory = System.Windows.Forms.Application.StartupPath;
									info.FileName = System.Windows.Forms.Application.StartupPath + "\\X2EncryptFileViewer.exe";
									info.Arguments = System.Windows.Forms.Application.StartupPath + "\\crash\\"+version+"\\"+time+"\\State.log ";
									info.Arguments += System.Windows.Forms.Application.StartupPath + "\\crash\\"+version+"\\"+time+"\\_StateLog.txt";
									info.CreateNoWindow = true;
									info.UseShellExecute = false;
									process = System.Diagnostics.Process.Start(info);
									process.WaitForExit();


									info = new System.Diagnostics.ProcessStartInfo();
									info.WorkingDirectory = System.Windows.Forms.Application.StartupPath;
									info.FileName = System.Windows.Forms.Application.StartupPath + "\\X2EncryptFileViewer.exe";
									info.Arguments = System.Windows.Forms.Application.StartupPath + "\\crash\\"+version+"\\"+time+"\\ErrorLog.txt ";
									info.Arguments += System.Windows.Forms.Application.StartupPath + "\\crash\\"+version+"\\"+time+"\\_ErrorLog.txt";
									info.CreateNoWindow = true;
									info.UseShellExecute = false;
									process = System.Diagnostics.Process.Start(info);
									process.WaitForExit();
									//}} state.log, error.log를 decrypt
									


									//{{ dmlee 2008.05.25 메일 제목과 본문 내용 출력
									System.IO.StreamWriter fsw = System.IO.File.AppendText( System.Windows.Forms.Application.StartupPath+"\\crash\\"+version+"\\"+time+"\\dmp.txt" );
									fsw.Write( "__MAIL_SUBJECT_START \n" );
									fsw.Write( msg.Subject );
									fsw.Write( "__MAIL_SUBJECT_END \n" );
									fsw.Write( "__MAIL_BODY_START \n" );
									fsw.Write( msg.Body );
									fsw.Write( "__MAIL_BODY_END \n" );
									fsw.Flush();
									fsw.Close();
									//}} dmlee 2008.05.25 메일 제목과 본문 내용 출력


									// fix!! state.log, error.log의 끝에 10줄 정도를 같이 출력하자


									
																		

									// 크래쉬 덤프 텍스트 파싱
									FormattingLog( System.Windows.Forms.Application.StartupPath+"\\crash\\"+version+"\\"+time+"\\dmp.txt" );
									//System.IO.File.Delete( "crash\\"+version+"\\"+time+"\\dmp.txt" ); // fix!! 메일 제목과 본문내용을 보기위해 지우지 말고 남겨둠
								}
							}	
						}
						catch (System.Exception e)
						{
							MessageBox.Show(e.ToString());
							continue;
						}
						
					}
				}

				this.progressBar2.Value++;
				this.label2.Text = string.Format( "다운로드 && 분석 ({0}/{1})", this.progressBar2.Value, this.progressBar2.Maximum );			
			}			
			
			this.label3.Text = "";

			thread = null;
			this.DialogResult = DialogResult.OK;
		}


		System.Threading.Thread thread;
		public void UpdateMailList( Outlook.MAPIFolder folder )
		{			
			thread = new System.Threading.Thread( new System.Threading.ThreadStart( UpdateMail ) );
			this.folder = folder;
			thread.Start();
		}


		protected override void OnClosing( CancelEventArgs e )
		{			
			if( thread != null )
			{
				thread.Suspend();
				if( MessageBox.Show( "업데이트 중입니다. 종료하시겠습니까?", "경고",
					System.Windows.Forms.MessageBoxButtons.YesNo,
					System.Windows.Forms.MessageBoxIcon.Warning ) == DialogResult.Yes )
				{
					thread.Resume();
					thread.Abort();
					thread.Join();
					thread = null;
					e.Cancel = false;
					this.DialogResult = DialogResult.Cancel;
				}
				else
				{
					e.Cancel = true;
					thread.Resume();
				}
			}			
			base.OnClosing( e );
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
			this.progressBar1 = new System.Windows.Forms.ProgressBar();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.progressBar2 = new System.Windows.Forms.ProgressBar();
			this.label3 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// progressBar1
			// 
			this.progressBar1.Location = new System.Drawing.Point(40, 40);
			this.progressBar1.Name = "progressBar1";
			this.progressBar1.Size = new System.Drawing.Size(456, 23);
			this.progressBar1.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(40, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(95, 17);
			this.label1.TabIndex = 1;
			this.label1.Text = "리스트 업데이트";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(40, 72);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(95, 17);
			this.label2.TabIndex = 3;
			this.label2.Text = "다운로드 && 분석";
			// 
			// progressBar2
			// 
			this.progressBar2.Location = new System.Drawing.Point(40, 96);
			this.progressBar2.Name = "progressBar2";
			this.progressBar2.Size = new System.Drawing.Size(456, 23);
			this.progressBar2.TabIndex = 2;
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(216, 72);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(272, 17);
			this.label3.TabIndex = 4;
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// Form2
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
			this.ClientSize = new System.Drawing.Size(538, 143);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.progressBar2);
			this.Controls.Add(this.progressBar1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "Form2";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "업데이트";
			this.ResumeLayout(false);

		}
		#endregion

		private void menuItem1_Click( object sender, System.EventArgs e )
		{
	
		}

		private void menuItem3_Click( object sender, System.EventArgs e )
		{			
			System.Threading.Thread thread = new System.Threading.Thread( new System.Threading.ThreadStart( UpdateMail ) );
//			this.client = client;
			thread.Start();
		}
		
	}
}
