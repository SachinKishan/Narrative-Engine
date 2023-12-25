#pragma once

#define MAX_RAY_ITERATIONS 100


class SphereCollider
{
private:
	double radius;
	
public:
	SphereCollider()
	{
		radius = 1.0;
	}
	SphereCollider(double r)
	{
		radius = r;
	}
	void SetRadius(double r)
	{
		radius = r;
	}
	double GetRadius() const { return radius; }

	bool CollisionTest(glm::vec3 sphere_pos,glm::vec3 ray_pos)
	{
		//march along ray, find radius, check for distance
		return distance(sphere_pos, ray_pos) <= radius;

		//glm::vec3 oc=

	}



};