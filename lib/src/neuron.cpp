#include "neural-web/neuron.hpp"

#include <cmath>
#include <iostream>

#include "neural-web/functions.hpp"

using Layer = std::vector<Neuron>;

double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;

Neuron::Neuron(unsigned numOutputs, unsigned ownIndex, double outputValue)
    : outputValue{outputValue}, ownIndex{ownIndex},
      transferFunction{ActivationFunctions::sigmoid}, numOutputs{numOutputs}
{
  for (unsigned c = 0; c < numOutputs; ++c)
  {
    outputWeights.push_back(Connection());
    outputWeights.back().weight = randomWeight();
  }
}

const std::vector<Connection> &Neuron::getOutputWeights() const
{
  return outputWeights;
}

void Neuron::feedForward(const std::vector<Neuron> &prevLayer)
{
  double sum = 0.0;

  for (auto &neuron : prevLayer)
    sum += neuron.outputValue * neuron.outputWeights[ownIndex].weight;

  outputValue = transferFunction.function(sum);
}

void Neuron::calcOutputGradients(double targetVal)
{
  double delta = targetVal - outputValue;
  gradient = delta * transferFunction.derivative(outputValue);
}

void Neuron::calcHiddenGradients(const std::vector<Neuron> &nextLayer)
{
  double dow = sumDOW(nextLayer);
  gradient = dow * transferFunction.derivative(outputValue);
}

double Neuron::sumDOW(const std::vector<Neuron> &nextLayer) const
{
  double sum = 0.0;

  for (unsigned n = 0; n < nextLayer.size() - 1; ++n)
    sum += outputWeights[n].weight * nextLayer[n].gradient;

  return sum;
}

void Neuron::updateInputWeights(std::vector<Neuron> &prevLayer)
{
  for (auto &neuron : prevLayer)
  {
    double oldDeltaWeight = neuron.outputWeights[ownIndex].deltaWeight;

    double newDeltaWeight =
        // Individual input, magnified by the gradient and train rate:
        eta * neuron.getOutputVal() * gradient
        // Also add momentum = a fraction of the previous delta weight
        + alpha * oldDeltaWeight;

    neuron.outputWeights[ownIndex].deltaWeight = newDeltaWeight;
    neuron.outputWeights[ownIndex].weight += newDeltaWeight;
  }
}

void Neuron::setOutputVal(double val)
{
  outputValue = val;
}

double Neuron::getOutputVal(void) const
{
  return outputValue;
}

void Neuron::printWeights() const
{
  for (auto &connection : outputWeights)
    std::cout << connection.weight << " ";
  std::cout << std::endl;
}