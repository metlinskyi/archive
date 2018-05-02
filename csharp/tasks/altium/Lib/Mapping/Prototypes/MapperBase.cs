using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Lib.Mapping.Prototypes
{
    public abstract class MapperBase
    {
        protected Type Type { get; set; }

        protected IEnumerable<PropertyInfo> Properties { get; set; }

        protected MapperBase(Type type)
        {
            Type = type;
        }

        protected IEnumerable<PropertyInfo> GetProperties()
        {
            return Type
                .GetProperties(BindingFlags.Public | BindingFlags.Instance)
                .Where(x => x.CanWrite);
        }
    }
}
