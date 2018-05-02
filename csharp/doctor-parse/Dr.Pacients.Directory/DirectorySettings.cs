using System;
using System.Collections.Generic;
using System.IO;

namespace Dr.Pacients
{
    public class DirectorySettings
    {
        private SearchOption _mAllDirectories;

        public string Source { get; set; }

        public string Destination { get; set; }

        public string Extension { get; set; }

        public bool Subfolders
        {
            set { _mAllDirectories = value ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly; }
            get { return _mAllDirectories == (SearchOption.AllDirectories); }
        }

        public SearchOption AllDirectories
        {
            get { return _mAllDirectories; }
        }
    }
}