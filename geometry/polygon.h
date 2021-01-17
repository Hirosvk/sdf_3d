#ifndef LOAD_POLYGON
#define LOAD_POLYGON

#include <vector>
#include <algorithm>

#include "vertices.h"

namespace Geometry {
  typedef std::vector<Vertex*> VertexTrio;
  class Polygon {
  public:
    VertexTrio vertices;

    Polygon(Vertex* v1, Vertex* v2, Vertex* v3) {
      vertices.push_back(v1);
      vertices.push_back(v2);
      vertices.push_back(v3);
    }

    void reverse() {
      std::reverse(vertices.begin(), vertices.end());
    }
  };
}
#endif
