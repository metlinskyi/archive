using HtmlReader.Tags.Prototypes;
using System.Collections.Generic;

namespace HtmlReader
{
    public static class HtmlReaderHelper
    {
        public static IEnumerable<T> Select<T>(this HtmlReader reader)
            where T : ITag, new()
        {
            return new HtmlReader<T>(reader);
        }
    }
}
