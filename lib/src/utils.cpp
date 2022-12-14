#include "utils.hpp"

#include <stdexcept>

void PANIC(const char *message)
{
  throw std::runtime_error(message);
}