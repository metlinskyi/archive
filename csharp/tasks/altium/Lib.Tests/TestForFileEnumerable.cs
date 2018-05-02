using System.IO;
using System.Linq;
using Lib.Data.Entities;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Lib.Tests
{
    [TestClass]
    public class TestForFileEnumerable
    {
        private const string First = "415. Apple";
        private const string Last = "2. Banana is yellow";

        readonly FileInfo _fileInfo;

        public TestForFileEnumerable()
        {
            string _filename =
                 //@"D:\5ef243ee-91eb-47a3-a406-29850a13b0a7.txt"
                "Data.txt"
             ;

            _fileInfo = new FileInfo(_filename);

            Assert.IsTrue(_fileInfo.Exists);
        }

        [TestMethod]
        public void AsEnumerable_Method()
        {
            Assert.AreEqual(5, _fileInfo.AsEnumerable<FileEntity>().Count());
        }

        [TestMethod]
        public void AsEnumerable_Generic_Method()
        {
            FileEntity[] actual = _fileInfo
                .AsEnumerable<FileEntity>()
                .ToArray();

            Assert.AreEqual(5, actual.Length);

            var first = actual.First();
            var last = actual.Last();

            Assert.AreEqual(First, $"{first.Number} {first.Name}");
            Assert.AreEqual(Last, $"{last.Number} {last.Name}");
        }

        [TestMethod]
        public void ToFile_Method()
        {
            FileInfo actual = _fileInfo
                .AsEnumerable<FileEntity>()
                .ToFile();

            Assert.IsTrue(actual.Exists);
            Assert.IsTrue(actual.Length > 0);
        }

        [TestMethod]
        public void AsEnumerable_OrderBy_Method()
        {
            var query = _fileInfo
                .AsEnumerable<FileEntity>()
                .OrderBy(x => x.Name, x => x.Number);

            Assert.AreEqual(5, query.Count());
        }

        [TestMethod]
        public void AsEnumerable_OrderBy_ToFile_Method()
        {
            FileInfo actual = _fileInfo
                .AsEnumerable<FileEntity>()
                .OrderBy(x => x.Name, x => x.Number)
                .ToFile();

            Assert.IsTrue(actual.Exists);
            Assert.IsTrue(actual.Length > 0);
        }
    }
}
