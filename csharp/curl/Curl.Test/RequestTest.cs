namespace Curl.Test
{
    using System;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using System.Diagnostics;

    [TestClass]
    public class RequestTest
    {
        [TestMethod]
        public void GET_Test()
        {
            var actual = Curl
                .Get("https://juliet.life/")
                .Data("s", "the best girl in universe")
                .Responce()
                .Content();

            Assert.IsTrue(actual.TrimStart().StartsWith("<!DOCTYPE html>"));
            Assert.IsTrue(actual.Contains("the best girl in universe"));

            Trace.WriteLine(actual);
        }

        [TestMethod]
        public void POST_Test()
        {
            var actual = Curl
                .Post("http://metlinskyi.com/api/")
                .Data("action", "ping")
                .Responce()
                .Content();

            Assert.AreEqual("OK", actual);

            Trace.WriteLine(actual);
        }

        [TestMethod]
        public void SOAP_Test()
        {
            var actual = Curl
                .Post("http://metlinskyi.com/soap/")
                .Data("grant_type", "client_credentials")
                .Data("scope", "basic user")
                .Xml()
                .Content();

            Trace.WriteLine(actual);
        }

        [TestMethod]
        public void AUTH_Test()
        {
            var actual = Curl
                .Post("https://api.neverbounce.com/v3/access_token")
                .Proxy("127.0.0.1", 8888)
                .Data("grant_type", "client_credentials")
                .Data("scope", "basic user")
                .Auth("username","password")
                .Responce()
                .Content();

            Trace.WriteLine(actual);
        }


        [TestMethod]
        public void GENERIC_Test()
        {
            var actual = Curl
                .Get("http://metlinskyi.com/api/")
                .Json()
                .Content<success>();

            Assert.AreEqual("OK", actual.status);

            Trace.WriteLine(actual.status);
        }

        public class success
        {
            public string status { get; set; }
            public object data { get; set; }
        }
    }
}
