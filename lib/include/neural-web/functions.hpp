#pragma once

double randomWeight(void);

struct TransferFunction
{
  double (*function)(double);
  double (*derivative)(double);
};

namespace ActivationFunctions
{
extern TransferFunction sigmoid;
}
