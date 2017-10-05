/*
 ** geometry.c:
 **    Routines to implement 2 and 3 dimensional affine geometry.
 **    This package exports the coordinate-free abstractions of affine
 **    spaces, coordinate frames, points, vectors, normal vectors, and
 **    affine transformations.  Implementation details can be found in
 **    Tony D. DeRose, "A Coordinate-Free Approach to Geometric Programming,"
 **    Siggraph '89 Tutorial Notes, Courses #14 and #23, August 1989.
 **
 ** Converted to ANSI C and the routines NPPac, NPPvc, NVVlc, SVVCross
 **  were added by Stephen Mann, April 1996.
 **
 ** Copyright (c) 1989, Graphics and AI Laboratory, University of Washington
 ** Copying, use and development for non-commercial purposes permitted.
 **                  All rights for commercial use reserved.
 **		    This software is unsupported.
 **
 ** $Header: /p/SurfaceFitting/geometry.ansi/RCS/geometry.c,v 1.1 1996/04/05 15:25:57 smann Exp smann $
 **
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "geometry.h"

/* Convenient macros */
/* Fill in a row of a matrix */
#define SetRow(mat,row,rowvec)	{ME(mat,row,0) = ME(rowvec,0,0); \
  ME(mat,row,1) = ME(rowvec,0,1); \
  ME(mat,row,2) = ME(rowvec,0,2); \
  ME(mat,row,3) = ME(rowvec,0,3);}

#define GetRow(rowvec,mat,row)  {ME(rowvec,0,0) = ME(mat,row,0); \
  ME(rowvec,0,1) = ME(mat,row,1); \
  ME(rowvec,0,2) = ME(mat,row,2); \
  ME(rowvec,0,3) = ME(mat,row,3);}

/* Imports */
extern double fabs(double);

/* Forward declarations */
extern int DefAffineError(const char*);


/* Local Variables */
static int (*AffineError)(const char*) = DefAffineError;    /* Error handler */


/*--------------------------------------------------------------*/
/*			Error Handling 				*/
/*--------------------------------------------------------------*/

/*
 ** Set the affine error handling routine.
 */
void SetAffineError( int (*ErrorFunc)(const char*))
{
  AffineError = ErrorFunc;
}

/*
 ** Default error handling routine.
 */
int DefAffineError( const char *errmesg )
{
  fprintf(stderr, "Affine error: %s...dumping core.\n", errmesg);
  fflush(stderr);
  abort();
}


/*--------------------------------------------------------------*/
/*			Predicates				*/
/*--------------------------------------------------------------*/


/*
 ** Return true if p really is a point.
 */
int IsPoint(Point p)
{
  if (Dim(SpaceOf(p)) == 3){
    if ( MatrixRows(p.p) == 1  &&  MatrixColumns(p.p) == 4  &&
        fabs(ME(p.p, 0,3)-1.)<EPSILON ){
      return 1;
    } else {
      return 0;
    }
  } else {
    if ( MatrixRows(p.p) == 1  &&  MatrixColumns(p.p) == 3  &&
        fabs(ME(p.p, 0,2)-1.)<EPSILON ){
      return 1;
    } else {
      return 0;
    }
  }
}


/*
 ** Return true if v really is a vector.
 */
int IsVector(Vector v)
{
  if (Dim(SpaceOf(v)) == 3){
    if ( MatrixRows(v.v) == 1  &&  MatrixColumns(v.v) == 4  &&
        fabs(ME(v.v, 0,3))<EPSILON ){
      return 1;
    } else {
      return 0;
    }
  } else {
    if ( MatrixRows(v.v) == 1  &&  MatrixColumns(v.v) == 3  &&
        fabs(ME(v.v, 0,2))<EPSILON ){
      return 1;
    } else {
      return 0;
    }
  }
}


/*
 ** Return true if n really is a normal.
 */
int IsNormal(Normal n)
{
  if (Dim(SpaceOf(n)) == 3){
    if ( MatrixRows(n.n) == 4  &&  MatrixColumns(n.n) == 1  &&
        fabs(ME(n.n, 3,0))<EPSILON ){
      return 1;
    } else {
      return 0;
    }
  } else {
    if ( MatrixRows(n.n) == 3  &&  MatrixColumns(n.n) == 1  &&
        fabs(ME(n.n, 2,0))<EPSILON ){
      return 1;
    } else {
      return 0;
    }
  }
}



/*--------------------------------------------------------------*/
/*			Creation Routines			*/
/*--------------------------------------------------------------*/

/*
 ** Create a new affine space of the specified dimension.
 */
Space SCreate(char *name, int dim)
{
  Space NewSpace;

  NewSpace = (Space) malloc(sizeof(struct euclideanSpace));

  NewSpace->dim = dim;
  NewSpace->name = name;
  NewSpace->stdf.s = NewSpace;
  NewSpace->stdf.tostdf = MatrixIdentity( dim+1);
  NewSpace->stdf.fromstdf = MatrixIdentity( dim+1);

  return NewSpace;
}


/*
 ** Define a new coordinate frame.
 */

Frame FCreate(char *name,Point origin, Vector v0, Vector v1, ...)
{
  Frame NewFrame;
  Vector v2;
  int isValid = TRUE;

#ifdef DEBUG
  PPrintf( stderr, origin);
  VPrintf( stderr, v0);
  VPrintf( stderr, v1);
#endif

  if ( !IsPoint(origin) ){
    (*AffineError)("FCreate: origin not a Point.");
  }
  if ( !IsVector(v0) ){
    (*AffineError)("FCreate: v0 not a Vector.");
  }
  if ( !IsVector(v1) ){
    (*AffineError)("FCreate: v1 not a Vector.");
  }

  isValid = (SpaceOf(origin) == SpaceOf(v0)) && (SpaceOf(origin) == SpaceOf(v1));
  if (Dim(SpaceOf(origin)) == 3) {
    va_list ap;

    va_start(ap, v1);
    v2 = va_arg(ap, Vector);
    va_end(ap);

#ifdef DEBUG
    if (Dim(SpaceOf(origin)) == 3) VPrintf( stderr, v2);
#endif
    isValid = isValid && (SpaceOf(origin) == SpaceOf(v2));
    if ( !IsVector(v2) ){
      (*AffineError)("FCreate: v2 not a Vector.");
    }
  }
  if (isValid) {

    SpaceOf(NewFrame) = SpaceOf(origin);
    NewFrame.name = name;

    switch (Dim(SpaceOf(NewFrame))) {
      case 2:
        /* Create a frame for an affine 2-space */
        NewFrame.tostdf = MatrixZero( 3);

        /* The ith row of tostdf consists stdcoords of vi */
        SetRow(NewFrame.tostdf, 0, v0.v);
        SetRow(NewFrame.tostdf, 1, v1.v);

        /* The last row consists of stdcoords of origin */
        SetRow(NewFrame.tostdf, 2, origin.p);
        break;
      case 3:
        /* Create a frame for an affine 3-space */
        NewFrame.tostdf = MatrixZero( 4);

        /* The ith row of tostdf consists stdcoords of vi */
        SetRow(NewFrame.tostdf, 0, v0.v);
        SetRow(NewFrame.tostdf, 1, v1.v);
        SetRow(NewFrame.tostdf, 2, v2.v);

        /* The last row consists of stdcoords of origin */
        SetRow(NewFrame.tostdf, 3, origin.p);
        break;
    }

    /* Check to make sure a valid frame has been specified */
    if (fabs(MatrixDet( &(NewFrame.tostdf))) < EPSILON) {
#ifdef DEBUG
      MatrixPrint( NewFrame.tostdf, stderr);
#endif
      (*AffineError)("FCreate: Vectors not linearly independent.");
    } else {
      /* A valid frame, so compute fromstdf */
      NewFrame.fromstdf = MatrixInverse( &(NewFrame.tostdf));
    }

  } else {
    (*AffineError)("FCreate: elements not from same space.");
  }
  return NewFrame;
}

