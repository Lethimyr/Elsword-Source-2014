using System;
using System.Collections.Generic;

using System.IO;
using System.Text;

namespace ContentSelector
{
    static class StringManager
    {
        static public bool InitializeStringTable(string wstrGeneralStringFileName, string wstrScriptStringFileName)
        {
            return LoadStringTable(wstrGeneralStringFileName);
        }

        static public string GetGeneralString(int iID)
        {
            int iTableSize = m_vecGeneralStringTable.Count;

            // STR_ID_EMPTY 때문에 -1이 넘어올 수도 있다.
            if (iID < 0 || iID >= iTableSize)
            {
                return m_wstrEmpty;
            }

            return m_vecGeneralStringTable[iID];
        }
        //public string GetScriptString( int iID );
        //public string GetReplacedString( int iID, char* szParam, ... );

        static bool LoadStringTable(string wstrFileName)
        {
            m_vecGeneralStringTable.Clear();

            // 유니코드 파일을 읽으므로 바이너리 모드로 오픈한다.
            StreamReader sr;
            sr = new StreamReader(wstrFileName, Encoding.Unicode, false);

            //             char[] buf = new char[2];
            //             
            //             // UTF-16LE의 BOM( Byte Order Mark )을 뛰어 넘는다.
            //             sr.Read(buf, 0, 2);

            // string table 에서 한 줄을 읽으면 다음과 같은 모양이다.
            // [index]\t[string]\r\n
            const int ciMaxStringLength = 2048;
            char[] wszStringRead = new char[ciMaxStringLength];
            int iIndex = 0;

            do
            {
                string wstrStringRead;
                wstrStringRead = sr.ReadLine();

                // 최대 스트링 길이만큼 읽었다면 문제가 있는지 의심해봐야 한다.
                //ASSERT(wstrStringRead.Length < (ciMaxStringLength - 1));

                //09. 07. 03 김정협  XOR암호화 한 파일 복호화
                wstrStringRead = wstrStringRead.Replace("\r\n", "");
                wszStringRead = wstrStringRead.ToCharArray();

                short nKey = 16;
                int nSize = wstrStringRead.Length;

                for (int i = 0; i < nSize; i++)
                {
                    wszStringRead[i] = (char)(wstrStringRead[i] ^ nKey);
                }
                wstrStringRead = new string(wszStringRead);


                // tab 을 읽어서 index를 구분해낸다.
                int iTabPos = wstrStringRead.IndexOf("\t");
                string wstrIndex = wstrStringRead.Substring(0, iTabPos);
                int iIndexRead = int.Parse(wstrIndex);

                string wstrContent = "";
                // 파일의 index가 제대로 설정되어 있는지 검사한다.
                //ASSERT(iIndexRead == iIndex);
                if (iIndexRead == iIndex)
                {
                    // tab 을 찾은 경우에만 그 뒤의 내용을 읽어야 한다.
                    if (iTabPos != -1)
                    {
                        //09. 07. 03 김정협  미리 없애기 때문에 막음

                        //ASSERT(iTabPos < wstrStringRead.size());
                        // tab 뒤에 있는 것들만 읽는다.
                        if ((iTabPos + 1) < wstrStringRead.Length)
                        {
                            wstrContent = wstrStringRead.Substring(iTabPos + 1);
                        }
                    }
                }
                // \n 으로 표시한 부분에 실제로 줄바꿈 해준다.
                wstrContent.Replace("\\n", "\r\n");

                m_vecGeneralStringTable.Add(wstrContent);
                iIndex++;
            } while (sr.EndOfStream == false);

            sr.Close();

            return true;
        }

        static List<string> m_vecGeneralStringTable = new List<string>();
        static List<string> m_vecScriptStringTable = new List<string>();
        static string m_wstrEmpty = "";
    };
}