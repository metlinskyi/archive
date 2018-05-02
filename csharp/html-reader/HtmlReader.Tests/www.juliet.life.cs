using HtmlReader.Crawler;
using HtmlReader.Tags;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;

namespace HtmlReader.Tests
{
    [TestClass]
    public class www_juliet_life
    {
        [TestMethod]
        public void home()
        {
            // load real HTML page
            var reader = new WebCrawler("https://juliet.life/");

            Assert.IsTrue(!string.IsNullOrEmpty(reader.ToString()), "juliet.life access denied");

            // read link tags
            var links = reader.Select<A>().Distinct().ToArray();

            Assert.IsTrue(links.Length > 0);


            // read images tags
            var images = reader.Select<Img>().Distinct().ToArray();

            Assert.IsTrue(images.Length > 0);


            // read body
            var body = reader.Select<Body>().First().Content;

            Assert.IsTrue(!string.IsNullOrEmpty(body));
            Assert.IsTrue(body.Contains("Самая лучшая девушка во Вселенной!"));
        }
    }
}
