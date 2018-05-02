using System;
using System.Text.RegularExpressions;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Dr.Parser.UnitTest
{
    
    /// <summary>
    ///This is a test class for RegExUtilityTest and is intended
    ///to contain all RegExUtilityTest Unit Tests
    ///</summary>
    [TestClass()]
    public class RegExUtilityTest
    {

        /// <summary>
        ///A test for RegexOptionsParse
        ///</summary>
        [TestMethod()]
        public void RegexOptionsParseTest()
        {
            const RegexOptions optionsExpected = RegexOptions.IgnoreCase | RegexOptions.Singleline;
            const String optionsString = "IgnoreCase|singleline|IgnoreWhitespace|FirstMatch";

            bool isFirstMatch = false;

            var options = new RegexOptions();

            try
            {
                RegExUtility.RegexOptionsParse(optionsString, ref options, ref isFirstMatch);
            }
            catch (Exception ex)
            {
                Assert.IsTrue(ex is ArgumentException);
            }

            Assert.AreEqual(optionsExpected, options);
            Assert.AreEqual(true, isFirstMatch);

        }
    }
}
