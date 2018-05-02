using System;
using System.IO;
using System.Net.Http;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Xml.Serialization;

namespace Curl
{
    public static class Curl
    {
        public static CurlRequest Request(string url, HttpMethod httpMethod)
        {
            return new CurlRequest(url)
            {
                Method = httpMethod
            };
        }

        public static CurlRequest Get(string url = null)
        {
            return Request(url, HttpMethod.Get);
        }

        public static CurlRequest Post(string url = null)
        {
            return Request(url, HttpMethod.Post);
        }

        public static CurlRequest Delete(string url = null)
        {
            return Request(url, HttpMethod.Delete);
        }

        public static CurlRequest Put(string url = null)
        {
            return Request(url, HttpMethod.Put);
        }

        public static CurlRequest Parse(string command)
        {
            throw new NotImplementedException();
        }

        public static class MediaType
        {
            public const string Text = "text/plain";
            public const string Json = "application/json";
            public const string Xml = "application/xml";
            public const string Form = "application/x-www-form-urlencoded";
        }

        #region Static helpers

        /// <summary>
        /// Convert string to Base64
        /// </summary>
        public static string ToBase64(string value)
        {
            var bytes = Encoding.Default.GetBytes(value);
            return Convert.ToBase64String(bytes);
        }

        /// <summary>
        /// Json serialize
        /// </summary>
        public static string ToJson(object value)
        {
            var settings = new DataContractJsonSerializerSettings()
            {
                UseSimpleDictionaryFormat = true
            };

            var jsonSerializer = new DataContractJsonSerializer(value.GetType(), settings);

            using (var memory = new MemoryStream())
            {
                jsonSerializer.WriteObject(memory, value);

                var writer = new StreamWriter(memory);
                writer.Flush();

                memory.Position = 0;

                var reader = new StreamReader(memory);

                return reader.ReadToEnd();
            }
        }

        /// <summary>
        /// Xml serialize
        /// </summary>
        public static string ToXml(object value)
        {
            XmlSerializer xmlSerializer = new XmlSerializer(value.GetType());

            using (var writer = new StringWriter())
            {
                xmlSerializer.Serialize(writer, value);
                return writer.ToString();
            }
        }

        #endregion
    }
}