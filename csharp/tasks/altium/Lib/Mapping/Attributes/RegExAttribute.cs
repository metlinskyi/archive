using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
using Lib.Mapping.Prototypes;

namespace Lib.Mapping.Attributes
{
    /// <summary>
    /// The regular expression attribute
    /// </summary>
    public class RegExAttribute : MapperAttribute, IMapperAttribute
    {
        private readonly Regex _regex;

        public IEnumerable<string> Names { get; }

        public RegExAttribute(Regex regEx)
        {
            _regex = regEx;

            Names = _regex.GetGroupNames()
                .Skip(1)
                .ToArray();
        }

        public RegExAttribute(string regEx, RegexOptions options) :
            this(new Regex(regEx, options))
        {
        }

        public RegExAttribute(string regExs) :
            this(regExs, RegexOptions.Compiled)
        {
        }

        public IDictionary<string, string> GetValues<T>(T source)
        {
            return GetValues(source as string);
        }

        public IDictionary<string, string> GetValues(string x)
        {
            var dictionary = new Dictionary<string, string>();

            var match = _regex.Match(x);
            if (match.Success)
            {
                foreach (var name in Names)
                {
                    dictionary.Add(name, match.Groups[name].Value);
                }
            }

            return dictionary;
        }
    }
}
