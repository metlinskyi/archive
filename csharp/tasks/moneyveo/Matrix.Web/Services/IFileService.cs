using System;
using System.IO;

namespace Matrix.Web.Services
{
    public interface IFileService
    {
        Stream Get(Guid key);

        Stream Put(Guid key, Stream stream);
    }
}
