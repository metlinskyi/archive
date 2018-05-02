using System;
using Lib.Properties;

namespace Lib.Mapping
{
    public class MapperException : Exception
    {
        public bool Critical { get; private set; }

        public MapperException(string message, bool critical) :
            base(message)
        {
            Critical = critical;
        }

        public MapperException(string message) :
            this(message, true)
        {
        }

        public MapperException(bool critical) :
            this(Resources.MapperException, critical)
        {
        }

        public MapperException() :
            this(Resources.MapperException, true)
        {
        }
    }
}
