#include <iostream>
#include <array>
#include <algorithm>

// an example of abstract factory with auto-registration of the classes
int main() {
	try {
		std::array<const char *, 3> arr = { "aha", "oho", "ihi" };
		std::array<std::string, 3> res;

		std::transform(arr.begin(), arr.end(), res.begin(),
			[](const char * c) -> std::string {return std::string(c); });
	}
	catch (const std::exception&) {
		std::cout << "Oups, something went wrong..." << std::endl;
		return 1;
	}

	return 0;
}
