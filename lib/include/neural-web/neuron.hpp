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
  double outputValue;
  std::vector<Connection> outputWeights;
  unsigned ownIndex;

  /* @brief The transfer function (activation funciton) used to calculate the output
    value of the neuron.
  */
  TransferFunction &transferFunction;

public:
  const unsigned numOutputs;
  const std::vector<Connection> &getOutputWeights() const;

  Neuron(unsigned numOutputs, unsigned ownIndex, double outputValue = 0.0);

  void setOutputVal(double val);
  double getOutputVal(void) const;
  void feedForward(const std::vector<Neuron> &prevLayer);
};
