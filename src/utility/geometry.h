#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>

namespace Winedark
{
	/*-----------------------------------------------*/
	/* Rays */
	/*-----------------------------------------------*/
	struct Ray
	{
		glm::vec3 position;
		glm::vec3 direction;
	};

	/*-----------------------------------------------*/
	/* Vector Functions */
	/*-----------------------------------------------*/
	float Lerp(float a, float b, float step);
	glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float step);
	glm::vec4 Lerp(glm::vec4 a, glm::vec4 b, float step);

	/*-----------------------------------------------*/
	/* Quaternions */
	/*-----------------------------------------------*/
	struct Quaternion
	{
		float w;
		float x;
		float y;
		float z;

		void Normalize();

		Quaternion()
		{
			this->w = 1.0f;
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
		}

		Quaternion(float w, float x, float y, float z)
		{
			this->w = w;
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Quaternion(glm::vec3 axis, float angle)
		{
			float h = angle * 0.5f;
			glm::vec3 v = axis * glm::sin(h);

			this->w = glm::cos(h);
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
		}

		Quaternion operator*(const Quaternion& rhs) const noexcept
		{
			Quaternion q = { 0, 0, 0, 0 };
			q.w = (this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z);
			q.x = (this->w * rhs.x + this->x * rhs.w + this->y * rhs.z - this->z * rhs.y);
			q.y = (this->w * rhs.y - this->x * rhs.z + this->y * rhs.w + this->z * rhs.x);
			q.z = (this->w * rhs.z + this->x * rhs.y - this->y * rhs.x + this->z * rhs.w);
			return q;
		}

		bool operator==(const Quaternion& rhs) const noexcept
		{
			return ((this->w == rhs.w) && (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z));
		}

		Quaternion operator-() const noexcept
		{
			return Quaternion{ -this->w, -this->x, -this->y, -this->z };
		}
	};

	/*-----------------------------------------------*/
	/* Quaternion Functions */
	/*-----------------------------------------------*/
	Quaternion Slerp(Quaternion q, Quaternion r, float step);
	float QuaternionDistance(Quaternion l, Quaternion r);

	Quaternion LookAt(glm::vec3 position, glm::vec3 target);

	Quaternion NormalizeQuaternion(Quaternion q);
	Quaternion EulerToQuaternion(glm::vec3 e);
	glm::vec3 QuaternionToEuler(Quaternion q);

	glm::vec3 RotateRelative(glm::vec3 relative, glm::vec3 position, Quaternion q);
	glm::vec3 Rotate(glm::vec3 position, Quaternion q);

	/*-----------------------------------------------*/
	/* Bezier Curves */
	/*-----------------------------------------------*/
	struct BezierCurve
	{
		std::vector<glm::vec3> points;

		glm::vec3 GetPoint(float t);
	};

	/*-----------------------------------------------*/
	/* Bezier Curves for Quaternions */
	/*-----------------------------------------------*/
	struct BezierQuaternion
	{
		std::vector<Quaternion> rotations;

		Quaternion GetQuaternion(float t);
	};
}

#endif
