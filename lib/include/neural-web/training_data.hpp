#pragma once

#include <fstream>
#include <vector>

#include "neural-web/topology.hpp"

class TrainingData
{
public:
  TrainingData(const char *filename);
  bool isEof(void);
  void getTopology(Topology &topology);

  // Returns the number of input values read from the file:
  unsigned getNextInputs(std::vector<double> &inputVals);
  unsigned getTargetOutputs(std::vector<double> &targetOutputVals);

private:
  std::ifstream m_trainingDataFile;
};
