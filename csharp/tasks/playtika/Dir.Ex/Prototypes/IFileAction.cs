using System.IO;

namespace Dir.Ex.Prototypes
{
    public interface IFileAction<T>
    {
        string SearchPattern { get; }

        IFileActionResult<T> Output { get; set; }

        void Action(DirectoryInfo directory, FileInfo file);
    }
}
