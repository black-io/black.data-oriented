#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	MemoryPool<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::~MemoryPool()
	{
		EXPECTS( m_storage.use_count() < 2 );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	template< typename Tobject, typename... TArguments >
	inline std::shared_ptr<Tobject> MemoryPool<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::ConstructObject( TArguments&&... arguments )
	{
		static_assert( alignof( Tobject ) <= MEMORY_ALIGNMENT, "The alignment of required object exceeds the alignment of memory in pool." );

		using Allocator = Internal::RawMemoryAllocator<Tobject, MemoryStorage>;
		return std::allocate_shared<Tobject>( Allocator{ AccessMemoryStorage() }, std::forward<TArguments>( arguments )... );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t MEMORY_ALIGNMENT >
	inline const std::shared_ptr<typename MemoryPool<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::MemoryStorage>&
	MemoryPool<RAW_MEMORY_SIZE, MAX_FREE_PAGES, MEMORY_ALIGNMENT>::AccessMemoryStorage()
	{
		CRET( m_storage != nullptr, m_storage );

		m_storage = std::make_shared<MemoryStorage>();
		return m_storage;
	}
}
}
}
