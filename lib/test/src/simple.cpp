#include "neural-web/library.hpp"

auto main(int argc, char const *argv[]) -> int
{
  auto const lib = library{};
  return lib.name == "neural-web" ? 0 : 1;
}
