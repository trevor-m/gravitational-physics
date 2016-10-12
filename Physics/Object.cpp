#include "Object.h"
double Object::GRAV_CONST = GRAV_CONST_INIT;


Object::Object()
{
	x = y = 0;
	r = 10;
	vx = vy = 0;
	density = 1;
}

Object::Object(double x, double y, double r, double density)
{
	this->x = x;
	this->y = y;
	this->r = r;
	this->density = density;
	vx = vy = 0;
}

Object::Object(double x, double y, double mass)
{
	this->x = x;
	this->y = y;
	this->r = 50;
	this->density = 20;
	vx = vy = 0;
	AddMass(mass);
}

Object::~Object()
{
}

double Object::GetMass()
{
	double v = 4 / 3 * M_PI * pow(r, 3);
	return v * density;
}

double Object::AngleTo(Object* o)
{
	return atan2(o->y - this->y, o->x - this->x);
}

void Object::Update(vector<Object*>& objects, double dt)
{
	//find total acceleration in x & y directions
	double ax = 0, ay = 0;

	for (int i = 0; i < objects.size(); i++)
	{
		//skip this object and any objects it overlaps
		if (objects[i] == this || this->CollidesWith(objects[i]))
			continue;

		//calculate acceleration due to gravity a = G*m/r^2
		double accel = GRAV_CONST * objects[i]->GetMass() / DistanceSquared(objects[i]);
		//calculate angle to other object
		double angle = AngleTo(objects[i]);
		//add x component to net acceleration in x direction
		ax += accel * cos(angle);
		//add y component to net acceleration in y direction
		ay += accel * sin(angle);
	}

	vx += ax * dt;
	vy += ay * dt;
	x += vx * dt;
	y += vy * dt;
}

bool Object::CollidesWith(Object* o)
{
	if (sqrt(DistanceSquared(o)) <= (o->r + this->r))
		return true;
	return false;
}

double Object::DistanceSquared(Object* o)
{
	return pow(o->x - this->x, 2) + pow(o->y - this->y, 2);
}

void Object::Orbit(Object* o, int direction)
{
	//assume in line with sun
	// v = sqrt(G*M/r)
	vy = o->vy + direction* sqrt((Object::GRAV_CONST * o->GetMass()) / sqrt(DistanceSquared(o)));
}

void Object::AddMass(double m)
{
	double newMass = m + GetMass();

	double v = newMass / density;
	this->r = cbrt(3 * v / (4 * M_PI));
}