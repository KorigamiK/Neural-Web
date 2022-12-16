#include "visualizer/mlp.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

MLP::MLP(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect positionRect)
    : window{window}, renderer{renderer}, positionRect{positionRect}
{
  Topology networkTopology{
      .inputLayerSize = 3, .outputLayerSize = 2, .hiddenLayers = {2, 3}, .bias = 1};
  neuralNetwork = std::make_unique<Network>(networkTopology);
}

void MLP::draw()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(renderer, &positionRect);

  drawNetwork();
}

void MLP::updatePositionRect(SDL_Rect positionRect)
{
  this->positionRect = positionRect;
}

void MLP::drawNetwork()
{
  std::cout << "Drawing network" << std::endl;

  // Draw hidden layers
  auto &layers = neuralNetwork->getLayers();
  auto &topology = neuralNetwork->topology;

  int layerWidth = positionRect.w / (topology.hiddenLayers.size() + 2);
  int layerHeight = positionRect.h / (topology.hiddenLayers.size() + 2);

  int layerX = positionRect.x + layerWidth;
  int layerY = positionRect.y + layerHeight;

  for (auto &layer : layers)
  {
    for (auto &neuron : layer)
    {
      filledCircleRGBA(renderer, layerX, layerY, 10, 255, 255, 255, 255);
      layerY += layerHeight;
    }
    layerX += layerWidth;
    layerY = positionRect.y + layerHeight;
  }
}

void MLP::update()
{
}

MLP::~MLP()
{
}