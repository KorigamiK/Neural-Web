#pragma once

#include <vector>

#include "neural-web/functions.hpp"
#include "neural-web/topology.hpp"

struct Connection
{
  double weight;
  double deltaWeight;
};

// @brief An implicitly connected and fully connected neuron.
class Neuron
{
private:
  /// @brief The overall learning rate belongs to [0.0, 1,0].
  static double eta;
  /// @brief The momentum, multiplier of the last weight change (deltaWeight).
  static double alpha;

  double outputValue;
  std::vector<Connection> outputWeights;
  unsigned ownIndex;
  double gradient;
  /* @brief The transfer function (activation funciton) used to calculate the output
    value of the neuron.
  */
  TransferFunction &transferFunction;

  /// @brief Sum of the derivatives of the weights of the next layer.
  double sumDOW(const std::vector<Neuron> &nextLayer) const;

public:
  const unsigned numOutputs;
  const std::vector<Connection> &getOutputWeights() const;

  Neuron(unsigned numOutputs, unsigned ownIndex, double outputValue = 0.0);

  void printWeights() const;
  void setOutputVal(double val);
  double getOutputVal(void) const;
  void feedForward(const std::vector<Neuron> &prevLayer);
  void calcOutputGradients(double targetVal);
  void calcHiddenGradients(const std::vector<Neuron> &nextLayer);
  void updateInputWeights(std::vector<Neuron> &prevLayer);
};
