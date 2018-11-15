using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public interface IInterface<T>
{
    void apply<K>(ref T lhs, ref K rhs);
}

public class applyer : IInterface<double>
{
    public void apply<K>(ref double lhs, ref K rhs) {
        // do something
    }
}

namespace specialization
{
    class Program
    {
        static void Main(string[] args)
        {
            // the specialized type
            applyer v = new applyer();
            double d1 = 1.0, d2 = 2.0;
            v.apply(ref d1, ref d2);
        }
    }
}
