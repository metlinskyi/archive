using CsvHelper;
using Matrix.Services;
using System;
using System.IO;

namespace Matrix.Web.Services
{
    /// <summary>
    /// Matrix serializer factory
    /// </summary>
    public class MatrixSerializer : IMatrixSerializer
    {
        private const string ContentType = "text/csv";

        public Matrix<T> Deserialize<T>(Stream stream, string contentType)
        {
            if (string.Compare(contentType, ContentType, true) != 0)
                throw new NotSupportedException(contentType);

            var csv = new CsvReader(new StreamReader(stream));

            var matrix = Matrix.Create<T>(csv.Context.ColumnCount);

            uint x = 0;
            uint y = 0;

            do
            {
                while (csv.TryGetField<T>((int)x++, out T val))
                {
                    matrix[x, y] = val;
                }

                x = 0;

            } while (csv.Read() && y++ < matrix.Height);

            return null;
        }

        public Stream Serialize<T>(Matrix<T> matrix, string contentType)
        {
            if (string.Compare(contentType, ContentType, true) != 0)
                throw new NotSupportedException(contentType);

            MemoryStream memoryStream = new MemoryStream();

            var csv = new CsvWriter(new StreamWriter(memoryStream));

            // rows
            for (uint y = 0; y < matrix.Height; y++)
            {
                // columns
                for (uint x = 0; x < matrix.Width; x++)
                {
                    csv.WriteField<T>(matrix[x, y]);
                }

                csv.NextRecord();
            }

            return memoryStream;
        }

    }
}