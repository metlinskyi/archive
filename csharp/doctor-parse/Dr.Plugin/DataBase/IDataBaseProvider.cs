namespace Dr.Plugin.DataBase
{
    public interface IDataBaseProvider
    {
        bool Execute(string query);
    }
}
