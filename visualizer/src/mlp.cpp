#include "visualizer/mlp.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "neural-web/functions.hpp"
#include "visualizer/colors.hpp"

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

SDL_Point MLP::getAbsolutePosition(SDL_Point position)
{
  return std::move(SDL_Point{position.x + positionRect.x, position.y + positionRect.y});
}

void MLP::drawNeurons()
{
  auto &topology = neuralNetwork->topology;
  auto &layers = neuralNetwork->getLayers();

  int neuronRadius = NEURON_RADIUS;

  int layerDistance =
      (positionRect.w - 2 * neuronRadius) / (topology.hiddenLayers.size() + 3);

  std::cout << "Layer distance: " << layerDistance << std::endl;

  int x = positionRect.x + layerDistance + neuronRadius;

  for (int i = 0; i < layers.size(); i++)
  {
    int neuronVDistance = positionRect.h / (layers[i].size() + 1);
    int y = neuronRadius + neuronVDistance;

    for (int j = 0; j < layers[i].size(); j++)
    {
      // Draw connections
      int nextNeuronVDistance = positionRect.h / (layers[i + 1].size() + 1);
      for (int k = 0; k < layers[i][j].numOutputs; k++)
      {
        int nextLayer = i + 1;
        int nextNeuron = k;

        int nextX = x + layerDistance;
        int nextY = neuronRadius + nextNeuronVDistance * (nextNeuron + 1);

        int weight = layers[i][j].getOutputWeights()[k].weight;

        aalineRGBA(renderer, x, y, nextX, nextY, COLOR_WHITE, 255 * (1 - weight));
      }

      auto scaleFactor =
          ActivationFunctions::sigmoid.function(layers[i][j].getOutputVal() + 0.1);
      // Draw neuron with color scaled by output value
      filledCircleRGBA(renderer, x, y, neuronRadius,
                       GET_SCALE_COLOR(GET_COLOR_NEURON(), scaleFactor), 255);
      aacircleRGBA(renderer, x, y, neuronRadius, COLOR_WHITE, 255);

      y += neuronVDistance;
    }
    x += layerDistance;
    y = neuronRadius;
  }
}

void MLP::drawNetwork()
{
  std::cout << "Drawing network" << std::endl;

  drawNeurons();
}

void MLP::update()
{
}

MLP::~MLP()
{
}