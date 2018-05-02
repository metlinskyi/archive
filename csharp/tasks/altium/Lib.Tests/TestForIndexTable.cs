using System.IO;
using System.Linq;
using Lib.Data.Entities;
using Lib.Indexing;
using Lib.Indexing.Prototypes;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Lib.Tests
{
    [TestClass]
    public class TestForIndexTable
    {
        private readonly FileInfo _fileInfo;

        public TestForIndexTable()
        {
            string _filename =
                //@"D:\5ef243ee-91eb-47a3-a406-29850a13b0a7.txt"
                // @"D:\Apps\vs2015\Visual Studio 2015 MSDN\en_visual_studio_enterprise_2015_x86_x64_dvd_6850497.iso"
                "Data.txt";

            _fileInfo = new FileInfo(_filename);

            Assert.IsTrue(_fileInfo.Exists);
        }

        [TestMethod]
        public void Indexing_Method()
        {
            using (var indexTable = new IndexTable<FileEntity>().Selector(x => x.Name, x => x.Number))
            {
                foreach (var item in _fileInfo.AsEnumerable<FileEntity>())
                {
                    indexTable.AddToIndex(item);
                }

                indexTable.Sorting();

                var first = indexTable.First();
                var last = indexTable.Last();

                Assert.AreEqual(4, indexTable.Count);
                Assert.AreEqual('A', first.Index);
                Assert.AreEqual(2, first.Childs.Count());
                Assert.AreEqual('S', last.Index);
                Assert.AreEqual(1, last.Childs.Count());
            }
        }

        [TestMethod]
        public void IndexCollector_Method()
        {
            using (var collector = new IndexCollector<FileEntity>())
            {
                collector.Add(new FileEntity
                {
                    Raw = "TEST"
                });
            }
        }
    }
}
