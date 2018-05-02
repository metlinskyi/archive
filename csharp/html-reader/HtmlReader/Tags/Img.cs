using HtmlReader.Tags.Prototypes;
using System;

namespace HtmlReader.Tags
{
    [Tag("IMG")]
    public class Img : Tag
    {
        [Attribute]
        public string Src { get; set; }

        [Attribute]
        public string Alt { get; set; }

        public override string ToString()
        {
            return Src;
        }
    }
}
