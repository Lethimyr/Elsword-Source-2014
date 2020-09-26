using System;
using System.IO;

using LuaInterface;

namespace ContentSelector
{
    static class LuaTool
    {
        // Command Excution **************************

        // String으로 주어진 Lua 명령 실행.
        static public bool DoString(Lua lua, string strCmd)
        {
            try
            {
                lua.DoString(strCmd);
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }
        // File에 주어진 Lua 명령 실행.
        static public bool DoFile(Lua lua, string strFileName)
        {
            StreamReader sr = null;
            try
            {
                sr = new StreamReader(strFileName, System.Text.Encoding.UTF8, false);
                string file = sr.ReadToEnd();
                lua.DoString(file);
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);

                return false;
            }
        }

        static public bool GetValue(Lua lua, string fullpath, out int outvalue, int defaultvalue)
        {
            try
            {
                outvalue = (int)lua.GetNumber(fullpath);
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);

                outvalue = defaultvalue;
                return false;
            }
        }
        static public bool GetValue(Lua lua, string fullpath, out double outvalue, double defaultvalue)
        {
            try
            {
                outvalue = lua.GetNumber(fullpath);
                return true;
            }
            catch (Exception)
            {
                outvalue = defaultvalue;
                return false;
            }
        }
        static public bool GetValue(Lua lua, string fullpath, out float outvalue, float defaultvalue)
        {
            try
            {
                outvalue = (float)lua.GetNumber(fullpath);
                return true;
            }
            catch (Exception)
            {
                outvalue = defaultvalue;
                return false;
            }
        }
        static public bool GetValue(Lua lua, string fullpath, out string outvalue, string defaultvalue)
        {
            try
            {
                outvalue = lua.GetString(fullpath);
                return true;
            }
            catch (Exception)
            {
                outvalue = defaultvalue;
                return false;
            }
        }
        static public bool GetValue(Lua lua, string fullpath, out bool outvalue, bool defaultvalue)
        {
            try
            {
                int luavalue = 0;
                luavalue = (int)lua.GetNumber(fullpath);
                if (luavalue == 1)
                {
                    outvalue = true;
                }
                else
                {
                    outvalue = false;
                }
                return true;
            }
            catch (Exception)
            {
                outvalue = defaultvalue;
                return false;
            }
        }

        static public bool GetValue(LuaTable lua, string fullpath, out int outvalue, int defaultvalue)
        {
            try
            {
                outvalue = (int)(double)lua[fullpath];
                return true;
            }
            catch(Exception e)
            {
                Console.WriteLine(e.Message);
                outvalue = defaultvalue;
                return false;
            }

        }
        static public bool GetValue(LuaTable lua, string fullpath, out double outvalue, double defaultvalue)
        {
            try
            {
                outvalue = (double)lua[fullpath];
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                outvalue = defaultvalue;
                return false;
            }

        }
        static public bool GetValue(LuaTable lua, string fullpath, out float outvalue, float defaultvalue)
        {
            try
            {
                outvalue = (float)(double)lua[fullpath];
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                outvalue = defaultvalue;
                return false;
            }

        }
        static public bool GetValue(LuaTable lua, string fullpath, out string outvalue, string defaultvalue)
        {
            try
            {
                outvalue = (string)lua[fullpath];
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                outvalue = defaultvalue;
                return false;
            }

        }
        static public bool GetValue(LuaTable lua, string fullpath, out bool outvalue, bool defaultvalue)
        {
            try
            {
                int luavalue = 0;
                luavalue = (int)(double)lua[fullpath];
                if (luavalue == 1)
                {
                    outvalue = true;
                }
                else
                {
                    outvalue = false;
                }
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                outvalue = defaultvalue;
                return false;
            }

        }
        
        static public string ConvertStringForm(string input)
        {
            input = input.Replace("[\"", ".");
            input = input.Replace("\"]", "");
                        
            return input;
        }

        static public bool GetTable(Lua lua, string fullpath, out LuaTable outTable)
        {
            try
            {
                outTable = lua.GetTable(fullpath);
                return true;
            }
            catch(Exception)
            {
                outTable = null;
                return false;
            }
        }
        static public bool GetTable(LuaTable lua, string fullpath, out LuaTable outTable)
        {
            try
            {
                outTable = (LuaTable)lua[fullpath];
                if (outTable == null)
                    return false;
                return true;
            }
            catch (Exception)
            {
                outTable = null;
                return false;
            }
        }
               

    };

}