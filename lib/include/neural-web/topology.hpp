#pragma once

#include <vector>

/**
 * @brief Topology of the network
 *      - The first element is the number of inputs
 *      - The last element is the number of outputs
 *      - The rest are the number of neurons in each hidden layer not including the bias
 *        neuron
 *
 */
using Topology = std::vector<unsigned>;