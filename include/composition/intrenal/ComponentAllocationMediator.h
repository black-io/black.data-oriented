#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Composition
{
namespace Internal
{
	// Component allocation policy.
	template< typename TComponentMap >
	class ComponentAllocationMediator
		: private AllocationMask<TComponentMap::COMPONENTS_COUNT>
		, private StructuredMemoryBlock<TComponentMap::SIZE, TComponentMap::ALIGNMENT, typename TComponentMap::OffsetCollection>
	{
	// Public interface.
	public:
		// Get the memory for slot.
		inline void* GetMemory( const size_t index )				{ return Buffer::GetMemory( index ); };

		// Get the memory for slot.
		inline const void* GetMemory( const size_t index ) const	{ return Buffer::GetMemory( index ); };


		// Mark the memory slot as allocated.
		inline void SetAllocated( const size_t index )				{ Bits::MarkAllocated( index ); };

		// Mark the memory slot as free.
		inline void SetFree( const size_t index )					{ Bits::MarkFree( index ); };

		// Whether the memory slot is allocated.
		inline const bool IsAllocated( const size_t index ) const	{ return Bits::IsAllocated( index ); };

	// Private inner types.
	private:
		// Parent structured memory buffer.
		using Buffer	= StructuredMemoryBlock<TComponentMap::SIZE, TComponentMap::ALIGNMENT, typename TComponentMap::OffsetCollection>;

		// Parent allocation mask.
		using Bits		= AllocationMask<TComponentMap::COMPONENTS_COUNT>;
	};
}
}
}
}
