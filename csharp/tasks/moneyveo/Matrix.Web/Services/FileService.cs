using System;
using System.IO;
using System.Web;

namespace Matrix.Web.Services
{
    public class FileService : IFileService
    {
        public Stream Get(Guid key)
        {
            string path = GetPath(key);

            var fileInfo = new FileInfo(path);
            if (!fileInfo.Exists)
                throw new FileNotFoundException(key.ToString());

            return fileInfo.OpenRead();
        }

        public Stream Put(Guid key, Stream stream)
        {
            string path = GetPath(key);

            var fileInfo = new FileInfo(path);
            if (fileInfo.Exists)
                fileInfo.Delete();

            using (Stream file = fileInfo.OpenWrite())
            {
                stream.Seek(0, SeekOrigin.Begin);
                stream.CopyTo(file);
            }

            return stream;
        }

        protected virtual string GetPath(Guid key)
        {
            return HttpContext.Current.Server.MapPath($"~/App_Data/{key}");
        }
    }
}
