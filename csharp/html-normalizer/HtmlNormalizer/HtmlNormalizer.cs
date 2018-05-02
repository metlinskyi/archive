namespace HtmlNormalizer
{
    public class HtmlNormalizer
    {
        internal static class Keyword
        {
            public const string normalizerIgnore = "data-normalizer=\"ignore\"";
            public const string tagScript = "<script";
            public const string tagStyle = "<style";
            public const string tagLink = "<link";
            public const string tagMeta = "<meta";
            public const string tagBase = "<base";
        }

        public virtual string Process(string source)
        {
            var tags = new Core.TagCollection();

            // Get all tags
            source = Core.RegexCollection.Tags.Replace(source.Trim(), (m) =>
            {
                if (m.Value.StartsWith(Keyword.tagScript, false, null))
                {
                    if (m.Value.Contains(Keyword.normalizerIgnore))
                        return m.Value;

                    tags.AddScript(m.Value);
                }
                else if (m.Value.StartsWith(Keyword.tagStyle, false, null))
                {
                    tags.AddStyle(m.Value);
                }
                else if (m.Value.StartsWith(Keyword.tagLink, false, null))
                {
                    tags.AddLink(m.Value);
                }
                else if (m.Value.StartsWith(Keyword.tagMeta, false, null))
                {
                    tags.AddMeta(m.Value);
                }
                else if (m.Value.StartsWith(Keyword.tagBase, false, null))
                {
                    tags.Base = m.Value;
                }

                return string.Empty;
            });

            var page = new Core.HtmlPage(source);

            // Scripts insert to end
            page.InsertToEnd(tags.Scripts);

            // styles insert to head
            page.InsertToHead(tags.Styles);

            // links insert to head
            page.InsertToHead(tags.Links);

            // meta insert to head
            page.InsertToHead(tags.Meta);

            // base insert to head
            page.InsertToHead(tags.Base);

            return page.ToString();
        }
    }
}