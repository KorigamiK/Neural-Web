#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <memory>

#include "neural-web/network.hpp"

#define GRID_SIZE_X 10
#define GRID_SIZE_Y 15

struct Output
{
  int r, g, b;
};

enum class OutputType
{
  RED,
  GREEN,
  BLUE
};

class IOGrid
{
private:
  SDL_Renderer *renderer;
  SDL_Rect positionRect;
  std::shared_ptr<Network> neuralNetwork;
  std::pair<int, int> cellHovered = {-1, -1};
  std::map<std::pair<int, int>, Output> UserInputs;
  OutputType outputType = OutputType::RED;

  void drawUserInputs();
  void drawGridLines();
  void drawHoverdCell();

public:
  IOGrid(SDL_Renderer *renderer, SDL_Rect positionRect);
  ~IOGrid();

  void draw();
  void update();
  void updateMousePosition(int x, int y);
  void addSelectedCell();
  void updatePositionRect(SDL_Rect positionRect);
  void setOutputType(OutputType outputType);
};