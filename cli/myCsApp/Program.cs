using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using managed_ns;

namespace myCsApp
{
    class Program
    {
        static void Main(string[] args)
        {
            var cl = new myManagedClass(3.0);
            cl.doub = 4.0;
            Console.WriteLine(cl.doub);
        }
    }
}
