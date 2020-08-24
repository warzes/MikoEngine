#pragma once

namespace Rhi
{
	struct IndexBufferFormat final
	{
		enum Enum
		{
			UNSIGNED_CHAR = 0,	// One byte per element, uint8_t (may not be supported by each RHI implementation, primarily for mobile devices)
			UNSIGNED_SHORT = 1,	// Two bytes per element, uint16_t (best support across multiple RHI implementations)
			UNSIGNED_INT = 2	// Four bytes per element, uint32_t (may not be supported by each RHI implementation)
		};

		[[nodiscard]] static inline constexpr const uint32_t GetNumberOfBytesPerElement(Enum indexFormat)
		{
			constexpr const uint32_t MAPPING[] =
			{
				1,	// One byte per element, uint8_t (may not be supported by each RHI implementation, primarily for mobile devices)
				2,	// Two bytes per element, uint16_t (best support across multiple RHI implementations)
				4	// Four bytes per element, uint32_t (may not be supported by each RHI implementation)
			};
			return MAPPING[indexFormat];
		}
	};
} // namespace Rhi