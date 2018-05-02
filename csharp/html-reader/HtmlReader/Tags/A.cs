using HtmlReader.Tags.Prototypes;

namespace HtmlReader.Tags
{
    [Tag("A")]
    public class A : Tag
    {
        [Attribute("href")]
        public string Url { get; set; }

        [Attribute]
        public string Title { get; set; }

        [Content]
        public string Text { get; set; }

        public override string ToString()
        {
            return Url;
        }
    }
}