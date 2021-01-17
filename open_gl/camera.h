#ifndef LOAD_CAMERA
#define LOAD_CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL {
  const float DEFAULT_FOV = 45.0;
  class Camera {
  public:
    glm::vec3 eye,
              up,
              target;
    unsigned int screenWidth,
                 screenHeight;
    float fov,
          near,
          far,
          screenRatio;

    Camera(unsigned int screenWidth_, unsigned int screenHeight_, float fovDeg = DEFAULT_FOV);

    glm::mat4 view();
    glm::mat4 projection();
    void rotate(float angleDeg);
    void rotateV(float angleDeg);
    void moveHor(float distance);
    void moveVir(float distance);
    void zoom(float distance);

    void set(glm::vec3 eye, glm::vec3 target);
  };
}
#endif
