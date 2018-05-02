using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Lib.Data;
using Lib.Indexing.Prototypes;

namespace Lib.Indexing.Collectors
{
    internal class InFile<TEntity> : IIndexCollector<TEntity>, IDisposable
        where TEntity : IEntity
    {

        private const string Extension = ".txt";

        private FileInfo File { get; }

        private StreamWriter Writer { get; set; }

        public InFile(FileInfo fileInfo)
        {
            File = fileInfo;
        }

        public InFile(string fileName) :
            this(new FileInfo(fileName))
        {
        }

        public InFile() :
            this(Path.GetTempPath() + Guid.NewGuid().ToString("D") + Extension)
        {
        }

        public void Dispose()
        {
            Writer?.Dispose();
            File.Delete();
        }

        public void Add(TEntity entity)
        {
            if (Writer == null)
                Writer = File.AppendText();

            Writer.WriteLine(entity.Raw);
        }

        public IEnumerator<TEntity> GetEnumerator()
        {
            return null;// File.AsEnumerable<TEntity>().GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        private static IEnumerable<TSource> Merging<TSource>(DirectoryInfo directoryInfo, int size)
            where TSource : IEntity, new()
        {
            IEnumerable<FileInfo> files;

            int skip = 0;

            do
            {
                files =
                    directoryInfo
                        .EnumerateFiles()
                        .Skip(skip)
                        .Take(size)
                        .ToArray();

                skip += size;

                foreach (var item in files.SelectMany(file => file.AsEnumerable<TSource>()))
                {
                    yield return item;
                }

            } while (files.Any());

            directoryInfo.Delete(true);
        }

        private static void Slicing<TSource>(FileQuery<TSource> fileQuery, int size, params Func<TSource, string>[] selectors)
            where TSource : IEntity, new()
        {
            int skip = 0;
            int take = size;
            Guid guid = Guid.NewGuid();

            DirectoryInfo directoryInfo = null;
            IEnumerable<TSource> part;

            do
            {
                part =
                    fileQuery
                        .Skip(skip)
                        .Take(take)
                        .OrderBy(selectors.First())
                        .ThenBy(selectors.Last())
                        .ToArray();

                skip += take;

                if (part.Any())
                    part.ToFile(directoryInfo);

            } while (part.Any());
        }
    }
}