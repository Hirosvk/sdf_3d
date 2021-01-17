#include <iostream>

#include "vertices.h"

using namespace Geometry;

unsigned long int Vertices::getKey(unsigned int id1, unsigned int id2) {
  unsigned long int more = std::max(id1, id2);
  unsigned long int less = std::min(id1, id2);

  return (less << 16) | more;
}

void Vertices::addVertex(Voxel &v1, Voxel &v2) {
  unsigned long int id = Vertices::getKey(v1.id, v2.id);
  if (vertices.find(id) != vertices.end()) return;

  glm::vec3 sum = (v1.point + v2.point);
  glm::vec3 midPt = glm::vec3(sum.x / 2.0, sum.y / 2.0, sum.z / 2.0);
  vertices[id] = new Vertex(id, midPt);
}

Vertex* Vertices::findVertex(Voxel &v1, Voxel &v2) {
  unsigned long int id = Vertices::getKey(v1.id, v2.id);
  if (vertices.find(id) == vertices.end()) std::cout << "  no vtx found" << std::endl;
  return vertices.find(id)->second;
}

unsigned int Vertices::size() {
  return vertices.size();
}
