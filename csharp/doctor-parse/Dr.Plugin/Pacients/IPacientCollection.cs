namespace Dr.Plugin.Pacients
{
    public interface IPacientCollection 
    {
        Pacient Next { get; }
        Pacient Prev { get; }
        int Count { get; }

        bool Save(Pacient pacient);
    }
}