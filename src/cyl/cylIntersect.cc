#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "geometry.h"

#include "cylinder.h"
#include "unionfind.h"
#include "cylIntersect.h"
#include "s3d.h"

#include "polyroots.h"

#include "miller87.h"

extern Space W3;
extern Space W2;
extern Frame F3;
extern Frame F2;

static int prints3d=0;
static int printinfo=0;
#define VVDotCaller 0

#define SQ(x) ((x)*(x))
#define printCaller(function) printf("%s called %s in %s at line %d\n", __func__, function, __FILE__, __LINE__)

Point LineIntersectPlane(Point P, Vector v, Point Q, Normal n) {
  return PVAdd(P,SVMult(NVApply(n,PPDiff(Q,P))/NVApply(n,v),
        v));
}


/* Compute the signed distange of P1-P2 relative to vector dir */
Scalar PPSDist(Vector dir, Point P1, Point P2) {
  Scalar dist;
  dist = PPDist(P1,P2);
#if VVDotCaller
  printCaller("VVDot");
#endif
  if ( VVDot(dir,PPDiff(P1,P2)) < 0 ) {
    return -dist;
  } else {
    return dist;
  }
}

int CircleIntersectEllipse(Scalar r1, Point ecenter,
    Scalar d, Scalar e, Point pts[]) {
  Scalar y4,y3,y2,y1,cnst;
  int ne=0;
  Scalar x0,y0;

  PCoords(ecenter,F2,&x0,&y0);

  /*
     y4 = d^4 + e^4 -2e^2d^2;
     y3 = -4d^4y0 + 4e^2d^2y0;
     y2 = 6d^4y0^2 -2d^4e^2 + 2e^2r1^2d^2 -2e^2d^2y0^2 -2e^4r1^2+2e^4d^2 + 4e^4x0^4
     +2e^2x0^2d^2 - 2e^4x0^2;
     y= 4d^4y0e^2 - 4d^4y0^3 - -4e^2x0^2d^2y0 + 4e^2r1^2d^2y0;
     cnst = d^4y0^4 + e^4d^4 -2d^4y0^2e^2 + 2e^2r1^2d^2y0^2 + e^4 r1^4 -2e^4r1^2d^2 -4e^4x0^4r1^2
     e^4x0^4 + 2e^2x0^2d^2y0^2 + 2e^4r1^2x0^2 -2e^4x0^2d^2;
     */
  y4 = pow(d,4)
    + pow(e,4)
    -2.*SQ(e)*SQ(d);
  y3 = -4.*pow(d,4)*y0
    + 4*SQ(e)*SQ(d)*y0;
  y2 = -2.*SQ(e)*SQ(d)*SQ(y0)
    +2.*SQ(e)*SQ(x0)*SQ(d)
    +2.*SQ(e)*SQ(r1)*SQ(d)
    -2.*pow(e,4)*SQ(r1)
    -2.*pow(e,4)*SQ(x0)
    +2.*pow(e,4)*SQ(d)
    +6.*pow(d,4)*SQ(y0)
    -2.*pow(d,4)*SQ(e)
    + 4.*pow(e,4)*SQ(x0);
  y1= - 4.*pow(d,4)*pow(y0,3)
    -4.*SQ(e)*SQ(r1)*SQ(d)*y0
    -4.*SQ(e)*SQ(x0)*SQ(d)*y0
    +4.*pow(d,4)*y0*SQ(e) ;
  cnst =
    + pow(e,4)*pow(r1,4)
    + pow(e,4)*pow(x0,4)
    + pow(d,4)*pow(y0,4)
    + pow(e,4)*pow(d,4)
    + 2.*SQ(e)*SQ(r1)*SQ(d)*SQ(y0)
    + 2.*SQ(e)*SQ(x0)*SQ(d)*SQ(y0)
    + 2.*pow(e,4)*SQ(r1)*SQ(x0)
    -2.*pow(e,4)*SQ(r1)*SQ(d)
    -2.*pow(e,4)*SQ(x0)*SQ(d)
    -2.*pow(d,4)*SQ(y0)*SQ(e)

    -4.*pow(e,4)*SQ(x0)*SQ(r1);

  double yroots[4];
  double xroots[4];

  if ( printinfo )
    fprintf(stderr,"qR: %g %g %g %g\n",y3/y4, y2/y4, y1/y4, cnst/y4);
  int nr = quarticRoots(y3/y4, y2/y4, y1/y4, cnst/y4, yroots);

  int i;
  for (i=0; i<nr; i++) {
    if ( r1*r1-yroots[i]*yroots[i] > 0 ) {
      if ( printinfo )
        fprintf(stderr,"%g %g   %g\n",yroots[i],sqrt(r1*r1-yroots[i]*yroots[i]),
            SQ(SQ(yroots[i]))+y3/y4*yroots[i]*SQ(yroots[i])+
            y2/y4*SQ(yroots[i]) + y1/y4*yroots[i] + cnst/y4);
      double x,y;
      y = yroots[i];
      x = sqrt(r1*r1-y*y);
      if ( fabs((x-x0)*(x-x0)/SQ(d)+(y-y0)*(y-y0)/SQ(e)-1.) < 1e-5 ) {
        xroots[i] = sqrt(r1*r1-yroots[i]*yroots[i]);
        pts[ne] = PCreate(F2, sqrt(r1*r1-yroots[i]*yroots[i]),yroots[i]);
        ne++;
      } else {
        xroots[i] = -1.*sqrt(r1*r1-yroots[i]*yroots[i]);
        pts[ne] = PCreate(F2, -1*sqrt(r1*r1-yroots[i]*yroots[i]),yroots[i]);
        ne++;
      }
    }
  }
  return ne;
}

