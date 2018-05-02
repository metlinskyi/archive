using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace Dr.Parser
{
    public class YamlNode
    {
        private readonly String _mName;

        private String _mValue;
        private Type _mType;
        private YamlNode _mParrent;
        private YamlNode[] _mChilds;

        private readonly Dictionary<string, string> _mParameters;

        public YamlNode(String name)
        {
            _mName = name;
            _mType = typeof(String);
            _mParameters = new Dictionary<string, string>();
            _mChilds = null;
            _mParrent = null;
        }

        public String Name
        {
            get { return _mName; }
        }

        public String Value
        {
            get { return _mValue; }
            set
            {
                _mValue = value;

                if (Regex.IsMatch(value, @"^\d+$"))
                    _mType = typeof(Int32);
                else if (Regex.IsMatch(value.ToLower(), @"^(on|of|no|yes|true|false)$"))
                    _mType = typeof(Boolean);
                else
                    _mType = typeof(String);
            }
        }

        public Type Type
        {
            get { return _mType; }
        }


        public Dictionary<string, string> Parameters
        {
            get { return _mParameters; }
        }
    }
}