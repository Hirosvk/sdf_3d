#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h> // GLEW vs GLAD
#include <GLFW/glfw3.h>

#include "input_handler.h"

namespace OpenGL {
  class Window
  {
  public:
    int screenWidth, screenHeight;
    GLFWwindow *window;
    InputHandler &inputHandler;

    Window(int screenWidth_, int screenHeight_, InputHandler &inputHandler_);
    void clear();
    void processInput();

    void swapBuffers();
    void pollAndProcessEvents();
    void terminate();
    bool shouldClose();

  private:
    void initGLFW();
    void initGL();
  };
}
