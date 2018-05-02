using HtmlReader.Attributes.Prototypes;
using System;

namespace HtmlReader
{
    [AttributeUsage(AttributeTargets.Property)]
    public class ContentAttribute : HtmlElementAttribute
    {
        public bool TextOnly { get; }

        public ContentAttribute()
        {
        }

        public ContentAttribute(bool textOnly)
            : this()
        {
            TextOnly = textOnly;
        }
    }
}
