using Dir.Ex.Prototypes;
using System.Linq;
using System.IO;

namespace Dir.Ex.Actions.Reverses
{
    internal class ReverseA : Reverse
    {
        public override void Process(ref string value)
        {
            string[] segments = value
                  .Split(Path.DirectorySeparatorChar)
                  .Reverse()
                  .ToArray();

            value = string.Join(Path.DirectorySeparatorChar.ToString(), segments);
        }
    }
}
