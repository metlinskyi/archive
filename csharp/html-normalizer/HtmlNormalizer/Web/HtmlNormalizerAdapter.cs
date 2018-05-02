using System.IO;
using System.Web.UI;
using System.Web.UI.Adapters;

namespace HtmlNormalizer.Web
{
    public class HtmlNormalizerAdapter : PageAdapter
    {
        private HtmlNormalizer _normalizer;

        public HtmlNormalizerAdapter()
        {
            _normalizer = new HtmlNormalizer();
        }

        protected override void Render(HtmlTextWriter writer)
        {
            using (StringWriter stringWriter = new StringWriter())
            using (HtmlTextWriter htmlWriter = new HtmlTextWriter(stringWriter, string.Empty))
            {
                htmlWriter.NewLine = string.Empty;

                base.Render(htmlWriter);

                writer.Write(_normalizer.Process(stringWriter.ToString()));
            }
        }
    }
}
