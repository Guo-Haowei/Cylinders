#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "geometry.h"

#include "cylinder.h"

extern Frame F3;
extern Frame F2;

void S3dTriangle(Point p0, Point p1, Point p2,
		 Normal n0, Normal n1, Normal n2) {
	Scalar c00,c01,c02;
	Scalar c10,c11,c12;
	Scalar c20,c21,c22;

	Scalar n00,n01,n02;
	Scalar n10,n11,n12;
	Scalar n20,n21,n22;

	PCoords(p0,F3, &c00,&c01,&c02);
	PCoords(p1,F3, &c10,&c11,&c12);
	PCoords(p2,F3, &c20,&c21,&c22);

	NCoords(n0,F3, &n00,&n01,&n02);
	NCoords(n1,F3, &n10,&n11,&n12);
	NCoords(n2,F3, &n20,&n21,&n22);

	printf("P 3 0 0\n");
	printf("n %g %g %g\n",n00,n01,n02);
	printf("v %g %g %g\n",c00,c01,c02);
	printf("n %g %g %g\n",n10,n11,n12);
	printf("v %g %g %g\n",c10,c11,c12);
	printf("n %g %g %g\n",n20,n21,n22);
	printf("v %g %g %g\n",c20,c21,c22);
	printf("E 0 0 0\n");
}

void S3dCircle(Point P, Vector v, Scalar r) {
	Scalar c0,c1,c2;
	Vector v1,v2;
	Normal n;

	VCoords(v,F3, &c0,&c1,&c2);
	//	fprintf(stderr,"%g %g %g\n",c0,c1,c2);
	if ( fabs(c0) <= fabs(c1) && fabs(c0) <= fabs(c2) ) {
		v1 = FV(F3,0);
	} else if ( fabs(c1) <= fabs(c0) && fabs(c1) <= fabs(c2) ) {
		v1 = FV(F3,1);
	} else {
		v1 = FV(F3,2);
	}
	//	VPrintf(stderr,v);
	//	VPrintf(stderr,v1);
	v2 = VNormalize(VVCross(v,v1));
	v1 = SVMult(r,v1);
	v2 = SVMult(r,v2);

	const int density=60;
	const float fdensity=density;
	int i,j;
	n = VDual(v);
	printf("L %d 2 0\n",density+1);
	for (i=0; i<=density; i++) {
		Point p1,p2;
		p1 = PVAdd(P,VVAdd(
			   SVMult(cos(((float)i)/fdensity*M_PI*2.),v1),
			   SVMult(sin(((float)i)/fdensity*M_PI*2.),v2)));
		PCoords(p1,F3, &c0,&c1,&c2);
		printf("v %g %g %g\n",c0,c1,c2);
	}
	printf("E 0 0 0\n");
}

static Vector va[10][10];
static int vaInit=0;

void S3dBall(Point P, Scalar r) {
	Scalar c0,c1,c2;
	PCoords(P,F3, &c0,&c1,&c2);
	printf("M 6 8 0\n");
	printf("v %g %g %g # 0\n",r+c0,c1,c2);
	printf("v %g %g %g # 1\n",c0,r+c1,c2);
	printf("v %g %g %g # 2\n",c0,c1,r+c2);
	printf("v %g %g %g # 3\n",c0-r,c1,c2);
	printf("v %g %g %g # 4\n",c0,c1-r,c2);
	printf("v %g %g %g # 5\n",c0,c1,c2-r);
	printf("f 2 0 1 # 0\n\
f 2 1 3 # 1\n\
f 2 3 4 # 2\n\
f 2 4 0 # 3\n\
f 5 1 0 # 4\n\
f 5 3 1 # 5\n\
f 5 4 3 # 6\n\
f 5 0 4 # 7\n\
E 0 0 0\n");

}

void S3dDisk(Point P, Vector v, Scalar r) {
	Scalar c0,c1,c2;
	Vector v1,v2;
	Normal n;

	VCoords(v,F3, &c0,&c1,&c2);
	//	fprintf(stderr,"%g %g %g\n",c0,c1,c2);
	if ( fabs(c0) <= fabs(c1) && fabs(c0) <= fabs(c2) ) {
		v1 = FV(F3,0);
	} else if ( fabs(c1) <= fabs(c0) && fabs(c1) <= fabs(c2) ) {
		v1 = FV(F3,1);
	} else {
		v1 = FV(F3,2);
	}
	//	VPrintf(stderr,v);
	//	VPrintf(stderr,v1);
	v2 = VNormalize(VVCross(v,v1));
	v1 = VVCross(v,v2);
	v1 = SVMult(r,v1);
	v2 = SVMult(r,v2);

	const int density=60;
	const float fdensity=density;
	int i,j;
	n = VDual(v);
	for (i=0; i<density; i++) {
		Point p1,p2;
		p1 = PVAdd(P,VVAdd(
			   SVMult(cos(((float)i)/fdensity*M_PI*2.),v1),
			   SVMult(sin(((float)i)/fdensity*M_PI*2.),v2)));
		p2 = PVAdd(P,VVAdd(
			   SVMult(cos(((float)((i+1)%density))/fdensity*M_PI*2.),v1),
			   SVMult(sin(((float)((i+1)%density))/fdensity*M_PI*2.),v2)));
		S3dTriangle(P,p1,p2,n,n,n);
	}
}

