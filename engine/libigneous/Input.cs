using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Igneous
{
    public class Input
    {
        public delegate void KeyEvent(int key, int scancode, int action, int mods);
        public delegate void MouseButtonEvent(int button, int action, int mods);
        public delegate void ScrollEvent(double xoffset, double yoffset);
        public delegate void CursorMoveEvent(double xpos, double ypos);
    }
}
