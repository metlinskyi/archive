using Dir.Ex.Actions.Reverses;
using Dir.Ex.Prototypes;
using System.IO;

namespace Dir.Ex.Actions
{
    public class Reversed1Action : ReversedAction
    {
        public Reversed1Action() : base(true)
        {
        }
    }

    public class Reversed2Action : ReversedAction
    {
        public Reversed2Action() : base(false)
        {
        }
    }

    public class ReversedAction : Prototypes.IFileAction<string>
    {
        private Reverse strategy;

        public string SearchPattern
        {
            get { return "*.*"; }
        }

        public IFileActionResult<string> Output { get; set; }


        public ReversedAction(bool reverse)
        {
            if (reverse)
                strategy = new ReverseA();
            else
                strategy = new ReverseB();
        }

        public void Action(DirectoryInfo directory, FileInfo file)
        {
            string result = file.FullName
                .Replace(directory.FullName, string.Empty)
                .TrimStart(Path.DirectorySeparatorChar);

            strategy.Process(ref result);

            if (Output != null)
                Output.Add(result);
        }
    }

}
