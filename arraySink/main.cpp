
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/positioning.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/write.hpp>

namespace bis = boost::iostreams;

int main() {
	unsigned char* bbuf = new unsigned char[20];
	bis::basic_array_sink<unsigned char> sink(bbuf, 20);
	bis::stream<bis::basic_array_sink<unsigned char>> out(sink);

	out << "Hello fehwe kfjheiu fhewfuhweiuf";
	out.flush();
	delete[] bbuf;
	return 0;
}