/*
    Draws on screen using opengl 3 in PC or openglES 2 in browser.
    Precompile command : "D:\exarion\vendor\emscripten\SDK\emscripten\1.35.0\emcc" --clear-cache
    Compile command : "D:\exarion\vendor\emscripten\SDK\emscripten\1.35.0\emcc" Source.cpp -s USE_SDL=2 -s FULL_ES2=1 -o
   test.html -O3 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s SDL2_IMAGE_FORMATS="['png']" -s
   EXPORTED_FUNCTIONS="['_main', '_mainLoop']" -std=c++11 -Werror -s WARN_ON_UNDEFINED_SYMBOLS=1 -s SIMD=1 -s
   NO_EXIT_RUNTIME=1 -s AGGRESSIVE_VARIABLE_ELIMINATION=1 -s SEPARATE_ASM=1 Run generated page in server.
*/

#include "SDL2/SDL.h"

#ifdef __EMSCRIPTEN__
#include "SDL2/SDL_opengles2.h"
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL2/SDL_opengl.h>
#endif

#include <stdio.h>
#include <string>

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

GLuint programObject;
int width = 1024;
int height = 768;

GLint uniformZoom, uniformColor, uniformMVP;
float zoom = 1.0;

SDL_Window *window;
SDL_GLContext context;

// Main loop flag
bool quit = false;

// Event handler
SDL_Event e;

// The application time based timer
class LTimer
{
public:
  // Initializes variables
  LTimer();

  // The various clock actions
  void start();
  void stop();
  void pause();
  void unpause();

  // Gets the timer's time
  Uint32 getTicks();

  // Checks the status of the timer
  bool isStarted();
  bool isPaused();

private:
  // The clock time when the timer started
  Uint32 mStartTicks;

  // The ticks stored when the timer was paused
  Uint32 mPausedTicks;

  // The timer status
  bool mPaused;
  bool mStarted;
};

LTimer::LTimer()
{
  // Initialize the variables
  mStartTicks = 0;
  mPausedTicks = 0;

  mPaused = false;
  mStarted = false;
}

void LTimer::start()
{
  // Start the timer
  mStarted = true;

  // Unpause the timer
  mPaused = false;

  // Get the current clock time
  mStartTicks = SDL_GetTicks();
  mPausedTicks = 0;
}

void LTimer::stop()
{
  // Stop the timer
  mStarted = false;

  // Unpause the timer
  mPaused = false;

  // Clear tick variables
  mStartTicks = 0;
  mPausedTicks = 0;
}

void LTimer::pause()
{
  // If the timer is running and isn't already paused
  if (mStarted && !mPaused)
  {
    // Pause the timer
    mPaused = true;

    // Calculate the paused ticks
    mPausedTicks = SDL_GetTicks() - mStartTicks;
    mStartTicks = 0;
  }
}

void LTimer::unpause()
{
  // If the timer is running and paused
  if (mStarted && mPaused)
  {
    // Unpause the timer
    mPaused = false;

    // Reset the starting ticks
    mStartTicks = SDL_GetTicks() - mPausedTicks;

    // Reset the paused ticks
    mPausedTicks = 0;
  }
}

Uint32 LTimer::getTicks()
{
  // The actual timer time
  Uint32 time = 0;

  // If the timer is running
  if (mStarted)
  {
    // If the timer is paused
    if (mPaused)
    {
      // Return the number of ticks when the timer was paused
      time = mPausedTicks;
    }
    else
    {
      // Return the current time minus the start time
      time = SDL_GetTicks() - mStartTicks;
    }
  }

  return time;
}

bool LTimer::isStarted()
{
  // Timer is running and paused or unpaused
  return mStarted;
}

bool LTimer::isPaused()
{
  // Timer is running and paused
  return mPaused && mStarted;
}

// The frames per second timer
LTimer fpsTimer;

// Start counting frames per second
int countedFrames = 0;

