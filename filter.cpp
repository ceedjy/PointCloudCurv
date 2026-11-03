#include "CorrectedNormalCurrentFormulaEigen.h"
#include "polyscope/polyscope.h"

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
    for(int j =1; j <= delta; j++){
      if (i+j < size) {
        sum = {sum[0] + pts[i+j][0], sum[1] + pts[i+j][1], sum[2] + pts[i+j][2]};
        nbPtsInvolved++;
      } 
      if (i-j >= 0){
        nbPtsInvolved++;
        sum = {sum[0] + pts[i-j][0], sum[1] + pts[i-j][1], sum[2] + pts[i-j][2]};
      }
    }
    // point moyen
    sum = {sum[0]/nbPtsInvolved, sum[1]/nbPtsInvolved, sum[2]/nbPtsInvolved};
    // distance entre ce point moyen et p
    float dist = sqrt((sum[0]-p[0])*(sum[0]-p[0]) + (sum[1]-p[1])*(sum[1]-p[1]) + (sum[2]-p[2])*(sum[2]-p[2]));
    if (dist <= epsi){
      output.push_back(p);
    }
    i++;
  }
  return output;
}