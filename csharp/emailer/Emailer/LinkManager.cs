using Emailer.Prototypes;

namespace Emailer
{
    public class LinkManager
    {
        public static string GetEmailLink<T>(Email email)
            where T : Email
        {
            return "http://metlinskyi.com/" + typeof (T).Name.ToLower().Replace("email", string.Empty);
        }
    }
}
