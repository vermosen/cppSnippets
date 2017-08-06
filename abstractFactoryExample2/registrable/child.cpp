#include "child.hpp"

registerType<base, std::tuple<int, std::string>, child1>
	child1::register_(std::tuple<int, std::string>(1, "A"));

registerType<base, std::tuple<int, std::string>, child2>
child2::register_(std::tuple<int, std::string>(1, "B"));

registerType<base, std::tuple<int, std::string>, child3>
child3::register_(std::tuple<int, std::string>(1, "C"));

registerType<base, std::tuple<int, std::string>, child4>
	child4::register_(std::tuple<int, std::string>(1, "D"));

registerType<base, std::tuple<int, std::string>, child5>
	child5::register_(std::tuple<int, std::string>(1, "E"));
