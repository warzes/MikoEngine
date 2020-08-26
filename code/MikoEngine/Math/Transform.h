#pragma once

#include "Math/Math.h"

// Transform class containing 64 bit position, 32 bit rotation and 32 bit scale
class Transform final
{
public:
	static const Transform IDENTITY;	// Identity transform

	glm::dvec3 position;	// 64 bit world space position, or depending on the use-case in another coordinate system
	glm::quat  rotation;
	glm::vec3  scale;


	inline Transform() noexcept 
		: position(Math::DVEC3_ZERO)
		, rotation(Math::QUAT_IDENTITY)
		, scale(Math::VEC3_ONE)
	{
		// Nothing here
	}

	inline explicit Transform(const glm::dvec3& _position)  
		: position(_position)
		, rotation(Math::QUAT_IDENTITY)
		, scale(Math::VEC3_ONE)
	{
		// Nothing here
	}

	inline explicit Transform(const glm::dmat4& transformMatrix)
	{
		setByMatrix(transformMatrix);
	}

	inline Transform(const glm::dvec3& _position, const glm::quat& _rotation) 
		: position(_position)
		, rotation(_rotation)
		, scale(Math::VEC3_ONE)
	{
		// Nothing here
	}

	inline Transform(const glm::dvec3& _position, const glm::quat& _rotation, const glm::vec3& _scale) 
		: position(_position)
		, rotation(_rotation)
		, scale(_scale)
	{
		// Nothing here
	}

	void getAsMatrix(glm::dmat4& objectSpaceToWorldSpace) const;
	void getAsMatrix(glm::mat4& objectSpaceToWorldSpace) const;	// Do only use this 32 bit precision if you're certain it's sufficient (for example because you made the position camera relative)
	void setByMatrix(const glm::dmat4& objectSpaceToWorldSpace);
	Transform& operator+=(const Transform& other);
};