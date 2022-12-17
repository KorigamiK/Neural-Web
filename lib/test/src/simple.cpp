#include "neural-web/network.hpp"
#include "neural-web/training_data.hpp"

#include <cassert>
#include <iostream>
#include <vector>

using std::vector;

void showVectorVals(std::string label, vector<double> &v)
{
  std::cout << label << " ";
  for (unsigned i = 0; i < v.size(); ++i)
    std::cout << v[i] << " ";

  std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
  TrainingData trainData("/home/korigamik/Dev/projects/ml/neural-web/training-data.txt");

  // e.g., { 3, 2, 1 }
  Topology topology;
  trainData.getTopology(topology);

  Network myNet(topology);

  vector<double> inputVals, targetVals, resultVals;
  int trainingPass = 0;

  while (!trainData.isEof())
  {
    ++trainingPass;
    std::cout << std::endl << "Pass " << trainingPass;

    // Get new input data and feed it forward:
    if (trainData.getNextInputs(inputVals) != topology[0])
    {
      break;
    }
    showVectorVals(": Inputs:", inputVals);
    myNet.feedForward(inputVals);

    // Collect the net's actual output results:
    myNet.getResults(resultVals);
    showVectorVals("Outputs:", resultVals);

    // Train the net what the outputs should have been:
    trainData.getTargetOutputs(targetVals);
    showVectorVals("Targets:", targetVals);
    assert(targetVals.size() == topology.back());

    myNet.backPropagate(targetVals);

    // Report how well the training is working, average over recent samples:
    std::cout << "Net recent average error: " << myNet.getRecentAverageError()
              << std::endl;
  }

  std::cout << std::endl << "Done" << std::endl;
}