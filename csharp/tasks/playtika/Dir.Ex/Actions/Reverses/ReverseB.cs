using Dir.Ex.Prototypes;
using System.IO;
using System.Linq;

namespace Dir.Ex.Actions.Reverses
{
    internal class ReverseB : Reverse
    {
        public override void Process(ref string value)
        {
            string[] segments = value
                  .Split(Path.DirectorySeparatorChar)
                  .Reverse()
                  .Select(i => new string(i.Reverse().ToArray()))
                  .ToArray();

            value = string.Join(Path.DirectorySeparatorChar.ToString(), segments);
        }
    }
}
