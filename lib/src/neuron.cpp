#include "neural-web/neuron.hpp"

#include <cmath>

#include "neural-web/functions.hpp"

using Layer = std::vector<Neuron>;

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
    : outputValue{0.0}, ownIndex{myIndex}, transferFunction{ActivationFunctions::sigmoid}
{
  for (unsigned c = 0; c < numOutputs; ++c)
  {
    outputWeights.push_back(Connection());
    outputWeights.back().weight = randomWeight();
  }
}

void Neuron::feedForward(const Layer &prevLayer)
{
  double sum = 0.0;

  for (auto &neuron : prevLayer)
    sum += neuron.getOutputVal() * neuron.outputWeights[ownIndex].weight;

  outputValue = transferFunction.function(sum);
}

void Neuron::setOutputVal(double val)
{
  outputValue = val;
}

double Neuron::getOutputVal(void) const
{
  return outputValue;
}