int FindExtremeOfCircleIntersectEllipse(Scalar r1, Point ecenter, Scalar d, Scalar e, Point pts[]) {
  int ne=0;
  Scalar x0,y0;

  PCoords(ecenter,F2,&x0,&y0);
  if ( printinfo )
    fprintf(stderr,"FEOCIE: circle r=%g, ellipse (%g,%g) %g %g\n",r1,x0,y0,d,e);

  // Create edges of circle, ellipse
  Point eright = PCreate(F2, x0+d, y0);
  Point eleft = PCreate(F2, x0-d, y0);
  Point cright = PCreate(F2, r1,0.);
  Point cleft = PCreate(F2, -r1,0.);

  // Create required info about ellipse
  double f;
  Point lfoci,rfoci;
  if ( d<e ) {
    f = sqrt(e*e-d*d);
    lfoci = PCreate(F2, x0, y0-f);
    rfoci = PCreate(F2, x0, y0+f);
  } else {
    f = sqrt(d*d-e*e);
    lfoci = PCreate(F2, x0-f, y0);
    rfoci = PCreate(F2, x0+f, y0);
  }


  // If edges of ellipse in circle, then add to list of points
  if ( PPDist(eright,FOrg(F2))<=r1 ) {
    pts[ne] = eright;
    ne++;
  }
  if ( PPDist(eleft,FOrg(F2))<=r1 ) {
    pts[ne] = eleft;
    ne++;
  }

  // If edges of circle inside ellipse, then add to list of points
  double td;
  if ( e > d ) td = e; else td = d;
  if ( PPDist(cleft,lfoci)+PPDist(cleft,rfoci)<= 2*td) {
    pts[ne] = cleft;
    ne++;
  }
  if ( PPDist(cright,lfoci)+PPDist(cright,rfoci)<= 2*td) {
    pts[ne] = cright;
    ne++;
  }

  // If two of circle or ellipse edges are in the result, then we
  // don't need to solve 4th degree equation
  if ( ne == 2 ) {
#if VVDotCaller
    printCaller("VVDot");
#endif
    if ( VVDot(FV(F2,0),PPDiff(pts[0],cleft)) >
        VVDot(FV(F2,0),PPDiff(pts[1],cleft)) ) {
      Point tmp;
      tmp = pts[0];
      pts[0] = pts[1];
      pts[1] = tmp;
    }
    return 2;
  }

  if ( ne > 2 ) {
    fprintf(stderr,"Warning!  Bad ne in FindExtremeOfCircleIntersectEllipse\n");
  }

  //	PrintPoint(lfoci);
  //	PrintPoint(rfoci);


  Point pts2[4];
  int i;

  int nr = CircleIntersectEllipse(r1, ecenter, d, e, pts2);
  if ( printinfo )
    fprintf(stderr,"nr is %d\n",nr);


  for (i=0; i<nr; i++) {
    pts[ne] = pts2[i];
    ne++;
  }

  /*
   * Need to thin roots to no more than 2!
   */
  if ( ne > 2 ) {
    double d[6];
#if VVDotCaller
    printCaller("VVDot");
#endif
    for (i=0; i<ne; i++) {
      d[i] = VVDot(FV(F2,0),PPDiff(pts[i],cleft));
    }
    int imin=0,imax=0; Scalar fmin=d[0],fmax=d[0];
    Point pmin=pts[0],pmax=pts[0];
    for (i=1; i<ne; i++) {
      if ( d[i]<fmin ) {
        imin=i;
        fmin = d[i];
        pmin = pts[i];
      }
      if ( d[i]>fmax ) {
        imax=i;
        fmax = d[i];
        pmax = pts[i];
      }
    }
    pts[0] = pmin;
    pts[1] = pmax;
    ne=2;
  }
  return ne;
}

