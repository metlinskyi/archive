using System;
using System.Collections.Generic;
using System.Linq;
using Lib.Data;
using Lib.Mapping.Prototypes;
using Lib.Properties;

namespace Lib.Mapping.Mappers
{
    /// <summary>
    /// The data mapper by entity attribute rules 
    /// </summary>
    internal class AttributeMapper<TEntity> : MapperBase, IMapper<TEntity>
        where TEntity : IEntity, new()
    {
        protected IMapperAttribute Attribute { get; }

        public AttributeMapper(IEnumerable<IMapperAttribute> attributes)
            : base(typeof (TEntity))
        {
            Attribute = attributes.FirstOrDefault();

            Properties = GetProperties()
                .Where(x => Attribute.Names.Contains(x.Name))
                .ToArray();
        }

        public TEntity Mapping<TSource>(TSource source)
        {
            return Mapping(source as string);
        }

        public TEntity Mapping(string source)
        {
            if (string.IsNullOrEmpty(source))
                throw new MapperException(Resources.MapperSourceIsEmpty, false);

            var values = Attribute.GetValues(source);
            if (values.Keys.Count == 0)
                throw new MapperException(Resources.MapperValuesNotFound, false);

            var instance = new TEntity {Raw = source};

            foreach (var property in Properties)
            {
                string value;
                if (values.TryGetValue(property.Name, out value))
                    property.SetValue(instance, value);
            }

            return instance;
        }
    }
}