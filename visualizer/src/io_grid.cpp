#include "visualizer/io_grid.hpp"

#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>

#include "visualizer/colors.hpp"

IOGrid::IOGrid(SDL_Renderer *renderer, SDL_Rect positionRect)
    : renderer{renderer}, positionRect{positionRect}
{
  std::cout << "IOGrid constructor" << std::endl;
}

void IOGrid::drawUserInputs()
{
  for (auto &input : UserInputs)
  {
    int cellRadius = positionRect.w / GRID_SIZE_X / 4;

    // draw a circle instead of a rectangle
    int x = positionRect.x + input.first.first * positionRect.w / GRID_SIZE_X +
            positionRect.w / GRID_SIZE_X / 2;

    int y = positionRect.y + input.first.second * positionRect.h / GRID_SIZE_Y +
            positionRect.h / GRID_SIZE_Y / 2;

    filledCircleRGBA(renderer, x, y, cellRadius, input.second.r, input.second.g,
                     input.second.b, 255);
    aacircleRGBA(renderer, x, y, cellRadius, input.second.r, input.second.g,
                 input.second.b, 255);
  }
}

void IOGrid::draw()
{
  SDL_SetRenderDrawColor(renderer, COLOR_GRID, 255);
  SDL_RenderFillRect(renderer, &positionRect);

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

  drawUserInputs();
}

void IOGrid::updateMousePosition(int x, int y)
{
  SDL_Point mousePosition = {x, y};
  if (SDL_PointInRect(&mousePosition, &positionRect))
  {
    cellHovered.first = (x - positionRect.x) * GRID_SIZE_X / positionRect.w;
    cellHovered.second = (y - positionRect.y) * GRID_SIZE_Y / positionRect.h;
  }
  else
  {
    cellHovered.first = -1;
    cellHovered.second = -1;
  }
}

void IOGrid::addSelectedCell()
{
  if (cellHovered.first != -1 && cellHovered.second != -1)
    UserInputs[cellHovered] = {255, 255, 255};
}

void IOGrid::update()
{
}

void IOGrid::updatePositionRect(SDL_Rect positionRect)
{
  this->positionRect = positionRect;
}

IOGrid::~IOGrid()
{
  std::cout << "IOGrid destructor" << std::endl;
}
