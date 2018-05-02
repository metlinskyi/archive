using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Reflection;

namespace Dr.Parser
{
    public enum MultiThreadMode 
    {
        Unknown,
        Default,
        Pool,
        Background
    }

    public class Settings
    {
        protected Dictionary<string, YamlNode> m_Settings = new Dictionary<string, YamlNode>();

        public Settings()
        {
            _mMultiThread = MultiThreadMode.Unknown;
        }

        public string Pacients
        {
            get {
                return m_Settings["Pacients"] != null 
                    ? m_Settings["Pacients"].Value.Trim() 
                    : string.Empty;
            }
        }

        public Dictionary<string, string> PacientsSettings
        {
            get { return m_Settings["Pacients"].Parameters; }
        }

        public string DataBase
        {
            get {
                return m_Settings["DataBase"] != null 
                    ? m_Settings["DataBase"].Value.Trim() 
                    : string.Empty;
            }
        }

        public Dictionary<string, string> DataBaseSettings
        {
            get { return m_Settings["DataBase"].Parameters; }
        }

        private MultiThreadMode _mMultiThread;
        public MultiThreadMode MultiThread
        {
            get
            {
                if (_mMultiThread == MultiThreadMode.Unknown)
                    if (m_Settings["MultiThread"] == null)
                        _mMultiThread = MultiThreadMode.Default;
                    else
                        switch(m_Settings["MultiThread"].Value.ToUpper())
                        {
                            case "POOL":
                                _mMultiThread = MultiThreadMode.Pool;
                                break;
                            case "BACKGROUND":
                                _mMultiThread = MultiThreadMode.Background;
                                break;
                            default:
                                _mMultiThread = MultiThreadMode.Default;
                                break;
                        }

                return _mMultiThread;
            }
        }

        public int MaxThreads
        {
            get
            {
                return m_Settings["MaxThreads"] != null 
                    ? Convert.ToInt32(m_Settings["MaxThreads"].Value.Trim()) 
                    : 0;
            }
        }

        public int MinThreads
        {
            get
            {
                return m_Settings["MinThreads"] != null 
                    ? Convert.ToInt32(m_Settings["MinThreads"].Value.Trim()) 
                    : 0;
            }
        }

    }
}