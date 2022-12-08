#include <iostream>

#include "neural-web/library.hpp"
#include "visualizer/app.hpp"

auto main(int argc, char const *argv[]) -> int
{
  std::cout << "Visualizer with constant " << APP_CONSTANT << std::endl;
  library lib;

  std::cout << "Library " << lib.name << std::endl;

  return 0;
}
