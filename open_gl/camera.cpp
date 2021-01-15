#include "camera.h"

using namespace OpenGL;

Camera::Camera(unsigned int screenWidth_, unsigned int screenHeight_, float fovDeg) :
  screenWidth(screenWidth_),
  screenHeight(screenHeight_)
{
  up = glm::vec3(0.0f, 0.0f, 1.0f);
  fov = fovDeg * M_PI / 180.0;
  screenRatio = (float) screenWidth / (float) screenHeight;
  near = 0.1f;
  far = 1000.0f;
}

void Camera::set(glm::vec3 eye_, glm::vec3 target_) {
  eye = eye_;
  target = target_;
}

glm::mat4 Camera::view() {
  return glm::lookAt(eye, target, up);
}

glm::mat4 Camera::projection() {
  return glm::perspective(fov, screenRatio, near, far);
}
