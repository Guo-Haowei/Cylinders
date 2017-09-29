#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "geometry.h"

#include "cylinder.h"
#include "unionfind.h"

#include "polyroots.h"

extern Space W3;
extern Space W2;
extern Frame F3;
extern Frame F2;

#define SQ(A) ((A)*(A))

// Assume infinite spheres intersect
// Return resulting points in iP; maxi is the max number of points; the
// return value is the actual number of points
void MillerIntersectInfCyl(Cylinder C1, Cylinder C2, int maxi, Point iP1[], Point iP2[], int* np1, int* np2) {
	double c[3];

	// create a frame on base of cylinder
	Scalar c0,c1,c2;
	Vector v1,v2;
	VCoords(C2.v,F3,&c0,&c1,&c2);
	if ( fabs(c0) <= fabs(c1) && fabs(c0) <= fabs(c2) ) {
		v1 = VNormalize(VVCross(C2.v,FV(F3,0)));
	} else if ( fabs(c1) <= fabs(c0) && fabs(c1) <= fabs(c2) ) {
		v1 = VNormalize(VVCross(C2.v,FV(F3,1)));
	} else {
		v1 = VNormalize(VVCross(C2.v,FV(F3,2)));
	}
	v2 = VNormalize(VVCross(C2.v,v1));

	// c[2] is constant
	c[2] = 1.-SQ(VVDot(C1.v,C2.v));
	if ( fabs(c[2])<1e-4 ) {
		fprintf(stderr,"MillerIntersectInfCyl: axes are parallel.  Exiting.\n");
		exit(1);
	}

	int i;
	int n1=0,n2=0;
	int nsamples=200;
	for (i=0; i<nsamples; i++) {
		double t = ((double)i)/((double)nsamples);
		Point gammaP=PVAdd(C2.P, VVAdd(SVMult(C2.r*cos(2.*M_PI*t),v1),
					       SVMult(C2.r*sin(2.*M_PI*t),v2)));
		//		printf("# a gammaP ball\n");
		//		S3dBall(gammaP,0.05);
		c[1] = 2*VVDot(VVDiff(C2.v,SVMult(VVDot(C2.v,C1.v),C1.v)),
			       PPDiff(gammaP,C1.P));
		c[0] = VVDot(PPDiff(gammaP,C1.P),PPDiff(gammaP,C1.P))-
			SQ(VVDot(PPDiff(gammaP,C1.P),C1.v))-
			C1.r*C1.r;
		double roots[2]; int nr;
		nr = quadraticRoots(c[2],c[1],c[0],roots);
		//		fprintf(stderr,"%g x^2  +  %g x  + %g\n", c[2],c[2],c[0]);
		//		if ( nr > 0 ) fprintf(stderr,"Miller87: %d roots (%g %g)\n",nr,roots[0],roots[1]);
		if ( nr >= 1 ) {
			iP1[n1] = PVAdd(gammaP,SVMult(roots[0],C2.v));
			n1++;
		}
		if ( nr == 2 ) {
			iP2[n2] = PVAdd(gammaP,SVMult(roots[1],C2.v));
			n2++;
		}
	}
	*np1 = n1;
	*np2 = n2;
}
