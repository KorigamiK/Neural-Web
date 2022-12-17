#include "neural-web/training_data.hpp"

#include <iostream>
#include <sstream>

#include "neural-web/topology.hpp"

bool TrainingData::isEof(void)
{
  return m_trainingDataFile.eof();
}

void TrainingData::getTopology(Topology &topology)
{
  std::string line;
  std::string label;

  getline(m_trainingDataFile, line);
  std::stringstream ss(line);
  ss >> label;
  if (this->isEof() || label.compare("topology:") != 0)
  {
    abort();
  }

  while (!ss.eof())
  {
    unsigned n;
    ss >> n;
    topology.push_back(n);
  }

  return;
}

TrainingData::TrainingData(const char *filename)
{
  m_trainingDataFile.open(filename);
}

unsigned TrainingData::getNextInputs(std::vector<double> &inputVals)
{
  inputVals.clear();

  std::string line;
  getline(m_trainingDataFile, line);
  std::stringstream ss(line);

  std::string label;
  ss >> label;
  if (label.compare("in:") == 0)
  {
    double oneValue;
    while (ss >> oneValue)
    {
      inputVals.push_back(oneValue);
    }
  }

  return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(std::vector<double> &targetOutputVals)
{
  targetOutputVals.clear();

  std::string line;
  getline(m_trainingDataFile, line);
  std::stringstream ss(line);

  std::string label;
  ss >> label;
  if (label.compare("out:") == 0)
  {
    double oneValue;
    while (ss >> oneValue)
    {
      targetOutputVals.push_back(oneValue);
    }
  }

  return targetOutputVals.size();
}
