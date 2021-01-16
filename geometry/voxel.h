#include <functional>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Geometry {
  class Voxel {
  public:
    Voxel(int id_, glm::vec3 point_, float sd) :
      id(id_),
      point(point_),
      signedDistance(sd)
    {}
    int id;
    glm::vec3 point;
    float signedDistance;
  };

  //  typedef std::reference_wrapper<Voxel> VoxelRef;
  //  typedef std::vector<VoxelRef> VoxelRow;
  typedef std::vector<Voxel*> VoxelRow;
  typedef std::vector<VoxelRow> VoxelRowCol;
  typedef std::vector<VoxelRowCol> VoxelGrid;
}
