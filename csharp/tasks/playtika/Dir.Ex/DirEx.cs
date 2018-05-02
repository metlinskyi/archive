using Dir.Ex.Prototypes;
using System.IO;
using System.Threading.Tasks;

namespace Dir.Ex
{
    public class DirEx
    {
        public DirectoryInfo Directory { get; private set; }

        public delegate void ActionHandler<T>(T item) where T : FileSystemInfo;

        public event ActionHandler<FileInfo> OnFile;
        public event ActionHandler<DirectoryInfo> OnDirectory;

        public DirEx(DirectoryInfo directory)
        {
            Directory = directory;
        }

        public void Action<T>(IFileAction<T> action)
        {
            Task handler = InternalActionHandler(Directory, action);

            // do something else ...

            handler.Wait();
        }

        public async Task ActionAsync<T>(IFileAction<T> action)
        {
            Task handler = InternalActionHandler(Directory, action);

            // do something else ...

            await handler;
        }

        private async Task InternalActionHandler<T>(DirectoryInfo parentDirectory, IFileAction<T> action)
        {
            new Task(() =>
            {
                if (OnDirectory != null)
                    OnDirectory(parentDirectory);
            })
            .Start();

            Task handler = InternalDirectoryHandler(parentDirectory, action);
            
            foreach (DirectoryInfo directory in parentDirectory.GetDirectories())
            {
                await InternalActionHandler(directory, action);
            }

            await handler;
        }

        private async Task InternalDirectoryHandler<T>(DirectoryInfo parentDirectory, IFileAction<T> action)
        {
            await Task.Run(() =>
            {
                foreach (FileInfo file in parentDirectory.GetFiles(action.SearchPattern))
                {
                    new Task(() =>
                    {
                        if (OnFile != null)
                            OnFile(file);
                    })
                    .Start();

                    action.Action(Directory, file);
                }
            });
        }
    }
}
