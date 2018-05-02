using HtmlReader.Attributes.Prototypes;
using HtmlReader.Tags.Prototypes;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text.RegularExpressions;

namespace HtmlReader
{
    public class HtmlReader<T> : IEnumerable<T>
        where T : ITag, new()
    {
        static TagAttribute _tag;

        static KeyValuePair<PropertyInfo, AttributeAttribute>[] _attributes;

        static KeyValuePair<PropertyInfo, ContentAttribute>[] _contents;

        static PropertyInfo[] _properties;

        static HtmlReader()
        {
            // set tag name
            _tag = Attribute.GetCustomAttributes(typeof(T))
                .OfType<TagAttribute>()
                .FirstOrDefault();

            // set attributes 
            _attributes = GetCustomAttributes<AttributeAttribute>();

            // set content
            _contents = GetCustomAttributes<ContentAttribute>();
        }

        private readonly HtmlReader _reader;

        public HtmlReader(HtmlReader reader)
        {
            _reader = reader;
        }

        public IEnumerator<T> GetEnumerator()
        {
            foreach (Match tag in RegexCollection.TagEx(_tag.Name).Matches(_reader.Source))
            {
                string value = tag.Groups[1].Value;

                T item = new T();

                // get all attributes for current tag
                foreach (var a in _attributes)
                {
                    Match attr = RegexCollection
                        .AttributeEx(string.IsNullOrEmpty(a.Value.Name) ? a.Key.Name : a.Value.Name)
                        .Match(value);

                    if (attr.Success)
                    {
                        a.Key.SetValue(item, attr.Groups[1].Value, null);
                    }
                }

                // get tag content
                foreach (var content in _contents.Where(x => x.Key != null))
                {
                    if (content.Value.TextOnly)
                    {
                        string innerText = RegexCollection.Trim
                            .Replace(RegexCollection.Clean.Replace(value, " "), " ")
                            .Trim();

                        content.Key.SetValue(item, innerText, null);
                    }
                    else
                    {
                        string innerHtml = RegexCollection.ContentEx(_tag.Name)
                            .Replace(value, string.Empty)
                            .Trim();

                        content.Key.SetValue(item, innerHtml, null);
                    }
                }

                yield return item;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private static KeyValuePair<PropertyInfo, TAttribute>[] GetCustomAttributes<TAttribute>()
            where TAttribute : HtmlElementAttribute
        {
            if(_properties == null)
            {
                // get tag properties 
                _properties = typeof(T).GetProperties()
                   .Where(property => property.CanWrite)
                   .ToArray();
            }

            return (
                    from property in _properties
                    from attribute in property.GetCustomAttributes(true).OfType<TAttribute>()
                    select new KeyValuePair<PropertyInfo, TAttribute>(property, attribute)
                    ).ToArray();
        }
    }

    public class HtmlReader
    {
        public string Source { get; }

        private HtmlReader()
        {
        }

        public HtmlReader(string source)
        {
            Source = source;
        }

        public override string ToString()
        {
            return Source;
        }
    }
}
