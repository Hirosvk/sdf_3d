#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "geometry/signed_distance_function.h"
#include "geometry/sphere.h"
#include "geometry/mesher.h"


using namespace Geometry;

//  float unit = 10.0;
//  glm::vec3 points = {
  //  gml::vec3(0.0, 0.0, 0.0),
  //  gml::vec3(unit, 0.0, 0.0),
  //  gml::vec3(unit, unit, 0.0),
  //  gml::vec3(0.0, unit, 0.0),

  //  gml::vec3(0.0, 0.0, unit),
  //  gml::vec3(unit, 0.0, unit),
  //  gml::vec3(unit, unit, unit),
  //  gml::vec3(0.0, unit, unit),
//  };

class SDF : public SignedDistanceFunction {
public:
  float unit;
  SDF(float unit_) : unit(unit_) {}
  float operator() (const glm::vec3 &pt) {
    if (within(pt.x) && within(pt.y) && within(pt.z)) return -1.0;
    return 1.0;
  }

  bool within(float n) {
    return (0.0 <= n && n <= unit);
  }
};

int main () {
  float unit = 10.0;
  float halfUnit = unit / 2.0;
  SDF sdf = SDF(unit);
  SignedDistanceFunction *sdf_ptr = &sdf;
  Mesher mesher(
    glm::vec2(-halfUnit, unit + halfUnit),
    glm::vec2(-halfUnit, unit + halfUnit),
    glm::vec2(-halfUnit, unit + halfUnit),
    halfUnit,
    *sdf_ptr
  );

  mesher.generateVoxels();
  mesher.generateVertices();

  std::cout << mesher.vertices.size() << std::endl;

  for (auto &row: mesher.voxels) {
    for (auto &col: row) {
      for (auto &voxel: col) {
        if (voxel->sign == false) {
          std::cout
            << voxel->point.x << ", "
            << voxel->point.y << ", "
            << voxel->point.z << ", "
            << std::endl;
        }
      }
    }
  }

  return 0;
}