GLuint LoadShader(GLenum type, const char *shaderSrc)
{
  GLuint shader;
  GLint compiled;

  shader = glCreateShader(type);
  if (shader == 0)
    return 0;

  glShaderSource(shader, 1, &shaderSrc, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1)
    {
      char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      printf("Error compiling shader:\n%s\n", infoLog);
      free(infoLog);
    }
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

int Init()
{
  char vShaderStr[] = "attribute vec4 vPosition;                \n"
                      "uniform mat4 uMVPMatrix; \n"
                      "uniform float zoom;	\n"
                      "void main()                              \n"
                      "{                                        \n"
                      "   gl_Position = uMVPMatrix * vPosition;              \n"
                      "   gl_Position.x = gl_Position.x * zoom; \n"
                      "   gl_Position.y = gl_Position.y * zoom; \n"
                      "}                                        \n";

  char fShaderStr[] = "precision mediump float;\n"
                      "uniform vec4 vColor;"
                      "void main()                                  \n"
                      "{                                            \n"
                      "  gl_FragColor = vColor;        \n"
                      "}                                            \n";

  GLuint vertexShader;
  GLuint fragmentShader;
  GLint linked;

  vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
  fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

  programObject = glCreateProgram();
  if (programObject == 0)
    return 0;

  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);
  glBindAttribLocation(programObject, 0, "vPosition");
  glLinkProgram(programObject);
  uniformColor = glGetUniformLocation(programObject, "vColor");
  uniformZoom = glGetUniformLocation(programObject, "zoom");
  uniformMVP = glGetUniformLocation(programObject, "uMVPMatrix");
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    GLint infoLen = 0;
    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen > 1)
    {
      char *infoLog = static_cast<char *>(malloc(sizeof(char) * infoLen));
      glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
      printf("Error linking program:\n%s\n", infoLog);
      free(infoLog);
    }
    glDeleteProgram(programObject);
    return GL_FALSE;
  }
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  fpsTimer.start();

  return GL_TRUE;
}

///
// Draw using the shader pair created in Init()
//
void Draw()
{
  GLfloat vVertices[] = {0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};
  // No clientside arrays, so do this in a webgl-friendly manner
  GLuint vertexPosObject;
  glGenBuffers(1, &vertexPosObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
  glBufferData(GL_ARRAY_BUFFER, 9 * 4, vVertices, GL_STATIC_DRAW);

  glViewport(0, 0, width, height);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(programObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexPosObject);
  glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
  glEnableVertexAttribArray(0);

  int w, h, fs;
#ifdef __EMSCRIPTEN__
  emscripten_get_canvas_size(&w, &h, &fs); // width, height, isFullscreen
#else
  w = width;
  h = height;
#endif
  float xs = (float)h / w;
  float ys = 1.0f;
  float mat[] = {xs, 0, 0, 0, 0, ys, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
  glUniformMatrix4fv(uniformMVP, 1, 0, mat);

  glUniform1f(uniformZoom, zoom);
  float color[] = {0.2f, 0.1f, 0.51f, 1.0f};
  glUniform4fv(uniformColor, 1, color);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void mainLoop()
{
  // Calculate and correct fps
  float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
  if (avgFPS > 2000000)
  {
    avgFPS = 0;
  }
#ifdef __EMSCRIPTEN__
  if (quit)
    emscripten_cancel_main_loop();
#endif
  while (SDL_PollEvent(&e) != 0)
  {

    // Update screen
    SDL_GL_SwapWindow(window);
    if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
    {
      quit = true;
    }
    else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
    {
      // Get mouse position
      int x, y;
      SDL_GetMouseState(&x, &y);
      std::cout << "x = " << x << " y = " << y << " type = " << e.type << std::endl;
    }

    else if (e.type == SDL_KEYDOWN)
    {
      // Select surfaces based on key press
      switch (e.key.keysym.sym)
      {
      case SDLK_UP:
        zoom += 1.0 / 100.0;
        break;
      case SDLK_DOWN:
        zoom -= 1.0 / 100.0;
        break;
      default:
        break;
      }
    }
  }
  // Render
  Draw();
  std::cout << " avgFPS = " << avgFPS << std::endl;
  ++countedFrames;
}

int main(int argc, char **argv)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
  {
    printf("Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
  {
    printf("Warning: Linear texture filtering not enabled!");
  }

#ifdef __EMSCRIPTEN__
  emscripten_set_canvas_size(width, height); // using SDL_SetVideoMode
#endif

  window = SDL_CreateWindow("sdl_gl_read", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                            SDL_WINDOW_OPENGL);

  if (!window)
  {
    printf("Unable to create window: %s\n", SDL_GetError());
    return 1;
  }

#ifdef __EMSCRIPTEN__
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

  context = SDL_GL_CreateContext(window);

#ifndef __EMSCRIPTEN__
  // Initialize GLEW
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if (glewError != GLEW_OK)
  {
    printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
  }
#endif

  // Use Vsync
  if (SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
  }

  Init();

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(mainLoop, 0, 1);
#else
  while (!quit)
  {
    mainLoop();
  }
#endif

  SDL_DestroyWindow(window);
  window = NULL;

  // Quit SDL subsystems
  SDL_Quit();
  return 0;
}