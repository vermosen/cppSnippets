
#include <array>
#include <vector>
#include <sstream>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>


namespace ios = boost::iostreams;

int main()
{
  std::string bindata;

  {
    std::vector<double> input = { 12.5526, 26465.325, 45893.2356 };

    ios::filtering_ostream out;
    out.push(ios::back_inserter(bindata));

    auto sz = input.size();
    out.write(reinterpret_cast<const char*>(&sz), sizeof(input.size()));

    for (auto it : input) {
      out.write(reinterpret_cast<const char*>(&it), sizeof(double));
    }
  }

  std::vector<double> output;

  {
    std::stringstream iss; iss << bindata;
    ios::filtering_streambuf<ios::input> in;
    in.push(iss);

    std::size_t sz = 0;
    auto read = in.sgetn(reinterpret_cast<char*>(&sz), sizeof(std::size_t));

    if (sz > 0) {
      for (std::size_t i = 0; i < sz; i++) {
        double temp = 0.0;
        read = in.sgetn(reinterpret_cast<char*>(&temp), sizeof(double));
        output.push_back(temp);
      }
    }
  }

  std::cout << bindata;
  return EXIT_SUCCESS;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
