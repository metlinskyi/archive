using Matrix.Web.Services;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.IO;
using System.Linq;
using System.Reflection;

namespace Matrix.UnitTests
{
    [TestClass]
    public class TestForServices
    {
        private static Guid TempKey = Guid.NewGuid();

        private static string TempFile = Path.Combine( Path.GetTempPath(), TempKey.ToString());

        private class TestFileService : FileService
        {
            protected override string GetPath(Guid key)
            {
                return TempFile;
            }
        }

        public TestContext TestContext { get; set; }

        [TestMethod]
        public void FileService()
        {
            var file = new FileInfo(TempFile);
            if (file.Exists)
                file.Delete();

            IFileService service = new TestFileService();

            using (var stream = Assembly.GetExecutingAssembly().GetManifestResourceStream($"Matrix.UnitTests.Matrix.csv"))
            {
                using (service.Put(TempKey, stream))
                {
                    file.Refresh();

                    Assert.IsTrue(file.Exists);
                    Assert.AreEqual(stream.Length, file.Length);
                };
            }

            using (var stream = service.Get(TempKey))
            {
                Assert.AreEqual(stream.Length, file.Length);
            }
        }

        [TestMethod]
        public void CsvMatrixSerialize()
        {
            var matrix = Matrix.Create<int>(10);

            var fileInfo = new FileInfo(Path.GetTempFileName() + ".csv");

            using (Stream file = fileInfo.Create())
            {
                using (var csv = new MatrixSerializer().Serialize<int>(matrix, "text/csv"))
                {
                    Assert.IsTrue(csv.Length > 0);

                    csv.Seek(0, SeekOrigin.Begin);
                    csv.CopyTo(file);
                }
            }

            TestContext.AddResultFile(fileInfo.FullName);
        }

        [TestMethod]
        public void CsvMatrixDeserialize()
        {
            using (var stream = Assembly.GetExecutingAssembly().GetManifestResourceStream($"Matrix.UnitTests.Matrix.csv"))
            {
                Assert.IsNotNull(stream, "Matrix source is null");

                var matrix = new MatrixSerializer().Deserialize<int>(stream, "text/csv");

                Assert.IsNotNull(matrix);
                Assert.AreEqual(43, matrix.Cast<int>().Sum());
            }
        }

        [TestCleanup]
        public void Cleanup()
        {
            File.Delete(TempFile);
        }
    }
}
