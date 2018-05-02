using Lib.Mapping.Attributes;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Lib.Tests
{
    [TestClass]
    public class TestForRegExAttribute
    {
        [TestMethod]
        public void GetValues_Method()
        {
            var x = "30432.Something something something";

            var actual = new RegExAttribute(@"^(?<Number>\d+\.)\s*(?<Name>.*)$")
                .GetValues(x);

            Assert.IsNotNull(actual);
            Assert.AreEqual(2, actual.Keys.Count);
        }
    }
}
