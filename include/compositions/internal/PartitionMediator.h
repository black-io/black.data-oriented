#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Mediator type for partition block.
	template< typename TPartitonMap >
	class PartitionMediator
		: private AllocationMask<TPartitonMap::PARTS_COUNT>
		, private PartitionBlock<TPartitonMap::SIZE, TPartitonMap::ALIGNMENT, typename TPartitonMap::OffsetCollection>
	{
	// Public interface.
	public:
		// Get the offset for part with given index.
		inline const size_t GetOffset( const size_t index ) const	{ return Partition::GetOffset( index ); };


		// Get the memory for the part with given index.
		inline void* GetMemory( const size_t index )				{ return Partition::GetMemory( index ); };

		// Get the memory for the part with given index.
		inline const void* GetMemory( const size_t index ) const	{ return Partition::GetMemory( index ); };


		// Set allocation bit for part with given index.
		inline void SetAllocated( const size_t index )				{ Bits::MarkAllocated( index ); };

		// Unset allocation bit for part with given index.
		inline void SetFree( const size_t index )					{ Bits::MarkFree( index ); };

		// Whether the allocation bit is set for part with given index.
		inline const bool IsAllocated( const size_t index ) const	{ return Bits::IsAllocated( index ); };

	// Private inner types.
	private:
		// Parent structured memory buffer.
		using Partition	= PartitionBlock<TPartitonMap::SIZE, TPartitonMap::ALIGNMENT, typename TPartitonMap::OffsetCollection>;

		// Parent allocation mask.
		using Bits		= AllocationMask<TPartitonMap::PARTS_COUNT>;
	};
}
}
}
}
