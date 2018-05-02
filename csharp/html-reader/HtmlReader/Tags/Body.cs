using HtmlReader.Tags.Prototypes;

namespace HtmlReader.Tags
{
    [Tag("BODY")]
    public class Body : Tag
    {
        [Content(true)]
        public string Content { get; set; }
    }
}
