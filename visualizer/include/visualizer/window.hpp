#pragma once

#include <SDL2/SDL.h>
#include <memory>

#define APP_CONSTANT (0.01f)

#define WINDOW_WIDTH (480)
#define WINDOW_HEIGHT (272)

/// @brief Base window class for the application
class Window
{
  friend int main(int, char const *argv[]);

private:
  bool running = true;
  void stop();

public:
  SDL_Window *window;
  SDL_Renderer *renderer;

  /// @brief Initialize and create the SDL window
  Window();
  ~Window();

  void mainLoop();
};
