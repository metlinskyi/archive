using Dir.Ex;
using Dir.Ex.Actions;
using Dir.Ex.Prototypes;
using Ninject.Modules;
using Ninject.Syntax;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dir.Console.Args
{
    public class ArgsBinder : NinjectModule
    {
        public static string[] Args { get; set; }

        public static bool TryParse(string[] args, out DirEx direx)
        {
            Args = args;

            direx = null;

            if (args.Length == 0)
                return false;

            DirectoryInfo directory = null;
            foreach (string arg in args)
            {
                if (Directory.Exists(arg) && (directory = new DirectoryInfo(arg)) != null)
                    break;
            }

            if (directory == null)
                return false;

            direx = new DirEx(directory);

            return true;
        }

        public override void Load()
        {
            foreach (string arg in Args)
            {
                if (BindIFileAction(arg.ToLower()))
                    break;
            }

            Bind<IFileActionResult<string>>().To<FileResult>();
        }

        public bool BindIFileAction(string name)
        {
            switch (name)
            {
                case "all":
                    Bind<IFileAction<string>>().To<AllAction>();
                    return true;

                case "cpp":
                    Bind<IFileAction<string>>().To<CppAction>();
                    return true;

                case "reversed1":
                    Bind<IFileAction<string>>().To<Reversed1Action>();
                    return true;

                case "reversed2":
                    Bind<IFileAction<string>>().To<Reversed2Action>();
                    return true;

            }

            return false;
        }
    }
}
