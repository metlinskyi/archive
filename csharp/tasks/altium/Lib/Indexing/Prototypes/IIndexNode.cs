using System.Collections.Generic;

namespace Lib.Indexing.Prototypes
{
    public interface IIndexNode
    {
        long Occurrences { get; }

        IIndexNode Parent { get; set; }

        IEnumerable<IIndexNode> Childs { get; set; }
    }
}
