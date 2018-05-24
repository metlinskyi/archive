using Matrix.Services;
using System;
using System.IO;
using CsvHelper;

namespace Matrix.Web.Services
{
    /// <summary>
    /// Matrix serializer factory
    /// </summary>
    public class MatrixSerializer : IMatrixSerializer
    {
        private const string ContentType = "text/csv";

        public Matrix Deserialize(Stream stream, string contentType)
        {
            if (string.Compare(contentType, ContentType, true) != 0)
                throw new NotSupportedException(contentType);

            var csv = new CsvReader(new StreamReader(stream));

            return null;
        }

        public Stream Serialize(Matrix matrix, string contentType)
        {
            if (string.Compare(contentType, ContentType, true) != 0)
                throw new NotSupportedException(contentType);

            MemoryStream memoryStream = new MemoryStream();

            var csv = new CsvWriter(new StreamWriter(memoryStream));

            return memoryStream;
        }

    }
}