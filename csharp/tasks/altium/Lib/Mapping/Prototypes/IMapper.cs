using Lib.Data;

namespace Lib.Mapping.Prototypes
{
    public interface IMapper<out TDestination>
    {
        TDestination Mapping<TSource>(TSource source);
    }

    public interface IMapper
    {
        IEntity Mapping<TSource>(TSource source);
    }
}
