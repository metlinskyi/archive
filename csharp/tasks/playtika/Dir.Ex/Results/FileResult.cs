using Dir.Ex.Prototypes;
using System;
using System.IO;

namespace Dir.Ex.Results
{
    public class FileResult : IFileActionResult<string>, IDisposable
    {
        private static object locker = new object();

        private Lazy<StreamWriter> fileStream;

        protected FileInfo OutputFile { get; private set; }

        public FileResult(FileInfo outputFile)
        {
            OutputFile = outputFile;

            fileStream = new Lazy<StreamWriter>(() =>
            {
                return new StreamWriter(OutputFile.Exists ? OutputFile.OpenWrite() : OutputFile.Create());
            }, true);
        }

        public FileResult(string fileName)
            : this(new FileInfo(fileName))
        {
        }

        public void Add(string result)
        {
            lock(locker)
                fileStream.Value.WriteLine(result);
        }

        public void Dispose()
        {
            if (fileStream == null || !fileStream.IsValueCreated)
                return;

            fileStream.Value.Close();
            fileStream.Value.Dispose();
        }
    }
}
