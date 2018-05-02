using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Dir.Console.Args
{
    class ArgsExeption : Exception
    {
        protected static string GetMessage(string name)
        {
            using (Stream stream = Assembly.GetExecutingAssembly().GetManifestResourceStream(name))
                using (StreamReader reader = new StreamReader(stream))
                {
                    return reader.ReadToEnd();
                }
        }

        public ArgsExeption(string[] args)
            : base(GetMessage("Dir.Console.Args.Help.txt"))
        {

        }
    }
}
