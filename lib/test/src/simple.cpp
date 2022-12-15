#include "neural-web/network.hpp"

auto main(int argc, char const *argv[]) -> int
{
  auto const lib = Network(Topology({2, 3, 1}));
  return lib.name == "neural-web" ? 0 : 1;
}