Frame FCreate3(char *name,Point origin, Vector v0, Vector v1, Vector v2)
{
  Frame NewFrame;
  int isValid = TRUE;

#ifdef DEBUG
  PPrintf( stderr, origin);
  VPrintf( stderr, v0);
  VPrintf( stderr, v1);
#endif

  if ( !IsPoint(origin) ){
    (*AffineError)("FCreate3: origin not a Point.");
  }
  if ( !IsVector(v0) ){
    (*AffineError)("FCreate3: v0 not a Vector.");
  }
  if ( !IsVector(v1) ){
    (*AffineError)("FCreate3: v1 not a Vector.");
  }
  if ( !IsVector(v2) ){
    (*AffineError)("FCreate3: v2 not a Vector.");
  }

  isValid = (SpaceOf(origin) == SpaceOf(v0)) && (SpaceOf(origin) == SpaceOf(v1));
  if (isValid) {

    SpaceOf(NewFrame) = SpaceOf(origin);
    NewFrame.name = name;

    switch (Dim(SpaceOf(NewFrame))) {
      case 3:
        /* Create a frame for an affine 3-space */
        NewFrame.tostdf = MatrixZero( 4);

        /* The ith row of tostdf consists stdcoords of vi */
        SetRow(NewFrame.tostdf, 0, v0.v);
        SetRow(NewFrame.tostdf, 1, v1.v);
        SetRow(NewFrame.tostdf, 2, v2.v);

        /* The last row consists of stdcoords of origin */
        SetRow(NewFrame.tostdf, 3, origin.p);
        break;
     default:
        (*AffineError)("FCreate3: Space not 3 dimensional.");
        break;
    }

    /* Check to make sure a valid frame has been specified */
    if (fabs(MatrixDet( &(NewFrame.tostdf))) < EPSILON) {
#ifdef DEBUG
      MatrixPrint( NewFrame.tostdf, stderr);
#endif
      (*AffineError)("FCreate3: Vectors not linearly independent.");
    } else {
      /* A valid frame, so compute fromstdf */
      NewFrame.fromstdf = MatrixInverse( &(NewFrame.tostdf));
    }

  } else {
    (*AffineError)("FCreate3: elements not from same space.");
  }
  return NewFrame;
}

/*
 ** Create and return a new point.  The coordinates of the
 ** point are (c0,c1,[c2,]1) relative to the frame F.  The
 ** coordinate c2 is only used if F spans a 3-space.
 */
Point PCreate( Frame F,  Scalar c0, Scalar c1, ... )
{
  Point NewPoint;
  Scalar c2;

  if ( Dim(SpaceOf(F)) == 3 ) {
    va_list ap;
    va_start(ap, c1);
    c2 = va_arg(ap, Scalar);
    va_end(ap);
  }

  SpaceOf(NewPoint) = SpaceOf(F);
  switch (Dim(SpaceOf(NewPoint))) {
    case 2:
      NewPoint.p = MatrixCreate( 1, 3);

      /* Compute standard coords */
      ME(NewPoint.p, 0, 0) = c0;
      ME(NewPoint.p, 0, 1) = c1;
      ME(NewPoint.p, 0, 2) = 1.0;
      break;
    case 3:
      NewPoint.p = MatrixCreate( 1, 4);

      /* Compute standard coords */
      ME(NewPoint.p, 0, 0) = c0;
      ME(NewPoint.p, 0, 1) = c1;
      ME(NewPoint.p, 0, 2) = c2;
      ME(NewPoint.p, 0, 3) = 1.0;
      break;
  }

  NewPoint.p = MatrixMult( &(NewPoint.p), &(F.tostdf));
  return NewPoint;
}

Point PCreate3(Frame F, Scalar c0, Scalar c1, Scalar c2)
{
  Point NewPoint;

  SpaceOf(NewPoint) = SpaceOf(F);
  if (Dim(SpaceOf(NewPoint))!=3) {
    (*AffineError)("PCreate3: Space not 3-dimensional.");
  }
  NewPoint.p = MatrixCreate( 1, 4);

  /* Compute standard coords */
  ME(NewPoint.p, 0, 0) = c0;
  ME(NewPoint.p, 0, 1) = c1;
  ME(NewPoint.p, 0, 2) = c2;
  ME(NewPoint.p, 0, 3) = 1.0;

  NewPoint.p = MatrixMult( &(NewPoint.p), &(F.tostdf));
  return NewPoint;
}


/*
 ** Create and return a new vector.  The coordinates of the
 ** vector are (c0,c1,[c2,]1) relative to the frame F.  The
 ** coordinate c2 is only used if F spans a 3-space.
 */
Vector VCreate(Frame F, Scalar c0, Scalar c1, ...)
{
  Vector NewVector;
  Scalar c2;

  if ( Dim(SpaceOf(F)) == 3 ) {
    va_list ap;
    va_start(ap, c1);
    c2 = va_arg(ap, Scalar);
    va_end(ap);
  }

  SpaceOf(NewVector) = SpaceOf(F);
  switch (Dim(SpaceOf(NewVector))) {
    case 2:
      NewVector.v = MatrixCreate( 1, 3);

      /* Compute standard coords */
      ME(NewVector.v, 0, 0) = c0;
      ME(NewVector.v, 0, 1) = c1;
      ME(NewVector.v, 0, 2) = 0.0;
      break;
    case 3:
      NewVector.v = MatrixCreate( 1, 4);

      /* Compute standard coords */
      ME(NewVector.v, 0, 0) = c0;
      ME(NewVector.v, 0, 1) = c1;
      ME(NewVector.v, 0, 2) = c2;
      ME(NewVector.v, 0, 3) = 0.0;
      break;
  }

  NewVector.v = MatrixMult( &(NewVector.v), &(F.tostdf));
  return NewVector;
}

