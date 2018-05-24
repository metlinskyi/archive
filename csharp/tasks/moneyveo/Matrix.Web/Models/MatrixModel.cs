using System;

namespace Matrix.Web.Models
{
    public class MatrixModel
    {
        public Guid Id { get; set; }

        public uint Width { get; set; }

        public uint Height { get; set; }

        public static explicit operator MatrixModel(Matrix matrix)
        {
            return new MatrixModel
            {

            };
        }
    }
}