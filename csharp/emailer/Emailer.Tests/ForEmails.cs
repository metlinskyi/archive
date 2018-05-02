using Emailer.Emails;
using Emailer.Entities;
using Emailer.Prototypes;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Emailer.Tests
{
    [TestClass]
    public class ForEmails
    {
        private const string Link = "http://metlinskyi.com/";

        public class Sender : ISender
        {
            public Email Send(Email email)
            {
                email.IsSended = true;

                return email;
            }
        }

        private readonly ISender _sender = new Sender();

        private readonly User _user = new User
        {
            FullName = "Roman Metlinskyi",
            Title = "Software Engineer"
        };

        [TestMethod]
        public void WelcomeEmail()
        {
           var email = new WelcomeEmail(_user).Send(_sender);

            Assert.IsTrue(email.IsSended);
            Assert.IsTrue(email.Body.Contains(Link));
            Assert.IsTrue(email.Body.Contains(_user.FullName));
        }

        [TestMethod]
        public void RecoveryEmail()
        {
            var email = new RecoveryEmail(_user).Send(_sender);

            Assert.IsTrue(email.IsSended);
            Assert.IsTrue(email.Body.Contains(Link));
            Assert.IsTrue(email.Body.Contains(_user.FullName));
        }
    }
}