Vector VCreate3(Frame F, Scalar c0, Scalar c1, Scalar c2)
{
  Vector NewVector;

  SpaceOf(NewVector) = SpaceOf(F);
  if (Dim(SpaceOf(NewVector))!=3) {
    (*AffineError)("VCreate3: Space not 3-dimensional.");
  }
  NewVector.v = MatrixCreate( 1, 4);

  /* Compute standard coords */
  ME(NewVector.v, 0, 0) = c0;
  ME(NewVector.v, 0, 1) = c1;
  ME(NewVector.v, 0, 2) = c2;
  ME(NewVector.v, 0, 3) = 0.0;

  NewVector.v = MatrixMult( &(NewVector.v), &(F.tostdf));
  return NewVector;
}

/*
 ** Create and return a new normal vector.
 */
Normal NCreate( Frame f, Scalar c0, Scalar c1, ... )
{
  Scalar c2;

  if ( Dim(SpaceOf(f)) == 3 ) {
    va_list ap;
    va_start(ap, c1);
    c2 = va_arg(ap, Scalar);
    va_end(ap);
  }
  return VDual( VCreate( f, c0, c1, c2));
}

/*
 ** Return the zero vector in space S.
 */
Vector VZero(Space S)
{
  Vector ZeroVec;

  SpaceOf(ZeroVec) = S;
  ZeroVec.v = MatrixCreate(1, Dim(SpaceOf(ZeroVec)) + 1);

  return ZeroVec;
}

/*
 ** Return the transformation that carries the origin of F onto
 ** Oprime, and the basis in F onto v0prime, v1prime, [and v2prime.]
 */
AffineMap ACreate(Frame F,Point Oprime,
    Vector v0prime, Vector v1prime, ...)
{
  AffineMap T;			/* The returned transform */
  Matrix Tprime;			/* Rows built from primed objects */
  int isValid = 1;
  Vector v2prime;


  if ( !IsPoint(Oprime) ){
    (*AffineError)("ACreate: Oprime not a Point.");
  }
  if ( !IsVector(v0prime) ){
    (*AffineError)("ACreate: v0prime not a Vector.");
  }
  if ( !IsVector(v1prime) ){
    (*AffineError)("ACreate: v1prime not a Vector.");
  }

  isValid = (SpaceOf(Oprime) == SpaceOf(v0prime)) &&
    (SpaceOf(Oprime) == SpaceOf(v1prime));
  if (Dim(SpaceOf(F)) == 3) {
    va_list ap;
    va_start(ap, v1prime);
    v2prime = va_arg(ap,Vector);
    va_end(ap);
    if ( !IsVector(v2prime) ){
      (*AffineError)("ACreate: v2prime not a Vector.");
    }
    isValid = isValid && (SpaceOf(Oprime) == SpaceOf(v2prime));
  }
  if (isValid) {
    T.domain = SpaceOf(F);
    T.range  = SpaceOf(Oprime);

    /* Assume for now that the map isn't invertible */
    T.invertible = FALSE;

    /* Build Tprime */
    Tprime = MatrixCreate( Dim(SpaceOf(F))+1, Dim(SpaceOf(Oprime))+1);

    switch (Dim(SpaceOf(F))) {
      case 2:
        SetRow(Tprime, 0, v0prime.v);
        SetRow(Tprime, 1, v1prime.v);
        SetRow(Tprime, 2, Oprime.p);
        break;
      case 3:
        SetRow(Tprime, 0, v0prime.v);
        SetRow(Tprime, 1, v1prime.v);
        SetRow(Tprime, 2, v2prime.v);
        SetRow(Tprime, 3, Oprime.p);
        break;
    }

    /*-----------------------------------------*/
    /* T.t satisfies: F.tostdf * T.t = Tprime, */
    /* So T.t = F.fromstdf * Tprime.           */
    /*-----------------------------------------*/
    T.t = MatrixMult( &(F.fromstdf), &(Tprime));

    /*-------------------------------------------------------*/
    /* If T's domain and range have the same dimension, then */
    /* the inverse matrix may also be stored.                */
    /*-------------------------------------------------------*/
    if (Dim(SpaceOf(F)) == Dim(SpaceOf(Oprime))) {
      /* See if T.t is invertible */
      if (fabs(MatrixDet(&(T.t))) > EPSILON) {
        T.invt = MatrixInverse( &(T.t));
        T.invertible = TRUE;
      }
    }
  } else {
    (*AffineError)("ACreate: image objects not in common space.");
  }
#ifdef DEBUG
  fprintf( stderr, "ACreated...\n");
  TPrintf( stderr, T);
#endif
  return T;
}

AffineMap ACreate3(Frame F,Point Oprime,
    Vector v0prime, Vector v1prime, Vector v2prime)
{
  AffineMap T;			/* The returned transform */
  Matrix Tprime;			/* Rows built from primed objects */
  int isValid = 1;


  if ( !IsPoint(Oprime) ){
    (*AffineError)("ACreate3: Oprime not a Point.");
  }
  if ( !IsVector(v0prime) ){
    (*AffineError)("ACreate3: v0prime not a Vector.");
  }
  if ( !IsVector(v1prime) ){
    (*AffineError)("ACreate3: v1prime not a Vector.");
  }
  if ( !IsVector(v2prime) ){
    (*AffineError)("ACreate3: v2prime not a Vector.");
  }

  isValid = (SpaceOf(Oprime) == SpaceOf(v0prime)) &&
    (SpaceOf(Oprime) == SpaceOf(v1prime)) && 
    (SpaceOf(Oprime) == SpaceOf(v2prime));

  if (isValid) {
    T.domain = SpaceOf(F);
    T.range  = SpaceOf(Oprime);

    /* Assume for now that the map isn't invertible */
    T.invertible = FALSE;

    /* Build Tprime */
    Tprime = MatrixCreate( Dim(SpaceOf(F))+1, Dim(SpaceOf(Oprime))+1);

    switch (Dim(SpaceOf(F))) {
      case 3:
        SetRow(Tprime, 0, v0prime.v);
        SetRow(Tprime, 1, v1prime.v);
        SetRow(Tprime, 2, v2prime.v);
        SetRow(Tprime, 3, Oprime.p);
        break;
      default:
        (*AffineError)("ACreate3: Space not 3 dimensional.");
        break;
    }

    /*-----------------------------------------*/
    /* T.t satisfies: F.tostdf * T.t = Tprime, */
    /* So T.t = F.fromstdf * Tprime.           */
    /*-----------------------------------------*/
    T.t = MatrixMult( &(F.fromstdf), &(Tprime));

    /*-------------------------------------------------------*/
    /* If T's domain and range have the same dimension, then */
    /* the inverse matrix may also be stored.                */
    /*-------------------------------------------------------*/
    if (Dim(SpaceOf(F)) == Dim(SpaceOf(Oprime))) {
      /* See if T.t is invertible */
      if (fabs(MatrixDet(&(T.t))) > EPSILON) {
        T.invt = MatrixInverse( &(T.t));
        T.invertible = TRUE;
      }
    }
  } else {
    (*AffineError)("ACreate3: image objects not in common space.");
  }
#ifdef DEBUG
  fprintf( stderr, "ACreate3ed...\n");
  TPrintf( stderr, T);
#endif
  return T;
}


