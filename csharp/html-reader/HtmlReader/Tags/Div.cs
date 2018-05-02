using HtmlReader.Tags.Prototypes;

namespace HtmlReader.Tags
{
    [Tag("DIV")]
    public class Div : Tag
    {
        [Attribute]
        public string Id { get; set; }

        [Attribute]
        public string Class { get; set; }

        [Content]
        public string Text { get; set; }

        public override string ToString()
        {
            return Id;
        }
    }
}