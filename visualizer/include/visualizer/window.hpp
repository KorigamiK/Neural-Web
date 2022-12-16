#pragma once

#include <SDL2/SDL.h>
#include <memory>

#include "visualizer/io_grid.hpp"
#include "visualizer/mlp.hpp"

#define APP_CONSTANT (0.01f)

#define WINDOW_WIDTH (480)
#define WINDOW_HEIGHT (272)

/// @brief Base window class for the application
class Window
{
  friend int main(int, char const *argv[]);

private:
  bool running = true;
  bool repaint = true;
  std::shared_ptr<Network> neuralNetwork;
  std::unique_ptr<MLP> mlpVisualizer;
  std::unique_ptr<IOGrid> ioGrid;

  void stop();
  void initNeuralNetwork();

public:
  SDL_Window *window;
  SDL_Renderer *renderer;

  /// @brief Initialize and create the SDL window
  Window();
  ~Window();

  void mainLoop();
};
