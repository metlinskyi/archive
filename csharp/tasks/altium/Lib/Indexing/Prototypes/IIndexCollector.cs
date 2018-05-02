using System.Collections.Generic;
using Lib.Data;

namespace Lib.Indexing.Prototypes
{
    public interface IIndexCollector<TEntity> : IEnumerable<TEntity>
        where TEntity : IEntity 
    {
        void Add(TEntity entity);
    }
}
