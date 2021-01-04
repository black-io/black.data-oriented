#pragma once


namespace Black
{
inline namespace DataOriented
{
inline namespace Memory
{
namespace Internal
{
	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::RawMemoryPageCollection()
	{
		m_free_pages.reserve( MAX_FREE_PAGES );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::~RawMemoryPageCollection()
	{
		CRET( m_used_pages.empty() );
		BLACK_LOG_WARNING( LOG_CHANNEL, "{} raw memory pages of {}b size still remains active (Leaked probably).", m_used_pages.size(), RAW_MEMORY_SIZE );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	inline const typename RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::SharedMemoryPage&
	RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::RetainMemoryPage( const size_t required_size )
	{
		CRET( m_used_pages.empty(), PullEmptyPage() );
		CRET( m_used_pages.back()->HasEnoughMemory( required_size ), m_used_pages.back() );
		return PullEmptyPage();
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	inline void RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::ReleaseMemoryPage(
		const typename RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::SharedMemoryPage& memory_page
	)
	{
		CRETD( !Black::RemoveItem( m_used_pages, memory_page ), , LOG_CHANNEL, "Wrong release of memory page." );
		CRET( m_free_pages.size() >= MAX_FREE_PAGES );
		m_free_pages.emplace_back( memory_page );
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	inline typename RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::SharedMemoryPage&
	RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::PullEmptyPage()
	{
		CRET( m_free_pages.empty(), AllocateNewPage() );

		SharedMemoryPage empty_page = std::move( m_free_pages.back() );

		m_free_pages.pop_back();
		m_used_pages.emplace_back( empty_page );
		return empty_page;
	}

	template< size_t RAW_MEMORY_SIZE, size_t MAX_FREE_PAGES, size_t ALIGNMENT >
	inline typename RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::SharedMemoryPage&
	RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::AllocateNewPage()
	{
		SharedMemoryPage empty_page = std::make_shared<typename RawMemoryPageCollection<RAW_MEMORY_SIZE, MAX_FREE_PAGES, ALIGNMENT>::MemoryPage>();

		m_used_pages.emplace_back( empty_page );
		return empty_page;
	}
}
}
}
}
