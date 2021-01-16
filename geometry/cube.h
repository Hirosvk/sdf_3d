#ifndef LOAD_CUBE
#define LOAD_CUBE

#include <vector>
#include <set>

#include "voxel.h"
#include "vertices.h"
#include "polygon.h"

namespace Geometry {
  typedef std::set<unsigned int> Ids;
  typedef std::pair<Vertex*, Vertex*> VertexPair;

  const unsigned int adjacencyLookup[8][3] = {
  /*0*/ {1, 3, 4},
  /*1*/ {2, 0, 5},
  /*2*/ {3, 1, 6},
  /*3*/ {0, 2, 7},

  /*4*/ {0, 7, 5},
  /*5*/ {1, 4, 6},
  /*6*/ {2, 5, 7},
  /*7*/ {3, 6, 4}
  };

  class Cube {
    Voxels voxels;
    Vertices &vertices;
    std::vector<Ids> groupedVoxelIds;

    void groupVoxels(Ids&);

    void generatePolygonsFromGroup(Ids&);
    void generatePolygon1Corner(Ids&);
    void generatePolygon1Side(Ids&);

    Vertex* getVertex(unsigned int voxId); // when 2 of the adjacent voxels are negative
    VertexPair getVertices(unsigned int voxId, unsigned int otherVoxId); // when only 1 of the adjacent voxels is negative

  public: // debug
    bool isIsosurface;

  public:
    std::vector<Polygon*> polygons;

    Cube(Voxels &voxels_, Vertices &vertices_);

    void generatePolygons();
  };
}

/*
 * 1 corner
 * 2 side => 1 side
 * 3 corners
 * 4 corner - plane
 * 4 corner - not plane
 */
#endif
