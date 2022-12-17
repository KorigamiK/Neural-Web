#pragma once

#include <vector>

#include "neural-web/functions.hpp"
#include "neural-web/topology.hpp"

struct Connection
{
  double weight;
  double deltaWeight;
};

class Neuron;
using Layer = std::vector<Neuron>;

// @brief An implicitly connected and fully connected neuron.
class Neuron
{
private:
  /// @brief The overall learning rate belongs to [0.0, 1,0].
  static double eta;
  /// @brief The momentum, multiplier of the last weight change (deltaWeight).
  static double alpha;

  double m_outputVal;
  std::vector<Connection> m_outputWeights;
  unsigned m_myIndex;
  double m_gradient;
  /** @brief The transfer function (activation funciton) used to calculate the output
  value of the neuron.
  */
  static double transferFunction(double x);
  static double transferFunctionDerivative(double x);

  double sumDOW(const Layer &nextLayer) const;

public:
  Neuron(unsigned numOutputs, unsigned myIndex);
  void setOutputVal(double val);
  double getOutputVal(void) const;
  void feedForward(const Layer &prevLayer);
  void calcOutputGradients(double targetVal);
  void calcHiddenGradients(const Layer &nextLayer);
  void updateInputWeights(Layer &prevLayer);
};