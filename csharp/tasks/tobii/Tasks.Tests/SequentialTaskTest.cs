using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Linq;

namespace Tasks.Tests
{
    [TestClass]
    public class SequentialTaskTest
    {
        private List<int> _sequence;

        [TestMethod]
        public void Factory()
        {
            Stage(i => SequentialTaskFactory.Factory.StartNew(Dummy, i));
        }

        [TestMethod]
        public void Queue()
        {
            Stage(i => SequentialTaskQueue.Queue.StartNew(Dummy, i));
        }

        private void Stage(Func<int, Task> func)
        {
            _sequence = new List<int>(100);

            var sequence = Enumerable
                    .Range(0, _sequence.Capacity)
                    .ToArray();

            var a = sequence
                .Take(_sequence.Capacity >> 1)
                .Select((i) => func(i))
                .ToArray();

            var b = sequence
                .Skip(_sequence.Capacity >> 1)
                .Select((i) => func(i))
                .ToArray();

            Task.WaitAll(a);
            Task.WaitAll(b);

            Assert.AreEqual(sequence.Count(), _sequence.Count);

            CollectionAssert.AreEqual(sequence, _sequence);
        }

        /// <summary>
        /// task is a code to be executed (e.g. a delegate) 
        /// </summary>
        private void Dummy(object state)
        {
            var i = (int)state;
            Task.Delay(new Random().Next(0, _sequence.Capacity << 2 / (i + 1))).Wait();
            lock (_sequence) { _sequence.Add(i); }
        }
    }
}