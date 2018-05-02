using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using Dr.Plugin.DataBase;

namespace Dr.DataBase
{
    public class MSSQLServer : IDataBaseProvider, IDisposable
    {
        private readonly SqlConnection _connection;

        public bool Execute(string query)
        {
            var cmd = new SqlCommand {CommandText = query, Connection = _connection};

            if (_connection.State == ConnectionState.Closed)
                 _connection.Open();

            return cmd.ExecuteNonQuery() > 0;
        }

        public MSSQLServer(MSSQLServerSettings settings)
        {
            _connection = new SqlConnection(settings.Connection);
        }

        public void Dispose()
        {
            if ((_connection != null) && 
                (_connection.State == ConnectionState.Open))
                _connection.Close();
        }
    }
}