using System;

namespace Matrix.Services
{
    public static class IMatrixStorageHelper
    {
        public static Guid Put(this IMatrixStorage matrixStorage, Matrix matrix)
        {
            Guid key = Guid.NewGuid();

            matrixStorage.Put(key, matrix);

            return key;
        }
    }
}
