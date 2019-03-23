using System;
using Igneous;

namespace sandbox
{
    public class Sandbox
    {
        static void Main(string[] args)
        {
            // Display the number of command line arguments:
            Igneous.Console.WriteLine("Hello from C#!", LogLevel.Info);
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