/*
 ** Return the affine transformation that carries the frame
 ** F1 onto the frame F2.  This could be done by calling ACreate,
 ** but the routine provided is a bit more efficient.
 */
AffineMap ACreateF(Frame F1, Frame F2)
{
  AffineMap T;

  if (Dim(SpaceOf(F1)) != Dim(SpaceOf(F2))) {
    (*AffineError)("ACreate: dim(F1) != dim(F2).");
  } else {
    T.domain = SpaceOf(F1);
    T.range = SpaceOf(F2);
    T.invertible = TRUE;
    T.t = MatrixMult( &(F1.fromstdf), &(F2.tostdf));
    T.invt = MatrixInverse(&(T.t));
  }
  return T;
}


/*
 ** Return the identity transformation from space S onto itself.
 */
AffineMap AIdentity(Space S)
{
  AffineMap Identity;

  Identity.domain = S;
  Identity.range = S;
  Identity.invertible = TRUE;

  Identity.t = MatrixIdentity( Dim(S) + 1);
  Identity.invt = Identity.t;
  return Identity;
}




/*--------------------------------------------------------------*/
/*			Coordinate Routines			*/
/*--------------------------------------------------------------*/


/*
 ** Return the coordinates of the point p relative to the frame F.
 */
void PCoords(Point p, Frame F, Scalar *c0, Scalar *c1, ...)
{
  Matrix Coords;
  Scalar *c2;

  if ( Dim(SpaceOf(F)) == 3 ) {
    va_list ap;

    va_start(ap, c1);
    c2 = va_arg(ap, Scalar*);
    va_end(ap);
  }

  if ( !IsPoint(p) ){
    (*AffineError)("PCoords: p not a Point.");
  }

  Coords = MatrixMult( &(p.p), &(F.fromstdf));
  *c0 = ME(Coords,0,0);
  *c1 = ME(Coords,0,1);

  if (Dim(SpaceOf(p)) == 3) {
    *c2 = ME(Coords,0,2);
  }
}


/*
 ** Return the coordinates of the vector v relative to the frame F.
 */
void VCoords(Vector v, Frame F, Scalar *c0, Scalar *c1, ...)
{
  Matrix Coords;
  Scalar *c2;

  if ( Dim(SpaceOf(F)) == 3 ) {
    va_list ap;

    va_start(ap, c1);
    c2 = va_arg(ap, Scalar*);
    va_end(ap);
  }
  /*va_end(ap);*/

  if ( !IsVector(v) ){
    (*AffineError)("VCoords: v not a Vector.");
  }

  Coords = MatrixMult( &(v.v), &(F.fromstdf));
  *c0 = ME(Coords,0,0);
  *c1 = ME(Coords,0,1);

  if (Dim(SpaceOf(v)) == 3) {
    *c2 = ME(Coords,0,2);
  }
}

/*
 ** Return the coordinates of the normal vector n relative to the frame F.
 */
void NCoords(Normal n, Frame F, Scalar *c0, Scalar *c1, ...)
{
  Matrix Coords;
  Scalar *c2;
  Matrix MT;

  if ( Dim(SpaceOf(F)) == 3 ) {
    va_list ap;

    va_start(ap, c1);
    c2 = va_arg(ap, Scalar*);
    va_end(ap);
  }

  if ( !IsNormal(n) ){
    (*AffineError)("NCoords: n not a Normal.");
  }

  MT = MatrixTranspose(&(n.n));
  Coords = MatrixMult( &MT, &(F.fromstdf));
  *c0 = ME(Coords,0,0);
  *c1 = ME(Coords,0,1);

  if (Dim(SpaceOf(n)) == 3) {
    *c2 = ME(Coords,0,2);
  }
}

/*
 ** Return the origin of the given frame.
 */
Point FOrg(Frame F)
{
  Point Origin;

  SpaceOf(Origin) = SpaceOf(F);
  Origin.p = MatrixCreate( 1, Dim(SpaceOf(Origin)) + 1);

  if (Dim(SpaceOf(Origin)) == 2) {
    GetRow(Origin.p, F.tostdf, 2);
  } else {
    GetRow(Origin.p, F.tostdf, 3);
  }

  return Origin;
}

/*
 ** Return the ith basis vector in F (numbered starting at 0).
 */
Vector FV(Frame F, int i)
{
  Vector Bi;

  SpaceOf(Bi) = SpaceOf(F);
  Bi.v = MatrixCreate( 1, Dim(SpaceOf(Bi)) + 1);
  GetRow(Bi.v, F.tostdf, i);

  return Bi;
}


/*--------------------------------------------------------------*/
/*			Affine Space Operators			*/
/*--------------------------------------------------------------*/

/*
 ** Return the vector difference of two points.  Returned
 ** is p-q, the vector from q to p.
 */
Vector PPDiff(Point p, Point q)
{
  Vector v;

  if ( !IsPoint(p) ){
    (*AffineError)("PPDiff: p not a Point.");
  }

  if ( !IsPoint(q) ){
    (*AffineError)("PPDiff: q not a Point.");
  }

  if (SpaceOf(p) != SpaceOf(q)) {
    (*AffineError)("PPDiff: points not in same space.");
  }
  SpaceOf(v) = SpaceOf(p);
  v.v = MatrixCreate( 1, Dim(SpaceOf(v))+1);
  ME(v.v,0,0) = ME(p.p,0,0) - ME(q.p,0,0);
  ME(v.v,0,1) = ME(p.p,0,1) - ME(q.p,0,1);
  if (Dim(SpaceOf(v)) == 3) {
    ME(v.v,0,2) = ME(p.p,0,2) - ME(q.p,0,2);
    ME(v.v,0,3) = 0.0;
  } else {
    ME(v.v,0,2) = 0.0;
  }

  return v;
}

