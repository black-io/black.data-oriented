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
		@brief	Shared object allocator with strong bond to memory page director.
		Object of this type will be deduced by `std::allocate_shared` from `StrongObjectAllocator`.

		@tparam	TStoredObject		Type of shared object.
		@tparam	TMemoryPageDirector	Type of memory page director, that will be used to store shared pointer of produced object.
	*/
	template< typename TStoredObject, typename TMemoryPageDirector >
	class StrongSharedAllocator
	{
	public:
		/// @see `allocator::value_type`
		using value_type = TStoredObject;

		/// @brief	Type of parent allocator.
		template< typename TRebindObject >
		using OriginAllocator = Black::StrongObjectAllocator<TRebindObject, TMemoryPageDirector>;

		/// @see `allocator::rebind`
		template< typename TRebindObject >
		struct rebind
		{
			/// @see `allocator::rebind::other`
			using other = OriginAllocator<TRebindObject>;
		};

		StrongSharedAllocator() = delete;

		template< typename TRebindObject >
		StrongSharedAllocator( const OriginAllocator<TRebindObject>& origin_allocator )
			: m_memory_collection( origin_allocator.template GetCollection<sizeof( TStoredObject )>() )
			, m_memory_page( origin_allocator.template GetPage<sizeof( TStoredObject )>() )
		{

		}

		/// @see `allocator::allocate`
		inline value_type* allocate( size_t objects_count )
		{
			using PageCollection = typename TMemoryPageDirector::template PageCollection<sizeof( TStoredObject )>;

			EXPECTS( objects_count == 1 );
			EXPECTS( !m_memory_collection.expired() );
			CRET( m_memory_page != nullptr, (value_type*)m_memory_page->RetainMemory() );

			auto collection	= std::static_pointer_cast<PageCollection>( m_memory_collection.lock() );
			m_memory_page	= collection->RetainMemoryPage();
			return reinterpret_cast<value_type*>( m_memory_page->RetainMemory() );
		}

		/// @see `allocator::deallocate`
		inline void deallocate( value_type* object_ptr, size_t objects_count )
		{
			using PageCollection = typename TMemoryPageDirector::template PageCollection<sizeof( TStoredObject )>;

			EXPECTS( objects_count == 1 );
			EXPECTS( m_memory_page != nullptr );

			m_memory_page->ReleaseMemory( object_ptr );
			CRET( m_memory_collection.expired() );

			// Return memory page to collection only if collection still alive.
			auto collection	= std::static_pointer_cast<PageCollection>( m_memory_collection.lock() );
			collection->ReleaseMemoryPage( m_memory_page );
		}

	private:
		std::weak_ptr<BasicMemoryPageCollection>	m_memory_collection;	// Weak bond to memory page collection.
		std::shared_ptr<BasicMemoryPage>			m_memory_page;			// Strong bond to memory page, where the pointer to produced object was grabbed.
	};
}
}
}
}
