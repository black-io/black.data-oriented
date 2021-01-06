#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	// Raw memory allocator.
	template< typename TProduct, typename TMemoryCollection >
	class RawMemoryAllocator final
	{
	// Public inner types and friends.
	public:
		// Type of allocator state.
		using AllocatorState = typename TMemoryCollection::AllocatorState;

		// C++ Allocator requirements: Type of allocated value.
		using value_type = TProduct;

	// Construction and initialization.
	public:
		RawMemoryAllocator()							= delete;
		RawMemoryAllocator( const RawMemoryAllocator& )	= default;
		RawMemoryAllocator( RawMemoryAllocator&& )		= default;

		RawMemoryAllocator( TMemoryCollection& collection );

		template< typename TOtherProduct, typename = std::enable_if_t<!std::is_same_v<TOtherProduct, TProduct>> >
		RawMemoryAllocator( const RawMemoryAllocator<TOtherProduct, TMemoryCollection>& other );

	// Public interface.
	public:
		// C++ Allocator requirements: Allocates storage suitable for an array object of type `T[n]`.
		inline value_type* allocate( const size_t sequence_length );

		// C++ Allocator requirements: Deallocates storage pointed to `p`.
		inline void deallocate( value_type* memory, const size_t sequence_length ) noexcept;


		// Get the bound memory collection.
		inline TMemoryCollection& GetMemoryCollection() const					{ return m_collection; };

		// Get the shared state of bound allocators.
		inline const std::shared_ptr<AllocatorState>& GetSharedState() const	{ return m_shared_state; };

	// Private interface.
	private:
		// Make guaranty that the stored memory page has enough memory for required allocation.
		void EnsureEnoughMemory() const;

	// Private state.
	private:
		TMemoryCollection&				m_collection;	// The host memory collection.
		std::shared_ptr<AllocatorState> m_shared_state; // Shared state for produced allocators.
	};
}
}
}
}
