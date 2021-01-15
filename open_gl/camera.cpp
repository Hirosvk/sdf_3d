#include "camera.h"

using namespace OpenGL;

float toRad(float deg) {
  return deg * M_PI / 180.0;
}

Camera::Camera(unsigned int screenWidth_, unsigned int screenHeight_, float fovDeg) :
  screenWidth(screenWidth_),
  screenHeight(screenHeight_)
{
  up = glm::vec3(0.0f, 0.0f, 1.0f);
  fov = toRad(fovDeg);
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

void Camera::rotate(float angleDeg) {
  glm::mat4 input(1.0f);
  input[0][3] = -target[0];
  input[1][3] = -target[1];
  input[2][3] = -target[2];

  glm::mat4 rotation = glm::rotate(input, toRad(angleDeg), up);

  glm::vec4 eye_(eye[0], eye[1], eye[2], 1.0f);
  eye_ = rotation * eye_;

  eye = glm::vec3(eye_[0], eye_[1], eye_[2]);
}

void Camera::rotateV(float angleDeg) {
  glm::mat4 input(1.0f);
  input[0][3] = -target[0];
  input[1][3] = -target[1];
  input[2][3] = -target[2];

  glm::vec3 cross = glm::cross(up, (target - eye));
  glm::mat4 rotation = glm::rotate(input, toRad(angleDeg), cross);

  glm::vec4 eye_(eye[0], eye[1], eye[2], 1.0f);
  eye_ = rotation * eye_;
  eye = glm::vec3(eye_[0], eye_[1], eye_[2]);

  glm::vec4 up_(up[0], up[1], up[2], 1.0f);
  up_ = rotation * up_;
  up = glm::vec3(up_[0], up_[1], up_[2]);
}
