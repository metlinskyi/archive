using System.Collections.Generic;
using System.Reflection;

namespace Dr.Plugin.Pacients
{
    public class PacientsProvider : PluginProvider, IPacientCollection
    {
        public PacientsProvider(string name, Dictionary<string,string> settings) 
        {
            if (PlugIn("Dr.Pacients", name, settings))
            {

            }
        }

        private PropertyInfo _prNext;
        public Pacient Next
        {
            get
            {
                if (_prNext == null)
                    _prNext = InstanceType.GetProperty("Next");

                return _prNext.GetValue(Instance, null) as Pacient;
            }
        }

        private PropertyInfo _prPrev;
        public Pacient Prev
        {
            get
            {
                if (_prPrev == null)
                    _prPrev = InstanceType.GetProperty("Prev");

                return _prPrev.GetValue(Instance, null) as Pacient;
            }
        }

        private PropertyInfo _prCount;
        public int Count
        {
            get
            {
                if (_prCount == null)
                    _prCount = InstanceType.GetProperty("Count");

                return (int)_prCount.GetValue(Instance, null);
            }
        }

        private  MethodInfo _methodSave;
        public bool Save(Pacient pacient)
        {
            if(_methodSave == null)
                _methodSave = InstanceType.GetMethod("Save");

            return (bool)_methodSave.Invoke(Instance, new object[] { pacient });
        }
    }
}
