using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public interface IMath<T>
{
    T Add(T value1, T value2);
}

public class Math<T> : IMath<T>
{
    //default implementation
    T IMath<T>.Add(T value1, T value2)
    {
        dynamic v1 = value1, v2 = value2;
        return v1 + v2;
    }
}

class Math : IMath<int>
{
    //specialized for int
    int IMath<int>.Add(int value1, int value2)
    {
        return value1 + value2;
    }
}

namespace specialization
{
    class Program
    {
        static void Main(string[] args)
        {
            // the specialized type
            IMath<int> m1 = new Math();
            var res = m1.Add(1, 2);

            // the generic type
            IMath<double> m2 = new Math<double>();
            var res2 = m2.Add(1.0, 2.0);
        }
    }
}
