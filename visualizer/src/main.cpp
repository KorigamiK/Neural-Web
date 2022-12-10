#include <iostream>

#include "neural-web/network.hpp"
#include "visualizer/app.hpp"

auto main(int argc, char const *argv[]) -> int
{
  std::cout << "Visualizer with constant " << APP_CONSTANT << std::endl;
  Network lib;

  std::cout << "Library " << lib.name << std::endl;

  return 0;
}
