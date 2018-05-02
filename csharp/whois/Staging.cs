// ------------------------------------------------------------------------
// <author>
//	Roman Metlinskyi
// </author>
// ------------------------------------------------------------------------

namespace WhoIs
{
	using System;
	using System.Diagnostics;
	using Microsoft.VisualStudio.TestTools.UnitTesting;

	[TestClass]
	public class Staging
	{
		private static readonly Stopwatch watcher = new Stopwatch();

		public static long SynthTest(TestContext context, string name, int x, Action action)
		{
			GC.Collect();
			GC.WaitForPendingFinalizers();

			action();

			lock (watcher)
			{
				int i;

				watcher.Reset();
				watcher.Start();

				for (i = 0; i < x; i++)
				{
					action();
				}

				watcher.Stop();
			}

			context.WriteLine("{0}(x{1}): {2} ms, {3} ticks", name, x, watcher.ElapsedMilliseconds, watcher.ElapsedTicks);

			return watcher.ElapsedTicks;
		}

		private TestContext testContext;
		public TestContext TestContext
		{
			get { return testContext; }
			set { testContext = value; }
		}

		[TestMethod]
		public void SynthTestEmpty()
		{
			int numberOfСycles = 100000;

			for (int i = 0; i < 10; i++)
			{
				SynthTest(TestContext, "F1", numberOfСycles, () =>
				{
				});

				SynthTest(TestContext, "F2", numberOfСycles, () =>
				{
				});

				SynthTest(TestContext, "F3", numberOfСycles, () =>
				{
				});

				TestContext.WriteLine(string.Empty);
			}
		}
	}
}
