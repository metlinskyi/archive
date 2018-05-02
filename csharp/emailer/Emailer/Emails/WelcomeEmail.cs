using Emailer.Emails.Dto;
using Emailer.Entities;
using Emailer.Properties;
using Emailer.Prototypes;

namespace Emailer.Emails
{
    public class WelcomeEmail : EmailTemplate<WelcomeEmail.WelcomeEmailDto>
    {
        public class WelcomeEmailDto : EmailForUser
        {
            public string Url { get; set; }
        }

        private readonly User _user;

        public WelcomeEmail(User user) 
            : base(Resources.EmailTeamplate_WelcomeEmail)
        {
            _user = user;
        }

        protected override WelcomeEmailDto Data => new WelcomeEmailDto
        {
            User = _user,
            Url = LinkManager.GetEmailLink<WelcomeEmail>(this)
        };
    }
}
