using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace ContentSelector
{
    static public class CharacterScriptCreator
    {
        static void CreateRadioButton( ref StreamWriter sw, string texturefilename, string Name, string MSG, int xPos )
        {
            string RBName = "g_pRadioButton" + Name + "_Slot";

            sw.WriteLine(RBName + " = g_pUIDialog:CreateRadioButton()");
            sw.WriteLine( "g_pUIDialog:AddControl( " + RBName + " )" );
            sw.WriteLine( RBName + ":SetFixOverByCheck( true )" );
            sw.WriteLine( RBName + ":SetShowOffBGByCheck( true )" );
            sw.WriteLine( RBName + ":SetName( \"" + Name + "_Slot\" )" );
            sw.WriteLine( RBName + ":SetNormalTex( \"" + texturefilename + "\", \"" + Name + "_Normal\" )" );
            sw.WriteLine( RBName + ":SetOverTex( \"" + texturefilename + "\", \"" + Name + "_Over\" )" );
            sw.WriteLine( RBName + ":SetCheckedTex( \"" + texturefilename + "\", \"" + Name + "_Over\" )" );

            sw.WriteLine( RBName + ":SetBGPoint" );
            sw.WriteLine( "{" );
	        sw.WriteLine( "\t\"LEFT_TOP		= D3DXVECTOR2(" + xPos.ToString() + ",547)\",");
	        sw.WriteLine( "\t\"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)\"," );
	        sw.WriteLine( "\tCHANGE_TIME		= 0.0," );
            sw.WriteLine("}");

            sw.WriteLine(RBName + ":SetBGMouseOverPoint");
            sw.WriteLine("{");
            sw.WriteLine("\t\"LEFT_TOP		= D3DXVECTOR2(" + xPos.ToString() + ",547)\",");
            sw.WriteLine("\t\"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)\",");
            sw.WriteLine("\tCHANGE_TIME		= 0.0,");
            sw.WriteLine("}");

            sw.WriteLine(RBName + ":SetRBPoint");
            sw.WriteLine("{");
            sw.WriteLine("\tUSE_TEXTURE_SIZE	= FALSE,");
            sw.WriteLine("\t\"LEFT_TOP		= D3DXVECTOR2(" + xPos.ToString() + ",547)\",");
            sw.WriteLine("\t\"RIGHT_BOTTOM	= D3DXVECTOR2(" + (xPos + 156).ToString() + ",703)\",");
            sw.WriteLine("\t\"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)\",");
            sw.WriteLine("\tCHANGE_TIME		= 0.0,");
            sw.WriteLine("}");

            sw.WriteLine(RBName + ":SetCheckPoint");
            sw.WriteLine("{");
            sw.WriteLine("\tADD_SIZE_X = -2," );
            sw.WriteLine("\tADD_SIZE_Y = -2," );
            sw.WriteLine("\t\"LEFT_TOP		= D3DXVECTOR2(" + (xPos+1).ToString() + ",548)\",");
            sw.WriteLine("\t\"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)\",");
            sw.WriteLine("\tCHANGE_TIME		= 0.0,");
            sw.WriteLine("}");

            sw.WriteLine(RBName + ":SetGroupID( 1 )");
            sw.WriteLine(RBName + ":SetCustomMsgChecked( STATE_CREATE_UNIT_UI_CUSTOM_MSG[\""+ MSG +"\"]  )");
        }

        static void CreateNewMark(ref StreamWriter sw, int xPos)
        {
            sw.WriteLine("g_pPicture_new = g_pUIDialog:CreatePicture()");
            sw.WriteLine("g_pStaticnew:AddPicture( g_pPicture_new )");
            sw.WriteLine("g_pPicture_new:SetTex( \"DLG_Common_New_Texture44.TGA\", \"quest_list_new\" )");
            sw.WriteLine();
            sw.WriteLine("g_pPicture_new:SetPoint");
            sw.WriteLine("{");
            sw.WriteLine("\t\"LEFT_TOP		= D3DXVECTOR2(" + (xPos+97).ToString() + ",675)\", ");
            sw.WriteLine("\t\"COLOR			= D3DXCOLOR(1.0,1.0,1.0,1.0)\",");
            sw.WriteLine("\tCHANGE_TIME		= 0.0,");
            sw.WriteLine("}");
            sw.WriteLine("");
        }

        static public void CreateScript( ref CheckedListBox CharacterBox, ref CheckedListBox newmarkbox, string TargetFolder, bool AddCode, string code, Encoding encoding )
        {
            string swfilename;
            if (AddCode && code.Length > 0)
            {
                swfilename = TargetFolder + "\\DLG_Unit_Create_Unit_Button@" + code + ".lua";
            }
            else
            {
                swfilename = TargetFolder + "\\DLG_Unit_Create_Unit_Button.lua";
            }

            StreamWriter sw = null;
            try
            {
                float xPos = 90.0f;
                float xOffset = 170.75f;

                sw = new StreamWriter(swfilename, false, encoding);
                sw.WriteLine("-- lua header. UTF-8 인코딩 인식을 위해 이 줄은 지우지 마세요.");
                sw.WriteLine();
                sw.WriteLine("---------New Static---------");
                sw.WriteLine("g_pStaticnew = g_pUIDialog:CreateStatic()");
                sw.WriteLine("g_pUIDialog:AddControl( g_pStaticnew )");
                sw.WriteLine("g_pStaticnew:SetName( \"new\" )");
                sw.WriteLine();
                sw.WriteLine("---------라디오 버튼--------");
                sw.WriteLine();

                System.Collections.IEnumerator ie;
                ie = CharacterBox.CheckedIndices.GetEnumerator();
                
                System.Collections.IEnumerator ienew;
                ienew = newmarkbox.CheckedIndices.GetEnumerator();
                
                while (ie.MoveNext())             
                {
                    switch((int)ie.Current)
                    {
                        case 0: // 엘소드
                            {
                                CreateRadioButton( ref sw, "DLG_Common_Texture04.TGA", "Elsword", "SCUUCM_ELSWORD_BUTTON_UP", (int)xPos );
                            } break;
                        case 1: // 아이샤
                            {
                                CreateRadioButton(ref sw, "DLG_Common_Texture04.TGA", "Aisha", "SCUUCM_ARME_BUTTON_UP", (int)xPos );
                            } break;
                        case 2: // 레나
                            {
                                CreateRadioButton(ref sw, "DLG_Common_Texture04.TGA", "Rena", "SCUUCM_LIRE_BUTTON_UP", (int)xPos );
                            } break;
                        case 3: // 레이븐
                            {
                                CreateRadioButton(ref sw, "DLG_Common_New_Texture57.TGA", "Raven", "SCUUCM_RAVEN_BUTTON_UP", (int)xPos );
                            } break;
                        case 4: // 이브
                            {
                                CreateRadioButton(ref sw, "DLG_UI_Character01.TGA", "Eve", "SCUUCM_EVE_BUTTON_UP", (int)xPos );
                            } break;
                    }

                    ienew.Reset();
                    while (ienew.MoveNext())
                    {
                        if ((int)ienew.Current == (int)ie.Current)
                        {
                            CreateNewMark(ref sw, (int)xPos);
                            break;
                        }
                    }

                    xPos = xPos + xOffset;
                }

                sw.WriteLine("g_pUIDialog:ChangeSequenceByName( \"new\", true )");
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }
            finally
            {
                if (sw != null)
                    sw.Close();
            }


            
        }
    }
}
