#include "neural-web/functions.hpp"

#include <cstdlib>
#include <math.h>

double randomWeight(void)
{
  return rand() / double(RAND_MAX);
}

TransferFunction ActivationFunctions::Sigmoid = {
    [](double x) { return 1.0 / (1.0 + exp(-x)); },
    [](double x) { return x * (1.0 - x); }};

TransferFunction ActivationFunctions::Tanh = {[](double x) { return tanh(x); },
                                              [](double x) { return 1.0 - x * x; }};

TransferFunction ActivationFunctions::ReLU = {
    [](double x) { return x > 0.0 ? x : 0.0; },
    [](double x) { return x > 0.0 ? 1.0 : 0.0; }};