#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Component allocation mask.
	template< size_t MASK_LENGH >
	class AllocationMask
	{
	// Construction and assignment.
	public:
		AllocationMask()	{ Black::ZeroMemory( m_bits ); };
		~AllocationMask()	= default;

	// Public interface.
	public:
		// Mark the given index as allocated.
		inline void MarkAllocated( const size_t index )				{ GetBuffer( index ) = GetBuffer( index ) | GetBitMask( index ); };

		// Mark the given index as free.
		inline void MarkFree( const size_t index )					{ GetBuffer( index ) = GetBuffer( index ) & ~GetBitMask( index ); };


		// Whether the bit is up for given allocation index.
		inline const bool IsAllocated( const size_t index ) const	{ return ( GetBuffer( index ) & GetBitMask( index ) ) != 0; };

	// Private inner stuff.
	private:
		// Storage for bit-mask.
		using BitBuffer = size_t;

		// Storage element alignment in bits.
		static constexpr const size_t BUFFER_BIT_ALIGNMENT	= alignof( BitBuffer ) * 8;

		// Storage element size on bits.
		static constexpr const size_t BITS_PER_BUFFER		= sizeof( BitBuffer ) * 8;

		// Aligned length of bit-mask storage.
		static constexpr const size_t STORAGE_LENGTH		= Black::GetAlignedSize( MASK_LENGH, BUFFER_BIT_ALIGNMENT ) / BITS_PER_BUFFER;

		// Single first bit of storage.
		static constexpr const BitBuffer FirstBit			= 1;

	// Private interface.
	private:
		// Get index of storage for given allocation index.
		static constexpr const size_t GetBufferIndex( const size_t bit_index )	{ return bit_index / BITS_PER_BUFFER; };

		// Get index of bit inside of storage for give allocation index.
		static constexpr const size_t GetBitIndex( const size_t bit_index )		{ return bit_index % BITS_PER_BUFFER; };

		// Get the bit-mask for given allocation index.
		static constexpr const BitBuffer GetBitMask( const size_t bit_index )	{ return FirstBit << GetBitIndex( bit_index ); };

		// Test the allocation index to be valid.
		static inline void TestIndex( const size_t bit_index )					{ EXPECTS_DEBUG( bit_index < MASK_LENGH ); };


		// Get bit storage for given allocation index.
		inline BitBuffer& GetBuffer( const size_t bit_index )					{ return m_bits[ GetBufferIndex( bit_index ) ]; };

		// Get bit storage for given allocation index.
		inline const BitBuffer GetBuffer( const size_t bit_index ) const		{ return m_bits[ GetBufferIndex( bit_index ) ]; };

	// Private state.
	private:
		BitBuffer	m_bits[ STORAGE_LENGTH ];	// Allocation bits storage.
	};
}
}
}
}
