// ------------------------------------------------------------------------
// <author>
//	Roman Metlinskyi
// </author>
// ------------------------------------------------------------------------

namespace WhoIs
{
	using System;
	using System.Collections.Generic;
	using System.Diagnostics;

	public static class Engine
	{
		const int limit = 1000;

		public static int Loop(Action<int> action)
		{
			GC.Collect();
			GC.WaitForFullGCComplete();

			Stopwatch stopwatch = new Stopwatch();
			stopwatch.Start();

			for (int i = 0; i < limit; i++)
			{
				action(i);
			}

			stopwatch.Stop();

			Trace.WriteLine(stopwatch.Elapsed.Milliseconds);

			return stopwatch.Elapsed.Milliseconds;
		}

		public static IEnumerable<int> Enumerable()
		{
			for (int i = 0; i < limit; i++)
			{
				yield return i;
			}
		}
	}
}
