using Matrix.Handlers;
using Matrix.Web.Services;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Linq;
using System.IO;

namespace Matrix.UnitTests
{
    [TestClass]
    public class TestForServices
    {
        const string contentType = "text/csv";

        public TestContext TestContext { get; set; }

        [TestMethod]
        public void MatrixSerialize()
        {
            var matrix = Matrix.Create<int>(10);

            var fileInfo = new FileInfo(Path.GetTempFileName() + ".csv");

            using (Stream file = fileInfo.Create())
            {
                using (var csv = new MatrixSerializer().Serialize<int>(matrix, contentType))
                {
                    Assert.IsTrue(csv.Length > 0);

                    csv.Seek(0, SeekOrigin.Begin);
                    csv.CopyTo(file);
                }
            }

            TestContext.AddResultFile(fileInfo.FullName);
        }

        [TestMethod]
        public void MatrixDeserialize()
        {
            new MatrixSerializer().Deserialize<int>(null, contentType);
        }
    }
}
