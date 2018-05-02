using System;
using System.Net;
using System.Text;

namespace HtmlReader.Crawler
{
    public class WebCrawler : HtmlReader
    {
        public WebCrawler(string source, Encoding encoding)
            : base(GetHtmlSource(source, encoding))
        {
        }
        public WebCrawler(string source)
            : base(GetHtmlSource(source, Encoding.UTF8))
        {
        }

        public static string GetHtmlSource(string source, Encoding encoding)
        {
            Uri uri;
            if (TryGetUri(source, out uri))
            {
                byte[] page = new WebClient().DownloadData(uri);
                return encoding.GetString(page);
            }

            return source;
        }

        public static bool TryGetUri(string source, out Uri uri)
        {
            uri = null;

            try
            {
                uri = new UriBuilder(source).Uri;
            }
            catch
            {
                return false;
            }

            return true;
        }
    }
}