void S3dCylinder(Cylinder C, int topbottom) {
	Scalar c0,c1,c2;
	Vector v1,v2;
	Normal n;

	VCoords(C.v,F3, &c0,&c1,&c2);
	//	fprintf(stderr,"%g %g %g\n",c0,c1,c2);
	if ( fabs(c0) <= fabs(c1) && fabs(c0) <= fabs(c2) ) {
		v1 = FV(F3,0);
	} else if ( fabs(c1) <= fabs(c0) && fabs(c1) <= fabs(c2) ) {
		v1 = FV(F3,1);
	} else {
		v1 = FV(F3,2);
	}
	//	VPrintf(stderr,C.v);
	//	VPrintf(stderr,v1);
	v2 = VNormalize(VVCross(C.v,v1));
	v1 = VVCross(C.v,v2);
	v1 = SVMult(C.r,v1);
	v2 = SVMult(C.r,v2);

	const int density=60;
	const float fdensity=density;
	int i,j;
	if ( topbottom ) {
		printf("# bottom\n");
		S3dDisk(C.P,SVMult(-1.,C.v),C.r);
		printf("# top\n");
		S3dDisk(PVAdd(C.P,SVMult(C.h,C.v)),C.v,C.r);
	}
	printf("# body\n");
	for (i=0; i<density; i++) {
		Point p1,p2,p3,p4;
		Normal n1,n2,n3,n4;
		p1 = PVAdd(C.P,VVAdd(
			   SVMult(cos(((float)i)/fdensity*M_PI*2.),v1),
			   SVMult(sin(((float)i)/fdensity*M_PI*2.),v2)));
		p2 = PVAdd(C.P,VVAdd(
			   SVMult(cos(((float)((i+1)%density))/fdensity*M_PI*2.),v1),
			   SVMult(sin(((float)((i+1)%density))/fdensity*M_PI*2.),v2)));
		p3 = PVAdd(PVAdd(C.P,SVMult(C.h,C.v)),
			   VVAdd(
			   SVMult(cos(((float)i)/fdensity*M_PI*2.),v1),
			   SVMult(sin(((float)i)/fdensity*M_PI*2.),v2)));
		p4 = PVAdd(PVAdd(C.P,SVMult(C.h,C.v)),
			   VVAdd(
			   SVMult(cos(((float)((i+1)%density))/fdensity*M_PI*2.),v1),
			   SVMult(sin(((float)((i+1)%density))/fdensity*M_PI*2.),v2)));
		n1 = VDual(VNormalize(PPDiff(p1,C.P)));
		n2 = VDual(VNormalize(PPDiff(p2,C.P)));
		n3 = VDual(VNormalize(PPDiff(p3,(PVAdd(C.P,SVMult(C.h,C.v))))));
		n4 = VDual(VNormalize(PPDiff(p4,(PVAdd(C.P,SVMult(C.h,C.v))))));

		S3dTriangle(p1,p2,p4,n1,n2,n4);
		S3dTriangle(p4,p3,p1,n4,n3,n1);
	}
}

void S3dInfiniteCylinder(Cylinder C) {
	C.P = PVAdd(C.P,SVMult(-15.,C.v));
	C.h += 20;
	C.r *= .999;
	S3dCylinder(C,0);
}

double colors[14][3]={
	1,0,0,
	0,1,0,
	0,0,1,
	1,1,0,
	1,0,1,
	0,1,1,
	.1,.1,.1,
	1,1,1,
	1,.5,.5,
	.5,1,.5,
	.5,.5,1,
	1,1,0.5,
	1,0.5,1,
	0.5,1,1,
};
void S3dColor(int color) {
	fprintf(stderr,"S3dColor(%d)\n",color);
	printf("d %g %g %g\n",colors[color%11][0],colors[color%11][1],colors[color%11][2]);
}
