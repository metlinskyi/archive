using System.Collections.Generic;
using Lib.Data;
using Lib.Indexing.Prototypes;

namespace Lib.Indexing.Collectors
{
    public class InMemory<TEntity> : List<TEntity>, IIndexCollector<TEntity>
        where TEntity : IEntity
    {
    }
}
