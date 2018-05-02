using System.IO;
using System.Net.Http;
using System.Runtime.Serialization.Json;
using System.Threading.Tasks;

namespace Curl
{
    public class JsonResponce : CurlResponce
    {
        public JsonResponce(Task<HttpResponseMessage> httpResponse)
            : base(httpResponse)
        {

        }

        public T Content<T>() where T : new()
        {
            var settings = new DataContractJsonSerializerSettings()
            {
                UseSimpleDictionaryFormat = true
            };

            var jsonSerializer = new DataContractJsonSerializer(typeof(T), settings);

            Task<Stream> stream = HttpContent.ReadAsStreamAsync();

            stream.Wait();
            
            return (T)jsonSerializer.ReadObject(stream.Result);
        }
    }
}
