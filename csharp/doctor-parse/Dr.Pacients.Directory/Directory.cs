using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml.Schema;
using Dr.Plugin.Pacients;

namespace Dr.Pacients
{
    internal delegate void FileProcess(FileInfo file);

    class OnFileEventArgs
    {
        public OnFileEventArgs(FileInfo f) { File = f; }
        public FileInfo File { get; private set; }
    }

    public class Directory : IPacientCollection
    {
        private readonly DirectorySettings _settings;
        private readonly DirectoryInfo _mSrcDirectory;
        private readonly DirectoryInfo _mDesDirectory;
        private readonly FileSystemInfo[] _mFiles;
        private readonly int _mFilesCount;

        private int _mFilesIndex;
        private int _mDirectoriesCount;

        private Pacient _mCurrentPacinet;

        public Directory(DirectorySettings settings)
        {
            _settings = settings;

            _mSrcDirectory = new DirectoryInfo(_settings.Source);
            if (!_mSrcDirectory.Exists)
            {

            }

            _mDesDirectory = new DirectoryInfo(_settings.Destination.Replace("%CurrentDirectory%",AppDomain.CurrentDomain.BaseDirectory));
            if (!_mDesDirectory.Exists)
                _mDesDirectory.Create();
            else
            {
                _mDesDirectory.Delete(true);
                _mDesDirectory.Create();
            }

            _mFiles = _mSrcDirectory.GetFileSystemInfos();
            _mFilesIndex = -1;
            _mDirectoriesCount = 0;
            _mFilesCount = _mFiles.Length;
        }

        public int Count
        {
            get { return 0; }
        }

        public Pacient Next
        {
            get {
                
                while (_mFilesCount > (++_mFilesIndex))
                {
                    if (!_mFiles[_mFilesIndex].FullName.Contains(_settings.Extension)) 
                        continue;

                    Prev = _mCurrentPacinet;

                    ////////////////////////////////////////////////////////////////
                    _mCurrentPacinet = new Pacient
                    {
                        Name = _mFiles[_mFilesIndex].FullName.Replace(_settings.Source, string.Empty)
                    };

                    ReadPacient(ref _mCurrentPacinet);

                    return _mCurrentPacinet;
                }
                return null;
            }
        }

        public Pacient Prev { get; private set; }

        public bool Save(Pacient pacient)
        {
            WritePacient(ref pacient); 
            return false;
        }

/*
        private void FilesCounter(FileInfo file)
        {
            mFilesCount++;
        }

        public delegate void OnFileEventHandler(OnFileEventArgs e);

        public event OnFileEventHandler OnFile;

        private void Recursion(FileSystemInfo[] FSInfo, FileProcess fp)
        {
            foreach (FileSystemInfo f in FSInfo)
            {
                if (f is DirectoryInfo)
                {
                    mDirectoriesCount++;

                    DirectoryInfo dInfo = (DirectoryInfo)f;
                    Recursion(dInfo.GetFileSystemInfos(), fp);
                }
                else if (f is FileInfo)
                {
                    if ((Settings.Extension != string.Empty) && (((FileInfo)f).Extension.ToUpper() == Settings.Extension))
                    {
                        if (this.OnFile != null)
                            this.OnFile(new OnFileEventArgs((FileInfo)f));

                        if (fp != null)
                            fp((FileInfo)f);
                    }
                }
            }
        }

        public void Process(FileProcess fp)
        {
            Recursion(mSrcDirectory.GetFileSystemInfos(), fp);
        }
*/

        private void ReadPacient(ref Pacient pacient)
        {
            StreamReader reader = null;

            try
            {
                reader = new StreamReader( _mSrcDirectory.FullName + pacient.Name);
                pacient.Body = reader.ReadToEnd();
            }
            catch (Exception ex)
            {
                var e = ex;
            }
            finally
            {
                if (reader != null) reader.Close();
            }
        }

        private void WritePacient(ref Pacient pacient)
        {
            StreamWriter writer = null;

            try
            {
                writer = new StreamWriter(_mDesDirectory.FullName + pacient.Name);
                writer.Write(pacient.Body);
            }
            catch (Exception ex)
            {
                var e = ex;
            }
            finally
            {
                if (writer != null) writer.Close();
            }
        }
    }
}