#pragma once

#include <string>
#include <vector>

#include "neural-web/neuron.hpp"
#include "neural-web/topology.hpp"

using Layer = std::vector<Neuron>;

/**
 * @brief The core implementation of the neural Network
 * 		- For now it's a fully, forward connected network
 *
 */
class Network
{
public:
  std::string name;

  Network(const Topology topology);

  void feedForward(const std::vector<double> &inputs);

  void backPropagate(const std::vector<double> &targets);

  void getResults(std::vector<double> &results) const;

private:
  Topology topology;
  std::vector<Layer> layers;
  double error;
};
