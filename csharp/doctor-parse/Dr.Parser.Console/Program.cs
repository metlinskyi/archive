#define DEBUG

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Dr.Parser.Console
{

    class Program
    {
        static void Main(string[] args)
        {

#if DEBUG
            var operations = new Operations(new FileInfo(Path.Combine(Directory.GetCurrentDirectory(),@"debug.yaml")));
#else
            if (args.Length == 0) return;
            Operations operations = new Operations(new FileInfo(args[0]));
#endif
            var oroom = new OperatingRoom(operations);
            oroom.DoOperations();

            System.Console.ReadKey();
        }
    }
}