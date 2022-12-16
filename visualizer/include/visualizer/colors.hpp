#pragma once

#define COLOR_PINK 254, 94, 120

#define COLOR_BACKGROUND COLOR_PINK

#define COLOR_NEURON GET_COLOR_NEURON()

#define COLOR_WHITE 255, 255, 255

#define SCALE_COLOR(r, g, b, scale) (r * scale), (g * scale), (b * scale)
#define GET_COLOR_NEURON() 0, 39, 56
#define GET_SCALE_COLOR(ARG, scale) SCALE_COLOR(ARG, scale)