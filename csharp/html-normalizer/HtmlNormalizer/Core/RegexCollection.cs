using System.Text.RegularExpressions;

namespace HtmlNormalizer.Core
{
    public static class RegexCollection
    {
        public static readonly Regex Head;
        public static readonly Regex End;
        public static readonly Regex Tags;

        static RegexCollection()
        {
            string[] tags = new[] {
                @"<base.*?[^>]>([\s\S]?<\/base>)?",
                @"<meta.*?(\s\w+=[""].[^""]*[""])*[^>]*>([\s\S]?<\/meta>)?",
                @"<link.*?[^>]>([\s\S]?<\/link>)?",
                @"<style\b[^>]*>[\s\S]*?<\/style>",
                @"<script\b[^>]*>[\s\S]*?<\/script>"
            };

            RegexOptions defaultRegexOptions =
                RegexOptions.Compiled
                | RegexOptions.Multiline
                | RegexOptions.CultureInvariant
                ;

            Tags = new Regex(string.Join("|", tags), defaultRegexOptions);

            Head = new Regex(@"<head.*?>", defaultRegexOptions);

            End = new Regex(@"<\/body>", defaultRegexOptions);
        }
    }
}
