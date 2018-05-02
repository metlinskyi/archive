using System;
using System.IO;

namespace Dir.Ex.Prototypes
{
    public abstract class FileAction : Prototypes.IFileAction<string>
    {
        public string SearchPattern { get; private set; }

        public IFileActionResult<string> Output { get; set; }

        public virtual void Action(DirectoryInfo directory, FileInfo file)
        {
            throw new NotImplementedException();
        }

        public FileAction(string searchPattern)
        {
            SearchPattern = searchPattern;
        }
    }
}
