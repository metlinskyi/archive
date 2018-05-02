// ------------------------------------------------------------------------
// <author>
//	Roman Metlinskyi
// </author>
// ------------------------------------------------------------------------

namespace WhoIs
{
	using Microsoft.VisualStudio.TestTools.UnitTesting;
	using System.Linq;

	[TestClass]
	public class Faster
	{
		[TestMethod]
		public void ToArray()
		{
			var data = Engine.Enumerable().Select(i => i);

			Engine.Loop((i) => { var result = data.ToArray(); });
		}

		[TestMethod]
		public void ToList()
		{
			var data = Engine.Enumerable().Select(i => i);

			Engine.Loop((i) => { var result = data.ToList(); });
		}

		[TestMethod]
		public void ToDictionary()
		{
			var data = Engine.Enumerable().Select(i => i);

			Engine.Loop((i) => { var result = data.ToDictionary(x => x); });
		}

		[TestMethod]
		public void ToLookup()
		{
			var data = Engine.Enumerable().Select(i => i);

			Engine.Loop((i) => { var result = data.ToLookup(x => x); });
		}
	}
}
