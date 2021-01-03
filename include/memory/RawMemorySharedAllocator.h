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
		@brief	Shared object allocator with strong bond to memory page the object was allocated.
		Object of this type will be deduced by `std::allocate_shared` from `RawMemoryObjectAllocator`.

		@tparam	TStoredObject				Type of objects this allocator will produce.
		@tparam	TRawMemoryPageCollection	Type of memory page collection, which this allocator will use.
	*/
	template< typename TStoredObject, typename TRawMemoryPageCollection >
	class RawMemorySharedAllocator
	{
	public:
		/// @see `allocator::value_type`
		using value_type = TStoredObject;

		/// @brief	Type of parent allocator.
		template< typename TRebindObject >
		using OriginAllocator	= RawMemoryObjectAllocator<TRebindObject, TRawMemoryPageCollection>;

		/// @brief
		using RawMemoryPage		= typename TRawMemoryPageCollection::MemoryPage;

		/// @see `allocator::rebind`
		template< typename TRebindObject >
		struct rebind
		{
			/// @see `allocator::rebind::other`
			using other = OriginAllocator<TRebindObject>;
		};

		RawMemorySharedAllocator() = delete;

		template< typename TRebindObject >
		RawMemorySharedAllocator( const OriginAllocator<TRebindObject>& origin_allocator )
			: m_collection( origin_allocator.GetMemoryPageCollection() )
			, m_memory_page( origin_allocator.GetMemoryPage( sizeof( value_type ) ) )
		{

		}

		/// @see `allocator::allocate`
		inline value_type* allocate( size_t objects_count )
		{
			constexpr size_t object_size = sizeof( value_type );

			EXPECTS( objects_count == 1 );
			EXPECTS( !m_collection.expired() );
			EXPECTS( m_memory_page != nullptr );

			value_type* result_ptr = reinterpret_cast<value_type*>( m_memory_page->RetainMemory( object_size ) );
			ENSURES( result_ptr != nullptr );
			return result_ptr;
		}

		/// @see `allocator::deallocate`
		inline void deallocate( value_type* object_ptr, size_t objects_count )
		{
			EXPECTS( objects_count == 1 );
			EXPECTS( m_memory_page != nullptr );

			m_memory_page->ReleaseMemory( object_ptr );
			CRET( m_collection.expired() );
			CRET( !m_memory_page->IsEmpty() );

			auto collection = m_collection.lock();
			collection->ReleaseMemoryPage( m_memory_page );
		}

	private:
		std::weak_ptr<TRawMemoryPageCollection>	m_collection;
		std::shared_ptr<RawMemoryPage>			m_memory_page;
	};
}
}
}
}
