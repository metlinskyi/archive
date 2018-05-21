using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Linq;

namespace Tasks.Tests
{
    [TestClass]
    public class SequentialTaskFactoryTest
    {
        private readonly List<int> _sequence = new List<int>(100);

        [TestMethod]
        public void Factory()
        {
            var sequence = Enumerable
                .Range(0, _sequence.Capacity)
                .ToArray();

            Task.WaitAll(sequence
                .Select((i) => MockTask(i))
                .ToArray());

            Assert.AreEqual(sequence.Count(), _sequence.Count);

            CollectionAssert.AreEqual(sequence, _sequence);
        }

        private Task MockTask(int index)
        {
            // task is a code to be executed (e.g. a delegate) 
            return SequentialTaskFactory.Factory.StartNew((state) =>
            {
                var i = (int)state;
                Task.Delay(new Random().Next(0, _sequence.Capacity << 2 / (i + 1))).Wait();
                lock (_sequence) { _sequence.Add(i); }
            }, index);
        }
    }
}