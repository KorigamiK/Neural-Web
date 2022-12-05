#include <iostream>
#include <string>

#include "lib/neural-web.hpp"

auto main() -> int
{
  auto const lib = library{};
  auto const message = "Hello from " + lib.name + "!";
  std::cout << message << '\n';
  return 0;
}
