using System;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Dir.Console.Args;
using Dir.Console;
using Dir.Ex;
using Dir.Ex.Actions;
using Ninject;
using System.Reflection;
using Dir.Ex.Prototypes;

namespace Dir.Test
{
    [TestClass]
    public class ConsoleTest
    {
        [TestMethod]
        public void ArgsTest()
        {
            string dir = Directory.GetCurrentDirectory();

            DirEx direx;

            string[] args = new string[]{
                dir,
                "all",
                "results.txt"
            };

            Assert.IsTrue(ArgsBinder.TryParse(args, out direx));
            Assert.IsNotNull(direx);
            Assert.AreEqual(dir, direx.Directory.FullName);
        }
    }
}
