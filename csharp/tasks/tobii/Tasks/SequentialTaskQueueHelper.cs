using System;
using System.Threading.Tasks;

namespace Tasks
{
    public static class SequentialTaskQueueHelper
    {
        public static Task StartNew(this SequentialQueue<Task> queue, Action<object> action, object state)
        {
            var task = new Task(action, state);

            queue.Enqueue(task);

            return task;
        }
    }
}
