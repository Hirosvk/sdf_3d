#include "input_handler.h"

using namespace OpenGL;

InputHandler::InputHandler(Camera &camera_) :
  camera(camera_),
  keysToWatch(std::vector<int>(keysToWatch_, keysToWatch_ + 10))
{}

void InputHandler::handle(int key) {
  if (key == GLFW_KEY_RIGHT_BRACKET) camera.rotate(5.0);
  if (key == GLFW_KEY_LEFT_BRACKET) camera.rotate(-5.0);

  if (key == GLFW_KEY_EQUAL) camera.rotateV(5.0);
  if (key == GLFW_KEY_APOSTROPHE) camera.rotateV(-5.0);

  if (key == GLFW_KEY_RIGHT) camera.moveHor(2.0);
  if (key == GLFW_KEY_LEFT) camera.moveHor(-2.0);

  if (key == GLFW_KEY_UP) camera.moveVir(2.0);
  if (key == GLFW_KEY_DOWN) camera.moveVir(-2.0);

  if (key == GLFW_KEY_J) camera.zoom(2.0);
  if (key == GLFW_KEY_K) camera.zoom(-2.0);
}
