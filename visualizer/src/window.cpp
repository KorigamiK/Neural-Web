#include "visualizer/window.hpp"

#include "utils.hpp"
#include "visualizer/colors.hpp"
#include <SDL2/SDL_image.h>

Window::Window()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0 || IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    PANIC("Failed to initialize SDL");

  window = std::shared_ptr<SDL_Window>(
      SDL_CreateWindow("Visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN),
      SDL_DestroyWindow);

  if (!window)
    PANIC("Failed to create SDL window");

  renderer = std::shared_ptr<SDL_Renderer>(
      SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), SDL_DestroyRenderer);

  if (!renderer)
    PANIC("Failed to create SDL renderer");

  SDL_SetRenderDrawColor(renderer.get(), COLOR_PINK, 255);

  SDL_RenderPresent(renderer.get());
}

Window::~Window()
{
}

void Window::mainLoop()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
      exit(0);
  }

  SDL_RenderClear(renderer.get());
  SDL_RenderPresent(renderer.get());
}