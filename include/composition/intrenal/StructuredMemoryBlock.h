#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Block of structured memory.
	template< size_t BLOCK_SIZE, size_t BLOCK_ALIGNMENT, size_t... OFFSETS >
	class StructuredMemoryBlock : private MemoryBlock<BLOCK_SIZE, BLOCK_ALIGNMENT>
	{
	// Public interface.
	public:
		// Get the memory block for given offset index.
		inline void* GetMemory( const size_t offset_index )				{ return Parent::GetMemory( GetOffset( offset_index ) ); };

		// Get the memory block for given offset index.
		inline const void* GetMemory( const size_t offset_index ) const	{ return Parent::GetMemory( GetOffset( offset_index ) ); };


		// Get the number of valid offsets.
		constexpr const size_t GetOffsetsCount() const					{ return sizeof...( OFFSETS ); };

	// Private inner types.
	private:
		// Type of parent memory block.
		using Parent = MemoryBlock<BLOCK_SIZE, BLOCK_ALIGNMENT>;

	// Private interface.
	private:
		// Get the offset by index.
		static inline const size_t GetOffset( const size_t offset_index )	{ return TestOffsetIndex( offset_index ), INDEXED_OFFSETS[ offset_index ]; };

		// Test the offset index to fit the range of allowed offsets.
		static inline void TestOffsetIndex( const size_t offset_index )		{ EXPECTS_DEBUG( offset_index < sizeof...( OFFSETS ) ); };

	// Private fields.
	private:
		static constexpr const size_t INDEXED_OFFSETS[] = { OFFSETS... };
	};
}
}
}
}