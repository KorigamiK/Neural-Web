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

/// @brief Rectified Linear Unit (ReLU) activation function
/// @details Will output the input directly if it is positive, otherwise, it will output
/// zero.
extern TransferFunction ReLU;
} // namespace ActivationFunctions