#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	/**
		@brief	Object allocator with strong bound to memory page the object was allocated.
		Objects of this type may only be used as allocators for shred pointers of arbitrary type.

		@note		Produced shared object will be valid even after object pool will be destroyed.
		@warning	So that is a strong bond with memory page. Allocator of produced shared object will capture memory page of that object.

		@tparam	TStoredObject				Type of objects this allocator will produce.
		@tparam	TRawMemoryPageCollection	Type of memory page collection, which this allocator will use.
	*/
	template< typename TStoredObject, typename TRawMemoryPageCollection >
	class RawMemoryObjectAllocator
	{
	public:
		/// @see `allocator::value_type`
		using value_type = TStoredObject;

		/// @brief
		using RawMemoryPage		= typename TRawMemoryPageCollection::MemoryPage;

		/// @see `allocator::rebind`
		template< typename TRebindObject >
		struct rebind
		{
			/// @brief	Whether the rebind is falsely provoked by STL.
			static constexpr bool IS_SAME = std::is_same_v<TStoredObject, TRebindObject>;

			/// @see	`allocator::rebind::other`
			using other = std::conditional_t<IS_SAME, RawMemoryObjectAllocator, RawMemorySharedAllocator<TRebindObject, TRawMemoryPageCollection>>;
		};

		RawMemoryObjectAllocator() = delete;
		RawMemoryObjectAllocator( const std::shared_ptr<TRawMemoryPageCollection>& collection ) : m_collection( collection ) {};

		inline const std::shared_ptr<TRawMemoryPageCollection>& GetMemoryPageCollection() const	{ return m_collection; };

		inline const std::shared_ptr<RawMemoryPage>& GetMemoryPage( const size_t required_size ) const
		{
			CRET( m_memory_page != nullptr, m_memory_page );

			m_memory_page = m_collection->RetainMemoryPage( required_size );
			return m_memory_page;
		}

	private:
		std::shared_ptr<TRawMemoryPageCollection>	m_collection;
		mutable std::shared_ptr<RawMemoryPage>		m_memory_page;
	};
}
}
}
}
