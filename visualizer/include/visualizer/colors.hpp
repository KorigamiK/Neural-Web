#pragma once

#define COLOR_PINK 254, 94, 120

#define COLOR_BACKGROUND COLOR_PINK

#define COLOR_NEURON GET_COLOR_NEURON()

#define COLOR_WHITE 255, 255, 255

#define SCALE_COLOR(r, g, b, scale) (r * scale), (g * scale), (b * scale)
#define GET_COLOR_NEURON() 0, 39, 56
#define GET_SCALE_COLOR(ARG, scale) SCALE_COLOR(ARG, scale)

#define COLOR_GRID 28, 39, 38
#define COLOR_GRID_LINES 120, 120, 120
#define COLOR_GRID_LINES_HOVERED 75, 255, 220

#define COLOR_OUTLINE 255, 255, 255

#define COLOR_FONT 217, 218, 219