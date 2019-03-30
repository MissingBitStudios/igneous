using Igneous;
using System.Runtime.InteropServices;

namespace Sandbox
{
    public class Sandbox
    {
        private delegate void TestEvent(string t);

        static void Main(string[] args)
        {
            Console.WriteLine("Hello from C#!", Console.LogLevel.Info);
            Console.WriteLine(string.Format("args = [{0}]", string.Join(",", args)), Console.LogLevel.Info);

            Console.AddCommand("test", TestCommand);
            Console.Execute("print t");

            Events.Add<Input.KeyEvent>(KeyCallback);
            Events.Add<TestEvent>(TestEventCallback);
        }

        static void Update(float dt)
        {
            //Events.Call<TestEvent>(dt.ToString() + " test");
        }

        static void Shutdown()
        {
            Console.WriteLine("Goodbye from C#!", Console.LogLevel.Info);
        }

        static void TestCommand(string cmd, string[] args)
        {
            Console.WriteLine(string.Format("name = {0}, args = [{1}]", cmd, string.Join(",", args)), Console.LogLevel.Info);
        }

        static void KeyCallback(int key, int scancode, int action, int mods)
        {
            Console.WriteLine(string.Format("You pressed the {0} key", key), Console.LogLevel.Info);
        }

        static void TestEventCallback(string t)
        {
            Console.WriteLine(string.Format("dt = {0}", t));
        }
    }
}