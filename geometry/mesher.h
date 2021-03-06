#ifndef LOAD_MESHER
#define LOAD_MESHER

#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "signed_distance_function.h"
#include "voxel.h"
#include "vertices.h"
#include "cube.h"

namespace Geometry {
  class Mesher {
    glm::vec2 xRange, yRange, zRange;
    float voxelUnit;
    SignedDistanceFunction &sdf;

  public: // for debugging
    VoxelGrid voxels;
    Vertices vertices;
    void generateVoxels();
    void generateVertices();
    void generateCubes();
    std::vector<Cube*> cubes;

  public:
    Mesher(
      glm::vec2 xRange_,
      glm::vec2 yRange_,
      glm::vec2 zRange_,
      float voxelUnit_,
      SignedDistanceFunction &sdf_
    );
    ~Mesher();

    void generatePolygons(std::vector<float>&, std::vector<unsigned int>&);
  };
}
#endif
