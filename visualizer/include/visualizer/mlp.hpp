#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <neural-web/network.hpp>

/// @brief Class for a Multi-Layer Perceptron visualization
class MLP
{
private:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Rect positionRect;

  std::unique_ptr<Network> neuralNetwork;

  void drawNetwork();

public:
  MLP(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect positionRect);

  void draw();
  void update();

  void updatePositionRect(SDL_Rect positionRect);

  ~MLP();
};