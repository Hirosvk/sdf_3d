#include "signed_distance_function.h"

namespace Geometry {
  class Sphere : public SignedDistanceFunction {
    glm::vec3 centerPt;
    float radius;

  public:
    Sphere(glm::vec3 centerPt_, float radius_) :
      centerPt(centerPt_), radius(radius_)
    {}

    float operator() (const glm::vec3 &pt){
      return glm::length(centerPt - pt) - radius;
    }
  };
}
