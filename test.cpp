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
    glm::vec2(-unit, unit * 2),
    glm::vec2(-unit, unit * 2),
    glm::vec2(-unit, unit * 2),
    halfUnit,
    *sdf_ptr
  );

  mesher.generateVoxels();
  mesher.generateVertices();

  //  std::cout << mesher.vertices.size() << std::endl;

  //  for (auto &row: mesher.voxels) {
    //  for (auto &col: row) {
      //  for (auto &voxel: col) {
        //  if (voxel->sign == false) {
          //  std::cout
            //  << voxel->point.x << ", "
            //  << voxel->point.y << ", "
            //  << voxel->point.z << ", "
            //  << std::endl;
        //  }
      //  }
    //  }
  //  }
  //  return 0;

  //  for(auto &vMap: mesher.vertices.vertices) {
    //  std::cout
      //  << vMap.second->point.x << ", "
      //  << vMap.second->point.y << ", "
      //  << vMap.second->point.z << ", "
      //  << std::endl;
  //  }
  //  return 0;

  mesher.generateCubes();
  int c = 0;
  for(auto &cube: mesher.cubes) {
    if (cube->isIsosurface && cube->polygons.size() > 0) {
      for(auto &polygon: cube->polygons) {
        std::cout
          << polygon->vertices[0]->point.x << ", "
          << polygon->vertices[0]->point.y << ", "
          << polygon->vertices[0]->point.z << ", "

          << polygon->vertices[1]->point.x << ", "
          << polygon->vertices[1]->point.y << ", "
          << polygon->vertices[1]->point.z << ", "

          << polygon->vertices[2]->point.x << ", "
          << polygon->vertices[2]->point.y << ", "
          << polygon->vertices[2]->point.z << ", "
          << std::endl;
      }
    }
  }

  return 0;
}
