using Emailer.Emails.Dto;
using Emailer.Entities;
using Emailer.Properties;
using Emailer.Prototypes;

namespace Emailer.Emails
{
    public class RecoveryEmail : EmailTemplate<RecoveryEmail.RecoveryEmailDto>
    {
        public class RecoveryEmailDto : EmailForUser
        {
            public string Link { get; set; }
        }

        private readonly User _user;

        public RecoveryEmail(User user)
            : base(Resources.EmailTeamplate_RecoveryEmail)
        {
            _user = user;
        }

        protected override RecoveryEmailDto Data => new RecoveryEmailDto
        {
            User = _user,
            Link = LinkManager.GetEmailLink<RecoveryEmail>(this)
        };
    }
}
