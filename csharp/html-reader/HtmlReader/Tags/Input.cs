using HtmlReader.Tags.Prototypes;

namespace HtmlReader.Tags
{
    [Tag("INPUT")]
    public class Input : Tag
    {
        [Attribute]
        public string Id { get; set; }

        [Attribute]
        public string Name { get; set; }

        [Attribute]
        public string Type { get; set; }

        [Attribute("onchange")]
        public string OnChange { get; set; }

        public override string ToString()
        {
            return Id;
        }
    }
}
