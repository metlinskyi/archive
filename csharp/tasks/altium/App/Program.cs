using System;
using System.IO;
using System.Linq;
using App.Properties;
using Lib;
using Lib.Data.Entities;
using Lib.Mapping;

namespace App
{
    class Program
    {
        static Program() 
        {
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
        }

        static void Main(string[] args)
        {
            var source = new FileInfo(GetArg("-s",args));
            var destination = new FileInfo(GetArg("-d", args));

            try
            {
                source
                    .AsEnumerable<FileEntity>()
                    .OrderBy(x => x.Name, x => x.Number)
                    .ToFile(destination);
            }
            catch (MapperException ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        static string GetArg(string name, string[] args, bool mandatory = true)
        {
            if (args.Length == 0)
                throw new ArgsException();

            var value = args
                .SkipWhile(x => !x.TrimStart().StartsWith(name))
                .Skip(1)
                .FirstOrDefault();

            if(mandatory && string.IsNullOrEmpty(value))
                throw new ArgsException();

            return value;
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            var exception = e.ExceptionObject as ArgsException;
            if (exception != null)
            {
                Console.WriteLine(Resources.ArgsHelper);
                Environment.Exit(0);
            }
        }
    }
}
