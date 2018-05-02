using System;
using System.Collections.Generic;

namespace Switch
{
    public class Switch<T> 
    {

        private readonly IDictionary<T, Action<T, object>> _actions;

        private Action<object> _default;

        public Switch()
        {
            _actions = new Dictionary<T, Action<T, object>>();
        }

		public Switch<T> Case(Action<T, object> statement, params T[] constants)
		{
			foreach (var value in constants)
			{
				this.Case(statement, value);
			}

			return this;
		}

		public Switch<T> Case(Action<T, object> statement, T constant)
        {

			if (!_actions.ContainsKey(constant))
				_actions.Add(constant, statement);
            else
				_actions[constant] = statement;

            return this;
        }

		public Switch<T> Default(Action<object> statement)
        {
			_default = statement;

            return this;
        }

		public void Invoke(T expression, Func<object> func = null)
        {
			if (!_actions.ContainsKey(expression))
            {
                if (_default != null)
                    _default((func != null) ? func() : null);

                return;
            }

			_actions[expression](expression, (func != null) ? func() : null);
        }
    }
}
