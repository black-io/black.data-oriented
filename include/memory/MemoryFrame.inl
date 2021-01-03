#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::MemoryFrame()
	{
		m_free_pages.reserve( MAX_FREE_PAGES );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::~MemoryFrame()
	{
		EXPECTS_DEBUG( m_used_pages.empty() );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	void MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::Clear()
	{
		m_used_pages.clear();
		m_free_pages.clear();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	void MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::Reset()
	{
		for( auto& used_page : m_used_pages )
		{
			used_page->Refine();
			CCON( m_free_pages.size() >= MAX_FREE_PAGES );
			m_free_pages.emplace_back( used_page );
		}

		m_used_pages.clear();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	void MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::Refine()
	{
		m_free_pages.erase( m_free_pages.begin(), m_free_pages.end() );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	template< typename TObjectType >
	inline TObjectType* MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::AllocateObject()
	{
		return AllocateObject<TObjectType>( 1 );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	template< typename TObjectType >
	inline TObjectType* MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::AllocateObject( size_t objects_count )
	{
		constexpr size_t object_size	= sizeof( TObjectType );
		//constexpr size_t object_align	= std::max( alignof( TObjectType ), ALIGNMENT ); // TODO: Implement alignment extension.
		auto& memory_page				= GetFreeMemoryPage( object_size * objects_count );
		return reinterpret_cast<TObjectType*>( memory_page.RetainMemory( object_size * objects_count ) );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	template< typename TObjectType, typename... TArguments >
	inline TObjectType* MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::ConstructObject( TArguments... arguments )
	{
		auto object_memory = AllocateObject<TObjectType>( 1 );
		return ::new( object_memory ) TObjectType( std::forward<TArguments>( arguments )... );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	inline typename MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::MemoryPage&
	MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::GetFreeMemoryPage( const size_t size )
	{
		CRET( m_used_pages.empty(), AllocateFreePage() );
		CRET( !m_used_pages.back()->HasEnoughMemory( size ), AllocateFreePage() );
		return *m_used_pages.back();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT>
	inline typename MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::MemoryPage&
	MemoryFrame<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::AllocateFreePage()
	{
		if( m_free_pages.empty() )
		{
			m_used_pages.push_back( std::make_shared<MemoryPage>() );
		}
		else
		{
			m_used_pages.emplace_back( m_free_pages.back() );
			m_free_pages.pop_back();
		}

		return *m_used_pages.back();
	}
}
}
}