// Compute the intersection of a circle with a cylinder.
// Assume the circle is in the xy=plane, and that the axis of
// the cylinder is NOT aligned with the z-axis.
// We will construct the ellipse that is the intersection of the plane
// parallel to the xy-plane (through the center of the circle) 
// with the cylinder.
// The xy-coordinates of the cylinder's axis will point in the direction
// of the major axis (v1); v2=v1xz will point in the direction of the minor
// axis.
Intersection ComputeIntersection(Point Cntr, Vector v, Scalar r, Cylinder C) {
  Scalar c0,c1,c2;
  Scalar p0,p1,p2;
  Cylinder CR;
  Intersection intersection;

  //	S3dDisk(Cntr, v, r*3);
  if ( printinfo ) {
    fprintf(stderr,"ComputeIntersection: \n  ");
    PPrintf(stderr,Cntr);
    fprintf(stderr,"  ");VPrintf(stderr,v);
  }

  VCoords(C.v,F3, &c0,&c1,&c2);
  Vector v1=VCreate3(F3,c0/sqrt(c0*c0+c1*c1),c1/sqrt(c0*c0+c1*c1),0);
  //	Vector v1=VCreate3(F3,sqrt(c0*c0+c1*c1),0.,c2);
  if ( printinfo ) {
    fprintf(stderr,"  New frame v1:  "); VPrintf(stderr,v1);
  }
  Vector v2=VVCross(FV(F3,2),v1);
  if ( printinfo ) {
	  fprintf(stderr,"  FCreate:\n");
	  fprintf(stderr,"   "); VPrintf(stderr,v1);
	  fprintf(stderr,"   "); VPrintf(stderr,v2);
	  fprintf(stderr,"   "); VPrintf(stderr,FV(F3,2));
  }

  Frame FT=FCreate3("tmp",FOrg(F3),v1,v2,FV(F3,2));
  AffineMap AT = ACreateF(FT,F3);
  AffineMap ATR = ACreateF(F3,FT);
  // The following shouldn't be needed
  if(1){
  CR.P =PAxform(C.P,AT);
  CR.v = VAxform(C.v,AT);
  CR.r = C.r;
  CR.h = C.h;
  CR.A = CR.P;
  CR.B = PVAdd(CR.A,SVMult(CR.h,CR.v));
  if ( printinfo ) {
    fprintf(stderr,"  C.v before rotation: ");
    VPrintf(stderr,C.v);
    fprintf(stderr,"  C.v after rotation: ");
    VPrintf(stderr,CR.v);
  }
  }
  // We need this to map from 3D down to 2D for the ellipse center
  AffineMap Project = ACreate(F3, FOrg(F2), FV(F2, 0), FV(F2, 1),
      VZero(W2));
  Point ECntr;
  // Which is correct?
  if (1) {
  ECntr = LineIntersectPlane(CR.P,CR.v, Cntr, VDual(v));
  Scalar tc0,tc1,tc2;
  PCoords(ECntr,F3, &tc0,&tc1,&tc2);
  if ( printinfo )
    fprintf(stderr," ECntr %g %g %g\n",tc0,tc1,tc2);
  } else {
  ECntr = LineIntersectPlane(C.P,C.v, Cntr, VDual(v));
  if ( printinfo ) {
    Scalar tc0,tc1,tc2;
    PCoords(C.P,F3, &tc0,&tc1,&tc2);
    fprintf(stderr," C.P %g %g %g\n",tc0,tc1,tc2);
    PCoords(ECntr,F3, &tc0,&tc1,&tc2);
    fprintf(stderr," ECntr %g %g %g\n",tc0,tc1,tc2);
  }
  }

	  
  PCoords(Cntr,F3, &p0,&p1,&p2); // we only care about p2
  Point pts[6];
  double denom;
  VCoords(C.v,F3, &c0,&c1,&c2); // we don't really need to extract these again, but...
  denom = sqrt(c0*c0+c1*c1+c2*c2);
  c0 = c0/denom;
  c1 = c1/denom;

  switch (FindExtremeOfCircleIntersectEllipse(r, PAxform(ECntr, Project),
        C.r/sqrt(1-(c0*c0+c1*c1)), C.r,
        pts)) {
    case 0:
      if ( printinfo )
        fprintf(stderr,"CI: case 0\n");
      intersection.hitmiss = 0;
      return intersection;
    case 1: {
              if ( printinfo )
                fprintf(stderr,"CI: case 1\n");
              intersection.hitmiss = 1;
              PCoords(pts[0],F2, &p0,&p1);
              Point tmpP = PCreate3(FT, p0,p1,p2);
              intersection.a = intersection.b = VVDot(PPDiff(tmpP,C.P),C.v);
              break;
            }
    case 2: {
              if ( printinfo )
                fprintf(stderr,"CI: case 2\n");

              intersection.hitmiss = 1;
              PCoords(pts[0],F2, &p0,&p1);
              Point tmpP = PCreate3(FT, p0,p1,p2);
              if ( prints3d )
                printf("o 1 1 0\n");
              intersection.a = VVDot(PPDiff(tmpP,C.P),C.v);

              PCoords(pts[1],F2, &p0,&p1);
              tmpP = PCreate3(FT, p0,p1,p2);
              if ( prints3d )
                printf("o 1 1 0\n");
              intersection.b = VVDot(PPDiff(tmpP,C.P),C.v);

	      if ( intersection.a > intersection.b ) {
		      double t;
		      t = intersection.a;
		      intersection.a = intersection.b;
		      intersection.b = t;
	      }
	      if ( printinfo ) {
		      fprintf(stderr," intersection = [%g,%g]\n",
			      intersection.a,intersection.b);
	      }
              break;
            }
    default:
            fprintf(stderr,"ComputeIntersection: what are we doing here?\n");
            exit(1);
  }
  return intersection;
}


