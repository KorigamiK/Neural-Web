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
  std::string name;

  const Topology topology;

  Network(const Topology topology);

  void feedForward(const std::vector<double> &inputs);

  void backPropagate(const std::vector<double> &targets);

  /// @brief Get the results of the last feed forward
  /// @param results The container of the results
  void getResults(std::vector<double> &results) const;

  const std::vector<Layer> &getLayers() const;
  double getRecentAverageError() const;
  void printLayers() const;

private:
  std::vector<Layer> layers;
  double error;
  double recentAverageError;
  double recentAverageSmoothingFactor;

  void createInputLayer();
  void createHiddenLayers();
  void createOutputLayer();
  void createBiasNeurons(unsigned layerNum, unsigned numOutputs);
};
