using System;
using System.Reflection;

namespace Dr.Plugin.Tools
{
    public static class PnP
    {
        /// <summary>
        /// CreateInstance - default constructor 
        /// </summary>
        /// <param name="name">type name</param>
        /// <param name="path">dll path</param>
        /// <returns></returns>
        public static object CreateInstance(string name, string path)
        {
            object instance = null;

            Assembly assembly = Assembly.LoadFile(path);

            Type type = assembly.GetType(name, false, true);
            if (type != null)
            {
                // create instance
                ConstructorInfo constructor = type.GetConstructor(new Type[] { });
                if(constructor != null)
                    instance = constructor.Invoke(new object[] { });
            }

            return instance;
        }

        /// <summary>
        /// CreateInstance - default constructor 
        /// </summary>
        /// <param name="name">type name</param>
        /// <param name="path">dll path</param>
        /// <param name="parameters"></param>
        /// <returns></returns>
        public static object CreateInstance(string name, string path, object[] parameters)
        {
            object instance = null;

            Assembly assembly = Assembly.LoadFile(path);

            Type type = assembly.GetType(name, false, true);
            if (type != null)
            {

                Type[] types = new Type[parameters.Length];
                for(int i = 0; i< parameters.Length; i++)
                    types[i] = parameters[i].GetType();

                // create instance
                ConstructorInfo constructor = type.GetConstructor(types);
                if(constructor != null)
                    instance = constructor.Invoke(parameters);
            }

            return instance;
        }
    }
}