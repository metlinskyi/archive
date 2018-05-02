using System;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;

namespace Curl
{
    public class CurlResponce
    {
        Task<HttpResponseMessage> _httpResponse;

        protected HttpContent HttpContent { get; set; }

        public string Data { get; set; }

        public CurlResponce(Task<HttpResponseMessage> httpResponse)
        {
            if (httpResponse == null)
                throw new ArgumentNullException("httpResponse");

            _httpResponse = httpResponse;

            _httpResponse.Wait();

            HttpContent = _httpResponse.Result.Content;
        }

        public virtual string Content()
        {
            Task<Stream> stream = HttpContent.ReadAsStreamAsync();

            stream.Wait();

            using (var reader = new StreamReader(stream.Result))
            {
                return Data = reader.ReadToEnd();
            }
        }

    }
}
