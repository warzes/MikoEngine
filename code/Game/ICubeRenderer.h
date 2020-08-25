#pragma once

class ICubeRenderer
{
public:
	inline virtual ~ICubeRenderer()
	{
	}

	virtual void setNumberOfCubes(uint32_t numberOfCubes) = 0;

	/**
	*  @brief
	*    Draw the cubes by filling a given command buffer
	*
	*  @param[in] globalTimer
	*    Global timer
	*  @param[in] globalScale
	*    Global scale
	*  @param[in] lightPositionX
	*    X component of the light position
	*  @param[in] lightPositionY
	*    Y component of the light position
	*  @param[in] lightPositionZ
	*    Z component of the light position
	*  @param[out] commandBuffer
	*    RHI command buffer to fill
	*/
	virtual void fillCommandBuffer(float globalTimer, float globalScale, float lightPositionX, float lightPositionY, float lightPositionZ, Rhi::CommandBuffer& commandBuffer) = 0;

	
protected:
	inline ICubeRenderer()
	{
	}
	explicit ICubeRenderer(const ICubeRenderer&) = delete;
	ICubeRenderer& operator =(const ICubeRenderer&) = delete;

	static constexpr uint32_t MAXIMUM_NUMBER_OF_TEXTURES = 8;	///< Maximum number of textures
};