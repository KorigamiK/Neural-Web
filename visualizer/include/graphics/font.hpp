#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#define FONT_PATH "res/fonts/IBMPlexMono-Medium.ttf"

namespace FontManager
{
static TTF_Font *fontBase;
static const int fontBaseSize = 10;

int init();
void close();
void loadFont(const char *text, int size);
SDL_Texture *getTexture(std::string &text, SDL_Color color, SDL_Renderer *renderer);

} // namespace FontManager
