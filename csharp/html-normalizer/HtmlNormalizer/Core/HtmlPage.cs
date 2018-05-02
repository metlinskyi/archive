using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace HtmlNormalizer.Core
{
    public class HtmlPage
    {
        private StringBuilder _page;

        private Match _end;
        private Match _head;

        private int _headOffset = 0;
        private int _endOffset = 0;

        private int _index = 0;

        public HtmlPage(string source)
        {
            _page = new StringBuilder(source);

            _end = RegexCollection.End.Match(source);

            _endOffset = _end.Success ? _end.Index : _page.Length;

            _head = RegexCollection.Head.Match(source);

            _headOffset = _head.Success ? _head.Index + _head.Length : 0;
        }

        public override string ToString()
        {
            return _page.ToString();
        }

        public void InsertToHead(StringBuilder value)
        {
            _page.Insert(_headOffset, value);
        }

        public void InsertToHead(string value)
        {
            _page.Insert(_headOffset, value);
        }

        public void InsertToHead(List<string> tags)
        {
            for (_index = tags.Count - 1; _index >= 0; _index--)
                InsertToHead(tags[_index]);
        }

        public void InsertToEnd(StringBuilder value)
        {
            _page.Insert(_endOffset, value);
        }

        public void InsertToEnd(string value)
        {
            _page.Insert(_endOffset, value);
        }

        public void InsertToEnd(List<string> tags)
        {
            for (_index = tags.Count - 1; _index >= 0; _index--)
                InsertToEnd(tags[_index]);
        }
    }
}
