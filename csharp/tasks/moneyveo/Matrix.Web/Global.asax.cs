using Autofac;
using Autofac.Integration.Mvc;
using Matrix.Services;
using Matrix.Web.Services;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;

namespace Matrix.Web
{
    public class MvcApplication : System.Web.HttpApplication
    {
        protected void Application_Start()
        {
            var builder = new ContainerBuilder();

            builder.RegisterControllers(typeof(MvcApplication).Assembly);
            builder.RegisterModelBinders(typeof(MvcApplication).Assembly);
            builder.RegisterModelBinderProvider();
            builder.RegisterModule<AutofacWebTypesModule>();
            builder.RegisterSource(new ViewRegistrationSource());
            builder.RegisterFilterProvider();

            builder.RegisterType<FileService>()
                    .As<IFileService>()
                    .SingleInstance();

            builder.RegisterType<MatrixSerializer>()
                    .As<IMatrixSerializer>()
                    .SingleInstance();

            builder.RegisterType<MatrixStorage>()
                    .As<IMatrixStorage>()
                    .SingleInstance();

            DependencyResolver.SetResolver(new AutofacDependencyResolver(builder.Build()));

            AreaRegistration.RegisterAllAreas();
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);
        }
    }
}
