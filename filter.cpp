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
  cout << "Number of lines found : " << output.size() << endl;
  return output;
}
