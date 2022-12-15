#include "neural-web/network.hpp"

#include <cassert>
#include <iostream>
#include <math.h>

#include "neural-web/neuron.hpp"

Network::Network(const Topology topology) : name{"neural-web"}
{
  std::cout << "library::library()" << std::endl;

  this->topology = topology;

  for (unsigned layerNum = 0; layerNum < topology.topology.size(); ++layerNum)
  {
    layers.push_back(Layer());
    unsigned numOutputs =
        layerNum == topology.topology.size() - 1 ? 0 : topology.topology[layerNum + 1];

    for (unsigned neuronNum = 0; neuronNum <= topology.topology[layerNum]; ++neuronNum)
    {
      layers.back().push_back(Neuron(numOutputs, neuronNum));
      std::cout << "Made a Neuron!" << std::endl;
    }

    layers.back().back().setOutputVal(1.0);
  }
}

void Network::feedForward(const std::vector<double> &inputs)
{
  assert(inputs.size() == topology.topology[0]);

  for (unsigned i = 0; i < inputs.size(); ++i)
  {
    layers[0][i].setOutputVal(inputs[i]);
  }

  for (unsigned layerNum = 1; layerNum < layers.size(); ++layerNum)
  {
    Layer &prevLayer = layers[layerNum - 1];
    for (unsigned n = 0; n < layers[layerNum].size() - 1; ++n)
    {
      layers[layerNum][n].feedForward(prevLayer);
    }
  }
}

void Network::backPropagate(const std::vector<double> &targets)
{
  // Calculate overall net error (RMS of output neuron errors)
  // RMS = sqrt(1/N * sum((target - actual)^2))
  // N = number of output neurons

  Layer &outputLayer = layers.back();
  error = 0.0;

  for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
  {
    double delta = targets[n] - outputLayer[n].getOutputVal();
    error += delta * delta;
  }
  error /= outputLayer.size() - 1; // get average error squared
  error = sqrt(error);             // RMS

  // Calculate output layer gradients
  // Each output neuron's gradient = (target - actual) *
  // transferFunctionDerivative(output)

  // Calculate hidden layer gradients
  // Each hidden neuron's gradient = sum(output gradients * output weights) *
  // transferFunctionDerivative(output)

  // For all layers from outputs to first hidden layer,
  // update connection weights
  // Each connection's weight = weight + learningRate * output gradient * output value

  // Update bias weights
  // Each bias weight = bias weight + learningRate * output gradients
  // (bias value is always 1)
}
