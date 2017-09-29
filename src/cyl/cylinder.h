typedef struct polygon {
	Point P[10];
	int nv;
} Polygon;

typedef struct ray {
	Point P;
	Vector v;
} Ray;

typedef struct cyl {
    Point P;
    Vector v;
    double r;
	double h;
	Point A;
	Point B;
} Cylinder;

typedef struct {
	int hitmiss;
	Scalar a,b;
} Intersection;

typedef enum hitMiss { HH, HM, MH, MM } HitMiss;
