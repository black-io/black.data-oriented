#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	/// @brief	Compile-time representation for memory chunk information.
	template< size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t CHUNK_OFFSET >
	struct MemoryChunkInfo final
	{
		/// @brief	Specified size of chunk.
		static constexpr const size_t SIZE		= CHUNK_SIZE;

		/// @brief	Specified alignment of chunk.
		static constexpr const size_t ALIGNMENT	= CHUNK_ALIGNMENT;

		/// @brief	Specified internal offset of chunk.
		static constexpr const size_t OFFSET	= CHUNK_OFFSET;
	};
}
}
}
}
