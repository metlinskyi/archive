using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;
using Dr.Plugin.DataBase;
using Dr.Plugin.Pacients;

namespace Dr.Parser
{
    public class OperatingRoom
    {
        readonly PacientsProvider _pcProvider = null;
        readonly DataBaseProvider _dbProvider = null;

        readonly Operations _operations;

        public OperatingRoom(Operations operations)
        {
            _operations = operations;

            _pcProvider = (!string.IsNullOrEmpty(operations.Pacients)) ?
                new PacientsProvider(operations.Pacients, operations.PacientsSettings) : null;
            if ((_pcProvider != null) && (_pcProvider.Instance is IPacientCollection))
            {
                // throw new NotImplementedException();
            }

            _dbProvider = (!string.IsNullOrEmpty(operations.DataBase)) ?
                new DataBaseProvider(operations.DataBase, operations.DataBaseSettings) : null;
            if ((_dbProvider != null) && (_dbProvider.Instance is IDataBaseProvider))
            {
                // throw new NotImplementedException();
            }
        }

        public static void _Callback(Object o)
        {

        }

        public static void _Background(Object o)
        {

        }

        public void DoOperations()
        {
            switch (_operations.MultiThread)
            {
                case MultiThreadMode.Pool:
                {
                    int completion;
                    int available;

                    ThreadPool.GetAvailableThreads(out available, out completion);
                    if ((_operations.MinThreads > 1) && (_operations.MinThreads < available))
                        if (ThreadPool.SetMinThreads(_operations.MinThreads, completion))
                        {
                            // throw new NotImplementedException();
                        }

                    ThreadPool.QueueUserWorkItem(_Callback, null);
                }
                break;

                case MultiThreadMode.Background:
                {
                    var thread = new Thread(_Background, 0) {IsBackground = true};
                    thread.Start();
                }
                break;

                default:
                {
                    Pacient pacient;

                    var doc = new Doctor();
                    doc.OnException += OnException;
                    doc.OnExecute += OnExecute;
                    doc.OnFile += OnFile;

                    do
                    {
                        pacient = _pcProvider.Next;
                        if (pacient == null)
                            continue;

                        foreach (YamlNode op in _operations.List)
                        {
                            doc.Do(op.Name, op.Parameters, ref pacient);
                        }

                        _pcProvider.Save(pacient);
                    }
                    while (pacient != null);
                }
                break;

            }
        }

        #region Events of doctor >

        private void OnExecute(DoctorExecuteQueryArgs e)
        {
            if (_dbProvider.Execute(e.Query))
            {

            }
        }

        private static void OnException(DoctorExceptionArgs e)
        {
            Console.WriteLine(e.Message);
        }

        private static void OnFile(DoctorFileArgs e)
        {
            if (!e.File.Exists)
                e.File.Create();

            using (var file = new FileStream(e.File.FullName, FileMode.Append))
            {
                byte[] buffer = Encoding.Default.GetBytes(e.Data);
                file.Write(buffer, 0, buffer.Length);
            }
        }

        #endregion
    }
}
