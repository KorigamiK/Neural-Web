#include "neural-web/network.hpp"

auto main(int argc, char const *argv[]) -> int
{
  auto const lib = Network(Topology{
      .inputLayerSize = 3, .outputLayerSize = 2, .hiddenLayers = {2, 3}, .bias = 1});
  return lib.name == "neural-web" ? 0 : 1;
}
