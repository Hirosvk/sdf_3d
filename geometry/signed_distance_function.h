#ifndef LOAD_SDF
#define LOAD_SDF

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Geometry {
  class SignedDistanceFunction {
  public:
    virtual float operator() (const glm::vec3&) = 0;
  };
}
#endif
