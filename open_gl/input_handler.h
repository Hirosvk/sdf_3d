#ifndef LOAD_INPUT_HANDLER
#define LOAD_INPUT_HANDLER

#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h> // GLEW vs GLAD
#include <GLFW/glfw3.h>

#include "camera.h"

namespace OpenGL {
  const int keysToWatch_[] = {
    GLFW_KEY_RIGHT_BRACKET,
    GLFW_KEY_LEFT_BRACKET,
    GLFW_KEY_EQUAL,
    GLFW_KEY_APOSTROPHE,
  };

  class InputHandler
  {
    Camera &camera;

  public:
    const std::vector<int> keysToWatch;

    InputHandler(Camera &camera_);
    void handle(int key);
  };
}
#endif
