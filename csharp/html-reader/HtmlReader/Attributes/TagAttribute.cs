using HtmlReader.Attributes.Prototypes;
using System;

namespace HtmlReader
{
    [AttributeUsage(AttributeTargets.Class)]
    public class TagAttribute : HtmlElementAttribute
    {
        public string Name { get; set; }

        public TagAttribute()
        {
        }

        public TagAttribute(string name)
            : this()
        {
            Name = name;
        }
    }
}
