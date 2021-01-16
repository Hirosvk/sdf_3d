#include "signed_distance_function.h"

namespace Geometry {
  class Square : public SignedDistanceFunction {
    glm::vec3 corner;
    float dimension;
    glm::mat4 translation;

  public:
    Square(glm::vec3 corner_, float dimension_) :
      corner(corner_), dimension(dimension_)
    {
      translation = glm::inverse(glm::translate(glm::mat4(1.0), corner));
    }

    float operator() (const glm::vec3 &pt) {
      glm::vec4 pt_ = glm::vec4(pt.x, pt.y, pt.z, 1.0f);
      glm::vec4 ptTr = translation * pt_;

      if (within(ptTr.x) && within(ptTr.y) && within(ptTr.z)) return -1.0;
      return 1.0;
    }

    bool within(float n) {
      return (0.0 <= n && n <= dimension);
    }
  };
}
