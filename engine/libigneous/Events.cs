using System;
using System.Collections.Generic;

namespace Igneous
{
    public static class Events
    {   
        private static Dictionary<Type, List<Delegate>> eventCallbacks = new Dictionary<Type, List<Delegate>>();

        public static void Add<T>(T callback) where T : Delegate
        {
            List<Delegate> list;
            if (eventCallbacks.TryGetValue(typeof(T), out list))
            {
                list.Add(callback);
            }
			else
			{
                eventCallbacks.Add(typeof(T), new List<Delegate>{ callback });
			}
        }

        public static void Remove<T>(T callback) where T : Delegate
        {
            List<Delegate> list;
            if (eventCallbacks.TryGetValue(typeof(T), out list))
            {
                list.Remove(callback);
            }
        }

        public static void Call<T>(params object[] args) where T : Delegate
        {
            List<Delegate> list;
            if (eventCallbacks.TryGetValue(typeof(T), out list))
            {
                foreach (T callback in list)
                {
                    callback.DynamicInvoke(args);
                }
            }
        }
    }
}
