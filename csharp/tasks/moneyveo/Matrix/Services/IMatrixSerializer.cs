using System.IO;

namespace Matrix.Services
{
    public interface IMatrixSerializer
    {
        Stream Serialize(Matrix matrix, string contentType);

        Matrix Deserialize(Stream stream, string contentType);
    }
}