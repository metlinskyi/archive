using System.Collections.Generic;

namespace Lib.Mapping.Prototypes
{
    public interface IMapperAttribute
    {
        IEnumerable<string> Names { get; }

        IDictionary<string, string> GetValues<TSource>(TSource source);
    }
}
