using System;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;

namespace Emailer.Prototypes
{
    public abstract class EmailTemplate<T> : Email
        where T : IEmailTemplateDto
    {
        private const string Tag = "Tag";

        private const char Delimiter = '.';

        private readonly StringBuilder _template;

        protected EmailTemplate(string template)
        {
            _template = new StringBuilder(template);

            Properties = typeof (T).GetProperties();
        }

        protected virtual Regex Regex =>
            new Regex($@"%%(?<{Tag}>.*)%%", RegexOptions.Multiline | RegexOptions.Compiled);

        protected virtual T Data
        {
            get
            {
                throw new NotSupportedException();
            }
        }

        public PropertyInfo[] Properties { get; }

        public override string Body => GetBody();

        private string GetBody()
        {
            var data = Data;

            foreach (Match match in Regex.Matches(_template.ToString()))
            {
                var current = Properties;

                var tag = match.Groups[0].Value;
                var names = match.Groups[Tag].Value.Split(Delimiter);

                object value = data;

                foreach (var name in names)
                {
                    value =
                        current
                            .FirstOrDefault(x => string.Compare(x.Name, name, StringComparison.OrdinalIgnoreCase) == 0)?
                            .GetValue(value);

                    if(value != null && names.Length > 1)
                        current = value.GetType().GetProperties();
                }

               _template.Replace(tag, value?.ToString() ?? string.Empty);
            }

            return _template.ToString();
        }
    }
}
