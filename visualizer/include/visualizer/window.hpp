#pragma once

#include <SDL2/SDL.h>
#include <memory>

#define APP_CONSTANT (0.01f)

/// @brief Base window class for the application
class Window
{
public:
  std::shared_ptr<SDL_Window> window;
  std::shared_ptr<SDL_Renderer> renderer;

  /// @brief Initialize and create the SDL window
  Window();
  ~Window();

  void mainLoop();
};