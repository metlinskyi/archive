namespace Dir.Console
{
    public class FileResult : Dir.Ex.Results.FileResult
    {
        public FileResult()
            : base(Properties.Settings.Default.ResultFileName)
        {
        }
    }
}
