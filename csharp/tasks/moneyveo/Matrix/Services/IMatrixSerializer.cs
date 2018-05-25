using System.IO;

namespace Matrix.Services
{
    public interface IMatrixSerializer
    {
        Stream Serialize<T>(Matrix<T> matrix, string contentType);

        Matrix<T> Deserialize<T>(Stream stream, string contentType);
    }
}