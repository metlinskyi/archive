using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.IO;
using System.IO.Compression;
using System.Reflection;

namespace HtmlNormalizer.Test
{
    [TestClass]
    public class Staging
    {
        private static HtmlNormalizer htmlNormalizer;

        static Staging()
        {
            htmlNormalizer = new HtmlNormalizer();
        }

        public string GetResource(string resource)
        {
            var assembly = Assembly.GetExecutingAssembly();
            using (Stream resourceStream = assembly.GetManifestResourceStream(resource))
                using (GZipStream gzip = new GZipStream(resourceStream, CompressionMode.Decompress))
                    using (StreamReader reader = new StreamReader(gzip))
                    {
                        return reader.ReadToEnd();
                    }

            throw new ArgumentException("Resource not found");
        }

        public string Test(string resource)
        {
            var source = GetResource(resource);

            var result = htmlNormalizer.Process(source);

            return result;
        }

        [TestMethod]
        public void PageTest()
        {
            Assert.IsNotNull(Test("HtmlNormalizer.Test.Resource.page.gz"));
        }

        [TestMethod]
        public void HomeTest()
        {
            Assert.IsNotNull(Test("HtmlNormalizer.Test.Resource.home.gz"));
        }

        [TestMethod]
        public void ProductTest()
        {
            Assert.IsNotNull(Test("HtmlNormalizer.Test.Resource.product.gz"));
        }

        [TestMethod]
        public void SearchTest()
        {
            Assert.IsNotNull(Test("HtmlNormalizer.Test.Resource.search.gz"));
        }
    }
}
