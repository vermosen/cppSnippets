#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/positioning.hpp>
#include <iostream>

namespace bis = boost::iostreams;

using bis::position_to_offset;
using bis::seek;
using bis::stream_offset;

int main() {

	const char* curr = nullptr;
	const char* end = nullptr;

	bis::mapped_file mmap("sample.txt", bis::mapped_file::readonly);

	if (mmap.is_open()) {
		curr = mmap.const_data();
		end = curr + mmap.size();
	}

	if (*(end - 1) != '\n') {
		std::cout << "the file is not eol terminated!" << std::endl;
	}

	while (curr < end) {
		const char* index = static_cast<const char*>(memchr(curr, '\n', end - curr));
		std::string container = std::string(curr, index - curr);
		curr = index + 1;
	}

	return  0;
}