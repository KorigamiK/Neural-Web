#include "visualizer/io_grid.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <algorithm>
#include <iostream>
#include <vector>

#include "visualizer/colors.hpp"

IOGrid::IOGrid(SDL_Renderer *renderer, SDL_Rect positionRect,
               std::shared_ptr<Network> neuralNetwork)
    : renderer{renderer}, positionRect{positionRect}, neuralNetwork{neuralNetwork}
{
  std::cout << "IOGrid constructor" << std::endl;
}

void IOGrid::drawUserInputs()
{
  for (auto &input : UserInputs)
  {
    int cellRadius = positionRect.w / GRID_SIZE_X / 4;

    int x = positionRect.x + input.first.first * positionRect.w / GRID_SIZE_X +
            positionRect.w / GRID_SIZE_X / 2;

    int y = positionRect.y + input.first.second * positionRect.h / GRID_SIZE_Y +
            positionRect.h / GRID_SIZE_Y / 2;

    filledCircleRGBA(renderer, x, y, cellRadius,
                     SCALE_COLOR(input.second.r, input.second.g, input.second.b, 255),
                     255);
    aacircleRGBA(renderer, x, y, cellRadius, COLOR_OUTLINE, 255);
  }
}

void IOGrid::drawGridLines()
{
  for (int i = 0; i < GRID_SIZE_X; ++i)
  {
    SDL_SetRenderDrawColor(renderer, COLOR_GRID_LINES, 255);
    SDL_RenderDrawLine(renderer, positionRect.x + i * positionRect.w / GRID_SIZE_X,
                       positionRect.y, positionRect.x + i * positionRect.w / GRID_SIZE_X,
                       positionRect.y + positionRect.h);
  }

  for (int i = 0; i < GRID_SIZE_Y; ++i)
  {
    SDL_SetRenderDrawColor(renderer, COLOR_GRID_LINES, 255);
    SDL_RenderDrawLine(renderer, positionRect.x,
                       positionRect.y + i * positionRect.h / GRID_SIZE_Y,
                       positionRect.x + positionRect.w,
                       positionRect.y + i * positionRect.h / GRID_SIZE_Y);
  }
}

void IOGrid::drawHoverdCell()
{
  if (cellHovered.first != -1 && cellHovered.second != -1)
  {
    SDL_SetRenderDrawColor(renderer, COLOR_GRID_LINES_HOVERED, 255);
    SDL_Rect cellRect = {
        .x = positionRect.x + cellHovered.first * positionRect.w / GRID_SIZE_X,
        .y = positionRect.y + cellHovered.second * positionRect.h / GRID_SIZE_Y,
        .w = positionRect.w / GRID_SIZE_X,
        .h = positionRect.h / GRID_SIZE_Y};
    SDL_RenderDrawRect(renderer, &cellRect);
  }
}

void IOGrid::colorGrid()
{
  static std::vector<double> output;
  for (int i = 0; i < GRID_SIZE_X; ++i)
  {
    for (int j = 0; j < GRID_SIZE_Y; ++j)
    {
      std::vector<double> input = {i / (double)GRID_SIZE_X, j / (double)GRID_SIZE_Y};
      neuralNetwork->feedForward(input);
      neuralNetwork->getResults(output);
      double r = output[0] * 255;
      double g = output[1] * 255;
      double b = output[2] * 255;
      SDL_Rect cellRect = {.x = positionRect.x + i * positionRect.w / GRID_SIZE_X,
                           .y = positionRect.y + j * positionRect.h / GRID_SIZE_Y,
                           .w = positionRect.w / GRID_SIZE_X,
                           .h = positionRect.h / GRID_SIZE_Y};
      SDL_SetRenderDrawColor(renderer, r, g, b, 255);
      SDL_RenderFillRect(renderer, &cellRect);
    }
  }
  std::cout << neuralNetwork->getRecentAverageError() << std::endl;
}

void IOGrid::draw()
{
  SDL_SetRenderDrawColor(renderer, COLOR_GRID, 255);
  SDL_RenderFillRect(renderer, &positionRect);

  colorGrid();
  // drawGridLines();
  // drawHoverdCell();
  drawUserInputs();
}

/* // Not used
bool IOGrid::updateMousePosition(int x, int y)
{
  SDL_Point mousePosition = {x, y};
  if (SDL_PointInRect(&mousePosition, &positionRect))
  {
    cellHovered.first = (x - positionRect.x) * GRID_SIZE_X / positionRect.w;
    cellHovered.second = (y - positionRect.y) * GRID_SIZE_Y / positionRect.h;
    // std::cout << cellHovered.first << " " << cellHovered.second << std::endl;
    return true;
  }
  else
  {
    cellHovered.first = -1;
    cellHovered.second = -1;
    return false;
  }
}
*/

bool IOGrid::addSelectedCell(int x, int y)
{
  /*
if (cellHovered.first == -1 || cellHovered.second == -1)
  return false;

if (UserInputs.find(cellHovered) != UserInputs.end())
{
  UserInputs.erase(cellHovered);
  return true;
}
  */

  SDL_Point mousePosition = {x, y};
  if (!SDL_PointInRect(&mousePosition, &positionRect))
    return false;

  int cellX = (x - positionRect.x) * GRID_SIZE_X / positionRect.w;
  int cellY = (y - positionRect.y) * GRID_SIZE_Y / positionRect.h;
  std::pair<int, int> cell = {cellX, cellY};

  if (UserInputs.find(cell) != UserInputs.end())
    UserInputs.erase(cell);

  std::pair<int, int> selectedCell = {cellX, cellY};

  switch (outputType)
  {
  case OutputType::RED:
    UserInputs[selectedCell] = {1, 0, 0};
    break;
  case OutputType::GREEN:
    UserInputs[selectedCell] = {0, 1, 0};
    break;
  case OutputType::BLUE:
    UserInputs[selectedCell] = {0, 0, 1};
    break;
  }

  return true;
}

void IOGrid::setOutputType(OutputType outputType)
{
  this->outputType = outputType;
}

void IOGrid::update()
{
  static std::vector<double> targetVector(3);
  static std::vector<double> inputVector(2);

  std::cout << "training..." << std::endl;
  for (int i = 0; i < GRID_SIZE_X; ++i)
  {
    for (int j = 0; j < GRID_SIZE_Y; ++j)
    {
      inputVector[0] = (double)(i / GRID_SIZE_X);
      inputVector[1] = (double)(j / GRID_SIZE_Y);
      neuralNetwork->feedForward(inputVector);

      if (UserInputs.find({i, j}) != UserInputs.end())
      {
        targetVector[0] = UserInputs[{i, j}].r;
        targetVector[1] = UserInputs[{i, j}].g;
        targetVector[2] = UserInputs[{i, j}].b;
      }
      else
      {
        targetVector[0] = 0;
        targetVector[1] = 0;
        targetVector[2] = 0;
      }

      neuralNetwork->backPropagate(targetVector);
    }
  }
}

void IOGrid::updatePositionRect(SDL_Rect positionRect)
{
  this->positionRect = positionRect;
}

IOGrid::~IOGrid()
{
  std::cout << "IOGrid destructor" << std::endl;
}
