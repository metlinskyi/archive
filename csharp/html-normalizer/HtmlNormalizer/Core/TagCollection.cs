using System.Collections.Generic;
using System.Text;

namespace HtmlNormalizer.Core
{
    public class TagCollection
    {
        public string Base { get; set; }

        public StringBuilder Meta { get; private set; }
        public StringBuilder Links { get; private set; }
        public StringBuilder Styles { get; private set; }
        public StringBuilder Scripts { get; private set; }

        public TagCollection()
        {
            Meta = new StringBuilder();
            Links = new StringBuilder();
            Styles = new StringBuilder();
            Scripts = new StringBuilder();
        }

        public void AddScript(string value)
        {
            AddTo(Scripts, value);
        }

        public void AddStyle(string value)
        {
            AddTo(Styles, value);
        }

        public void AddLink(string value)
        {
            AddTo(Links, value);
        }

        public void AddMeta(string value)
        {
            AddTo(Meta, value);
        }

        private static void AddTo(List<string> list, string value)
        {
            list.Add(value);
        }

        private static void AddTo(StringBuilder builder, string value)
        {
            builder.Append(value);
        }
    }
}
