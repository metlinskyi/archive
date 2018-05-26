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

            using (var csv = new CsvReader(new StreamReader(stream)))
            {
                csv.Configuration.IgnoreBlankLines = false;
                csv.Read();

                var matrix = Matrix.Create<T>(csv.Context.Record.Length);

                // rows
                for (uint y = 0; y < matrix.Height; y++)
                {
                    // columns
                    for (uint x = 0; x < matrix.Width; x++)
                    {
                        matrix[x, y] = csv.TryGetField<T>((int)x, out T val) ? val : default(T);
                    }

                    csv.Read();
                }

                return matrix;
            }
        }

        public Stream Serialize<T>(Matrix<T> matrix, string contentType)
        {
            if (string.Compare(contentType, ContentType, true) != 0)
                throw new NotSupportedException(contentType);

            var output = new StreamWriter(new MemoryStream());

            using (var csv = new CsvWriter(output, true))
            {
                // rows
                for (uint y = 0; y < matrix.Height; y++)
                {
                    // columns
                    for (uint x = 0; x < matrix.Width; x++)
                    {
                        csv.WriteField<T>(matrix[x, y]);
                    }

                    csv.NextRecord();
                    output.Flush();
                }
            }

            output.BaseStream.Seek(0, SeekOrigin.Begin);

            return output.BaseStream;
        }

    }
}