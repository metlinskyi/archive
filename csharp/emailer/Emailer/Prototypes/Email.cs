using System;

namespace Emailer.Prototypes
{
    public abstract class Email
    {
        public virtual string Body { get; set; }

        public bool IsSended { get; set; }

        public Email Send(ISender sender)
        {
            return sender.Send(this);
        }
    }
}
