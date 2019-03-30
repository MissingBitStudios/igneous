using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Igneous
{
    public class Console
    {
        public delegate void CommandCallback(string cmd, string[] args);
        public delegate void VariableCallback(string oldValue, string newValue);

        public enum LogLevel
        {
            Trace,
            Debug,
            Info,
            Warn,
            Error,
            Critical,
            Off
        };

        private static Dictionary<string, CommandCallback> commands = new Dictionary<string, CommandCallback>();

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void AddAlias(string name, string exe);

        public static void AddCommand(string name, CommandCallback callback)
        {
            commands.Add(name, callback);
            AddCommandInternal(name);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern private static void AddCommandInternal(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void AddVariable(string name, string defaultValue, VariableCallback callback = null);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void AddVariable(string name, string defaultValue, string initialValue, VariableCallback callback = null);

        private static void RunCommandCallback(string name, string[] args)
        {
            CommandCallback callback;
            if (commands.TryGetValue(name, out callback))
            {
                callback(name, args);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void Execute(string exe, bool record = false, bool positive = true);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void Remove(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern public static void WriteLine(string output, LogLevel level = LogLevel.Trace);
    }
}