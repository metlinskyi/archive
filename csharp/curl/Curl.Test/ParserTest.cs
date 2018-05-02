namespace Curl.Test
{
    using System;
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class ParserTest
    {
        [TestMethod]
        public void ParseTest()
        {
            Curl.Parse(@"
                        curl -X POST -u username:password http://metlinskyi.com/api/
                            -d grant_type=client_credentials
                            -d scope=basic+user
                            -p 127.0.0.1:8888 
                    ")
                    .Responce()
                    .Content();
        }
    }
}
