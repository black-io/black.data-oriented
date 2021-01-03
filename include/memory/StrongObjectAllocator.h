#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	/**
		@brief	Object allocator with strong bond to memory page director.
		Object of this type may be used as allocator for `std::allocate_shared`.

		@note		Produced shared object will be valid even after object pool will be destroyed.
		@warning	So that is a strong bond with memory page director. Allocator of produced shared object will capture memory page of that object.

		@tparam	TStoredObject		Type of object the `std::allocate_shared` must produce.
		@tparam	TMemoryPageDirector	Type of memory page director, that will be used to store shared pointer of produced object.
	*/
	template< typename TStoredObject, typename TMemoryPageDirector >
	class StrongObjectAllocator
	{
	public:
		/// @see `allocator::value_type`
		using value_type = TStoredObject;

		/// @see `allocator::rebind`
		template< typename TRebindObject >
		struct rebind
		{
			/// @brief	Whether the rebind is falsely provoked by STL.
			static constexpr bool IS_SAME = std::is_same_v<TStoredObject, TRebindObject>;

			/// @see	`allocator::rebind::other`
			using other = std::conditional_t<IS_SAME, StrongObjectAllocator, Internal::StrongSharedAllocator<TRebindObject, TMemoryPageDirector>>;
		};

		StrongObjectAllocator() = delete;
		StrongObjectAllocator( TMemoryPageDirector& director )
			: m_director( director )
		{

		}

		/// @brief	Get the actual collection of shared objects (function is used only by shared allocator).
		template< size_t ITEM_SIZE >
		inline const std::shared_ptr<Internal::BasicMemoryPageCollection>& GetCollection() const
		{
			CRET( !m_memory_collection, AcquireCollection<ITEM_SIZE>() );
			return m_memory_collection;
		}

		/// @brief	Get the actual memory page (function is used only by shared allocator).
		template< size_t ITEM_SIZE >
		inline const std::shared_ptr<Internal::BasicMemoryPage>& GetPage() const
		{
			CRET( !m_memory_page, AcquirePage<ITEM_SIZE>() );
			return m_memory_page;
		}

	private:
		/// @brief	Construct the actual collection of shared objects.
		template< size_t ITEM_SIZE >
		inline const std::shared_ptr<Internal::BasicMemoryPageCollection>& AcquireCollection() const
		{
			m_memory_collection = std::static_pointer_cast<Internal::BasicMemoryPageCollection>( m_director.template GetCollection<ITEM_SIZE>() );
			return m_memory_collection;
		}

		/// @brief	Construct the actual memory page.
		template< size_t ITEM_SIZE >
		inline const std::shared_ptr<Internal::BasicMemoryPage>& AcquirePage() const
		{
			using PageCollection = typename TMemoryPageDirector:: template PageCollection<ITEM_SIZE>;

			auto collection = std::static_pointer_cast<PageCollection>( GetCollection<ITEM_SIZE>() );
			m_memory_page = std::static_pointer_cast<Internal::BasicMemoryPage>( collection->RetainMemoryPage() );
			return m_memory_page;
		}

	private:
		TMemoryPageDirector&	m_director;	// Memory page director, used only at first time get actual memory page collection.

		mutable std::shared_ptr<Internal::BasicMemoryPageCollection>	m_memory_collection;	// Actual memory page collection, used by shared allocator.
		mutable std::shared_ptr<Internal::BasicMemoryPage>				m_memory_page;			// Actual memory page, used by shared allocator.
	};
}
}
}
