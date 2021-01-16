#ifndef LOAD_VOXEL
#define LOAD_VOXEL

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Geometry {
  class Voxel {
  public:
    Voxel(unsigned int id_, glm::vec3 point_, float sd, bool s) :
      id(id_),
      point(point_),
      signedDistance(sd),
      sign(s)
    {}
    unsigned int id;
    glm::vec3 point;
    float signedDistance;
    bool sign;
  };

  typedef std::vector<Voxel*> VoxelRow;
  typedef std::vector<VoxelRow> VoxelRowCol;
  typedef std::vector<VoxelRowCol> VoxelGrid;

  typedef VoxelRow Voxels;
}
#endif
