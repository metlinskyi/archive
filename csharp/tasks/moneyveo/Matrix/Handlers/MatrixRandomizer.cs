using System;

namespace Matrix.Handlers
{
    public class MatrixRandomizer<T> : IMatrixHandler
    {
        private readonly Func<Random, T> _func;

        public MatrixRandomizer(Func<Random,T> func)
        {
            _func = func;
        }

        public Matrix For(Matrix matrix)
        {
            return For((Matrix<T>)matrix);
        }

        private Matrix For(Matrix<T> matrix)
        {
            var rnd = new Random();

            for (uint x = 0; x < matrix.Width; x++)
            {
                for (uint y = 0; y < matrix.Height; y++)
                {
                    matrix[x, y] = _func(rnd);
                }
            }

            return matrix;
        }

    }
}
