using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Switch;

namespace Switch.Test
{
    [TestClass]
    public class UnitTest1
    {
        [TestMethod]
        public void TestMethod1()
        {
	        var compareObject = new { Id = 1, Value = "test" };
	        var compareArray = new object[] {String.Empty, "TEST"};

	        var yes = false;
			var no = true;

	        var _switch =
		        new Switch<object>()

					.Case((constant, value) =>
			        {

			        }, 1)

					.Case((constant, value) =>
			        {
						yes = true;
			        }, compareArray)

					.Case((constant, value) =>
					{
						no = false;
					}, compareObject.Value, "Yes", 100.00)

			        .Default((value) =>
			        {

			        });

            _switch.Invoke("TEST", () => string.Empty);

			Assert.IsTrue(yes);
			Assert.IsTrue(no);
        }
    }
}
