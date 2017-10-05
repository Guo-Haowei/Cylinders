/*
 ** geometry.h: Header file for the affine geometry package.
 **
 ** Copyright (c) 1989, Graphics and AI Laboratory, University of Washington
 ** Copying, use and development for non-commercial purposes permitted.
 **                  All rights for commercial use reserved.
 **		    This software is unsupported.
 **
 ** Converted to ANSI C and the routines NPPac, NPPvc, NVVlc, SVVCross
 **  were added by Stephen Mann, April 1996.
 **
 ** $Header: /p/SurfaceFitting/geometry.ansi/RCS/geometry.h,v 1.1 1996/04/05 15:25:57 smann Exp smann $
 **
 */
#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "matrix.h"
#include "math.h"

/* Constants */
#define TWOSPACE	2
#define THREESPACE	3

/* Access macros */
#define SpaceOf(obj)	(obj.s)	        /* Pointer to containing space */
#define Name(S)		(S->name)	/* The name of the space.  */
#define Dim(S)		(S->dim)	/* The dimension of an affine space */
#define StdFrame(S)	(S->stdf)	/* Standard frame for a space */

/* Macros for Derived operations */
#define VMag(v)			sqrt(VVDot(v,v))
#define PPDist(p1,p2)		VMag(PPDiff(p2,p1))

typedef struct frame {			/* An affine frame.                */
  struct euclideanSpace *s;	/* The containing space.           */
  char *name;			/* Printable name for debugging.   */
  Matrix tostdf;			/* Rep of frame rel to s->stdf.    */
  Matrix fromstdf;		/* Rep of s->stdf rel to frame.    */
} Frame;

typedef struct euclideanSpace {		/* A Euclidean space.            */
  int dim;			/* The dimension of the space    */
  char *name;			/* Printable name for debugging  */
  Frame stdf;			/* A predefined Cartesian frame. */
} *Space;

typedef struct {			/* A point.                      */
  Space s;			/* The containing space.         */
  Matrix p;			/* Coords relative to s->stdf    */
} Point;

typedef struct {			/* A vector.                     */
  Space s;                        /* The containing space.         */
  Matrix v;			/* Coords relative to s->stdf    */
} Vector;

typedef struct {			/* A normal vector.              */
  Space s;         		/* The containing space.         */
  Matrix n;			/* Coords relative to s->stdf    */
} Normal;

typedef struct {			/* An affine transformation.     */
  char invertible;                /* TRUE if an invertible xform.  */
  Space range, domain;
  Matrix t, invt;			/* For efficiency, both the  */
  /* transform and its inverse */
  /* are stored.               */
} AffineMap;

/* Imported routines */
extern double sqrt(double);

/* Creation routines */
extern Space       SCreate(char*, int);
extern Frame       FCreate(char *name,Point origin, Vector, Vector, ...);
extern Frame       FCreate3(char *name,Point origin, Vector, Vector, Vector);
extern Point       PCreate(Frame F,  Scalar, Scalar, ...);
extern Point       PCreate3(Frame F,  Scalar, Scalar, Scalar);
extern Vector      VCreate(Frame F,  Scalar, Scalar, ...);
extern Vector      VCreate3(Frame F,  Scalar, Scalar, Scalar);
extern Vector      VZero(Space S);
extern Normal      NCreate( Frame f,  Scalar, Scalar, ...);
extern AffineMap   ACreate(Frame F,Point Oprime, Vector v0prime,
    Vector v1prime, ...);
extern AffineMap   ACreate3(Frame F,Point Oprime, Vector v0prime, Vector v2prime);
extern AffineMap   ACreateF(Frame F1, Frame F2);
extern AffineMap   AIdentity(Space S);

/* Routines that return coordinates */
extern void        PCoords(Point p, Frame F, Scalar*, Scalar*, ...);
extern void        VCoords(Vector v, Frame F, Scalar*, Scalar*, ...);
extern void        NCoords(Normal n, Frame F, Scalar*, Scalar*, ...);

/* Functions for accessing frames */
extern Point       FOrg(Frame F);
extern Vector	   FV(Frame F, int i);

/* Functions for combining points and vectors */
extern Point       PVAdd(Point p, Vector v);
extern Vector      PPDiff(Point p, Point q);
extern Point	   PPrr(Point p1, Point p2, Scalar r1, Scalar r2);
extern Point	   PPac(Point p1, Point p2, Scalar a1);
extern Point	   PPac3(Point p1, Point p2, Point p3,
    Scalar a1, Scalar a2, Scalar a3);
extern Point	   PPacN(int n, Point p[], Scalar a[]);
extern Point	   NPPac(int n, Point, Scalar, ...);
extern Vector      PPvcN(int n, Point p[], Scalar a[]);
extern Vector	   NPPvc(int n, Point, Scalar, ...);
extern Normal      PPPNormal(Point p1, Point p2, Point p3);

/* Primitive vector space operations */
extern Vector      SVMult(Scalar scalar, Vector vector);
extern Vector      VVAdd(Vector v1, Vector v2);
extern Vector      VVDiff(Vector v1, Vector v2);
extern Scalar      VVDot(Vector v1, Vector v2);
extern Vector	   VVCross(Vector v1, Vector v2);
extern Scalar      SVVCross(Vector v1, Vector v2);
extern Vector      VVProj(Vector v, Vector w);
extern Normal      VDual(Vector v);
extern Vector      NDual(Normal phi);
extern Scalar      NVApply(Normal phi, Vector v);
extern Vector      VVlcN(int n, Vector v[], Scalar a[]);
extern Vector	   NVVlc(int n, Vector, Scalar, ...);

/* Derived operations */
extern Vector	   VNormalize(Vector v);

/* Transformations */
extern Point       PAxform(Point p, AffineMap T);
extern Vector      VAxform(Vector v, AffineMap T);
extern Normal      NAxform(Normal n, AffineMap T);
extern AffineMap   AACompose(AffineMap T1, AffineMap T2);

/* Bad print functions, but useful for debugging */
void PPrintf( FILE *fp, Point p);
void VPrintf( FILE *fp, Vector v);
void APrintf( FILE *fp, AffineMap T);
void F_Printf( FILE *fp, Frame F);
#endif
