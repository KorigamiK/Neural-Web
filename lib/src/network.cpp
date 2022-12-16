#include "neural-web/network.hpp"

#include <cassert>
#include <iostream>
#include <math.h>

#include "neural-web/neuron.hpp"

void Network::createBiasNeurons(unsigned layerNum, unsigned numOutputs)
{
  for (unsigned neuronNum = 0; neuronNum < topology.bias; ++neuronNum)
    layers[layerNum].push_back(
        Neuron(numOutputs, layers[layerNum].size() - 1 + neuronNum, 1.0));
}

void Network::createInputLayer()
{
  layers.push_back(Layer()); // topology.hiddenLayers[0] + topology.bias));

  for (unsigned neuronNum = 0; neuronNum < topology.inputLayerSize; ++neuronNum)
    layers.back().push_back(Neuron(topology.hiddenLayers[0] + topology.bias, neuronNum));

  createBiasNeurons(0, topology.hiddenLayers[0] + topology.bias);
}

void Network::createOutputLayer()
{
  layers.push_back(Layer()); // topology.outputLayerSize)); // don't add bias neurons

  for (unsigned neuronNum = 0; neuronNum < topology.outputLayerSize; ++neuronNum)
    layers.back().push_back(Neuron(0, neuronNum));
}

void Network::createHiddenLayers()
{
  for (unsigned layerNum = 0; layerNum < topology.hiddenLayers.size() - 1; ++layerNum)
  {
    layers.push_back(Layer()); // topology.hiddenLayers[layerNum] + topology.bias));
    unsigned numOutputs = topology.hiddenLayers[layerNum + 1] + topology.bias;

    for (unsigned neuronNum = 0; neuronNum < topology.hiddenLayers[layerNum]; ++neuronNum)
      layers.back().push_back(Neuron(numOutputs, neuronNum));

    createBiasNeurons(layerNum + 1, numOutputs); // add bias neurons next from input layer
  }

  layers.push_back(Layer()); // topology.hiddenLayers.back() + topology.bias));
  for (unsigned neuronNum = 0; neuronNum < topology.hiddenLayers.back(); ++neuronNum)
    layers.back().push_back(Neuron(topology.outputLayerSize, neuronNum));

  createBiasNeurons(layers.size() - 1,
                    topology.outputLayerSize); // add bias neurons to last output layer
}

Network::Network(const Topology topology) : name{"neural-web"}, topology{topology}
{
  std::cout << "Network::Network()" << std::endl;
  createInputLayer();
  createHiddenLayers();
  createOutputLayer();
}

void Network::feedForward(const std::vector<double> &inputs)
{
  assert(inputs.size() == topology.inputLayerSize);

  for (unsigned i = 0; i < inputs.size(); ++i)
    layers[0][i].setOutputVal(inputs[i]);

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
