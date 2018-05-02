using Dir.Ex.Prototypes;
using System.Collections.Generic;

namespace Dir.Test
{
    public class TestOutput : IFileActionResult<string>
    {
        public List<string> List { get; private set; }

        public TestOutput()
        {
            List = new List<string>();
        }

        public void Add(string result)
        {
            List.Add(result);
        }
    }
}
