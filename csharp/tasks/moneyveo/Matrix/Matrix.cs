using Matrix.Handlers;
using System;
using System.Linq;
using System.Collections;
using System.Collections.Generic;

namespace Matrix
{
    public abstract class Matrix : IEnumerable
    {
        public uint Width { get; protected set; }

        public uint Height { get; protected set; }

        public abstract Type Type { get; }

        public abstract Matrix Handling(IMatrixHandler handler);

        public abstract IEnumerator GetEnumerator();

        public static Matrix Create<T>(uint size)
        {
            return new Matrix<T>(size, size);
        }
    }

    public class Matrix<T> : Matrix, IEnumerable<T>
    {
        private T[,] _matrix;

        private uint _centerX;

        private uint _centerY;

        public Matrix(uint width, uint height)
        {
            Width = width;
            Height = height;

            _centerX = Width >> 1;
            _centerY = Height >> 1;
        }

        public T this[uint x, uint y]
        {
            get
            {
                Validation(x, y);

                return _matrix[x, y];
            }
            set
            {
                Validation(x, y);

                if (_matrix == null)
                    _matrix = new T[Width, Height];

                _matrix[x, y] = value;
            }
        }

        public override Type Type => typeof(T);

        public override Matrix Handling(IMatrixHandler handler)
        {
            return handler.For(this);
        }

        #region IEnumerable<T>

        public override IEnumerator GetEnumerator()
        {
            for (uint x = 0; x < Width; x++)
            {
                for (uint y = 0; y < Height; y++)
                {
                    yield return _matrix[x, y];
                }
            }
        }

        IEnumerator<T> IEnumerable<T>.GetEnumerator()
        {
            for (uint x = 0; x < Width; x++)
            {
                for (uint y = 0; y < Height; y++)
                {
                    yield return _matrix[x, y];
                }
            }
        }

        #endregion

        private void Validation(uint x, uint y)
        {
            if (x >= Width)
                throw new System.IndexOutOfRangeException("X");

            if (y >= Height)
                throw new System.IndexOutOfRangeException("Y");
        }
    }
}
