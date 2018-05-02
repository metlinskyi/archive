using Dir.Console.Args;
using Dir.Ex;
using Dir.Ex.Prototypes;
using Ninject;
using System;
using System.IO;
using System.Reflection;
using System.Threading.Tasks;
using Cmd = System.Console;

namespace Dir.Console
{
    class Program
    {
        private static object locker = new object();

        private static int counter = 0;

        static Program()
        {
            AppDomain.CurrentDomain
                .UnhandledException += CurrentDomain_UnhandledException;
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Exception exception = e.ExceptionObject as Exception;
            if (exception == null)
                goto Exite;

            Cmd.WriteLine(exception.Message);

            Exite:
#if DEBUG
            Cmd.ReadKey();
#endif
            Environment.Exit(1);
        }

        static void Main(string[] args)
        {
            DirEx direx;
            if (!ArgsBinder.TryParse(args, out direx))
            {
                throw new ArgsExeption(args);
            }

            IKernel kernal = new StandardKernel();
            kernal.Load(Assembly.GetExecutingAssembly());

            IFileAction<string> fileAction = kernal.Get<IFileAction<string>>();
            fileAction.Output = kernal.Get<IFileActionResult<string>>();

            direx.OnFile += Direx_OnFileAction;
            direx.OnDirectory += Direx_OnDirectory;
            direx.Action(fileAction);

            if (fileAction.Output is IDisposable)
                ((IDisposable)fileAction.Output).Dispose();

#if DEBUG
            Cmd.ReadKey();
#endif
        }

        private static void Direx_OnFileAction(FileInfo item)
        {
            if (item == null)
            {
                Cmd.WriteLine("Oops!");
                return;
            }

            // count will not be in order without locker
            lock (locker)
                Cmd.WriteLine("[{0}]{1}", ++counter, item.Name);
        }

        private static void Direx_OnDirectory(DirectoryInfo item)
        {
            if (item == null)
            {
                Cmd.WriteLine("Oops i did it again!");
                return;
            }

            // Cmd.WriteLine("{0}", item.Name);
        }
    }
}
