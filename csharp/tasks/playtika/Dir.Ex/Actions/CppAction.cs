using System.IO;

namespace Dir.Ex.Actions
{
    public class CppAction : Prototypes.FileAction
    {
        public CppAction() 
            : base("*.cpp")
        {
        }

        public override void Action(DirectoryInfo directory, FileInfo file)
        {
            string result = file.FullName
                .Replace(directory.FullName, string.Empty)
                .TrimStart(Path.DirectorySeparatorChar) + " /";

            if (Output != null)
                Output.Add(result);
        }
    }
}