/*
 ** Return the point q = p + v.
 */
Point PVAdd(Point p, Vector v)
{
  if ( !IsPoint(p) ){
    (*AffineError)("PVAdd: p not a Point.");
  }

  if ( !IsVector(v) ){
    (*AffineError)("PVAdd: v not a Vector.");
  }

  if (SpaceOf(p) != SpaceOf(v)) {
    (*AffineError)("PVAdd: point and vector from different spaces.");
  }
  ME(p.p,0,0) += ME(v.v,0,0);
  ME(p.p,0,1) += ME(v.v,0,1);
  if (Dim(SpaceOf(p)) == 3) {
    ME(p.p,0,2) += ME(v.v,0,2);
    ME(p.p,0,3) = 1.0;
  } else {
    ME(p.p,0,2) = 1.0;
  }
  return p;
}


/*
 ** Return the point that breaks line segment p1,p2 into
 ** relative ratios r1 and r2.
 */
Point PPrr(Point p1, Point p2, Scalar r1, Scalar r2)
{
  if ( !IsPoint(p1) ){
    (*AffineError)("PPrr: p1 not a Point.");
  }

  if ( !IsPoint(p2) ){
    (*AffineError)("PPrr: p2 not a Point.");
  }

  return PPac( p1, p2, r2/(r2+r1));
}

/*
 ** Return the point a1*p1 + (1-a1)*p2.
 **
 ** This point can also be written as p2 + a1*(p1-p2), which
 ** is how its actually calculated.
 */
Point PPac(Point p1, Point p2, Scalar a1)
{
  if ( !IsPoint(p1) ){
    (*AffineError)("PPac: p1 not a Point.");
  }

  if ( !IsPoint(p2) ){
    (*AffineError)("PPac: p2 not a Point.");
  }

  return PVAdd( p2, SVMult( a1, PPDiff( p1, p2)));
}


/*
 ** Return the point a1*p1 + a2*p2 + a3*p3, where
 ** a1 + a2 + a3 = 1.
 */
Point PPac3(Point p1, Point p2, Point p3, Scalar a1, Scalar a2, Scalar a3)
{
  Point p[3];
  Scalar a[3];

  if ( !IsPoint(p1) ){
    (*AffineError)("PPac3: p1 not a Point.");
  }

  if ( !IsPoint(p2) ){
    (*AffineError)("PPac3: p2 not a Point.");
  }

  if ( !IsPoint(p3) ){
    (*AffineError)("PPac3: p3 not a Point.");
  }

  p[0] = p1;  p[1] = p2;  p[2] = p3;
  a[0] = a1;  a[1] = a2;  a[2] = a3;
  return PPacN( 3, p, a);
}


/*
 ** Do an affine combination of n points. Returned is the point
 **
 **       sum_{i=0}^{n-1} a[i] * p[i]
 **
 ** where the a's sum to one.
 */
Point PPacN(int n, Point p[], Scalar a[])
{
  Scalar x, y, z, sum;
  Space S = SpaceOf(p[0]);
  int i, dim = Dim(SpaceOf(p[0]));

  /* For efficiency, appeal to coordinates */
  x = 0.0;  y = 0.0;  z = 0.0;  sum = 0.0;
  for (i = 0; i < n; i++) {
    if ( !IsPoint(p[i]) ){
      (*AffineError)("PPacN: non-Point passed as Point.");
    }

    if (SpaceOf(p[i]) != S) {
      (*AffineError)("PPacN: points not from a common space.\n");
    }
    sum += a[i];
    x += a[i] * ME(p[i].p,0,0);
    y += a[i] * ME(p[i].p,0,1);
    if (dim == 3) {
      z += a[i] * ME(p[i].p,0,2);
    }
  }
  if (fabs(sum - 1.0) > EPSILON) {
    (*AffineError)("PPacN: coefficients don't sum to one.\n");
  }
  if ( dim == 3 ) {
    return PCreate( StdFrame(S), x, y, z);
  } else {
    return PCreate( StdFrame(S), x, y);
  }
}


/*
 ** Do an affine combination of n points. Returned is the point
 **
 **       sum_{i=0}^{n-1} s[i] * p[i]
 **
 ** where the s's sum to one.  Note that the s[i] and p[i] are
 ** extracted from the varargs.
 */
Point NPPac(int n, Point P, Scalar s, ...)
{
  Scalar x, y, z, sum;
  Space S=SpaceOf(P);
  int i, dim=Dim(SpaceOf(P));
  va_list ap;

  if ( n <= 0 ) {
    (*AffineError)("NPPac: must have at least 1 point in AC\n");
  }

  /* For efficiency, appeal to coordinates */
  x = 0.0;  y = 0.0;  z = 0.0;  sum = 0.0;

  if ( !IsPoint(P) ){
    (*AffineError)("NPPac: non-Point passed as Point.");
  }

  sum += s;
  x += s * ME(P.p,0,0);
  y += s * ME(P.p,0,1);
  if (dim == 3) {
    z += s * ME(P.p,0,2);
  }
  va_start(ap, s);
  for (i = 1; i < n; i++) {
    /* Reuse the space of P and s */
    P = va_arg(ap, Point);
    s = va_arg(ap, Scalar);
    if ( !IsPoint(P) ){
      (*AffineError)("NPPac: non-Point passed as Point.");
    }
    if (SpaceOf(P) != S) {
      (*AffineError)("NPPac: points not from a common space.\n");
    }
    sum += s;
    x += s * ME(P.p,0,0);
    y += s * ME(P.p,0,1);
    if (dim == 3) {
      z += s * ME(P.p,0,2);
    }
  }
  va_end(ap);
  if (fabs(sum - 1.0) > EPSILON) {
    (*AffineError)("NPPac: coefficients don't sum to one.\n");
  }
  if ( dim == 3 ) {
    return PCreate( StdFrame(S), x, y, z);
  } else {
    return PCreate( StdFrame(S), x, y);
  }
}



/*
 ** Do a "vector combination" of n points. Returned is the vector
 **
 **       sum_{i=0}^{n-1} a[i] * p[i]
 **
 ** where the a's sum to zero.
 */
