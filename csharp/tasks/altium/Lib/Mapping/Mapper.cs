using System.Linq;
using System.Reflection;
using Lib.Data;
using Lib.Mapping.Mappers;
using Lib.Mapping.Prototypes;
using Lib.Properties;

namespace Lib.Mapping
{
    /// <summary>
    /// The common mapper
    /// </summary>
    public class Mapper<TEntity> : IMapper<TEntity>
        where TEntity : IEntity, new()
    {
        private readonly IMapper<TEntity> _mapper;

        public Mapper()
        {
            var attributes = GetCustomAttributes();
            if (attributes.Any())
                _mapper = new AttributeMapper<TEntity>(attributes);

            if(_mapper == null)
                throw new MapperException(Resources.MapperNotFound);
        }

        public TEntity Mapping<TSource>(TSource source)
        {
            try
            {
                return _mapper.Mapping(source);
            }
            catch (MapperException ex)
            {
                if(ex.Critical)
                    throw;

                return default(TEntity);
            }
        }

        private static IMapperAttribute[] GetCustomAttributes()
        {
            return typeof(TEntity)
                .GetCustomAttributes()
                .OfType<IMapperAttribute>()
                .ToArray();
        }
    }
}
