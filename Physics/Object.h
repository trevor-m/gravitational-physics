#pragma once
#include <vector>
using std::vector;
#define _USE_MATH_DEFINES
#include <math.h>
#define PARENT_PLAYER 0
#define PARENT_ENEMY 1

#define GRAV_CONST_INIT 0.000001

class Object
{
protected:
	
public:
	static double GRAV_CONST;
	double x, y;
	double vx, vy;
	double r;
	double density;
	Object();
	Object(double x, double y, double r, double density);
	Object(double x, double y, double mass);
	~Object();
	virtual void Update(vector<Object*>& objects, double dt);

	double GetMass();
	void AddMass(double m);
	bool CollidesWith(Object* o);
	double DistanceSquared(Object* o);
	double AngleTo(Object* o);
	void Orbit(Object* o, int direction);
};