Vector PPvcN(int n, Point p[], Scalar a[])
{
  Scalar x, y, z, sum;
  Space S = SpaceOf(p[0]);
  int i, dim = Dim(SpaceOf(p[0]));

  /* For efficiency, appeal to coordinates */
  x = 0.0;  y = 0.0;  z = 0.0;  sum = 0.0;
  for (i = 0; i < n; i++) {
    if ( !IsPoint(p[i]) ){
      (*AffineError)("PPvcN: non-Point passed as Point.");
    }

    if (SpaceOf(p[i]) != S) {
      (*AffineError)("PPvcN: points not from a common space.");
    }
    sum += a[i];
    x += a[i] * ME(p[i].p,0,0);
    y += a[i] * ME(p[i].p,0,1);
    if (dim == 3) {
      z += a[i] * ME(p[i].p,0,2);
    }
  }
  if (fabs(sum) > EPSILON) {
    (*AffineError)("PPvcN: coefficients don't sum to zero.");
  }
  if ( dim == 3 ) {
    return VCreate( StdFrame(S), x, y, z);
  } else {
    return VCreate( StdFrame(S), x, y);
  }
}

/*
 ** Do a "vector combination" of n points. Returned is the vector
 **
 **       sum_{i=0}^{n-1} s[i] * p[i]
 **
 ** where the s's sum to zero.  Note that the s[i] and p[i] are
 ** extracted from the varargs.
 */
Vector NPPvc(int n, Point P, Scalar s, ...)
{
  Scalar x, y, z, sum;
  Space Sp = SpaceOf(P);
  int i, dim = Dim(SpaceOf(P));
  va_list ap;

  if ( n <= 0 ) {
    (*AffineError)("NPPvc: must have at least 1 point in AC\n");
  }

  /* For efficiency, appeal to coordinates */
  x = 0.0;  y = 0.0;  z = 0.0;  sum = 0.0;

  if ( !IsPoint(P) ){
    (*AffineError)("NPPvc: non-Point passed as Point.");
  }

  sum += s;
  x += s * ME(P.p,0,0);
  y += s * ME(P.p,0,1);
  if (dim == 3) {
    z += s * ME(P.p,0,2);
  }
  va_start(ap, s);
  for (i = 1; i < n; i++) {
    P = va_arg(ap, Point);
    s = va_arg(ap, Scalar);
    if ( !IsPoint(P) ){
      (*AffineError)("NPPvc: non-Point passed as Point.");
    }
    if (SpaceOf(P) != Sp) {
      (*AffineError)("NPPvc: points not from a common space.\n");
    }
    sum += s;
    x += s * ME(P.p,0,0);
    y += s * ME(P.p,0,1);
    if (dim == 3) {
      z += s * ME(P.p,0,2);
    }
  }
  va_end(ap);
  if (fabs(sum) > EPSILON) {
    (*AffineError)("NPPvc: coefficients don't sum to zero.\n");
  }
  if ( dim == 3 ) {
    return VCreate( StdFrame(Sp), x, y, z);
  } else {
    return VCreate( StdFrame(Sp), x, y);
  }
}

/*
 ** Return the outward pointing normal vector to the plane defined
 ** by p1, p2 and p3.  The normal points toward a viewer positioned
 ** so that p1,p2,p3 is counterclockwise.
 ** pointing.
 */
Normal PPPNormal(Point p1, Point p2, Point p3)
{
  if ( !IsPoint(p1) ){
    (*AffineError)("PPPNormal: p1 not a Point.");
  }

  if ( !IsPoint(p2) ){
    (*AffineError)("PPPNormal: p2 not a Point.");
  }

  if ( !IsPoint(p3) ){
    (*AffineError)("PPPNormal: p3 not a Point.");
  }

  return VDual(VVCross( PPDiff( p2, p1), PPDiff( p3, p1)));
}


/*--------------------------------------------------------------*/
/*			Vector Space Operators			*/
/*--------------------------------------------------------------*/

/*
 ** Scalar-Vector Multiplication.
 */
Vector SVMult(Scalar scalar, Vector vector)
{
  Vector Vprime;

  if ( !IsVector(vector) ){
    (*AffineError)("SVMult: vector is not a Vector.");
  }

  SpaceOf(Vprime) = SpaceOf(vector);
  Vprime.v = MatrixCreate( 1, Dim(SpaceOf(Vprime)) + 1);
  ME(Vprime.v,0,0) = ME(vector.v,0,0) * scalar;
  ME(Vprime.v,0,1) = ME(vector.v,0,1) * scalar;
  if ( Dim(SpaceOf(vector)) == 3 ) {
    ME(Vprime.v,0,2) = ME(vector.v,0,2) * scalar;
    ME(Vprime.v,0,3) = 0.0;
  } else {
    ME(Vprime.v,0,2) = 0.0;
  }

  return Vprime;
}

/*
 ** Return the sum of the two vectors.
 */
Vector VVAdd(Vector v1, Vector v2)
{
  Vector v;

  if ( !IsVector(v1) ){
    (*AffineError)("VVAdd: v1 is not a Vector.");
  }

  if ( !IsVector(v2) ){
    (*AffineError)("VVAdd: v2 is not a Vector.");
  }

  if (SpaceOf(v1) != SpaceOf(v2)) {
    (*AffineError)("VVAdd: vectors from different spaces.");
  }
  SpaceOf(v) = SpaceOf(v1);
  v.v = MatrixCreate( 1, Dim(SpaceOf(v1)) + 1);
  ME(v.v,0,0) = ME(v1.v,0,0)+ME(v2.v,0,0);
  ME(v.v,0,1) = ME(v1.v,0,1)+ME(v2.v,0,1);
  if (Dim(SpaceOf(v)) == 3) {
    ME(v.v,0,2) = ME(v1.v,0,2)+ME(v2.v,0,2);
    ME(v.v,0,3) = 0.0;
  } else {
    ME(v.v,0,2) = 0.0;
  }
  return v;
}


/*
 ** Do a linear combination of n vectors. Returned is the vector
 **
 **       sum_{i=0}^{n-1} a[i] * v[i]
 **
 */
Vector VVlcN(int n, Vector v[], Scalar a[])
{
  Scalar x, y, z;
  Space S = SpaceOf(v[0]);
  int i, dim = Dim(SpaceOf(v[0]));

  /* For efficiency, appeal to coordinates */
  x = 0.0;  y = 0.0;  z = 0.0;
  for (i = 0; i < n; i++) {

    if ( !IsVector(v[i]) ){
      (*AffineError)("VVlcN: non-Vector passed as Vector.");
    }

    if (SpaceOf(v[i]) != S) {
      (*AffineError)("VVlcN: vectors not from a common space.\n");
    }
    x += a[i] * ME(v[i].v,0,0);
    y += a[i] * ME(v[i].v,0,1);
    if (dim == 3) {
      z += a[i] * ME(v[i].v,0,2);
    }
  }
  if ( dim == 3 ) {
    return VCreate( StdFrame(S), x, y, z);
  } else {
    return VCreate( StdFrame(S), x, y);
  }
}


