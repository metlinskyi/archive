using System.Collections;
using System.Collections.Generic;
using System.IO;
using Lib.Data;
using Lib.Mapping.Prototypes;

namespace Lib
{
    public abstract class FileQuery
    {
        protected FileInfo FileInfo { get; }

        protected FileQuery(FileInfo fileInfo)
        {
            FileInfo = fileInfo;
        }
    }

    public class FileQuery<TSource> : FileQuery, IEnumerable<TSource>
        where TSource: IEntity
    {
        private IMapper<TSource> Mapper { get; }

        public FileQuery(FileInfo fileInfo, IMapper<TSource> mapper)
            : base(fileInfo)
        {
            Mapper = mapper;
        }

        public IEnumerator<TSource> GetEnumerator()
        {
            using (var buffere = new BufferedStream(FileInfo.OpenRead()))
            using (var reader = new StreamReader(buffere))
            {
                while (!reader.EndOfStream)
                {
                    yield return (TSource) Mapper.Mapping(reader.ReadLine());
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
