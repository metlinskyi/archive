using HtmlReader.Crawler;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

namespace HtmlReader.Tests
{
    [TestClass]
    public class www_sonycreativesoftware_com
    {
        const string host = "http://www.sonycreativesoftware.com";

        [TestMethod]
        public void loops()
        {
            const string buy = "/buy/";
            const string loops = "{0}/loops/{1}";
            const string api = "{0}/api/loopdetails.aspx?loopname={1}";

            var all = new Dictionary<string, Dictionary<string, IEnumerable<string>>>();

            all.Add("premiumcollection", null);
            all.Add("standardcollection", null);
            all.Add("classiccollection", null);


            // get loop pages

            foreach (string collection in all.Keys.ToArray())
            {
                string url = string.Format(loops, host, collection);

                var reader = new WebCrawler(url);
                Assert.IsTrue(!String.IsNullOrEmpty(reader.Source), collection + " access denied");

                // read url "Add to Cart"
                all[collection] = reader.Select<Tags.A>()
                                    .Distinct()
                                    .Where(i => !string.IsNullOrEmpty(i.Url) && i.Url.StartsWith(buy))
                                    .Select(i => i.Url.Replace(buy, string.Empty))
                                    .ToArray()
                                    .GroupBy(x => x, x => string.Empty)
                                    .ToDictionary(x => x.Key, x => Enumerable.Empty<string>());


                Assert.IsNotNull(all[collection], collection + " links not found");
            }


            // get mp3 links

            foreach (string collection in all.Keys.ToArray())
            {
                foreach (string loopname in all[collection].Keys.ToArray())
                {
                    string url = string.Format(api, host, loopname);

                    all[collection][loopname]
                        = new WebCrawler(url).Select<Tags.Div>()
                            .Where(i => string.CompareOrdinal(i.Class, "hide") == 0)
                            .Distinct()
                            .Select(i => i.Text.Trim())
                            .ToArray();
                }
            }

            // save links
            foreach (string collection in all.Keys.ToArray())
            {
                using (StreamWriter file =
                    new StreamWriter(Path.Combine(Directory.GetCurrentDirectory(), collection + "-links.txt")))
                {
                    foreach (string link in all[collection].SelectMany(i => i.Value).Select(i => i))
                        file.WriteLine(string.Format("{0}{1}", host, link));
                }
            }
        }

        [TestMethod]
        public void productionmusic()
        {
            string url = string.Format("{0}/productionmusic/ambient", host);

            var all = new Dictionary<string, string[]>();
            all.Add("genre", null);
            all.Add("use", null);
            all.Add("mood", null);
            all.Add("inst", null);

            Regex split = new Regex(@"'[\s\S][^']*'", RegexOptions.Compiled);

            // load HTML page
            var source = new WebCrawler(url).Select<Tags.Input>()
                    .Where(i => i.OnChange != null && i.Type != null && string.CompareOrdinal(i.Type, "checkbox") == 0)
                    .ToArray();

            foreach (var list in all.Keys.ToArray())
            {
                all[list] = source.Where(i => i.OnChange.Contains(list + "_list"))
                        .Distinct()
                        .Select(i => split.Matches(i.OnChange)[2].Value.Replace("'", string.Empty))
                        .OrderBy(i => i)
                        .ToArray();

                Assert.IsNotNull(list, list + " not found");
            }
        }
    }
}
