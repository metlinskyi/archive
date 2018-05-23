using System.Collections.Concurrent;
using System.Threading.Tasks;

namespace Tasks
{
    public abstract class SequentialQueue<TItem> : ConcurrentQueue<TItem>
    {
        private bool _dequeuing;

        private object _locker = new object();

        public new void Enqueue(TItem item)
        {
            base.Enqueue(item);

            lock (_locker)
            {
                if (!_dequeuing)
                {
                    _dequeuing = true;

                    new Task(Dequeuing).Start();
                }
            }
        }

        protected abstract void Handler(TItem item);

        private void Dequeuing()
        {
            while (TryDequeue(out TItem item))
            {
                Handler(item);
            }

            lock (_locker) _dequeuing = false;
        }

        private void Done(Task task)
        {
            lock (_locker) _dequeuing = false;
        }
    }
}
