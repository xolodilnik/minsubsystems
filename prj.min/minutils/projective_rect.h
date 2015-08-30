/*

Copyright (c) 2011, Smart Engines Limited. All rights reserved. 

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of copyright holders.

*/


#pragma once
#ifndef PROJECTIVE_RECT_H_INCLUDED
#define PROJECTIVE_RECT_H_INCLUDED

#include <minutils/minpoint.h>
#include <minutils/minquad.h>
#include <minutils/projective.h>

#include <vector>

namespace se {

// different focal lenths
// use: image width * value
#define FOCAL_LENGTH_DEFAULT 0.625
#define FOCAL_LENGTH_IPH4 0.838
#define FOCAL_LENGTH_IPH4S 0.932



template< class CT >
inline bool delta_point(
  const pnt2< CT >& point, // given point 
  pnt2 < CT >* pntr,       // delta-neighborhood of given point pointer
  const CT& delta          // parameter delta of the delta-neighborhood
  )
{
  pntr->x = point.x - delta;
  pntr->y = point.y - delta;

  (pntr+1)->x = point.x - delta;
  (pntr+1)->y = point.y + delta;

  (pntr+2)->x = point.x + delta;
  (pntr+2)->y = point.y + delta;

  (pntr+3)->x = point.x + delta;
  (pntr+3)->y = point.y - delta;

  return true;
};


template< class CT >
inline bool is_it_true_that_image_is_rectangle(
  const quad2< CT >& prj, // projection of rectangle
  const CT& flen,               // focal length to compute
  const CT& rat,                // ratio of sides
  const CT& accuracy            // maximal apropriate error
  )
{
  pnt2< CT > PA[4], PB[4], PC[4], PD[4];
  CT  ratio[4][4][4][4];
  CT  cosan[4][4][4][4];
  CT  maxratio = -1;
  CT  minratio = 10000000.;
  CT  maxcosan = -2.;
  CT  mincosan = 2.;

  delta_point(prj.a, PA, accuracy);
  delta_point(prj.b, PB, accuracy);
  delta_point(prj.c, PC, accuracy);
  delta_point(prj.d, PD, accuracy); 

  int fl = 0;
  for(int i = 0; i < 4; ++i) {
    for(int j = 0; j < 4; ++j) {
      for(int k = 0; k < 4; ++k) {
        for(int l = 0; l < 4; ++l) {
          quad2< CT > quadr(PA[i], PB[j], PC[k], PD[l]);
          quad3< CT > quad_3d;
          if(!restore_parallelogram_by_projection(quadr, quad_3d, flen)) 
            fl = 1;
          else {
            ratio[i][j][k][l] = length(quad_3d.a-quad_3d.b) / length(quad_3d.a-quad_3d.d);
            cosan[i][j][k][l] = cosangle((quad_3d.a-quad_3d.b), (quad_3d.a-quad_3d.d));
     
            if(cosan[i][j][k][l] > maxcosan)
              maxcosan = cosan[i][j][k][l];
            if(cosan[i][j][k][l] < mincosan)
              mincosan = cosan[i][j][k][l];
            if(ratio[i][j][k][l] > maxratio)
              maxratio = ratio[i][j][k][l];
            if(ratio[i][j][k][l] < minratio)
              minratio = ratio[i][j][k][l];
          }
        }
      }
    }
  }

  if(mincosan <= 0 && maxcosan >= 0 && minratio <= rat && maxratio >= rat)
    fl = 1;

  return (fl == 1);
}


template< class CT >
inline double err_dist(         // distance in error-space
  const quad2< CT >& prj,       // projection of rectangle
  const CT& flen,               // focal length to compute
  const CT& rat                 // ratio of sides
  )
{
  double accuracy;              // apropriate error
  double acc1 = 0.0;
  double acc2 = length(prj.a-prj.b)+1.0;

  while(acc2 - acc1 > 0.0001) {
    accuracy = (acc2 + acc1) / 2;
    if(is_it_true_that_image_is_rectangle(prj, flen, rat, accuracy))
      acc2 = accuracy;  
    else
      acc1 = accuracy;   
  }
  
  return accuracy;
}



template< class CT >
inline bool normal_check(
  const quad2< CT >& prj,       // projection of rectangle
  const CT& flen,               // focal length to compute
  const CT& min_cos             // minimal apropriate value of the normal angle cosinus
  )
{
  quad3< CT > quad_3d;
  pnt3< CT > v1, v2, norm;
  double cos;

  restore_parallelogram_by_projection(prj, quad_3d, flen);
 
  v1 = quad_3d.a - quad_3d.c;
  v2 = quad_3d.b - quad_3d.d;
  norm = cross(v1, v2);

  cos = (1.0 * norm.z) / length(norm);
  cos = (cos < 0) ? -1.0 * cos : cos;

  return (cos > (double) min_cos);
}


template< class CT >
inline bool numerical_Jacobi(
  const quad2< CT >& prj,       // projection of rectangle
  const CT& flen,               // focal length to compute
  std::vector< std::pair<double,double> >& J  // Jacobi matrix
  )
// (x1,y1), (x2,y2), (x3,y3), (x4,y4) --> phi1(x1,...,y4), phi2(x1,...,y4) = ( ratio^2, cos^2(alpha) )  
//
//     | \frac{\partial phi1}{\partial x1} ... \frac{\partial phi1}{\partial y4} |
// J = |                                                                         |
//     | \frac{\partial phi2}{\partial x1} ... \frac{\partial phi2}{\partial y4} |
{
  bool res = true;
  double eps = 0.001;
  J.resize(8);

  for(int i = 0; i < 4; ++i) {
    quad3< double > paral1, paral2;
    quad2< double > quadr1 = prj, quadr2 = prj;
    quadr1[i].x += eps/2;
    quadr2[i].x -= eps/2;
    if(! restore_parallelogram_by_projection(quadr1, paral1, flen))
      res = false;
    if(! restore_parallelogram_by_projection(quadr2, paral2, flen))
      res = false;
    double r1 = length(paral1.a-paral1.b) / length(paral1.a-paral1.d);
    double r2 = length(paral2.a-paral2.b) / length(paral2.a-paral2.d);
    double c1 = cosangle((paral1.a-paral1.b), (paral1.a-paral1.d));
    double c2 = cosangle((paral2.a-paral2.b), (paral2.a-paral2.d));
    J[i].first  = (r1*r1 - r2*r2) / eps;
    J[i].second = (c1*c1 - c2*c2) / eps;
  }

  for(int i = 0; i < 4; ++i) {
    quad3< double > paral1, paral2;
    quad2< double > quadr1 = prj, quadr2 = prj;
    quadr1[i].y += eps/2;
    quadr2[i].y -= eps/2;
    if(! restore_parallelogram_by_projection(quadr1, paral1, flen))
      res = false;
    if(! restore_parallelogram_by_projection(quadr2, paral2, flen))
      res = false;
    double r1 = length(paral1.a-paral1.b) / length(paral1.a-paral1.d);
    double r2 = length(paral2.a-paral2.b) / length(paral2.a-paral2.d);
    double c1 = cosangle((paral1.a-paral1.b), (paral1.a-paral1.d));
    double c2 = cosangle((paral2.a-paral2.b), (paral2.a-paral2.d));
    J[i+4].first  = (r1*r1 - r2*r2) / eps;
    J[i+4].second = (c1*c1 - c2*c2) / eps;
  }

  return res;
}



template< class CT >
inline bool Jacobi(
  const quad2< CT >& prj,       // projection of rectangle
  const CT& flen,               // focal length to compute
  std::vector< std::pair<double,double> >& J  // Jacobi matrix
  )
// (x1,y1), (x2,y2), (x3,y3), (x4,y4) --> phi1(x1,...,y4), phi2(x1,...,y4) = ( ratio^2, cos^2(alpha) )  
//
//     | \frac{\partial phi1}{\partial x1} ... \frac{\partial phi1}{\partial y4} |
// J = |                                                                         |
//     | \frac{\partial phi2}{\partial x1} ... \frac{\partial phi2}{\partial y4} |
{
  bool res = true;
  double ff = flen * flen;
  J.resize(8);

  double x1 = prj.a.x;
  double y1 = prj.a.y;
  double x2 = prj.b.x;
  double y2 = prj.b.y;
  double x3 = prj.c.x;
  double y3 = prj.c.y;
  double x4 = prj.d.x;
  double y4 = prj.d.y;

  std::vector<double> A(9, 0.0), B(9, 0.0), C(9, 0.0); 
  // A[8] = A, A[0] = \frac{\partial A}\{\partial x1}, ..., A[7] = \frac{\partial A}{\partial y4} 
  // B[8] = B, B[0] = \frac{\partial B}\{\partial x1}, ..., B[7] = \frac{\partial B}{\partial y4} 
  // C[8] = C, C[0] = \frac{\partial C}\{\partial x1}, ..., C[7] = \frac{\partial C}{\partial y4} 

  std::vector<double> k1(9, 0.0), k2(9, 0.0), k3(9, 0.0), k4(9, 0.0);
  k1[8] = x4*y3-y4*x3 + x3*y2-y3*x2 + x2*y4-y2*x4;
  k2[8] = x4*y3-y4*x3 + x3*y1-y3*x1 + x1*y4-y1*x4;
  k3[8] = x4*y2-y4*x2 + x2*y1-y2*x1 + x1*y4-y1*x4;
  k4[8] = x3*y2-y3*x2 + x2*y1-y2*x1 + x1*y3-y1*x3;

  k1[0] = 0;
  k1[1] = y4 - y3;
  k1[2] = y2 - y4;
  k1[3] = y3 - y2;
  k1[4] = 0;
  k1[5] = x3 - x4;
  k1[6] = x4 - x2;
  k1[7] = x2 - x3;
  
  k2[0] = y4 - y3;
  k2[1] = 0;
  k2[2] = y1 - y4;
  k2[3] = y3 - y1;
  k2[4] = x3 - x4;
  k2[5] = 0;
  k2[6] = x4 - x1;
  k2[7] = x1 - x3;
  
  k3[0] = y4 - y2;
  k3[1] = y1 - y4;
  k3[2] = 0;
  k3[3] = y2 - y1;
  k3[4] = x2 - x4;
  k3[5] = x4 - x1;
  k3[6] = 0;
  k3[7] = x1 - x2;
  
  k4[0] = y3 - y2;
  k4[1] = y1 - y3;
  k4[2] = y2 - y1;
  k4[3] = 0;
  k4[4] = x2 - x3;
  k4[5] = x3 - x1;
  k4[6] = x1 - x2;
  k4[7] = 0;


  A[8] = (k1[8]*x1-k2[8]*x2)*(k1[8]*x1-k2[8]*x2) + (k1[8]*y1-k2[8]*y2)*(k1[8]*y1-k2[8]*y2) + (k1[8]-k2[8])*(k1[8]-k2[8])*ff;
  B[8] = (k1[8]*x1-k4[8]*x4)*(k1[8]*x1-k4[8]*x4) + (k1[8]*y1-k4[8]*y4)*(k1[8]*y1-k4[8]*y4) + (k1[8]-k4[8])*(k1[8]-k4[8])*ff;
  C[8] = (k1[8]*x1-k2[8]*x2)*(k1[8]*x1-k4[8]*x4) + (k1[8]*y1-k2[8]*y2)*(k1[8]*y1-k4[8]*y4) + (k1[8]-k2[8])*(k1[8]-k4[8])*ff;

  A[0] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[0]*x1+k1[8]-k2[0]*x2) + (k1[8]*y1-k2[8]*y2)*(k1[0]*y1-k2[0]*y2)       + (k1[8]-k2[8])*(k1[0]-k2[0])*ff );   
  A[1] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[1]*x1-k2[1]*x2-k2[8]) + (k1[8]*y1-k2[8]*y2)*(k1[1]*y1-k2[1]*y2)       + (k1[8]-k2[8])*(k1[1]-k2[1])*ff );   
  A[2] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[2]*x1-k2[2]*x2)       + (k1[8]*y1-k2[8]*y2)*(k1[2]*y1-k2[2]*y2)       + (k1[8]-k2[8])*(k1[2]-k2[2])*ff );   
  A[3] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[3]*x1-k2[3]*x2)       + (k1[8]*y1-k2[8]*y2)*(k1[3]*y1-k2[3]*y2)       + (k1[8]-k2[8])*(k1[3]-k2[3])*ff );   
  A[4] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[4]*x1-k2[4]*x2)       + (k1[8]*y1-k2[8]*y2)*(k1[4]*y1+k1[8]-k2[4]*y2) + (k1[8]-k2[8])*(k1[4]-k2[4])*ff );   
  A[5] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[5]*x1-k2[5]*x2)       + (k1[8]*y1-k2[8]*y2)*(k1[5]*y1-k2[8]-k2[5]*y2) + (k1[8]-k2[8])*(k1[5]-k2[5])*ff );   
  A[6] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[6]*x1-k2[6]*x2)       + (k1[8]*y1-k2[8]*y2)*(k1[6]*y1-k2[6]*y2)       + (k1[8]-k2[8])*(k1[6]-k2[6])*ff );   
  A[7] = 2*( (k1[8]*x1-k2[8]*x2)*(k1[7]*x1-k2[7]*x2)       + (k1[8]*y1-k2[8]*y2)*(k1[7]*y1-k2[7]*y2)       + (k1[8]-k2[8])*(k1[7]-k2[7])*ff );   
  
  B[0] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[0]*x1+k1[8]-k4[0]*x4) + (k1[8]*y1-k4[8]*y4)*(k1[0]*y1-k4[0]*y4)       + (k1[8]-k4[8])*(k1[0]-k4[0])*ff );
  B[1] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[1]*x1-k4[1]*x4)       + (k1[8]*y1-k4[8]*y4)*(k1[1]*y1-k4[1]*y4)       + (k1[8]-k4[8])*(k1[1]-k4[1])*ff );
  B[2] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[2]*x1-k4[2]*x4)       + (k1[8]*y1-k4[8]*y4)*(k1[2]*y1-k4[2]*y4)       + (k1[8]-k4[8])*(k1[2]-k4[2])*ff );
  B[3] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[3]*x1-k4[3]*x4-k4[8]) + (k1[8]*y1-k4[8]*y4)*(k1[3]*y1-k4[3]*y4)       + (k1[8]-k4[8])*(k1[3]-k4[3])*ff );
  B[4] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[4]*x1-k4[4]*x4)       + (k1[8]*y1-k4[8]*y4)*(k1[4]*y1+k1[8]-k4[4]*y4) + (k1[8]-k4[8])*(k1[4]-k4[4])*ff );
  B[5] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[5]*x1-k4[5]*x4)       + (k1[8]*y1-k4[8]*y4)*(k1[5]*y1-k4[5]*y4)       + (k1[8]-k4[8])*(k1[5]-k4[5])*ff );
  B[6] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[6]*x1-k4[6]*x4)       + (k1[8]*y1-k4[8]*y4)*(k1[6]*y1-k4[6]*y4)       + (k1[8]-k4[8])*(k1[6]-k4[6])*ff );
  B[7] = 2*( (k1[8]*x1-k4[8]*x4)*(k1[7]*x1-k4[7]*x4)       + (k1[8]*y1-k4[8]*y4)*(k1[7]*y1-k4[7]*y4-k4[8]) + (k1[8]-k4[8])*(k1[7]-k4[7])*ff );

  C[0] = (k1[0]*x1+k1[8]-k2[0]*x2)*(k1[8]*x1-k4[8]*x4) + (k1[0]*y1-k2[0]*y2)*(k1[8]*y1-k4[8]*y4)       + (k1[0]-k2[0])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[0]*x1+k1[8]-k4[0]*x4) + (k1[8]*y1-k2[8]*y2)*(k1[0]*y1-k4[0]*y4)       + (k1[8]-k2[8])*(k1[0]-k4[0])*ff;
  C[1] = (k1[1]*x1-k2[1]*x2-k2[8])*(k1[8]*x1-k4[8]*x4) + (k1[1]*y1-k2[1]*y2)*(k1[8]*y1-k4[8]*y4)       + (k1[1]-k2[1])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[1]*x1-k4[1]*x4)       + (k1[8]*y1-k2[8]*y2)*(k1[1]*y1-k4[1]*y4)       + (k1[8]-k2[8])*(k1[1]-k4[1])*ff;
  C[2] = (k1[2]*x1-k2[2]*x2)*(k1[8]*x1-k4[8]*x4)       + (k1[2]*y1-k2[2]*y2)*(k1[8]*y1-k4[8]*y4)       + (k1[2]-k2[2])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[2]*x1-k4[2]*x4)       + (k1[8]*y1-k2[8]*y2)*(k1[2]*y1-k4[2]*y4)       + (k1[8]-k2[8])*(k1[2]-k4[2])*ff;
  C[3] = (k1[3]*x1-k2[3]*x2)*(k1[8]*x1-k4[8]*x4)       + (k1[3]*y1-k2[3]*y2)*(k1[8]*y1-k4[8]*y4)       + (k1[3]-k2[3])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[3]*x1-k4[3]*x4-k4[8]) + (k1[8]*y1-k2[8]*y2)*(k1[3]*y1-k4[3]*y4)       + (k1[8]-k2[8])*(k1[3]-k4[3])*ff;
  C[4] = (k1[4]*x1-k2[4]*x2)*(k1[8]*x1-k4[8]*x4)       + (k1[4]*y1+k1[8]-k2[4]*y2)*(k1[8]*y1-k4[8]*y4) + (k1[4]-k2[4])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[4]*x1-k4[4]*x4)       + (k1[8]*y1-k2[8]*y2)*(k1[4]*y1+k1[8]-k4[4]*y4) + (k1[8]-k2[8])*(k1[4]-k4[4])*ff;
  C[5] = (k1[5]*x1-k2[5]*x2)*(k1[8]*x1-k4[8]*x4)       + (k1[5]*y1-k2[5]*y2-k2[8])*(k1[8]*y1-k4[8]*y4) + (k1[5]-k2[5])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[5]*x1-k4[5]*x4)       + (k1[8]*y1-k2[8]*y2)*(k1[5]*y1-k4[5]*y4)       + (k1[8]-k2[8])*(k1[5]-k4[5])*ff;
  C[6] = (k1[6]*x1-k2[6]*x2)*(k1[8]*x1-k4[8]*x4)       + (k1[6]*y1-k2[6]*y2)*(k1[8]*y1-k4[8]*y4)       + (k1[6]-k2[6])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[6]*x1-k4[6]*x4)       + (k1[8]*y1-k2[8]*y2)*(k1[6]*y1-k4[6]*y4)       + (k1[8]-k2[8])*(k1[6]-k4[6])*ff;
  C[7] = (k1[7]*x1-k2[7]*x2)*(k1[8]*x1-k4[8]*x4)       + (k1[7]*y1-k2[7]*y2)*(k1[8]*y1-k4[8]*y4)       + (k1[7]-k2[7])*(k1[8]-k4[8])*ff +\
         (k1[8]*x1-k2[8]*x2)*(k1[7]*x1-k4[7]*x4)       + (k1[8]*y1-k2[8]*y2)*(k1[7]*y1-k4[7]*y4-k4[8]) + (k1[8]-k2[8])*(k1[7]-k4[7])*ff;


  for(int i = 0; i < 8; ++i) {
    J[i].first  = (A[i]*B[8] - A[8]*B[i]) / (B[8]*B[8]);
    J[i].second = (2*C[8]*C[i]*A[8]*B[8] - C[8]*C[8]*A[i]*B[8] - C[8]*C[8]*A[8]*B[i]) / (A[8]*A[8]*B[8]*B[8]);
  }

  return res;
}


}; // namespace se 

#endif // PROJECTIVE_RECT_H_INCLUDED

