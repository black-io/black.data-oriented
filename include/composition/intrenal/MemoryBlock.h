#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Trivial memory block.
	template< size_t BLOCK_SIZE, size_t BLOCK_ALIGNMENT >
	class alignas( BLOCK_ALIGNMENT ) MemoryBlock : private Black::NonTransferable
	{
		static_assert( BLOCK_SIZE > 0, "The size of memory block should be greater than 0." );

	// Public interface.
	public:
		// Get the memory with requested offset.
		inline void* GetMemory( const size_t offset )						{ return TestOffset( offset ), m_memory + offset; };

		// Get the memory with requested offset.
		inline const void* GetMemory( const size_t offset ) const			{ return TestOffset( offset ), m_memory + offset; };


		// Whether the iterator is valid for this block.
		inline const bool IsInside( const void* memory ) const noexcept		{ return ( memory >= GetHead() ) && ( memory <= GetTail() ); };

		// Get the alignment of block.
		constexpr const size_t GetAlignment() const noexcept				{ return BLOCK_ALIGNMENT; };

		// Get the size (in bytes) of block.
		constexpr const size_t GetSize() const noexcept						{ return BLOCK_SIZE; };

	// Private interface.
	private:
		// Get the head of block, the first element pointer.
		inline const void* GetHead() const noexcept							{ return m_memory; };

		// Get the tail of block, the last element pointer.
		inline const void* GetTail() const noexcept							{ return m_memory + BLOCK_SIZE - 1; };

		// Test the offset to satisfy the block size.
		inline void TestOffset( const size_t offset ) const					{ EXPECTS_DEBUG( offset < GetSize() ); };

	// Private state.
	private:
		uint8_t m_memory[ BLOCK_SIZE ];
	};
}
}
}
}
