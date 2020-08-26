#pragma once

class Plane final
{
public:
	glm::vec3 normal;	// Plane normal, not necessarily normalized but no zero vector
	float	  d;		// Plane distance from origin

	inline Plane() = delete;

	inline Plane(const glm::vec4& normalDistance) noexcept :
		normal(normalDistance.x, normalDistance.y, normalDistance.z),
		d(normalDistance.w)
	{
		// Nothing here
	}

	inline void normalize()
	{
		const float magnitude = glm::length(normal);
		normal /= magnitude;
		d /= magnitude;
	}
};