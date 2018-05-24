using System;

namespace Matrix.Services
{
    public interface IMatrixStorage
    {
        Matrix Get(Guid key);

        void Put(Guid key, Matrix matrix);
    }
}
