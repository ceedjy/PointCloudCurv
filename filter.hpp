#include <iostream>
#include <string>

typedef Eigen::Vector3d  RealPoint;

struct line {
  std::vector<RealPoint> points;
  int size;
};
struct cluster {
  std::vector<RealPoint> points;
  RealPoint barycenter;
  int size;
  float meanDist;
};
// Naively filter point | pts : all data points | delta : number of point taken into account before and after the viewed point | espi : tolerated distance 
std::vector< RealPoint > filterNaive( const std::vector< RealPoint >& pts, int delta, float epsi);
// Calculate distance between 2 points | pt1 : first point | pt2 : second point
float distance(RealPoint pt1, RealPoint pt2);
// Split data in LiDAR lines | pts : all data points | delta : tolerance level
std::vector <line> cutInLine( const std::vector< RealPoint >& pts, float delta);
// Split data using kmean to create k clusters | k : number of cluster | beta : number of times we do kmean to create clusters
std::vector<cluster> kmeanCluster(const std::vector< RealPoint >& pts, int k, int beta);
// Use kmean to filter outliers from point set | k : number of cluster | beta : nomber of times we do kmean to create clusters | delta : tolerated distance 
std::vector<cluster> kmeanFilter(const std::vector< RealPoint >& pts, int k, int beta, float delta);
