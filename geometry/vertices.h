#ifndef LOAD_VERTICES
#define LOAD_VERTICES

#include <map>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "voxel.h"

namespace Geometry {
  struct Vertex {
    unsigned long int id;
    glm::vec3 point;
    Vertex(unsigned long int i, glm::vec3 p) : id(i), point(p) {}
  };

  class Vertices {
  public: // debug
    static unsigned long int getKey(unsigned int id1, unsigned int id2);

    std::map<unsigned long int, Vertex*> vertices;

  public:
    ~Vertices() {
      for(auto it = vertices.begin(); it != vertices.end(); it++) {
        delete it->second;
      }
    }

    unsigned int size();
    void addVertex(Voxel &v1, Voxel &v2);
    Vertex *findVertex(Voxel &v1, Voxel &v2);
  };
}
#endif
