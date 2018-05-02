using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Lib.Data;
using Lib.Indexing.Prototypes;

namespace Lib.Indexing
{
    public class IndexTable<TEntity> : List<IndexNode>, IDisposable
        where TEntity : IEntity
    {
        private readonly Lazy<IEnumerable<PropertyInfo>> _properties;

        private Func<TEntity, string>[] _selectors;

        public IndexTable()
        {
            _properties = new Lazy<IEnumerable<PropertyInfo>>(GetProperties);
        }

        public void Dispose()
        {
            foreach (var collector in this.Select(x => x.Collector).OfType<IDisposable>())
            {
                collector.Dispose();
            }
        }

        public IndexTable<TEntity> Selector(params Func<TEntity, string>[] selector)
        {
            _selectors = selector;
            return this;
        }

        public TEntity AddToIndex(TEntity entity)
        {
            char[] index = _selectors?
                .Select(x => x(entity).ToString().FirstOrDefault())
                .ToArray() ?? _properties.Value
                    .Select(x => GetIndex(entity, x))
                    .ToArray();

            Index(index)?.Collector?.Add(entity);

            return entity;
        }

        public void Sorting(bool descending = false)
        {
            var index = Sort(this, descending).ToArray();

            Clear();
            AddRange(index);
        }

        public IEnumerable<TEntity> Sorting(IEnumerable<TEntity> entities, bool descending)
        {
            Sorting(descending);

            foreach (var entity in entities)
            {
                yield return entity;
            }
        }

        private IndexNode Index(IReadOnlyList<char> index)
        {
            int level = 0;
            IIndexNode parent = null;
            IndexNode node = null;
            IList<IndexNode> nodes = this;

            while (level < index.Count)
            {
                var value = index[level++];

                node = nodes
                    .SingleOrDefault(item => item.Index == value);

                if (node == null)
                {
                    node = new IndexNode
                    {
                        Index = value,
                        Occurrences = 1,
                        Parent = parent,
                        Childs = new List<IndexNode>()
                    };

                    nodes.Add(node);
                }
                else
                {
                    node.Occurrences++;
                }

                node.Collector = node.Collector ?? new IndexCollector<IEntity>();

                nodes =  node.Childs as IList<IndexNode> ?? new List<IndexNode>();
                parent = node;
            }

            return node;
        }

        private static IEnumerable<IndexNode> Sort(IEnumerable<IndexNode> nodes, bool descending)
        {
            var sorted = descending 
                ? nodes.OrderByDescending(x => x.Index) 
                : nodes.OrderBy(x => x.Index);

            foreach (var x in sorted.Where(x => x.Childs.Any()))
            {
                x.Childs = Sort(x.Childs as IEnumerable<IndexNode> ?? Enumerable.Empty<IndexNode>(), descending);
            }

            return sorted;
        }

        private static IEnumerable<PropertyInfo> GetProperties()
        {
            return typeof(TEntity)
                .GetProperties(BindingFlags.Public | BindingFlags.Instance)
                .Where(x => x.CanRead)
                .ToArray();
        }

        private static char GetIndex(TEntity entity, PropertyInfo propert)
        {
            return propert.GetValue(entity).ToString().FirstOrDefault();
        }
    }
}
