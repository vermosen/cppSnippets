using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public interface IConvertibleToNative<K>
{
    K toNative();
}

public class Base : IConvertibleToNative<Native>
{
    public Native toNative() {
        return default(Native);
    }
}

public interface INative<T> { }

public class Native : INative<Base> { }

public class mapper<T, K> where T : IConvertibleToNative<K>
{
    K convert(T newkey) {
        return newkey.toNative();
    }
}

namespace typemapper
{
    class Program
    {
        static void Main(string[] args)
        {

        }
    }
}

