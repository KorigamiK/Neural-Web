#pragma once

#include <string>

#include "neural-web/neuron.hpp"
#include "neural-web/topology.hpp"

typedef std::vector<Neuron> Layer;

/**
 * @brief The core implementation of the neural Network
 * 		- For now it's a fully, forward connected network
 *
 */
class Network
{
public:
  Network(const Topology &topology);

  void feedForward(const std::vector<double> &inputs);

  void backPropagate(const std::vector<double> &targets);

  /** @brief Get the results of the last feed forward
   @param results The container of the results */
  void getResults(std::vector<double> &results) const;

  double getRecentAverageError(void) const;

  const Topology &getTopology(void) const;

  const std::vector<Layer> &getLayers(void) const;

private:
  std::vector<Layer> m_layers;
  double m_error;
  double m_recentAverageError;
  /// @brief Number of training samples to average over
  static double m_recentAverageSmoothingFactor;
  Topology m_topology;
};
