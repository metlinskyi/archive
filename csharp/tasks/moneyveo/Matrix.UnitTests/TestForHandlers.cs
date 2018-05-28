using System;
using System.Linq;
using Matrix.Handlers;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Matrix.UnitTests.Handlers
{
    [TestClass]
    public class TestForHandlers
    {
        [TestMethod]
        public void MatrixRandomizer()
        {
            var random = Enumerable.Range(1, 100).ToArray();
            var index = 0;

            var matrix = Matrix
                .Create<int>(10)
                .Handling(new MatrixRandomizer<int>(r => random[index++]));

            CollectionAssert.AreEqual(random, matrix.Cast<int>().ToArray());
        }

        [TestMethod]
        public void MatrixRotation()
        {
            var random = Enumerable.Range(1, 25).ToArray();
            var index = 0;

            var matrix = Matrix
                .Create<int>(5)
                .Handling(new MatrixRandomizer<int>(r => random[index++]))
                .Handling(new MatrixRotation<int>());

            Assert.AreEqual(random.Sum(), matrix.Cast<int>().Sum());
            Assert.AreEqual(21, (int)matrix.Get(0,0));
            Assert.AreEqual(1, (int)matrix.Get(4, 0));
            Assert.AreEqual(5, (int)matrix.Get(4, 4));
            Assert.AreEqual(25, (int)matrix.Get(0, 4));
        }
    }
}
