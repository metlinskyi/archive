using Emailer.Entities;
using Emailer.Prototypes;

namespace Emailer.Emails.Dto
{
    public class EmailForUser : IEmailTemplateDto
    {
        public User User { get; set; }
    }
}