// Assume C1 canonical
// Return value:
// -1 - too far away
// 0 - parallel axes
// 1 close enough, and CP1 and CP2 contain closest point on each axis
int ClosestPointsOnCylinders(Cylinder C1, Cylinder C2, Point* CP1, Point* CP2) {
  Scalar a1,a2,a3;
  Scalar p1,p2,p3;
  PCoords(C2.P, F3, &p1,&p2,&p3);
  VCoords(C2.v, F3, &a1,&a2,&a3);

  // special case - axes are parallel, so just check distance
  if (printinfo) fprintf(stderr,"how close is close?  %g %g\n",a3,fabs(a3-1.));
  if ( fabs(a3-1.) < 1e-5 ) {
    Point CP2 = PVAdd(C2.P, SVMult(VVDot(C2.v, PPDiff(C1.P,C2.P)),
          C2.v));
    Scalar dist;
    dist = PPDist(C1.P,CP2);
    if (printinfo) {
#if VVDotCaller
      printCaller("VVDot");
#endif
      fprintf(stderr,"CPOC: Parallel axes dist %g vs %g (%g)\n",
          dist,C1.r+C2.r,VVDot(C2.v,PPDiff(C1.P,CP2)));
    }
    if ( dist <= C1.r + C2.r ) {
      return 0;
    } else {
      return -1;
    }
  }

  // Compute closest point in 2D
  Point P2 = PCreate(F2,p1,p2);
  Vector A2 = VCreate(F2,a1,a2);
  Vector v = PPDiff(P2,FOrg(F2));
#if VVDotCaller
  printCaller("VVDot");
#endif
  Point ClosestPoint = PVAdd(P2,SVMult(-VVDot(v,VNormalize(A2)),
        VNormalize(A2)));
  Scalar dist = PPDist(FOrg(F2),ClosestPoint);
  if ( printinfo )
    fprintf(stderr,"CPOC: dist is %g, |A2| is %g\n",dist,VMag(A2));
  // No intersection!
  if ( dist > C1.r + C2.r ) {
    return -1;
  }

  // Convert 2D closest point on C2 projected axis to 3D closest point
  *CP2 = PVAdd(C2.P, SVMult(PPSDist(A2,ClosestPoint,P2)/VMag(A2),C2.v));

  Vector cp;
  cp = VVCross(C1.v, C2.v);
#if VVDotCaller
  printCaller("VVDot");
#endif
  if ( VVDot(cp, PPDiff(C1.P,C2.P)) < 0 ) {
    cp = SVMult(-1.,cp);
  }
  *CP1 = PVAdd(*CP2,SVMult(dist,VNormalize(cp)));
  return 1;
}


// Make axis of C2 have positive dot product with axis of C1
void AlignCylinders(Cylinder C1, Cylinder* C2) {
#if VVDotCaller
  printCaller("VVDot");
#endif
  if ( VVDot(C1.v,C2->v) > 0 ) {
    return;
  } else {
    C2->v = SVMult(-1.,C2->v);
    C2->P = C2->B;
    C2->B = C2->A;
    C2->A = C2->P;
    return;
  }
}

// Create a frame where the last coordinate vector is the cylinder's axix
static Frame CylFrame(Cylinder C, const char* name) {
  Vector v1,v2,v3;
  Scalar c0,c1,c2;

  v3 = C.v;
  VCoords(v3,F3, &c0,&c1,&c2);
  if ( fabs(c0)<=fabs(c1) && fabs(c0)<=fabs(c2) ) {
    v1 = VVCross(v3,FV(F3,0));
  } else if ( fabs(c1)<=fabs(c0) && fabs(c1)<=fabs(c2) ) {
    v1 = VVCross(v3,FV(F3,1));
  } else {
    v1 = VVCross(v3,FV(F3,2));
  }
  v2 = VVCross(v1,v3);

  Frame FT;
  FT = FCreate3(name,C.P,v1,v2,v3);
  return FT;
}


static void MapToCanonicalCylinder(Cylinder C1, Cylinder C2,
    Cylinder* C1C, Cylinder* C2C) {
#if 0
  Vector v1,v2,v3;
  Scalar c0,c1,c2;

  v3 = C1.v;
  VCoords(v3,F3, &c0,&c1,&c2);
  if ( fabs(c0)<=fabs(c1) && fabs(c0)<=fabs(c2) ) {
    v1 = VVCross(v3,FV(F3,0));
  } else if ( fabs(c1)<=fabs(c0) && fabs(c1)<=fabs(c2) ) {
    v1 = VVCross(v3,FV(F3,1));
  } else {
    v1 = VVCross(v3,FV(F3,2));
  }
  v2 = VVCross(v1,v3);
#endif

  Frame FT;
  FT = CylFrame(C1,"test");
  //    FT = FCreate3("test",C1.P,v1,v2,v3);
  AffineMap Map = ACreateF(FT,F3);

  // now map C1 and C2 through Map
  C1C->P = PAxform(C1.P, Map);
  C1C->v = VAxform(C1.v, Map);
  C1C->A = C1C->P;
  C1C->B = PAxform(C1.B, Map);
  C1C->h = C1.h;
  C1C->r = C1.r;

  C2C->P = PAxform(C2.P, Map);
  C2C->v = VAxform(C2.v, Map);
  C2C->A = C2C->P;
  C2C->B = PAxform(C2.B, Map);
  C2C->h = C2.h;
  C2C->r = C2.r;
}

int IntervalsOverlap(Scalar A1, Scalar B1, Scalar A2, Scalar B2) {
	if (printinfo) 
		fprintf(stderr,"IntervalsOverlap(%g,%g, %g,%g)\n",A1,B1,A2,B2);
  if ( A1 > B1 ) {
    Scalar T;
    T = A1;
    A1 = B1;
    B1 = T;
  }
  if ( A2 > B2 ) {
    Scalar T;
    T = A2;
    A2 = B2;
    B2 = T;
  }

  if ( (A2 > B1  &&  B2 > B1) ||
      (A2 < A1  &&  B2 < A1)) {
    return 0;
  } else {
    return 1;
  }
}

