using System.Threading.Tasks;

namespace Tasks
{
    public class SequentialTaskQueue : SequentialQueue<Task>
    {
        /// <summary>
        /// Task can be added at ant time by any number of clients
        /// </summary>
        public static SequentialQueue<Task> Queue { get; } = new SequentialTaskQueue();

        /// <summary>
        /// The singleton task queue 
        /// </summary>
        private SequentialTaskQueue() { }

        protected override void Handler(Task task)
        {
            task.RunSynchronously();
        }
    }
}
