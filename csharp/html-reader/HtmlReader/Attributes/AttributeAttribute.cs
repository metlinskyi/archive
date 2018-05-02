using HtmlReader.Attributes.Prototypes;
using System;

namespace HtmlReader
{
    [AttributeUsage(AttributeTargets.Property)]
    public class AttributeAttribute : HtmlElementAttribute
    {
        public string Name { get; set; }

        public AttributeAttribute()
        {
        }

        public AttributeAttribute(string name)
            : this()
        {
            Name = name;
        }
    }
}