/*
 ** Do a linear combination of n vectors. Returned is the vector
 **
 **       sum_{i=0}^{n-1} s[i] * v[i]
 **
 ** Note that s and v are passed as varargs
 */
Vector NVVlc(int n, Vector V, Scalar s, ...)
{
  Scalar x, y, z, sum;
  Space S = SpaceOf(V);
  int i, dim = Dim(SpaceOf(V));
  va_list ap;

  if ( n <= 0 ) {
    (*AffineError)("NVVlc: must have at least 1 point in AC\n");
  }

  va_start(ap, s);
  /* For efficiency, appeal to coordinates */
  x = 0.0;  y = 0.0;  z = 0.0;  sum = 0.0;

  if ( !IsVector(V) ){
    (*AffineError)("NVVlc: non-Vector passed as Vector.");
  }

  sum += s;
  x += s * ME(V.v,0,0);
  y += s * ME(V.v,0,1);
  if (dim == 3) {
    z += s * ME(V.v,0,2);
  }
  for (i = 1; i < n; i++) {
    V = va_arg(ap, Vector);
    s = va_arg(ap, Scalar);
    if ( !IsVector(V) ){
      (*AffineError)("NVVlc: non-Vector passed as Vector.");
    }
    if (SpaceOf(V) != S) {
      (*AffineError)("NVVlc: Vectors not from a common space.\n");
    }
    sum += s;
    x += s * ME(V.v,0,0);
    y += s * ME(V.v,0,1);
    if (dim == 3) {
      z += s * ME(V.v,0,2);
    }
  }
  va_end(ap);

  if ( dim == 3 ) {
    return VCreate( StdFrame(S), x, y, z);
  } else {
    return VCreate( StdFrame(S), x, y);
  }
}

/*
 ** Return the difference of vectors: v1 - v2;
 */
Vector VVDiff(Vector v1, Vector v2)
{
  Vector v;

  if ( !IsVector(v1) ){
    (*AffineError)("VVDiff: v1 is not a Vector.");
  }

  if ( !IsVector(v2) ){
    (*AffineError)("VVDiff: v2 is not a Vector.");
  }

  if (SpaceOf(v1) != SpaceOf(v2)) {
    (*AffineError)("VVDiff: vectors from different spaces.");
  }
  SpaceOf(v) = SpaceOf(v1);
  v.v = MatrixCreate( 1, Dim(SpaceOf(v1)) + 1);
  ME(v.v,0,0) = ME(v1.v,0,0) - ME(v2.v,0,0);
  ME(v.v,0,1) = ME(v1.v,0,1) - ME(v2.v,0,1);
  if (Dim(SpaceOf(v)) == 3) {
    ME(v.v,0,2) = ME(v1.v,0,2) - ME(v2.v,0,2);
    ME(v.v,0,3) = 0.0;
  } else {
    ME(v.v,0,2) = 0.0;
  }
  return v;
}



/*
 ** Return the geometric dot product of v1 and v2.
 */
Scalar VVDot(Vector v1, Vector v2)
{
  Scalar dot;

  if ( !IsVector(v1) ){
    (*AffineError)("VVDot: v1 is not a Vector.");
  }

  if ( !IsVector(v2) ){
    (*AffineError)("VVDot: v2 is not a Vector.");
  }

  if (SpaceOf(v1) != SpaceOf(v2)) {
    (*AffineError)("VVDot: vectors from different spaces.");
  }
  dot = ME(v1.v,0,0)*ME(v2.v,0,0) + ME(v1.v,0,1)*ME(v2.v,0,1);
  if (Dim(SpaceOf(v1)) == 3) {
    dot += ME(v1.v,0,2)*ME(v2.v,0,2);
  }
  return dot;
}


/*
 ** Return the vector given by v1 cross v2.
 */
Vector VVCross(Vector v1, Vector v2)
{
  Vector v;

  if ( !IsVector(v1) ){
    (*AffineError)("VVCross: v1 is not a Vector.");
  }

  if ( !IsVector(v2) ){
    (*AffineError)("VVCross: v2 is not a Vector.");
  }

  if (SpaceOf(v1) != SpaceOf(v2)) {
    (*AffineError)("VVCross: vectors from different spaces.");
  }
  if (Dim(SpaceOf(v1)) != 3) {
    (*AffineError)("VVCross: vectors must be from a 3-space.");
  }
  SpaceOf(v) = SpaceOf(v1);
  v.v = MatrixCreate( 1, Dim(SpaceOf(v1)) + 1);
  ME(v.v,0,0) = ME(v1.v,0,1)*ME(v2.v,0,2) - ME(v1.v,0,2)*ME(v2.v,0,1);
  ME(v.v,0,1) = ME(v1.v,0,2)*ME(v2.v,0,0) - ME(v1.v,0,0)*ME(v2.v,0,2);
  ME(v.v,0,2) = ME(v1.v,0,0)*ME(v2.v,0,1) - ME(v1.v,0,1)*ME(v2.v,0,0);

  return v;
}

/*
 ** Return the scalar given by v1 cross v2.
 */
Scalar SVVCross(Vector v1, Vector v2)
{
  Scalar x1,y1,x2,y2;

  if ( !IsVector(v1) ){
    (*AffineError)("VVCross: v1 is not a Vector.");
  }

  if ( !IsVector(v2) ){
    (*AffineError)("VVCross: v2 is not a Vector.");
  }

  if (SpaceOf(v1) != SpaceOf(v2)) {
    (*AffineError)("VVCross: vectors from different spaces.");
  }
  if (Dim(SpaceOf(v1)) != 2) {
    (*AffineError)("SVVCross: vectors must be from a 2-space.");
  }
  return ME(v1.v,0,0)*ME(v2.v,0,1) - ME(v1.v,0,1)*ME(v2.v,0,0);
}



/*
 ** Return the orthogonal projection of v onto w.
 */
Vector VVProj(Vector v, Vector w)
{
  Scalar factor;

  if ( !IsVector(v) ){
    (*AffineError)("VVProj: v is not a Vector.");
  }

  if ( !IsVector(w) ){
    (*AffineError)("VVProj: w is not a Vector.");
  }

  factor = VVDot( v, w) / VVDot( w, w);
  return SVMult( factor, w);
}


