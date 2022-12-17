#pragma once

double randomWeight(void);

struct TransferFunction
{
  double (*function)(double);
  double (*derivative)(double);
};

namespace ActivationFunctions
{
extern TransferFunction Sigmoid;
extern TransferFunction Tanh;
} // namespace ActivationFunctions
