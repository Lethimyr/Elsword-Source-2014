using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using Outlook = Microsoft.Office.Interop.Outlook;


namespace HackUserReport
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

            System.Collections.ArrayList hackList = new ArrayList();
            string startPath = System.Windows.Forms.Application.StartupPath + "\\HackPGM.txt";
            System.IO.StreamReader readerOpt = new System.IO.StreamReader( startPath, System.Text.Encoding.Default );

            hackList.Clear();
            while(readerOpt.Peek() > 0)
            {
                string hackPgm = readerOpt.ReadLine();
                hackList.Add(hackPgm);
            }

            readerOpt.Close();



			System.IO.StreamReader reader = new System.IO.StreamReader( filename, System.Text.Encoding.Default );
            string procName = "";
			//string data = reader.ReadToEnd();

            bool bFind = false;
            bool userFind = false;
            string userName = "";

            while(reader.Peek() > 0)
            {
                string strData = reader.ReadLine();
             
                bFind = false;
                foreach(string strProc in hackList)
                {
                    if(strData.Equals(strProc) == true || strData.IndexOf(strProc) >= 0)
                    {
                        bFind = true;
                        break;
                    }                    
                }

                if(bFind == true )
                {
                    procName = ("<font size=2 color=red>" + strData + "</font>\n") + procName;
                }
                else if(strData.Length > 0)
                {
                    if(strData.IndexOf("[") >= 0)
                    {
                        // id
                        if(userFind == false)
                        {
                            userFind = true;
                            userName = strData+"\n\n";
                        }
                    }
                    else
                    {
                        procName += ("<font size=2 color=black>" + strData + "</font>\n");               
                    }
                    
                }
                
            }

            reader.Close();

            procName = userName + procName;
            //data = data.Replace("\r\n", "\n");
			
			string newdata = "";
			newdata += "<body link=darkgreen vlink=lightgreen><font size=4>";
			int path_end = filename.IndexOf( "dmp.txt" );
			string path = filename.Substring( 0, path_end );
			
			newdata += "</font>";
            newdata += "<font size=3><pre>";
			// process name 출력

            
            newdata += procName;
					
			newdata+="</pre>";
			System.IO.StreamWriter writer = new System.IO.StreamWriter( System.IO.Path.GetDirectoryName( filename )+"\\"+
				System.IO.Path.GetFileNameWithoutExtension( filename )+".html" );

			writer.Write( newdata );
			writer.Close();
		}
		
		// e메일 업데이트 - 크래쉬 덤프 첨부 파일 찾아서 파싱
		void UpdateMail()
		{			
            Outlook.MAPIFolder selectedFolder = folder;

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
					
                    string msgDate = string.Format("{0:yyyy/MM/dd}", msg.CreationTime );

                    string hackUser = msg.Subject.Substring(0, msg.Subject.IndexOf( "님") );

                    string []str = selectedFolder.FolderPath.Split('\\');
                    string version = str[str.Length-1];
                    

                    updatelist.Add( iIdx );
                    if( parselist.Contains( version ) == false )
                        parselist.Add( version, 0 );
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
			
            System.Collections.ArrayList ProcessName = new ArrayList();
            int nProcess = 0;

			foreach( int i in updatelist )
			{
				lock( thread )
				{						
					Outlook.MailItem msg = (Outlook.MailItem)items[i];                    
                    ProcessName.Clear();

					if( msg != null )
					{
						try
						{
                            string time = string.Format("{0:yyyy/MM/dd}", msg.CreationTime );

                            
                            string hackUser = msg.Subject.Substring(0, msg.Subject.IndexOf( "님") );
                            string []str = selectedFolder.FolderPath.Split('\\');
                            string version = str[str.Length-1];
                            int nCount = 0;
                            char[] ch = {'\r'};
                            string msgBody = msg.Body;                            
                            msgBody = msgBody.Replace("\r\n", "\r");
                            string[] bodyArray = msgBody.Split(ch);	
							nCount = bodyArray.GetLength(0);
                            

							// 버전 폴더 생성
							if( System.IO.Directory.Exists( "HackUser\\"+version ) == false )
								System.IO.Directory.CreateDirectory( "HackUser\\"+version );
					
                            // 버전 폴더 생성
                            if( System.IO.Directory.Exists( "HackUser\\"+version + "\\" + hackUser ) == false )
                                System.IO.Directory.CreateDirectory( "HackUser\\"+version + "\\" + hackUser);
							
							this.label3.Text = version+" " +hackUser;

							string filename = System.Windows.Forms.Application.StartupPath+"\\HackUser\\"+version+"\\"+hackUser+"\\dmp.txt";

                            nProcess = 0;
                            if( System.IO.File.Exists( filename ) == true )
                            {
                                System.IO.StreamReader reader = new System.IO.StreamReader( filename, System.Text.Encoding.Default );

                                
                                while(reader.Peek() > 0)
                                {
                                    string strData = reader.ReadLine();
                                    ProcessName.Add(strData);
                                    ++nProcess;
                                }
                                

                                reader.Close();
                            }

                            bool bFind;
                            
                            for(int index1=0; index1<nCount; ++index1)
                            {
                                bFind = false;
                                for(int index2=0; index2<nProcess; ++index2)
                                {
                                    if(ProcessName[index2].Equals(bodyArray[index1]) == true)
                                    {
                                        bFind = true;
                                        break;
                                    }
                                }

                                if(bFind == false)
                                {
                                    ProcessName.Add(bodyArray[index1]);
                                    ++nProcess;
                                }
                            }
                        
							                         
							System.IO.StreamWriter fsw = System.IO.File.CreateText( filename );
							
                            string temp = "["+hackUser+"]";
                            if( nCount == 0 || temp.Equals( ProcessName[0] ) == false )
                                fsw.Write("["+hackUser+"]\r\n\r\n");

                            for(int index3=0; index3 < nProcess; ++index3)
                            {
                                fsw.Write( ProcessName[index3] + "\r\n" );
                            }
							
							
							fsw.Flush();
							fsw.Close();
							//}} dmlee 2008.05.25 메일 제목과 본문 내용 출력


							// fix!! state.log, error.log의 끝에 10줄 정도를 같이 출력하자


							
																

							// 크래쉬 덤프 텍스트 파싱
							FormattingLog( System.Windows.Forms.Application.StartupPath+"\\HackUser\\"+version+"\\"+hackUser+"\\dmp.txt" );
							//System.IO.File.Delete( "HackUser\\"+version+"\\"+time+"\\dmp.txt" ); // fix!! 메일 제목과 본문내용을 보기위해 지우지 말고 남겨둠
						
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
            this.folder = folder;
			thread = new System.Threading.Thread( new System.Threading.ThreadStart( UpdateMail ) );
			
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
