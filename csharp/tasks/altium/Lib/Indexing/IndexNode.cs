using System;
using System.Collections.Generic;
using Lib.Data;
using Lib.Indexing.Prototypes;

namespace Lib.Indexing
{
    public class IndexNode : IIndexNode
    {
        public char Index { get; set; }

        public IIndexCollector<IEntity> Collector { get; set; }

        #region IIndexNode implementation

        public long Occurrences { get; set; }

        public IEnumerable<IIndexNode> Childs { get; set; }

        public IIndexNode Parent { get; set; }

        #endregion

        public override string ToString()
        {
#if DEBUG
            return Index + "," + string.Join(string.Empty, Childs);
#endif
        }
    }
}
