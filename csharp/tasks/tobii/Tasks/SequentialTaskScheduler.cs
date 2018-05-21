using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;

namespace Tasks
{
    /// <summary>
    /// The sequential queue of tasks
    /// </summary>
    internal class SequentialTaskScheduler : TaskScheduler
    {
        [ThreadStatic]
        private static bool _currentThreadIsProcessingItems;

        private readonly LinkedList<Task> _tasks;

        private readonly int _maximumConcurrencyLevel;

        private int _delegatesQueuedOrRunning;

        private SequentialTaskScheduler()
        {
            _tasks = new LinkedList<Task>();
            _delegatesQueuedOrRunning = 0;
        }

        public SequentialTaskScheduler(int maximumConcurrencyLevel)
            : this()
        {
            if (maximumConcurrencyLevel < 1)
                throw new ArgumentOutOfRangeException(nameof(maximumConcurrencyLevel));

            _maximumConcurrencyLevel = maximumConcurrencyLevel;
        }

        public sealed override int MaximumConcurrencyLevel => _maximumConcurrencyLevel;

        protected sealed override void QueueTask(Task task)
        {
            lock (_tasks)
            {
                _tasks.AddLast(task);

                if (_delegatesQueuedOrRunning < _maximumConcurrencyLevel)
                {
                    ++_delegatesQueuedOrRunning;

                    NotifyThreadPoolOfPendingWork();
                }
            }
        }

        protected sealed override bool TryExecuteTaskInline(Task task, bool taskWasPreviouslyQueued)
        {
            if (!_currentThreadIsProcessingItems)
                return false;

            if (taskWasPreviouslyQueued)
                return TryDequeue(task) ? base.TryExecuteTask(task) : false;
            else
                return base.TryExecuteTask(task);
        }

        protected sealed override bool TryDequeue(Task task)
        {
            lock (_tasks) return _tasks.Remove(task);
        }

        protected sealed override IEnumerable<Task> GetScheduledTasks()
        {
            bool lockTaken = false;

            try
            {
                Monitor.TryEnter(_tasks, ref lockTaken);

                return lockTaken ? _tasks : throw new NotSupportedException();
            }
            finally
            {
                if (lockTaken) Monitor.Exit(_tasks);
            }
        }

        private void NotifyThreadPoolOfPendingWork()
        {
            ThreadPool.UnsafeQueueUserWorkItem(_ =>
            {
                _currentThreadIsProcessingItems = true;

                try
                {
                    while (true)
                    {
                        Task item;

                        lock (_tasks)
                        {
                            if (_tasks.Count == 0)
                            {
                                --_delegatesQueuedOrRunning;
                                break;
                            }

                            item = _tasks.First.Value;

                            _tasks.RemoveFirst();
                        }

                        base.TryExecuteTask(item);

                        // tasks are executed sequentially in order they were added 
                        item.Wait();
                    }
                }
                finally
                {
                    _currentThreadIsProcessingItems = false;
                }
            }, null);
        }
    }
}