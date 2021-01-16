#ifndef LOAD_CUBE
#define LOAD_CUBE

#include <vector>
#include <set>

#include "voxel.h"
#include "vertices.h"
#include "polygon.h"

namespace Geometry {
  typedef unsigned int Id;
  typedef std::set<Id> Ids;
  typedef std::pair<Vertex*, Vertex*> VertexPair;

  const Id adjacencyLookup[8][3] = {
  /*0*/ {1, 3, 4},
  /*1*/ {2, 0, 5},
  /*2*/ {3, 1, 6},
  /*3*/ {0, 2, 7},

  /*4*/ {0, 7, 5},
  /*5*/ {1, 4, 6},
  /*6*/ {2, 5, 7},
  /*7*/ {3, 6, 4}
  };

  const Id planeConfigs[6][4] = {
    {0, 3, 2, 1},
    {0, 1, 5, 4},
    {4, 5, 6, 7},
    {6, 2, 3, 7},
    {0, 4, 7, 3},
    {1, 2, 6, 5}
  };

  class Cube {
  public: // debug
    Voxels voxels;
    Vertices &vertices;
    std::vector<Ids> groupedVoxelIds;

    void groupVoxels(Ids&);

    void generatePolygonsFromGroup(Ids&);
    void generatePolygon1Corner(Ids&);
    void generatePolygon1Side(Ids&);

    void generatePolygon3Corner(Ids&);
    void generatePolygon4CornerPlane(Ids&);

    int findPlaneId(Ids&group);
    bool isPlaneConfig(Ids&);

    Vertex* getVertex(Id voxId); // when 2 of the adjacent voxels are negative
    VertexPair getVertices(Id voxId, Id otherVoxId); // when only 1 of the adjacent voxels is negative


    int extractAdjacent(Ids &negSignedVoxIds, Id id, Ids &targetGroup);

    int countFreeVox(Id voxId, Ids &group);

    Id getFreeAdajacentVoxId(Id voxId, Id otherVoxId1, Id otherVoxId2);
    Id getRightAdjacentVoxId(Id voxId, Id freeVoxId);
    Id getLeftAdjacentVoxId(Id voxId, Id freeVoxId);

    Ids getConnectedVoxIds(Id voxId, Ids &group);

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
