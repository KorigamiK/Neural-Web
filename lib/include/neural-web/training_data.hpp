#pragma once

#include <fstream>
#include <vector>

class TrainingData
{
public:
  TrainingData(const char *filename);
  inline bool isEof(void)
  {
    return m_trainingDataFile.eof();
  }
  void getTopology(std::vector<unsigned> &topology);

  // Returns the number of input values read from the file:
  unsigned getNextInputs(std::vector<double> &inputVals);
  unsigned getTargetOutputs(std::vector<double> &targetOutputVals);

private:
  std::ifstream m_trainingDataFile;
};
