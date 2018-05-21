using System.Threading.Tasks;

namespace Tasks
{
    /// <summary>
    /// Implement a C# class which responsibility it to sequentially execute tasks.
    /// </summary>
    public sealed class SequentialTaskFactory : TaskFactory
    {
        /// <summary>
        /// Task can be added at ant time by any number of clients
        /// </summary>
        public static TaskFactory Factory { get; } = new SequentialTaskFactory();

        /// <summary>
        /// 1 = only one task can be executed at a time
        /// </summary>
        private const int maximumConcurrencyLevel = 1;

        /// <summary>
        /// The singleton task factory 
        /// </summary>
        private SequentialTaskFactory()
            : base(new SequentialTaskScheduler(maximumConcurrencyLevel))
        {
        }
    }
}
