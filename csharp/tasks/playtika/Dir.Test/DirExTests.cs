using System;
using System.Collections;
using System.Linq;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Dir.Ex;
using Dir.Ex.Actions;
using Dir.Ex.Results;
using System.Collections.Generic;

namespace Dir.Test
{
    [TestClass]
    public class DirExTests
    {
        public DirectoryInfo Source
        {
            get
            {
                return new DirectoryInfo(Path.Combine(Directory.GetCurrentDirectory(), "Source"));
            }
        }

        [TestMethod]
        public void AllTest()
        {
            var result = new TestOutput();
            var action = new AllAction() { Output = result };

            DirEx direx = new DirEx(Source);
            direx.Action(action);

            Assert.AreEqual(6, result.List.Count);
            Assert.AreEqual(2, result.List.Count(i=>i.EndsWith(".txt")));
            Assert.AreEqual(3, result.List.Count(i => i.StartsWith("folder")));
        }

        [TestMethod]
        public void CppTest()
        {
            var result = new TestOutput();
            var action = new CppAction() { Output = result };

            DirEx direx = new DirEx(Source);
            direx.Action(action);

            Assert.AreEqual(4, result.List.Count);
            Assert.AreEqual(4, result.List.Count(i => i.EndsWith(" /")));
        }

        [TestMethod]
        public void Reversed1Test()
        {
            var result = new TestOutput();
            var action = new ReversedAction(true) { Output = result };

            DirEx direx = new DirEx(Source);
            direx.Action(action);

            Assert.AreEqual(6, result.List.Count);
            Assert.AreEqual(3, result.List.Count(i => i.EndsWith("folder")));
        }

        [TestMethod]
        public void Reversed2Test()
        {
            var result = new TestOutput();
            var action = new ReversedAction(false) { Output = result };

            DirEx direx = new DirEx(Source);
            direx.Action(action);

            Assert.AreEqual(6, result.List.Count);
            Assert.AreEqual(3, result.List.Count(i => i.EndsWith(new string("folder".Reverse().ToArray()))));
        }

        [TestMethod]
        public void FileResultTest()
        {
            FileInfo file = new FileInfo("result.txt");
            if (file.Exists)
            {
                file.Delete();
                file.Refresh();
            }

            using (var result = new FileResult(file))
            {
                var action = new AllAction() { Output = result };

                DirEx direx = new DirEx(Source);
                direx.Action(action);
            }

            file.Refresh();

            Assert.IsTrue(file.Exists);

            List<string> rows = new List<string>();

            using (StreamReader reader = new StreamReader(file.OpenRead()))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                    rows.Add(line);
            }

            Assert.AreEqual(6, rows.Count);
            Assert.AreEqual(2, rows.Count(i => i.EndsWith(".txt")));
            Assert.AreEqual(3, rows.Count(i => i.StartsWith("folder")));

            file.Delete();
        }
    }
}
