#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::ChunkedMemoryCollection()
	{
		m_free_pages.reserve( MAX_FREE_PAGES );
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::~ChunkedMemoryCollection()
	{
		CRET( m_used_pages.empty() );
		BLACK_LOG_WARNING( LOG_CHANNEL, "{} memory pages of {}b size still remains active (Leaked probably).", m_used_pages.size(), RAW_MEMORY_SIZE );
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void* ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::AllocateChunk()
	{
		auto& memory_page = RetainMemoryPage();
		return memory_page->Allocate();
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::FreeChunk( Black::NotNull<void*> memory )
	{
		EXPECTS( !m_used_pages.empty() );
		auto last_page = --m_used_pages.end();

		if( (*last_page)->IsResidentChunk( memory ) )
		{
			(*last_page)->Free( memory.Get() );
			MaintainUsedMemory( last_page );
			return;
		}

		EXPECTS( m_used_pages.size() > 1 );
		auto found_host_page = std::find_if(
			m_used_pages.begin(),
			last_page,
			[&memory]( const SharedMemoryPage& page ) { return page->IsResidentChunk( memory ); }
		);

		EXPECTS( found_host_page != last_page );
		(*found_host_page)->Free( memory );
		MaintainUsedMemory( found_host_page );
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline const typename ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::SharedMemoryPage&
	ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::RetainMemoryPage()
	{
		CRET( m_used_pages.empty(), PullEmptyPage() );
		CRET( m_used_pages.back()->HasFreeChunks(), m_used_pages.back() );
		CRET( m_used_pages.size() == 1, PullEmptyPage() );

		// Last memory page already checked and can not be used.
		auto search_end		= --m_used_pages.end();
		// Older pages are placed at beginning, it might have more free memory.
		auto usable_page	= std::find_if( m_used_pages.begin(), search_end, []( const SharedMemoryPage& page ) { return page->HasFreeChunks(); } );
		return ( usable_page == search_end )? PullEmptyPage() : *usable_page;
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::ReleaseMemoryPage( const SharedMemoryPage& used_page )
	{
		CRET( !used_page->IsEmpty() );
		auto found_page_slot = std::find_if(
			m_used_pages.begin(), m_used_pages.end(),
			[&used_page]( const SharedMemoryPage& page ) { return page == used_page; }
		);

		EXPECTS( found_page_slot != m_used_pages.end() );
		MaintainUsedMemory( found_page_slot );
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::ReleaseMemoryPage(
		const std::shared_ptr<BasicMemoryPage>& used_page
	)
	{
		ReleaseMemoryPage( std::static_pointer_cast<MemoryPage>( used_page ) );
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline const typename ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::SharedMemoryPage&
	ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::PullEmptyPage()
	{
		CRET( m_free_pages.empty(), AllocateNewPage() );
		m_used_pages.emplace_back( std::move( m_free_pages.back() ) );
		m_free_pages.pop_back();
		return m_used_pages.back();
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline const typename ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::SharedMemoryPage&
	ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::AllocateNewPage()
	{
		m_used_pages.emplace_back( std::make_shared<MemoryPage>() );
		return m_used_pages.back();
	}

	template< size_t CHUNKS_COUNT, size_t CHUNK_SIZE, size_t CHUNK_ALIGNMENT, size_t MAX_FREE_PAGES >
	inline void ChunkedMemoryCollection<CHUNKS_COUNT, CHUNK_SIZE, CHUNK_ALIGNMENT, MAX_FREE_PAGES>::MaintainUsedMemory(
		typename std::deque<SharedMemoryPage>::iterator page_slot
	)
	{
		CRET( !(*page_slot)->IsEmpty() );

		SharedMemoryPage empty_page{ std::move( *page_slot ) };
		m_used_pages.erase( page_slot );

		CRET( m_free_pages.size() == MAX_FREE_PAGES );
		m_free_pages.emplace_back( std::move( empty_page ) );
	}
}
}
}
}
