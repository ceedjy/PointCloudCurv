#include "CorrectedNormalCurrentFormulaEigen.h"
#include "polyscope/polyscope.h"
#include "filter.hpp"
using namespace std;

typedef Eigen::Vector3d  RealPoint;

std::vector< RealPoint >
filterNaive( const std::vector< RealPoint >& pts, int delta, float epsi)
{
  std::vector< RealPoint > output;
  int size = pts.size();
  int i = 0;
  for ( auto& p : pts ) {
    glm::vec3 sum = {0.0, 0.0, 0.0};
    int nbPtsInvolved = 0;
    // we take delta points before and delta points after
    for(int j =1; j <= delta; j++){
      if (i+j < size) { // is in tab
        sum = {sum[0] + pts[i+j][0], sum[1] + pts[i+j][1], sum[2] + pts[i+j][2]};
        nbPtsInvolved++;
      } 
      if (i-j >= 0){ // is in tab
        nbPtsInvolved++;
        sum = {sum[0] + pts[i-j][0], sum[1] + pts[i-j][1], sum[2] + pts[i-j][2]};
      }
    }
    // middle point
    sum = {sum[0]/nbPtsInvolved, sum[1]/nbPtsInvolved, sum[2]/nbPtsInvolved};
    // distance between this middle point and p
    float dist = sqrt((sum[0]-p[0])*(sum[0]-p[0]) + (sum[1]-p[1])*(sum[1]-p[1]) + (sum[2]-p[2])*(sum[2]-p[2]));
    if (dist <= epsi){
      output.push_back(p);
    }
    i++;
  }
  return output;
}

float distance(RealPoint pt1, RealPoint pt2){
  return sqrt((pt2[0]-pt1[0])*(pt2[0]-pt1[0]) + (pt2[1]-pt1[1])*(pt2[1]-pt1[1]) + (pt2[2]-pt1[2])*(pt2[2]-pt1[2]));
}

std::vector <line>
cutInLine( const std::vector< RealPoint >& pts, float delta)
{
  std::vector< line > output;
  std::vector< RealPoint > oneLine;
  int size = pts.size();
  int i = 0;
  // mean distance between points
  float mean = 0.0f;
  float sum = 0.0f;
  int nbDistInvolved = 0;
  for ( auto& p : pts ) {
    if (i+1 < size){ // not at the end of tab points
      float dist = distance(pts[i], pts[i+1]);
      if (mean*delta < dist && mean != 0.0f){ // distance has blow up = new line
        // stock in struct
        line l;
        l.points = oneLine;
        l.size = nbDistInvolved+1;
        output.push_back(l);
        // reset variables
        sum = 0.0f;
        nbDistInvolved = 0;
        mean = 0.0f;
        oneLine = {};
      } else { // distance approximately the same = same line
        oneLine.push_back(p);
        sum += dist;
        nbDistInvolved++;
        mean = sum / nbDistInvolved;
      }
    } else { // at the end of tab points
      oneLine.push_back(p);
      // stock in struct
      line l;
      l.points = oneLine;
      l.size = nbDistInvolved+1;
      output.push_back(l);
    }
    i++;
  }
  std::cout << "Number of lines found : " << output.size() << endl;
  return output;
}

std::vector<cluster>
kmeanCluster(const std::vector< RealPoint >& pts, int k, int beta){
  std::vector< cluster > output;
  std::vector< RealPoint > oneCluster;
  int size = pts.size();

  // Choose k points in our data set to initialize clusters
  int startIndex = (int)(size / (k+1)); 
  for (int j = 1; j < k+1; j++){
    cluster c;
    c.barycenter = {pts[startIndex*j][0], pts[startIndex*j][1], pts[startIndex*j][2]};
    output.push_back(c);
  }

  // Apply kmean beta times
  for (int b = 0; b < beta; b++){
    // initialize all point set in cluster to nothing
    for (int j = 0; j < k; j++){
      output[j].points = {};
    }
    for ( auto& p : pts ) {
      // find to which cluster this point belong
      float minDist = std::numeric_limits<float>::max();
      int index = 0;
      for (int j = 0; j < k; j++){
        float dist = distance(p, output[j].barycenter);
        if (dist < minDist){
          minDist = dist;
          index = j;
        }
      }
      // change the output 
      output[index].points.push_back(p);
    }
    // recalculate barycentre
    for(int j = 0; j < k; j++){
      glm::vec3 sum = {0.0, 0.0, 0.0};
      int nbPtsInvolved = output[j].points.size();
      for (auto& q : output[j].points){
        sum = {sum[0] + q[0], sum[1] + q[1], sum[2] + q[2]};
      }
      output[j].barycenter = {sum[0]/nbPtsInvolved, sum[1]/nbPtsInvolved, sum[2]/nbPtsInvolved};
    }
  }

  // Calculate mean distance between each point and the barycenter 
  for (int j = 0; j < k; j++){
    float sum = 0.0f;
    for (auto& q : output[j].points){
      sum += distance(q, output[j].barycenter);
    }
    output[j].size = output[j].points.size();
    output[j].meanDist = sum/output[j].size;
  }

  return output;
}

std::vector<cluster> 
kmeanFilter(const std::vector< RealPoint >& pts, int k, int beta, float delta){
  std::vector<cluster> output = kmeanCluster(pts, k, beta);
  // get rid of outliers
  for(int j = 0; j < k; j++){
    std::vector<RealPoint> newPoints = {};
    for (auto& q : output[j].points){
      float dist = distance(q, output[j].barycenter);
      if(dist <= output[j].meanDist*delta){
        newPoints.push_back(q);
      }
    }
    output[j].points = newPoints;
  }
  return output;
}

// todo : parallélister là ou on faire des trucs séparément par cluster
