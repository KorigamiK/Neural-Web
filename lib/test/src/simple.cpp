#include "neural-web/network.hpp"
#include "neural-web/training_data.hpp"

#include <cassert>
#include <iostream>
#include <vector>

void showVectorVals(std::string label, std::vector<double> &v)
{
  std::cout << label << " ";
  for (unsigned i = 0; i < v.size(); ++i)
    std::cout << v[i] << " ";

  std::cout << std::endl;
}

auto main(int argc, char const *argv[]) -> int
{
  auto network = Network(Topology{
      .inputLayerSize = 2, .outputLayerSize = 1, .hiddenLayers = {4}, .bias = 1});
  auto trainingData =
      TrainingData("/home/korigamik/Dev/projects/ml/neural-web/training-data.txt");

  std::vector<double> inputVals, targetVals, resultVals;
  unsigned trainingPass = 0;

  while (!trainingData.isEof())
  {
    ++trainingPass;
    std::cout << std::endl << "Pass " << trainingPass;

    // Get new input data and feed it forward:
    assert(trainingData.getNextInputs(inputVals) == network.topology.inputLayerSize);
    showVectorVals(": Inputs:", inputVals);
    network.feedForward(inputVals);
    network.getResults(resultVals);
    showVectorVals("Outputs:", resultVals);

    // Train the net what the outputs should have been:
    trainingData.getTargetOutputs(targetVals);
    showVectorVals("Targets:", targetVals);
    assert(targetVals.size() == network.topology.outputLayerSize);

    network.backPropagate(targetVals);

    // Report how well the training is working, average over recent samples:
    std::cout << "Net recent average error: " << network.getRecentAverageError()
              << std::endl;
  }

  std::cout << std::endl << "Done" << std::endl;

  return network.name == "neural-web" ? 0 : 1;
}
