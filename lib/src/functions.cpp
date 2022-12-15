#include "neural-web/functions.hpp"

#include <cstdlib>
#include <math.h>

double randomWeight(void)
{
  return rand() / double(RAND_MAX);
}

TransferFunction ActivationFunctions::sigmoid = {
    [](double x) { return 1.0 / (1.0 + exp(-x)); },
    [](double x) { return x * (1.0 - x); }};
