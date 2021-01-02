#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Global
{
inline namespace Types
{
	/**
		@brief	Trivial memory block.
		The type implements self-contained memory block, that could be allocated for any kind of usage.
		It's a basic type for any memory pool or some kind of paged allocation strategy.
	*/
	template< size_t RAW_MEMORY_SIZE, size_t MEMORY_ALIGNMENT >
	class alignas( MEMORY_ALIGNMENT ) RawMemoryBlock
	{
	// Contracts.
	public:
		static_assert( RAW_MEMORY_SIZE > 0, "Block size should be grater than 0." );
		static_assert( MEMORY_ALIGNMENT > 0, "Block alignment should be grater than 0." );
		static_assert( Black::IsSingleBitValue( MEMORY_ALIGNMENT ), "Block alignment should be to valid PoT value." );

	// Construction and assignment.
	public:
		RawMemoryBlock()												{ BLACK_NON_RELEASE_CODE( Black::ZeroMemory( m_memory ) ); };

	// Public interface.
	public:
		// Get the size (in bytes) of block.
		constexpr size_t GetSize() const noexcept						{ return RAW_MEMORY_SIZE; };

		// Get the block alignment.
		constexpr size_t GetAlignment() const							{ return MEMORY_ALIGNMENT; };


		// Get the begin of block.
		inline const void* const GetBeginAddress() const				{ return m_memory; };

		// Get the end of block.
		inline const void* const GetEndAddress() const					{ return m_memory + GetSize(); };

		// Get the first available block address.
		inline const void* const GetFirstAddress() const				{ return m_memory; };

		// Get the last available block address.
		inline const void* const GetLastAddress() const					{ return m_memory + GetSize() - 1; };


		// Get the memory pointer with given offset.
		inline void* const GetMemory( const size_t offset )				{ TestOffset( offset ); return m_memory + offset; };

		// Get the memory pointer with given offset.
		inline const void* const GetMemory( const size_t offset ) const	{ TestOffset( offset ); return m_memory + offset; };


		// whether the given memory pointer lies inside of block.
		inline const bool IsInside( const void* const memory ) const	{ return ( memory >= GetBeginAddress() ) && ( memory < GetEndAddress() ); };

	// Private interface.
	private:
		// Test the offset to satisfy the block size.
		inline void TestOffset( const size_t offset ) const				{ EXPECTS_DEBUG( offset < GetSize() ); };

	// Private constants.
	private:
		// Length of aligned block.
		static constexpr size_t ALIGNED_MEMORY_SIZE = Black::GetAlignedSize( RAW_MEMORY_SIZE, MEMORY_ALIGNMENT );

	// Private state.
	private:
		std::byte m_memory[ ALIGNED_MEMORY_SIZE ]; // The memory stored by block.
	};
}
}
}
}
