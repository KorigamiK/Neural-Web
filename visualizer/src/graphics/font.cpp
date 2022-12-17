#include "graphics/font.hpp"

#include <filesystem>

#include "util.hpp"

int FontManager::init()
{
  log("FontManager::init()");

  if (TTF_Init() != 0 || !std::filesystem::exists(FONT_PATH))
    return -1;

  loadFont(FONT_PATH, fontBaseSize);
  return 0;
}

void FontManager::loadFont(const char *path, int size)
{
  log("FontManager::loadFont()");

  TTF_Font *font = TTF_OpenFont(path, size);
  if (font == nullptr)
    log("Failed to load font ");
  else
    fontBase = font;
}

SDL_Texture *FontManager::getTexture(std::string &text, SDL_Color color,
                                     SDL_Renderer *renderer)
{
  SDL_Surface *surface = TTF_RenderText_Solid(fontBase, text.c_str(), color);
  if (surface == nullptr)
    log("Failed to create surface from text");

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (texture == nullptr)
    log("Failed to create texture from surface");

  SDL_FreeSurface(surface);

  return texture;
}

void FontManager::close()
{
  log("FontManager::close()");

  TTF_CloseFont(fontBase);
  TTF_Quit();
}
