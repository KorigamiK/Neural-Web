#include <SDL2/SDL_image.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "utils.hpp"
#include "visualizer/colors.hpp"
#include "visualizer/window.hpp"

Window::Window()
{
  log("Window::Window()");

  if (SDL_Init(SDL_INIT_VIDEO) != 0 || IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    PANIC("Failed to initialize SDL");

  window = SDL_CreateWindow("Visualizer", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                            WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

  if (window == nullptr)
    PANIC("Failed to create window");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == nullptr)
    PANIC("Failed to create renderer");

  SDL_SetRenderDrawColor(renderer, COLOR_PINK, 255);

  SDL_RenderPresent(renderer);
}

Window::~Window()
{
  log("Window::~Window()");
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
}

void Window::mainLoop()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
      stop();
  }

  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

void Window::stop()
{
  running = false;
#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
#endif
}
