#pragma once

#include <vector>

struct Topology
{
  Topology() = default;
  inline Topology(const std::vector<unsigned> &topology) : topology{topology}
  {
  }
  inline Topology(const std::initializer_list<unsigned> &topology) : topology{topology}
  {
  }

  std::vector<unsigned> topology;
};
