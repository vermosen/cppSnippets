#include <string>
#include <iostream>

#include <boost/regex.hpp>

int main()
{
  std::map<std::string, std::string> kv = {
      {"hello", "foo"}
    , {"world", "bar"}
  };

  std::string s = "{{hel_lo}} {{world}}!\r";
  boost::regex expr("{{(\\s*[\\w\\.]+\\s*)}}");
  boost::smatch match;

  boost::sregex_token_iterator iter(s.begin(), s.end(), expr, 0);
  boost::sregex_token_iterator end;

  for (; iter != end; ++iter) {
    std::cout << *iter << std::endl;
  }

  return 0;
}
