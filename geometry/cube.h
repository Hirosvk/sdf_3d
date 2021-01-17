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
      // =>
      // {
      //    { {0, 4}, {1,5}, {3, 7} },
      //    { {0, 4}, {1,5}, {3, 7} },
      // }
    {0, 1, 5, 4},
    {4, 5, 6, 7},
    {6, 2, 3, 7},
    {0, 4, 7, 3},
    {1, 2, 6, 5}
  };

  //  const Id planeConfigs[6][2][2] = {
    //  {
      //  { {0,4}, {1,5}, {3,7} },
      //  { {1,5}, {2,6}, {3,7} },
    //  }
  //  };

  const Id TriCornerConfigs[24][3] = {
    {0, 1, 3},
      // => { {0, 4}, {1, 5}, {3, 7} }
      // => { {1, 5}, {1, 2}, {3, 7} }
  };

  class Cube {
  public: // debug
    Voxels voxels;
    Vertices &vertices;
    std::vector<Ids> groupedVoxelIds;


    void crawl(Id id, Ids &targetGroup);

    void groupVoxels(Ids&);

    void generatePolygonsFromGroup(Ids&);

    void generatePolygon1Corner(Ids&, bool reverse);
    void generatePolygon2Corner(Ids&, bool reverse);
    void generatePolygon3Corner(Ids&, bool reverse);

    void generatePolygon4CornerPlane(Ids&);
    void generatePolygon4CornerCone(Ids&, Id);
    void generatePolygon4CornerOther(Ids&);

    int findPlaneId(Ids&group);
    bool isPlaneConfig(Ids&);

    Vertex* getVertex(Id voxId); // when 2 of the adjacent voxels are negative
    VertexPair getVertices(Id voxId, Id otherVoxId); // when only 1 of the adjacent voxels is negative


    int extractAdjacent(Ids &negSignedVoxIds, Id id, Ids &targetGroup);

    int countFreeVox(Id voxId, Ids &group);

    Id getRightAdjacentVoxId(Id voxId, Id fromVoxId);
    Id getLeftAdjacentVoxId(Id voxId, Id freeVoxId);

    Ids getConnectedVoxIds(Id voxId, Ids &group);
    Ids getFreeVoxIds(Id voxId, Ids &group);
    Ids reverseGroup(Ids&group);

    bool isIsosurface;

  public:
    std::vector<Polygon*> polygons;

    Cube(Voxels &voxels_, Vertices &vertices_);

    void generatePolygons();
  };
}
#endif