Intersection IntersectLineCircle(Point P, Vector v,
    Point C, Normal n, Scalar r) {
  Intersection intersection;
  double roots[2];
#if VVDotCaller
  printCaller("VVDot");
#endif
  int nr=quadraticRoots(1., 2*VVDot(PPDiff(P,C),v), SQ(PPDist(P,C))-r*r,
      roots);
  if ( nr==0 ) {
    intersection.hitmiss=0;
  } else if ( nr==1 ) {
    intersection.hitmiss=1;
    intersection.a = intersection.b = roots[0];
  } else {
    intersection.hitmiss=1;
    if ( roots[0] < roots[1] ) {
      intersection.a = roots[0];
      intersection.b = roots[1];
    } else {
      intersection.a = roots[1];
      intersection.b = roots[0];
    }
  }
  return intersection;
}

int IntersectCircles(Point C1, Normal n1, Scalar r1,
    Point C2, Normal n2, Scalar r2) {
#if 0
  S3dCircle(C1,NDual(n1),r1);
  S3dCircle(C2,NDual(n2),r2);
  fprintf(stderr,"IC:\nC1 %g\n  ",r1);
  PPrintf(stderr,C1);
  fprintf(stderr,"  ");VPrintf(stderr,NDual(n1));
  fprintf(stderr,"C2 %g\n  ",r2);
  PPrintf(stderr,C2);
  fprintf(stderr,"  ");VPrintf(stderr,NDual(n2));
#endif
  Vector v;
  v = VVCross(NDual(n1),NDual(n2));
  Vector v1=VNormalize(VVCross(v,NDual(n1)));
#if VVDotCaller
  printCaller("VVDot");
#endif
  Point P = PVAdd(C1, SVMult(VVDot(PPDiff(C2,C1),NDual(n2))/VVDot(v1,NDual(n2)),
        v1));
  //	S3dBall(P,.3);
  //	S3dBall(PVAdd(P,v),.4);
  Intersection cl1 = IntersectLineCircle(P,v, C1,n1,r1);
  if ( !cl1.hitmiss ) return 0;
  Intersection cl2 = IntersectLineCircle(P,v, C2,n2,r2);
  if ( !cl2.hitmiss ) return 0;

  return IntervalsOverlap(cl1.a, cl1.b, cl2.a, cl2.b);
}

#if 0
void PrintS3dIntersection(Cylinder C1, Cylinder C2) {
  double daxis;
  Point ap[31][2];
  int np=0;
  Cylinder C1C, C2C;

  MapToCanonicalCylinder(C1,C2,&C1C,&C2C);

  daxis = C1.h/10;
  if ( daxis > .2 ) { daxis = .2; }
  if ( C1.h/daxis > 30 ) { daxis = C1.h/30; }

  double t;
  for (t=0; t<=C1.h; t+=daxis) {
    Point pts[4];
    int nr;
    nr = CircleIntersectEllipse(C1C.r, LineIntersectPlane(C2C.P,C2C.v,
          ?,VDual(?)),
        ?,?,pts);
    switch (nr){
      case 0:
        break;
      case 1:
        ap[np][0] = pts[0];
        ap[np][1] = pts[0];
        np++;
        break;
      case 2:
        /* will likely need to do more */
        ap[np][0] = pts[0];
        ap[np][1] = pts[1];
        np++;
        break;
      default:
        fprintf(stderr,"PrintS3dIntersection: too many int\n");
        break;
    }
  }
}
#endif

HitMiss HitStatus(int h1, int h2) {
  if ( h1 && h2 ) {
    if ( printinfo ) { fprintf(stderr,"HitStatus: HH\n"); }
    return HH;
  } else if ( h1 && !h2 ) {
    if ( printinfo ) { fprintf(stderr,"HitStatus: HM\n"); }
    return HM;
  } else if ( !h1 && h2 ) {
    if ( printinfo ) { fprintf(stderr,"HitStatus: MH\n"); }
    return MH;
  } else {
    if ( printinfo ) { fprintf(stderr,"HitStatus: MM\n"); }
    return MM;
  }
}

