#include "visualizer/mlp.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "graphics/font.hpp"
#include "neural-web/functions.hpp"
#include "visualizer/colors.hpp"

MLP::MLP(SDL_Renderer *renderer, SDL_Rect positionRect,
         std::shared_ptr<Network> neuralNetwork)
    : renderer{renderer}, positionRect{positionRect}, neuralNetwork{neuralNetwork}
{
  std::cout << "MLP created" << std::endl;
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
  auto &topology = neuralNetwork->getTopology();
  auto &layers = neuralNetwork->getLayers();

  int neuronRadius = NEURON_RADIUS;

  int layerDistance = (positionRect.w - 2 * neuronRadius) / (topology.size() + 1);

  int x = positionRect.x + layerDistance + neuronRadius;

  for (int i = 0; i < layers.size(); i++)
  {
    int neuronVDistance = positionRect.h / (layers[i].size() + 1);
    int y = neuronRadius + neuronVDistance;

    for (int j = 0; j < layers[i].size(); j++)
    {
      // Draw connections
      int nextNeuronVDistance = positionRect.h / (layers[i + 1].size() + 1);
      for (int k = 0; k < layers[i][j].getNumOutputs(); k++)
      {
        int nextLayer = i + 1;
        int nextNeuron = k;

        int nextX = x + layerDistance;
        int nextY = neuronRadius + nextNeuronVDistance * (nextNeuron + 1);

        int weight = layers[i][j].getOutputWeights()[k].weight;

        aalineRGBA(renderer, x, y, nextX, nextY, COLOR_WHITE, 255 * (1 - weight));
      }

      auto scaleFactor =
          ActivationFunctions::Sigmoid.function(layers[i][j].getOutputVal() + 0.1);

      filledCircleRGBA(renderer, x, y, neuronRadius,
                       GET_SCALE_COLOR(GET_COLOR_NEURON(), scaleFactor), 255);
      aacircleRGBA(renderer, x, y, neuronRadius, COLOR_WHITE, 255);

      // Draw neuron value
      // TODO: Fix this

      std::stringstream fixedNeuronValue;
      fixedNeuronValue << std::fixed << std::setprecision(2)
                       << layers[i][j].getOutputVal();
      std::string neuronValue = fixedNeuronValue.str();
      auto *valueTexture =
          FontManager::getTexture(neuronValue, SDL_Color{COLOR_FONT}, renderer);

      int valueTextureWidth;
      int valueTextureHeight;
      SDL_QueryTexture(valueTexture, NULL, NULL, &valueTextureWidth, &valueTextureHeight);

      SDL_Rect valueTextureRect = {x - valueTextureWidth / 2, y - valueTextureHeight / 2,
                                   valueTextureWidth, valueTextureHeight};

      SDL_RenderCopy(renderer, valueTexture, NULL, &valueTextureRect);

      SDL_DestroyTexture(valueTexture);

      y += neuronVDistance;
    }
    x += layerDistance;
    y = neuronRadius;
  }
}

void MLP::drawNetwork()
{
  drawNeurons();
}

void MLP::update()
{
}

MLP::~MLP()
{
  std::cout << "MLP destroyed" << std::endl;
}
