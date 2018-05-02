using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Lib.Data;
using Lib.Indexing;
using Lib.Mapping;
using Lib.Properties;

namespace Lib
{
    public static class FileEnumerable
    {
        public static FileQuery<TSource> AsEnumerable<TSource>(this FileInfo fileInfo)
            where TSource : IEntity, new()
        {
            if (!fileInfo.Exists)
                throw new FileNotFoundException(Resources.FileNotFound, fileInfo.FullName);

            return new FileQuery<TSource>(fileInfo, new Mapper<TSource>());
        }

        private static IEnumerable<TSource> OrderBy<TSource>(this FileQuery<TSource> fileQuery, bool descending,
            params Func<TSource, string>[] selectors)
            where TSource : IEntity, new()
        {
            using (var index = new IndexTable<TSource>().Selector(selectors))
            {
                foreach (var item in fileQuery)
                {
                    index.AddToIndex(item);
                }

                foreach (var item in index.Sorting(fileQuery, descending))
                {
                    yield return item;
                }
            }
        }

        public static IEnumerable<TSource> OrderBy<TSource>(this FileQuery<TSource> fileQuery,
            params Func<TSource, string>[] selectors)
            where TSource : IEntity, new()
        {
            return fileQuery.OrderBy(false, selectors);
        }

        public static IEnumerable<TSource> OrderByDescending<TSource>(this FileQuery<TSource> fileQuery,
            params Func<TSource, string>[] selectors)
            where TSource : IEntity, new()
        {
            return fileQuery.OrderBy(true, selectors);
        }

        public static FileInfo ToFile<TSource>(this IEnumerable<TSource> fileQuery, FileInfo fileInfo)
            where TSource : IEntity
        {
            using (var file = fileInfo.AppendText())
            {
                foreach (var item in fileQuery)
                {
                    file.WriteLine(item.Raw);
                }
            }

            return fileInfo;
        }

        public static FileInfo ToFile<TSource>(this IEnumerable<TSource> fileQuery)
            where TSource : IEntity
        {
            return fileQuery.ToFile(CreateTempFile());
        }

        public static FileInfo ToFile<TSource>(this IEnumerable<TSource> fileQuery, DirectoryInfo directoryInfo)
            where TSource : IEntity
        {
            return fileQuery.ToFile(CreateTempFile());
        }

        private static FileInfo CreateTempFile(string extension = ".txt")
        {
            return new FileInfo(Path.GetTempPath() + Guid.NewGuid().ToString("D") + extension);
        }

        private static DirectoryInfo CreateTempDirectory(Guid guid)
        {
            return new DirectoryInfo(Path.Combine(Path.GetTempPath(), guid.ToString("D")));
        }
    }
}
