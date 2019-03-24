using System;
using Igneous;

namespace sandbox
{
    public class Sandbox
    {
        static void Main(string[] args)
        {
            Igneous.Console.WriteLine("Hello from C#!", LogLevel.Info);
            Igneous.Console.WriteLine(String.Format("args = [{0}]", String.Join(",", args)), LogLevel.Info);
        }

        static void Update(float dt)
        {
            Igneous.Console.WriteLine(String.Format("dt = {0}", dt));
        }

        static void Shutdown()
        {
            Igneous.Console.WriteLine("Goodbye from C#!", LogLevel.Info);
        }
    }
}