/*
 ** Turn the primal vector into a normal.  In vector space
 ** terminology, v is mapping to a linear functional according
 ** to the rule v --> phi_v(w) = (v .)(w), where (.) is the dot
 ** product.  Normals are represented as column matrices.
 */
Normal VDual(Vector v)
{
  Normal phi;

  if ( !IsVector(v) ){
    (*AffineError)("VDual: v is not a Vector.");
  }

  SpaceOf(phi) = SpaceOf(v);
  phi.n = MatrixTranspose( &(v.v));

  return phi;
}


/*
 ** Turn the normal vector into a primal vector.
 */
Vector NDual(Normal phi)
{
  Vector v;

  if ( !IsNormal(phi) ){
    (*AffineError)("NDual: phi is not a Normal.");
  }

  SpaceOf(v) = SpaceOf(phi);
  v.v = MatrixTranspose( &(phi.n));

  return v;
}


/*
 ** Apply a linear functional (ie a normal vector) to a
 ** primal vector.
 */
Scalar NVApply(Normal phi, Vector v)
{
  Matrix M;

  if ( !IsNormal(phi) ){
    (*AffineError)("NVApply: phi is not a Normal.");
  }

  if ( !IsVector(v) ){
    (*AffineError)("NVApply: v is not a Vector.");
  }

  if (SpaceOf(phi) != SpaceOf(v)) {
    (*AffineError)("NVApply: vector and normal not from a common space.");
  }
  M = MatrixMult( &(v.v), &(phi.n));
  return ME(M, 0, 0);
}


/*
 ** Normalize the given vector.
 */
Vector VNormalize(Vector v)
{
  Scalar mag = VMag(v);

  if ( !IsVector(v) ){
    (*AffineError)("VNormalize: v is not a Vector.");
  }

  if (mag < EPSILON) {
    (*AffineError)("VNormalize: can't normalize zero vector");
  }

  return SVMult( 1.0/mag, v);
}

/*--------------------------------------------------------------*/
/*			Transformation Routines			*/
/*--------------------------------------------------------------*/

/*
 ** Return the point p transformed by transformation T.
 */
Point PAxform(Point p, AffineMap T)
{
  Point pp;			/* pp = T(p) = */

  if ( !IsPoint(p) ){
    (*AffineError)("PAxform: p is not a Point.");
  }

  if (SpaceOf(p) != T.domain) {
    (*AffineError)("PAxform: point not in domain space.");
    return pp;
  }
  SpaceOf(pp) = T.range;
  pp.p = MatrixMult( &(p.p), &(T.t));
#ifdef DEBUG
  fprintf(stderr, "AffineMap: ");
  PPrintf(stderr, p);
  fprintf(stderr, " --> ");
  PPrintf(stderr, pp);
  fprintf(stderr, "\n");
#endif
  return pp;
}


/*
 ** Return the vector v transformed by transformation T.
 */
Vector VAxform(Vector v, AffineMap T)
{
  Vector vv;			/* vv = T(v) = */

  if ( !IsVector(v) ){
    (*AffineError)("VAxform: v is not a Vector.");
  }

  if (SpaceOf(v) != T.domain) {
    (*AffineError)("VAxform: vector not in domain space.");
    return vv;
  }
  SpaceOf(vv) = T.range;
  vv.v = MatrixMult( &(v.v), &(T.t));
#ifdef DEBUG
  fprintf(stderr, "AffineMap: ");
  VPrintf(stderr, v);
  fprintf(stderr, " --> ");
  VPrintf(stderr, vv);
  fprintf(stderr, "\n");
#endif
  return vv;
}



/*
 ** Return the linear functional (ie, normal vector) transformed by
 ** transformation T.
 */
Normal NAxform(Normal n, AffineMap T)
{
  Normal nn;

  if ( !IsNormal(n) ){
    (*AffineError)("NAxform: n is not a Normal.");
  }

  if (SpaceOf(n) != T.domain) {
    (*AffineError)("NAxform: normal not in domain space.");
    return nn;
  }
  SpaceOf(nn) = T.range;

  /* Normals transform by multiplication on the left by the   */
  /* inverse of the matrix that transforms vectors and points.*/
  if (!T.invertible) {
    (*AffineError)
      ("NAxform: attempt to map a normal through a non-invertible map.");
  }
  nn.n = MatrixMult( &(T.invt), &(n.n));

  /* Only the linear portion of T should have been inverted.     */
  /* To compensate, we must zero out the last component manually.*/
  switch (Dim(SpaceOf(nn))) {
    case 2:
      ME(nn.n,2,0) = 0.0;
      break;
    case 3:
      ME(nn.n,3,0) = 0.0;
      break;
  }

#ifdef DEBUG
  fprintf(stderr, "AffineMap: ");
  NPrintf(stderr, n);
  fprintf(stderr, " --> ");
  NPrintf(stderr, nn);
  fprintf(stderr, "\n");
#endif
  return nn;
}


/*
 ** Return the composite transformation T2(T1(.)).
 */
AffineMap AACompose(AffineMap T1, AffineMap T2)
{
  AffineMap T;

  if (T1.range != T2.domain) {
    (*AffineError)("AACompose: Range and domains don't match.");
  } else {
    T.domain = T1.domain;
    T.range  = T2.range;
    T.t = MatrixMult( &(T1.t), &(T2.t));
    if (T1.invertible && T2.invertible) {
      T.invertible = TRUE;
      T.invt = MatrixMult( &(T2.invt), &(T1.invt));
    } else {
      T.invertible = FALSE;
    }
  }
  return T;
}



/*--------------------------------------------------------------*/
/*			Utility Routines			*/
/*--------------------------------------------------------------*/



void PPrintf( FILE *fp, Point p)
{
  fprintf(fp, "Point: space: %s ", Name(SpaceOf(p)));
  MatrixPrint(p.p, fp);
}

void VPrintf( FILE *fp, Vector v)
{
  fprintf(fp, "Vector: space: %s ", Name(SpaceOf(v)));
  MatrixPrint(v.v, fp);
}

void APrintf( FILE *fp, AffineMap T)
{
  fprintf( fp, "AffineMap: domain: %s, range: %s\n",
      Name(T.domain), Name(T.range));
  MatrixPrint( T.t, fp);
}


void F_Printf( FILE *fp, Frame F)
{
  fprintf( fp, "Frame:\n");
  VPrintf( fp, FV(F,0));
  VPrintf( fp, FV(F,1));
  if (Dim(SpaceOf(F))==3) VPrintf( fp, FV(F,2));
  PPrintf( fp, FOrg(F));
}
