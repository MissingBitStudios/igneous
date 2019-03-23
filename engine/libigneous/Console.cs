using System;
using System.Runtime.CompilerServices;

namespace Igneous
{
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

    public class Console
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void WriteLine(string output, LogLevel level = LogLevel.Trace);
    }
}