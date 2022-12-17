#include <iostream>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "neural-web/network.hpp"
#include "visualizer/window.hpp"

auto main(int argc, char const *argv[]) -> int
{
  std::cout << "Visualizer with constant " << APP_CONSTANT << std::endl;

  Window window;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(
      [](void *arg) {
        Window *window = (Window *)arg;
        window->mainLoop();
      },
      &window, 0, 1);
#else
  while (window.running)
  {
    window.mainLoop();
  }
#endif

  return 0;
}
