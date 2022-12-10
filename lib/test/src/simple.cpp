#include "neural-web/network.hpp"

auto main(int argc, char const *argv[]) -> int
{
  auto const lib = Network{};
  return lib.name == "neural-web" ? 0 : 1;
}
