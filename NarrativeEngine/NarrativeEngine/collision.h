#pragma once
#include <algorithm> // Include for std::max
#define MAX_RAY_ITERATIONS 100


class Collider
{
public:
	Collider(){}

	virtual bool CollisionTest(glm::vec3, glm::vec3,glm::vec3,glm::vec3);
	virtual bool changeRotation();
	virtual bool changeScale();
	virtual bool changeTranslation();

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


class BoxCollider {
private:

public:
	BoxCollider(){}

	float sdBox(const glm::vec3 p, glm::vec3 boxCenter, glm::vec3 boxRotation, glm::vec3 boxScale) const {
		glm::vec3 localPoint = inverseTransformPoint(p, boxCenter, boxRotation, boxScale);
		glm::vec3 halfExtent =boxScale / 2.0f;
		glm::vec3 q = glm::abs(localPoint) - halfExtent;
		float dist = glm::length(vecMax(glm::vec3(0.0), q)) + min(max(q.x, max(q.y, q.z)), 0.0f);
		//std::cout << "\nBox dist: " << dist<<"\n";
		return dist;
	}

	bool CollisionTest(glm::vec3 ray_pos,glm::vec3 boxCenter, glm::vec3 boxRotation, glm::vec3 boxScale) {
		return sdBox(ray_pos, boxCenter, boxRotation, boxScale)-0.001<= 0;
	}


	//world space to local space
	glm::vec3 inverseTransformPoint(const glm::vec3& p,glm::vec3 boxCenter ,glm::vec3 boxRotation, glm::vec3 boxScale) const {
		// Inverse translation
		glm::vec3 translatedPoint = p - boxCenter;

		// Inverse rotation
		glm::mat4 inverseRotationMatrix = glm::rotate(glm::mat4(1.0f),glm::radians(-boxRotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-boxRotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(-boxRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat3 inverseRotationMat = glm::mat3(inverseRotationMatrix);

		translatedPoint = inverseRotationMat * translatedPoint;
		
		return translatedPoint;
	}


private:
	glm::vec3 vecMax(glm::vec3 a, glm::vec3 b) const {
		float q = max(a.x, b.x);
		float r = max(a.y, b.y);
		float s = max(a.z, b.z);
		return { q, r, s };
	}

};

template<typename T>
bool ray_collision_impl(glm::vec3 origin, glm::vec3 dir, const std::vector<std::shared_ptr<T>>& objects, std::shared_ptr<T>& obj)
{
	float totaldist = 0;
	glm::vec3 current_pos = origin;
	for (int i = 0; i < MAX_RAY_ITERATIONS; i++)
	{
		float min_dist = std::numeric_limits<float>::infinity();
		double calc_distance = 0;
		for (const auto& object : objects)
		{
			if (object->collider.CollisionTest(current_pos, object->transform.translation, object->transform.rotation, object->transform.scale))
			{
				obj = object;
				return true;
			}
			calc_distance = object->collider.sdBox(current_pos, object->transform.translation, object->transform.rotation, object->transform.scale);
			if (calc_distance < min_dist)
			{
				min_dist = calc_distance;
			}
		}
		totaldist += min_dist;
		current_pos = origin + dir * totaldist;
		if (totaldist > 100000)
		{
			return false;
		}
	}
	return false;
}