int CylIntersect(Cylinder C1, Cylinder C2) {
  Point CP1,CP2;
  Cylinder C1C, C2C;

  if (printinfo) {
    double x, y, z;
    fprintf(stderr,"\n\nCylinder 1\n");
    PCoords(C1.P, F3, &x, &y, &z);
    fprintf(stderr,"P: %g, %g, %g\n", x, y, z);
    PCoords(C1.A, F3, &x, &y, &z);
    fprintf(stderr,"A: %g, %g, %g\n", x, y, z);
    PCoords(C1.B, F3, &x, &y, &z);
    fprintf(stderr,"B: %g, %g, %g\n", x, y, z);
    VCoords(C1.v, F3, &x, &y, &z);
    fprintf(stderr,"v: %g, %g, %g\n", x, y, z);
    fprintf(stderr,"r: %g\n", C1.r);
    fprintf(stderr,"h: %g\n", C1.h);

    fprintf(stderr,"\n\nCylinder 2\n");
    PCoords(C2.P, F3, &x, &y, &z);
    fprintf(stderr,"P: %g, %g, %g\n", x, y, z);
    PCoords(C2.A, F3, &x, &y, &z);
    fprintf(stderr,"A: %g, %g, %g\n", x, y, z);
    PCoords(C2.B, F3, &x, &y, &z);
    fprintf(stderr,"B: %g, %g, %g\n", x, y, z);
    VCoords(C2.v, F3, &x, &y, &z);
    fprintf(stderr,"v: %g, %g, %g\n", x, y, z);
    fprintf(stderr,"r: %g\n", C2.r);
    fprintf(stderr,"h: %g\n", C2.h);
  }
  AlignCylinders(C1,&C2);
  MapToCanonicalCylinder(C1,C2,&C1C,&C2C);
    if ( printinfo ) {
    double x, y, z;
    fprintf(stderr,"\n\nCylinder 1 Canonical\n");
    PCoords(C1C.P, F3, &x, &y, &z);
    fprintf(stderr,"P: %g, %g, %g\n", x, y, z);
    PCoords(C1C.A, F3, &x, &y, &z);
    fprintf(stderr,"A: %g, %g, %g\n", x, y, z);
    PCoords(C1C.B, F3, &x, &y, &z);
    fprintf(stderr,"B: %g, %g, %g\n", x, y, z);
    VCoords(C1C.v, F3, &x, &y, &z);
    fprintf(stderr,"v: %g, %g, %g\n", x, y, z);
    fprintf(stderr,"r: %g\n", C1C.r);
    fprintf(stderr,"h: %g\n", C1C.h);

    fprintf(stderr,"\n\nCylinder 2 Canonical\n");
    PCoords(C2C.P, F3, &x, &y, &z);
    fprintf(stderr,"P: %g, %g, %g\n", x, y, z);
    PCoords(C2C.A, F3, &x, &y, &z);
    fprintf(stderr,"A: %g, %g, %g\n", x, y, z);
    PCoords(C2C.B, F3, &x, &y, &z);
    fprintf(stderr,"B: %g, %g, %g\n", x, y, z);
    VCoords(C2C.v, F3, &x, &y, &z);
    fprintf(stderr,"v: %g, %g, %g\n", x, y, z);
    fprintf(stderr,"r: %g\n", C2C.r);
    fprintf(stderr,"h: %g\n", C2C.h);
  }
  //	C1C = C1;
  //	C2C = C2;

  switch(ClosestPointsOnCylinders(C1C,C2C,&CP1,&CP2)) {
    case -1:
      if ( printinfo )
        fprintf(stderr,"distance between closest points too large.\n");
      return 0; // TEST: closest points are too far away
    case 0:;
           //		if ( printinfo )
           fprintf(stderr,"Parallel axes\n");
           // The axes are parallel.  We assume that the distance
           // between the axes is less than C1C.r+C2C.r
           Scalar c0,c1,c2,A1,B1,A2,B2;
           PCoords(C1C.A,F3, &c0,&c1,&c2);
           A1 = c2;
           PCoords(C1C.B,F3, &c0,&c1,&c2);
           B1 = c2;

           PCoords(C2C.A,F3, &c0,&c1,&c2);
           A2 = c2;
           PCoords(C2C.B,F3, &c0,&c1,&c2);
           B2 = c2;

#if 0
           if ( A1 > B1 ) {
             if ( (A2 > A1  &&  B2 > A1) ||
                 (A2 < B1  &&  B2 < B1)) {
               return 0;
             } else {
               if ( printinfo )
                 fprintf(stderr,"Parallel cylinders\n");
               return 1;
             }
           } else {
             if ( (A2 < A1  &&  B2 < A1) ||
                 (A2 > B1  &&  B2 > B1)) {
               return 0;
             } else {
               if ( printinfo )
                 fprintf(stderr,"Parallel cylinders\n");
               return 1;
             }
           }
#else
           // TEST: check that parallel cylinders overlap
           if ( printinfo )
             fprintf(stderr,"Parallel cylinders: %g %g, %g %g\n",
                 A1,B1,A2,B2);
           return IntervalsOverlap(A1,B1, A2,B2);
#endif

    case 1:;
           // First check if CP's are both on cylinder segments
           Scalar cps1,cps2;
           if ( printinfo ) {
             PPrintf(stderr,CP1);
             PPrintf(stderr,CP2);
           }
           cps1 = PPSDist(C1C.v,CP1,C1C.P);
           cps2 = PPSDist(C2C.v,CP2,C2C.P);
           if ( printinfo )
             fprintf(stderr,"cps1 %g (%g), cps2 %g (%g)\n",
                 cps1,C1C.h,cps2,C2C.h);

           // TEST: closest points inside each finite cylinder
           // If closest point on each axis lies on inside of each finite
           // cylinder, then there's an intersection.  This works because
           // the line between the closest points is perpendicular to
           // the axes and thus in the planes the two circles at the
           // closest point on each axis.
           if ( cps1 >= 0 && cps1 <= C1C.h &&
               cps2 >= 0 && cps2 <= C2C.h ) {
             if ( printinfo )
               fprintf(stderr,"ClosestPoints lie inside each other\n");
             return 1;
           }

           // Test perpendicular axes
#if VVDotCaller
           printCaller("VVDot");
#endif
           if ( fabs(VVDot(C1.v,C2.v)) < 1e-4 ) {
             if (printinfo)
               fprintf(stderr,"Calling IntersectCircles!\n");
             CP1 = PVAdd(C1.P,SVMult(cps1,C1.v));
             if ( cps1 < 0 ) {
               CP1 = C1.A;
             } else if ( cps1 > C1.h ) {
               CP1 = C1.B;
             }
             CP2 = PVAdd(C2.P,SVMult(cps2,C2.v));
             if ( cps2 < 0 ) {
               CP2 = C2.A;
             } else if ( cps2 > C2.h ) {
               CP2 = C2.B;
             }
             return IntersectCircles(CP1,VDual(C1.v),C1.r,
                 CP2,VDual(C2.v),C2.r);
           }

           if (printinfo)
             fprintf(stderr,"NOT calling IntersectCircles\n");

           // Now, intersect the top and bottom of each cylinder with
           // the other ellipse
           //		S3dCircle(C1C.A,C1C.v,C1C.r);
           //		S3dCircle(C1C.B,C1C.v,C1C.r);
           Intersection I1B; I1B = ComputeIntersection(C1C.A,C1C.v,C1C.r, C2C);
           Intersection I1T; I1T = ComputeIntersection(C1C.B,C1C.v,C1C.r, C2C);
           HitMiss hm1;

           hm1 = HitStatus(I1B.hitmiss, I1T.hitmiss);

           if ( prints3d ) {
             S3dDisk(C1.A,C1.v,C1.r*3);
             S3dDisk(C1.B,C1.v,C1.r*3);
           }

           // Display the region of intersection on opposite
           // infinite cylinder
           if ( I1B.hitmiss && printinfo && prints3d ) {
             fprintf(stderr,"I1B hit: %g to %g\n",I1B.a,I1B.b);
             VPrintf(stderr,C2C.v);
             PPrintf(stderr,C2C.P);
             S3dCircle(PVAdd(C2.P,SVMult(I1B.a,C2.v)),C2.v,C2.r*1.001);
             S3dCircle(PVAdd(C2.P,SVMult(I1B.b,C2.v)),C2.v,C2.r*1.001);
           }
           if ( I1T.hitmiss && printinfo && prints3d ) {
             fprintf(stderr,"I1T hit: %g to %g\n",I1T.a,I1T.b);
             VPrintf(stderr,C2C.v);
             PPrintf(stderr,C2C.P);
             S3dCircle(PVAdd(C2.P,SVMult(I1T.a,C2.v)),C2.v,C2.r*1.001);
             S3dCircle(PVAdd(C2.P,SVMult(I1T.b,C2.v)),C2.v,C2.r*1.001);
           }
           if ( printinfo )
             fprintf(stderr,"Intersection 1 hitmiss: %d %d\n",I1B.hitmiss,I1T.hitmiss);
           // TEST: HH
           // If both C1's top and bottom intersect infinite cylinder C2,
           // then just see if zone covered by C1 on infinite cylinder C2
           // overlaps the finite cylinder C2.
           if ( hm1 == HH ) {
             if ( printinfo )
               fprintf(stderr,"HH: IntervalsOverlap?\n");
             return IntervalsOverlap(I1B.a,I1T.b, 0., C2.h);
           }

           Cylinder C1D,C2D;
           MapToCanonicalCylinder(C2,C1,&C2D,&C1D);
           Intersection I2B; I2B = ComputeIntersection(C2D.A,C2D.v,C2D.r, C1D);
           Intersection I2T; I2T = ComputeIntersection(C2D.B,C2D.v,C2D.r, C1D);
           HitMiss hm2 = HitStatus(I2B.hitmiss, I2T.hitmiss);

           if ( prints3d ) {
             S3dDisk(C2.A,C2.v,C2.r*3);
             S3dDisk(C2.B,C2.v,C2.r*3);
           }

           if ( printinfo )
             fprintf(stderr,"Intersection 2 hitmiss: %d %d\n",I2B.hitmiss,I2T.hitmiss);

           // Display the region of intersection on the opposite
           // infinite cylinder
           if ( I2B.hitmiss && printinfo && prints3d ) {
             fprintf(stderr,"I2B hit: %g to %g\n",I2B.a,I2B.b);
             VPrintf(stderr,C1.v);
             PPrintf(stderr,C1.P);
             S3dCircle(PVAdd(C1.P,SVMult(I2B.a,C1.v)),C1.v,C1.r*1.001);
             S3dCircle(PVAdd(C1.P,SVMult(I2B.b,C1.v)),C1.v,C1.r*1.001);
           }
           if ( I2T.hitmiss && printinfo && prints3d ) {
             fprintf(stderr,"I2T hit: %g to %g\n",I2T.a,I2T.b);
             VPrintf(stderr,C1.v);
             PPrintf(stderr,C1.P);
             S3dCircle(PVAdd(C1.P,SVMult(I2T.a,C1.v)),C1.v,C1.r*1.001);
             S3dCircle(PVAdd(C1.P,SVMult(I2T.b,C1.v)),C1.v,C1.r*1.001);
           }

           // TEST: HH
           // If both C2's top and bottom intersect infinite cylinder C1,
           // then just see if zone covered by C2 on infinite cylinder C1
           // overlaps the finite cylinder C1.
           if ( hm2 == HH ) {
             if ( printinfo )
               fprintf(stderr,"HH: IntervalsOverlap?\n");
             return IntervalsOverlap(I2B.a,I2T.b, 0., C1.h);
           }

           // TEST: MM cases
           if ( (hm1==MM && (cps1 < 0 || cps1 > C1.h)) ||
               (hm2==MM && (cps2 < 0 || cps2 > C2.h)) ) {
             if ( printinfo )
               fprintf(stderr,"MM, with CP not on finite cylinder. %d %g; %d %g\n",hm1, cps1, hm2, cps2);
             return 0;
           }
           if ( hm1==MM || hm2==MM ) {
             if ( printinfo )
               fprintf(stderr,"MM, with CP on finite cylinder. %d %g; %d %g\n",hm1, cps1, hm2, cps2);
             return 1;
           }

           // TEST: HM,HM
           if ( hm1==HM && hm2==HM ) {
             if ( I1B.b < 0 && I2B.b < 0 ) {
               if ( printinfo )
                 fprintf(stderr,"HM,HM: one below the other!\n");
               return 0;
             } else {
               if ( printinfo )
                 fprintf(stderr,"HM,HM: overlap.\n");
               return 1;
             }
           }
           // TEST: MH,MH
           if ( hm1==MH && hm2==MH ) {
             if ( I1B.b > C2.h && I2B.b > C1.h ) {
               if ( printinfo )
                 fprintf(stderr,"MH,MH: one below the other!\n");
               return 0;
             } else {
               if ( printinfo )
                 fprintf(stderr,"MH,MH: overlap.\n");
               return 1;
             }
           }

           // TEST: HM,MH
           if ( hm1==HM && hm2==MH ) {
             if ( I1B.a > C2.h ) {
               if ( printinfo )
                 fprintf(stderr,"HM,MH: I1B.a > C2.h\n");
               return 0;
             } else {
               if ( printinfo )
                 fprintf(stderr,"HM,MH: I1B.a <= C2.h\n");
               return 1;
             }
           }
           // TEST: MH,HM
           if ( hm1==MH && hm2==HM ) {
             if ( I2B.a > C1.h ) {
               if ( printinfo )
                 fprintf(stderr,"MH,HM: I2B.a > C1.h\n");
               return 0;
             } else {
               if ( printinfo )
                 fprintf(stderr,"MH,HM: I2B.a <= C1.h\n");
               return 1;
             }
           }

           // Old code from here
           if ( printinfo )
             fprintf(stderr,"RUNNING OLD CODE\n");

           // TEST: CpMM, CpMH, HMCp, MMCp
           if ( (cps1 < 0 && ! I1B.hitmiss) ||
               (cps1 > C1.h && ! I1T.hitmiss) ||
               (cps2 < 0 && ! I2B.hitmiss) ||
               (cps2 > C2.h && ! I2T.hitmiss) ) {
             if ( printinfo )
               fprintf(stderr,"One cylinder to side of infinite cyl intersection.\n");
             return 0;
           }

           // TEST: MCpM|HM or MCpM|MH
           // Another case where both contain the double root
           if ( (! I1B.hitmiss && ! I1T.hitmiss &&
                 cps1 >= 0 && cps1 <= C1C.h &&
                 (I2B.hitmiss || I2T.hitmiss)) ||
               (! I2B.hitmiss && ! I2T.hitmiss &&
                cps2 >= 0 && cps2 <= C2C.h &&
                (I1B.hitmiss || I1T.hitmiss))
              ) {
             if ( printinfo )
               fprintf(stderr,"Found MCPM and HM\n");
             return 1;
           }

           // TEST: CpHM|MHCp or MHCp|CpHM
           // These are only hits if the H side of one cylinder
           // overlaps the finite interval of the other cylinder
           // If (for example) C1's bottom intersects the infinite C2
           // cylinder, then we only have an intersection between
           // cylinders if the range of this intersection overlaps
           // the finite C2 cylinder
           if ( (I1T.hitmiss && I2B.hitmiss) ) {
             return IntervalsOverlap(I2B.a,I2B.b, 0., C1.h);
           } else if ( (I1B.hitmiss && I2T.hitmiss) ) {
             return IntervalsOverlap(I2T.a,I2T.b, 0., C1.h);
           }
           if ( printinfo )
             fprintf(stderr,"CylInterect: WE SHOULDN'T BE HERE!!!");
  }

  if ( printinfo )
    fprintf(stderr,"Algorithm not completed yet (!!! IT SHOULD BE COMPLETE!!!).\n");
  return 0;
}

void AssignColors(UnionFindNode UF[], int nc) {
  int i; int c=0;
  for (i=0; i<nc; i++) {
    if ( UF[i].parent == &(UF[i]) ) {
      UF[i].color = c;
      c++;
    }
  }
}

Scalar PointOnCylCoord(Point P, Cylinder C) {
  Frame CylF;
  Scalar c0,c1,c2;

  CylF = CylFrame(C, "PointOnCylCoord Frame");
  PCoords(P,CylF,&c0,&c1,&c2);
  return c2;
}
