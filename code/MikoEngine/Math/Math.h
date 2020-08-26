#pragma once

namespace Rhi
{
	class IRhi;
}
namespace Renderer
{
	class IFileManager;
}

//[-------------------------------------------------------]
//[ Global definitions                                    ]
//[-------------------------------------------------------]
typedef const char* VirtualFilename;	// UTF-8 virtual filename, the virtual filename scheme is "<mount point = project name>/<asset directory>/<asset name>.<file extension>" (example "Example/Mesh/Monster/Squirrel.mesh"), never ever a null pointer and always finished by a terminating zero


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
class Math final
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
public:
	static constexpr uint32_t FNV1a_INITIAL_HASH_32 = 0xcbf29ce4u;
	static constexpr uint64_t FNV1a_INITIAL_HASH_64 = 0xcbf29ce484222325u;
	// "glm::vec3" constants
	static constexpr glm::vec3 VEC3_ZERO = glm::vec3(0.0f, 0.0f, 0.0f);	// 0 0 0
	static constexpr glm::vec3 VEC3_ONE = glm::vec3(1.0f, 1.0f, 1.0f);	// 1 1 1
	static constexpr glm::vec3 VEC3_UNIT_X = glm::vec3(1.0f, 0.0f, 0.0f);	// 1 0 0
	static constexpr glm::vec3 VEC3_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);	// 1 0 0; coordinate system axis terminology, don't remove
	static constexpr glm::vec3 VEC3_UNIT_Y = glm::vec3(0.0f, 1.0f, 0.0f);	// 0 1 0
	static constexpr glm::vec3 VEC3_UP = glm::vec3(0.0f, 1.0f, 0.0f);	// 0 1 0; coordinate system axis terminology, don't remove
	static constexpr glm::vec3 VEC3_UNIT_Z = glm::vec3(0.0f, 0.0f, 1.0f);	// 0 0 1
	static constexpr glm::vec3 VEC3_FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);	// 0 0 1; coordinate system axis terminology, don't remove
	// "glm::dvec3" constants
	static constexpr glm::dvec3 DVEC3_ZERO = glm::dvec3(0.0, 0.0, 0.0);	// 0 0 0
	static constexpr glm::dvec3 DVEC3_ONE = glm::dvec3(1.0, 1.0, 1.0);	// 1 1 1
	static constexpr glm::dvec3 DVEC3_UNIT_X = glm::dvec3(1.0, 0.0, 0.0);	// 1 0 0
	static constexpr glm::dvec3 DVEC3_RIGHT = glm::dvec3(1.0, 0.0, 0.0);	// 1 0 0; coordinate system axis terminology, don't remove
	static constexpr glm::dvec3 DVEC3_UNIT_Y = glm::dvec3(0.0, 1.0, 0.0);	// 0 1 0
	static constexpr glm::dvec3 DVEC3_UP = glm::dvec3(0.0, 1.0, 0.0);	// 0 1 0; coordinate system axis terminology, don't remove
	static constexpr glm::dvec3 DVEC3_UNIT_Z = glm::dvec3(0.0, 0.0, 1.0);	// 0 0 1
	static constexpr glm::dvec3 DVEC3_FORWARD = glm::dvec3(0.0, 0.0, 1.0);	// 0 0 1; coordinate system axis terminology, don't remove
	// "glm::vec4" constants
	static constexpr glm::vec4 VEC4_ZERO = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);	// 0 0 0 0
	static constexpr glm::vec4 VEC4_ONE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);	// 1 1 1 1
	// "glm::dvec4" constants
	static constexpr glm::dvec4 DVEC4_ZERO = glm::dvec4(0.0, 0.0, 0.0, 0.0);	// 0 0 0 0
	static constexpr glm::dvec4 DVEC4_ONE = glm::dvec4(1.0, 1.0, 1.0, 1.0);	// 1 1 1 1
	// "glm::mat4" constants
	static constexpr glm::mat4 MAT4_IDENTITY = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	// "glm::dmat4" constants
	static constexpr glm::dmat4 DMAT4_IDENTITY = glm::dmat4(1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	// "glm::quat" constants
	static constexpr glm::quat QUAT_IDENTITY = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	// "glm::squat" constants
	static constexpr glm::dquat DQUAT_IDENTITY = glm::dquat(1.0, 0.0, 0.0, 0.0);


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
public:
	/**
	*  @brief
	*    Calculate tangent frame quaternion (QTangent) basing of a provided 3x3 tangent frame matrix
	*
	*  @param[in, out] tangentFrameMatrix
	*    3x3 tangent frame matrix, will be manipulated during calculation (no internal copy for performance reasons)
	*
	*  @return
	*    The calculated tangent frame quaternion (QTangent)
	*
	*  @note
	*  - QTangent basing on http://dev.theomader.com/qtangents/ "QTangents" which is basing on
	*    http://www.crytek.com/cryengine/presentations/spherical-skinning-with-dual-quaternions-and-qtangents "Spherical Skinning with Dual-Quaternions and QTangents"
	*/
	[[nodiscard]] static glm::quat calculateTangentFrameQuaternion(glm::mat3& tangentFrameMatrix);

	/**
	*  @brief
	*    Calculate inner bounding sphere radius
	*
	*  @param[in] minimumBoundingBoxPosition
	*    The minimum bounding box position
	*  @param[in] maximumBoundingBoxPosition
	*    The maximum bounding box position
	*
	*  @return
	*    Inner bounding sphere radius
	*/
	[[nodiscard]] static float calculateInnerBoundingSphereRadius(const glm::vec3& minimumBoundingBoxPosition, const glm::vec3& maximumBoundingBoxPosition);

	/**
	*  @brief
	*    Ensure that the given value is within the given interval [minimum, maximum] by wrapping the value
	*
	*  @param[in] value
	*    Value to check
	*  @param[in] minimum
	*    Minimum of the interval, must be < maximum
	*  @param[in] maximum
	*    Maximum of the interval, must be > minimum
	*
	*  @return
	*    The value within the interval [minimum, maximum]
	*
	*  @note
	*    - In case of violating the interface specification by swapping minimum/maximum the result will not be different to the one of the correct order
	*/
	[[nodiscard]] static float wrapToInterval(float value, float minimum, float maximum);
	[[nodiscard]] static double wrapToInterval(double value, double minimum, double maximum);

	/**
	*  @brief
	*    Make a given value to a multiple of another given value
	*
	*  @param[in] value
	*    Value to make a multiple of the given second value (e.g. 0.9)
	*  @param[in] primaryValue
	*    Value the first one should be a multiple of (e.g. 1.5)
	*
	*  @return
	*    The updated value (e.g. 1.5)
	*/
	[[nodiscard]] static float makeMultipleOf(float value, float primaryValue);
	[[nodiscard]] static double makeMultipleOf(double value, float primaryValue);
	[[nodiscard]] static uint32_t makeMultipleOf(uint32_t value, uint32_t primaryValue);

	/**
	*  @brief
	*    Get RHI dependent texture scale bias matrix
	*
	*  @param[in] rhi
	*    Used RHI
	*
	*  @return
	*    RHI dependent texture scale bias matrix
	*/
	[[nodiscard]] static const glm::mat4& getTextureScaleBiasMatrix(const Rhi::IRhi& rhi);

	//[-------------------------------------------------------]
	//[ Hash                                                  ]
	//[-------------------------------------------------------]
	[[nodiscard]] static uint32_t calculateFNV1a32(const uint8_t* content, uint32_t numberOfBytes, uint32_t hash = FNV1a_INITIAL_HASH_32);
	[[nodiscard]] static uint64_t calculateFNV1a64(const uint8_t* content, uint32_t numberOfBytes, uint64_t hash = FNV1a_INITIAL_HASH_64);
	[[nodiscard]] static uint64_t calculateFileFNV1a64ByVirtualFilename(const Renderer::IFileManager& fileManager, VirtualFilename virtualFilename);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
private:
	Math() = delete;
	~Math() = delete;
	explicit Math(const Math&) = delete;
	Math& operator=(const Math&) = delete;
};