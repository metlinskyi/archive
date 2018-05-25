using System;

namespace Matrix.Services
{
    public interface IMatrixStorage
    {
        Matrix<T> Get<T>(Guid key);

        void Put<T>(Guid key, Matrix<T> matrix);
    }
}
