#include "neural-web/network.hpp"

#include <cassert>
#include <iostream>
#include <math.h>

#include "neural-web/neuron.hpp"

double Network::m_recentAverageSmoothingFactor = 100.0;

void Network::getResults(std::vector<double> &resultVals) const
{
  resultVals.clear();

  for (unsigned n = 0; n < m_layers.back().size() - 1; ++n)
    resultVals.push_back(m_layers.back()[n].getOutputVal());
}

const Topology &Network::getTopology(void) const
{
  return m_topology;
}

const std::vector<Layer> &Network::getLayers(void) const
{
  return m_layers;
}

void Network::backPropagate(const std::vector<double> &targetVals)
{
  // for (int i = 0; i < targetVals.size(); i++)
  //   std::cout << "targetVals[" << i << "] = " << targetVals[i] << std::endl;

  // Calculate overall net error (RMS of output neuron errors)
  Layer &outputLayer = m_layers.back();
  m_error = 0.0;

  for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
  {
    double delta = targetVals[n] - outputLayer[n].getOutputVal();
    m_error += delta * delta;
  }
  m_error /= outputLayer.size() - 1; // get average error squared
  m_error = sqrt(m_error);           // RMS

  // Implement a recent average measurement
  m_recentAverageError =
      (m_recentAverageError * m_recentAverageSmoothingFactor + m_error) /
      (m_recentAverageSmoothingFactor + 1.0);

  // Calculate output layer gradients
  for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
  {
    outputLayer[n].calcOutputGradients(targetVals[n]);
  }

  // Calculate hidden layer gradients
  for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
  {
    Layer &hiddenLayer = m_layers[layerNum];
    Layer &nextLayer = m_layers[layerNum + 1];

    for (unsigned n = 0; n < hiddenLayer.size(); ++n)
    {
      hiddenLayer[n].calcHiddenGradients(nextLayer);
    }
  }

  // For all layers from outputs to first hidden layer,
  // update connection weights

  for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
  {
    Layer &layer = m_layers[layerNum];
    Layer &prevLayer = m_layers[layerNum - 1];

    for (unsigned n = 0; n < layer.size() - 1; ++n)
    {
      layer[n].updateInputWeights(prevLayer);
    }
  }
}

void Network::feedForward(const std::vector<double> &inputVals)
{
  assert(inputVals.size() == m_layers[0].size() - 1);

  // Assign (latch) the input values into the input neurons
  for (unsigned i = 0; i < inputVals.size(); ++i)
  {
    m_layers[0][i].setOutputVal(inputVals[i]);
  }

  // forward propagate
  for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum)
  {
    Layer &prevLayer = m_layers[layerNum - 1];
    for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n)
    {
      m_layers[layerNum][n].feedForward(prevLayer);
    }
  }
}

double Network::getRecentAverageError(void) const
{
  return m_recentAverageError;
}

Network::Network(const Topology &topology) : m_topology(topology)
{
  unsigned numLayers = topology.size();
  for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
  {
    m_layers.push_back(Layer());
    unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

    // We have a new layer, now fill it with neurons, and
    // add a bias neuron in each layer.
    for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)
    {
      m_layers.back().push_back(Neuron(numOutputs, neuronNum));
      std::cout << "Made a Neuron!" << std::endl;
    }

    // Force the bias node's output to 1.0 (it was the last neuron pushed in
    // this layer):
    m_layers.back().back().setOutputVal(1.0);
  }
}