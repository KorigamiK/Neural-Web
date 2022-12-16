#pragma once

#include <vector>

struct Topology
{
  unsigned inputLayerSize;
  unsigned outputLayerSize;
  std::vector<unsigned> hiddenLayers;
  int bias = 0;

  Topology() = default;
};
