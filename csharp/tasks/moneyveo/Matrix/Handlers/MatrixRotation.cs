namespace Matrix.Handlers
{
    public class MatrixRotation<T> : IMatrixHandler
    {
        private readonly bool _clockwise;

        public MatrixRotation(bool clockwise = true)
        {
            _clockwise = clockwise;
            if (_clockwise == false)
                throw new System.NotImplementedException();
        }

        public Matrix For(Matrix matrix)
        {
            return For((Matrix<T>)matrix);
        }

        private Matrix For(Matrix<T> matrix)
        {
            uint n = matrix.Width;
            uint offset = n - 1;
            uint edge = n - 1;
            uint index;

            T[] values = new T[4];

            uint[,] indexes = new uint[values.Length, 2];

            int i, v;

            // спуск по главной диагонали до центра
            for (uint x = 0, y = 0; x < n >> 1; x++, y++, offset--, edge >>= 1)
            {
                // проход по грани 
                for (index = 0; index < edge; index++)
                {
                    // расчет индексов

                    indexes[0, 0] = x + index;
                    indexes[0, 1] = y;

                    indexes[1, 0] = offset;
                    indexes[1, 1] = y + index;

                    indexes[2, 0] = offset - index;
                    indexes[2, 1] = offset;

                    indexes[3, 0] = x;
                    indexes[3, 1] = offset - index;

                    // вращение 

                    for (i = 0; i < values.Length; i++)
                    {
                        values[i] = matrix[indexes[i, 0], indexes[i, 1]];
                    }

                    for (v= 0; v < values.Length; v++)
                    {
                        i = v + 1;
                        i = i >= values.Length || i < 0 ? 0 : i;

                        matrix[indexes[i, 0], indexes[i, 1]] = values[v];
                    }
                }
            }

            return matrix;
        }
    }
}
