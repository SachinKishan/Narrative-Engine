#pragma once

#define MAX_RAY_ITERATIONS 100


class Collider
{
public:
	Collider(){}

	virtual bool CollisionTest(glm::vec3, glm::vec3);

};

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

	bool CollisionTestSphere(glm::vec3 sphere_pos,glm::vec3 ray_pos)
	{
		//march along ray, find radius, check for distance
		return distance(sphere_pos, ray_pos) <= radius;

		//glm::vec3 oc=

	}

	bool CollisionTest(glm::vec3 sphere_pos, glm::vec3 ray_pos) 
	{
		return CollisionTestSphere(sphere_pos, ray_pos);
	}

};

class PlaneCollider: public Collider
{
private:
	glm::vec4 planeVariables;

public:
	void setPlane(glm::vec4 variables) { planeVariables = variables; }
	glm::vec4 getPlane()
	{
		return planeVariables;
	}
	PlaneCollider(glm::vec4 variables)
	{
		setPlane(variables);
	}

	bool CollisionTest(glm::vec3 pos, glm::vec3 ray_pos) override
	{
		return distancePointPlane(ray_pos, planeVariables)<0.5;
	}

	double distancePointPlane(const glm::vec3& point, const glm::vec4& plane)
	{
		// Calculate the distance between a point and a plane in 3D space
		double numerator = std::abs(glm::dot(glm::vec3(plane), point) + plane.w);
		double denominator = glm::length(glm::vec3(plane));

		return numerator / denominator;
	}


};