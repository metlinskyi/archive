using System;

namespace Matrix.Services
{
    public static class IMatrixStorageHelper
    {
        public static Guid Put<T>(this IMatrixStorage matrixStorage, Matrix<T> matrix)
        {
            Guid key = Guid.NewGuid();

            matrixStorage.Put<T>(key, matrix);

            return key;
        }

        public static Guid Put<T>(this IMatrixStorage matrixStorage, Matrix matrix)
        {
            Guid key = Guid.NewGuid();

            matrixStorage.Put<T>(key, (Matrix<T>)matrix);

            return key;
        }
    }
}
