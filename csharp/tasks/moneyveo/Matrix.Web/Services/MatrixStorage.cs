using Matrix.Services;
using System;

namespace Matrix.Web.Services
{
    public class MatrixStorage : IMatrixStorage
    {
        /// <summary>
        /// Default content type for storage
        /// </summary>
        private const string ContentType = "text/csv";

        private readonly IMatrixSerializer _matrixSerializer;

        private readonly IFileService _fileService;

        public MatrixStorage(IMatrixSerializer matrixSerializer, IFileService fileService)
        {
            _matrixSerializer = matrixSerializer;
            _fileService = fileService;
        }

        public Matrix Get(Guid key)
        {
            using (var stream = _fileService.Get(key))
            {
                return _matrixSerializer.Deserialize(stream, ContentType);
            }
        }

        public void Put(Guid key, Matrix matrix)
        {
            using (var stream = _matrixSerializer.Serialize(matrix, ContentType))
            using (_fileService.Put(key, stream)) { };
        }
    }
}