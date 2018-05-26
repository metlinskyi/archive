using System.Collections.Generic;

namespace Matrix.Web.Helpers
{
    public static class FileHelper
    {
        private static readonly IDictionary<string, string> _extensions;

        static FileHelper()
        {
            _extensions = new Dictionary<string, string>
            {
                { "text/csv", "csv" }
            };
        }

        public static string GetFileName<T>(T item, string contentType)
        {
            return _extensions.TryGetValue(contentType, out string value)
                ? $"{ item.ToString() }.{value}"
                : item.ToString();
        }
    }
}