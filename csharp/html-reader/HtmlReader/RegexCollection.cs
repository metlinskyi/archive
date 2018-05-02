using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace HtmlReader
{
    internal class RegexCollection
    {
        public static readonly Regex Trim = new Regex(@"\s+", RegexOptions.Compiled);

        public static readonly Regex Clean = new Regex(@"\s*<.*?>\s*", RegexOptions.Singleline | RegexOptions.Compiled);

        public static Regex TagEx(string name)
        {
            return Collection.GetEx("#", name, @"(<{0}.[^>]*?>.*?</{0}>|<{0}.[^>]*/?>)");
        }

        public static Regex AttributeEx(string name)
        {
            return Collection.GetEx("@", name, @"{0}=[\""](.*?)[\""]");
        }

        public static Regex ContentEx(string name)
        {
            return Collection.GetEx(":", name, @"^<{0}.*?>|</{0}>$");
        }

        #region Implementation 

        private static RegexCollection Collection { get; } = new RegexCollection();

        private readonly IDictionary<string, Regex> _dictionary;

        private RegexCollection()
        {
            _dictionary = new Dictionary<string, Regex>();
        }

        public Regex GetEx(string key, string name, string pattern, RegexOptions options)
        {
            key += name;

            Regex regex;
            if (!_dictionary.TryGetValue(key, out regex))
            {
                regex = new Regex(string.Format(pattern, name), options);

                _dictionary.Add(key, regex);
            }

            return regex;
        }

        public Regex GetEx(string key, string name, string pattern)
        {
            return GetEx(key, name, pattern, RegexOptions.IgnoreCase | RegexOptions.Singleline | RegexOptions.Compiled);
        }

        #endregion

    }
}
