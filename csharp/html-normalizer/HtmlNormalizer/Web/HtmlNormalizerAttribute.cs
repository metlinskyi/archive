using System.Web.Mvc;

namespace HtmlNormalizer.Web
{
    public class HtmlNormalizerAttribute : ActionFilterAttribute
    {
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            var httpContext = filterContext.HttpContext;
            if (httpContext.Items[ToString()] == null)
            {
                httpContext.Response.Filter = new HtmlStream(httpContext.Response.Filter);
                httpContext.Items[ToString()] = true;
            }

            base.OnActionExecuting(filterContext);
        }
    }
}
