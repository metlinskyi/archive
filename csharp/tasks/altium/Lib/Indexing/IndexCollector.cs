using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Lib.Data;
using Lib.Indexing.Collectors;
using Lib.Indexing.Prototypes;

namespace Lib.Indexing
{
    /// <summary>
    /// The IndexCollector wrappert
    /// </summary>
    /// <typeparam name="TEntity"></typeparam>
    public class IndexCollector<TEntity> : IIndexCollector<TEntity> , IDisposable 
        where TEntity : IEntity
    {
        private static readonly IDictionary<IndexCollectorEnum, Func<IIndexCollector<IEntity>>> Collectors;
        private static IIndexCollector<IEntity> GetCurrentCollector()
        {
            Func<IIndexCollector<IEntity>> collector;
            return Collectors.TryGetValue(IndexCollectorEnum.InFile, out collector) ? collector() : null;
        }

        static IndexCollector()
        {
            Collectors =
                new Dictionary<IndexCollectorEnum, Func<IIndexCollector<IEntity>>>
                {
                    {IndexCollectorEnum.InFile, ()=> new InFile<IEntity>()},
                    {IndexCollectorEnum.InMemory, ()=> new InMemory<IEntity>()}
                };
        }

        private IIndexCollector<IEntity> _collector;

        public IndexCollector()
        {
        }

        public void Add(TEntity entity)
        {
            if (_collector == null)
                _collector = GetCurrentCollector();

            _collector?.Add(entity);
        }

        public void Dispose()
        {
            (_collector as IDisposable)?.Dispose();
        }

        public IEnumerator<TEntity> GetEnumerator()
        {
            return _collector.OfType<TEntity>().GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
