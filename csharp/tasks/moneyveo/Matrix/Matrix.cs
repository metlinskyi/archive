using Matrix.Handlers;
using System;
using System.Collections;
using System.Collections.Generic;

namespace Matrix
{
    public abstract class Matrix : IEnumerable
    {
        public uint Width { get; protected set; }

        public uint Height { get; protected set; }

        public abstract Type Type { get; }

        public abstract object Get(uint x, uint y);

        public abstract Matrix Handling(IMatrixHandler handler);

        public abstract IEnumerator GetEnumerator();

        public static Matrix<T> Create<T>(uint size)
        {
            return new Matrix<T>(size, size);
        }

        public static Matrix<T> Create<T>(int size)
        {
            return Create<T>((uint)size);
        }
    }

    public class Matrix<T> : Matrix, IEnumerable<T>
    {
        private readonly T[,] _matrix;

        internal Matrix(uint width, uint height)
        {
            _matrix = new T[width, height];

            Width = width;
            Height = height;
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

                _matrix[x, y] = value;
            }
        }

        public override Type Type => typeof(T);

        public override object Get(uint x, uint y) => this[x, y];

        public override Matrix Handling(IMatrixHandler handler) => handler.For(this);

        #region IEnumerable<T>

        public override IEnumerator GetEnumerator() => this as IEnumerator<T>;

        IEnumerator<T> IEnumerable<T>.GetEnumerator()
        {
            for (uint y = 0; y < Height; y++)
            {
                for (uint x = 0; x < Width; x++)
                {
                    yield return _matrix[x, y];
                }
            }
        }

        #endregion

        private void Validation(uint x, uint y)
        {
            if (x >= Width)
                throw new IndexOutOfRangeException("X");

            if (y >= Height)
                throw new IndexOutOfRangeException("Y");
        }

    }
}
