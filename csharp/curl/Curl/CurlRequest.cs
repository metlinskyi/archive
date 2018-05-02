using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace Curl
{
    public class CurlRequest
    {

        HttpClient _httpClient;

        HttpMessageHandler _httpClientHandler;

        HttpContent _httpContent;

        Uri _uri;

        String _mediaType;

        List<KeyValuePair<string, string>> _nameValueCollection;

        protected HttpClient Client
        {
            get
            {
                return _httpClient ?? (_httpClient = new HttpClient(ClientHandler));
            }
        }

        protected HttpMessageHandler ClientHandler
        {
            get
            {
                return _httpClientHandler ?? (_httpClientHandler = new HttpClientHandler());
            }
            set
            {
                _httpClientHandler = value;
            }
        }

        protected HttpContent Content
        {
            get
            {
                return _httpContent ?? (_httpContent = new FormUrlEncodedContent(_nameValueCollection));
            }
            set
            {
                _httpContent = value;
            }
        }
 
        public HttpMethod Method { get; set; }

        public CurlRequest(string url)
        {
            Url(url);

            _nameValueCollection = new List<KeyValuePair<string, string>>();
        }

        private T Responce<T>(Func<Task<HttpResponseMessage>, T> constructor) where T : CurlResponce
        {
            return constructor(GetResponseMessage());
        }

        private Task<HttpResponseMessage> GetResponseMessage()
        {
            Task<HttpResponseMessage> responseMessage;

            switch (Method.Method.ToUpper())
            {
                case "DELETE":
                    responseMessage = Client.DeleteAsync(_uri);
                    break;

                case "PUT":
                    responseMessage = Client.PutAsync(_uri, Content);
                    break;

                case "POST":
                    responseMessage = Client.PostAsync(_uri, Content);
                    break;

                case "GET":
                default:
                    {

                        if (_nameValueCollection.Any())
                        {
                            UriBuilder builder = new UriBuilder(_uri);

                            builder.Query = string.Join("&", _nameValueCollection
                                .Select(i => string.Format("{0}={1}", i.Key, i.Value))
                                .ToArray());

                            _uri = builder.Uri;
                        }

                        responseMessage = Client.GetAsync(_uri);
                    }
                    break;
            }

            return responseMessage;
        }

        public CurlRequest Url(string url)
        {
            if (!string.IsNullOrEmpty(url))
                _uri = new Uri(url);

            return this;
        }

        public CurlRequest Proxy(string address, short port)
        {
            WebProxy proxy = new WebProxy(address, port);

            var handler =  ClientHandler as HttpClientHandler;
            if (handler == null)
            {
                ClientHandler = handler = new HttpClientHandler();
            }

            handler.Proxy = proxy;

            return this;
        }

        public CurlRequest Authorization(string scheme, string parameter)
        {
            Client
                .DefaultRequestHeaders
                .Authorization = new AuthenticationHeaderValue(scheme, parameter);

            return this;
        }

        public CurlRequest Auth(string username, string password)
        {
            return Authorization("Basic", Curl.ToBase64(string.Format("{0}:{1}", username, password)));
        }

        public CurlRequest Data(string name, string value)
        {
            _nameValueCollection.Add(new KeyValuePair<string, string>(name, value));

            return this;
        }

        public CurlRequest Data(string name, object value)
        {
            return Data(name, value.ToString());
        }

        public CurlRequest DataType(string mediaType = Curl.MediaType.Text)
        {
            _mediaType = mediaType;

            return this;
        }

        public CurlResponce Responce()
        {
            return Responce<CurlResponce>(x => new CurlResponce(x));
        }

        public JsonResponce Json(string mediaType = Curl.MediaType.Json)
        {
            var dictionary = _nameValueCollection
                .ToDictionary(x => x.Key, x => x.Value);

            Content = new StringContent(Curl.ToJson(dictionary), Encoding.UTF8, mediaType);

            return DataType(mediaType)
                        .Responce<JsonResponce>(x => new JsonResponce(x));
        }

        public CurlResponce Xml(string name = "Curl", string mediaType = Curl.MediaType.Xml)
        {
            var xml = new XElement(name,
                _nameValueCollection.Select(x => new XElement(x.Key, x.Value))
            );

            Content = new StringContent(Curl.ToXml(xml), Encoding.UTF8, mediaType);

            return DataType(mediaType)
                        .Responce<CurlResponce>(x => new CurlResponce(x));
        }
    }
}
