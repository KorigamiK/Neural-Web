#include "visualizer/window.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL2/SDL_image.h>

#include "graphics/font.hpp"
#include "util.hpp"
#include "visualizer/colors.hpp"

Window::Window()
{
  log("Window::Window()");

  if (SDL_Init(SDL_INIT_VIDEO) != 0 || IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG ||
      FontManager::init() != 0)
    PANIC("Failed to initialize SDL");

  window = SDL_CreateWindow(
      "Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
      WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  if (window == nullptr)
    PANIC("Failed to create window");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == nullptr)
    PANIC("Failed to create renderer");

  initNeuralNetwork();

  mlpVisualizer = std::make_unique<MLP>(
      renderer,
      SDL_Rect{.x = 20, .y = 20, .w = 2 * WINDOW_WIDTH / 3 - 20, .h = WINDOW_HEIGHT - 40},
      neuralNetwork);

  ioGrid = std::make_unique<IOGrid>(renderer,
                                    SDL_Rect{.x = 2 * WINDOW_WIDTH / 3,
                                             .y = 20,
                                             .w = WINDOW_WIDTH / 3 - 20,
                                             .h = WINDOW_HEIGHT - 40},
                                    neuralNetwork);
}

Window::~Window()
{
  log("Window::~Window()");
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  FontManager::close();
  SDL_Quit();
}

void Window::mainLoop()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      stop();
      break;

    case SDL_WINDOWEVENT:
      switch (event.window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
        mlpVisualizer->updatePositionRect(
            std::move(SDL_Rect{.x = 20,
                               .y = 20,
                               .w = 2 * event.window.data1 / 3 - 20,
                               .h = event.window.data2 - 40}));

        ioGrid->updatePositionRect(std::move(SDL_Rect{.x = 2 * event.window.data1 / 3,
                                                      .y = 20,
                                                      .w = event.window.data1 / 3 - 20,
                                                      .h = event.window.data2 - 40}));
        repaint = true;
        break;
      }
      break;

      /*	// Hover actions are not needed for now
        case SDL_MOUSEMOTION:
            ioGrid->updateMousePosition(event.motion.x,
                                                                    event.motion.y);
            break;
            */

    case SDL_MOUSEBUTTONDOWN:
      if (ioGrid->addSelectedCell(event.button.x, event.button.y))
      {
        repaint = true;
        iterationCount = epochCount;
      }
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
      case SDLK_r:
        ioGrid->setOutputType(OutputType::RED);
        repaint = true;
        break;

      case SDLK_g:
        ioGrid->setOutputType(OutputType::GREEN);
        repaint = true;
        break;

      case SDLK_b:
        ioGrid->setOutputType(OutputType::BLUE);
        repaint = true;
        break;
      }
      break;
    }
  }

  if (repaint || iterationCount > 0)
  {
    repaint = false;
    iterationCount--;

    SDL_SetRenderDrawColor(renderer, COLOR_BACKGROUND, 255);
    SDL_RenderClear(renderer);

    ioGrid->update();
    mlpVisualizer->update();

    ioGrid->draw();
    mlpVisualizer->draw();

    SDL_RenderPresent(renderer);
  }
#ifndef __EMSCRIPTEN__
  SDL_Delay(50);
#endif
}

void Window::initNeuralNetwork()
{
  Topology networkTopology{2, 4, 2, 3};
  neuralNetwork = std::make_shared<Network>(networkTopology);
}

void Window::stop()
{
  running = false;
#ifdef __EMSCRIPTEN__
  emscripten_cancel_main_loop();
#endif
}
