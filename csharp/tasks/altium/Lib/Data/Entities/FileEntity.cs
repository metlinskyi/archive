using Lib.Mapping.Attributes;

namespace Lib.Data.Entities
{
    [RegEx(@"^(?<Number>\d+\.)\s*(?<Name>.*)$")]
    public class FileEntity : IEntity
    {
        public string Number { get; set; }
        public string Name { get; set; }
        public string Raw { get; set; }
        public override string ToString()
        {
            return Raw;
        }
    }
}
