using System.Collections.Generic;
using System.Reflection;

namespace Dr.Plugin.DataBase
{
    public class DataBaseProvider : PluginProvider , IDataBaseProvider
    {
        public DataBaseProvider(string name, Dictionary<string, string> settings)
        {
            if (PlugIn("Dr.DataBase", name, settings))
            {

            }
        }

        private MethodInfo _methodExecute;
        public bool Execute(string query)
        {
            if (_methodExecute == null)
                _methodExecute = InstanceType.GetMethod("Execute");

            return (bool)_methodExecute.Invoke(Instance, new object[] { query });
        }
    }
}
