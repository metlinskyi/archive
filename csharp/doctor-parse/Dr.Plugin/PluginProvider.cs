using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.IO;
using Dr.Plugin.Tools;

namespace Dr.Plugin
{
    public class PluginProvider
    {
        protected object m_Instance;
        protected Type InstanceType;

        public object Instance
        {
            get { return m_Instance; }
        }

        public PluginProvider()
        {
            m_Instance = null;
            InstanceType = null;
        }

        public bool PlugIn(string nspace, string type, Dictionary<string, string> settings)
        {
            string dllPath = Path.Combine(Directory.GetCurrentDirectory(), string.Format("{0}.{1}.dll", nspace, type));
            if (File.Exists(dllPath))
            {

                object instanceSettings = PnP.CreateInstance(string.Format("{0}.{1}Settings", nspace, type), dllPath);
                if (instanceSettings != null)
                {

                    // Settings property
                    PropertyInfo[] properties = instanceSettings.GetType().GetProperties();
                    foreach (PropertyInfo p in properties.Where(p => p.CanWrite))
                    {
                        p.SetValue(instanceSettings,
                            p.PropertyType == typeof (String)
                                ? settings[p.Name].Trim()
                                : Convert.ChangeType(settings[p.Name].Trim(), p.PropertyType), null);
                    }

                    ///////////////////////////
                    m_Instance = PnP.CreateInstance(string.Format("{0}.{1}", nspace, type), dllPath, new [] { instanceSettings });
                    if (m_Instance != null)
                    {
                        InstanceType = m_Instance.GetType();
                    }
                    else
                        return false;
                }
            }
            return true;
        }
    }
}