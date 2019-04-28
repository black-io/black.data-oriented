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
	class AllocationMask : private Black::NonTransferable
	{
	// Construction and assignment.
	public:
		AllocationMask()	{ Black::ZeroMemory( m_bits ); };
		~AllocationMask()	= default;

	// Public interface.
	public:
		// Mark the given index as allocated.
		inline void MarkAllocated( const size_t index )				{ GetStorage( index ) = GetStorage( index ) | GetAllocationMask( index ); };

		// Mark the given index as free.
		inline void MarkFree( const size_t index )					{ GetStorage( index ) = GetStorage( index ) & ~GetAllocationMask( index ); };


		// Whether the bit is up for given allocation index.
		inline const bool IsAllocated( const size_t index ) const	{ return ( GetStorage( index ) & GetAllocationMask( index ) ) != 0; };

	// Private inner stuff.
	private:
		// Storage for bit-mask.
		using BitStorage = size_t;

		// Aligned length of bit-mask storage.
		static constexpr const size_t STORAGE_LENGTH	= Black::GetAlignedSize( MASK_LENGH, sizeof( BitStorage ) ) / sizeof( BitStorage );

		// Single first bit of storage.
		static constexpr const BitStorage FirstBit		= 1;

	// Private interface.
	private:
		// Get index of storage for given allocation index.
		static constexpr const size_t GetStorageIndex( const size_t allocation_index )			{ return allocation_index / sizeof( BitStorage ); };

		// Get index of bit inside of storage for give allocation index.
		static constexpr const size_t GetAllocationBit( const size_t allocation_index )			{ return allocation_index % sizeof( BitStorage ); };

		// Get the bit-mask for given allocation index.
		static constexpr const BitStorage GetAllocationMask( const size_t allocation_index )	{ return FirstBit << GetAllocationBit( allocation_index ); };

		// Test the allocation index to be valid.
		static inline void TestIndex( const size_t allocation_index )							{ EXPECTS_DEBUG( allocation_index < MASK_LENGH ); };


		// Get bit storage for given allocation index.
		inline BitStorage& GetStorage( const size_t allocation_index )							{ return m_bits[ GetStorageIndex( allocation_index ) ]; };

		// Get bit storage for given allocation index.
		inline const BitStorage GetStorage( const size_t allocation_index ) const				{ return m_bits[ GetStorageIndex( allocation_index ) ]; };

	// Private state.
	private:
		BitStorage	m_bits[ STORAGE_LENGTH ];	// Allocation bits storage.
	};
}
}
}
}
